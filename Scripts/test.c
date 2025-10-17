#define _POSIX_C_SOURCE 199309L //this is added for visual code purposes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>           //we need this to measure time
#include <openssl/evp.h>    // ...  for encrypt/decrypt
#include <openssl/rand.h>   // ...  for randomize KEY and IV
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#define KEY_BITS 128    //128bit for the key
#define KEY_LENGTH (KEY_BITS/8) //in bytes
#define IV_LENGTH 16    //16 bit, because we chose AES/MD4/Camellia works with 16bit blocks

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
    double start = get_real_time_msec();
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "ERROR: EVP_CIPHER_CTX_new failed\n");
        return -1;
    }

    const EVP_CIPHER *cipher = NULL;
    int key_bytes = KEY_LENGTH / 8;

    if (strcmp(algorithm, AES) == 0) {
        cipher = EVP_aes_128_cbc();        /* uses 16-byte key */
    } else if (strcmp(algorithm, CAMELLIA) == 0) {
        cipher = EVP_camellia_128_cbc();
    } else if (strcmp(algorithm, SM4) == 0) {
#ifdef EVP_sm4_cbc
        cipher = EVP_sm4_cbc();
#else
        fprintf(stderr, "ERROR: SM4 not available in this OpenSSL build\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
#endif
    } else {
        fprintf(stderr, "ERROR: unknown algorithm: %s\n", algorithm);
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    if (1 != EVP_EncryptInit_ex(ctx, cipher, NULL, key, iv)) {
        fprintf(stderr, "ERROR: EVP_EncryptInit_ex failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    int block_size = EVP_CIPHER_block_size(cipher);
    unsigned char *out = malloc((size_t)file_size + block_size);
    if (!out) {
        perror("malloc");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    int outlen = 0, tmplen = 0;
    if (file_size > 0) {
        if (1 != EVP_EncryptUpdate(ctx, out, &outlen, (unsigned char*)plaintext, file_size)) {
            fprintf(stderr, "ERROR: EVP_EncryptUpdate failed\n");
            free(out);
            EVP_CIPHER_CTX_free(ctx);
            return -1;
        }
    }

    if (1 != EVP_EncryptFinal_ex(ctx, out + outlen, &tmplen)) {
        fprintf(stderr, "ERROR: EVP_EncryptFinal_ex failed\n");
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    outlen += tmplen;

    /* out contains the ciphertext of length outlen. We free it because caller only measures time. */
    free(out);
    EVP_CIPHER_CTX_free(ctx);

    double elapsed = get_real_time_msec() - start;
    return elapsed;
}

double testing_decryption(const char* algorithm, const char* ciphertext, int file_size, unsigned char* key, unsigned char* iv){
    double start = get_real_time_msec();
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "ERROR: EVP_CIPHER_CTX_new failed\n");
        return -1;
    }

    const EVP_CIPHER *cipher = NULL;
    int key_bytes = KEY_LENGTH / 8;

    if (strcmp(algorithm, AES) == 0) {
        cipher = EVP_aes_128_cbc();
    } else if (strcmp(algorithm, CAMELLIA) == 0) {
        cipher = EVP_camellia_128_cbc();
    } else if (strcmp(algorithm, SM4) == 0) {
#ifdef EVP_sm4_cbc
        cipher = EVP_sm4_cbc();
#else
        fprintf(stderr, "ERROR: SM4 not available in this OpenSSL build\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
#endif
    } else {
        fprintf(stderr, "ERROR: unknown algorithm: %s\n", algorithm);
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    if (1 != EVP_DecryptInit_ex(ctx, cipher, NULL, key, iv)) {
        fprintf(stderr, "ERROR: EVP_DecryptInit_ex failed\n");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    int block_size = EVP_CIPHER_block_size(cipher);
    unsigned char *out = malloc((size_t)file_size + block_size);
    if (!out) {
        perror("malloc");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    int outlen = 0, tmplen = 0;
    if (file_size > 0) {
        if (1 != EVP_DecryptUpdate(ctx, out, &outlen, (unsigned char*)ciphertext, file_size)) {
            fprintf(stderr, "ERROR: EVP_DecryptUpdate failed\n");
            free(out);
            EVP_CIPHER_CTX_free(ctx);
            return -1;
        }
    }

    if (1 != EVP_DecryptFinal_ex(ctx, out + outlen, &tmplen)) {
        /* bad padding / authentication (for AEAD) */
        /* still measure time but report error */
        //fprintf(stderr, "ERROR: EVP_DecryptFinal_ex failed (bad padding?)\n");
        free(out);
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    outlen += tmplen;

    free(out);
    EVP_CIPHER_CTX_free(ctx);

    double elapsed = get_real_time_msec() - start;
    return elapsed;
}

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
void testing_camellia(int number_file, unsigned char* key, unsigned char* iv){
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
void testing_sm4(int number_file, unsigned char* key, unsigned char* iv){
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
    for (int i =1; i<=3; i++){
        for (int j = 1; j<=3; j++){
            if (i==1) testing_aes(j, key, iv);
            else if (i==2) testing_camellia(j,key,iv);
            else if (i==3) testing_sm4(j, key, iv);
            else{
                fprintf(stderr, "ERROR: i should not be here :(\n");
                return 1;
            }
        }
    }
    printf("----------------------------------------------\n");
    return 0;
}
