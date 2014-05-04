#ifndef AST_H
#define AST_H

#include "all.h"
#include "Value.h"
#include "ProgramElem.h"
#include "SymTabEntry.h"

class BlockEntry;
class InterCodesClass;
class InterCode;
class EventEntry;
class FunctionEntry;
class LabelEntry;
class OpNode;
//enum class OpCode;
class PatNode;
//enum class PatNode;
class PrimitivePatNode;
class RulezNode;
class SymTabEntry;
class VariableEntry;

/*****************************************************************************
   Here is the class hierarchy:
                                               ProgramElem
				                  |
                                                AstNode
     +--------------------------------------------+----------------+
     |		     |                 |                           | 
 BasePatNode      ExprNode          RuleNode                    StmtNode
     |               |                                             |
     |               |                                             |
     |      +---------+----------+-----------+                     |
     |      |         |          |           |                     |
     |  RefExprNode  OpNode  ValueNode  InvocationNode             |
     |                                                             |
     |                                      +---------------+------+-----+
     |                                      |               |            |
     |                                 ExprStmtNode   CompoundStmtNode IfNode
     |
     |
   +-+------------------+
   |                    |
   |                    |
PrimitivePatNode    PatNode

******************************************************************************/
    
class AstNode: public ProgramElem {
    public:
        enum class NodeType  {
            PAT_NODE,
            EXPR_NODE, 
            REACTION_NODE, 
            STMT_NODE, 
            SEQUENCE_NODE, 
            RULE_NODE
        };

    public: 
        AstNode(NodeType nt, int line=0, int column=0, string file="");
        AstNode(const AstNode&); // copy constructor 
        virtual ~AstNode() {};
        //virtual AstNode* clone() = 0;

        NodeType nodeType() const { return nodeType_;}

        virtual const Type* typeCheck() = 0;
        virtual void print(ostream& os, int indent=0) const = 0;

        virtual void renameRV(string prefix) {}; // new names start with given prefix
        virtual bool operator==(const AstNode&) const { return false; };
        virtual bool operator!=(const AstNode& a) const 
        { return !operator==(a); };

    private: 
        NodeType nodeType_;
        const AstNode* operator=(const AstNode& other); /* disable asg */
};

inline ostream& operator<<(ostream& os, const AstNode& an) {
    an.print(os);
    return os;
};

/****************************************************************/
class ExprNode: public AstNode {
    public:
        enum class ExprNodeType   {
            REF_EXPR_NODE, 
            OP_NODE, 
            VALUE_NODE, 
            INV_NODE,
            VREG_NODE,
            PRINT_NODE
        };

    public:
        ExprNode(ExprNodeType et, const Value* val=0, int line=0, int column=0,
                string file=""); // val is saved, but not copied
        ExprNode(const ExprNode&);
        virtual ~ExprNode() {};

        virtual ExprNode* clone() const=0;

        ExprNodeType exprNodeType() const { return exprType_;};
        void exprNodeType(ExprNodeType t) { exprType_ = t; };

        const Value* value() const { return val_; }

        const Type* coercedType() const { return coercedType_; }
        void coercedType(const Type* type) { coercedType_ = type; }

        void print(ostream& os, int indent=0) const=0;
        
        virtual ExprNode* getRefNode ();
    private:
        ExprNodeType exprType_;
        const Value *val_; // reference semantics for val_ and coercedType_
        const Type* coercedType_; 
};

/****************************************************************/
class RefExprNode: public ExprNode {
    public:
        RefExprNode(string ext, const SymTabEntry* ste=NULL, 
                int line=0, int column=0, string file="");
        RefExprNode(const RefExprNode&);
        ExprNode* clone() const { return new RefExprNode(*this); }

        ~RefExprNode() {};

        string ext() const { return ext_;};
        void ext(string str) { ext_ = str;}; 

        const SymTabEntry* symTabEntry() const { return sym_;};
        void symTabEntry(const SymTabEntry *ste)  { sym_ = ste;};

