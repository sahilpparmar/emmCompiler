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
_event_a: MOVL L12 R020
STI  R020  R000           // Return Addr Pushed on Stack
SUB  R000 4  R000
JMP  foo
L12: ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
// Function/Event Module Ends

// Function/Event Module begins
foo: MOVI 10 R021
MOVI 10 R022
L2: PRTS "\n outer while: "
ADD R022 1 R023
MOVI R023 R022
JMPC EQ R022 15 L5
JMP L4
L5: PRTS "\nexit"
JMP L3
L4: JMP L6
L6: JMPC LE R021 15 L7
JMP L8
L7: ADD R021 1 R024
MOVI R024 R021
PRTS "\n inner while: "
JMPC EQ R021 13 L10
JMP L9
L10: PRTS "\ncontinue to outer while"
JMP L2
L9: JMP L6
L8: JMP L2
L3: ADD  R000 4  R000
LDI  R000  R004           // Return Addr Popped from Stack
JMPI  R004
// Function/Event Module Ends

global: JMP _S_START

_S_EXIT: PRTS "Successfully Exited\n"

