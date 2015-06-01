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
    Property<tag_double, double> > > cnt('z', 123, 111.1);




    return 0;
}