#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include "StringBase.h"
#include "String.h"

#include "memtrace.h"

class Encryptor {
protected:
    static char first_char;
    static char last_char;
    char shiftIntoScope(int c) const;
    bool isValidInput(int c) const; 
public:
    virtual char encode(char c) const = 0;
    String encode(const String& str) const;
    virtual char decode(char c) const = 0;
    String decode(const String& str) const;
    virtual Encryptor* clone() const = 0;
    virtual Encryptor* cloneInverse() const = 0;
    virtual ~Encryptor();
};

class XorEncryptor: public Encryptor {
private:
    char key;
public:
    using Encryptor::encode;  //avoid name hiding
    using Encryptor::decode;  //avoid name hiding
    XorEncryptor(char key='0');
    void set_key(char key);
    char get_key() const;
    char encode(char c) const override;
    char decode(char c) const override;
    XorEncryptor operator-() const;
    Encryptor* clone() const override;
    Encryptor* cloneInverse() const override;
};

class ShiftEncryptor: public Encryptor {
private:
    int shift;
public:
    using Encryptor::encode;  //avoid name hiding
    using Encryptor::decode;  //avoid name hiding
    ShiftEncryptor(int shift = 0);
    void set_shift(int shift);
    char get_shift() const;
    char encode(char c) const override;
    char decode(char c) const override;
    ShiftEncryptor operator-() const;
    Encryptor* clone() const override;
    Encryptor* cloneInverse() const override;
};

#endif  // ENCRYPTOR_H