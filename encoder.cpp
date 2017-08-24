#include "symbol.hpp"
#include <iostream>
#include <iterator>
#include <algorithm>

int main() {
    std::cin.unsetf(std::ios::skipws);
    std::copy(
        std::istream_iterator<RawSymbol>{std::cin},
        std::istream_iterator<RawSymbol>{},
        std::ostream_iterator<CompressedSymbol>{std::cout}
    );
}
