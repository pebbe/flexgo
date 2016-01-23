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

%{

var toggle = true

%}


%%

foo/bar		{
			yy.Out.Write(bytes.ToUpper(yy.Text))
			yy.Out.Write([]byte{'B'})
		}
fo+/[a-np-z]+	{
			yy.Out.Write(bytes.ToUpper(yy.Text))
			yy.Out.Write([]byte{'X'})
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
