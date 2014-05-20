#include "InterCode.h"
#define TAB_SPACE 8
#define E_PREFIX    "_event_"

long LabelClass::labelCount = 0;
std::map <string, InterCode*> LabelClass::label_interCode_map;  

void InterCode::print(ostream &os) {
    ExprNode** op = (ExprNode**)opnds_;

    switch (optype_) {
        case EXPR:  {
                        switch (subCode_) {
                            default: 
                                if (op[0] && op[1] && op[2]) {
                                    prtSpace(os, TAB_SPACE);
                                    os << op[0]->getRefName() << " = " << op[1]->getRefName();
                                    os << " " << OpNode::opInfo[(int )subCode_].name_ << " ";
                                    os << op[2]->getRefName();
                                }
                                break;

                            case OpNode::OpCode::NOT:
                            case OpNode::OpCode::BITNOT:
                            case OpNode::OpCode::UMINUS:
                                if (op[0] && op[1]) {
                                    prtSpace(os, TAB_SPACE);
                                    os << op[0]->getRefName() << " = ";
                                    os << OpNode::opInfo[(int )subCode_].name_;
                                    os << op[1]->getRefName();
                                }
                                break;

                            case OpNode::OpCode::ASSIGN:  
                                prtSpace(os, TAB_SPACE);
                                os << op[0]->getRefName() << " = " << op[1]->getRefName();
                                break;
                        }
                        break;
                    }                

        case GOTO:  {
                        InterCode* goto_lab = (InterCode*) op[0];
                        prtSpace(os, TAB_SPACE);
                        os << "goto "; 
                        goto_lab->print(os);
                        break; 
                    }

        case LABEL: {
                        string *n = (string *)op[1];
                        if (n && n->length()) { 
                            os << *n << ":";
                        } else {
                            os << "L" << (long )op[0] << ":";
                        } 
                    }
                    break;

        case IFREL: {
                        assert(op[0] && "Expected Operand 0");

                        prtSpace(os, TAB_SPACE);
                        os << "if ";
                        ExprNode* cond = (ExprNode*) op[0];
                        InterCode* true_lab = cond->OnTrue();
                        InterCode* false_lab = cond->OnFalse();

                        if (op[2] && op[1]) {
                            os << op[1]->getRefName() << " ";
                            os << OpNode::opInfo[(int )subCode_].name_;
                            os << " " << op[2]->getRefName();
                        } else if (op[1]) {
                            os << OpNode::opInfo[(int )subCode_].name_ << op[1]->getRefName();
                        } else {
                            os << op[0]->getRefName();
                        }

                        os << " goto ";
                        true_lab->print(os);

                        if (false_lab) {
                            os << endl;
                            prtSpace(os, TAB_SPACE);
                            os << "goto ";
                            false_lab->print(os);
                        }
                    }
                    break; 

        case ENTER: {
                        prtSpace(os, TAB_SPACE);
                        os << "enter " << ((FunctionEntry*)op[0])->name();
                    }
                    break;

        case LEAVE: {
                        prtSpace(os, TAB_SPACE);
                        os << "leave " << ((FunctionEntry*)op[0])->name();
                    }
                    break;

        case CALL:  {
                        prtSpace(os, TAB_SPACE);
                        if (op[1]) {
                            os << op[1]->getRefName() << " = ";
                        }
                        os << "call " << ((InvocationNode*)op[0])->symTabEntry()->name();
                    }
                    break;

        case RETURN:{
                        prtSpace(os, TAB_SPACE);
                        os << "return";
                        if (op[0]) {
                            os << " " << op[0]->getRefName();
                        }
                    }
                    break;

        case APARAM:{
                        prtSpace(os, TAB_SPACE);
                        os << "aparam " << op[0]->getRefName();
                    }
                    break;

        case FPARAM:{
                        prtSpace(os, TAB_SPACE);
                        os << "fparam " << op[0]->getRefName();
                    }
                    break;

        case PRINT: {
                        prtSpace(os, TAB_SPACE);
                        os << "print " << op[0]->getRefName();
                    }
                    break;

        default:
                    assert(0 && "Unsupported 3AddrCode");
    }
}

string InterCode::getLabel() { 
    if (optype_ != LABEL) return "";
    int id = (long)opnds_[0];
    string *name = (string *) opnds_[1];
    if (name && name->length()) {
        return *name;
    } else {
        ostringstream os;
        os << "L" << id;
        return os.str();
    }
}

bool InterCode::xchgSubcode() { 

    switch (subCode_) {
        case OpNode::OpCode::EQ:
            subCode_ = OpNode::OpCode::NE;
            break;
        case OpNode::OpCode::NE:
            subCode_ = OpNode::OpCode::EQ;
            break;
        case OpNode::OpCode::GT:
            subCode_ = OpNode::OpCode::LE;
            break;
        case OpNode::OpCode::GE:
            subCode_ = OpNode::OpCode::LT;
            break;
        case OpNode::OpCode::LE:
            subCode_ = OpNode::OpCode::GT;
            break;
        case OpNode::OpCode::LT:
            subCode_ = OpNode::OpCode::GE;
            break;
        default:
            return false;
            //subCode_ = OpNode::OpCode::INVALID;
            //cout<<"Cannot negate the subcode_";
            break;
    }
    return true;
}

void InterCodesClass::printMap() {
    map <string, vector<InterCode*>*>::iterator it = labelUsageMap.begin();
    for (; it != labelUsageMap.end(); it++) {
        cout << "\n##### Label Name : " << (*it).first << "\t References : " << (*it).second->size() << "#####" << endl;
    }
}

void InterCodesClass::insertMap(string name, InterCode* ic) {

    vector <InterCode*>* tempVector;  

    if (labelUsageMap.find(name) == labelUsageMap.end()) {
        tempVector = new vector<InterCode*>;
        if (ic->getOPNType() != InterCode::LABEL)
            tempVector->push_back(ic);
    }
    else
    {
        tempVector = labelUsageMap.find(name)->second;
        if (ic->getOPNType() != InterCode::LABEL)
            tempVector->push_back(ic);
    }
    labelUsageMap.insert (pair<string, vector <InterCode*>*> (name, tempVector));
}

