#ifndef action_H
#define action_H

extern "C" void grow_tok();
extern "C" void emit_code();

class Chunk;
typedef const Chunk* Value;

Value lor( Value x, Value op, Value y );
Value land( Value x, Value op, Value y );
Value unary_arith_op( Value op, Value y, char operation );
Value ternary_op( Value x, Value op, Value y, Value colon, Value z );
Value binary_op( Value x, Value op, Value y, int operation );
Value paren( Value left, Value x, Value right );
Value expand( Value symbol );
Value defined( Value op, Value left, Value x, Value right );
Value tok();
void if_( Value if_tok, Value x, Value trail );
void elif( Value elif_tok, Value x, Value trail );
void else_( Value else_tok, Value trail );
void endif( Value endif_tok, Value trail );
void ifdef(Value op, Value id, Value trail, bool isIfDef);

Value cat(Value x, Value y);

template<typename... T>
Value cat(Value x, Value y, T... rest) {
    return cat(x,cat(y,rest...));
}

#endif /* action_H */
