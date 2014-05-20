#include "FinalMachineCodeGen.h"
#include "InterCode.h"
#include  <stack>

#define     IS_UNKNOWN(type) Type::isUnknown(type->tag())
#define     IS_FLOAT(type)   Type::isFloat(type->tag())

#define     IS_INT(type)     Type::isInt  (type->tag())
#define     IS_STRING(type)  Type::isString  (type->tag())
#define     PRT_REG          os<<src1_regName<<" "<<src2_regName<<" "<<dst_regName<<endl;

static int float_reg_count  = 10;
static int int_reg_count    = 20;
static vector     <string>  reg_list;
static int fl_reg_used_cnt  = 0 ;
static int int_reg_used_cnt = 0 ;
static int in;
static int fl;

stack<ExprNode*>formal_param_list;

string allocateNewRegName(bool floatStatus)
{
    char newRegName[5];
    for(int i = 0 ; i < 5; i++)
        newRegName[i] = '\0';
    (floatStatus) ? sprintf(newRegName, "F%.3d", float_reg_count++) : sprintf(newRegName, "R%.3d", int_reg_count++);
    (floatStatus) ? fl_reg_used_cnt++ : int_reg_used_cnt++ ;
    string temp(newRegName);
    return temp;
}

string convertToInt(ExprNode *expr, ostream &os)
{
    os<<"MOVFI ";
    os<<expr->getRegisterName()<<" ";
    string new_reg = allocateNewRegName(false);
    os<<new_reg<<endl;
    return new_reg;
}

string convertToFloat(ExprNode *expr, ostream &os)
{
    os<<"MOVIF ";
    os<<expr->getRegisterName()<<" ";
    string new_reg = allocateNewRegName(true);
    os<<new_reg<<endl;
    return new_reg;
}

void relational(string dst_regName, ostream &os)
{
    LabelClass *true_lab  = new LabelClass();     
    InterCode  *ic1 = true_lab->assignLabel();    
    LabelClass *false_lab = new LabelClass();     
    InterCode  *ic2 = false_lab->assignLabel();   
                                                                      
    os<<ic1->getLabel()<<endl;
    os<<"MOVI 0 "<<dst_regName<<endl;
    os<<"JMP "<<ic2->getLabel()<<endl;
    os<<ic1->getLabel()<<": ";
    os<<"MOVI 1 "<<dst_regName<<endl;
    os<<ic2->getLabel()<<": ";
}

void NOTLogic(string src1_regName, string dst_regName, ostream &os)
{
    LabelClass *true_lab  = new LabelClass(); 
    InterCode  *ic1 = true_lab->assignLabel();
    LabelClass *false_lab = new LabelClass(); 
    InterCode  *ic2 = false_lab->assignLabel();
    os<<"JMPC EQ ";
    os<<src1_regName<<" 0 "<<(ic2->getLabel())<<endl;
    os<<"MOVI 0 "<<dst_regName<<" "<<endl;
    os<<"JMP "<<(ic1->getLabel())<<endl;
    os<<(ic2->getLabel())<<": ";
    os<<"MOVI 1 "<<dst_regName<<endl;
    os<<(ic1->getLabel()) << ": ";
}

void ShiftLogic(bool isSHL, string src1_regName, string src2_regName, string dst_regName, ostream &os)
{
     LabelClass *l1  = new LabelClass();
     InterCode  *ic1 = l1->assignLabel();
     LabelClass *l2  = new LabelClass();
     InterCode  *ic2 = l2->assignLabel();

     os<<"MOVI "<<src1_regName<<" "<<RSH_Val<<endl;
     os<<"MOVI "<<src2_regName<<" "<<RSH_Cnt<<endl;
     os<<ic2->getLabel()<<": ";
     os<<"JMPC GT 1 "<<RSH_Cnt<<" "<<ic1->getLabel()<<endl;
     if (isSHL)
         os<<"MUL "<<RSH_Val<<" 2 "<<RSH_Val<<endl;
     else
         os<<"DIV "<<RSH_Val<<" 2 "<<RSH_Val<<endl;
     os<<"SUB "<<RSH_Cnt<<" 1 "<<RSH_Cnt<<endl;
     os<<"JMP "<<ic2->getLabel()<<endl;
     os<<ic1->getLabel()<<": ";
     os<<"MOVI "<<RSH_Val<<" "<<dst_regName<<endl;
}

