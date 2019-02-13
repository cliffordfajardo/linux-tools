include ../Makefile

echo: clean setup
	$(CC) $(CFLAGS) *.c -o $(BIN_DIR)/$@
	$(VALGRIND) $(BIN_DIR)/$@ "Hello," "world!"


