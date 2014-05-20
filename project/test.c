#include "E--_RT.c"
int main(int argc, char *argv[]) {
	E_RT_init(argc, argv);
JMP(begin);
_S_a: MOVL(_S_END, R005);
STI(R005, R000);
R000=SUB(R000,4);
JMP(_event_a);
_S_END: IN(R005);
_S_START: PRTS("\nEnter Event Name ('0' for exit): ");
IN(R005);
JMPC(EQ(R005, 97), _S_a);
JMPC(EQ(R005, 48), _S_EXIT);
PRTS("Invalid Event Name\n");
JMP(_S_END);
begin: MOVI(10000, R000);
JMP(global);
_event_a: MOVL(L6, R020);
STI(R020, R000);
R000=SUB(R000,4);
JMP(dead);
L6: PRTS("\nReturn from dead()");
R000=ADD(R000,4);
LDI(R000, R004);
JMPI(R004);
dead: MOVI(3, R021);
MOVI(4, R022);
JMPC(NE(R023, 0), L4);
L3: R024=ADD(R023,R021);
MOVI(R024, R023);
PRTS("Hello ");
PRTI(R023);
JMP(L2);
L4: R025=ADD(R023,R022);
MOVI(R025, R023);
PRTS("World ");
PRTI(R023);
L2: R000=ADD(R000,4);
LDI(R000, R004);
JMPI(R004);
global: MOVI(1, R023);
MOVI(2, R026);
JMP(_S_START);
_S_EXIT: PRTS("Successfully Exited\n");

	E_RT_exit(); return 0;
}
