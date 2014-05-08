/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 2 "E--_parser.y++"


#include "Ast.h"
#include "ParserUtil.h"
#include "Error.h"

using namespace std;

extern int verbosity;
extern int yylex();
extern void yyerror(const char *s);
extern const OpNode::OpInfo opInfo[];

#define prt(x) //cout << x << endl;

static Type                   *curType        =  NULL;
static vector<ExprNode*>      *curExprList    =  NULL;
static vector<ExprNode*>      *funcParamList  =  NULL;
static list<StmtNode*>        *curStmtList1   =  NULL;
static list<StmtNode*>        *curStmtList3   =  NULL;
static vector<Type*>          *curTypeList    =  NULL; 
static vector<VariableEntry*> *curVarList     =  NULL; 
static FunctionEntry          *curFuncEnt     =  NULL;
static EventEntry             *curEventEnt    =  NULL;
static RuleBlockEntry         *curRBlkNode    =  NULL;
static int                    curClassSize    =  0;
static char                   *funcName       =  NULL;

static EventEntry* getEventEntry(char *eventName) {
    SymTabEntry *sym = stm.lookUp(string(eventName));
    if (sym && sym->type()->tag() == Type::EVENT) {
        return (EventEntry*) sym;
    }
    errMsg((string)"Expected an event name, but got \"" + (string)eventName + (string)"\"");
    return NULL;
}

static Type* getUserType(char *typeName) {
    SymTabEntry *sym = stm.lookUp(string(typeName));
    if (sym && sym->type()->tag() == Type::CLASS) {
        return sym->type();
    } else {
        errMsg((string)"Undeclared type \"" + (string)typeName + (string)"\"");
        return new Type(Type::VOID);
    }
}

static bool IsFuncSymEntry(SymTabEntry *sym) {
    if (sym && sym->kind() == SymTabEntry::Kind::FUNCTION_KIND)
        return true;
    return false;
}

static bool IsEventSymEntry(SymTabEntry *sym) {
    if (sym && sym->kind() == SymTabEntry::Kind::EVENT_KIND)
        return true;
    return false;
}

static bool IsCurRuleBlockEntry() {
    return curRBlkNode ? true : false;
}

static VariableEntry* allocVarEntry(string name, VariableEntry::VarKind v, Type* type, ExprNode* init) {
    VariableEntry *retVarEnt = (VariableEntry*)stm.lookUpInScope(name, stm.currentScope()->kind());

    if (retVarEnt) {
        errMsg((string)"Redefinition of name " + name);
    } else {
        retVarEnt = new VariableEntry(name, v, type, init, yylinenum, yycolumnno, string(yyfilename));
        stm.insert(retVarEnt);
    }

    return retVarEnt;
}

static ClassEntry* allocClassEntry(char* name) {
    assert(name != NULL);
    ClassEntry *retClassEnt = (ClassEntry*)stm.lookUpInScope(name, stm.currentScope()->kind());

    if (retClassEnt) {
        errMsg((string)"Redefinition of name " + name);
    } else {
        retClassEnt = new ClassEntry(string(name), yylinenum, yycolumnno, string(yyfilename));
        Type *te = new Type(retClassEnt, Type::CLASS);
        retClassEnt->type(te);
        stm.insert(retClassEnt);
        stm.enterScope(retClassEnt);
    }
    return retClassEnt;
}

static ClassFuncInvocationNode* allocClassFuncInvNode(char *objName, char* funcName, vector<ExprNode*>* param_l) {
    assert(objName != NULL);
    assert(funcName != NULL);
    SymTabEntry *objSym = stm.lookUpInScope(objName, stm.currentScope()->kind());
    SymTabEntry *classSym = NULL, *funcSym = NULL; 
    if (objSym && objSym->type()->tag() == Type::CLASS) {
        classSym = stm.lookUp(objSym->type()->typeDesc()->name());
        funcSym = classSym->symTab()->lookUp(string(funcName));
        }
    if (!(IsFuncSymEntry(funcSym) || IsEventSymEntry(funcSym))) {
        errMsg((string)"\"" + (string)funcName + (string)"\" is not a function or module name");
        return NULL;
    }
    return new ClassFuncInvocationNode(objSym, funcSym, param_l, yylinenum, yycolumnno, string(yyfilename));
}

static ClassRefExprNode* allocClassRefExprNode(char* objName, char* varName) {
    assert(objName != NULL);
    assert(varName != NULL);
    SymTabEntry *objSym = stm.lookUpInScope(string(objName), stm.currentScope()->kind());
    SymTabEntry *classSym = NULL, *varSym = NULL; 
    if (objSym == NULL) {
        errMsg((string)"Undeclared identifier \"" + (string)objName + (string)"\"");
    }
    if (objSym && objSym->type()->tag() == Type::CLASS) {
        classSym = stm.lookUp(objSym->type()->typeDesc()->name());
        varSym = classSym->symTab()->lookUp(string(varName));
        }
    if (varSym == NULL) {
        errMsg((string)"Undeclared identifier \"" + (string)varName + (string)"\"");
    }
    
    return new ClassRefExprNode(string(varName), objSym, varSym, yylinenum, yycolumnno, string(yyfilename));
}

static EventEntry* allocEventEntry(char* name = NULL, vector<Type*>* param_l = NULL) {
    EventEntry* retEventEnt = curEventEnt;

    if (retEventEnt) {
        assert(name != NULL);
        if (IsEventSymEntry(stm.lookUp(string(name)))) {
            errMsg((string)"Redefinition of name " + name);
        }
        Type *te = new Type(param_l, Type::EVENT);
        retEventEnt->name(name);
        retEventEnt->type(te);
        curEventEnt = NULL;
    } else {
        retEventEnt = new EventEntry(string("dummy_entry"), yylinenum, yycolumnno, string(yyfilename));
        stm.enterScope(retEventEnt);
    }

    return retEventEnt;
}

static FunctionEntry* allocFuncEntry(char* name = NULL, Type* rettype = NULL, vector<Type*>* param_l = NULL) {
    FunctionEntry* retFuncEnt = curFuncEnt;

    if (retFuncEnt) {
        assert(name != NULL);
        if (IsFuncSymEntry(stm.lookUp(string(name)))) {
            errMsg((string)"Redefinition of name " + name);
        }
        Type *te = new Type(param_l, rettype);
        retFuncEnt->name(name);
        retFuncEnt->type(te);
        curFuncEnt = NULL;
    } else {
        retFuncEnt = new FunctionEntry(string("dummy_func"), NULL, yylinenum, yycolumnno, string(yyfilename));
        //cout << "Current Scope : "<<stm.currentScope()->name();
        stm.enterScope(retFuncEnt);
    }
    return retFuncEnt;
}

static RefExprNode* allocRefExprNode(char* str) {
    assert(str != NULL);
    SymTabEntry *sym = stm.lookUp(string(str));
    if (sym == NULL) {
        errMsg((string)"Undeclared identifier \"" + (string)str + (string)"\"");
    }
    return new RefExprNode(string(str), sym, yylinenum, yycolumnno, string(yyfilename));
}

static InvocationNode* allocInvNode(char* str, vector<ExprNode*>* param_l) {
    assert(str != NULL);
    SymTabEntry *sym = stm.lookUp(string(str));

 if (!(IsFuncSymEntry(sym) || IsEventSymEntry(sym))) {
        errMsg((string)"\"" + (string)str + (string)"\" is not a function or module name");
        return NULL;
    }
    return new InvocationNode(sym, param_l, yylinenum, yycolumnno, string(yyfilename));
}

