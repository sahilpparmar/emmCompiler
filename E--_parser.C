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
#line 1 "E--_parser.y++"


#include "Ast.h"
#include "ParserUtil.h"
#include "Error.h"

using namespace std;

extern int verbosity;
extern int yylex();
extern void yyerror(const char *s);
extern const OpNode::OpInfo opInfo[];

#define prt(x) //cout << x << endl;

static Type                   *curType      = NULL;
static Value                  *tempValNode  = NULL;
static vector<ExprNode*>      *curExprList  = NULL;
static list<StmtNode*>        *curStmtList1 = NULL;
static list<StmtNode*>        *curStmtList3 = NULL;
static vector<Type*>          *curTypeList  = NULL; 
static vector<VariableEntry*> *curVarList   = NULL; 
static FunctionEntry          *curFuncEnt   = NULL;
static EventEntry             *curEventEnt  = NULL;
static RuleBlockEntry         *curRBlkNode  = NULL;
static int                    curClassSize  = 0;

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

    if (strcmp(str, "print") == 0) {
        sym = new FunctionEntry(string("print"), NULL);
    } else if (!(IsFuncSymEntry(sym) || IsEventSymEntry(sym))) {
        errMsg((string)"\"" + (string)str + (string)"\" is not a function or module name");
        return NULL;
    }
    return new InvocationNode(sym, param_l, yylinenum, yycolumnno, string(yyfilename));
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
    return new ReturnStmtNode(e, (FunctionEntry*)stm.currentScope(SymTabEntry::Kind::FUNCTION_KIND));
}

