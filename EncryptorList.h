/// @file EncryptorList.h
/// @author Gaál Botond
/// @brief Encryptorok láncolt listáját implementáló header.
#ifndef ENCRYPTOR_LIST_H
#define ENCRYPTOR_LIST_H

#include "Encryptor.h"

#include "memtrace.h"

/// @brief Encryptor implementáció, amely Encryptorok láncolt listája.
///
/// A lista duplán láncolt. A tartalmazott elemek Encryptor*-ok, így egy
/// heterogén tárolót hozva létre. Titkosításkor az összes tárolt Encryptor
/// sorrendben titkosít, dekódoláskor fordított sorrendben dekódolnak.
class EncryptorList: public Encryptor {
private:
    /// @brief A láncolt lista egy eleme.
    class Node {
    public:
        Encryptor* pEncr; ///< A tárolt adat.
        Node* next; ///< A következő elem.
        Node* prev; ///< Az előző elem.
        Node(Encryptor* pEncr); ///< Konstruktor, amely dinamikusan foglalt területre mutató pointert vár.
        ~Node();  ///< Destruktor a dinamikusan foglalt adat törlésére.
    };
    Node* head;  ///< A lista eleje: head->prev = nullptr.
    Node* tail;  ///< A lista vége: tail->next = nullptr.
public:
    using Encryptor::encode;  //avoid name hiding
    using Encryptor::decode;  //avoid name hiding
    EncryptorList(); ///< Paraméter nélküli konstruktor, üres listát hoz létre.
    EncryptorList(const EncryptorList& other); ///< Másoló konstruktor, a dinamikus adatok kezelése végett.
    EncryptorList& operator=(const EncryptorList& other); ///< Egyenlőségadás, a dinamikus adatok kezelése végett.
    ~EncryptorList(); ///< Destruktor, a dinamikusan foglalt adatok felszabadítása véget..
    
    /// @brief A lista bejárásához használt iretátor.
    class iterator {
    private:
        Node* current;
    public:
        iterator(Node* current = nullptr);
        iterator& operator++(); ///> Preinkremens operátor: current->next-re lép.
        iterator& operator--(); ///> Predekremens operátor: current->prev-re lép.
        iterator operator++(int); ///> Posztinkremens operátor: current->next-re lép.
        iterator operator--(int); ///> Posztdekremens operátor: current->prev-re lép.
        Encryptor* operator*(); ///> Dereferálás a kurrens Encryptor*-ra.
        Encryptor** operator->();
        bool operator==(iterator rhs) const;
        bool operator!=(iterator rhs) const;
    };
    iterator begin() const; ///> A lista első elemét mutató iterátort generál.
    iterator end() const; ///> A lista első eleme *után* mutató iterátort generál.
    iterator rBegin() const;  ///> A lista első eleme *elé* mutató iterátort generál, a visszafele bejáráshoz.
    iterator rEnd() const;  ///> A lista utolsó elemére mutató iterátort generál, a visszafele bejáráshoz.
    void append(const Encryptor& encr); ///> Klónozza a kapott Encryptort, és hozzáfűzi a lista végéhez.
    void append(Encryptor* pEncr); ///> A kapott dinamikusan foglalt Encryptor pointerét hozzáfűzi a lista végéhez.

    /// @brief Kötelező override klónozó függvény: dinamikusan foglal egy másolatot.
    /// @return A dinamikusan foglalt területre mutató pointer.
    Encryptor* clone() const override;

    /// @brief Kötelező override inverz klónozó függvény: dinamikusan foglal egy inverzet.
    /// @return A dinamikusan foglalt területre mutató pointer.
    Encryptor* cloneInverse() const override;

    /// @brief Kötelező override kódoló függvény: sorrendben átejti a betűt az összes tárolt titkosítón.
    char encode(char c) const override;

    /// @brief Kötelező override dekódoló függvény: fordított sorrendben dekódolja az összes titkosítóval a betűt.
    char decode(char c) const override;
    EncryptorList& operator+=(const Encryptor& rhs); ///> Klónozza az Encryptort, és hozzáfűzi a lista végéhez.
    EncryptorList& operator-=(const Encryptor& rhs); ///> Inverz klónozza az Encryptort, és hozzáfűzi a lista végéhez.
    EncryptorList& operator+=(Encryptor* rhs);  ///> Hozzáfűzi a dinamikusan foglalt titkosítót a lista végéhez.
    EncryptorList& operator-=(Encryptor* rhs);  ///> Inverzét képezi a dinamikusn foglalt titkosítónak, és felfűzi a listára.
    EncryptorList operator-() const;    
};

/// @brief Globális összeadás operátor, amely egy új listát képez az arugmentumaiból.
/// @param e1 A létrejövő listában az első titkosító.
/// @param e2 A létrejövő listában a második titkosító.
EncryptorList operator+(const Encryptor& e1, const Encryptor& e2);

/// @brief Globális összeadás operátor, amely egy új listát képez az arugmentumaiból.
/// @param e1 A létrejövő listában az első titkosító.
/// @param e2 A létrejövő listában a második titkosító *inverze*.
EncryptorList operator-(const Encryptor& e1, const Encryptor& e2);

#endif //ENCRYPTOR_LIST_H