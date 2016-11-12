/* The 9 tests below excercise all combinations of 0/1/unknown values
   for an #if and #elif combination */

/* 0-0 */
#if zero
    discard 
#elif zero
    delete
#endif

/* 0-1 */
#if zero
    eradicate
#elif one
    retain
#endif

/* 0-? */
#if zero
    remove
#elif whatever
    waffle
#endif

/* 1-0 */
#if one
    hold
#elif zero
    delete
#endif

/* 1-1 */
#if one
    display
#elif one
    vaporize 
#endif

/* 1-? */
#if one
    preserve
#elif whatever
    reject
#endif

/* ?-0 */
#if whatever
    maybe this
#elif zero
    delete
#endif

/* ?-1 */
#if whatever
    hedge
#elif one /* preserve this comment too */
    retain
#endif

/* ?-? */
#if whatever
    might keep
#elif whatever
    maybe keep
#endif
