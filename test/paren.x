/* Tests below should retain surrounding parentheses,  
   because the enclosed expression is not simplified. */
#if (defined(whatever))
#endif

#if (A)
#endif

#if ((A))
#endif

#if (A||B)
#endif

#if (A&&B)
#endif

/* Tests below should lose surrounding parentheses,  
   because the enclosed expression was simplified. */
#if defined(whatever)
#endif

#if A
#endif

#if defined(whatever)
#endif

#if B
#endif

#if C
#endif

#if E
#endif

#if F
#endif

/* Should lose surrounding parentheses, but gain a separating space */
#if G
#endif