void InterCodesClass::createLabelDUChain()
{
    int i;
    vector<InterCode*>* dupICodeVector  = getICodeVector();
    ExprNode* cond; 
    InterCode* true_lab, *false_lab, *goto_lab, *start_lab;

    for ( i = 0; i < (int )dupICodeVector->size(); i++) {

        if (dupICodeVector->at(i)->getOPNType() == InterCode::IFREL)  {

            ExprNode** operands = (ExprNode**)dupICodeVector->at(i)->get3Operands();

            cond = operands[0];  

            true_lab = cond->OnTrue();
            insertMap(true_lab->getLabel(), dupICodeVector->at(i));

            false_lab = cond->OnFalse();
            if(false_lab)
                insertMap(true_lab->getLabel(), dupICodeVector->at(i));
        }
        else if (dupICodeVector->at(i)->getOPNType() == InterCode::GOTO)  {

            ExprNode** operands = (ExprNode**)dupICodeVector->at(i)->get3Operands();

            // cout << "\nLabel Name = " << goto_lab->getLabel();
            goto_lab = (InterCode *)operands[0];
            insertMap(goto_lab->getLabel(), dupICodeVector->at(i));

        }
        else if (dupICodeVector->at(i)->getOPNType() == InterCode::LABEL)  {

            // cout << "\nLabel Name = " << goto_lab->getLabel();
            start_lab = dupICodeVector->at(i);
            insertMap(start_lab->getLabel(), dupICodeVector->at(i));

        }
        else if (dupICodeVector->at(i)->getOPNType() == InterCode::CALL)  {

            ExprNode** operands = (ExprNode**)dupICodeVector->at(i)->get3Operands();

            //goto_lab = (ExprNode *)operands[0];
            InvocationNode *temp = (InvocationNode *)operands[0]; 
            //            cout << "\nLabel Name = " << (InvocationNode *)goto_lab->symTabEntry()->name();
            insertMap(temp->symTabEntry()->name(), dupICodeVector->at(i));

        }
    }
}



/****************************************************************************************************************************  
 *  Input :
 *
 *  if a then goto L1 
 *      else goto L2
 *  L1: 
 *      .... L1 body ........
 *  L2:
 *      .... L2 body ........
 *
 *  Output :
 *
 *  if !a then goto L2
 *      else goto L1
 *  L1:
 *      .... L1.body ........
 *  L2:
 *      .... L2 body ........
 *
 *****************************************************************************************************************************/

void InterCodesClass::ifThenElseOpt(int *isOptimized) {

    int i;
    vector<InterCode*>* dupICodeVector  = getICodeVector();
    ExprNode* cond; 
    InterCode* true_lab, *false_lab, *start_lab;
    bool flag                           = false;

    for ( i = 0; i < (int )dupICodeVector->size(); i++) {

        if (dupICodeVector->at(i)->getOPNType() == InterCode::IFREL)  {

            ExprNode** operands = (ExprNode**)dupICodeVector->at(i)->get3Operands();

            cond = operands[0];  

            true_lab = cond->OnTrue();
            false_lab = cond->OnFalse();
            if(false_lab) {
                if (dupICodeVector->at(i+1)->getOPNType() == InterCode::LABEL)  {
                    start_lab = dupICodeVector->at(i+1);
                    if (true_lab->getLabel() == start_lab->getLabel())  {
                        if (dupICodeVector->at(i)->xchgSubcode())   {
                            cond->OnTrue(false_lab);
                            cond->OnFalse(true_lab);
                        }
                    }    
                    else if (false_lab->getLabel() == start_lab->getLabel())  {
                        if (dupICodeVector->at(i)->xchgSubcode())   {
                            cond->OnTrue(false_lab);
                            cond->OnFalse(true_lab);
                        }
                    }
                }
            }
        }
    }       
    if(flag)
        *isOptimized = 1;

}

void InterCodesClass::removeContLabelGoto(int *isOptimized) {

    int i;
    vector<InterCode*>* dupICodeVector = getICodeVector();
    ExprNode* cond; 
    InterCode* goto_lab, *false_lab, *start_lab;
    bool flag                           = false;

    for ( i = 0; i < (int )dupICodeVector->size(); i++) {

        if (dupICodeVector->at(i)->getOPNType() == InterCode::GOTO)  {

            ExprNode** operands = (ExprNode**)dupICodeVector->at(i)->get3Operands();

            goto_lab = (InterCode *)operands[0];

            //            cout << "\nLabel Name = " << goto_lab->getLabel();
            if (dupICodeVector->at(i+1)->getOPNType() == InterCode::LABEL)  {
                start_lab = dupICodeVector->at(i+1);
                if (goto_lab->getLabel() == start_lab->getLabel()) {
                    dupICodeVector->erase(dupICodeVector->begin() + i);
                    flag = true;
                }
            }    
        }
        if (dupICodeVector->at(i)->getOPNType() == InterCode::IFREL)  {
            ExprNode** operands = (ExprNode**)dupICodeVector->at(i)->get3Operands();
            cond = operands[0];  
            false_lab = cond->OnFalse();
            if (false_lab) {
                //cout << "\nLabel Name = " << false_lab->getLabel();
                if (dupICodeVector->at(i+1)->getOPNType() == InterCode::LABEL)  {
                    start_lab = dupICodeVector->at(i+1);
                    if (false_lab->getLabel() == start_lab->getLabel()) {
                        cond->OnFalse(NULL);
                        flag = true;
                    }
                }
            }
        }
    }    
    if(flag)
        *isOptimized = 1;

}

