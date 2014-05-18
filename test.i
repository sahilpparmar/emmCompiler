
=================Lexical and Syntax Parsing==================

=========================AST Builder=========================
 
  int x;
  double zz;
  event a(int aa, int bb);
  int fac(int a) {
    double j = a;
    double k = 4.5;
  
    print("a from fac: ", a);
    print("j from fac: ", j);
    if (j>=k)     return 90;
    else     return 100;
  };
  int bar(int a) {
    int b = (5-2);
    int c = (1+10*100);
    int d = a*b;
    int e = (1-c);
    double f = (b/4);
    int g = (10/2);
    int h = (10%2);
    int i = (1|(1&0));
    int j = (0&1);
    int l = (1<<2);
    int m = (100>>2);
    bool bb = true;
    bool cc = true;
    double n = (((a%b)-(c*d/3))/c*c);
  
    a=(10+20);
    print("a: ", a);
    print("b: ", b);
    print("c: ", c);
    print("d: ", d);
    print("e: ", e);
    print("f: ", f);
    print("g: ", g);
    print("h: ", h);
    print("i: ", i);
    print("j: ", j);
    print("l: ", l);
    print("m: ", m);
    print("n: ", n);
    if ((bb&&cc))     a=1000;
    else     a=99;
    return a;
  };
 
  (a(int i, int j))-->   {
    print("\nFirst Param :", i);
    i=fac(i);
    print("\nResult(fac) :", i);
    i=bar(i);
    print("\nResult(fac) :", i);
  };
  ;;

========================Type Checking========================

======================3 Addr Generation======================
global:
fac:
        enter fac
        fparam a
        j = a
        k = 4.5
        print "a from fac: "
        print a
        print "j from fac: "
        print j
        if j >= k goto L3:
        goto L4:
L3:
        return 90
        goto L2:
L4:
        return 100
L2:
        leave fac
bar:
        enter bar
        fparam a
        __vreg1 = 5 - 2
        b = __vreg1
        __vreg2 = 10 * 100
        __vreg3 = 1 + __vreg2
        c = __vreg3
        __vreg4 = a * b
        d = __vreg4
        __vreg5 = 1 - c
        e = __vreg5
        __vreg6 = b / 4
        f = __vreg6
        __vreg7 = 10 / 2
        g = __vreg7
        __vreg8 = 10 % 2
        h = __vreg8
        __vreg9 = 1 & 0
        __vreg10 = 1 | __vreg9
        i = __vreg10
        __vreg11 = 0 & 1
        j = __vreg11
        __vreg12 = 1 << 2
        l = __vreg12
        __vreg13 = 100 >> 2
        m = __vreg13
        bb = 1
        cc = 1
        __vreg14 = a % b
        __vreg15 = c * d
        __vreg16 = __vreg15 / 3
        __vreg17 = __vreg14 - __vreg16
        __vreg18 = c * c
        __vreg19 = __vreg17 / __vreg18
        n = __vreg19
        __vreg20 = 10 + 20
        a = __vreg20
        print "a: "
        print a
        print "b: "
        print b
        print "c: "
        print c
        print "d: "
        print d
        print "e: "
        print e
        print "f: "
        print f
        print "g: "
        print g
        print "h: "
        print h
        print "i: "
        print i
        print "j: "
        print j
        print "l: "
        print l
        print "m: "
        print m
        print "n: "
        print n
        if bb goto L9:
        goto L8:
L9:
        if cc goto L7:
        goto L8:
L7:
        a = 1000
        goto L6:
L8:
        a = 99
L6:
        return a
        leave bar
_event_a:
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam i
        __vreg21 = call fac
L11:
        i = __vreg21
        print "\nResult(fac) :"
        print i
        aparam i
        __vreg22 = call bar
L12:
        i = __vreg22
        print "\nResult(fac) :"
        print i
        leave _event_a

====================== Basic Code Optimization ======================
global:
fac:
        enter fac
        fparam a
        j = a
        k = 4.5
        print "a from fac: "
        print a
        print "j from fac: "
        print j
        if j >= k goto L3:
        goto L4:
L3:
        return 90
        goto L2:
L4:
        return 100
L2:
        leave fac
