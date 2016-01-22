%top{

package main

import (
	"bytes"
	"fmt"
	"io"
	"log"
	"os"
)

}


%%

foo|bar	yyout.Write(bytes.ToUpper(yytext))
C	yyout.Write([]byte("Go"))

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
