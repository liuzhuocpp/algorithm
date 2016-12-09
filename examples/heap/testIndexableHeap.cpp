/*
 * testIndexableHeap.cpp
 *
 *  Created on: 2016年12月7日
 *      Author: LZ
 */

#include "lz/heap.h"
#include "lz/map.h"
#include <bits/stdc++.h>
#include <cassert>
using namespace lz;
using namespace std;

int n = 1000;
int main()
{
    SharedArrayMap<std::size_t> indexMap(n);
    for(int i = 0; i < n; ++ i) indexMap[i] = -1;

    IndexableHeap<int, SharedArrayMap<std::size_t> > iHeap(indexMap);


    iHeap.push(33);
    iHeap.push(11);
    iHeap.push(67);
    iHeap.push(22);
    iHeap.push(93);
    iHeap.push(88);


    iHeap.c[0] = 9;
    indexMap[9] = 0;
    indexMap[0] = -1;
    iHeap.decrease(9);
//
//     iHeap.update(2, 1);
//     iHeap.update(1, 12);
//     iHeap.update(12, 88);
//     iHeap.push(876);
//     iHeap.push(876);
//     iHeap.push(876);


//     assert(iHeap.contains(88));
//     assert(!iHeap.contains(2));
//     assert(!iHeap.contains(3));
//     assert(iHeap.contains(876));



     cout << iHeap.contains(88) << endl;

     cout << iHeap.contains(2) << endl;
     cout << iHeap.contains(3) << endl;
     cout << iHeap.contains(876) << endl;
     cout << string(100, '-') << endl;





    while(!iHeap.isEmpty())
    {
        cout << iHeap.top() << endl;
        iHeap.pop();
    }






    return 0;
}

