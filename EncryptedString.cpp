#include "EncryptedString.h"
#include <cstring>

#include "memtrace.h"

EncryptedString::~EncryptedString() {
    delete pEncryptor;
}

void EncryptedString::set_str(const StringBase& new_str) {
    len = new_str.get_len();
    delete[] str;
    str = new char[len+1];
    strcpy(str, new_str.c_str());
}

EncryptedString::EncryptedString(const Encryptor& encr, const String& pw, const char* str):
    StringBase(str),
    pEncryptor(encr.clone()),
    password(pw) {set_str(pEncryptor->encode(str));}

EncryptedString::EncryptedString(Encryptor* pEncr, const String& pw, const char* str):
    StringBase(str),
    pEncryptor(pEncr),
    password(pw) {set_str(pEncryptor->encode(str));}

EncryptedString::EncryptedString(const Encryptor& encr, const String& pw, const char c):
    StringBase(c),
    pEncryptor(encr.clone()),
    password(pw) {set_str(pEncryptor->encode(str));}

EncryptedString::EncryptedString(Encryptor* pEncr, const String& pw, const char c):
    StringBase(c),
    pEncryptor(pEncr),
    password(pw) {set_str(pEncryptor->encode(str));}

EncryptedString::EncryptedString(const Encryptor& encr, const String& pw, const String& str):
    StringBase(str),
    pEncryptor(encr.clone()),
    password(pw) {set_str(pEncryptor->encode(str));}

EncryptedString::EncryptedString(Encryptor* pEncr, const String& pw, const String& str):
    StringBase(str),
    pEncryptor(pEncr),
    password(pw) {set_str(pEncryptor->encode(str));}

EncryptedString::EncryptedString(const EncryptedString& other): 
    StringBase(other),
    pEncryptor(other.pEncryptor->clone()),
    password(other.password) {}

EncryptedString& EncryptedString::operator=(const StringBase& right) {
    set_str(pEncryptor->encode(right));
    return *this;
}

EncryptedString& EncryptedString::operator=(const EncryptedString& right) {
    password = right.password; 
    set_str(right);
    delete pEncryptor;
    pEncryptor = right.pEncryptor->clone();
    return *this;
}

EncryptedString& EncryptedString::operator+=(const String& str) {
    String additional = pEncryptor->encode(str);
    set_str(this->str + additional);
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


#include "memtrace.h"
