/* The 9 tests below excercise all combinations of 0/1/unknown values
   for an #if and #elif combination */

/* 0-0 */

/* 0-1 */
    retain

/* 0-? */
#if whatever
    waffle
#endif

/* 1-0 */
    hold

/* 1-1 */
    display

/* 1-? */
    preserve

/* ?-0 */
#if whatever
    maybe this
#endif

/* ?-1 */
#if whatever
    hedge
#else  /* preserve this comment too */
    retain
#endif

/* ?-? */
#if whatever
    might keep
#elif whatever
    maybe keep
#endif
