#ifndef ENCRYPTOR_LIST_H
#define ENCRYPTOR_LIST_H

#include "Encryptor.h"

#include "memtrace.h"

class EncryptorList: public Encryptor {
private:
    class Node {
    public:
        Encryptor* pEncr;
        Node* next;
        Node* prev;        
        Node(Encryptor* pEncr);
        ~Node();
    };
    Node* head;  //head->prev is nullptr
    Node* tail;  //tail->next is nullptr
public:
    using Encryptor::encode;  //avoid name hiding
    using Encryptor::decode;  //avoid name hiding
    EncryptorList();
    EncryptorList(const EncryptorList& other);
    EncryptorList& operator=(const EncryptorList& other); 
    ~EncryptorList();
    class iterator {
    private:
        Node* current;
    public:
        iterator(Node* current = nullptr);
        iterator& operator++();
        iterator& operator--();
        iterator operator++(int);
        iterator operator--(int);
        Encryptor* operator*();
        Encryptor** operator->();
        bool operator==(iterator rhs) const;
        bool operator!=(iterator rhs) const;
    };
    iterator begin() const;
    iterator end() const;
    iterator rBegin() const;  //reverse
    iterator rEnd() const;  //reverse
    void append(const Encryptor& encr);
    void append(Encryptor* pEncr);
    Encryptor* clone() const override;
    Encryptor* cloneInverse() const override;
    char encode(char c) const override;
    char decode(char c) const override;
    EncryptorList& operator+=(const Encryptor& rhs);
    EncryptorList& operator-=(const Encryptor& rhs);
    EncryptorList& operator+=(Encryptor* rhs);  //nem biztos hogy ezt akarjuk, hogy létezzen
    EncryptorList& operator-=(Encryptor* rhs);  //nem biztos hogy ezt akarjuk, hogy létezzen
    EncryptorList operator-();    
};

EncryptorList operator+(const Encryptor& e1, const Encryptor& e2);

EncryptorList operator-(const Encryptor& e1, const Encryptor& e2);

#endif //ENCRYPTOR_LIST_H