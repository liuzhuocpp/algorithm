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


namespace lz {


struct LexicalSymbol
{

    enum class Category: unsigned
    {
        Identifier,
        IntNumber,
        DoubleNumber,

#define X(name, punctuation) name,
#include <lz/compiler/c/punctuation_list.def>
#undef X
        End,
#define X(name) name,
#include <lz/compiler/c/keyword_list.def>
#undef X
    };

    static constexpr const char * names[] = {
            "Identifier",
            "IntNumber",
            "DoubleNumber",

#define X(punctuationName, punctuation) punctuation,
#include <lz/compiler/c/punctuation_list.def>
#undef X
            "end",

#define X(keyword) #keyword,
#include <lz/compiler/c/keyword_list.def>
#undef X


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
public:
    Category category() const
    {
        return m_category;
    }
    std::string value() const
    {
        if(m_category == Category::Identifier || m_category == Category::IntNumber)
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
            for(int i = static_cast<int>(Category::IntNumber) + 1; i < static_cast<int>(Category::End); ++ i)
            {
                if(s == names[i])
                {
                    m_category = static_cast<Category>(i);
                    return ;
                }
            }
            assert(0);
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



        if(ls.m_category == Category::Identifier ||
            ls.m_category == Category::IntNumber ||
            ls.m_category == Category::DoubleNumber)
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



const std::unordered_map<std::string, LexicalSymbol::Category> LexicalSymbol::keywordToType =
{

#define X(keyword) {LexicalSymbol::lowerFirstChar(#keyword), Category::keyword},
#include <lz/compiler/c/keyword_list.def>
#undef X


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

    for(unsigned i: irange(static_cast<unsigned>(LexicalSymbol::Category::End) ) )
    {
        regexAndFuncs.push_back(
            std::make_pair(
                LexicalSymbol::regex(static_cast<LexicalSymbol::Category>(i)),
                func)
        );
    }

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
