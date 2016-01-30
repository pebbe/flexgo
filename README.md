A version of flex that can produce Go code.

Based on flex version 2.6.0, see: http://flex.sourceforge.net/

Keywords: flex, lex, go, golang

## Minimal scanner

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

        // rules: no rules, just echo all input to output

    %%

        // code to put at the end of the file

        func main() {
            // runs yy.Lex() on files given as arguments, or stdin if there are no arguments
            YYmain(os.Args[1:]...)
        }

## Not implemented

Multiple buffers are not supported. If you want to switch input sources
temporarily, just start another scanner. There are no global variables.

Ignored options:

 * --array
 * --c++
 * --reentrant
 * others... TODO

## Difference between C output and Go output

### Functions and variables

            C                  Go
    ----------------------------------------------------------------
            yyin               yy.In      io.Reader
            yyout              yy.Out     io.Writer
            yylineno           yy.Lineno  int
            yytext             yytext     []byte

    YY_DECL yylex(void)        yy.Lex() YYtype
        int yywrap(void)       yy.Wrap() bool
        int input(void)        yyinput() (byte, error)
       void unput(int)         yyunput(byte)

Inside actions, the names `yyout` and `yylineno` are also available.

### yy.IsInteractive(io.Reader)

`yy.IsInteractive(io.Reader)` is a variable to a function that returns a
bool. The default returns `true` is `flexgo` was called with option
`-I`, and `false` with option `-B`. (TODO: default value with neither
option?)

### YYmain()

TODO

### Macros

TODO
