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

%%

foo     {
          ECHO
          yymore()
        }

bar     {
          ECHO
          c, err := yy.Input()
	  if err != nil {
	      log.Fatalln(err)
	  }
          yyout.Write([]byte{c,c,c})
        }

\<baz\> {
          ECHO
          ECHO
       }

baz    {
	 yycopy := string(yytext) // Copy yytext because yy.Unput() trashes yytext
         yy.Unput('>')
         for i := yyleng - 1; i >= 0; i-- {
             yy.Unput(yycopy[i])
         }
         yy.Unput('<')
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
