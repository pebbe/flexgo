
all: \
	test01c \
	test01go \
	test02c \
	test02go

test01c: test01c.l
	flex -B -8 -o test01c.c test01c.l
	gcc -s -Wall -o test01c test01c.c

test01go: test01go.l
	flexgo -B -8 -o test01go.go test01go.l
	go build test01go.go

test02c: test02c.l
	flex -B -8 -o test02c.c test02c.l
	gcc -s -Wall -o test02c test02c.c

test02go: test02go.l
	flexgo -B -8 -o test02go.go test02go.l
	go build test02go.go
