
all: \
	test01c \
	test01go_b \
	test01go_i \
	test02c \
	test02go_b \
	test02go_i

test01c: test01c.l
	flex -B -8 -o test01c.c test01c.l
	gcc -s -Wall -o test01c test01c.c

test01go_b: test01go.l
	flexgo -B -o test01go_b.go test01go.l
	go build test01go_b.go

test01go_i: test01go.l
	flexgo -I -o test01go_i.go test01go.l
	go build test01go_i.go

test02c: test02c.l
	flex -B -8 -o test02c.c test02c.l
	gcc -s -Wall -o test02c test02c.c

test02go_b: test02go.l
	flexgo -B -8 -o test02go_b.go test02go.l
	go build test02go_b.go

test02go: test02go.l
	flexgo -I -8 -o test02go_i.go test02go.l
	go build test02go_i.go
