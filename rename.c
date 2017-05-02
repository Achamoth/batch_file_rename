#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

//Finds an instance of 'find' in 'curName', and replaces it with 'replace'
//Returns a pointer to new string (allocated on heap, so needs to be freed)
char *replace(char *str, char *find, char *replace) {
    char *new = (char *) malloc(sizeof(char));
    int newLen = 0;

    //First copy section of 'str' before 'find' starts
    char *substringStart = strstr(str, find);
    char *ind = str;
    while(ind != substringStart) {
        new[newLen++] = *ind;
        new = (char *) realloc(new, sizeof(char) * (newLen+1));
        ind++;
    }

    //Now, replace the instance of 'find' with 'replace'
    int replaceLen = strlen(replace);
    for(int i=0; i<replaceLen; i++) {
        new[newLen++] = replace[i];
        new = (char *) realloc(new, sizeof(char) * (newLen+1));
    }

    //Now, copy section of 'str' after 'find' ends
    ind = substringStart + strlen(find);
    while (*ind != '\0') {
        new[newLen++] = *ind;
        new = (char *) realloc(new, sizeof(char) * (newLen+1)); //TODO: Error here
        ind++;
    }
    new[newLen] = '\0';

    return new;
}

int main(int argc, char **argv) {
    //Ensure correct number of arguments (need a string to search for, and a string to replace with)
    if(argc != 3) {
        perror("2 arguments required");
        exit(EXIT_FAILURE);
    }

    //Get two specified strings
    char *find = strdup(argv[1]);
    char *new = strdup(argv[2]);

    //Set up structs for searching directory
    DIR *dp;
    struct dirent *ep;
    dp = opendir("./");

    //Ensure directory was opened successfully
    if (dp==NULL) {
        perror("Couldn't open directory");
        exit(EXIT_FAILURE);
    }

    //Loop through all entries in directory
    while ((ep = readdir(dp))) {
        //Get filename of current entry
        char *curName = strdup(ep -> d_name);
        //Check if the filename contains the specified string
        if(strstr(curName, find)) {
            //Filename contains the specified string
            char *newName = replace(curName, find, new);
            rename(curName, newName);
            free(newName);
        }
        free(curName);
    }

    free(find);
    free(new);
    return 1;
}
