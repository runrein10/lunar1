#include "CryptoUtil.h"
#include <cstdint> // For uint32_t
#include <cstdio>  // For printf (if needed for debugging)

static const uint32_t _IV[5] = {
    0x67452301,
    0xefcdab89,
    0x98badcfe,
    0x10325476,
    0xc3d2e1f0
};

static const uint32_t _K0 = 0x5a827999;
static const uint32_t _K1 = 0x6ed9eba1;
static const uint32_t _K2 = 0x8f1bbcdc;
static const uint32_t _K3 = 0xa953fd4e;

static const uint32_t _K4 = 0x7a6d76e9;
static const uint32_t _K5 = 0x6d703ef3;
static const uint32_t _K6 = 0x5c4dd124;
static const uint32_t _K7 = 0x50a28be6;

static uint32_t endian(uint32_t x) {
    return (x << 24) | ((x << 8) & 0x00ff0000) | ((x >> 8) & 0x0000ff00) | (x >> 24);
}

static uint32_t rotl(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

static uint32_t F(uint32_t x, uint32_t y, uint32_t z) {
    return x ^ y ^ z;
}

static uint32_t G(uint32_t x, uint32_t y, uint32_t z) {
    return ((x & y) | (~x & z));
}

static uint32_t H(uint32_t x, uint32_t y, uint32_t z) {
    return ((x | ~y) ^ z);
}

static uint32_t I(uint32_t x, uint32_t y, uint32_t z) {
    return ((x & z) | (y & ~z));
}

static uint32_t J(uint32_t x, uint32_t y, uint32_t z) {
    return (x ^ (y | ~z));
}

static void FF(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e, uint32_t x, uint32_t s) {
    a += F(b, c, d) + x;
    a = rotl(a, s) + e;
    c = rotl(c, 10);
}

static void GG(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e, uint32_t x, uint32_t s) {
    a += G(b, c, d) + x + _K0;
    a = rotl(a, s) + e;
    c = rotl(c, 10);
}

static void HH(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e, uint32_t x, uint32_t s) {
    a += H(b, c, d) + x + _K1;
    a = rotl(a, s) + e;
    c = rotl(c, 10);
}

static void II(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e, uint32_t x, uint32_t s) {
    a += I(b, c, d) + x + _K2;
    a = rotl(a, s) + e;
    c = rotl(c, 10);
}

static void JJ(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e, uint32_t x, uint32_t s) {
    a += J(b, c, d) + x + _K3;
    a = rotl(a, s) + e;
    c = rotl(c, 10);
}

static void FFF(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e, uint32_t x, uint32_t s) {
    a += F(b, c, d) + x;
    a = rotl(a, s) + e;
    c = rotl(c, 10);
}

static void GGG(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e, uint32_t x, uint32_t s) {
    a += G(b, c, d) + x + _K4;
    a = rotl(a, s) + e;
    c = rotl(c, 10);
}

static void HHH(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e, uint32_t x, uint32_t s) {
    a += H(b, c, d) + x + _K5;
    a = rotl(a, s) + e;
    c = rotl(c, 10);
}

static void III(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e, uint32_t x, uint32_t s) {
    a += I(b, c, d) + x + _K6;
    a = rotl(a, s) + e;
    c = rotl(c, 10);
}

static void JJJ(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e, uint32_t x, uint32_t s) {
    a += J(b, c, d) + x + _K7;
    a = rotl(a, s) + e;
    c = rotl(c, 10);
}

void crypto::ripemd160(const uint32_t *x, uint32_t *digest) {
    uint32_t a1 = _IV[0];
    uint32_t b1 = _IV[1];
    uint32_t c1 = _IV[2];
    uint32_t d1 = _IV[3];
    uint32_t e1 = _IV[4];

    uint32_t a2 = _IV[0];
    uint32_t b2 = _IV[1];
    uint32_t c2 = _IV[2];
    uint32_t d2 = _IV[3];
    uint32_t e2 = _IV[4];

    // Rounds 1-5 (left lane)
    FF(a1, b1, c1, d1, e1, x[0], 11);
    FF(e1, a1, b1, c1, d1, x[1], 14);
    FF(d1, e1, a1, b1, c1, x[2], 15);
    FF(c1, d1, e1, a1, b1, x[3], 12);
    FF(b1, c1, d1, e1, a1, x[4], 5);
    FF(a1, b1, c1, d1, e1, x[5], 8);
    FF(e1, a1, b1, c1, d1, x[6], 7);
    FF(d1, e1, a1, b1, c1, x[7], 9);
    FF(c1, d1, e1, a1, b1, x[8], 11);
    FF(b1, c1, d1, e1, a1, x[9], 13);
    FF(a1, b1, c1, d1, e1, x[10], 14);
    FF(e1, a1, b1, c1, d1, x[11], 15);
    FF(d1, e1, a1, b1, c1, x[12], 6);
    FF(c1, d1, e1, a1, b1, x[13], 7);
    FF(b1, c1, d1, e1, a1, x[14], 9);
    FF(a1, b1, c1, d1, e1, x[15], 8);

    // Rounds 1-5 (right lane)
    JJJ(a2, b2, c2, d2, e2, x[5], 8);
    JJJ(e2, a2, b2, c2, d2, x[14], 9);
    JJJ(d2, e2, a2, b2, c2, x[7], 9);
    JJJ(c2, d2, e2, a2, b2, x[0], 11);
    JJJ(b2, c2, d2, e2, a2, x[9], 13);
    JJJ(a2, b2, c2, d2, e2, x[2], 15);
    JJJ(e2, a2, b2, c2, d2, x[11], 15);
    JJJ(d2, e2, a2, b2, c2, x[4], 5);
    JJJ(c2, d2, e2, a2, b2, x[13], 7);
    JJJ(b2, c2, d2, e2, a2, x[6], 7);
    JJJ(a2, b2, c2, d2, e2, x[15], 8);
    JJJ(e2, a2, b2, c2, d2, x[8], 11);
    JJJ(d2, e2, a2, b2, c2, x[1], 14);
    JJJ(c2, d2, e2, a2, b2, x[10], 14);
    JJJ(b2, c2, d2, e2, a2, x[3], 12);
    JJJ(a2, b2, c2, d2, e2, x[12], 6);

    // Final addition
    digest[0] = endian(_IV[1] + c1 + d2);
    digest[1] = endian(_IV[2] + d1 + e2);
    digest[2] = endian(_IV[3] + e1 + a2);
    digest[3] = endian(_IV[4] + a1 + b2);
    digest[4] = endian(_IV[0] + b1 + c2);
}
