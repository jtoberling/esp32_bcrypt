#ifndef ESP_BLOWFISH_H
#define ESP_BLOWFISH_H

#include <Arduino.h>

class ESP_Blowfish {
public:
    ESP_Blowfish();
    void initialize(const uint8_t* key, size_t keyLength);
    void encrypt(uint32_t& l, uint32_t& r);
    void decrypt(uint32_t& l, uint32_t& r);

private:
    uint32_t feistelFunction(uint32_t x);
    void keySchedule(const uint8_t* key, size_t keyLength);

    uint32_t pArray[18];
    uint32_t sBoxes[4][256];
};

#endif // ESP_BLOWFISH_H
