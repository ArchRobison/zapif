#ifndef action_H
#define action_H

extern "C" {
    /** \defgroup Lexical Flex Actions
        These actions act on a string buffer.
        Output of the string buffer is enabled or disabled
        as preprocessor actions are intepreted by the Bison actions.
    @{*/

    /** \brief Append yytext to the current string buffer. */
    void grow_tok();
    /** \brief do grow_tok(), output the string buffer if enabled, and clear the buffer. */
    void emit_code();
    /** \brief Record where elif token is in token buffer. */
    void mark_elif();
    /**@}*/
}

class Chunk;
typedef const Chunk* Value;

/** \defgroup Actions Bison Actions
    Each function does a semantic action associated with the Bison grammar.
    The extra arguments such as "op", "colon", "left", "right", and "trail"
    hold the input text associated with an operator or punctuation.
    This text may include surrounding whitespace, comments, or continuation
    sequences.
@{*/

//! Evaluate x?y:z
Value ternary_op( Value x, Value op, Value y, Value colon, Value z );
//! Evaluate x||y.
Value lor( Value x, Value op, Value y );
//! Evaluate x&&y.
Value land( Value x, Value op, Value y );
//! Evaluate a unary operation denoted by the given char.
Value unary_arith_op( Value op, Value y, char operation );
//! Evaluate x operation y, where the operation is ADD, SUB, etc.
Value binary_op( Value x, Value op, Value y, int operation );
//! Evaluate (x).
Value paren( Value left, Value x, Value right );
//! Evaluate given symbol, expanding it per any -D or -U options.
Value expand( Value symbol );
//! Evaluate defined(x).
Value defined( Value op, Value left, Value x, Value right );
//! Evaluate if(x)
void if_( Value if_tok, Value x, Value trail );
//! Evaluate elif(x)
void elif( Value elif_tok, Value x, Value trail );
//! Evaluate else
void else_( Value else_tok, Value trail );
//! Evaluate endif
void endif( Value endif_tok, Value trail );
/** \brief Evaluate ifdef(id) or ifndef(id).
  * \param op: text of the ifdef or ifndef, and any trailing whitespace.
  * \param id: symbol for query
  * \param trail: whitespace/comments after the symbol
  * \param isIfDef: false for ifndef, true for ifdef.
  */
void ifdef(Value op, Value id, Value trail, bool isIfDef);
//! Get current string buffer as a text Value, and clear the buffer.
Value tok();

//! Concatenate x and y.
Value cat(Value x, Value y);

//! Concatenate the values.
template<typename... T>
Value cat(Value x, Value y, T... rest) {
    return cat(x,cat(y,rest...));
}

/**@}*/

#endif /* action_H */
