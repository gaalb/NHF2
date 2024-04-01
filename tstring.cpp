#include "tstring.h"

#include "memtrace.h"

TString::~TString() {
    delete titkosito;
}

TString::TString(const String& jelszo, const String& str):
    jelszo(jelszo),
    titkosito(new ShiftTitkosito(0)),
    str(titkosito->encode(str)) {}

TString::TString(const String& jelszo, const String& str, const Titkosito& t):
    jelszo(jelszo),
    titkosito(t.clone()),
    str(titkosito->encode(str)) {}

TString::TString(const String& jelszo, const String& str, Titkosito* p_t):
    jelszo(jelszo),
    titkosito(p_t),
    str(titkosito->encode(str)) {}

TString::TString(const TString& other): 
    jelszo(other.jelszo),
    titkosito(other.titkosito->clone()),
    str(other.str) {}

TString& TString::operator=(const TString& right) {
    jelszo = right.jelszo;
    str = right.str;
    delete titkosito;
    titkosito = right.titkosito->clone();
    return *this;
}

TString& TString::operator=(const String& right) {
    str = titkosito->encode(right);
    return *this;
}

void TString::check_pw(const String& pw) const {
    if (jelszo != pw) {throw IncorrectPasswordException("Helytelen jelszo!");}
}

void TString::set_pw(const String& new_pw, const String& old_pw) {
    check_pw(old_pw);
    jelszo = new_pw;
}

String TString::decode(const String& pw) const {
    check_pw(pw);
    return titkosito->decode(str);
}

TString& TString::operator+=(const String& str) {
    this->str += titkosito->encode(str);
    return *this;
}

const char* TString::c_str() const {
    return str.c_str();
}

char& TString::operator[](const size_t idx) {
    return str[idx];
}

const char& TString::operator[](const size_t idx) const {
    return str[idx];
}

std::ostream& operator<<(std::ostream& os, const TString& str) {
    os << str.c_str();
    return os;
}

void TString::set_decoder(const Titkosito& t, const String& pw) {
    check_pw(pw);
    delete titkosito;
    titkosito = t.clone();
}

void TString::set_decoder(Titkosito* p_t, const String& pw) {
    check_pw(pw);
    delete titkosito;
    titkosito = p_t;
}

//stuff for the iterator
TString::iterator::iterator(): it() {}

TString::iterator::iterator(String::iterator it): it(it) {}

TString::iterator& TString::iterator::operator++() {
    ++it;
    return *this;
}

TString::iterator TString::iterator::operator++(int) {
    iterator masolat = *this;
    ++(*this);
    return masolat;
}

char& TString::iterator::operator*() const {
    return *it;
}

char* TString::iterator::operator->() const {
    return it.operator->();
}

bool TString::iterator::operator==(const iterator& other) const {
    return it == other.it;
}

bool TString::iterator::operator!=(const iterator& other) const {
    return it != other.it;
}

TString::iterator TString::begin() {
    return iterator(str.begin());
}

TString::iterator TString::end() {
    return iterator(str.end());
}


#include "memtrace.h"
