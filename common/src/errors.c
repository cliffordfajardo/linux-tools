#include "errors.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

void display_errors(const char* const supplemental_info, const int err) {
  const char* const err_desc = strerror(err);
  // TODO: Use perror()?
  printf("Error: %s - %d - %s\n", supplemental_info, err, err_desc);
  return;
}
