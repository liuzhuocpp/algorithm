/*
 * intermediate_representation.h
 *
 *  Created on: 2017年4月28日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_INTERMEDIATE_REPRESENTATION_H_
#define LZ_COMPILER_C_INTERMEDIATE_REPRESENTATION_H_

#include <vector>

namespace lz {

struct IntermediateRepresentation
{
    std::string op, arg1, arg2, res;
    int label = -1;// 可选的label, -1 表示无label
    IntermediateRepresentation() = default;
    IntermediateRepresentation(std::string op, std::string arg1, std::string arg2, std::string res):
        op(op), arg1(arg1), arg2(arg2), res(res){}


};

struct IRTable: std::vector<IntermediateRepresentation>
{
    using std::vector<IntermediateRepresentation>::vector;




    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const IRTable& IR)
    {

        return os;
    }




};















} // namespace lz





#endif /* LZ_COMPILER_C_INTERMEDIATE_REPRESENTATION_H_ */
