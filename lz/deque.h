/*
 * deque.h
 *
 *  Created on: 2015Äê9ÔÂ13ÈÕ
 *      Author: LZ
 */

#ifndef LZ_DEQUE_H_
#define LZ_DEQUE_H_


#include <vector>

#include <iostream>
namespace lz {

	namespace DequePrivate{
	}


using std::vector;
using std::endl;
using std::cout;
using std::cin;
template<typename T>
class Deque
{
	vector<T> f, b;
	size_t pf, pb;


public:
	Deque()
	{
		pf = pb = 0;
	}

	void push_back(const T & x)
	{
		if(pf == 0)
		{
			b.push_back(x);
		}
		else
		{
			f[--pf] = x;
		}
	}
	void pop_back()
	{
		if(!b.empty()) b.pop_back();
		else
		{
			++ pf;
		}
	}
	void push_front(const T &x)
	{
		if(pb == 0)
		{
			f.push_back(x);
		}
		else b[-- pb] = x;
	}
	void pop_front()
	{
		if(!f.empty()) f.pop_back();
		else ++ pb;
	}

	T& operator[](size_t i)
	{
		if(pf + i < f.size()) return f[pf + i];
		else return b[i - f.size()];
	}
	size_t size() const
	{
		return b.size() - pb + f.size() - pf;
	}
	void out()
	{
		for(int i = 0; i < size(); ++ i)
		{
			cout << (*this)[i] << ", ";
		}
		cout << endl;

		return ;
//		cout << "UIU:" << endl;
		for(int i = int(f.size()) - 1; i >= int(pf); -- i)
			cout << f[i] << ",";
//		cout << "FFF" << endl;
		for(int i = pb; i < b.size(); ++ i)
			cout << b[i] << ",";
		cout << endl;
	}

};



















}


#endif /* LZ_DEQUE_H_ */
