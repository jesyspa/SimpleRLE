#include "symbol.hpp"
#include <istream>
#include <ostream>

template<>
struct SymbolImpl<IOSetting::Raw> {
    static constexpr IOSetting S = IOSetting::Raw;
    static void read(std::istream& is, Symbol<S>& symbol) {
        auto first = is.get();
        unsigned counter = 0;
        while (is.peek() == first && counter < 0x7F) {
            is.ignore();
            counter += 1;
        }
        symbol.count = counter;
        symbol.token = first;
    }

    static void write(std::ostream& os, Symbol<S> symbol) {
        for (unsigned char i = 0; i < symbol.count + 1; ++i)
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
            symbol.count = 0;
            symbol.token = header;
        }
    }

    static void write(std::ostream& os, Symbol<S> symbol) {
        if (symbol.count > 0) {
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

template std::istream& operator>><IOSetting::Raw>(std::istream&, Symbol<IOSetting::Raw>&);
template std::istream& operator>><IOSetting::Compressed>(std::istream&, Symbol<IOSetting::Compressed>&);
template std::ostream& operator<<<IOSetting::Raw>(std::ostream&, Symbol<IOSetting::Raw>);
template std::ostream& operator<<<IOSetting::Compressed>(std::ostream&, Symbol<IOSetting::Compressed>);
