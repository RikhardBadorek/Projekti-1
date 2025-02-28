#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

//function to read the file
//input file or user input from stdin
char **read_file(FILE *input, int *cnt_out) {
    int cap = 10;
    int cnt = 0;
    char **lines = malloc(cap * sizeof(char *));

    if(lines == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t n; 

    //reads each line and duplicates them for later use
    while((n = getline(&line, &len, input)) != -1){

        if (n > 0 && line[n-1] == '\n') {
            line[n-1] = '\0';
        }

        char *copy = strdup(line);

        if(copy == NULL){
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        if(cnt >= cap){
            cap = cap * 3;
            char **t = realloc(lines, cap * sizeof(char *));

            if(t == NULL){
                fprintf(stderr,"malloc failed\n");
                exit(1);
            }
            lines = t;
        }
        lines[cnt++] = copy;

    }
    free(line);
    *cnt_out = cnt;
    return lines;
}


int main(int argc, char *argv[]) {
    // When no arguments runs this
    if(argc == 1) {
        printf("Enter the text that will be reversed: ");
        int cnt;
        char **lines = read_file(stdin, &cnt);

        //print to stdout
        printf("\nReversed text:\n");
        for(int i = cnt-1; i >= 0; i--){
            printf("%s\n", lines[i]);
            free(lines[i]);
        }
        free(lines);

    } else if(argc == 2) {
        // if only one argument is given
        FILE *read = fopen(argv[1], "r");
        if(read == NULL){
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        int cnt;
        char **lines = read_file(read, &cnt);
        fclose(read);

        //print to stdout
        printf("\nReversed text:\n");
        for(int i = cnt-1; i >= 0; i--){
            printf("%s\n", lines[i]);
            free(lines[i]);
        }

        free(lines);

    } else if(argc == 3) {
        //if two arguments are given
        if(strcmp(argv[1], argv[2]) == 0){
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }

        FILE *read = fopen(argv[1], "r");

        if(read == NULL){
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        int cnt;
        char **lines = read_file(read, &cnt);
        fclose(read);

        //open the output file and write to it
        FILE *write = fopen(argv[2], "w");

        if(write == NULL){
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }

        for(int i = cnt-1; i >= 0; i--){
            fprintf(write, "%s\n", lines[i]);
            free(lines[i]);
        }
        free(lines);
        fclose(write);
    } else {
        //if too many arguments
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }


    return 0;
}