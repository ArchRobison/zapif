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

Zapif uses CMake to build, and supports both Linux and Windows. The basic dependencies are:

* flex 2.5 or 2.6
* bison 2.3 or 3.0
* a C++11 compiler. The default is `g++`.
* CMake 4.0 or higher

Building for windows still requires flex and bison. The suggested distribution is [winflexbison](https://github.com/lexxmark/winflexbison) 2.5.25 or higher.

Alternatively, [Conan](https://conan.io) can provide flex, bison, and CMake automatically — see [Building with Conan](#building-with-conan) — leaving a C++11 compiler as the only tool you install yourself.

# Building `zapif`

You can build either with [Conan](https://conan.io) — which provisions flex, bison,
and CMake for you — or directly with CMake if you already have those tools installed.

## Building with Conan

Conan installs the required flex, bison, and CMake versions automatically (and uses
`winflexbison` on Windows), so the only things you provide yourself are a C++11
compiler and Conan 2:

```sh
pip install conan       # or: pipx install conan
conan profile detect    # first time only; detects your compiler
```

Then, from the `zapif` directory (this one):

```sh
conan install . --build=missing
conan build .
```

The executable is written to `build/Release/zapif` (or `build/<config>/zapif`).

To build, package, and smoke-test `zapif` as a Conan package — for example to reuse
it as a build tool in another project via `tool_requires` — run:

```sh
conan create .
```

The recipe sets `vendor = True`, so projects that consume the package only need its
prebuilt binary, and not dependencies.

As a result, Conan will not build `zapif` from source as a dependency of another
project unless that project passes `-c tools.graph:vendor=build`.

## Building directly with CMake

If you already have flex, bison, and CMake 4.0 or higher installed, run the following
commands in the `zapif` directory (this one):

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
* `-v` - print version information on stdout and exit.
* `--help` - print description of comand line usage.

## Limitations

* Numerals are recognized as having values only if they fit in a C++ `long long`,
  and follow C++14 conventions for decimal, octal, binary, or hexadecimal literals.

* Constant folding uses `long long`, even if a literal has a suffix specifying "unsigned".
  A warning is issued if a literal has an unsigned suffix and exceeds the range of `long long`.
