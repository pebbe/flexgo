
.SUFFIXES:

all: \
	test01c_b \
	test01c_i \
	test01go_b \
	test01go_i \
	test02c \
	test02go_b \
	test02go_i

test01c_b: test01c.l
	flex -d -B -8 -o test01c_b.c test01c.l
	gcc -s -Wall -o test01c_b test01c_b.c

test01c_i: test01c.l
	flex -d -I -8 -o test01c_i.c test01c.l
	gcc -s -Wall -o test01c_i test01c_i.c

test01go_b: test01go.l
	flexgo -d -B -o test01go_b.go test01go.l
	go build test01go_b.go

test01go_i: test01go.l
	flexgo -d -I -o test01go_i.go test01go.l
	go build test01go_i.go

test02c: test02c.l
	flex -B -8 -o test02c.c test02c.l
	gcc -s -Wall -o test02c test02c.c

test02go_b: test02go.l
	flexgo -B -8 -o test02go_b.go test02go.l
	go build test02go_b.go

test02go_i: test02go.l
	flexgo -I -8 -o test02go_i.go test02go.l
	go build test02go_i.go
