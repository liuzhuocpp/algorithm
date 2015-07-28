/*
 * codeforces_310_div1_E.cpp
 *
 *  Created on: 2015Äê7ÔÂ26ÈÕ
 *      Author: LZ
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <tuple>

#include "adjacency_list.h"
#include "tarjan_lca.h"
#include "biconnected_components_of_edge.h"




using namespace std;
using namespace lz;

typedef AdjacencyList<Undirected> G;

const int N = 2e5 + 9;
G g, ng;
int qa[N], qb[N], lca[N];
int nqa[N], nqb[N];
int qn;
int nid[N];
int up[N], down[N];

namespace Com
{
	G &g = ng;
    bool vi[N];
    void dfs(int u)
    {
    	typename G::OutEdgeIterator oi, oi_end;
    	for(tie(oi, oi_end) = g.outEdges(u); oi != oi_end; ++ oi)
        {
    		int to = opposite(g, *oi, u);
            if(vi[to]) continue;
            vi[to] = 1;
            dfs(to);
        }
    }
    void create()
    {
        memset(vi, 0, sizeof(vi));
        int n = g.vertexNumber();
        g.addVertex();
        for(int i = 0; i < n; ++ i)
        {
            if(vi[i] == 0)
            {
                vi[i] = 1;
                dfs(i);
                g.addEdge(n, i);
            }
        }
    }
}

void dfs(int u, int fa)
{
	typename G::OutEdgeIterator oi, oi_end;
	tie(oi, oi_end) = ng.outEdges(u);
    for(; oi != oi_end; oi ++)
    {
    	int e = *oi;
    	int to = opposite(ng, e, u);
        if(to == fa) continue;
        dfs(to, u);
        up[u] += up[to];
        down[u] += down[to];
    }

}

template<typename G>
void out(const G &g)
{
	cout << "output graph:" << endl;
	typename GraphTraits<G>::OutEdgeIterator oi, oi_end;
	int n = g.vertexNumber();
	for(int u = 0; u < n; ++ u)
	{
		tie(oi, oi_end) = g.outEdges(u);
		cout << "u="<< u << " :";
		for(;oi != oi_end; ++ oi)
		{
			int e = *oi;
			cout << opposite(g, e, u) << " " ;
		}
		cout << endl;


	}
}

bool solve()
{
	int n = g.vertexNumber();
	int ng_n = BiconnectedComponentsOfEdge(g, nid);

//	cout << "New" << endl;

//	for(int i = 0; i < n; ++ i)
//	{
//		cout << "nid: " << i << " " << nid[i] << endl;
//	}
//	return 0;

	for(int i = 0; i < ng_n; ++ i) ng.addVertex();


	for(int u = 0; u < n; ++ u)
	{
		typename G::OutEdgeIterator oi, oi_end;
		for(tie(oi, oi_end) = g.outEdges(u); oi != oi_end; ++ oi)
		{
			int a = g.source(*oi), b = g.target(*oi);
			if(nid[a] != nid[b])
			{
				ng.addEdge(nid[a], nid[b]);
			}
		}
	}
//	cout << "^&^&" << endl;
    Com::create();
//    cout << "~~~~~~~~~~" << endl;
    int root = ng.vertexNumber() - 1;
    for(int i = 0; i < qn; ++ i)
    {
        nqa[i] = nid[qa[i]];
        nqb[i] = nid[qb[i]];
    }
//    out(ng);
    tarjanLCA(ng, ng_n, nqa, nqb, qn, lca);
//    cout << string(100, '|') << endl;
//    cout <<"Ng: " << endl;
//    ng.out();
//    for(int i = 1; i <= qn; ++ i)
//    {
//        cout << "lca: " << qa[i] << " " << qb[i] << " "
//        << nqa[i] << " " << nqb[i] << " " << lca[i] << endl;
//    }

    memset(up, 0, sizeof(up));
    memset(down, 0, sizeof(down));
    for(int i = 0; i < qn; ++ i)
    {
        if(lca[i] == root) return 0;
        int a = nqa[i], b = nqb[i], r = lca[i];
        if(a == b) continue;
        up[a] ++;
        up[r] --;
        down[b] ++;
        down[r] --;
    }
//    cout << string(100, '+') << endl;
    dfs(root, root);
//    cout << string(100, '^') << endl;
    for(int i = 0; i < ng.vertexNumber(); ++ i)
    {
        if(up[i] > 0 && down[i] > 0) return 0;
    }
    return 1;




}






int main()
{
//    freopen("in.txt", "r", stdin);

	int n, m;
    scanf("%d%d%d", &n, &m, &qn);
//    cout << "NN:: " << n << " " << m << " " << qn << endl;
    while(n --) g.addVertex();

    while(m --)
    {
        int a, b;
        scanf("%d%d", &a, &b);
        a --;
        b --;
        g.addEdge(a, b);
    }

    for(int i = 0; i < qn; ++ i)
    {
        int a, b;scanf("%d%d", &a, &b);
        a --;
        b --;
        qa[i] = a, qb[i] = b;
    }

//    cout << "RR" << endl;
    int ans = solve();
    if(ans) puts("Yes");
    else puts("No");








    return 0;
}

