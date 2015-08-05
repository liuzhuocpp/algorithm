#ifndef SORT_H
#define SORT_H

#include <utility>
#include <functional> 
namespace lz {
using std::swap;
using std::less;



    namespace SortPrivate{

    }

    template<typename Iterator, typename Less>
    void quickSort(Iterator first, Iterator last, Less less)
    {
        if(first == last) return ;
        Iterator a = first;
        int n = last - first;
        int i = -1;
        for(int j = 0; j < n - 1; ++ j)
        {
            if(less(a[j], a[n - 1]))
            {
                ++ i;
                if(i != j) 
                    swap(a[i], a[j]);
            }
        }
        ++ i;
        if(i != n - 1) swap(a[i], a[n - 1]);
        quickSort(first, first + i, less);
        quickSort(first + i + 1, last, less);
    }
    template<typename Iterator>
    void quickSort(Iterator first, Iterator last)
    {
        quickSort(first, last, less<decltype(*first)>());
    }


}



#endif // SORT_H

