SHELL:=/bin/bash
CC:= clang
CFLAGS:= -std=c11 -g -Weverything -Werror -lm
COMMON_INC:= common/include
COMMON_SRC:= common/src/errors.c

clean:
	-rm bin/*

setup:
	mkdir bin

cat:
	echo "testfile one" > testfile1.txt
	echo "testfile two" > testfile2.txt
	$(CC) $(CFLAGS) -I include/$@/ -I $(COMMON_INC) src/$@/main.c $(COMMON_SRC) -o bin/$@
	bin/$@ testfile1.txt testfile2.txt
	@echo "--------------"
	cat testfile1.txt testfile2.txt
	rm testfile*

echo:
	$(CC) $(CFLAGS) src/$@/*.c -o bin/$@
	bin/$@ "Hello," "world!"
	

