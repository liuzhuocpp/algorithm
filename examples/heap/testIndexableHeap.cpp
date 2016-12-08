/*
 * testIndexableHeap.cpp
 *
 *  Created on: 2016年12月7日
 *      Author: LZ
 */

#include "lz/heap.h"
#include "lz/map.h"
#include <bits/stdc++.h>

using namespace lz;
using namespace std;

int n = 1000;
int main()
{
    SharedArrayMap<std::size_t> indexMap(n);

    IndexableHeap<int, SharedArrayMap<std::size_t> > iHeap(indexMap);


    iHeap.push(3);


//    iHeap.push(1);
//    iHeap.push(0);
//    iHeap.push(2);
//    iHeap.push(9);
//    iHeap.push(88);

     iHeap.update(3, 2);

     iHeap.update(2, 1);
     iHeap.update(1, 12);
     iHeap.update(12, 88);
     iHeap.push(876);
     iHeap.push(876);
     iHeap.push(876);








    while(!iHeap.isEmpty())
    {
        cout << iHeap.top() << endl;
        iHeap.pop();
    }






    return 0;
}

