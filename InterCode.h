#ifndef INTERCODE
#define INTERCODE

#include "STEClasses.h"
#include "Value.h"
#include "ParserUtil.h"
#include "all.h"
#include "Ast.h"
#include <map>
#include <utility>

#define TAB_SPACE 8

using namespace std;

/* Each InterCode class will store single three address Instruction 
 * opnds_ array is for three operands
 * OPNTYPE gives which type of instruction is this
 */
class InterCode {
    public:
       enum OPNTYPE {CALL, FPARAM, APARAM, RETURN, EXPR, LABEL, GOTO, IFREL, ENTER, LEAVE, PRINT};
       
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
        
        void ifThenElseOpt();
        void removeContLabelGoto();

        void optimize () {
                ifThenElseOpt();
                removeContLabelGoto();
        }
        
    protected:
        vector <InterCode*> InterCodeVector;       
};


class LabelClass {
    static long labelCount;
    static map <string, InterCode*> label_interCode_map;  
    
    public:
        friend class InterCodesClass; 
        static void insertInMap(string str, InterCode* ic) {
           label_interCode_map.insert(pair<string, InterCode*>(str, ic));
        }
        
        long getLabelCount() { return labelCount; } 
        void setLabelCount (long c) { labelCount = c; }
        
        static InterCode* assignLabel (string name="") {
            InterCode* ic = new InterCode (InterCode::OPNTYPE::LABEL, OpNode::OpCode::INVALID, 
                                          (void *)(labelCount), (void *)(new string(name)));
             
            string str; 
            if (name.length() == 0) {
                str = "L";
                str += itoa(labelCount);
            } else {
                str = name;
            }

            insertInMap(str, ic);    
            labelCount++;
            return ic; 
        }
};



class BasicBlock {
    public : 
        BasicBlock (string label) {
           blocklabel.assign(label);
        }
        
        vector<InterCode*>* getICodeVector() {
                return &InterCodeVector;
        }
        
        void setICodeVector(vector<InterCode*>* vec) {
            InterCodeVector.erase(InterCodeVector.begin(), InterCodeVector.end());
            InterCodeVector.assign(vec->begin(), vec->end());
        }
        
        
        void addNextBlock(string nextlabel) {
           vector<string>::iterator it = NextBlockLabels.begin();
           for(; it != NextBlockLabels.end(); ++it) {
               if (nextlabel.compare(*it) == 0)
                   return;
           }
           NextBlockLabels.push_back(nextlabel);
        }

        void setBlockLabel(string s) {
            blocklabel.assign(s);
        }

        string getBlockLabel() {
            return blocklabel;
        }
        
        void addCode (InterCode* cs) {
            if (cs) InterCodeVector.push_back (cs);
        }
       

        void print(ostream &os) {
            //os << "Block Label: " << blocklabel << endl;
            os << blocklabel << ":" << endl;
            vector <InterCode*>::iterator it = InterCodeVector.begin();

            for (; it != InterCodeVector.end(); ++it) {
                (*it)->print(os);
                os << endl;
            }
        
            prtSpace(os, TAB_SPACE);
            os << "(next ";  
            vector<string>::iterator iter = NextBlockLabels.begin();
            
            for ( ; iter != NextBlockLabels.end(); ++iter) {
                os << " " << *iter;
            }
            os << ")";  
            os << endl << endl; 
        }
        
        void check() {
            vector<InterCode*>::iterator it = InterCodeVector.begin(); 
            
            for (; it != InterCodeVector.end(); ++it) {

            if ((*it)->getOPNType() == InterCode::OPNTYPE::EXPR ) { 
               ExprNode **op =  (ExprNode **)(*it)->get3Operands();
                for (int i = 0; i < 1; ++i) { 
                    if (op[i] && op[i]->coercedType())
                        cout <<"\n---------"<< op[i]->coercedType()->name();
                    else if (op[i] && op[i]->type())
                        cout << endl<<op[i]->type()->name();
                }
            }
            }
        }
        void constantFolding();
        void constantPropogation();
        void redundantGotoRemoval();
        void zeroRemoval();

    private : 
        string blocklabel; 
        vector <InterCode*> InterCodeVector;       
        vector <string> NextBlockLabels;
};


class BasicBlocksClass {
    private:
        vector <BasicBlock*> bbVector;
    
    public:
        vector<BasicBlock*> getBasicBlock()
        {
            return bbVector;
        }

        BasicBlock* getBlockWithLabel (string label) {
           vector <BasicBlock*>::iterator it = bbVector.begin();

            for (; it != bbVector.end(); ++it) {
                if (label.compare((*it)->getBlockLabel()) == 0)
                    return (*it);
            }
            //if block does not exist,then create block with new label
            BasicBlock *newbb = new BasicBlock(label);
            bbVector.push_back(newbb);
            return newbb;
        }
        
        void createBlocks (InterCodesClass* ic);
        
        void blockOptimize () {
           vector <BasicBlock*>::iterator it; 
            
            //TODO : Need to convert this to iterative 
            for (it = bbVector.begin(); it != bbVector.end(); ++it) {
                (*it)->constantFolding();
                (*it)->constantPropogation();
                (*it)->redundantGotoRemoval();
                (*it)->zeroRemoval();
            }
        }
        
        void print(ostream &os) {
             vector <BasicBlock*>::iterator it = bbVector.begin();
             for (; it != bbVector.end(); ++it) {
                 (*it)->print(os);
             }
        }
        void check() {
             cout << "DEBUG ON"; 
             vector <BasicBlock*>::iterator it = bbVector.begin();
             for (; it != bbVector.end(); ++it) {
                 (*it)->check();
             }
        }

};

#endif
