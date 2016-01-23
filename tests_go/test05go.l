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
		yy.Out.Write([]byte("<<"))
		yy.Begin(_str)
        }

^.	yy.Out.Write(bytes.ToUpper(yy.Text))

foo/bar	{
		toggle = !toggle
		if toggle {
			yy.Out.Write(bytes.ToUpper(yy.Text))
		} else {
			yy.Reject()
		}
	}

}

<_str>{

[^\"\\]+  yy.Echo()

"\""    {
		yy.Out.Write([]byte(">>"))
		yy.Begin(Initial)
        }

\\.	yy.Out.Write(yy.Text[1:])

<<EOF>> yy.Terminate(fmt.Errorf("Closing quote is missing"))

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