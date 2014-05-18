#include "Ast.h"
#include "ParserUtil.h"
#include "InterCode.h"
#include "AbstractMachineCode.h"

AstNode::AstNode(NodeType nt, int line, int column, string file):
    ProgramElem(NULL, line, column, file) 
{

}

AstNode::AstNode(const AstNode& ast): ProgramElem(ast) 
{

}

/****************************************************************/

VregNode::VregNode (int line=0, int column=0, string file="") : 
    ExprNode (ExprNode::ExprNodeType::VREG_NODE, NULL, line, column, file) 
{
    name_    = newName("vreg"); 
    refnode_ = this;
}

string VregNode::getRegisterName()
{
    if(registerName_.length() != 0) return registerName_ ;
    Type *type_ = (Type*)coercedType();
    if(!type_) type_ = type();
    (type_ && type_->isFloat(type_->tag())) ? setRegisterName(allocateNewRegName(true)) : setRegisterName(allocateNewRegName(false)) ; 
    return registerName_;
}

void VregNode::setRegisterName(string reg_name)
{
    registerName_ = reg_name;
}

const Type* VregNode::typeCheck()
{
    return type();
}

/****************************************************************/

ExprNode* ExprNode:: getRefNode () 
{
    if (refnode_ == NULL) {
        refnode_ = new VregNode();

        Type* t = const_cast<Type*>(typeCheck());
        refnode_->type(t);
        if (refnode_->type() == NULL)
        {
            Type *t = const_cast<Type*>(typeCheck());
            refnode_->type(t);
        }
    }
    return (ExprNode *)refnode_;
}

ExprNode::ExprNode(ExprNodeType et, const Value* val, int line, int column, 
        string file):
    AstNode(AstNode::NodeType::EXPR_NODE, line, column, file)
{
    exprType_ = et;
    val_      = val;
}

ExprNode::ExprNode(const ExprNode& e) : AstNode(e)
{

}

/****************************************************************/

RefExprNode::RefExprNode(string ext, const SymTabEntry* ste, 
        int line, int column, string file): 
    ExprNode(ExprNode::ExprNodeType::REF_EXPR_NODE, 0, line, column, file)
{
    type((Type*)ste->type());
    ext_     = ext;
    sym_     = ste;
    refnode_ = this;
    type((Type*)ste->type());
}

RefExprNode::RefExprNode(const RefExprNode& ref) : ExprNode(ref)
{
    refnode_ = this;
}

void RefExprNode::print(ostream& os, int indent) const
{
    os << ext_;
}

const Type* RefExprNode::typeCheck()
{
    const Type* typ = NULL; 
    if (symTabEntry())    
        typ = sym_->type();

    return typ;
}

InterCodesClass* RefExprNode::codeGen()
{
    return NULL;
}

string RefExprNode::getRegisterName()
{
    return ((SymTabEntry*)sym_)->getRegisterName();
}

void   RefExprNode::setRegisterName(string reg_name)
{
    SymTabEntry* ste = (SymTabEntry *)sym_ ;
    if(ste) ste -> setRegisterName(reg_name);
}
/****************************************************************/

void ValueNode::print(ostream& os, int indent) const
{
    value()->print(os, indent); 
}

const Type* ValueNode::typeCheck()
{
    return value()->type(); 
}

InterCodesClass* ValueNode::codeGen()
{
    return NULL;
}


/****************************************************************/

void ExprStmtNode::print(ostream& os, int indent) const {
    if (expr_ != NULL) { 
        prtSpace(os, indent);
        expr_->print(os, indent);
    }
    os << ";"; 
}

const Type* ExprStmtNode::typeCheck() {
    if (expr_ != NULL) { 
        return expr_->typeCheck();
    }
    return NULL;
}

InterCodesClass* ExprStmtNode::codeGen() {
    if (expr_ != NULL) { 
        InterCodesClass *cls = new InterCodesClass();
        cls->addCode (expr_->codeGen());
        return cls;
    }
    return NULL;
}

/****************************************************************/

void IfNode::print(ostream& os, int indent) const
{
    prtSpace(os, indent);

    // If (condition)
    os << "if (";
    cond_->print(os, indent);
    os << ") ";

    // then
    then_->print(os, indent);

    // else
    if (else_) {
        os << endl;
        prtSpace(os, indent);
        os << "else ";
        else_->print(os, indent);
    }
}

const Type* IfNode::typeCheck() 
{
    if (cond_) {
        const Type* cond_type = cond_->typeCheck();
        
        if (cond_type && cond_type->tag() != Type::TypeTag::BOOL) {
            errMsg("Boolean argument expected", cond_);
       }

        if (then_)
            then_->typeCheck();

        if(else_)
            else_->typeCheck();
    }
    return NULL;
}

static bool IsIfRelNeeded = false;

InterCodesClass* IfNode::codeGen()
{
    assert(cond_ && "If Condition Expected");
    if (then_) {
        InterCodesClass* cls = new InterCodesClass();    
        InterCode* nxt = LabelClass::assignLabel();

        cond_->OnTrue(LabelClass::assignLabel()); 
        then_->next (nxt);

        if (!else_) {
            cond_->OnFalse (nxt);
        } else {
            cond_->OnFalse(LabelClass::assignLabel());   
            else_->next (nxt);
        }
        IsIfRelNeeded = true;
        InterCodesClass* ic_cond;
        if ((ic_cond = cond_->codeGen()) != NULL) {
            cls->addCode(ic_cond);
        } else {
            cls->addCode(InterCode::OPNTYPE::IFREL, cond_);
        }
        IsIfRelNeeded = false;

        cls->addCode(cond_->onTrue_);
        cls->addCode(then_->codeGen());

        if (else_) {
            cls->addCode (InterCode::OPNTYPE::GOTO, nxt); 
            cls->addCode (cond_->onFalse_);
            cls->addCode (else_->codeGen());
        }
        cls->addCode (nxt); 

        return cls;
    }
    return NULL;
}

/****************************************************************/