bar:
        enter bar
        fparam a
        __vreg1 = 5 - 2
        b = __vreg1
        __vreg2 = 10 * 100
        __vreg3 = 1 + __vreg2
        c = __vreg3
        __vreg4 = a * b
        d = __vreg4
        __vreg5 = 1 - c
        e = __vreg5
        __vreg6 = b / 4
        f = __vreg6
        __vreg7 = 10 / 2
        g = __vreg7
        __vreg8 = 10 % 2
        h = __vreg8
        __vreg9 = 1 & 0
        __vreg10 = 1 | __vreg9
        i = __vreg10
        __vreg11 = 0 & 1
        j = __vreg11
        __vreg12 = 1 << 2
        l = __vreg12
        __vreg13 = 100 >> 2
        m = __vreg13
        bb = 1
        cc = 1
        __vreg14 = a % b
        __vreg15 = c * d
        __vreg16 = __vreg15 / 3
        __vreg17 = __vreg14 - __vreg16
        __vreg18 = c * c
        __vreg19 = __vreg17 / __vreg18
        n = __vreg19
        __vreg20 = 10 + 20
        a = __vreg20
        print "a: "
        print a
        print "b: "
        print b
        print "c: "
        print c
        print "d: "
        print d
        print "e: "
        print e
        print "f: "
        print f
        print "g: "
        print g
        print "h: "
        print h
        print "i: "
        print i
        print "j: "
        print j
        print "l: "
        print l
        print "m: "
        print m
        print "n: "
        print n
        if bb goto L9:
        goto L8:
L9:
        if cc goto L7:
        goto L8:
L7:
        a = 1000
        goto L6:
L8:
        a = 99
L6:
        return a
        leave bar
_event_a:
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam i
        __vreg21 = call fac
L11:
        i = __vreg21
        print "\nResult(fac) :"
        print i
        aparam i
        __vreg22 = call bar
L12:
        i = __vreg22
        print "\nResult(fac) :"
        print i
        leave _event_a


====================Basic Block creation=====================

#####_event_a#####
_event_a:
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam i
        __vreg21 = call fac
        next: ( L11 )
L11:
        i = __vreg21
        print "\nResult(fac) :"
        print i
        aparam i
        __vreg22 = call bar
        next: ( L12 )
L12:
        i = __vreg22
        print "\nResult(fac) :"
        print i
        leave _event_a
        next: ( )


#####bar#####
bar:
        enter bar
        fparam a
        __vreg1 = 5 - 2
        b = __vreg1
        __vreg2 = 10 * 100
        __vreg3 = 1 + __vreg2
        c = __vreg3
        __vreg4 = a * b
        d = __vreg4
        __vreg5 = 1 - c
        e = __vreg5
        __vreg6 = b / 4
        f = __vreg6
        __vreg7 = 10 / 2
        g = __vreg7
        __vreg8 = 10 % 2
        h = __vreg8
        __vreg9 = 1 & 0
        __vreg10 = 1 | __vreg9
        i = __vreg10
        __vreg11 = 0 & 1
        j = __vreg11
        __vreg12 = 1 << 2
        l = __vreg12
        __vreg13 = 100 >> 2
        m = __vreg13
        bb = 1
        cc = 1
        __vreg14 = a % b
        __vreg15 = c * d
        __vreg16 = __vreg15 / 3
        __vreg17 = __vreg14 - __vreg16
        __vreg18 = c * c
        __vreg19 = __vreg17 / __vreg18
        n = __vreg19
        __vreg20 = 10 + 20
        a = __vreg20
        print "a: "
        print a
        print "b: "
        print b
        print "c: "
        print c
        print "d: "
        print d
        print "e: "
        print e
        print "f: "
        print f
        print "g: "
        print g
        print "h: "
        print h
        print "i: "
        print i
        print "j: "
        print j
        print "l: "
        print l
        print "m: "
        print m
        print "n: "
        print n
        if bb goto L9:
        goto L8:
        next: ( L9 L8 )
L9:
        if cc goto L7:
        goto L8:
        next: ( L7 L8 )
L7:
        a = 1000
        goto L6:
        next: ( L6 )
L8:
        a = 99
        next: ( L6 )
L6:
        return a
        leave bar
        next: ( )


#####fac#####
fac:
        enter fac
        fparam a
        j = a
        k = 4.5
        print "a from fac: "
        print a
        print "j from fac: "
        print j
        if j >= k goto L3:
        goto L4:
        next: ( L3 L4 )
L3:
        return 90
        goto L2:
        next: ( L2 )
L4:
        return 100
        next: ( L2 )
L2:
        leave fac
        next: ( )


#####global#####
global:
        next: ( )
global:
        next: ( )


=========================Optimization========================


=====Constant propogation and folding optimization=======
#####_event_a#####
_event_a:
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam i
        __vreg21 = call fac
        next: ( L11 )
L11:
        i = __vreg21
        print "\nResult(fac) :"
        print i
        aparam i
        __vreg22 = call bar
        next: ( L12 )
L12:
        i = __vreg22
        print "\nResult(fac) :"
        print i
        leave _event_a
        next: ( )


