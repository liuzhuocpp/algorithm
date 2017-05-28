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
    X(Unknown)\


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

//    enum class Category
//    {
//        Bool,
//        Int32,
//        Int64,
//        Float,
//        Double,
//
//        Array,
//
//        Empty,
//    };
private:
    Category m_category = Category::Empty;
    int m_offset = -1;
public:
    ThreeAddressInstructionArgumentType() = default;
    ThreeAddressInstructionArgumentType(Category category, int offset):
        m_category(category), m_offset(offset) {}

    Category category() const
    {
        return m_category;
    }

    int offset() const
    {
        return m_offset;
    }

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const ThreeAddressInstructionArgumentType& argType)
    {
//        switch(argType.m_category)
//        {
            os << categoryToName(argType.m_category) << "," << argType.m_offset;
//        default:
//            assert(0);
//        }
        return os;
    }


};



struct ThreeAddressInstructionArgument
{
    enum class Category
    {
        Variable, //存在于符号表中
        TempVariable, // 从0开始的
        Number,
        Label,
        Empty,

        True,
        False,
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

    static ThreeAddressInstructionArgument makeFalse()
    {
        return ThreeAddressInstructionArgument(Category::False);
    }

    static ThreeAddressInstructionArgument makeTrue()
    {
        return ThreeAddressInstructionArgument(Category::True);
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
        case Category::True:
            os << "True";
            break;
        case Category::False:
            os << "False";
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
    lz_name_to_enum(NameToCategory,Category, instruction_list, [](std::string){ return std::string();})


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
        os << argAndType.first << ":" << argAndType.second;
    }

    return os;
}

struct ThreeAddressCode: private std::vector<ThreeAddressInstruction>
{

//    X(beginFunc)\
//    X(endFunc)\
//    X(If)\
//    X(IfLess, "if<")\
//    X(IfMore, "if>")\
//    X(IfLessEqual, "if<=")\
//    X(IfMoreEqual, "if>=")\
//    X(IfEqual, "if==")\
//    X(IfNotEqual, "if!=")\
//    X(Goto, "goto")\
//\
//    X(Plus, "+")\
//    X(Minus, "-")\
//    X(Multiply, "*")\
//    X(Divide, "/")\
//    X(Assign, "=")\
//\
//    X(UnaryPlus, "plus")\
//    X(UnaryMinus, "minus")\
//\
//    X(ReadArray, "=[]")\
//    X(WriteArray, "[]=")\
//    X(Unknown)\

    // three address instruction argument type info map
    ThreeAddressInstructionArgumentTypeMap globalArgumentTypeMap;

    struct FunctionDefination
    {
        std::string functionName;
        int beginIndex, endIndex; // [beginIndex, endIndex) 这段区间的指令均属于  此函数
        ThreeAddressInstructionArgumentTypeMap argumentTypeMap;
    };

    std::vector<FunctionDefination> functionDefinations;



    using std::vector<ThreeAddressInstruction>::vector;
    using Argument = ThreeAddressInstructionArgument;
    using ArgumentType = ThreeAddressInstructionArgumentType;
private:
    std::set<int> labels;
    using Category = ThreeAddressInstruction::Category;

    void _generateCode(Category cate, Argument arg1, Argument arg2, Argument res)
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

    void beginFunction(std::string functionName)
    {
        functionDefinations.push_back(FunctionDefination{functionName, nextInstructionIndex(), -1});
        _generateCode(Category::beginFunc, Argument::makeEmpty(), Argument::makeEmpty(), Argument::makeEmpty());
    }

    void endFunction()
    {

        _generateCode(Category::endFunc, Argument::makeEmpty(), Argument::makeEmpty(), Argument::makeEmpty());
        functionDefinations.back().endIndex = nextInstructionIndex();
    }

    void clear() // must add this！
    {
        std::vector<ThreeAddressInstruction>::clear();
        labels.clear();
        functionDefinations.clear();
    }

    int nextInstructionIndex() const
    {
        return size();
    }


    void generateCode(Category cate, Argument arg1, Argument arg2, Argument res)
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
        _generateCode(Category::Goto, Argument::Category::Empty, Argument::Category::Empty, Argument::Category::Empty);
    }
    void generateGotoCode(int label)
    {
        _generateCode(Category::Goto, Argument::Category::Empty, Argument::Category::Empty,
            Argument(Argument::Category::Label, label));
        labels.insert(label);
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
                os << functionIterator->functionName << ":\n";
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