void ReturnStmtNode::print(ostream& os, int indent) const 
{
    prtSpace(os, indent);
    os << "return ";
    if (expr_ != NULL) expr_->print(os, indent);
    else os << "NULL";
    os << ";";
}

const Type* ReturnStmtNode::typeCheck() 
{   
    const Type* return_type = NULL;
    const Type* func_type = fun_->type()->retType();        

    if (expr_) {
        return_type = expr_->typeCheck(); 

        if (return_type && func_type) {

            if (func_type->isSubType(return_type->tag())) {
                expr_->coercedType(func_type); 

            } else if (func_type->tag() == Type::TypeTag::VOID) {
                errMsg("No return value expected for void a function", expr_);

            } else {
                errMsg("Return value incompatible with current function type", expr_);
            }
        }
    } else if (func_type->tag() != Type::TypeTag::VOID) {
        errMsg("Return value expected", this);
    }
    return return_type;
}

InterCodesClass* ReturnStmtNode::codeGen()
{
    InterCodesClass *cls = new InterCodesClass();
    
    if (expr_ != NULL) {
        cls->addCode(expr_->codeGen());
        cls->addCode(InterCode::OPNTYPE::RETURN, expr_->getRefNode());
    } else {
        cls->addCode(InterCode::OPNTYPE::RETURN);
    }

    return cls;
}

/****************************************************************/

void WhileNode::print(ostream& os, int indent) const
{
    prtSpace(os, indent);

    // While (condition)
    os << "while (";
    cond_->print(os, indent);
    os << ") ";

    // while body
    body_->print(os, indent);

}

static vector<const WhileNode*> nested_while_l;

const Type* WhileNode::typeCheck() 
{
    nested_while_l.push_back(this);

    if (cond_) {
        const Type* cond_type = cond_->typeCheck();
        
        if (cond_type && cond_type->tag() != Type::TypeTag::BOOL) {
            errMsg("Boolean argument expected", cond_);
        }

        if (body_)
            body_->typeCheck();

    }
    nested_while_l.pop_back();
    return NULL;
}

InterCodesClass* WhileNode::codeGen()
{
    InterCodesClass *cls = new InterCodesClass();
    InterCode* loop_l = LabelClass::assignLabel();
    InterCode* body_l = LabelClass::assignLabel();
    InterCode* end_l = LabelClass::assignLabel();
    InterCodesClass* ic_cond;

    body_->next(loop_l);
    cls->addCode(loop_l);

    cond_->OnTrue(body_l); 
    cond_->OnFalse(end_l);
    IsIfRelNeeded = true;
    if ((ic_cond = cond_->codeGen()) != NULL) {
        cls->addCode(ic_cond);
    } else {
        cls->addCode(InterCode::OPNTYPE::IFREL, cond_);
    }
    IsIfRelNeeded = false;

    cls->addCode(body_l);
    cls->addCode(body_->codeGen());
    cls->addCode (InterCode::OPNTYPE::GOTO, loop_l);
    cls->addCode(end_l);
    
    return cls;
}

/****************************************************************/

void BreakStmtNode::print(ostream& os, int indent) const 
{
    prtSpace(os, indent);
    os << "break ";
    expr_->print(os, indent);
    os << ";";
}

const Type* BreakStmtNode::typeCheck()
{   
    int depth = ((ValueNode*)expr_)->value()->ival();
    int curDepth = nested_while_l.size();

    if (depth > 0 && (curDepth - depth) < 0)
        errMsg("Invalid reference to outer While loops", this);

    wnode_ = (WhileNode*) nested_while_l[curDepth-depth];

    return expr_->typeCheck(); 
}

InterCodesClass* BreakStmtNode::codeGen()
{
    InterCodesClass *cls = new InterCodesClass();
    cls->addCode (InterCode::OPNTYPE::GOTO, wnode_->cond()->OnFalse());
    return cls;
}

/****************************************************************/

void ContinueStmtNode::print(ostream& os, int indent) const 
{
    prtSpace(os, indent);
    os << "continue ";
    expr_->print(os, indent);
    os << ";";
}

const Type* ContinueStmtNode::typeCheck()
{   
    int depth = ((ValueNode*)expr_)->value()->ival();
    int curDepth = nested_while_l.size();

    if (depth > 0 && (curDepth - depth) < 0)
        errMsg("Invalid reference to outer While loops", this);

    wnode_ = (WhileNode*) nested_while_l[curDepth-depth];

    return expr_->typeCheck(); 
}

InterCodesClass* ContinueStmtNode::codeGen()
{
    InterCodesClass *cls = new InterCodesClass();
    cls->addCode (InterCode::OPNTYPE::GOTO, wnode_->bodyStmt()->next());
    return cls;
}

/****************************************************************/

void CompoundStmtNode::printWithoutBraces(ostream& os, int indent) const
{
    if (stmts_ == NULL || stmts_->size() == 0) return;

    cout << endl;
    for (std::list<StmtNode*>::iterator it = stmts_->begin(); it != stmts_->end(); it++) {
        (*it)->print(os, indent+STEP_INDENT);
        os << endl;
    }
}

void CompoundStmtNode::print(ostream& os, int indent) const
{
    prtSpace(os, indent);
    os << "{";
    printWithoutBraces(os, indent);
    prtSpace(os, indent);
    os << "};";
}

const Type* CompoundStmtNode::typeCheck() {

    if (stmts_ == NULL || stmts_->size() == 0) return NULL;

    for (std::list<StmtNode*>::iterator it = stmts_->begin(); it != stmts_->end(); it++) {
        (*it)->typeCheck();
    }
    return NULL;
}

InterCodesClass* CompoundStmtNode::codeGen() {
    InterCodesClass *cls = new InterCodesClass();
    
    if (stmts_) {
        list <StmtNode*>::iterator it = stmts_->begin(); 
        for (; it != stmts_->end(); ++it) {
            cls->addCode((*it)->codeGen());
        }
    }
    return cls;
}

/****************************************************************/

InvocationNode::InvocationNode(const SymTabEntry *ste, vector<ExprNode*>* param, 
        int line, int column, string file):
    ExprNode(ExprNode::ExprNodeType::INV_NODE, 0, line, column, file)
{
    assert(ste);
    ste_ = ste;
    params_ = param;
}

