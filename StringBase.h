#ifndef STRING_BASE_H
#define STRING_BASE_H

#include <cstddef>  //size_t miatt
#include <iostream>

#include "memtrace.h" //utoljára includeoljuk

class StringBase {
protected:
    size_t len;
    char* str;
public:
    size_t get_len() const;
    const char* c_str() const;
    virtual ~StringBase();
    StringBase(const char* str = "");
    StringBase(const char c);
    StringBase(const StringBase& other);
    char& operator[](const size_t idx);
    const char& operator[](const size_t idx) const;
    virtual StringBase& operator=(const StringBase& rhs) = 0;    
    bool operator==(const char c) const;
    bool operator!=(const char c) const;
    bool operator==(const char* rhs) const;
    bool operator!=(const char* rhs) const;
    bool operator==(const StringBase& rhs) const;
    bool operator!=(const StringBase& rhs) const;

    class iterator {
    private:
        char* ptr;
    public:
        iterator(char* ptr = NULL);
        iterator& operator++();
        iterator operator++(int);
        char& operator*() const;
        char* operator->() const;
        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;
    };
    iterator begin();
    iterator end();

    class const_iterator{
    private:
        char* ptr;
    public:
        const_iterator(char* ptr = NULL);
        const_iterator& operator++();
        const_iterator operator++(int);
        const char& operator*() const;
        const char* operator->() const;
        bool operator==(const const_iterator& rhs) const;
        bool operator!=(const const_iterator& rhs) const;
    }; 
    const_iterator begin() const;
    const_iterator end() const;
};

std::ostream& operator<<(std::ostream& os, const StringBase& str);
bool operator==(const char* left, const StringBase& right);
bool operator!=(const char* left, const StringBase& right);
bool operator==(const char left, const StringBase& right);
bool operator!=(const char left, const StringBase& right);
#endif //STRING_BASE_H