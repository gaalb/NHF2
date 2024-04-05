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
    TEST(String, default_ctor) {
        String ures;
        EXPECT_EQ(static_cast<size_t>(0), ures.get_len()) << "A default ctor nem 0 hosszt ad!" << endl;
        EXPECT_STREQ("", ures.c_str()) << "A default ctor nem ures stringet ad!" << endl;
    } ENDM
    TEST(String, 1param_ctor) {
        const char c = 'S';
        String egybetu(c);
        EXPECT_EQ(static_cast<size_t>(1), egybetu.get_len()) << "A char ctor nem 1 hosszt ad!" << endl;
        EXPECT_STREQ("S", egybetu.c_str()) << "A char ctor nem megfelelo!" << endl;
    } ENDM
    TEST(String, char*_ctor) {
        const char str[] = "Hello";
        String string(str);
        EXPECT_EQ(strlen(str), string.get_len()) << "A char* ctor nem megfelelo!" << endl;
        EXPECT_STREQ(str, string.c_str())  << "A char* ctor nem megfelelo!" << endl;
    } ENDM
    TEST(String, copy_ctor) {
        String s1("String1");
        String s2(s1);
        EXPECT_EQ(s1.get_len(), s2.get_len()) << "A masolo ctor nem megfelelo!" << endl;
        EXPECT_STREQ(s1.c_str(), s2.c_str()) << "A masolo ctor nem megfelelo!" << endl;
    } ENDM
    TEST(String, StringBase_ctor) {
        String pw = "pw";
        EncryptedString s1(new ShiftEncryptor(1), pw, "12ABab"); //12ABab -> 23BCbc
        String s2(s1);
        EXPECT_EQ(s1.get_len(), s2.get_len()) << "A StringBase ctor nem megfelelo!" << endl;
        EXPECT_STREQ(s1.c_str(), s2.c_str()) << "A StringBase ctor nem megfelelo!" << endl;
        EXPECT_STREQ("23BCbc", s2.c_str()) << "A StringBase ctor nem megfelelo!" << endl;
    } ENDM
    TEST(String, operator=) {
        String s1 = "String1";
        String s2 = "String2";
        s1 = s2;
        EXPECT_EQ(s1.get_len(), s2.get_len()) << "Az = operator nem megfelelo!" << endl;
        EXPECT_STREQ(s1.c_str(), s2.c_str()) << "Az = operator nem megfelelo!" << endl;
        String pw = "pw";
        EncryptedString s3(new ShiftEncryptor(1), pw, "12ABab"); //12ABab -> 23BCbc
        s2 = "Hello";
        /*
        Tesztelés közben a fenti sornál a következő hibát kapom (nem minden fordítónál, de néhánynál igen):
        error: ambiguous overload for 'operator=' (operand types are 'String' and 'const char [6]')
        Vagyis ezt a sort többféle képpen is tudja értelmezni. Személyesen én nem értem, mit lehet ezen
        többféleképpen értelmezni. A stringeknek 2db operator=-je van, az egyik StringBase-é konvertálna,
        a másik String-é. Mivel StringBase-é nem lehet konvertálni (olyat nem lehet példányosítani), ezért
        csak 1db valid opció van. Elmélet: A fordító nem elég okos hogy ezt kitalálja, és nem tudja eldönteni
        melyiket használja? Pedig elvileg minden információ rendelkezésére áll.
        */
        s1 = s2 = s3;
        EXPECT_EQ(s1.get_len(), s3.get_len()) << "Az = operator nem megfelelo!" << endl;
        EXPECT_STREQ(s1.c_str(), s3.c_str()) << "Az = operator nem megfelelo!" << endl;
        EXPECT_STREQ(s2.c_str(), s3.c_str()) << "Az = operator nem megfelelo!" << endl;
        EXPECT_STREQ("23BCbc", s1.c_str()) << "Az = operator nem megfelelo!" << endl;
    } ENDM
    TEST(String, operator+=) {
        String s1;
        s1 += 'a';
        EXPECT_STREQ(s1.c_str(), "a") << "A +=(char) operator nem megfelelo!" << endl;
        s1 += "bc";
        EXPECT_STREQ(s1.c_str(), "abc") << "A +=(char*) operator nem megfelelo!" << endl;
        s1 += String("de");
        EXPECT_STREQ(s1.c_str(), "abcde") << "A +=(String) operator nem megfelelo!" << endl;
        s1 += EncryptedString(new ShiftEncryptor(1), "pw", "fg");  //fg -> gh
        EXPECT_STREQ(s1.c_str(), "abcdegh") << "A +=(StringBase) operator nem megfelelo!" << endl;
        EXPECT_EQ(strlen("abcdegh"), s1.get_len()) << "A += kontakenalas sikeres de a hossz nem jo!" << endl;
    } ENDM
    TEST(String, operator+) {
        String s1 = 'a';
        s1 = s1 + 'b';
        EXPECT_STREQ(s1.c_str(), "ab") << "A +(char) operator nem megfelelo!" << endl;
        s1 = s1 + "c";
        EXPECT_STREQ(s1.c_str(), "abc") << "A +(char*) operator nem megfelelo!" << endl;
        s1 = s1 + String("de");
        EXPECT_STREQ(s1.c_str(), "abcde") << "A +(String) operator nem megfelelo!" << endl;
        s1 = s1 + EncryptedString(new ShiftEncryptor(1), "pw", "fg");  //fg -> gh
        EXPECT_STREQ(s1.c_str(), "abcdegh") << "A +(StringBase) operator nem megfelelo!" << endl;
        EXPECT_EQ(strlen("abcdegh"), s1.get_len()) << "A + kontakenalas sikeres de a hossz nem jo!" << endl;
        String s2;
        s2 = '0' + s1;
        EXPECT_STREQ(s2.c_str(), "0abcdegh") << "A globalis char+String operator nem megfelelo!" << endl;
        s2 = "0" + s1;
        EXPECT_STREQ(s2.c_str(), "0abcdegh") << "A globalis char*+String operator nem megfelelo!" << endl;
    } ENDM
    TEST(String, operator[]) {
        String s1 = "0123";
        const String s2 = s1;
        EXPECT_THROW(s1[10], std::out_of_range&) << "Nincs letiltva a tulindexeles!" << endl;
        EXPECT_NO_THROW(s2[0]) << "Nincs megcsinalva a const indexeles!" << endl;
        EXPECT_EQ('2', s1[2]) << "Nem jo karaktert ad vissza az indexeles" << endl;
        s1[0] = 'a';
        EXPECT_STREQ("a123", s1.c_str()) << "Nem mukodik helyesen az indexen keresztul modositas" << endl;
    } ENDM
    TEST(String, operator==/!=) {
        String a = "a";
        EXPECT_TRUE(a == 'a') << "== nem megfelelo!" << endl;
        EXPECT_TRUE(a == "a") << "== nem megfelelo!" << endl;
        EXPECT_TRUE(a == String("a")) << "== nem megfelelo!" << endl;
        EXPECT_TRUE('a' == a) << "== nem megfelelo!" << endl;
        EXPECT_TRUE("a" == a) << "== nem megfelelo!" << endl;
        EXPECT_TRUE(a != 'b') << "!= nem megfelelo!" << endl;
        EXPECT_TRUE(a != "b") << "!= nem megfelelo!" << endl;
        EXPECT_TRUE(a != String("b")) << "!= nem megfelelo!" << endl;
        EXPECT_TRUE('b' != a) << "!= nem megfelelo!" << endl;
        EXPECT_TRUE("b" != a) << "!= nem megfelelo!" << endl;
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
        EXPECT_STREQ(s1.c_str(), s2.c_str()) << "const_iterator helytelen!" << endl;
        s2 = "0123";
        for (String::iterator iter = s2.begin(); iter != s2.end(); ++iter) {
            int szamkent = static_cast<int>(*iter);
            *iter = static_cast<char>(szamkent + 1); 
        }
        EXPECT_STREQ("1234", s2.c_str()) << "iterator helytelen!" << endl;
    } ENDM    

    TEST(ShiftEncryptor, encode) {
        ShiftEncryptor shift0;
        ShiftEncryptor shift10(10);
        ShiftEncryptor inv_shift10 = -shift10;
        Encryptor* p_inv_shift10 = shift10.cloneInverse();
        Encryptor* p_shift10 = shift10.clone();
        char first = '(';
        char last = '}';
        EXPECT_EQ(first, shift0.encode(first)) << "kodolasi hiba" << endl;
        EXPECT_EQ('2', shift10.encode(first)) << "kodolasi hiba" << endl;
        EXPECT_EQ('t', inv_shift10.encode(first)) << "kodolasi hiba" << endl;
        EXPECT_EQ('t', p_inv_shift10->encode(first)) << "kodolasi hiba" << endl;
        EXPECT_EQ('2', p_shift10->encode(first)) << "kodolasi hiba" << endl;

        EXPECT_EQ('1', shift10.encode(last)) << "kodolasi hiba" << endl;
        EXPECT_EQ('s', inv_shift10.encode(last)) << "kodolasi hiba" << endl;
        EXPECT_EQ('s', p_inv_shift10->encode(last)) << "kodolasi hiba" << endl;
        EXPECT_EQ('1', p_shift10->encode(last)) << "kodolasi hiba" << endl;

        delete p_inv_shift10;
        delete p_shift10;
    } ENDM

    TEST(ShiftEncryptor, decode) {
        ShiftEncryptor enc(234566);
        Encryptor* inv_enc = enc.cloneInverse();
        char c = ']';
        String NiceString = "NiceString!";
        EXPECT_EQ(c, enc.decode(enc.encode(c))) << "dekodolasi hiba" << endl;
        EXPECT_EQ(c, enc.encode(enc.decode(c))) << "dekodolasi hiba" << endl;
        EXPECT_EQ(c, enc.encode((-enc).encode(c))) << "dekodolasi hiba" << endl;
        EXPECT_EQ(c, enc.decode((-enc).decode(c))) << "dekodolasi hiba" << endl;
        EXPECT_EQ(enc.encode(c), inv_enc->decode(c)) << "dekodolasi hiba" << endl;
        EXPECT_EQ(enc.decode(c), inv_enc->encode(c)) << "dekodolasi hiba" << endl;
        EXPECT_STREQ("NiceString!", enc.encode(enc.decode(NiceString)).c_str()) << "dekodolasi hiba" << endl;
        EXPECT_STREQ("NiceString!", inv_enc->encode(enc.encode(NiceString)).c_str()) << "dekodolasi hiba" << endl;
        delete inv_enc;
    } ENDM

    TEST(RandEncryptor, encode/decode) {
        RandEncryptor enc;
        String str = "Hello there! General Kenobi!";
        String str_encoded = enc.encode(str);
        String::iterator iter1, iter2;
        for (iter1 = str.begin(), iter2 = str_encoded.begin();iter1 != str.end(), iter2 != str_encoded.end(); ++iter1, ++iter2) {
            if (*iter1 <= '}' && *iter1 >= '(') {
                EXPECT_TRUE(*iter2 <= '}') << "kileptunk a tartomanybol!" << endl;
                EXPECT_TRUE(*iter2 >= '(') << "kileptunk a tartomanybol!" << endl;
            }            
        }
        EXPECT_STREQ(str.c_str(), enc.decode(str_encoded).c_str()) << "nem adja vissza az eredetit a dekoldolas" << endl;
        RandEncryptor inv_enc;
        inv_enc = -enc;
        EXPECT_TRUE(enc.encode(str) == inv_enc.decode(str)) << "nem adja vissza az eredetit az inverz" << endl;
        Encryptor* p_enc = enc.clone();
        Encryptor* p_inv_enc = enc.cloneInverse();
        EXPECT_STREQ(enc.encode(str).c_str(), p_enc->encode(str).c_str()) << "klonozas nem jo" << endl;
        EXPECT_STREQ(inv_enc.encode(str).c_str(), p_inv_enc->encode(str).c_str()) << "inverz klonozas nem jo" << endl;
        RandEncryptor other_enc;
        EXPECT_FALSE(enc.encode(str) == enc.decode(str)) << "ugyanaz az encode mint a decode?" << endl;
        EXPECT_FALSE(enc.encode(str) == other_enc.encode(str)) << "nem randomizal!" << endl;
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
        EXPECT_FALSE(str == lst.encode(str)) << "Nem tortent kodolas!" << endl;
        EXPECT_STREQ(str.c_str(), decoded.c_str()) << "Nem az inverze a dekodolas!" << endl;
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
        EXPECT_EQ(encStr.get_len(), static_cast<size_t>(1)) << "Hossz nem jo!" << endl;
        EXPECT_STREQ(encStr.c_str(), lst.encode(String(c)).c_str()) << "Nem helyesen kodol!" << endl;
    } ENDM

    TEST(EncryptedString, char*_ctor) {
        char str[] = "Hello!";
        EncryptedString encStr(lst, pw, str);
        EXPECT_EQ(encStr.get_len(), static_cast<size_t>(6)) << "Hossz nem jo!" << endl;
        EXPECT_STREQ(encStr.c_str(), lst.encode(String(str)).c_str()) << "Nem helyesen kodol!" << endl;
    } ENDM

    TEST(EncryptedString, string_ctor) {
        String str = "Hello!";
        EncryptedString encStr(lst, pw, str);
        EXPECT_EQ(encStr.get_len(), static_cast<size_t>(6)) << "Hossz nem jo!" << endl;
        EXPECT_STREQ(encStr.c_str(), lst.encode(String(str)).c_str()) << "Nem helyesen kodol!" << endl;
    } ENDM

    TEST(EncryptedString, copy_ctor) {
        EncryptedString encStr1(lst, pw, "Copy ctor!");
        EncryptedString encStr2 = encStr1;
        EXPECT_EQ(encStr1.get_len(), encStr2.get_len()) << "Hossz nem jo!" << endl;
        EXPECT_STREQ(encStr1.c_str(), encStr2.c_str()) << "Nem helyesen kodol!" << endl;
    } ENDM

    TEST(EncryptedString, operator!=/==) {
        char c = 'C';
        char c_str[] = "Hello!";
        String str = "!Olleh";
        EncryptedString encStr(lst, pw, c);
        EXPECT_TRUE(lst.encode(c) == encStr) << "char==EncrypterString hibás" << endl;
        EXPECT_TRUE(encStr == lst.encode(c)) << "EncrypterString==char hibás" << endl;
        EXPECT_FALSE(lst.encode(c) != encStr) << "char!=EncrypterString hibás" << endl;
        EXPECT_FALSE(encStr != lst.encode(c)) << "EncrypterString==char hibás" << endl;

        encStr = EncryptedString(lst, pw, c_str);
        EXPECT_TRUE(lst.encode(c_str) == encStr) << "char*==EncrypterString hibás" << endl;
        EXPECT_TRUE(encStr == lst.encode(c_str)) << "EncrypterString==char* hibás" << endl;
        EXPECT_FALSE(lst.encode(c_str) != encStr) << "char*!=EncrypterString hibás" << endl;
        EXPECT_FALSE(encStr != lst.encode(c_str)) << "EncrypterString==char* hibás" << endl;

        encStr = EncryptedString(lst, pw, str);
        EXPECT_TRUE(lst.encode(str) == encStr) << "String==EncrypterString hibás" << endl;
        EXPECT_TRUE(encStr == lst.encode(str)) << "EncrypterString==String hibás" << endl;
        EXPECT_FALSE(lst.encode(str) != encStr) << "String!=EncrypterString hibás" << endl;
        EXPECT_FALSE(encStr != lst.encode(str)) << "EncrypterString==String hibás" << endl;
    } ENDM

    TEST(EncryptedString, operator=) {
        EncryptedString encStr1(lst, pw, "Copy ctor!");
        EncryptedString encStr2 = EncryptedString(RandEncryptor(), "otherPw", "SomeString");
        encStr2 = encStr1;
        EXPECT_STREQ(encStr1.c_str(), encStr2.c_str()) << "Nem helyesen kodol!" << endl;
        char c = '?';
        encStr1 = c;
        EXPECT_STREQ(encStr1.c_str(), lst.encode(String(c)).c_str()) << "Nem helyesen kodol!" << endl;
        char c_str[] = "c string";
        encStr1 = c_str;
        EXPECT_STREQ(encStr1.c_str(), lst.encode(c_str).c_str()) << "Nem helyesen kodol!" << endl;
        String str = "MyString";
        encStr1 = str;
        EXPECT_STREQ(encStr1.c_str(), lst.encode(str).c_str()) << "Nem helyesen kodol!" << endl;
    } ENDM

    TEST(EncryptedString, decode) {
        String str = "Thank you EncryptedString! Very cool!";
        EncryptedString encStr = EncryptedString(lst, pw, str);
        EXPECT_FALSE(str == encStr) << "Nem tortent kodolas" << endl;
        EXPECT_TRUE(lst.encode(str) == encStr) << "Nem helyesen kodol!" << endl;
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
        EXPECT_TRUE(lst.encode(str) == encStr) << "Nem helyes a konkatenalas" << endl;
        encStr += c_str;
        str += c_str;
        EXPECT_TRUE(lst.encode(str) == encStr) << "Nem helyes a konkatenalas" << endl;
        encStr += c;
        str += c;
        EXPECT_TRUE(lst.encode(str) == encStr) << "Nem helyes a konkatenalas" << endl;
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

    TEST(EncryptedString, file/operator>>) {
        std::ifstream inputFile;
        std::ofstream outputFile;
        inputFile = std::ifstream("lorem_ipsum.txt");
        outputFile = std::ofstream("String.txt");
        if (!inputFile.is_open() || !outputFile.is_open()) {
            std::cerr << "Failed to one or both files." << endl;
            return 1;
        }
        String word;
        while (inputFile >> word) {
            outputFile << word << " ";
        }
        inputFile.close();
        outputFile.close();

        inputFile = std::ifstream("lorem_ipsum.txt");
        outputFile = std::ofstream("Encrypted.txt");
        if (!inputFile.is_open() || !outputFile.is_open()) {
            std::cerr << "Failed to one or both files." << endl;
            return 1;
        }
        EncryptedString encWord(lst, pw);
        while (inputFile >> encWord) {
            outputFile << encWord << " ";
        }
        inputFile.close();
        outputFile.close();
        
        std::ifstream txtString("String.txt");
        std::ifstream txtEncrypted("Encrypted.txt");
        if (!txtString.is_open() || !txtEncrypted.is_open()) {
            std::cerr << "Failed to one or both files." << endl;
            return 1;
        }
        char c, c_encrypted;
        while (txtString >> c && txtEncrypted >> c_encrypted) {
            EXPECT_EQ(c, lst.decode(c_encrypted)) << "Egy karakter nem jol lett dekodolva!" << endl;
        }
        txtString.close();
        txtEncrypted.close();
    } ENDM
    return 0;
}