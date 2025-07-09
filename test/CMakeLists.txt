enable_testing()

set(ZAPIF_TEST_SOURCE_FILES
    0.c
    0.k
    0.x
    args.c
    bitwise.c
    bitwise.k
    bitwise.x
    blank.c
    c++_c_comment.cpp
    comment_round_if.c
    cplus_comment.cpp
    cplus_comment.xpp
    def_empty.c
    defined.c
    defined.x
    elif.c
    elif.x
    elif_elif.c
    elif_elif.x
    elif_elif_else.c
    elif_elif_else.x
    elif_else.c
    elif_else.x
    extended.e
    # extended.f not actually a test input file!
    if.c
    if.x
    ifdef.c
    ifdef.x
    ifndef.c
    ifndef.x
    ifndef_cr.c
    less.c
    less.x
    literal.cpp
    literal.kpp
    logical_alt_and.c
    logical_alt_and.cpp
    logical_alt_and.x
    logical_alt_and.xpp
    logical_alt_not.c
    logical_alt_not.cpp
    logical_alt_not.x
    logical_alt_not.xpp
    logical_alt_or.c
    logical_alt_or.cpp
    logical_alt_or.x
    logical_alt_or.xpp
    logical_alt_xor.c
    logical_alt_xor.cpp
    logical_alt_xor.x
    logical_alt_xor.xpp
    logical_and.c
    logical_and.x
    logical_and_or.c
    logical_and_or.x
    logical_not.c
    logical_not.k
    logical_not.x
    logical_or.c
    logical_or.x
    mul_div_rem.c
    mul_div_rem.x
    nest.c
    nest.x
    nonewline.c
    normalize.c
    normalize.n
    paren.c
    paren.x
    quote.c
    quote.x
    radix.c
    radix.k
    raw_string.cpp
    raw_string.xpp
    shift.c
    shift.x
    stringify.c
    ternary.c
    ternary.x
    tightcomment.c
    true_false.c
    true_false.cpp
    true_false.kpp
)

# Setup the zapif test cases based on the input files.
# Based on the extension of the input file, we engage a different type of test case.
# For ".c" or ".cpp" files, we run a simple no-arg test.
# For ".x" or ".xpp" files, we run replacement.
# For ".k" or ".kpp" files, we run replacement and interpret literals.
# For ".n" files, we run normalization.
# For ".e" files, we run tests to validate extended character sets.
# For most tests, the corresponding ".c" or ".cpp" file with the same name is used as
# the input, with the test case being the expected output.
foreach(test_file IN LISTS ZAPIF_TEST_SOURCE_FILES)
    cmake_path(GET test_file STEM test_filename)
    cmake_path(GET test_file EXTENSION test_extension)
    # The test case file itself is always the expected output.
    set(expected_file "${CMAKE_CURRENT_LIST_DIR}/${test_file}")
    # The input file is the corresponding ".c" or ".cpp" file.
    string(REGEX MATCH "\.[a-z]pp$" is_cpp_test ${test_extension})

    if(is_cpp_test)
        set(input_file "${CMAKE_CURRENT_LIST_DIR}/${test_filename}.cpp")
    else()
        set(input_file "${CMAKE_CURRENT_LIST_DIR}/${test_filename}.c")
    endif()
    # And then the output file is always the test case filename + .out
    # This is a _bit_ disingenuous, since the test case file isn't the one being processed
    set(output_file "${test_file}.out")

    # Next, we start building the args for this test case.
    set(ZAPIF_ARGS "")

    # C tests require the -C flag to engage the C parser instead of the CPP parser.
    if(NOT is_cpp_test)
        list(APPEND ZAPIF_ARGS "-c")
    endif()

    # Figure out the test type and the resulting zapif args based on the file extension.
    if(test_extension STREQUAL ".c" OR test_extension STREQUAL ".cpp")
        set(test_type "no_op")
    elseif(test_extension STREQUAL ".x" OR test_extension STREQUAL ".xpp")
        set(test_type "replacement")
        list(APPEND ZAPIF_ARGS
            -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing
        )
    elseif(test_extension STREQUAL ".k" OR test_extension STREQUAL ".kpp")
        set(test_type "replacement")
        list(APPEND ZAPIF_ARGS
            -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing
            -k
        )
    elseif(test_extension STREQUAL ".n")
        set(test_type "normalization")
        list(APPEND ZAPIF_ARGS
            -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing
            -n
        )
    elseif(test_extension STREQUAL ".e")
        set(test_type "extended_charset")
        # Extended charset tests actually use the corresponding ".f" file as input.
        set(input_file "${CMAKE_CURRENT_LIST_DIR}/${test_filename}.f")
        list(APPEND ZAPIF_ARGS
            -Dzero=0 -Done=1 -Dtwo=2 -Dfive=5 -Unil -Unothing
            -e -D$three=3 -D@four=4
        )
    else()
        message(FATAL_ERROR "Unknown test extension type: ${test_file}")
    endif()

    # And finally, generate the actual test.
    set(test_name ${test_type}-${test_file})
    add_test(
        NAME ${test_name}
        COMMAND
            $<TARGET_FILE:zapif> ${ZAPIF_ARGS} -o ${output_file} ${input_file}
            && diff -U 5 ${expected_file} ${output_file}
    )
    set_property(
        TEST ${test_name}
        PROPERTY PASS_REGULAR_EXPRESSION "^$" # Any output from `diff` means the test failed.
    )
endforeach()

# Add a final test to validate that the zapif version matches the version from version.txt.
add_test(
    NAME test_zapif_version
    COMMAND
        $<TARGET_FILE:zapif> -v
)
set_property(
        TEST test_zapif_version
        PROPERTY PASS_REGULAR_EXPRESSION "zapif ${ZAPIF_VERSION}"
)