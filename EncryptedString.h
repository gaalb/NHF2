/// @file EncryptedString.h
/// @author Gaál Botond
/// @brief 

#ifndef ENCRYPTED_STRING_H
#define ENCRYPTED_STRING_H

#include "String.h"
#include "Encryptor.h"
#include <stdexcept>

#include "memtrace.h"

/// @brief Kivétel osztály a hibás jelszó jelzésére.
class IncorrectPasswordException: public std::exception {
private:
    String msg;  ///< Az üzenet ami felhívja a hibás jelszóra a figyelmet.
public:
    IncorrectPasswordException(const String& msg);  ///< Konstruktor, melynek meg kell adni az üzenetet.
    virtual const char* what() const noexcept override; ///< C stringet visszaadó függvény.
};

/// @brief A házi feladatot ténylegesen megvalósító titkosított string osztály.
class EncryptedString : public StringBase {
private:    
    Encryptor* pEncryptor; ///< Pointer a *dinamikusan foglalt* titkosítóra.
    String password; ///< A jelszó, amellyel hozzá lehet férni a nyers adathoz.

    /// @brief Függvény, amely csekkolja, helyes-e a jelszó.
    /// @note Ha nem helyes a megadott jelszó, IncorrectPasswordException-t dob!
    void checkPw(const String& pw) const;

    /// @brief Függvény, amellyel közvetlenül be lehet állítani a C-stringet.
    ///
    /// Ez a függvény privát, mivel csak egy könnyebbségi függvény a többi
    /// privát metódus használatára. Ezért nem jelszóvédett.
    /// @param new_str A StringBase (vagy azzá alakítható) objektum, amely C stringje másolva lesz. 
    void set_str(const StringBase& new_str);
public:
    ~EncryptedString(); ///< Destruktor, amely a dinamikusan foglalt Encryptor-t törli. 

    /// @brief C stringet, vagy semmilyen kezdő adatot nem váró konstruktor, mely másolja a titkosítót.
    /// @param encr Encryptor (nem pointer), mely a clone() metódussal másolva lesz.
    /// @param pw A jelszó, mely védeni fogja az adatot.
    /// @param str A tárolt kódolandó adat, lehet üresen hagyni, és akkor "" lesz.
    EncryptedString(const Encryptor& encr, const String& pw, const char* str = ""); 
    
    /// @brief C stringet, vagy semmilyen kezdő adatot nem váró konstruktor, melynek már lefoglalt titkosító pointerét kell adni.
    /// @param pEncr Encryptor*, egy korábban foglalt területre.
    /// @note A pointer new-al foglalt, vagy clone() által adott területre kell mutasson, mert
    /// később meg lesz kísérelve a törlése!
    /// @param pw A jelszó, mely védeni fogja az adatot.
    /// @param str A tárolt kódolandó adat, lehet üresen hagyni, és akkor "" lesz.
    EncryptedString(Encryptor* pEncr, const String& pw, const char* str = "");
 
    /// @brief Karakter kezdő adatot váró konstruktor, mely másolja a titkosítót.
    /// @param encr Encryptor (nem pointer), mely a clone() metódussal másolva lesz.
    /// @param pw A jelszó, mely védeni fogja az adatot.
    /// @param c A tárolt adat titkosítandó adat (1 betű).
    EncryptedString(const Encryptor& encr, const String& pw, const char c); 

    /// @brief Karakter kezdő adatot váró konstruktor, melynek már lefoglalt titkosító pointerét kell adni.
    /// @param pEncr Encryptor*, egy korábban foglalt területre.
    /// @note A pointer new-al foglalt, vagy clone() által adott területre kell mutasson, mert
    /// később meg lesz kísérelve a törlése!
    /// @param pw A jelszó, mely védeni fogja az adatot.
    /// @param c A tárolt adat titkosítandó adat (1 betű).
    EncryptedString(Encryptor* pEncr, const String& pw, const char c);
    
    /// @brief String kezdő adatot váró konstruktor, mely másolja a titkosítót.
    /// @param encr Encryptor (nem pointer), mely a clone() metódussal másolva lesz.
    /// @param pw A jelszó, mely védeni fogja az adatot. 
    /// @param str A kezdő adat, mely titkosítva lesz.
    EncryptedString(const Encryptor& encr, const String& pw, const String& str);