        void print(ostream& os, int indent=0) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  
        string getRefName() { return ext_; }
    private:
        string ext_;
        const SymTabEntry* sym_;
};

/****************************************************************/

#define MAX_OP_ARITY 2

class OpNode: public ExprNode {
    public:
        enum class OpCode {
            UMINUS, PLUS, MINUS, MULT, DIV, MOD, 
            EQ, NE, GT, LT, GE, LE,
            AND, OR, NOT, 
            BITNOT, BITAND, BITOR, BITXOR, SHL, SHR,
            ASSIGN, PRINT, INVALID
        };

        enum class OpPrintType {PREFIX, INFIX, POSTFIX};
        struct OpInfo {
            OpCode code_;
            const char* name_;
            int arity_;
            int needParen_;
            OpPrintType prtType_;
            Type::TypeTag argType_[3]; 
            // operators with > 2 args can be supported
            // only if types of args k through N are identical, for 1 <= k <= 3, 
            // and are given by argType[k-1]
            Type::TypeTag outType_;
            const char *typeConstraints_;
        };

    public:
        static const int VARIABLE = 100;
        static const OpInfo opInfo[];
    public:
        OpNode(OpCode op, ExprNode *l, ExprNode *r=NULL,
                int line=0, int column=0, string file="");
        OpNode(const OpNode&);
        ExprNode* clone() const { return new OpNode(*this); }
        ~OpNode() {};

        OpCode opCode() const { return opCode_;};
        const ExprNode* arg(unsigned int i) const 
        { return (i < arg_.size())? arg_[i] : NULL; };
        unsigned int arity() const { return arity_; }

        void opCode(OpCode a) { opCode_ = a; };
        ExprNode* arg(unsigned int i) 
        { return (i < arg_.size())? arg_[i] : NULL; };
        vector<ExprNode*>* args() 
        { return &arg_; }

        void print(ostream& os, int indent=0) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  
    private: 
        unsigned int arity_;
        OpCode   opCode_;
        vector<ExprNode*> arg_;
};

/****************************************************************/

class ValueNode: public ExprNode {
    public:
        ValueNode(Value* val=0, int line=0, int column=0, string file="")
            : ExprNode(ExprNode::ExprNodeType::VALUE_NODE, val, line,column,file) {
                if (val != NULL) type((Type*)(val->type()));
                refnode_ = this;
            }
        ValueNode(const ValueNode& val): ExprNode(val) {};
        ExprNode* clone() const { return new ValueNode(*this); }
        ~ValueNode() {};

        void print(ostream& os, int indent=0) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  

        string getRefName() {

            ostringstream os;
            Type* tp = (Type*)coercedType();
            if (tp==NULL) {
                tp = type();
            }
            if (Type::isBool(tp->tag())) {
                os << value()->bval();
            } else {
                if (Type::isFloat(tp->tag())) {
                    os.precision(1);
                    os.setf(ios::fixed,ios::floatfield);
                    os << value()->dval();
                }
                else {
                    value()->print(os,0);
                }
            }
            return os.str();
        }

    private:
        /* val_ field is already included in ExprNode, so no new data members */
};

/****************************************************************/

class InvocationNode: public ExprNode {
    // Used to represent function invocation
    public:
        InvocationNode(const SymTabEntry *ste, vector<ExprNode*>* param=0, 
                int line=0, int column=0, string file="");
        InvocationNode(const InvocationNode&);
        ExprNode* clone() const { return new InvocationNode(*this); }
        ~InvocationNode() {};

        const SymTabEntry* symTabEntry() const { return ste_; };

        const vector<ExprNode*>* params() const { return params_;};

        vector<ExprNode*>* params() { return params_;}

        void params(vector<ExprNode*>* args) { params_ = args;};

        const ExprNode* param(unsigned int i) const {
            return (params_ != NULL && i < params_->size()) ? (const ExprNode*)((*params_)[i]) : NULL; 
        }

        ExprNode* param(unsigned int i) {
            return (ExprNode*)((const InvocationNode*)this->param(i));
        }

