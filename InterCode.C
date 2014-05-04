#include "InterCode.h"
#define TAB_SPACE 8

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
                                    os << op[2]->getRefName();
                                }
                                break;

                            case OpNode::OpCode::BITNOT:
                            case OpNode::OpCode::UMINUS:
                                if (op[0] && op[1]) {
                                    prtSpace(os, TAB_SPACE);
                                    os << op[0]->getRefName() << " = ";
                                    os << OpNode::opInfo[(int )subCode_].name_;
                                    os << op[2]->getRefName();
                                }
                                break;

                            case OpNode::OpCode::ASSIGN:  
                                prtSpace(os, TAB_SPACE);
                                os << op[0]->getRefName() << " = " << op[1]->getRefName();
                                break;
                        }
                    }                
                    break;

        case GOTO:  {
                        InterCode* goto_lab = (InterCode*) op[0];
                        prtSpace(os, TAB_SPACE);
                        os << "goto "; 
                        goto_lab->print(os);
                    }
                    break; 

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
                        os << endl;
                        prtSpace(os, TAB_SPACE);
                        os << "goto ";
                        false_lab->print(os);
                    }
                    break; 

        case ENTER: {
                        prtSpace(os, TAB_SPACE);
                        os << "enter " << (char*)op[0];
                    }
                    break;

        case LEAVE: {
                        prtSpace(os, TAB_SPACE);
                        os << "leave " << (char*)op[0];
                    }
                    break;

        case CALL:  {
                        prtSpace(os, TAB_SPACE);
                        os << "call " << (char*)op[0];
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

        case PARAM: {
                        prtSpace(os, TAB_SPACE);
                        os << "param " << op[0]->getRefName();
                    }
                    break;
        default:
            assert(0 && "Unsupported 3AddrCode");
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
