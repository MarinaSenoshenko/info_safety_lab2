#include <stdio.h>
#include <string.h>
#include "malloc.h"

#define LEN 256

void swap(unsigned char *first_string, unsigned char *second_string) {
    unsigned char temp = *first_string;
    *first_string = *second_string;
    *second_string = temp;
}

void init(char *key, int key_len, char *s_block) {
    int i, j = 0;

    for (i = 0; i < LEN; i++) {
        s_block[i] = i;
    }

    for (i = 0; i < LEN; i++) {
        j = (j + s_block[i] + key[i % key_len]) % LEN;
        swap((unsigned char*)&s_block[i], (unsigned char*)&s_block[j]);
    }
}

void rc4(char *plain_text, int text_length, char *key, int key_length, char *cipher_text) {
    char s_block[LEN], s_block_x, s_block_y;
    int x = 0, y = 0, i;

    init(key, key_length, s_block);

    for (i = 0; i < text_length; i++) {
        // Pseudo-Random Generation Algorithm
        x = (x + 1) % LEN;
        s_block_x = s_block[x];

        y = (y + s_block_x) % LEN;
        s_block_y = s_block[y];

        swap((unsigned char*)&s_block_x, (unsigned char*)&s_block_y);

        // make xor
        cipher_text[i] = plain_text[i] ^ s_block[(s_block_x + s_block_y) % LEN];
    }
}

int main() {
    char *plain_text = (char*)malloc(sizeof(char) * LEN);
    char *cipher_text = (char*)malloc(sizeof(char) * LEN);
    char *key = (char*)malloc(sizeof(char) * LEN);

    printf("Enter the string:");
    fgets(plain_text, LEN, stdin);

    printf("Enter key: ");
    fgets(key, LEN, stdin);

    int text_length = strlen(plain_text) - 1;
    int key_length = strlen(key) - 1;

    rc4(plain_text, text_length, key, key_length, cipher_text);

    printf("Encrypted string: %s\n", cipher_text);

    rc4(cipher_text, text_length, key, key_length, plain_text);

    printf("Decrypted string: %s\n", plain_text);

    return 0;
}
