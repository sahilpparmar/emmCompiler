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

#define prt(x) cout << x << endl;

static VariableEntry* allocateVarNode(string name, VariableEntry::VarKind v, Type* type, ExprNode* init) {
    return new VariableEntry(name, v, type, init, yylineno, yycolumnno, string(yyfilename));
}



/* Line 268 of yacc.c  */
#line 94 "E--_parser.C"

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
#line 23 "E--_parser.y++"

  unsigned int 	     uVal;
  double             dVal;
  char*              cVal;
  vector<string>*    strVector;
  Value*             valVal;

  ExprNode*          exprVal;
  vector<ExprNode*>* exprList;
  RefExprNode*       refexpVal;

  ClassEntry*        classEntry;
  Type*              typeVal;
  vector<Type*>*     typeList; 

  EventEntry*        eventEntry;                        
  VariableEntry*     variableEntry;
  FunctionEntry*     functionEntry;

  BasePatNode*       patVal;
  PrimitivePatNode*  primPatVal;
  StmtNode*          stmtVal;
  list<StmtNode*>*   stmtList;
  IfNode*            ifVal;
  CompoundStmtNode*  compoundStmtVal;
  RuleNode*          ruleNode;

  vector<RuleNode*>* transList;
  
  vector<Type*>*     formalTypeVal;
  const OpNode::OpInfo*    funIval;



