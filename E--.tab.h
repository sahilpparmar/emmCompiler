/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_E_PARSER_H_INCLUDED
# define YY_YY_E_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
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

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 237 "E--_parser.y++" /* yacc.c:1909  */

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
    CompoundStmtNode*   stmt_plusVal;
    RuleNode*           ruleNode;

    BasePatNode*        patVal;
    PrimitivePatNode*   primPatVal;
    vector<RuleNode*>*  transList;

    const OpNode::OpInfo*   funIval;

#line 157 "E--_parser.H" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_E_PARSER_H_INCLUDED  */
