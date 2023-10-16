#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <wctype.h>
#include <locale.h>
#include <wchar.h>

/**
 * @brief Counter structure containing counted characters, words and lines
 * 
 */
typedef struct  {  
  unsigned long bytes;
  unsigned long chars;
  unsigned long words;
  unsigned long lines;
} Counter;

/**
 * @brief function that calculates chars, words and lines 
 * 
 * @param fd file descriptor we are reading from
 * @return Counter struct
 */
Counter read_from_fd(FILE *fd) {
  wint_t wc;
  bool wopen = false;
  Counter cnt = { .bytes = 0, .chars = 0, .words = 0, .lines = 0 };

  while(WEOF != (wc = fgetwc(fd))) {
    if(wc == wctob(wc)) {
      cnt.bytes++;
    } else {
      cnt.bytes += 2;
    }
    cnt.chars++;
    if(wc == L'\n') cnt.lines++;

    bool space = iswspace(wc);
    if(wopen) {
      if(space) {
        wopen = false;
      }
    } else {
      if(!space) {
        wopen = true;
        cnt.words++;
      }
    }
  }

  return cnt;
}

int main(int argc, char ** argv) {

  bool cflag = false, wflag = false, lflag = false, mflag = false;
  char *filename = NULL;
  FILE *inputfd;

  setlocale(LC_ALL, "");

  int c;
  while ((c = getopt(argc, argv, "-mcwl")) != -1) {
    switch (c) {
    case 'c':
      cflag = true;
      break;
    case 'w':
      wflag = true;
      break;
    case 'l':
      lflag = true;
      break;
    case 'm':
      mflag = true;
      break;
    case '?':
      fprintf(stderr, "[ERROR] Unknown option: %c\n", optopt);
      exit(EXIT_FAILURE);
    case 1:
      filename = optarg;
      break;
    }
  }

  if(!cflag && !wflag && !lflag && !mflag) {
    cflag = wflag = lflag = true;
  }

  

  if(filename) {
    inputfd = fopen(filename, "r");
    if(inputfd == NULL) {
      fprintf(stderr, "[ERROR] Cannot open file: %s\n", filename);
      exit(EXIT_FAILURE);
    }
  } else {
    inputfd = stdin;
  }

  Counter counter = read_from_fd(inputfd);

  if(lflag) printf("%ld ", counter.lines);
  if(wflag) printf("%ld ", counter.words);
  if(mflag) printf("%ld ", counter.chars);
  if(cflag) printf("%ld ", counter.bytes);
  if(filename) printf("%s", filename);

  printf("\n");
  
  return 0;
}

