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


void dir_recursiv(const char *dir2 , int fd)
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
            char buf[1024];
            sprintf(buf, "Director: %s\n", d->d_name);
            write(fd, buf, strlen(buf));
            dir_recursiv(c , fd);
        } else {
            char buf[1024];
            sprintf(buf, "ID: %ld\n", st.st_ino);
            write(fd, buf, strlen(buf));
            sprintf(buf, "Nume: %s\n", d->d_name);
            write(fd, buf, strlen(buf));
            sprintf(buf, "Ultima modificare: %s", ctime(&st.st_mtime));
            write(fd, buf, strlen(buf));
            sprintf(buf, "Marime fisier: %ld bytes\n", st.st_size);
            write(fd, buf, strlen(buf));
        }
    }

    closedir(dir);
}

void actualizeaza_snapshot(const char *dir_path, const char *dir_iesire) {
    char output_path[1024];
    sprintf(output_path, "%s/snapshots.txt", dir_iesire);
    int fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC , 0644);
    if(fd == -1){
        fprintf(stderr , "Eroare la fisierul snapshot\n");
        exit(EXIT_FAILURE);
    }

    dir_recursiv(dir_path, fd);

    close(fd);
}

int main(int argc , char *argv[])
{
    if(argc < 3 || argc > 10){
        fprintf(stderr , "Numar incorect de argumente\n");
        exit(EXIT_FAILURE);
    }

    char *dir_iesire = NULL;
    int nr_directoare = 0;
    char *directoare[10];

    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-o") == 0) {
            if(i + 1 < argc) {
                dir_iesire = argv[i + 1];
                i++; 
            } else {
                fprintf(stderr, "lipsa dir de iesire\n");
                exit(EXIT_FAILURE);
            }
        } else {
            if(nr_directoare < 10) {
                directoare[nr_directoare] = argv[i];
                nr_directoare++;
            } else {
                fprintf(stderr, "Prea multe directoare\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    
    struct stat st;
    if(stat(dir_iesire, &st) == -1 || !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Directorul de iesire nu este valid\n");
        exit(EXIT_FAILURE);
    }

    
    for(int i = 0; i < nr_directoare; i++) {
        if(stat(directoare[i], &st) == -1 || !S_ISDIR(st.st_mode)) {
            fprintf(stderr, "%s nu este un director valid\n", directoare[i]);
            continue;
        }

        actualizeaza_snapshot(directoare[i], dir_iesire);
    }

    return 0;
}
