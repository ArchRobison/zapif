#if one // C++ style comment 
yes
#else
no
#endif

#if one
// foo '\\', '\t', '\n', '"', bar
#endif

#if one
// really '\\' \
   long comment
line above is in comment, but not this line
#endif

// next line is continuation line \
#if one
but not this line