    /// @brief String kezdő adatot váró konstruktor, melynek már lefoglalt titkosító pointerét kell adni.
    /// @param pEncr Encryptor*, egy korábban foglalt területre.
    /// @note A pointer new-al foglalt, vagy clone() által adott területre kell mutasson, mert
    /// később meg lesz kísérelve a törlése!
    /// @param pw A jelszó, mely védeni fogja az adatot.
    /// @param str A kezdő adat, mely titkosítva lesz.
    EncryptedString(Encryptor* pEncr, const String& pw, const String& str);

    /// @brief Másoló konstruktor, mely lemásolja mind a nyers adatot, mind a titkosítást.
    EncryptedString(const EncryptedString& other);

    /// @brief Kötelező override értékadás, amely titkosítja a paraméterét.
    /// @param rhs A titkosítandó string.
    /// @return EncryptedString&, referencia magára a titkosított stringre, a láncolhatósághoz.
    EncryptedString& operator=(const StringBase& rhs) override;

    /// @brief Kötelező override értékadás, amely titkosítja a paraméterét.
    /// @param rhs A titkosítandó nullterminált C string.
    /// @return EncryptedString&, referencia magára a titkosított stringre, a láncolhatósághoz.
    EncryptedString& operator=(const char* rhs) override;

    /// @brief Kötelező override értékadás, amely titkosítja a paraméterét.
    /// @param rhs A titkosítandó karakter.
    /// @return EncryptedString&, referencia magára a titkosított stringre, a láncolhatósághoz.
    EncryptedString& operator=(const char rhs) override;

    /// @brief "Másoló" értékadás.
    /// @note Ellenben a többi operator=() függvénnyel, ez már titkosított adatot vesz át, így
    /// nincs szükség további titkosításra: átmásolja a titkosított adatot, és a titkosítót.
    /// @param rhs Az értékül adandó, másik titkosított string.
    /// @return EncryptedString&, referencia magára a titkosított stringre, a láncolhatósághoz.
    EncryptedString& operator=(const EncryptedString& rhs);

    /// @brief Operátor, amely titkosítja, majd hozzé fűzni az argumentumát a titkosított adathoz.
    /// @param str String, vagy implicit azzá al
    /// @return 
    EncryptedString& operator+=(const String& str);

    /// @brief Jelszó átállítására használt függvény.
    /// @note Jelszóval védett függvény, vagyis kivételt tud dobni.
    /// @param new_pw Új jelszó.
    /// @param old_pw Régi jelszó.
    void set_pw(const String& new_pw, const String& old_pw);

    /// @brief A nyers (titkosítatlan) stringet visszaadó függvény.
    /// @note Jelszóval védett függvény, vagyis kivételt tud dobni.
    /// @param pw A kötelező jelszó.
    /// @return A nyers adat.
    String decode(const String& pw) const;
    
    /// @brief A titkosítót átállító függvény, mely másolja a titkosítót.
    ///
    /// Mivel valaki beállíthatna egy olyan titkosítót, amely nem is titkosít 
    /// (minden karaktert saját magára képez le), ezzel kinyerhető lenne az adat,
    /// és ezért jelszóvédett a művelet.
    /// @note Jelszóval védett függvény, vagyis kivételt tud dobni.
    /// @param enc Az új titkosító, referenciaként átvéve, mely clone()-ozva lesz.
    /// @param pw A kötelező jelszó.
    void setEncryptor(const Encryptor& enc, const String& pw); 
    
    /// @brief A titkosítót átállító függvény, melynek egy előre lefoglalt titkosító pointerét lehet adni.
    ///
    /// Mivel valaki beállíthatna egy olyan titkosítót, amely nem is titkosít 
    /// (minden karaktert saját magára képez le), ezzel kinyerhető lenne az adat,
    /// és ezért jelszóvédett a művelet.
    /// @note Jelszóval védett függvény, vagyis kivételt tud dobni.
    /// @note A pointer new-al foglalt, vagy clone() által adott területre kell mutasson, mert később meg lesz kísérelve a törlése!
    /// @param pEnc Az új titkosító pointere (new-al, vagy clone()-val foglaltnak kell lennie).
    /// @param pw A kötelező jelszó.
    void setEncryptor(Encryptor* pEnc, const String& pw); //ptr param
};


#endif // ENCRYPTED_STRING_H