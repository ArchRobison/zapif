"/* inside quotes does not begin C-style comment"
#if one==1
*/
#else
discard
#endif

"next char is literal quote\"/*"
#if one==1
*/
#else
throw away
#endif

"next char is is backslash\\" /*
#if zero
keep
#endif
*/

"\\*"

"\\\*"

#ifndef nil
#define foo bar('x'), bar('"'), bar('#')
#endif

#if '\0' == zero   
retain
#else
toss
#endif