static PrintFunctionNode* allocPrintNode(vector<ExprNode*>* param_l) {
        return new PrintFunctionNode(param_l, yylinenum, yycolumnno, string(yyfilename)); 
    } 

static ValueNode* allocValueNode(Value* val) {
    return new ValueNode(val, yylinenum, yycolumnno, string(yyfilename));
}

static OpNode* allocOpNode(OpNode::OpCode op, ExprNode *l, ExprNode *r) {
    assert(l);
    return new OpNode(op, l, r, yylinenum, yycolumnno, string(yyfilename));
}

static ExprStmtNode* allocExprStmtNode(ExprNode* e) {
    return new ExprStmtNode(e, yylinenum, yycolumnno, string(yyfilename));
}

static ReturnStmtNode* allocReturnNode(ExprNode* e) {
    if (IsCurRuleBlockEntry())
        errMsg((string)"Return statement is valid only within a function");
    return new ReturnStmtNode(e, (FunctionEntry*)stm.currentScope(SymTabEntry::Kind::FUNCTION_KIND),
            yylinenum, yycolumnno, string(yyfilename));
}

static BreakStmtNode* allocBreakNode(ExprNode* e) {
    return new BreakStmtNode(e, yylinenum, yycolumnno, string(yyfilename));
}

static ContinueStmtNode* allocContinueNode(ExprNode* e) {
    return new ContinueStmtNode(e, yylinenum, yycolumnno, string(yyfilename));
}

static IfNode* allocIfNode(ExprNode* cond, StmtNode* thenStmt, StmtNode* elseStmt) {
    return new IfNode(cond, thenStmt, elseStmt, yylinenum, yycolumnno, string(yyfilename));
}

static WhileNode* allocWhileNode(ExprNode* cond, StmtNode* bodyStmt) {
    return new WhileNode(cond, bodyStmt, yylinenum, yycolumnno, string(yyfilename));
}

static CompoundStmtNode* allocCompStmtNode(list<StmtNode*> *Slist) {
    if (!Slist) Slist = new list<StmtNode*>;
    return new CompoundStmtNode(Slist, yylinenum, yycolumnno, string(yyfilename));
}

static CompoundStmtNode* allocScopeNode(CompoundStmtNode* node) {
    CompoundStmtNode *retCompNode = allocCompStmtNode(new list<StmtNode*>);
    retCompNode->add(node);
    return retCompNode;
}

static PrimitivePatNode* allocPrimPatNode(EventEntry* ee, vector<VariableEntry*>* params, ExprNode* c) {
    if (ee)
        return new PrimitivePatNode(ee, params, c, yylinenum, yycolumnno, string(yyfilename));
    return NULL;
}

static PatNode* allocPatNode(BasePatNode::PatNodeKind pk, BasePatNode *p1, BasePatNode *p2) {
    if (p1 && 
        ((pk == BasePatNode::PatNodeKind::NEG || pk == BasePatNode::PatNodeKind::STAR) ||
        ((pk == BasePatNode::PatNodeKind::OR  || pk == BasePatNode::PatNodeKind::SEQ) && p2)))
    {
        return new PatNode(pk, p1, p2, yylinenum, yycolumnno, string(yyfilename));
    }
    return NULL;
}

static RuleNode* allocRuleNode(BasePatNode* pat, StmtNode* reaction) {
    RuleNode *retRuleNode = NULL;

    if (pat) {
        retRuleNode = new RuleNode(curRBlkNode, pat, reaction, yylinenum, yycolumnno, string(yyfilename));
        GlobalEntry *globalSymEnt = (GlobalEntry*) stm.currentScope(SymTabEntry::Kind::GLOBAL_KIND);
        globalSymEnt->addRule(retRuleNode);
    }
    stm.leaveScope();
    curRBlkNode = NULL;

    return retRuleNode;
}

static RuleBlockEntry* allocRuleBlockNode() {
    if (!curRBlkNode) {
        curRBlkNode = new RuleBlockEntry(yylinenum, yycolumnno, string(yyfilename));
        stm.enterScope(curRBlkNode);
    }
    return curRBlkNode;
}



/* Line 268 of yacc.c  */
#line 350 "E--_parser.C"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_LEX_ERROR = 258,
     TOK_PAT_STAR = 259,
     TOK_PAT_OR = 260,
     TOK_PAT_NOT = 261,
     TOK_PLUS = 262,
     TOK_MINUS = 263,
     TOK_UMINUS = 264,
     TOK_MULT = 265,
     TOK_DIV = 266,
     TOK_MOD = 267,
     TOK_BITAND = 268,
     TOK_BITOR = 269,
     TOK_BITNOT = 270,
     TOK_BITXOR = 271,
     TOK_SHL = 272,
     TOK_SHR = 273,
     TOK_GT = 274,
     TOK_LT = 275,
     TOK_GE = 276,
     TOK_LE = 277,
     TOK_EQ = 278,
     TOK_NE = 279,
     TOK_AND = 280,
     TOK_OR = 281,
     TOK_NOT = 282,
     TOK_COMMA = 283,
     TOK_ASSIGN = 284,
     TOK_SEMICOLON = 285,
     TOK_COLON = 286,
     TOK_ARROW = 287,
     TOK_QMARK = 288,
     TOK_LBRACE = 289,
     TOK_RBRACE = 290,
     TOK_LPAREN = 291,
     TOK_RPAREN = 292,
     TOK_LBRACK = 293,
     TOK_RBRACK = 294,
     TOK_IF = 295,
     TOK_ELSE = 296,
     TOK_CLASS = 297,
     TOK_RETURN = 298,
     TOK_EVENT = 299,
     TOK_ANY = 300,
     TOK_PRINT = 301,
     TOK_WHILE = 302,
     TOK_BREAK = 303,
     TOK_CONTINUE = 304,
     TOK_UINTNUM = 305,
     TOK_DOUBLENUM = 306,
     TOK_VOID = 307,
     TOK_STRING = 308,
     TOK_STRCONST = 309,
     TOK_INT = 310,
     TOK_UNSIGNED = 311,
     TOK_BOOL = 312,
     TOK_BYTE = 313,
     TOK_DOUBLE = 314,
     TOK_SHORT = 315,
     TOK_LONG = 316,
     TOK_BIT = 317,
     TOK_ENUM = 318,
     TOK_TRUE = 319,
     TOK_FALSE = 320,
     TOK_IDENT = 321,
     TOK_DOUBLE_COLON = 322,
     TOK_DOT = 323
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 280 "E--_parser.y++"

    unsigned int        uVal;
    double              dVal;
    char*               cVal;
    vector<string>*     strVector;
    Value*              valVal;

    ExprNode*           exprVal;
    vector<ExprNode*>*  exprList;
    RefExprNode*        refexpVal;

    ClassEntry*         classEntry;
    ClassFuncInvocationNode*   classFuncVal;
    ClassRefExprNode*   classRefExpVal;
    
    Type*               typeVal;
    vector<Type*>*      typeList; 

    FunctionEntry*      functionEntry;
    EventEntry*         eventEntry;                        
    VariableEntry*      variableEntry;
    vector<VariableEntry*>* varList;

    StmtNode*           stmtVal;
    list<StmtNode*>*    stmtList;
    IfNode*             ifVal;
    WhileNode*          whileVal;
    BreakStmtNode*      breakVal;
    ContinueStmtNode*   continueVal;
    ReturnStmtNode*     returnVal;
    CompoundStmtNode*   stmt_plusVal;
    RuleNode*           ruleNode;

    BasePatNode*        patVal;
    PrimitivePatNode*   primPatVal;
    vector<RuleNode*>*  transList;

    const OpNode::OpInfo*   funIval;



