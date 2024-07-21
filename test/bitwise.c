#if (five & 3 ^ 3 | 8) == 10
yes 11
#else
no 11
#endif

#if 0 |  foo
yes foo
#else
no foo
#endif

#if 0 ^  bar
yes bar
#else
no bar
#endif

#if 0 &  quod
yes quod
#else
no quod
#endif
