#include "calculator.hpp"
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::cout << "enter path to file containing candidate numbers\n";
    std::string filename; std::getline(std::cin, filename);
    std::ifstream fin(filename);
    if (!fin) { std::cerr << "cannot open " << filename << '\n'; return 1; }

    const std::string addend = "-123.456";
    std::string line;
    while (std::getline(fin, line)) {
        if (!isValidDouble(line)) { std::cout << line << " is invalid\n"; continue; }
        std::cout << line << " + (" << addend << ") = " << add(line, addend) << '\n';
    }
    return 0;
}
