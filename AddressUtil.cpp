#include "AddressUtil.h"
#include "CryptoUtil.h" // For SHA-256 and RIPEMD-160 hashing
#include <sstream>
#include <iomanip>

namespace Address {

    std::string privateKeyToAddress(const std::string& privateKeyHex) {
        // Step 1: Generate the public key from the private key
        secp256k1::uint256 privateKey(privateKeyHex);
        secp256k1::ecpoint publicKey = secp256k1::multiplyPoint(privateKey, secp256k1::G());

        // Step 2: Hash the public key
        unsigned int hash[5];
        Hash::hashPublicKey(publicKey, hash);

        // Step 3: Add version byte (0x00 for Bitcoin mainnet)
        unsigned int payload[6] = { 0x00 }; // Version byte + 20-byte hash
        for (int i = 0; i < 5; i++) {
            payload[i + 1] = hash[i];
        }

        // Step 4: Compute checksum (double SHA-256)
        unsigned int checksum[8];
        crypto::sha256(reinterpret_cast<unsigned char*>(payload), 21, reinterpret_cast<unsigned char*>(checksum));
        crypto::sha256(reinterpret_cast<unsigned char*>(checksum), 32, reinterpret_cast<unsigned char*>(checksum));

        // Step 5: Combine payload and checksum
        unsigned int addressData[7];
        for (int i = 0; i < 6; i++) {
            addressData[i] = payload[i];
        }
        addressData[6] = checksum[0]; // First 4 bytes of checksum

        // Step 6: Encode in Base58
        secp256k1::uint256 addressInt(addressData);
        std::string address = Base58::toBase58(addressInt);

        return address;
    }

    // Other existing functions...
}
