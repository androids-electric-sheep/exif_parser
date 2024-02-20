#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exif-data.h"

#define HEADER_SIZE 3

void cleanup(void *jpg_file) {
  printf("[+] Cleaning up\n");
  free(jpg_file);
}

char *read_jpg_header(const char *filename) {
  size_t bytes_read = 0;

  printf("[+] Opening %s\n", filename);
  FILE *fd = fopen(filename, "r");
  if (fd == NULL) {
    perror("fopen");
    exit(1);
  }

  fseek(fd, 0, SEEK_END);
  size_t file_size = ftell(fd);
  rewind(fd);
  printf("[+] File is %lu bytes long\n", file_size);

  printf("[+] Allocating space for file\n");
  char *jpg_header = (char *)malloc(HEADER_SIZE + 1);
  if (jpg_header == NULL) {
    perror("malloc");
    fclose(fd);
    exit(1);
  }

  // Read in the first three bytes for the header
  printf("[+] Reading file\n");
  bytes_read = fread(jpg_header, HEADER_SIZE, 1, fd);
  if (bytes_read != 1) {
    perror("fread");
    fclose(fd);
    exit(1);
  }
  jpg_header[HEADER_SIZE] = '\0';
  fclose(fd);

  return jpg_header;
}

int main(int argc, char **argv) {

  if (argc != 2) {
    fprintf(stderr, "[-] Usage: %s INPUT_FILE\n", argv[0]);
    exit(1);
  }

  // Check it's a valid jpg
  char *jpg_header = read_jpg_header(argv[1]);
  if (strcmp(jpg_header, "\xff\xd8\xff")) {
    fprintf(stderr, "[-] Only working on JPG files\n");
    cleanup(jpg_header);
    exit(1);
  }
  cleanup(jpg_header);

  ExifData *data = NULL;
  data = exif_data_new_from_file(argv[1]);
  exif_data_unref(data);
  return 0;
}
