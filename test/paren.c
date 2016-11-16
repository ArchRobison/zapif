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
#if (defined(whatever) || defined(nil))
#endif

#if (zero || A)
#endif

#if (defined(whatever) && defined(zero))
#endif

#if (one && B)
#endif

#if ((zero || C) && one)
#endif

#if (zero ? D : E)
#endif

#if (one ? F : G)
#endif

/* Should lose surrounding parentheses, but gain a separating space */
#if(G||zero)
#endif

