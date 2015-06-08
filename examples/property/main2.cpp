#include <iostream>

#include "property.h"




using namespace std;
using namespace lz;
using namespace PropertyPrivate;

struct tag_int{};
struct tag_double{};
struct tag_char{};
struct tag_string{};
struct tag_AA{};
struct tag_BB{};


struct AA
{
    AA()
    {
        cout << "con" << endl;
    }
    AA(const AA &o) 
    {
        cout << "copy" << endl;
    }
    AA(AA &&o)
    {
        cout << "move" << endl;
    }
    AA& operator=(const AA &o) 
    {
        cout << "operator= copy" << endl;
    }
    AA& operator=(AA &&o)
    {
        cout << "operator= move" << endl;
    }
};


AA aa1, aa2;

int main()
{


    // Property<tag_int, int> ii(1111);

    cout << "BEGIN" << endl;

    typedef 
    Property<tag_char, char,
    Property<tag_int, int> > MP;

    MP cc;


    cout << get<tag_char>(cc) << endl;
    cout << get<tag_int>(cc) << endl;

    get<tag_char>(cc) = '(';
    get<tag_int>(cc) = 80981203;

    cout << "UIUIUI" << endl;
    cout << get<tag_char>(cc) << endl;
    cout << get<tag_int>(cc) << endl;

    return 0;
}