InvocationNode::InvocationNode(const InvocationNode& ref) : ExprNode(ref)
{

}

void InvocationNode::print(ostream& out, int indent) const
{
    assert(ste_);
    out << ste_->name() << "(";
    if (params_) {
        unsigned int i = 0; 
        for (std::vector<ExprNode*>::iterator it = params_->begin(); it != params_->end(); i++, it++) {
            if (i > 0) out << ", ";
            (*it)->print(out, indent);
        }
    }
    out << ")";
}

const Type* InvocationNode::typeCheck()
{
    FunctionEntry* func_entry  = (FunctionEntry *) symTabEntry();  
    vector<Type*>* argtypes    = func_entry->type()->argTypes();

    int caller_param = params_ ? params_->size() : 0;
    int callee_param = argtypes ? argtypes->size() : 0;

    if (caller_param != callee_param) {
        errMsg((string)itoa(callee_param) + " arguments expected for " + func_entry->name(), this);
        return NULL; 

    } else if (params_) {
        unsigned int i = 1; 
        vector<Type*>::iterator t_iter    = argtypes->begin();
        vector<ExprNode*>::iterator p_iter = params_->begin();

        for (; t_iter != argtypes->end(); ++t_iter) {

            ExprNode* expr_node    = *p_iter; 
            const Type* param_type = expr_node->typeCheck(); 

            if (*t_iter && param_type && (*t_iter)->isSubType (param_type->tag(), param_type)) {

                expr_node->coercedType (*t_iter);

            } else {
                errMsg("Type Mismatch for argument " + (string)itoa(i) + " to " + func_entry->name(), expr_node); 
            }
            ++i;
            ++p_iter;
        }
    }

    return func_entry->type()->retType();
}

InterCodesClass* InvocationNode::codeGen()
{
    InterCodesClass *cls = new InterCodesClass();
    FunctionEntry* func_entry  = (FunctionEntry *) symTabEntry();  
    InterCode* nxt = LabelClass::assignLabel();

    if (params_) {
        for (int ii = params_->size() - 1; ii >= 0; ii--) {
            ExprNode* expr_node = (*params_)[ii];
            cls->addCode(expr_node->codeGen());
            cls->addCode(InterCode::OPNTYPE::APARAM, expr_node->getRefNode());
        }
    }

    if (func_entry->type()->retType()->tag() == Type::VOID) {
        cls->addCode(InterCode::OPNTYPE::CALL, this);
    } else {
        cls->addCode(InterCode::OPNTYPE::CALL, this, this->getRefNode());
    }

    // Add next label for return address of call
    this->next(nxt);
    cls->addCode(nxt);

    return cls;
}

/****************************************************************/

PrintFunctionNode::PrintFunctionNode( vector<ExprNode*>* param, int line, int column, string file):
    ExprNode(ExprNode::ExprNodeType::PRINT_NODE, 0, line, column, file)
{
    params_ = param;
}

PrintFunctionNode::PrintFunctionNode(const PrintFunctionNode& ref) : ExprNode(ref)
{

}

void PrintFunctionNode::print(ostream& out, int indent) const
{
    out<<"print(";
    if (params_) {
        unsigned int i = 0;
        for (std::vector<ExprNode*>::iterator it = params_->begin(); it != params_->end(); i++, it++) {
            if (i > 0) out << ", ";
            (*it)->print(out, indent);
        } 
    }
    out<<")";
}

const Type* PrintFunctionNode::typeCheck()
{
    if (params_) {
        vector<ExprNode*>::iterator p_iter = params_->begin();
        for (; p_iter != params_->end(); ++p_iter) {
            ExprNode* expr_node = *p_iter; 
            expr_node->typeCheck(); 
        }
    }
    // 'print' function has return type VOID
    return new Type(Type::TypeTag::VOID);
}

InterCodesClass* PrintFunctionNode::codeGen() 
{
    if (params_) {
        InterCodesClass *cls = new InterCodesClass();
        vector<ExprNode*>::iterator p_iter = params_->begin();
        for (; p_iter != params_->end(); ++p_iter) {
            ExprNode* expr_node = *p_iter; 
            cls->addCode(expr_node->codeGen());
            cls->addCode(InterCode::OPNTYPE::PRINT, expr_node->getRefNode());
        }
        return cls;
    }
    return NULL;
}

/****************************************************************/

ClassFuncInvocationNode::ClassFuncInvocationNode(const SymTabEntry *oste, const SymTabEntry *fste,
                        vector<ExprNode*>* param, int line, int column, string file):
    ExprNode(ExprNode::ExprNodeType::INV_NODE, 0, line, column, file)
{
    assert(oste);
    assert(fste);
    oste_ = oste;
    fste_ = fste;
    params_ = param;
}

ClassFuncInvocationNode::ClassFuncInvocationNode(const ClassFuncInvocationNode& ref) : ExprNode(ref)
{

}

void ClassFuncInvocationNode::print(ostream& out, int indent) const
{
    assert(oste_);
    assert(fste_);
    out << oste_->name() << ".";
    out << fste_->name() << "(";
    if (params_) {
        unsigned int i = 0; 
        for (std::vector<ExprNode*>::iterator it = params_->begin(); it != params_->end(); i++, it++) {
            if (i > 0) out << ", ";
            (*it)->print(out, indent);
        }
    }
    out << ")";
}

