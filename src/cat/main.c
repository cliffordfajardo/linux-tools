#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "errors.h"

// cat - concatenate files and print on the standard output

int main(int argc, char** argv) {
  if (argc <= 1) {
    printf("usage: ./cat <file> <file>...\n");
    return -1;
  }

  int filec = argc - 1;
  char** filev = &argv[1];
  int* fds = (int*)malloc(sizeof(int*) * (size_t)filec);

  // Open each file, save fd
  int fd, i;
  for (i = 0; i < filec; i++) {
    fd = open(filev[i], O_RDONLY);
    if (fd == -1) {
      display_errors("Opening file", errno);
      free(fds);
      return -1;
    }
    fds[i] = fd;
  }

  // Print the contents of each file to stdout
  int current_file;
  ssize_t rd_bytes;
  off_t file_size;
  for (i = 0; i < filec; i++) {
    current_file = fds[i];

    // Get file size and return to file start
    file_size = lseek(current_file, 0, SEEK_END);
    if (file_size == -1) {
      display_errors("Getting file size", errno);
      free(fds);
      return -1;
    }
    lseek(current_file, 0, 0);
    if (file_size == -1) {
      display_errors("Seeking to file start", errno);
      free(fds);
      return -1;
    }

    // Read contents of file into buffer and print it
    char* file_contents = (char*)malloc(sizeof(char) * (size_t)file_size);
    rd_bytes = read(current_file, file_contents, (size_t)file_size);
    if (rd_bytes == -1) {
      display_errors("Reading file contents", errno);
      free(file_contents);
      free(fds);
      return -1;
    }
    printf("%s", file_contents);
    free(file_contents);
  }

  free(fds);
  return 0;
}
