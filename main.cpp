#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <openssl/ripemd.h>

// Function to check for repeating characters
bool hasRepeatingCharacters(const std::string& key, int maxRepeats = 3) {
    for (size_t i = 0; i + maxRepeats < key.size(); ++i) {
        bool allSame = true;
        for (int j = 1; j <= maxRepeats; ++j) {
            if (key[i] != key[i + j]) {
                allSame = false;
                break;
            }
        }
        if (allSame) return true;
    }
    return false;
}

// Function to check for ascending/descending sequences
bool hasAscendingDescendingSequence(const std::string& key, int sequenceLength = 3) {
    for (size_t i = 0; i + sequenceLength <= key.size(); ++i) {
        bool ascending = true;
        bool descending = true;
        for (int j = 1; j < sequenceLength; ++j) {
            if (key[i + j] != key[i + j - 1] + 1) ascending = false;
            if (key[i + j] != key[i + j - 1] - 1) descending = false;
        }
        if (ascending || descending) return true;
    }
    return false;
}

// Function to compute SHA-256 hash
std::string sha256(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.c_str(), data.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Function to compute RIPEMD-160 hash
std::string ripemd160(const std::string& data) {
    unsigned char hash[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160_CTX ripemd160;
    RIPEMD160_Init(&ripemd160);
    RIPEMD160_Update(&ripemd160, data.c_str(), data.size());
    RIPEMD160_Final(hash, &ripemd160);
    std::stringstream ss;
    for (int i = 0; i < RIPEMD160_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Function to convert a private key to a Bitcoin address (placeholder logic)
std::string privateKeyToAddress(const std::string& privateKeyHex) {
    // Step 1: Generate public key (placeholder; replace with actual logic)
    std::string publicKey = privateKeyHex; // Placeholder

    // Step 2: Hash the public key
    std::string sha256Hash = sha256(publicKey);
    std::string ripemd160Hash = ripemd160(sha256Hash);

    // Step 3: Add version byte (0x00 for Bitcoin mainnet)
    std::string payload = "\x00" + ripemd160Hash;

    // Step 4: Compute checksum
    std::string checksum = sha256(sha256(payload)).substr(0, 8);

    // Step 5: Encode in Base58 (placeholder; use a Base58 library for real implementation)
    std::string address = payload + checksum; // Placeholder
    return address;
}

// Function to display a progress bar
void displayProgress(uint64_t current, uint64_t end) {
    const int barWidth = 50;
    float progress = static_cast<float>(current) / static_cast<float>(end);
    int pos = static_cast<int>(barWidth * progress);

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << std::fixed << std::setprecision(2) << progress * 100.0 << " %\r";
    std::cout.flush();
}

// Function to search for valid keys
void searchKeys(uint64_t start, uint64_t end, const std::string& targetAddress) {
    uint64_t totalKeys = end - start + 1;
    uint64_t keysChecked = 0;

    for (uint64_t i = start; i <= end; ++i) {
        std::stringstream ss;
        ss << std::hex << i;
        std::string key = ss.str();

        // Display progress
        displayProgress(i - start, totalKeys);

        // Check if the key is valid
        if (!hasRepeatingCharacters(key) && !hasAscendingDescendingSequence(key)) {
            std::string address = privateKeyToAddress(key);
            if (address == targetAddress) {
                std::cout << "\nMatch found! Private key: " << key << ", Address: " << address << std::endl;
                return;
            }
        }

        keysChecked++;
    }
    std::cout << "\nNo match found in the searched range. Keys checked: " << keysChecked << std::endl;
}

int main() {
    // User input for range and target address
    uint64_t start, end;
    std::string targetAddress;

    std::cout << "Enter start of range (hexadecimal, e.g., 0x60000000000000000): ";
    std::cin >> std::hex >> start;
    std::cout << "Enter end of range (hexadecimal, e.g., 0x60000000000100000): ";
    std::cin >> std::hex >> end;
    std::cout << "Enter target Bitcoin address: ";
    std::cin >> targetAddress;

    // Start the search
    searchKeys(start, end, targetAddress);

    return 0;
}
