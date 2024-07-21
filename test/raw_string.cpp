#if one == 1
"ab\"cd\"ef"
#endif

R"(
#if one == 1
"gh\"ij\"kl"
#endif
)";

R"!#%^&*_+-={}[](
#if one == 2
"mn\"op\"qr"
)!#%^&*_+-={}[]
)!#%^&*_+-={}["
)#%^&*_+-={}[]"
#endif
)!#%^&*_+-={}[]";

#if one == 2
"st\"uv\"wx"
#endif

R""'049ANZanz(
#if one == 3
""
#endif
)"'049ANZanz";

#if one == 3
""
#endif

LR":';<>?,./("
#if one == 4
""
#endif
):';<>?,./";

#if one == 4
""
#endif