#####bar#####
bar:
        enter bar
        fparam a
        __vreg1 = 3
        b = 3
        __vreg2 = 1000
        __vreg3 = 1001
        c = 1001
        __vreg4 = a * 3
        d = __vreg4
        __vreg5 = -1000
        e = -1000
        __vreg6 = 0
        f = 0
        __vreg7 = 5
        g = 5
        __vreg8 = 0
        h = 0
        __vreg9 = 0
        __vreg10 = 1
        i = 1
        __vreg11 = 0
        j = 0
        __vreg12 = 4
        l = 4
        __vreg13 = 25
        m = 25
        bb = 1
        cc = 1
        __vreg14 = a % 3
        __vreg15 = 1001 * __vreg4
        __vreg16 = __vreg15 / 3
        __vreg17 = __vreg14 - __vreg16
        __vreg18 = 1002001
        __vreg19 = __vreg17 / 1002001
        n = __vreg19
        __vreg20 = 30
        a = 30
        print "a: "
        print a
        print "b: "
        print b
        print "c: "
        print c
        print "d: "
        print d
        print "e: "
        print e
        print "f: "
        print f
        print "g: "
        print g
        print "h: "
        print h
        print "i: "
        print i
        print "j: "
        print j
        print "l: "
        print l
        print "m: "
        print m
        print "n: "
        print n
        if bb goto L9:
        goto L8:
        next: ( L9 L8 )
L9:
        if cc goto L7:
        goto L8:
        next: ( L7 L8 )
L7:
        a = 1000
        goto L6:
        next: ( L6 )
L8:
        a = 99
        next: ( L6 )
L6:
        return a
        leave bar
        next: ( )


#####fac#####
fac:
        enter fac
        fparam a
        j = a
        k = 4.5
        print "a from fac: "
        print a
        print "j from fac: "
        print j
        if j >= k goto L3:
        goto L4:
        next: ( L3 L4 )
L3:
        return 90
        goto L2:
        next: ( L2 )
L4:
        return 100
        next: ( L2 )
L2:
        leave fac
        next: ( )


#####global#####
global:
        next: ( )
global:
        next: ( )



 blockName: _event_a
live vars at start: 
live vars at End: __vreg21,

 blockName: L11
live vars at start: __vreg21,
live vars at End: __vreg22,

 blockName: L12
live vars at start: 
live vars at End: 

 blockName: bar
live vars at start: 
live vars at End: cc,

 blockName: L9
live vars at start: cc,
live vars at End: 

 blockName: L7
live vars at start: 
live vars at End: a,

 blockName: L8
live vars at start: 
live vars at End: a,

 blockName: L6
live vars at start: a,
live vars at End: 

 blockName: fac
live vars at start: 
live vars at End: 

 blockName: L3
live vars at start: 
live vars at End: 

 blockName: L4
live vars at start: 
live vars at End: 

 blockName: L2
live vars at start: 
live vars at End: 

=========Dead Code Elmination Optimization==================
#####_event_a#####
_event_a:
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam i
        __vreg21 = call fac
        next: ( L11 )
L11:
        i = __vreg21
        print "\nResult(fac) :"
        print i
        aparam i
        __vreg22 = call bar
        next: ( L12 )
L12:
        print "\nResult(fac) :"
        print i
        leave _event_a
        next: ( )


#####bar#####
bar:
        enter bar
        fparam a
        bb = 1
        cc = 1
        print "a: "
        print a
        print "b: "
        print b
        print "c: "
        print c
        print "d: "
        print d
        print "e: "
        print e
        print "f: "
        print f
        print "g: "
        print g
        print "h: "
        print h
        print "i: "
        print i
        print "j: "
        print j
        print "l: "
        print l
        print "m: "
        print m
        print "n: "
        print n
        if bb goto L9:
        goto L8:
        next: ( L9 L8 )
L9:
        if cc goto L7:
        goto L8:
        next: ( L7 L8 )
L7:
        a = 1000
        goto L6:
        next: ( L6 )
L8:
        a = 99
        next: ( L6 )
L6:
        return a
        leave bar
        next: ( )


#####fac#####
fac:
        enter fac
        fparam a
        j = a
        k = 4.5
        print "a from fac: "
        print a
        print "j from fac: "
        print j
        if j >= k goto L3:
        goto L4:
        next: ( L3 L4 )
L3:
        return 90
        goto L2:
        next: ( L2 )
L4:
        return 100
        next: ( L2 )
L2:
        leave fac
        next: ( )


#####global#####
global:
        next: ( )
global:
        next: ( )


#####_event_a#####
_event_a:
        enter _event_a
        fparam j
        fparam i
        print "\nFirst Param :"
        print i
        aparam i
        __vreg21 = call fac
        next: ( L11 )
L11:
        i = __vreg21
        print "\nResult(fac) :"
        print i
        aparam i
        __vreg22 = call bar
        next: ( L12 )
