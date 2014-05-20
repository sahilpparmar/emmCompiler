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
_event_a: MOVL L6 R020
STI  R020  R000           // Return Addr Pushed on Stack
SUB  R000 4  R000
JMP  dead
L6: PRTS "\nReturn from dead()"
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
// Function/Event Module Ends

// Function/Event Module begins
dead: MOVI 3 R021
MOVI 4 R022
JMPC NE R023 0 L4
L3: ADD R023 R021 R024
MOVI R024 R023
PRTS "Hello "
PRTI R023
JMP L2
L4: ADD R023 R022 R025
MOVI R025 R023
PRTS "World "
PRTI R023
L2: ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
// Function/Event Module Ends

global: MOVI 1 R023
MOVI 2 R026
JMP _S_START

_S_EXIT: PRTS "Successfully Exited\n"

