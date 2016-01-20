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

%Start _str

%%

<INITIAL>{

"\""    {
		YYout.Write([]byte("<<"))
		BEGIN(_str)
        }

^.	YYout.Write(bytes.ToUpper(YYtext))

foo/bar	{
		toggle = !toggle
		if toggle {
			YYout.Write(bytes.ToUpper(YYtext))
		} else {
			REJECT()
		}
	}

}

<_str>{

[^\"\\]+  ECHO()

"\""    {
		YYout.Write([]byte(">>"))
		BEGIN(INITIAL)
        }

\\.	YYout.Write(YYtext[1:])

}

%%

func main() {
	YYmain(os.Args[1:]...)
	fmt.Println("Done")
}