/* Line 293 of yacc.c  */
#line 496 "E--_parser.C"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 508 "E--_parser.C"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   733

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  61
/* YYNRULES -- Number of rules.  */
#define YYNRULES  167
/* YYNRULES -- Number of states.  */
#define YYNSTATES  273

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   323

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    12,    14,    16,    18,
      20,    24,    27,    29,    32,    34,    38,    43,    45,    49,
      53,    56,    57,    59,    61,    65,    67,    71,    75,    79,
      82,    85,    87,    92,    96,    98,    99,   103,   107,   113,
     118,   122,   124,   126,   128,   131,   133,   136,   139,   141,
     143,   145,   147,   149,   151,   155,   158,   161,   165,   168,
     172,   177,   181,   185,   188,   192,   197,   201,   206,   210,
     214,   216,   220,   222,   224,   226,   228,   230,   232,   236,
     239,   242,   245,   249,   253,   257,   261,   265,   269,   273,
     277,   281,   285,   289,   293,   297,   301,   305,   309,   313,
     317,   319,   321,   325,   329,   331,   333,   337,   340,   344,
     347,   351,   354,   357,   358,   362,   366,   368,   370,   374,
     379,   384,   388,   391,   394,   399,   403,   406,   409,   410,
     414,   418,   420,   422,   426,   430,   434,   438,   440,   443,
     446,   451,   455,   459,   461,   464,   467,   470,   472,   474,
     476,   478,   480,   482,   484,   486,   488,   491,   493,   495,
     497,   499,   501,   503,   505,   507,   509,   511
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      70,     0,    -1,    71,    -1,    71,    72,    -1,    71,    73,
      -1,    -1,   122,    -1,   113,    -1,   103,    -1,   100,    -1,
      78,    32,    74,    -1,    75,   125,    -1,    75,    -1,    96,
     125,    -1,   125,    -1,    34,    75,    35,    -1,    34,    75,
     125,    35,    -1,    76,    -1,    34,    77,    35,    -1,    34,
     125,    35,    -1,    77,    87,    -1,    -1,    81,    -1,    80,
      -1,    81,    14,    96,    -1,    79,    -1,    36,    78,    37,
      -1,    78,     5,    78,    -1,    78,    31,    78,    -1,    78,
       4,    -1,    27,    78,    -1,    45,    -1,    66,    36,    82,
      37,    -1,    82,    28,    66,    -1,    66,    -1,    -1,    47,
      92,    85,    -1,    40,    92,    85,    -1,    40,    92,    85,
      41,    85,    -1,    34,    86,    35,   125,    -1,    34,    86,
      35,    -1,    91,    -1,    87,    -1,   125,    -1,    86,    87,
      -1,    87,    -1,    93,   125,    -1,    95,   125,    -1,    84,
      -1,    88,    -1,    83,    -1,    89,    -1,    90,    -1,   111,
      -1,    43,    96,   125,    -1,    43,   125,    -1,    48,   125,
      -1,    48,    50,   125,    -1,    49,   125,    -1,    49,    50,
     125,    -1,    34,   125,    35,   125,    -1,    34,   125,    35,
      -1,    34,    35,   125,    -1,    34,    35,    -1,    36,    96,
      37,    -1,    66,    36,    94,    37,    -1,    66,    36,    37,
      -1,    46,    36,    94,    37,    -1,    46,    36,    37,    -1,
      94,    28,    96,    -1,    96,    -1,    99,    29,    96,    -1,
     126,    -1,    99,    -1,    93,    -1,    95,    -1,    97,    -1,
      98,    -1,    36,    96,    37,    -1,    27,    96,    -1,    15,
      96,    -1,     8,    96,    -1,    96,    26,    96,    -1,    96,
      25,    96,    -1,    96,    23,    96,    -1,    96,    24,    96,
      -1,    96,    19,    96,    -1,    96,    20,    96,    -1,    96,
      21,    96,    -1,    96,    22,    96,    -1,    96,    14,    96,
      -1,    96,    16,    96,    -1,    96,    13,    96,    -1,    96,
      17,    96,    -1,    96,    18,    96,    -1,    96,     7,    96,
      -1,    96,     8,    96,    -1,    96,    10,    96,    -1,    96,
      11,    96,    -1,    96,    12,    96,    -1,    66,    -1,   110,
      -1,   127,   101,   125,    -1,   101,    28,   102,    -1,   102,
      -1,    66,    -1,    66,    29,    96,    -1,   104,   125,    -1,
     104,   105,   125,    -1,    42,    66,    -1,    34,   106,    35,
      -1,   106,   107,    -1,   106,   113,    -1,    -1,   127,   108,
     125,    -1,   108,    28,   109,    -1,   109,    -1,    66,    -1,
      66,    68,    66,    -1,    66,    68,   112,   125,    -1,    66,
      36,    94,    37,    -1,    66,    36,    37,    -1,   119,   125,
      -1,   119,   114,    -1,    34,   115,    35,   125,    -1,    34,
     115,    35,    -1,   115,   116,    -1,   115,    87,    -1,    -1,
     127,   117,   125,    -1,   117,    28,   118,    -1,   118,    -1,
      66,    -1,    66,    29,    96,    -1,   127,    66,   120,    -1,
      36,   120,    37,    -1,   120,    28,   121,    -1,   121,    -1,
      36,    37,    -1,   127,    66,    -1,    44,    66,   123,   125,
      -1,    36,   123,    37,    -1,   123,    28,   124,    -1,   124,
      -1,    36,    37,    -1,   127,    66,    -1,   125,    30,    -1,
      30,    -1,    50,    -1,    51,    -1,    54,    -1,    64,    -1,
      65,    -1,   128,    -1,   129,    -1,    66,    -1,    56,   128,
      -1,    52,    -1,    57,    -1,    58,    -1,    53,    -1,    59,
      -1,    60,    -1,    61,    -1,    62,    -1,    63,    -1,    55,
      -1,    56,    55,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   395,   395,   398,   399,   400,   403,   404,   405,   406,
     409,   412,   413,   414,   415,   418,   419,   420,   423,   424,
     427,   428,   431,   432,   433,   434,   435,   438,   439,   440,
     441,   444,   447,   451,   452,   454,   457,   460,   461,   464,
     465,   466,   467,   468,   471,   472,   475,   476,   477,   478,
     479,   480,   481,   482,   485,   486,   489,   490,   493,   494,
     497,   498,   499,   500,   503,   506,   507,   508,   509,   512,
     513,   516,   519,   520,   521,   522,   523,   524,   525,   528,
     529,   530,   533,   534,   535,   536,   537,   538,   539,   540,
     541,   542,   543,   544,   545,   546,   547,   548,   549,   550,
     553,   554,   557,   560,   561,   564,   565,   568,   569,   572,
     575,   578,   579,   580,   583,   586,   587,   590,   594,   597,
     600,   601,   604,   605,   608,   609,   612,   613,   614,   617,
     620,   621,   624,   625,   628,   631,   632,   633,   634,   636,
     640,   643,   644,   645,   646,   649,   653,   654,   658,   659,
     660,   661,   662,   665,   666,   667,   668,   672,   673,   674,
     675,   676,   677,   679,   681,   683,   687,   688
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_LEX_ERROR", "TOK_PAT_STAR",
  "TOK_PAT_OR", "TOK_PAT_NOT", "TOK_PLUS", "TOK_MINUS", "TOK_UMINUS",
  "TOK_MULT", "TOK_DIV", "TOK_MOD", "TOK_BITAND", "TOK_BITOR",
  "TOK_BITNOT", "TOK_BITXOR", "TOK_SHL", "TOK_SHR", "TOK_GT", "TOK_LT",
  "TOK_GE", "TOK_LE", "TOK_EQ", "TOK_NE", "TOK_AND", "TOK_OR", "TOK_NOT",
  "TOK_COMMA", "TOK_ASSIGN", "TOK_SEMICOLON", "TOK_COLON", "TOK_ARROW",
  "TOK_QMARK", "TOK_LBRACE", "TOK_RBRACE", "TOK_LPAREN", "TOK_RPAREN",
  "TOK_LBRACK", "TOK_RBRACK", "TOK_IF", "TOK_ELSE", "TOK_CLASS",
  "TOK_RETURN", "TOK_EVENT", "TOK_ANY", "TOK_PRINT", "TOK_WHILE",
  "TOK_BREAK", "TOK_CONTINUE", "TOK_UINTNUM", "TOK_DOUBLENUM", "TOK_VOID",
  "TOK_STRING", "TOK_STRCONST", "TOK_INT", "TOK_UNSIGNED", "TOK_BOOL",
  "TOK_BYTE", "TOK_DOUBLE", "TOK_SHORT", "TOK_LONG", "TOK_BIT", "TOK_ENUM",
  "TOK_TRUE", "TOK_FALSE", "TOK_IDENT", "TOK_DOUBLE_COLON", "TOK_DOT",
  "$accept", "s_specification", "specification", "declaration", "rule",
  "rule_stmts", "compound_stmt2", "compound_stmt1", "stmt_plus1",
  "primitive_all", "binary_event_oprs", "any_event", "normal_event",
  "event_aparam_l", "while_stmt", "if_then_else_stmt", "basic_block",
  "stmt_plus2", "simple_stmt", "return_stmt", "break_stmt",
  "continue_stmt", "empty_block", "condition", "func_invocation",
  "caller_param_l", "assignment", "expression", "unary_operation",
  "binary_operation", "ref_expr", "global_decl", "global_l", "global_var",
  "class_proto", "class_decl", "class_defn", "class_body",
  "class_var_decl", "class_var_l", "class_var", "obj_var", "obj_func",
  "class_func_invocation", "function_proto", "function_defn",
  "function_body", "local_decl", "local_l", "local_var", "function_decl",
  "func_param_l", "func_param", "event_decl", "event_param_l",
  "event_param", "semi_colon", "literal", "type", "baseType", "intType", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    69,    70,    71,    71,    71,    72,    72,    72,    72,
      73,    74,    74,    74,    74,    75,    75,    75,    76,    76,
      77,    77,    78,    78,    78,    78,    78,    79,    79,    79,
      79,    80,    81,    82,    82,    82,    83,    84,    84,    85,
      85,    85,    85,    85,    86,    86,    87,    87,    87,    87,
      87,    87,    87,    87,    88,    88,    89,    89,    90,    90,
      91,    91,    91,    91,    92,    93,    93,    93,    93,    94,
      94,    95,    96,    96,    96,    96,    96,    96,    96,    97,
      97,    97,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      99,    99,   100,   101,   101,   102,   102,   103,   103,   104,
     105,   106,   106,   106,   107,   108,   108,   109,   110,   111,
     112,   112,   113,   113,   114,   114,   115,   115,   115,   116,
     117,   117,   118,   118,   119,   120,   120,   120,   120,   121,
     122,   123,   123,   123,   123,   124,   125,   125,   126,   126,
     126,   126,   126,   127,   127,   127,   127,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   129,   129
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     0,     1,     1,     1,     1,
       3,     2,     1,     2,     1,     3,     4,     1,     3,     3,
       2,     0,     1,     1,     3,     1,     3,     3,     3,     2,
       2,     1,     4,     3,     1,     0,     3,     3,     5,     4,
       3,     1,     1,     1,     2,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     3,     2,     2,     3,     2,     3,
       4,     3,     3,     2,     3,     4,     3,     4,     3,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     3,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     3,     3,     1,     1,     3,     2,     3,     2,
       3,     2,     2,     0,     3,     3,     1,     1,     3,     4,
       4,     3,     2,     2,     4,     3,     2,     2,     0,     3,
       3,     1,     1,     3,     3,     3,     3,     1,     2,     2,
       4,     3,     3,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     2,     1,     0,     0,     0,     0,    31,   157,
     160,   166,     0,   158,   159,   161,   162,   163,   164,   165,
     155,     3,     4,     0,    25,    23,    22,     9,     8,     0,
       7,     0,     6,     0,   153,   154,     0,    30,     0,   109,
       0,   167,   156,    35,    29,     0,     0,     0,     0,   147,
     113,     0,   107,   128,   123,   122,   105,     0,   104,    26,
       0,   155,     0,   143,     0,    34,     0,    27,    28,     0,
       0,     0,    21,     0,     0,   148,   149,   150,   151,   152,
     100,    10,    12,    17,    74,    75,     0,    76,    77,    73,
     101,    14,    72,    24,     0,   108,   146,     0,     0,     0,
     134,   137,     0,     0,   102,   144,     0,     0,   140,   145,
       0,    32,    81,    80,    79,     0,     0,     0,     0,     0,
       0,     0,    11,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    13,     0,   110,   111,   112,     0,   125,     0,     0,
       0,     0,     0,   100,    50,    48,   127,    49,    51,    52,
       0,     0,     0,    53,   126,     0,   106,   138,     0,     0,
     139,   105,   103,   141,   142,    33,    15,     0,    18,   100,
      20,    19,    78,    68,     0,    70,    66,     0,   118,    95,
      96,    97,    98,    99,    92,    90,    91,    93,    94,    86,
      87,    88,    89,    84,    85,    83,    82,    71,   117,     0,
     116,   124,     0,     0,     0,    55,     0,     0,    56,     0,
      58,     0,    46,    47,   132,     0,   131,   135,   136,    16,
       0,    67,    65,     0,   114,     0,     0,    37,    42,    41,
      43,    54,    36,    57,    59,   118,     0,     0,     0,   129,
      69,   117,   115,    64,    63,     0,    45,     0,     0,     0,
     119,   133,   130,    62,    40,    44,    61,    38,   121,     0,
      39,    60,   120
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    21,    22,    81,    82,    83,   116,    23,
      24,    25,    26,    66,   154,   155,   237,   255,   238,   157,
     158,   159,   239,   213,    84,   184,    85,   185,    87,    88,
      89,    27,    57,    58,    28,    29,    51,    94,   144,   209,
     210,    90,   163,   246,    30,    54,    97,   164,   225,   226,
      31,   100,   101,    32,    62,    63,   240,    92,   102,    34,
      35
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -213
static const yytype_int16 yypact[] =
{
    -213,     9,   333,  -213,    -4,    -4,   -45,    15,  -213,  -213,
    -213,  -213,   670,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
       1,  -213,  -213,    79,  -213,  -213,   131,  -213,  -213,    -3,
    -213,    82,  -213,    59,  -213,  -213,     1,  -213,    40,  -213,
     538,  -213,  -213,    94,  -213,    -4,    -4,   128,   317,  -213,
    -213,   118,   121,  -213,  -213,   121,   351,    85,  -213,  -213,
     498,  -213,   126,  -213,    95,  -213,   -12,    11,   166,   317,
     317,   317,   123,   317,   140,  -213,  -213,  -213,  -213,  -213,
     -28,  -213,   118,  -213,  -213,  -213,   607,  -213,  -213,   152,
    -213,   121,  -213,   628,   395,   121,  -213,   380,   317,   510,
     155,  -213,   120,   125,   121,  -213,    22,   658,   121,  -213,
     132,  -213,  -213,   269,   667,    30,   424,    98,   485,   193,
     218,   133,   121,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   121,   317,  -213,  -213,  -213,   134,   118,   148,   258,
     148,   -11,    24,   -10,  -213,  -213,  -213,  -213,  -213,  -213,
     118,   118,   152,  -213,  -213,   136,   628,  -213,    38,   658,
    -213,   158,  -213,  -213,  -213,  -213,  -213,   111,  -213,   -19,
    -213,  -213,  -213,  -213,    45,   628,  -213,    48,  -213,    68,
      68,  -213,  -213,  -213,   269,   289,   289,   127,   127,   685,
     685,   685,   685,   685,   685,   667,   648,   628,   550,   137,
    -213,   121,   317,    21,   607,   121,    21,   118,   121,   118,
     121,   138,   121,   121,   160,   145,  -213,  -213,  -213,  -213,
     317,  -213,  -213,   143,   121,   507,   175,   156,  -213,  -213,
     121,   121,  -213,   121,   121,   167,   118,   317,   136,   121,
     628,  -213,  -213,  -213,   118,   434,  -213,   117,    21,   283,
     121,   628,  -213,   121,   118,  -213,   118,  -213,  -213,    80,
     121,   121,  -213
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -213,  -213,  -213,  -213,  -213,  -213,   139,  -213,  -213,     2,
    -213,  -213,  -213,  -213,  -213,  -213,  -212,  -213,   -96,  -213,
    -213,  -213,  -213,    56,   -92,  -117,   -87,   -35,  -213,  -213,
     -86,  -213,  -213,   110,  -213,  -213,  -213,  -213,  -213,  -213,
     -17,  -213,  -213,  -213,   142,  -213,  -213,  -213,  -213,   -34,
    -213,   129,    50,  -213,   171,   135,   -29,  -213,    12,   215,
    -213
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -156
static const yytype_int16 yytable[] =
{
      52,   156,    55,   187,   242,   160,    37,    38,   120,     3,
     161,   162,    86,    93,    33,    44,   110,   120,    91,    49,
     180,    39,    95,     4,   160,   111,   120,    49,   104,   161,
     162,    50,     5,   108,   112,   113,   114,    43,   118,   217,
     121,     8,    46,   117,    44,    45,   267,    67,    68,   221,
     107,    49,    64,   122,    49,   236,  -155,   141,   221,   173,
      49,   148,    36,   166,   149,   176,   169,    74,   150,   151,
     152,    46,    64,   230,   219,   227,   230,    59,   125,   126,
     127,    40,   231,    44,    45,   232,   177,   179,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   206,   146,   207,   230,   165,
      46,    47,    49,   103,   214,    49,    53,   272,   211,    64,
     215,   160,   218,   220,   160,    56,   161,   162,    96,   161,
     162,   222,   223,   181,   123,   124,    69,   125,   126,   127,
     256,    96,   269,    70,   160,    48,   229,    96,    49,   161,
     162,    96,   266,    49,   107,    71,    49,    72,    49,   265,
      65,   109,    72,   160,    73,   233,   160,    49,   161,   162,
      44,   161,   162,   248,    74,    49,   119,   235,    75,    76,
     234,   142,    77,   169,   212,   241,   170,    98,   243,   247,
     244,   171,    78,    79,    80,   250,   249,   258,   175,   188,
     208,    69,   224,   259,   245,    49,   216,   257,    70,   251,
     254,   115,   261,   172,   262,   148,   252,   260,   149,   228,
      71,    74,   150,   151,   152,   263,    69,    42,   168,    73,
     183,   106,     0,    70,     0,   270,   145,   271,     0,    74,
       0,   179,   174,    75,    76,    71,     0,    77,     0,     0,
       0,     0,     0,     0,    73,   186,     0,    78,    79,    80,
       0,     0,     0,     0,    74,     0,    69,     0,    75,    76,
       0,     0,    77,    70,     0,     0,   123,   124,     0,   125,
     126,   127,    78,    79,    80,    71,   131,   132,    49,     0,
       0,    69,     0,     0,    73,     0,   123,   124,    70,   125,
     126,   127,   128,     0,    74,     0,   131,   132,    75,    76,
      71,     0,    77,     0,     0,     0,     0,     0,     0,    73,
     268,     0,    78,    79,    80,    69,     0,     0,     0,    74,
       0,     0,    70,    75,    76,     0,     0,    77,     0,     0,
       0,     0,     0,     0,    71,     0,     0,    78,    79,    80,
       0,     0,     0,    73,     0,     0,     0,     0,     0,     0,
       4,     0,     0,    74,     0,     0,     0,    75,    76,     5,
       0,    77,     0,     0,     0,     6,     0,     7,     8,     0,
      98,    78,    79,    80,     0,     9,    10,    99,    11,    12,
      13,    14,    15,    16,    17,    18,    19,     0,     0,    20,
       0,     0,     0,     9,    10,     0,    11,    12,    13,    14,
      15,    16,    17,    18,    19,   147,     0,    61,     0,     0,
     148,     0,     0,   149,     0,     0,    74,   150,   151,   152,
     143,     0,     9,    10,     0,    11,    12,    13,    14,    15,
      16,    17,    18,    19,     0,     0,   153,     9,    10,     0,
      11,    12,    13,    14,    15,    16,    17,    18,    19,   178,
       0,    61,     0,     0,   148,     0,     0,   149,     0,   264,
      74,   150,   151,   152,   148,     0,     0,   149,     0,     0,
      74,   150,   151,   152,     0,     0,     0,     0,     0,     0,
     179,     0,   123,   124,     0,   125,   126,   127,   128,   129,
     179,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,     0,     0,   123,   124,     0,   125,   126,   127,
     128,   129,   182,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,    60,   105,     0,     0,     0,     0,
       0,     0,     0,     0,   253,     0,    99,   167,     0,     0,
       9,    10,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,     9,    10,    61,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    60,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,     0,
       9,    10,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,     9,    10,    61,    11,    12,    13,    14,    15,
      16,    17,    18,    19,   123,   124,    61,   125,   126,   127,
     128,   129,     0,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,     0,   123,   124,    49,   125,   126,
     127,   128,   129,     0,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   123,   124,     0,   125,   126,
     127,   128,   129,     0,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   123,   124,     0,   125,   126,   127,
     128,   129,     0,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   123,   124,     0,   125,   126,   127,   128,   129,
       0,   130,   131,   132,  -156,  -156,  -156,  -156,  -156,  -156,
       9,    10,     0,    11,    12,    13,    14,    15,    16,    17,
      18,    19,     9,    10,    61,    41,     0,    13,    14,    15,
      16,    17,    18,    19
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-213))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-156))

