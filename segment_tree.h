#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H


template<typename Value>
class SegmentTree
{
public:

    SegmentTree():c(0){}
    SegmentTree(Value *a, int n) { build(a, n); }
    ~SegmentTree() 
    {
        if(c) delete [] c;
    }
    void build(Value *_a, int _n)
    {        
        if(c == 0) c = new Value[_n * 4 + 9];
        else if(_n > n) 
        {            
            delete [] c;
            c = new Value[_n * 4 + 9];
        }

        n = _n;
        a = _a;
        _build(1, 0, n - 1);
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
    Value *c, *a;
    int n;
#define ls (p << 1)
#define rs (ls | 1)
#define mid ((L + R) >> 1)
    void _build(int p, int L, int R)
    {
        if(L == R)
        {
            c[p] = a[L];
            return ;
        }
        _build(ls, L, mid);
        _build(rs, mid + 1, R);
        c[p] = c[ls] + c[rs];
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
        c[p] = c[ls] + c[rs];
    }
    Value _query(int p, int L, int R, int l, int r)
    {
        if(l == L && r == R) return c[p];
        if(r <= mid) return _query(ls, L, mid, l, r);
        else if(l > mid) return _query(rs, mid + 1, R, l, r);
        else return _query(ls, L, mid, l, mid) + 
                    _query(rs, mid + 1, R, mid + 1, r);
    }
#undef mid
#undef ls
#undef rs
    
};



#endif //end for SEGMENT_TREE_H