void BasicBlock::constantFolding (int *isOptimized) {
    int result = 0, val_1 = 0, val_2 = 0, i;
    double resultf = 0.0, valf_1 = 0.0, valf_2 = 0.0;

    vector<InterCode*>* dupICodeVector  = getICodeVector();
    vector<InterCode*>* tempICodeVector = new vector<InterCode*> ();
    bool flag                           = false;
    bool isfloat;

    //    cout << "\t Inside Folding" << dupICodeVector->size();
    for ( i = 0; i < (int )dupICodeVector->size(); i++) {

        //cout << "\t " << i;
        //tempICodeVector->push_back(dupICodeVector->at(i)); 
        ExprNode** operands = (ExprNode**)dupICodeVector->at(i)->get3Operands();
        isfloat = false;

        if (operands[0] && operands[1] && operands[2]) {
            ExprNode* new1 = operands[1]; 
            ExprNode* new2 = operands[2]; 
            //cout << "\t if part";

            // Check if op[1] and op[2] are valuenodes and intercode type is expression

            if ((new1->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE) && (new2->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE)) {

                if ((new1->value()->type()->tag() == Type::TypeTag::INT || new1->value()->type()->tag() == Type::TypeTag::UINT) &&
                        (new2->value()->type()->tag() == Type::TypeTag::INT || new2->value()->type()->tag() == Type::TypeTag::UINT)) {

                    isfloat = false;
                    val_1   = stoi(new1->getRefName());
                    val_2   = stoi(new2->getRefName());

                } else if (new1->value()->type()->tag() == Type::TypeTag::DOUBLE || new2->value()->type()->tag() == Type::TypeTag::DOUBLE) {
                    isfloat = true;
                    valf_1  = stof(new1->getRefName());
                    valf_2  = stof(new2->getRefName());
                }

                if (dupICodeVector->at(i)->getOPNType() == InterCode::OPNTYPE::EXPR) {

                    //cout << "DEBUG: " << OpNode::opInfo[(int )dupICodeVector->at(i)->getsubCode()].name_;
                    switch(dupICodeVector->at(i)->getsubCode()) {
                        case OpNode::OpCode::PLUS :
                            if(isfloat) resultf = valf_1 + valf_2;
                            else result = val_1 + val_2;
                            flag = true;
                            break; 
                        case OpNode::OpCode::MINUS :
                            if(isfloat) resultf = valf_1 - valf_2;
                            else result = val_1 - val_2;
                            flag = true;
                            break; 
                        case OpNode::OpCode::MULT :
                            if(isfloat) resultf = valf_1 * valf_2;
                            else result = val_1 * val_2;
                            flag = true;
                            break; 
                        case OpNode::OpCode::DIV :
                            if(isfloat) resultf = valf_1 / valf_2;
                            else result = val_1 / val_2;
                            flag = true;
                            break; 
                        case OpNode::OpCode::MOD :
                            if(!isfloat) { result = val_1 % val_2; flag = true; }
                            break; 
                        case OpNode::OpCode::BITOR :
                            if(!isfloat) { result = val_1 | val_2; flag = true; }
                            break;
                        case OpNode::OpCode::BITAND :
                            if(!isfloat) { result = val_1 & val_2; flag = true; }
                            break;
                        case OpNode::OpCode::BITXOR :
                            if(!isfloat) { result = val_1 ^ val_2; flag = true; }
                            break;
                        case OpNode::OpCode::SHL :
                            if(!isfloat) { result = val_1 << val_2; flag = true; }
                            break;
                        case OpNode::OpCode::SHR :
                            if(!isfloat) { result = val_1 >> val_2; flag = true; }
                            break;
                        default : 
                            cout << "\nUnhandled OpCode \n";
                            break;
                    }
                    //cout << result << "\n";
                    ValueNode *temp; 
                    
                    if (isfloat) {
                        temp = new ValueNode(new Value(resultf));
                    } else {
                        temp =  new ValueNode(new Value(result, Type::INT));
                    }

                    tempICodeVector->push_back(new InterCode(InterCode::OPNTYPE::EXPR, 
                                OpNode::OpCode::ASSIGN, operands[0], temp));


                } else if (dupICodeVector->at(i)->getOPNType() == InterCode::OPNTYPE::IFREL) {

                    //bool cond = false;
                    //switch(dupICodeVector->at(i)->getsubCode()) {
                    //    case OpNode::OpCode::EQ: 
                    //        if(!isfloat) cond = (val_1 == val_2);
                    //        else cond = (valf_1 == valf_2);
                    //        break;
                    //    case OpNode::OpCode::NE:
                    //        if(!isfloat) cond = (val_1 != val_2);
                    //        else cond = (valf_1 != valf_2);
                    //        break;
                    //    case OpNode::OpCode::GE:
                    //        if(!isfloat) cond = (val_1 >= val_2);
                    //        else cond = (valf_1 >= valf_2);
                    //        break;
                    //    case OpNode::OpCode::LE:
                    //        if(!isfloat) cond = (val_1 <= val_2);
                    //        else cond = (valf_1 <= valf_2);
                    //        break;
                    //    case OpNode::OpCode::GT:
                    //        if(!isfloat) cond = (val_1 > val_2);
                    //        else cond = (valf_1 > valf_2);
                    //        break;
                    //    case OpNode::OpCode::LT:
                    //        if(!isfloat) cond = (val_1 < val_2);
                    //        else cond = (valf_1 < valf_2);
                    //        break;
                    //    default : 
                    //              break;
                    //}
                    //If condition is true then directly goto to op[0] 
                    //TODO: please fix the following GOTO 
                   // if (cond) {
                   //     flag = true; 
                   //     tempICodeVector->push_back(new InterCode(InterCode::OPNTYPE::GOTO, 
                   //                 OpNode::OpCode::INVALID, operands[0]->OnTrue()));
                   // } else {
                   //     flag = true; 
                   //     if (operands[0]->OnFalse())  {
                   //             tempICodeVector->push_back(new InterCode(InterCode::OPNTYPE::GOTO, 
                   //                      OpNode::OpCode::INVALID, operands[0]->OnFalse()));
                   //     } 
                   // }
                   tempICodeVector->push_back(dupICodeVector->at(i));
                } else {
                    tempICodeVector->push_back(dupICodeVector->at(i));
                }
            } else {
                tempICodeVector->push_back(dupICodeVector->at(i));
            } 
        } else {
            if (dupICodeVector->at(i)->getOPNType() == InterCode::OPNTYPE::EXPR) {
                
                switch(dupICodeVector->at(i)->getsubCode()){
                    case OpNode::OpCode::BITNOT : {
                             //special case where third operand is null 
                             ExprNode* new1 = operands[1]; 

                             if (new1->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE) {
                                 if ((new1->value()->type()->tag() == Type::TypeTag::INT || new1->value()->type()->tag() == Type::TypeTag::UINT)) {
                                    
                                     flag            = true;
                                     int val         = stoi(new1->getRefName());
                                     ValueNode *temp =  new ValueNode(new Value(~val, Type::INT));
                                     tempICodeVector->push_back(new InterCode(InterCode::OPNTYPE::EXPR, OpNode::OpCode::ASSIGN, operands[0], temp));
                                 } else {
                                     tempICodeVector->push_back(dupICodeVector->at(i));
                                 }
                            } else {
                                     tempICodeVector->push_back(dupICodeVector->at(i));
                            }
                            break;
                    } 
                    
                    case OpNode::OpCode::NOT : {
                                     ExprNode* new1 = operands[1]; 
                                     if ((new1->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE) && 
                                         (new1->value()->type()->tag() == Type::TypeTag::BOOL)) {

                                             flag     = true;
                                             bool val = false;
                                             if (new1->getRefName().compare("1") == 0)
                                                 val = true;
                                             ValueNode *temp =  new ValueNode(new Value(!val));
                                             tempICodeVector->push_back(new InterCode(InterCode::OPNTYPE::EXPR, OpNode::OpCode::ASSIGN, operands[0], temp));
                                    } else {
                                             tempICodeVector->push_back(dupICodeVector->at(i));
                                    }
                            break;
                    }

                 default: tempICodeVector->push_back(dupICodeVector->at(i));
                          break;

                 }
            } else {

                tempICodeVector->push_back(dupICodeVector->at(i));
            }
        }     
    }

    setICodeVector(tempICodeVector);

    if(flag)
        *isOptimized = 1;
}


