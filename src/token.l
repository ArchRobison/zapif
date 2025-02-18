%{
#include "gram.tab.hpp"
#include <stdio.h>
void growTok();
void emitCode();
void mark_if();
void stash_raw_string_dchar_seq();
int is_raw_string_terminator();
extern int cMode;
int extendedCharMode;
#if 0
#define RET(x) fprintf(stderr,"[%s]",#x); return (x);
#else
#define RET(x) return (x);
#endif
#define YY_DECL int uniflex()

static void munch();
void transition(int from, int to);
static int old_state;
static int if_tok_ret;

%}
%x preproc
%x c_comment
%x cplus_comment
%x if_tok
%x squote
%x dquote
%x raw_string
%option yylineno

SPACE ([ \t]|\\\n)*
PAREN_DEBRIS [ \t]*\([^)]*\)
HASH ^[ \t]*#[ \t]*
LEFT_SQUOTE ([LUu]|u8)?\'
IF_DELIM [^a-zA-Z0-9_]
DCHAR [!"#$%&'*+,-./0-9:;<=>?@A-Z\[\\\]^_`a-z{|}~]*
INT_SUFFIX [uU]?[lL]?[lL]?
%%

<INITIAL>{
    {HASH}ifdef[ \t]+       {BEGIN(preproc); RET(IFDEF);}
    {HASH}ifndef[ \t]+      {BEGIN(preproc); RET(IFNDEF);}
    {HASH}if/{IF_DELIM}     {if_tok_ret=IF; transition(if_tok,if_tok); mark_if();}
    {HASH}elif/{IF_DELIM}   {if_tok_ret=ELIF; transition(if_tok,if_tok); mark_if();}
    {HASH}endif             {BEGIN(preproc); RET(ENDIF);}
    {HASH}else              {BEGIN(preproc); RET(ELSE);}
    {HASH}.                 {emitCode();}

    "/*"    {transition(INITIAL,c_comment);}

    "\""    {transition(INITIAL,dquote);}

    R\"{DCHAR}\(    {stash_raw_string_dchar_seq(); transition(INITIAL,raw_string);}

    "//"    {transition(INITIAL,cplus_comment);}

    {LEFT_SQUOTE}   {transition(INITIAL,squote);}

    (.|\n)  {emitCode();}
}

<if_tok>{
   [ \t]    {munch();}
   "\\\n"   {munch();}
   "/*"     {transition(if_tok,c_comment);}
   ""/[^ ]  {BEGIN(preproc); RET(if_tok_ret);}
}

<dquote>{
    \"          {if(old_state==preproc) {BEGIN(old_state); RET(ID);}
                 else {BEGIN(old_state); munch();}}
    \\(\"|\\)   {munch();}
    .           {munch();}
}

<squote>{
    \'          {if(old_state==preproc) {BEGIN(old_state); RET(ID);}
                 else {munch(); BEGIN(old_state);}}
    \\(\'|\\)   {munch();}
    .           {munch();}
}

<raw_string>{
    \){DCHAR}\" {if (is_raw_string_terminator()) {BEGIN(old_state);}
                 munch();}
    (.|\n)      {munch();}
}

<c_comment>{
    "*/"    {BEGIN(old_state); munch();}
    (.|\n)  {munch();}
}

<cplus_comment>{
    \\\n {munch();}
    \n {BEGIN(old_state); munch();}
    . {munch();}
}

<preproc>{
    defined[ \t]*   {RET(DEFINED);}
    __has_(include|feature){PAREN_DEBRIS} {RET(PRIMARY);}
    \,{SPACE}       {RET(',');}
    "?"{SPACE}      {RET('?');}
    ":"{SPACE}      {RET(':');}
    "||"{SPACE}     {RET(LOR);}
    or{SPACE}       {RET(cMode ? ID : LOR);}
    &&{SPACE}       {RET(LAND);}
    and{SPACE}      {RET(cMode ? ID : LAND);}
    \|{SPACE}       {RET(IOR);}
    \^{SPACE}       {RET(XOR);}
    xor{SPACE}      {RET(cMode ? ID : XOR);}
    \&{SPACE}       {RET(AND);}
    !={SPACE}       {RET(NOT_EQUAL);}
    =={SPACE}       {RET(EQUAL);}
    "<="{SPACE}     {RET(LESS_OR_EQUAL);}
    ">="{SPACE}     {RET(GREATER_OR_EQUAL);}
    "<<"{SPACE}     {RET(LSHIFT);}
    ">>"{SPACE}     {RET(RSHIFT);}
    \<{SPACE}       {RET(LESS);}
    \>{SPACE}       {RET(GREATER);}
    \+{SPACE}       {RET(ADD);}
    \-{SPACE}       {RET(SUB);}
    \*{SPACE}       {RET(MUL);}
    \/{SPACE}       {RET(DIV);}
    \%{SPACE}       {RET(REM);}
    !{SPACE}        {RET(LNOT);}
    not{SPACE}      {RET(cMode ? ID : LNOT);}
    ~{SPACE}        {RET(NOT);}
    \({SPACE}       {RET(LPAREN);}
    \)              {RET(RPAREN);}
    [1-9](\'?[0-9])*{INT_SUFFIX}                    {RET(ID);} /* Decimal literal */
    0[bB][01](\'?[01])*{INT_SUFFIX}                 {RET(ID);} /* Binary literal */
    0(\'?[0-7])*{INT_SUFFIX}                        {RET(ID);} /* Octal literal */
    0[xX][0-9A-Fa-f](\'?[0-9A-Fa-f])*{INT_SUFFIX}   {RET(ID);} /* Hex literal */
    [A-Za-z_][A-Za-z_0-9]*  {RET(ID);}   /* Identifier or boolean literal */
    [A-Za-z\$\@][A-Za-z_0-9\$\@]* {
                        if (!extendedCharMode)
                        {
                            fprintf(stderr, "%d identifer with $ or @: %s\n", yylineno, yytext);
                        }
                        RET(ID);
                    }
    {LEFT_SQUOTE}   {transition(preproc,squote);}
    "\""            {transition(preproc,dquote);}
    ("//".*)?(\n|\r)? {BEGIN(INITIAL); RET(TRAIL);}
    [ \t]*"/*"      {transition(preproc,c_comment);}
    {SPACE}         {growTok();}
    <<EOF>>         {RET(TRAIL);}

    .           {
                    if(strlen(yytext)<=1)
                        fprintf(stderr,"%d unrecognized char: 0x%X\n", yylineno, yytext[0]);
                    else
                        fprintf(stderr,"%d unrecognized string: %s\n", yylineno, yytext);
                }
}

%%
void munch() {
    if(old_state==INITIAL)
        emitCode();
    else
        growTok();
}

void transition(int from, int to) {
    old_state=from;
    BEGIN(to);
    munch();
}
