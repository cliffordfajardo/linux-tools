#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1
#define PRINT_USAGE() fprintf(stderr, "usage: tee [-a] <filepath>\n")
#define PRINT_ERROR_AND_QUIT(errno) \
  char* msg = strerror(errno);      \
  perror(msg);                      \
  return -1;

/*
tee - writes stdin until EOF, and simultaneously writes to a file, and
to stdout. Also suppots the -a flag, which supports appending to the given file
instead of overwriting.
*/

int main(int argc, char** argv) {
  int flags;
  char* filepath;

  // Read args; print usage message and quit if they are invalid
  // valid args are `tee filename` and `tee -a filename`
  switch (argc) {
    case 2:
      flags = O_WRONLY | O_CREAT | O_TRUNC;
      filepath = argv[1];
      break;
    case 3:
      if (strcmp(argv[1], "-a") != 0) {
        PRINT_USAGE();
        return -1;
      }
      flags = O_WRONLY | O_APPEND;
      filepath = argv[2];
      break;
    default:
      PRINT_USAGE();
      return -1;
  }

  // attempt to open file for output
  int outfile = open(filepath, flags);
  if (outfile == -1) {
    PRINT_ERROR_AND_QUIT(errno);
  }

  // Read data from stdin until either an error occurs or there's no more data
  char temp[1000] = {0};  // holds chars temporarily prior to being
  ssize_t n;
  while ((n = read(STDIN, temp, 1000)) > 0) {
    write(STDOUT, temp, (size_t)n);
    write(outfile, temp, (size_t)n);
  }
  if (n == -1) {
    PRINT_ERROR_AND_QUIT(errno);
  }
  if (n == 0) {
    if (close(outfile) == -1) {
      PRINT_ERROR_AND_QUIT(errno);
    }
  }
  return 0;
}
