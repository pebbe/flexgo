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

%x _str

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

<<EOF>> YYterminate(fmt.Errorf("Closing quote is missing"))

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
