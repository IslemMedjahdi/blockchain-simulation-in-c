#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *calculateHash(char *data)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, strlen(data));
    SHA256_Final(hash, &sha256);

    char *hash_str = (char *)malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(hash_str + i * 2, "%02x", hash[i]);
    }
    hash_str[SHA256_DIGEST_LENGTH * 2] = '\0';

    return hash_str;
}