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

/**
 * @brief A C stringeket kezelő osztály implementációja
 * Az osztály dinamikus tömbként működik: a konstruktorok, és az = operátor
 * foglalnak helyet a len(+1) karakternek, melyek tömbjére str mutat.
 * 
 */
class String {
protected:
    /**
     * @brief A string mérete, a lezáró 0 nélkül.
     * 
     */
    size_t len; 

    /**
     * @brief A strin 0. elemére mutató pointer.
     * 
     */
    char* str;    
public:
    /**
     * @brief Publikus getter a hosszra.
     * 
     * @return size_t this->len
     */
    size_t get_len() const;

    /**
     * @brief Publikus getter a c string reprezentációra
     * 
     * @return const char* this->str
     */
    const char* c_str() const;

    /**
     * @brief C típusú stringet váró konstruktor, valamint paraméter nélküli konstruktor. 
     * 
     * @param str A C string ami bemásolásra kerül this->str adattagba .
     */
    String(const char* str = "");

    /**
     * @brief Egyetlen karaktert váró konstruktor.
     * 
     * @param c A karakter amiből állni fog a String.
     * 
     */
    String(const char c);

    /**
     * @brief Másoló konstruktor: deep copy megvalósítása.
     * 
     * @param other A másolt String.
     * 
     */
    String(const String& other);

    /**
     * @brief A String dinamikus memóriakezelést valósít meg, így szükség van 
     * destruktorra, ami felszabadítja a foglalt területet.
     * 
     */
    ~String();

    /**
     * @brief Értékadás operátor.
     * 
     * @param rhs A String aminek az értékét másoljuk.
     * @return String& típusú, az objektumra referencia aminek a metódusaként meghívódott a
     * függvény, a láncolhatóság érdekében.
     */
    String& operator=(const String& rhs);

    /**
     * @brief Összeadás operátor: konkatenál két Stringet.
     * 
     * @param rhs A String, amely az eredményben jobb oldalt lesz.
     * @return String, a két String összefűzve.
     */
    String operator+(const String& rhs) const;
    #if 0
    /**
     * @brief Összeadás operátor: konkatenál egy Stringet, és egy c stringet.
     * 
     * @param rhs A c string, amely az eredményben jobb oldalt lesz.
     * @return String, a bal oldali Stringhez hozzáfűzve a c string.
     */
    String operator+(const char* rhs) const;

    /**
     * @brief Összeadás operátor: egy String végére fűz egy karaktert.
     * 
     * @param rhs A karakter, amely az eredményben zárni fogja a stringet.
     * @return String, a bal oldlai Stringhez hozzáfűzve a karakter.
     */
    String operator+(const char rhs) const;
    #endif
    /**
     * @brief Indexelő operátor (nem konstans)
     * 
     * @param idx A keresett elem indexe.
     * @return char&, Referencia a keresett elemre, amely módosítható is ezen a 
     * referencián keresztül.
     */
    char& operator[](const size_t idx);

    /**
     * @brief Indexelő operátor (konstans).
     * 
     * @param idx A keresett elem indexe.
     * @return const char&, konstans referencia a keresett elemre
     */
    const char& operator[](const size_t idx) const;

    ////////////////////////////////////////////

    bool operator==(const String& rhs) const;
    bool operator!=(const String& rhs) const;
    /*
    bool operator==(const char* rhs) const;
    bool operator!=(const char* rhs) const;
    bool operator==(const char c) const;
    bool operator!=(const char c) const;
    */

    String& operator+=(const String& str);
    //String& operator+=(const char* str);
    //String& operator+=(const char c);

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

/**
 * @brief Kiíratása egy String objektumnak
 * 
 * @param os Ostream objektum, amivel írunk (pl. fájlba, vagy cout-ra).
 * @param str Kiírandó Stream.
 * @return std::ostream& vissza kell adni a láncolhatóságért.
 */
std::ostream& operator<<(std::ostream& os, const String& str);

/**
 * @brief Összeadás operátor, de a bal oldal beépített típus.
 * 
 * @param c A kezdő karaktere a végeredménynek.
 * @param str A String amit a kezdő karakter után fűzünk.
 * @return String Az összefűzés eredménye. 
 */
String operator+(const char c, const String& str);

/**
 * @brief Összeadás operátor, de a bal oldal egy hagyományos C string.
 * 
 * @param c Nullterminált string.
 * @param str A saját String típusunknak megfelelő string.
 * @return String a két string összefűzve.
 */
String operator+(const char* left, const String& right);


/////////////////////////////////////////
bool operator==(const char* left, const String& right);
bool operator!=(const char* left, const String& right);
bool operator==(const char left, const String& right);
bool operator!=(const char left, const String& right);

#endif // STRING_H