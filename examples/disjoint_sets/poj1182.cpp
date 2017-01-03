//#pragma GCC diagnostic error "-std=c++0x"
//
//
//#include <iostream>
//#include <cstdio>
//#include <cstring>
//
//#include <vector>
//
//
///*
// * disjoint_sets.h
// *
// *  Created on: 2015��11��29��
// *      Author: LZ
// */
//
//#ifndef LZ_DISJOINT_SETS_H_
//#define LZ_DISJOINT_SETS_H_
//
///*
// * map.h
// *
// *  Created on: 2015��8��5��
// *      Author: LZ
// */
//
///*
// * Map concept is similar to boost property map library,
// * but Map concept is sample. It only contain operator[] for
// * a Map object.
// */
//#ifndef LZ_MAP_H_
//#define LZ_MAP_H_
//
////#include <iostream>
//
//namespace lz {
//
////using std::cout;
////using std::endl;
//
//template<typename Map>
//struct MapTraits
//{
//	using KeyType = typename Map::KeyType;
//	using ValueType = typename Map::ValueType;
//};
//
//template<typename Key, typename Value, typename ReferenceType = Value&>
//struct MapFacade
//{
//	using KeyType = Key;
//	using ValueType = Value;
//};
//
//
//template<typename Key>
//struct IdentityMap:public MapFacade<Key, Key>
//{
//	Key operator[](Key key) const
//	{
//		return key;
//	}
//};
//
////template<typename UnaryFunction, typename Key, typename Value>
////class FunctionMap:public MapFacade<Key, Value>
////{
////	const UnaryFunction* f = nullptr;
////	using Base = MapFacade<Key, Value>;
////public:
////	const UnaryFunction* function(){ return f;}
////
////	explicit FunctionMap() = default;
////	explicit FunctionMap(const UnaryFunction &f):f(&f) {}
////	typename Base::ValueType operator[](typename Base::KeyType key) const
////	{
////		return (*f)(key);
////	}
////};
//template<typename I,
//		 typename Key = typename std::iterator_traits<I>::difference_type,
//		 typename Value = int& >
//class IteratorMap:public MapFacade<Key, Value>
//{
//	I i;
//	using Base = MapFacade<Key, Value>;
//public:
//	IteratorMap() = default;
//	IteratorMap(I i):i(i){}
//
//	using Iterator = I;
//	Iterator iterator() const { return i;}
//
//	typename Base::ValueType operator[](typename Base::KeyType key)
//	{
//		return i[key];
//	}
//};
//template<typename I>
//IteratorMap<I> makeIteratorMap(I i)
//{
//	return IteratorMap<I>(i);
//}
//
//
//template<typename FM, typename SM>
//class ComposeMap:public MapFacade<typename MapTraits<FM>::KeyType, typename MapTraits<SM>::ValueType >
//{
//	FM fm;
//	SM sm;
//	using Base = MapFacade<typename MapTraits<FM>::KeyType, typename MapTraits<SM>::ValueType>;
//public:
//	using FirstMap = FM;
//	using SecondMap = SM;
//
//	explicit ComposeMap() = default;
//	explicit ComposeMap(FM fm, SM sm):fm(fm), sm(sm){}
//
//	FirstMap firstMap() const { return fm; }
//	SecondMap secondMap() const { return sm; }
//
//	typename Base::ValueType operator[](typename Base::KeyType key) const
//	{
//		return sm[fm[key]];
//	}
//};
//
//template<typename FM, typename SM>
//ComposeMap<FM, SM> makeComposeMap(FM fm, SM sm)
//{
//	return ComposeMap<FM, SM>(fm, sm);
//}
//
//
//
//
//
//
//
//}// namespace lz
//
//
//
//
//#endif /* LZ_MAP_H_ */
//
//
//
//
//namespace lz {
//
//
//
//struct DisjointSetsParams
//{
//	template<typename V>
//	void initializeParent(V u) {}
//
//	template<typename V>
//	void changeParent(V u, V new_parent) {}
//
//};
//
//template<typename ParentMap, typename Params = DisjointSetsParams>
//class DisjointSets
//{
//	ParentMap p;
//	Params params;
//public:
//	using ValueType = typename MapTraits<ParentMap>::ValueType;
//	DisjointSets(ParentMap p, Params &params):p(p), params(params) {}
//	void makeSet(ValueType u)
//	{
//		params.initializeParent(u);
//		p[u] = u;
//	}
//	template<typename ElementIterator>
//	void makeSet(ElementIterator first, ElementIterator last)
//	{
//		for(;first != last; first ++)
//		{
//			makeSet(*first);
//		}
//	}
//	ValueType findSet(ValueType u)
//	{
//		if(p[u] != u)
//		{
//			ValueType new_parent = findSet(p[u]);
//			params.changeParent(u, new_parent);
//			p[u] = new_parent;
//		}
//		return p[u];
//	}
//	void link(ValueType x, ValueType y)
//	{
//		params.changeParent(x, y);
//		p[x] = y;
//	}
//	void unionSet(ValueType x, ValueType y)
//	{
//		link(findSet(x), findSet(y));
//	}
//
//};
//
//
//
//
//}
//
//
//
//
//
//
//
//#endif /* LZ_DISJOINT_SETS_H_ */
//
//
//
//
///*
// * 0: u same to parent
// * 1: u eat parent
// * 2: parent eat u
// */
//// poj 1182
////using namespace std;
//using namespace lz;
//
//
//const int N = 50000 + 9;
//
//int p[N], rank[N];
//using V = int;
//struct P:public DisjointSetsParams
//{
//	void initializeParent(V u)
//	{
//		rank[u] = 0;
//	}
//	void changeParent(V u, V new_p)
//	{
//		V old = p[u];
//		if(u == old) return;
//
//		if(rank[u] == 0) rank[u] = rank[old];
//		else if(rank[u] == 1)
//		{
//			if(rank[old] == 0) rank[u] = 1;
//			else if(rank[old] == 1) rank[u] = -1;
//			else if(rank[old] == -1) rank[u] = 0;
//		}
//		else if(rank[u] == -1)
//		{
//			if(rank[old] == 0) rank[u] = -1;
//			else if(rank[old] == 1) rank[u] = 0;
//			else rank[u] = 1;
//		}
//
//	}
//};
//
//int main()
//{
//	P params;
//	DisjointSets<IteratorMap<int*> , P> dis_set(makeIteratorMap(p), params);
//	int n, k;
//	std::scanf("%d%d", &n, &k);
//	for(int i = 1; i <= n; ++ i)
//	{
//		dis_set.makeSet(i);
//	}
//	int ans = 0;
//	while(k --)
//	{
//		int c, x, y;
//		scanf("%d%d%d", &c, &x, &y);
//		if(x > n || y > n || (c == 2 && x == y))
//		{
//			ans ++; continue;
//		}
//
//
//		int rx = dis_set.findSet(x),
//			ry = dis_set.findSet(y);
//		if(c == 1)
//		{
//			if(rx != ry)
//			{
//				dis_set.link(rx, ry);
//				rank[rx] = 0;
//			}
//			else
//			{
//				if(!(rank[x] == rank[y])) ans ++;
//			}
//		}
//		else
//		{
//			if(rx == ry)
//			{
//				if(x != rx && y != rx)
//				{
//					if(
//					   (rank[x] == 1 && rank[y] == 0) ||
//					   (rank[x] == 0 && rank[y] == -1) ||
//					   (rank[x] == -1 && rank[y] == 1) );
//					else ans ++;
//				}
//				else if(x == rx)
//				{
//					if(rank[y] == -1);
//					else ans ++;
//				}
//				else
//				{
//					if(rank[x] == 1);
//					else ans ++;
//				}
//			}
//			else
//			{
//				dis_set.link(rx, ry);
//				rank[rx] = 1;
//			}
//
//
//		}
//
//
//	}
//	std::printf("%d\n", ans);
//
//
//
//
//
//
//
//
//
//	return 0;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
