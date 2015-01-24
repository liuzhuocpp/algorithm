#ifndef HEAVY_LIGHT_PATH_H
#define HEAVY_LIGHT_PATH_H

#include <vector>
#include <algorithm>
#include "adjacency_list.h"
using std::swap;
class HeavyLightPath
{
    
public:
    HeavyLightPath(){}
    HeavyLightPath(const AdjacencyList &_g, int rt = 0){ decomposition(_g, rt); }
    void decomposition(const AdjacencyList &_g, int rt = 0)
    {
        root = rt;
        g = &_g;
        int n = g->vertexNumber();
        siz.assign(n, 1);       
        dep.assign(n, 1);
        son.assign(n, -1);
        p.assign(n, -1);
        top.clear();
        for(int i = 0; i < n; ++ i) top.push_back(i);
        nid.assign(n, -1);
        inc = 0;
        dfs1(root, root);
        dfs2(root, root);

        vector<int> tp(n, -1), ttop(n, -1), tdep(n, 1);

        // np.assign(n, -1);
        // ntop.assign(n, -1);
        
        for(int i = 0; i < n; ++ i)
        {
            ttop[nid[i]] = nid[top[i]];
            tdep[nid[i]] = dep[i];
            if(p[i] == -1) continue;
            tp[nid[i]] = nid[p[i]];
        }
        p = std::move(tp);
        top = std::move(ttop);
        dep = std::move(tdep);
    }
    inline vector<int>& newId()  { return nid; }
    inline vector<int>& parent()  { return p; }
    inline vector<int>& topVertex()  { return top; }
    inline vector<int>& depth() { return dep; }
    int getPath(int a, int b, vector<pair<int, int> > &st) /// a->b
    {
        st.clear();
        while(top[a] != top[b])
        {
            if(dep[top[a]] < dep[top[b]]) swap(a, b);
            st.push_back(make_pair(top[a], a));
            a = p[top[a]];
        }

        if(dep[a] > dep[b]) swap(a, b);
        st.push_back(make_pair(a, b));
        return a;
    }

private:
    void dfs1(int u, int fa)
    {
        for(int i = g->begin(u); ~i; i = g->next(i))
        {
            int to = g->to(i);
            if(to == fa) continue;
            dep[to] = dep[u] + 1;
            p[to] = u;
            dfs1(to, u);
            siz[u] += siz[to];
            if(son[u] == -1 || siz[son[u]] < siz[to])
            {
                son[u] = to;
            }
        }
    }
    void dfs2(int u, int fa)
    {
        nid[u] = inc ++;
        if(son[u] != -1)
        {
            top[son[u]] = top[u];
            dfs2(son[u], u);
        }
        for(int i = g->begin(u); ~i; i = g->next(i))
        {
            int to = g->to(i);
            if(to == fa || to == son[u]) continue;
            dfs2(to, u);
        }
    }
    const AdjacencyList *g;
    vector<int> siz, son, dep, p, nid, top;
    // vector<int> np, ntop, ndep;

    int inc, root;

};


#endif //HEAVY_LIGHT_PATH_H
