%{

package main

import (
        "bufio"
        "io"
        "log"
        "os"
)

%}


%%

C	YYout.Write([]byte("Go"))

%%

func main() {
	YYmain(os.Args[1:]...)
}
