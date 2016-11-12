/* The 9 tests below excercise all combinations of 0/1/unknown values
   for an #if and #elif combination */

#if zero
    discard 
#elif zero
    delete
#else
    keep
#endif

#if zero
    eradicate
#elif one
    retain
#else
    squash
#endif

#if zero
    remove
#elif whatever
    waffle
#else
    perhaps
#endif

#if one
    hold
#elif zero
    delete
#else
    erase
#endif

#if one
    display
#elif one
    vaporize 
#else
    discard
#endif

#if one
    preserve
#elif whatever
    reject
#else
    toss
#endif

#if whatever
    maybe this
#elif zero
    delete
#else
    perhaps this
#endif

#if whatever
    hedge
#elif one /* preserve this comment too */
    retain
#else
    trash
#endif

#if whatever
    might keep
#elif whatever
    maybe keep
#else
    perhaps keep
#endif
