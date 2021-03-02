#include <stdio.h>
#include <ctype.h>

#define MAXFILENAMENUM 100

void wc(FILE *ofile, FILE *infile, char *inname) {

}

int main (int argc, char *argv[]) {
    char fstr[MAXFILENAMENUM];
    if(argc==1)
        fscanf(stdin,"%s",fstr); /* None file is specified. */
    else *fstr = argv[1];

    FILE* fp;
    errno_t err;

    err = fopen(&fp,fstr,"w");

    wc(NULL,fp,fstr);

    fclose(fp);

    return 0;
}
