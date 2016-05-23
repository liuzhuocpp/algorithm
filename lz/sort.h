#ifndef SORT_H
#define SORT_H

#include <utility>
#include <functional> 
#include <iterator>
#include <vector>
#include <iostream>
namespace lz {
using std::swap;
using std::less;
using std::cout;
using std::endl;
    namespace SortPrivate{

    }

    template<typename Iterator, typename Less>
    void quickSort(Iterator first, Iterator last, Less less)
    {
        if(last - first < 2) return ;
        using DiffType = typename std::iterator_traits<Iterator>::difference_type;

        Iterator a = first;
        DiffType n = last - first;
        DiffType i = -1;
        for(DiffType j = 0; j < n - 1; ++ j)
        {
            if(less(a[j] , a[n - 1]))
            {
                ++ i;
                if(i != j) 
                    swap(a[i], a[j]);
            }
        }
        if(i + 1 != n - 1)
        	swap(a[i + 1], a[n - 1]);
        quickSort(first, first + i + 1, less);
        quickSort(first + i + 2, last, less);
    }
    template<typename Iterator>
    void quickSort(Iterator first, Iterator last)
    {

        quickSort(first, last, std::less<typename std::iterator_traits<Iterator>::value_type>());
    }





    template<typename Iterator, typename Less>
    void quickSortNotRecursion(Iterator first, Iterator end, Less less)
    {
		using DiffType = typename std::iterator_traits<Iterator>::difference_type;
		Iterator a = first;
		DiffType n = end - first;
    	std::vector<std::pair<DiffType, DiffType> > v;

    	v.push_back(std::make_pair(0, n - 1));
    	for(int i = 0; i < n; ++ i)
    	{
    		cout << a[i] << endl;
    	}
    	while(v.size())
    	{
    		DiffType x, y;
    		std::tie(x, y) = v.back();
    		v.pop_back();
    		DiffType i = x - 1;
    		for(auto j = x; j < y; ++ j)
    		{
    			if(less(a[j], a[y]))
    			{
    				++ i;
    				if(i != j) swap(a[i], a[j]);
    			}
    		}

    		if(i + 1 != y)
    			swap(a[i + 1], a[y]);

    		if(i - x + 1 > 1)
    		{
    			v.push_back(std::make_pair(x, i));
    		}

    		if(y - (i + 2) + 1 > 1)
    		{
    			v.push_back(std::make_pair(i + 2, y));
    		}

    	}
    }

    template<typename Iterator>
	void quickSortNotRecursion(Iterator first, Iterator last)
	{
    	quickSortNotRecursion(first, last, std::less<typename std::iterator_traits<Iterator>::value_type>());
	}



    // pre(x, i): key for radix sort, key range: [0, maxKey)
    // x -> ValueType
    // i -> Dimension , range: [0, dim)
    // priority : 0->dim increment
    template<typename Iterator, typename Predicate, typename Dimension, typename KeyType>
    void radixSort(Iterator first, Iterator last, Predicate pre, Dimension dim, KeyType maxKey)
    {
    	using DiffType = typename std::iterator_traits<Iterator>::difference_type;
    	using ValueType = typename  std::iterator_traits<Iterator>::value_type;
    	std::vector<KeyType> cot;
    	std::vector<ValueType> tmp;
    	auto a = first;
    	auto n = last - first;
    	for(Dimension i = 0; i < dim; ++ i)
    	{
    		cot.assign(maxKey, 0);
    		for(DiffType j = 0; j < n; ++ j) cot[pre(a[j], i)] ++;
    		for(KeyType j = 1; j < maxKey; ++ j) cot[j] += cot[j - 1];
    		tmp.assign(first, last);
    		for(DiffType j = n - 1; j >= 0; -- j)
    		{
    			a[--cot[pre(tmp[j], i)]] = tmp[j];
    		}
    	}


    }


































}



#endif // SORT_H

