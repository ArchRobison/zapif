#include <cstdio>
#include <cctype>
#include <cerrno>
#include <cassert>
#include <cstring>
#include "Chunk.h"

extern int yyparse();
extern char* yytext;
extern int yylineno;

static bool iseof;

extern "C" int yylex() {
    extern int uniflex();
    return iseof ? 0 : uniflex();
}

extern "C" int yywrap() {
    iseof = true;
    return 1;
}

static const char* fileName = nullptr;

void yyerror(const char* msg) {
    std::fprintf(stderr,"%s %d: %s at '%s'\n",fileName ? fileName : "<stdin>", yylineno, msg, yytext);
}

static char* skipId(char* s) {
    if(isalpha(*s)||*s=='_') {
        ++s;
        while(isalnum(*s)||*s=='_')
            ++s;
    }
    return s;
}

static bool printVersion = false;

static int parseOptions(int argc, char* argv[]) {
    int i;
    for( i=1; i<argc && argv[i][0]=='-'; ++i ) {
        auto a = argv[i];
        switch( a[1] ) {
            default:
                fprintf(stderr,"unknown option %s\n",a);
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
            case 'k':
                interpretConstants = true;
                break;
            case 'n':
                normalizeCond = true;
                break;
            case 'v':
                printVersion = true;
                break;
        }
    }
    return i;
}

static FILE* openFile(const char* filename) {
    FILE * f = std::fopen(filename, "r");
    if( !f ) {
        std::fprintf(stderr, "Cannot open file %s: %s\n", filename, strerror(errno));
        exit(1);
    }
    fileName = filename;
    return f;
}

int main(int argc, char* argv[]) {
    int c = parseOptions(argc,argv);
    if( printVersion )
    {
        fprintf(stderr, "zapif 1.1.0\n");
        return 0;
    }
    extern FILE* yyin;
    if( c<argc ) {
        yyin = openFile(argv[c]);
    } else {
        yyin = stdin;
    }
    return yyparse();
}