const Type* ClassFuncInvocationNode::typeCheck()
{
    FunctionEntry* func_entry = (FunctionEntry *) symTabEntryFunction();  
    vector<Type*>* argtypes   = func_entry->type()->argTypes();
    
    int caller_param = params_ ? params_->size() : 0;
    int callee_param = argtypes ? argtypes->size() : 0;
       
    if (caller_param != callee_param) {
        errMsg((string)itoa(callee_param) + " arguments expected for " + func_entry->name(), this);
        return NULL; 

    } else if (params_) {
        unsigned int i = 1; 
        vector<Type*>::iterator t_iter    = argtypes->begin();
        vector<ExprNode*>::iterator p_iter = params_->begin();
         
        for (; t_iter != argtypes->end(); ++t_iter) {
            
            ExprNode* expr_node    = *p_iter; 
            const Type* param_type = expr_node->typeCheck(); 
            
            if (*t_iter && param_type && (*t_iter)->isSubType (param_type->tag(), param_type)) {
                
                expr_node->coercedType (*t_iter);
            
            } else {
               errMsg("Type Mismatch for argument " + (string)itoa(i) + " to " + func_entry->name(), expr_node); 
            }
            ++i;
            ++p_iter;
        }
    }

    if (func_entry)
        return func_entry->type()->retType();
    
    return NULL;
}

InterCodesClass* ClassFuncInvocationNode::codeGen()
{
    InterCodesClass *cls = new InterCodesClass();
    //TODO:
    return cls;
}

/****************************************************************/

ClassRefExprNode::ClassRefExprNode(string ext, const SymTabEntry* objSte, const SymTabEntry* varSte,
        int line, int column, string file): 
    ExprNode(ExprNode::ExprNodeType::REF_EXPR_NODE, 0, line, column, file)
{
    ext_ = ext;
    objSym_ = objSte;
    varSym_ = varSte;
}

ClassRefExprNode::ClassRefExprNode(const ClassRefExprNode& ref) : ExprNode(ref)
{

}

void ClassRefExprNode::print(ostream& os, int indent) const
{
    os << objSym_->name();
    os << ".";
    os << ext_;
}

const Type* ClassRefExprNode::typeCheck() {
    const Type* typ = NULL; 
    if (symTabEntryVariable())    
        typ = varSym_->type();

    return typ;
}

InterCodesClass* ClassRefExprNode::codeGen()
{
    InterCodesClass *cls = new InterCodesClass();
    //TODO:
    return cls;
}

/****************************************************************/

RuleNode::RuleNode(BlockEntry *re, BasePatNode* pat, StmtNode* reaction, 
        int line, int column, string file) :
    AstNode(AstNode::NodeType::RULE_NODE, line, column, file)
{
    rste_ = re;
    pat_ = pat;
    reaction_ = reaction;
}

void RuleNode::print(ostream& out, int indent) const 
{
    prtSpace(out, indent);

    pat_->print(out, indent);
    out << "--> ";
    reaction_->print(out, indent);

    out << endl;
    prtSpace(out, indent);
    out << ";;";
}

const Type* RuleNode::typeCheck() 
{
    if (pat_)
        pat_->typeCheck();

    if (reaction_)
        reaction_->typeCheck();
    
    return NULL;
}

static FunctionEntry *gRuleFuncSym = NULL;
InterCodesClass* RuleNode::codeGen()
{
    // TODO: Currently only Primitive PatNodes are supported
    if (pat_->kind() == BasePatNode::PatNodeKind::PRIMITIVE) {
        InterCodesClass *cls = new InterCodesClass();

        cls->addCode(pat_->codeGen());
        if (reaction_)
            cls->addCode(reaction_->codeGen());
        cls->addCode(InterCode::OPNTYPE::LEAVE, gRuleFuncSym);

        gRuleFuncSym = NULL;
        return cls;
    }
    return NULL;
}

/****************************************************************/

bool PrimitivePatNode::hasAnyOrOther() const 
{
    string ee_name = ee_->name();
    if (ee_name.compare ("any") == 0 || ee_name.compare("other") == 0)
        return true;
    return false;
}

void PrimitivePatNode::print(ostream& out, int indent) const 
{
    out << "(";
    out << ee_->name();

    if (!hasAnyOrOther()) {
        vector<Type*>* argtype_l = ee_->type()->argTypes();

        out << "(";
        if (params_) {
            if (params_->size() != argtype_l->size()) {
                errMsg("Invalid event parameters", this);
                return;
            }

            unsigned int size = params_->size();
            if (size > 0) {
                unsigned int i;
                std::vector<Type*>::iterator          type_it = argtype_l->begin();
                std::vector<VariableEntry*>::iterator var_it  = params_->begin();

                for (i = 0; i < size; i++) {
                    if (i > 0) out << ", ";
                    out << (*type_it)->fullName() << " " << (*var_it)->name();
                    type_it++; var_it++;
                }
            }
        }
        out << ")";
    }

    if (cond_) {
        out << "|";
        cond_->print(out, indent); 
    }
    out << ")";
}

const Type* PrimitivePatNode::typeCheck() 
{   //TODO: Blindly copied from print, need to check for segfaults. code seems bit susceptible to segfaults
    //TODO: type mismatch condition is not there in any input files. need to look into if correct.

    vector<Type*>* argtype_l = ee_->type()->argTypes();

    if (params_) {
        if (params_->size() > argtype_l->size()) { 
            errMsg(ee_->name() + ": mismatch in the number of arguments", this);  
        
        } else if (params_->size() < argtype_l->size()) {
            errMsg("Event " + ee_->name() + " requires " + (string)itoa(argtype_l->size()) + " arguments", this);
        }

        unsigned int size = params_->size();
        if (size > 0) {
            std::vector<Type*>::iterator type_it          = argtype_l->begin();
            std::vector<VariableEntry*>::iterator var_it  = params_->begin();

            for (; var_it != params_->end(); ++var_it) {
                VariableEntry* var_entry = *var_it; 
                var_entry->typeCheck(); 
                
                var_entry->type(*type_it);
                type_it++;
            }
        }
    
    } else if (argtype_l && argtype_l->size() > 0) {
        errMsg("Event " + ee_->name() + " requires " + (string)itoa(argtype_l->size()) + " arguments", this);
    }

    if (cond_) {
        cond_->typeCheck();
    }
    return NULL;
}

