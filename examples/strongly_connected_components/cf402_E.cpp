

#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>
#include <ctime>
#include "lz/graph_utility.h"
#include "lz/adjacency_list.h"

#include "lz/parameter.h"
#include "lz/strongly_connected_components.h"




using namespace std;
using namespace lz;


using G = AdjacencyList<>;



int main()
{
//	freopen("in.txt", "r", stdin);

	G g;
	int n;
//	cin >> n;
	scanf("%d" , &n);

	for(int i = 0; i < n; ++ i) g.addVertex();
//	cout << "N " << n << endl;

	for(int i = 0; i < n; ++ i)
	{
		for(int j = 0; j < n; ++ j)
		{
			int x;
			scanf("%d", &x);
//			cin >> x;
			if(x)
			{
//				cout << "XX: " << endl;
				g.addEdge(i, j);
			}
		}
	}
//	cout << "FFFFJJJ " << endl;

	vector<int> compMap(n);
	int ans = stronglyConnectedComponents(g, makeIteratorMap(compMap.begin()));

//	for(int i = 0; i < n; ++ i)
//	{
//		cout << compMap[i] << endl;
//	}
	if(ans == 1) puts("YES");
	else puts("NO");




	return 0;





}
