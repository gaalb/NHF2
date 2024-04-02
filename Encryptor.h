#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include "String.h"

#include "memtrace.h"

class Encryptor {
protected:
    char shiftIntoScope(int c) const;
    bool isValidInput(int c) const; 
public:
    virtual char encode(char c) const = 0;
    String encode(const String& str) const;
    virtual char decode(char c) const = 0;
    String decode(const String& str) const;
    virtual Encryptor* clone() const = 0;
    virtual ~Encryptor();
};

class XorEncryptor: public Encryptor {
private:
    char key;
public:
    XorEncryptor(char key='0');
    void set_key(char key);
    char get_key() const;
    char encode(char c) const;
    char decode(char c) const;
    XorEncryptor operator-() const;
    Encryptor* clone() const;
};

class ShiftEncryptor: public Encryptor {
private:
    int shift;
public:
    ShiftEncryptor(int shift = 0);
    void set_shift(int shift);
    char get_shift() const;
    char encode(char c) const;
    char decode(char c) const;
    ShiftEncryptor operator-() const;
    Encryptor* clone() const;
};


#endif  // ENCRYPTOR_H