#include "action.h"
#include "Chunk.h"
#include "build/gram.tab.hpp"
#include <vector>
#include <cstdio>

extern char* yytext;

Value expand( Value x ) {
    if( const Chunk* def = x->lookup() ) 
        return def;
    return x;
}

// Evaluate defined(x) or defined x
// Here op="defined", left="(", right=")" with neighboring whitespace.
Value defined( Value op, Value left, Value x, Value right ) {
    assert(!left==!right);
    if( const Chunk* def = x->lookup() ) 
        return Chunk::make(!def->isUndef());
    else if( left )
        // Have parentheses
        return cat(op,left,x,right);
    else
        // No parentheses
        return cat(op,x);
}

Value paren( Value left, Value x, Value right ) {
    if( x->isInt() ) 
        // Propagate the value
        return x;
    // Retain all text
    return cat(left,x,right);
}

Value unary_arith_op(Value op, Value x, char operation) {
    Chunk::integer_type i;
    if( x->asInt(i,false) ) {
        Chunk::integer_type k;
        switch( operation ) {
            default: assert(0);
            case '!': k = i==0; break;
            case '-': k = -i; break;
            case '+': k = +i; break;
            case '~': k = ~i; break;
        }
        return Chunk::make(k);
    }
    return cat(op,x);
}

Value lor(Value x, Value op, Value y) {
    if( *x==0 ) return y;
    if( *y==0 ) return x;
    if( *x!=0 || *y!=0 ) return Chunk::make(1);
    return cat(x,op,y);
}

Value land(Value x, Value op, Value y) {
    if( *x!=0 ) return y;
    if( *y!=0 ) return x;
    if( *x==0 || *y==0 ) return Chunk::make(0);
    return cat(x,op,y);
}

static bool asInts(Value x, Value y, Chunk::integer_type& i, Chunk::integer_type& j ) {
    if( x->isInt() || y->isInt() ) {
        return x->asInt(i,true) && y->asInt(j,true);
    }
    return false;
}

Value binary_op( Value x, Value op, Value y, int operation ) {
    // Do algebraic simplifications.  Though there are many possible,
    // the code here handles only ones which seem to have practical
    // use in real header files.
    switch(operation) {
        case IOR:
            if( *x==0 ) return y;
            if( *y==0 ) return x;
            break;
    }
    Chunk::integer_type i, j;
    if( asInts(x,y,i,j) ) {
        Chunk::integer_type k;
        switch(operation) {
            default: assert(false);
            case IOR: k = i|j; break;
            case XOR: k = i^j; break;
            case AND: k = i&j; break;
            case EQUAL: k = i==j; break;
            case NOT_EQUAL: k = i!=j; break;
            case LESS: k = i<j; break;
            case GREATER: k = i>j; break;
            case LESS_OR_EQUAL: k = i<=j; break;
            case GREATER_OR_EQUAL: k = i>=j; break;
            case LSHIFT: k = i<<j; break;
            case RSHIFT: k = i>>j; break;
            case ADD: k = i+j; break;
            case SUB: k = i-j; break;
            case MUL: k = i*j; break;
            case DIV: if( j==0 ) goto concat; k = i/j; break;
            case REM: if( j==0 ) goto concat; k = i%j; break;
        }
        return Chunk::make(k);
    }
concat:
    return cat(x,op,y);
}

Value ternary_op( Value x, Value op, Value y, Value colon, Value z ) {
    if( *x!=0 ) return y;
    if( *x==0 ) return z;
    return cat(x,op,y,colon,z);
}

// See http://stackoverflow.com/questions/14418560 for why yymore() cannot be used.
static std::string buffer;

void grow_tok() {
    buffer += yytext; 
}

Value tok() {
    grow_tok();
    Value x = Chunk::make(buffer, Chunk::as_text);
    buffer.clear();
    return x;
}

static std::vector<Value> enable;

void if_( Value if_tok, Value x, Value trail ) {
    enable.push_back(x);
    if( !x->isInt() ) 
        print(if_tok,x,trail);
}

void ifdef(Value op, Value id, Value trail, bool isIfDef) {
    Value x = id;
    if( const Chunk* def = x->lookup() ) 
        x = Chunk::make(def->isUndef() != isIfDef);
    enable.push_back(x);
    if( !x->isInt() )
        print(op,id,trail);
}

void elif( Value elif, Value x, Value trail ) {
    enable.back() = x;
    if( !x->isInt() )
        print(elif,x,trail);
}

void else_( Value else_tok, Value trail ) {
    Value x = enable.back();
    if( x->isInt() )
        enable.back() = Chunk::make(*x==0);
    else
        print(else_tok, trail);
}

void endif( Value endif_tok, Value trail ) {
    Value x = enable.back();
    enable.pop_back();
    if( !x->isInt() ) 
        print(endif_tok, trail);
}

void emit_code() {
    grow_tok();
    if( enable.empty() || !(*enable.back()==0))
        printf("%s",buffer.c_str());
    buffer.clear();
}
