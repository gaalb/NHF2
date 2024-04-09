/**
 * @file EncryptedString.h
 * @author Gaál Botond
 * @brief 
 */
#ifndef ENCRYPTED_STRING_H
#define ENCRYPTED_STRING_H

#include "String.h"
#include "Encryptor.h"
#include <stdexcept>

#include "memtrace.h"

class IncorrectPasswordException: public std::exception {
private:
    String msg;
public:
    IncorrectPasswordException(const String& msg);
    virtual const char* what() const noexcept override;
};

class EncryptedString : public StringBase {
private:    
    Encryptor* pEncryptor;
    String password;
    void checkPw(const String& pw) const;
    void set_str(const StringBase& new_str);
public:
    ~EncryptedString();
    EncryptedString(const Encryptor& encr, const String& pw, const char* str = ""); //
    EncryptedString(Encryptor* pEncr, const String& pw, const char* str = ""); //
    EncryptedString(const Encryptor& encr, const String& pw, const char c); //
    EncryptedString(Encryptor* pEncr, const String& pw, const char c); //
    EncryptedString(const Encryptor& encr, const String& pw, const String& str); //
    EncryptedString(Encryptor* pEncr, const String& pw, const String& str); //
    EncryptedString(const EncryptedString& other); //

    EncryptedString& operator=(const StringBase& rhs) override;
    EncryptedString& operator=(const char* rhs) override;
    EncryptedString& operator=(const char rhs) override;
    EncryptedString& operator=(const EncryptedString& rhs);

    EncryptedString& operator+=(const String& str);

    void set_pw(const String& new_pw, const String& old_pw);
    String decode(const String& pw) const;
    
    void setEncryptor(const Encryptor& enc, const String& pw); //reference param
    void setEncryptor(Encryptor* pEnc, const String& pw); //ptr param
};


#endif // ENCRYPTED_STRING_H