#include "esp_bcrypt.h"
#include "esp_blowfish.h"
#include <mbedtls/base64.h>
#include <mbedtls/md.h>
#include <mbedtls/sha256.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>

ESP_BCrypt::ESP_BCrypt() {
    // Initialize Blowfish state
}

String ESP_BCrypt::generateSalt(int cost) {
    if (cost < 4 || cost > 31) {
        return "";
    }

    uint8_t salt[ESPBC_CONST_SALT_LENGTH];
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    const char *pers = "esp_bcrypt";

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers));

    mbedtls_ctr_drbg_random(&ctr_drbg, salt, ESPBC_CONST_SALT_LENGTH);

    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);

    char encodedSalt[24];
    mbedtls_base64_encode((unsigned char *)encodedSalt, 24, NULL, salt, ESPBC_CONST_SALT_LENGTH);

    char saltString[29];
    snprintf(saltString, 29, "$2b$%02d$%s", cost, encodedSalt);

    return String(saltString);
}

void ESP_BCrypt::initializeBlowfishState(const String& password, const String& salt, int cost) {
    // Decode the Base64 salt
    uint8_t rawSalt[ESPBC_CONST_SALT_LENGTH];
    mbedtls_base64_decode(rawSalt, ESPBC_CONST_SALT_LENGTH, NULL, (const unsigned char*)salt.c_str(), salt.length());

    // Prepare the key from the password
    uint8_t key[password.length()];
    memcpy(key, password.c_str(), password.length());

    // Initialize Blowfish state with the key and salt
    blowfish.initialize(key, password.length());

    // Repeat the key expansion process according to the cost factor
    for (int i = 0; i < (1 << cost); ++i) {
        blowfish.initialize(key, password.length());
    }
}

String ESP_BCrypt::encryptMagicValue() {
    // Magic value "OrpheanBeholderScryDoubt"
    uint32_t magicValue[2] = {0x4f727068, 0x65616e42}; // "Orph" and "eanB"

    // Encrypt the magic value 64 times
    for (int i = 0; i < 64; ++i) {
        blowfish.encrypt(magicValue[0], magicValue[1]);
    }

    // Return the encrypted magic value as a string
    return String((char*)magicValue, 8);
}

String ESP_BCrypt::bcrypt(const String& password, const String& salt) {
    int cost = extractCostFactor(salt);
    String saltPart = extractSalt(salt);

    if (cost < 4 || cost > 31 || saltPart.length() != 22) {
        return "";
    }

    initializeBlowfishState(password, saltPart, cost);

    String hash = encryptMagicValue();

    char hashString[61];
    snprintf(hashString, 61, "$2b$%02d$%s", cost, hash.c_str());

    return String(hashString);
}

bool ESP_BCrypt::bcryptVerify(const String& password, const String& hash) {
    String salt = extractSalt(hash);
    String newHash = bcrypt(password, hash);

    return compareHashes(newHash, hash);
}

void ESP_BCrypt::initializeBlowfishState(const String& password, const String& salt, int cost) {
    // Initialize Blowfish state with password and salt
}

String ESP_BCrypt::encryptMagicValue() {
    // Encrypt the magic value using Blowfish
    return "";
}

int ESP_BCrypt::extractCostFactor(const String& salt) {
    int cost = 0;
    sscanf(salt.c_str(), "$2b$%d$", &cost);
    return cost;
}

String ESP_BCrypt::extractSalt(const String& hash) {
    int pos = hash.indexOf('$', 4);
    if (pos != -1) {
        return hash.substring(pos + 1, pos + 23);
    }
    return "";
}

bool ESP_BCrypt::compareHashes(const String& hash1, const String& hash2) {
    if (hash1.length() != hash2.length()) {
        return false;
    }

    for (size_t i = 0; i < hash1.length(); ++i) {
        if (hash1[i] != hash2[i]) {
            return false;
        }
    }

    return true;
}
