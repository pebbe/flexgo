A version of flex that can produce Go code.

This is a modified version of flex version 2.6.0, see: http://flex.sourceforge.net/

Keywords: flex, lex, go, golang

## A basic scanner

    %top{

        // code to put at the very top of the file

        package main

        import (
            "io"
            "log"
            "os"
        )

        // return type of yy.Lex()
        type YYtype interface{}

    }

    %{

        // code to put near the top of the file

        YY_USER_DATA(
            // user data inside Lex()
        )

        YY_USER_INIT(
            // code to run inside Lex() when it is called the first time
        )

        YY_USER_ACTION(
            // code to run at the start of each action
        )

        // more code

    %}

        // definitions: no definition

    %%

    Hi      yyout.Write([]byte("Hello world!")

    %%

        // code to put at the end of the file

        func main() {
            // runs yy.Lex() on files given as arguments, or stdin if there are no arguments
            YYmain(os.Args[1:]...)
        }

See directory `tests_go` for more complex examples.

To compile a flex file into Go code, call one of these:

    flex --go
    flex -G
    flexgo

Run `flex` without `--go` or `-G` to get the behaviour of regular `flex`.

Regular `flex` output is C code with a lot of global variables. With the
option `--reentrant` you get a scanner with those variables encapsulated
in a struct, and there are special commands to interact with the
scanner.

The Go code produced with `flexgo` has no global variables, other then
those that should never be changed. A scanner is an object you create
with `NewScanner()` and then you call its `Lex()` method.

## Not implemented

Multiple buffers are not supported. If you want to switch input sources
temporarily, just start another scanner. There are no global variables.

Ignored/irrelevant options:

 * --array
 * --c++
 * --reentrant
 * others... TODO

## Difference between C output and Go output

### Variables and functions

            C                  Go
    ----------------------------------------------------------------
            yyin               yy.In       io.Reader
            yyout              yy.Out      io.Writer
            yylineno           yy.Lineno   int
            yytext             yytext      []byte
        int yywrap(void)       yy.Wrap     func() bool

    YY_DECL yylex(void)        yy.Lex() YYtype
        int input(void)        yyinput() (byte, error)
       void unput(int)         yyunput(byte)

Inside actions, the names `yyout` and `yylineno` are also available.

The default value of `yy.Wrap` is a function that always returns `true`.

### Macros

Because Go doesn't have the `#define` of C, `flexgo` uses more M4 macros
than regular `flex`, to achieve the same functionality. Like `flex`, the
tokens `[[` and `]]` have special meaning.

The following are defined as macros:

 * `INITIAL`
 * `BEGIN`
 * `ECHO`
 * `REJECT`
 * `yyterminate()`
 * `yyless(n)`
 * `yymore()`
 * `yyinput()`
 * `yyunput(c)`
 * `YY_START`
 * `YYSTATE`

Only those relevant to the user are listed.

The M4 macro mechanism has some particularities you have to watch out
for.

`BEGIN` is defined as a macro without argument:

    BEGIN state     // OK
    BEGIN (state)   // OK
    BEGIN(state)    // Error: MUST use space
    BEGIN x + y     // Error: MUST use parenthesis
    BEGIN (x + y)   // OK
    BEGIN(x + y)    // Error: MUST use space

`yyless(n)` and `yyinput(c)` are defined as macros with argument:

    yyless(n)      // OK
    yyless (n)     // Error: MUST NOT use space

Furthermore, there are three macros that can be used to set data and
code inside the scanner, `YY_USER_DATA()` (not in regular `flex`),
`YY_USER_INIT()`, and `YY_USER_ACTION()`. If you use these macros, you
must put them in the block between `%{` and `%}`. See above, the basic
scanner.

The macro `YY_USER_INIT()` doesn't seem very useful in
Go, but it is there in regular `flex`.

### YYmain

`YYmain(filenames ...string)` is a convenience function. If you call it
without arguments, it is the same as this:

    scanner := NewScanner()
    return scanner.Lex(), nil

When there are filenames, `YYmain` sets up `scanner.YYwrap()` to iterate
over all files before calling `scanner.Lex()`.

Inside actions in the scanner, the current file name is available as
`yy.Filename`.

### IsInteractive

Regular `flex` checks to see if the input file is a terminal. It then
reads in bytes one by one instead of in large chunks.

In `flexgo`, the input is a `io.Reader`, and there is no easy way to
tell if it's interactive or not. Calling `scanner := NewScanner()` sets
up the variable `scanner.IsInteractive(io.Reader)` to return `true` if
you called `flexgo` with the option `-I`, and `false` if you called
`flexgo` with the option `-B`. You can point `scanner.IsInteractive` to
another function of you need more control.

### To do

 * Use `YY_FATAL()` for critical errors?
 * More tests

