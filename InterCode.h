/* Each InterCode class will store single three address Instruction in Quadruple form
 * opnds_ array is for three operands
 * OPNTYPE gives which type of instruction is this
 * subCode_ is to distinguish expr type operand
 * 
 * InterCodesClass     - vector to store all InterCodes in a program
 * BasicBlock          - Divide InterCodesvec into basicblocks 
 * BasicBlocksClass    - Vector of all basic blocks within one function reachable from one another 
 * BasicBlocksContainer- GLobal BasicBlocks structure - vector of all basicblocksclass
 */

#ifndef INTERCODE
#define INTERCODE

#include "STEClasses.h"
#include "Value.h"
#include "ParserUtil.h"
#include "all.h"
#include "Ast.h"
#include <map>
#include <utility>

using namespace std;

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
        
        void addPrevBlock(string prevlabel) {
           vector<string>::iterator it = PrevBlockLabels.begin();
           for(; it != PrevBlockLabels.end(); ++it) {
               if (prevlabel.compare(*it) == 0)
                   return;
           }
           PrevBlockLabels.push_back(prevlabel);
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
            
            os << "\n\nPrevBlocks: ";
            vector<string>::iterator iter = PrevBlockLabels.begin();
            for ( ; iter != PrevBlockLabels.end(); ++iter) {
                os << *iter << ",";
            }
            os << endl; 
            os << "Block Start: " << blocklabel << endl;
            vector <InterCode*>::iterator it = InterCodeVector.begin();

            for (; it != InterCodeVector.end(); ++it) {
                (*it)->print(os);
                os << endl;
            }
        
            os << "Block end: " << blocklabel << endl ;
            os << "NextBlocks:";  
            iter = NextBlockLabels.begin();
            for ( ; iter != NextBlockLabels.end(); ++iter) {
                os << *iter << ",";
            }
        }
        
        void constantFolding();
        void constantPropogation();
    private : 
        string blocklabel; 
        vector <InterCode*> InterCodeVector;       
        vector <string> NextBlockLabels;
        vector <string> PrevBlockLabels;
};


class BasicBlocksClass {
    private:
        vector <BasicBlock*> bbVector;
        map <string, BasicBlock*> label_block_map; 
    public:
        
        vector<BasicBlock*>* getVector() {
            return &bbVector;
        }

        vector<BasicBlock*> getBasicBlock() {
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
            label_block_map.insert(pair<string, BasicBlock*> (label, newbb));
            return newbb;
        }
        
        void createBlocks (InterCodesClass* ic);
        
        void constantOptimize () {
           vector <BasicBlock*>::iterator it; 
            
            //TODO : Need to convert this to iterative 
            for (it = bbVector.begin(); it != bbVector.end(); ++it) {
                (*it)->constantFolding();
                (*it)->constantPropogation();
            }
        }
        
        void print(ostream &os) {
             vector <BasicBlock*>::iterator it = bbVector.begin();
             for (; it != bbVector.end(); ++it) {
                 (*it)->print(os);
             }
        }

};

class BasicBlocksContainer {
    private: 
        map <string, BasicBlocksClass*> bbContainer; 
        
    public:
       void createBlockStruct (InterCodesClass* ic);
       map <string, BasicBlocksClass*>* getContainer() {
           return &bbContainer;
       }        

       BasicBlocksClass* insertInContainer (string name) {

           if (bbContainer.find(name) == bbContainer.end()) {
               BasicBlocksClass* bbCls = new BasicBlocksClass(); 
               bbContainer.insert (pair<string, BasicBlocksClass*> (name, bbCls));
               
               return bbCls;
           } else {
               return bbContainer.find(name)->second;
           }
       }
    
       void optimize() {
             map <string, BasicBlocksClass*>::iterator it = bbContainer.begin();
             for (; it != bbContainer.end(); ++it) {
                 (*it).second->constantOptimize();
             }
       }
        
       void print(ostream &os) {
            map <string, BasicBlocksClass*>::iterator it = bbContainer.begin();
            for (; it != bbContainer.end(); ++it) {
                os << "\n====Basic Blocks Container: " << (*it).first << "=====" << endl;
                (*it).second->print(os);
                os << endl;
            }
       }
};


#endif
