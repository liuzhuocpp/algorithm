/*
 * intermediate_representation.h
 *
 *  Created on: 2017年4月28日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_THREE_ADDRESS_INSTRUCTION_H_
#define LZ_COMPILER_C_THREE_ADDRESS_INSTRUCTION_H_

#include <vector>

namespace lz {

struct ThreeAddressInstruction
{
    std::string op, arg1, arg2, res;
//    int label = -1;// 可选的label, -1 表示无label， label是自增的
    ThreeAddressInstruction() = default;
//    ThreeAddressInstruction(std::string op, std::string arg1, std::string arg2, std::string res):
//        op(op), arg1(arg1), arg2(arg2), res(res){}

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const ThreeAddressInstruction& IR)
    {
        os << IR.op << " " << IR.arg1 << " " << IR.arg2 << " " << IR.res;
        return os;
    }

};

struct ThreeAddressCode: std::vector<ThreeAddressInstruction>
{
    using std::vector<ThreeAddressInstruction>::vector;

    std::set<int> labels;
    void clear() // must add this！
    {
        std::vector<ThreeAddressInstruction>::clear();
        labels.clear();
    }
    int nextInstructionIndex() const
    {
        return size();
    }

    void generateCode(std::string op, std::string arg1, std::string arg2, std::string res)
    {
        this->push_back({op, arg1, arg2, res});
    };

    template<typename Iterator>
    void backPatchLabel(Iterator first, Iterator last, int instructionId)
    {
        if(first != last)
            this->labels.insert(instructionId);
        for(;first != last; first ++)
        {
            (*this)[*first].res = "L" + std::to_string(instructionId);


        }

    }


    void generateGotoCode(int label)
    {
        generateCode("goto", "", "", "L" + std::to_string(label));

        labels.insert(label);
    }

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const ThreeAddressCode& table)
    {
        int maxWidth = 0;
        for(auto i : lz::irange(table.size()))
        {
            if(table.labels.count(i))
                maxWidth = std::max(maxWidth, (int)std::to_string(i).size());
        }

        constexpr const int paddingSpaceNumber = 3;
        for(auto i : lz::irange(table.size()))
        {

            if(!table.labels.count(i))
            {

                os << std::string(maxWidth + 2, ' '); // 2 for L :
            }
            else
            {
                std::string labelString = std::to_string(i);

                os << "L" << labelString << ":" << std::string(maxWidth - labelString.size(), ' ');
            }
            os  << std::string(paddingSpaceNumber, ' ') <<  table[i] << std::endl;
        }
        if(table.labels.count(table.size()))
        {
            os << "L" << table.size() << ":\n";
        }

        return os;
    }




};















} // namespace lz





#endif /* LZ_COMPILER_C_THREE_ADDRESS_INSTRUCTION_H_ */
