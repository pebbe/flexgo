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

C	yyout.Write([]byte("Go"))

%%

func main() {

        if len(os.Args) > 1 {
                files := os.Args[1:]
                r, err := os.Open(files[0])
                if err != nil {
                        log.Fatal(err)
                }
                yyin = r
                yywrap = func() bool {
                        r.Close()
                        files = files[1:]
                        if len(files) == 0 {
                                return true
                        }
                        r, err = os.Open(files[0])
                        yyrestart(r)
                        if err != nil {
                                log.Fatal(err)
                        }
                        return false
                }
        }

        yylex()
}