static BreakStmtNode* allocBreakNode(ExprNode* e) {
    //if (IsCurRuleBlockEntry())
    //    errMsg((string)"Return statement is valid only within a function");
    //return new BreakStmtNode(e, (FunctionEntry*)stm.currentScope(SymTabEntry::Kind::FUNCTION_KIND));
    return new BreakStmtNode(e);
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
#line 309 "E--_parser.C"

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
     TOK_UINTNUM = 304,
     TOK_DOUBLENUM = 305,
     TOK_VOID = 306,
     TOK_STRING = 307,
     TOK_STRCONST = 308,
     TOK_INT = 309,
     TOK_UNSIGNED = 310,
     TOK_BOOL = 311,
     TOK_BYTE = 312,
     TOK_DOUBLE = 313,
     TOK_SHORT = 314,
     TOK_LONG = 315,
     TOK_BIT = 316,
     TOK_ENUM = 317,
     TOK_TRUE = 318,
     TOK_FALSE = 319,
     TOK_IDENT = 320,
     TOK_DOUBLE_COLON = 321,
     TOK_DOT = 322
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 238 "E--_parser.y++"

    unsigned int        uVal;
    double              dVal;
    char*               cVal;
    vector<string>*     strVector;
    Value*              valVal;

    ExprNode*           exprVal;
    vector<ExprNode*>*  exprList;
    RefExprNode*        refexpVal;

    ClassEntry*         classEntry;
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
    ReturnStmtNode*     returnVal;
    CompoundStmtNode*   stmt_plusVal;
    RuleNode*           ruleNode;

    BasePatNode*        patVal;
    PrimitivePatNode*   primPatVal;
    vector<RuleNode*>*  transList;

    const OpNode::OpInfo*   funIval;



/* Line 293 of yacc.c  */
#line 450 "E--_parser.C"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 462 "E--_parser.C"

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
#define YYLAST   561

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  58
/* YYNRULES -- Number of rules.  */
#define YYNRULES  152
/* YYNRULES -- Number of states.  */
#define YYNSTATES  246

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   322

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
      65,    66,    67
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
     143,   145,   147,   151,   154,   157,   161,   166,   170,   174,
     179,   183,   185,   187,   191,   193,   197,   199,   201,   203,
     205,   207,   209,   213,   216,   219,   222,   226,   230,   234,
     238,   242,   246,   250,   254,   258,   262,   266,   270,   274,
     278,   282,   286,   290,   294,   296,   300,   304,   306,   308,
     312,   315,   319,   322,   326,   329,   332,   333,   337,   341,
     343,   345,   348,   351,   356,   360,   363,   366,   367,   371,
     375,   377,   379,   383,   387,   391,   395,   397,   400,   403,
     408,   412,   416,   418,   421,   424,   427,   429,   431,   433,
     435,   437,   439,   441,   443,   445,   448,   450,   452,   454,
     456,   458,   460
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      69,     0,    -1,    70,    -1,    70,    71,    -1,    70,    72,
      -1,    -1,   118,    -1,   109,    -1,   102,    -1,    99,    -1,
      77,    32,    73,    -1,    74,   121,    -1,    74,    -1,    95,
     121,    -1,   121,    -1,    34,    74,    35,    -1,    34,    74,
     121,    35,    -1,    75,    -1,    34,    76,    35,    -1,    34,
     121,    35,    -1,    76,    86,    -1,    -1,    80,    -1,    79,
      -1,    80,    14,    95,    -1,    78,    -1,    36,    77,    37,
      -1,    77,     5,    77,    -1,    77,    31,    77,    -1,    77,
       4,    -1,    27,    77,    -1,    45,    -1,    65,    36,    81,
      37,    -1,    81,    28,    65,    -1,    65,    -1,    -1,    47,
      90,    84,    -1,    40,    90,    84,    -1,    40,    90,    84,
      41,    84,    -1,    34,    85,    35,   121,    -1,    34,    85,
      35,    -1,    89,    -1,    86,    -1,   121,    -1,    85,    86,
      -1,    86,    -1,    91,   121,    -1,    94,   121,    -1,    83,
      -1,    87,    -1,    82,    -1,    88,    -1,    43,    95,   121,
      -1,    43,   121,    -1,    48,   121,    -1,    48,    49,   121,
      -1,    34,   121,    35,   121,    -1,    34,   121,    35,    -1,
      36,    95,    37,    -1,    92,    36,    93,    37,    -1,    92,
      36,    37,    -1,    65,    -1,    46,    -1,    93,    28,    95,
      -1,    95,    -1,    98,    29,    95,    -1,   122,    -1,    98,
      -1,    91,    -1,    94,    -1,    96,    -1,    97,    -1,    36,
      95,    37,    -1,    27,    95,    -1,    15,    95,    -1,     8,
      95,    -1,    95,    26,    95,    -1,    95,    25,    95,    -1,
      95,    23,    95,    -1,    95,    24,    95,    -1,    95,    19,
      95,    -1,    95,    20,    95,    -1,    95,    21,    95,    -1,
      95,    22,    95,    -1,    95,    14,    95,    -1,    95,    16,
      95,    -1,    95,    13,    95,    -1,    95,    17,    95,    -1,
      95,    18,    95,    -1,    95,     7,    95,    -1,    95,     8,
      95,    -1,    95,    10,    95,    -1,    95,    11,    95,    -1,
      95,    12,    95,    -1,    65,    -1,   123,   100,   121,    -1,
     100,    28,   101,    -1,   101,    -1,    65,    -1,    65,    29,
      95,    -1,   103,   121,    -1,   103,   104,   121,    -1,    42,
      65,    -1,    34,   105,    35,    -1,   105,   106,    -1,   105,
     109,    -1,    -1,   123,   107,   121,    -1,   107,    28,   108,
      -1,   108,    -1,    65,    -1,   115,   121,    -1,   115,   110,
      -1,    34,   111,    35,   121,    -1,    34,   111,    35,    -1,
     111,   112,    -1,   111,    86,    -1,    -1,   123,   113,   121,
      -1,   113,    28,   114,    -1,   114,    -1,    65,    -1,    65,
      29,    95,    -1,   123,    65,   116,    -1,    36,   116,    37,
      -1,   116,    28,   117,    -1,   117,    -1,    36,    37,    -1,
     123,    65,    -1,    44,    65,   119,   121,    -1,    36,   119,
      37,    -1,   119,    28,   120,    -1,   120,    -1,    36,    37,
      -1,   123,    65,    -1,   121,    30,    -1,    30,    -1,    49,
      -1,    50,    -1,    53,    -1,    63,    -1,    64,    -1,   124,
      -1,   125,    -1,    65,    -1,    55,   124,    -1,    51,    -1,
      56,    -1,    57,    -1,    52,    -1,    58,    -1,    54,    -1,
      55,    54,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   348,   348,   351,   352,   353,   356,   357,   358,   359,
     362,   365,   366,   367,   368,   371,   372,   373,   376,   377,
     380,   381,   384,   385,   386,   387,   388,   391,   392,   393,
     394,   397,   400,   404,   405,   407,   410,   413,   414,   417,
     418,   419,   420,   421,   424,   425,   428,   429,   430,   431,
     432,   433,   436,   437,   440,   441,   444,   445,   448,   451,
     452,   455,   456,   459,   460,   463,   466,   467,   468,   469,
     470,   471,   472,   475,   476,   477,   480,   481,   482,   483,
     484,   485,   486,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   500,   503,   506,   507,   510,   511,
     514,   515,   518,   521,   524,   525,   526,   529,   532,   533,
     536,   541,   542,   545,   546,   549,   550,   551,   554,   557,
     558,   561,   562,   565,   568,   569,   570,   571,   573,   577,
     580,   581,   582,   583,   586,   590,   591,   595,   596,   597,
     598,   599,   602,   603,   604,   605,   609,   610,   611,   612,
     613,   616,   617
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
  "TOK_BREAK", "TOK_UINTNUM", "TOK_DOUBLENUM", "TOK_VOID", "TOK_STRING",
  "TOK_STRCONST", "TOK_INT", "TOK_UNSIGNED", "TOK_BOOL", "TOK_BYTE",
  "TOK_DOUBLE", "TOK_SHORT", "TOK_LONG", "TOK_BIT", "TOK_ENUM", "TOK_TRUE",
  "TOK_FALSE", "TOK_IDENT", "TOK_DOUBLE_COLON", "TOK_DOT", "$accept",
  "s_specification", "specification", "declaration", "rule", "rule_stmts",
  "compound_stmt2", "compound_stmt1", "stmt_plus1", "primitive_all",
  "binary_event_oprs", "any_event", "normal_event", "event_aparam_l",
  "while_stmt", "if_then_else_stmt", "basic_block", "stmt_plus2",
  "simple_stmt", "return_stmt", "break_stmt", "empty_block", "condition",
  "func_invocation", "func_name", "caller_param_l", "assignment",
  "expression", "unary_operation", "binary_operation", "ref_expr",
  "global_decl", "global_l", "global_var", "class_proto", "class_decl",
  "class_defn", "class_body", "class_var_decl", "class_var_l", "class_var",
  "function_proto", "function_defn", "function_body", "local_decl",
  "local_l", "local_var", "function_decl", "func_param_l", "func_param",
  "event_decl", "event_param_l", "event_param", "semi_colon", "literal",
  "type", "baseType", "intType", 0
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
     315,   316,   317,   318,   319,   320,   321,   322
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    68,    69,    70,    70,    70,    71,    71,    71,    71,
      72,    73,    73,    73,    73,    74,    74,    74,    75,    75,
      76,    76,    77,    77,    77,    77,    77,    78,    78,    78,
      78,    79,    80,    81,    81,    81,    82,    83,    83,    84,
      84,    84,    84,    84,    85,    85,    86,    86,    86,    86,
      86,    86,    87,    87,    88,    88,    89,    89,    90,    91,
      91,    92,    92,    93,    93,    94,    95,    95,    95,    95,
      95,    95,    95,    96,    96,    96,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    98,    99,   100,   100,   101,   101,
     102,   102,   103,   104,   105,   105,   105,   106,   107,   107,
     108,   109,   109,   110,   110,   111,   111,   111,   112,   113,
     113,   114,   114,   115,   116,   116,   116,   116,   117,   118,
     119,   119,   119,   119,   120,   121,   121,   122,   122,   122,
     122,   122,   123,   123,   123,   123,   124,   124,   124,   124,
     124,   125,   125
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     0,     1,     1,     1,     1,
       3,     2,     1,     2,     1,     3,     4,     1,     3,     3,
       2,     0,     1,     1,     3,     1,     3,     3,     3,     2,
       2,     1,     4,     3,     1,     0,     3,     3,     5,     4,
       3,     1,     1,     1,     2,     1,     2,     2,     1,     1,
       1,     1,     3,     2,     2,     3,     4,     3,     3,     4,
       3,     1,     1,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     3,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     1,     1,     3,
       2,     3,     2,     3,     2,     2,     0,     3,     3,     1,
       1,     2,     2,     4,     3,     2,     2,     0,     3,     3,
       1,     1,     3,     3,     3,     3,     1,     2,     2,     4,
       3,     3,     1,     2,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     2,     1,     0,     0,     0,     0,    31,   146,
     149,   151,     0,   147,   148,   150,   144,     3,     4,     0,
      25,    23,    22,     9,     8,     0,     7,     0,     6,     0,
     142,   143,     0,    30,     0,   102,     0,   152,   145,    35,
      29,     0,     0,     0,     0,   136,   106,     0,   100,   117,
     112,   111,    98,     0,    97,    26,     0,   144,     0,   132,
       0,    34,     0,    27,    28,     0,     0,     0,    21,     0,
      62,   137,   138,   139,   140,   141,    94,    10,    12,    17,
      68,     0,    69,     0,    70,    71,    67,    14,    66,    24,
       0,   101,   135,     0,     0,     0,   123,   126,     0,     0,
      95,   133,     0,     0,   129,   134,     0,    32,    75,    74,
      73,     0,     0,     0,     0,    11,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    13,     0,   103,   104,   105,
       0,   114,     0,     0,     0,     0,    61,    50,    48,   116,
      49,    51,     0,     0,     0,   115,     0,    99,   127,     0,
       0,   128,    98,    96,   130,   131,    33,    15,     0,    18,
      20,    19,    72,    60,     0,    64,    89,    90,    91,    92,
      93,    86,    84,    85,    87,    88,    80,    81,    82,    83,
      78,    79,    77,    76,    65,   110,     0,   109,   113,     0,
       0,     0,    53,     0,     0,    54,    46,    47,   121,     0,
     120,   124,   125,    16,     0,    59,     0,   107,     0,     0,
      37,    42,    41,    43,    52,    36,    55,     0,     0,   118,
      63,   110,   108,    58,     0,    45,     0,     0,   122,   119,
      40,    44,    57,    38,    39,    56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    17,    18,    77,    78,    79,   112,    19,
      20,    21,    22,    62,   147,   148,   220,   234,   221,   150,
     151,   222,   200,    80,    81,   174,    82,    83,    84,    85,
      86,    23,    53,    54,    24,    25,    47,    90,   138,   196,
     197,    26,    50,    93,   155,   209,   210,    27,    96,    97,
      28,    58,    59,   223,    88,    98,    30,    31
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -197
static const yytype_int16 yypact[] =
{
    -197,     3,   207,  -197,   -26,   -26,   -54,   -44,  -197,  -197,
    -197,  -197,   217,  -197,  -197,  -197,    -9,  -197,  -197,    25,
    -197,  -197,    38,  -197,  -197,    76,  -197,    79,  -197,   -23,
    -197,  -197,    -9,  -197,   135,  -197,   387,  -197,  -197,    -5,
    -197,   -26,   -26,   128,    -2,  -197,  -197,    87,   105,  -197,
    -197,   105,   175,   131,  -197,  -197,   363,  -197,   137,  -197,
      83,  -197,   -11,    28,   169,    -2,    -2,    -2,    95,    -2,
    -197,  -197,  -197,  -197,  -197,  -197,   150,  -197,    87,  -197,
    -197,   154,  -197,   455,  -197,  -197,   112,   105,  -197,    59,
     315,   105,  -197,   300,    -2,   375,   161,  -197,   130,   140,
     105,  -197,    27,   351,   105,  -197,   147,  -197,  -197,   543,
     495,   102,   339,   153,   286,   105,   160,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,    -2,
      -2,    -2,    -2,    -2,    -2,   105,    -2,  -197,  -197,  -197,
     149,    87,   166,   192,   166,    19,   -15,  -197,  -197,  -197,
    -197,  -197,    87,    87,   112,  -197,   155,    59,  -197,    96,
     351,  -197,   189,  -197,  -197,  -197,  -197,  -197,   173,  -197,
    -197,  -197,  -197,  -197,   126,    59,   225,   225,  -197,  -197,
    -197,   543,   531,   531,   278,   278,   513,   513,   513,   513,
     513,   513,   495,   476,    59,   399,   152,  -197,   105,    -2,
     236,   455,   105,   236,    87,   105,   105,   105,   210,   171,
    -197,  -197,  -197,  -197,    -2,  -197,   179,   105,   308,   104,
     205,  -197,  -197,   105,   105,  -197,   105,    -2,   155,   105,
      59,  -197,  -197,  -197,   348,  -197,   186,   236,    59,  -197,
      87,  -197,    87,  -197,   105,   105
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -197,  -197,  -197,  -197,  -197,  -197,   180,  -197,  -197,     4,
    -197,  -197,  -197,  -197,  -197,  -197,  -196,  -197,   -89,  -197,
    -197,  -197,   103,   -88,  -197,  -197,   -81,   -29,  -197,  -197,
     -77,  -197,  -197,   151,  -197,  -197,  -197,  -197,  -197,  -197,
      37,   164,  -197,  -197,  -197,  -197,    32,  -197,   172,   117,
    -197,   222,   177,   -25,  -197,    18,   269,  -197
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -145
static const yytype_int16 yytable[] =
{
      48,     4,    51,     3,   149,   152,    65,   225,    33,    34,
       5,    35,   153,    66,   -94,    89,   154,   106,    87,     8,
      29,    36,    91,   170,   152,    67,   107,    39,   100,    40,
      41,   153,    40,   104,    69,   154,   108,   109,   110,    32,
     114,   243,    52,   113,    70,    63,    64,    71,    72,    45,
    -144,    73,    44,   115,    60,   103,    42,    43,   135,    42,
      61,    74,    75,    76,   164,   157,   117,   118,   204,   119,
     120,   121,   122,   123,    60,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   168,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,    45,   194,   140,    45,
      46,   156,   152,    49,   201,   152,   198,    45,   202,   153,
     205,    60,   153,   154,   160,    45,   154,   206,   207,    68,
     235,   152,    45,   211,    45,    92,    65,   167,   153,    40,
      41,   136,   154,    66,   142,   241,   152,   143,   105,   152,
      70,   144,   145,   153,   214,    67,   153,   154,    45,    99,
     154,    45,    68,   215,    69,   103,    42,    45,    65,    76,
     218,   217,    55,    40,    70,    66,   224,    71,    72,   226,
     216,    73,    45,    92,   229,   230,   -61,    67,   171,   160,
     116,    74,    75,    76,   236,   161,    69,   173,   238,   228,
      65,    45,   199,    92,    94,   162,    70,    66,   213,    71,
      72,    95,   166,    73,   195,   244,    92,   245,    94,    67,
     208,   242,    45,    74,    75,    76,     9,    10,    69,    11,
      12,    13,    14,    15,     4,   119,   120,   121,    70,   227,
      57,    71,    72,     5,   231,    73,   237,   203,   111,     6,
     163,     7,     8,   232,   139,    74,    75,    76,     9,    10,
     239,    11,    12,    13,    14,    15,    45,   159,     9,    10,
     219,    37,    16,    13,    14,    15,   142,   212,   102,   143,
     165,    38,    70,   144,   145,   117,   118,     0,   119,   120,
     121,     0,     0,   117,   118,     0,   119,   120,   121,   122,
     123,    76,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,     0,     0,   117,   118,     0,   119,   120,
     121,   122,   123,   172,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   141,     0,     0,     0,     0,
     142,     0,     0,   143,     0,   233,    70,   144,   145,     0,
     137,     9,    10,     0,    11,    12,    13,    14,    15,     0,
       0,     0,     0,     0,     0,   146,     9,    10,     0,    11,
      12,    13,    14,    15,   169,     0,     0,     0,     0,   142,
      57,     0,   143,   240,     0,    70,   144,   145,   142,     0,
       0,   143,     0,     0,    70,   144,   145,     0,     0,    56,
     101,     0,     9,    10,    76,    11,    12,    13,    14,    15,
       0,    95,   158,    76,     9,    10,    57,    11,    12,    13,
      14,    15,     0,    56,     0,     0,     9,    10,    57,    11,
      12,    13,    14,    15,     0,    95,     0,     0,     9,    10,
      57,    11,    12,    13,    14,    15,     0,     0,     0,     0,
       9,    10,    57,    11,    12,    13,    14,    15,     0,     0,
       0,     0,   117,   118,    57,   119,   120,   121,   122,   123,
       0,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,     0,   117,   118,    45,   119,   120,   121,   122,
     123,     0,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   117,   118,     0,   119,   120,   121,   122,   123,
       0,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     117,   118,     0,   119,   120,   121,   122,   123,     0,   124,
     125,   126,  -145,  -145,  -145,  -145,  -145,  -145,   117,   118,
       0,   119,   120,   121,   122,     0,     0,     0,   125,   126,
     117,   118,     0,   119,   120,   121,     0,     0,     0,     0,
     125,   126
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-197))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-145))

