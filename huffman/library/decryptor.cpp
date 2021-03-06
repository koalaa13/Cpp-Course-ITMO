//
// Created by koalaa13 on 6/8/19.
//

#include "decryptor.h"

decryptor::decryptor(const counter &cnts) : tree(cnts) {}

std::vector<symbol> decryptor::decrypt(symbol const &c, u_ll &memory_size) {
    std::vector<symbol> to_print;
    if (memory_size >= 8) {
        memory_size -= 8;
        for (ptrdiff_t i = 7; i >= 0; --i) {
            bool to_go = c & (1 << i);
            u_int16_t res = tree.jump(to_go);
            if (res < MAX_SYMBOL) {
                to_print.push_back(static_cast<symbol>(res));
            }
        }
    } else {
        for (size_t i = 0, bit_id = 7; i < memory_size; ++i, --bit_id) {
            bool to_go = c & (1 << bit_id);
            u_int16_t res = tree.jump(to_go);
            if (res < MAX_SYMBOL) {
                to_print.push_back(static_cast<symbol>(res));
            }
        }
        memory_size = 0;
    }
    return to_print;
}

