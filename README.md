A version of flex that can produce Go code.

Based on flex version 2.6.0, see: http://flex.sourceforge.net/

Keywords: flex, lex, go, golang

**Needs more testing**

## To do

 * ~~a simple pattern~~
 * ~~a simple pattern with ^~~
 * ~~trailing context~~
 * ~~pattern with EOF~~
 * ~~yy.Wrap()~~
 * ~~REJECT~~
 * ~~REJECT with trailing context~~
 * ~~BEGIN(state)~~
 * ~~yyless(n)~~
 * ~~yymore()~~
 * ~~yyinput()~~
 * ~~yyunput(c)~~
 * ~~yy.UserData~~
 * ~~yy.UserAction~~
 * ~~--yylineno~~
 * ~~non-default table compression~~
 * copy and modify all tests from subdirectory `tests` to work with `flexgo`
 * documentation

## Not implemented

Multiple buffers are not supported. If you want to switch input sources
temporarily, just start another scanner. There are no global variables.

Ignored options:

 * --7bit
 * --array
 * --c++
 * --reentrant

## Minimal scanner

    %top{

        // code to put at the very top of the file

        package main

        import (
            "fmt"
            "io"
            "log"
            "os"
        )

        // return type of yy.Flex()
        type YYtype interface{}

        // returned on end of file
        var YYnull YYtype = nil

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
            // runs the scanner on files given as arguments, or stdin if there are no arguments
            YYmain(os.Args[1:]...)
        }

## Difference between C output and Go output

### Functions and variables

            C                  Go
    ----------------------------------------------------------------
            yyin               yy.In      io.Reader
            yyout              yy.Out     io.Writer
            yytext             yy.Text    []byte
            yyleng             yy.Leng    int
            yylineno           yy.Lineno  int

    YY_DECL yylex(void)        yy.Lex() YYtype
        int input(void)        yyinput() (byte, error)
            unput(int)         yyunput(byte)

Inside actions, the names `yyout`, `yytext`, `yyleng`, and `yylineno`
are also available.

### yy.Wrap()

`yy.Wrap` is a variable containing a function that returns a bool. The
default returns `true`.

### yy.IsInteractive(io.Reader)

`yy.IsInteractive(io.Reader)` is a variable to a function that returns a
bool. The default returns `true` is `flexgo` was called with option
`-I`, and `false` with option `-B`. (TODO: default value with neither
option?)

### YYmain()

TODO
