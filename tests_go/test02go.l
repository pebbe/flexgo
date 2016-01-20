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

^.	YYout.Write(bytes.ToUpper(YYtext))

%%

func main() {
	YYmain(os.Args[1:]...)
	fmt.Println("Done")
}