        void param(ExprNode* arg, unsigned int i) { 
            if (params_ != NULL && i < params_->size())
                (*params_)[i] = arg;
        }

        void print(ostream& os, int indent=0) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();

    private:
        vector<ExprNode*>* params_;
        const SymTabEntry *ste_; // reference semantics
};


class PrintFunctionNode: public ExprNode {
    // Used to represent function invocation
    public:
        PrintFunctionNode( vector<ExprNode*>* param=0, int line=0, int column=0, string file="");
        PrintFunctionNode(const PrintFunctionNode&);
        ExprNode* clone() const { return new PrintFunctionNode(*this); }
        ~PrintFunctionNode() {};

        const vector<ExprNode*>* params() const { return params_;};

        vector<ExprNode*>* params() { return params_;}

        void params(vector<ExprNode*>* args) { params_ = args;};

        const ExprNode* param(unsigned int i) const {
            return (params_ != NULL && i < params_->size()) ? (const ExprNode*)((*params_)[i]) : NULL; 
        }

        ExprNode* param(unsigned int i) {
            return (ExprNode*)((const PrintFunctionNode*)this->param(i));
        }

        void param(ExprNode* arg, unsigned int i) { 
            if (params_ != NULL && i < params_->size())
                (*params_)[i] = arg;
        }

        void print(ostream& os, int indent=0) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  

    private:
        vector<ExprNode*>* params_;
};

/****************************************************************/

class ClassFuncInvocationNode: public ExprNode {
    // Used to represent function invocation
    public:
        ClassFuncInvocationNode(const SymTabEntry *oste, const SymTabEntry *fste, vector<ExprNode*>* param=0, 
                int line=0, int column=0, string file="");
        ClassFuncInvocationNode(const ClassFuncInvocationNode&);
        ExprNode* clone() const { return new ClassFuncInvocationNode(*this); }
        ~ClassFuncInvocationNode() {};

        const SymTabEntry* symTabEntryObject() const { return oste_; };
        
        const SymTabEntry* symTabEntryFunction() const { return fste_; };

        const vector<ExprNode*>* params() const { return params_;};

        vector<ExprNode*>* params() { return params_;}

        void params(vector<ExprNode*>* args) { params_ = args;};

        const ExprNode* param(unsigned int i) const {
            return (params_ != NULL && i < params_->size()) ? (const ExprNode*)((*params_)[i]) : NULL; 
        }

        ExprNode* param(unsigned int i) {
            return (ExprNode*)((const ClassFuncInvocationNode*)this->param(i));
        }

        void param(ExprNode* arg, unsigned int i) { 
            if (params_ != NULL && i < params_->size())
                (*params_)[i] = arg;
        }

        void print(ostream& os, int indent=0) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  

    private:
        vector<ExprNode*>* params_;
        const SymTabEntry *oste_; // reference symbol table entry for object.
        const SymTabEntry *fste_; // reference symbol table entry for function.
};

/****************************************************************/

class ClassRefExprNode: public ExprNode {
    public:
        ClassRefExprNode(string ext, const SymTabEntry* objSte=NULL, const SymTabEntry* varSte = NULL,
                int line=0, int column=0, string file="");
        ClassRefExprNode(const ClassRefExprNode&);
        ExprNode* clone() const { return new ClassRefExprNode(*this); }

        ~ClassRefExprNode() {};

        string ext() const { return ext_;};
        void ext(string str) { ext_ = str;}; 

        const SymTabEntry* symTabEntryObject() const { return objSym_;};
        const SymTabEntry* symTabEntryVariable() const { return varSym_;};
        void symTabEntryObject(const SymTabEntry *objSte)  { objSym_ = objSte;};
        void symTabEntryVariable(const SymTabEntry *varSte)  { varSym_ = varSte;};

        void print(ostream& os, int indent=0) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  

    private:
        string ext_;
        const SymTabEntry* objSym_;
        const SymTabEntry* varSym_;
};

