#ifndef INTERCODE
#define INTERCODE

#include "STEClasses.h"
#include "Value.h"
#include "ParserUtil.h"
#include "all.h"
#include "Ast.h"

using namespace std;

/* Each InterCode class will store single three address Instruction 
 * opnds_ array is for three operands
 * OPNTYPE gives which type of instruction is this
 */
class InterCode {
    public:
       enum OPNTYPE {CALL, PARAM, RETURN, EXPR, LABEL, GOTO, IFREL, ENTER, LEAVE, PRINT};
       
       InterCode (OPNTYPE op, OpNode::OpCode subopc = OpNode::OpCode::INVALID, 
                   void *a = NULL, void *b  = NULL, void *c = NULL) {
            optype_   = op;
            subCode_  = subopc;
            opnds_[0] = a;
            opnds_[1] = b;
            opnds_[2] = c;
       }
     
       void print(ostream &os);
       void** get3Operands() { return opnds_; }
       void set3Operands(void *opnd[3]) { *opnds_ = opnd; }
       OPNTYPE getOPNType() { return optype_; }
       OpNode::OpCode getsubCode() { return subCode_; }
       void setSubCode(OpNode::OpCode opCode) { subCode_ = opCode; }
    
       string getLabel() { 
           if (optype_ != LABEL) return "";
//           int id = (long)opnds_[0];
           string *name = (string *) opnds_[1];
           //if (name && name->length())
           return *name;
           /*else {
                ostringstream os;
                os << "L" << id;
                return os.str();
            }*/
        }    
        void xchgSubcode() { 
            
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
                        cout<<"Cannot negate the subcode_";
                        break;
            }
        }
        
        protected:
        void *opnds_[3]; 
        OpNode::OpCode subCode_; 
        OPNTYPE optype_;  
    
};

/* Stores vector of all three address instructions 
 * Will be used for breaking basic blocks based on OPNTYPE label
 */
class InterCodesClass {
    public:
        InterCodesClass() {}
        virtual ~InterCodesClass() {}
        void print   (ostream &os);
        void addCode (InterCode *code);
        void addCode (InterCodesClass* code);
        void addCode (InterCode::OPNTYPE op, void *a = NULL, 
               void *b  = NULL, void *c = NULL, 
                OpNode::OpCode subopc = OpNode::OpCode::INVALID);
       
        vector<InterCode*>* getICodeVector() {
                return &InterCodeVector;
        }

        void setICodeVector(vector <InterCode*> *tempVector) {
                InterCodeVector = *tempVector;
        }
        
    protected:
        vector <InterCode*> InterCodeVector;       
};

class LabelClass {
    static long labelCount;
    public:
        friend class InterCodesClass; 
        long getLabelCount() { return labelCount; } 
        void setLabelCount (long c) { labelCount = c; }
        
        static InterCode* assignLabel (string name="") {
            InterCode* ic = new InterCode (InterCode::OPNTYPE::LABEL, OpNode::OpCode::INVALID, 
                                          (void *)(labelCount++), (void *)(new string(name)));
            return ic; 
        }
};


#endif
