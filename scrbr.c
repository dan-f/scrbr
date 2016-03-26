#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "scrbr.h"

int main(int argc, char **argv)
{
  char opt;
  int sign = 0;
  char *pct = NULL;

  while ((opt = getopt(argc, argv, "i:d:")) != -1) {
    switch (opt) {
    case 'i':
      sign = 1;
      pct = optarg;
      set_brightness(str_to_pct(pct), sign);
      break;
    case 'd':
      sign = -1;
      pct = optarg;
      set_brightness(str_to_pct(pct), sign);
      break;
    default:
      fprintf(stderr, "Usage: %s [-i|-d] n\n", argv[0]);
    }

    if (sign == 0) {
      die("Must provide either increase or decrease option\n");
    }
  }
}

/*
 * Sets the screen brightness to the difference between the current brightness
 * and the specified percentage.
 */
void set_brightness(float pct, int sign)
{
  FILE *max_br_file,
    *cur_br_file,
    *set_br_file;
  int max_br,
    cur_br,
    delta,
    new_br;
  char max_br_buf[BUFSIZE],
    cur_br_buf[BUFSIZE],
    new_br_buf[BUFSIZE];

  // get the maximum brightness
  memset(max_br_buf, 0, sizeof(max_br_buf));
  max_br_file = fopen(max_br_fname, "r");
  safe_read(max_br_file, max_br_buf, BUFSIZE);
  max_br = atoi(max_br_buf);

  // get the current brightness
  memset(cur_br_buf, 0, sizeof(max_br_buf));
  cur_br_file = fopen(cur_br_fname, "r");
  safe_read(cur_br_file, cur_br_buf, BUFSIZE);
  cur_br = atoi(cur_br_buf);

  // compute the new brightness
  delta = (int) max_br * pct * sign;
  new_br = cur_br + delta;
  if (new_br > max_br) {
    new_br = max_br;
  } else if (new_br < 0) {
    new_br = 0;
  }

  // write to the gmux
  set_br_file = fopen(set_br_fname, "w");
  memset(new_br_buf, 0, sizeof(new_br_buf));
  itoa(new_br, new_br_buf);
  safe_write(set_br_file, new_br_buf, sizeof(new_br_buf));

  // clean up
  fclose(max_br_file);
  fclose(cur_br_file);
  fclose(set_br_file);
}

/*
 * Converts an integer to a string and places it in the provided buffer.
 * Only handles positive integers
 */
void itoa(int input, char *buf)
{
  int num = input;
  char *bufptr = buf;
  do {
    *(bufptr++) = (num % 10) + '0';
  } while ((num /= 10) > 0);
  *bufptr = '\0';
  reverse(buf);
}

void reverse(char *buf)
{
  int i, j;
  char tmp;
  for (i = 0, j = strlen(buf) - 1; i < j; ++i, --j) {
    tmp = buf[i];
    buf[i] = buf[j];
    buf[j] = tmp;
  }
}

/*
 * Convert a string percent to a float.
 *
 * E.g. "25" -> 0.25
 */
float str_to_pct(char *pct)
{
  return strtof(pct, NULL) / 100.0;
}

/*
 * Read a specified length of bytes from a file into a buffer.
 *
 * Assumes that the buffer is large enough.
 */
void safe_read(FILE *file, char *buf, int len)
{
  int i = 0,
    bufsize = strlen(buf);
  char cur;
  while ((cur = fgetc(file)) != EOF && i < len) {
    buf[i++] = cur;
  }
}

void safe_write(FILE *file, char *buf, int len)
{
  int i,
    ret;
  for (int i = 0, ret; i < len; ret = fputc(buf[i], file), i++) {
    if (ret == EOF) {
      die("write error");
    }
  }
}

/*
 * Print a message to stderr and exit.
 */
void die(char *msg)
{
  fprintf(stderr, msg);
  exit(EXIT_FAILURE);
}
