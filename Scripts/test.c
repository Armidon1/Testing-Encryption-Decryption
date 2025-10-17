#define _POSIX_C_SOURCE 199309L //this is added for visual code purposes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>           //we need this to measure time
#include <openssl/evp.h>    // ...  for encrypt/decrypt
#include <openssl/rand.h>   // ...  for randomize KEY and IV
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define KEY_LENGTH 128  //128bit for the key
#define IV_LENGTH 16    //16 bit, because we chose AES works with 16bit blocks

#define FILE_SIZE_1 (16)             //16B
#define FILE_PATH_1 "../files/firstFile.txt"

#define FILE_SIZE_2 (20*1024)        //20KB 
#define FILE_PATH_2 "../files/secondFile.txt"

#define FILE_SIZE_3 (2*1024*1024)    //2MB 
#define FILE_PATH_3 "../files/thirdFile.txt"

#define AES "aes-algorithm"
#define CAMELLIA "camelia-algorithm"
#define SM4 "sm4-algorithm"

double get_real_time_msec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec*1E03 + ts.tv_nsec*1E-06;
}

double testing_encryption(const char* algorithm, const char* plaintext, int file_size, unsigned char* key, unsigned char* iv){
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new(); //preparing contex

    if (EVP_EncryptInit_ex(ctx, plaintext, NULL, key, iv)!=1){
        fprintf(stderr, "ERROR: something went wrong in EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv) inside testing encryption\n");
        return -1;
    }

    
}

double testing_decryption(const char* algorithm, const char* ciphertext, int file_size, unsigned char* key, unsigned char* iv);

