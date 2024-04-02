#include "EncryptorList.h"

EncryptorList::Node::Node(Encryptor* pEncr): pEncr(pEncr), next(nullptr), prev(nullptr) {}

EncryptorList::Node::~Node() {
    delete pEncr;
}

EncryptorList::EncryptorList(): head(nullptr), tail(nullptr) {}

EncryptorList::iterator::iterator(Node* current): current(current) {}

EncryptorList::iterator& EncryptorList::iterator::operator++() {
    if (current != nullptr) {
        current = current->next;
    }
    return *this;
}

EncryptorList::iterator& EncryptorList::iterator::operator--() {
    if (current != nullptr) {
        current = current->prev;
    }
    return *this;    
}

EncryptorList::iterator EncryptorList::iterator::operator++(int) {
    iterator copy = *this;
    ++(*this);
    return copy;
}

EncryptorList::iterator EncryptorList::iterator::operator--(int) {
    iterator copy = *this;
    --(*this);
    return copy;
}

Encryptor* EncryptorList::iterator::operator*() {
    return current->pEncr;
}

Encryptor** EncryptorList::iterator::operator->() {
    return &(current->pEncr);
}

bool EncryptorList::iterator::operator==(EncryptorList::iterator rhs) const {
    return current == rhs.current;
}

bool EncryptorList::iterator::operator!=(EncryptorList::iterator rhs) const {
    return current != rhs.current;
}

EncryptorList::iterator EncryptorList::begin() const {
    return EncryptorList::iterator(head);
}

EncryptorList::iterator EncryptorList::end() const {
    return EncryptorList::iterator(nullptr);
}

EncryptorList::iterator EncryptorList::rBegin() const {
    return EncryptorList::iterator(tail);
}

EncryptorList::iterator EncryptorList::rEnd() const {
    return EncryptorList::iterator(nullptr);
}

EncryptorList::~EncryptorList() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void EncryptorList::append(const Encryptor& encr) {
    Node* newNode = new Node(encr.clone()); 
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

void EncryptorList::append(Encryptor* pEncr) {
    Node* newNode = new Node(pEncr); 
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

Encryptor* EncryptorList::clone() const {
    EncryptorList* newEncryptorPtr = new EncryptorList();
    for (iterator iter = begin(); iter != end(); ++iter) {
        newEncryptorPtr->append(**iter); //*iter: Encryptor*, **iter: Encryptor (clone())
    }
    return newEncryptorPtr;
}

char EncryptorList::encode(char c) const {
    char encoded = c;
    for (iterator iter = begin(); iter!= end(); ++iter) {
        encoded = (*iter)->encode(c);
    }
    return encoded;
}

char EncryptorList::decode(char c) const {
    char decoded = c;
    for (iterator iter = begin(); iter!= end(); ++iter) {
        decoded = (*iter)->decode(c);
    }
    return decoded;
}

#include "memtrace.h"