/****************************************************************/
// There are 3 kinds of PatNodes:
//   PrimitivePatNodes are of the form: event|cond
//   AtomicPatNodes consist of PrimitivePatNodes with one or more "||"
//      operators, plus an optional negation symbol
//   PatNodes consist of PatNodes or AtomicPatNodes composed with
//      ".", "||" and "*" operators
// We have a single base class for pattern nodes called BasePatNode. In
// addition, the functionality of Atomic and PatNodes have been
// combined into a single class PatNode.

class BasePatNode: public AstNode {
    public:
        enum class PatNodeKind {PRIMITIVE, EMPTY, NEG, SEQ, OR, STAR, UNDEFINED};

    public:
        BasePatNode(PatNodeKind pk, int ln=0, int col=0, string f=""):
            AstNode(AstNode::NodeType::PAT_NODE, ln, col, f) 
        {
            parent_ = NULL; root_ = NULL; patKind_ = pk;
        }
        BasePatNode(const BasePatNode& bpn): AstNode(bpn) {
            patKind_ = bpn.patKind_; parent_ = bpn.parent_; root_ = bpn.root_;
        }
        ~BasePatNode() {};
        //virtual BasepatNode* clone() const { return new BasePatNode(*this);}	

        PatNodeKind kind() const { return patKind_; };
        void kind(PatNodeKind p) {patKind_ = p;}

        const BasePatNode* parent() const { return parent_; } 
        BasePatNode* parent() { return parent_;} 

        virtual bool hasSeqOps() const=0;
        virtual bool hasNeg() const=0;
        virtual bool hasAnyOrOther() const=0;
        virtual bool isNegatable() const {
            return ((!hasSeqOps()) && (!hasNeg())); 
        }

    private:
        PatNodeKind patKind_;
        BasePatNode* parent_;
        BasePatNode* root_;
};

/****************************************************************/

class PrimitivePatNode: public BasePatNode {
    public:
        PrimitivePatNode(EventEntry* ee, vector<VariableEntry*>* params, 
                ExprNode* c=NULL,
                int line=0, int column=0, string file="") :
            BasePatNode(PatNodeKind::PRIMITIVE, line, column, file)
        {
            ee_ = ee;
            params_ = params;
            /* cond_ may contain assignments as well as other expressions */
            /* condition_ contains all expresions in cond_ other than assignments */
            cond_= c;      
            condition_ = c; 
            //asgs_ = 
        }
        //PrimitivePatNode(const PrimitivePatNode& ppn);
        ~PrimitivePatNode() {};
        //BasePatNode* clone() { return new PrimitivePatNode(*this); }

        const EventEntry* event() const { return ee_; }
        EventEntry* event() { return ee_; }

        const vector<const VariableEntry*>* params() const { 
            return (vector<const VariableEntry*>*)params_; }
        vector<VariableEntry*>* params() { return params_; }

        const ExprNode* cond() const { return cond_; }
        ExprNode* cond() { return cond_; }
        void cond(ExprNode* c) { cond_ = c;}

        ExprNode* condition() { return condition_; }
        const ExprNode* condition() const { return condition_; }

        const list<const OpNode*>& asgs() const { 
            return (list<const OpNode*>&)asgs_; 
        } 
        list<OpNode*>& asgs() { return asgs_; }  

        // TODO
        bool hasNeg() const {
            if (kind() == BasePatNode::PatNodeKind::NEG)
                return true;
            return false;
        }
        bool hasSeqOps() const {
            return false;
        }
        bool hasAnyOrOther() const;

        void print(ostream& os, int indent=0) const;
        const Type* typeCheck();

    private:

        EventEntry* ee_;
        vector<VariableEntry*>* params_;
        /* cond_ may contain assignments as well as other expressions */
        /* condition_ contains all expresions in cond_ other than assignments */
        ExprNode* cond_;      
        ExprNode* condition_; 
        list<OpNode*> asgs_;
};

