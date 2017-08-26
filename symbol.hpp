#pragma once
#include <iosfwd>

enum class IOSetting {
    Raw, Compressed
};

struct SymbolData {
    unsigned char count;
    unsigned char token;
};

template<IOSetting>
struct SymbolImpl;

template<IOSetting S>
struct Symbol : private SymbolData {
    Symbol() = default;
    template<IOSetting S2>
    Symbol(Symbol<S2> const& other) : SymbolData(other) {}

    template<IOSetting>
    friend class SymbolImpl;
    template<IOSetting>
    friend class Symbol;
};

using RawSymbol = Symbol<IOSetting::Raw>;
using CompressedSymbol = Symbol<IOSetting::Compressed>;

template<IOSetting S>
std::istream& operator>>(std::istream& is, Symbol<S>& symbol);
template<IOSetting S>
std::ostream& operator<<(std::ostream& os, Symbol<S> symbol);