static const yytype_int16 yycheck[] =
{
      25,    27,    27,     0,    93,    93,     8,   203,     4,     5,
      36,    65,    93,    15,    29,    44,    93,    28,    43,    45,
       2,    65,    47,   112,   112,    27,    37,    36,    53,     4,
       5,   112,     4,    58,    36,   112,    65,    66,    67,    65,
      69,   237,    65,    68,    46,    41,    42,    49,    50,    30,
      65,    53,    14,    78,    36,    28,    31,    32,    83,    31,
      65,    63,    64,    65,    37,    94,     7,     8,    49,    10,
      11,    12,    13,    14,    56,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,   111,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,    30,   136,    90,    30,
      34,    93,   200,    34,   143,   203,   141,    30,   143,   200,
     145,   103,   203,   200,    28,    30,   203,   152,   153,    34,
     219,   219,    30,    37,    30,    30,     8,    35,   219,     4,
       5,    29,   219,    15,    40,   234,   234,    43,    65,   237,
      46,    47,    48,   234,    28,    27,   237,   234,    30,    28,
     237,    30,    34,    37,    36,    28,    31,    30,     8,    65,
     199,   196,    37,     4,    46,    15,   201,    49,    50,   204,
      28,    53,    30,    30,   209,   214,    36,    27,    35,    28,
      36,    63,    64,    65,   219,    65,    36,    37,   227,    28,
       8,    30,    36,    30,    29,    65,    46,    15,    35,    49,
      50,    36,    65,    53,    65,   240,    30,   242,    29,    27,
      65,    35,    30,    63,    64,    65,    51,    52,    36,    54,
      55,    56,    57,    58,    27,    10,    11,    12,    46,    29,
      65,    49,    50,    36,    65,    53,    41,   144,    68,    42,
      99,    44,    45,   216,    90,    63,    64,    65,    51,    52,
     228,    54,    55,    56,    57,    58,    30,    95,    51,    52,
      34,    54,    65,    56,    57,    58,    40,   160,    56,    43,
     103,    12,    46,    47,    48,     7,     8,    -1,    10,    11,
      12,    -1,    -1,     7,     8,    -1,    10,    11,    12,    13,
      14,    65,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,     7,     8,    -1,    10,    11,
      12,    13,    14,    37,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    35,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    43,    -1,    37,    46,    47,    48,    -1,
      35,    51,    52,    -1,    54,    55,    56,    57,    58,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    51,    52,    -1,    54,
      55,    56,    57,    58,    35,    -1,    -1,    -1,    -1,    40,
      65,    -1,    43,    35,    -1,    46,    47,    48,    40,    -1,
      -1,    43,    -1,    -1,    46,    47,    48,    -1,    -1,    36,
      37,    -1,    51,    52,    65,    54,    55,    56,    57,    58,
      -1,    36,    37,    65,    51,    52,    65,    54,    55,    56,
      57,    58,    -1,    36,    -1,    -1,    51,    52,    65,    54,
      55,    56,    57,    58,    -1,    36,    -1,    -1,    51,    52,
      65,    54,    55,    56,    57,    58,    -1,    -1,    -1,    -1,
      51,    52,    65,    54,    55,    56,    57,    58,    -1,    -1,
      -1,    -1,     7,     8,    65,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    -1,     7,     8,    30,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,     7,     8,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       7,     8,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,     7,     8,
      -1,    10,    11,    12,    13,    -1,    -1,    -1,    17,    18,
       7,     8,    -1,    10,    11,    12,    -1,    -1,    -1,    -1,
      17,    18
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    69,    70,     0,    27,    36,    42,    44,    45,    51,
      52,    54,    55,    56,    57,    58,    65,    71,    72,    77,
      78,    79,    80,    99,   102,   103,   109,   115,   118,   123,
     124,   125,    65,    77,    77,    65,    65,    54,   124,    36,
       4,     5,    31,    32,    14,    30,    34,   104,   121,    34,
     110,   121,    65,   100,   101,    37,    36,    65,   119,   120,
     123,    65,    81,    77,    77,     8,    15,    27,    34,    36,
      46,    49,    50,    53,    63,    64,    65,    73,    74,    75,
      91,    92,    94,    95,    96,    97,    98,   121,   122,    95,
     105,   121,    30,   111,    29,    36,   116,   117,   123,    28,
     121,    37,   119,    28,   121,    65,    28,    37,    95,    95,
      95,    74,    76,   121,    95,   121,    36,     7,     8,    10,
      11,    12,    13,    14,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,   121,    29,    35,   106,   109,
     123,    35,    40,    43,    47,    48,    65,    82,    83,    86,
      87,    88,    91,    94,    98,   112,   123,    95,    37,   116,
      28,    65,    65,   101,    37,   120,    65,    35,   121,    35,
      86,    35,    37,    37,    93,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,    95,    95,    95,    95,    65,   107,   108,   121,    36,
      90,    95,   121,    90,    49,   121,   121,   121,    65,   113,
     114,    37,   117,    35,    28,    37,    28,   121,    95,    34,
      84,    86,    89,   121,   121,    84,   121,    29,    28,   121,
      95,    65,   108,    37,    85,    86,   121,    41,    95,   114,
      35,    86,    35,    84,   121,   121
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
#line 348 "E--_parser.y++"
    {   prt("Spec");    }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 356 "E--_parser.y++"
    {   stm.leaveScope();   stm.insert((yyvsp[(1) - (1)].eventEntry)); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 357 "E--_parser.y++"
    {   stm.leaveScope();   stm.insert((yyvsp[(1) - (1)].functionEntry)); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 358 "E--_parser.y++"
    {   stm.leaveScope();                   }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 362 "E--_parser.y++"
    {   (yyval.ruleNode) = allocRuleNode((yyvsp[(1) - (3)].patVal), (yyvsp[(3) - (3)].stmtVal)); prt("Rule");   }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 365 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (2)].stmt_plusVal);    }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 366 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].stmt_plusVal);    }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 367 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode((yyvsp[(1) - (2)].exprVal)); prt("AssignStmt");  }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 368 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode(NULL);   }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 371 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocScopeNode((yyvsp[(2) - (3)].stmt_plusVal));    prt("CompoundStmt");   }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 372 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocScopeNode((yyvsp[(2) - (4)].stmt_plusVal));    prt("CompoundStmt");   }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 373 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = (yyvsp[(1) - (1)].stmt_plusVal);    }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 376 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(curStmtList1); curStmtList1 = NULL;  prt("CompoundStmt");  }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 377 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(new list<StmtNode*>);                prt("CompoundStmt");  }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 380 "E--_parser.y++"
    {   if (!curStmtList1) curStmtList1 = new list<StmtNode*>; curStmtList1->push_back((yyvsp[(2) - (2)].stmtVal));}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 384 "E--_parser.y++"
    {   (yyval.patVal) = allocPrimPatNode((yyvsp[(1) - (1)].eventEntry), curVarList, NULL);    curVarList = NULL;    prt("PrimitivePat"); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 385 "E--_parser.y++"
    {   (yyval.patVal) = allocPrimPatNode((yyvsp[(1) - (1)].eventEntry), curVarList, NULL);    curVarList = NULL;    prt("PrimitivePat"); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 386 "E--_parser.y++"
    {   (yyval.patVal) = allocPrimPatNode((yyvsp[(1) - (3)].eventEntry), curVarList, (yyvsp[(3) - (3)].exprVal));      curVarList = NULL;    prt("PrimitivePatWithCond");  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 387 "E--_parser.y++"
    {   (yyval.patVal) = (yyvsp[(1) - (1)].patVal);                        }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 388 "E--_parser.y++"
    {   (yyval.patVal) = (yyvsp[(2) - (3)].patVal);    prt("(EventPat)");  }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 391 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::OR  , (yyvsp[(1) - (3)].patVal), (yyvsp[(3) - (3)].patVal));   prt("EventPat \\/ EventPat");  }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 392 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::SEQ , (yyvsp[(1) - (3)].patVal), (yyvsp[(3) - (3)].patVal));   prt("EventPat : EventPat");    }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 393 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::STAR, (yyvsp[(1) - (2)].patVal), NULL); prt("EventPat **");            }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 394 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::NEG , (yyvsp[(2) - (2)].patVal), NULL); prt("!EventPat");              }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 397 "E--_parser.y++"
    {   allocRuleBlockNode();   (yyval.eventEntry) = any;   prt("anyEvent");    }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 400 "E--_parser.y++"
    {   (yyval.eventEntry) = getEventEntry((yyvsp[(1) - (4)].cVal));             prt("NormalEvent"); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 404 "E--_parser.y++"
    {   curVarList->push_back(allocVarEntry((yyvsp[(3) - (3)].cVal), VariableEntry::UNDEFINED, new Type(Type::UNKNOWN), NULL)); prt("EventFormalParam");}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 405 "E--_parser.y++"
    {   allocRuleBlockNode(); curVarList = new vector<VariableEntry*>(); 
                                                                                curVarList->push_back(allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::UNDEFINED, new Type(Type::UNKNOWN), NULL)); prt("EventFormalParam");}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 410 "E--_parser.y++"
    {   (yyval.whileVal) = allocWhileNode((yyvsp[(2) - (3)].exprVal), (yyvsp[(3) - (3)].stmtVal));  prt("WhileStatement"); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 413 "E--_parser.y++"
    {   (yyval.ifVal) = allocIfNode((yyvsp[(2) - (3)].exprVal), (yyvsp[(3) - (3)].stmtVal), NULL); prt("IfThenStmt");          }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 414 "E--_parser.y++"
    {   (yyval.ifVal) = allocIfNode((yyvsp[(2) - (5)].exprVal), (yyvsp[(3) - (5)].stmtVal), (yyvsp[(5) - (5)].stmtVal));   prt("IfThenElseStmt");      }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 417 "E--_parser.y++"
    {   (yyval.stmtVal) = allocCompStmtNode((yyvsp[(2) - (4)].stmtList)); prt("CompoundStmt");   }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 418 "E--_parser.y++"
    {   (yyval.stmtVal) = allocCompStmtNode((yyvsp[(2) - (3)].stmtList)); prt("CompoundStmt");   }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 419 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].stmt_plusVal);                                                    }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 420 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].stmtVal);                                                    }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 421 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode(NULL);                               }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 424 "E--_parser.y++"
    {   if((yyvsp[(2) - (2)].stmtVal)) (yyval.stmtList)->push_back((yyvsp[(2) - (2)].stmtVal));                               }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 425 "E--_parser.y++"
    {   (yyval.stmtList) = new list<StmtNode*>; if((yyvsp[(1) - (1)].stmtVal)) (yyval.stmtList)->push_back((yyvsp[(1) - (1)].stmtVal));     }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 428 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode((yyvsp[(1) - (2)].exprVal)); prt("FunctionInvocationStmt");  }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 429 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode((yyvsp[(1) - (2)].exprVal)); prt("AssignStmt");              }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 430 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].ifVal);                                                    }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 431 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].returnVal);                                                    }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 432 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].whileVal);						    }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 433 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].breakVal);                                                    }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 436 "E--_parser.y++"
    {   (yyval.returnVal) = allocReturnNode((yyvsp[(2) - (3)].exprVal));   prt("ReturnStmt");              }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 437 "E--_parser.y++"
    {   (yyval.returnVal) = allocReturnNode(NULL); prt("ReturnStmt");              }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 440 "E--_parser.y++"
    {	(yyval.breakVal) = allocBreakNode(NULL);   prt("BreakStmt");		    }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 441 "E--_parser.y++"
    {   tempValNode = new Value((yyvsp[(2) - (3)].uVal), Type::UINT); (yyval.breakVal) = allocBreakNode(allocValueNode(tempValNode));     prt("BreakStmt");    }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 444 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(new list<StmtNode*>); prt("CompoundStmt");  }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 445 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(new list<StmtNode*>); prt("CompoundStmt");  }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 448 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(2) - (3)].exprVal);    prt("(Expr)"); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 451 "E--_parser.y++"
    {   (yyval.exprVal) = allocInvNode((yyvsp[(1) - (4)].cVal), curExprList); curExprList = NULL; prt("FunctionInvocation"); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 452 "E--_parser.y++"
    {   (yyval.exprVal) = allocInvNode((yyvsp[(1) - (3)].cVal), NULL);                            prt("FunctionInvocation"); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 455 "E--_parser.y++"
    {   (yyval.cVal) = (yyvsp[(1) - (1)].cVal);                }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 456 "E--_parser.y++"
    {   (yyval.cVal) = (char*)"print";    }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 459 "E--_parser.y++"
    {   curExprList->push_back((yyvsp[(3) - (3)].exprVal));                                             }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 460 "E--_parser.y++"
    {   curExprList = new vector<ExprNode*>(); curExprList->push_back((yyvsp[(1) - (1)].exprVal));      }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 463 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::ASSIGN, (yyvsp[(1) - (3)].refexpVal), (yyvsp[(3) - (3)].exprVal)); prt("Assignment");    }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 466 "E--_parser.y++"
    {   (yyval.exprVal) = allocValueNode((yyvsp[(1) - (1)].valVal));    }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 467 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].refexpVal);                    }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 468 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 469 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 470 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 471 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 472 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(2) - (3)].exprVal);    prt("(Expr)");  }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 475 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::NOT   , (yyvsp[(2) - (2)].exprVal), NULL); prt("!Expr");         }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 476 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITNOT, (yyvsp[(2) - (2)].exprVal), NULL); prt("~Expr");         }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 477 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::UMINUS, (yyvsp[(2) - (2)].exprVal), NULL); prt("-Expr");         }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 480 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::OR    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr || Expr");    }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 481 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::AND   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr && Expr");    }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 482 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::EQ    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr == Expr");    }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 483 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::NE    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr != Expr");    }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 484 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::GT    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr > Expr");     }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 485 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::LT    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr < Expr");     }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 486 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::GE    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr >= Expr");    }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 487 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::LE    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr <= Expr");    }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 488 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITOR , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr | Expr");     }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 489 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITXOR, (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr ^ Expr");     }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 490 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITAND, (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr & Expr");     }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 491 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::SHL   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr << Expr");    }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 492 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::SHR   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr >> Expr");    }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 493 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::PLUS  , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr + Expr");     }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 494 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::MINUS , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr - Expr");     }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 495 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::MULT  , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr * Expr");     }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 496 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::DIV   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr / Expr");     }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 497 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::MOD   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr % Expr");     }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 500 "E--_parser.y++"
    {   (yyval.refexpVal) = allocRefExprNode((yyvsp[(1) - (1)].cVal)); prt("RefExpr"); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 510 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::GLOBAL_VAR, curType, NULL); prt("VariableDecl");   }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 511 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (3)].cVal), VariableEntry::GLOBAL_VAR, curType, (yyvsp[(3) - (3)].exprVal)); prt("VariableDeclWithInit");}
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 514 "E--_parser.y++"
    {   (yyval.classEntry) = (yyvsp[(1) - (2)].classEntry);                                                   }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 515 "E--_parser.y++"
    {   (yyval.classEntry) = (yyvsp[(1) - (3)].classEntry); (yyval.classEntry)->type()->size(curClassSize); curClassSize = 0; }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 518 "E--_parser.y++"
    {   (yyval.classEntry) = allocClassEntry((yyvsp[(2) - (2)].cVal)); prt("ClassDecl");  }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 525 "E--_parser.y++"
    {   stm.leaveScope();   stm.insert((yyvsp[(2) - (2)].functionEntry));         }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 536 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::CLASS_VAR, curType, NULL); prt("ClassVarDecl");  
                  ;                                                             curClassSize += (yyval.variableEntry)->type()->size();         }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 541 "E--_parser.y++"
    {   (yyval.functionEntry) = (yyvsp[(1) - (2)].functionEntry);                    }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 542 "E--_parser.y++"
    {   (yyvsp[(1) - (2)].functionEntry)->body((yyvsp[(2) - (2)].stmt_plusVal));   (yyval.functionEntry) = (yyvsp[(1) - (2)].functionEntry);    }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 545 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(curStmtList3);        curStmtList3 = NULL;     prt("FunctionBody");  }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 546 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(curStmtList3);        curStmtList3 = NULL;     prt("FunctionBody");  }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 550 "E--_parser.y++"
    {   if (!curStmtList3) curStmtList3 = new list<StmtNode*>; curStmtList3->push_back((yyvsp[(2) - (2)].stmtVal)); }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 561 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::LOCAL_VAR, curType, NULL); prt("VariableDecl");    }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 562 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (3)].cVal), VariableEntry::LOCAL_VAR, curType, (yyvsp[(3) - (3)].exprVal)); prt("VariableDeclWithInit"); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 565 "E--_parser.y++"
    {   (yyval.functionEntry) = allocFuncEntry((yyvsp[(2) - (3)].cVal), (yyvsp[(1) - (3)].typeVal), curTypeList);   curTypeList = NULL;     prt("FunctionDecl");  }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 568 "E--_parser.y++"
    {   prt("FormalParamList");     }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 569 "E--_parser.y++"
    {   curTypeList->push_back((yyvsp[(3) - (3)].typeVal)); }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 570 "E--_parser.y++"
    {   curTypeList = new vector<Type*>(); curTypeList->push_back((yyvsp[(1) - (1)].typeVal));  }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 571 "E--_parser.y++"
    {   curFuncEnt = allocFuncEntry();  }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 573 "E--_parser.y++"
    {   if (!curFuncEnt) curFuncEnt = allocFuncEntry();  
                                                                                (yyval.typeVal) = (yyvsp[(1) - (2)].typeVal); allocVarEntry((yyvsp[(2) - (2)].cVal), VariableEntry::PARAM_VAR, (yyvsp[(1) - (2)].typeVal), NULL);  prt("FormalParam"); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 577 "E--_parser.y++"
    {   (yyval.eventEntry) = allocEventEntry((yyvsp[(2) - (4)].cVal), curTypeList);  curTypeList = NULL;     prt("EventDecl");  }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 580 "E--_parser.y++"
    {   prt("FormalParamList");         }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 581 "E--_parser.y++"
    {   curTypeList->push_back((yyvsp[(3) - (3)].typeVal));     }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 582 "E--_parser.y++"
    {   curTypeList = new vector<Type*>(); curTypeList->push_back((yyvsp[(1) - (1)].typeVal));}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 583 "E--_parser.y++"
    {   curEventEnt = allocEventEntry();}
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 586 "E--_parser.y++"
    {   if (!curEventEnt) curEventEnt = allocEventEntry();  
                                                                                (yyval.typeVal) = (yyvsp[(1) - (2)].typeVal); allocVarEntry((yyvsp[(2) - (2)].cVal), VariableEntry::PARAM_VAR, (yyvsp[(1) - (2)].typeVal), NULL); prt("FormalParam");}
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 595 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].uVal), Type::UINT); prt("Literal");     }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 596 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].dVal));             prt("Literal");     }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 597 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].cVal));             prt("Literal");     }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 598 "E--_parser.y++"
    {   (yyval.valVal) = new Value(true);           prt("Literal");     }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 599 "E--_parser.y++"
    {   (yyval.valVal) = new Value(false);          prt("Literal");     }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 602 "E--_parser.y++"
    {   (yyval.typeVal) = curType = (yyvsp[(1) - (1)].typeVal);                  prt("Type");    }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 603 "E--_parser.y++"
    {   (yyval.typeVal) = curType = (yyvsp[(1) - (1)].typeVal);                  prt("Type");    }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 604 "E--_parser.y++"
    {   (yyval.typeVal) = curType = getUserType((yyvsp[(1) - (1)].cVal));     prt("Type");    }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 605 "E--_parser.y++"
    {   (yyval.typeVal) = curType = new Type(Type::VOID);prt("Type");
                                                                                errMsg("Invalid use of keyword \"unsigned\"");      }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 609 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::VOID);    }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 610 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::BOOL);    }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 611 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::BYTE);    }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 612 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::STRING);  }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 613 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::DOUBLE);  }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 616 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::INT);     }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 617 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::UINT);    }
    break;



/* Line 1806 of yacc.c  */
#line 2916 "E--_parser.C"
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
#line 620 "E--_parser.y++"


