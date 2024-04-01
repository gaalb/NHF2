#include "titkosito.h"

#include "memtrace.h"

// Template function to print the binary representation of any type
template <typename T>
void printAsBinary(const T& value) {
    const char* valuePtr = reinterpret_cast<const char*>(&value);
    for (int i = sizeof(T) - 1; i >= 0; --i) {
        for (int j = 7; j >= 0; --j) {
            std::cout << ((valuePtr[i] >> j) & 1);
        }
        std::cout << " "; // Optional: Adds a space between bytes for readability
    }
}

Titkosito::~Titkosito() {}

bool Titkosito::valid(int c) const {
    if (c >= static_cast<int>('0') && c <= static_cast<int>('9')) {
        return true;  //0 és 9 közötti 
    }
    if (c >= static_cast<int>('A') && c <= static_cast<int>('Z')) {
        return true; //nagy betű
    }
    if (c >= static_cast<int>('a') && c <= static_cast<int>('z')) {
        return true; //kis betű
    }
    return false;
}

char Titkosito::tartomanyba(int c) const {
    int width = static_cast<int>('z') - static_cast<int>('0') + 1;
    while (c > static_cast<int>('z')) {
        c -= width;
    }
    while (c < static_cast<int>('0'))
    {
        c += width;
    }
    return static_cast<char>(c);                
}

//ezek nem tuti hogy jók:
String Titkosito::encode(const String& str) const {
    String ret = str;
    for (String::iterator iter = ret.begin(); iter != ret.end(); ++iter) {
        *iter = this->encode(static_cast<char>(*iter));
    }
    return ret;
}

String Titkosito::decode(const String& str) const {
    String ret = str;
    for (String::iterator iter = ret.begin(); iter != ret.end(); ++iter) {
        *iter = this->decode(static_cast<char>(*iter));
    }
    return ret;
}

XorTitkosito::XorTitkosito(char key): key(key) {};

void XorTitkosito::set_key(char key) {
    this->key = key;
}

char XorTitkosito::get_key() const {
    return key;
}

char XorTitkosito::encode(char c) const {
    return (c ^ key);
}

char XorTitkosito::decode(char c) const {
    return encode(c);  // a xor a sajat ellentete
}

XorTitkosito XorTitkosito::operator-() const {
    return XorTitkosito(key);
}

Titkosito* XorTitkosito::clone() const {
    return new XorTitkosito(key);
}

//////////////////////////////////////////////////////////

ShiftTitkosito::ShiftTitkosito(int eltolas): shift(eltolas) {}

void ShiftTitkosito::set_shift(int eltolas) {
    this->shift = eltolas;
}

char ShiftTitkosito::get_shift() const {
    return shift;
}

char ShiftTitkosito::encode(char c) const {
    if (!valid(static_cast<int>(c))) {
        return c;
    } else {
        int kep = static_cast<int>(c) + shift;
        return tartomanyba(kep);
    }
}

char ShiftTitkosito::decode(char c) const {
    if (c < '0' || c > 'z') {
        return c;
    } else {
        int kep = static_cast<int>(c) - shift;
        return tartomanyba(kep);
    }
}

ShiftTitkosito ShiftTitkosito::operator-() const {
    return ShiftTitkosito(-shift);
}

Titkosito* ShiftTitkosito::clone() const {
    return new ShiftTitkosito(shift);
}