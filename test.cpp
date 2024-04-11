/// @file test.cpp
/// @author Gaál Botond
/// @brief Teszt program a Prog2 nagyházihoz 


#include "StringBase.h"
#include "String.h"
#include "Encryptor.h"
#include "EncryptorList.h"
#include "EncryptedString.h"
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <fstream>

#include "gtest_lite.h"
#include "memtrace.h"


using std::endl;
using std::cout;
int main(void) {
    srand(static_cast<unsigned>(time(nullptr)));
    //A String tesztcsoport ellenőrzi az std::string-gel megegyező működést.
    TEST(String, default_ctor) {
        String ures;
        EXPECT_EQ(static_cast<size_t>(0), ures.get_len()) << endl;
        EXPECT_STREQ("", ures.c_str()) << endl;
    } ENDM

    TEST(String, 1param_ctor) {
        const char c = 'S';
        String egybetu(c);
        EXPECT_EQ(static_cast<size_t>(1), egybetu.get_len()) << endl;
        EXPECT_STREQ("S", egybetu.c_str()) << endl;
    } ENDM

    TEST(String, char*_ctor) {
        const char str[] = "Hello";
        String string(str);
        EXPECT_EQ(strlen(str), string.get_len()) << endl;
        EXPECT_STREQ(str, string.c_str()) << endl;
    } ENDM

    TEST(String, copy_ctor) {
        String s1("String1");
        String s2(s1);
        EXPECT_EQ(s1.get_len(), s2.get_len()) << endl;
        EXPECT_STREQ(s1.c_str(), s2.c_str()) << endl;
    } ENDM

    TEST(String, StringBase_ctor) {
        String pw = "pw";
        EncryptedString s1(new ShiftEncryptor(1), pw, "12ABab"); //12ABab -> 23BCbc
        String s2(s1);
        EXPECT_EQ(s1.get_len(), s2.get_len()) << endl;
        EXPECT_STREQ(s1.c_str(), s2.c_str()) << endl;
        EXPECT_STREQ("23BCbc", s2.c_str()) << endl;
    } ENDM

    TEST(String, operator=) {
        String s1 = "String1";
        String s2 = "String2";
        s1 = s2;
        EXPECT_EQ(s1.get_len(), s2.get_len()) << endl;
        EXPECT_STREQ(s1.c_str(), s2.c_str()) << endl;
        String pw = "pw";
        EncryptedString s3(new ShiftEncryptor(1), pw, "12ABab"); //12ABab -> 23BCbc
        s2 = "Hello";
        s1 = s2 = s3;
        EXPECT_EQ(s1.get_len(), s3.get_len()) << endl;
        EXPECT_STREQ(s1.c_str(), s3.c_str()) << endl;
        EXPECT_STREQ(s2.c_str(), s3.c_str()) << endl;
        EXPECT_STREQ("23BCbc", s1.c_str()) << endl;
    } ENDM

    TEST(String, operator+=) {
        String s1;
        s1 += 'a';
        EXPECT_STREQ(s1.c_str(), "a") << endl;
        s1 += "bc";
        EXPECT_STREQ(s1.c_str(), "abc") << endl;
        s1 += String("de");
        EXPECT_STREQ(s1.c_str(), "abcde") << endl;
        s1 += EncryptedString(new ShiftEncryptor(1), "pw", "fg");  //fg -> gh
        EXPECT_STREQ(s1.c_str(), "abcdegh") << endl;
        EXPECT_EQ(strlen("abcdegh"), s1.get_len()) << endl;
    } ENDM

    TEST(String, operator+) {
        String s1 = 'a';
        s1 = s1 + 'b';
        EXPECT_STREQ(s1.c_str(), "ab") << endl;
        s1 = s1 + "c";
        EXPECT_STREQ(s1.c_str(), "abc") << endl;
        s1 = s1 + String("de");
        EXPECT_STREQ(s1.c_str(), "abcde") << endl;
        s1 = s1 + EncryptedString(new ShiftEncryptor(1), "pw", "fg");  //fg -> gh
        EXPECT_STREQ(s1.c_str(), "abcdegh") << endl;
        EXPECT_EQ(strlen("abcdegh"), s1.get_len()) << endl;
        String s2;
        s2 = '0' + s1;
        EXPECT_STREQ(s2.c_str(), "0abcdegh") << endl;
        s2 = "0" + s1;
        EXPECT_STREQ(s2.c_str(), "0abcdegh") << endl;
    } ENDM

    TEST(String, operator[]) {
        String s1 = "0123";
        const String s2 = s1;
        EXPECT_THROW(s1[10], std::out_of_range&) << "Nincs letiltva a tulindexeles!" << endl;
        EXPECT_NO_THROW(s2[0]) << "Nincs megcsinalva a const indexeles!" << endl;
        EXPECT_EQ('2', s1[2]) << endl;
        s1[0] = 'a';
        EXPECT_STREQ("a123", s1.c_str()) << "Nem mukodik helyesen az indexen keresztul modositas" << endl;
    } ENDM

    TEST(String, operator==/!=) {
        String a = "a";
        EXPECT_TRUE(a == 'a') << endl;
        EXPECT_TRUE(a == "a") << endl;
        EXPECT_TRUE(a == String("a")) << endl;
        EXPECT_TRUE('a' == a) << endl;
        EXPECT_TRUE("a" == a) << endl;
        EXPECT_TRUE(a != 'b') << endl;
        EXPECT_TRUE(a != "b") << endl;
        EXPECT_TRUE(a != String("b")) << endl;
        EXPECT_TRUE('b' != a) << endl;
        EXPECT_TRUE("b" != a) << endl;
    } ENDM

    TEST(String, operator<<) {
        String s("Hello");
        std::stringstream ss;
        ss << s;
        EXPECT_STREQ("Hello", ss.str().c_str());
    } ENDM

    TEST(String, iterator) {
        const String s1("Hello there! General Kenobi!");
        String s2;
        //const String-en const_iterator!
        for (String::const_iterator iter = s1.begin(); iter != s1.end(); iter++) { //posztinkremens
            s2 += *iter;
        }
        EXPECT_STREQ(s1.c_str(), s2.c_str()) << endl;
        s2 = "0123";
        for (String::iterator iter = s2.begin(); iter != s2.end(); ++iter) {
            int szamkent = static_cast<int>(*iter);
            *iter = static_cast<char>(szamkent + 1); 
        }
        EXPECT_STREQ("1234", s2.c_str()) << endl;
    } ENDM    

    TEST(ShiftEncryptor, encode) {
        ShiftEncryptor shift0;
        ShiftEncryptor shift10(10);
        ShiftEncryptor inv_shift10 = -shift10;
        Encryptor* p_inv_shift10 = shift10.cloneInverse();
        Encryptor* p_shift10 = shift10.clone();
        char first = '!';
        char last = '}';
        EXPECT_EQ(first, shift0.encode(first)) << endl;
        EXPECT_EQ('+', shift10.encode(first)) << endl;
        EXPECT_EQ('t', inv_shift10.encode(first)) << endl;
        EXPECT_EQ('t', p_inv_shift10->encode(first)) << endl;
        EXPECT_EQ('+', p_shift10->encode(first)) << endl;

        EXPECT_EQ('*', shift10.encode(last)) << endl;
        EXPECT_EQ('s', inv_shift10.encode(last)) << endl;
        EXPECT_EQ('s', p_inv_shift10->encode(last)) << endl;
        EXPECT_EQ('*', p_shift10->encode(last)) << endl;

        delete p_inv_shift10;
        delete p_shift10;
    } ENDM

    TEST(ShiftEncryptor, decode) {
        ShiftEncryptor enc(234566);
        Encryptor* inv_enc = enc.cloneInverse();
        char c = ']';
        String NiceString = "NiceString!";
        EXPECT_EQ(c, enc.decode(enc.encode(c))) << endl;
        EXPECT_EQ(c, enc.encode(enc.decode(c))) << endl;
        EXPECT_EQ(c, enc.encode((-enc).encode(c))) << endl;
        EXPECT_EQ(c, enc.decode((-enc).decode(c))) << endl;
        EXPECT_EQ(enc.encode(c), inv_enc->decode(c)) << endl;
        EXPECT_EQ(enc.decode(c), inv_enc->encode(c)) << endl;
        EXPECT_STREQ("NiceString!", enc.encode(enc.decode(NiceString)).c_str()) << endl;
        EXPECT_STREQ("NiceString!", inv_enc->encode(enc.encode(NiceString)).c_str()) << endl;
        delete inv_enc;
    } ENDM

    TEST(RandEncryptor, encode/decode) {
        RandEncryptor enc;
        String str = "Hello there! General Kenobi!";
        String str_encoded = enc.encode(str);
        String::iterator iter1, iter2;
        for (iter1 = str.begin(), iter2 = str_encoded.begin();iter1 != str.end(), iter2 != str_encoded.end(); ++iter1, ++iter2) {
            if (*iter1 <= '}' && *iter1 >= '!') {
                EXPECT_TRUE(*iter2 <= '}') << "Kileptunk a tartomanybol!" << endl;
                EXPECT_TRUE(*iter2 >= '!') << "Kileptunk a tartomanybol!" << endl;
            }            
        }
        EXPECT_STREQ(str.c_str(), enc.decode(str_encoded).c_str()) << "Nem adja vissza az eredetit a dekoldolas!" << endl;
        RandEncryptor inv_enc;
        inv_enc = -enc;
        EXPECT_TRUE(enc.encode(str) == inv_enc.decode(str)) << "Nem adja vissza az eredetit az inverz!" << endl;
        Encryptor* p_enc = enc.clone();
        Encryptor* p_inv_enc = enc.cloneInverse();
        EXPECT_STREQ(enc.encode(str).c_str(), p_enc->encode(str).c_str()) << "Klonozas nem volt sikeres!" << endl;
        EXPECT_STREQ(inv_enc.encode(str).c_str(), p_inv_enc->encode(str).c_str()) << "Inverz klonozas nem volt sikeres!" << endl;
        RandEncryptor other_enc;
        EXPECT_FALSE(enc.encode(str) == enc.decode(str)) << "Ugyanaz az encode mint a decode!" << endl;
        EXPECT_FALSE(enc.encode(str) == other_enc.encode(str)) << "Nem randomizal!" << endl;
        delete p_enc;
        delete p_inv_enc;
    } ENDM

    TEST(EncryptorList, encode/decode) {
        EncryptorList lst;
        EncryptorList sublist;
        ShiftEncryptor shift1(1000);
        ShiftEncryptor shift2(-5000);
        ShiftEncryptor shift3(5);
        RandEncryptor rand1;
        RandEncryptor rand2;
        RandEncryptor rand3;
        lst += shift1; //lst: shift1
        lst -= rand1.clone(); // lst: shift1 -> (-rand1) 
        lst.append(-shift2); // lst: shift1 -> (-rand1) -> (-shift2)
        lst = lst + rand2; // lst: shift1 -> (-rand1) -> (-shift2) -> rand2
        sublist += shift3; // sublist: shift3
        sublist -= rand3; // sublist: shift3 -> (-rand3), azaz -sublist = rand3 -> (-shift3)
        lst = lst - sublist; // lst: shift1->(-rand1)->(-shift2)->rand2->(-sublist)=shift1->(-rand1)->(-shift2)->rand2->rand3->(-shift3)
        String str = "Hello there! General Kenobi!";
        String encoded = lst.encode(str);
        String decoded = lst.decode(encoded);
        EXPECT_FALSE(str == lst.encode(str)) << endl;
        EXPECT_STREQ(str.c_str(), decoded.c_str()) << "Nem inverz a dekodolas!" << endl;
        String str2 = str;
        str2 = shift1.encode(str2);
        str2 = (-rand1).encode(str2);
        str2 = (-shift2).encode(str2);
        str2 = rand2.encode(str2);
        str2 = rand3.encode(str2); //sublist
        str2 = (-shift3).encode(str2); //sublist
        EXPECT_STREQ(str2.c_str(), encoded.c_str()) << "Nem bomlik szet a megfelelo elemekre a lista!" << endl;
        EncryptorList lst_copy(lst);
        EXPECT_STREQ(lst.encode(str).c_str(), lst_copy.encode(str).c_str()) << "Nem jo a masolo ctor!" << endl;
        EncryptorList lst_assign;
        lst_assign.append(ShiftEncryptor(-1000000)); //nézzük meg felülíródik-e
        lst_assign = lst_copy;
        EXPECT_STREQ(lst.decode(str).c_str(), lst_assign.decode(str).c_str()) << "Nem jo az ertekadas operator!" << endl;
        Encryptor* p_lst = lst.clone();
        Encryptor* p_inv_lst = lst.cloneInverse();
        EXPECT_STREQ(lst.encode(str).c_str(), p_lst->encode(str).c_str()) << "Nem jo a klonozas!" << endl;
        EXPECT_STREQ((-lst).encode(str).c_str(), p_inv_lst->encode(str).c_str()) << "Nem jo az inverz klonozas!" << endl;
        delete p_lst;
        delete p_inv_lst;
    } ENDM;

    ShiftEncryptor noEncr; 
    RandEncryptor randEnc1;
    RandEncryptor randEnc2;
    ShiftEncryptor shiftEncr(500000);
    EncryptorList sublst;
    sublst = randEnc1 - randEnc2;
    EncryptorList lst = shiftEncr + sublst;
    String pw("pw");

    TEST(EncryptedString, char_ctor) {
        char c = 'c';
        EncryptedString encStr(lst, pw, c);
        EXPECT_EQ(encStr.get_len(), static_cast<size_t>(1)) << endl;
        EXPECT_STREQ(encStr.c_str(), lst.encode(String(c)).c_str()) << endl;
    } ENDM

    TEST(EncryptedString, char*_ctor) {
        char str[] = "Hello!";
        EncryptedString encStr(lst, pw, str);
        EXPECT_EQ(encStr.get_len(), static_cast<size_t>(6)) << endl;
        EXPECT_STREQ(encStr.c_str(), lst.encode(String(str)).c_str()) << endl;
    } ENDM

    TEST(EncryptedString, string_ctor) {
        String str = "Hello!";
        EncryptedString encStr(lst, pw, str);
        EXPECT_EQ(encStr.get_len(), static_cast<size_t>(6)) << endl;
        EXPECT_STREQ(encStr.c_str(), lst.encode(String(str)).c_str()) << endl;
    } ENDM

    TEST(EncryptedString, copy_ctor) {
        EncryptedString encStr1(lst, pw, "Copy ctor!");
        EncryptedString encStr2 = encStr1;
        EXPECT_EQ(encStr1.get_len(), encStr2.get_len()) << endl;
        EXPECT_STREQ(encStr1.c_str(), encStr2.c_str()) << endl;
    } ENDM

    TEST(EncryptedString, operator!=/==) {
        char c = 'C';
        char c_str[] = "Hello!";
        String str = "!Olleh";
        EncryptedString encStr(lst, pw, c);
        EXPECT_TRUE(lst.encode(c) == encStr) << endl;
        EXPECT_TRUE(encStr == lst.encode(c)) << endl;
        EXPECT_FALSE(lst.encode(c) != encStr) << endl;
        EXPECT_FALSE(encStr != lst.encode(c)) << endl;

        encStr = EncryptedString(lst, pw, c_str);
        EXPECT_TRUE(lst.encode(c_str) == encStr) << endl;
        EXPECT_TRUE(encStr == lst.encode(c_str)) << endl;
        EXPECT_FALSE(lst.encode(c_str) != encStr)  << endl;
        EXPECT_FALSE(encStr != lst.encode(c_str)) << endl;

        encStr = EncryptedString(lst, pw, str);
        EXPECT_TRUE(lst.encode(str) == encStr) << endl;
        EXPECT_TRUE(encStr == lst.encode(str)) << endl;
        EXPECT_FALSE(lst.encode(str) != encStr) << endl;
        EXPECT_FALSE(encStr != lst.encode(str)) << endl;
    } ENDM

    TEST(EncryptedString, operator=) {
        EncryptedString encStr1(lst, pw, "Copy ctor!");
        EncryptedString encStr2 = EncryptedString(RandEncryptor(), "otherPw", "SomeString");
        encStr2 = encStr1;
        EXPECT_STREQ(encStr1.c_str(), encStr2.c_str()) << endl;
        char c = '?';
        encStr1 = c;
        EXPECT_STREQ(encStr1.c_str(), lst.encode(String(c)).c_str()) << endl;
        char c_str[] = "c string";
        encStr1 = c_str;
        EXPECT_STREQ(encStr1.c_str(), lst.encode(c_str).c_str()) << endl;
        String str = "MyString";
        encStr1 = str;
        EXPECT_STREQ(encStr1.c_str(), lst.encode(str).c_str()) << endl;
    } ENDM

    TEST(EncryptedString, decode) {
        String str = "Thank you EncryptedString! Very cool!";
        EncryptedString encStr = EncryptedString(lst, pw, str);
        EXPECT_FALSE(str == encStr) << "Nem tortent kodolas" << endl;
        EXPECT_TRUE(lst.encode(str) == encStr) << endl;
        EXPECT_THROW(encStr.decode("WrongPassword"), IncorrectPasswordException&) << "Nem dob hibat a rossz jelszo!" << endl;
        EXPECT_TRUE(str == encStr.decode(pw)) << "Nem sikerult visszacsinalni a titkositast!" << endl;
    } ENDM

    TEST(EncryptedString, iterator) {
        String str = "Thank you EncryptedString! Very cool!";
        String encoded = lst.encode(str);
        EncryptedString encStr(lst, pw, str);
        String::iterator iter1;
        EncryptedString::iterator iter2;
        for (iter1 = encoded.begin(), iter2 = encStr.begin(); iter1 != encoded.end(), iter2 != encStr.end(); ++iter1, ++iter2) {
            EXPECT_EQ(*iter1, *iter2) << "Nem jo elemekre mutatnak az iteratorok!" << endl;
        }
    } ENDM

    TEST(EncryptedString, operator+=) {
        String str = "Some men just want to watch the world burn. ";
        char c_str[] = "Jporta";
        char c = 'L';
        EncryptedString encStr = EncryptedString(lst, pw);
        EXPECT_TRUE("" == encStr) << "Nem ures string!" << endl;
        encStr += str;
        EXPECT_TRUE(lst.encode(str) == encStr) << endl;
        encStr += c_str;
        str += c_str;
        EXPECT_TRUE(lst.encode(str) == encStr) << endl;
        encStr += c;
        str += c;
        EXPECT_TRUE(lst.encode(str) == encStr) << endl;
    } ENDM

    TEST(EncryptedString, change_encryptor/change_pw) {
        RandEncryptor encr;
        String str = "12345678";
        EncryptedString encStr = EncryptedString(lst, pw, str);
        EXPECT_TRUE(encStr == lst.encode(str)) << "Nem jol jott letre az encrypted string!" << endl;
        encStr.setEncryptor(encr, pw);
        EXPECT_TRUE(encStr == encr.encode(str)) << "Nem allitodott at a kodolo" << endl;
        EXPECT_TRUE(encStr.decode(pw) == str) << "Nem adja vissza az eredeti stringet az uj titkosito!" << endl;
        String new_pw = "NewPassword";
        EXPECT_THROW(encStr.set_pw(new_pw, "WrongPassword"), IncorrectPasswordException&) << "El lett fogadva a rossz jelszo!" << endl;
        EXPECT_NO_THROW(encStr.set_pw(new_pw, pw)) << "Nem engedte atallitani a jelszot!" << endl;
        EXPECT_THROW(encStr.decode(pw), IncorrectPasswordException&) << "El lett fogadva a rossz jelszo!" << endl;
        EXPECT_NO_THROW(encStr.decode(new_pw)) << "Nem fogadta el a helyes jelszavat!" << endl;
    } ENDM

    TEST(EncryptedString, file) {
        std::ifstream inputFile("lorem_ipsum.dat");
        if (!inputFile.is_open()) {
            std::cerr << "Failed to open file." << endl;
            return 1;
        }
        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        inputFile.close();
        String raw(buffer.str().c_str());
        EncryptedString encrypted(lst, pw);
        encrypted = raw;
        String decoded;
        EXPECT_THROW(decoded = encrypted.decode("WrongPassword"),IncorrectPasswordException&) << "El lett fogadva a rossz jelszo!" << endl;
        EXPECT_NO_THROW(decoded = encrypted.decode(pw)) << "Nem lett elfogadva a jelszo pedig helyes volt!" << endl;
        EXPECT_FALSE(raw == encrypted);
        EXPECT_TRUE(decoded == raw);
    } ENDM
    return 0;
}