/* Line 293 of yacc.c  */
#line 225 "E--_parser.C"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 237 "E--_parser.C"

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   454

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  61
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  122
/* YYNRULES -- Number of states.  */
#define YYNSTATES  206

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
       0,     0,     3,     5,     7,    10,    13,    14,    16,    18,
      20,    22,    26,    31,    35,    38,    40,    43,    45,    47,
      51,    53,    57,    61,    65,    69,    72,    73,    75,    78,
      82,    86,    88,    91,    95,    97,   100,   101,   104,   107,
     109,   113,   119,   123,   125,   127,   129,   133,   137,   142,
     146,   150,   152,   156,   158,   160,   162,   164,   166,   168,
     172,   175,   178,   181,   185,   189,   193,   197,   201,   205,
     209,   213,   217,   221,   225,   229,   233,   237,   241,   245,
     249,   253,   255,   259,   263,   267,   269,   275,   279,   284,
     288,   291,   294,   295,   300,   304,   307,   310,   314,   318,
     320,   323,   326,   329,   334,   338,   342,   344,   347,   350,
     353,   355,   357,   359,   361,   363,   365,   367,   369,   371,
     373,   375,   377
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      62,     0,    -1,    63,    -1,    64,    -1,    64,    65,    -1,
      64,    66,    -1,    -1,    96,    -1,    93,    -1,    89,    -1,
      86,    -1,    68,    32,    67,    -1,    33,    73,    34,    30,
      -1,    33,    73,    34,    -1,    77,    30,    -1,    77,    -1,
      82,    30,    -1,    71,    -1,    70,    -1,    71,    14,    82,
      -1,    69,    -1,    35,    68,    36,    -1,    68,     5,    68,
      -1,    68,    31,    68,    -1,    68,     4,    68,    -1,    27,
      68,    -1,    -1,    42,    -1,    57,    72,    -1,    35,    72,
      36,    -1,    72,    28,    57,    -1,    57,    -1,    35,    36,
      -1,    33,    73,    34,    -1,    77,    -1,    73,    74,    -1,
      -1,    79,    30,    -1,    81,    30,    -1,    75,    -1,    37,
      78,    76,    -1,    37,    78,    76,    38,    76,    -1,    33,
      73,    34,    -1,    77,    -1,    74,    -1,    30,    -1,    33,
      30,    34,    -1,    35,    82,    36,    -1,    57,    35,    80,
      36,    -1,    57,    35,    36,    -1,    80,    28,    82,    -1,
      82,    -1,    85,    29,    82,    -1,    99,    -1,    85,    -1,
      79,    -1,    81,    -1,    83,    -1,    84,    -1,    35,    82,
      36,    -1,    27,    82,    -1,    15,    82,    -1,     8,    82,
      -1,    82,    26,    82,    -1,    82,    25,    82,    -1,    82,
      23,    82,    -1,    82,    24,    82,    -1,    82,    19,    82,
      -1,    82,    20,    82,    -1,    82,    21,    82,    -1,    82,
      22,    82,    -1,    82,    14,    82,    -1,    82,    16,    82,
      -1,    82,    13,    82,    -1,    82,    17,    82,    -1,    82,
      18,    82,    -1,    82,     7,    82,    -1,    82,     8,    82,
      -1,    82,    10,    82,    -1,    82,    11,    82,    -1,    82,
      12,    82,    -1,    57,    -1,   100,    87,    30,    -1,   100,
      88,    30,    -1,    87,    28,    57,    -1,    57,    -1,    88,
      28,    57,    29,    82,    -1,    57,    29,    82,    -1,    39,
      57,    30,    30,    -1,    39,    57,    30,    -1,    90,    86,
      -1,    90,    74,    -1,    -1,    33,    90,    34,    30,    -1,
     100,    57,    94,    -1,    92,    30,    -1,    92,    91,    -1,
      35,    94,    36,    -1,    94,    28,    95,    -1,    95,    -1,
      35,    36,    -1,   100,    57,    -1,    57,    57,    -1,    41,
      57,    97,    30,    -1,    35,    97,    36,    -1,    97,    28,
      98,    -1,    98,    -1,    35,    36,    -1,   100,    57,    -1,
      57,    57,    -1,    44,    -1,    45,    -1,    48,    -1,    55,
      -1,    56,    -1,   101,    -1,    46,    -1,    51,    -1,    52,
      -1,    47,    -1,    49,    -1,    53,    -1,    50,    49,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   152,   152,   155,   158,   159,   160,   163,   164,   165,
     166,   169,   172,   173,   174,   175,   176,   179,   180,   181,
     182,   183,   186,   187,   188,   189,   190,   193,   196,   199,
     200,   201,   202,   206,   207,   208,   209,   212,   213,   214,
     217,   218,   221,   222,   223,   224,   227,   231,   234,   235,
     238,   239,   242,   244,   245,   246,   247,   248,   249,   250,
     253,   254,   255,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   278,   281,   282,   285,   286,   289,   290,   292,   293,
     296,   297,   298,   301,   303,   306,   307,   310,   311,   312,
     313,   315,   316,   319,   322,   323,   324,   325,   328,   329,
     333,   334,   335,   336,   337,   340,   343,   344,   345,   346,
     347,   348,   349
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
  "s_specification", "program", "specification", "declaration", "rule",
  "rule_stmts", "primitive_all", "binary_event_oprs", "any_event",
  "normal_event", "event_aparam_l", "compoundStmt", "simple_stmt",
  "if_then_else_stmt", "basic_block", "empty_block", "condition",
  "func_invocation", "caller_param_l", "assignment", "expression",
  "unary_operation", "binary_operation", "ref_expr", "variable_decl",
  "variable_l", "variable_init_l", "class_decl", "function_body",
  "function_defn", "function_decl", "function_proto", "func_param_l",
  "func_param", "event_decl", "event_param_l", "event_param", "literal",
  "type", "baseType", 0
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
       0,    61,    62,    63,    64,    64,    64,    65,    65,    65,
      65,    66,    67,    67,    67,    67,    67,    68,    68,    68,
      68,    68,    69,    69,    69,    69,    69,    70,    71,    72,
      72,    72,    72,    73,    73,    73,    73,    74,    74,    74,
      75,    75,    76,    76,    76,    76,    77,    78,    79,    79,
      80,    80,    81,    82,    82,    82,    82,    82,    82,    82,
      83,    83,    83,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    85,    86,    86,    87,    87,    88,    88,    89,    89,
      90,    90,    90,    91,    92,    93,    93,    94,    94,    94,
      94,    95,    95,    96,    97,    97,    97,    97,    98,    98,
      99,    99,    99,    99,    99,   100,   101,   101,   101,   101,
     101,   101,   101
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     0,     1,     1,     1,
       1,     3,     4,     3,     2,     1,     2,     1,     1,     3,
       1,     3,     3,     3,     3,     2,     0,     1,     2,     3,
       3,     1,     2,     3,     1,     2,     0,     2,     2,     1,
       3,     5,     3,     1,     1,     1,     3,     3,     4,     3,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     3,     3,     3,     1,     5,     3,     4,     3,
       2,     2,     0,     4,     3,     2,     2,     3,     3,     1,
       2,     2,     2,     4,     3,     3,     1,     2,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       6,     0,     2,    26,     1,    26,    26,     0,     0,    27,
     116,   119,   120,     0,   117,   118,   121,     0,     4,     5,
       0,    20,    18,    17,    10,     9,     0,     8,     7,     0,
     115,    25,     0,     0,     0,   122,     0,    31,    28,    26,
      26,    26,     0,     0,    95,    92,    96,    85,     0,     0,
      21,    89,     0,     0,     0,   106,     0,    32,     0,     0,
      24,    22,    23,     0,     0,     0,    36,     0,   110,   111,
     112,   113,   114,    81,    11,    15,    55,    56,     0,    57,
      58,    54,    53,    19,     0,     0,     0,     0,    94,    99,
       0,     0,    82,     0,    83,    88,   107,     0,   109,     0,
     103,   108,    29,    30,    62,    61,    60,     0,    36,     0,
      34,     0,     0,    14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    16,     0,     0,     0,    91,    39,     0,     0,
       0,    90,     0,    87,   100,     0,   102,     0,   101,    84,
       0,   104,   105,    46,     0,    13,    35,    59,    49,     0,
      51,    76,    77,    78,    79,    80,    73,    71,    72,    74,
      75,    67,    68,    69,    70,    65,    66,    64,    63,    52,
      93,     0,     0,    37,    38,    85,    97,    98,     0,    33,
      12,     0,    48,     0,    45,    36,    44,    40,    43,    86,
      50,    47,     0,     0,    42,    41
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,    18,    19,    74,    20,    21,    22,
      23,    38,   109,   156,   137,   197,   110,   182,    76,   159,
      77,    78,    79,    80,    81,    24,    48,    49,    25,    84,
      46,    26,    27,    88,    89,    28,    54,    55,    82,    56,
      30
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -101
static const yytype_int16 yypact[] =
{
    -101,    19,  -101,   117,  -101,   -21,   -21,   -50,   -44,  -101,
    -101,  -101,  -101,   -19,  -101,  -101,  -101,    13,  -101,  -101,
      11,  -101,  -101,    49,  -101,  -101,    73,  -101,  -101,    39,
    -101,  -101,    29,    67,   292,  -101,    16,  -101,    85,   -21,
     -21,   -21,     2,   168,  -101,  -101,  -101,    81,   -10,    79,
    -101,    88,   264,    66,   115,  -101,    80,  -101,    26,    97,
    -101,    18,   120,   168,   168,   168,   106,   168,  -101,  -101,
    -101,  -101,  -101,   107,  -101,   127,  -101,  -101,   261,  -101,
    -101,   133,  -101,   340,   255,   168,   283,   108,   143,  -101,
     116,   118,  -101,   121,  -101,  -101,  -101,    33,  -101,   158,
    -101,  -101,  -101,  -101,  -101,   436,   379,   148,   106,   -25,
    -101,   219,   145,  -101,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   168,   168,   168,   168,   168,
     168,   168,  -101,   168,   147,   144,  -101,  -101,   154,   155,
     133,  -101,   129,   340,  -101,    76,  -101,   376,  -101,  -101,
     159,  -101,  -101,  -101,     7,   157,  -101,  -101,  -101,    86,
     340,    56,    56,  -101,  -101,  -101,   436,   424,   424,   210,
     210,   397,   397,   397,   397,   397,   397,   379,   360,   340,
    -101,   168,    78,  -101,  -101,   162,  -101,  -101,   168,  -101,
    -101,   168,  -101,   240,  -101,   106,  -101,   156,  -101,   340,
     340,  -101,    68,    78,  -101,  -101
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -101,  -101,  -101,  -101,  -101,  -101,  -101,    -1,  -101,  -101,
    -101,   160,  -100,   -82,  -101,   -11,   -42,  -101,   -83,  -101,
     -56,   -40,  -101,  -101,   -53,   113,  -101,  -101,  -101,  -101,
    -101,  -101,  -101,   112,    52,  -101,   167,   135,  -101,     8,
    -101
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -4
static const yytype_int16 yytable[] =
{
      75,   138,   136,    83,    31,    32,     5,    33,   154,   155,
      63,    29,   135,    34,     6,    39,    40,    64,    91,     4,
      92,     9,    39,   104,   105,   106,   138,   111,   139,    65,
      35,   140,    73,    39,    40,    66,    17,    67,    60,    61,
      62,   189,    41,    42,   135,   143,    68,    69,    36,    41,
      70,    36,    57,   139,    59,    90,   140,    71,    72,    73,
      41,    99,   102,    43,    73,    50,   116,   117,   118,   151,
      37,   138,   160,    37,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   142,   179,    90,   202,    47,    51,   139,   138,
     196,   140,   204,    44,   147,   135,    45,    93,   194,    94,
      85,   195,   186,    59,   191,   135,    86,    -3,    95,   138,
     138,   196,   192,    98,    39,    73,   139,    10,    11,   140,
      12,    13,    14,    15,    16,    73,   107,   101,    87,   108,
     198,   193,   112,    99,     5,   100,   139,   139,   199,   140,
     140,   200,     6,    63,   103,    90,     7,   113,     8,     9,
      64,   198,   133,    10,    11,   146,    12,    13,    14,    15,
      16,   147,    65,   148,    17,   149,    63,   180,   150,   181,
      67,   158,   153,    64,   183,   184,   185,   190,   188,    68,
      69,    85,   205,    70,   203,    65,    58,   141,   145,   187,
      71,    72,    73,    67,    10,    11,     0,    12,    13,    14,
      15,    16,    68,    69,     0,    53,    70,   114,   115,    97,
     116,   117,   118,    71,    72,    73,   114,   115,     0,   116,
     117,   118,   119,   120,   152,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,     0,   114,   115,     0,
     116,   117,   118,   119,   120,   157,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,     0,   114,   115,
       0,   116,   117,   118,   119,   120,   201,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,     0,   134,
       0,   132,   135,     0,     0,     0,     0,     0,     0,    52,
      96,    10,    11,     0,    12,    13,    14,    15,    16,     0,
      10,    11,    73,    12,    13,    14,    15,    16,    86,   144,
       0,    53,     0,     0,     0,     0,     0,    52,     0,    10,
      11,     0,    12,    13,    14,    15,    16,     0,    10,    11,
      87,    12,    13,    14,    15,    16,     0,   114,   115,    53,
     116,   117,   118,   119,   120,     0,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   114,   115,     0,
     116,   117,   118,   119,   120,     0,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   114,   115,     0,   116,
     117,   118,   119,   120,     0,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   114,   115,     0,   116,   117,   118,
     119,   120,     0,   121,   122,   123,    -4,    -4,    -4,    -4,
      -4,    -4,    10,    11,     0,    12,    13,    14,    15,    16,
       0,   114,   115,    87,   116,   117,   118,   119,     0,     0,
       0,   122,   123,   114,   115,     0,   116,   117,   118,     0,
       0,     0,     0,   122,   123
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-101))

