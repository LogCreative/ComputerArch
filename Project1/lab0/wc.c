#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <ctype.h>

#define MAXLINE 1000

int isSplit(char c) {
    if (c == '\0' || c == ' ' || c == '\n') return 1;
    else return 0;
}

void wc(FILE *ofile, FILE *infile, char *inname) {
    if(infile==NULL){
        fprintf(stderr,"Can't open %s\n",inname);
        return ;
    }

    char prev = '\0';
    char cur = '\0';
    long int wordCount = 0;
    long int lineCount = 0;
    while((cur=getc(infile))!=EOF) {
        switch (cur) {
        case '\n': 
            ++lineCount;
        case ' ':
            if(isSplit(prev)==0)
                ++wordCount;
            break;
        default:
            break;
        }
        prev = cur;
    }
    // Handle ...EOF
    if(cur==EOF && !isSplit(prev)) {
        ++wordCount;
        ++lineCount;
    }

    // Get file size
    fseek(infile,0,SEEK_SET);
    long int begin = ftell(infile);
    fseek(infile,0,SEEK_END);
    long int end = ftell(infile);
    long int bytesize = end - begin;
    
    // Output the message
    printf("%4ld %4ld %4ld %4s\n",lineCount,wordCount,bytesize,inname);
}

int main (int argc, char *argv[]) {
    FILE* fin;
    if(argc==1){
        char fstr[MAXLINE];
        printf("Input the filename:");
        fscanf(stdin,"%s",fstr); /* None file is specified. */
        fin = fopen(fstr,"r");
        wc(NULL,fin, fstr);
    } else{
        fin = fopen(argv[1],"r");
        wc(NULL,fin, argv[1]);
    }
    fclose(fin);

    return 0;
}
