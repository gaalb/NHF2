/**
 * @file Gaál Botond
 * @author your name (you@domain.com)
 * @brief A String osztály implementációját tartalmazó file.
 *
 */
#include "string.h"
#include <cstring>
#include <stdexcept>

#include "memtrace.h"

String::String(const char* str): len(strlen(str)), str(new char[len+1]) {
    strcpy(this->str, str);
}

String::String(const char c): len(1), str(new char[len+1]) {
    str[0] = c;
    str[1] = '\0';
}

String::String(const String& str): len(str.len), str(new char[len+1]) {
    strcpy(this->str, str.str);
}

String::~String() {
    delete[] str;
}

String& String::operator=(const String& rhs) {
    if (this != &rhs) {
        delete[] str;
        len = rhs.len;
        strcpy(str, rhs.str);
    }    
    return *this; 
}

size_t String::get_len() const {return len;}

const char* String::c_str() const {return str;}

String String::operator+(const String& rhs) const {
    String ret;
    ret.len = rhs.len + this->len;
    delete[] ret.str;
    ret.str = new char[ret.len + 1];
    strcpy(ret.str, this->str);
    strcat(ret.str, rhs.str);
    return ret;
}

String String::operator+(const char* rhs) const {
    return *this + String(rhs);
}

String String::operator+(const char rhs) const {
    return *this + String(rhs);
}

std::ostream& operator<<(std::ostream& os, const String& str) {
    os << str.c_str();
    return os;
}

String operator+(const char c, const String& str) {
    return String(c) + str;
}

String operator+(const char* left, const String& right) {
    return String(left) + right;
}

char& String::operator[](const size_t idx) {
    if (idx >= len) {
        throw std::out_of_range("Index is out of range");
    }
    return str[idx];
}

const char& String::operator[](const size_t idx) const {
    if (idx >= len) {
        throw std::out_of_range("Index is out of range");
    }
    return str[idx];
}

String::iterator::iterator(char* ptr_c): ptr_c(ptr_c) {}

String::iterator& String::iterator::operator++() {
    if (*ptr_c != '\0') {
        ++ptr_c;
    }
    return *this;
}

String::iterator String::iterator::operator++(int) {
    iterator masolat = *this;
    ++(*this);
    return masolat;
}

char& String::iterator::operator*() const {
    return *ptr_c;
}

char* String::iterator::operator->() const {
    return ptr_c;
}

bool String::iterator::operator==(String::iterator rhs) const {
    return ptr_c == rhs.ptr_c;  // hmmmmmmmmmmmm
}

bool String::iterator::operator!=(String::iterator rhs) const {
    return ptr_c != rhs.ptr_c;
}

String::iterator String::begin() const {
    return String::iterator(str);
}

String::iterator String::end() const {
    return String::iterator(str+len);
}