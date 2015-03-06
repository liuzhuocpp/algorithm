#ifndef BICONNECTED_COMPONENTS_OF_POINT_H
#define BICONNECTED_COMPONENTS_OF_POINT_H

#include <vector>
#include <algorithm>

#include "adjacency_list.h"

namespace lz {


using std::vector;
using std::min;
class BiconnectedComponentsOfPoint
{
public:
    BiconnectedComponentsOfPoint() {}
    BiconnectedComponentsOfPoint(const AdjacencyList<> &_g) { shrink(_g); }
    ~BiconnectedComponentsOfPoint() { delete ng; }
    void shrink(const AdjacencyList<> &_g)
    {
        g = &_g;
        int n = g->vertexNumber();
        stk.clear();
        low.assign(n, -1);
        dfn.assign(n, -1);
        nid.assign(n, -1);
        iscut.assign(n, 0);

        p.assign(2 * n, -1);
        inc = 0;
        tot = 0;

        dfs(0, 0);

        ng = new AdjacencyList<>;
        ng->clear(tot);
        p.resize(tot);
        for(int i = 0; i < tot; ++ i)
        {
            if(p[i] != -1) ng->addBidirectionalEdge(i, p[i]);
            else rt = i;
        }
    }
    AdjacencyList<> &newTree() { return *ng; }
    vector<int> &parent() { return p; }
    vector<int> &newId() { return nid; }
    vector<bool> &isCut() { return iscut; }
    int root() { return rt; }
private:
    void dfs(int u, int fa)
    {
        low[u] = dfn[u] = ++ inc;
        stk.push_back(u);
        for(int i = g->begin(u); ~i; i = g->next(i))
        {
            int to = g->to(i);
            if(to == fa) continue;
            if(dfn[to] == -1)
            {
                dfs(to, u);
                low[u] = min(low[u], low[to]);
                if(low[to] >= dfn[u])
                {
                    if(nid[u] == -1) nid[u] = tot ++, iscut[u] = 1;
                    p[tot] = nid[u];
                    int c;
                    do
                    {
                        c = stk.back();
                        stk.pop_back();
                        if(iscut[c] == 1) p[nid[c]] = tot;
                        else nid[c] = tot;
                    }while(c != to);
                    tot ++;
                }
            }
            else if(dfn[u] > dfn[to])
            {
                low[u] = min(low[u], dfn[to]);
            }
        }
    }

    const AdjacencyList<> *g;
    AdjacencyList<> *ng;
    vector<int> stk, low, dfn, p, nid;
    vector<bool> iscut;
    int inc, tot, rt;
};


} // namespace lz

#endif //BICONNECTED_COMPONENTS_H