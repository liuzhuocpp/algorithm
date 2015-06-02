#include <iostream>

#include "property.h"




using namespace std;
using namespace lz;
using namespace PropertyPrivate;

struct tag_int{};
struct tag_double{};
struct tag_char{};
struct tag_string{};

int main()
{
    cout << CountVariadic<int, int, double>::value << endl;


    Property<tag_char, char, 
    Property<tag_int, int, 
    Property<tag_double, double> > > cnt('z', 123
        , 11.11
        );

    cout << get<tag_char>(cnt) << endl;
    get<tag_char>(cnt) = '^';


    cout << get<tag_double>(cnt) << endl;
    get<tag_double>(cnt) = 22.211;

    cout << get<tag_double>(cnt) << endl;
    cout << get<tag_char>(cnt) << endl;


    typedef 
    Property<tag_char, char, 
    Property<tag_int, int, 
    Property<tag_double, double> > > PP;

    const PP aa('$', 12309, 91023.1223);

    cout << get<tag_char>(aa) << endl;
    // get<tag_char>(aa) = '1';




    return 0;
}