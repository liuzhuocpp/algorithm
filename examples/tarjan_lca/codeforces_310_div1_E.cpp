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
//
typedef AdjacencyList<Undirected> G;
G g, ng;
int qa[N], qb[N], lca[N];
int nqa[N], nqb[N];
int m, qn;
int nid[N];
int up[N], down[N];

namespace Com
{
    G *g;
    bool vi[N];

    void dfs(int u)
    {
        for(int i = 0; i < sz(g->e[u]); ++ i)
        {
            int to = g->e[u][i];
            if(vi[to]) continue;
            vi[to] = 1;
            dfs(to);
        }
    }
    void create()
    {
        memset(vi, 0, sizeof(vi));
        for(int i = 1; i <= g->n; ++ i)
        {
            if(vi[i] == 0)
            {
                vi[i] = 1;
                dfs(i);
                g->e[g->n + 1].push_back(i);
                g->e[i].push_back(g->n + 1);
            }
        }
        g->n ++;
    }
}

void dfs(int u, int fa)
{
	typename G::OutEdgeIterator oi, oi_end;
	tie(oi, oi_end) = g.outEdges(u);
    for(; oi != oi_end; oi ++)
    {
    	int e = *oi;
    	int to = opposite(g, e, u);
        if(to == fa) continue;
        dfs(to, u);
        up[u] += up[to];
        down[u] += down[to];
    }

}


bool solve()
{
	int ng_n = BiconnectedComponentsOfEdge(g, nid);


    Com::g = &ng;
    Com::create();

    for(int i = 1; i <= qn; ++ i)
    {
        nqa[i] = nid[qa[i]];
        nqb[i] = nid[qb[i]];
    }

    Lca::g = &ng;
    Lca::qn = qn;
    Lca::qa = nqa;
    Lca::qb = nqb;
    Lca::lca = lca;
    Lca::root = ng.n;
    Lca::start();

//    cout <<"Ng: " << endl;
//    ng.out();
//    for(int i = 1; i <= qn; ++ i)
//    {
//        cout << "lca: " << qa[i] << " " << qb[i] << " "
//        << nqa[i] << " " << nqb[i] << " " << lca[i] << endl;
//    }

    memset(up, 0, sizeof(up));
    memset(down, 0, sizeof(down));
    for(int i = 1; i <= qn; ++ i)
    {
        if(lca[i] == ng.n) return 0;
        int a = nqa[i], b = nqb[i], r = lca[i];
        if(a == b) continue;
        up[a] ++;
        up[r] --;
        down[b] ++;
        down[r] --;
    }
    dfs(ng.n, ng.n);
    for(int i = 1; i <= ng.n; ++ i)
    {
        if(up[i] > 0 && down[i] > 0) return 0;
    }
    return 1;




}






int main()
{
//    freopen("in.txt", "r", stdin);

    scanf("%d%d%d", &g.n, &m, &qn);
    while(m --)
    {
        int a, b;scanf("%d%d", &a, &b);
        g.e[a].push_back(b);
        g.e[b].push_back(a);
    }
    for(int i = 1; i <= qn; ++ i)
    {
        int a, b;scanf("%d%d", &a, &b);
        qa[i] = a, qb[i] = b;
    }


    int ans = solve();
    if(ans) puts("Yes");
    else puts("No");








    return 0;
}

