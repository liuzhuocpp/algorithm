/*
 * intermediate_representation.h
 *
 *  Created on: 2017年4月28日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_THREE_ADDRESS_CODE_H_
#define LZ_COMPILER_C_THREE_ADDRESS_CODE_H_

#include <vector>

namespace lz {

struct ThreeAddressInstruction
{
    enum class Category
    {
#define X(cat, name) cat,
#include <lz/compiler/c/instruction.def>
#undef X
        Unknown,
    };


    static const std::unordered_map<std::string, Category> NameToCategory;
    static constexpr const char* Names[] =
    {

#define X(cat, name) name,
#include <lz/compiler/c/instruction.def>
#undef X
        "unknown",

    };


private:
    Category category = Category::Unknown;
    std::string  m_arg1, m_arg2, m_res;
//    arg1()
public:
    std::string& result()
    {
        return m_res;
    }


    ThreeAddressInstruction() = default;

    ThreeAddressInstruction(std::string op, std::string arg1, std::string arg2, std::string res):
        m_arg1(arg1), m_arg2(arg2), m_res(res)
    {
        if(auto it = NameToCategory.find(op); it != NameToCategory.end())
        {
            category = it->second;
        }
        else
        {
            assert(0);
        }

    }

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const ThreeAddressInstruction& IR)
    {
        os << IR.Names[static_cast<unsigned>(IR.category)] << " " << IR.m_arg1 << " " << IR.m_arg2 << " " << IR.m_res;
        return os;
    }

};


const std::unordered_map<std::string, ThreeAddressInstruction::Category>
    ThreeAddressInstruction::NameToCategory =
{


#define X(cat, name) {name, ThreeAddressInstruction::Category::cat},
#include <lz/compiler/c/instruction.def>
#undef X


};



struct ThreeAddressCode: private std::vector<ThreeAddressInstruction>
{
    using std::vector<ThreeAddressInstruction>::vector;

private:
    std::set<int> labels;
public:
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
        emplace_back(op, arg1, arg2, res);
    };

    template<typename Iterator>
    void backPatch(Iterator first, Iterator last, int instructionId)
    {
        if(first != last)
            this->labels.insert(instructionId);
        for(;first != last; first ++)
        {
            (*this)[*first].result() = "L" + std::to_string(instructionId);


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
            {
                maxWidth = std::max(maxWidth, (int)std::to_string(i).size());
            }
        }

        constexpr int paddingSpaceNumber = 3;
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





#endif /* LZ_COMPILER_C_THREE_ADDRESS_CODE_H_ */
