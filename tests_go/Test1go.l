/* -*-go-*- */
%top{

package main

import (
	"bytes"
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
	toggle = false
	count  = make([]int, yyNumRules+1)
)

YY_USER_ACTION(
	count[yyAct]++
)

%}

%s f1 f2

%%

<<EOF>>		{
				fmt.Fprintf(yyout, "<<EOF:%d>>\n", YY_START)
				yyterminate(nil)
			}

bar			fmt.Fprint(yyout, "BAR")

\n			ECHO

<INITIAL>{

(.|\n){4}	{
				fmt.Fprintf(yyout, "{1:%s}", string(yytext))
				for i := 0; i < 10000; i++ {
			    	    yyunput(byte(32 + i % 95))
				}
				BEGIN f1
			}

}

<f1,f2>{

zx		|

[zZ][xX] { yyout.Write([]byte("FOO"))
		  yymore()
        }

de[0-9]+ { ECHO
		  yyless(yyleng-2)
       }

^.		{
			fmt.Fprintf(yyout, "%5d\t", yylineno)
			REJECT
		}

^[a-z]		|

[aeiou]+/[0-9]+	|

qt		{
			toggle = !toggle
		    if toggle {
		       REJECT
		    }
		    yyout.Write(bytes.ToUpper(yytext))
		}

q[0-9]+		{
			for i := 0; i < 100; i++ {
			    _, err := yyinput()
			    if err != nil {
			       yyterminate()
			    }
			}
		}

"FINAL PART" {
		ECHO
		yyout.Write([]byte("\n>>>"))
		b := make([]byte, 1)
		for {
		    c, err := yyinput()
		    if err != nil {
		       break
		    }
		    b[0] = c
		    yyout.Write(b)
		}
		yyout.Write([]byte("<<<\n"))
		yyterminate()
	}

}

%%

type Data struct {
	count []int
}

func main() {

	var errval error

	yy := NewScanner()

	filenames := os.Args[1:]
	if len(filenames) > 0 {
		yy.Filename = filenames[0]
		yy.In, errval = os.Open(yy.Filename)
		if errval != nil {
			log.Fatalln(errval)
		}
		yy.Wrap = func(yyy *Scanner) bool {
			if len(filenames) == 0 {
				return true
			}
			yyy.In.(*os.File).Close()
			filenames = filenames[1:]
			if len(filenames) == 0 {
				return true
			}
			yyy.Filename = filenames[0]
			yyy.In, errval = os.Open(yyy.Filename)
			if errval != nil {
				log.Fatalln(errval)
			}
			return false
		}
	}

	yy.Lex()

	fmt.Println()
	fmt.Println()

	for i, n := range count {
		if i == 0 {
			continue
		}
		fmt.Printf("%4d\t%8d\n", i, n)
	}

}
