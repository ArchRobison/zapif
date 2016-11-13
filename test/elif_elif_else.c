/* The 27 tests below excercise all combinations of 0/1/unknown values
   for an #if...#elif...#elif...#else combination */

#if zero
    delete part 1 of 0-0-0
#elif zero
    delete part 2 of 0-0-0
#elif zero
    delete part 3 of 0-0-0
#else
    keep part 4 of 0-0-0
#endif

#if zero
    delete part 1 of 0-0-1
#elif zero
    delete part 2 of 0-0-1
#elif one
    keep part 3 of 0-0-1
#else
    delete part 4 of 0-0-1
#endif

#if zero
    delete part 1 of 0-0-?
#elif zero
    delete part 2 of 0-0-?
#elif whatever
    keep part 3 of 0-0-?
#else
    keep part 4 of 0-0-?
#endif
--------------------------
#if zero
    delete part 1 of 0-1-0
#elif one
    keep part 2 of 0-1-0
#elif zero
    delete part 3 of 0-1-0
#else
    keep part 4 of 0-1-0
#endif

#if zero
    delete part 1 of 0-1-1
#elif one
    keep part 2 of 0-1-1
#elif one
    delete part 3 of 0-1-1
#else
    delete part 4 of 0-1-1
#endif

#if zero
    delete part 1 of 0-1-?
#elif one
    keep part 2 of 0-1-?
#elif whatever
    delete part 3 of 0-1-?
#else
    delete part 4 of 0-1-?
#endif
--------------------------
#if zero
    delete part 1 of 0-?-0
#elif whatever
    keep part 2 of 0-?-0
#elif zero
    delete part 3 of 0-?-0
#else
    keep part 4 of 0-?-0
#endif

#if zero
    delete part 1 of 0-?-1
#elif whatever
    keep part 2 of 0-?-1
#elif one
    keep part 3 of 0-?-1
#else
    delete part 4 of 0-?-1
#endif

#if zero
    delete part 1 of 0-?-?
#elif whatever
    keep part 2 of 0-?-?
#elif whatever
    keep part 3 of 0-?-?
#else
    keep part 4 of 0-?-?
#endif
==========================
#if one
    keep part 1 of 1-0-0
#elif zero
    delete part 2 of 1-0-0
#elif zero
    delete part 3 of 1-0-0
#else
    delete part 4 of 1-0-0
#endif

#if one
    keep part 1 of 1-0-1
#elif zero
    delete part 2 of 1-0-1
#elif one
    delete part 3 of 1-0-1
#else
    delete part 4 of 1-0-1
#endif

#if one
    keep part 1 of 1-0-?
#elif zero
    delete part 2 of 1-0-?
#elif whatever
    delete part 3 of 1-0-?
#else
    delete part 4 of 1-0-?
#endif
--------------------------
#if one
    keep part 1 of 1-1-0
#elif one
    delete part 2 of 1-1-0
#elif zero
    delete part 3 of 1-1-0
#else
    delete part 4 of 1-1-0
#endif

#if one
    keep part 1 of 1-1-1
#elif one
    delete part 2 of 1-1-1
#elif one
    delete part 3 of 1-1-1
#else
    delete part 4 of 1-1-1
#endif

#if one
    keep part 1 of 1-1-?
#elif one
    delete part 2 of 1-1-?
#elif whatever
    delete part 3 of 1-1-?
#else
    delete part 4 of 1-1-?
#endif
--------------------------
#if one
    keep part 1 of 1-?-0
#elif whatever
    delete part 2 of 1-?-0
#elif zero
    delete part 3 of 1-?-0
#else
    delete part 4 of 1-?-0
#endif

#if one
    keep part 1 of 1-?-1
#elif whatever
    delete part 2 of 1-?-1
#elif one
    delete part 3 of 1-?-1
#else
    delete part 4 of 1-?-1
#endif

#if one
    keep part 1 of 1-?-?
#elif whatever
    delete part 2 of 1-?-?
#elif whatever
    delete part 3 of 1-?-?
#else
    delete part 4 of 1-?-?
#endif
==========================
#if something
    keep part 1 of ?-0-0
#elif zero
    delete part 2 of ?-0-0
#elif zero
    delete part 3 of ?-0-0
#else
    keep part 4 of ?-0-0
#endif

#if something
    keep part 1 of ?-0-1
#elif zero
    delete part 2 of ?-0-1
#elif one /* preserve comment */
    keep part 3 of ?-0-1
#else
    delete part 4 of ?-0-1
#endif

#if something
    keep part 1 of ?-0-?
#elif zero
    delete part 2 of ?-0-?
#elif whatever
    keep part 3 of ?-0-?
#else
    keep part 4 of ?-0-?
#endif
--------------------------
#if something
    keep part 1 of ?-1-0
#elif one
    keep part 2 of ?-1-0
#elif zero
    delete part 3 of ?-1-0
#else
    delete part 4 of ?-1-0
#endif

#if something
    keep part 1 of ?-1-1
#elif one
    keep part 2 of ?-1-1
#elif one
    delete part 3 of ?-1-1
#else
    delete part 4 of ?-1-1
#endif

#if something
    keep part 1 of ?-1-?
#elif one
    keep part 2 of ?-1-?
#elif whatever
    delete part 3 of ?-1-?
#else
    delete part 4 of ?-1-?
#endif
--------------------------
#if something
    keep part 1 of ?-?-0
#elif whatever
    keep part 2 of ?-?-0
#elif zero
    delete part 3 of ?-?-0
#else
    keep part 4 of ?-?-0
#endif

#if something
    keep part 1 of ?-?-1
#elif whatever
    keep part 2 of ?-?-1
#elif one
    keep part 3 of ?-?-1
#else
    delete part 4 of ?-?-1
#endif

#if something
    keep part 1 of ?-?-?
#elif whatever
    keep part 2 of ?-?-?
#elif unknown
    keep part 3 of ?-?-?
#else
    keep part 4 of ?-?-?
#endif
==========================
