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
