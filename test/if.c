#if one
yes one
#else
no one
#endif

#if(zero)
one in parentheses
#elif(two)
two in parentheses
#else
neither one or two in parentheses
#endif

#if+one
yes one with plus
#else
no one with plus
#endif

#if-one
yes one with minus
#else
no one with minus
#endif

/* should ignore if or elif with trailing letters or numbers */
#iff 1
#eliff 1
#if1
#elif2
