#ifndef _ADDRESS_UTIL_H
#define _ADDRESS_UTIL_H

#include "secp256k1.h"
#include <string>

namespace Address {
    // Convert a public key to a Bitcoin address
    std::string fromPublicKey(const secp256k1::ecpoint &p, bool compressed = false);

    // Verify if a Bitcoin address is valid
    bool verifyAddress(std::string address);

    // Convert a private key (in hex) to a Bitcoin address
    std::string privateKeyToAddress(const std::string& privateKeyHex);
};

namespace Base58 {
    // Convert a uint256 value to a Base58 string
    std::string toBase58(const secp256k1::uint256 &x);

    // Convert a Base58 string to a uint256 value
    secp256k1::uint256 toBigInt(const std::string &s);

    // Get the min and max values for a given Base58 prefix
    void getMinMaxFromPrefix(const std::string &prefix, secp256k1::uint256 &minValueOut, secp256k1::uint256 &maxValueOut);

    // Convert a Base58 string to a 160-bit hash
    void toHash160(const std::string &s, unsigned int hash[5]);

    // Check if a string is valid Base58
    bool isBase58(std::string s);
};

namespace Hash {
    // Hash a public key (uncompressed)
    void hashPublicKey(const secp256k1::ecpoint &p, unsigned int *digest);

    // Hash a public key (compressed)
    void hashPublicKeyCompressed(const secp256k1::ecpoint &p, unsigned int *digest);

    // Hash a public key (uncompressed) given x and y coordinates
    void hashPublicKey(const unsigned int *x, const unsigned int *y, unsigned int *digest);

    // Hash a public key (compressed) given x and y coordinates
    void hashPublicKeyCompressed(const unsigned int *x, const unsigned int *y, unsigned int *digest);
};

#endif
