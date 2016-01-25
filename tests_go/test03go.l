%top{

package main

import (
	"bytes"
	"fmt"
	"io"
	"log"
	"os"
)

// return type of yy.Flex()
type YYtype interface{}

// returned on end of file
var YYnull YYtype = nil

}

%{

var toggle = true

%}


%%

^.	yyout.Write(bytes.ToUpper(yytext))

foo/bar	{
		toggle = !toggle
		if (toggle) {
			yyout.Write([]byte("FOO"))
		} else {
			REJECT
		}
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
