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

%s f1 f2

%%

<<EOF>>		{
				fmt.Fprintf(yyout, "<<EOF:%d>>\n", YY_START)
				yyterminate(nil)
			}

bar			fmt.Fprint(yyout, "BAR")

<INITIAL>{

(.|\n){4}	{
				fmt.Fprintf(yyout, "{1:%s}", string(yytext))
				for i := 0; i < 10000; i++ {
			    	    yyunput(byte(32 + i % 96))
				}
				BEGIN f1
			}

}

<f1,f2>{

Foo		|

foo		yyout.Write([]byte("FOO"))

^[a-z]		|

[aeiou]+/[0-9]+	{
		    if yytext[0] == 'a' {
		       REJECT
		    }
		    yyout.Write(bytes.ToUpper(yytext))
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
		yyterminate(nil)
	}

}

%%

type Data struct {
	count []int
}

func main() {

	var errval error

	yy := NewScanner()

	yy.UserData = Data{
		count: make([]int, yyNumRules+1),
	}
	yy.UserAction = func(yy *Scanner) {
		yy.UserData.(Data).count[yy.act]++
	}

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

	for i, n := range yy.UserData.(Data).count {
		if i == 0 {
			continue
		}
		fmt.Printf("%4d\t%8d\n", i, n)
	}

}
