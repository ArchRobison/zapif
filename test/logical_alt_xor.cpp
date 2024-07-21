#if two xor five
Retain
#else
Elide
#endif

#if five xor five
Delete
#else
Keep
#endif

#if zero xor unknown
Yes
#else
No
#endif

#if mystery 	 xor  	unknown
Condition has extra tabs and spaces.
#endif