void ANDLogic  (string src1_regName, string src2_regName, string dst_regName, ostream &os)
{
    LabelClass *true_lab  = new LabelClass(); 
    InterCode  *ic1 = true_lab->assignLabel();
    LabelClass *false_lab = new LabelClass(); 
    InterCode  *ic2 = false_lab->assignLabel();

    os<<"JMPC EQ "  <<src1_regName<<" 0 "<<(ic2->getLabel())<<endl;
    os<<"JMPC EQ "  <<src2_regName<<" 0 "<<(ic2->getLabel())<<endl;
    os<<"MOVI 1 "   <<dst_regName<<endl;
    os<<"JMP "      <<(ic1->getLabel())<<endl;
    os<<(ic2->getLabel())<<": ";
    os<<"MOVI 0 "   <<dst_regName<<endl;
    os<<(ic1->getLabel())<<": ";

}

void ORLogic  (string src1_regName, string src2_regName, string dst_regName, ostream &os) {
    LabelClass *true_lab  = new LabelClass(); 
    InterCode  *ic1 = true_lab->assignLabel();
    LabelClass *false_lab = new LabelClass(); 
    InterCode  *ic2 = false_lab->assignLabel();

    os<<"JMPC EQ "  <<src1_regName<<" 1 "<<(ic2->getLabel())<<endl;
    os<<"JMPC EQ "  <<src2_regName<<" 1 "<<(ic2->getLabel())<<endl;
    os<<"MOVI 0 "   <<dst_regName<<endl;
    os<<"JMP "      <<(ic1->getLabel())<<endl;
    os<<(ic2->getLabel())<<": ";
    os<<"MOVI 1 "   <<dst_regName<<endl;
    os<<(ic1->getLabel())<<": ";
}

void push_registers(string retAdd, ostream &os) {
    char newRegName[5];
    in = int_reg_count-1;
    fl = float_reg_count-1;
    for(int i=0; i<int_reg_used_cnt; i++)
    {
        sprintf(newRegName, "R%.3d", in);
        string temp(newRegName);
        os<<"STI  "<< temp<<" "<<RSP<<endl;
        os<<"SUB "<<RSP<<" 4 "<<RSP<<endl;
        in--;
    }

    for(int i=0; i<fl_reg_used_cnt; i++)
    {
        sprintf(newRegName, "F%.3d", fl);
        string temp(newRegName);
        os<<"STF  "<< temp<<" "<<RSP<<endl;
        os<<"SUB "<<RSP<<" 4 "<<RSP<<endl;
        fl--;
    }

    string dst_regName     = allocateNewRegName(false);
    int_reg_used_cnt--;
    os<<"MOVL "<<retAdd<<" "<<dst_regName<<endl;
    os<<"STI  "<<dst_regName<<" "<<RSP<< C_PUSH_RET <<endl;
    os<<"SUB "<<RSP<<" 4 "<<RSP << endl;

    vector <string> ::iterator iter1  = reg_list.begin();
    for(; iter1 != reg_list.end(); iter1++)
    {
        os<<(*iter1)<<endl;
        os<<"SUB "<<RSP<<" 4 "<<RSP<<endl;
    }
}

void pop_registers(ostream &os) {
    char newRegName[5];

    for(int i=0; i<fl_reg_used_cnt; i++)
    {
        sprintf(newRegName, "F%.3d", ++fl);
        string temp(newRegName);
        os<<"ADD "<<RSP<<" 4 "<<RSP<<endl;
        os<<"LDF "<<RSP<<" "<< temp<<endl;
   //     fl++;
    }

    for(int i=0; i<int_reg_used_cnt; i++)
    {
        sprintf(newRegName, "R%.3d", ++in);
        string temp(newRegName);
        os<<"ADD "<<RSP<<" 4 "<<RSP<<endl;
        os<<"LDI "<<RSP<<" "<< temp<<endl;
 //       in++;
    }
}

