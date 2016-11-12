%{
#include "gram.tab.hpp"
#include <stdio.h>
void grow_tok();
void emit_code();
void mark_elif();
#if 0
#define RET(x) fprintf(stderr,"[%s]",#x); return x;
#else
#define RET(x) return x;
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
%option yylineno

SPACE ([ \t]|\\\n)*
PAREN_DEBRIS [ \t]*\([^)]*\)
HASH ^[ \t]*#[ \t]*
LEFT_SQUOTE ([LUu]|u8)?\'
IF_DELIM [^a-zA-Z0-9_]
%%

<INITIAL>{
    {HASH}ifdef[ \t]+       {BEGIN(preproc); RET(IFDEF);}
    {HASH}ifndef[ \t]+      {BEGIN(preproc); RET(IFNDEF);}
    {HASH}if/{IF_DELIM}     {if_tok_ret=IF; transition(if_tok,if_tok);}
    {HASH}elif/{IF_DELIM}   {if_tok_ret=ELIF; transition(if_tok,if_tok); mark_elif();}
    {HASH}endif             {BEGIN(preproc); RET(ENDIF);}
    {HASH}else              {BEGIN(preproc); RET(ELSE);}
    {HASH}.                 {emit_code();}

    "/*"    {transition(INITIAL,c_comment);}

    "\""    {transition(INITIAL,dquote);}

    "//"    {transition(INITIAL,cplus_comment);}

    {LEFT_SQUOTE}   {transition(INITIAL,squote);}

    (.|\n)  {emit_code();}
}

<if_tok>{
   [ \t]    {munch();}
   "\\\n"   {munch();}
   "/*"     {transition(if_tok,c_comment);}
   ""/[^ ]  {BEGIN(preproc); RET(if_tok_ret);}
}

<dquote>{
    \"          {BEGIN(old_state); munch();}
    \\(\"|\\)   {munch();}
    .           {munch();}
}

<squote>{
    \'          {if(old_state==preproc) {BEGIN(old_state); RET(ID);}
                 else {munch(); BEGIN(old_state);}}
    \\(\'|\\)   {munch();}
    .           {munch();}
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
    &&{SPACE}       {RET(LAND);}
    \|{SPACE}       {RET(IOR);}
    \^{SPACE}       {RET(XOR);}
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
    ~{SPACE}        {RET(NOT);}
    \({SPACE}       {RET(LPAREN);}
    \)              {RET(RPAREN);}
    [A-Za-z_0-9]+   {RET(ID);}   /* Really id or numeral */
    {LEFT_SQUOTE}   {transition(preproc,squote);}
    ("//".*)?(\n|\r)? {BEGIN(INITIAL); RET(TRAIL);}
    [ \t]*"/*"      {transition(preproc,c_comment);}
    {SPACE}         {grow_tok();}
    <<EOF>>         {RET(TRAIL);}

    .           {
                    if(strlen(yytext)<=1)
                        fprintf(stderr,"%d unrecognized char: 0x%X", yylineno, yytext[0]);
                    else
                       fprintf(stderr,"%d unrecognized string: %s\n", yylineno, yytext);
                }
}

%%
void munch() {
    if(old_state==INITIAL)
        emit_code();
    else
        grow_tok();
}

void transition(int from, int to) {
    old_state=from;
    BEGIN(to);
    munch();
}
