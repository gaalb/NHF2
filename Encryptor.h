/// @file Encryptor.h
/// @author Gaál Botond
/// @brief Az Encryptor ősosztályt, és egyszerűbb Encryptorokat megvalósító fájl.
#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include "StringBase.h"
#include "String.h"

#include "memtrace.h"

/// @brief Absztrakt titkosító ősosztály.
/// 
/// Magában foglalja a konkrét Encryptor implementációk felé támasztott követelményeket.
class Encryptor {
protected:
    static char first_char; ///< Statikus változó, mely megmutatja az értelmezési tartomány elejét.
    static char last_char; ///< Statikus változó, mely megmutatja az értelmezési tartomány végét.
    static size_t range;  ///< Statikus változó, mely megmutatja az értelmezési tartomány méretét.
    char shiftIntoScope(int c) const; ///< Segédfüggvény, amely az értelmezési tartományba tol egy inputot.
    bool isValidInput(int c) const; ///< Segédfüggvény, amely megmondja, az értelmezési tartományba esik-e egy input.
public:

    /// @brief Tisztán virtuális függvény, amely pontosan egy karaktert titkosít.
    /// @note Megkötés, hogy a titkosított karakter is az értelmezési tartományba essen.
    virtual char encode(char c) const = 0;

    /// @brief Egy Stringet titkosít, úgy, hogy végig hívja rajta az egy karaktert kódoló verzióját.
    String encode(const String& str) const;

    /// @brief Tisztán virtuális függvény, mely pontosan egy karaktert dekódol.
    /// @note Evidens, hogy a dekódolt karakternek az értelmezési tartományba kell esnie, de
    /// az is megkötés, hogy invertálhatónak kell lennie a titkosításnak, azaz léteznie kell decode-nak.
    virtual char decode(char c) const = 0;

    /// @brief Egy Stringet dekódol, úgy, hogy végig hívja rajta az egy karaktert dekódoló verzióját.
    String decode(const String& str) const;

    /// @brief Dinamikus memóriaterületen foglal egy Encryptor-nak helyet, és visszaadja a pointerét.
    virtual Encryptor* clone() const = 0;

    /// @brief Dinamikus memóriaterületen foglal egy Encryptor inverzének helyet, és visszaadja a pointerét.
    /// @note Megkötés, volt, hogy invertálhatónak kell lennie a titkosításnak.
    /// @see char decode(char c) const
    virtual Encryptor* cloneInverse() const = 0;
    virtual ~Encryptor();
};

/// @brief A lehető legegyszerűbb Encryptor implementáció: eltolja valahánnyal a karaktert
/// @note Ha az eltolt karakter kiesik az értelmezési tartományból, bele kell vinni.
class ShiftEncryptor: public Encryptor {
private:
    int shift; ///< Az eltolás mértéke, lehet negatív is.
public:
    using Encryptor::encode;  //avoid name hiding
    using Encryptor::decode;  //avoid name hiding
    
    /// @brief Egyparaméteres és paraméter nélküli konstruktor
    /// @note Nincs szükség másoló konstruktorra/értékadásra/destruktorra, mert nincs dinamikus adattag
    /// @note A default shift értéke 0, ami egy olyan titkosítás, ami nem is titkosít. 
    ShiftEncryptor(int shift = 0);  
    void set_shift(int shift); ///< Publikus setter a privát tagnak.
    char get_shift() const; ///< Publikus getter a privát tagnak.
    
    /// @brief Kötelező override kódoló függvény: eltolja shift-tel a karaktert.
    char encode(char c) const override;

    /// @brief Kötelező override dekódoló függvény: eltolja (-shift)-tel a karaktert. 
    char decode(char c) const override;

    /// @brief Új ShiftEncryptor-t létrehozó operátor
    /// @return ShiftEncryptor, amely pontosan az inverze az eredetinek.
    ShiftEncryptor operator-() const;

    /// @brief Kötelező override klónozó függvény: dinamikusan foglal egy másolatot.
    /// @return A dinamikusan foglalt területre mutató pointer.
    Encryptor* clone() const override;

    /// @brief Kötelező override inverz klónozó függvény: dinamikusan foglal egy inverzet.
    /// @return A dinamikusan foglalt területre mutató pointer.
    Encryptor* cloneInverse() const override;
};

/// @brief Encryptor implementáció, amely egy véletlenszerű hozzárendelést ír elő
/// az összes lehetséges bemenet és kimenet között.
class RandEncryptor: public Encryptor {
private:
    char* encodeMap; ///< Mapelés, amely a kódolást határozza meg.
    char* decodeMap; ///< Mapelés, amely a dekódolást határozza meg: inverze encodeMap-nek.
    void invert(); ///< Segédfüggvény, megcseréli a két Map-et, így invertál.
    static void shuffle(char* array, size_t size); ///< Segédfüggvény, összekeveri egy tömb elemeit.
public:
    using Encryptor::encode;  //avoid name hiding
    using Encryptor::decode;  //avoid name hiding

    /// @brief Konstruktor, amelyből ez esetben csak paraméter nélküli létezik
    /// 
    /// Helyet foglalt encodeMap és decodeMap-nak, amelyek ebben az implementációban
    /// minden esetben pontosan akkorák, amekkora az értelmezési tartomány. 
    /// A helyfoglaláson kívül össze is keveri őket, így létrehozva egy Map-elést. 
    RandEncryptor();

    /// @brief Másoló konstruktor, a dinamikusan foglalt terület helyes kezelése végett. 
    RandEncryptor(const RandEncryptor& other);    

    /// @brief Destruktor, a dinamikusn foglalt terület felszabadítása végett.
    ~RandEncryptor();

    /// @brief Értékadó operátor, a dinamikusan foglalt terület helyes kezelése végett. 
    /// @return RandEcryptor&, referencia a láncolhatóság végett.
    RandEncryptor& operator=(const RandEncryptor& rhs);

    /// @brief Kötelező override kódoló függvény: encodeMap-ból a (c-elso)-edik karaktert adja vissza
    char encode(char c) const;

    /// @brief Kötelező override dekódoló függvény: decodeMap-ból a (c-elso)-edik karaktert adja vissza
    char decode(char c) const;

    /// @brief Kötelező override klónozó függvény: dinamikusan foglal egy másolatot.
    /// @return A dinamikusan foglalt területre mutató pointer.
    Encryptor* clone() const;

    /// @brief Kötelező override inverz klónozó függvény: dinamikusan foglal egy inverzet.
    /// @return A dinamikusan foglalt területre mutató pointer.
    Encryptor* cloneInverse() const;    

    /// @brief Új RandEncryptor-t létrehozó operátor
    /// @return RandEncryptor, amely pontosan az inverze az eredetinek.
    RandEncryptor operator-() const;
};
#endif  // ENCRYPTOR_H