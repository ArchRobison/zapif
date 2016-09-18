#include "Chunk.h"
#include <cassert>
#include <map>
#include <cstdlib>

static std::map<int,Chunk> IntMap;
static std::map<std::string,Chunk> TextMap;
static std::map<std::string,Chunk> RhsMap;
static std::map<std::string,Value> LookupMap;
static std::map<std::pair<Value,Value>,Chunk> CatMap;

bool interpretConstants;

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

static bool tryParseInt( const std::string& s, Chunk::integer_type& x ) {
    char *endptr;
    x = std::strtoll(s.c_str(), &endptr, 0);
    return *endptr==0;
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
 
Value Chunk::make(int x) {
    auto& i = IntMap[x]; if( i.my_kind==Chunk::fresh ) {
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
        note("{cons ");
        print(x->my_left);
        print(x->my_right);
        note("}");
    }
}

