#include "StringBase.h"
#include <cstring>
#include <stdexcept>

#include "memtrace.h" //utoljára includeoljuk

size_t StringBase::get_len() const {
    return len;
}

const char* StringBase::c_str() const {
    return str;
}

StringBase::~StringBase() {
    delete[] str;
}

StringBase::StringBase(const char* str): len(strlen(str)), str(new char[len+1]) {
    strcpy(this->str, str);
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

bool StringBase::operator==(const StringBase& rhs) const {
    return strcmp(str, rhs.str) == 0;
}

bool StringBase::operator!=(const StringBase& rhs) const {
    return strcmp(str, rhs.str) != 0;
}

//iterator:
StringBase::iterator::iterator(char* ptr): ptr(ptr) {}

StringBase::iterator& StringBase::iterator::operator++() {
    if (*ptr != '\0') {
        ++ptr;
    }
    return *this;
}

StringBase::iterator StringBase::iterator::operator++(int) {
    iterator masolat = *this;
    ++(*this);
    return masolat;
}

char& StringBase::iterator::operator*() const {
    return *ptr;
}

char* StringBase::iterator::operator->() const {
    return ptr;
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

//const iterator:
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

const char& StringBase::const_iterator::operator*() const { // const !
    return *ptr;
}

const char* StringBase::const_iterator::operator->() const { // const return!
    return ptr;
}

bool StringBase::const_iterator::operator==(const StringBase::const_iterator& rhs) const {
    return ptr == rhs.ptr;
}

bool StringBase::const_iterator::operator!=(const StringBase::const_iterator& rhs) const {
    return ptr != rhs.ptr;
}

StringBase::const_iterator StringBase::begin() const{ //const (iterator-nal is lehetne const, de ez az overloadhoz kell, mert csupán visszatéréssel nem lehet overloadolni)
    return const_iterator(str);
}

StringBase::const_iterator StringBase::end() const { //const (iterator-nal is lehetne const, de ez az overloadhoz kell, mert csupán visszatéréssel nem lehet overloadolni)
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