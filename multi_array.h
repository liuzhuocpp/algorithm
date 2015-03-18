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

	namespace ns_MultiArray { // this namespace for implement class MultiArray 

	// for collecting the extents, and this class need to control multi thread 
	struct E
	{
		static vector<int> d;
		E operator[](int i) { d.push_back(i); return E(); }
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
	};

	// the data of class MultiArray
	template<typename T, int N>	
	struct 	D
	{
		vector<T> v;
		int d[N];
	};
	// inner class for MultiArray, not for user
	// this class is for class MultiArray to use operator[] continuously
	template<typename T,int n, int N>
	struct S
	{
		typedef S<T, n - 1, N> Type;
		typedef const S<T, n - 1, N> ConstType;

		D<T, N>* data;
		int id;
		S(D<T, N>* _data, int _id):data(_data), id(_id){}

		Type operator[](int o)
		{
			return S<T, n - 1, N>(data, id * data->d[N - n] + o);
		}
		ConstType operator[](int o) const
		{
			return S<T, n - 1, N>(data, id * data->d[N - n] + o);
		}

	};
	template<typename T, int N>
	struct S<T, 1, N>
	{
		typedef typename vector<T>::reference Type;
		typedef typename vector<T>::const_reference  ConstType;

		D<T, N>* data;
		int id;
		S(D<T, N>* _data, int _id):data(_data), id(_id){}

		Type operator[](int o)
		{
			return data->v[id * data->d[N - 1] + o];
		}
		ConstType operator[](int o) const
		{
			return data->v[id * data->d[N - 1] + o];
		}
	};

	}//	namespace ns_MultiArray


ns_MultiArray::F extents;
typedef ns_MultiArray::E Extent;

template<typename T, int N>
class MultiArray
{
	typedef ns_MultiArray::D<T, N> D;
	mutable D data;

	template<typename _T, int _n, int _N> friend class ns_MultiArray::S;
	typedef ns_MultiArray::S<T, N, N> S;
public:		
	typedef typename S::Type NextType; // this is the return-type after call operator[]
	typedef typename S::ConstType ConstNextType; // this is the return-type after call operator[] const
	typedef typename vector<T>::reference reference;
	typedef typename vector<T>::const_reference const_reference;
	MultiArray() = default;
	explicit MultiArray(const Extent & e) { resize(e); }
	NextType operator[](int i) { return S(&data, 0)[i]; }
	ConstNextType operator[](int i) const {	const S t(&data, 0); return t[i]; }
	inline int size() const { return data.v.size(); }
	inline int DimensionNumber() const { return N; }
	inline int dimension(int i) const { return data.d[i]; } // the size of i-th demension 
	void resize(const ns_MultiArray::E &e)
	{
		assert(int(e.d.size()) == N);
		int num = 1;
		for(int i = 0; i < N; ++ i) data.d[i] = e.d[i], num *= e.d[i];
		data.v.assign(num, T());
	}

};








} // namespace lz












#endif // MULTI_ARRAY_H