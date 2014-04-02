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
int offSet;
extern const OpNode::OpInfo opInfo[];

#define prt(x) //cout << x << endl;

static Type                   *curType      = NULL;
static vector<ExprNode*>      *curExprList  = NULL;
static list<StmtNode*>        *curStmtList1 = NULL;
static list<StmtNode*>        *curStmtList2 = NULL;
static list<StmtNode*>        *curStmtList3 = NULL;
static vector<Type*>          *curTypeList  = NULL; 
static vector<VariableEntry*> *curVarList   = NULL; 
static FunctionEntry          *curFuncEnt   = NULL;
static EventEntry             *curEventEnt  = NULL;
static RuleBlockEntry         *curRBlkNode  = NULL;

static EventEntry* getEventEntry(char *eventName) {
    SymTabEntry *sym = stm.lookUp(string(eventName));
    if (sym && sym->type()->tag() == Type::EVENT) {
        int caller_param = curVarList ? curVarList->size() : 0;
        int callee_param = sym->type()->argTypes() ? sym->type()->argTypes()->size() : 0;

        if (caller_param == callee_param) {
            return (EventEntry*) sym;
        } else {
            errMsg((string)eventName + (string)": mismatch in the number of arguments");
            return NULL;
        }
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
        retVarEnt = new VariableEntry(name, v, type, init, yylineno, yycolumnno, string(yyfilename));
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
        retClassEnt = new ClassEntry(string(name), yylineno, yycolumnno, string(yyfilename));
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
        retEventEnt = new EventEntry(string("dummy_entry"), yylineno, yycolumnno, string(yyfilename));
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
        retFuncEnt = new FunctionEntry(string("dummy_func"), NULL, yylineno, yycolumnno, string(yyfilename));
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
    return new RefExprNode(string(str), sym, yylineno, yycolumnno, string(yyfilename));
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
    return new InvocationNode(sym, param_l, yylineno, yycolumnno, string(yyfilename));
}

static ValueNode* allocValueNode(Value* val) {
    return new ValueNode(val, yylineno, yycolumnno, string(yyfilename));
}

static OpNode* allocOpNode(OpNode::OpCode op, ExprNode *l, ExprNode *r) {
    assert(l);
    return new OpNode(op, l, r, yylineno, yycolumnno, string(yyfilename));
}

static ExprStmtNode* allocExprStmtNode(ExprNode* e) {
    return new ExprStmtNode(e, yylineno, yycolumnno, string(yyfilename));
}

static ReturnStmtNode* allocReturnNode(ExprNode* e) {
    if (IsCurRuleBlockEntry())
        errMsg((string)"Return statement is valid only within a function");
    return new ReturnStmtNode(e, (FunctionEntry*)stm.currentScope(SymTabEntry::Kind::FUNCTION_KIND));
}

static IfNode* allocIfNode(ExprNode* cond, StmtNode* thenStmt, StmtNode* elseStmt) {
    return new IfNode(cond, thenStmt, elseStmt, yylineno, yycolumnno, string(yyfilename));
}

static CompoundStmtNode* allocCompStmtNode(list<StmtNode*> *Slist) {
    if (!Slist) Slist = new list<StmtNode*>;
    return new CompoundStmtNode(Slist, yylineno, yycolumnno, string(yyfilename));
}

static CompoundStmtNode* allocScopeNode(CompoundStmtNode* node) {
    CompoundStmtNode *retCompNode = allocCompStmtNode(new list<StmtNode*>);
    retCompNode->add(node);
    return retCompNode;
}

static PrimitivePatNode* allocPrimPatNode(EventEntry* ee, vector<VariableEntry*>* params, ExprNode* c) {
    if (ee)
        return new PrimitivePatNode(ee, params, c, yylineno, yycolumnno, string(yyfilename));
    return NULL;
}

static int hasInvalidOps = 0;
static PatNode* allocPatNode(BasePatNode::PatNodeKind pk, BasePatNode *p1, BasePatNode *p2) {
    if (p1 && 
        ((pk == BasePatNode::PatNodeKind::NEG || pk == BasePatNode::PatNodeKind::STAR) ||
        ((pk == BasePatNode::PatNodeKind::OR  || pk == BasePatNode::PatNodeKind::SEQ) && p2)))
    {
        // To handle valid combination of operations
        if (pk == BasePatNode::PatNodeKind::NEG) {
            if (hasInvalidOps > 0) {
                hasInvalidOps = -2;
                errMsg((string)"Only simple patterns without `.', `*', and `!' operatorscan be negated");
            } else if (hasInvalidOps == 0) {
                hasInvalidOps = -1;
            } 
        } else if (pk == BasePatNode::PatNodeKind::STAR) {
            if (hasInvalidOps == -1) {
                hasInvalidOps = -2;
                errMsg((string)"Only simple patterns without `.', `*', and `!' operatorscan be negated");
            } else if (hasInvalidOps != -2) {
                hasInvalidOps++;
            }
        } else if (pk == BasePatNode::PatNodeKind::SEQ) {
            hasInvalidOps = 0;
        }
        return new PatNode(pk, p1, p2, yylineno, yycolumnno, string(yyfilename));
    }
    return NULL;
}

static RuleNode* allocRuleNode(BasePatNode* pat, StmtNode* reaction) {
    RuleNode *retRuleNode = NULL;

    if (pat) {
        retRuleNode = new RuleNode(curRBlkNode, pat, reaction, yylineno, yycolumnno, string(yyfilename));
        GlobalEntry *globalSymEnt = (GlobalEntry*) stm.currentScope(SymTabEntry::Kind::GLOBAL_KIND);
        globalSymEnt->addRule(retRuleNode);
    }
    stm.leaveScope();
    curRBlkNode = NULL;

    return retRuleNode;
}

static RuleBlockEntry* allocRuleBlockNode() {
    if (!curRBlkNode) {
        hasInvalidOps = 0;
        curRBlkNode = new RuleBlockEntry(yylineno, yycolumnno, string(yyfilename));
        stm.enterScope(curRBlkNode);
    }
    return curRBlkNode;
}



/* Line 268 of yacc.c  */
#line 326 "E--_parser.C"

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
     TOK_LBRACE = 288,
     TOK_RBRACE = 289,
     TOK_LPAREN = 290,
     TOK_RPAREN = 291,
     TOK_IF = 292,
     TOK_ELSE = 293,
     TOK_CLASS = 294,
     TOK_RETURN = 295,
     TOK_EVENT = 296,
     TOK_ANY = 297,
     TOK_PRINT = 298,
     TOK_UINTNUM = 299,
     TOK_DOUBLENUM = 300,
     TOK_VOID = 301,
     TOK_STRING = 302,
     TOK_STRCONST = 303,
     TOK_INT = 304,
     TOK_UNSIGNED = 305,
     TOK_BOOL = 306,
     TOK_BYTE = 307,
     TOK_DOUBLE = 308,
     TOK_SHORT = 309,
     TOK_TRUE = 310,
     TOK_FALSE = 311,
     TOK_IDENT = 312,
     TOK_DOUBLE_COLON = 313,
     TOK_LBRACK = 314,
     TOK_DOT = 315
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 255 "E--_parser.y++"

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
    CompoundStmtNode*   stmt_plusVal;
    RuleNode*           ruleNode;

    BasePatNode*        patVal;
    PrimitivePatNode*   primPatVal;
    vector<RuleNode*>*  transList;

    const OpNode::OpInfo*   funIval;



/* Line 293 of yacc.c  */
#line 457 "E--_parser.C"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 469 "E--_parser.C"

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
#define YYLAST   488

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  133
/* YYNRULES -- Number of states.  */
#define YYNSTATES  217

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   315

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
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    12,    14,    16,    18,
      20,    24,    27,    29,    32,    34,    38,    43,    45,    49,
      53,    56,    57,    59,    61,    65,    67,    71,    75,    79,
      82,    85,    87,    92,    96,    98,    99,   103,   109,   113,
     115,   117,   119,   122,   123,   126,   129,   131,   135,   138,
     142,   146,   151,   155,   157,   159,   163,   165,   169,   171,
     173,   175,   177,   179,   181,   185,   188,   191,   194,   198,
     202,   206,   210,   214,   218,   222,   226,   230,   234,   238,
     242,   246,   250,   254,   258,   262,   266,   268,   272,   276,
     278,   280,   284,   288,   291,   294,   299,   302,   305,   306,
     310,   314,   316,   318,   322,   326,   330,   334,   336,   339,
     342,   347,   351,   355,   357,   360,   363,   366,   368,   370,
     372,   374,   376,   378,   380,   382,   384,   387,   389,   391,
     393,   395,   397,   399
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      62,     0,    -1,    63,    -1,    63,    64,    -1,    63,    65,
      -1,    -1,   102,    -1,    93,    -1,    92,    -1,    89,    -1,
      70,    32,    66,    -1,    67,   105,    -1,    67,    -1,    85,
     105,    -1,   105,    -1,    33,    67,    34,    -1,    33,    67,
     105,    34,    -1,    68,    -1,    33,    69,    34,    -1,    33,
     105,    34,    -1,    69,    78,    -1,    -1,    73,    -1,    72,
      -1,    73,    14,    85,    -1,    71,    -1,    35,    70,    36,
      -1,    70,     5,    70,    -1,    70,    31,    70,    -1,    70,
       4,    -1,    27,    70,    -1,    42,    -1,    57,    35,    74,
      36,    -1,    74,    28,    57,    -1,    57,    -1,    -1,    37,
      80,    76,    -1,    37,    80,    76,    38,    76,    -1,    33,
      77,    34,    -1,    79,    -1,    78,    -1,   105,    -1,    77,
      78,    -1,    -1,    81,   105,    -1,    84,   105,    -1,    75,
      -1,    40,    85,   105,    -1,    40,   105,    -1,    33,   105,
      34,    -1,    35,    85,    36,    -1,    82,    35,    83,    36,
      -1,    82,    35,    36,    -1,    57,    -1,    43,    -1,    83,
      28,    85,    -1,    85,    -1,    88,    29,    85,    -1,   106,
      -1,    88,    -1,    81,    -1,    84,    -1,    86,    -1,    87,
      -1,    35,    85,    36,    -1,    27,    85,    -1,    15,    85,
      -1,     8,    85,    -1,    85,    26,    85,    -1,    85,    25,
      85,    -1,    85,    23,    85,    -1,    85,    24,    85,    -1,
      85,    19,    85,    -1,    85,    20,    85,    -1,    85,    21,
      85,    -1,    85,    22,    85,    -1,    85,    14,    85,    -1,
      85,    16,    85,    -1,    85,    13,    85,    -1,    85,    17,
      85,    -1,    85,    18,    85,    -1,    85,     7,    85,    -1,
      85,     8,    85,    -1,    85,    10,    85,    -1,    85,    11,
      85,    -1,    85,    12,    85,    -1,    57,    -1,   107,    90,
     105,    -1,    90,    28,    91,    -1,    91,    -1,    57,    -1,
      57,    29,    85,    -1,    39,    57,   105,    -1,    99,   105,
      -1,    99,    94,    -1,    33,    95,    34,   105,    -1,    95,
      96,    -1,    95,    78,    -1,    -1,   107,    97,   105,    -1,
      97,    28,    98,    -1,    98,    -1,    57,    -1,    57,    29,
      85,    -1,   107,    57,   100,    -1,    35,   100,    36,    -1,
     100,    28,   101,    -1,   101,    -1,    35,    36,    -1,   107,
      57,    -1,    41,    57,   103,   105,    -1,    35,   103,    36,
      -1,   103,    28,   104,    -1,   104,    -1,    35,    36,    -1,
     107,    57,    -1,   105,    30,    -1,    30,    -1,    44,    -1,
      45,    -1,    48,    -1,    55,    -1,    56,    -1,   108,    -1,
     109,    -1,    57,    -1,    50,   108,    -1,    46,    -1,    51,
      -1,    52,    -1,    47,    -1,    53,    -1,    49,    -1,    50,
      49,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   361,   361,   364,   365,   366,   369,   370,   371,   372,
     375,   378,   379,   380,   381,   384,   385,   386,   389,   390,
     393,   394,   397,   398,   399,   400,   401,   404,   405,   406,
     407,   410,   413,   417,   418,   420,   423,   424,   427,   428,
     429,   430,   433,   434,   437,   438,   439,   440,   441,   444,
     447,   450,   451,   454,   455,   458,   459,   462,   465,   466,
     467,   468,   469,   470,   471,   474,   475,   476,   479,   480,
     481,   482,   483,   484,   485,   486,   487,   488,   489,   490,
     491,   492,   493,   494,   495,   496,   499,   502,   505,   506,
     509,   510,   513,   516,   517,   520,   523,   524,   525,   528,
     531,   532,   535,   536,   539,   542,   543,   544,   545,   547,
     551,   554,   555,   556,   557,   560,   564,   565,   569,   570,
     571,   572,   573,   576,   577,   578,   579,   583,   584,   585,
     586,   587,   590,   591
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
  "TOK_LBRACE", "TOK_RBRACE", "TOK_LPAREN", "TOK_RPAREN", "TOK_IF",
  "TOK_ELSE", "TOK_CLASS", "TOK_RETURN", "TOK_EVENT", "TOK_ANY",
  "TOK_PRINT", "TOK_UINTNUM", "TOK_DOUBLENUM", "TOK_VOID", "TOK_STRING",
  "TOK_STRCONST", "TOK_INT", "TOK_UNSIGNED", "TOK_BOOL", "TOK_BYTE",
  "TOK_DOUBLE", "TOK_SHORT", "TOK_TRUE", "TOK_FALSE", "TOK_IDENT",
  "TOK_DOUBLE_COLON", "TOK_LBRACK", "TOK_DOT", "$accept",
  "s_specification", "specification", "declaration", "rule", "rule_stmts",
  "compound_stmt2", "compound_stmt1", "stmt_plus1", "primitive_all",
  "binary_event_oprs", "any_event", "normal_event", "event_aparam_l",
  "if_then_else_stmt", "basic_block", "stmt_plus2", "simple_stmt",
  "empty_block", "condition", "func_invocation", "func_name",
  "caller_param_l", "assignment", "expression", "unary_operation",
  "binary_operation", "ref_expr", "global_decl", "global_l", "global_var",
  "class_decl", "function_proto", "function_defn", "function_body",
  "local_decl", "local_l", "local_var", "function_decl", "func_param_l",
  "func_param", "event_decl", "event_param_l", "event_param", "semi_colon",
  "literal", "type", "baseType", "intType", 0
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
     315
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    61,    62,    63,    63,    63,    64,    64,    64,    64,
      65,    66,    66,    66,    66,    67,    67,    67,    68,    68,
      69,    69,    70,    70,    70,    70,    70,    71,    71,    71,
      71,    72,    73,    74,    74,    74,    75,    75,    76,    76,
      76,    76,    77,    77,    78,    78,    78,    78,    78,    79,
      80,    81,    81,    82,    82,    83,    83,    84,    85,    85,
      85,    85,    85,    85,    85,    86,    86,    86,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    88,    89,    90,    90,
      91,    91,    92,    93,    93,    94,    95,    95,    95,    96,
      97,    97,    98,    98,    99,   100,   100,   100,   100,   101,
     102,   103,   103,   103,   103,   104,   105,   105,   106,   106,
     106,   106,   106,   107,   107,   107,   107,   108,   108,   108,
     108,   108,   109,   109
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     0,     1,     1,     1,     1,
       3,     2,     1,     2,     1,     3,     4,     1,     3,     3,
       2,     0,     1,     1,     3,     1,     3,     3,     3,     2,
       2,     1,     4,     3,     1,     0,     3,     5,     3,     1,
       1,     1,     2,     0,     2,     2,     1,     3,     2,     3,
       3,     4,     3,     1,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     3,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     1,
       1,     3,     3,     2,     2,     4,     2,     2,     0,     3,
       3,     1,     1,     3,     3,     3,     3,     1,     2,     2,
       4,     3,     3,     1,     2,     2,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     2,     1,     0,     0,     0,     0,    31,   127,
     130,   132,     0,   128,   129,   131,   125,     3,     4,     0,
      25,    23,    22,     9,     8,     7,     0,     6,     0,   123,
     124,     0,    30,     0,     0,     0,   133,   126,    35,    29,
       0,     0,     0,     0,   117,    98,    94,    93,    90,     0,
      89,    26,    92,     0,   125,     0,   113,     0,    34,     0,
      27,    28,     0,     0,     0,    21,     0,    54,   118,   119,
     120,   121,   122,    86,    10,    12,    17,    60,     0,    61,
       0,    62,    63,    59,    14,    58,    24,     0,   116,     0,
       0,   104,   107,     0,     0,    87,   114,     0,     0,   110,
     115,     0,    32,    67,    66,    65,     0,     0,     0,     0,
      11,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      13,     0,     0,     0,     0,    53,    46,    97,     0,     0,
       0,    96,     0,    91,   108,     0,     0,   109,    90,    88,
     111,   112,    33,    15,     0,    18,    20,    19,    64,    52,
       0,    56,    81,    82,    83,    84,    85,    78,    76,    77,
      79,    80,    72,    73,    74,    75,    70,    71,    69,    68,
      57,    95,     0,     0,     0,    48,    44,    45,   102,     0,
     101,   105,   106,    16,     0,    51,     0,    43,    36,    40,
      39,    41,    47,     0,     0,    99,    55,    50,     0,     0,
       0,   103,   100,    38,    42,    49,    37
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    17,    18,    74,    75,    76,   107,    19,
      20,    21,    22,    59,   136,   198,   208,   199,   200,   183,
      77,    78,   160,    79,    80,    81,    82,    83,    23,    49,
      50,    24,    25,    46,    87,   141,   189,   190,    26,    91,
      92,    27,    55,    56,   201,    85,    57,    29,    30
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -87
static const yytype_int16 yypact[] =
{
     -87,     2,   192,   -87,    -5,    -5,   -32,    -9,   -87,   -87,
     -87,   -87,   249,   -87,   -87,   -87,    22,   -87,   -87,    38,
     -87,   -87,    48,   -87,   -87,   -87,   -21,   -87,    10,   -87,
     -87,    22,   -87,    15,    42,   338,   -87,   -87,    16,   -87,
      -5,    -5,    20,   180,   -87,   -87,   -87,    49,   259,   127,
     -87,   -87,    49,   310,   -87,   132,   -87,    58,   -87,    30,
      40,   101,   180,   180,   180,    -6,   180,   -87,   -87,   -87,
     -87,   -87,   -87,    81,   -87,    42,   -87,   -87,    84,   -87,
     307,   -87,   -87,    95,    49,   -87,   386,   301,   -87,   180,
     329,   105,   -87,    79,    80,    49,   -87,   102,   422,    49,
     -87,    91,   -87,   -87,   470,   425,   -27,    83,   -20,   240,
      49,   134,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
      49,   180,    42,   116,   157,   -12,   -87,   -87,    42,    42,
      95,   -87,   117,   386,   -87,   107,   422,   -87,   146,   -87,
     -87,   -87,   -87,   -87,    26,   -87,   -87,   -87,   -87,   -87,
     118,   386,    99,    99,   -87,   -87,   -87,   470,   186,   186,
     210,   210,   443,   443,   443,   443,   443,   443,   425,   406,
     386,    49,   180,    88,   307,    49,    49,    49,   147,   138,
     -87,   -87,   -87,   -87,   180,   -87,   261,    42,   142,   -87,
     -87,    49,    49,   180,   117,    49,   386,   -87,   110,    44,
      88,   386,   -87,   -87,   -87,   -87,   -87
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -87,   -87,   -87,   -87,   -87,   -87,   120,   -87,   -87,     0,
     -87,   -87,   -87,   -87,   -87,   -54,   -87,   -86,   -87,   -87,
     -81,   -87,   -87,   -76,   -30,   -87,   -87,   -69,   -87,   -87,
      87,   -87,   -87,   -87,   -87,   -87,   -87,   -18,   -87,    93,
      60,   -87,   155,   111,   -26,   -87,    13,   198,   -87
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -126
static const yytype_int16 yytable[] =
{
      47,   137,     3,    44,    32,    33,   138,   153,    52,    44,
      88,   139,    45,    86,   157,    28,    84,   -86,   140,    39,
      40,   156,     4,    95,    44,    34,   138,    65,    62,    99,
       5,   139,   103,   104,   105,    63,   109,     8,   140,   108,
      60,    61,    39,    40,    39,  -125,    41,    64,    35,   110,
      44,    51,    31,    65,   130,    66,    88,    38,   101,   143,
     193,    93,    43,    67,    68,    69,   102,    48,    70,    41,
      42,    41,    44,    58,    88,    71,    72,    73,   215,    88,
     154,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     142,   180,   138,    93,   184,    39,   181,   139,   185,   114,
     115,   116,   186,   187,   140,   100,   -53,   155,    44,   111,
     133,   197,   214,   134,   131,   133,    67,   138,   134,   138,
      98,    67,   139,   146,   139,   146,   147,   148,   150,   140,
      73,   140,    62,   191,   213,    73,   194,   133,   152,    63,
     134,   182,   196,    67,   195,    94,   216,    44,   202,    93,
      98,    64,    44,   205,   206,    62,   204,    73,    44,    66,
     159,   209,    63,   211,   188,    89,   203,    67,    68,    69,
     210,   149,    70,   145,    64,   106,   212,    44,    62,    71,
      72,    73,    66,   112,   113,    63,   114,   115,   116,   117,
      67,    68,    69,   120,   121,    70,   192,    64,    97,   151,
      37,     0,    71,    72,    73,    66,     0,   112,   113,     4,
     114,   115,   116,    67,    68,    69,     0,     5,    70,     0,
       0,     6,     0,     7,     8,    71,    72,    73,     9,    10,
       0,    11,    12,    13,    14,    15,     0,   112,   113,    16,
     114,   115,   116,   117,   118,     0,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,     0,   112,   113,
       0,   114,   115,   116,   117,   118,   158,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,    89,     0,
       0,     0,     0,     0,    90,     9,    10,   207,    36,     0,
      13,    14,    15,     0,     0,     9,    10,     0,    11,    12,
      13,    14,    15,     0,   112,   113,    54,   114,   115,   116,
     117,   118,     0,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,     0,   132,     0,    44,   133,     0,
       0,   134,     0,     0,    67,    53,    96,     9,    10,     0,
      11,    12,    13,    14,    15,     0,     9,    10,   135,    11,
      12,    13,    14,    15,    90,   144,     0,    54,     0,     0,
       0,     0,     0,    53,     0,     9,    10,     0,    11,    12,
      13,    14,    15,     0,     9,    10,    54,    11,    12,    13,
      14,    15,     0,   112,   113,    54,   114,   115,   116,   117,
     118,     0,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   112,   113,     0,   114,   115,   116,   117,
     118,     0,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   112,   113,     0,   114,   115,   116,   117,   118,
       0,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     112,   113,     0,   114,   115,   116,   117,   118,     0,   119,
     120,   121,  -126,  -126,  -126,  -126,  -126,  -126,     9,    10,
       0,    11,    12,    13,    14,    15,     0,   112,   113,    54,
     114,   115,   116,     0,     0,     0,     0,   120,   121
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-87))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-126))

