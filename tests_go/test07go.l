%top{

package main

import (
	"fmt"
	"io"
	"log"
	"os"
	"strings"
)

}

%%

^.	{ fmt.Fprintf(yyout, "%4d.\t%s", yylineno, strings.ToUpper(string(yytext))) }

\n      ECHO() // or yylineno won't get updated


%%

func main() {
	result, err := yymain(os.Args[1:]...)
	if err != nil {
		log.Fatal(err)
	}
	if result != nil {
		fmt.Println(result)
	}
	fmt.Println("Done")
}
