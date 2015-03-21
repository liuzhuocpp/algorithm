#ifndef KMP_H
#define KMP_H

#include <algorithm>
#include <vector>

namespace lz {
using std::vector;
using std::string;

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

class ExtendKmp
{
    template<typename RandomIterator>
    struct Node
    {
        RandomIterator sf, tf;
        int sn;
        Node(RandomIterator _sf, RandomIterator _tf, int _sn):
        sf(_sf),tf(_tf), sn(_sn) {}

        decltype(*sf) operator[](int i)
        {
            if(i < sn) return sf[i];
            else return tf[i - sn];
        }
    };

    template<typename RandomIterator>
    static vector<int> _computePrefix(RandomIterator s, int n)
    {        
        vector<int> p(n);
        if(n == 0) return p;
        p[0] = n - 1;
        if(n == 1) return p;
        int L = 1, R = 0;
        for(int i = 1; i < n && s[i] == s[i - 1]; ++ i) R = i;
        p[1] = R - L;
        for(int i = 2; i < n; ++ i)
        {
            if(i <= R)
            {
                if(p[i - L] < R - i) p[i] = p[i - L];
                else
                {
                    while(R + 1 < n && s[R + 1 - i] == s[R + 1]) R ++;
                    L = i;
                    p[i] = R - i;
                }
            }
            else
            {
                L = i, R = i - 1;
                while(R + 1 < n && s[R + 1 - i] == s[R + 1]) R ++;
                p[i] = R - i;
            }
        }
        return std::move(p);

    }

public:
    template<typename RandomIterator>
    static vector<int> computePrefix(RandomIterator first, RandomIterator end)
    {
        return std::move(_computePrefix(first, end - first));
    }




    template<typename RandomIterator>
    static vector<int> computePrefix(RandomIterator sf, RandomIterator se, 
                                     RandomIterator tf, RandomIterator te)
    {
        int ns = se - sf, nt = te - tf;
        Node<RandomIterator> f(sf, tf, ns);
        vector<int> p = _computePrefix(f, ns + nt);
        for(int i = ns; i < ns + nt; ++ i) p[i - ns] = p[i];
        p.resize(nt);
        return p;
    }
};





} // namespace lz


#endif // KMP_H