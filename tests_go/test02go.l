%{

package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"os"
	"strings"
)

%}


%%

^.	YYout.Write([]byte(strings.ToUpper(string(yytext))))

%%

func main() {
	YYmain(os.Args[1:]...)
	fmt.Println("Done")
}
