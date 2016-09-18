/* Test for && */

#if foo && one==1
maybe left
#else
could be left
#endif

#if one==1 && foo
maybe right
#else
could be right
#endif

#if foo && one!=1
no way right
#else
yup because opposite no way right
#endif

#if one!=1 && foo
no way left
#else
yes because opposite no way left
#endif

#if foo && 0
keep this
#else
keep also
#endif

#if 0 && foo
retain this
#else
retain also
#endif
