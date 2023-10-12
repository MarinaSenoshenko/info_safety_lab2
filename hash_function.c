#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 16
#define KEY "secret_key"

void hash(const char *message, int message_length, char *hash_value) {
    int i, j;
    int key_length = strlen(KEY);

    memset(hash_value, 0, BLOCK_SIZE);

    for (i = 0; i < message_length; i += BLOCK_SIZE) {
        for (j = 0; j < BLOCK_SIZE; j++) {
            hash_value[j] ^= message[i + j % key_length];
        }
    }
}

int main() {
    char original_message[] = "Hello, world!";
    char modified_message[] = "Hello, modified!";

    char original_hash_value[BLOCK_SIZE];
    char modified_hash_value[BLOCK_SIZE];

    hash(original_message, strlen(original_message), original_hash_value);
    hash(modified_message, strlen(modified_message), modified_hash_value);

    printf("Original Hash value: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x", (unsigned char)original_hash_value[i]);
    }
    printf("\n");

    printf("Modified Hash value: ");
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x", (unsigned char)modified_hash_value[i]);
    }
    printf("\n");

    return 0;
}