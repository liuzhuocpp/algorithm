/*
 * sparse_matrix.h
 *
 *  Created on: 2017年3月7日
 *      Author: LZ
 */

#ifndef LZ_SPARSE_MATRIX_H_
#define LZ_SPARSE_MATRIX_H_

#include <vector>

namespace lz{


// 基于Trie树的三数组思想，还可以进一步优化


template<typename T>
struct SparseMatrix
{
    using DiffType = typename std::vector<T>::difference_type;
    std::vector<std::pair<T, DiffType> > pool;

    std::vector<DiffType> base;

//    std::vector<DiffType> head;
//    std::vector<DiffType> next;
    DiffType m;
    SparseMatrix(DiffType n, DiffType m): base(n, -1),m(m)
    {

    }

    std::pair<T, bool> getValue(DiffType i, DiffType j)
    {
        DiffType t = base[i] + j;
        if(base[i] != -1 && t < pool.size() && pool[t].second == i)
        {
            return std::make_pair(pool[t].first, 1);
        }
        else
        {
            return std::make_pair(T(), 0);
        }
    }
    void setValue(DiffType i, DiffType j, const T& val)
    {
        DiffType poolSize = pool.size();
        if(base[i] == -1)
        {
            DiffType newBegin = findNewBegin(i, j, {});
            if(newBegin + j >= pool.size())
            {
                resize(newBegin + j + 1);
            }
            pool[newBegin + j].first = val;
            pool[newBegin + j].second = i;

            base[i] = newBegin;

        }
        else if(base[i] + j >= poolSize)
        {
            resize(base[i] + j + 1);
            pool[base[i] + j].first = val;
            pool[base[i] + j].second = i;
        }
        else if(pool[base[i] + j].second == -1 || pool[base[i] + j].second == i)
        {
            pool[base[i] + j].first = val;
            pool[base[i] + j].second = i;
        }
        else
        {
            std::vector<DiffType> cols;
            for(DiffType c = 0; c < m; ++ c)
            {
                if(base[i] + c < poolSize && pool[base[i] + c].second == i )
                {
                    cols.push_back(c);
                }
            }
            DiffType newBegin = findNewBegin(i, j, cols);
            if(newBegin + j >= pool.size())
            {
                resize(newBegin + j + 1);
            }

            relocate(i, newBegin, cols);
            pool[newBegin + j].first = val;
            pool[newBegin + j].second = i;
            base[i] = newBegin;
        }

    }
private:
    void resize(DiffType newSize)
    {
        DiffType oldSize = pool.size();
        pool.resize(newSize);
        std::fill(pool.begin() + oldSize, pool.end() - 1, std::make_pair(T(), -1));
    }
    void relocate(DiffType i, DiffType newBegin, const std::vector<DiffType>& cols)
    {
        for(auto c : cols)
        {
            DiffType t = newBegin + c;
            pool[t].first = pool[base[i] + c].first;
            pool[t].second = i;
            pool[base[i] + c].second = -1;
        }

    }

    DiffType findNewBegin(DiffType i, DiffType j, const std::vector<DiffType>& cols)
    {
        DiffType newBegin = base[i] + 1;
        for( ; ;newBegin ++)
        {
            if(checkNewBegin(newBegin, i, j, cols)) return newBegin;
        }
        // assert(0);
    }
    bool checkNewBegin(DiffType newBegin, DiffType i,  DiffType j, std::vector<DiffType> cols)
    {
        cols.push_back(j);
        for(auto c: cols)
        {
            DiffType t = newBegin + c;
            if(t >= pool.size()) return 1;
            if(pool[t].second != -1 && pool[t].second != i)
            {
                return 0;
            }
        }


        return 1;
    }






};











}// namesapce lz

#endif /* LZ_SPARSE_MATRIX_H_ */
