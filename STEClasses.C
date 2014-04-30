#include "STEClasses.h"
#include "Value.h"
#include "ParserUtil.h"

#define DEBUG 0
#define prt_off(s, i) \
               do { if (DEBUG) printf("\n Variable %s, Offset : %d", s, i); } while(0)

#define prt(s) \
               do { if (DEBUG) cout << s; } while(0)


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

void EventEntry::print(ostream& out, int indent) const
{
    out << type()->name() << " " << name();
    printST(out, indent, '(', ')', false);
    out << ";";
}

void ClassEntry::print(ostream& out, int indent) const
{
    out << type()->name() << " " << name();
    printST(out, indent, '{', '}', true, 0, 100000);
    out<<";";
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

const Type* GlobalEntry::typeCheck() const
{
    typeCheckST(); 

    vector<RuleNode*>::const_iterator it; 
    for (it = rules_.begin(); it != rules_.end(); ++it) {
        (*it)->typeCheck();    
    }
    return type();
}

void GlobalEntry :: memAlloc() 
{
    addr_mng.setAddress (AddressManage::OffKind::GLOBAL, 0);
    prt("\n Showing Global variables");
    memAllocST();

}


const Type* ClassEntry::typeCheck() const
{
    typeCheckST();
    return type();
}

void ClassEntry::memAlloc() 
{
    addr_mng.setAddress (AddressManage::OffKind::NONGLOBAL, 0);
    memAllocST();
}


const Type* VariableEntry::typeCheck() const
{
    const Type* t1 = type();
    if (initVal()) {
        const Type* t2 = initVal_->typeCheck();
        assert(t2 && "Invalid rvalue type");

        if (!t1->isSubType(t2->tag(), t2)) {
            errMsg("Assignment between incompatible types", initVal_);
        }
    }

    return t1; 
}

void VariableEntry::memAlloc() 
{   int off;
    bool flag = false;

    if (varKind() == VarKind::GLOBAL_VAR) {
        off = addr_mng.getAddress (AddressManage::OffKind::GLOBAL, type()->size(), INCR);
        offSet(off);
        flag = true;
    } else if (varKind() == VarKind::LOCAL_VAR) {
        off = addr_mng.getAddress (AddressManage::OffKind::NONGLOBAL, type()->size(), DECR);
        offSet(off);
        flag = true;
    } else if (varKind() == VarKind::PARAM_VAR) { 
        off = addr_mng.getAddress (AddressManage::OffKind::NONGLOBAL, type()->size(), INCR);
        offSet(off);
        flag = true; 
    }
    
    if(flag)
        prt_off(name().c_str(), off);
}


void FunctionEntry::memAlloc() 
{
    int numParams = type()->arity();
    
    prt(std::string("\n Entering function ") + name());
    if (numParams) {
        addr_mng.setAddress (AddressManage::OffKind::NONGLOBAL, 8);
        memAllocST (0, numParams);
    } 

    if (body_) {
        addr_mng.setAddress (AddressManage::OffKind::NONGLOBAL, 0);
        memAllocST (numParams, 10000);
    }
}

const Type* FunctionEntry::typeCheck() const
{
    const vector<const Type*>* param_l = type()->argTypes();
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

void EventEntry::memAlloc() 
{
    addr_mng.setAddress (AddressManage::OffKind::NONGLOBAL, 0);
    memAllocST();
}
const Type* EventEntry::typeCheck() const
{
    typeCheckST();
    return type();
}

void BlockEntry::print(ostream& out, int indent) const
{

}

