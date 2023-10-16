# MCCWC - Monk Coding Challenge 1: wc tool


Clone of wc UNIX command tool. Done as part of [Monk Coding Challenge](https://shellmonk.io/posts/monk-coding-challenge-0-prelude/) for [CodingChallenges.FYI](https://codingchallenges.fyi).

Challenge specification: [Write Your Own wc Tool](https://codingchallenges.fyi/challenges/challenge-wc)

## Blog Post

[Monk Coding Challenge 1: wc tool](https://shellmonk.io/posts/monk-coding-challenge-1-wc/)


## Prerequisites

- POSIX compliant operating system (any Linux will do)
- gcc

## Build and run

```bash
make
./mccwc
```

## Usage
```
Usage: mccwc [OPTION] ... [FILE]

Print newline, word, and byte counts for FILE. A word is a non-zero-length sequence of printable characters delimited by white space.

-c      print the byte count
-m      print the character count
-l      print the newline count
-w      print the wird count

Without FILE, read standard input
```

## License

This project is licensed under the [MIT License](http://opensource.org/licenses/MIT).