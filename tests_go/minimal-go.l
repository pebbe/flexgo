%top{

package main

import (
	"io"
	"log"
	"os"
)

// return type of yy.Lex()
type YYtype interface{}

}

%{

YY_USER_DATA(
  // my very own user data
)

YY_USER_INIT(
  // my very own user init
)

YY_USER_ACTION(
  // my very own user action
)
%}

        // definitions: no definition


%%

	// rules: no rules, just echo all input to output

%%

func main() {
	// runs the scanner on files given as arguments, or stdin if there are no arguments
	YYmain(os.Args[1:]...)
}
