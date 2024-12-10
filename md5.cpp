#include "md5.h"
#include <cstring>
#include <iomanip>
#include <sstream>

// MD5 Constants
const uint32_t MD5::T[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

// Shift by s bits
const uint32_t MD5::shifts[64] = {
        7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

MD5::MD5() {
    initializeState();
}

void MD5::initializeState() {
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
}

std::string MD5::computeHash(const std::string& input) {
    initializeState();
    uint64_t bitLength = input.size() * 8;
    uint8_t buffer[64] = { 0 };

    size_t i;
    for (i = 0; i + 64 <= input.size(); i += 64) {
        std::memcpy(buffer, input.data() + i, 64);
        transform(state, buffer);
    }

    std::memset(buffer, 0, 64);
    std::memcpy(buffer, input.data() + i, input.size() - i);
    buffer[input.size() - i] = 0x80;

    if (input.size() % 64 >= 56) {
        transform(state, buffer);
        std::memset(buffer, 0, 64);
    }

    std::memcpy(buffer + 56, &bitLength, 8);
    transform(state, buffer);

    std::ostringstream result;
    for (unsigned int j : state) {
        result << std::hex << std::setw(2) << std::setfill('0')
               << (j & 0xff) << ((j >> 8) & 0xff)
               << ((j >> 16) & 0xff) << ((j >> 24) & 0xff);
    }

    return result.str();
}

// Transform function
void MD5::transform(uint32_t state[4], const uint8_t block[64]) {
    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint32_t M[16];

    for (int i = 0; i < 16; ++i) {
        M[i] = (block[i * 4]) | (block[i * 4 + 1] << 8) |
               (block[i * 4 + 2] << 16) | (block[i * 4 + 3] << 24);
    }

    for (int i = 0; i < 64; ++i) {
        uint32_t F_temp;
        int g;
        if (i < 16) {
            F_temp = F(b, c, d);
            g = i;
        } else if (i < 32) {
            F_temp = G(b, c, d);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            F_temp = H(b, c, d);
            g = (3 * i + 5) % 16;
        } else {
            F_temp = I(b, c, d);
            g = (7 * i) % 16;
        }

        uint32_t temp = d;
        d = c;
        c = b;
        b = b + ROTATE_LEFT((a + F_temp + T[i] + M[g]), shifts[i]);
        a = temp;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}