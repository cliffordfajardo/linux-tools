include ../Makefile

all cat:
	echo "testfile one" > testfile1.txt
	echo "testfile two" > testfile2.txt
	$(CC) $(CFLAGS) -I include/$@/ -I $(COMMON_INC) src/$@/main.c $(COMMON_SRC) -o bin/$@
	bin/$@ testfile1.txt testfile2.txt
	@echo "--------------"
	cat testfile1.txt testfile2.txt
	rm testfile*
