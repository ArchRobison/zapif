/* two-level nesting */
#ifdef nil
#ifdef nothing
should be erased
#else
should be erased too
#endif
#endif
keep this line
#ifdef nil
#if 0
should be deleted
#endif
#if 1
should also be deleted
#endif
#endif
keep this line too
#ifdef nil
#if 0
should be scrubbed
#elif nothing
should be scrubbed too
#else
should be scrubbed also
#endif
#endif

/* three-level nesting */
#if zero
    0
    #if zero
        00
        #if zero
            000
        #else
            001
        #endif
    #else
        01
        #if zero
            010
        #else
            011
        #endif
    #endif
#else
    1
    #if zero
        10
        #if zero
            100
        #else
            101
        #endif
    #else
        11
        #if zero
            110
        #else
            111
        #endif
    #endif
#endif
