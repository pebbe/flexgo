%top{

package main

import (
	"fmt"
	"io"
	"log"
	"os"
)

// return type of yy.Lex()
type YYtype interface{}

// type of Scanner.Context
type YYcontext interface{}

}

%s f1

%%

<INITIAL>{

(.|\n){4}	{
				fmt.Fprintf(yyout, "{1:%s}", string(yytext))
				for i := 0; i < 10000; i++ {
					yyunput(byte(32 + i % 96))
				}
				BEGIN f1
			}

}

<f1>{

foo     {
          ECHO
          yymore()
        }

bar     {
          ECHO
          c, err := yyinput()
	  if err != nil {
	      log.Fatalln(err)
	  }
          yyout.Write([]byte{c,c,c})
        }

\[baz\] {
          ECHO
          ECHO
       }

baz    {
	 yycopy := string(yytext) // Copy yytext because yy.Unput() trashes yytext
         yyunput(']')
         for i := yyleng - 1; i >= 0; i-- {
             yyunput(yycopy[i])
         }
         yyunput('[')
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

func main() {
	result, err := YYmain(os.Args[1:]...)
	if err != nil {
		log.Fatal(err)
	}
	if result != nil {
		fmt.Println(result)
	}
	fmt.Println("Done")
}
