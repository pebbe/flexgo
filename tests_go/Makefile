
.SUFFIXES:

TARGETS = \
	test01c_i.ok \
	test01go_b.ok \
	test01go_i.ok \
	test02c_i.ok \
	test02go_b.ok \
	test02go_i.ok \
	test03c_i.ok \
	test03go_b.ok \
	test03go_i.ok

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.out \
		test01c_b \
		test01c_i \
		test01go_b \
		test01go_i \
		test02c_b \
		test02c_i \
		test02go_b \
		test02go_i \
		test03c_b \
		test03c_i \
		test03go_b \
		test03go_i

test01c_b.out: test01c_b
	./test01c_b text1.txt text2.txt text3.txt > test01c_b.out 2> /dev/null

test01c_i.out: test01c_i
	./test01c_i text1.txt text2.txt text3.txt > test01c_i.out 2> /dev/null

test01go_b.out: test01go_b
	./test01c_b text1.txt text2.txt text3.txt > test01go_b.out 2> /dev/null

test01go_i.out: test01go_i
	./test01go_i text1.txt text2.txt text3.txt > test01go_i.out 2> /dev/null

test02c_b.out: test02c_b
	./test02c_b text1.txt text2.txt text3.txt > test02c_b.out 2> /dev/null

test02c_i.out: test02c_i
	./test02c_i text1.txt text2.txt text3.txt > test02c_i.out 2> /dev/null

test02go_b.out: test02go_b
	./test02c_b text1.txt text2.txt text3.txt > test02go_b.out 2> /dev/null

test02go_i.out: test02go_i
	./test02go_i text1.txt text2.txt text3.txt > test02go_i.out 2> /dev/null

test03c_b.out: test03c_b
	./test03c_b text1.txt text2.txt text3.txt > test03c_b.out 2> /dev/null

test03c_i.out: test03c_i
	./test03c_i text1.txt text2.txt text3.txt > test03c_i.out 2> /dev/null

test03go_b.out: test03go_b
	./test03c_b text1.txt text2.txt text3.txt > test03go_b.out 2> /dev/null

test03go_i.out: test03go_i
	./test03go_i text1.txt text2.txt text3.txt > test03go_i.out 2> /dev/null

test01c_i.ok: test01c_b.out test01c_i.out
	diff -q test01c_b.out test01c_i.out
	touch test01c_i.ok

test01go_b.ok: test01c_b.out test01go_b.out
	diff -q test01c_b.out test01go_b.out
	touch test01go_b.ok

test01go_i.ok: test01c_b.out test01go_i.out
	diff -q test01c_b.out test01go_i.out
	touch test01go_i.ok

test02c_i.ok: test02c_b.out test02c_i.out
	diff -q test02c_b.out test02c_i.out
	touch test02c_i.ok

test02go_b.ok: test02c_b.out test02go_b.out
	diff -q test02c_b.out test02go_b.out
	touch test02go_b.ok

test02go_i.ok: test02c_b.out test02go_i.out
	diff -q test02c_b.out test02go_i.out
	touch test02go_i.ok

test03c_i.ok: test03c_b.out test03c_i.out
	diff -q test03c_b.out test03c_i.out
	touch test03c_i.ok

test03go_b.ok: test03c_b.out test03go_b.out
	diff -q test03c_b.out test03go_b.out
	touch test03go_b.ok

test03go_i.ok: test03c_b.out test03go_i.out
	diff -q test03c_b.out test03go_i.out
	touch test03go_i.ok


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

test02c_b: test02c.l
	flex -B -8 -o test02c_b.c test02c.l
	gcc -s -Wall -o test02c_b test02c_b.c

test02c_i: test02c.l
	flex -B -8 -o test02c_i.c test02c.l
	gcc -s -Wall -o test02c_i test02c_i.c

test02go_b: test02go.l
	flexgo -B -8 -o test02go_b.go test02go.l
	go build test02go_b.go

test02go_i: test02go.l
	flexgo -I -8 -o test02go_i.go test02go.l
	go build test02go_i.go

test03c_b: test03c.l
	flex -B -8 -o test03c_b.c test03c.l
	gcc -s -Wall -o test03c_b test03c_b.c

test03c_i: test03c.l
	flex -B -8 -o test03c_i.c test03c.l
	gcc -s -Wall -o test03c_i test03c_i.c

test03go_b: test03go.l
	flexgo -B -8 -o test03go_b.go test03go.l
	go build test03go_b.go

test03go_i: test03go.l
	flexgo -I -8 -o test03go_i.go test03go.l
	go build test03go_i.go
