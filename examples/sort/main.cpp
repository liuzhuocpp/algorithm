#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>



#include "sort.h"
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
    vector<int> a;
    for(int i = 0; i < 10; ++ i) a.push_back(10 - i);


    random_shuffle(a.begin(), a.end());
    out(a);


    quickSort(a.begin(), a.end());
    
    out(a);    

    return 0;
}