static bool IsLastBlockEmpty;
void FinalMachineCodeGen::finalCodeGen (BasicBlocksContainer *bbCls, ostream & os) {
    if (!bbCls)
        return;

    os << "begin: ";
    os << "MOVI " << " " << 10000 << RSP << C_INIT_RSP << endl;
    os << "JMP global" << endl << endl;

    map <string, BasicBlocksClass*>::iterator it = bbCls->getContainer()->begin();
    for (; it != bbCls->getContainer()->end(); ++it) {
        vector <BasicBlock*>* basicBlock     = (*it).second->getVector();      
        vector <BasicBlock*>::iterator iter1 = basicBlock->begin();
        string blockLabel = (*iter1)->getBlockLabel();

        if (blockLabel != "global")
            os << "// Function/Event Module begins" << endl;
        IsLastBlockEmpty = false;

        for (; iter1 != basicBlock->end(); iter1++) {
            vector <InterCode*> ::iterator iter2  = (*iter1)->getICodeVector()->begin();

            if (IsLastBlockEmpty)
                os << "JMP " << (*iter1)->getBlockLabel() << endl;
            os << (*iter1)->getBlockLabel() << ": ";
            IsLastBlockEmpty = true;

            for(; iter2 != (*iter1)->getICodeVector()->end(); iter2++) {
                IsLastBlockEmpty = false;
                convert_IC_MC(*iter2, os);
            }
        }

        // Jump to event handling
        if (blockLabel == "global")
            os << "JMP " << S_START;
        else
            os << "// Function/Event Module Ends" << endl;
        os << endl;
    }
    os << endl << S_EXIT << ": ";
    os << "PRTS " << "\"Successfully Exited\\n\"" << endl << endl;
}

