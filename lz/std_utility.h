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
 * Some extend utility function for std library
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






}// namesapce lz

#endif /* LZ_STD_UTILITY_H_ */
