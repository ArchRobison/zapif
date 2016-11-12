/* The 27 tests below excercise all combinations of 0/1/unknown values
   for an #if...#elif...#elif...#else combination */

    keep part 4 of 0-0-0

    keep part 3 of 0-0-1

#if whatever
    keep part 3 of 0-0-?
#else
    keep part 4 of 0-0-?
#endif
--------------------------
    keep part 2 of 0-1-0

    keep part 2 of 0-1-1

    keep part 2 of 0-1-?
--------------------------
#if whatever
    keep part 2 of 0-?-0
#else
    keep part 4 of 0-?-0
#endif

#if whatever
    keep part 2 of 0-?-1
#else 
    keep part 3 of 0-?-1
#endif

#if whatever
    keep part 2 of 0-?-?
#elif whatever
    keep part 3 of 0-?-?
#else
    keep part 4 of 0-?-?
#endif
==========================
    keep part 1 of 1-0-0

    keep part 1 of 1-0-1

    keep part 1 of 1-0-?
--------------------------
    keep part 1 of 1-1-0

    keep part 1 of 1-1-1

    keep part 1 of 1-1-?
--------------------------
    keep part 1 of 1-?-0

    keep part 1 of 1-?-1

    keep part 1 of 1-?-?
==========================
#if something
    keep part 1 of ?-0-0
#else
    keep part 4 of ?-0-0
#endif

#if something
    keep part 1 of ?-0-1
#else  /* preserve comment */
    keep part 3 of ?-0-1
#endif

#if something
    keep part 1 of ?-0-?
#elif whatever
    keep part 3 of ?-0-?
#else
    keep part 4 of ?-0-?
#endif
--------------------------
#if something
    keep part 1 of ?-1-0
#else 
    keep part 2 of ?-1-0
#endif

#if something
    keep part 1 of ?-1-1
#else 
    keep part 2 of ?-1-1
#endif

#if something
    keep part 1 of ?-1-?
#else 
    keep part 2 of ?-1-?
#endif
--------------------------
#if something
    keep part 1 of ?-?-0
#elif whatever
    keep part 2 of ?-?-0
#else
    keep part 4 of ?-?-0
#endif

#if something
    keep part 1 of ?-?-1
#elif whatever
    keep part 2 of ?-?-1
#else 
    keep part 3 of ?-?-1
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
