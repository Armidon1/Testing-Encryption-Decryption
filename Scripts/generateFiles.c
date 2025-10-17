#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#define SIZE_1 (16)             //16B
#define SIZE_2 (20*1024)        //20KB 
#define SIZE_3 (2*1024*1024)    //2MB 

void fill_document(FILE* fd, int size){ 
    for (int i=0; i<size;i++) putc('A',fd); //i could use fwrite with a string of 3MB size to be more efficient 
}

int ensure_dir_exists(const char *path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode)) return 0;
        errno = ENOTDIR;
        return -1;
    }
    if (mkdir(path, 0755) == 0 || errno == EEXIST) return 0;
    return -1;
}

int main(){
    //checks if ../file directory exists
    if (ensure_dir_exists("../files") != 0) {
        fprintf(stderr, "ERROR: cannot ensure ../files directory: %s\n", strerror(errno));
        return 1;
    }
    
    //16B
    FILE* fd = fopen("../files/firstFile.txt", "w");
    if (!fd){
        fprintf(stderr, "ERROR: failed to open the file in ../files/firstFile.txt");
        return 1;
    }
    fill_document(fd, SIZE_1); 
    fclose(fd);   

    //20KB
    fd = fopen("../files/secondFile.txt", "w");
    if (!fd){
        fprintf(stderr, "ERROR: failed to open the file in ../files/secondFile.txt");
        return 1;
    }
    fill_document(fd, SIZE_2); 
    fclose(fd);

    //2MB
    fd = fopen("../files/thirdFile.txt", "w");
    if (!fd){
        fprintf(stderr, "ERROR: failed to open the file in ../files/thirdFile.txt");
        return 1;
    }
    fill_document(fd, SIZE_3); 
    fclose(fd);

    //finishing
    return 0;
}