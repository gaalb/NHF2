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

char Encryptor::first_char = '(';

char Encryptor::last_char = '}';

/*Megmondja, egy karakter az értelmezési tartományon belül esik-e*/
bool Encryptor::isValidInput(int c) const {
    if (c >= static_cast<int>(first_char) && c <= static_cast<int>(last_char)) {
        return true; 
    } else {
        return false;
    }
    return false;
}

/*Egy értelmezési tartományon kívül eső karaktert bele tol, úgy, hogy
a tartomány szélességével tolja el. Pl. az utolsó utáni karakterből az első lesz.*/
char Encryptor::shiftIntoScope(int c) const {
    int width = static_cast<int>(last_char) - static_cast<int>(first_char) + 1;
    while (c > static_cast<int>(last_char)) {
        c -= width;
    }
    while (c < static_cast<int>(first_char))
    {
        c += width;
    }
    return static_cast<char>(c);                
}

/*Felhasználva az ugyanilyen nevű, de karaktert váró és karaktert adó függvényt,
amely minden leszármazottra egyedi, kódol egy Stringet.*/
String Encryptor::encode(const String& str) const {
    String ret = str;
    for (String::iterator iter = ret.begin(); iter != ret.end(); ++iter) {
        *iter = this->encode(static_cast<char>(*iter));
    }
    //std::cout << "ENCODE: " << str << " => " << ret << std::endl;
    return ret;
}

/*Felhasználva az ugyanilyen nevű, de karaktert váró és karaktert adó függvényt,
amely minden leszármazottra egyedi, dekódol egy Stringet.*/
String Encryptor::decode(const String& str) const {
    String ret = str;
    for (String::iterator iter = ret.begin(); iter != ret.end(); ++iter) {
        *iter = this->decode(static_cast<char>(*iter));
    }
    //std::cout << "DECODE: " << str << " => " << ret << std::endl;
    return ret;
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
    if (!isValidInput(static_cast<int>(c))) {  //az invalid karaktereket nem kódoljuk
        return c;
    } else {
        int kep = static_cast<int>(c) + shift;
        return shiftIntoScope(kep);
    }
}

char ShiftEncryptor::decode(char c) const {
    if (!isValidInput(static_cast<int>(c))) { //az invalid karaktereket nem dekódoljuk
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

Encryptor* ShiftEncryptor::cloneInverse() const {
    return new ShiftEncryptor(-shift);
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

Encryptor* XorEncryptor::cloneInverse() const {
    return new XorEncryptor(key);
}