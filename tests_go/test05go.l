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
		yyout.Write([]byte("<<"))
		BEGIN(_str)
        }

^.	yyout.Write(bytes.ToUpper(yytext))

foo/bar	{
		toggle = !toggle
		if toggle {
			yyout.Write(bytes.ToUpper(yytext))
		} else {
			REJECT()
		}
	}

}

<_str>{

[^\"\\]+  ECHO()

"\""    {
		yyout.Write([]byte(">>"))
		BEGIN(INITIAL)
        }

\\.	yyout.Write(yytext[1:])

<<EOF>> yyterminate(fmt.Errorf("Closing quote is missing"))

}

%%

func main() {
	result, err := yymain(os.Args[1:]...)
	if err != nil {
		log.Fatal(err)
	}
	if result != nil {
		fmt.Println(result)
	}
	fmt.Println("Done")
}
