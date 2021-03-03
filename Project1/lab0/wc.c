#include <stdio.h>
#include <ctype.h>

#define MAXLINE 10000

int isSplit(char c) {
    if (c == '\0' || c == ' ' || c == '\n') return 1;
    else return 0;
}

void wc(FILE *ofile, FILE *infile, char *inname) {

    char prev = '\0';
    char cur = '\0';
    long int wordCount = 0;
    long int lineCount = 0;
    long int charCount = 0;
    
    if (inname      /* it is not STDIN */
        && !infile  /* file is not accessable */) {
        fprintf(stderr,"Can't open %s\n",inname);
        return ;
    }

    // Count
    while((cur=(inname? getc(infile): getchar()))!=EOF) {
        switch (cur) {
        case '\n': 
            ++lineCount;
        case ' ':
            if(!isSplit(prev))
                ++wordCount;
            break;
        default:
            break;
        }
        prev = cur;
        ++charCount;
    }
    // Handle ...EOF
    if(cur==EOF && !isSplit(prev)) {
        ++wordCount;
        ++lineCount;
    }

    // Output the message
    printf("%4ld %4ld %4ld %4s\n",lineCount,wordCount,charCount,inname?inname:"STDIN");
}

int main (int argc, char *argv[]) {
    
    if(argc==1){/* None file is specified. */
        wc(NULL, NULL, NULL);
    } else {
        FILE* fin;
        fin = fopen(argv[1],"r");
        wc(NULL, fin, argv[1]);
        fclose(fin);
    }

    return 0;
}
