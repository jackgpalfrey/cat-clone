#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Flags {
	// A implemented with -vET
	int b;
	// e implemented with -vE
	int E;
	int n;
	int s;
	// t implemented with -vT
	int T; // NOT IMPLEMENTED
	int v; // NOT IMPLEMENTED
};

void catFile(FILE *fp, struct Flags *flags, int *lineCount) {
	int lineLen = 0;
	int prevLineEmpty = 0;

	int fd = fileno(fp);
	char buf[128];

	read(fd, buf, 256);

	char *cur = buf;

	while (*cur != '\0') {
		if (lineLen == 0){
			// CAN ASSUME THIS IS A NEW LINE
			if (flags->s && *cur == '\n') {
				if (prevLineEmpty) {
					*cur++;
					continue;
				}
				prevLineEmpty = 1;
			} else {
				prevLineEmpty = 0;
			}

			if (flags->b){
				if (*cur != '\n')  printf("     %i\t", ++*lineCount);
			}
			else if (flags->n) printf("     %i\t", ++*lineCount);
		}


		lineLen++;


		if (*cur == '\n'){
			// HANDLE NEW LINE STUFF
			if (flags->E) printf("$");
			
			printf("\n");


			lineLen = 0;


		} else if (flags->T && *cur == '\t') printf("^I"); 
		else {
			if (*cur < 32 && *cur != 9){
				if (flags->v) printf("^%c",*cur + 64);
			} else { 
				printf("%c", *cur);
			}
		}
		*cur++;
	}

}


int main(int argc, char **argv) {
	// FLAGS
	int opt;
	struct Flags flags = {0,0,0,0,0,0};
	while ((opt = getopt(argc, argv, "AbeEnstTv")) != -1){
		switch(opt){
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
				flags.v =1;
				break;

		}
	}




	int lineCount = 0;

	int curFileIdx = optind;
	if (argc <= curFileIdx){
		while (1){
			catFile(stdin, &flags, &lineCount);
		}
		return 0;
	}	

	while (curFileIdx < argc){
		FILE *fp;

		if (!strcmp(argv[curFileIdx],"-")){
			while (1){
				catFile(stdin, &flags, &lineCount);
			}
		} else {
			fp = fopen(argv[curFileIdx], "rb");
			if (fp == NULL){
				fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[curFileIdx]);
				exit(1);
			}
		}
		catFile(fp, &flags, &lineCount);
		curFileIdx++;
	}


}
