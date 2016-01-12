
all: demo-c demo-go demo-test

demo-c: demo-c.c
	gcc -s -Wall -o demo-c demo-c.c

demo-c.c: demo.l
	flex -S debug.skel -d -B -o demo-c.c demo.l

demo-go: demo-go.go
	go build demo-go.go

demo-test:
	echo -n 'ACabc' | ./demo-go
