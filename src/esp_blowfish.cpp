#include "esp_blowfish.h"

ESP_Blowfish::ESP_Blowfish() {
    // Initialize Blowfish constants
}

void ESP_Blowfish::initialize(const uint8_t* key, size_t keyLength) {
    // Initialize P-array and S-boxes with constants
    // ...

    // XOR key with P-array
    // ...
    keySchedule(key, keyLength);
}

void ESP_Blowfish::encrypt(uint32_t& l, uint32_t& r) {
    for (int i = 0; i < 16; i += 2) {
        l ^= pArray[i];
        r ^= feistelFunction(l);
        r ^= pArray[i + 1];
        l ^= feistelFunction(r);
    }
    l ^= pArray[16];
    r ^= pArray[17];
    uint32_t temp = l;
    l = r;
    r = temp;
}

void ESP_Blowfish::decrypt(uint32_t& l, uint32_t& r) {
    for (int i = 16; i > 0; i -= 2) {
        l ^= pArray[i + 1];
        r ^= feistelFunction(l);
        r ^= pArray[i];
        l ^= feistelFunction(r);
    }
    l ^= pArray[1];
    r ^= pArray[0];
    uint32_t temp = l;
    l = r;
    r = temp;
}

uint32_t ESP_Blowfish::feistelFunction(uint32_t x) {
    uint8_t a = (x >> 24) & 0xFF;
    uint8_t b = (x >> 16) & 0xFF;
    uint8_t c = (x >> 8) & 0xFF;
    uint8_t d = x & 0xFF;
    return ((sBoxes[0][a] + sBoxes[1][b]) ^ sBoxes[2][c]) + sBoxes[3][d];
}

void ESP_Blowfish::keySchedule(const uint8_t* key, size_t keyLength) {
    // Initialize P-array and S-boxes with constants
    // ...

    // XOR key with P-array
    for (int i = 0, j = 0; i < 18; ++i) {
        uint32_t data = 0;
        for (int k = 0; k < 4; ++k) {
            data = (data << 8) | key[j];
            j = (j + 1) % keyLength;
        }
        pArray[i] ^= data;
    }

    // Encrypt all-zero block and replace P-array entries
    uint32_t l = 0, r = 0;
    for (int i = 0; i < 18; i += 2) {
        encrypt(l, r);
        pArray[i] = l;
        pArray[i + 1] = r;
    }

    // Encrypt all-zero block and replace S-boxes entries
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 256; j += 2) {
            encrypt(l, r);
            sBoxes[i][j] = l;
            sBoxes[i][j + 1] = r;
        }
    }
}
