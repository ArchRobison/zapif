/* Test for && */

#if foo
maybe left
#else
could be left
#endif

#if foo
maybe right
#else
could be right
#endif

yup because opposite no way right

yes because opposite no way left

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