/***************************************************************************************************************
 * 3 Address Code :
 *      
 *      a := 6
 *      __vreg0 := c + a
 *
 * Code Optimized : Constant Propagation
 *
 *      a := 6
 *      __vreg0 := c + 6
 *
 * **************************************************************************************************************/
void BasicBlock::constantPropogation (int *isOptimized) {

    //cout <<"\n ENTER " << this->getBlockLabel() << " Size = " << dupICodeVector->size() << "\n";
    map <string, ExprNode*> cvar_map;
    vector<InterCode*>* dupICodeVector  = getICodeVector();
    vector<InterCode*>::iterator it     = dupICodeVector->begin();
    vector<InterCode*>* tempICodeVector = new vector<InterCode*> ();
    bool flag                           = false;

    for (; it != dupICodeVector->end(); it++) {

        ExprNode** op = (ExprNode**)(*it)->get3Operands();

        if (op[0] == NULL)
        {
            tempICodeVector->push_back(*it);
            continue;
        }

        //Node of type EXPR and op[0] = op[1] , op[2] is NULL 
        if ((*it)->getOPNType() == InterCode::OPNTYPE::EXPR && 
               (*it)->getsubCode() == OpNode::OpCode::ASSIGN && op[1] &&
                op[1]->exprNodeType() ==  ExprNode::ExprNodeType::VALUE_NODE) {

            //if variable already exists in map
            string str = op[0]->getRefName();
            //cout << "\n string: "<< str;
            if (cvar_map.find(str) != cvar_map.end()) {
                cvar_map.erase(str);
            }

            cvar_map.insert(pair<string, ExprNode*>(str, (ValueNode *)op[1]));
            //cout << "\n line 1 debug: str" << op[1]->getRefName();
        }

        switch ((*it)->getOPNType()) {
            case InterCode::OPNTYPE::EXPR :  {
                                                 ExprNode** oprnd = (ExprNode**)(*it)->get3Operands();

                                                 switch ((*it)->getsubCode()) {

                                                     case OpNode::OpCode::PLUS:
                                                     case OpNode::OpCode::MINUS:
                                                     case OpNode::OpCode::MULT:
                                                     case OpNode::OpCode::DIV:
                                                     case OpNode::OpCode::MOD:
                                                     case OpNode::OpCode::SHL:
                                                     case OpNode::OpCode::SHR:
                                                     case OpNode::OpCode::AND:
                                                     case OpNode::OpCode::OR:
                                                     case OpNode::OpCode::EQ: 
                                                     case OpNode::OpCode::NE: 
                                                     case OpNode::OpCode::GT:
                                                     case OpNode::OpCode::LT:
                                                     case OpNode::OpCode::GE:
                                                     case OpNode::OpCode::LE: 
                                                     case OpNode::OpCode::BITOR:
                                                     case OpNode::OpCode::BITAND:
                                                     case OpNode::OpCode::BITXOR:
                                                         {
                                                             //iterate over map. check and replace op[0] and op[1] value
                                                             if (cvar_map.find(oprnd[1]->getRefName()) != cvar_map.end()) {
                                                                 oprnd[1] = cvar_map.find(oprnd[1]->getRefName())->second; 
                                                                 flag     = true;
                                                             } 

                                                             if (cvar_map.find(oprnd[2]->getRefName()) != cvar_map.end()) {
                                                                 oprnd[2] = cvar_map.find(oprnd[2]->getRefName())->second; 
                                                                 flag     = true;
                                                             } 
                                                         }
                                                         break;

                                                     case OpNode::OpCode::BITNOT:
                                                     case OpNode::OpCode::UMINUS:
                                                     case OpNode::OpCode::NOT:
                                                         {   
                                                             //TODO: please fix this magic code
                                                             oprnd[1]->type()->name();
                                                             if (cvar_map.find(oprnd[1]->getRefName()) != cvar_map.end()) {
                                                                 oprnd[1] = cvar_map.find(oprnd[1]->getRefName())->second; 
                                                                 flag     =  true;
                                                             }
                                                         }
                                                         break;
                                                     
                                                     case OpNode::OpCode::ASSIGN:
                                                         {   
                                                             //TODO: please fix this magic code
                                                             oprnd[1]->type()->name();
                                                             if (cvar_map.find(oprnd[1]->getRefName()) != cvar_map.end()) {
                                                                 oprnd[1] = cvar_map.find(oprnd[1]->getRefName())->second; 
                                                                 flag     =  true;
                                                                 cvar_map.insert(pair<string, ExprNode*>(oprnd[0]->getRefName(), (ValueNode *)op[1]));
                                                             }
                                                         }
                                                         break;
                                                     default : break;
                                                 }
                                                 tempICodeVector->push_back(new InterCode (InterCode::OPNTYPE::EXPR, 
                                                             (*it)->getsubCode(), oprnd[0], oprnd[1], oprnd[2]));
                                             }
                                             break;

            
           // case InterCode::OPNTYPE::IFREL : {
           //                                     ExprNode** oprnd = (ExprNode**)(*it)->get3Operands();
           //                                        if (op[0] && op[1] == NULL && op[2] == NULL)  {
           //                                           if (cvar_map.find(oprnd[0]->getRefName()) != cvar_map.end()) {
           //                                               oprnd[0] = cvar_map.find(oprnd[0]->getRefName())->second; 
           //                                               flag     =  true;
           //                                           }
           //                                         } else {
           //                                                 if (op[1])  {
           //                                                      if (cvar_map.find(oprnd[1]->getRefName()) != cvar_map.end()) {
           //                                                          oprnd[1] = cvar_map.find(oprnd[1]->getRefName())->second; 
           //                                                          flag     =  true;
           //                                                      }
           //                                                 }
           //                                                 if (op[2])  {
           //                                                      if (cvar_map.find(oprnd[2]->getRefName()) != cvar_map.end()) {
           //                                                          oprnd[2] = cvar_map.find(oprnd[2]->getRefName())->second; 
           //                                                          flag     =  true;
           //                                                      }
           //                                                 }
           //                                        }
           //                                        tempICodeVector->push_back(new InterCode (InterCode::OPNTYPE::IFREL,
           //                                                  (*it)->getsubCode(), oprnd[0], oprnd[1], oprnd[2]));

           //                                 }
           //                                 break;

            default : 
                                             tempICodeVector->push_back(*it);
                                             break;
        }/*end outer switch*/

    }/*end for*/ 

    //cout <<"\n Exit " << this->getBlockLabel() << " Size = " << tempICodeVector->size() << "\n";

    setICodeVector(tempICodeVector);
    if(flag)
        *isOptimized = 1;
}

