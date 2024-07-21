#include "Chunk.h"
#include <cassert>
#include <map>
#include <cstdlib>

static std::map<int,Chunk> IntMap;
static std::map<std::string,Chunk> TextMap;
static std::map<std::string,Chunk> RhsMap;
static std::map<std::string,Value> LookupMap;
static std::map<std::pair<Value,Value>,Chunk> CatMap;

//! Try to interpret text input as integer values when applicable.
bool interpretConstants;

/** Convert following expressions *if* result of simplification:
    #if defined(X) -> #ifdef X
    #if !defined(X) -> #ifndef X */
bool normalizeCond;

const Chunk* Chunk::lookup() const {
    auto i = LookupMap.find(my_str);
    if( i==LookupMap.end() )
        return nullptr;
    else
        return i->second;
}

void createDef(const std::string& symbol, const std::string& right) {
    assert(!symbol.empty());
    LookupMap[symbol] = Chunk::make(right, Chunk::as_def);
}

void createUndef(const std::string& symbol) {
    assert(!symbol.empty());
    LookupMap[symbol] = Chunk::make(symbol, Chunk::as_undef);
}

static Chunk simplifiedMarker;

Value markSimplified(Value x) {
    if( !x->isInt() && x->my_tag!=misc ) {
        Chunk* sm = &simplifiedMarker;
        // Lazy initialization
        if( sm->my_kind==Chunk::fresh )
            sm->my_kind = Chunk::text;
        return cat(sm, x);
    }
    return x;
}

Value wasSimplified(Value x) {
    if( x->isInt() )
        return x;
    if( x->my_left==&simplifiedMarker ) {
        assert(x->my_kind==Chunk::cons);
        return x->my_right;
    }
    return nullptr;
}

int cMode;

//! Try to parse as a token having an integer value.
//! If successful, set x to the the value and return true.
//! Otherwise leave x in an unspecified state and return false.
static bool tryParseInt( const std::string& s, Chunk::integer_type& x ) {
    char *endptr;
    x = std::strtoll(s.c_str(), &endptr, 0);
    if( *endptr==0 )
        return true;
    if( !cMode ) {
        if( s == "true" ) {
            x = 1;
            return true;
        }
        if( s == "false" ) {
            x = 0;
            return true;
        }
    }
    return false;
}

bool Chunk::asInt(integer_type& x, bool relaxed) const {
    const Chunk* y;
    if( isInt() )
        y = this;
    else if( auto z = lookup() )
        // y is an undef
        y = z;
    else
        y = this;
    if( y->isInt() ) {
        x = my_value;
        return true;
    }
    return relaxed && (y->my_kind & text) && tryParseInt(y->my_str,x);
}

Value Chunk::make(integer_type x) {
    auto& i = IntMap[x];
    if( i.my_kind==Chunk::fresh ) {
        i.my_kind = Chunk::integer;
        i.my_value = x;
    }
    return &i;
}

Value Chunk::make(const std::string& t, makeKind mk) {
    auto& i = mk==as_text ? TextMap[t] : RhsMap[t];
    if( i.my_kind==Chunk::fresh ) {
        i.my_kind = Chunk::text;
        i.my_str = t;
        switch( mk ) {
            case as_text:
                if( !interpretConstants )
                    break;
                // fallthrough
            case as_def:
                if( tryParseInt(t,i.my_value) )
                    i.my_kind = Chunk::integer_text;
                break;
            case as_undef:
                i.my_value = 0;
                i.my_kind = Chunk::undef;
                break;
        }
    }
    return &i;
}

Value cat( Value x, Value y ) {
    auto& i = CatMap[std::make_pair(x,y)];
    if( i.my_kind==Chunk::fresh ) {
        i.my_kind = Chunk::cons;
        i.my_left = x;
        i.my_right = y;
    }
    return &i;
}

char leftmostChar( Value x ) {
    while( x->my_kind==Chunk::cons ) {
        char c = leftmostChar(x->my_left);
        if(c) return c;
        x = x->my_right;
    }
    if( x->my_kind & Chunk::text )
        return x->my_str.empty() ? 0 : x->my_str.front();
    // Return representative char, not actual leftmost char.
    return '0';
}

char rightmostChar( Value x ) {
    while( x->my_kind==Chunk::cons ) {
        char c = rightmostChar(x->my_right);
        if(c) return c;
        x = x->my_left;
    }
    if( x->my_kind & Chunk::text )
        return x->my_str.empty() ? 0 : x->my_str.back();
    // Return representative char, not actual rightmost char.
    return '0';
}

Value part( Value x, unsigned index ) {
    assert(index>0);
    for(;index>1;index>>=1) {
        assert( x->my_kind==Chunk::cons );
        x = index&1 ? x->my_right : x->my_left;
    }
    return x;
}

static bool annotate = false;

static void note(const char* s) {
    if( annotate ) {
        printf("%s",s);
    }
}

void print(Value x) {
    if( x->my_kind&Chunk::text ) {
        note("{text ");
        printf("%s",x->my_str.c_str());
        note("}");
    } else if( x->isInt() ) {
        note("{value ");
        printf("%lld",x->my_value);
        note("}");
    } else {
        note("{");
        switch(x->my_tag) {
            case defined_with_paren: note("defined_with_paren "); break;
            case defined_sans_paren: note("defined_sans_paren "); break;
            case lnot: note("lnot "); break;
            default: note("cons ");
        }
        assert( x->my_kind&Chunk::cons );
        print(x->my_left);
        print(x->my_right);
        note("}");
    }
}

void printWithReplacement(Value x, size_t pos, size_t len, const char* replacement) {
    assert( x->my_kind&Chunk::text );
    auto& s = x->my_str;
    assert( pos<=s.size() );
    assert( len<=pos );
    note("{repl ");
    printf("%s%s%s", s.substr(0,pos-len).c_str(), replacement, s.c_str()+pos);
    note("}");
}
