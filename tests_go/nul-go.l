%top{

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

var (
     nul = []byte("<NUL>")
     a0b = []byte("[a0b]")
     L = []byte("(")
     R = []byte(")")
)

%}

%%

notthis yyterminate()

\0   yyout.Write(nul)

a\0b yyout.Write(a0b)

a*\0+b*	{
	    for i := yyleng - 1; i >= 0; i-- {
	        if yytext[i] == 0 {
		    yytext[i] = '0'
		}
	    }
	    yyout.Write(L)
	    yyout.Write(yytext)
	    yyout.Write(R)
	}

wontmatch yyterminate()

%%

func main() {
	YYmain(os.Args[1:]...)
}
