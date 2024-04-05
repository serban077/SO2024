#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>


void nr_litere(const char *f, int *nr) {
    int fd = open(f, O_RDONLY , 0);

    if (fd == -1) {
        fprintf(stderr, "Eroare la deschiderea fisierului\n");
        return;
    }

    char buff[1024];
    ssize_t bytes;

    while ((bytes = read(fd, buff, 1024)) > 0) {
        for (ssize_t i = 0; i < bytes; i++) {
            char c = buff[i];
            if (isalpha(c)) {
                if (isupper(c)) {
                    nr[c - 'A']++;
                } else {
                    nr[c - 'a']++;
                }
            }
        }

    }

    close(fd);

}

void dir_recursiv(const char *dir2 , FILE *f)
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
            fprintf(f , "Director: %s\n", d->d_name);
            dir_recursiv(c , f);
        } else {
            fprintf(f , "ID: %ld\n", st.st_ino);
            fprintf(f , "Nume: %s\n", d->d_name);
            fprintf(f , "Ultima modificare: %s", ctime(&st.st_mtime));
            fprintf(f , "Marime fisier: %ld bytes\n", st.st_size);
        }
    
    }

     closedir(dir);

}

int main(int argc , char *argv[])
{
    if(argc != 4){
        fprintf(stderr , "numar incorect de variabile\n");
        exit(EXIT_FAILURE);
    }

    FILE *f = fopen("statistica.txt" , "w");
    if(!f){
        fprintf(stderr , "Eroare la deschiderea fisierului\n");
        exit(EXIT_FAILURE);
    }

    dir_recursiv(argv[1] , f);

    fclose(f);

    return 0;
}