%top{

package main

import (
	"bytes"
	"fmt"
	"io"
	"log"
	"os"
)

// return type of yy.Lex()
type YYtype interface{}

}

%%

foo/bar		{
			yyout.Write(bytes.ToUpper(yytext))
			yyout.Write([]byte{'B'})
		}
fo+/[a-np-z]+	{
			yyout.Write(bytes.ToUpper(yytext))
			yyout.Write([]byte{'X'})
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
