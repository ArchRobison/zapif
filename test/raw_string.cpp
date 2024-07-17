#if one == 1
"ab\"cd\"ef"
#endif

R"(
#if one == 1
"gh\"ij\"kl"
#endif
)";

R"(
#if one == 2
"mn\"op\"qr"
#endif
)";

#if one == 2
"st\"uv\"wx"
#endif
