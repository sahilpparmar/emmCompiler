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
#include <set>

#define TAB_SPACE 8

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
    
       bool xchgSubcode();
       string getLabel();
        
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
        
        void ifThenElseOpt(int *isOptimized);
        void removeContLabelGoto(int *isOptimized);
        void createLabelDUChain();
        void insertMap(string str, InterCode* ic);

        void ioptimize () {
            int isOptimized = 0; 
            do {
                    isOptimized = 0;
                    ifThenElseOpt(&isOptimized);
                    removeContLabelGoto(&isOptimized);
            } while (isOptimized);
            createLabelDUChain();
            //printMap();
        }

        void printMap();

    protected:
        vector <InterCode*> InterCodeVector;       
        map <string, vector <InterCode*>*> labelUsageMap;  
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
       
        vector <string>* getPrevBlockLabels() {
            return &PrevBlockLabels;
        }

        void print(ostream &os);
        
        void constantFolding(int *isOptimized);
        void constantPropogation(int *isOptimized);
        void redundantGotoRemoval(int *isOptimized);
        void zeroRemoval(int *isOptimized);

        set <string> EndLiveVars, StartLiveVars;
    private : 
        string blocklabel; 
        vector <InterCode*> InterCodeVector;       
        vector <string> NextBlockLabels, PrevBlockLabels;
};


class BasicBlocksClass {
    private:
        vector <BasicBlock*> bbVector;
        map <string, BasicBlock*> label_block_map; 
    public:
        
        vector<string> ordervec;
        
        map <string, BasicBlock*>* getLabelMap() {
            return &label_block_map;
        }
       
        void setVector (vector <BasicBlock *> &vec) {
            bbVector = vec;
        }
        
        vector<BasicBlock*>* getVector() {
            return &bbVector;
        }

        vector<BasicBlock*> getBasicBlock() {
            return bbVector;
        }

        BasicBlock* getBlockWithLabel (string);

        
        void createBlocks (InterCodesClass* ic);
        
        void blockOptimize () {
           vector <BasicBlock*>::iterator it;
            int isOptimized = 0; 
            do {
                 isOptimized = 0;
                 for (it = bbVector.begin(); it != bbVector.end(); ++it) {
                     (*it)->constantFolding (&isOptimized);
                     (*it)->constantPropogation (&isOptimized);
                     (*it)->redundantGotoRemoval(&isOptimized);
                     (*it)->zeroRemoval(&isOptimized);
                 }
            } while (isOptimized);
        }
        
        void liveVariableAnalysis();
        void commonSubExprElimination();
        
        void print(ostream &os) {
             vector <BasicBlock*>::iterator it = bbVector.begin();
             for (; it != bbVector.end(); ++it) {
                    (*it)->print(os);
             }
        }

};

class BasicBlocksContainer {
    private: 
        set <string> bbUsedContainers;        
        map <string, BasicBlocksClass*> bbContainer; 
    public:
       void createBlockStruct (InterCodesClass* ic);
       void insertInUsedList (string str)   {
            bbUsedContainers.insert(str);
       }

       set <string>* getUsedContainers() {
           return &bbUsedContainers;
       }        
       
       void printUsedContainers() {
           
           set <string>::iterator it;
           cout << "\n Called Blocks :";
           for (it = bbUsedContainers.begin(); it != bbUsedContainers.end(); ++it) {
               //no need of live var analysis for global 
               cout << (*it) << "\t";
           }
           cout << endl;
       }


       map <string, BasicBlocksClass*>* getContainer() {
           return &bbContainer;
       }        
       
       void removeBlocks();
       BasicBlocksClass* insertInContainer (string);
       void optimize();
       void print(ostream &os);
       
};


#endif
