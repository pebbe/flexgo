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

foo|bar	YYout.Write(bytes.ToUpper(YYtext))
C	YYout.Write([]byte("Go"))

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
