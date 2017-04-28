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


int rangeSplitterIndex()
{
    static int i = std::ios_base::xalloc();
    return i;
}


template <class Char, class Traits>
std::basic_ostream<Char, Traits>&
commaRangeSplitter(std::basic_ostream<Char, Traits>& os)
{
    os.iword(rangeSplitterIndex()) = 0; // =0 表示以逗号分割
    return os;
}

template <class Char, class Traits>
std::basic_ostream<Char, Traits>&
newLineRangeSplitter(std::basic_ostream<Char, Traits>& os)
{
    os.iword(rangeSplitterIndex()) = 1; // =1 表示以\n分割
    return os;
}

template <class Char, class Traits>
auto currentRangeSplitter(std::basic_ostream<Char, Traits>& os)
{
    switch(os.iword(rangeSplitterIndex()))
    {
    case 0:
        return commaRangeSplitter<Char, Traits>;
        break;
    case 1:
        return newLineRangeSplitter<Char, Traits>;
        break;
    default:
        assert(0);
    }

    return commaRangeSplitter<Char, Traits>; // never run to this;
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
        os << *first;

        // 判断rangeSpliter：
        switch(os.iword(rangeSplitterIndex()))
        {
        case 0:
            os << ",";
            break;
        case 1:
            os << "\n";
            break;
        case 2:
        default:
            os << ",";
        }


        ++first;
    }
    os << *first;
    return os << "]";
}




template <class Char, class Traits, typename T1, typename T2>
std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os,
           const std::pair<T1, T2>&  _pair)
{
    os << "std::pair(" << _pair.first << ", " << _pair.second << ")";
    return os;
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
