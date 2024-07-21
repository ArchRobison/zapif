#ifndef action_H
#define action_H

extern "C" {
    /** \defgroup Lexical Flex Actions
        These actions act on a string buffer.
        Output of the string buffer is enabled or disabled
        as preprocessor actions are intepreted by the Bison actions.
    @{*/

    /** \brief Append yytext to the current string buffer. */
    void growTok();
    /** \brief do grow_tok(), output the string buffer if enabled, and clear the buffer. */
    void emitCode();
    /** \brief Record where if or elif token is in token buffer. */
    void mark_if();
    /** \brief Record the d-char-seq for a raw string.

        Expects yytext to begin with `R"` and last character to be '('. */
    void stash_raw_string_dchar_seq();
    /** \brief True if yytext matches terminator of raw string.

        Expects yytext to have form `).*"`.
        Returns 1 if the `.*` matches the d-char-seq remembered by stash_raw_string_dchar_seq, otherwise 0. */
    int is_raw_string_terminator();
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
Value ternaryOp( Value x, Value op, Value y, Value colon, Value z );
//! Evaluate x||y.
Value lor( Value x, Value op, Value y );
//! Evaluate x&&y.
Value land( Value x, Value op, Value y );
//! Evaluate a unary operation denoted by the given char.
Value unaryArithOp( Value op, Value y, char operation );
//! Evaluate x operation y, where the operation is ADD, SUB, etc.
Value binaryOp( Value x, Value op, Value y, int operation );
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

//! Mark value as being safe to omit any enclosing parentheses.
void markPrimary(Value x);

//! Concatenate x and y.
Value cat(Value x, Value y);

//! Concatenate the values.
template<typename... T>
Value cat(Value x, Value y, T... rest) {
    return cat(x,cat(y,rest...));
}

/**@}*/

#endif /* action_H */
