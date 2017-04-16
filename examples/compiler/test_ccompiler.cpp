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
    };
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
    Type type;
    string value;

    LexicalSymbol(Type type, string value):type(type), value(value){}
    LexicalSymbol() = default;
    LexicalSymbol(Type type): type(type){}

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const LexicalSymbol&  ls)
    {

        os << names[static_cast<unsigned>(ls.type)];

        if(ls.type == Type::Identifier || ls.type == Type::Integer)
        {
            os << " : " << ls.value;
        }
        return os;

    }


};
template<typename Iterator>
auto lexicalAnalyze(Iterator textBegin, Iterator textEnd)
{
    using T = char;
    using NFA = AdjacencyList<DirectedGraphTag, NoProperty, Property<EdgeWeightTag, T> >;


    vector<LexicalSymbol> ans;
    NFA nfa;

    auto startAndVertexToFunc =
//    auto [start, vertexToFunc] =
            parseMultiRegex(nfa, {

            {"(_|[a-zA-Z])(_|[a-zA-Z0-9])*", [&](auto first, auto last) {

                ans.push_back(LexicalSymbol(LexicalSymbol::Type::Identifier, string(first, last)));

            }},

            {"([0-9][0-9]*)", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol(LexicalSymbol::Type::Integer, string(first, last)));
            }},

            {"\\+", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::Plus);
            }},

            {"-", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::Minus);
            }},


            {"\\*", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::Asterisk);



            }},

            {"/", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::Slash);
            }},


            {"&", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::Ampersand);

            }},

            {"&&", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::DoubleAmpersand);
            }},

            {"&&&&*", [&](auto first, auto last) {
                assert(0);
//                ans.push_back(LexicalSymbol::Type::Plus);
            }},


            {R"(\|)", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::VerticalBar);
            }},

            {R"(\|\|)", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::DoubleVerticalBar);
            }},

            {R"(\|\|\|\|*)", [&](auto first, auto last) {
                assert(0);
            }},

            {"!", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::DoubleVerticalBar);
            }},

            {"\\(", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::LeftParenthese);
            }},

            {"\\)", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::RightParenthese);
            }},

            {"\\[", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::LeftSquareBracket);
            }},

            {"\\]", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::RightSquareBracket);
            }},

            {"\\{", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::LeftBrace);
            }},

            {"\\}", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::RightBrace);
            }},

            {"\\;", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::Semicolon);
            }},

            {R"(\=)", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::AssignMark);
            }},

            {R"(\=\=)", [&](auto first, auto last) {
                ans.push_back(LexicalSymbol::Type::EqualMark);
            }},

            {R"(\=\=\=\=*)", [&](auto first, auto last) {
                assert(0);
//                ans.push_back(LexicalSymbol::Type::Semicolon);
            }},





            {"  *", [&](auto first, auto last) {

            }},

            {"\n\n*", [&](auto first, auto last) {
            }},




    });


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
