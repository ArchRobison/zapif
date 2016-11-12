/* The 9 tests below excercise all combinations of 0/1/unknown values
   for an #if and #elif combination */

    keep

    retain

#if whatever
    waffle
#else
    perhaps
#endif

    hold

    display

    preserve

#if whatever
    maybe this
#else
    perhaps this
#endif

#if whatever
    hedge
#else  /* preserve this comment too */
    retain
#endif

#if whatever
    might keep
#elif whatever
    maybe keep
#else
    perhaps keep
#endif
