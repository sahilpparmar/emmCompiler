#include "Ast.h"
#include "InterCode.h"

#define RSP       " R000" 

#define RRV_I     " R001" 
#define RRV_F     " F001"

#define RSH_Cnt   " R002"           //Shift Register Counter 
#define RSH_Val   " R003"           //Shift Register Value 
#define RRET_ADD  " R004" 

// Used for Event Handling
#define R_PARAM   " R005"
#define F_PARAM   " F005"

#define S_START     "_S_START"
#define S_EVENT_END "_S_END"
#define S_EXIT      "_S_EXIT"
#define S_PREFIX    "_S_"
#define E_PREFIX    "_event_"

// Comments inlined in final codegen
#define C_APARAM    "           // Parameter Pushed on Stack"
#define C_FPARAM    "           // Parameter Popped from Stack"
#define C_PUSH_RET  "           // Return Addr Pushed on Stack"
#define C_POP_RET   "           // Return Addr Popped from Stack"
#define C_INIT_RSP  "           // RSP Initialized"
#define C_IN_EPARAM "           // READ Event Parameter Input"
#define C_DUMMY_INP "           // Dummy Instruction"

enum OPNTYPE {CALL, FPARAM, APARAM, RETURN, EXPR, LABEL, GOTO, IFREL, ENTER, LEAVE, PRINT};
string allocateNewRegName (bool isFloat = false);

class FinalMachineCodeGen {
    public:
        FinalMachineCodeGen() {}
        static void finalCodeGen (BasicBlocksContainer *bbCls, ostream & os);
        static void convert_IC_MC (InterCode *interCode, ostream &os);
};
