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

^.	{ fmt.Fprintf(yy.Out, "%4d.\t%s", yylineno, strings.ToUpper(string(yy.Text))) }

\n      yy.Echo() // or yy.LineNo won't get updated


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
