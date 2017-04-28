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
    int label = -1;// 可选的label, -1 表示无label， label是自增的
    IntermediateRepresentation() = default;
    IntermediateRepresentation(std::string op, std::string arg1, std::string arg2, std::string res):
        op(op), arg1(arg1), arg2(arg2), res(res){}

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const IntermediateRepresentation& IR)
    {
        os << op << " " << arg1 << " " << arg2 << " " << res;
        return os;
    }

};

struct IRTable: std::vector<IntermediateRepresentation>
{
    using std::vector<IntermediateRepresentation>::vector;




    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const IRTable& table)
    {
        int maxWidth = 0;
        for(auto IR: table)
        {
            maxWidth = std::max(maxWidth, std::to_string(IR.label).size());
        }

        for(auto IR: table)
        {
            if(maxWidth != 0)
            {
                auto label = std::to_string(IR.label);
                os << "L" << label << ":" << std::string(maxWidth - label.size(), ' ');
            }

            os << IR << std::endl;
        }

        return os;
    }




};















} // namespace lz





#endif /* LZ_COMPILER_C_INTERMEDIATE_REPRESENTATION_H_ */