InterCodesClass* PrimitivePatNode::codeGen()
{
    InterCodesClass *cls = new InterCodesClass();
    string funcName = (string)"_event_" + ee_->name();

    Type *ltype = new Type(ee_->type()->argTypes(), new Type(Type::VOID));
    gRuleFuncSym = new FunctionEntry(funcName, ltype);

    cls->addCode(LabelClass::assignLabel(funcName));
    cls->addCode(InterCode::OPNTYPE::ENTER, gRuleFuncSym);

    for (int ii = params_->size() - 1; ii >= 0; ii--) {
        VariableEntry* var_entry = (*params_)[ii]; 
        cls->addCode(var_entry->codeGen());
    }

    return cls;
}

/****************************************************************/

void PatNode::print(ostream& out, int indent) const 
{
    out << "(";
    if (hasNeg()) {
        out << "!";
    }
    pat1_->print(out, indent);
    if (!hasNeg() && kind() != PatNodeKind::STAR) {
        switch (kind()) {
            default:
            case PatNodeKind::PRIMITIVE: 
            case PatNodeKind::UNDEFINED:
            case PatNodeKind::EMPTY:
                errMsg("Invalid PatNodeKind", this);
                break;
            case PatNodeKind::SEQ:
                out << ":";
                break;
            case PatNodeKind::OR:
                out << " \\/ ";
                break;
        }
        pat2_->print(out, indent);
    }
    if (kind() == PatNodeKind::STAR)
        out << "**";
    out << ")";
}

const Type* PatNode::typeCheck() 
{
    switch(kind()) {
        case PatNodeKind::PRIMITIVE:
        case PatNodeKind::UNDEFINED:    
        case PatNodeKind::EMPTY:
            break;
        case PatNodeKind::STAR: 
            pat1_->typeCheck();
            break;
        case PatNodeKind::SEQ:
        case PatNodeKind::OR:
            pat1_->typeCheck(); 
            pat2_->typeCheck(); 
            break;
        case PatNodeKind::NEG:
            if (hasSeqOps()) {
                errMsg("Only simple patterns without `.', `*', and `!' operatorscan be negated", this);
            }
            pat1_->typeCheck();
            break;
    }
    return NULL;
}

bool PatNode::hasSeqOps() const 
{
    if (pat1_ && pat2_ && kind() == PatNodeKind::SEQ)
        return true;
    if ((pat1_ || pat2_) && kind() == PatNodeKind::STAR)
        return true;
    if ((pat1_ && pat1_->hasSeqOps()) || (pat2_ && pat2_->hasSeqOps()))
        return true;
    return false;
}

/****************************************************************/

