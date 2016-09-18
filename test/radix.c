/* Test that -k option preserves radix of numerals */
#if 100  &   foo
yes
#endif

#if 0x100  &   foo
yes hex
#endif

#if 0100  &   foo
yes octal
#endif
