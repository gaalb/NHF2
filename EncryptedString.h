#ifndef ENCRYPTED_STRING_H
#define ENCRYPTED_STRING_H

#include "String.h"
#include "Encryptor.h"
#include <stdexcept>

#include "memtrace.h"

class IncorrectPasswordException: public std::exception {
private:
    String msg;
public:
    IncorrectPasswordException(const String& msg): msg(msg) {}
    virtual const char* what() const noexcept override {
        return msg.c_str();
    }
};

class EncryptedString {
private:
    String password;
    Encryptor* pEncryptor;
    String str;    
    void checkPw(const String& pw) const;
public:
    ~EncryptedString();
    EncryptedString(const String& pw, const String& str = ""); //csak jelszo
    EncryptedString(const String& pw, const String& str, const Encryptor& encr); //reference param   
    EncryptedString(const String& pw, const String& str, Encryptor* pEncr);  //ptr param
    EncryptedString(const EncryptedString& other);
    EncryptedString& operator=(const EncryptedString& right);
    EncryptedString& operator=(const String& right);
    void set_pw(const String& new_pw, const String& old_pw);
    String decode(const String& pw) const;
    EncryptedString& operator+=(const String& str);
    const char* c_str() const;
    char& operator[](const size_t idx);
    const char& operator[]( const size_t idx) const;
    void setEncryptor(const Encryptor& enc, const String& pw); //reference param
    void setEncryptor(Encryptor* pEnc, const String& pw); //ptr param
    class iterator {
    private:
        String::iterator it;
    public:
        iterator();
        iterator(String::iterator it);
        iterator& operator++();
        iterator operator++(int);
        char& operator*() const;
        char* operator->() const;
        bool operator==(const iterator& other) const;
        bool operator!=(const iterator& other) const;
    };
    iterator begin();
    iterator end();

};

std::ostream& operator<<(std::ostream& os, const EncryptedString& str);

#endif // ENCRYPTED_STRING_H