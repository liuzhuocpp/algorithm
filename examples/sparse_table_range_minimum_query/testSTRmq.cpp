
#include <bits/stdc++.h>
#include "lz/sparse_table_range_minimum_query.h"

using namespace lz;
using namespace std;


int main()
{
    vector<int> a = {2, 3, -9991, 5, 6, 10, -6666, 1112, 33};
    cout << "FFF" << endl;
    auto rmq = sparseTableRangeMinimumQuery(a.begin(), a.end()   );
    cout << "FFF" << endl;
    cout << rmq(0, 3) << endl;
    cout << rmq(0, 6) << endl;
    cout << rmq(0, 8) << endl;
    cout << rmq(0, int(a.size()) - 1) << endl;
    cout << rmq(3, int(a.size()) - 1) << endl;

	return 0;
}
