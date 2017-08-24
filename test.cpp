#include "symbol.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>

struct TestFailure : std::exception {
    std::string msg;
    TestFailure(std::string expected, std::string received)
    : msg("Expected " + expected + "; got " + received)
    {}

    char const* what() const noexcept override {
        return msg.c_str();
    }
};

void test_encode_decode_symmetry(std::string const& s) {
    std::istringstream iss(s);
    iss.unsetf(std::ios::skipws);
    std::stringstream ss;
    std::copy(
        std::istream_iterator<RawSymbol>{iss},
        std::istream_iterator<RawSymbol>{},
        std::ostream_iterator<CompressedSymbol>{ss}
    );
    std::ostringstream oss;
    std::copy(
        std::istream_iterator<CompressedSymbol>{ss},
        std::istream_iterator<CompressedSymbol>{},
        std::ostream_iterator<RawSymbol>{oss}
    );
    auto result = oss.str();
    if (s != result)
        throw TestFailure(s, result);
}

int main() {
    std::vector<std::string> test_strings = {
        "AAAAAAAA",
        "AAA\nAAA",
        "ABCDEFGH",
        "____AA__",
        "AaAaAaAa"
    };
    test_strings.push_back(std::string(1000, 'a'));
    for (auto const& s : test_strings) {
        try {
            test_encode_decode_symmetry(s);
        }
        catch (std::exception& e) {
            std::cerr << "Fail: " << e.what() << '\n';
        }
    }
}

