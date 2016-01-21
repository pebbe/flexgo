
.SUFFIXES:

all: makemake makeall

clean: makemake makeclean

makemake: Makefile2

Makefile2: Make Make.yaml
	./Make

makeall:
	make -f Makefile2 all

makeclean:
	make -f Makefile2 clean
	rm -f Makefile2 Make .gitignore

Make: Make.go
	go build Make.go
