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

#if L'A' == 0
upper case L
#endif

#if U'A' == 0
upper case U
#endif

#if u'A' == 0
lower case u
#endif

#if u8'A' == 0
lower case u8
#endif