const OpNode::OpInfo OpNode::opInfo[] = {
    // print name, arity, paren_flag, fixity, arg types, out type, constraints
    //
    // Paren_flag -- opnode->print() outputs is surrounded by parenthesis if 
    // this flag is set. As set below, the expression may not print correctly
    // in some rare cases, e.g., ~(b * c) will get printed as ~b * c,
    // which actually corresponds to (~b)*c. To ensure that things get printed
    // correctly all the time, more paren_flags should be set to 1, but this
    // will lead to more clutter in printed output. Basically, what we have done
    // here is to look are expressions by type -- arithmetic, relational, 
    // boolean, bit operations, etc. Within each type, the highest priority 
    // operator is printed without paren. This will work correctly, as long
    // as the language doesn't permit mixing of different types of expressions.
    // But this assumption doesn't always hold, as in the example above. Also,
    // there is an exception to this general approach in the case of unary minus
    // and * -- since (-a)*b and -(a*b) have the same meaning, we can exclude
    // paren for * without an error.
    //
    // Codes for constraints:
    // first character:
    //    N: No additional constraint over what is given by argTypes
    //    I: all arguments must have identical type
    //    S: one of the arguments must have a type that is a supertype of
    //        of all other arguments. All other arguments require a coercion 
    //        operation to be introduced so as to convert their type to S.
    //    s: one of the arguments must have a type that is a subtype of
    //        of all other arguments. 
    //    L: all list arguments (and list output) must have same type. In 
    //        addition, all non-list arguments (and output) must have same 
    //        type as that of elements in these lists
    //    T: all tuple arguments to the function must have same type.
    //    A: (assignment). Type of second argument must be a subtype of
    //       the first argument
    //
    // second character:
    //    O: output type is the same as out type. (In the following cases,
    //        the output type need not be equal to out type, but a subtype
    //        of it.) Since a TypeTag provides complete type information only
    //        for primitive types, `O' is applicable only in this case.
    //    digit: output type is the same as that of the digit'th argument
    //       In this case, a third character may be used, the code for
    //       which is as follows:
    //         'e' denotes that the output is of type alpha, where
    //             the type of digit'th argument is list(alpha)
    //         'l' denotes that the output is of type list(alpha), where
    //             alpha is the type of the digit'th argument.
    //    S: The output type is the same as that of the argument with the
    //        most general type. (Typically used with first character 'S')
    //    s: The output type is the same as that of the argument with the
    //        least general type. (Typically used with first character 'S')
    //    P: The output type is the product of the types of all arguments
    //    p: The output type is a component of the input tuple type. The
    //        following character specifies the component. A digit k specifies
    //        that the component number as k. The character 'a' indicates that
    //        the component number is given by an integer argument to the
    //        operator. The argument number is given by the following digit.
    //        'p' can be used only in conjunction with first character 'P'.
    //    L: Output type is the same as type of list arguments. Can be used
    //        only in conjunction with first character L.
    //    e: Output type is the same as type of element of list arguments. 
    //        Can be used only in conjunction with first character L.
    //
    {OpNode::OpCode::UMINUS, "-",  1, 0, OpNode::OpPrintType::PREFIX, {Type::SIGNED}, Type::SIGNED, "N1"},
    {OpNode::OpCode::PLUS, "+",  2, 1, OpNode::OpPrintType::INFIX, {Type::NUMERIC, Type::NUMERIC}, Type::NUMERIC, "SS"},
    {OpNode::OpCode::MINUS, "-",  2, 1, OpNode::OpPrintType::INFIX, {Type::NUMERIC, Type::NUMERIC}, Type::NUMERIC, "SS"},
    {OpNode::OpCode::MULT, "*",  2, 0, OpNode::OpPrintType::INFIX, {Type::NUMERIC, Type::NUMERIC}, Type::NUMERIC, "SS"},
    {OpNode::OpCode::DIV, "/",  2, 1, OpNode::OpPrintType::INFIX, {Type::NUMERIC, Type::NUMERIC}, Type::NUMERIC, "SS"},
    {OpNode::OpCode::MOD, "%",  2, 1, OpNode::OpPrintType::INFIX, {Type::INTEGRAL, Type::INTEGRAL}, Type::INTEGRAL, "S2"},
    {OpNode::OpCode::EQ, "==", 2, 0, OpNode::OpPrintType::INFIX, {Type::PRIMITIVE, Type::PRIMITIVE}, Type::BOOL, "SO"},
    {OpNode::OpCode::NE, "!=", 2, 0, OpNode::OpPrintType::INFIX, {Type::PRIMITIVE, Type::PRIMITIVE}, Type::BOOL, "SO"},
    {OpNode::OpCode::GT, ">",  2, 0, OpNode::OpPrintType::INFIX, {Type::SCALAR, Type::SCALAR}, Type::BOOL, "SO"},
    {OpNode::OpCode::LT, "<",  2, 0, OpNode::OpPrintType::INFIX, {Type::SCALAR, Type::SCALAR}, Type::BOOL, "SO"},
    {OpNode::OpCode::GE, ">=", 2, 0, OpNode::OpPrintType::INFIX, {Type::SCALAR, Type::SCALAR}, Type::BOOL, "SO"},
    {OpNode::OpCode::LE, "<=", 2, 0, OpNode::OpPrintType::INFIX, {Type::SCALAR, Type::SCALAR}, Type::BOOL, "SO"},
    {OpNode::OpCode::AND, "&&",  2, 1, OpNode::OpPrintType::INFIX, {Type::BOOL, Type::BOOL}, Type::BOOL, "NO"},
    {OpNode::OpCode::OR, "||",  2, 1, OpNode::OpPrintType::INFIX, {Type::BOOL, Type::BOOL}, Type::BOOL, "NO"},
    {OpNode::OpCode::NOT, "!",  1, 0, OpNode::OpPrintType::PREFIX, {Type::BOOL}, Type::BOOL, "NO"}, 
    {OpNode::OpCode::BITNOT, "~",  1, 0, OpNode::OpPrintType::PREFIX, {Type::INTEGRAL}, Type::INTEGRAL, "N1"},
    {OpNode::OpCode::BITAND, "&",  2, 1, OpNode::OpPrintType::INFIX, {Type::INTEGRAL, Type::INTEGRAL}, Type::INTEGRAL, "Ss"},
    {OpNode::OpCode::BITOR, "|",  2, 1, OpNode::OpPrintType::INFIX, {Type::INTEGRAL, Type::INTEGRAL}, Type::INTEGRAL, "SS"},
    {OpNode::OpCode::BITXOR, "^",  2, 0, OpNode::OpPrintType::INFIX, {Type::INTEGRAL, Type::INTEGRAL}, Type::INTEGRAL, "SS"},
    {OpNode::OpCode::SHL, "<<", 2, 1, OpNode::OpPrintType::INFIX, {Type::INTEGRAL, Type::INTEGRAL}, Type::INTEGRAL, "N1"},
    {OpNode::OpCode::SHR, ">>", 2, 1, OpNode::OpPrintType::INFIX, {Type::INTEGRAL, Type::INTEGRAL}, Type::INTEGRAL, "N1"},
    {OpNode::OpCode::ASSIGN, "=",  2, 0, OpNode::OpPrintType::INFIX, {Type::NATIVE, Type::NATIVE}, Type::VOID, "AO"},
    {OpNode::OpCode::PRINT, "print", OpNode::VARIABLE, 1, OpNode::OpPrintType::PREFIX, {Type::NATIVE}, Type::VOID, "NO"},
    {OpNode::OpCode::INVALID, "invalid",            0, 0, OpNode::OpPrintType::PREFIX, {}, Type::ERROR, "NO"}
};

OpNode::OpNode(OpCode op, ExprNode* a1, ExprNode* a2, int ln, int col, string file):
    ExprNode(ExprNode::ExprNodeType::OP_NODE, NULL, ln,col,file) 
{
    opCode_ = op;
    if (a1 != NULL) {
        arity_ = 1;
        arg_.push_back(a1);
        if (a2 != NULL) {
            arity_++;
            arg_.push_back(a2);
        }
    }
}

OpNode::OpNode(const OpNode &other): ExprNode(other) {
    arity_ = other.arity();
    opCode_ = other.opCode();
    for (unsigned int i=0; (i < other.arity()); i++) {
        if (other.arg_[i]) {
            arg_.push_back((other.arg_[i])->clone());
        } 
        else {
            arg_.push_back(NULL);
        }
    }
}

void OpNode::print(ostream& os, int indent) const {
    int iopcode = static_cast<int>(opCode_);
    if (opInfo[iopcode].prtType_ == OpNode::OpPrintType::PREFIX) {
        os << opInfo[iopcode].name_;
        if (arity_ > 0) {
            if (opInfo[iopcode].needParen_) 
                os << '(';
            for (unsigned i=0; i < arity_-1; i++) {
                if (arg_[i])
                    arg_[i]->print(os, indent);
                else os << "NULL";
                os << ", ";
            }
            if (arg_[arity_-1])
                arg_[arity_-1]->print(os, indent);
            else os << "NULL";
            if (opInfo[iopcode].needParen_) 
                os << ") ";
        }
    }
    else if ((opInfo[iopcode].prtType_ == OpNode::OpPrintType::INFIX) && (arity_ == 2)) {
        if (opInfo[iopcode].needParen_) 
            os << "(";
        if(arg_[0])
            arg_[0]->print(os, indent);
        else os << "NULL";
        os << opInfo[iopcode].name_; 
        if(arg_[1])
            arg_[1]->print(os, indent);
        else os << "NULL";
        if (opInfo[iopcode].needParen_) 
            os << ")";
    }
    else internalErr("Unhandled case in OpNode::print");
}


