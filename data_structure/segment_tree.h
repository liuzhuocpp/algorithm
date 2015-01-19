#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H



template<typename Value>
class SegmentTreePrivate;
template<typename Value, typename >
class SegmentTree
{
public:

    SegmentTree();

    template<typename RandomIterator>
    SegmentTree(RandomIterator first, RandomIterator end);

    template<typename RandomIterator>
    void build(RandomIterator first, RandomIterator end);

    void modify(int l, int r, const Value &c);
    Value query(int l, int r);
private:
    SegmentTreePrivate<Value> *seg; 
};



#endif //end for SEGMENT_TREE_H