#include <bits/stdc++.h>

#include <lz/iterator_range.h>
#include <lz/std_utility.h>
using namespace lz;
using namespace std;


int main()
{
    int n = 100;
    int step = 11;
    vector<int> a(n);
    for(int i = 0; i < a.size(); ++ i) a[i] = i;

    StepIteratorRange<vector<int>::iterator> stepIt(a.begin(), a.end(), step);


    auto _range = irange(66, 1, -3);



    for(auto x: _range)
    {
        cout << x << endl;
    }

    assert(_range.begin()[0] == 66);
    assert(_range.begin()[1] == 63);
    assert(_range.begin()[2] == 60);

    assert(_range.end()[-3] == 9);
    assert(_range.end()[-2] == 6);
    assert(_range.end()[-1] == 3);


	return 0;
}
