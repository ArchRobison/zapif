#include "action.h"
#include "Chunk.h"
#include "build/gram.tab.hpp"
#include <vector>
#include <cstdio>

extern char* yytext;

void markPrimary( Value x ) {
    x->setIsPrimary();
}

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
    if( Value y = wasSimplified(x) )
        return y;
    // Retain all text
    return cat(left,x,right);
}

Value unaryArithOp(Value op, Value x, char operation) {
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
    if( *x==0 ) return markSimplified(y);
    if( *y==0 ) return markSimplified(x);
    if( *x!=0 || *y!=0 ) return Chunk::make(1);
    return cat(x,op,y);
}

Value land(Value x, Value op, Value y) {
    if( *x!=0 ) return markSimplified(y);
    if( *y!=0 ) return markSimplified(x);
    if( *x==0 || *y==0 ) return Chunk::make(0);
    return cat(x,op,y);
}

static bool asInts(Value x, Value y, Chunk::integer_type& i, Chunk::integer_type& j ) {
    if( x->isInt() || y->isInt() ) {
        return x->asInt(i,true) && y->asInt(j,true);
    }
    return false;
}

Value binaryOp( Value x, Value op, Value y, int operation ) {
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

Value ternaryOp( Value x, Value op, Value y, Value colon, Value z ) {
    if( *x!=0 ) return markSimplified(y);
    if( *x==0 ) return markSimplified(z);
    return cat(x,op,y,colon,z);
}

// See http://stackoverflow.com/questions/14418560 for why yymore() cannot be used.
static std::string buffer;

void growTok() {
    buffer += yytext;
}

Value tok() {
    growTok();
    Value x = Chunk::make(buffer, Chunk::as_text);
    buffer.clear();
    return x;
}

static size_t elif_pos;

void mark_elif() {
    elif_pos = buffer.size()-4;
    assert(buffer.substr(elif_pos)=="elif");
}

static std::vector<bit_triple_type> enable {3};

static bool push_level(Value x) {
    bit_triple_type outer = enable.back();
    enable.push_back(outer & 1 ? x->bit_triple() : 0);
    return enable.back()==7;
}

void if_( Value if_tok, Value x, Value trail ) {
    if( push_level(x) )
        print(if_tok,x,trail);
}

void ifdef(Value op, Value id, Value trail, bool isIfDef) {
    Value x = id;
    if( const Chunk* def = x->lookup() )
        x = Chunk::make(def->isUndef() != isIfDef);
    if( push_level(x) )
        print(op,id,trail);
}

void elif( Value elif, Value x, Value trail ) {
    bit_triple_type yp = enable.back();
    if( yp & 2 ) {
        bit_triple_type xp = x->bit_triple();
        enable.back() = xp | (yp&4);
        if( xp==7 ) {
            if( (yp&6)==6 ) {
                print(elif, x, trail);
            } else if( yp==2 ) {
                printWithReplacement(elif, elif_pos, 4, "if");
                print(x,trail);
            }
        } else if( xp==1 && (yp&6)==6 ) {
            printWithReplacement(elif, elif_pos, 4, "else");
            print(trail);
        }
    } else {
        enable.back() = yp & ~3;
    }
}

void else_( Value else_tok, Value trail ) {
    bit_triple_type xp = enable.back();
    if( (xp&6)==6 )
        print(else_tok, trail);
    enable.back() = (xp&4) | (xp&3)>>1;
}

void endif( Value endif_tok, Value trail ) {
    bit_triple_type x = enable.back();
    enable.pop_back();
    if( x&4 )
        print(endif_tok, trail);
}

void emitCode() {
    growTok();
    if( enable.back()&1 )
        printf("%s",buffer.c_str());
    buffer.clear();
}
