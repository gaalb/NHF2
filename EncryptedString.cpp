#include "EncryptedString.h"

#include "memtrace.h"

EncryptedString::~EncryptedString() {
    delete pEncryptor;
}

EncryptedString::EncryptedString(const String& pw, const String& str):
    password(pw),
    pEncryptor(new ShiftEncryptor(0)),
    str(pEncryptor->encode(str)) {}

EncryptedString::EncryptedString(const String& pw, const String& str, const Encryptor& encr):
    password(pw),
    pEncryptor(encr.clone()),
    str(pEncryptor->encode(str)) {}

EncryptedString::EncryptedString(const String& pw, const String& str, Encryptor* pEncr):
    password(pw),
    pEncryptor(pEncr),
    str(pEncryptor->encode(str)) {}

EncryptedString::EncryptedString(const EncryptedString& other): 
    password(other.password),
    pEncryptor(other.pEncryptor->clone()),
    str(other.str) {}

EncryptedString& EncryptedString::operator=(const EncryptedString& right) {
    password = right.password;
    str = right.str;
    delete pEncryptor;
    pEncryptor = right.pEncryptor->clone();
    return *this;
}

EncryptedString& EncryptedString::operator=(const String& right) {
    str = pEncryptor->encode(right);
    return *this;
}

void EncryptedString::checkPw(const String& pw) const {
    if (password != pw) {throw IncorrectPasswordException("Helytelen jelszo!");}
}

void EncryptedString::set_pw(const String& new_pw, const String& old_pw) {
    checkPw(old_pw);
    password = new_pw;
}

String EncryptedString::decode(const String& pw) const {
    checkPw(pw);
    return pEncryptor->decode(str);
}

EncryptedString& EncryptedString::operator+=(const String& str) {
    this->str += pEncryptor->encode(str);
    return *this;
}

const char* EncryptedString::c_str() const {
    return str.c_str();
}

char& EncryptedString::operator[](const size_t idx) {
    return str[idx];
}

const char& EncryptedString::operator[](const size_t idx) const {
    return str[idx];
}

std::ostream& operator<<(std::ostream& os, const EncryptedString& str) {
    os << str.c_str();
    return os;
}

void EncryptedString::setEncryptor(const Encryptor& enc, const String& pw) {
    checkPw(pw);
    delete pEncryptor;
    pEncryptor = enc.clone();
}

void EncryptedString::setEncryptor(Encryptor* pEnc, const String& pw) {
    checkPw(pw);
    delete pEncryptor;
    pEncryptor = pEnc;
}

//stuff for the iterator
EncryptedString::iterator::iterator(): it() {}

EncryptedString::iterator::iterator(String::iterator it): it(it) {}

EncryptedString::iterator& EncryptedString::iterator::operator++() {
    ++it;
    return *this;
}

EncryptedString::iterator EncryptedString::iterator::operator++(int) {
    iterator masolat = *this;
    ++(*this);
    return masolat;
}

char& EncryptedString::iterator::operator*() const {
    return *it;
}

char* EncryptedString::iterator::operator->() const {
    return it.operator->();
}

bool EncryptedString::iterator::operator==(const iterator& other) const {
    return it == other.it;
}

bool EncryptedString::iterator::operator!=(const iterator& other) const {
    return it != other.it;
}

EncryptedString::iterator EncryptedString::begin() {
    return iterator(str.begin());
}

EncryptedString::iterator EncryptedString::end() {
    return iterator(str.end());
}


#include "memtrace.h"