static const yytype_int16 yycheck[] =
{
      29,    97,    31,   120,   216,    97,     4,     5,    36,     0,
      97,    97,    47,    48,     2,     4,    28,    36,    47,    30,
     116,    66,    51,    27,   116,    37,    36,    30,    57,   116,
     116,    34,    36,    62,    69,    70,    71,    36,    73,    50,
      68,    45,    31,    72,     4,     5,   258,    45,    46,    68,
      28,    30,    40,    82,    30,    34,    66,    86,    68,    37,
      30,    40,    66,    98,    43,    35,    28,    46,    47,    48,
      49,    31,    60,    28,    50,    37,    28,    37,    10,    11,
      12,    66,    37,     4,     5,    37,   115,    66,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,    94,   142,    28,    97,
      31,    32,    30,    28,   149,    30,    34,    37,   147,   107,
     149,   213,   151,   152,   216,    66,   213,   213,    30,   216,
     216,   160,   161,    35,     7,     8,     8,    10,    11,    12,
     236,    30,   259,    15,   236,    14,    35,    30,    30,   236,
     236,    30,    35,    30,    28,    27,    30,    34,    30,   255,
      66,    66,    34,   255,    36,    28,   258,    30,   255,   255,
       4,   258,   258,    28,    46,    30,    36,   212,    50,    51,
     209,    29,    54,    28,    36,   214,    66,    29,   217,    29,
     219,    66,    64,    65,    66,   230,   225,    41,    66,    66,
      66,     8,    66,    36,    66,    30,   150,   236,    15,    66,
      35,    72,   247,   103,   248,    40,   233,   246,    43,   169,
      27,    46,    47,    48,    49,   254,     8,    12,    99,    36,
      37,    60,    -1,    15,    -1,   264,    94,   266,    -1,    46,
      -1,    66,   107,    50,    51,    27,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    37,    -1,    64,    65,    66,
      -1,    -1,    -1,    -1,    46,    -1,     8,    -1,    50,    51,
      -1,    -1,    54,    15,    -1,    -1,     7,     8,    -1,    10,
      11,    12,    64,    65,    66,    27,    17,    18,    30,    -1,
      -1,     8,    -1,    -1,    36,    -1,     7,     8,    15,    10,
      11,    12,    13,    -1,    46,    -1,    17,    18,    50,    51,
      27,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      37,    -1,    64,    65,    66,     8,    -1,    -1,    -1,    46,
      -1,    -1,    15,    50,    51,    -1,    -1,    54,    -1,    -1,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    64,    65,    66,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,    -1,
      27,    -1,    -1,    46,    -1,    -1,    -1,    50,    51,    36,
      -1,    54,    -1,    -1,    -1,    42,    -1,    44,    45,    -1,
      29,    64,    65,    66,    -1,    52,    53,    36,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    -1,    -1,    66,
      -1,    -1,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    35,    -1,    66,    -1,    -1,
      40,    -1,    -1,    43,    -1,    -1,    46,    47,    48,    49,
      35,    -1,    52,    53,    -1,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    -1,    -1,    66,    52,    53,    -1,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    35,
      -1,    66,    -1,    -1,    40,    -1,    -1,    43,    -1,    35,
      46,    47,    48,    49,    40,    -1,    -1,    43,    -1,    -1,
      46,    47,    48,    49,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    -1,     7,     8,    -1,    10,    11,    12,    13,    14,
      66,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,    -1,     7,     8,    -1,    10,    11,    12,
      13,    14,    37,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    36,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37,    -1,    36,    37,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    52,    53,    66,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    36,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      52,    53,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    52,    53,    66,    55,    56,    57,    58,    59,
      60,    61,    62,    63,     7,     8,    66,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,     7,     8,    30,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,     7,     8,    -1,    10,    11,
      12,    13,    14,    -1,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,     7,     8,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,     7,     8,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      52,    53,    -1,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    52,    53,    66,    55,    -1,    57,    58,    59,
      60,    61,    62,    63
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    70,    71,     0,    27,    36,    42,    44,    45,    52,
      53,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      66,    72,    73,    78,    79,    80,    81,   100,   103,   104,
     113,   119,   122,   127,   128,   129,    66,    78,    78,    66,
      66,    55,   128,    36,     4,     5,    31,    32,    14,    30,
      34,   105,   125,    34,   114,   125,    66,   101,   102,    37,
      36,    66,   123,   124,   127,    66,    82,    78,    78,     8,
      15,    27,    34,    36,    46,    50,    51,    54,    64,    65,
      66,    74,    75,    76,    93,    95,    96,    97,    98,    99,
     110,   125,   126,    96,   106,   125,    30,   115,    29,    36,
     120,   121,   127,    28,   125,    37,   123,    28,   125,    66,
      28,    37,    96,    96,    96,    75,    77,   125,    96,    36,
      36,    68,   125,     7,     8,    10,    11,    12,    13,    14,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,   125,    29,    35,   107,   113,   127,    35,    40,    43,
      47,    48,    49,    66,    83,    84,    87,    88,    89,    90,
      93,    95,    99,   111,   116,   127,    96,    37,   120,    28,
      66,    66,   102,    37,   124,    66,    35,   125,    35,    66,
      87,    35,    37,    37,    94,    96,    37,    94,    66,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    66,   108,
     109,   125,    36,    92,    96,   125,    92,    50,   125,    50,
     125,    68,   125,   125,    66,   117,   118,    37,   121,    35,
      28,    37,    37,    28,   125,    96,    34,    85,    87,    91,
     125,   125,    85,   125,   125,    66,   112,    29,    28,   125,
      96,    66,   109,    37,    35,    86,    87,   125,    41,    36,
     125,    96,   118,   125,    35,    87,    35,    85,    37,    94,
     125,   125,    37
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 395 "E--_parser.y++"
    {   prt("Spec");    }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 403 "E--_parser.y++"
    {   stm.leaveScope();   stm.insert((yyvsp[(1) - (1)].eventEntry)); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 404 "E--_parser.y++"
    {   stm.leaveScope();   stm.insert((yyvsp[(1) - (1)].functionEntry)); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 405 "E--_parser.y++"
    {   stm.leaveScope();                   }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 409 "E--_parser.y++"
    {   (yyval.ruleNode) = allocRuleNode((yyvsp[(1) - (3)].patVal), (yyvsp[(3) - (3)].stmtVal)); prt("Rule");   }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 412 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (2)].stmt_plusVal);    }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 413 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].stmt_plusVal);    }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 414 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode((yyvsp[(1) - (2)].exprVal)); prt("AssignStmt");  }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 415 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode(NULL);   }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 418 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocScopeNode((yyvsp[(2) - (3)].stmt_plusVal));    prt("CompoundStmt");   }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 419 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocScopeNode((yyvsp[(2) - (4)].stmt_plusVal));    prt("CompoundStmt");   }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 420 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = (yyvsp[(1) - (1)].stmt_plusVal);    }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 423 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(curStmtList1); curStmtList1 = NULL;  prt("CompoundStmt");  }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 424 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(new list<StmtNode*>);                prt("CompoundStmt");  }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 427 "E--_parser.y++"
    {   if (!curStmtList1) curStmtList1 = new list<StmtNode*>; curStmtList1->push_back((yyvsp[(2) - (2)].stmtVal));}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 431 "E--_parser.y++"
    {   (yyval.patVal) = allocPrimPatNode((yyvsp[(1) - (1)].eventEntry), curVarList, NULL);    curVarList = NULL;    prt("PrimitivePat"); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 432 "E--_parser.y++"
    {   (yyval.patVal) = allocPrimPatNode((yyvsp[(1) - (1)].eventEntry), curVarList, NULL);    curVarList = NULL;    prt("PrimitivePat"); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 433 "E--_parser.y++"
    {   (yyval.patVal) = allocPrimPatNode((yyvsp[(1) - (3)].eventEntry), curVarList, (yyvsp[(3) - (3)].exprVal));      curVarList = NULL;    prt("PrimitivePatWithCond");  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 434 "E--_parser.y++"
    {   (yyval.patVal) = (yyvsp[(1) - (1)].patVal);                        }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 435 "E--_parser.y++"
    {   (yyval.patVal) = (yyvsp[(2) - (3)].patVal);    prt("(EventPat)");  }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 438 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::OR  , (yyvsp[(1) - (3)].patVal), (yyvsp[(3) - (3)].patVal));   prt("EventPat \\/ EventPat");  }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 439 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::SEQ , (yyvsp[(1) - (3)].patVal), (yyvsp[(3) - (3)].patVal));   prt("EventPat : EventPat");    }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 440 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::STAR, (yyvsp[(1) - (2)].patVal), NULL); prt("EventPat **");            }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 441 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::NEG , (yyvsp[(2) - (2)].patVal), NULL); prt("!EventPat");              }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 444 "E--_parser.y++"
    {   allocRuleBlockNode();   (yyval.eventEntry) = any;   prt("anyEvent");    }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 447 "E--_parser.y++"
    {   (yyval.eventEntry) = getEventEntry((yyvsp[(1) - (4)].cVal));             prt("NormalEvent"); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 451 "E--_parser.y++"
    {   curVarList->push_back(allocVarEntry((yyvsp[(3) - (3)].cVal), VariableEntry::UNDEFINED, new Type(Type::UNKNOWN), NULL)); prt("EventFormalParam");}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 452 "E--_parser.y++"
    {   allocRuleBlockNode(); curVarList = new vector<VariableEntry*>(); 
                                                                                curVarList->push_back(allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::UNDEFINED, new Type(Type::UNKNOWN), NULL)); prt("EventFormalParam");}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 457 "E--_parser.y++"
    {   (yyval.whileVal) = allocWhileNode((yyvsp[(2) - (3)].exprVal), (yyvsp[(3) - (3)].stmtVal));  prt("WhileStatement"); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 460 "E--_parser.y++"
    {   (yyval.ifVal) = allocIfNode((yyvsp[(2) - (3)].exprVal), (yyvsp[(3) - (3)].stmtVal), NULL); prt("IfThenStmt");          }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 461 "E--_parser.y++"
    {   (yyval.ifVal) = allocIfNode((yyvsp[(2) - (5)].exprVal), (yyvsp[(3) - (5)].stmtVal), (yyvsp[(5) - (5)].stmtVal));   prt("IfThenElseStmt");      }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 464 "E--_parser.y++"
    {   (yyval.stmtVal) = allocCompStmtNode((yyvsp[(2) - (4)].stmtList)); prt("CompoundStmt");   }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 465 "E--_parser.y++"
    {   (yyval.stmtVal) = allocCompStmtNode((yyvsp[(2) - (3)].stmtList)); prt("CompoundStmt");   }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 466 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].stmt_plusVal);                                                    }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 467 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].stmtVal);                                                    }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 468 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode(NULL);                               }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 471 "E--_parser.y++"
    {   if((yyvsp[(2) - (2)].stmtVal)) (yyval.stmtList)->push_back((yyvsp[(2) - (2)].stmtVal));                               }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 472 "E--_parser.y++"
    {   (yyval.stmtList) = new list<StmtNode*>; if((yyvsp[(1) - (1)].stmtVal)) (yyval.stmtList)->push_back((yyvsp[(1) - (1)].stmtVal));     }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 475 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode((yyvsp[(1) - (2)].exprVal)); prt("FunctionInvocationStmt");  }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 476 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode((yyvsp[(1) - (2)].exprVal)); prt("AssignStmt");              }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 477 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].ifVal);                                                    }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 478 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].returnVal);                                                    }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 479 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].whileVal);						    }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 480 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].breakVal);                                                    }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 481 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].continueVal);                                                    }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 482 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode((yyvsp[(1) - (1)].classFuncVal));    prt("ObjectFunctionCall");   }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 485 "E--_parser.y++"
    {   (yyval.returnVal) = allocReturnNode((yyvsp[(2) - (3)].exprVal));   prt("ReturnStmt");              }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 486 "E--_parser.y++"
    {   (yyval.returnVal) = allocReturnNode(NULL); prt("ReturnStmt");              }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 489 "E--_parser.y++"
    {	(yyval.breakVal) = allocBreakNode(allocValueNode(new Value(1,  Type::UINT)));     prt("BreakStmt");   }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 490 "E--_parser.y++"
    {   (yyval.breakVal) = allocBreakNode(allocValueNode(new Value((yyvsp[(2) - (3)].uVal), Type::UINT)));     prt("BreakStmt");   }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 493 "E--_parser.y++"
    {   (yyval.continueVal) = allocContinueNode(allocValueNode(new Value(1,  Type::UINT)));  prt("ContinueStmt");}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 494 "E--_parser.y++"
    {   (yyval.continueVal) = allocContinueNode(allocValueNode(new Value((yyvsp[(2) - (3)].uVal), Type::UINT)));  prt("ContinueStmt");}
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 497 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(new list<StmtNode*>); prt("CompoundStmt");  }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 498 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(new list<StmtNode*>); prt("CompoundStmt");  }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 499 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(new list<StmtNode*>); prt("CompoundStmt");  }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 500 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(new list<StmtNode*>); prt("CompoundStmt");  }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 503 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(2) - (3)].exprVal);    prt("(Expr)"); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 506 "E--_parser.y++"
    {   (yyval.exprVal) = allocInvNode  ((yyvsp[(1) - (4)].cVal), curExprList); curExprList = NULL;   prt("FunctionInvocation"); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 507 "E--_parser.y++"
    {   (yyval.exprVal) = allocInvNode  ((yyvsp[(1) - (3)].cVal), NULL);                              prt("FunctionInvocation"); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 508 "E--_parser.y++"
    {   (yyval.exprVal) = allocPrintNode(curExprList);     curExprList = NULL;   prt("PrintFunctionNode");  }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 509 "E--_parser.y++"
    {   (yyval.exprVal) = allocPrintNode  ( NULL);                               prt("FunctionInvocation"); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 512 "E--_parser.y++"
    {   curExprList->push_back((yyvsp[(3) - (3)].exprVal));                                             }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 513 "E--_parser.y++"
    {   curExprList = new vector<ExprNode*>(); curExprList->push_back((yyvsp[(1) - (1)].exprVal));      }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 516 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::ASSIGN, (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Assignment");    }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 519 "E--_parser.y++"
    {   (yyval.exprVal) = allocValueNode((yyvsp[(1) - (1)].valVal));    }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 520 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 521 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 522 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 523 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 524 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 525 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(2) - (3)].exprVal);    prt("(Expr)");  }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 528 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::NOT   , (yyvsp[(2) - (2)].exprVal), NULL); prt("!Expr");         }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 529 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITNOT, (yyvsp[(2) - (2)].exprVal), NULL); prt("~Expr");         }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 530 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::UMINUS, (yyvsp[(2) - (2)].exprVal), NULL); prt("-Expr");         }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 533 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::OR    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr || Expr");    }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 534 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::AND   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr && Expr");    }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 535 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::EQ    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr == Expr");    }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 536 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::NE    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr != Expr");    }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 537 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::GT    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr > Expr");     }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 538 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::LT    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr < Expr");     }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 539 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::GE    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr >= Expr");    }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 540 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::LE    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr <= Expr");    }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 541 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITOR , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr | Expr");     }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 542 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITXOR, (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr ^ Expr");     }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 543 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITAND, (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr & Expr");     }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 544 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::SHL   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr << Expr");    }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 545 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::SHR   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr >> Expr");    }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 546 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::PLUS  , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr + Expr");     }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 547 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::MINUS , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr - Expr");     }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 548 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::MULT  , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr * Expr");     }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 549 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::DIV   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr / Expr");     }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 550 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::MOD   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr % Expr");     }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 553 "E--_parser.y++"
    {   (yyval.exprVal) = allocRefExprNode((yyvsp[(1) - (1)].cVal)); prt("RefExpr"); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 554 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);    prt("Object Function Call");   }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 564 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::GLOBAL_VAR, curType, NULL); prt("VariableDecl");   }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 565 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (3)].cVal), VariableEntry::GLOBAL_VAR, curType, (yyvsp[(3) - (3)].exprVal)); prt("VariableDeclWithInit");}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 568 "E--_parser.y++"
    {   (yyval.classEntry) = (yyvsp[(1) - (2)].classEntry);                                                   }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 569 "E--_parser.y++"
    {   (yyval.classEntry) = (yyvsp[(1) - (3)].classEntry); (yyval.classEntry)->type()->size(curClassSize); curClassSize = 0; }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 572 "E--_parser.y++"
    {   (yyval.classEntry) = allocClassEntry((yyvsp[(2) - (2)].cVal)); prt("ClassDecl");  }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 579 "E--_parser.y++"
    {   stm.leaveScope();   stm.insert((yyvsp[(2) - (2)].functionEntry));         }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 590 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::CLASS_VAR, curType, NULL); prt("ClassVarDecl");  
                  ;                                                             curClassSize += (yyval.variableEntry)->type()->size();         }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 594 "E--_parser.y++"
    {   (yyval.exprVal) = allocClassRefExprNode((yyvsp[(1) - (3)].cVal), (yyvsp[(3) - (3)].cVal)); prt("classVariableCall");     }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 597 "E--_parser.y++"
    {   (yyval.classFuncVal) = allocClassFuncInvNode((yyvsp[(1) - (4)].cVal), funcName, funcParamList); prt("classFuntionInvocation");   }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 600 "E--_parser.y++"
    {   funcName = (yyvsp[(1) - (4)].cVal); funcParamList = curExprList; curExprList = NULL; prt("ClassFunctionInvocation"); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 601 "E--_parser.y++"
    {   funcName = (yyvsp[(1) - (3)].cVal); funcParamList = NULL;                            prt("ClassFunctionInvocation"); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 604 "E--_parser.y++"
    {   (yyval.functionEntry) = (yyvsp[(1) - (2)].functionEntry);                    }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 605 "E--_parser.y++"
    {   (yyvsp[(1) - (2)].functionEntry)->body((yyvsp[(2) - (2)].stmt_plusVal));   (yyval.functionEntry) = (yyvsp[(1) - (2)].functionEntry);    }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 608 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(curStmtList3);        curStmtList3 = NULL;     prt("FunctionBody");  }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 609 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(curStmtList3);        curStmtList3 = NULL;     prt("FunctionBody");  }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 613 "E--_parser.y++"
    {   if (!curStmtList3) curStmtList3 = new list<StmtNode*>; curStmtList3->push_back((yyvsp[(2) - (2)].stmtVal)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 624 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::LOCAL_VAR, curType, NULL); prt("VariableDecl");    }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 625 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (3)].cVal), VariableEntry::LOCAL_VAR, curType, (yyvsp[(3) - (3)].exprVal)); prt("VariableDeclWithInit"); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 628 "E--_parser.y++"
    {   (yyval.functionEntry) = allocFuncEntry((yyvsp[(2) - (3)].cVal), (yyvsp[(1) - (3)].typeVal), curTypeList);   curTypeList = NULL;     prt("FunctionDecl");  }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 631 "E--_parser.y++"
    {   prt("FormalParamList");     }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 632 "E--_parser.y++"
    {   curTypeList->push_back((yyvsp[(3) - (3)].typeVal)); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 633 "E--_parser.y++"
    {   curTypeList = new vector<Type*>(); curTypeList->push_back((yyvsp[(1) - (1)].typeVal));  }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 634 "E--_parser.y++"
    {   curFuncEnt = allocFuncEntry();  }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 636 "E--_parser.y++"
    {   if (!curFuncEnt) curFuncEnt = allocFuncEntry();  
                                                                                (yyval.typeVal) = (yyvsp[(1) - (2)].typeVal); allocVarEntry((yyvsp[(2) - (2)].cVal), VariableEntry::PARAM_VAR, (yyvsp[(1) - (2)].typeVal), NULL);  prt("FormalParam"); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 640 "E--_parser.y++"
    {   (yyval.eventEntry) = allocEventEntry((yyvsp[(2) - (4)].cVal), curTypeList);  curTypeList = NULL;     prt("EventDecl");  }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 643 "E--_parser.y++"
    {   prt("FormalParamList");         }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 644 "E--_parser.y++"
    {   curTypeList->push_back((yyvsp[(3) - (3)].typeVal));     }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 645 "E--_parser.y++"
    {   curTypeList = new vector<Type*>(); curTypeList->push_back((yyvsp[(1) - (1)].typeVal));}
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 646 "E--_parser.y++"
    {   curEventEnt = allocEventEntry();}
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 649 "E--_parser.y++"
    {   if (!curEventEnt) curEventEnt = allocEventEntry();  
                                                                                (yyval.typeVal) = (yyvsp[(1) - (2)].typeVal); allocVarEntry((yyvsp[(2) - (2)].cVal), VariableEntry::PARAM_VAR, (yyvsp[(1) - (2)].typeVal), NULL); prt("FormalParam");}
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 658 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].uVal), Type::UINT); prt("Literal");     }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 659 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].dVal));             prt("Literal");     }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 660 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].cVal));             prt("Literal");     }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 661 "E--_parser.y++"
    {   (yyval.valVal) = new Value(true);           prt("Literal");     }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 662 "E--_parser.y++"
    {   (yyval.valVal) = new Value(false);          prt("Literal");     }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 665 "E--_parser.y++"
    {   (yyval.typeVal) = curType = (yyvsp[(1) - (1)].typeVal);                  prt("Type");    }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 666 "E--_parser.y++"
    {   (yyval.typeVal) = curType = (yyvsp[(1) - (1)].typeVal);                  prt("Type");    }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 667 "E--_parser.y++"
    {   (yyval.typeVal) = curType = getUserType((yyvsp[(1) - (1)].cVal));     prt("Type");    }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 668 "E--_parser.y++"
    {   (yyval.typeVal) = curType = new Type(Type::VOID);prt("Type");
                                                                                errMsg("Invalid use of keyword \"unsigned\"");      }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 672 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::VOID);    }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 673 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::BOOL);    }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 674 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::BYTE);    }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 675 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::STRING);  }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 676 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::DOUBLE);  }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 677 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::INT);     
                                                                                warnMsg("Unsupported datatype, converting 'short' to 'int'");   }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 679 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::INT);     
                                                                                warnMsg("Unsupported datatype, converting 'long' to 'int'");    }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 681 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::INT);
                                                                                warnMsg("Unsupported datatype, Converting 'bit' to 'int'");     }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 683 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::VOID);
                                                                                errMsg("Unsupported datatype 'enum'");  }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 687 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::INT);     }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 688 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::UINT);    }
    break;



/* Line 1806 of yacc.c  */
#line 3126 "E--_parser.C"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 691 "E--_parser.y++"


