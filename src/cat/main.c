#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
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
  int fds[filec];

  // Open each file, save fd
  for (i = 0; i < filec; i++) {
    fd = open(filev[i], O_RDONLY);
    if (fd == -1) {
      display_errors("Opening file", errno);
      return -1;
    }
    fds[i] = fd;
  }

  // Print the contents of each file to stdout
  int current_file, file_size, rd_bytes;
  for (i = 0; i < filec; i++) {
    current_file = fds[i];

    // Get file size and return to file start
    file_size = lseek(current_file, 0, SEEK_END);
    if (file_size == -1) {
      display_errors("Getting file size", errno);
      return -1;
    }
    lseek(current_file, 0, 0);
    if (file_size == -1) {
      display_errors("Seeking to file start", errno);
      return -1;
    }

    // Read contents of file into buffer and print it
    char* file_contents = (char*)malloc(sizeof(char) * file_size);
    rd_bytes = read(current_file, file_contents, file_size);
    if (rd_bytes == -1) {
      display_errors("Reading file contents", errno);
      free(file_contents);
      return -1;
    }
    printf("%s\n", file_contents);
    free(file_contents);
  }

  return 0;
}

// Go through each arg, ensuring that the files exist;
// if so, add them to our array of pollfd for use
// with poll(); else, die.
int i, fd;
struct stat stat_info;
char* pipe_path;
struct pollfd* pollfds =
    (struct pollfd*)malloc((sizeof(struct pollfd) * (unsigned long)argc));
int num_pipes = argc - 1;
char** pipes = &argv[1];

for (i = 0; i < num_pipes; i++) {
  // Open the pipe
  pipe_path = pipes[i];
  fd = open(pipe_path, O_RDONLY | O_NONBLOCK);
  // printf("fd for pipe %d: %d\n", i, fd);
  if (fd == -1) {
    display_errors(pipe_path, errno);
    return -1;
  }

  // Test it's a pipe
  int stat_err;
  stat_err = fstat(fd, &stat_info);
  if (stat_err == -1) {
    display_errors(pipe_path, errno);
    return -1;
  }
  if (!(S_ISFIFO(stat_info.st_mode))) {
    printf("Error: %s - not a named pipe.\n", pipe_path);
  } else {
    // Create pollfd and add it
    pollfds[i].fd = fd;
    pollfds[i].events = POLLIN;
    printf("FIFO %d opened and ready.\n", i);
  }
}

// Open the logfile for writing
int writelog;
writelog = open("writelog.txt", O_WRONLY | O_CREAT);
if (writelog == -1) {
  display_errors("writelog: ", errno);
  return -1;
}
printf("Log file open.\n");

// Poll/read loop until all pipes EOF
int ret;
char pipe_text[10];
long err;
printf("Polling %d pipes...\n", num_pipes);
while ((ret = poll(pollfds, (nfds_t)num_pipes, TIMEOUT))) {
  for (i = 0; i < num_pipes; i++) {
    struct pollfd current_pipe = pollfds[i];
    if (current_pipe.revents && POLLIN) {
      printf("Reading from pipe %d...\n", i);
      // read 10 bytes at a time because I can't figure out how to determine
      // how much is in a pipe
      char prefix[] = {(char)i + 48, ':', ' '};
      err = write(writelog, prefix, 3);
      if (err == -1) {
        display_errors("Couldn't write prefix to logfile; ", errno);
      }
      while ((err = read(current_pipe.fd, pipe_text, 10))) {
        printf("Read from pipe: %s\n", pipe_text);
        if (err == -1) {
          display_errors("Couldn't read from pipe; ", errno);
        }
        err = write(writelog, pipe_text, 10);
        if (err == -1) {
          display_errors("Couldn't write to logfile; ", errno);
        }
      }
      err = write(writelog, "\n", 1);
      if (err == -1) {
        display_errors("Couldn't write newline to logfile; ", errno);
      }
    }
  }
}
printf("ret: %d\n", ret);

// Cleanup and done
free(pollfds);
return 0;
}