L12:
        print "\nResult(fac) :"
        print i
        leave _event_a
        next: ( )


#####bar#####
bar:
        enter bar
        fparam a
        bb = 1
        cc = 1
        print "a: "
        print a
        print "b: "
        print b
        print "c: "
        print c
        print "d: "
        print d
        print "e: "
        print e
        print "f: "
        print f
        print "g: "
        print g
        print "h: "
        print h
        print "i: "
        print i
        print "j: "
        print j
        print "l: "
        print l
        print "m: "
        print m
        print "n: "
        print n
        if bb goto L9:
        goto L8:
        next: ( L9 L8 )
L9:
        if cc goto L7:
        goto L8:
        next: ( L7 L8 )
L7:
        a = 1000
        goto L6:
        next: ( L6 )
L8:
        a = 99
        next: ( L6 )
L6:
        return a
        leave bar
        next: ( )


#####fac#####
fac:
        enter fac
        fparam a
        j = a
        k = 4.5
        print "a from fac: "
        print a
        print "j from fac: "
        print j
        if j >= k goto L3:
        goto L4:
        next: ( L3 L4 )
L3:
        return 90
        goto L2:
        next: ( L2 )
L4:
        return 100
        next: ( L2 )
L2:
        leave fac
        next: ( )


#####global#####
global:
        next: ( )
global:
        next: ( )


===================Final Code generation=====================
JMP begin
_S_a: MOVL _S_START R005
STI R005 R000           // Return Addr Pushed on Stack
SUB R000 4 R000
INI R005           // READ Event Parameter Input
STI R005 R000           // Parameter Pushed on Stack
SUB R000 4 R000
INI R005           // READ Event Parameter Input
STI R005 R000           // Parameter Pushed on Stack
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
LDI  R000 R020           // Parameter Poped from Stack
ADD  R000 4  R000
LDI  R000 R021           // Parameter Poped from Stack
PRTS "\nFirst Param :"
PRTI R021
STI  R021  R000
SUB  R000 4  R000
STI  R020  R000
SUB  R000 4  R000
MOVL L13 R022
STI  R022  R000           // Return Addr Pushed on Stack
SUB  R000 4  R000
STI R021  R000           // Parameter Pushed on Stack
SUB  R000 4  R000
JMP  fac
L13: ADD  R000 4  R000
LDI  R000 R020
ADD  R000 4  R000
LDI  R000 R021
MOVI  R001 R023
L11: MOVI R023 R021
PRTS "\nResult(fac) :"
PRTI R021
STI  R023  R000
SUB  R000 4  R000
STI  R022  R000
SUB  R000 4  R000
MOVL L14 R024
STI  R024  R000           // Return Addr Pushed on Stack
SUB  R000 4  R000
STI R021  R000           // Parameter Pushed on Stack
SUB  R000 4  R000
JMP  bar
L14: ADD  R000 4  R000
LDI  R000 R022
ADD  R000 4  R000
LDI  R000 R023
MOVI  R001 R025
L12: PRTS "\nResult(fac) :"
PRTI R021
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
// Function/Event Module Ends

// Function/Event Module begins
bar: ADD  R000 4  R000
LDI  R000 R026           // Parameter Poped from Stack
MOVI 1 R027
MOVI 1 R028
PRTS "a: "
PRTI R026
PRTS "b: "
PRTI R029
PRTS "c: "
PRTI R030
PRTS "d: "
PRTI R031
PRTS "e: "
PRTI R032
PRTS "f: "
PRTF F010
PRTS "g: "
PRTI R033
PRTS "h: "
PRTI R034
PRTS "i: "
PRTI R035
PRTS "j: "
PRTI R036
PRTS "l: "
PRTI R037
PRTS "m: "
PRTI R038
PRTS "n: "
PRTF F011
JMPC NE R027 0 L9
JMP L8
L9: JMPC NE R028 0 L7
JMP L8
L7: MOVI 1000 R026
JMP L6
L8: MOVI 99 R026
L6: MOVI R026  R001
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
// Function/Event Module Ends

// Function/Event Module begins
fac: ADD  R000 4  R000
LDI  R000 R039           // Parameter Poped from Stack
MOVIF R039 F013
MOVF F013 F012
MOVF 4.5 F014
PRTS "a from fac: "
PRTI R039
PRTS "j from fac: "
PRTF F012
JMPC FGE F012 F014 L3
JMP L4
L3: MOVI 90  R001
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
JMP L2
L4: MOVI 100  R001
ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
L2: ADD  R000 4  R000
LDI  R000  R004           // Return Addr Poped from Stack
JMPI  R004
// Function/Event Module Ends

global: global: JMP _S_START

_S_END: PRTS "DONE"


Compilation Successful