void BasicBlock::redundantGotoRemoval(int *isOptimized) {

    unsigned int i = 0;
    vector<InterCode*>* dupICodeVector  = getICodeVector();
    vector<InterCode*>* tempICodeVector = new vector<InterCode*> ();
    InterCode* gotoStart;
    bool flag                           = false;

    //cout <<"\n ENTER " << this->getBlockLabel() << " Size = " << dupICodeVector->size() << "\n";
    for (i = 0; i < dupICodeVector->size(); i++) {

        gotoStart = dupICodeVector->at(i);
        while (dupICodeVector->at(i)->getOPNType() == InterCode::OPNTYPE::GOTO) {
            i++;
            if ( i == dupICodeVector->size())
                break;
        }
        tempICodeVector->push_back(gotoStart);

    }/*end for*/ 

    setICodeVector(tempICodeVector);

    if (flag)
        *isOptimized = 1;

    //cout <<"\n Exit " << this->getBlockLabel() << " Size = " << tempICodeVector->size() << "\n";

}

void BasicBlock::zeroRemoval(int *isOptimized) {

    int i;
    vector<InterCode*>* dupICodeVector  = getICodeVector();
    vector<InterCode*>* tempICodeVector = new vector<InterCode*> ();
    bool flag                           = false;

    for ( i = 0; i < (int )dupICodeVector->size(); i++) {

        //tempICodeVector->push_back(dupICodeVector->at(i)); 
        ExprNode** operands = (ExprNode**)dupICodeVector->at(i)->get3Operands();
        ExprNode *new1, *new2; 

        if (operands[0] && operands[1] && operands[2]) {
            new1 = operands[1]; 
            new2 = operands[2]; 

            // Check if op[1] is valuenode and op[2] is not value and intercode type is expression
            if (dupICodeVector->at(i)->getOPNType() == InterCode::OPNTYPE::EXPR) {

                if ((new2->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE) && 
                        (new1->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE)) {

                    if (((new2->value()->type()->tag() == Type::TypeTag::INT || 
                                    new2->value()->type()->tag() == Type::TypeTag::UINT) && (new2->value()->ival() == 0)) 
                            ||  ((new2->value()->type()->tag() == Type::TypeTag::DOUBLE) && (new2->value()->dval() == 0.0))) {


                        switch(dupICodeVector->at(i)->getsubCode()) {
                            case OpNode::OpCode::PLUS  :
                            case OpNode::OpCode::MINUS :
                                tempICodeVector->push_back(new InterCode(InterCode::OPNTYPE::EXPR, 
                                            OpNode::OpCode::ASSIGN, operands[0], new1));
                                flag = true;
                                break;
                            case OpNode::OpCode::MULT  :
                                tempICodeVector->push_back(new InterCode(InterCode::OPNTYPE::EXPR, 
                                            OpNode::OpCode::ASSIGN, operands[0], new2));
                                flag = true;
                                break;
                            default :
                                tempICodeVector->push_back(dupICodeVector->at(i));
                                break;
                        }                                 
                    }
                    else
                        tempICodeVector->push_back(dupICodeVector->at(i));
                }
                else if ((new1->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE) && 
                        (new2->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE)) {

                    if (((new1->value()->type()->tag() == Type::TypeTag::INT || 
                                    new1->value()->type()->tag() == Type::TypeTag::UINT) && (new1->value()->ival() == 0)) 
                            ||  ((new1->value()->type()->tag() == Type::TypeTag::DOUBLE) && (new1->value()->dval() == 0.0))) {

                        switch(dupICodeVector->at(i)->getsubCode()) {
                            case OpNode::OpCode::PLUS  :
                                tempICodeVector->push_back(new InterCode(InterCode::OPNTYPE::EXPR, 
                                            OpNode::OpCode::ASSIGN, operands[0], new2));
                                flag = true;
                                break;
                            case OpNode::OpCode::MULT  :
                                tempICodeVector->push_back(new InterCode(InterCode::OPNTYPE::EXPR, 
                                            OpNode::OpCode::ASSIGN, operands[0], new1));
                                flag = true;
                                break;
                            default :
                                tempICodeVector->push_back(dupICodeVector->at(i));
                                break;
                        }                                 
                    }    
                    else
                        tempICodeVector->push_back(dupICodeVector->at(i));
                }
                else
                    tempICodeVector->push_back(dupICodeVector->at(i));
            }
            else
                tempICodeVector->push_back(dupICodeVector->at(i));
        }
        else if ((operands[0]) && (operands[1])) { 
            /* TODO : Remove self (dead) assignment by not pushing */
            if ((dupICodeVector->at(i)->getOPNType() == InterCode::OPNTYPE::EXPR) && 
                    (operands[0]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE) && 
                    (operands[1]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE)) {

                if (operands[0]->getRefName() == operands[1]->getRefName()) {
                    flag = true;
                }
                else
                    tempICodeVector->push_back(dupICodeVector->at(i));
            }
            else
                tempICodeVector->push_back(dupICodeVector->at(i));
        }
        else
            tempICodeVector->push_back(dupICodeVector->at(i));
    }

    setICodeVector(tempICodeVector);

    //cout << "\nReturned \n";

    if (flag)
        *isOptimized = 1;
}

void BasicBlock::print(ostream &os) {

    os << endl; 
    os << blocklabel << ":" << endl;
    prtSpace(os, TAB_SPACE);
    os << "Prev: (";  

    vector<string>::iterator iter = PrevBlockLabels.begin();
    for ( ; iter != PrevBlockLabels.end(); ++iter) {
        os << " " << *iter;
    }
    os << " )\n\n";

    vector <InterCode*>::iterator it = InterCodeVector.begin();

    for (; it != InterCodeVector.end(); ++it) {
        (*it)->print(os);
        os << endl;
    }

    os << endl; 
    prtSpace(os, TAB_SPACE);
    os << "next: (";  
    iter = NextBlockLabels.begin();
    for ( ; iter != NextBlockLabels.end(); ++iter) {
        os << " " << *iter;
    }
    os << " )\n";
}



void InterCodesClass::addCode (InterCode *code) {
    if (code != NULL) 
        InterCodeVector.push_back(code);  
}

