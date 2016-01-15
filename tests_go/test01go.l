%{

package main

import (
	"bufio"
	"fmt"
	"io"
	"log"
	"os"
)

%}


%%

C	YYout.Write([]byte("Go"))

%%

func main() {
	YYmain(os.Args[1:]...)
	fmt.Println("Done")
}