const Type* OpNode::typeCheck() {

    int iopcode = static_cast<int>(opCode_);
    const Type* targ1, *targ2;
    targ1 = arg_[0]->typeCheck();

    if (iopcode == (int)OpNode::OpCode::UMINUS) {
        
        if (!(targ1->isIntegral(targ1->tag()) || targ1->isFloat(targ1->tag()) || targ1->isSigned(targ1->tag()))){
            errMsg("Incompatible Type for argument 1 for operator `-'", this);
        }
    
    } else if ((iopcode >= (int)OpNode::OpCode::PLUS && iopcode <= (int)OpNode::OpCode::MOD)) {
    /*************** For MOD Operator: both i/p Integer ****************/

        targ2 = arg_[1]->typeCheck();
        assert(arg_[0] && arg_[1] && "Invalid args");

        if(opCode_ == OpNode::OpCode::MOD) {
            
            if (!targ1->isIntegral(targ1->tag())) {
                errMsg("Incompatible type for argument 1 for operator `%'", this);
            } 
            
            if (!targ2->isIntegral(targ2->tag())) {
                errMsg("Incompatible type for argument 2 for operator `%'", this);
            } 
            
            if (targ1->tag() == targ2->tag()) {
               //cout<<"\n types same";
           
            } else if (targ1->isSubType(targ2->tag())) {
                arg_[1]->coercedType(new Type(targ2->tag()));

            } else if (targ2->isSubType(targ1->tag())) {
                arg_[0]->coercedType(new Type(targ1->tag()));
            }
            
        } else {
            Type *tp = NULL; 
            
            if (!targ1->isNumeric(targ1->tag())) {
                errMsg("Incompatible type for agrument 1 for operator `" + (string)opInfo[iopcode].name_ + "'", this);
            }
            
            if (!targ2->isNumeric(targ2->tag())) {
                errMsg("Incompatible type for agrument 2 for operator `" + (string)opInfo[iopcode].name_ + "'", this);
            }
            
            if (targ1->tag() == targ2->tag()) {
               { //cout<<"\n binary types same";
                tp = new Type(targ1->tag()); }
            
            } else if (targ1->isSubType(targ2->tag())) {
                //cout<<"\n diff arith args";
                arg_[1]->coercedType(new Type(targ2->tag()));
                tp = new Type(targ1->tag());
            
            } else if (targ2->isSubType(targ1->tag())) {
                //cout<<"\n diff arith args";
                arg_[0]->coercedType(new Type(targ1->tag()));
                tp = new Type(targ2->tag());
            }
            if(tp)
                return tp;
        }
    
    } else if((iopcode >= (int)OpNode::OpCode::EQ && iopcode <= (int)OpNode::OpCode::LE)) {
    /*************** For relational operator***********************************/
        
        targ2 = arg_[1]->typeCheck();
        assert(arg_[0] && arg_[1] && "Invalid args");
       
        if (!(targ1->isPrimitive(targ1->tag()))) { 
            errMsg("Incompatible type for agrument 1 for operator `" + (string)opInfo[iopcode].name_ + "'", this);
        }
       
        if (!(targ2->isPrimitive(targ2->tag()))) { 
            errMsg("Incompatible type for agrument 2 for operator `" + (string)opInfo[iopcode].name_ + "'", this);
        }
        
            
        if(targ1->tag() == targ2->tag()) {
            // cout<<"\n types same";
        
        } else if(targ1->isSubType(targ2->tag())) {
           
           arg_[1]->coercedType(new Type(targ2->tag()));
           //cout<<"\n diff args";

        } else if(targ2->isSubType(targ1->tag())) {
           
           arg_[0]->coercedType(new Type(targ1->tag()));
           //cout<<"\n diff args";
        }
        
        return (new Type(opInfo[iopcode].outType_)); 
  
    } else if((iopcode >= (int)OpNode::OpCode::AND && iopcode <= (int)OpNode::OpCode::NOT)) {
    /*************** For logical operator**************************************/
        
        if (!targ1->isBool(targ1->tag())) {
            warnMsg("Incompatible type for agrument 1 for operator `" + (string)opInfo[iopcode].name_ + "'", this);
        }
        
        if(!(iopcode == (int)OpNode::OpCode::NOT)) {
        
             targ2 = arg_[1]->typeCheck();
             assert(arg_[0] && arg_[1] && "Invalid args");
             if (!targ2->isBool(targ2->tag())) {
                 warnMsg("Incompatible type for agrument 2 for operator `" + (string)opInfo[iopcode].name_ + "'", this);
             }
        } 
        return (new Type(opInfo[iopcode].outType_)); 
        
    } else if(iopcode == (int)OpNode::OpCode::BITNOT) {

        assert(arg_[0] && "Invalid args");
        if(!(targ1->isIntegral(targ1->tag()))) {
            errMsg("Incompatible type for agrument 1 for operator `" + (string)opInfo[iopcode].name_ + "'", this);
        }
    
    } else if((iopcode >= (int)OpNode::OpCode::BITAND && iopcode <= (int)OpNode::OpCode::SHR)) {
    /*************** For Bitwise operator ***********************************************/
        targ2 = arg_[1]->typeCheck();
        assert(arg_[0] && arg_[1] && "Invalid args");
        
        if(!(targ1->isIntegral(targ1->tag()))) {
            errMsg("Incompatible type for agrument 1 for operator `" + (string)opInfo[iopcode].name_ + "'", this);
        }
        
        if(!(targ2->isIntegral(targ2->tag()))) {
            errMsg("Incompatible type for agrument 2 for operator `" + (string)opInfo[iopcode].name_ + "'", this);
        }
        
        if(targ1->isIntegral(targ1->tag()) && targ2->isIntegral(targ2->tag())) {
          //cout<<"\n bit operands satisfied";
            Type *tp = NULL; 

            if(targ1->isSubType(targ2->tag())) {
                tp = new Type(targ2->tag());
                arg_[0]->coercedType(tp);
            
            } else if(targ2->isSubType(targ1->tag())) {
                tp = new Type(targ1->tag());
                arg_[1]->coercedType(tp);
            }
            return tp;
        }
    } else if(iopcode == (int)OpNode::OpCode::ASSIGN) {

        targ2 = arg_[1]->typeCheck();
        assert(arg_[0] && arg_[1] && "Invalid args");
        
        if(targ1->isNative(targ1->tag()) && targ2->isNative(targ2->tag())) {
          
              if(targ1->tag() == targ2->tag()) {
              } else if(targ1->isSubType(targ2->tag())) {
                  arg_[0]->coercedType(new Type(targ2->tag()));

              } else {
                  errMsg("Assigned expression must be a subtype of target", this);
              }
        }
        return (new Type(opInfo[iopcode].outType_)); 
    }
    return targ1;
}


