/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
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
     TOK_DOT = 288,
     TOK_LBRACE = 289,
     TOK_RBRACE = 290,
     TOK_LPAREN = 291,
     TOK_RPAREN = 292,
     TOK_LBRACK = 293,
     TOK_IF = 294,
     TOK_ELSE = 295,
     TOK_CLASS = 296,
     TOK_RETURN = 297,
     TOK_EVENT = 298,
     TOK_ANY = 299,
     TOK_PRINT = 300,
     TOK_ULONGNUM = 301,
     TOK_UINTNUM = 302,
     TOK_DOUBLENUM = 303,
     TOK_VOID = 304,
     TOK_STRING = 305,
     TOK_STRCONST = 306,
     TOK_INT = 307,
     TOK_UNSIGNED = 308,
     TOK_BOOL = 309,
     TOK_BYTE = 310,
     TOK_DOUBLE = 311,
     TOK_SHORT = 312,
     TOK_LONG = 313,
     TOK_TRUE = 314,
     TOK_FALSE = 315,
     TOK_IDENT = 316
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 10 "E--_parser.y++"

   char* cVal;
   unsigned int uVal;
   double dblVal;



/* Line 2068 of yacc.c  */
#line 119 "E--_parser.H"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


