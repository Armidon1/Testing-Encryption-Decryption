#define _POSIX_C_SOURCE 199309L //this is added for visual code purposes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>           //we need this to measure time
#include <openssl/evp.h>    // ...  for encrypt/decrypt
#include <openssl/rand.h>   // ...  for randomize KEY and IV

#define KEY_LENGTH 128  //128bit for the key
#define IV_LENGTH 16    //16 bit, because we chose AES works with 16bit blocks

double get_real_time_msec() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec*1E03 + ts.tv_nsec*1E-06;
}

double testing_encryption(const char* algorithm){
    
}
double testing_decryption(const char* algorithm);


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

    //preparing the context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();


    return 0;
}
