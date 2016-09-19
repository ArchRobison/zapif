"/* inside quotes does not begin C-style comment"
*/

"next char is literal quote\"/*"
*/

"next char is is backslash\\" /*
#if zero
keep
#endif
*/

"\\*"

"\\\*"

#define foo bar('x'), bar('"'), bar('#')

#if '\0' == 0   
retain
#else
toss
#endif
