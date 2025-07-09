# zapif
`zapif` algebraic simplifies C and C++ preprocessor conditionals, and removes code that would never be selected by the preprocessor.

Unlike [`unifdef`](http://dotat.at/prog/unifdef/), `zapif` simplifies conditionals that it cannot eliminate. For example, given a file `demo.c` with:
```
#if foo && bar || quod
void f() {}
#endif
```
the command ``zapif -Dbar=1 -Dquod=0 demo.c` will produce:
```
#if foo
void f() {}
#endif
```
When simplifying a preprocessor directive, `zapif` tries to preserve whitespace
and the original form of numerals when simplifying. Literal numerals such as
`0` and `1` are treated as unknown values unless the `-k` option is present.

Examples of the simplifications that `zapif` knows:
* 0 || y -> y
* x || y -> 1 if x or y is nonzero.
* 0 && y -> 0
* 0 | y -> y
* 0 ^ y -> y
* 0 & y -> 0

Algebraic simplifications are limited to ones that seem to be useful for real C++ and C source. If you think I've missed a useful one, please open an issue, ideally with a citation to the source code.

# Prerequisites

I developed `zapif` on Mac OS X, but it should build on Linux without
difficulty. To build `zapif` you will need:

* flex 2.5 or 2.6
* bison 2.3 or 3.0
* a C++11 compiler. The default is `g++`.
* CMake 4.0 or higher

# Building `zapif`

Run the following comments in the `zapif` directory (this one).

```sh
cmake . -B build
cmake --build build
```

# Testing `zapif`

Zapif can be tested through `ctest`. To run the tests, execute the following command in the build directory:

```sh
ctest --output-on-failure
```

 If the test fails, you will see output from `diff`. If the `diff` output is not helpful, you can run:

```sh
ctest --rerun-failed --verbose
```

which will give you additional info, such as the stderr from `zapif`. You can also review the `.out` files in the `test` subdir (in the build dir).

# Using `zapif`

The basic syntax is:
* `zapif` _options_ [_inputfile_]

If no input file is specified, input is `stdin`.
Output goes to `stdout` unless the `-o` option is used.

The recognized options are:

* `-Dfoo=42` - treat `foo` as having the value `42`.
* `-Dfoo` - same as `-Dfoo=1`
* `-Ufoo` - treat `foo` as undefined. The preprocessor expression `defined(foo)` will evaluate to 0, and `foo` will be treated as 0 in other preprocessor expressions.
* `-c` - assume input is C, not C++. Causes `and`, `false`, `not`, `or`, `true` and `xor` to be treated as identifiers instead of their C++ meanings in `#if` expressions.
* `-e` - allow `$` and `@` in identifiers.
* `-k` - interpret numerals in preprocessor expressions. Without this option, numerals such as `0` are treated as unknown values.
* `-n` - normalize `#if defined(x)` to `#ifdef x` if the whole expression was the result of simplification. Do likewise for `#if !defined(x)` and the equivalent parentheses-free forms.
* `-o file` - use file for output instead of stdout.
* `-v` - print version information and exit.
* `--help` - print description of comand line usage.

## Limitations

* Numerals are recognized as having values only if they fit in a C++ `long long`,
  and follow C++14 conventions for decimal, octal, binary, or hexadecimal literals.

* Constant folding uses `long long`, even if a literal has a suffix specifying "unsigned".
  A warning is issued if a literal has an unsigned suffix and exceeds the range of `long long`.
