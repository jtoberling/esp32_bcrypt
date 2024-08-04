#ifndef BASE64_UTILS_H
#define BASE64_UTILS_H

#include <Arduino.h>

String base64_encode(const unsigned char* data, size_t length);

#endif // BASE64_UTILS_H