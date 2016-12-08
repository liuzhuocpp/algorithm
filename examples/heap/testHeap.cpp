/*
 * testHeap.cpp
 *
 *  Created on: 2016年12月7日
 *      Author: LZ
 */

#include "lz/heap.h"
#include <bits/stdc++.h>

using namespace lz;
using namespace std;

int main()
{
    Heap<int> h;
    h.push(10);
    h.push(20);
    h.push(30);
    h.push(40);

    h.push(2000);
    h.push(3300);
    h.push(33);
    h.push(4234);

    while (!h.isEmpty())
    {
        cout << h.top() << endl;
        h.pop();
    }

    return 0;
}

