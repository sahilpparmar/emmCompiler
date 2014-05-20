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
     TOK_INTNUM = 259,
     TOK_DOUBLENUM = 260,
     TOK_STRCONST = 261,
     TOK_IDENT = 262,
     TOK_IREG = 263,
     TOK_FREG = 264,
     TOK_BINIOP = 265,
     TOK_BINFOP = 266,
     TOK_NEG = 267,
     TOK_FNEG = 268,
     TOK_CMPIOP = 269,
     TOK_CMPFOP = 270,
     TOK_BNOT = 271,
     TOK_MOVL = 272,
     TOK_MOVS = 273,
     TOK_MOVI = 274,
     TOK_MOVF = 275,
     TOK_MOVIF = 276,
     TOK_MOVFI = 277,
     TOK_STI = 278,
     TOK_STF = 279,
     TOK_LDI = 280,
     TOK_LDF = 281,
     TOK_JMP = 282,
     TOK_JMPI = 283,
     TOK_JMPC = 284,
     TOK_JMPCI = 285,
     TOK_PRTI = 286,
     TOK_PRTF = 287,
     TOK_PRTS = 288,
     TOK_IN = 289,
     TOK_INI = 290,
     TOK_INF = 291,
     TOK_NEWLINE = 292,
     TOK_COLON = 293,
     TOK_LPAREN = 294,
     TOK_RPAREN = 295
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 19 "ICode.y"

  char*              cVal;



/* Line 2068 of yacc.c  */
#line 96 "ICode_parser.H"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


