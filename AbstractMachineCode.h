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

enum OPNTYPE {CALL, FPARAM, APARAM, RETURN, EXPR, LABEL, GOTO, IFREL, ENTER, LEAVE, PRINT};
string allocateNewRegName (bool isFloat = false);

class AbstractMachineCode {
    public:
        AbstractMachineCode() {}
        static void genAMC (BasicBlocksContainer *bbCls, ostream & os);
        static void convert_IC_AMC (InterCode *interCode, ostream &os);
};
