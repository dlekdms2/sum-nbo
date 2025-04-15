#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <iomanip>  // std::setw, std::setfill
#include <arpa/inet.h>

uint32_t read_file(const std::string &filename, bool &success) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        success = false;
        return 0;
    }

    uint32_t number;
    file.read(reinterpret_cast<char*>(&number), sizeof(number));
    if (!file) {
        std::cerr << "Error reading from file: " << filename << std::endl;
        success = false;
        return 0;
    }

    // netbyte to hostbyte
    success = true;
    return ntohl(number);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file1> [<file2>...]" << std::endl;
        return EXIT_FAILURE;
    }

    uint32_t totalSum = 0;
    std::vector<uint32_t> numbers;

    for (int i = 1; i < argc; ++i) {
        bool success;
        uint32_t num = read_file(argv[i], success);
        if (success) {
            numbers.push_back(num);
            totalSum += num;
        }
    }

    for (size_t i = 0; i < numbers.size(); ++i) {
        std::cout << std::dec << numbers[i] << "(0x"
                  << std::hex << std::setw(8) << std::setfill('0') << numbers[i] << ")";
        if (i < numbers.size() - 1) {
            std::cout << " + ";
        } else {
            std::cout << " = ";
        }
    }
    std::cout << std::dec << totalSum << "(0x"
              << std::hex << std::setw(8) << std::setfill('0') << totalSum << ")" << std::endl;

    return EXIT_SUCCESS;
}
