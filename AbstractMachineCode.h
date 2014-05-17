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
#define S_END       "_S_END"
#define S_PREFIX    "_S_"
#define E_PREFIX    "_event_"

// Comments inlined in final codegen
#define C_APARAM    "           // Parameter Pushed on Stack"
#define C_FPARAM    "           // Parameter Poped from Stack"
#define C_PUSH_RET  "           // Return Addr Pushed on Stack"
#define C_POP_RET   "           // Return Addr Poped from Stack"
#define C_INIT_RSP  "           // RSP Initialized"
#define C_IN_EPARAM "           // READ Event Parameter Input"

enum OPNTYPE {CALL, FPARAM, APARAM, RETURN, EXPR, LABEL, GOTO, IFREL, ENTER, LEAVE, PRINT};
string allocateNewRegName (bool isFloat = false);

class AbstractMachineCode {
    public:
        AbstractMachineCode() {}
        static void genAMC (BasicBlocksContainer *bbCls, ostream & os);
        static void convert_IC_AMC (InterCode *interCode, ostream &os);
};
