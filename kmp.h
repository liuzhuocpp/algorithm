#ifndef KMP_H
#define KMP_H

#include <algorithm>
#include <vector>

namespace lz {
using std::vector;

class Kmp
{
public:
	template<typename RandomIterator>
	static vector<int> computePrefix(RandomIterator first, RandomIterator end)
	{
		int n = end - first;
		RandomIterator s = first;
		vector<int> p(n);
		for(int k = p[0] = -1, i = 1; i < n; ++ i)
		{			
			while(k != -1 && !(s[k + 1] == s[i]) ) k = p[k];
			if(k != -1) k ++;
			else if(s[0] == s[i]) k = 0;
			p[i] = k;
		}
		return std::move(p);
	}
	template<typename RandomIteratorS, typename RandomIteratorT>
	static RandomIteratorT search(RandomIteratorS sfirst, RandomIteratorS send, 
					  RandomIteratorT tfirst, RandomIteratorT tend)
	{
		vector<int> p = computePrefix(sfirst, send);
		return search(p, sfirst, send, tfirst, tend);
	}
	template<typename RandomIteratorS, typename RandomIteratorT>
	static RandomIteratorT search(const vector<int> &p, RandomIteratorS sfirst, RandomIteratorS send, 
					  RandomIteratorT tfirst, RandomIteratorT tend)
	{		
		RandomIteratorS s = sfirst;
		RandomIteratorT t = tfirst;
		int ns = send - sfirst, nt = tend - tfirst;		
		for(int i = -1, j = 0; j < nt; ++ j)
		{
			while(i != -1 && !(s[i + 1] == t[j]) ) i = p[i];
			if(i != -1) i ++;
			else if(s[0] == t[j]) i = 0;

			if(i == ns - 1) return t + j - i;
		}
		return tend;
	}
};




} // namespace lz


#endif // KMP_H