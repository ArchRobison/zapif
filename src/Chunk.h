#include <string>
#include <cassert>
#include "action.h"

/** \brief A pair of bits dnoting which part of if-else to keep.

    Meaning of values:
    0 - keep neither
    1 - keep "then"
    2 - keep "else" and supress "#else"
    5 - keep "then" ana emit "#endif"
    6 - keep "else" and emit "#else"
    7 - keep both
  */
typedef unsigned char bit_triple_type;

//! Tags for Chunks with certain syntactic forms.
enum Tag {
    misc,
    defined_with_paren,
    defined_sans_paren,
    lnot,
    primary
};

/** \brief A chunk of text.

    The representation may be string, number, both,
    or the concatentation of two chunks.  When represented as
    both a string and a number, the string exists so that the
    original input text form can be copied to the output. */
class Chunk {
public:
    //! Signed integer type used for value of numerals.
    typedef long long integer_type;
    //! Has integer value
    bool isInt() const {return (my_kind&integer)!=0;}
    /** \brief Return true iff *this has an integer value.
        \param value: set to value of *this if it has a value.
        \param relaxed: if true and text is a numeral, consider it to have a value. */
    bool asInt(integer_type& value, bool relaxed) const;
    //! True if *this is forcibly undefined via -U option.
    bool isUndef() const {return my_kind==undef;}
    //! Return true iff *this has integer value equal to y.
    bool operator==(int y) const {return isInt() && my_value==y;}
    //! Return false iff *this has integer value equal to y.
    bool operator!=(int y) const {return isInt() && my_value!=y;}
    //! Return triple of bits indicating whether *this is ==0, !=0.
    bit_triple_type bit_triple() const {return isInt() ? my_value!=0 ? 1 : 2 : 7;}
    //! If *this is defined (via -D or -U), return its definition.
    const Chunk* lookup() const;
    //! Make a pure integer, i.e. Value without a text representation.
    static Value make(integer_type);
    //! Options for making Value from a string.
    enum makeKind {
        as_def,     //!< string is being defined via -D
        as_undef,   //!< string is being undefined via -U
        as_text     //!< string is file input text
    };
    /** \brief Make a Value from its string representation.
      *
      * \param t: string representation of the value
      * \param mk: context
      */
    static Value make(const std::string& t, makeKind mk);
    Value setTag(Tag k) const {my_tag = k; return this;}
    bool hasTag(Tag k) const {return my_tag==k;}
    void setIsPrimary() const {
        if(my_tag==misc)
            my_tag=primary;
    }
    friend void createDef(const std::string& symbol, const std::string& value);
    friend void createUndef(const std::string& symbol);
    friend Value cat( Value x, Value y );
    friend void print(Value x);
    friend void printWithReplacement(Value x, size_t pos, size_t len, const char* replacement);
    friend Value markSimplified(Value x);
    friend Value wasSimplified(Value x);
    friend char leftmostChar(Value x);
    friend char rightmostChar(Value x);

    /** \brief Get value from tree, using index scheme shown below.
      *       1
      *     /   \
      *    2     3
      *   / \   / \
      *  4   6 5   7
      * /|  /| |\  |\
      *8 C A E 9 D B F
      */
    friend Value part(Value x, unsigned index);
private:
    enum kind_flags: char {
        fresh=0,
        integer=1,      //!< my_value is valid
        text=2,         //!< my_str is valid
        integer_text=3,
        undef=7,        //!< Like integer_text, but is undef, i.e. from -U option.
        cons=8          //!< my_left and my_right are valid
    };
    kind_flags my_kind;
    mutable Tag my_tag = misc;
    integer_type my_value;
    std::string my_str;
    Value my_left;
    Value my_right;
};

//! Create a def that defines symbol as value.
void createDef(const std::string& symbol, const std::string& value);

//! Create an undef for the given symbol.
void createUndef(const std::string& symbol);

//! Print x in verbose form.  Intended for debugging.
void print(Value x);

//! Invoke print(y) for each argument y.
template<typename... T>
void print(Value x, T... rest) {
    print(x);
    print(rest...);
}

extern bool interpretConstants;
extern bool normalizeCond;
extern int cMode;