/****************************************************************/
class PatNode: public BasePatNode {
    public: 
        PatNode(int line=0, int column=0, string file="") :
            BasePatNode(PatNodeKind::EMPTY, line, column, file)
        {
            pat1_ = NULL;
            pat2_ = NULL;
        }

        PatNode(PatNodeKind pk, BasePatNode *p1, BasePatNode*p2 = NULL, int line = 0, int column = 0, string file = "") :
            BasePatNode(pk, line, column, file)
        {
            pat1_ = p1;
            pat2_ = p2;
        }

        ~PatNode() {};
        //AstNode* clone() 
        //  { return new PatNode(*this); }

        const BasePatNode* pat1() const { return pat1_; }
        BasePatNode* pat1() { return pat1_; }
        const BasePatNode* pat2() const { return pat2_; }
        BasePatNode* pat2() { return pat2_; }

        // TODO
        bool hasNeg() const {
            if (kind() == BasePatNode::PatNodeKind::NEG)
                return true;
            return false;
        }
        bool hasSeqOps() const;

        bool hasAnyOrOther() const {
            return true;
        }
        void print(ostream& os, int indent=0) const;
        const Type* typeCheck();
    private: 
        PatNode(const PatNode&);

        BasePatNode *pat1_;
        BasePatNode *pat2_;
};


/****************************************************************/

class StmtNode: public AstNode {
    public:
        enum class StmtNodeKind { ILLEGAL=-1, EXPR, IF, COMPOUND, RETURN, WHILE, BREAK, CONTINUE};
    public: 
        StmtNode(StmtNodeKind skm, int line=0, int column=0, string file=""):
            AstNode(AstNode::NodeType::STMT_NODE, line, column, file) { skind_ = skm; };
        ~StmtNode() {};
        //AstNode* clone() 
        //  { return new StmtNode(*this); }

        StmtNodeKind stmtNodeKind() const { return skind_;}

        void print(ostream& os, int indent) const = 0;
    private:
        StmtNodeKind skind_;
};

/****************************************************************/

class ReturnStmtNode: public StmtNode {
    public:
        ReturnStmtNode(ExprNode *e, FunctionEntry* fe, 
                int line=0, int column=0, string file=""):
            StmtNode(StmtNode::StmtNodeKind::RETURN,line,column,file) { expr_ = e; fun_ = fe;};
        ~ReturnStmtNode() {};

        void print(ostream& os, int indent) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  

    private:
        ExprNode* expr_;
        FunctionEntry* fun_;
};

/****************************************************************/

class ExprStmtNode: public StmtNode {
    public:
        ExprStmtNode(ExprNode* e, int line=0, int column=0, string file=""):
            StmtNode(StmtNode::StmtNodeKind::EXPR, line, column, file) { expr_ = e; };
        ~ExprStmtNode() {};
        //AstNode* clone() 
        //  { return new ExprStmtNode(*this); }

        void print(ostream& os, int indent) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  

    private:
        ExprNode* expr_;
};

/****************************************************************/

class CompoundStmtNode: public StmtNode {
    public: 
        CompoundStmtNode(list<StmtNode*> *Slist, int ln=0, int col=0, string fl=""):
            StmtNode(StmtNode::StmtNodeKind::COMPOUND, ln, col, fl) { stmts_ = Slist; }
        ~CompoundStmtNode() { };
        //AstNode* clone() 
        //  { return new CompoundStmtNode(*this); }

        const list<StmtNode*>* stmts() const { return stmts_; }

        list<StmtNode*>* stmts() { return stmts_; }

        void add(StmtNode *s) { 
            if (stmts_ != NULL) stmts_->push_back(s); 
        }

        void  printWithoutBraces(ostream& os, int indent) const;
        void  print(ostream& os, int indent) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  
    private:
        CompoundStmtNode(const CompoundStmtNode&);
        list<StmtNode*>   *stmts_;
};

/****************************************************************/

class IfNode: public StmtNode {
    public: 

