yes one

two in parentheses

yes one with plus

yes one with minus

/* should ignore if or elif with trailing letters or numbers */
#iff 1
#eliff 1
#if1
#elif2

#if/*'*/foo
conditional on foo
#endif

#if/*"*//*'*/foo
conditional on foo again
#endif 

#if /*"*/ /*'*/ foo
conditional on foo yet again
#endif

#if\
/*"*/foo
conditional on foo on new line
#endif
