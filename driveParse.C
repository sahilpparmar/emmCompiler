#include <libgen.h>
#include <unistd.h>

#include <iostream>
#include "all.h"
#include "Ast.h"
#include <stdio.h>
#include "SymTabMgr.h"
#include "STEClasses.h"
#include "InterCode.h"
#include "SymTab.h"
#include "Value.h"
#include "FinalMachineCodeGen.h"

using namespace std;

#include "E--.tab.h"

#define CPP_PROG_NAME "CPP" // Environment variable to specify location of cpp
#define DEFAULT_CPP_PROG_NAME "cpp"
#define CC_PROG_NAME "CC" // Environment variable to specify C compiler
#define DEFAULT_CC_PROG_NAME "gcc -g"
#define CC_PROG_OPTS "CFLAGS"

#define MAX_DEBUG_LEVEL 2

#ifdef DEBUG_LEXER
#ifdef LEXER_OUT
ofstream lexOutStr(LEXER_OUT);
ostream& lexOut = lexOutStr;
#else
ostream& lexOut = cout;
#endif
#endif

extern int debugLevel;
extern int optLevel;
extern EventEntry* any;
extern int yyparse();
extern int yylinenum;
extern const char* yyfilename;
extern YYSTYPE yylval;

void yyerror(const char *s)
{
    errMsg(s);
}

SymTabMgr stm;
string outputFile;
const char* inputFile = "";
string cppShellCmd, ccShellCmd;

bool genCCode;
bool genBinCode;
bool genSharedLib;
bool genDottyCode;
bool genPrologCode;
string outFileSuffix;  
extern FILE* yyin;

void
printUsage(const char* cmd) {
    cerr << "Usage: " << cmd << //" [-v <num>] "
        "[-I include_dir] [-L<library directory>]\n   [-l<library>]"
        "[-Dmacro[=defn]] [-Uname] " //[-t <target language>]\n   "
        "[-o <output file>] <input file>\n" 
        /*"\t-v: debugging\n"
          "\t\t0: none\n"
          "\t\t1: print AST 'out' file\n"
          "\t\t2: print lots of internal debugging info\n"
          "\t-o output file name\n"
          "\t-t: Specify type of output\n"
          "\t\tc: Output C code\n"
          "\t\tb: Output a binary (.o) file\n"
          "\t\ts: Output a shared library (default)\n"
          "\t\td: Output a dotty file\n"
          "\t\tp: Output Prolog code\n"*/
        "Environment variable CPP can be used to override the default command\n"
        "for running cpp program. This variable can include the program name\n"
        "as well as command-line arguments to the command. Similarly, the\n"
        "environment variable CC can specify the command to be run for compiling\n"
        "C code produced by this compiler, but the options to the compiler can\n"
        "specified independently using the environment variable CFLAGS.\n";
}

int lParseOption(char *fOption) {
    if (strcmp(fOption, "-debug") == 0 || strcmp(fOption, "--debug-dump") == 0)
        debugLevel = 1;
    else if (strcmp(fOption, "-noopt") == 0 || strcmp(fOption, "--no-optimizations") == 0)
        optLevel = 0;
    else {
        cerr << "Invalid Option Specified '" << fOption << "'\n";
        return 1;
    }
    return 0;
}

int 
parseOptions(int argc, char* argv[]) {
    int fileInputFound, invalidOptions, i;
    opterr = 0; // Suppress printing of errors by getopt.

    if (getenv(CPP_PROG_NAME) == NULL)
        cppShellCmd = DEFAULT_CPP_PROG_NAME;  else cppShellCmd = getenv(CPP_PROG_NAME);
    cppShellCmd += " ";

    if (getenv(CC_PROG_NAME) == NULL)
        ccShellCmd = DEFAULT_CC_PROG_NAME;
    else ccShellCmd = getenv(CC_PROG_NAME);
    ccShellCmd += " ";
    if (getenv(CC_PROG_OPTS) != NULL) {
        ccShellCmd += getenv(CC_PROG_OPTS);
        ccShellCmd += " ";
    }

    fileInputFound = 0;
    invalidOptions = 0;
    debugLevel = 0;
    optLevel = 1;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // Commandline Options
            invalidOptions += lParseOption(argv[i]);
        } else if (fileInputFound == 0) {
            fileInputFound = 1;
            inputFile = argv[i];
        } else {
            fileInputFound++;
        }
    }

    if (fileInputFound != 1) {
        cerr << "Please specify only a single input file\n";
        return -1;
    } else if (invalidOptions) {
        return -1;
    } else {
        return 0;
    }

    genSharedLib = !(genCCode || genBinCode || genDottyCode || genPrologCode);
    if (genSharedLib)
        outFileSuffix = ".so";

    if (*inputFile == '\0') 
        return -1;

    if (outputFile == "") {
        size_t pos;
        if (genSharedLib) {
            string sinputFile(inputFile);
            if ((pos = sinputFile.rfind('/')) == string::npos)
                outputFile = "lib" + sinputFile;
            else outputFile = 
                sinputFile.substr(0, pos) + "/lib" + sinputFile.substr(pos+1);
        }
        else outputFile = inputFile;

        if ((pos = outputFile.rfind('.')) != outputFile.npos)
            outputFile.replace(pos, outputFile.size(), outFileSuffix);
        else outputFile += outFileSuffix;
    }

    return 0;
}

