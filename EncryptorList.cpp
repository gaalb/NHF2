/**
 * @file EncryptorList.cpp
 * @author Gaál Botond
 * @brief 
 */
#include "EncryptorList.h"

/*A láncolt lista egy eleme. Mivel a láncolt lista ebben az esetben egy heterogén tároló,
egy Node-ban az adat base pointer. Természetesen ezen kívül van egy-egy mutató az
előző és a következő elemre.*/ 

EncryptorList::Node::Node(Encryptor* pEncr): pEncr(pEncr), next(nullptr), prev(nullptr) {}

EncryptorList::Node::~Node() {
    delete pEncr;  //az elemek dinamikusan foglalt területre mutató pointerek -> felszabadítjuk
}

EncryptorList::EncryptorList(): head(nullptr), tail(nullptr) {}

EncryptorList::EncryptorList(const EncryptorList& other): head(nullptr), tail(nullptr) {
    for (iterator iter = other.begin(); iter != other.end(); iter++) {
        /*Az iterátort ha dereferáljuk, megkapjuk a láncolt lista elemeiben tárolt adatot,
        ami (mivel az heterogén tároló) egy Encryptor absztrakt típusra mutató pointer.
        Tehát *iter az Encryptor pointere, és ezért az Encryptor.clone()-hoz az
        (*iter)->clone() visz. A nyíl precedenciája erősebb, ezért kell a zárójel. */
        append((*iter)->clone()); 
    }
}

EncryptorList& EncryptorList::operator=(const EncryptorList& other) {
    if (this != &other) {
        /*kitisztítjuk a meglévő adatokat. Mivel itt törölve lesz az aktuális Node, 
        nem használhatjuk az egyszerű iterátort (iter++ hibát dobna)*/
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next; // elmentjük, hova mutat
            delete current; //kitöröljük
            current = next; //és nézzük, ahova mutatott
        }
        head = tail = nullptr;
        //copy over
        for (iterator iter=other.begin(); iter != other.end(); iter++) {
            append((*iter)->clone());  //*iter típusa Encryptor*, ezért ->
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
    /*kitisztítjuk a meglévő adatokat. Mivel itt törölve lesz az aktuális Node, 
    nem használhatjuk az egyszerű iterátort (iter++ hibát dobna)*/
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void EncryptorList::append(const Encryptor& encr) {
    Node* newNode = new Node(encr.clone()); 
    //ha eddig üres volt a lista, tail->next hibát dobna, máshogy kezeljük
    if (head == nullptr) {
        head = newNode; //head->prev és head->next is 0 (friss Node)
        tail = newNode; //tail->prev és tail->next is 0 (friss Node)
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
    //felépítjük elejétől végéig ugyanazokkal a titkosítókkal (dinamikusan foglaltak!)
    for (iterator iter = begin(); iter != end(); ++iter) {
        newEncryptorPtr->append((*iter)->clone()); //*iter: Encryptor*
    }
    return newEncryptorPtr;
}

Encryptor* EncryptorList::cloneInverse() const {
    EncryptorList* newEncryptorPtr = new EncryptorList();
    /*Ahhoz, hogy ez dekódoljon, a végéről kezdve kell az eredeti kódolók inverzeit
    bele pakolni (ugyanúgy dinamikusan foglalva)*/
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

EncryptorList EncryptorList::operator-() const {
    EncryptorList ret;
    for (iterator iter = rBegin(); iter!= rEnd(); --iter) {
        ret.append((*iter)->cloneInverse());
    }
    return ret;  //ezeknél ki van használva a másoló konstruktor
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

EncryptorList& EncryptorList::operator+=(const Encryptor& rhs) {
    append(rhs);
    return *this;
}

EncryptorList& EncryptorList::operator-=(const Encryptor& rhs) {
    append(rhs.cloneInverse());
    return *this;
}

EncryptorList& EncryptorList::operator+=(Encryptor* rhs) {
    append(rhs);
    return *this;
}

EncryptorList& EncryptorList::operator-=(Encryptor* rhs) {
    append(rhs->cloneInverse());
    delete rhs;
    return *this;
}

#include "memtrace.h"