void InterCodesClass::addCode (InterCodesClass* code) {
    if (code != NULL) {
        vector <InterCode*> code_l = code->InterCodeVector;
        for (int i = 0; i < (int )code_l.size(); ++i)
            InterCodeVector.push_back(code_l.at(i));
    }
}

void InterCodesClass::addCode (InterCode::OPNTYPE op, void *a, 
        void *b, void *c, OpNode::OpCode subopc) { 

    InterCodeVector.push_back(new InterCode (op, subopc, a, b, c));     
}

void InterCodesClass::print (ostream &os) {
    vector <InterCode*>::iterator it =  InterCodeVector.begin();
    for (; it != InterCodeVector.end(); ++it)  {
        (*it)->print(os);
        os << endl;
    }
}


BasicBlock* BasicBlocksClass::getBlockWithLabel (string label) {
    vector <BasicBlock*>::iterator it = bbVector.begin();

    for (; it != bbVector.end(); ++it) {
        if (label.compare((*it)->getBlockLabel()) == 0)
            return (*it);
    }
    //if block does not exist,then create block with new label
    BasicBlock *newbb = new BasicBlock(label);
    bbVector.push_back(newbb);
    label_block_map.insert(pair<string, BasicBlock*> (label, newbb));
    return newbb;
}

void BasicBlocksContainer::removeBlocks()  {
    map <string, BasicBlocksClass*>::iterator it = bbContainer.begin();
    for (; it != bbContainer.end(); ++it) {
        if ((bbUsedContainers.count((*it).first) == 0) && (((*it).first).compare("global") != 0) && (((*it).first).substr(0, 7) != E_PREFIX)) {
            bbContainer.erase(it);
        }
    }
}


BasicBlocksClass* BasicBlocksContainer::insertInContainer (string name) {

    if (bbContainer.find(name) == bbContainer.end()) {
        BasicBlocksClass* bbCls = new BasicBlocksClass(); 
        bbContainer.insert (pair<string, BasicBlocksClass*> (name, bbCls));

        return bbCls;
    } else {
        return bbContainer.find(name)->second;
    }
}

void BasicBlocksContainer::optimize() {

    removeBlocks();
    if (debugLevel > 0) {
        cout << "\n\n===== Uncalled Functions Removal Optimization =======";
        //printUsedContainers();
        print(cout);
    }

    map <string, BasicBlocksClass*>::iterator it = bbContainer.begin();

    for (; it != bbContainer.end(); ++it) {
        (*it).second->blockOptimize();

    }

    if (debugLevel > 0) {
        cout << "\n\n=====Constant propogation and folding optimization=======";
        print(cout);
    }

    for (it = bbContainer.begin() ; it != bbContainer.end(); ++it) {

        //no need of live var analysis for global 
        if ((*it).first.compare("global") != 0) 
            (*it).second->liveVariableAnalysis();
    }

    if (debugLevel > 0) {
        cout << "\n\n=========Dead Code Elmination Optimization==================";
        print(cout);
    }
}

void BasicBlocksContainer::print(ostream &os) {
    map <string, BasicBlocksClass*>::iterator it = bbContainer.begin();
    for (; it != bbContainer.end(); it++) {
        os << "\n#####" << (*it).first << "#####" << endl;
        (*it).second->print(os);
        os << endl;
    }
}

void BasicBlocksContainer::createBlockStruct (InterCodesClass* ic) {

    vector <InterCode*>* icvec = ic->getICodeVector();
    vector <InterCode*>::iterator it =  icvec->begin();

    string str;
    BasicBlocksClass *BBcls = insertInContainer ("global");
    BasicBlock* block       = BBcls->getBlockWithLabel("global");
    bool isPrevJmp          = false; 
    bool isIfJmp            = true;
    bool isPrevGoto         = false;
    bool isPrevReturn       = false;
    BasicBlock* bb          = NULL; 


    for (; it != icvec->end(); ++it) {
        InterCode::OPNTYPE op = (*it)->getOPNType();
        void** opd            = (*it)->get3Operands();

        if (isPrevGoto && op != InterCode::OPNTYPE::LABEL)
            continue;
        
        if (isPrevReturn && !(op == InterCode::OPNTYPE::LABEL || op == InterCode::OPNTYPE::LEAVE))
            continue;
        
        if ((block == NULL || BBcls == NULL)) { 
            BBcls = insertInContainer ("global"); 
            block = BBcls->getBlockWithLabel("global");
        }
        
        if (op == InterCode::OPNTYPE::CALL ) {
            str = ((InvocationNode*)opd[0])->symTabEntry()->name();
            insertInUsedList (str);
        }

        if (op == InterCode::OPNTYPE::GOTO ) {
            block->addCode (*it); 

            str = ((InterCode *)opd[0])->getLabel(); 
            block->addNextBlock(str);

            bb =  BBcls->getBlockWithLabel(str); 
            bb->addPrevBlock (block->getBlockLabel());

            isPrevJmp   = true;
            isPrevGoto  = true;
        } else if (op == InterCode::OPNTYPE::IFREL) {
            block->addCode (*it); 

            ExprNode* cond       = (ExprNode*) opd[0];
            InterCode* true_lab  = cond->OnTrue();
            InterCode* false_lab = cond->OnFalse();

            if(true_lab) {
                block->addNextBlock(true_lab->getLabel());
                bb =  BBcls->getBlockWithLabel(true_lab->getLabel()); 
                bb->addPrevBlock (block->getBlockLabel());
            }

            if(false_lab) {
                block->addNextBlock(false_lab->getLabel());
                bb =  BBcls->getBlockWithLabel(false_lab->getLabel()); 
                bb->addPrevBlock (block->getBlockLabel());
            }
            isIfJmp     = true;
            isPrevJmp   = true;

        } else if (op == InterCode::OPNTYPE::LABEL) {
            isPrevGoto   = false;
            isPrevReturn = false;
            str          = (*it)->getLabel();

            if (block == NULL) {
                /* to determine if new container to create or to add it to global block*/ 

                if (((it + 1) != icvec->end()) && ((*(it + 1))->getOPNType() != InterCode::OPNTYPE::ENTER))
                    str = "global"; 

                BBcls = insertInContainer (str);
                block = BBcls->getBlockWithLabel(str);

            } else {
                /* when new label appeared and last statement was not leave */
                

                if (((it + 1) != icvec->end()) && ((*(it + 1))->getOPNType() == InterCode::OPNTYPE::ENTER)) {
                    BBcls = insertInContainer (str);
                    block = BBcls->getBlockWithLabel(str);
                } else {
                    /* this is normal block*/
                    if (isPrevJmp == false && (str.compare("global") != 0)) {
                        block->addNextBlock(str); 
                        bb =  BBcls->getBlockWithLabel(str); 
                        bb->addPrevBlock (block->getBlockLabel());
                    }
                    if (isIfJmp == true && (str.compare("global") != 0)) {
                        block->addNextBlock(str); 
                        bb =  BBcls->getBlockWithLabel(str); 
                        bb->addPrevBlock (block->getBlockLabel());
                        isIfJmp     = false;
                    }
                    block = BBcls->getBlockWithLabel(str);
                }
            }
            BBcls->ordervec.push_back(str);

        } else if (op == InterCode::OPNTYPE::LEAVE ) {
            block->addCode (*it); 
            block       = NULL; 
            isPrevJmp   = true;
            isPrevReturn = false;

        } else if (op == InterCode::OPNTYPE::RETURN ) {
            block->addCode (*it); 
            isPrevReturn = true; 
        
        } else {
            block->addCode (*it); 
            isPrevJmp   = false;
        }
    }

    //Reorder bbvector in each basicblockclassj according to ordervec
    map<string, BasicBlocksClass*>::iterator it2 = getContainer()->begin();
    for (; it2 != getContainer()->end(); ++it2 ) {

        vector <BasicBlock*> newBBVec;

        BasicBlocksClass* BBCls        = (*it2).second;
        vector<string>::iterator iter2 = BBCls->ordervec.begin(); 
        for (; iter2 != BBCls->ordervec.end(); ++iter2) {
            string str    = *iter2;
            BasicBlock *b = (BBCls->getLabelMap()->find(str))->second;

            if(b) 
                newBBVec.push_back(b);
        }
        BBCls->setVector (newBBVec);
    }

}

