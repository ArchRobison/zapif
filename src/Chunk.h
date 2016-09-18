#include <string>
#include <cassert>
#include "action.h"

class Chunk {
    enum kind_flags {
        fresh=0,
        integer=1,      // my_value is valid
        text=2,         // my_str is valid
        integer_text=3,
        undef=7,        // Like integer_text, but is undef
        cons=8          // my_left and my_right are valid    
    };
    kind_flags my_kind;
    long my_value;      
    std::string my_str; 
    Value my_left;      
    Value my_right;
public:
    typedef long integer_type;
    bool isInt() const {return (my_kind&integer)!=0;}
    bool asInt(integer_type& value, bool relaxed) const;
    bool isUndef() const {return my_kind==undef;}
    bool operator==(int y) const {return isInt() && my_value==y;}
    bool operator!=(int y) const {return isInt() && my_value!=y;}
    const Chunk* lookup() const;
    enum makeKind {
        as_def,
        as_undef,
        as_text
    };
    static Value make(int);
    static Value make(const std::string& t, makeKind mk);
    friend void createDef(const std::string& symbol, const std::string& value);
    friend void createUndef(const std::string& symbol);
    friend Value cat( Value x, Value y );
    friend void print(Value x); 
};

void createDef(const std::string& symbol, const std::string& value);
void createUndef(const std::string& symbol);

void print(Value x);

template<typename... T>
void print(Value x, T... rest) {
    print(x);
    print(rest...);
}

extern bool interpretConstants;
