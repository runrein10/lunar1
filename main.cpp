#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <random>
#include "CryptoUtil.h" // For SHA-256 and RIPEMD-160 hashing
#include "AddressUtil.h" // For privateKeyToAddress and address utilities
#include "secp256k1.h"   // For secp256k1 elliptic curve operations

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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(start, end);

    for (uint64_t i = start; i <= end; ++i) {
        if (keysChecked % 50000000 == 0 && keysChecked != 0) {
            i = dis(gen); // Randomize start point every 50 million keys
        }

        std::stringstream ss;
        ss << std::hex << i;
        std::string key = ss.str();

        // Display progress
        displayProgress(i - start, totalKeys);

        // Check if the key is valid
        if (!hasRepeatingCharacters(key) && !hasAscendingDescendingSequence(key)) {
            std::string address = Address::privateKeyToAddress(key);
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
