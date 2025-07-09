#include <cstdio>
#include <cctype>
#include <cerrno>
#include <cassert>
#include <cstring>
#include "Chunk.h"

extern int yyparse();
extern "C" char* yytext;
extern "C" int yylineno;
extern "C" int extendedCharMode;
extern "C" FILE* yyin;
static bool iseof;

extern "C" int yylex() {
    extern int uniflex();
    return iseof ? 0 : uniflex();
}

extern "C" int yywrap() {
    iseof = true;
    return 1;
}

static const char* inputFileName = nullptr;

static const char* outputFileName = nullptr;

void yyerror(const char* msg) {
    std::fprintf(stderr,"%s %d: %s at '%s'\n", inputFileName ? inputFileName : "<stdin>", yylineno, msg, yytext);
}

static bool isExtendedChar(char c) {
    return extendedCharMode && (c == '$' || c == '@');
}

static char* skipId(char* s) {
    if(isalpha(*s)||*s=='_'||isExtendedChar(*s)) {
        ++s;
        while(isalnum(*s)||*s=='_'||isExtendedChar(*s))
            ++s;
    }
    return s;
}

static bool printVersion = false;

static const char helpText[] =
"Usage: zapif [OPTIONS] [INPUTFILE]\n"
"Algebraically simplify C and C++ preprocessor conditionals, and remove\n"
"code that would never be selected by the preprocessor.\n"
"\n"
"If no input file is specified, input is `stdin`.\n"
"Output goes to `stdout` unless `-o` is specified.\n"
"\n"
"  -Dfoo=42 treat foo as having the value 42.\n"
"  -Dfoo    same as -Dfoo=1\n"
"  -Ufoo    treat foo as undefined.\n"
"  -c       assume input is C, not C++.\n"
"  -e       allow $ and @ in identifiers.\n"
"  -k       interpret numerals in preprocessor expressions. Otherwise\n"
"           they are treated as unknown values.\n"
"  -n       normalize #if defined(x) to #ifdef x if the whole expression\n"
"           was the result of simplification. Do likewise for\n"
"           #if !defined(x) and equivalent parentheses-free forms.\n"
"  -o file  use file for output instead of stdout.\n"
"  -v       print version and exit.\n"
"  --help   print thie message and exit.\n"
"\n"
"Full documentation: https://github.com/ArchRobison/zapif\n";

static int parseOptions(int argc, char* argv[]) {
    int i;
    for( i=1; i<argc && argv[i][0]=='-'; ++i ) {
        auto a = argv[i];
        switch( a[1] ) {
            case '-':
                if( strcmp(a, "--help") == 0 ) {
                    printf(helpText);
                    exit(0);
                }
                // Fall through
            default:
                fprintf(stderr, "unknown option '%s'\n"
                                "Try 'zapif --help' for more information.\n", a);
                exit(1);

            case 'U': {
                char* symbol = a+2;
                if( *skipId(symbol) ) {
                    fprintf(stderr,"option %s has garbled identifier\n",a);
                    exit(1);
                }
                createUndef(symbol);
                break;
            }
            case 'D': {
                char* symbol = a+2;
                char* rest = skipId(symbol);
                const char* def;
                switch( *rest ) {
                    default:
                        fprintf(stderr,"option %s garbled\n",a);
                        exit(1);
                    case '=':
                        *rest = 0;
                        def = rest+1;
                        break;
                    case 0:
                        def = "1";
                        break;
                }
                createDef(symbol,def);
                break;
            }
            case 'c':
                cMode = true;
                break;
            case 'e':
                extendedCharMode = true;
                break;
            case 'k':
                interpretConstants = true;
                break;
            case 'n':
                normalizeCond = true;
                break;
            case 'o':
                if( outputFileName ) {
                    fprintf(stderr,"duplicate -o option");
                    exit(1);
                }
                if( ++i >= argc ) {
                    fprintf(stderr,"filename expected after -o");
                    exit(1);
                }
                outputFileName = argv[i];
                break;
            case 'v':
                printVersion = true;
                break;
        }
    }
    return i;
}

static FILE* openFile(const char* filename, bool output) {
    FILE * f = std::fopen(filename, output ? "w" : "r");
    if( !f ) {
        std::fprintf(stderr, "Cannot open file %s: %s\n", filename, strerror(errno));
        exit(1);
    }
    if (!output)
        inputFileName = filename;
    return f;
}

int main(int argc, char* argv[]) {
    int c = parseOptions(argc,argv);
    if( printVersion )
    {
        fprintf(stderr, "zapif 1.5.0\n");
        return 0;
    }
    yyin = c<argc ? openFile(argv[c], false) : stdin;

    extern FILE* outputFile;
    outputFile = outputFileName ? openFile(outputFileName, true) : stdout;
    return yyparse();
}
