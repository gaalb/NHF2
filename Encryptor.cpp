#include "Encryptor.h"

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

/*Code for Encryptors in general*/
Encryptor::~Encryptor() {}

bool Encryptor::isValidInput(int c) const {
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

char Encryptor::shiftIntoScope(int c) const {
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

String Encryptor::encode(const String& str) const {
    String ret = str;
    for (String::iterator iter = ret.begin(); iter != ret.end(); ++iter) {
        *iter = this->encode(static_cast<char>(*iter));
    }
    return ret;
}

String Encryptor::decode(const String& str) const {
    String ret = str;
    for (String::iterator iter = ret.begin(); iter != ret.end(); ++iter) {
        *iter = this->decode(static_cast<char>(*iter));
    }
    return ret;
}

/*Code for XorEncryptor (not a great encryptor for representation)*/
XorEncryptor::XorEncryptor(char key): key(key) {};

void XorEncryptor::set_key(char key) {
    this->key = key;
}

char XorEncryptor::get_key() const {
    return key;
}

char XorEncryptor::encode(char c) const {
    return (c ^ key);
}

char XorEncryptor::decode(char c) const {
    return encode(c);  // a xor a sajat ellentete
}

XorEncryptor XorEncryptor::operator-() const {
    return XorEncryptor(key);
}

Encryptor* XorEncryptor::clone() const {
    return new XorEncryptor(key);
}


/*Code for ShiftEncryptor*/
ShiftEncryptor::ShiftEncryptor(int eltolas): shift(eltolas) {}

void ShiftEncryptor::set_shift(int eltolas) {
    this->shift = eltolas;
}

char ShiftEncryptor::get_shift() const {
    return shift;
}

char ShiftEncryptor::encode(char c) const {
    if (!isValidInput(static_cast<int>(c))) {
        return c;
    } else {
        int kep = static_cast<int>(c) + shift;
        return shiftIntoScope(kep);
    }
}

char ShiftEncryptor::decode(char c) const {
    if (c < '0' || c > 'z') {
        return c;
    } else {
        int kep = static_cast<int>(c) - shift;
        return shiftIntoScope(kep);
    }
}

ShiftEncryptor ShiftEncryptor::operator-() const {
    return ShiftEncryptor(-shift);
}

Encryptor* ShiftEncryptor::clone() const {
    return new ShiftEncryptor(shift);
}