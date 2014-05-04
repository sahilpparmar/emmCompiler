#include "InterCode.h"
#define TAB_SPACE 4

long LabelClass::labelCount = 0;

void InterCode::print(ostream &os) {
    ExprNode** op = (ExprNode**)opnds_;

    //os << "NEXT\n";
    switch (optype_) {
        case EXPR:  {
                        switch (subCode_) {
                            default: 
                                if (op[0] && op[1] && op[2]) {
                                    prtSpace(os, TAB_SPACE);
                                    os << op[0]->getRefName() << " = " << op[1]->getRefName();
                                    os << " " << OpNode::opInfo[(int )subCode_].name_ << " ";
                                    os << op[2]->getRefName() << ";";
                                }
                                break;

                            case OpNode::OpCode::BITNOT:
                            case OpNode::OpCode::UMINUS:
                                if (op[0] && op[1]) {
                                    prtSpace(os, TAB_SPACE);
                                    os << op[0]->getRefName() << " = ";
                                    os << OpNode::opInfo[(int )subCode_].name_;
                                    os << op[2]->getRefName() << ";";
                                }
                                break;

                            case OpNode::OpCode::ASSIGN:  
                                prtSpace(os, TAB_SPACE);
                                os << op[0]->getRefName() << " = " << op[1]->getRefName() << ";";
                                break;
                        }
                    }                
                    break;

        case GOTO : {
                        if (op[0]) {
                            prtSpace(os, TAB_SPACE);
                            os << "GOTO "; 
                            os << "L" << (long )op[0] << ";";
                        }
                    }
                    break; 

        case LABEL: {
                        os << endl;
                        string *n = (string *)op[1];
                        if (n && n->length()) { 
                            os << *n << ":";
                        } else {
                            os << "L" << (long )op[0] << ":";
                        } 
                    }
                     break;

        case IFREL: {
                        if (op[0] && op[1] && op[2]) {
                            prtSpace(os, TAB_SPACE);
                            os << "IF " << op[1]->getRefName() << " " << OpNode::opInfo[(int )subCode_].name_;
                            os << " " << op[2]->getRefName() << " GOTO "<< op[0] << ";";
                        }
                    }
                     break; 

        case ENTER: {
                        os << "Enter " << (char*)op[0];
                    }
                    break;

        case LEAVE: {
                        os << "Leave " << (char*)op[0];
                    }
                    break;

        default:;
            //assert(0 && "Unsupported 3AddrCode");
    }
}

void InterCodesClass::addCode (InterCode *code) {
    if (code != NULL) 
        InterCodeList.push_back(code);  
}

void InterCodesClass::addCode (InterCodesClass* code) {
    if (code != NULL) {
        vector <InterCode*> code_l = code->InterCodeList;
        for (int i = 0; i < (int )code_l.size(); ++i)
            InterCodeList.push_back(code_l.at(i));
    }
}

void InterCodesClass::addCode (InterCode::OPNTYPE op, void *a, 
               void *b, void *c, OpNode::OpCode subopc) { 

    InterCodeList.push_back(new InterCode (op, subopc, a, b, c));     
}

void InterCodesClass::print (ostream &os) {
    vector <InterCode*>::iterator it =  InterCodeList.begin();
    for (; it != InterCodeList.end(); ++it)  {
        (*it)->print(os);
        os << endl;
    }

}
