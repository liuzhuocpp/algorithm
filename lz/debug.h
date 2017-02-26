/*
 * debug.h
 *
 *  Created on: 2017年1月18日
 *      Author: LZ
 */

#ifndef LZ_DEBUG_H_
#define LZ_DEBUG_H_

#include <lz/iterator_range.h>
#include <iostream>
#include <iterator>

namespace lz {

template<typename InputIterator1, typename InputIterator2>
bool isEqual(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
{
    while(first1 != last1)
    {
        if(*first1 != *first2) return 0;
        first1 ++;
        first2 ++;
    }
    if(first2 != last2) return 0;
    return 1;
}

template<typename InputIterator>
bool isEqual(InputIterator first1, InputIterator last1,
    const std::vector<typename std::iterator_traits<InputIterator>::value_type>& b)
{
    return isEqual(first1, last1, b.begin(), b.end());
}

template<typename T>
bool isEqual(const std::vector<T> &a, const std::vector<T> &b)
{
    return isEqual(a.begin(), a.end(), b.begin(), b.end());
}


template <class Char, class Traits, typename IteratorRange>
std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os,
           const IteratorRange&  range)
{
    auto first = std::begin(range), last = std::end(range);
    os << "[";
    if(first == last)
    {
        return os << "]";
    }
    while(first != last)
    {
        auto nfirst = first;
        ++ nfirst;
        if(nfirst == last)
        {
            break;
        }
        os << *first << ",";
        ++first;
    }
    os << *first;
    return os << "]";
}

#define OUT_FUNCTION_NAME cout << "\n\n"<< __FUNCTION__ << " ------------------------------------------------" << endl;
//template<typename IteratorRange>
//void out(IteratorRange range)
//{
//    cout << "[";
//    while(first != last)
//    {
//        cout << *first << ",";
//        first++;
//    }
//    cout << "]";
//}



} // namespace lz





#endif /* LZ_DEBUG_H_ */
