/*
 * adjacency_list_base.h
 *
 *  Created on: 2017年4月11日
 *      Author: LZ
 */

#ifndef LZ_ADJACENCY_LIST_BASE_H_
#define LZ_ADJACENCY_LIST_BASE_H_

#include <lz/iterator.h>
#include <lz/iterator_range.h>

namespace lz {

    namespace Detail
    {

    using DiffType = std::vector<std::size_t>::difference_type;
    constexpr DiffType NullIndex = -1;

    template<typename T>
    struct Node
    {
        DiffType next;
        T val;
        Node(DiffType next, T val): next(next), val(val){}
    };


    template<typename ALB, typename T>
    struct LocalIterator: public lz::IteratorFacade<LocalIterator<ALB, T>, std::forward_iterator_tag, T>
    {
    protected:
        ALB *alg = nullptr;
        DiffType i = NullIndex;
    public:
        LocalIterator() = default;
        LocalIterator(ALB& alg, DiffType i): alg(&alg), i(i){}

        LocalIterator& operator++()
        {
            i = alg->data[i].next;
            return *this;
        }

        friend bool operator==(LocalIterator a, LocalIterator b)
        {
            return a.i == b.i && a.alg == b.alg;
        }

    };


    } // Detail



template<typename T>
struct AdjacencyListBase
{
private:
    using DiffType = Detail::DiffType;
    using Node = Detail::Node<T>;
    static constexpr  auto NullIndex = Detail::NullIndex;

    friend Detail::LocalIterator<AdjacencyListBase, T>;
    friend Detail::LocalIterator<const AdjacencyListBase, T>;

    std::vector<std::size_t> head;
    std::vector<Detail::Node<T> > data;
public:
    using SizeType = std::size_t;
    using ValueType = T;

    AdjacencyListBase() = default;
    AdjacencyListBase(SizeType bucketCount): head(bucketCount, NullIndex) {}

    void clear()
    {
        head.clear();
        data.clear();
    }



    void addBucket()
    {
        head.push_back(NullIndex);
    }

    SizeType bucketCount() const
    {
        return head.size();
    }

    void insert(int i, T val)
    {
        data.push_back(Node(head[i], val));
        head[i] = data.size() - 1;
    }

    struct LocalIterator: public Detail::LocalIterator<AdjacencyListBase, T>
    {
        using Detail::LocalIterator<AdjacencyListBase, T>::LocalIterator;
        T& operator*()
        {
            return this->alg->data[this->i].val;
        }
    };

    struct ConstLocalIterator: public Detail::LocalIterator<const AdjacencyListBase, T>
    {
        using Detail::LocalIterator<const AdjacencyListBase, T>::LocalIterator;
        T& operator*()
        {
            return this->alg->data[this->i].val;
        }
    };

    IteratorRange<LocalIterator> locals(SizeType i)
    {
        return makeIteratorRange(
                LocalIterator(*this, head[i]),
                LocalIterator(*this, NullIndex));
    }

    IteratorRange<ConstLocalIterator> locals(SizeType i) const
    {
        return makeIteratorRange(
                ConstLocalIterator(*this, head[i]),
                ConstLocalIterator(*this, NullIndex));
    }

    struct Iterator: public IteratorAdapter<Iterator,
        typename std::vector<Node>::iterator, std::random_access_iterator_tag, T>
    {
        using IteratorAdapter<Iterator, typename std::vector<Node>::iterator,
            std::random_access_iterator_tag, T>::IteratorAdapter;
        T& operator*()
        {
            return this->base()->val;
        }
    };

    struct ConstIterator: public IteratorAdapter<Iterator,
        typename std::vector<Node>::const_iterator, std::random_access_iterator_tag, T>
    {
        using IteratorAdapter<Iterator, typename std::vector<Node>::const_iterator,
            std::random_access_iterator_tag, T>::IteratorAdapter;
        const T& operator*() const
        {
            return this->base()->val;
        }
    };

    Iterator begin()
    {
        return Iterator(data.begin());
    }

    ConstIterator begin() const
    {
        return ConstIterator(data.begin());
    }

    Iterator end()
    {
        return Iterator(data.end());
    }

    ConstIterator end() const
    {
        return ConstIterator(data.end());
    }



    using size_type = SizeType;
    using value_type = ValueType;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
};




} // namespace lz





#endif /* LZ_ADJACENCY_LIST_BASE_H_ */
