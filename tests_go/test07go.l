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

^.	{ fmt.Fprintf(YYout, "%4d.\t%s", YYlineno, strings.ToUpper(string(YYtext))) }

\n      ECHO() // or yylineno won't get updated


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
