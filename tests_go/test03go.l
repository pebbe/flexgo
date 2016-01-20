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

^.	YYout.Write(bytes.ToUpper(YYtext))

foo/bar	{
		toggle = !toggle
		if (toggle) {
			YYout.Write([]byte("FOO"))
		} else {
			REJECT();
		}
	}

%%

func main() {
	YYmain(os.Args[1:]...)
	fmt.Println("Done")
}