void inline check_remove (set<string> &st, string str) {
    set<string>::iterator it = st.find(str);
    if(it != st.end())
        st.erase(it);

}


void iterateOnSingleBlock (BasicBlock *BB, map<string, bool> &ifVisited, 
                BasicBlocksClass *bbCls, bool remove, int *updated) {

        string bb_label = BB->getBlockLabel(); 
        
        //mark block as visited
        if (ifVisited.find(bb_label)->second == 0) {
            ifVisited.erase(bb_label); 
            ifVisited.insert (pair<string, bool>(bb_label, 1));
        }

        //start from bottom.
        //define vector of variables which are being used and if definition is encountered then remove it
        set <string> variablesUsed;
        
        //store livevars in vector to check later if values have changed.
        //If not we can return updated as false, to break recursion
        set <string> OriginalLiveVars (BB->StartLiveVars);

        //initialize start live vars of block as equal to end vars
        //and when corresponding def is encountered in this block, remove them from start live vars
        BB->StartLiveVars = BB->EndLiveVars;
        
        vector <InterCode*>::reverse_iterator rit = BB->getICodeVector()->rbegin();
        
        for (; rit != BB->getICodeVector()->rend(); ++rit) {
               
               ExprNode** opnds = (ExprNode**)(*rit)->get3Operands();
               
               switch ((*rit)->getOPNType()) {
                       case InterCode::OPNTYPE::EXPR : {
                                    
                                    switch ((*rit)->getsubCode()) {
                                           case OpNode::OpCode::ASSIGN :  
                                                      if (remove) {
                                                            //check if its present in variables used and endliveVar else remove it
                                                            if (variablesUsed.find(opnds[0]->getRefName()) == variablesUsed.end() 
                                                                && BB->EndLiveVars.find(opnds[0]->getRefName()) == BB->EndLiveVars.end() ) {
                                                                
                                                                VariableEntry* ventry = (VariableEntry*)(((RefExprNode *)(opnds[0]))->symTabEntry());
                                                                if(ventry->varKind() != VariableEntry::VarKind::GLOBAL_VAR) {
                                                                    vector <InterCode*>::iterator it = rit.base();
                                                                    it--;
                                                                    BB->getICodeVector()->erase(it);
                                                                    break;
                                                                }
                                                            }
                                                      } 
                                                      check_remove (variablesUsed, opnds[0]->getRefName()); 
                                                      check_remove (BB->StartLiveVars, opnds[0]->getRefName()); 
                                                            
                                                      if (opnds[1]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE) 
                                                           variablesUsed.insert(opnds[1]->getRefName());
                                                      
                                                      break;
                                           default: 
                                                      if (opnds[0]) {
                                                             if (remove) {
                                                                //check if its present in variables used and endliveVar else remove it
                                                                if (variablesUsed.find(opnds[0]->getRefName()) == variablesUsed.end() 
                                                                    && BB->EndLiveVars.find(opnds[0]->getRefName()) == BB->EndLiveVars.end()) {
                                                                       
                                                                    VariableEntry* ventry = (VariableEntry*)(((RefExprNode *)(opnds[0]))->symTabEntry());
                                                                    if(ventry->varKind() != VariableEntry::VarKind::GLOBAL_VAR) {
                                                                            vector <InterCode*>::iterator it = rit.base();
                                                                            it--;
                                                                            BB->getICodeVector()->erase(it);
                                                                            break;
                                                                    } 
                                                                }
                                                             } 
                                                             check_remove (variablesUsed, opnds[0]->getRefName()); 
                                                             check_remove (BB->StartLiveVars, opnds[0]->getRefName()); 
                                                      }
                                                     
                                                      if (opnds[1] && opnds[1]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE)
                                                              variablesUsed.insert(opnds[1]->getRefName());
                                                      if (opnds[2] && opnds[2]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE)
                                                              variablesUsed.insert(opnds[2]->getRefName());
                                                      
                                                      break;
                                    }
                       }
                       break;
                       
                       case InterCode::OPNTYPE::PRINT: {
                                      if (opnds[0]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE) 
                                           variablesUsed.insert(opnds[0]->getRefName());
                       }
                       break;
                       
                       case InterCode::OPNTYPE::CALL: {
                                    if (opnds[1]) {
                                          if (remove) {
                                              //check if its present in variables used and endliveVar else remove it
                                              if (variablesUsed.find(opnds[1]->getRefName()) == variablesUsed.end() 
                                                  && BB->EndLiveVars.find(opnds[1]->getRefName()) == BB->EndLiveVars.end()) {
                                                
                                                      VariableEntry* ventry = (VariableEntry*)(((RefExprNode *)(opnds[0]))->symTabEntry());
                                                      if(ventry->varKind() != VariableEntry::VarKind::GLOBAL_VAR) {
                                                            opnds[1] = NULL;
                                                            break;
                                                      }
                                              }
                                          } 
                                          check_remove (variablesUsed, opnds[1]->getRefName()); 
                                          check_remove (BB->StartLiveVars, opnds[1]->getRefName()); 
                                    }
                       }
                       break;

                       case InterCode::OPNTYPE::RETURN : 
                       case InterCode::OPNTYPE::APARAM : {
                                    if (opnds[0] && opnds[0]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE)
                                           variablesUsed.insert(opnds[0]->getRefName());
                       } 
                       break;

                       case InterCode::OPNTYPE::FPARAM : {
                                   check_remove (variablesUsed, opnds[0]->getRefName()); 
                                   check_remove (BB->StartLiveVars, opnds[0]->getRefName()); 
                       }
                       break;
                       
                       case InterCode::OPNTYPE::IFREL : {
                                   if (opnds[2] && opnds[1]) {
                                        
                                        if (opnds[1]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE) 
                                            variablesUsed.insert(opnds[1]->getRefName());
                                        if (opnds[2]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE) 
                                            variablesUsed.insert(opnds[2]->getRefName());
                                        
                                   } else if (opnds[1] && opnds[1]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE) {
                                        variablesUsed.insert(opnds[1]->getRefName());
                                   } else if (opnds[0]->exprNodeType() != ExprNode::ExprNodeType::VALUE_NODE) {
                                        variablesUsed.insert(opnds[0]->getRefName());
                                   }
                       }
                       break;
                       
                       default: break; 
               }
        }

        //push back used vars in livevars at start of block
        for (set<string>::iterator iter = variablesUsed.begin(); iter != variablesUsed.end(); ++iter) {
           BB->StartLiveVars.insert(*iter);
        }
        
        if ((BB->StartLiveVars.size() > 0) && (OriginalLiveVars.size() != BB->StartLiveVars.size())) {
           *updated = 1;
            return;
        }
       
        for(set<string>::iterator iter = BB->StartLiveVars.begin(); iter != BB->StartLiveVars.end(); ++iter) {
            if(OriginalLiveVars.find(*iter) == OriginalLiveVars.end()) {
                *updated = 1;
                 return;
            }
        }
        
        *updated = 0;
        return;
}

