A version of flex that can produce Go code.

Based on flex version 2.6.0, see: http://flex.sourceforge.net/

Keywords: flex, lex, go, golang

**Work in progress**

## To do

 * ~~a simple pattern~~
 * ~~a simple pattern with ^~~
 * ~~YYwrap()~~
 * ~~REJECT()~~
 * REJECT() with trailing context
 * BEGIN(state)
 * YYless(n)
 * YYmore()
 * Unput(c)
 * Input()
 * non-default table compression
 * other functions, like yyrestart(), multiple buffers, etc...

## Difference between C output and Go output

### Functions and variables

C macro's are replaced by functions, sometimes with different behaviour.

    C           Go         Go type
    --------------------------------
    yyin        YYin       io.Reader
    yyout       YYout      io.Writer
	yytext      YYtext     []byte
    yylex()     YYlex()  
    REJECT      REJECT()
    BEGIN s     BEGIN(s)

### REJECT()

In C output, `REJECT` is a macro that ends with a `goto` statement. In
Go output, `REJECT()` is a function that sets a flag variable.

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
                       REJECT()
                   } else {
                       bar()
                   }
               }       

### YYterminate()

In C output, `yyterminate()` is a macro that ends with a `return` statement. In
Go output, `YYterminate()` is a function that sets a flag variable.

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
                       YYterminate()
                   } else {
                       bar()
                   }
               }       

### YYwrap()

`YYwrap` is a variable containing a function that returns a bool. The
default returns `true`.

### IsInteractive(io.Reader)

`IsInteractive(io.Reader)` is a variable to a function that returns a
bool. The default returns `true` is `flexgo` was called with option
`-I`, and `false` with option `-B`. (TODO: default value with neither
option?)

### YYmain()

TODO
