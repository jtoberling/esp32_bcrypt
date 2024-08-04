#include <Arduino.h>
#include <esp_bcrypt.h>

ESP_BCrypt bcrypt;

void setup() {
    Serial.begin(115200);
    while (!Serial);

    String password = "mySecretPassword";
    String salt = bcrypt.generateSalt();
    String hash = bcrypt.bcrypt(password, salt);

    Serial.println("Password: " + password);
    Serial.println("Salt: " + salt);
    Serial.println("Hash: " + hash);

    bool isValid = bcrypt.bcryptVerify(password, hash);
    Serial.println("Verification: " + String(isValid));
}

void loop() {
    // Nothing to do here
}