int 
main(int argc, char *argv[], char *envp[]) {

    string ccCmd;
    int optionsOK = parseOptions(argc, argv);
    if (optionsOK < 0)
        return -1;

    cppShellCmd += inputFile;
    cppShellCmd += " ";

    /* ccCmd = "mv ";
       ccCmd += outputFile;
       ccCmd += " ";
       ccCmd += outputFile;
       ccCmd += ".c; ";
       ccCmd += ccShellCmd;
       if (genSharedLib)
       ccCmd += "-shared ";
       ccCmd += "-o ";
       ccCmd += outputFile;*/

    if ((yyin = popen(cppShellCmd.c_str(), "r")) == NULL) {
        cerr << "Unexpected error in reading input file\n";
        return 1;
    }

#ifdef TEST_LEXER
    int token;

    while (token = yylex()) {
        if (token == TOK_SYNTAX_ERROR) {
            cout << yyfilename << ":" << yylinenum 
                << ": Error: Unrecognized token `" << yytext << "'\n";
        }
        else {
            cout << "Token: " << token << " ";
            switch(token)
            {
                case TOK_UINTNUM:
                    cout << "Attribute: (int) " << yylval.uVal;
                    break;

                case TOK_DOUBLENUM:
                    cout << "Attribute: (float) " << yylval.dVal;
                    break;

                case TOK_STRCONST:
                    cout << "Attribute: (string) " << yylval.cVal;
                    break;

                case TOK_ID:
                    cout << "Attribute: (id) " << yylval.cVal;
                    break;

                default:
                    break;
            }
            cout << endl;
        }
    }

    return 0;
#else
    //   yydebug = 1;
    any = new EventEntry("any");
    if (stm.insert(any) == OK) {
        Type *te = new Type((vector<Type*>*)NULL, Type::EVENT);
        any->type(te);
    }

    DEBUG("=================Lexical and Syntax Parsing==================\n");
    yyparse();
    if (errCount() > 0) {
        errMsg(itoa(errCount()) + " syntax error(s) reported.\nCompilation terminated.");
        return 1;
    }

    stm.leaveToScope(SymTabEntry::Kind::GLOBAL_KIND);
    GlobalEntry *ge = (GlobalEntry*)(stm.currentScope());
    if (ge != NULL) {
        DEBUG("=========================AST Builder=========================\n");
        if (debugLevel > 0) {
            ge->print(cout, 0);
        }

        DEBUG("========================Type Checking========================\n");
        ge->typeCheck();
        if (errCount() > 0) {
            errMsg(itoa(errCount()) + " error(s) reported.\nCompilation terminated.");
            return 1;
        }

        DEBUG("======================3 Addr Generation======================\n");
        InterCodesClass* in = ge->codeGen();
        if (debugLevel > 0) {
           in->print(cout);
        }

        if (optLevel) {
            DEBUG("====================== Basic Code Optimization ======================\n");
            in->ioptimize();
            if (debugLevel > 0) {
                    in->print(cout);
            }
        }
            
        DEBUG("====================Basic Block creation=====================\n");
        BasicBlocksContainer *bbC = new BasicBlocksContainer();
        bbC->createBlockStruct (in);
        if (debugLevel > 0) {
           bbC->print(cout);
        }
            
        if (optLevel) {
            DEBUG("=========================Optimization========================\n");
            bbC->optimize();
            if (debugLevel > 0) {
                DEBUG("========================= Final Optimized Code ========================\n");
                bbC->print(cout);
            }
        }
        
        DEBUG("===================Final Code generation=====================\n");
        ge->eventHandler(cout);
        FinalMachineCodeGen::finalCodeGen(bbC, cout);

        DEBUG("Compilation Successful\n");
    }
#endif

}