InterCodesClass* OpNode::codeGen() 
{
    InterCodesClass *cls = new InterCodesClass();
    InterCodesClass* ic_cond;

    switch (opCode()) {

        // BINARY Conditional Operators
        case OpCode::EQ:
        case OpCode::NE:
        case OpCode::GT:
        case OpCode::LT:
        case OpCode::GE:
        case OpCode::LE:
            cls->addCode (arg_[0]->codeGen());
            cls->addCode (arg_[1]->codeGen());
            if (IsIfRelNeeded) {
                cls->addCode (InterCode::OPNTYPE::IFREL, this,
                        arg_[0]->getRefNode(), arg_[1]->getRefNode(), opCode_);
            } else {
                cls->addCode (InterCode::OPNTYPE::EXPR, getRefNode(),
                        arg_[0]->getRefNode(), arg_[1]->getRefNode(), opCode_);
            }
            break;

        // Short Circuit '&&' Operator
        case OpCode::AND:
        {
            if (IsIfRelNeeded) {
                InterCode* arg0_true_l = LabelClass::assignLabel();

                arg_[0]->OnTrue(arg0_true_l);
                arg_[0]->OnFalse(this->OnFalse());

                if ((ic_cond = arg_[0]->codeGen()) != NULL) {
                    cls->addCode(ic_cond);
                } else {
                    cls->addCode(InterCode::OPNTYPE::IFREL, arg_[0]);
                }

                cls->addCode(arg0_true_l);
                 
                arg_[1]->OnTrue(this->OnTrue());
                arg_[1]->OnFalse(this->OnFalse());

                if ((ic_cond = arg_[1]->codeGen()) != NULL) {
                    cls->addCode(ic_cond);
                } else {
                    cls->addCode(InterCode::OPNTYPE::IFREL, arg_[1]);
                }
            } else {
                cls->addCode (arg_[0]->codeGen());
                cls->addCode (arg_[1]->codeGen());
                cls->addCode (InterCode::OPNTYPE::EXPR, getRefNode(),
                        arg_[0]->getRefNode(), arg_[1]->getRefNode(), opCode_);
            }
            break;
        }

        // Short Circuit '||' Operator
        case OpCode::OR:
        {
            if (IsIfRelNeeded) {
                InterCode* arg0_false_l = LabelClass::assignLabel();

                arg_[0]->OnTrue(this->OnTrue());
                arg_[0]->OnFalse(arg0_false_l);

                if ((ic_cond = arg_[0]->codeGen()) != NULL) {
                    cls->addCode(ic_cond);
                } else {
                    cls->addCode(InterCode::OPNTYPE::IFREL, arg_[0]);
                }

                cls->addCode(arg0_false_l);
                 
                arg_[1]->OnTrue(this->OnTrue());
                arg_[1]->OnFalse(this->OnFalse());

                if ((ic_cond = arg_[1]->codeGen()) != NULL) {
                    cls->addCode(ic_cond);
                } else {
                    cls->addCode(InterCode::OPNTYPE::IFREL, arg_[1]);
                }
            } else {
                cls->addCode (arg_[0]->codeGen());
                cls->addCode (arg_[1]->codeGen());
                cls->addCode (InterCode::OPNTYPE::EXPR, getRefNode(),
                        arg_[0]->getRefNode(), arg_[1]->getRefNode(), opCode_);
            }

            break;
        }

        // UNARY Conditional Operators
        case OpCode::NOT: 
            if (IsIfRelNeeded) {
                arg_[0]->OnFalse(this->OnTrue());
                arg_[0]->OnTrue(this->OnFalse());

                if ((ic_cond = arg_[0]->codeGen()) != NULL) {
                    cls->addCode(ic_cond);
                } else {
                    cls->addCode(InterCode::OPNTYPE::IFREL, arg_[0]);
                }
            } else {
                cls->addCode (arg_[0]->codeGen());
                cls->addCode (InterCode::OPNTYPE::EXPR, getRefNode(), 
                        arg_[0]->getRefNode(), NULL, opCode_);
            }
            break;

        // BINARY Arithmetic Operators
        case OpCode::PLUS: 
        case OpCode::MINUS:
        case OpCode::MULT:
        case OpCode::DIV:
        case OpCode::MOD:
        case OpCode::BITAND:
        case OpCode::BITOR:
        case OpCode::BITXOR:
        case OpCode::SHL:
        case OpCode::SHR:
            cls->addCode (arg_[0]->codeGen());
            cls->addCode (arg_[1]->codeGen());
            cls->addCode (InterCode::OPNTYPE::EXPR, getRefNode(),
                    arg_[0]->getRefNode(), arg_[1]->getRefNode(), opCode_);
            break;

        // UNARY Arithmetic Operators
        case OpCode::BITNOT:
        case OpCode::UMINUS:
            cls->addCode (arg_[0]->codeGen());
            cls->addCode (InterCode::OPNTYPE::EXPR, getRefNode(), 
                    arg_[0]->getRefNode(), NULL, opCode_);
            break;

        case OpCode::ASSIGN:
            cls->addCode (arg_[0]->codeGen()); 
            cls->addCode (arg_[1]->codeGen()); 
            cls->addCode (InterCode::OPNTYPE::EXPR, arg_[0]->getRefNode(), 
                    arg_[1]->getRefNode(), NULL, opCode_);
            break; 

        case OpCode::INVALID:
        default:
            assert(0 && "Unhandled Opcode"); 
    }
    
    return cls;
}

