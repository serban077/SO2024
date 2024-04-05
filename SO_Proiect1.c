#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

void dir_recursiv(const char *dir2)
{
    DIR *dir = opendir(dir2);
    struct stat st;
    struct dirent *d;

    if(opendir(dir2) == NULL){
        fprintf(stderr , "Eroare la deschiderea directorului\n");
        exit(EXIT_FAILURE);
    }

    while((d = readdir(dir)) != NULL){
         if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0){
            continue;
         }

        char c[2048];
        sprintf(c , "%s/%s" , dir2 , d->d_name);

    
        if (stat(c , &st) < 0) {
            fprintf(stderr , "Eroare la accesarea metadatelor pentru %s\n", d->d_name);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            printf("Director: %s\n", d->d_name);
            dir_recursiv(c);
        } else {
            printf("ID: %ld\n", st.st_ino);
            printf("Nume: %s\n", d->d_name);
            printf("Ultima modificare: %s", ctime(&st.st_mtime));
            printf("Marime fisier: %ld bytes\n", st.st_size);
        }
    
    }

     closedir(dir);

}

int main(int argc , char *argv[])
{
    if(argc != 2){
        fprintf(stderr , "numar incorect de variabile\n");
        exit(EXIT_FAILURE);
    }

    dir_recursiv(argv[1]);

    return 0;
}