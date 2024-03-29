### General vars
SHELL :=/bin/bash
CONTAINER_NAME :=linux_tools
DIR :=/linux_tools
BIN_DIR :=$(DIR)/bin

### Uncomment this to run Clang's static analyzer while building; this makes the build slower.
ANALYZER:=scan-build --status-bugs

### Compiler settings
CC:=clang
CFLAGS :=-std=gnu11 -g -lm
WARNINGS :=-Weverything -Werror
COMMON_INC :=-I $(DIR)/common/include
COMMON_SRC := $(DIR)/common/src/*.c
COMPILE:=$(ANALYZER) $(CC) $(CFLAGS) $(WARNINGS) $(EXTRA_FLAGS) $(INCLUDES) $(LIBS)

### Valgrind target for memory analysis
VALGRIND := valgrind -q --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=42

clean:
	reset
	-rm -r $(BIN_DIR)

setup:
	-mkdir $(BIN_DIR)

### Dockerized Linux workspace for consistent environment
docker-clean:
	-docker stop $(CONTAINER_NAME)
	-docker rm $(CONTAINER_NAME)

docker: docker-clean
	docker pull ubuntu
	docker run \
	-dt \
	--name $(CONTAINER_NAME) \
	-v `pwd`:/$(DIR) \
	ubuntu
	docker exec $(CONTAINER_NAME) apt-get update
	docker exec $(CONTAINER_NAME) apt-get install -y make valgrind clang clang-tools cdecl perl

shell:
	docker exec -it $(CONTAINER_NAME) /bin/bash

workspace: docker-clean docker shell