void recurseOnBlocks (BasicBlock *BB, map<string, bool> &ifVisited, BasicBlocksClass *bbCls) {
        
        if(BB == NULL)
            return;
        
        int updated = 0; 
        iterateOnSingleBlock(BB, ifVisited, bbCls, false, &updated);
        
        if (updated == 0)
          return;
        
        //go over all its previous blocks
        vector<string>::iterator it = BB->getPrevBlockLabels()->begin();
        for (; it != BB->getPrevBlockLabels()->end(); ++it) {
            
            BasicBlock *newBB =  bbCls->getLabelMap()->find(*it)->second;
            
            //add livevars to parent's end vars
            for (set<string>::iterator it = BB->StartLiveVars.begin();
                        it != BB->StartLiveVars.end(); ++it) {
                newBB->EndLiveVars.insert(*it);
            }
            
            //recursive calls to all previous blocks 
            recurseOnBlocks (newBB, ifVisited, bbCls);
        }
}


void BasicBlocksClass::liveVariableAnalysis() {

    //keep bit vector of all blocks to check which are visited
    map <string, bool> ifVisited;
    vector <BasicBlock*>::iterator it;
    string bb_label;

    //all blocks are initially unvisited
    for (it = bbVector.begin(); it != bbVector.end(); ++it) {
        ifVisited.insert (pair<string, bool>((*it)->getBlockLabel(), 0));
    }

    //iterate over all blocks
    for (it = bbVector.begin(); it != bbVector.end(); ++it) {

        BasicBlock *BB = (BasicBlock *)(*it); 
        string bb_label = BB->getBlockLabel(); 

        //if block is unvisited
        if (ifVisited.find(bb_label)->second == 0) {
            recurseOnBlocks (BB, ifVisited, this);
        }
    }

    //Dead code removal
    for (it = bbVector.begin(); it != bbVector.end(); ++it) {
        int u = 0;
        iterateOnSingleBlock((BasicBlock *)(*it), ifVisited, this, true, &u);
    }

    //DEBUG
    if (debugLevel > 0) {
        for (it = bbVector.begin(); it != bbVector.end(); ++it) {
            BasicBlock *BB = (BasicBlock *)(*it);
            bb_label       = BB->getBlockLabel(); 

            cout << "\n\n blockName: " << bb_label << endl;
            cout << "live vars at start: ";
            for (set<string>::iterator it = BB->StartLiveVars.begin(); it != BB->StartLiveVars.end(); ++it) 
                cout << *it << ",";

            cout << endl << "live vars at End: ";
            for (set<string>::iterator it = BB->EndLiveVars.begin(); it != BB->EndLiveVars.end(); ++it) 
                cout << *it << ",";
        }
    }

}

//void removeExpr (BasicBlock *BB, vector<pair<string, int>> exprMap, map<string, vector<int>> op1Map, map <string, vector<int>> op2Map) {
//        
//        //copy parents exprmap into curr block 
//        vector<pair<string, int>> curr_exprMap (exprMap);
//        
//        vector <InterCode*>::reverse_iterator it = BB->getICodeVector()->begin();
//        
//        for (; it != BB->getICodeVector()->end(); ++it) {
//               ExprNode** opnds = (ExprNode**)(*it)->get3Operands();
//               
//               switch ((*it)->getOPNType()) {
//                       case InterCode::OPNTYPE::EXPR : {
//                                    
//                                    switch ((*it)->getsubCode()) {
//                                           case default: {
//                                                 if (op[0] && op[1] && op[2]) {
//                                                     ostringstream os; 
//                                                     os << op[0]->getRefName() << " = " << op[1]->getRefName();
//                                                     os << " " << OpNode::opInfo[(int )subCode_].name_ << " ";
//                                                     os << op[2]->getRefName();
//                                                     
//                                                     curr_exprMap.push_back(pair<string, int> (os.str(), 1)); 
//                                                 }
//                                                         
//
//
//                                           }
//                                           break;
//                                            
//                                           default : break;
//                                    }
//                        }
//                        break;
//
//                        default : break;
//               }
//       }
//}
//
//void BasicBlocksClass::commonSubExprElimination() {
//
//    vector <BasicBlock*>::iterator it;
//    
//    for (it = bbVector.begin(); it != bbVector.end(); ++it) {
//        BasicBlock *BB = (BasicBlock *)(*it);
//        if (BB->getPrevBlockLabels()->size() == 0)
//            removeExpr (BB);
//    }
//
