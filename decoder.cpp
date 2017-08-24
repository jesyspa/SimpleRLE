#include "symbol.hpp"
#include <iostream>
#include <iterator>
#include <algorithm>

int main() {
    std::cin.unsetf(std::ios::skipws);
    std::copy(
        std::istream_iterator<CompressedSymbol>{std::cin},
        std::istream_iterator<CompressedSymbol>{},
        std::ostream_iterator<RawSymbol>{std::cout}
    );
}
