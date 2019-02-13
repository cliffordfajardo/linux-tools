include ../Makefile

cat: clean setup
	echo "testfile one" > $(BIN_DIR)/testfile1.txt
	echo "testfile two" > $(BIN_DIR)/testfile2.txt
	$(CC) $(CFLAGS) $(COMMON_INC) main.c $(COMMON_SRC) -o $(BIN_DIR)/$@
	$(VALGRIND) $(BIN_DIR)/$@ $(BIN_DIR)/testfile1.txt $(BIN_DIR)/testfile2.txt
