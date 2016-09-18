#if one <<   two  ==  4
left yes
#else
left no
#endif

#if two >> one == 1
right yes
#else
right no
#endif

#if one << 9 >> 5 == 16
left right yes
#else
left right no
#endif
