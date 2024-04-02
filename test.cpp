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
    ShiftEncryptor t1(100);
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
    EncryptorList lst;
    lst.append(t1);
    lst.append(t2);
    cout << "s1: " << s1 << ", encode: " << lst.encode(s1) << ", decode: " << lst.decode(lst.encode(s1)) << endl; 
    return 0;
}