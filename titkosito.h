#ifndef TITKOSITO_H
#define TITKOSITO_H

#include "string.h"

#include "memtrace.h"

class Titkosito {
protected:
    char tartomanyba(int c) const;
    bool valid(int c) const; 
public:
    virtual char encode(char c) const = 0;
    String encode(const String& str) const;
    virtual char decode(char c) const = 0;
    String decode(const String& str) const;
    virtual Titkosito* clone() const = 0;
    virtual ~Titkosito();
};

class XorTitkosito: public Titkosito {
private:
    char key;
public:
    XorTitkosito(char key='\0');
    void set_key(char key);
    char get_key() const;
    char encode(char c) const;
    char decode(char c) const;
    XorTitkosito operator-() const;
    Titkosito* clone() const;
};

class ShiftTitkosito: public Titkosito {
private:
    int shift;
public:
    ShiftTitkosito(int shift = 0);
    void set_shift(int shift);
    char get_shift() const;
    char encode(char c) const;
    char decode(char c) const;
    ShiftTitkosito operator-() const;
    Titkosito* clone() const;
};



#endif  // TITKOSITO_H