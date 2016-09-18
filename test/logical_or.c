#if foo || one==1
definitely 
#else
never
#endif

#if one==1 || foo
definitely 
#else
never
#endif

#if foo || one==2
possibly
#else
perhaps
#endif

#if one==2 || foo
possibly
#else
perhaps
#endif

#if foo || 1
maybe
#else
could be
#endif

#if 1 || foo
maybe
#else
could be
#endif

