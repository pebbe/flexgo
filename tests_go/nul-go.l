%top{

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

var (
     nul = []byte("<NUL>")
     a0b = []byte("[a0b]")
     L = []byte("(")
     R = []byte(")")
)

%}


%%

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

%%

func main() {
	// runs the scanner on files given as arguments, or stdin if there are no arguments
	YYmain(os.Args[1:]...)
}
