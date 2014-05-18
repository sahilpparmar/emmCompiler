
=================Lexical and Syntax Parsing==================

=========================AST Builder=========================
 
  event a(double aa, double bb);
  double fac(double b, double c) {
    bool d = b>c;
  
    print("b>c: ", d);
    d=b>=c;
    print("b>=c: ", d);
    d=b<c;
    print("b<c: ", d);
    d=b<=c;
    print("b<=c: ", d);
    d=b==c;
    print("b==c: ", d);
    d=b!=c;
    print("b!=c: ", d);
    return b;
  };
 
  (a(double i, double j))-->   {
    print("\nFirst Param :", i);
    i=fac(i, j);
    print("\nResult(fac) :", i);
    i=(i+j);
    print("\n i+j: ", i);
  };
  ;;

========================Type Checking========================

======================3 Addr Generation======================
global:
fac:
        enter fac
        fparam b
        fparam c
        __vreg1 = b > c
        d = __vreg1
        print "b>c: "
        print d
        __vreg2 = b >= c
        d = __vreg2
        print "b>=c: "
        print d
        __vreg3 = b < c
        d = __vreg3
        print "b<c: "
        print d
        __vreg4 = b <= c
        d = __vreg4
        print "b<=c: "
        print d
        __vreg5 = b == c
        d = __vreg5
        print "b==c: "
        print d
        __vreg6 = b != c
        d = __vreg6
        print "b!=c: "
        print d
        return b
        leave fac
_event_a:
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam j
        aparam i
        __vreg7 = call fac
L3:
        i = __vreg7
        print "\nResult(fac) :"
        print i
        __vreg8 = i + j
        i = __vreg8
        print "\n i+j: "
        print i
        leave _event_a

====================== Basic Code Optimization ======================
global:
fac:
        enter fac
        fparam b
        fparam c
        __vreg1 = b > c
        d = __vreg1
        print "b>c: "
        print d
        __vreg2 = b >= c
        d = __vreg2
        print "b>=c: "
        print d
        __vreg3 = b < c
        d = __vreg3
        print "b<c: "
        print d
        __vreg4 = b <= c
        d = __vreg4
        print "b<=c: "
        print d
        __vreg5 = b == c
        d = __vreg5
        print "b==c: "
        print d
        __vreg6 = b != c
        d = __vreg6
        print "b!=c: "
        print d
        return b
        leave fac
_event_a:
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam j
        aparam i
        __vreg7 = call fac
L3:
        i = __vreg7
        print "\nResult(fac) :"
        print i
        __vreg8 = i + j
        i = __vreg8
        print "\n i+j: "
        print i
        leave _event_a


====================Basic Block creation=====================

#####_event_a#####

_event_aPrev: ( )
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam j
        aparam i
        __vreg7 = call fac
        next: ( L3 )

L3Prev: ( _event_a )
        i = __vreg7
        print "\nResult(fac) :"
        print i
        __vreg8 = i + j
        i = __vreg8
        print "\n i+j: "
        print i
        leave _event_a
        next: ( )


#####fac#####

facPrev: ( )
        enter fac
        fparam b
        fparam c
        __vreg1 = b > c
        d = __vreg1
        print "b>c: "
        print d
        __vreg2 = b >= c
        d = __vreg2
        print "b>=c: "
        print d
        __vreg3 = b < c
        d = __vreg3
        print "b<c: "
        print d
        __vreg4 = b <= c
        d = __vreg4
        print "b<=c: "
        print d
        __vreg5 = b == c
        d = __vreg5
        print "b==c: "
        print d
        __vreg6 = b != c
        d = __vreg6
        print "b!=c: "
        print d
        return b
        leave fac
        next: ( )


#####global#####

globalPrev: ( )
        next: ( )

globalPrev: ( )
        next: ( )


=========================Optimization========================


=====Constant propogation and folding optimization=======
#####_event_a#####

_event_aPrev: ( )
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam j
        aparam i
        __vreg7 = call fac
        next: ( L3 )

L3Prev: ( _event_a )
        i = __vreg7
        print "\nResult(fac) :"
        print i
        __vreg8 = __vreg7 + j
        i = __vreg8
        print "\n i+j: "
        print i
        leave _event_a
        next: ( )


#####fac#####

facPrev: ( )
        enter fac
        fparam b
        fparam c
        __vreg1 = b > c
        d = __vreg1
        print "b>c: "
        print d
        __vreg2 = b >= c
        d = __vreg2
        print "b>=c: "
        print d
        __vreg3 = b < c
        d = __vreg3
        print "b<c: "
        print d
        __vreg4 = b <= c
        d = __vreg4
        print "b<=c: "
        print d
        __vreg5 = b == c
        d = __vreg5
        print "b==c: "
        print d
        __vreg6 = b != c
        d = __vreg6
        print "b!=c: "
        print d
        return b
        leave fac
        next: ( )


#####global#####

globalPrev: ( )
        next: ( )

globalPrev: ( )
        next: ( )



 blockName: _event_a
live vars at start: 
live vars at End: __vreg7,j,

 blockName: L3
live vars at start: __vreg7,j,
live vars at End: 

 blockName: fac
live vars at start: 
live vars at End: 

=========Dead Code Elmination Optimization==================
#####_event_a#####

_event_aPrev: ( )
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam j
        aparam i
        __vreg7 = call fac
        next: ( L3 )

