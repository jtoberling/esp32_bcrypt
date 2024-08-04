#include "esp_bcrypt.h"
#include <mbedtls/md.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>

ESP_BCrypt::ESP_BCrypt() {
    // Initialize Blowfish state
    memset(pArray, 0, sizeof(pArray));
    memset(sBoxes, 0, sizeof(sBoxes));
}

String ESP_BCrypt::generateSalt(int cost) {
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    unsigned char salt[ESPBC_CONST_SALT_LENGTH];
    char saltString[29];

    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);

    mbedtls_ctr_drbg_random(&ctr_drbg, salt, ESPBC_CONST_SALT_LENGTH);

    snprintf(saltString, sizeof(saltString), "$2a$%02d$%s", cost, base64_encode(salt, ESPBC_CONST_SALT_LENGTH).c_str());

    mbedtls_entropy_free(&entropy);
    mbedtls_ctr_drbg_free(&ctr_drbg);

    return String(saltString);
}

String ESP_BCrypt::bcrypt(const String& password, const String& salt) {
    int cost = extractCostFactor(salt);
    initializeBlowfishState(password, salt, cost);
    String hash = encryptMagicValue();
    return hash;
}

bool ESP_BCrypt::bcryptVerify(const String& password, const String& hash) {
    String salt = extractSalt(hash);
    String generatedHash = bcrypt(password, salt);
    return compareHashes(generatedHash, hash);
}

void ESP_BCrypt::initializeBlowfishState(const String& password, const String& salt, int cost) {
    // Implementation of Blowfish state initialization
    // This is a placeholder for the actual Blowfish initialization code
}

String ESP_BCrypt::encryptMagicValue() {
    // Implementation of encrypting the magic value
    // This is a placeholder for the actual encryption code
    return String("OrpheanBeholderScryDoubt_encrypted");
}

int ESP_BCrypt::extractCostFactor(const String& salt) {
    int cost;
    sscanf(salt.c_str(), "$2a$%d$", &cost);
    return cost;
}

String ESP_BCrypt::extractSalt(const String& hash) {
    return hash.substring(0, 29);
}

bool ESP_BCrypt::compareHashes(const String& hash1, const String& hash2) {
    return hash1.equals(hash2);
}

String base64_encode(const unsigned char* data, size_t length) {
    // Implementation of base64 encoding
    // This is a placeholder for the actual base64 encoding code
    return String("base64_encoded_salt");
}