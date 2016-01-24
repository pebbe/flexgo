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

\"    {
		yyout.Write([]byte("<<"))
		BEGIN _str
        }

^.	yyout.Write(bytes.ToUpper(yytext))

foo/bar	{
		toggle = !toggle
		if !toggle {
			REJECT
		}
		yyout.Write(bytes.ToUpper(yytext))
	}

}

<_str>{

[^\"\\]+  ECHO

\\.	yyout.Write(yytext[1:])

\"      {
		yyout.Write([]byte(">>"))
		BEGIN INITIAL
        }

<<EOF>> yyterminate(fmt.Errorf("Closing quote is missing"))

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
