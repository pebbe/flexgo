%top{

package main

import (
	"fmt"
	"io"
	"log"
	"os"
)

}

%%

fofo    { yyout.Write([]byte("[FOFO]"))
	  yyless(2)
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
