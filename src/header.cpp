#include "header.h"

void Header::showData() {
    for (auto &i : mData) {
        std::cout << "] " << i.first << " - " << i.second << "\n";
    }
}