%{

package main

import (
	"bufio"
	"bytes"
	"fmt"
	"io"
	"log"
	"os"
	"strings"
)

%}


%%

^.	YYout.Write(bytes.ToUpper(yytext))

%%

func main() {
	YYmain(os.Args[1:]...)
	fmt.Println("Done")
}
