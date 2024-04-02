#include "StringBase.h"
#include "String.h"
#include "Encryptor.h"
#include "EncryptorList.h"
#include "EncryptedString.h"

#include "gtest_lite.h"
#include "memtrace.h"


using std::endl;
using std::cout;
int main(void) {
    TEST(Ctor, Ctor) {
        String ures;
        EXPECT_EQ(static_cast<size_t>(0), ures.get_len()) << "Nem jo a parameter nelkuli konstruktor" << endl;
        EXPECT_STREQ("", ures.c_str()) << "Nem jo a parameter nelkuli konstruktor" << endl;
        const char c = 'a';
        String egybetu(c);
        EXPECT_EQ(static_cast<size_t>(1), egybetu.get_len()) << "Nem jo a betu konstruktor" << endl;
        EXPECT_STREQ("a", egybetu.c_str()) << "Nem jo a betu konstruktor" << endl;
        const char c_str[] = "hello";
        String str(c_str);
        EXPECT_EQ(static_cast<size_t>(5), str.get_len()) << "Nem jo a c_string konstruktor" << endl;
        EXPECT_STREQ(c_str, str.c_str()) << "Nem jo a betu konstruktor" << endl;
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