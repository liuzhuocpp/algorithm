#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H




#include <functional>
#include <vector>
namespace lz {

#define ls (p << 1)
#define rs (ls | 1)
#define mid ((L + R) >> 1)

using std::plus;
using std::vector;

template<typename Value, typename Plus = plus<Value> >
class SegmentTree
{    
public:

    void build(const vector<Value> &a)
    {
        n = a.size();
        c.assign(4 * n, 0);
        _build(1, 0, n - 1, a);
    }
    void modify(int x, const Value &v)
    {
        _modify(1, 0, n - 1, x, v);
    }
    Value query(int l, int r)
    {
        return _query(1, 0, n - 1, l, r);
    }
private:
    template<typename Array>
    void _build(int p, int L, int R, const Array &a)
    {
        if(L == R)
        {
            c[p] = a[L]; return ;
        }
        _build(ls, L, mid, a);
        _build(rs, mid + 1, R, a);
        c[p] = Plus()(c[ls], c[rs]);
    }
    void _modify(int p, int L, int R, int x, const Value &v)
    {
        if(L == R)
        {
            c[p] = v;
            return ;
        }
        if(x <= mid) _modify(ls, L, mid, x, v);
        else _modify(rs, mid + 1, R, x, v);
        c[p] = Plus()(c[ls], c[rs]);
    }
    Value _query(int p, int L, int R, int l, int r)
    {
        if(l == L && r == R) return c[p];
        if(r <= mid) return _query(ls, L, mid, l, r);
        else if(l > mid) return _query(rs, mid + 1, R, l, r);
        else return Plus()( _query(ls, L, mid, l, mid),
                        _query(rs, mid + 1, R, mid + 1, r) );
    }
    int n;
    vector<Value> c;

};


#undef mid
#undef ls
#undef rs

}// namespace lz

#endif // SEGMENT_TREE_H