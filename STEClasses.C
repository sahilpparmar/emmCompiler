#include "STEClasses.h"
#include "Value.h"
#include "ParserUtil.h"
#include "InterCode.h"
#include "FinalMachineCodeGen.h"

#define PRINT_OFFSET(s, i) DEBUG((string)"Variable: " + (string)s + (string)", Offset: " + (string)itoa(i));

static AddressManage addr_mng;

void GlobalEntry::print(ostream& out, int indent) const
{
    printST(out, indent, ' ', ' ');

    out << endl;
    for (unsigned int  ii = 0; ii < rules_.size(); ii++) {
        rules_[ii]->print(out, indent+STEP_INDENT);
        out << endl;
    }
}

const Type* GlobalEntry::typeCheck()
{
    typeCheckST(); 

    vector<RuleNode*>::const_iterator it; 
    for (it = rules_.begin(); it != rules_.end(); ++it) {
        (*it)->typeCheck();    
    }
    return type();
}


InterCodesClass* GlobalEntry::codeGen()
{
    InterCodesClass *cls = new InterCodesClass();

    if (symTab()) {
        // Assign new global label
        cls->addCode(LabelClass::assignLabel ("global"));
        
        SymTab::iterator it = symTab()->begin();
        for(; it != symTab()->end(); ++it) {
            if (*it) 
                cls->addCode((*it)->codeGen());
        }
    }

    vector<RuleNode*>::const_iterator it; 
    for (it = rules_.begin(); it != rules_.end(); ++it) {
        cls->addCode((*it)->codeGen());
    }
    
    return cls;
}

void GlobalEntry::memAlloc() 
{
    DEBUG("\n====Global Declarations");
    addr_mng.setAddress (AddressManage::OffKind::GLOBAL, 0);
    memAllocST();

}

void ClassEntry::print(ostream& out, int indent) const
{
    out << type()->name() << " " << name();
    printST(out, indent, '{', '}', true, 0, 100000);
    out<<";";
}

const Type* ClassEntry::typeCheck()
{
    typeCheckST();
    return type();
}

void ClassEntry::memAlloc() 
{
    DEBUG("\n====Class '" + name() + "'");
    addr_mng.setAddress (AddressManage::OffKind::NONGLOBAL, 0);
    memAllocST();
    DEBUG("");
}

void VariableEntry::print(ostream& out, int indent) const
{
    // GLOBAL_VAR, LOCAL_VAR, PARAM_VAR, UNDEFINED
    out << type()->fullName() << " " << name();
    if (varKind() != PARAM_VAR) {
        if (initVal()) {
            out << " = ";
            initVal()->print(out, indent);
        }
        out << ";";
    }   
}

string VariableEntry::getRegisterName()
{
    if(registerName_.length() != 0) return registerName_ ;
    Type *type_ = type();
//    cout<<"\n tag is: "<<type_->tag();
    if(!type_) type_ = type();
    (type_ && type_->isFloat(type_->tag())) ? setRegisterName(allocateNewRegName(true)) : setRegisterName(allocateNewRegName(false)) ; 
    return registerName_;
}

void   VariableEntry::setRegisterName(string reg_name)
{
    registerName_ = reg_name;
}

InterCodesClass* VariableEntry::codeGen() 
{
    InterCodesClass *cls = NULL;
    //if initval exists, create a refexprnode
    if (initVal_) {
        cls               = new InterCodesClass(); 
        RefExprNode *node = new RefExprNode (name(), this);
        cls->addCode(initVal_->codeGen()); 
        cls->addCode(InterCode::OPNTYPE::EXPR, node, initVal_->getRefNode(), NULL, OpNode::OpCode::ASSIGN); 

    } else if (varKind() == PARAM_VAR) {
        cls               = new InterCodesClass(); 
        RefExprNode *node = new RefExprNode (name(), this);
        cls->addCode(InterCode::OPNTYPE::FPARAM, node);
    }
    return cls;
}

const Type* VariableEntry::typeCheck()
{
    const Type* t1 = type();
    if (initVal()) {
        const Type* t2 = initVal_->typeCheck();
        assert(t2 && "Invalid rvalue type");

        if (!t1->isSubType(t2->tag(), t2)) {
            errMsg("Assignment between incompatible types", initVal_);
        }
    cout<<"\n event type: "<<t1->tag();
    }

    return t1; 
}

