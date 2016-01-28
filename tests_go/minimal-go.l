%top{

package main

import (
	"fmt"
	"io"
	"log"
	"os"
)

// return type of yy.Flex()
type YYtype interface{}

// returned on end of file
var YYnull YYtype = nil

}

        /* no definitions. */


%%

	/* No rules so default action of copying each input character to the output is performed. */

%%

func main() {
	// runs the scanner on files given as arguments, or stdin if there are no arguments
	YYmain(os.Args[1:]...)
}
