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



template <class Char, class Traits, typename IteratorRange>
std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os,
           const IteratorRange&  range)
{
    auto first = begin(range), last = end(range);
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

#define OUT_FUNCTION_NAME cout << __FUNCTION__ << " ------------------------------------------------" << endl;
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
