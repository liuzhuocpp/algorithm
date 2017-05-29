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
#include <lz/compiler/c/global_identifier_table_pointer.h>

#define instruction_list(X)\
    X(beginFunc)\
    X(endFunc)\
    X(If)\
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
    X(Call)\
    X(Unknown)


#define argument_type_list(X) \
    X(Bool)\
    X(Int32)\
    X(Int64)\
    X(Float)\
    X(Double)\
    X(Array)\
    X(Empty)


namespace lz {



struct ThreeAddressInstructionArgumentType
{

    LZ_MAKE_NAMED_ENUM(Category, categoryToName, argument_type_list)

private:
    Category m_category = Category::Empty;
    int m_offset = -1;
    int m_width = -1;
public:
    ThreeAddressInstructionArgumentType() = default;
    ThreeAddressInstructionArgumentType(Category category, int offset):
        m_category(category), m_offset(offset) {}


    int getWidth() const
    {
        switch(this->m_category)
        {
        case Category::Bool: return 1;
        case Category::Int32: return 4;
        case Category::Int64: return 8;
        case Category::Array: return m_width;
        case Category::Float: return 4;
        case Category::Double: return 8;
        default:
            assert(0);
        }
        return -1;
    }


    Category category() const
    {
        return m_category;
    }

    int offset() const
    {
        return m_offset;
    }

    void setCategory(Category c)
    {
        m_category = c;
    }

    void setOffset(int offset)
    {
        m_offset = offset;
    }

    // 按照字节
    void setArrayWidth(int w)
    {
        assert(m_category == Category::Array);
        m_width = w;
    }


    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const ThreeAddressInstructionArgumentType& argType)
    {
        os << categoryToName(argType.m_category) << "," << argType.m_offset << "," << argType.getWidth();

        return os;
    }


};



struct ThreeAddressInstructionArgument
{
    // only  Variable, TempVariable have type
    enum class Category
    {
        Variable, //存在于符号表中
        TempVariable, // 从0开始的
        Number,
        Label,
        Empty,
    };

private:
    Category m_category = Category::Empty;
    unsigned index = -1;
public:

    bool operator<(ThreeAddressInstructionArgument o) const
    {
        if(m_category != o.m_category) return m_category < o.m_category;
        return index < o.index;
    }

    ThreeAddressInstructionArgument() {}
    ThreeAddressInstructionArgument(Category category):m_category(category) {}
    ThreeAddressInstructionArgument(Category category, unsigned value):m_category(category), index(value) {}

    static ThreeAddressInstructionArgument makeVariable(unsigned v)
    {
        return ThreeAddressInstructionArgument(Category::Variable, v);
    }

    static ThreeAddressInstructionArgument makeTempVariable(unsigned v)
    {
        return ThreeAddressInstructionArgument(Category::TempVariable, v);
    }

    static ThreeAddressInstructionArgument makeNumber(unsigned v)
    {
        return ThreeAddressInstructionArgument(Category::Number, v);
    }

    static ThreeAddressInstructionArgument makeLabel(unsigned v)
    {
        return ThreeAddressInstructionArgument(Category::Label, v);
    }

    static ThreeAddressInstructionArgument makeEmpty()
    {
        return ThreeAddressInstructionArgument(Category::Empty);
    }


    Category category() const
    {
        return m_category;
    }
    unsigned value() const
    {
        return index;
    }


    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const ThreeAddressInstructionArgument& arg)
    {
        switch(arg.m_category)
        {
        case Category::Variable:
            os << globalIdentifierTablePointer->identifier(arg.value());
            break;
        case Category::TempVariable:
            os << "$" << arg.value();
            break;
        case Category::Number:
            os << arg.value();
            break;
        case Category::Label:
            os << "L" << arg.value();
            break;
        case Category::Empty:
            os <<"";
            break;
        default:
            assert(0);
        }
        return os;
    }



};

struct ThreeAddressCode;
struct ThreeAddressInstruction
{
    friend struct ThreeAddressCode;
    LZ_MAKE_NAMED_ENUM(Category, Names, instruction_list);
    lz_name_to_enum(NameToCategory,Category, instruction_list, [](std::string i){ return i;})


private:
    Category category = Category::Unknown;

    using Argument = ThreeAddressInstructionArgument;
    Argument arg1, arg2, res;
public:


    ThreeAddressInstruction() = default;


    ThreeAddressInstruction(Category op, Argument arg1, Argument arg2, Argument res):
            category(op), arg1(arg1), arg2(arg2), res(res)
    {
        if(category >= Category::Unknown) assert(0);
    }

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const ThreeAddressInstruction& IR)
    {
        os << IR.Names(IR.category)  << " " << IR.arg1 << " " << IR.arg2 << " " << IR.res;
        return os;
    }

    static Category toIfRel(const std::string& rel)
    {
        auto optional = NameToCategory("if" + rel);
        if(optional.hasValue()) return optional.value();
        return Category::Unknown;
    }

    static Category toBinaryArithmetic(char op)
    {
        auto optional = NameToCategory(std::string(1, op));
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


using ThreeAddressInstructionArgumentTypeMap =
    std::map<ThreeAddressInstructionArgument, ThreeAddressInstructionArgumentType>;

template <class Char, class Traits>
std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& os,
           const ThreeAddressInstructionArgumentTypeMap& argTypeMap)
{
    for(auto argAndType: argTypeMap)
    {
        os << argAndType.first << ": " << argAndType.second << "\n";
    }

    return os;
}

struct ThreeAddressCode: private std::vector<ThreeAddressInstruction>
{
private:

    // three address instruction argument type info map
    ThreeAddressInstructionArgumentTypeMap globalArgumentTypeMap;

    struct FunctionDefination
    {
        int functionId;
        int beginIndex, endIndex; // [beginIndex, endIndex) 这段区间的指令均属于  此函数
        ThreeAddressInstructionArgumentTypeMap argumentTypeMap;
    };

    std::vector<FunctionDefination> functionDefinations;

    using std::vector<ThreeAddressInstruction>::vector;


    using Argument = ThreeAddressInstructionArgument;
    using ArgumentType = ThreeAddressInstructionArgumentType;
    std::set<int> labels;
    using Instuction = ThreeAddressInstruction;
    using InstuctionCategory = ThreeAddressInstruction::Category;

    void _generateCode(InstuctionCategory cate, Argument arg1, Argument arg2, Argument res)
    {
        emplace_back(cate, arg1, arg2, res);
    }

public:

    void addArgument(Argument arg, ArgumentType argType)
    {
        functionDefinations.back().argumentTypeMap[arg] = argType;
    }

    void addGlobalArgument(Argument arg, ArgumentType argType)
    {
        globalArgumentTypeMap[arg] = argType;
    }

    void beginFunction(int functionId)
    {
        functionDefinations.push_back(FunctionDefination{functionId, nextInstructionIndex(), -1});
        _generateCode(InstuctionCategory::beginFunc, Argument::makeEmpty(), Argument::makeEmpty(), Argument::makeEmpty());
    }

    void endFunction()
    {

        _generateCode(InstuctionCategory::endFunc, Argument::makeEmpty(), Argument::makeEmpty(), Argument::makeEmpty());
        functionDefinations.back().endIndex = nextInstructionIndex();
    }

    void clear() // must add this！
    {
        std::vector<ThreeAddressInstruction>::clear();
        labels.clear();
        functionDefinations.clear();
        globalArgumentTypeMap.clear();
    }

    void generateIfRelCode(std::string rel, Argument a, Argument b, int labelIndex)
    {
        _generateCode(Instuction::toIfRel(rel), a, b, Argument::makeLabel(labelIndex));
    }

    void generateIfRelCode(std::string rel, Argument a, Argument b)
    {
        _generateCode(Instuction::toIfRel(rel), a, b, Argument::makeEmpty());
    }

    // unary +， unary -
    void generateUnaryArithmeticCode(char op, Argument a, Argument res)
    {
        InstuctionCategory c;
        if(op == '+')
            c = InstuctionCategory::UnaryPlus;
        else if(op == '-')
            c = InstuctionCategory::UnaryMinus;
        else assert(0);

        _generateCode(c, a, Argument::makeEmpty(), res);
    }

    void generateBinaryArithmeticCode(char op, Argument a, Argument b, Argument res)
    {
        InstuctionCategory c;
        c = Instuction::toBinaryArithmetic(op);
        _generateCode(c, a, b, res);
    }

    void generateAssignCode(Argument a, Argument res)
    {
        _generateCode(InstuctionCategory::Assign, a, Argument::makeEmpty(), res);
    }

//arrayId 在identifierTable中
    void generateWriteArrayCode(Argument fromValue, Argument arrayOffset, int arrayId)
    {
        _generateCode(InstuctionCategory::WriteArray, fromValue, arrayOffset, Argument::makeVariable(arrayId));
    }

    void generateReadArrayCode(int arrayId, Argument arrayOffset, Argument toValue)
    {
        _generateCode(InstuctionCategory::ReadArray, Argument::makeVariable(arrayId), arrayOffset, toValue);
    }

    int nextInstructionIndex() const
    {
        return size();
    }

    template<typename Iterator>
    void backPatch(Iterator first, Iterator last, int instructionId)
    {
        if(first != last)
            this->labels.insert(instructionId);
        for(;first != last; first ++)
        {
            (*this)[*first].res =   Argument(Argument::Category::Label, instructionId);
        }
    }

    template<typename Container>
    void backPatch(const Container &c, int instructionId)
    {
        backPatch(std::begin(c), std::end(c), instructionId);
    }

    void generateGotoCode()
    {
        _generateCode(InstuctionCategory::Goto, Argument::Category::Empty, Argument::Category::Empty, Argument::Category::Empty);
    }
    void generateGotoCode(int label)
    {
        _generateCode(InstuctionCategory::Goto, Argument::Category::Empty, Argument::Category::Empty,
            Argument(Argument::Category::Label, label));
        labels.insert(label);
    }

    void generateCallCode(int v)
    {
        _generateCode(InstuctionCategory::Call, Argument::Category::Empty, Argument::Category::Empty, Argument::makeVariable(v));
    }




    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const ThreeAddressCode& table)
    {
        os << table.globalArgumentTypeMap << "\n";

        int maxWidth = 0;
        for(auto i : lz::irange(table.size()))
        {
            if(table.labels.count(i))
            {
                maxWidth = std::max(maxWidth, (int)std::to_string(i).size());
            }
        }

        if(table.size() == 0) return os;

        if(table.functionDefinations.empty())
        {
            assert(0);
        }

        auto functionIterator = table.functionDefinations.begin();

        constexpr int paddingSpaceNumber = 3;
        for(auto i : lz::irange(table.size()))
        {
            if(table.functionDefinations.end() == functionIterator)
            {
                assert(0);
            }



            if(functionIterator->beginIndex == i)
            {

                os << globalIdentifierTablePointer->identifier(functionIterator->functionId) << ":\n";
                os << functionIterator->argumentTypeMap << "\n";

            }
            if(functionIterator->endIndex == i + 1)
            {
                functionIterator ++;
            }



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