L3Prev: ( _event_a )
        i = __vreg7
        print "\nResult(fac) :"
        print i
        __vreg8 = __vreg7 + j
        i = __vreg8
        print "\n i+j: "
        print i
        leave _event_a
        next: ( )


#####fac#####

facPrev: ( )
        enter fac
        fparam b
        fparam c
        __vreg1 = b > c
        d = __vreg1
        print "b>c: "
        print d
        __vreg2 = b >= c
        d = __vreg2
        print "b>=c: "
        print d
        __vreg3 = b < c
        d = __vreg3
        print "b<c: "
        print d
        __vreg4 = b <= c
        d = __vreg4
        print "b<=c: "
        print d
        __vreg5 = b == c
        d = __vreg5
        print "b==c: "
        print d
        __vreg6 = b != c
        d = __vreg6
        print "b!=c: "
        print d
        return b
        leave fac
        next: ( )


#####global#####

globalPrev: ( )
        next: ( )

globalPrev: ( )
        next: ( )


#####_event_a#####

_event_aPrev: ( )
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam j
        aparam i
        __vreg7 = call fac
        next: ( L3 )

L3Prev: ( _event_a )
        i = __vreg7
        print "\nResult(fac) :"
        print i
        __vreg8 = __vreg7 + j
        i = __vreg8
        print "\n i+j: "
        print i
        leave _event_a
        next: ( )


#####fac#####

facPrev: ( )
        enter fac
        fparam b
        fparam c
        __vreg1 = b > c
        d = __vreg1
        print "b>c: "
        print d
        __vreg2 = b >= c
        d = __vreg2
        print "b>=c: "
        print d
        __vreg3 = b < c
        d = __vreg3
        print "b<c: "
        print d
        __vreg4 = b <= c
        d = __vreg4
        print "b<=c: "
        print d
        __vreg5 = b == c
        d = __vreg5
        print "b==c: "
        print d
        __vreg6 = b != c
        d = __vreg6
        print "b!=c: "
        print d
        return b
        leave fac
        next: ( )


#####global#####

globalPrev: ( )
        next: ( )

globalPrev: ( )
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

_S_START: PRTS "\nEnter event name:"
IN R005
JMPC EQ R005 97 _S_a
JMPC EQ R005 120 _S_END
PRTS "Invalid Input\n"
JMP _S_START

begin: MOVI  10000 R000           // RSP Initialized
JMP global

// Function/Event Module begins
_event_a: ADD  R000 4  R000
LDF  R000 F010           // Parameter Poped from Stack
ADD  R000 4  R000
LDF  R000 F011           // Parameter Poped from Stack
PRTS "\nFirst Param :"
PRTI F011
STF  F011  R000
SUB  R000 4  R000
STF  F010  R000
SUB  R000 4  R000
MOVL L4 R020
STI  R020  R000           // Return Addr Pushed on Stack
SUB  R000 4  R000
STF F010  R000           // Parameter Pushed on Stack
SUB  R000 4  R000
STF F011  R000           // Parameter Pushed on Stack
SUB  R000 4  R000
JMP  fac
L4: ADD  R000 4  R000
LDF  R000 F010
ADD  R000 4  R000
LDF  R000 F011
MOVF  F001 F012
L3: MOVFI F012 R021
MOVI R021 F011
PRTS "\nResult(fac) :"
PRTI F011
MOVIF F010 F013
FADD F012 F013 F014
MOVFI F014 R022
MOVI R022 F011
PRTS "\n i+j: "
PRTI F011
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
// Function/Event Module Ends

// Function/Event Module begins
fac: ADD  R000 4  R000
LDF  R000 F015           // Parameter Poped from Stack
ADD  R000 4  R000
LDF  R000 F016           // Parameter Poped from Stack
MOVFI F015 R023
MOVFI F016 R024
JMPC GT R023 R024 L5
MOVI 0 R025
JMP L6
L5: MOVI 1 R025
L6: MOVI R025 R026
PRTS "b>c: "
PRTI R026
MOVFI F015 R027
MOVFI F016 R028
JMPC GE R027 R028 L7
MOVI 0 R029
JMP L8
L7: MOVI 1 R029
L8: MOVI R029 R026
PRTS "b>=c: "
PRTI R026
MOVFI F015 R030
MOVFI F016 R031
JMPC LT R030 R031 L9
MOVI 0 R032
JMP L10
L9: MOVI 1 R032
L10: MOVI R032 R026
PRTS "b<c: "
PRTI R026
MOVFI F015 R033
MOVFI F016 R034
JMPC LE R033 R034 L11
MOVI 0 R035
JMP L12
L11: MOVI 1 R035
L12: MOVI R035 R026
PRTS "b<=c: "
PRTI R026
MOVFI F015 R036
MOVFI F016 R037
JMPC EQ R036 R037 L13
MOVI 0 R038
JMP L14
L13: MOVI 1 R038
L14: MOVI R038 R026
PRTS "b==c: "
PRTI R026
MOVFI F015 R039
MOVFI F016 R040
JMPC NE R039 R040 L15
MOVI 0 R041
JMP L16
L15: MOVI 1 R041
L16: MOVI R041 R026
PRTS "b!=c: "
PRTI R026
MOVF F015  F001
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
// Function/Event Module Ends

global: global: JMP _S_START

_S_END: PRTS "DONE"


Compilation Successful
