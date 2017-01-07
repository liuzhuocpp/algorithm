/*
 * sparse_table_range_minimum_query.h
 *
 *  Created on: 2017年1月4日
 *      Author: LZ
 */

#ifndef LZ_SPARSE_TABLE_RANGE_MINIMUM_QUERY_H_
#define LZ_SPARSE_TABLE_RANGE_MINIMUM_QUERY_H_

#include <iterator>
#include <functional>
#include <lz/parameter.h>

namespace lz {

    namespace SparseTableRangeMinimumQueryKeywords {

    LZ_PARAMETER_KEYWORD(tag, ceilLog2)
    LZ_PARAMETER_KEYWORD(tag, floorLog2)

    }


// [begin, end)
template<typename RandomIterator,
         typename Less = std::less<typename std::iterator_traits<RandomIterator>::value_type>,
         typename Params = EmptyParamPack>

auto sparseTableRangeMinimumQuery(RandomIterator begin, RandomIterator end,
     Less less = Less(),
     const Params& params = Params())
{
    using IndexType = typename std::iterator_traits<RandomIterator>::difference_type;
    using ValueType = typename std::iterator_traits<RandomIterator>::value_type;
    auto ceilLog2 = params[SparseTableRangeMinimumQueryKeywords::ceilLog2 |
                           [](IndexType x) { return  (IndexType)std::ceil(std::log2(x)); } ];

    auto floorLog2 = params[SparseTableRangeMinimumQueryKeywords::floorLog2 |
                               [](IndexType x) { return  (IndexType)std::log2(x); } ];

    IndexType n = end - begin;
    IndexType powerNumber = ceilLog2(n);
    std::vector<std::vector<ValueType>> data(n, std::vector<ValueType>(powerNumber, 0));

    for(IndexType i = 0; i < n - 1; ++ i)
    {
        data[i][0] = std::min(begin[i], begin[i + 1], less);
    }
    for(IndexType j = 1; j < powerNumber; ++ j)
    {
        for(IndexType i = 0; (i + (1 << (j - 1))) < n; ++ i)
        {
            data[i][j] = std::min(data[i][j - 1], data[i + (1 << (j - 1))][j - 1], less);
        }
    }

    return   // [i, j)
            [=, data = std::move(data)](IndexType i, IndexType j)
//                ->const ValueType&
            //TODO will fix here
                ->ValueType
           {
                IndexType len = j - i;
                if(len == 1)
                {
                    return begin[i];
                }
                IndexType k = floorLog2(len);
                return std::min(data[i][k - 1], data[j - (1 << (k - 1))][k - 1], less);
            };





}






}// namesapce lz

#endif /* LZ_SPARSE_TABLE_RANGE_MINIMUM_QUERY_H_ */
