#include "print.hpp"
#include <fstream>

int main() {
    std::ofstream file("log.txt");
    print("hello", file);
    return 0;
}
