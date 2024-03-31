#include "string.h"

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
    return 0;
}