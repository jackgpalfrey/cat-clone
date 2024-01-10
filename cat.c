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
    int s;
    // t implemented with -vT
    int T;
    int v;
};


int main(int argc, char *argv[]){
    // Basic file and buffer vars
    FILE *fp;
    const int bufferSize = 4096;
    char buffer[bufferSize];

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


    // For multiple files
    int currentFileIdx = optind;
    // printf("%i, %i", currentFileIdx, argc);
    do {
        if (currentFileIdx == argc){
            fp = stdin;
        } 
        else {
            fp = fopen(argv[currentFileIdx], "rb");
            if (fp == NULL) {
                fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[argc - 1]);
                exit(1);
            }
        } 



        int prevNewLine = 0;
        char *numFString = "     %i\t";

        while (fgets(buffer, bufferSize, fp)) {
            int bufLen = strlen(buffer);
            int isEmptyLine = bufLen <= 1;

            if (flags.s && isEmptyLine && prevNewLine) continue;
            prevNewLine = isEmptyLine;

            if (flags.b && !isEmptyLine) fprintf(stdout, numFString, lineNum++);
            else if (flags.n) fprintf(stdout, numFString, lineNum++); 

            buffer[bufLen - 1] = '\0';
            fprintf(stdout, "%s", buffer);
            if (flags.E) fprintf(stdout, "$");
            fprintf(stdout, "\n");
        }
        currentFileIdx++;
    } while (currentFileIdx < argc);

    fclose(fp);
    return 0;
}
