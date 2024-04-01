#ifndef TSTRING_H
#define TSTRING_H

#include "string.h"
#include "titkosito.h"
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

class TString {
private:
    String jelszo;
    Titkosito* titkosito;
    String str;    
    void check_pw(const String& pw) const;
public:
    ~TString();
    TString(const String& jelszo, const String& str = ""); //csak jelszo
    TString(const String& jelszo, const String& str, const Titkosito& t);    
    TString(const String& jelszo, const String& str, Titkosito* p_t);  
    TString(const TString& other);
    TString& operator=(const TString& right);
    TString& operator=(const String& right);
    void set_pw(const String& new_pw, const String& old_pw);
    String decode(const String& pw) const;
    TString& operator+=(const String& str);
    const char* c_str() const;
    char& operator[](const size_t idx);
    const char& operator[]( const size_t idx) const;
    void set_decoder(const Titkosito& t, const String& pw);
    void set_decoder(Titkosito* p_t, const String& pw);
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

std::ostream& operator<<(std::ostream& os, const TString& str);

#endif // TSTRING_H