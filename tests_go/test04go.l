%{

package main

import (
	"bytes"
	"fmt"
	"io"
	"log"
	"os"
)

var toggle = true

%}


%%

foo/bar		{
			YYout.Write(bytes.ToUpper(YYtext))
			YYout.Write([]byte{'B'})
		}
fo+/[a-z]+	{
			YYout.Write(bytes.ToUpper(YYtext))
			YYout.Write([]byte{'X'})
		}

%%

func main() {
	YYmain(os.Args[1:]...)
	fmt.Println("Done")
}
