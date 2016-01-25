A version of flex that can produce Go code.

Based on flex version 2.6.0, see: http://flex.sourceforge.net/

Keywords: flex, lex, go, golang

**Work in progress**

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
 * yyunput(c)
 * ~~yy.UserData~~
 * ~~yy.UserAction~~
 * ~~--yylineno~~
 * non-default table compression
 * other functions, like yy.Restart(), ~~multiple buffers~~, etc...
 * copy and modify all tests from subdirectory `tests` to work with `flexgo`

## Difference between C output and Go output

### Functions and variables

        C                  Go
    -----------------------------------------------
        yyin               yy.In    io.Reader
        yyout              yy.Out   io.Writer
        yytext             yy.Text  []byte
        yyleng             yy.Leng
    int yylex(void)        yy.Lex() interface()
        yyterminate(int)   yyterminate(interface{})
        input()            yyinput()

Inside actions, the usual names `yyout`, `yytext`, and `yyleng` are also
available.

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
