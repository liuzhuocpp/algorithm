#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>



#include "lz/sort.h"
using namespace std;
using namespace lz;

template<typename Seq>
void out(Seq &a)
{
    cout << "[";
    for(int i = 0; i < a.size(); ++ i)
    {
        if(i > 0) putchar(',');
        cout << a[i] ;
    }
    cout << "]" << endl;
}


int main()
{
	vector<int> a = {2, 3, 88, 5, 11, 23, 23, 23};

//	lz::radixSort(a.begin(), a.end(), [](int x, int i) { return x; }, 1, 100);

	lz::radixSort(a.begin(), a.end(),
			[](int x, int i) {
				if(i == 0) return x % 10;
				return x / 10;
			},
			2, 10);

//	lz::quickSortNotRecursion(a.begin(), a.end());
//	lz::quickSort(a.begin(), a.end());
//	lz::quickSort(a.begin(), a.end());
	out(a);

//    vector<int> a;
//    for(int i = 0; i < 10; ++ i) a.push_back(10 - i);
//
//
//    random_shuffle(a.begin(), a.end());
//
//
//
//    quickSort(a.begin(), a.end());
//
//    out(a);

    return 0;
}
