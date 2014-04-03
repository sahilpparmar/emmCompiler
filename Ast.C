#include "Ast.h"
#include "ParserUtil.h"


AstNode::AstNode(NodeType nt, int line, int column, string file):
    ProgramElem(NULL, line, column, file) 
{

}

AstNode::AstNode(const AstNode& ast): ProgramElem(ast) 
{

}

/****************************************************************/

ExprNode::ExprNode(ExprNodeType et, const Value* val, int line, int column, 
        string file):
    AstNode(AstNode::NodeType::EXPR_NODE, line, column, file)
{
    exprType_ = et;
    val_ = val;
}


ExprNode::ExprNode(const ExprNode& e) : AstNode(e)
{

}

/****************************************************************/

RefExprNode::RefExprNode(string ext, const SymTabEntry* ste, 
        int line, int column, string file): 
    ExprNode(ExprNode::ExprNodeType::REF_EXPR_NODE, 0, line, column, file)
{
    ext_ = ext;
    sym_ = ste;
}

RefExprNode::RefExprNode(const RefExprNode& ref) : ExprNode(ref)
{

}

void RefExprNode::print(ostream& os, int indent) const
{
    os << ext_;
}

const Type* RefExprNode::typeCheck() const {
    return sym_->type();
}
/****************************************************************/

void ValueNode::print(ostream& os, int indent) const
{
    value()->print(os, indent); 
}

const Type* ValueNode::typeCheck() const {
    //cout<<"\n valnode";
    return value()->type(); 
}

/****************************************************************/

void ExprStmtNode::print(ostream& os, int indent) const {
    if (expr_ != NULL) { 
        prtSpace(os, indent);
        expr_->print(os, indent);
    }
    os << ";"; 
}

const Type* ExprStmtNode::typeCheck() const {
    if (expr_ != NULL) { 
        return expr_->typeCheck();
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

/****************************************************************/

void ReturnStmtNode::print(ostream& os, int indent) const 
{
    prtSpace(os, indent);
    os << "return ";
    if (expr_ != NULL) expr_->print(os, indent);
    else os << "NULL";
    os << ";";
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

const Type* CompoundStmtNode::typeCheck() const {
    
    if (stmts_ == NULL || stmts_->size() == 0) return NULL;

    for (std::list<StmtNode*>::iterator it = stmts_->begin(); it != stmts_->end(); it++) {
        (*it)->typeCheck();
    }
    return NULL;
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
                errMsg("Invalid event parameters");
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
                errMsg("Invalid PatNodeKind");
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

/****************************************************************/

extern const OpNode::OpInfo opInfo[] = {
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

const Type* OpNode::typeCheck() const {

    int iopcode = static_cast<int>(opCode_);

    //cout << "OPNODE!!";
    if (opInfo[iopcode].prtType_ == OpNode::OpPrintType::PREFIX) {
        //cout << "PREFIX";
        const Type *targ = NULL;

        if (arity_ > 0) {
            for (unsigned i = 0; i < arity_; i++) {
                if (arg_[i]) {
                    targ = arg_[i]->typeCheck();

                    if (targ->tag() == Type::TypeTag::DOUBLE 
                     || targ->tag() == Type::TypeTag::INT
                     || targ->tag() == Type::TypeTag::UINT)
                    ;
                    else
                        errMsg("Not matching");
                }
            }
        }
        // TODO: For now returning signed integer return type "INT"
        return new Type(Type::INT);
    }

    else if ((opInfo[iopcode].prtType_ == OpNode::OpPrintType::INFIX) && (arity_ == 2)) {
        const Type* targ1, *targ2;

        targ1 = arg_[0]->typeCheck();
        targ2 = arg_[1]->typeCheck();

        assert(arg_[0] && arg_[1] && "Invalid args");

        if (targ1->tag() == Type::TypeTag::DOUBLE 
         || targ1->tag() == Type::TypeTag::INT
         || targ1->tag() == Type::TypeTag::UINT)
        ;
        else
            cout<<"\n type not satisfied op1";

        if (targ2->tag() == Type::TypeTag::DOUBLE 
         || targ2->tag() == Type::TypeTag::INT
         || targ2->tag() == Type::TypeTag::UINT)
        ;
        else
            cout<<"\n type not satisfied op2";

        // TODO: For now returning targ1
        return targ1;
    }
    return NULL;
}
