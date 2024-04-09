/**
 * @file encryptor.cpp
 * @author Gaál Botond
 * @brief 
 */
#include "Encryptor.h"
#include <cstdlib>

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

char Encryptor::first_char = '!';

char Encryptor::last_char = '}';

size_t Encryptor::range = static_cast<size_t>(Encryptor::last_char - Encryptor::first_char + 1);

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
    int width = static_cast<int>(Encryptor::range);
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

// RandEncryptor: random map-eli a karaktereket egymáshoz az értelmezési tartományban
/* Ehhez a shuffle függvény az, ami egy adott hosszú tömb elemeit összekeveri.
Azzal, hogy csak cserélhetjük az elemeket, biztosítjuk hogy minden index csak egyszer
lesz benne. Ezekkel az indexekkel fogjuk majd az értelmezési tartomány és (a megegyező)
képtér elemeit egymáshoz rendelni.*/
void RandEncryptor::shuffle(char* array, size_t size) {
    for (size_t i = 0; i < size-1; i++) {
        //size_t j = rand() % (i+1); // az i-edik elemet valamelyik előtte lévővel cseréljük meg (máshogy is lehetne)
        size_t j = rand() % size;
        char tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

RandEncryptor::RandEncryptor() {
    encodeMap = new char[Encryptor::range];
    decodeMap = new char[Encryptor::range];
    for (size_t i = 0; i < Encryptor::range; ++i) {
        encodeMap[i] = Encryptor::first_char + i;  // feltöltjük az indexekkel, amelyeket megkeverünk majd
    }
    shuffle(encodeMap, Encryptor::range);
    // majd ennek a listának az inverzéfel töltjük fel a dekódolót
    for (size_t i = 0; i < Encryptor::range; ++i) {
        char encodedChar = encodeMap[i];
        decodeMap[static_cast<size_t>(encodedChar - Encryptor::first_char)] = Encryptor::first_char + i;
    }
}

RandEncryptor::~RandEncryptor() {
    delete[] encodeMap;
    delete[] decodeMap;
}

RandEncryptor::RandEncryptor(const RandEncryptor& other): 
    encodeMap(new char[Encryptor::range]),  decodeMap(new char[Encryptor::range]){
    for (size_t i = 0; i < Encryptor::range; i++) {
        encodeMap[i] = other.encodeMap[i];
        decodeMap[i] = other.decodeMap[i];
    }
}

RandEncryptor& RandEncryptor::operator=(const RandEncryptor& other) {
    /*Gyakorlatilag mivel minden Encryptornak, és így minden RandEncryptornak is megegyezik
    az értelmezési tartománya, és a RandEncryptorok esetében ez egybeesik a képtérrel is,
    így az encodeMap és a decodeMap mérete mindig Encryptor::range, és nem lenne szükség
    kitörölni, majd újat foglalni (hiszen ugyanakkorát foglalunk le, mint amekkorát kitöröltünk).
    Elég lenne a meglévő elemeket felülrírni. Ennek ellenére ezt itt meghagyom, mert lehetséges, 
    hogy később meggondolom magam, és meg akarom engedni, hogy más karakterhalmazon legyenek
    értelmezve.*/
    if (this != &other) {
        delete[] encodeMap;
        delete[] decodeMap;
        encodeMap = new char[Encryptor::range];
        decodeMap = new char[Encryptor::range];
        for (size_t i = 0; i < Encryptor::range; i++) {
            encodeMap[i] = other.encodeMap[i];
            decodeMap[i] = other.decodeMap[i];
        }
    }
    return *this;
}

char RandEncryptor::encode(char c) const {
    if (isValidInput(static_cast<int>(c))) {
        return encodeMap[static_cast<size_t>(c - Encryptor::first_char)];
    }
    return c;
}

char RandEncryptor::decode(char c) const {
    if (isValidInput(static_cast<int>(c))) {
        return decodeMap[static_cast<size_t>(c - Encryptor::first_char)];
    }
    return c;
}

Encryptor* RandEncryptor::clone() const {
    return new RandEncryptor(*this);
}

//segédfüggvény az inverzek képzéséhez
void RandEncryptor::invert() {
    char* tmp = encodeMap;
    encodeMap = decodeMap;
    decodeMap = tmp;
}

Encryptor* RandEncryptor::cloneInverse() const {
    RandEncryptor* inverse = new RandEncryptor(*this);
    inverse->invert();
    Encryptor* ret = inverse;
    return ret;
}

RandEncryptor RandEncryptor::operator-() const {
    RandEncryptor ret(*this);
    ret.invert();
    return ret;
}
