
/*
 * testEP.cpp
 *
 *  Created on: 2015��11��10��
 *      Author: LZ
 */


#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <vector>


#include "lz/adjacency_list.h"
#include "lz/map.h"
#include "lz/property.h"

// property graph test
using namespace std;
using namespace lz;

struct int_tag{};
struct char_tag{};
int main()
{


    using VP = Property<int_tag, int,
               Property<char_tag, char> >;

    VP vp(10, '#');
    using G = AdjacencyList<DirectedGraphTag, VP>;
    G g;
    int n = 5;
    for(int i = 0; i < n; ++ i)
    {
        addVertex(g, VP(make_tuple(i, '#')) );
    }
    auto intMap = vertexPropertyMap(g, int_tag());

    for(int i = 0; i < n; ++ i)
    cout << intMap[i] << endl;


















    return 0;
}
















