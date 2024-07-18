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

#if foo("one")
Literal string in #if
#endif
