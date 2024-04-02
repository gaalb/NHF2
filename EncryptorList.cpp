#include "EncryptorList.h"

EncryptorList::Node::Node(Encryptor* pEncr): pEncr(pEncr), next(nullptr), prev(nullptr) {}

EncryptorList::Node::~Node() {
    delete pEncr;
}

EncryptorList::EncryptorList(): head(nullptr), tail(nullptr) {}

EncryptorList::EncryptorList(const EncryptorList& other): head(nullptr), tail(nullptr) {
    for (iterator iter = other.begin(); iter != other.end(); iter++) {
        append((*iter)->clone());  //*iter: Encryptor*, hence the ->
    }
}

EncryptorList& EncryptorList::operator=(const EncryptorList& other) {
    if (this != &other) {
        //clear
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        //copy over
        for (iterator iter=other.begin(); iter != other.end(); iter++) {
            append((*iter)->clone());  //*iter: Encryptor*, hence the ->
        }
    }
    return *this;
}

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
        newEncryptorPtr->append((*iter)->clone()); //*iter: Encryptor*
    }
    return newEncryptorPtr;
}

Encryptor* EncryptorList::cloneInverse() const {
    EncryptorList* newEncryptorPtr = new EncryptorList();
    for (iterator iter = rBegin(); iter != rEnd(); --iter) {
        newEncryptorPtr->append((*iter)->cloneInverse()); //*iter: Encryptor*, **iter: Encryptor (clone())
    }
    return newEncryptorPtr;
}

char EncryptorList::encode(char c) const {
    char encoded = c;
    for (iterator iter = begin(); iter!= end(); ++iter) {
        encoded = (*iter)->encode(encoded);
    }
    return encoded;
}

char EncryptorList::decode(char c) const {
    char decoded = c;
    for (iterator iter = rBegin(); iter!= rEnd(); --iter) {
        decoded = (*iter)->decode(decoded);
    }
    return decoded;
}

EncryptorList EncryptorList::operator-() {
    EncryptorList ret;
    for (iterator iter = rBegin(); iter!= rEnd(); --iter) {
        ret.append((*iter)->cloneInverse());
    }
    return ret;  //needs copy ctor
}

EncryptorList operator+(const Encryptor& e1, const Encryptor& e2) {
    EncryptorList ret;
    ret.append(e1);
    ret.append(e2);
    return ret;
}

EncryptorList operator-(const Encryptor& e1, const Encryptor& e2) {
    EncryptorList ret;
    ret.append(e1);
    ret.append(e2.cloneInverse());
    return ret;
}

#include "memtrace.h"