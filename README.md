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
 * ~~yy.Reject()~~
 * ~~yy.Reject() with trailing context~~
 * ~~yy.Begin(state)~~
 * ~~yy.Less(n)~~
 * yy.More()
 * yy.Unput(c)
 * yy.Input()
 * --yylineno
 * non-default table compression
 * other functions, like yy.Restart(), ~~multiple buffers~~, etc...
 * copy and modify all tests from subdirectory `tests` to work with `flexgo`

## Difference between C output and Go output

### Functions and variables

C macro's are replaced by functions, sometimes with different behaviour.

    C               Go                        Go type
    ---------------------------------------------------
    yyin            yy.In                      io.Reader
    yyout           yy.Out                     io.Writer
	yytext          yy.Text                    []byte
    yylex()         yy.Lex()  
	yyterminate()   yy.Terminate(interface{})
    REJECT          yy.Reject()
    BEGIN s         yy.Begin(s)

### yy.Reject()

In C output, `REJECT` is a macro that ends with a `goto` statement. In
Go output, `yy.Reject()` is a function that sets a flag variable.

Example in C:

    pattern    {
                   if (foo) {
                       REJECT;
                   }
                   bar();
               }
               
To get the same behaviour in Go, the `bar()` must be after an else, or
it will always be called:

    pattern    {
                   if foo {
                       yy.Reject()
                   } else {
                       bar()
                   }
               }       

### yy.Terminate(return_value)

In C output, `yyterminate()` is a macro that ends with a `return` statement. In
Go output, `yy.Terminate(return_value)` is a function that sets a flag
variable. The type of `return_value` is `interface{}`.

Example in C:

    pattern    {
                   if (foo) {
                       yyterminate();
                   }
                   bar();
               }
               
To get the same behaviour in Go, the `bar()` must be after an else, or
it will always be called:

    pattern    {
                   if foo {
                       yy.Terminate(return_value)
                   } else {
                       bar()
                   }
               }       

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
