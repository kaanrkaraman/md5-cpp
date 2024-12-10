#include "md5.h"
#include <iostream>

int main() {
    std::string input;
    std::cout << "Enter string to hash: ";
    std::getline(std::cin, input);

    MD5 md5;
    std::string hash = md5.computeHash(input);

    std::cout << "MD5 Hash: " << hash << std::endl;

    return 0;
}