/**
 * @file string.h
 * @author Gaál Botond
 * @brief A String osztály deklarációit tartalmazó file.
 * 
 */

#ifndef STRING_H
#define STRING_H
#include <cstddef>  // size_t miatt
#include <iostream>

#include "memtrace.h"
class String {
protected:
    size_t len; 
    char* str;    
public:
    size_t get_len() const;  //getter
    const char* c_str() const;  //getter
    String(const char* str = "");  //also default ctor
    String(const char c);
    String(const String& other);
    ~String();  //delete allocation
    String& operator=(const String& rhs);  
    String operator+(const String& rhs) const;  
    char& operator[](const size_t idx);
    const char& operator[](const size_t idx) const;
    bool operator==(const String& rhs) const;
    bool operator!=(const String& rhs) const;
    String& operator+=(const String& str);
    class iterator {
    private:
        char* ptr_c;
    public:
        iterator(char* ptr_c = NULL);
        iterator& operator++();
        iterator operator++(int);
        char& operator*() const;
        char* operator->() const;
        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;
    };
    iterator begin() const;
    iterator end() const;
};
std::ostream& operator<<(std::ostream& os, const String& str);
String operator+(const char c, const String& str);
String operator+(const char* left, const String& right);
bool operator==(const char* left, const String& right);
bool operator!=(const char* left, const String& right);
bool operator==(const char left, const String& right);
bool operator!=(const char left, const String& right);

#endif // STRING_H