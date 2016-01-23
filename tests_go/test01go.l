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

foo|bar	yy.Out.Write(bytes.ToUpper(yy.Text))
C	yy.Out.Write([]byte("Go"))

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