#define yytable_value_is_error(yytable_value) \
  ((yytable_value) == (-4))

static const yytype_int16 yycheck[] =
{
      42,    84,    84,    43,     5,     6,    27,    57,   108,    34,
       8,     3,    37,    57,    35,     4,     5,    15,    28,     0,
      30,    42,     4,    63,    64,    65,   109,    67,    84,    27,
      49,    84,    57,     4,     5,    33,    57,    35,    39,    40,
      41,    34,    31,    32,    37,    85,    44,    45,    35,    31,
      48,    35,    36,   109,    28,    47,   109,    55,    56,    57,
      31,    28,    36,    14,    57,    36,    10,    11,    12,    36,
      57,   154,   112,    57,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,    84,   133,    86,   195,    57,    30,   154,   182,
     182,   154,    34,    30,    28,    37,    33,    28,    30,    30,
      29,    33,    36,    28,    28,    37,    35,     0,    30,   202,
     203,   203,    36,    57,     4,    57,   182,    46,    47,   182,
      49,    50,    51,    52,    53,    57,    30,    57,    57,    33,
     182,   181,    35,    28,    27,    30,   202,   203,   188,   202,
     203,   191,    35,     8,    57,   147,    39,    30,    41,    42,
      15,   203,    29,    46,    47,    57,    49,    50,    51,    52,
      53,    28,    27,    57,    57,    57,     8,    30,    57,    35,
      35,    36,    34,    15,    30,    30,    57,    30,    29,    44,
      45,    29,   203,    48,    38,    27,    36,    84,    86,   147,
      55,    56,    57,    35,    46,    47,    -1,    49,    50,    51,
      52,    53,    44,    45,    -1,    57,    48,     7,     8,    52,
      10,    11,    12,    55,    56,    57,     7,     8,    -1,    10,
      11,    12,    13,    14,    99,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,     7,     8,    -1,
      10,    11,    12,    13,    14,    36,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    -1,     7,     8,
      -1,    10,    11,    12,    13,    14,    36,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    -1,    34,
      -1,    30,    37,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      36,    46,    47,    -1,    49,    50,    51,    52,    53,    -1,
      46,    47,    57,    49,    50,    51,    52,    53,    35,    36,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    35,    -1,    46,
      47,    -1,    49,    50,    51,    52,    53,    -1,    46,    47,
      57,    49,    50,    51,    52,    53,    -1,     7,     8,    57,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,     7,     8,    -1,
      10,    11,    12,    13,    14,    -1,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,     7,     8,    -1,    10,
      11,    12,    13,    14,    -1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,     7,     8,    -1,    10,    11,    12,
      13,    14,    -1,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    46,    47,    -1,    49,    50,    51,    52,    53,
      -1,     7,     8,    57,    10,    11,    12,    13,    -1,    -1,
      -1,    17,    18,     7,     8,    -1,    10,    11,    12,    -1,
      -1,    -1,    -1,    17,    18
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    62,    63,    64,     0,    27,    35,    39,    41,    42,
      46,    47,    49,    50,    51,    52,    53,    57,    65,    66,
      68,    69,    70,    71,    86,    89,    92,    93,    96,   100,
     101,    68,    68,    57,    57,    49,    35,    57,    72,     4,
       5,    31,    32,    14,    30,    33,    91,    57,    87,    88,
      36,    30,    35,    57,    97,    98,   100,    36,    72,    28,
      68,    68,    68,     8,    15,    27,    33,    35,    44,    45,
      48,    55,    56,    57,    67,    77,    79,    81,    82,    83,
      84,    85,    99,    82,    90,    29,    35,    57,    94,    95,
     100,    28,    30,    28,    30,    30,    36,    97,    57,    28,
      30,    57,    36,    57,    82,    82,    82,    30,    33,    73,
      77,    82,    35,    30,     7,     8,    10,    11,    12,    13,
      14,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    30,    29,    34,    37,    74,    75,    79,    81,
      85,    86,   100,    82,    36,    94,    57,    28,    57,    57,
      57,    36,    98,    34,    73,    34,    74,    36,    36,    80,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      30,    35,    78,    30,    30,    57,    36,    95,    29,    34,
      30,    28,    36,    82,    30,    33,    74,    76,    77,    82,
      82,    36,    73,    38,    34,    76
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
#line 152 "E--_parser.y++"
    {       prt("Spec");    }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 169 "E--_parser.y++"
    {       prt("Rule");  }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 172 "E--_parser.y++"
    {       prt("CompoundStmt");  }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 173 "E--_parser.y++"
    {       prt("CompoundStmt");  }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 176 "E--_parser.y++"
    {       prt("AssignStmt");    }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 179 "E--_parser.y++"
    {       prt("PrimitivePat"); }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 180 "E--_parser.y++"
    {       prt("PrimitivePat");  }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 181 "E--_parser.y++"
    {       prt("PrimitivePatWithCond");  }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 183 "E--_parser.y++"
    {       prt("(EventPat)");  }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 186 "E--_parser.y++"
    {       prt("EventPat \\/ EventPat");     }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 187 "E--_parser.y++"
    {       prt("EventPat : EventPat");       }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 188 "E--_parser.y++"
    {       prt("EventPat **");      }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 189 "E--_parser.y++"
    {       prt("!EventPat");     }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 193 "E--_parser.y++"
    {       prt("anyEvent");   }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 196 "E--_parser.y++"
    {       prt("NormalEvent");   }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 200 "E--_parser.y++"
    {       prt("EventFormalParam");   }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 201 "E--_parser.y++"
    {       prt("EventFormalParam");   }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 206 "E--_parser.y++"
    {       prt("CompoundStmt");  }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 212 "E--_parser.y++"
    {       prt("FunctionInvocationStmt"); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 213 "E--_parser.y++"
    {       prt("AssignStmt"); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 217 "E--_parser.y++"
    {       prt("IfThenStmt"); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 218 "E--_parser.y++"
    {       prt("IfThenElseStmt"); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 221 "E--_parser.y++"
    {       prt("CompoundStmt");  }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 227 "E--_parser.y++"
    {       prt("CompoundStmt");  }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 231 "E--_parser.y++"
    {       prt("(Expr)"); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 234 "E--_parser.y++"
    {       prt("FunctionInvocation"); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 235 "E--_parser.y++"
    {       prt("FunctionInvocation"); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 242 "E--_parser.y++"
    {       prt("Assignment");        }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 250 "E--_parser.y++"
    {       prt("(Expr)");            }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 253 "E--_parser.y++"
    {       prt("!Expr");             }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 254 "E--_parser.y++"
    {       prt("~Expr");             }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 255 "E--_parser.y++"
    {       prt("-Expr");             }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 258 "E--_parser.y++"
    {       prt("Expr || Expr");      }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 259 "E--_parser.y++"
    {       prt("Expr && Expr");      }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 260 "E--_parser.y++"
    {       prt("Expr == Expr");      }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 261 "E--_parser.y++"
    {       prt("Expr != Expr");      }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 262 "E--_parser.y++"
    {       prt("Expr > Expr");       }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 263 "E--_parser.y++"
    {       prt("Expr < Expr");       }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 264 "E--_parser.y++"
    {       prt("Expr >= Expr");      }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 265 "E--_parser.y++"
    {       prt("Expr <= Expr");      }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 266 "E--_parser.y++"
    {       prt("Expr | Expr");       }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 267 "E--_parser.y++"
    {       prt("Expr ^ Expr");       }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 268 "E--_parser.y++"
    {       prt("Expr & Expr");       }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 269 "E--_parser.y++"
    {       prt("Expr << Expr");      }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 270 "E--_parser.y++"
    {       prt("Expr >> Expr");      }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 271 "E--_parser.y++"
    {       prt("Expr + Expr");       }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 272 "E--_parser.y++"
    {       prt("Expr - Expr");       }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 273 "E--_parser.y++"
    {       prt("Expr * Expr");       }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 274 "E--_parser.y++"
    {       prt("Expr / Expr");       }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 275 "E--_parser.y++"
    {       prt("Expr % Expr");       }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 278 "E--_parser.y++"
    {       prt("RefExpr");           }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 285 "E--_parser.y++"
    {       prt("VariableDecl"); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 286 "E--_parser.y++"
    {       prt("VariableDecl"); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 289 "E--_parser.y++"
    {       prt("VariableDeclWithInit");}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 290 "E--_parser.y++"
    {       prt("VariableDeclWithInit");}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 292 "E--_parser.y++"
    {       prt("ClassDecl");  }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 293 "E--_parser.y++"
    {       prt("ClassDecl");  }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 301 "E--_parser.y++"
    {       prt("FunctionBody");   }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 303 "E--_parser.y++"
    {       prt("FunctionDecl");  }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 310 "E--_parser.y++"
    {       prt("FormalParamList");   }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 315 "E--_parser.y++"
    {   (yyval.variableEntry) = allocateVarNode((yyvsp[(2) - (2)].cVal), VariableEntry::VarKind::PARAM_VAR, (yyvsp[(1) - (2)].typeVal), nullptr); prt("FormalParam");}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 316 "E--_parser.y++"
    {       prt("Type"); prt("FormalParam");  }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 319 "E--_parser.y++"
    {       prt("EventDecl");   }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 322 "E--_parser.y++"
    {       prt("FormalParamList");   }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 328 "E--_parser.y++"
    {   (yyval.variableEntry) = allocateVarNode((yyvsp[(2) - (2)].cVal), VariableEntry::VarKind::PARAM_VAR, (yyvsp[(1) - (2)].typeVal), nullptr); prt("FormalParam");}
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 329 "E--_parser.y++"
    {       prt("Type"); prt("FormalParam");  }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 333 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].uVal), Type::UINT); prt("Literal");    }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 334 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].dVal));             prt("Literal");    }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 335 "E--_parser.y++"
    {   (yyval.valVal) = new Value((yyvsp[(1) - (1)].cVal));             prt("Literal");    }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 336 "E--_parser.y++"
    {   (yyval.valVal) = new Value(true);           prt("Literal");    }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 337 "E--_parser.y++"
    {   (yyval.valVal) = new Value(false);          prt("Literal");    }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 340 "E--_parser.y++"
    {   (yyval.typeVal) = (yyvsp[(1) - (1)].typeVal);                        prt("Type");       }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 343 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::VOID);    }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 344 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::BOOL);    }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 345 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::BYTE);    }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 346 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::STRING);  }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 347 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::INT);     }
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 348 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::DOUBLE);  }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 349 "E--_parser.y++"
    {   (yyval.typeVal) = new Type(Type::UINT);    }
    break;



/* Line 1806 of yacc.c  */
#line 2274 "E--_parser.C"
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
#line 352 "E--_parser.y++"


