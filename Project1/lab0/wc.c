#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int isSplit(char c) {
    if (c == '\0' || c == ' ' || c == '\n') return 1;
    else return 0;
}

void wc(FILE* ofile, FILE* infile, char* inname) {
    // your code here:

    char prev = '\0';
    char cur = '\0';
    long int wordCount = 0;
    long int lineCount = 0;
    long int charCount = 0;

    if (inname      /* it is not STDIN */
        && !infile  /* file is not accessable */) {
        fprintf(stderr, "Can't open %s\n", inname);
        return;
    }

    // Count
    while ((cur = (inname ? getc(infile) : getchar())) != EOF) {
        switch (cur) {
        case '\n':
            ++lineCount;
        case ' ':
            if (!isSplit(prev))
                ++wordCount;
            break;
        default:
            break;
        }
        prev = cur;
        ++charCount;
    }
    // Handle ...EOF
    if (cur == EOF && !isSplit(prev)) {
        ++wordCount;
        ++lineCount;
    }

    // Output the message
    if (ofile != NULL) fprintf(ofile, "%4ld %4ld %4ld %4s\n", lineCount, wordCount, charCount, inname);
    else printf("%4ld %4ld %4ld %4s\n", lineCount, wordCount, charCount, inname ? inname : "STDIN");

    // Close files
    if (ofile != NULL) fclose(ofile);
    if (infile != NULL) fclose(infile);
}

int main(int argc, char* argv[]) {

    char* out_file_name = NULL;
    char* inname = NULL;
    if (argc == 1) wc(NULL, NULL, NULL);
    else if (argc == 2) {
        inname = argv[1];
        FILE* infile = fopen(inname, "r");
        if (!infile) {
            perror("Input file opening failed");
            exit(1);
        }
        wc(NULL, infile, inname);
    }
    else if (argc == 3) {
        inname = argv[1];
        FILE* infile = fopen(inname, "r");
        if (!infile) {
            perror("Input file opening failed");
            exit(1);
        }
        FILE* out_file = fopen(argv[2], "w");
        if (!out_file) {
            perror("Output file opening failed");
            exit(1);
        }
        wc(out_file, infile, inname);
    }
    else {
        printf("arguments error\n");
        exit(1);
    }
    return 0;
}