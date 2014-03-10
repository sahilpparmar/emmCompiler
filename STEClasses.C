#include "STEClasses.h"
#include "Value.h"
#include "ParserUtil.h"

void GlobalEntry::print(ostream& out, int indent) const
{
    printST(out, indent, ' ', ' ');
    
    for (unsigned int  ii = 0; ii < rules_.size(); ii++) {
        rules_[ii]->print(out, indent);
    }
    // Add your code
}

void EventEntry::print(ostream& out, int indent) const
{
    out << type()->name() << " " << name();
    printST(out, indent, '(', ')', false);
    out << ";";
}

void ClassEntry::print(ostream& out, int indent) const
{
    out << type()->name() << " " << name() << ";";
}

//SymTab::printST(ostream& os, int indent, char leftdelim, char rightdelim, 
//        bool linebreak, int first, int last) const {

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
        body_->printWithoutBraces(out, indent+STEP_INDENT);

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

void BlockEntry::print(ostream& out, int indent) const
{
}

