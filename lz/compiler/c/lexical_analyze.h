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
        Integer,

        Plus,
        Minus,
        Asterisk,
        Slash,

        Ampersand,
        DoubleAmpersand,

        VerticalBar,
        DoubleVerticalBar,

        ExclamationMark,


        LeftParenthese,
        RightParenthese,
        LeftSquareBracket,
        RightSquareBracket,
        LeftBrace,
        RightBrace,

        Semicolon,

        AssignMark,
        Equal,


        NotEqual,
        Less,
        LessEqual,
        More,
        MoreEqual,



        End,






        While,
        If,
        Else,
        For,
        Break,
        Continue,
        Return,

        True,
        False,



        Int,
        Float,
        Double,


    };
;
    friend bool operator<(const LexicalSymbol &a, const LexicalSymbol &b)
    {
        return a.type < b.type;
    }

    static const
    std::map<std::string, Category> keywordToType;

    static const
    std::map<Category, pair<std::string, std::string> > namesAndRegexs;

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

    Category type;
    std::string value;

    LexicalSymbol(Category type, std::string value):type(type), value(value){}
    LexicalSymbol() = default;
    LexicalSymbol(Category type): type(type){}
    LexicalSymbol(std::string s)
    {
        if(s[0] == '_' || islower(s[0]) || isupper(s[0]))
        {
            if(keywordToType.count(s))
            {
                type = keywordToType.at(s);
                value = s; // can be improved
            }
            else
            {
                type = Category::Identifier;
                value = s;
            }

        }
        else if(isdigit(s[0]))
        {
            type = Category::Integer;
            value = s;
        }
        else
        {
            for(int i = static_cast<int>(Category::Integer) + 1; i < static_cast<int>(Category::End); ++ i)
            {
                if(s == namesAndRegexs.at(static_cast<Category>(i)).first)
                {
                    type = static_cast<Category>(i);
                    return ;
                }
            }
            assert(0);
        }
    }
    LexicalSymbol(const char * s):LexicalSymbol(std::string(s))
    {

    }

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const LexicalSymbol&  ls)
    {
        if(namesAndRegexs.count(ls.type))
            os << namesAndRegexs.at(ls.type).first;
        else
        {
            os << ls.value;
        }


//        os << names[static_cast<unsigned>(ls.type)];

        if(ls.type == Category::Identifier || ls.type == Category::Integer)
        {

            os << "(" << ls.value << ")";
        }
        return os;

    }
};

const std::map<LexicalSymbol::Category, std::pair<std::string, std::string> > LexicalSymbol::namesAndRegexs =
{
        { Category::Identifier, {"identifier", "(_|[a-zA-Z])(_|[a-zA-Z0-9])*", }},
        { Category::Integer, {"integer", "([0-9][0-9]*)", }},
        { Category::Plus, {"+", R"(\+)", }},
        { Category::Minus, {"-", R"(\-)", }},
        { Category::Asterisk, {"*", R"(\*)", }},
        { Category::Slash, {"/", R"(\/)", }},
        { Category::Ampersand, {"&", R"(\&)", }},
        { Category::DoubleAmpersand, {"&&", R"(\&\&)", }},
        { Category::VerticalBar, {"|", R"(\|)", }},
        { Category::DoubleVerticalBar, {"||", R"(\|\|)", }},
        { Category::ExclamationMark, {"!", R"(\!)", }},
        { Category::LeftParenthese, {"(", R"(\()", }},
        { Category::RightParenthese, {")", R"(\))", }},
        { Category::LeftSquareBracket, {"[", R"(\[)", }},
        { Category::RightSquareBracket, {"]", R"(\])", }},
        { Category::LeftBrace, {"{", R"(\{)", }},
        { Category::RightBrace, {"}", R"(\})", }},
        { Category::Semicolon, {";", R"(\;)", }},
        { Category::AssignMark, {"=", R"(\=)", }},
        { Category::Equal, {"==", R"(\=\=)", }},


        { Category::NotEqual, {"!=",  R"(\!\=)", }},

        { Category::Less, {"<",  R"(\<)",}},

        { Category::LessEqual, {"<=", R"(\<\=)", }},

        { Category::More, {">", R"(\>)",}},

        { Category::MoreEqual, {">=", R"(\>\=)", }},


//        NotEqual,
//        Less,
//        LessEqual,
//        More,
//        MoreEqual,
//
//        R"(\!\=)",
//        R"(\<)",
//        R"(\<\=)",
//        R"(\>)",
//        R"(\>\=)",

};

const std::map<std::string, LexicalSymbol::Category> LexicalSymbol::keywordToType =
{
    {"while", Category::While},
    {"if", Category::If},
    {"else", Category::Else},

    {"for", Category::For},
    {"Break", Category::Break},
    {"Continue", Category::Continue},
    {"return", Category::Return},


    {"True", Category::True},
    {"False", Category::False},


    {"int", Category::Int},
    {"float", Category::Float},
    {"double", Category::Double},

};


template<typename Iterator>
auto lexicalAnalyze(Iterator textBegin, Iterator textEnd)
{
    using T = char;
    using NFA = AdjacencyList<DirectedGraphTag, NoProperty, Property<EdgeWeightTag, T> >;


    vector<LexicalSymbol> ans;
    NFA nfa;

    std::vector<std::pair<std::string, std::function<void(std::string::iterator, std::string::iterator)>>>
            regexAndFuncs;

    auto func = [&](auto first, auto last) {
        ans.push_back(LexicalSymbol(std::string(first, last)));
    };

    for(unsigned i: irange(static_cast<unsigned>(LexicalSymbol::Category::End) ) )
    {
        regexAndFuncs.push_back(std::make_pair(LexicalSymbol::namesAndRegexs.at(static_cast<LexicalSymbol::Category>(i)).second, func));
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



    for(std::string::iterator cntEnd;textBegin != textEnd; textBegin = cntEnd)
    {
        cntEnd = simulateNFA(textBegin, textEnd, nfa, start, vertexToFunc);

        if(cntEnd == textBegin)
        {
            cout << "Unrecognized: " << (int)*textBegin << endl;
            assert(0);
        }
    }

    return ans;
}

} // namespace lz





#endif /* LZ_COMPILER_C_LEXICAL_ANALYZE_H_ */
