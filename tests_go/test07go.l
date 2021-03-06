%top{

package main

import (
	"fmt"
	"io"
	"log"
	"os"
	"strings"
)

// return type of yy.Lex()
type YYtype interface{}

// type of Scanner.Context
type YYcontext interface{}

}

%%

^.	{ fmt.Fprintf(yyout, "%4d.\t%s", yy.Lineno, strings.ToUpper(string(yytext))) }

\n      ECHO // or yy.Lineno won't get updated

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
