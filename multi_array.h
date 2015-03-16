#ifndef MULTI_ARRAY_H
#define MULTI_ARRAY_H

#include <memory>
#include <vector>
#include <iostream>
#include <cassert>

namespace lz {

using std::allocator;
using std::cout;
using std::endl;
using std::vector;



template<typename T, int num_dim>
class MultiArray;

// for collecting the extents, and this class need to control multi thread 
struct E
{
	static vector<int> d;	
	E operator[](int i)
	{
		d.push_back(i); return E();
	}
};
vector<int> E::d(0);

// this is First class that call operator[] 
class F
{
public:	
	E operator[](int i)
	{
		(E::d).clear();
		return E()[i];
	}
}extents;





// inner class for MultiArray, not for user
// this class is for use operator[] continuously
template<typename T,int n, int N>
struct S
{
	typedef S<T, n - 1, N> type;

	MultiArray<T, N> * ma;
	int id;
	S(MultiArray<T, N> * _ma, int _id):ma(_ma), id(_id){}

	type operator[](int o)
	{
		return S<T, n - 1, N>(ma, id * (ma->d)[N - n] + o);
	}
};
template<typename T, int N>
struct S<T, 1, N>
{
	typedef T& type;

	MultiArray<T, N> * ma;
	int id;
	S(MultiArray<T, N> * _ma, int _id):ma(_ma), id(_id){}

	type operator[](int o)
	{
		return (ma->v)[id * (ma->d)[N - 1] + o];
	}
};

template<typename T, int N>
class MultiArray
{	
	int d[N];
	vector<T> v;

	template<typename _T, int _n, int _N>
	friend class S;

	MultiArray() = delete;
public:	
	explicit MultiArray(const E & e)
	{
		assert(int(e.d.size()) == N);
		int num = 1;
		for(int i = 0; i < N; ++ i) d[i] = e.d[i], num *= e.d[i];
		e.d.clear();
		v.assign(num, T());
	}
	typename S<T, N, N>::type operator[](int i)
	{
		return S<T, N, N>(this, 0)[i];
	}



};




} // namespace lz












#endif // MULTI_ARRAY_H