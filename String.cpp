/**
 * @file string.cpp
 * @author Gaál Botond
 * @brief 
 */
#include "StringBase.h"
#include "String.h"
#include <cstring>
#include <stdexcept>

#include "memtrace.h"

String::String(const char* str): StringBase(str) {}

String::String(const char c): StringBase(c) {}

String::String(const StringBase& other): StringBase(other) {}

String::~String() {}

String& String::operator=(const StringBase& rhs) {
    if (this != &rhs) {
        delete[] str;
        len = rhs.get_len();
        str = new char[len+1];
        strcpy(str, rhs.c_str());
    }    
    return *this; 
}

/*Elvileg ha azokat a függvényeket, melyek const String&-et 
vesznek át egy char, char* vagy StringBase& argumentummal
hívjuk meg, mivel van megfelelő konstruktor, át tudja konvertálni
Stringgé a char vagy char*-t. Ennek ellenére az én IDE-m az ilyen
felhasználásnál panaszkodik (hiába fordul a program).

Másnap másik fordítón ezt megnézve: valszeg az extra
operator=konstruktorokkal ez nem lesz baj.
A problémát specifikusan ez okozta:
String s1;
s1 = "valami"
A probléma az, hogy mivel a StringBase előírja, hogy lennie kell
StringBase-t fogadó operator=nek, muszáj egy ilyet csinálni String-nek.
Ez logikus is, mert különböző StringBase-ek között értelmesnek kell lennie
az értékadásnak (habár mindenhol különböző dolgot jelenthet). Azonban ha 
String=String-re felüldefiniáljuk (ami nem is feltétlen szükséges valószínűleg
a megoldás után), akkor a fordító nem tudja eldönteni, hogy egy konverziót igénylő
operator= hívásnál melyiket hívja. Pl.  a fenti példában megpróbálhatná a 
"valami" char* típust mind String-gé, mind StringBase-é konvertálni. Ez utóbbinak
persze nincs értelme, hiszen ahhoz, hogy "valami"-t StringBase-é konvertáljuk, hogy
átadjuk a függvénynek, arra lenne szükség, hogy példányosíthassuk StringBase-t, amit
nem tehetünk mert absztrakt. Erre az otthoni fordítóm rájött, a laptopon lévő nem.
A megoldás, hogy előírunk minden StringBase-nek char és char*-ból konvertáló 
operator= műveleteket (ezek léte egyébként logikus is). 
Ezután azt gondolnám, így már nem lesz szükséges az operator=(String), mert
azokra az esetekre, mikor String a rhs, ott lesz a StringBase argumentumú
verzió. Ennek ellenére ha kihagyom az operator=-t, hibát dob a linker. Az, hogy ez
miért van, azt még nem találtam ki. Mindenesetre itt van a hiba:
C:\Users\User\AppData\Local\Temp\cc87Wgch.o: In function `String::operator=(String const&)':
D:/NHF2/String.h:9: undefined reference to `StringBase::operator=(StringBase const&)'
C:\Users\User\AppData\Local\Temp\ccr0YvoS.o: In function `String::operator=(String&&)':
D:/NHF2/String.h:9: undefined reference to `StringBase::operator=(StringBase const&)'
extra megjegyzés ehhez:
Megpróbáltam kitörölni a StringBase-ben a kötelező operator= függvényre a 
kötelezettséget, hogy ne keresse (hiszen a hibaüzenetből látszik hogy keresi, de
nem találja), de úgy, hogy a megfelelőjét meghagytam String-ben. Ekkor valamiért
memtrace probléma volt, valamelyik ponton ki akartunk deletezni egy már felszabadított
string-et??

A további String&-et átvevő függvényeknek ez nem lesz probléma, mert ott nincsen 
StringBase-t átvevő verzió, így nincs semmi amit semmivel össze lehetne keverni. 
Vagyis, mikor az operator+(const String&)-nek adunk egy char/char*-ot, ott azonnal
tudja, hogy String-gé kell konvertálnia, és nem jön szóba a StringBase.
*/
String& String::operator=(const String& rhs) {
    if (this != &rhs) {
        delete[] str;
        len = rhs.get_len();
        str = new char[len+1];
        strcpy(str, rhs.c_str());
    }    
    return *this; 
}

String& String::operator=(const char* rhs) {
    return operator=(String(rhs));
}

String& String::operator=(const char rhs) {
    return operator=(String(rhs));
}

String String::operator+(const String& rhs) const {
    String ret;
    ret.len = rhs.len + this->len;
    delete[] ret.str;
    ret.str = new char[ret.len + 1];
    strcpy(ret.str, this->str);
    strcat(ret.str, rhs.str);
    return ret;
}

String& String::operator+=(const String& str) {
    *this = *this + str;
    return *this;
}

String operator+(const char c, const String& str) {
    return String(c) + str;
}

String operator+(const char* left, const String& right) {
    return String(left) + right;
}
