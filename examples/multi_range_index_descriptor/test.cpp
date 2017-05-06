#include <bits/stdc++.h>
#include <lz/multi_range_index_descriptor.h>

using namespace lz;
using namespace std;





void test()
{
    using D =  MultiRangeIndexDescriptor<unsigned int, 30U>;
    D a;
    a.set(3, 35);


    cout << a.rangeIndex() << endl;
    cout << a.index() << endl;

    cout << D::rangeSize(0) << endl;
    cout << D::rangeSize(3) << endl;
    cout << D::rangeSize(29) << endl;




    assert(D::rangeSize(0) == 143165576);
    assert(D::rangeSize(1) == 143165576);
    assert(D::rangeSize(29) == 143165591);



}


int main()
{
    test();
	return 0;
}
