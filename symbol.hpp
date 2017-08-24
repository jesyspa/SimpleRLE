#pragma once

#include <iosfwd>
#include <istream>
#include <ostream>

enum class IOSetting {
    Raw, Compressed
};

struct SymbolData {
    unsigned char count;
    unsigned char token;
};

template<IOSetting S>
struct Symbol : SymbolData {
    Symbol() = default;
    template<IOSetting S2>
    Symbol(Symbol<S2> const& other) : SymbolData(other) {}
};

template<IOSetting>
struct SymbolImpl;

template<>
struct SymbolImpl<IOSetting::Raw> {
    static constexpr IOSetting S = IOSetting::Raw;
    static void read(std::istream& is, Symbol<S>& symbol) {
        auto first = is.get();
        unsigned counter = 1;
        while (is.peek() == first && counter < 0x80) {
            is.ignore();
            counter += 1;
        }
        symbol.count = counter;
        symbol.token = first;
    }

    static void write(std::ostream& os, Symbol<S> symbol) {
        for (unsigned char i = 0; i < symbol.count; ++i)
            os.put(symbol.token);
    }
};

template<>
struct SymbolImpl<IOSetting::Compressed> {
    static constexpr IOSetting S = IOSetting::Compressed;
    static void read(std::istream& is, Symbol<S>& symbol) {
        auto header = is.get();
        if (header & 0x80) {
            // encoded
            symbol.count = header & 0x7F;
            symbol.token = is.get();
        } else {
            // unencoded
            symbol.count = 1;
            symbol.token = header;
        }
    }

    static void write(std::ostream& os, Symbol<S> symbol) {
        if (symbol.count > 1) {
            os.put(symbol.count | 0x80u);
            os.put(symbol.token);
        } else {
            os.put(symbol.token);
        }
    }
};

template<IOSetting S>
std::istream& operator>>(std::istream& is, Symbol<S>& symbol) {
    SymbolImpl<S>::read(is, symbol);
    return is;
}

template<IOSetting S>
std::ostream& operator<<(std::ostream& os, Symbol<S> symbol) {
    SymbolImpl<S>::write(os, symbol);
    return os;
}

using RawSymbol = Symbol<IOSetting::Raw>;
using CompressedSymbol = Symbol<IOSetting::Compressed>;
