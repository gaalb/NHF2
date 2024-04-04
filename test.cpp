#include "StringBase.h"
#include "String.h"
#include "Encryptor.h"
#include "EncryptorList.h"
#include "EncryptedString.h"
#include <sstream>

#include "gtest_lite.h"
#include "memtrace.h"


using std::endl;
using std::cout;
int main(void) {
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
        for (String::const_iterator iter = s1.begin(); iter != s1.end(); iter++) {
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
        EXPECT_EQ(c, enc.decode(enc.encode(c))) << "dekodolasi hiba" << endl;
        EXPECT_EQ(c, enc.encode(enc.decode(c))) << "dekodolasi hiba" << endl;
        EXPECT_EQ(c, enc.encode((-enc).encode(c))) << "dekodolasi hiba" << endl;
        EXPECT_EQ(c, enc.decode((-enc).decode(c))) << "dekodolasi hiba" << endl;
        EXPECT_EQ(enc.encode(c), inv_enc->decode(c)) << "dekodolasi hiba" << endl;
        EXPECT_EQ(enc.decode(c), inv_enc->encode(c)) << "dekodolasi hiba" << endl;
        delete inv_enc;
    } ENDM
   
    String iter_str("Hello there, General Kenobi!");
    String::iterator iter;
    for (iter = iter_str.begin(); iter != iter_str.end(); ++iter) {
        cout << *iter;
    }
    cout << endl << endl;
    ShiftEncryptor t1(6);
    String s1("01 89 abc");
    for (auto elem : s1) {
        char kodolva = t1.encode(elem);
        cout << "betu: " << elem << ", titkositva: " << kodolva << ", dekodolva: " << t1.decode(kodolva) << endl;
    }
    EncryptedString t_str(t1, "pw", s1);
    cout << "titkosítva: " << t_str << endl;
    t_str += "xyz ABC XYZ";
    cout << "titkosítva: " << t_str << endl;
    cout <<"eredeti: " << t_str.decode("pw") << endl;
    ShiftEncryptor t2(5);
    ShiftEncryptor t3(10);
    EncryptorList lst1;
    lst1.append(t1);
    lst1.append(t2);
    lst1.append(t3);
    cout << "s1: " << s1 << ", encoded: " << lst1.encode(s1) << ", decoded: " << lst1.decode(lst1.encode(s1)) << endl; 

    EncryptorList lst2(lst1);
    cout << "s1: " << s1 << ", encoded: " << lst2.encode(s1) << ", decoded: " << lst2.decode(lst2.encode(s1)) << endl; 

    EncryptorList lst3;
    lst3 = lst1;
    cout << "s1: " << s1 << ", encoded: " << lst3.encode(s1) << ", decoded: " << lst3.decode(lst3.encode(s1)) << endl; 

    EncryptorList lst4;
    lst4 = -lst1;
    cout << "s1: " << s1 << ", encoded: " << lst1.encode(s1) << ", inversed: " << lst4.encode(lst1.encode(s1)) << endl;

    Encryptor* p_st = lst1.cloneInverse();
    cout << "s1: " << s1 << ", encoded: " << lst1.encode(s1) << ", inversed: " << p_st->encode(lst1.encode(s1)) << endl;
    delete p_st;

    //please PLEASE
    EncryptorList lst5;  //t1 -> t2 -> lst4
    lst5.append(t1);
    lst5.append(t2);
    lst5.append(lst4);
    lst5 = -lst5 + lst1 - t3;
    cout << "s1: " << s1 << ", encoded: " << lst5.encode(s1) << ", inversed: " << lst5.decode(lst5.encode(s1)) << endl;

    

    return 0;
}