#include "base64_utils.h"

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

String base64_encode(const unsigned char* data, size_t length) {
    String encoded = "";
    unsigned char buffer[3];
    size_t i = 0;

    for (i = 0; i < length; i += 3) {
        memset(buffer, 0, 3);
        memcpy(buffer, data + i, (length - i) < 3 ? (length - i) : 3);

        encoded += base64_table[buffer[0] >> 2];
        encoded += base64_table[((buffer[0] & 0x03) << 4) | (buffer[1] >> 4)];
        encoded += (i + 1 < length) ? base64_table[((buffer[1] & 0x0f) << 2) | (buffer[2] >> 6)] : '=';
        encoded += (i + 2 < length) ? base64_table[buffer[2] & 0x3f] : '=';
    }

    return encoded;
}