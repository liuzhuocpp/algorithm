#include <bits/stdc++.h>
#include <lz/adjacency_list_base.h>

using namespace lz;
using namespace std;


int main()
{
    AdjacencyListBase<string> alb;
    int n = 5;
    for(int i = 0; i < n; ++ i)
    {
        alb.addBucket();
    }

    alb.insert(0, "sldfkj");
    alb.insert(0, "kkk");
    alb.insert(0, "aaa");
    alb.insert(0, "bbb");
    alb.insert(1, "fff");
    alb.insert(2, "eeee");

    for(auto x: alb.locals(0))
    {
        cout << x << endl;
    }

    cout << string(100, '-') << endl;
    for(auto x: alb)
    {
        cout << x << endl;
    }






	return 0;
}
