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
#include <lz/named_enum.h>


#define mutabel_lexical_symbol_list(X) \
    X(Identifier)\
    X(IntNumber)\
    X(DoubleNumber)


#define punctuation_list(X) \
    X(Plus, "+")\
    X(Minus, "-")\
    X(Asterisk, "*")\
    X(Slash, "/")\
\
    X(Ampersand, "&")\
    X(DoubleAmpersand, "&&")\
\
    X(VerticalBar, "|")\
    X(DoubleVerticalBar, "||")\
\
    X(Exclamation, "!")\
\
\
    X(LeftParenthese, "(")\
    X(RightParenthese, ")")\
    X(LeftSquareBracket, "[")\
    X(RightSquareBracket, "]")\
    X(LeftBrace, "{")\
    X(RightBrace, "}")\
\
    X(Semicolon, ";")\
\
    X(AssignMark, "=")\
    X(Equal, "==")\
\
\
    X(NotEqual, "!=")\
    X(Less, "<")\
    X(LessEqual, "<=")\
    X(More, ">")\
    X(MoreEqual, ">=")


#define keyword_list(X) \
    X(While)\
    X(If)\
    X(Else)\
    X(For)\
    X(Break)\
    X(Continue)\
    X(Return)\
\
    X(True)\
    X(False)\
\
\
    X(Bool)\
    X(Int)\
    X(Float)\
    X(Double)





#define allList(forEach) \
    mutabel_lexical_symbol_list(forEach)\
    punctuation_list(forEach)\
    keyword_list(forEach)

#define keywordAndPunctuationList(forEach) \
    punctuation_list(forEach) \
    keyword_list(forEach)


namespace lz {


struct LexicalSymbol
{

    LZ_MAKE_NAMED_ENUM(Category, categoryToName, allList)
#undef allList

    lz_name_to_enum(keywordToCategory, Category, keyword_list, LexicalSymbol::lowerFirstChar)

    lz_name_to_enum(punctuationToCategory, Category, punctuation_list, LexicalSymbol::lowerFirstChar)
//  使用复合列表会出错，以后改正，而且lz_name_to_enum 非常丑陋，需要进一步完善
//    lz_name_to_enum(keywordAndPunctuationToCategory, Category, keywordAndPunctuationList, LexicalSymbol::lowerFirstChar);

    friend bool operator<(const LexicalSymbol &a, const LexicalSymbol &b)
    {
        return a.m_category < b.m_category;
    }

//    static const std::unordered_map<std::string, Category> keywordToType;


private:
    Category m_category;
    std::string m_value;

    using ListRange = std::pair<Category, Category>;

    static bool isInPairRange(Category a, const std::pair<Category, Category> &range)
    {
        return a >= range.first && a <= range.second;
    }


public:
    static ListRange mutableLexicalSymbolRange() // can use constepxr lambda afterwards
    {
        return LZ_GET_ENUM_LIST_RANGE(Category, mutabel_lexical_symbol_list);
    }

    static ListRange keywordRange()
    {
        return LZ_GET_ENUM_LIST_RANGE(Category, keyword_list);
    }

    static ListRange punctuationRange()
    {
        return LZ_GET_ENUM_LIST_RANGE(Category, punctuation_list);
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
            return m_value;
        else
            return categoryToName(m_category);
    }



    LexicalSymbol(Category type, std::string value):m_category(type), m_value(value){}
    LexicalSymbol():m_category() {}

    LexicalSymbol(Category type): m_category(type){}
    LexicalSymbol(std::string s)
    {
        if(s[0] == '_' || islower(s[0]) || isupper(s[0]))
        {
            if(keywordToCategory(s).hasValue())
            {

                m_category = keywordToCategory(s).value();
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
            if(punctuationToCategory(s).hasValue())
            {
                m_category = punctuationToCategory(s).value();

            }
            else
            {
                assert(0);
            }
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

        const char *name = categoryToName(category);

        std::string ans;

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
        os << categoryToName(ls.m_category);
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


//const std::unordered_map<std::string, LexicalSymbol::Category> LexicalSymbol::keywordToType =
//{
//
//#define X1(keyword) {LexicalSymbol::lowerFirstChar(#keyword), Category::keyword},
//#define X2(key, name) {name, Category::key},
//
//        punctuation_list(X2)
//        keyword_list(X1)
//
//#undef X1
//#undef X2
//
//};


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
        auto cntRex = LexicalSymbol::regex(static_cast<LexicalSymbol::Category>(i));

//        std::cout << cntRex << " FFFrex" << std::endl;
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

    for(unsigned i: irange(puctuationBegin, punctuationEnd + 1))
    {
        regexAndFuncsPushBack(i);
    }

    for(unsigned i: irange(mutableBegin, mutableEnd + 1))
    {
        regexAndFuncsPushBack(i);
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
