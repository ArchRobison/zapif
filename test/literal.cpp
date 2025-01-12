#if 0x1234'56'789ab'cdef0 + 01'234'5'670 == 0b10'01000110100'0'10101100111100010011010111001101010101010101000
Octal and lowercase hex and binary
#endif

#if 0X1234'56'789ABCD'EF0 + 01'234'56'70 == 0B10'010001101000'1'0101100111100010011010'1110011010'10101010101000
Octal and uppercase hex and binary
#endif

#if 03l + 0xfll == 0b101u + 6ul + 7ull
Lowercase suffixes
#endif

#if 03L + 0xfLL == 0b101U + 6UL + 7ULL
Uppercase suffixes
#endif
