#include "AbstractMachineCode.h"
#include "InterCode.h"
#include  <stack>

#define     IS_FLOAT(type)   Type::isFloat(type->tag())
#define     IS_INT(type)     Type::isInt  (type->tag())
#define     IS_STRING(type)  Type::isString  (type->tag())

static int float_reg_count = 10;
static int int_reg_count   = 20;
stack<ExprNode*>formal_param_list;

string allocateNewRegName(bool floatStatus)
{
   char newRegName[5];
    for(int i = 0 ; i < 5; i++)
        newRegName[i] = '\0';
   (floatStatus) ? sprintf(newRegName, "F%.3d", float_reg_count++) : sprintf(newRegName, "R%.3d", int_reg_count++);
    
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

void AbstractMachineCode::genAMC (BasicBlocksContainer *bbCls, ostream & os) {
    if (!bbCls)
        return;

    os <<"begin:" << endl;
    os <<"MOVI " << RSP << " " << 10000 << "    // RSP Initialized" << endl;
    os <<"JMP global" << endl << endl;
     
     map <string, BasicBlocksClass*>::iterator it = bbCls->getContainer()->begin();
     for (; it != bbCls->getContainer()->end(); ++it) {
          
          vector <BasicBlock*>* basicBlock     = (*it).second->getVector();      
          vector <BasicBlock*>::iterator iter1 = basicBlock->begin();

          for(; iter1 != basicBlock->end(); iter1++) {
                vector <InterCode*> ::iterator iter2  = (*iter1)->getICodeVector()->begin();
                os << (*iter1)->getBlockLabel() << ":" << endl;
                for(; iter2 != (*iter1)->getICodeVector()->end(); iter2++) {
                    convert_IC_AMC(*iter2, os);
                    os<<endl;
                }
                // Jump to event handling
                if ((*iter1)->getBlockLabel() == "global")
                    os << "JMP " << S_START;
                os << endl;
          }
    }
    os << endl << S_END << ":" << endl;
    os << "PRTS " << "\"DONE\"" << endl << endl;
}
    

void   AbstractMachineCode::convert_IC_AMC(InterCode *interCode, ostream &os)
{
    string  dst_regName ;
    string  src1_regName;
    string  src2_regName;
    ExprNode **opndsList = (ExprNode **)interCode->get3Operands();
        switch(interCode->getOPNType())
        {
              case CALL   :   {
                                   string retAddrReg =   allocateNewRegName(false); 
                                   os<<"STI "<<retAddrReg<<" "<<RSP <<endl;
                                   os<<"SUB "<<RSP<<" 4 "<<RSP << endl;
                                   os<<"JMP "<<((InvocationNode*)opndsList[0])->symTabEntry()->name()<<endl;
                                   
                                   if(opndsList[1])
                                   {
                                        ExprNode *src1    = opndsList[1];
                                        string retValueReg = src1->getRegisterName();
                                        if(IS_FLOAT(src1->type()))
                                            os<<"MOVF RRV_F ";
                                        else
                                            os<<"MOVI RRV_I ";
                                        os<<retValueReg<<endl;

                                   }
                                break;
                              }
              case FPARAM  :  {
                                   ExprNode *src1       = opndsList[0];
                                   string fparamReg;
                                   if(src1)
                                   {
                                     fparamReg = src1->getRegisterName();
                                   }
                                   os<<"ADD "<<RSP<<" 4 "<<RSP << endl;
                                   os<<"LDI "<<RSP<<" "<<fparamReg;
                                   break; 
                              } 
              case APARAM  :  {
                                ExprNode *param       = opndsList[0];
                                    string    str;
                                if(param)
                                    {
                                    str  = param->getRegisterName();
                                    if(IS_FLOAT(param->type()))
                                            os<<"STF "<<str<<" "<<RSP;
                                    else if(IS_INT(param->type()))
                                            os<<"STI "<<str<<" "<<RSP;
                                    os<<endl;
                                    os<<"SUB "<<RSP<<" 4 "<<RSP;
                                    os<<endl;
                                    }
                                break;
                              }
              case RETURN :   {
                                ExprNode *ret_val    = opndsList[0];
                                Type     *ret_type   = (ret_val) ?  (ret_val ->coercedType() ? (Type*)ret_val ->coercedType() : ret_val ->type()): NULL ;
                                
                                if(IS_FLOAT(ret_type))
                                   {
                                       dst_regName = ret_val->getRegisterName();
                                       os<<"MOVF "<<dst_regName<<" "<<RRV_F;
                                   }
                                else
                                    {
                                        dst_regName = ret_val->getRegisterName();
                                        os<<"MOVI "<<dst_regName<<" "<<RRV_I;
                                    }
                                 
                                
                              break;
                              }
              case EXPR   :   {
//TODO: To bit and logical operators
                                ExprNode *dst        = opndsList[0];
                                ExprNode *src1       = opndsList[1];
                                ExprNode *src2       = opndsList[2];
                                if(dst)
                                    dst_regName = dst->getRegisterName();
                               
                               Type   *type_dst  = (dst ) ?  (dst ->coercedType() ? (Type*)dst ->coercedType() : dst ->type()): NULL ;
                               Type   *type_src1 = (src1) ?  (src1->coercedType() ? (Type*)src1->coercedType() : src1->type()): NULL ;
                               Type   *type_src2 = (src2) ?  (src2->coercedType() ? (Type*)src2->coercedType() : src2->type()): NULL ;
            
                               if(IS_FLOAT(type_dst))
                               {
                               if(src1)
                               {
                                 src1_regName = src1->getRegisterName();
                                 if(!IS_FLOAT(type_src1)) 
                                 {
                                     src1_regName = convertToFloat(src1, os);
                                 }
                               }
                               if(src2)
                               {
                                 src2_regName = src2->getRegisterName();
                                 if(!IS_FLOAT(type_src2))
                               {
                                    src2_regName= convertToFloat(src2, os);
                               }
                               }
                               
                               switch(interCode->getsubCode())
                               {
                                   case OpNode::OpCode::PLUS :     os<<"FADD "; break; 
                                   case OpNode::OpCode::MINUS:     os<<"FSUB "; break;
                                   case OpNode::OpCode::MULT :     os<<"FMUL "; break;     
                                   case OpNode::OpCode::DIV  :     os<<"FDIV "; break;
                                   case OpNode::OpCode::ASSIGN:    os<<"MOVF "; break; 
                                   default                    :  break ;    
                               }
                               os<<src1_regName<<" "<<src2_regName<<" "<<dst_regName;
                               }
                               else
                               {
                               bool isSHL = false; 
                               if(src1)
                               {
                                 src1_regName = src1->getRegisterName();
                                 if(IS_FLOAT(type_src1)) 
                                       src1_regName = convertToInt(src1, os);
                               }
                               if(src2)
                               {
                                 src2_regName = src2->getRegisterName();
                                 if(IS_FLOAT(type_src2)) 
                                       src2_regName = convertToInt(src2, os);
                               }
                               if(interCode->getsubCode() != OpNode::OpCode::SHL && interCode->getsubCode() != OpNode::OpCode::SHR)
                               {
                               switch(interCode->getsubCode())
                               {
                                   case OpNode::OpCode::PLUS :     os<<"ADD "; break; 
                                   case OpNode::OpCode::MINUS:     os<<"SUB "; break;
                                   case OpNode::OpCode::MULT :     os<<"MUL "; break;     
                                   case OpNode::OpCode::DIV  :     os<<"DIV "; break;
                                   case OpNode::OpCode::ASSIGN:    os<<"MOVI "; break; 
                    //TODO:        case OpNode::OpCode::NOT  :     cout<<"\n hi";  
                                  //                                 break;  
                                   default                    :  break ;    

                               }
                               os<<src1_regName<<" "<<src2_regName<<" "<<dst_regName;
                               }
                               else if(interCode->getsubCode() == OpNode::OpCode::SHL || interCode->getsubCode() == OpNode::OpCode::SHR)
                               {
                                   if(interCode->getsubCode() == OpNode::OpCode::SHL)   isSHL = true;
                                   else if(interCode->getsubCode() == OpNode::OpCode::SHL)   isSHL = false;

                                                                   LabelClass *l1  = new LabelClass();
                                                                   InterCode  *ic1 = l1->assignLabel();
                                                                   LabelClass *l2  = new LabelClass();
                                                                   InterCode  *ic2 = l2->assignLabel();

                                                                   os<<"MOVI "<<src1_regName<<" "<<RSH_Val<<endl;
                                                                   os<<"MOVI "<<src2_regName<<" "<<RSH_Cnt<<endl;
                                                                   os<<ic2->getLabel()<<": ";
                                                                   os<<"JMPC GT 0 "<<RSH_Cnt<<" "<<ic1->getLabel()<<endl;
                                                                   if(isSHL)
                                                                       os<<"MUL "<<RSH_Val<<" 2 "<<RSH_Val<<endl;
                                                                   else
                                                                       os<<"DIV "<<RSH_Val<<" 2 "<<RSH_Val<<endl;
                                                                   os<<"SUB "<<RSH_Cnt<<" 1 "<<RSH_Cnt<<endl;
                                                                   os<<"JMP "<<ic2->getLabel()<<endl;
                                                                   os<<ic1->getLabel()<<":"<<endl;
                                                                   os<<"MOVI "<<RSH_Val<<" "<<dst_regName<<endl;
                                                                   break;

                               }

                               }
                               break;
                               }
             case LABEL  :     {
                                cout<<"\n in label "<<interCode->getLabel();
                                os<<interCode->getLabel()<<endl;
                                break; }
             case GOTO   :     { 
                                // cout<<"\n in goto"; 
                                os<<"JMP "<<interCode->getLabel() <<endl;
                                break; }
             case IFREL  :   { 
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
                                    case OpNode::OpCode::LT   : (if_cond_int) ? os<<"GT " : os<<"FGT "; break; 
                                    case OpNode::OpCode::LE   : (if_cond_int) ? os<<"GE " : os<<"FGE "; break;
                                    default                   :  break ;    
                                }
                                if(interCode->getsubCode() == OpNode::OpCode::LT  || interCode->getsubCode() == OpNode::OpCode::LE)
                                {
                                    string temp  = src1_regName;
                                    src1_regName = src2_regName;
                                    src2_regName = temp;
                                }
                                os<<src1_regName<<" "<<src2_regName<<" "<<(true_lab->getLabel())<<endl;
                                if(false_lab)
                                    os<<"JMP "<<(false_lab->getLabel())<<endl;
                               }
                               else if(opndsList[1])
                               {
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
                                    os<<"JMPC NE ";
                                os<<src1_regName<<" 0 "<<(true_lab->getLabel())<<endl;
                                if(false_lab)
                                    os<<"JMP "<<(false_lab->getLabel())<<endl;
                               }
                               break;
                             }
             case ENTER  :   { 
//                               string retAddrReg =   allocateNewRegName(false); 
                               os<<"ADD "<<RSP<<" 4 "<<RSP << endl;
                               os<<"LDI "<<RSP <<" "<<RRET_ADD;
                               break;
                             }
             case LEAVE  :   { 
                               //os << "leave " << ((FunctionEntry*)opndsList[0])->name();
                               os << "JMP " <<RRET_ADD;
                              // os<<(char*)opndsList[0];
                             break;
                             }
             case PRINT  :   { cout<<"\n in print";break;}
                         
             default:       { cout<<"\n in default"; }
      }
}
