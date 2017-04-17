#include <bits/stdc++.h>
#include <lz/debug.h>

#include <lz/compiler/regex.h>
#include <lz/compiler/simulate_nfa.h>





using namespace lz;
using namespace std;

struct LexicalSymbol
{
    enum class Type: unsigned
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
        EqualMark,

        End,

        While,
        If,
        Int,
        Float,
        Double,
        Return,
        For,

    };

    static const
    map<string, Type> keywordToType;

    static const
    map<Type, pair<string, string> > namesAndRegexs;

    constexpr static const char* names[] = {
        "identifier",
        "integer",
        "+",
        "-",
        "*",
        "/",
        "&",
        "&&",
        "|",
        "||",
        "!",
        "(",
        ")",
        "[",
        "]",
        "{",
        "}",
        ";",
        "=",
        "=="


    };

    constexpr static const char* regexs[] = {
        "(_|[a-zA-Z])(_|[a-zA-Z0-9])*",
        "([0-9][0-9]*)",
        R"(\+)",
        R"(\-)",
        R"(\*)",
        R"(\/)",
        R"(\&)",
        R"(\&\&)",
        R"(\|)",
        R"(\|\|)",
        R"(\!)",
        R"(\()",
        R"(\))",
        R"(\[)",
        R"(\])",
        R"(\{)",
        R"(\})",
        R"(\;)",
        R"(\=)",
        R"(\=\=)",


    };

    Type type;
    string value;

    LexicalSymbol(Type type, string value):type(type), value(value){}
    LexicalSymbol() = default;
    LexicalSymbol(Type type): type(type){}
    LexicalSymbol(string s)
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
                type = Type::Identifier;
                value = s;
            }

        }
        else if(isdigit(s[0]))
        {
            type = Type::Integer;
            value = s;
        }
        else
        {
            for(int i = static_cast<int>(Type::Integer) + 1; i < static_cast<int>(Type::End); ++ i)
            {
                if(s == names[i])
                {
                    type = static_cast<Type>(i);
                    return ;
                }
            }
            assert(0);
        }
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

        if(ls.type == Type::Identifier || ls.type == Type::Integer)
        {
            os << " : " << ls.value;
        }
        return os;

    }
};

const map<LexicalSymbol::Type, pair<string, string> > LexicalSymbol::namesAndRegexs =
{
        { Type::Identifier, {"identifier", "(_|[a-zA-Z])(_|[a-zA-Z0-9])*", }},
        { Type::Integer, {"integer", "([0-9][0-9]*)", }},
        { Type::Plus, {"+", R"(\+)", }},
        { Type::Minus, {"-", R"(\-)", }},
        { Type::Asterisk, {"*", R"(\*)", }},
        { Type::Slash, {"/", R"(\/)", }},
        { Type::Ampersand, {"&", R"(\&)", }},
        { Type::DoubleAmpersand, {"&&", R"(\&\&)", }},
        { Type::VerticalBar, {"|", R"(\|)", }},
        { Type::DoubleVerticalBar, {"||", R"(\|\|)", }},
        { Type::ExclamationMark, {"!", R"(\!)", }},
        { Type::LeftParenthese, {"(", R"(\()", }},
        { Type::RightParenthese, {")", R"(\))", }},
        { Type::LeftSquareBracket, {"[", R"(\[)", }},
        { Type::RightSquareBracket, {"]", R"(\])", }},
        { Type::LeftBrace, {"{", R"(\{)", }},
        { Type::RightBrace, {"}", R"(\})", }},
        { Type::Semicolon, {";", R"(\;)", }},
        { Type::AssignMark, {"=", R"(\=)", }},
        { Type::EqualMark, {"==", R"(\=\=)", }},
};

const map<string, LexicalSymbol::Type> LexicalSymbol::keywordToType =
{
    {"while", Type::While},
    {"if", Type::If},
    {"int", Type::Int},
    {"float", Type::Float},
    {"double", Type::Double},
    {"return", Type::Return},
    {"for", Type::For},


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
        ans.push_back(LexicalSymbol(string(first, last)));
    };

    for(unsigned i: irange(static_cast<unsigned>(LexicalSymbol::Type::End) ) )
    {
        regexAndFuncs.push_back(std::make_pair(LexicalSymbol::regexs[i], func));
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



    for(string::iterator cntEnd;textBegin != textEnd; textBegin = cntEnd)
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


template<typename InputIterator>
void grammarAnalyze(InputIterator first, InputIterator last)
{

}







int main()
{
    freopen("in.txt", "r", stdin);

    string text, tmpText;
    while(getline(cin, tmpText))
    {
        text += tmpText + "\n";
    }
    vector<LexicalSymbol> ans = lexicalAnalyze(text.begin(), text.end());
    for(auto x : ans)
    {
        cout << x << endl;
    }
//    cout << ans << endl;





    return 0;
}
