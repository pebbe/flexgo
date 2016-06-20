%top{

package main

import (
	"bytes"
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

%%

^.	fmt.Fprintf(yyout, "%4d.\t%c", yylineno, bytes.ToUpper(yytext)[0])

.|\n    ECHO // or yylineno won't get updated

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
