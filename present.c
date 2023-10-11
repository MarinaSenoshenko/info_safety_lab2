#include <stdio.h>
#include <stdint.h>

#define ROUNDS 32
#define BLOCK_SIZE 8

uint64_t key = 0x0123456789ABCDEF;
uint64_t roundKeys[ROUNDS];

const uint8_t sBox[16] = {0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2};
const uint8_t invSBox[16] = {0x5, 0xE, 0xF, 0x8, 0xC, 0x1, 0x2, 0xD, 0xB, 0x4, 0x6, 0x3, 0x0, 0x7, 0x9, 0xA};

void generateRoundKeys() {
    uint64_t keyCopy = key;
    for (int i = 0; i < ROUNDS; i++) {
        roundKeys[i] = keyCopy;
        keyCopy = (keyCopy >> 61) | (keyCopy << 3);
        uint8_t keyByte = keyCopy & 0xF;
        keyCopy >>= 4;
        keyCopy ^= sBox[keyByte];
    }
}

void encryptBlock(uint8_t *block) {
    uint64_t state = *(uint64_t *)block;
    for (int i = 0; i < ROUNDS - 1; i++) {
        state ^= roundKeys[i];
        for (int j = 0; j < BLOCK_SIZE; j++) {
            uint8_t stateByte = state & 0xF;
            state >>= 4;
            stateByte = sBox[stateByte];
            state |= ((uint64_t)stateByte << 60);
        }
        state = (state >> 61) | (state << 3);
    }
    state ^= roundKeys[ROUNDS - 1];
    *(uint64_t *)block = state;
}

void decryptBlock(uint8_t *block) {
    uint64_t state = *(uint64_t *)block;
    state ^= roundKeys[ROUNDS - 1];
    for (int i = ROUNDS - 2; i >= 0; i--) {
        state = (state >> 3) | (state << 61);
        for (int j = BLOCK_SIZE - 1; j >= 0; j--) {
            uint8_t stateByte = state >> 60;
            state <<= 4;
            stateByte = invSBox[stateByte];
            state |= stateByte;
        }
        state ^= roundKeys[i];
    }
    *(uint64_t *)block = state;
}

int main() {
    FILE *inputFile = fopen("input.txt", "rb");
    FILE *encryptedFile = fopen("encrypted.txt", "wb");
    FILE *decryptedFile = fopen("decrypted.txt", "wb");

    generateRoundKeys();

    while (!feof(inputFile)) {
        uint8_t block[BLOCK_SIZE];
        size_t bytesRead = fread(block, 1, BLOCK_SIZE, inputFile);
        if (bytesRead > 0) {
            encryptBlock(block);
            fwrite(block, 1, BLOCK_SIZE, encryptedFile);
        }
    }

    fclose(inputFile);
    fclose(encryptedFile);

    encryptedFile = fopen("encrypted.txt", "rb");
    while (!feof(encryptedFile)) {
        uint8_t block[BLOCK_SIZE];
        size_t bytesRead = fread(block, 1, BLOCK_SIZE, encryptedFile);
        if (bytesRead > 0) {
            decryptBlock(block);
            fwrite(block, 1, BLOCK_SIZE, decryptedFile);
        }
    }

    fclose(encryptedFile);
    fclose(decryptedFile);

    return 0;
}
