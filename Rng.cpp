#include "CryptoUtil.h"
#include <random>
#include <algorithm> // For std::fill

namespace crypto {

    Rng::Rng() : _counter(0) {
        std::random_device rd;
        std::fill(_state.begin(), _state.end(), 0); // Initialize the state array with zeros
    }

    void Rng::reseed() {
        std::random_device rd;
        for (auto& s : _state) {
            s = rd(); // Fill the state array with random values
        }
    }

    void Rng::get(unsigned char *buf, int len) {
        for (int i = 0; i < len; ++i) {
            if (_counter % 16 == 0) {
                reseed(); // Reseed every 16 words (64 bytes)
            }
            buf[i] = static_cast<unsigned char>(_state[_counter % 16] & 0xFF); // Extract a byte from the state
            _counter++;
        }
    }
}
