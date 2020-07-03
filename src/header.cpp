#include "header.h"

void Header::showData() {
    std::cout << "\n==========Header==============\n";
    for( auto &i : mHeader) {
        std::cout << "] " << i.first << " - " << i.second << "\n";
    }

    std::cout << "=============Data==============\n";
    if( mData.empty()) {
        std::cout << "] nothing" << "\n";
    }
    else {
        for( auto &i : mData) {
            std::cout << "] " << i.first << " - " << i.second << "\n";
        }
    }
    std::cout << "===============================\n\n";
}