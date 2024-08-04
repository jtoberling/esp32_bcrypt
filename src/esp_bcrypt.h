#ifndef ESP_BCRYPT_H
#define ESP_BCRYPT_H

#include <Arduino.h>

#define ESPBC_CONST_DEFAULT_COST 10
#define ESPBC_CONST_SALT_LENGTH 16
#define ESPBC_CONST_HASH_LENGTH 60

class ESP_BCrypt {
public:
    ESP_BCrypt();
    String generateSalt(int cost = ESPBC_CONST_DEFAULT_COST);
    String bcrypt(const String& password, const String& salt);
    bool bcryptVerify(const String& password, const String& hash);

private:
    void initializeBlowfishState(const String& password, const String& salt, int cost);
    String encryptMagicValue();
    int extractCostFactor(const String& salt);
    String extractSalt(const String& hash);
    bool compareHashes(const String& hash1, const String& hash2);

    // Private members for Blowfish state
    uint8_t pArray[18];
    uint8_t sBoxes[4][256];
};

#endif // ESP_BCRYPT_H