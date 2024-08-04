#include <Arduino.h>
#include <esp_bcrypt.h>

ESP_BCrypt bcrypt;

void setup() {
    Serial.begin(115200);
	delay(3000);
    while (!Serial);

    String password = "mySecretPassword";
    String wrongpassword = "NotmySecretPassword";

    String salt = bcrypt.generateSalt();
    String hash = bcrypt.bcrypt(password, salt);

    Serial.println("Password: " + password);
    Serial.println("Salt: " + salt);
    Serial.println("Hash: " + hash);

    bool isValid = bcrypt.bcryptVerify(password, hash);
    Serial.println("Verification with good password: " + String(isValid));

    bool isValidBad = bcrypt.bcryptVerify(wrongpassword, hash);
    Serial.println("Verification with bad password: " + String(isValidBad));
}

void loop() {
    // Nothing to do here
}