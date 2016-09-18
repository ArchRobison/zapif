# zapif
`zapif` algebraic simplifies C proprocessor conditionals and removes code that would never be selected by the preprocessor.

Unlike [`unifdef`](http://dotat.at/prog/unifdef/), `zapif` simplifies conditionals that it cannot eliminate.  For example, given a file `demo.c` with:
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
When simplifying a preprocessor directive, 
`zapif` tries to preserve whitespace and the original form 
of numbers when simplifying.

Examples of the simplifications that `zapif` knows:
* 0 || y -> y
* x || y -> 1 if x or y is nonzero.
* 0 && y -> 0 
* 0 | y -> y 

Algebraic simplifications are limited to ones that seem to be useful for real header files.  If you think I've missed a useful one, please open an issue, ideally with a citation to the header.

# Prequisites 

I developed `zapif` on MacOS, but it should build on Linux without difficulty.
To build `zapif` you will need:

* flex
* bison
* a C++11 compiler.  The default is `g++`.

# Building `zapif`

1. `cd build`
2. `make`

# Using `zapif`

The basic syntax is:
* `zapif` _options_ [_inputfile_]

If no input file is specified, input is `stdin`.
Output always goes to `stdout`.

The recognized options are:

* `-Dfoo=42` - treat `foo` as having the value 42.
* `-Dfoo` - same as `-Dfoo=1`
* `-Ufoo` - treat `foo` as undefined.  The preprocessor expression `defined(foo)` will evaluate to 0, and `foo` will be treated as 0 in other preprocessor expressions.
* `-k` - interpret numerals in preprocessor expressions.  Without this option, numerals such as `0` are treated as unknown values.

## Limitations

* Numbers are recognized only if they fit in a C++ `long`
  and follow the C conventions for decimal, octal, or hexadecimal,
  without any suffixes.
