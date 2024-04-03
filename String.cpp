#include "StringBase.h"
#include "String.h"
#include <cstring>
#include <stdexcept>

#include "memtrace.h"

String::String(const char* str): StringBase(str) {}

String::String(const char c): StringBase(c) {}

String::String(const StringBase& other): StringBase(other) {}

String& String::operator=(const StringBase& rhs) {
    if (this != &rhs) {
        delete[] str;
        len = rhs.get_len();
        str = new char[len+1];
        strcpy(str, rhs.c_str());
    }    
    return *this; 
}

/*Elvileg ha az alábbi függvényeket (melyek const String&-et 
vesznek át) egy char, char* vagy StringBase& argumentummal
hívjuk meg, mivel van megfelelő konstruktor, át tudja konvertálni
Stringgé a char vagy char*-t. Ennek ellenére az én IDE-m az ilyen
felhasználásnál panaszkodik (hiába fordul a program).*/
String& String::operator=(const String& rhs) {
    if (this != &rhs) {
        delete[] str;
        len = rhs.get_len();
        str = new char[len+1];
        strcpy(str, rhs.c_str());
    }    
    return *this; 
}

String String::operator+(const String& rhs) const {
    String ret;
    ret.len = rhs.len + this->len;
    delete[] ret.str;
    ret.str = new char[ret.len + 1];
    strcpy(ret.str, this->str);
    strcat(ret.str, rhs.str);
    return ret;
}

String& String::operator+=(const String& str) {
    *this = *this + str;
    return *this;
}

String operator+(const char c, const String& str) {
    return String(c) + str;
}

String operator+(const char* left, const String& right) {
    return String(left) + right;
}

