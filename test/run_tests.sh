#!/bin/bash
ZAPIF=../build/zapif
TEST_SRC=../test
TMP=../build/tmp.out

# Test that zapif with no arguments is an identity transform.
for i in $TEST_SRC/*.c $TEST_SRC/*.cpp
do
    $ZAPIF $i > "$TMP"
    diff -U 5 "$TMP" $i
done

# Check tests that involve replacements
for i in $TEST_SRC/*.x $TEST_SRC/*.xpp
do
    input=${i/.x/.c}
    $ZAPIF -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing $input > $TMP
    diff -U 5 "$i" "$TMP"
done

# Check tests that involve replacments and interpreting literals (-k) 
for i in $TEST_SRC/*.k
do
    input=${i/.k/.c}
    $ZAPIF -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing -k $input > $TMP
    diff -U 5 "$i" "$TMP"
done

# Check tests that involve replacments and normalization (-n) 
for i in $TEST_SRC/*.n
do
    input=${i/.n/.c}
    $ZAPIF -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing -n $input > $TMP
    diff -U 5 "$i" "$TMP"
done
