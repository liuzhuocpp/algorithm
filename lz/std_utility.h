/*
 * std_utility.h
 *
 *  Created on: 2016年12月9日
 *      Author: LZ
 */

#ifndef LZ_STD_UTILITY_H_
#define LZ_STD_UTILITY_H_


/*
 *
 * Some utility functions for std library
 */

namespace std {


template<typename InputIterator>
InputIterator begin(std::pair<InputIterator, InputIterator> itPair)
{
    return itPair.first;
}

template<typename InputIterator>
InputIterator end(std::pair<InputIterator, InputIterator> itPair)
{
    return itPair.second;
}


template<typename RandomIterator>
void out(RandomIterator first, RandomIterator last)
{
    cout << "[";
    while(first != last)
    {
        cout << *first << ",";
        first++;
    }
    cout << "]";
}





}// namesapce lz

#endif /* LZ_STD_UTILITY_H_ */