void VariableEntry::memAlloc() 
{   int off;

    if (varKind() == VarKind::GLOBAL_VAR) {
        off = addr_mng.getAddress (AddressManage::OffKind::GLOBAL, type()->size(), INCR);
        offSet(off);
    
    } else if (varKind() == VarKind::LOCAL_VAR) {
        off = addr_mng.getAddress (AddressManage::OffKind::NONGLOBAL, type()->size(), DECR);
        offSet(off);
    
    } else if (varKind() == VarKind::PARAM_VAR) { 
        off = addr_mng.getAddress (AddressManage::OffKind::NONGLOBAL, type()->size(), INCR);
        offSet(off);

    } else if (varKind() == VarKind::CLASS_VAR) { 
        off = addr_mng.getAddress (AddressManage::OffKind::NONGLOBAL, type()->size(), INCR);
        offSet(off);

    } else {
        assert(0 && "Invalid Variable Symbol");
    }
    
    PRINT_OFFSET(name().c_str(), off);
}

InterCodesClass* FunctionEntry:: codeGen() {
    InterCodesClass* cls = NULL;
    if (body_) {
        cls = new InterCodesClass();
        cls->addCode(LabelClass::assignLabel (name()));
        cls->addCode(InterCode::OPNTYPE::ENTER, this);
        
        if (symTab()) {
            
            SymTab::iterator it = symTab()->begin();
            for(; it != symTab()->end(); ++it) {
                if ((*it) && (*it)->kind() == SymTabEntry::Kind::VARIABLE_KIND ) 
                    cls->addCode((*it)->codeGen());
            }
        }
        cls->addCode(body_->codeGen());

        cls->addCode(InterCode::OPNTYPE::LEAVE, this);

        return cls; 
    }
    return NULL;
}

void FunctionEntry::print(ostream& out, int indent) const
{
    const vector<const Type*>* param_l = type()->argTypes();
    int numParams = param_l ? param_l->size() : 0;

    // Function retType and name
    out << type()->retType()->name() << " " << name();

    // Function formal params 
    if (numParams) {
        
        printST(out, indent, '(', ')', false, 0, numParams);
    } else {
        out << "()";
    }
    // Function body Definition
    if (body_) {
        out << " {" << endl;
        prtSpace(out, indent+STEP_INDENT);

        // Local Declarations
        printST(out, indent, '\0', '\0', true, numParams, 100000);
        // Function Body
        body_->printWithoutBraces(out, indent);

        prtSpace(out, indent);
        out << "}";
    }
    out << ";";
}

const Type* FunctionEntry::typeCheck()
{
    vector<Type*>* param_l = type()->argTypes();
    int numParams = param_l ? param_l->size() : 0;
    
    if (numParams) {
        typeCheckST(0, numParams);
    } 
    if (body_) {
        typeCheckST(numParams, 10000); 
        body_->typeCheck();
    }
    return type();
}    

void FunctionEntry::memAlloc() 
{
    int numParams = type()->arity();

    DEBUG("\n====Function '" + name() + "'");
    if (numParams) {
        addr_mng.setAddress (AddressManage::OffKind::NONGLOBAL, 8);
        DEBUG("==Formal Parameters");
        memAllocST (0, numParams);
    } 

    if (body_) {
        addr_mng.setAddress (AddressManage::OffKind::NONGLOBAL, 0);
        DEBUG("==Local Variables");
        memAllocST (numParams, 10000);
    }
    DEBUG("");
}

void EventEntry::print(ostream& out, int indent) const
{
    out << type()->name() << " " << name();
    printST(out, indent, '(', ')', false);
    out << ";";
}

const Type* EventEntry::typeCheck()
{
    typeCheckST();
    return type();
}

void EventEntry::memAlloc() 
{
    DEBUG("\n====Event '" + name() + "'");
    addr_mng.setAddress (AddressManage::OffKind::NONGLOBAL, 0);
    memAllocST();
    DEBUG("");
}

void BlockEntry::print(ostream& out, int indent) const
{

}

