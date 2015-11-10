#include <iostream>
#include <typeinfo>
#include <vector>
#include "lz/property.h"
using namespace std;
using namespace lz;

struct int_tag{};
struct char_tag{};

int main()
{
	using PP = Property<int_tag, int,
			   Property<char_tag, char> >;
	const PP p(123123, '$');

	cout << get(p, int_tag()) << endl;
	cout << get(p, char_tag()) << endl;

	cout << p[int_tag()] << endl;
	cout << p[char_tag()] << endl;


	return 0;
}