        IfNode(ExprNode* cond, StmtNode* thenStmt, 
                StmtNode* elseStmt = NULL, int line = 0, int column = 0, string file = "") :
            StmtNode(StmtNode::StmtNodeKind::IF, line, column, file)
        {
            cond_ = cond;
            then_ = thenStmt;
            else_ = elseStmt;
        }
        ~IfNode() {};
        //AstNode* clone() 
        //  { return new IfNode(*this); }

        const ExprNode* cond() const {return cond_;}
        const StmtNode* elseStmt() const { return else_;};
        const StmtNode* thenStmt() const  { return then_;};

        ExprNode* cond() {return cond_;}      
        StmtNode* elseStmt() { return else_;};
        StmtNode* thenStmt() { return then_;};

        void print(ostream& os, int indent) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  

    private: 
        ExprNode *cond_;
        StmtNode *then_, *else_;

        IfNode(const IfNode&);
};

/****************************************************************/

class WhileNode: public StmtNode {
    public: 

        WhileNode(ExprNode* cond, StmtNode* bodyStmt, 
                int line = 0, int column = 0, string file = "") :
            StmtNode(StmtNode::StmtNodeKind::WHILE, line, column, file)
        {
            cond_ = cond;
            body_ = bodyStmt;
        }
        ~WhileNode() {};
        //AstNode* clone() 
        //  { return new IfNode(*this); }

        const ExprNode* cond() const {return cond_;}
        const StmtNode* bodyStmt() const { return body_;};

        ExprNode* cond() {return cond_;}      
        StmtNode* bodyStmt() { return body_;};

        void print(ostream& os, int indent) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  

    private: 
        ExprNode *cond_;
        StmtNode *body_;

        WhileNode(const WhileNode&);
};

/****************************************************************/

class BreakStmtNode: public StmtNode {
    public:
        BreakStmtNode(ExprNode *e, int line=0, int column=0, string file=""):
            StmtNode(StmtNode::StmtNodeKind::BREAK,line,column,file)
        {
            expr_ = e;
            wnode_ = NULL;
        }
        ~BreakStmtNode() {}

        void print(ostream& os, int indent) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  

    private:
        ExprNode* expr_;    // Depth of outer while
        WhileNode* wnode_;  // Reference to the WhileNode it would jump to the end of
};

/****************************************************************/

class ContinueStmtNode: public StmtNode {
    public:
        ContinueStmtNode(ExprNode *e, int line=0, int column=0, string file=""):
            StmtNode(StmtNode::StmtNodeKind::CONTINUE,line,column,file) 
        {
            expr_ = e;
            wnode_ = NULL;
        }
        ~ContinueStmtNode() {}

        void print(ostream& os, int indent) const;
        const Type* typeCheck();
        InterCodesClass* codeGen();  

    private:
        ExprNode* expr_;    // Depth of outer while
        WhileNode* wnode_;  // Reference to the WhileNode it would jump to the start of
};

/****************************************************************/

class RuleNode: public AstNode {
    public:
        RuleNode(BlockEntry *re, BasePatNode* pat, StmtNode* reaction, 
                int line=0, int column=0, string file="");
        ~RuleNode() {}
        //AstNode* clone() 
        //  { return new RuleNode(*this); }

        const BlockEntry* ruleEntry() const { return rste_; };
        BlockEntry* ruleEntry() { return rste_; };

        const BasePatNode* pat() const { return pat_; };
        BasePatNode* pat() { return pat_; };              

        const StmtNode* reaction() const { return reaction_; };   
        StmtNode* reaction() { return reaction_; };   

        void print(ostream& os, int indent=0) const;
        const Type* typeCheck();

    private:
        BlockEntry  *rste_;
        BasePatNode *pat_;
        StmtNode *reaction_;

        RuleNode(const RuleNode&);
};

/****************************************************************/

class VregNode: public ExprNode
{
    public:
        VregNode (int line, int column, string file); 
        ExprNode* clone() const {return NULL; }
        void print(ostream& os, int indent=0) const {}
        virtual const Type* typeCheck() { return NULL; };
        string getRefName() { return name_; }
    private:
        string name_;
};


/****************************************************************/
#endif
