#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <wctype.h>
#include <locale.h>
#include <wchar.h>

/**
 * @brief Counter structure containing counted bytes, characters, words and lines
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

  // current wide character
  wint_t wc;

  // wopen = word open
  // meaning that word is being read
  bool wopen = false;

  // new fancy way of initializing structs
  Counter cnt = { .bytes = 0, .chars = 0, .words = 0, .lines = 0 };

  // TODO: This can be improved by buffering,
  //       but reading one char at a time 
  //       works for now
  while(WEOF != (wc = fgetwc(fd))) {
    if(wc == wctob(wc)) {
      cnt.bytes++;
    } else {
      cnt.bytes += 4;
    }
    cnt.chars++;

    // if new line, add newline, obviously
    if(wc == L'\n') cnt.lines++;

    // check if character is whitespace - ' ', \n, \t, \r, etc.
    bool space = iswspace(wc);

    // counting the words
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

/**
 * @brief Print help
 */
void print_help() {
  printf("Usage: mccwc [OPTION] ... [FILE]\n\n");
  printf("Print newline, word, and byte counts for FILE. A word is a non-zero-length sequence of printable characters delimited by white space.\n\n");
  printf("-c\tprint the byte count\n");
  printf("-m\tprint the character count\n");
  printf("-l\tprint the newline count\n");
  printf("-w\tprint the wird count\n\n");
  printf("Without FILE, read standard input\n");
  exit(EXIT_SUCCESS);
}


int main(int argc, char ** argv) {

  // Reset all flags
  bool cflag = false, wflag = false, lflag = false, mflag = false, hflag = false;
  char *filename = NULL;

  // One file descriptor declaration can be used both for FILE and standard input
  // In UNIX world, everything is a file
  FILE *inputfd;

  // Locale must be set in order to properly work with wide characters
  setlocale(LC_ALL, "");

  int c;
  while ((c = getopt(argc, argv, "-mcwlh")) != -1) {
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
    case 'h':
      hflag = true;
      break;
    case '?':
      fprintf(stderr, "[ERROR] Unknown option: %c\n", optopt);
      exit(EXIT_FAILURE);
    case 1:
      filename = optarg;
      break;
    }
  }

  if(hflag) {
    print_help();
  }

  // if no flags - c, w and l are default
  if(!cflag && !wflag && !lflag && !mflag) {
    cflag = wflag = lflag = true;
  }

  // if file is provided, read it
  // if not, default to standard input
  if(filename) {
    inputfd = fopen(filename, "r");
    if(inputfd == NULL) {
      fprintf(stderr, "[ERROR] Cannot open file: %s\n", filename);
      exit(EXIT_FAILURE);
    }
  } else {
    inputfd = stdin;
  }

  // Very nice
  Counter counter = read_from_fd(inputfd);

  if(lflag) printf("  %ld", counter.lines);
  if(wflag) printf("  %ld", counter.words);
  if(mflag) printf("  %ld", counter.chars);
  if(cflag) printf("  %ld", counter.bytes);
  if(filename) printf("  %s", filename);

  printf("\n");
  
  // Be polite to your OS, always return 0
  return 0;
}

