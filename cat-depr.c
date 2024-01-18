#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Flags {
    // A implemented with -vET
    int b; // DONE
    // e implemented with -vE
    int E; // DONE
    int n; // DONE
    int s; // DONE
    // t implemented with -vT
    int T;
    int v;
};


void handleFile(FILE *fp, int bufSize){
    char buffer[bufSize];

    int prevNewLine = 0;
    char *numFString = "     %i\t";
    int code;
    char ch;
    char* bufPtr = buffer;

    do {
        code = fgetc(fp);
        if (code == EOF) break;

        ch = (char)code;
        *bufPtr++ = ch;
 
    } while (1);

    printf("%s", buffer);
    bufPtr = buffer;
}

int main(int argc, char *argv[]){
    // Basic file and buffer vars
    FILE *fp;
    const int bufferSize = 128;

    int lineNum = 1;

    // Flags
    int opt;
    struct Flags flags = {0,0,0,0,0,0};
    while ((opt = getopt(argc, argv, "AbeEnstTv")) != -1){
        switch (opt){
            case 'A':
                flags.v = 1;
                flags.E = 1;
                flags.T = 1;
                break;
            case 'b':
                flags.b = 1;
                break;
            case 'e':
                flags.v = 1;
                flags.E = 1;
                break;
            case 'E':
                flags.E = 1;
                break;
            case 'n':
                flags.n = 1;
                break;
            case 's':
                flags.s = 1;
                break;
            case 't':
                flags.v = 1;
                flags.T = 1;
                break;
            case 'T':
                flags.T = 1;
                break;
            case 'v':
                flags.v = 1;
                break;
        }
    }


    int currentFileIdx = optind;
    do {
        if (currentFileIdx == argc || !strcmp(argv[currentFileIdx],"-")){
            fp = stdin;
        } 
        else {
            fp = fopen(argv[currentFileIdx], "rb");
            if (fp == NULL) {
                fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[argc - 1]);
                exit(1);
            }
        } 

        handleFile(fp, bufferSize);

        currentFileIdx++;
    } while (currentFileIdx < argc);

    fclose(fp);
    return 0;
}
