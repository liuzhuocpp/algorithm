/*
 * intermediate_representation.h
 *
 *  Created on: 2017年4月28日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_THREE_ADDRESS_CODE_H_
#define LZ_COMPILER_C_THREE_ADDRESS_CODE_H_

#include <vector>
#include <lz/named_enum.h>

#define instruction_list(X)\
    X(If, "if")\
    X(IfLess, "if<")\
    X(IfMore, "if>")\
    X(IfLessEqual, "if<=")\
    X(IfMoreEqual, "if>=")\
    X(IfEqual, "if==")\
    X(IfNotEqual, "if!=")\
    X(Goto, "goto")\
\
    X(Plus, "+")\
    X(Minus, "-")\
    X(Multiply, "*")\
    X(Divide, "/")\
    X(Assign, "=")\
\
    X(UnaryPlus, "plus")\
    X(UnaryMinus, "minus")\
\
    X(ReadArray, "=[]")\
    X(WriteArray, "[]=")\
    X(Unknown)\




namespace lz {

struct ThreeAddressInstructionArgument
{
    enum class Category
    {
        Variable, //存在于符号表中
        TempVariable, // 从0开始的
        Number,
        Label,
        Unknown,

    };

private:
    Category m_category = Category::Unknown;
    unsigned index = -1;
public:
    ThreeAddressInstructionArgument() {}
    ThreeAddressInstructionArgument(Category category):m_category(category) {}

    void setVariableIndex(int variableId)
    {
        assert(m_category == Category::Variable);
        index = variableId;
    }

    int getVariableIndex() const
    {
        assert(m_category == Category::Variable);
        return index;
    }

    void setTempVariableIndex(int id)
    {
        assert(m_category == Category::TempVariable);
        index = id;
    }

    int gettTempVariableIndex() const
    {
        assert(m_category == Category::TempVariable);
        return index;
    }

    void setNumber(int number)
    {
        assert(m_category == Category::Number);
        index = number;
    }

    int getNumber() const
    {
        assert(m_category == Category::Number);
        return index;
    }

    void setLabel(int label)
    {
        assert(m_category == Category::Label);
        index = label;
    }
    int getLabel() const
    {
        assert(m_category == Category::Label);
        return index;
    }

};

struct ThreeAddressCode;
struct ThreeAddressInstruction
{
    friend struct ThreeAddressCode;
    LZ_MAKE_NAMED_ENUM(Category, Names, instruction_list);
    lz_name_to_enum(NameToCategory,Category, instruction_list, [](std::string){ return std::string();})


private:
    Category category = Category::Unknown;
    std::string  m_arg1, m_arg2, m_res;

    ThreeAddressInstructionArgument arg1, arg2, res;
public:
//    std::string& result()
//    {
//        return m_res;
//    }
//

    ThreeAddressInstruction() = default;


    ThreeAddressInstruction(Category op, std::string arg1, std::string arg2, std::string res):
            category(op), m_arg1(arg1), m_arg2(arg2), m_res(res)
    {
        if(category >= Category::Unknown) assert(0);
    }

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const ThreeAddressInstruction& IR)
    {
        os << IR.Names(IR.category)  << " " << IR.m_arg1 << " " << IR.m_arg2 << " " << IR.m_res;
        return os;
    }

    static Category toIfRel(const std::string& rel)
    {
        auto optional = NameToCategory("if" + rel);
        if(optional.hasValue()) return optional.value();


        return Category::Unknown;
    }

    static Category toCategory(const std::string& cate)
    {
        auto optional = NameToCategory(cate);
        if(optional.hasValue()) return optional.value();
        return Category::Unknown;
    }


    static bool isIfRel(Category cate)
    {
        return cate >= Category::IfLess && cate <= Category::IfNotEqual;
    }






};




struct ThreeAddressCode: private std::vector<ThreeAddressInstruction>
{
    using std::vector<ThreeAddressInstruction>::vector;

private:
    std::set<int> labels;
    using Category = ThreeAddressInstruction::Category;

    void _generateCode(Category cate, std::string arg1, std::string arg2, std::string res)
    {
        emplace_back(cate, arg1, arg2, res);
    };
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


    void generateCode(Category cate, std::string arg1, std::string arg2, std::string res)
    {
        if(cate == Category::Goto) // I know this ugly, but I have not known how to do better now.
        {
            std::cout << "Please use generateGotoCode()\n";
            assert(0);
        }
        _generateCode(cate, arg1, arg2, res);
    }


    template<typename Iterator>
    void backPatch(Iterator first, Iterator last, int instructionId)
    {
        if(first != last)
            this->labels.insert(instructionId);
        for(;first != last; first ++)
        {
            (*this)[*first].m_res = "L" + std::to_string(instructionId);


        }

    }

    void generateGotoCode()
    {
        _generateCode(Category::Goto, "", "", "-");
    }
    void generateGotoCode(int label)
    {
        _generateCode(Category::Goto, "", "", "L" + std::to_string(label));
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
