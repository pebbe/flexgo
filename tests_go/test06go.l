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

fofo    { YYout.Write([]byte("[FOFO]")
	  YYless(2)
	}


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
