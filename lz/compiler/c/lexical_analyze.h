/*
 * lexical_analyze.h
 *
 *  Created on: 2017年4月18日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_LEXICAL_ANALYZE_H_
#define LZ_COMPILER_C_LEXICAL_ANALYZE_H_


#include <lz/compiler/regex.h>
#include <lz/compiler/simulate_nfa.h>


#define X_OVERLOAD_DISPATCHER(_1, _2, NAME, ... ) NAME
#define X(...) X_OVERLOAD_DISPATCHER(__VA_ARGS__, X2, X1)(__VA_ARGS__)


namespace lz {


struct LexicalSymbol
{

    enum class Category: unsigned
    {

#define X1(key) key,
#define X2(key, name) key,
#include <lz/compiler/c/mutable_lexical_symbol_list.def>
#include <lz/compiler/c/punctuation_list.def>
//        End,
#include <lz/compiler/c/keyword_list.def>
#undef X1
#undef X2

    };

    static constexpr const char * names[] = {
#define X1(key) #key,
#define X2(key, name) name,
#include <lz/compiler/c/mutable_lexical_symbol_list.def>
#include <lz/compiler/c/punctuation_list.def>
//        "end",
#include <lz/compiler/c/keyword_list.def>
#undef X1
#undef X2

    };


    friend bool operator<(const LexicalSymbol &a, const LexicalSymbol &b)
    {
        return a.m_category < b.m_category;
    }

    static const
    std::unordered_map<std::string, Category> keywordToType;


private:
    Category m_category;
    std::string m_value;


//    static std::tuple<std::pa>
     static  const std::array<std::pair<Category, Category>, 3> categoryRanges;

//    static std::pair<Category, Category> m_mutableLexicalSymbolRange;
//    static std::pair<Category, Category> m_keywordRange;
//    static std::pair<Category, Category> m_punctuationRange;

    static std::array<std::pair<Category, Category>, 3> cal()
    {

#define CALCULATE_RANGE_DEF   bool beginInit;   Category begin, end;\
    std::pair<Category, Category> \
        m_mutableLexicalSymbolRange,\
        m_keywordRange,\
        m_punctuationRange;

#define CALCULATE_RANGE_INIT    beginInit = false;
#define CALCULATE_RANGE_ASSIGN(name) name = std::make_pair(begin, end);


#define X1(key) if(!beginInit) begin = Category::key, beginInit = true; end = Category::key;
#define X2(key, name) X1(key)


CALCULATE_RANGE_DEF

CALCULATE_RANGE_INIT
#include <lz/compiler/c/mutable_lexical_symbol_list.def>
CALCULATE_RANGE_ASSIGN(m_mutableLexicalSymbolRange)

CALCULATE_RANGE_INIT
#include <lz/compiler/c/keyword_list.def>
CALCULATE_RANGE_ASSIGN(m_keywordRange)

CALCULATE_RANGE_INIT
#include <lz/compiler/c/punctuation_list.def>
CALCULATE_RANGE_ASSIGN(m_punctuationRange)

#undef X1
#undef X2
#undef CALCULATE_RANGE_DEF
#undef CALCULATE_RANGE_INIT
#undef CALCULATE_RANGE_ASSIGN


        return {m_mutableLexicalSymbolRange, m_keywordRange, m_punctuationRange};
    }

    static bool isInPairRange(Category a, const std::pair<Category, Category> &range)
    {
        return a >= range.first && a <= range.second;
    }


public:
    static const auto& mutableLexicalSymbolRange()
    {
        return categoryRanges[0];
//        return m_mutableLexicalSymbolRange;
    }

    static const auto& keywordRange()
    {
        return categoryRanges[1];
//        return m_keywordRange;
    }

    static const auto& punctuationRange()
    {
        return categoryRanges[2];
//        return m_punctuationRange;
    }

    static bool isMutableLexicalSymbol(Category category)
    {
        return isInPairRange(category, mutableLexicalSymbolRange());
    }
    static bool isKeyword(Category category)
    {
        return isInPairRange(category, keywordRange());
    }

    static bool isPunctuation(Category category)
    {
        return isInPairRange(category, punctuationRange());
    }

    Category category() const
    {
        return m_category;
    }
    std::string value() const
    {
        if(isMutableLexicalSymbol(m_category))
//        if(m_category == Category::Identifier || m_category == Category::IntNumber)
            return m_value;
        else
            return names[static_cast<unsigned>(m_category)];
    }



    LexicalSymbol(Category type, std::string value):m_category(type), m_value(value){}
    LexicalSymbol() = default;
    LexicalSymbol(Category type): m_category(type){}
    LexicalSymbol(std::string s)
    {
        if(s[0] == '_' || islower(s[0]) || isupper(s[0]))
        {
            if(keywordToType.count(s))
            {
                m_category = keywordToType.at(s);
                m_value = s; // can be improved
            }
            else
            {
                m_category = Category::Identifier;
                m_value = s;
            }

        }
        else if(isdigit(s[0]))
        {
            if(s.find('.') != std::string::npos)
            {
                m_category = Category::DoubleNumber;
            }
            else
            {
                m_category = Category::IntNumber;
            }
            m_value = s;
        }
        else
        {
            if(keywordToType.count(s))
            {
                m_category = keywordToType.at(s);
            }
            else
            {
                assert(0);
            }
//            for(int i = static_cast<int>(Category::IntNumber) + 1; i < static_cast<int>(Category::End); ++ i)
//            {
//                if(s == names[i])
//                {
//                    m_category = static_cast<Category>(i);
//                    return ;
//                }
//            }

        }
    }
    LexicalSymbol(const char * s):LexicalSymbol(std::string(s))
    {

    }



    static std::string regex(Category category)
    {
        if(category == Category::Identifier)
        {
            return "(_|[a-zA-Z])(_|[a-zA-Z0-9])*";
        }
        else if(category == Category::IntNumber)
        {
            return "([0-9][0-9]*)";
        }
        else if(category == Category::DoubleNumber)
        {
            return "([0-9][0-9]*\\.[0-9][0-9]*)";
        }
        std::string ans;
        const char * name = names[static_cast<unsigned>(category)];
        for(int i = 0; name[i] != '\0'; ++ i)
        {
            ans.push_back('\\');
            ans.push_back(name[i]);

        }
        return ans;
    }

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const LexicalSymbol&  ls)
    {
        os << names[static_cast<int>(ls.m_category)];



        if(isMutableLexicalSymbol(ls.m_category))
        {

            os << "(" << ls.m_value << ")";
        }
        return os;

    }

private:
    // Can use this to improve it
    // http://stackoverflow.com/questions/40945727/convert-a-string-to-uppercase-at-compile-time

    static std::string lowerFirstChar(std::string s)
    {

        s[0] = std::tolower(s[0]);
        return s;
    }
};

const std::array<std::pair<LexicalSymbol::Category, LexicalSymbol::Category>, 3>
    LexicalSymbol::categoryRanges = LexicalSymbol::cal();
//std::pair<LexicalSymbol::Category, LexicalSymbol::Category> LexicalSymbol::m_keywordRange;
//std::pair<LexicalSymbol::Category, LexicalSymbol::Category> LexicalSymbol::m_mutableLexicalSymbolRange;


const std::unordered_map<std::string, LexicalSymbol::Category> LexicalSymbol::keywordToType =
{

#define X1(keyword) {LexicalSymbol::lowerFirstChar(#keyword), Category::keyword},
#define X2(key, name) {name, Category::key},

#include <lz/compiler/c/punctuation_list.def>
#include <lz/compiler/c/keyword_list.def>

#undef X1
#undef X2

};


template<typename Iterator>
auto lexicalAnalyze(Iterator textBegin, Iterator textEnd)
{
    using T = char;
    using NFA = AdjacencyList<DirectedGraphTag, NoProperty, Property<EdgeWeightTag, T> >;


    vector<LexicalSymbol> ans;
    NFA nfa;

    std::vector<std::pair<std::string, std::function<void(Iterator, Iterator)>>>
            regexAndFuncs;

    auto func = [&](auto first, auto last) {
        ans.push_back(LexicalSymbol(std::string(first, last)));
    };

    auto punctuationRange = LexicalSymbol::punctuationRange();
    auto mutableLexicalSymbolRange = LexicalSymbol::mutableLexicalSymbolRange();

    auto regexAndFuncsPushBack = [&](unsigned i) {
//        std::cout << "JJJJ " << i << std::endl;
        auto cntRex = LexicalSymbol::regex(static_cast<LexicalSymbol::Category>(i));

        std::cout << cntRex << " FFFrex" << std::endl;
        regexAndFuncs.push_back(
            std::make_pair(
                LexicalSymbol::regex(static_cast<LexicalSymbol::Category>(i)), func)  ) ;
    };

    unsigned
        puctuationBegin = static_cast<unsigned>(punctuationRange.first),
        punctuationEnd = static_cast<unsigned>(punctuationRange.second);

    unsigned
        mutableBegin = static_cast<unsigned>(mutableLexicalSymbolRange.first),
        mutableEnd = static_cast<unsigned>(mutableLexicalSymbolRange.second);

//    std::cout << (puctuationBegin == punctuationEnd) << "HHH" << std::endl;
    for(unsigned i: irange(puctuationBegin, punctuationEnd + 1))
    {
        regexAndFuncsPushBack(i);
    }

    for(unsigned i: irange(mutableBegin, mutableEnd + 1))
    {
        regexAndFuncsPushBack(i);
    }
//
//    for(unsigned i: irange(static_cast<unsigned>(LexicalSymbol::Category::End) ) )
//    {
//        regexAndFuncs.push_back(
//            std::make_pair(
//                LexicalSymbol::regex(static_cast<LexicalSymbol::Category>(i)),
//                func)
//        );
//    }
//
    regexAndFuncs.push_back({"&&&&*", [&](auto first, auto last) {
        assert(0);
    }});
    regexAndFuncs.push_back({R"(\|\|\|\|*)", [&](auto first, auto last) {
        assert(0);
    }});

    regexAndFuncs.push_back({R"(\=\=\=\=*)", [&](auto first, auto last) {
        assert(0);
    }});

    regexAndFuncs.push_back({"  *", [&](auto first, auto last) {

    }});

    regexAndFuncs.push_back({"\n\n*", [&](auto first, auto last) {
    }});




    auto startAndVertexToFunc =

            parseMultiRegex(nfa, regexAndFuncs);


    auto start = std::get<0>(startAndVertexToFunc);
    auto vertexToFunc = std::get<1>(startAndVertexToFunc);



    for(Iterator cntEnd;textBegin != textEnd; textBegin = cntEnd)
    {
        cntEnd = simulateNFA(textBegin, textEnd, nfa, start, vertexToFunc);

        if(cntEnd == textBegin)
        {
            cout << "Unrecognized: " << std::string(textBegin, textEnd) << endl;
            assert(0);
        }
    }

    return ans;
}

} // namespace lz






















#undef X
#undef X_OVERLOAD_DISPATCHER



















//    constexpr static const char* names[] = {
//        "identifier",
//        "integer",
//        "+",
//        "-",
//        "*",
//        "/",
//        "&",
//        "&&",
//        "|",
//        "||",
//        "!",
//        "(",
//        ")",
//        "[",
//        "]",
//        "{",
//        "}",
//        ";",
//        "=",
//        "==",
//
//
//
//        "!=",
//        "<",
//        "<=",
//        ">",
//        ">=",
//
//
//
//    };
//
//    constexpr static const char* regexs[] = {
//        "(_|[a-zA-Z])(_|[a-zA-Z0-9])*",
//        "([0-9][0-9]*)",
//        R"(\+)",
//        R"(\-)",
//        R"(\*)",
//        R"(\/)",
//        R"(\&)",
//        R"(\&\&)",
//        R"(\|)",
//        R"(\|\|)",
//        R"(\!)",
//        R"(\()",
//        R"(\))",
//        R"(\[)",
//        R"(\])",
//        R"(\{)",
//        R"(\})",
//        R"(\;)",
//        R"(\=)",
//        R"(\=\=)",
//
//
//        R"(\!\=)",
//        R"(\<)",
//        R"(\<\=)",
//        R"(\>)",
//        R"(\>\=)",
//
//
//
//
//
//    };



#endif /* LZ_COMPILER_C_LEXICAL_ANALYZE_H_ */
