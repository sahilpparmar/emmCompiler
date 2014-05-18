#include "STEClasses.h"
#include "Value.h"
#include "ParserUtil.h"
#include "InterCode.h"
#include "FinalMachineCodeGen.h"

#define TO_INT(str) ((int)(str).at(0))

void GlobalEntry::eventHandler(ostream& os) {
    vector<EventEntry*> event_l;
    EventEntry *event;
    vector<Type*>* argtype_l;

    // Store all EventEntry definitions
    vector<RuleNode*>::const_iterator it; 
    for (it = rules_.begin(); it != rules_.end(); ++it) {
        if ((*it)->pat()->kind() == BasePatNode::PatNodeKind::PRIMITIVE) {
            event_l.push_back(((PrimitivePatNode*)(*it)->pat())->event());
        }
    }

    os << "JMP begin" << endl;
    // Generate code to input event parameter
    for (unsigned int ii = 0; ii < event_l.size(); ii++) {
        event = event_l[ii];
        os << S_PREFIX << event->name() << ": ";
        
        argtype_l = event->type()->argTypes();

        os << "MOVL " << S_START << R_PARAM << endl;
        os << "STI" << R_PARAM << RSP << C_PUSH_RET << endl;
        os << "SUB" << RSP << " 4" << RSP << endl;
        
        if (argtype_l) {
            std::vector<Type*>::iterator type_it = argtype_l->begin();
            for (; type_it != argtype_l->end(); ++type_it) {

                if ((*type_it)->tag() == Type::DOUBLE) {
                    os << "INF" << F_PARAM << C_IN_EPARAM << endl;
                    os << "STF" << F_PARAM << RSP << C_APARAM << endl;
                } else {
                    os << "INI" << R_PARAM << C_IN_EPARAM << endl;
                    os << "STI" << R_PARAM << RSP << C_APARAM << endl;
                }
                os << "SUB" << RSP << " 4" << RSP << endl;
            }
        }

        os << "JMP " << E_PREFIX << event->name() << endl;
        os << endl;
    }

    os << S_START << ": ";
    os << "PRTS " << "\"\\nEnter event name ('0' for exit):\"" << endl;
    os << "IN" << R_PARAM << endl;

    // Generate code to select appropriate event
    for (unsigned int ii = 0; ii < event_l.size(); ii++) {
        event = event_l[ii];
        os << "JMPC EQ" << R_PARAM << " " << TO_INT(event->name()) << " " << S_PREFIX << event->name() << endl;
    }
    os << "JMPC EQ" << R_PARAM << " " << (int)'0' << " " << S_END << endl;
    os << "PRTS " << "\"Invalid Input\\n\"" << endl;
    os << "JMP " << S_START << endl << endl;

}

