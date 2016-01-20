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
	YYmain(os.Args[1:]...)
	fmt.Println("Done")
}
