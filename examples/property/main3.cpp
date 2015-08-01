#include <iostream>
#include "property.h"
#include <vector>
using namespace std;
using namespace lz;

struct int_tag{};
struct double_tag{};
struct char_tag{};

typedef Property<int_tag, int,
		Property<double_tag,double,
		Property<char_tag, char   >>> P;


template<typename Tag, typename Container>
void convertToIterator(Container &container)
{

}

template<typename P, typename Tag, typename C>
struct Impl
{
	vector<P> p;

	struct it_int
	{
		vector<P>::iterator it;
		int& operator[](int i)
		{
			return get<int_tag>(it[i]);
		}
	};
};




int main()
{
	P p;

	get<int_tag>(p) = 100;
	get<double_tag>(p) = 2.2222;
	get<char_tag>(p) = 'g';

	cout << get<int_tag> (p) << endl;
	cout << get<double_tag> (p) << endl;
	cout << get<char_tag> (p) << endl;





}
