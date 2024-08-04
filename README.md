# ESP_BCrypt Library Description

## Overview
The ESP_BCrypt library provides secure password hashing and verification using the bcrypt algorithm, specifically designed for ESP32 microcontrollers. This library is built using mbedtls and ESP32 core functions, ensuring efficient and secure password handling.

## Bcrypt Algorithm
Bcrypt is a password hashing function based on the Blowfish cipher. It includes a salt to protect against rainbow table attacks and is adaptive, meaning the iteration count can be increased to make it slower, thereby mitigating brute-force search attacks.

### Mathematical Description
1. **Salt Generation:**
   - A random 16-byte salt is generated.
   - The salt is formatted as a string with the cost factor (e.g., "$2a$10$").

2. **Hashing Process:**
   - The salt string is parsed to extract the cost factor.
   - The Blowfish state is initialized with the password, salt, and cost factor.
   - The magic value "OrpheanBeholderScryDoubt" is encrypted 64 times using the initialized Blowfish state.
   - The result is formatted as a hash string.

3. **Verification Process:**
   - The salt is extracted from the provided hash.
   - The password is re-hashed using the extracted salt.
   - The generated hash is compared with the provided hash.

## Best Practices
- **Cost Factor:**
  - The cost factor determines the computational cost (time complexity) of the hashing process. A higher cost factor makes the hashing process slower and more resistant to brute-force attacks.
  - Default cost factor: 10 (recommended for most applications).

- **Salt:**
  - A unique salt should be generated for each password to prevent rainbow table attacks.
  - Salt length: 16 bytes.

## Pseudocode

### generateSalt(int cost)
```plaintext
function generateSalt(cost):
    salt = generateRandomBytes(16)
    formattedSalt = formatSaltString(salt, cost)
    return formattedSalt
```

### bcrypt(const char* password, const char* salt)
```plaintext
function bcrypt(password, salt):
    cost = extractCostFactor(salt)
    blowfishState = initializeBlowfishState(password, salt, cost)
    hash = encryptMagicValue(blowfishState)
    formattedHash = formatHashString(hash)
    return formattedHash
```

### bcryptVerify(const char* password, const char* hash)
```plaintext
function bcryptVerify(password, hash):
    salt = extractSalt(hash)
    generatedHash = bcrypt(password, salt)
    return compareHashes(generatedHash, hash)
```

## Implementation Details
- **Dependencies:**
  - mbedtls for cryptographic functions.
  - ESP32 core functions for hardware-specific operations.

- **Optimization:**
  - The library is optimized for ESP32 microcontrollers, ensuring efficient use of resources.

## Example Usage
See `examples/main.ino` for a demonstration of how to use the ESP_BCrypt library to hash and verify passwords on an ESP32 board.

## License
This library is released under the MIT License. See `LICENSE` for more details.

## Changelog
See `CHANGELOG.md` for version history and changes.

## Version
Current version: 1.0.0

## PlatformIO Library Definition
See `library.json` and `library.properties` for PlatformIO library definition and properties.
