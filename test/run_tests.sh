#!/bin/bash
ZAPIF=../build/zapif
TEST_SRC=../test
TMP=../build/tmp.out
VERSION=../version.txt

# Test that zapif with no arguments is an identity transform for C++.
for i in $TEST_SRC/*.cpp
do
    $ZAPIF $i > "$TMP"
    diff -U 5 "$TMP" $i
done

# Test that zapif with only -c argument is an identity transform for C.
for i in $TEST_SRC/*.c
do
    $ZAPIF -c $i > "$TMP"
    diff -U 5 "$TMP" $i
done

# Check C++ tests that involve replacements and try -o option.
for i in $TEST_SRC/*.xpp
do
    input=${i/.x/.c}
    $ZAPIF -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing -o $TMP $input
    diff -U 5 "$i" "$TMP"
done

# Check C tests that involve replacements (-c)
for i in $TEST_SRC/*.x
do
    input=${i/.x/.c}
    $ZAPIF -c -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing $input > $TMP
    diff -U 5 "$i" "$TMP"
done

# Check C++ tests that involve replacments and interpreting literals (-k)
for i in $TEST_SRC/*.kpp
do
    input=${i/.k/.c}
    $ZAPIF -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing -k $input > $TMP
    diff -U 5 "$i" "$TMP"
done

# Check C tests that involve replacments and interpreting literals (-c -k)
# and try -o option.
for i in $TEST_SRC/*.k
do
    input=${i/.k/.c}
    $ZAPIF -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing -o $TMP -c -k $input
    diff -U 5 "$i" "$TMP"
done

# Check tests that involve replacments and normalization (-n)
for i in $TEST_SRC/*.n
do
    input=${i/.n/.c}
    $ZAPIF -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing -n $input > $TMP
    diff -U 5 "$i" "$TMP"
done

# Check tests that involve extended character set
for i in $TEST_SRC/*.e
do
    input=${i/.e/.f}
    $ZAPIF -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing -e '-D$three=3' '-D@four=4' -n $input > $TMP
    diff -U 5 "$i" "$TMP"
done

# Check that -v option works and is consistent with content of version.txt.
$ZAPIF -v >$TMP
echo zapif `cat "$VERSION"` | diff -U 5 - "$TMP"