void testing_aes(int number_file, unsigned char* key, unsigned char* iv){
    double result;
    int fd;
    char* shared_memory_pointer;
    //opening file
    switch (number_file){
    case 1:
        // open the file
        fd = open(FILE_PATH_1, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "ERROR: Something went wrong opening in opening file in testing_aes\n");
            exit(EXIT_FAILURE);
            break;
        }

        //mmap
        shared_memory_pointer = mmap(NULL,FILE_SIZE_1,PROT_READ,MAP_PRIVATE,fd,0);
        if (shared_memory_pointer== MAP_FAILED){
            perror("ERROR: Something went wring in mmap in testing_aes\n");
            close(fd);
            exit(EXIT_FAILURE);
        }

        //testing
        result = testing_encryption(AES, shared_memory_pointer, FILE_SIZE_1, key, iv);
        printf("Encryption - AES - %d - %lf \n", FILE_SIZE_1, result);
        result = testing_decryption(AES, shared_memory_pointer, FILE_SIZE_1, key, iv);
        printf("Decryption - AES - %d - %lf \n", FILE_SIZE_1, result);
        
        //cleaning memory
        close(fd);
        if (munmap(shared_memory_pointer, FILE_SIZE_1) == -1) {
            perror("ERROR: Something went wrong by unmapping in testing_aes\n");
            exit(EXIT_FAILURE);
        }

        break;

    case 2:
        // open the file
        fd = open(FILE_PATH_2, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "ERROR: Something went wrong opening in opening file in testing_aes\n");
            exit(EXIT_FAILURE);
            break;
        }

        //mmap
        shared_memory_pointer = mmap(NULL,FILE_SIZE_2,PROT_READ,MAP_PRIVATE,fd,0);
        if (shared_memory_pointer== MAP_FAILED){
            perror("ERROR: Something went wring in mmap in testing_aes\n");
            close(fd);
            exit(EXIT_FAILURE);
        }
        //testing
        result = testing_encryption(AES, shared_memory_pointer, FILE_SIZE_2, key, iv);
        printf("Encryption - AES - %d - %lf \n", FILE_SIZE_2, result);
        result = testing_decryption(AES, shared_memory_pointer, FILE_SIZE_2, key, iv);
        printf("Decryption - AES - %d - %lf \n", FILE_SIZE_2, result);
        
        //cleaning memory
        close(fd);
        if (munmap(shared_memory_pointer, FILE_SIZE_2) == -1) {
            perror("ERROR: Something went wrong by unmapping in testing_aes\n");
            exit(EXIT_FAILURE);
        }

        break;
        
    case 3:
        // open the file
        fd = open(FILE_PATH_3, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "ERROR: Something went wrong opening in opening file in testing_aes\n");
            exit(EXIT_FAILURE);
        }

        //mmap
        shared_memory_pointer = mmap(NULL,FILE_SIZE_3,PROT_READ,MAP_PRIVATE,fd,0);
        if (shared_memory_pointer== MAP_FAILED){
            perror("ERROR: Something went wring in mmap in testing_aes\n");
            close(fd);
            exit(EXIT_FAILURE);
        }
        //testing
        result = testing_encryption(AES, shared_memory_pointer, FILE_SIZE_3, key, iv);
        printf("Encryption - AES - %d - %lf \n", FILE_SIZE_3, result);
        result = testing_decryption(AES, shared_memory_pointer, FILE_SIZE_3, key, iv);
        printf("Decryption - AES - %d - %lf \n", FILE_SIZE_3, result);
        
        //cleaning memory
        close(fd);
        if (munmap(shared_memory_pointer, FILE_SIZE_3) == -1) {
            perror("ERROR: Something went wrong by unmapping in testing_aes\n");
            exit(EXIT_FAILURE);
        }

        break;

    default:
        fprintf(stderr, "ERROR: i should not be here :(\n");
        exit(EXIT_FAILURE);
        break;
    }
}
testing_camellia(int number_file, unsigned char* key, unsigned char* iv){
    double result;
    int fd;
    char* shared_memory_pointer;
    //opening file
    switch (number_file){
    case 1:
        // open the file
        fd = open(FILE_PATH_1, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "ERROR: Something went wrong opening in opening file in testing_camellia\n");
            exit(EXIT_FAILURE);
            break;
        }

        //mmap
        shared_memory_pointer = mmap(NULL,FILE_SIZE_1,PROT_READ,MAP_PRIVATE,fd,0);
        if (shared_memory_pointer== MAP_FAILED){
            perror("ERROR: Something went wring in mmap in testing_camellia\n");
            close(fd);
            exit(EXIT_FAILURE);
        }

        //testing
        result = testing_encryption(CAMELLIA, shared_memory_pointer, FILE_SIZE_1, key, iv);
        printf("Encryption - CAMELLIA - %d - %lf \n", FILE_SIZE_1, result);
        result = testing_decryption(CAMELLIA, shared_memory_pointer, FILE_SIZE_1, key, iv);
        printf("Decryption - CAMELLIA - %d - %lf \n", FILE_SIZE_1, result);
        
        //cleaning memory
        close(fd);
        if (munmap(shared_memory_pointer, FILE_SIZE_1) == -1) {
            perror("ERROR: Something went wrong by unmapping in testing_camellia\n");
            exit(EXIT_FAILURE);
        }

        break;

    case 2:
        // open the file
        fd = open(FILE_PATH_2, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "ERROR: Something went wrong opening in opening file in testing_camellia\n");
            exit(EXIT_FAILURE);
            break;
        }

        //mmap
        shared_memory_pointer = mmap(NULL,FILE_SIZE_2,PROT_READ,MAP_PRIVATE,fd,0);
        if (shared_memory_pointer== MAP_FAILED){
            perror("ERROR: Something went wring in mmap in testing_camellia\n");
            close(fd);
            exit(EXIT_FAILURE);
        }
        //testing
        result = testing_encryption(CAMELLIA, shared_memory_pointer, FILE_SIZE_2, key, iv);
        printf("Encryption - CAMELLIA - %d - %lf \n", FILE_SIZE_2, result);
        result = testing_decryption(CAMELLIA, shared_memory_pointer, FILE_SIZE_2, key, iv);
        printf("Decryption - CAMELLIA - %d - %lf \n", FILE_SIZE_2, result);
        
        //cleaning memory
        close(fd);
        if (munmap(shared_memory_pointer, FILE_SIZE_2) == -1) {
            perror("ERROR: Something went wrong by unmapping in testing_camellia\n");
            exit(EXIT_FAILURE);
        }

        break;
        
    case 3:
        // open the file
        fd = open(FILE_PATH_3, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "ERROR: Something went wrong opening in opening file in testing_camellia\n");
            exit(EXIT_FAILURE);
        }

        //mmap
        shared_memory_pointer = mmap(NULL,FILE_SIZE_3,PROT_READ,MAP_PRIVATE,fd,0);
        if (shared_memory_pointer== MAP_FAILED){
            perror("ERROR: Something went wring in mmap in testing_camellia\n");
            close(fd);
            exit(EXIT_FAILURE);
        }
        //testing
        result = testing_encryption(CAMELLIA, shared_memory_pointer, FILE_SIZE_3, key, iv);
        printf("Encryption - CAMELLIA - %d - %lf \n", FILE_SIZE_3, result);
        result = testing_decryption(CAMELLIA, shared_memory_pointer, FILE_SIZE_3, key, iv);
        printf("Decryption - CAMELLIA - %d - %lf \n", FILE_SIZE_3, result);
        
        //cleaning memory
        close(fd);
        if (munmap(shared_memory_pointer, FILE_SIZE_3) == -1) {
            perror("ERROR: Something went wrong by unmapping in testing_camellia\n");
            exit(EXIT_FAILURE);
        }

        break;

    default:
        fprintf(stderr, "ERROR: i should not be here :(\n");
        exit(EXIT_FAILURE);
        break;
    }
}
testing_sm4(int number_file, unsigned char* key, unsigned char* iv){
    double result;
    int fd;
    char* shared_memory_pointer;
    //opening file
    switch (number_file){
    case 1:
        // open the file
        fd = open(FILE_PATH_1, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "ERROR: Something went wrong opening in opening file in testing_sm4\n");
            exit(EXIT_FAILURE);
            break;
        }

        //mmap
        shared_memory_pointer = mmap(NULL,FILE_SIZE_1,PROT_READ,MAP_PRIVATE,fd,0);
        if (shared_memory_pointer== MAP_FAILED){
            perror("ERROR: Something went wring in mmap in testing_sm4\n");
            close(fd);
            exit(EXIT_FAILURE);
        }

        //testing
        result = testing_encryption(SM4, shared_memory_pointer, FILE_SIZE_1, key, iv);
        printf("Encryption - SM4 - %d - %lf \n", FILE_SIZE_1, result);
        result = testing_decryption(SM4, shared_memory_pointer, FILE_SIZE_1, key, iv);
        printf("Decryption - SM4 - %d - %lf \n", FILE_SIZE_1, result);
        
        //cleaning memory
        close(fd);
        if (munmap(shared_memory_pointer, FILE_SIZE_1) == -1) {
            perror("ERROR: Something went wrong by unmapping in testing_sm4\n");
            exit(EXIT_FAILURE);
        }

        break;

    case 2:
        // open the file
        fd = open(FILE_PATH_2, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "ERROR: Something went wrong opening in opening file in testing_sm4\n");
            exit(EXIT_FAILURE);
            break;
        }

        //mmap
        shared_memory_pointer = mmap(NULL,FILE_SIZE_2,PROT_READ,MAP_PRIVATE,fd,0);
        if (shared_memory_pointer== MAP_FAILED){
            perror("ERROR: Something went wring in mmap in testing_sm4\n");
            close(fd);
            exit(EXIT_FAILURE);
        }
        //testing
        result = testing_encryption(SM4, shared_memory_pointer, FILE_SIZE_2, key, iv);
        printf("Encryption - SM4 - %d - %lf \n", FILE_SIZE_2, result);
        result = testing_decryption(SM4, shared_memory_pointer, FILE_SIZE_2, key, iv);
        printf("Decryption - SM4 - %d - %lf \n", FILE_SIZE_2, result);
        
        //cleaning memory
        close(fd);
        if (munmap(shared_memory_pointer, FILE_SIZE_2) == -1) {
            perror("ERROR: Something went wrong by unmapping in testing_sm4\n");
            exit(EXIT_FAILURE);
        }

        break;
        
    case 3:
        // open the file
        fd = open(FILE_PATH_3, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "ERROR: Something went wrong opening in opening file in testing_sm4\n");
            exit(EXIT_FAILURE);
        }

        //mmap
        shared_memory_pointer = mmap(NULL,FILE_SIZE_3,PROT_READ,MAP_PRIVATE,fd,0);
        if (shared_memory_pointer== MAP_FAILED){
            perror("ERROR: Something went wring in mmap in testing_sm4\n");
            close(fd);
            exit(EXIT_FAILURE);
        }
        //testing
        result = testing_encryption(SM4, shared_memory_pointer, FILE_SIZE_3, key, iv);
        printf("Encryption - SM4 - %d - %lf \n", FILE_SIZE_3, result);
        result = testing_decryption(SM4, shared_memory_pointer, FILE_SIZE_3, key, iv);
        printf("Decryption - SM4 - %d - %lf \n", FILE_SIZE_3, result);
        
        //cleaning memory
        close(fd);
        if (munmap(shared_memory_pointer, FILE_SIZE_3) == -1) {
            perror("ERROR: Something went wrong by unmapping in testing_sm4\n");
            exit(EXIT_FAILURE);
        }

        break;

    default:
        fprintf(stderr, "ERROR: i should not be here :(\n");
        exit(EXIT_FAILURE);
        break;
    }
}


int main(){
    //working with the KEY and the IV
    unsigned char key[KEY_LENGTH];
    unsigned char iv[IV_LENGTH];

    if (RAND_bytes(key, KEY_LENGTH) !=1){ //randomizing key
        fprintf(stderr, "ERROR: something went wrong with RAND_bytes(key,...)\n");
        return 1;
    }
    if (RAND_bytes(iv, IV_LENGTH) !=1){ //randomizing IV
        fprintf(stderr, "ERROR: something went wrong with RAND_bytes(iv,...)\n");
        return 1;
    }

    //starting testing
    printf("---------------STARTING TESTING---------------\n");
    printf("TYPE - ALGORITHM - FILE_SIZE - RESULT(ms)\n");
    double result;
    for (int i =1; i<=3; i++){
        for (int j = 1; j<=3; j++){
            if (i==1) testing_aes(j, key, iv);
            else if (i=2) testing_camellia(j,key,iv);
            else if (i=3) testing_sm4(j, key, iv);
            else{
                fprintf(stderr, "ERROR: i should not be here :(\n");
                return 1;
            }
        }
    }
    printf("----------------------------------------------\n");
    return 0;
}
