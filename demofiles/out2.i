JMP begin
_S_a: MOVL _S_END R005
STI R005 R000           // Return Addr Pushed on Stack
SUB R000 4 R000
PRTS"Enter Param 1: "
INI R005           // READ Event Parameter Input
STI R005 R000           // Parameter Pushed on Stack
SUB R000 4 R000
PRTS"Enter Param 2: "
INI R005           // READ Event Parameter Input
STI R005 R000           // Parameter Pushed on Stack
SUB R000 4 R000
JMP _event_a

_S_END: IN R005           // Dummy Instruction
_S_START: PRTS "\nEnter Event Name ('0' for exit):"
IN R005
JMPC EQ R005 97 _S_a
JMPC EQ R005 48 _S_EXIT
PRTS "Invalid Event Name\n"
JMP _S_END

begin: MOVI  10000 R000           // RSP Initialized
JMP global

// Function/Event Module begins
_event_a: ADD  R000 4  R000
LDI  R000 R020           // Parameter Popped from Stack
ADD  R000 4  R000
LDI  R000 R021           // Parameter Popped from Stack
PRTS "\nFirst Param :"
PRTI R021
STI  R021  R000
SUB  R000 4  R000
STI  R020  R000
SUB  R000 4  R000
MOVL L14 R022
STI  R022  R000           // Return Addr Pushed on Stack
SUB  R000 4  R000
STI R021  R000           // Parameter Pushed on Stack
SUB  R000 4  R000
JMP  foo
L14: ADD  R000 4  R000
LDI  R000 R020
ADD  R000 4  R000
LDI  R000 R021
MOVI  R001 R023
L12: MOVI R023 R021
PRTS "\nResult(fac) :"
PRTI R021
STI  R023  R000
SUB  R000 4  R000
STI  R022  R000
SUB  R000 4  R000
MOVL L15 R024
STI  R024  R000           // Return Addr Pushed on Stack
SUB  R000 4  R000
STI R021  R000           // Parameter Pushed on Stack
SUB  R000 4  R000
JMP  bar
L15: ADD  R000 4  R000
LDI  R000 R022
ADD  R000 4  R000
LDI  R000 R023
MOVI  R001 R025
L13: MOVI R025 R021
PRTS "\nResult(fac) :"
PRTI R021
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
// Function/Event Module Ends

// Function/Event Module begins
bar: ADD  R000 4  R000
LDI  R000 R026           // Parameter Popped from Stack
MOVI 30 R027
MOVI 30 R028
MOVI 3 R029
MOVI 3 R030
MOVI 1000 R031
MOVI 1001 R032
MOVI 1001 R033
MOVI 90 R034
MOVI 90 R035
MOVI -1000 R036
MOVI -1000 R037
MOVI 0 R038
MOVI 0 R039
MOVI 5 R040
MOVI 5 R041
MOVI 0 R042
MOVI 0 R043
MOVI 0 R044
MOVI 1 R045
MOVI 1 R046
MOVI 0 R047
MOVI 0 R048
MOVI 0 R049
MOVI 0 R050
MOVI 4 R051
MOVI 4 R052
MOVI 25 R053
MOVI 25 R054
ADD R026 1 R055
MOVI R055  R001
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
// Function/Event Module Ends

// Function/Event Module begins
foo: ADD  R000 4  R000
LDI  R000 R056           // Parameter Popped from Stack
MOVI 10 R056
JMPC NE R056 0 L5
L4: MOVI 2 R056
JMP L3
L5: MOVI 3 R056
L3: JMPC NE R056 1 L8
L7: MOVI 2 R056
JMP L6
L8: STI  R056  R000
SUB  R000 4  R000
MOVL L16 R057
STI  R057  R000           // Return Addr Pushed on Stack
SUB  R000 4  R000
STI R056  R000           // Parameter Pushed on Stack
SUB  R000 4  R000
JMP  bar
L16: ADD  R000 4  R000
LDI  R000 R056
MOVI  R001 R058
L9: ADD 3 R058 R059
STI  R059  R000
SUB  R000 4  R000
STI  R058  R000
SUB  R000 4  R000
MOVL L17 R060
STI  R060  R000           // Return Addr Pushed on Stack
SUB  R000 4  R000
STI R056  R000           // Parameter Pushed on Stack
SUB  R000 4  R000
JMP  bar
L17: ADD  R000 4  R000
LDI  R000 R058
ADD  R000 4  R000
LDI  R000 R059
MOVI  R001 R061
L10: MUL R061 2 R062
ADD R059 R062 R063
MOVI R063 R056
L6: MOVI R056  R001
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
// Function/Event Module Ends

global: JMP _S_START

_S_EXIT: PRTS "DONE"