static const yytype_int16 yycheck[] =
{
      26,    87,     0,    30,     4,     5,    87,    34,    34,    30,
      30,    87,    33,    43,    34,     2,    42,    29,    87,     4,
       5,   107,    27,    49,    30,    57,   107,    33,     8,    55,
      35,   107,    62,    63,    64,    15,    66,    42,   107,    65,
      40,    41,     4,     5,     4,    57,    31,    27,    57,    75,
      30,    36,    57,    33,    80,    35,    30,    35,    28,    89,
      34,    48,    14,    43,    44,    45,    36,    57,    48,    31,
      32,    31,    30,    57,    30,    55,    56,    57,    34,    30,
     106,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      87,   131,   183,    90,   134,     4,   132,   183,   134,    10,
      11,    12,   138,   139,   183,    57,    35,    34,    30,    35,
      37,    33,   208,    40,    29,    37,    43,   208,    40,   210,
      28,    43,   208,    28,   210,    28,    57,    57,    36,   208,
      57,   210,     8,    36,    34,    57,    28,    37,    57,    15,
      40,    35,   182,    43,    36,    28,   210,    30,   184,   146,
      28,    27,    30,   189,   194,     8,    28,    57,    30,    35,
      36,   197,    15,   203,    57,    29,    29,    43,    44,    45,
      38,    94,    48,    90,    27,    65,   204,    30,     8,    55,
      56,    57,    35,     7,     8,    15,    10,    11,    12,    13,
      43,    44,    45,    17,    18,    48,   146,    27,    53,    98,
      12,    -1,    55,    56,    57,    35,    -1,     7,     8,    27,
      10,    11,    12,    43,    44,    45,    -1,    35,    48,    -1,
      -1,    39,    -1,    41,    42,    55,    56,    57,    46,    47,
      -1,    49,    50,    51,    52,    53,    -1,     7,     8,    57,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,     7,     8,
      -1,    10,    11,    12,    13,    14,    36,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    29,    -1,
      -1,    -1,    -1,    -1,    35,    46,    47,    36,    49,    -1,
      51,    52,    53,    -1,    -1,    46,    47,    -1,    49,    50,
      51,    52,    53,    -1,     7,     8,    57,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    -1,    34,    -1,    30,    37,    -1,
      -1,    40,    -1,    -1,    43,    35,    36,    46,    47,    -1,
      49,    50,    51,    52,    53,    -1,    46,    47,    57,    49,
      50,    51,    52,    53,    35,    36,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    46,    47,    -1,    49,    50,
      51,    52,    53,    -1,    46,    47,    57,    49,    50,    51,
      52,    53,    -1,     7,     8,    57,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,     7,     8,    -1,    10,    11,    12,    13,
      14,    -1,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,     7,     8,    -1,    10,    11,    12,    13,    14,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
       7,     8,    -1,    10,    11,    12,    13,    14,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    46,    47,
      -1,    49,    50,    51,    52,    53,    -1,     7,     8,    57,
      10,    11,    12,    -1,    -1,    -1,    -1,    17,    18
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    62,    63,     0,    27,    35,    39,    41,    42,    46,
      47,    49,    50,    51,    52,    53,    57,    64,    65,    70,
      71,    72,    73,    89,    92,    93,    99,   102,   107,   108,
     109,    57,    70,    70,    57,    57,    49,   108,    35,     4,
       5,    31,    32,    14,    30,    33,    94,   105,    57,    90,
      91,    36,   105,    35,    57,   103,   104,   107,    57,    74,
      70,    70,     8,    15,    27,    33,    35,    43,    44,    45,
      48,    55,    56,    57,    66,    67,    68,    81,    82,    84,
      85,    86,    87,    88,   105,   106,    85,    95,    30,    29,
      35,   100,   101,   107,    28,   105,    36,   103,    28,   105,
      57,    28,    36,    85,    85,    85,    67,    69,   105,    85,
     105,    35,     7,     8,    10,    11,    12,    13,    14,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
     105,    29,    34,    37,    40,    57,    75,    78,    81,    84,
      88,    96,   107,    85,    36,   100,    28,    57,    57,    91,
      36,   104,    57,    34,   105,    34,    78,    34,    36,    36,
      83,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,   105,    35,    80,    85,   105,   105,   105,    57,    97,
      98,    36,   101,    34,    28,    36,    85,    33,    76,    78,
      79,   105,   105,    29,    28,   105,    85,    36,    77,   105,
      38,    85,    98,    34,    78,    34,    76
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
#line 361 "E--_parser.y++"
    {   prt("Spec");    }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 369 "E--_parser.y++"
    {   stm.leaveScope();   stm.insert((yyvsp[(1) - (1)].eventEntry)); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 370 "E--_parser.y++"
    {   stm.leaveScope();   stm.insert((yyvsp[(1) - (1)].functionEntry)); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 371 "E--_parser.y++"
    {   stm.leaveScope();                   }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 375 "E--_parser.y++"
    {   (yyval.ruleNode) = allocRuleNode((yyvsp[(1) - (3)].patVal), (yyvsp[(3) - (3)].stmtVal)); prt("Rule");   }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 378 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (2)].stmt_plusVal);    }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 379 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].stmt_plusVal);    }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 380 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode((yyvsp[(1) - (2)].exprVal)); prt("AssignStmt");  }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 381 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode(NULL);   }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 384 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocScopeNode((yyvsp[(2) - (3)].stmt_plusVal));    prt("CompoundStmt");   }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 385 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocScopeNode((yyvsp[(2) - (4)].stmt_plusVal));    prt("CompoundStmt");   }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 386 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = (yyvsp[(1) - (1)].stmt_plusVal);    }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 389 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(curStmtList1); curStmtList1 = NULL;  prt("CompoundStmt");  }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 390 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(new list<StmtNode*>);                prt("CompoundStmt");  }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 393 "E--_parser.y++"
    {   if (!curStmtList1) curStmtList1 = new list<StmtNode*>; curStmtList1->push_back((yyvsp[(2) - (2)].stmtVal));}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 397 "E--_parser.y++"
    {   (yyval.patVal) = allocPrimPatNode((yyvsp[(1) - (1)].eventEntry), curVarList, NULL);    curVarList = NULL;    prt("PrimitivePat"); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 398 "E--_parser.y++"
    {   (yyval.patVal) = allocPrimPatNode((yyvsp[(1) - (1)].eventEntry), curVarList, NULL);    curVarList = NULL;    prt("PrimitivePat"); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 399 "E--_parser.y++"
    {   (yyval.patVal) = allocPrimPatNode((yyvsp[(1) - (3)].eventEntry), curVarList, (yyvsp[(3) - (3)].exprVal));      curVarList = NULL;    prt("PrimitivePatWithCond");  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 400 "E--_parser.y++"
    {   (yyval.patVal) = (yyvsp[(1) - (1)].patVal);                        }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 401 "E--_parser.y++"
    {   (yyval.patVal) = (yyvsp[(2) - (3)].patVal);    prt("(EventPat)");  }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 404 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::OR  , (yyvsp[(1) - (3)].patVal), (yyvsp[(3) - (3)].patVal));   prt("EventPat \\/ EventPat");  }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 405 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::SEQ , (yyvsp[(1) - (3)].patVal), (yyvsp[(3) - (3)].patVal));   prt("EventPat : EventPat");    }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 406 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::STAR, (yyvsp[(1) - (2)].patVal), NULL); prt("EventPat **");            }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 407 "E--_parser.y++"
    {   (yyval.patVal) = allocPatNode(BasePatNode::PatNodeKind::NEG , (yyvsp[(2) - (2)].patVal), NULL); prt("!EventPat");              }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 410 "E--_parser.y++"
    {   allocRuleBlockNode();   (yyval.eventEntry) = any;   prt("anyEvent");    }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 413 "E--_parser.y++"
    {   (yyval.eventEntry) = getEventEntry((yyvsp[(1) - (4)].cVal));             prt("NormalEvent"); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 417 "E--_parser.y++"
    {   curVarList->push_back(allocVarEntry((yyvsp[(3) - (3)].cVal), VariableEntry::UNDEFINED, new Type(Type::UNKNOWN), NULL)); prt("EventFormalParam");}
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 418 "E--_parser.y++"
    {   allocRuleBlockNode(); curVarList = new vector<VariableEntry*>(); 
                                                                                curVarList->push_back(allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::UNDEFINED, new Type(Type::UNKNOWN), NULL)); prt("EventFormalParam");}
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 423 "E--_parser.y++"
    {   (yyval.ifVal) = allocIfNode((yyvsp[(2) - (3)].exprVal), (yyvsp[(3) - (3)].stmtVal), NULL); prt("IfThenStmt");          }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 424 "E--_parser.y++"
    {   (yyval.ifVal) = allocIfNode((yyvsp[(2) - (5)].exprVal), (yyvsp[(3) - (5)].stmtVal), (yyvsp[(5) - (5)].stmtVal));   prt("IfThenElseStmt");      }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 427 "E--_parser.y++"
    {   (yyval.stmtVal) = allocCompStmtNode(curStmtList2); curStmtList2 = NULL; prt("CompoundStmt");   }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 428 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].stmt_plusVal);                                                    }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 429 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].stmtVal);                                                    }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 430 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode(NULL);                               }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 433 "E--_parser.y++"
    {   if (!curStmtList2) curStmtList2 = new list<StmtNode*>; curStmtList2->push_back((yyvsp[(2) - (2)].stmtVal));}
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 437 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode((yyvsp[(1) - (2)].exprVal)); prt("FunctionInvocationStmt");  }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 438 "E--_parser.y++"
    {   (yyval.stmtVal) = allocExprStmtNode((yyvsp[(1) - (2)].exprVal)); prt("AssignStmt");              }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 439 "E--_parser.y++"
    {   (yyval.stmtVal) = (yyvsp[(1) - (1)].ifVal);                                                    }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 440 "E--_parser.y++"
    {   (yyval.stmtVal) = allocReturnNode((yyvsp[(2) - (3)].exprVal));   prt("ReturnStmt");              }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 441 "E--_parser.y++"
    {   (yyval.stmtVal) = allocReturnNode(NULL); prt("ReturnStmt");              }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 444 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(new list<StmtNode*>); prt("CompoundStmt");  }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 447 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(2) - (3)].exprVal);    prt("(Expr)"); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 450 "E--_parser.y++"
    {   (yyval.exprVal) = allocInvNode((yyvsp[(1) - (4)].cVal), curExprList); curExprList = NULL; prt("FunctionInvocation"); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 451 "E--_parser.y++"
    {   (yyval.exprVal) = allocInvNode((yyvsp[(1) - (3)].cVal), NULL);                            prt("FunctionInvocation"); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 454 "E--_parser.y++"
    {   (yyval.cVal) = (yyvsp[(1) - (1)].cVal);                }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 455 "E--_parser.y++"
    {   (yyval.cVal) = (char*)"print";    }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 458 "E--_parser.y++"
    {   curExprList->push_back((yyvsp[(3) - (3)].exprVal));                                             }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 459 "E--_parser.y++"
    {   curExprList = new vector<ExprNode*>(); curExprList->push_back((yyvsp[(1) - (1)].exprVal));      }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 462 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::ASSIGN, (yyvsp[(1) - (3)].refexpVal), (yyvsp[(3) - (3)].exprVal)); prt("Assignment");    }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 465 "E--_parser.y++"
    {   (yyval.exprVal) = allocValueNode((yyvsp[(1) - (1)].valVal));    }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 466 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].refexpVal);                    }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 467 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 468 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 469 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 470 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(1) - (1)].exprVal);                    }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 471 "E--_parser.y++"
    {   (yyval.exprVal) = (yyvsp[(2) - (3)].exprVal);    prt("(Expr)");  }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 474 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::NOT   , (yyvsp[(2) - (2)].exprVal), NULL); prt("!Expr");         }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 475 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITNOT, (yyvsp[(2) - (2)].exprVal), NULL); prt("~Expr");         }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 476 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::UMINUS, (yyvsp[(2) - (2)].exprVal), NULL); prt("-Expr");         }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 479 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::OR    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr || Expr");    }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 480 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::AND   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr && Expr");    }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 481 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::EQ    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr == Expr");    }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 482 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::NE    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr != Expr");    }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 483 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::GT    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr > Expr");     }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 484 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::LT    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr < Expr");     }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 485 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::GE    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr >= Expr");    }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 486 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::LE    , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr <= Expr");    }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 487 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITOR , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr | Expr");     }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 488 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITXOR, (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr ^ Expr");     }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 489 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::BITAND, (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr & Expr");     }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 490 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::SHL   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr << Expr");    }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 491 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::SHR   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr >> Expr");    }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 492 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::PLUS  , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr + Expr");     }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 493 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::MINUS , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr - Expr");     }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 494 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::MULT  , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr * Expr");     }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 495 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::DIV   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr / Expr");     }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 496 "E--_parser.y++"
    {   (yyval.exprVal) = allocOpNode(OpNode::OpCode::MOD   , (yyvsp[(1) - (3)].exprVal), (yyvsp[(3) - (3)].exprVal)); prt("Expr % Expr");     }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 499 "E--_parser.y++"
    {   (yyval.refexpVal) = allocRefExprNode((yyvsp[(1) - (1)].cVal)); prt("RefExpr"); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 509 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::GLOBAL_VAR, curType, nullptr); prt("VariableDecl");   }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 510 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (3)].cVal), VariableEntry::GLOBAL_VAR, curType, (yyvsp[(3) - (3)].exprVal)); prt("VariableDeclWithInit");}
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 513 "E--_parser.y++"
    {   (yyval.classEntry) = allocClassEntry((yyvsp[(2) - (3)].cVal)); prt("ClassDecl");  }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 516 "E--_parser.y++"
    {   (yyval.functionEntry) = (yyvsp[(1) - (2)].functionEntry);                    }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 517 "E--_parser.y++"
    {   (yyvsp[(1) - (2)].functionEntry)->body((yyvsp[(2) - (2)].stmt_plusVal));   (yyval.functionEntry) = (yyvsp[(1) - (2)].functionEntry);    }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 520 "E--_parser.y++"
    {   (yyval.stmt_plusVal) = allocCompStmtNode(curStmtList3);        curStmtList3 = NULL;     prt("FunctionBody");  }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 524 "E--_parser.y++"
    {   if (!curStmtList3) curStmtList3 = new list<StmtNode*>; curStmtList3->push_back((yyvsp[(2) - (2)].stmtVal));}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 535 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (1)].cVal), VariableEntry::LOCAL_VAR, curType, nullptr); prt("VariableDecl");    }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 536 "E--_parser.y++"
    {   (yyval.variableEntry) = allocVarEntry((yyvsp[(1) - (3)].cVal), VariableEntry::LOCAL_VAR, curType, (yyvsp[(3) - (3)].exprVal)); prt("VariableDeclWithInit"); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 539 "E--_parser.y++"
    {   (yyval.functionEntry) = allocFuncEntry((yyvsp[(2) - (3)].cVal), (yyvsp[(1) - (3)].typeVal), curTypeList);   curTypeList = NULL;     prt("FunctionDecl");  }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 542 "E--_parser.y++"
    {   prt("FormalParamList");     }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 543 "E--_parser.y++"
    {   curTypeList->push_back((yyvsp[(3) - (3)].typeVal)); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 544 "E--_parser.y++"
    {   curTypeList = new vector<Type*>(); curTypeList->push_back((yyvsp[(1) - (1)].typeVal));  }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 545 "E--_parser.y++"
    {   curFuncEnt = allocFuncEntry();  }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 547 "E--_parser.y++"
    {   if (!curFuncEnt) curFuncEnt = allocFuncEntry();  
                                                                                (yyval.typeVal) = (yyvsp[(1) - (2)].typeVal); allocVarEntry((yyvsp[(2) - (2)].cVal), VariableEntry::PARAM_VAR, (yyvsp[(1) - (2)].typeVal), nullptr);  prt("FormalParam"); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 551 "E--_parser.y++"
    {   (yyval.eventEntry) = allocEventEntry((yyvsp[(2) - (4)].cVal), curTypeList);  curTypeList = NULL;     prt("EventDecl");   }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 554 "E--_parser.y++"
    {   prt("FormalParamList");         }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 555 "E--_parser.y++"
    {   curTypeList->push_back((yyvsp[(3) - (3)].typeVal));     }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 556 "E--_parser.y++"
    {   curTypeList = new vector<Type*>(); curTypeList->push_back((yyvsp[(1) - (1)].typeVal));}
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 557 "E--_parser.y++"
    {   curEventEnt = allocEventEntry();}
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 560 "E--_parser.y++"
    {   if (!curEventEnt) curEventEnt = allocEventEntry();  
                                                                                (yyval.typeVal) = (yyvsp[(1) - (2)].typeVal); allocVarEntry((yyvsp[(2) - (2)].cVal), VariableEntry::PARAM_VAR, (yyvsp[(1) - (2)].typeVal), nullptr); prt("FormalParam");}
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 569 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].uVal), Type::UINT); prt("Literal");     }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 570 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].dVal));             prt("Literal");     }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 571 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].cVal));             prt("Literal");     }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 572 "E--_parser.y++"
    {   (yyval.valVal) = new Value(true);           prt("Literal");     }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 573 "E--_parser.y++"
    {   (yyval.valVal) = new Value(false);          prt("Literal");     }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 576 "E--_parser.y++"
    {   (yyval.typeVal) = curType = (yyvsp[(1) - (1)].typeVal);                  prt("Type");    }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 577 "E--_parser.y++"
    {   (yyval.typeVal) = curType = (yyvsp[(1) - (1)].typeVal);                  prt("Type");    }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 578 "E--_parser.y++"
    {   (yyval.typeVal) = curType = getUserType((yyvsp[(1) - (1)].cVal));     prt("Type");    }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 579 "E--_parser.y++"
    {   (yyval.typeVal) = curType = new Type(Type::VOID);prt("Type");
                                                                                errMsg("Invalid use of keyword \"unsigned\"");      }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 583 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::VOID);    }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 584 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::BOOL);    }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 585 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::BYTE);    }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 586 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::STRING);  }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 587 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::DOUBLE);  }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 590 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::INT);     }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 591 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::UINT);    }
    break;



/* Line 1806 of yacc.c  */
#line 2779 "E--_parser.C"
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
#line 594 "E--_parser.y++"


