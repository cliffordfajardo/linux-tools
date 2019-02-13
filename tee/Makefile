include ../Makefile

tee: clean setup
	touch $(BIN_DIR)/foobar.txt
	$(COMPILE) $(COMMON_INC) main.c $(COMMON_SRC) -o $(BIN_DIR)/$@
	echo "foobar" | $(VALGRIND) $(BIN_DIR)/$@ $(BIN_DIR)/foobar.txt
	echo "foobar" | $(VALGRIND) $(BIN_DIR)/$@ -a $(BIN_DIR)/foobar.txt
