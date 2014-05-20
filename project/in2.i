JMP begin
_S_a: MOVL _S_END R005
STI R005 R000           // Return Addr Pushed on Stack
SUB R000 4 R000
JMP _event_a

_S_END: IN R005           // Dummy Instruction
_S_START: PRTS "\nEnter Event Name ('0' for exit): "
IN R005
JMPC EQ R005 97 _S_a
JMPC EQ R005 48 _S_EXIT
PRTS "Invalid Event Name\n"
JMP _S_END

begin: MOVI  10000 R000           // RSP Initialized
JMP global

// Function/Event Module begins
_event_a: MOVL L3 R020
STI  R020  R000           // Return Addr Pushed on Stack
SUB  R000 4  R000
JMP  opr
L3: ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
// Function/Event Module Ends

global: JMP _S_START
// Function/Event Module begins
opr: MOVI 30 R021
MOVI 30 R022
NEG 2 R023
SUB 30 R023 R024
MOVI R024 R025
MOVI 1000 R026
MOVI 1001 R027
MOVI 1001 R028
MUL 30 R025 R029
MOVI R029 R030
MOVI -1000 R031
MOVI -1000 R032
DIV R025 4 R033
MOVI R033 R034
MOVI 5 R035
MOVI 5 R036
MOVI 0 R037
MOVI 0 R038
MOVI 0 R039
MOVI 1 R040
MOVI 1 R041
MOVI R042 R043
MOVI 0 R044
MOVI 0 R045
MOVI 120 R046
MOVI 120 R047
MOVI 250 R048
MOVI 250 R049
MOD 30 R025 R050
MUL 1001 R030 R051
DIV R051 3 R052
SUB R050 R052 R053
MOVIF R053 F011
MOVF F011 F010
PRTS "\n a = (10+20): "
PRTI R022
PRTS "\n b = (a-(-2)): "
PRTI R025
PRTS "\n c = (1+10*100): "
PRTI R028
PRTS "\n d = (a*b): "
PRTI R030
PRTS "\n e = (1-c): "
PRTI R032
PRTS "\n f = (b/4): "
PRTI R034
PRTS "\n g = (10/2): "
PRTI R036
PRTS "\n h = (10%2): "
PRTI R038
PRTS "\n i = (1|1&0): "
PRTI R041
PRTS "\n j = (~i): "
PRTI R043
PRTS "\n k = (1^1): "
PRTI R045
PRTS "\n l = (a<<2): "
PRTI R047
PRTS "\n m = (c>>2): "
PRTI R049
PRTS "\n n = (a % b - c * d / 3): "
PRTF F010
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
// Function/Event Module Ends


_S_EXIT: PRTS "Successfully Exited\n"

