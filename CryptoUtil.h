#ifndef _CRYPTO_UTIL_H
#define _CRYPTO_UTIL_H

#include <cstdint>
#include <array>

namespace crypto {

    class Rng {
        std::array<uint32_t, 16> _state;
        uint32_t _counter;

        void reseed();

    public:
        Rng();
        void get(unsigned char *buf, int len);
    };

    // RIPEMD-160 hash function
    void ripemd160(const uint32_t *msg, uint32_t *digest);

    // SHA-256 initialization
    void sha256Init(uint32_t *digest);

    // SHA-256 hash function
    void sha256(const uint32_t *msg, uint32_t *digest);

    // Compute checksum
    uint32_t checksum(const uint32_t *hash);
};

#endif
