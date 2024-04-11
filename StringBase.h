/// @file StringBase.h
/// @author Gaál Botond
/// @brief Virtuális string őszosztály header fájl.
#ifndef STRING_BASE_H
#define STRING_BASE_H

#include <cstddef>  //size_t miatt
#include <iostream>

#include "memtrace.h" //utoljára includeoljuk

/// @brief Absztrakt string őszosztály.
///
/// Ez az osztály foglalja magába az összes string implementáció közös tulajdonságát.
/// @note Minden StringBase egy dinamikus tömb: a konstruktorok és érték adó operátorok
/// foglalnak dinamikus területet, a destruktor felszabadítja őket.
class StringBase {
protected:
    size_t len; ///< A tárolt karaktertömbből a hasznos (nem lezáró 0) karakterek száma.
    char* str;  ///< A dinamikusan foglalt karaktertömb pointere.
public:
    size_t get_len() const; ///< A protected len publikus gettere. A lezáró 0 nincs beszámolva.
    const char* c_str() const; ///< A protected karaktertömb publikus gettere.
    virtual ~StringBase(); ///< *Virtuális* destruktor, amely felszabadítja a dinamikusan tárolt adatot.
    
    /// @brief A paraméter nélküli, és C stringet váró konstruktor. Dinamikus helyfoglalással jár.
    /// @param str **Nullterminált** C string, mely bemásolódik this->str-be.
    StringBase(const char* str = "");
    StringBase(const char c);  ///< Egy karaktert váró konstruktor, amely 1 hosszú stringet eredményez.
    StringBase(const StringBase& other); ///< StringBase paraméterű konstruktor.
    
    /// @brief Indexelő operátor.
    /// @param idx A keresett karakter indexe.
    /// @return char& típusú, ugyanis a visszatérésen keresztül módosítható str tartalma.
    char& operator[](const size_t idx);
    
    /// @brief *Konstans* indexelő operátor, mely const StringBase-ekre is meghívható.
    /// @param idx A keresett karakter indexe.
    /// @return const char& típusú, ugyanis a visszatérésen keresztül *nem* módosítható str tartalma.
    const char& operator[](const size_t idx) const;
    
    /// @brief Tisztán virtuális értékadás operátor.
    /// Stringbase-eket logikusan muszáj tudni egymásnak értékül adni, hiszen
    /// mind karaktertömböket tárolnak, így ennek a függvénynek léteznie kell,
    /// azonban konkatenálás a kódolás miatt bizonyos esetekben nem 
    /// csak egy strcpy-ról lesz szó, vagyis a konkrét megvalósítás implementáció függő.
    /// @param rhs Az értékül adandó StringBase objektum(referencia)
    /// @return StringBase&, referencia saját magára
    virtual StringBase& operator=(const StringBase& rhs) = 0;
    
    /// @brief Tisztán virtuális értékadás operátor.
    /// @see StringBase& operator=(const StringBase& rhs)
    virtual StringBase& operator=(const char* rhs) = 0;
    
    /// @brief Tisztán virtuális értékadás operátor.
    /// @see StringBase& operator=(const StringBase& rhs)
    virtual StringBase& operator=(const char rhs) = 0;
    
    /// @brief Összehasonlító operátor.
    /// 
    /// Mivel minden StringBase objektum mögött egy karakter tömb lakozik,
    /// ezeknek összehasonlíthatónak kell lenniük.
    /// @param rhs Az összehasonlítás jobb oldala.
    /// @return bool: Megegyeznek-e a karaktertömbök.
    bool operator==(const StringBase& rhs) const;   
    
    /// @brief Összehasonlító operátor.
    /// @see operator==(const StringBase& rhs) const
    bool operator==(const char c) const;
    
    /// @brief Összehasonlító operátor.
    /// @see operator==(const StringBase& rhs) const
    bool operator!=(const char c) const;
    
    /// @brief Összehasonlító operátor.
    /// @see operator==(const StringBase& rhs) const
    bool operator==(const char* rhs) const;
    
    /// @brief Összehasonlító operátor.
    /// @see operator==(const StringBase& rhs) const
    bool operator!=(const char* rhs) const;
    
    /// @brief Összehasonlító operátor.
    /// @see operator==(const StringBase& rhs) const 
    bool operator!=(const StringBase& rhs) const;
    
    /// @brief Nem konstans iterátor osztály*
    class iterator {
    private:
        char* ptr; ///< Pointer a karakterre, ahol az iterátor jár.
    public:
        iterator(char* ptr = NULL);
        iterator& operator++();  ///< Preinkremens operátor.
        iterator operator++(int); ///< Posztinkremens operátor.
        
        /// @brief Indirekció operátor.
        /// @return char&, ugyanis módosítható rajta keresztül a mutatott karakter.
        char& operator*() const; 
        char* operator->() const; 
        bool operator==(const iterator& rhs) const; 
        bool operator!=(const iterator& rhs) const;
    };
    iterator begin();  ///< Nulladik karakterre mutató iterátor.
    iterator end(); ///< Lezáró 0-ra mutató iterátor.

    /// @brief Konstans iterátor osztály, amellyel const StringBase is bejárható.
    class const_iterator{
    private:
        char* ptr; ///< Pointer a karakterre, ahol az iterátor jár.
    public:
        const_iterator(char* ptr = NULL);
        const_iterator& operator++(); ///< Preinkremens operátor.
        const_iterator operator++(int); ///< Posztinkremens operátor.
        
        /// @brief Indirekció operátor.
        /// @return const char&, ugyanis nem módosítható rajta keresztül a mutatott karakter.
        const char& operator*() const;
        const char* operator->() const;
        bool operator==(const const_iterator& rhs) const;
        bool operator!=(const const_iterator& rhs) const;
    }; 
    
    /// @brief Nulladik karakterre mutató iterátor. Const, így meghívható const StringBase-re.
    /// @return Lezáró 0-ra mutató iterátor. Const, így meghívható const StringBase-re.
    const_iterator begin() const;
    const_iterator end() const;
};

/// @brief Kiíró operátor.
/// @param os Az std::ostream objektum, amire kiírunk.
/// @param str A StringBase, melynek a protected C stringjét kiírjuk.
/// @return std::ostream&, referencia az ostream-re amire kiírtunk, hogy láncolható legyen.
std::ostream& operator<<(std::ostream& os, const StringBase& str);

/// @brief Beolvasó operátor.
/// 
/// Az operátor a leadin whitespace-eket eldobja, majd az első whitespace-ig olvas.
/// @param  is Az std::istream objektum, amelyből beolvasunk.
/// @param  str A StringBase, melybe beolvasunk.
/// @return std::istream&, referencia az istream-re, amiből olvastunk, hogy láncolható legyen.
std::istream& operator>>(std::istream& is, StringBase& str);

/// @brief Összehasonlító operátor.
/// @see StringBase& operator=(const char rhs)
bool operator==(const char* left, const StringBase& right);

/// @brief Összehasonlító operátor.
/// @see StringBase& operator=(const char rhs)
bool operator!=(const char* left, const StringBase& right);

/// @brief Összehasonlító operátor.
/// @see StringBase& operator=(const char rhs)
bool operator==(const char left, const StringBase& right);

/// @brief Összehasonlító operátor.
/// @see StringBase& operator=(const char rhs)
bool operator!=(const char left, const StringBase& right);
#endif //STRING_BASE_H