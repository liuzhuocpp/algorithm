/*
 * iterator_range.h
 *
 *  Created on: 2017年1月13日
 *      Author: LZ
 */

#ifndef LZ_ITERATOR_RANGE_H_
#define LZ_ITERATOR_RANGE_H_


#include <utility>
#include <iterator>
#include <lz/iterator.h>

namespace lz {



template<typename Iterator>
struct IteratorRange: public std::pair<Iterator, Iterator>
{
    Iterator begin() const
    {
        return this->first;
    }
    Iterator end() const
    {
        return this->second;
    }
    IteratorRange() = default;
    IteratorRange(const Iterator& first, const Iterator& second):
        std::pair<Iterator, Iterator>(first, second) {}

};

template<typename Iterator>
IteratorRange<Iterator> makeIteratorRange(Iterator first, Iterator second)
{
    return IteratorRange<Iterator>(first, second);
}



    namespace IteratorRangePrivate {

    template<typename StepIteratorRange, typename RandomIterator>
    struct StepIterator: IteratorAdapter<
        StepIterator<StepIteratorRange, RandomIterator>,
        RandomIterator>
    {
    private:
        using DifferenceType = typename std::iterator_traits<RandomIterator>::difference_type;
        using Adapter = IteratorAdapter<StepIterator, RandomIterator>;
        const StepIteratorRange * range = nullptr;
    public:
        StepIterator() = default;
        StepIterator(const StepIteratorRange& range, RandomIterator it):
            range(&range), Adapter(it)
        {

        }

        typename std::iterator_traits<RandomIterator>::reference operator*() const
        {
            return *(  range->first + (this->base() - range->first) * range->step );
        }
    };


    } // namespace IteratorRangePrivate

template<typename RandomIterator>
struct StepIteratorRange
{
    using DifferenceType = typename std::iterator_traits<RandomIterator>::difference_type;
    DifferenceType step;
    RandomIterator first, last;
    using _StepIterator = IteratorRangePrivate::StepIterator<StepIteratorRange, RandomIterator>;

public:
    StepIteratorRange() = default;
    StepIteratorRange(RandomIterator first, RandomIterator last, DifferenceType step = 1):
        first(first), last(first + (last - first) / step + bool((last - first) % step) ), step(step) { }


    _StepIterator begin() const
    {
        return _StepIterator(*this, first);
    }
    _StepIterator end() const
    {
        return _StepIterator(*this, last);
    }
};




template<typename Integer>
struct IntegerIteratorRange: StepIteratorRange<IntegerIterator<Integer> >
{
    using DifferenceType = typename std::iterator_traits<IntegerIterator<Integer>>::difference_type;
    IntegerIteratorRange(Integer first, Integer last, DifferenceType step = 1):
        StepIteratorRange<IntegerIterator<Integer> >(
                IntegerIterator<Integer>(first),
                IntegerIterator<Integer>(last),
                step) {}
    IntegerIteratorRange() = default;
};

template<typename Integer>
IntegerIteratorRange<Integer> irange(Integer n)
{
    return IntegerIteratorRange<Integer>(0, n, 1);
}

template<typename Integer>
IntegerIteratorRange<Integer> irange(Integer first, Integer last, Integer step = 1)
{
    return IntegerIteratorRange<Integer>(first, last, step);
}







} // namespace lz





#endif /* LZ_ITERATOR_RANGE_H_ */
