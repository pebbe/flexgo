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

%{

var (
    toggle = false
    count  = make([]int, yyNumRules+1)
)

YY_USER_ACTION(
    count[yyAct]++
)

%}

%s f1 f2

%%

<<EOF>>           {
                      fmt.Fprintf(yyout, "<<EOF:%d>>\n", YY_START)
                      yyterminate()
                  }

bar               fmt.Fprint(yyout, "BAR")

\n                ECHO

<INITIAL>{

(.|\n){4}         {
                      fmt.Fprintf(yyout, "{1:%s}", string(yytext))
                      for i := 0; i < 10000; i++ {
                          yyunput(byte(32 + i % 95))
                      }
                      BEGIN f1
                  }

}

<f1,f2>{

zx                |

[zZ][xX]          {
                      yyout.Write([]byte("FOO"))
                      yymore()
                  }

de[0-9]+          {
                      ECHO
                      yyless(yyleng-2)
                  }

^.                {
                      fmt.Fprintf(yyout, "%5d\t", yylineno)
                      REJECT
                  }

^[a-z]            |

[aeiou]+/[0-9]+   |

qt                {
                      toggle = !toggle
                      if toggle {
                          REJECT
                      }
                      yyout.Write(bytes.ToUpper(yytext))
                  }

[a-z]/b\n         |

[A-Z]/B\n+        yyout.Write(yytext)

q[0-9]+           {
                      for i := 0; i < 100; i++ {
                          _, err := yyinput()
                          if err != nil {
                              yyterminate()
                          }
                      }
                  }

"FINAL PART"      {
                      ECHO
                      yyout.Write([]byte("\n>>>"))
                      b := make([]byte, 1)
                      for {
                          c, err := yyinput()
                          if err != nil {
                              break
                          }
                          b[0] = c
                          yyout.Write(b)
                      }
                      yyout.Write([]byte("<<<\n"))
                      yyterminate()
                  }

}

%%

type Data struct {
    count []int
}

func main() {

    YYmain(os.Args[1:]...)

    fmt.Println()
    fmt.Println()

    for i, n := range count {
        if i == 0 {
            continue
        }
        fmt.Printf("%4d\t%8d\n", i, n)
    }

}
