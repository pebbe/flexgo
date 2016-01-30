package main

import (
	//yaml "launchpad.net/goyaml"
	"gopkg.in/yaml.v2"

	"fmt"
	"io/ioutil"
	"os"
	"runtime"
)

type TestT struct {
	Name  string `yaml:"name"`
	Clex  string `yaml:"clex"`
	Glex  string `yaml:"glex"`
	Opts  string `yaml:"opts"`
	Files string `yaml:"files"`
}

func main() {
	data, err := ioutil.ReadFile("Make.yaml")
	x(err)

	tests := make([]TestT, 0)
	//var tests interface{}
	err = yaml.Unmarshal(data, &tests)

	fp, err := os.Create("Makefile2")
	x(err)
	fmt.Fprint(fp, "\n.SUFFIXES:\n\nall:")

	for _, test := range tests {
		fmt.Fprintf(fp, " \\\n\t%s.ok", test.Name)
	}

	fmt.Fprint(fp, "\n\nclean:\n\trm -f")

	for _, test := range tests {
		fmt.Fprintf(fp, " \\\n\t%s.ok \\\n\t%s-c.out \\\n\t%s-go.out \\\n\t%s-c.err \\\n\t%s-go.err \\\n\t%s-c.c \\\n\t%s-go.go \\\n\t%s-c \\\n\t%s-go",
			test.Name, test.Name, test.Name, test.Name, test.Name, test.Name, test.Name, test.Name, test.Name)
	}

	fmt.Fprintln(fp)

	for _, test := range tests {
		fmt.Fprintf(fp, "\n%s.ok: %s %s %s ../src/flex Make.yaml\n", test.Name, test.Clex, test.Glex, test.Files)
		fmt.Fprintf(fp, "\t../src/flex --noline %s -o %s-c.c %s 2> %s-c.err\n", test.Opts, test.Name, test.Clex, test.Name)
		fmt.Fprintf(fp, "\tgcc -s -Wall -o %s-c %s-c.c\n", test.Name, test.Name)
		fmt.Fprintf(fp, "\t./%s-c %s > %s-c.out\n", test.Name, test.Files, test.Name)
		fmt.Fprintf(fp, "\t../src/flex --go --noline %s -o %s-go.go %s 2> %s-go.err \n", test.Opts, test.Name, test.Glex, test.Name)
		fmt.Fprintf(fp, "\t@gofmt -w %s-go.go\n", test.Name)
		fmt.Fprintf(fp, "\tgo build %s-go.go\n", test.Name)
		fmt.Fprintf(fp, "\t./%s-go %s > %s-go.out\n", test.Name, test.Files, test.Name)
		fmt.Fprintf(fp, "\t@diff -q %s-c.out %s-go.out\n", test.Name, test.Name)
		fmt.Fprintf(fp, "\t@echo -n %c[1m\n", 27)
		fmt.Fprintf(fp, "\t@diff -U0 %s-c.err %s-go.err || true\n", test.Name, test.Name)
		fmt.Fprintf(fp, "\t@echo %c[0m\n", 27)
		fmt.Fprintf(fp, "\t@touch %s.ok\n", test.Name)
	}

	fp.Close()

	fp, err = os.Create(".gitignore")
	x(err)
	fmt.Fprintln(fp, ".gitignore\nMake\nMakefile2\nlex.backup")
	for _, test := range tests {
		fmt.Fprintf(fp, "%s.ok\n%s-c.out\n%s-go.out\n%s-c.err\n%s-go.err\n%s-c.c\n%s-go.go\n%s-c\n%s-go\n",
			test.Name, test.Name, test.Name, test.Name, test.Name, test.Name, test.Name, test.Name, test.Name)
	}
	fp.Close()

}

func x(err error) {
	if err == nil {
		return
	}
	_, filename, lineno, ok := runtime.Caller(1)
	if ok {
		fmt.Fprintf(os.Stderr, "Error: %v:%v: %v\n", filename, lineno, err)
	} else {
		fmt.Fprintln(os.Stderr, err)
	}
}
