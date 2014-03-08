
#define TOK_SYNTAX_ERROR 251

#define TOK_INTERFACE 	252
#define TOK_USE   	253
#define TOK_ANY 	254
#define TOK_BIT		255
#define TOK_BYTE 	256
#define TOK_BOOL	257
#define TOK_DOUBLE      258
#define TOK_ENUM        259
#define TOK_CLASS 	260
#define TOK_EVENT       261
#define TOK_PRINT       262
#define TOK_STRING      263
#define TOK_ELSE 	264
#define TOK_FALSE 	266
#define TOK_IF 		270
#define TOK_INT 	271
#define TOK_RETURN 	277
#define TOK_TRUE 	278
#define TOK_VOID 	279
	
#define TOK_UINTNUM 	280
#define TOK_DOUBLENUM   281
#define TOK_STRCONST 	282
	
#define TOK_ID		288
	
#define TOK_COLON 	289
#define TOK_COMMA 	290
#define TOK_DOT 	291
#define TOK_SEMICOLON 	292
#define TOK_LBRACK 	293
#define TOK_RBRACK 	294
#define TOK_LPAREN 	295
#define TOK_RPAREN 	296
#define TOK_LBRACE 	297
#define TOK_RBRACE 	298
#define TOK_PLUS 	299
#define TOK_MINUS 	300
#define TOK_MULT 	301
#define TOK_DIV 	302
#define TOK_ASSIGN 	305
#define TOK_AND 	306
#define TOK_OR 		307
#define TOK_NOT 	308
#define TOK_GT		309
#define TOK_LT 		310
#define TOK_EQ 		311
#define TOK_NE		312
#define TOK_GE 		313
#define TOK_LE 		314
#define TOK_MOD 	315
#define TOK_ARROW       316
#define TOK_QMARK	317
#define TOK_PAT_OR	318
#define TOK_BITAND	319
#define TOK_BITOR	320
#define TOK_BITNOT	321
#define TOK_BITXOR	322
#define TOK_SHL		323
#define TOK_SHR		324

typedef union {
   char* cVal;
   int   uVal;
   float dVal;
} YYSTYPE;

extern YYSTYPE yylval;

extern int yylinenum;
extern const char* yyfilename;
extern char *yytext;

extern "C" int yylex();