void FinalMachineCodeGen::convert_IC_MC(InterCode *interCode, ostream &os) {
    string  dst_regName ;
    string  src1_regName;
    string  src2_regName;
    ExprNode **opndsList = (ExprNode **)interCode->get3Operands();
    switch(interCode->getOPNType()) {
        case CALL   :   {
                            string func_name = ((InvocationNode*)opndsList[0])->symTabEntry()->name();
                            string retAddrReg;

                            if (int_reg_used_cnt || fl_reg_used_cnt) {
                                retAddrReg = LabelClass::assignLabel()->getLabel();
                            } else {
                                retAddrReg = opndsList[0]->next()->getLabel();
                            }

                            push_registers(retAddrReg, os); 
                            os << "JMP  " << func_name << endl;
                            if (int_reg_used_cnt || fl_reg_used_cnt)
                                os << retAddrReg << ": ";
                            pop_registers(os);
                            if(opndsList[1])
                            {
                                ExprNode *src1    = opndsList[1];
                                string retValueReg = src1->getRegisterName();
                                if(IS_FLOAT(src1->type()))
                                    os<<"MOVF "<<RRV_F<<" ";
                                else
                                    os<<"MOVI "<<RRV_I<<" ";
                                os<<retValueReg<<endl;
                                int_reg_used_cnt--; 
                            }
                            reg_list.resize(0);

                            break;
                        }
        case FPARAM:    {
                            ExprNode *src1       = opndsList[0];
                            dst_regName = src1->getRegisterName();

                            os << "ADD "<<RSP<<" 4 "<<RSP << endl;
                            if (IS_FLOAT(src1->type()))
                                os << "LDF "<<RSP<<" "<<dst_regName;
                            else 
                                os << "LDI "<<RSP<<" "<<dst_regName;
                            os << C_FPARAM << endl;
                            break; 
                        } 
        case APARAM:    {
                            ExprNode *param       = opndsList[0];
                            Type     *ret_type = (param) ? (param->coercedType() ? (Type*)param ->coercedType() : param->type()): NULL ;
                            string    str;
                            string regName;
                            if(param)
                            {
                                if(param->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE)
                                {
                                    if(IS_FLOAT(ret_type))
                                     {
                                         regName = allocateNewRegName(true);
                                         str = "STF "+regName+" "+RSP;
                                         fl_reg_used_cnt--;
                                     }
                                    else
                                        str = "STI "+param->getRefName()+" "+RSP;
                                }
                                else
                                {
                                    if(IS_FLOAT(ret_type))
                                       {
                                           string temp;
                                           regName = param->getRegisterName();
                                           if(regName.at(0) == 'R')
                                              {
                                                  temp =  convertToFloat(param, os);
                                                  str = "STF "+temp+" "+RSP;
                                              }
                                            else
                                                  str = "STF "+param->getRegisterName()+" "+RSP;
                                        }
                                    else
                                        str = "STI "+param->getRegisterName()+" "+RSP;

                                }
                                reg_list.push_back(str + C_APARAM);
                            }
                            break;
                        }
        case RETURN:    {
                            if (opndsList[0]) {
                                ExprNode *ret_val  = opndsList[0];
                                Type     *ret_type = (ret_val) ? (ret_val ->coercedType() ? (Type*)ret_val ->coercedType() : ret_val ->type()): NULL ;

                                if (IS_FLOAT(ret_type)) {
                                    dst_regName = ret_val->getRegisterName();
                                    os<<"MOVF "<<dst_regName<<" "<<RRV_F;
                                } else {
                                    dst_regName = ret_val->getRegisterName();
                                    os<<"MOVI "<<dst_regName<<" "<<RRV_I;
                                }
                                os<<endl;
                            }
                            os<<"ADD " << RSP << " 4 " << RSP << endl;
                            os<<"LDI " << RSP << " " << RRET_ADD << C_POP_RET << endl;
                            os << "JMPI " << RRET_ADD << endl;
                            break;
                        }
        case EXPR:      {
                            ExprNode *dst = opndsList[0];
                            Type   *type_dst  = dst ->coercedType() ? (Type*)dst ->coercedType() : dst ->type();
                            if (opndsList[0] && opndsList[1] && opndsList[2]) {
                                ExprNode *src1 = opndsList[1];
                                ExprNode *src2 = opndsList[2];

                                Type *type_src1 = src1->coercedType() ? (Type*)src1->coercedType() : src1->type();
                                Type *type_src2 = src2->coercedType() ? (Type*)src2->coercedType() : src2->type();
                                if (IS_FLOAT(type_dst)) {
                                    if (src1) {
                                        src1_regName = src1->getRegisterName();
                                        if (!IS_FLOAT(type_src1))
                                            src1_regName = convertToFloat(src1, os);
                                    }
                                    if (src2) {
                                        src2_regName = src2->getRegisterName();
                                        if(!IS_FLOAT(type_src2))
                                            src2_regName= convertToFloat(src2, os);
                                    }
                                    if (dst)
                                        dst_regName = dst->getRegisterName();

                                    switch(interCode->getsubCode()) {
                                        case OpNode::OpCode::PLUS  :   {   os<<"FADD ";    PRT_REG;    break; } 
                                        case OpNode::OpCode::MINUS :   {   os<<"FSUB ";    PRT_REG;    break; }
                                        case OpNode::OpCode::MULT  :   {   os<<"FMUL ";    PRT_REG;    break; }     
                                        case OpNode::OpCode::DIV   :   {   os<<"FDIV ";    PRT_REG;    break; }
                                        case OpNode::OpCode::GT    :   {   os<<"FGT " ;    PRT_REG;    break; }
                                        case OpNode::OpCode::GE    :   {   os<<"FGE " ;    PRT_REG;    break; }
                                        case OpNode::OpCode::LT    :   {   os<<"FLT " ;    PRT_REG;    break; }
                                        case OpNode::OpCode::LE    :   {   os<<"FLE " ;    PRT_REG;    break; }
                                        case OpNode::OpCode::EQ    :   {   os<<"FEQ " ;    PRT_REG;    break; }
                                        case OpNode::OpCode::NE    :   {   os<<"FEQ " ;    PRT_REG;    break; }
                                        default                    :                  break;    
                                    }
                                } else {
                                    if (src1) {
                                        src1_regName = src1->getRegisterName();
                                        if(IS_FLOAT(type_src1)) 
                                            src1_regName = convertToInt(src1, os);
                                    }
                                    if (src2) {
                                        src2_regName = src2->getRegisterName();
                                        if(IS_FLOAT(type_src2))
                                            src2_regName= convertToInt(src2, os);
                                    }
                                    if (dst)
                                        dst_regName = dst->getRegisterName();
                                    
                                    switch(interCode->getsubCode()) {
                                        case OpNode::OpCode::PLUS  :   {  os<<"ADD ";    PRT_REG;          break; }
                                        case OpNode::OpCode::MINUS :   {  os<<"SUB ";    PRT_REG;          break; }
                                        case OpNode::OpCode::MULT  :   {  os<<"MUL ";    PRT_REG;          break; }    
                                        case OpNode::OpCode::MOD   :   {  os<<"MOD ";    PRT_REG;          break; }    
                                        case OpNode::OpCode::DIV   :   {  os<<"DIV ";    PRT_REG;          break; }
                                        case OpNode::OpCode::BITAND:   {  os<<"AND ";    PRT_REG;          break; }
                                        case OpNode::OpCode::BITOR :   {  os<<"OR " ;    PRT_REG;          break; }
                                        case OpNode::OpCode::BITXOR:   {  os<<"XOR ";    PRT_REG;          break; }
                                        case OpNode::OpCode::GT    :   {  os<<"JMPC GT " <<src1_regName<<" "<<src2_regName<<" ";    relational( dst_regName, os);    break; }
                                        case OpNode::OpCode::GE    :   {  os<<"JMPC GE " <<src1_regName<<" "<<src2_regName<<" ";    relational( dst_regName, os);    break; }
                                        case OpNode::OpCode::LT    :   {  os<<"JMPC LT " <<src1_regName<<" "<<src2_regName<<" ";    relational( dst_regName, os);    break; }
                                        case OpNode::OpCode::LE    :   {  os<<"JMPC LE " <<src1_regName<<" "<<src2_regName<<" ";    relational( dst_regName, os);    break; }
                                        case OpNode::OpCode::EQ    :   {  os<<"JMPC EQ " <<src1_regName<<" "<<src2_regName<<" ";    relational( dst_regName, os);    break; }
                                        case OpNode::OpCode::NE    :   {  os<<"JMPC NE " <<src1_regName<<" "<<src2_regName<<" ";    relational( dst_regName, os);    break; }
                                        case OpNode::OpCode::SHL   :   {  ShiftLogic(true, src1_regName, src2_regName, dst_regName, os);    break; } 
                                        case OpNode::OpCode::SHR   :   {  ShiftLogic(false, src1_regName, src2_regName, dst_regName, os);           break; }
                                        case OpNode::OpCode::AND   :   {  ANDLogic  (src1_regName, src2_regName, dst_regName, os);    break; } 
                                        case OpNode::OpCode::OR    :   {  ORLogic   (src1_regName, src2_regName, dst_regName, os);           break; }
                                        default                    :                  break;    
                                    }
                                }
                            } else if(opndsList[0] && opndsList[1]) {
                                ExprNode *src1 = opndsList[1];
                                Type   *type_src1 = src1->coercedType() ? (Type*)src1->coercedType() : src1->type();
                                bool IsEndlNeeded = true;

                                src1_regName = src1->getRegisterName();
                                if(dst)
                                    dst_regName = dst->getRegisterName();

                                if (IS_FLOAT(type_dst)) {
                                    if (interCode->getsubCode() == OpNode::OpCode::ASSIGN) {
                                        if (!IS_FLOAT(type_src1))
                                            src1_regName = convertToFloat(src1, os);
                                        os<<"MOVF "<<src1_regName<<" "<<dst_regName;

                                    } else if (interCode->getsubCode() == OpNode::OpCode::UMINUS) {
                                        if (!IS_FLOAT(type_src1)) 
                                            src1_regName = convertToFloat(src1, os);
                                        os<<"FNEG "<<src1_regName<<" "<<dst_regName;
                                    }
                                } else {
                                    if (interCode->getsubCode() == OpNode::OpCode::ASSIGN) {
                                        if (IS_STRING(type_src1)) {
                                            os<<"MOVS "<<src1_regName<<" "<<dst_regName;
                                        } else {
                                            if (IS_FLOAT(type_src1)) 
                                                src1_regName = convertToInt(src1, os);
                                            os<<"MOVI "<<src1_regName<<" "<<dst_regName;
                                        }
                                    } else if (interCode->getsubCode() == OpNode::OpCode::UMINUS) {
                                        if (IS_FLOAT(type_src1)) 
                                            src1_regName = convertToInt(src1, os);
                                        os<<"NEG "<<src1_regName<<" "<<dst_regName;
                                    } else if (interCode->getsubCode() ==  OpNode::OpCode::NOT) {
                                        NOTLogic(src1_regName, dst_regName, os);
                                        IsEndlNeeded = false;
                                    } else if (interCode->getsubCode() ==  OpNode::OpCode::BITNOT) {
                                        os << "BNOT " << src1_regName << " " << dst_regName;
                                    }

                                }
                                if (IsEndlNeeded) os<<endl;
                            }
                            break;
                        }
          case LABEL  : {
                            os<<interCode->getLabel()<<endl;
                            break; 
                        }
          case GOTO   : { 
                            InterCode* goto_lab = (InterCode*) opndsList[0];

                            os<<"JMP "<<goto_lab->getLabel() <<endl;
                            break; 
                        }
          case IFREL  : { 
                            ExprNode* cond        = (ExprNode*) opndsList[0];
                            InterCode* true_lab   = cond->OnTrue();
                            InterCode* false_lab  = cond->OnFalse();
                            ExprNode* expr1       = (ExprNode*) opndsList[1];
                            ExprNode* expr2       = (ExprNode*) opndsList[2];
                            bool if_cond_int      = false;

                            Type   *type_src1 = (expr1) ?  (expr1->coercedType() ? (Type*)expr1->coercedType() : expr1->type()): NULL ;
                            Type   *type_src2 = (expr2) ?  (expr2->coercedType() ? (Type*)expr2->coercedType() : expr2->type()): NULL ;

                            if (opndsList[2] && opndsList[1]) 
                            {

                                src1_regName = expr1->getRegisterName();
                                src2_regName = expr2->getRegisterName();

                                if(!IS_FLOAT(type_src1) && !IS_FLOAT(type_src2))
                                    if_cond_int  = true;
                                else
                                {
                                    if(!IS_FLOAT(type_src1)) 
                                        src1_regName = convertToFloat(expr1, os);
                                    if(!IS_FLOAT(type_src2))
                                        src2_regName= convertToFloat(expr2, os);
                                    if_cond_int = false;
                                }

                                os<<"JMPC ";
                                switch(interCode->getsubCode())
                                {
                                    case OpNode::OpCode::EQ   : (if_cond_int) ? os<<"EQ " : os<<"FEQ "; break; 
                                    case OpNode::OpCode::NE   : (if_cond_int) ? os<<"NE " : os<<"FNE "; break;
                                    case OpNode::OpCode::GT   : (if_cond_int) ? os<<"GT " : os<<"FGT "; break; 
                                    case OpNode::OpCode::GE   : (if_cond_int) ? os<<"GE " : os<<"FGE "; break;
                                    case OpNode::OpCode::LT   : (if_cond_int) ? os<<"LT " : os<<"FLT "; break; 
                                    case OpNode::OpCode::LE   : (if_cond_int) ? os<<"LE " : os<<"FLE "; break;
                                    default                   :  break ;    
                                }
                                os<<src1_regName<<" "<<src2_regName<<" "<<(true_lab->getLabel())<<endl;
                                if(false_lab)
                                    os<<"JMP "<<(false_lab->getLabel())<<endl;
                            }
                            else if(opndsList[1])
                            {
                                if(cond->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE)
                                        src1_regName = expr1->getRefName(); 
                                else
                                        src1_regName = expr1->getRegisterName();
                                
                                if(Type::isBool(type_src1->tag()))
                                {
                                    if(interCode->getsubCode() == OpNode::OpCode::NOT)
                                        os<<"JMPC EQ ";
                                    os<<src1_regName<<" 0 "<<(true_lab->getLabel())<<endl;
                                    if(false_lab)
                                        os<<"JMP "<<(false_lab->getLabel())<<endl;
                                }
                            }
                            else
                            {
                                if(cond->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE)
                                        src1_regName = cond->getRefName(); 
                                else
                                        src1_regName = cond->getRegisterName();
                                os<<"JMPC NE ";
                                os<<src1_regName<<" 0 "<<(true_lab->getLabel())<<endl;
                                if(false_lab)
                                    os<<"JMP "<<(false_lab->getLabel())<<endl;
                            }
                            break;
                        }
         case ENTER  :  { 
                            IsLastBlockEmpty = true;
                            fl_reg_used_cnt  = 0 ;
                            int_reg_used_cnt = 0 ;
                            break;
                        }
         case LEAVE  :  { 
                            os<<"ADD "<<RSP<<" 4 "<<RSP << endl;
                            os<<"LDI "<<RSP <<" "<<RRET_ADD << C_POP_RET << endl;
                            os << "JMPI " << RRET_ADD << endl;
                            break;
                        }
         case PRINT  :  {
                            ExprNode *param        = opndsList[0];
                            Type   *type_dst     = (param ) ?  (param ->coercedType() ? (Type*)param ->coercedType() : param ->type()): NULL ;

                            if (IS_FLOAT(type_dst))         os<<"PRTF ";
                            else if (IS_STRING(type_dst))   os<<"PRTS ";
                            else                            os<<"PRTI ";

                            if (param->exprNodeType() == ExprNode::ExprNodeType::VALUE_NODE)
                                os << opndsList[0]->getRefName();
                            else 
                                os << opndsList[0]->getRegisterName();
                            os << endl;
                            break;
                        }
                     
         default:       {
                            assert(0 && "Invalid 3 address Opcode");
                        }
    }
}

