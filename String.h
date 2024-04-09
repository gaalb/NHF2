/**
 * @file String.h
 * @author Gaál Botond
 * @brief 
 */
#ifndef STRING_H
#define STRING_H
#include <cstddef>  // size_t miatt
#include <iostream>
#include "StringBase.h"

#include "memtrace.h"

class String : public StringBase{  
public:
    String(const char* str = "");
    String(const char c);
    String(const StringBase& other);
    ~String();
    String& operator=(const StringBase& rhs) override;
    String& operator=(const char* rhs) override;
    String& operator=(const char rhs) override;
    String& operator=(const String& rhs);
    String operator+(const String& rhs) const; 
    String& operator+=(const String& str);   
};

String operator+(const char c, const String& str);
String operator+(const char* left, const String& right);

#endif // STRING_H