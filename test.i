
=================Lexical and Syntax Parsing==================

=========================AST Builder=========================
 
  event a(double xx, double yy);
  event b;
  string s1 = "\nParam 1: ";
  string s2 = "\nParam 2: ";
  void aa(int x) {
    int b;
    int a = (b+b);
  
    print("a", a);
  };
 
  (a(double i, double j))-->   {
    i=(i+j);
  };
  ;;
  (b())-->   {
    print("\nEmpty Event");
  };
  ;;

========================Type Checking========================

======================3 Addr Generation======================
global:
        s1 = "\nParam 1: "
        s2 = "\nParam 2: "
aa:
        enter aa
        fparam x
        __vreg1 = b + b
        a = __vreg1
        print "a"
        print a
        leave aa
_event_a:
        enter _event_a
        fparam j
        fparam i
        __vreg2 = i + j
        i = __vreg2
        leave _event_a
_event_b:
        enter _event_b
        print "\nEmpty Event"
        leave _event_b

====================== Basic Code Optimization ======================
global:
        s1 = "\nParam 1: "
        s2 = "\nParam 2: "
aa:
        enter aa
        fparam x
        __vreg1 = b + b
        a = __vreg1
        print "a"
        print a
        leave aa
_event_a:
        enter _event_a
        fparam j
        fparam i
        __vreg2 = i + j
        i = __vreg2
        leave _event_a
_event_b:
        enter _event_b
        print "\nEmpty Event"
        leave _event_b

====================Basic Block creation=====================

#####_event_a#####

_event_a:
        Prev: ( )

        enter _event_a
        fparam j
        fparam i
        __vreg2 = i + j
        i = __vreg2
        leave _event_a

        next: ( )


#####_event_b#####

_event_b:
        Prev: ( )

        enter _event_b
        print "\nEmpty Event"
        leave _event_b

        next: ( )


#####aa#####

aa:
        Prev: ( )

        enter aa
        fparam x
        __vreg1 = b + b
        a = __vreg1
        print "a"
        print a
        leave aa

        next: ( )


#####global#####

global:
        Prev: ( )

        s1 = "\nParam 1: "
        s2 = "\nParam 2: "

        next: ( )


=========================Optimization========================


=====Constant propogation and folding optimization=======
#####_event_a#####

_event_a:
        Prev: ( )

        enter _event_a
        fparam j
        fparam i
        __vreg2 = i + j
        i = __vreg2
        leave _event_a

        next: ( )


#####_event_b#####

_event_b:
        Prev: ( )

        enter _event_b
        print "\nEmpty Event"
        leave _event_b

        next: ( )


#####aa#####

aa:
        Prev: ( )

        enter aa
        fparam x
        __vreg1 = b + b
        a = __vreg1
        print "a"
        print a
        leave aa

        next: ( )


#####global#####

global:
        Prev: ( )

        s1 = "\nParam 1: "
        s2 = "\nParam 2: "

        next: ( )


========================= Final Optimized Code ========================

#####_event_a#####

_event_a:
        Prev: ( )

        enter _event_a
        fparam j
        fparam i
        __vreg2 = i + j
        i = __vreg2
        leave _event_a

        next: ( )


#####_event_b#####

_event_b:
        Prev: ( )

        enter _event_b
        print "\nEmpty Event"
        leave _event_b

        next: ( )


#####aa#####

aa:
        Prev: ( )

        enter aa
        fparam x
        __vreg1 = b + b
        a = __vreg1
        print "a"
        print a
        leave aa

        next: ( )


#####global#####

global:
        Prev: ( )

        s1 = "\nParam 1: "
        s2 = "\nParam 2: "

        next: ( )


===================Final Code generation=====================
JMP begin
_S_a: MOVL _S_START R005
STI R005 R000           // Return Addr Pushed on Stack
SUB R000 4 R000
INF F005           // READ Event Parameter Input
STF F005 R000           // Parameter Pushed on Stack
SUB R000 4 R000
INF F005           // READ Event Parameter Input
STF F005 R000           // Parameter Pushed on Stack
SUB R000 4 R000
JMP _event_a

_S_b: MOVL _S_START R005
STI R005 R000           // Return Addr Pushed on Stack
SUB R000 4 R000
JMP _event_b

_S_START: PRTS "\nEnter event name ('0' for exit):"
IN R005
JMPC EQ R005 97 _S_a
JMPC EQ R005 98 _S_b
JMPC EQ R005 48 _S_END
PRTS "Invalid Input\n"
JMP _S_START

begin: MOVI  10000 R000           // RSP Initialized
JMP global

// Function/Event Module begins
_event_a: ADD  R000 4  R000
LDF  R000 F010           // Parameter Poped from Stack
ADD  R000 4  R000
LDF  R000 F011           // Parameter Poped from Stack
FADD F011 F010 F012
MOVF F012 F011
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
// Function/Event Module Ends

// Function/Event Module begins
_event_b: PRTS "\nEmpty Event"
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
// Function/Event Module Ends

// Function/Event Module begins
aa: ADD  R000 4  R000
LDI  R000 R020           // Parameter Poped from Stack
ADD R021 R021 R022
MOVI R022 R023
PRTS "a"
PRTI R023
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
// Function/Event Module Ends

global: MOVS "\nParam 1: " R024
MOVS "\nParam 2: " R025
JMP _S_START

_S_END: PRTS "DONE"


Compilation Successful
