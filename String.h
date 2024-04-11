/// @file StringBase.h
/// @author Gaál Botond
/// @brief std::string-nek megfelelő "sima" stringek működését megvalósító header.
#ifndef STRING_H
#define STRING_H
#include <cstddef>  // size_t miatt
#include <iostream>
#include "StringBase.h"

#include "memtrace.h"

/// @brief std::string-et megvalósító osztály.
class String : public StringBase{  
public:

    /// @brief a megfelelő StringBase konstruktorra hivatkozó konstruktor.
    String(const char* str = "");

    /// @brief a megfelelő StringBase konstruktorra hivatkozó konstruktor.
    String(const char c);
    
    /// @brief a megfelelő StringBase konstruktorra hivatkozó konstruktor.
    String(const StringBase& other);
    ~String();
    
    /// @brief Egyenlőségadás operátor: törli a meglévő dinamikus tömböt, újrafoglalja, és másol.
    /// @return String&, referencia magára a Stringre, a láncolhatóság végett.
    String& operator=(const StringBase& rhs) override;
    
    /// @brief Egyenlőségadás operátor: törli a meglévő dinamikus tömböt, újrafoglalja, és másol.
    /// @return String&, referencia magára a Stringre, a láncolhatóság végett.
    String& operator=(const char* rhs) override;
    
    /// @brief Egyenlőségadás operátor: törli a meglévő dinamikus tömböt, újrafoglalja, és másol.
    /// @return String&, referencia magára a Stringre, a láncolhatóság végett.
    String& operator=(const char rhs) override;
    
    /// @brief Egyenlőségadás operátor: törli a meglévő dinamikus tömböt, újrafoglalja, és másol.
    /// @return String&, referencia magára a Stringre, a láncolhatóság végett.
    String& operator=(const String& rhs);
    
    /// @brief Összeadás operátor: egy meglévő String-hez egy másikat fűz.
    /// @param str A hozzáfűzendő String, vagy más olyan változó ami Stringgé alakítható impliciten.
    String operator+(const String& rhs) const; 
    
    /// @brief += operátor: a Stringhez egy másikat fűz.
    /// @param str A hozzáfűzendő String, vagy más olyan változó ami Stringgé alakítható impliciten.
    /// @return String&, referencia magára a Stringre.
    String& operator+=(const String& str);   
};

/// @brief Globális char+String operátor.
/// @param c Bal oldali argumentum.
/// @param str Jobb oldali argumentum.
/// @return Egy új String objektum, benne az argumentumok összefűzve.
String operator+(const char c, const String& str);

/// @brief Globális char*+String operátor.
/// @param left Bal oldali argumentum.
/// @param right Jobb oldali argumentum.
/// @return Egy új String objektum, benne az argumentumok összefűzve.
String operator+(const char* left, const String& right);

#endif // STRING_H