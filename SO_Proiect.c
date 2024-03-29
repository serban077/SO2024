#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc , char *argv[])
{
    DIR *dir = opendir(argv[1]);
    struct stat st;
    struct dirent *d;

    if(argc != 2){
        fprintf(stderr , "numar incorect de variabile\n");
        exit(EXIT_FAILURE);
    }

    if(opendir(argv[1]) == NULL){
        fprintf(stderr , "Eroare la deschiderea directorului\n");
        exit(EXIT_FAILURE);
    }

    if(stat(argv[1] , &st) < 0){
        fprintf(stderr , "eroare\n");
        exit(EXIT_FAILURE);
    }

    while((d = readdir(dir)) != NULL){
        printf("%s\n" , d->d_name);

    }

    

    return 0;
}