//
// Created by Kaan Karaman on 10/12/2024.
//

#ifndef MD5ANDARGON2_MD5_H
#define MD5ANDARGON2_MD5_H

#include <string>
#include <cstdint>

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))

class MD5 {
public:
    MD5();

    std::string computeHash(const std::string& input);

private:
    static const uint32_t T[64];
    static const uint32_t shifts[64];

    uint32_t state[4]{};

    void initializeState();                           // Initialize state variables
    static void transform(uint32_t state[4], const uint8_t block[64]); // Main transformation
};


#endif //MD5ANDARGON2_MD5_H
