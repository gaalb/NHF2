/**
 * @file StringBase.cpp
 * @author Gaál Botond
 * @brief 
 */
#include "StringBase.h"
#include <cstring>
#include <stdexcept>
#include <iomanip>

#include "memtrace.h" //utoljára includeoljuk
size_t StringBase::get_len() const {
    return len;
}

const char* StringBase::c_str() const {
    return str;  //pointer a din. foglalt karaktertömbre
}

StringBase::~StringBase() {
    delete[] str;  //dinamikusan foglalt -> fel kell szabadítani
}

/*inicializáló listán ki van használva, hogy mivel len volt előbb deklarálva a headerben, előbb 
lesz inicializálva, és str inicializálásakor felhasználhatjuk (ez a későbbi ctorokban is így van)*/
StringBase::StringBase(const char* str): len(strlen(str)), str(new char[len+1]) {
    strcpy(this->str, str);  //hely le van foglalva, be kell másolni
}

StringBase::StringBase(const char c): len(1), str(new char[len+1]) {
    str[0] = c;
    str[1] = '\0';
}

StringBase::StringBase(const StringBase& other): len(other.len), str(new char[len+1]) {
    strcpy(str, other.str);
}

char& StringBase::operator[](const size_t idx) {
    if (idx >= len) {
        throw std::out_of_range("Index is out of range!");
    }
    return str[idx];
}

const char& StringBase::operator[](const size_t idx) const {
    if (idx >= len) {
        throw std::out_of_range("Index is out of range!");
    }
    return str[idx];
}

bool StringBase::operator==(const char c) const {
    char rhs[2];
    rhs[0] = c;
    rhs[1] = '\0';
    return strcmp(str, rhs) == 0;
}

bool StringBase::operator!=(const char c) const {
    char rhs[2];
    rhs[0] = c;
    rhs[1] = '\0';
    return strcmp(str, rhs) != 0;
}

bool StringBase::operator==(const char* rhs) const {
    return strcmp(str, rhs) == 0;
}

bool StringBase::operator!=(const char* rhs) const {
    return strcmp(str, rhs) != 0;
}

/*létezik StringBase& típusú == és != operátor, de ezek nem
megfelelőek olyankor, ha char és char* a jobboldal, mert 
nem lehet char/char*-ot StringBase-vé konvertálni, hiába van
erre megfelelő konstrukor, ugyanis absztrakt osztály!*/
bool StringBase::operator==(const StringBase& rhs) const {
    return strcmp(str, rhs.str) == 0;
}

bool StringBase::operator!=(const StringBase& rhs) const {
    return strcmp(str, rhs.str) != 0;
}

//az iterátor privát adattagja char* típusú
StringBase::iterator::iterator(char* ptr): ptr(ptr) {}

StringBase::iterator& StringBase::iterator::operator++() { //preinkremens: ++iter
    if (*ptr != '\0') {  //ha lezáró nullára mutat, már nincs értelme léptepni
        ++ptr;
    }
    return *this;  //eredetire referencia
}

/*a posztinkremens operátort abból ismeri meg a fordító, hogy van egy dummy int paramétere
mivel vissza kell adni egy iterátort, és csak aztán megváltoztatni, nem adhatunk az eredeti
iterátorra referenciát: másolatot kell vissza adni, és a return is iterator, iterator& helyett*/
StringBase::iterator StringBase::iterator::operator++(int) {
    iterator masolat = *this;
    ++(*this);
    return masolat;
}

char& StringBase::iterator::operator*() const {
    return *ptr;  //a belső pointer által mutatott karaktert adjuk vissza
}

char* StringBase::iterator::operator->() const {
    return ptr; // a -> operátor amit visszaad, dereferálja a compiler: a pointert adjuk vissza!
}


bool StringBase::iterator::operator==(const StringBase::iterator& rhs) const {
    return ptr == rhs.ptr;
}

bool StringBase::iterator::operator!=(const StringBase::iterator& rhs) const {
    return ptr != rhs.ptr;
}

StringBase::iterator StringBase::begin() {
    return iterator(str);
}

StringBase::iterator StringBase::end() {
    return iterator(str + len);
}

/*const_iterator: konstans objektumokon ez hívható meg, ezen keresztül nem
módosítható a tároló*/
StringBase::const_iterator::const_iterator(char* ptr): ptr(ptr) {}

StringBase::const_iterator& StringBase::const_iterator::operator++() {
    if (*ptr != '\0') {
        ++ptr;
    }
    return *this;
}

StringBase::const_iterator StringBase::const_iterator::operator++(int) {
    const_iterator masolat = *this;
    ++(*this);
    return masolat;
}

const char& StringBase::const_iterator::operator*() const { // const char& !
    return *ptr;
}

const char* StringBase::const_iterator::operator->() const { // const char* !
    return ptr;
}

bool StringBase::const_iterator::operator==(const StringBase::const_iterator& rhs) const {
    return ptr == rhs.ptr;
}

bool StringBase::const_iterator::operator!=(const StringBase::const_iterator& rhs) const {
    return ptr != rhs.ptr;
}

/*konstans tagfüggvény
Ha nem lenne külön const_iterator, akkor iterator-nál is lehetne a begin és az end const, 
hiszen azok nem változtatják a tároló állapotát, viszont akkor ez nem lehetne const, mert
akkor csak a return type választaná el az iterator és a const_iterator-ral visszatérő
begin és end függvényt, márpedig csupán visszatéréssel nem lehet overloadolni. Így, hogy
ezek const-ok, az iterator-ral visszatérők pedig nem, így a const-ság overloadol, és eltérő
tud lenni a return*/
StringBase::const_iterator StringBase::begin() const{ //const (iterator-nal is lehetne const, de ez az overloadhoz kell, mert csupán visszatéréssel nem lehet overloadolni)
    return const_iterator(str);
}

//const!
StringBase::const_iterator StringBase::end() const {
    return const_iterator(str + len);
}

//global
std::ostream& operator<<(std::ostream& os, const StringBase& str) {
    os << str.c_str();
    return os;
}

bool operator==(const char* left, const StringBase& right) {
    return strcmp(left, right.c_str()) == 0;
}
bool operator!=(const char* left, const StringBase& right) {
    return strcmp(left, right.c_str()) != 0;
}
bool operator==(const char left, const StringBase& right) {
    char str[2];
    str[0] = left;
    str[1] = '\0';
    return strcmp(str, right.c_str()) == 0;
}
bool operator!=(const char left, const StringBase& right) {
    char str[2];
    str[0] = left;
    str[1] = '\0';
    return strcmp(str, right.c_str()) != 0;
}

/*Leading whitespace karaktereket eldobjuk, és az első whitespace-ig olvasunk.*/
std::istream& operator>>(std::istream& is, StringBase& str) {
    char c;
    char* buffer = new char[1];
    size_t length = 0;
    is >> std::skipws;
    while ((is >> c) && !isspace(c)) {  // ha volt beolvasott nem whitespace char
        char* newBuffer = new char[length + 2];  //helyet foglalunk az új tömbnek
        for (size_t i=0; i<length; ++i) {  // belemásoljuk az adatot
            newBuffer[i] = buffer[i];
        }
        newBuffer[length++] = c;  // megtoldjuk az új karakterrel
        newBuffer[length] = '\0';
        delete[] buffer;
        buffer = newBuffer;
        is >> std::noskipws;
    }
    str = buffer;
    delete[] buffer;
    is >> std::skipws;
    return is;
}