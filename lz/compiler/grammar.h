/*
 * grammar.h
 *
 *  Created on: 2017年2月21日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_GRAMMAR_H_
#define LZ_COMPILER_GRAMMAR_H_



namespace lz {

    namespace Detail {



    }
enum class SymbolType
{
    Nonterminal,
    Terminal,
    EmptyString,
    EndTag,


//    Cat,
//    Alternative,
//    KleenStar,

};




using NonterminalType = long;

template<typename T>
struct Symbol
{
    SymbolType type;
    union
    {
        NonterminalType nonterminal;
        T terminal;
    };


    bool isNonterminal() const
    {
        return type == SymbolType::Nonterminal;
    }

    bool isTerminal() const
    {
        return type == SymbolType::Terminal;
    }

    bool isEmptyString() const
    {
        return type == SymbolType::EmptyString;
    }

    bool isEndTag() const
    {
        return type == SymbolType::EndTag;
    }


    bool operator<(const Symbol & o) const
    {
        if(type != o.type) return type < o.type;
        if(type == SymbolType::Nonterminal) return nonterminal < o.nonterminal;
        if(type == SymbolType::Terminal) return terminal < o.terminal;
        return false;
    }


    bool operator==(const Symbol & o) const
    {
        if(type != o.type) return false;
        if(isNonterminal()) return nonterminal == o.nonterminal;
        if(isTerminal()) return terminal == o.terminal;
        return true;
    }


    Symbol(SymbolType type):type(type)
    {

    }

    template<typename Any> // need improve
    Symbol(SymbolType type, Any any):type(type)
    {
        if(type == SymbolType::Nonterminal)
        {
            nonterminal = any;
        }
        else if(type == SymbolType::Terminal)
        {
            terminal = any;
        }
        else if(type == SymbolType::EmptyString)
        {

        }
        else {}
    }
};



template<typename T>
Symbol<T> makeNonterminal(NonterminalType i)
{
    return Symbol<T>(SymbolType::Nonterminal, i);
}

template<typename T>
Symbol<T> makeTerminal(T i)
{
    return Symbol<T>(SymbolType::Terminal, i);
}

template<typename T>
const Symbol<T> EmptyStringSymbol(SymbolType::EmptyString);

template<typename T>
const Symbol<T> EndTagSymbol(SymbolType::EndTag);



template<typename T>
struct RuleBody: std::vector<Symbol<T>>
{
    using std::vector<Symbol<T>>::vector;
};

// A->α1 | α2 | α3 | ...
template<typename T>
struct RuleBodyUnion: std::vector< RuleBody<T> >
{
    using std::vector< RuleBody<T>  >::vector;
};


template<typename T>
struct NonterminalProxy;

// T 表示终结符号
template<typename T>
struct Grammer : std::vector<RuleBodyUnion<T> >
{
    NonterminalProxy<T> getNonterminalProxy(NonterminalType i);

    using std::vector<RuleBodyUnion<T> >::vector;

};


// 为了给用户更加方便的编写文法的方式，最终文法将会存储在Grammar里，此类仅仅作为 各类操作符的重定义，使用户可以用接近EBNF的语法书写文法。
template<typename T>
struct NonterminalProxy
{
private:
    friend Grammer<T>;
    NonterminalType id;
    Grammer<T>& g;
    NonterminalProxy(NonterminalType id, Grammer<T>& g ):id(id), g(g) { }
public:

    friend RuleBody<T> operator>>(NonterminalProxy a, NonterminalProxy b)
    {
        return RuleBody<T>{makeNonterminal<T>(a.id), makeNonterminal<T>(b.id)};
    }

    friend RuleBody<T> operator>>(NonterminalProxy a, T b)
    {
        return RuleBody<T> {makeNonterminal<T>(a.id), makeTerminal<T>(b)};
    }

    friend RuleBody<T> operator>>(T a, NonterminalProxy b)
    {
        return RuleBody<T> {makeTerminal<T>(a), makeNonterminal<T>(b.id)};
    }

    friend RuleBody<T> operator>>(RuleBody<T> a, NonterminalProxy b)
    {
        a.push_back(makeNonterminal<T>(b.id));
        return a;
    }

    NonterminalProxy& operator=(RuleBody<T> o)
    {
        g[id].push_back(o);
        return *this;
    }

    NonterminalProxy& operator=(NonterminalProxy o)
    {
        g[id].push_back({makeNonterminal<T>(o.id)});
        return *this;
    }

    NonterminalProxy& operator=(Symbol<T> o)
    {
        g[id].push_back({o});
        return *this;
    }

    NonterminalProxy& operator=(T o)
    {
        g[id].push_back({makeTerminal(o)});
        return *this;
    }
};




template<typename T>
NonterminalProxy<T> Grammer<T>::getNonterminalProxy(NonterminalType i)
{
    return NonterminalProxy<T>(i, *this);
}

template<typename T>
RuleBody<T> operator>>(RuleBody<T> a, T b)
{
    a.push_back(Symbol<T>(SymbolType::Terminal, b));
    return a;
}




// For output----------------------------------------------------
template<typename T>
struct SymbolForOutput
{
    const Symbol<T>& symbol;
    const std::vector<std::string>& names;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const SymbolForOutput& so)
    {
        const Symbol<T>& s = so.symbol;

        if(s.isEmptyString())
        {
            os << "#"  ; // 暂时用#号代替空字符串字符
        }
        else if(s.isTerminal())
        {
            os << s.terminal;
        }
        else if(s.isNonterminal())
        {
            auto i = s.nonterminal;
            if(i < so.names.size() && !so.names[i].empty())
                os << so.names[i];
            else os << std::to_string(i);
        }
        else if(s.isEndTag())
        {
            os << "$";
        }
        else
        {

        }

        return os;
    }

};

template<typename T>
struct RuleBodyForOutput
{
    const RuleBody<T> &ruleBody;
    const std::vector<std::string>& names;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const RuleBodyForOutput& r)
    {
        for(auto s: r.ruleBody)
        {
            os << SymbolForOutput<T>{s, r.names};
            os << " " ;
        }
        return os;
    }

};

template<typename T>
struct RuleForOutput
{
    NonterminalType ruleHead;
    const RuleBody<T> &ruleBody;
    const std::vector<std::string>& names;
    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const RuleForOutput& r)
    {

        os << SymbolForOutput<T>{makeNonterminal<T>(r.ruleHead), r.names}
            << "->" << RuleBodyForOutput<T>{r.ruleBody, r.names};

        return os;
    }

};

template<typename T>
struct GrammerForOutput
{
    const Grammer<T>& g;
    const std::vector<std::string>& names;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const GrammerForOutput&  g)
    {
        for(NonterminalType i: irange(g.g.size()))
        {
            for(auto j: irange(g.g[i].size()))
            {
                os << RuleForOutput<T>{i, g.g[i][j], g.names} << "\n";
            }
        }
        return os;
    }

};








// LL1 first set calculation, maybe faster
//template<typename T>
//void calculateLL1TerminalsFirstSet(const Grammer<T>& g, std::vector<Set<Symbol<T>> >& firstSet , NonterminalType u)
//{
//    if(!firstSet[u].empty()) return;
//
//    for(auto ruleBody: g[u])
//    {
//        Symbol<T> r0 = ruleBody[0];
//        if(r0.isEmptyString())
//        {
//            firstSet[u].insert(r0);
//        }
//        else if(r0.isTerminal())
//        {
//            firstSet[u].insert(r0);
//        }
//        else if(r0.isNonterminal())
//        {
//            for(auto s: ruleBody)
//            {
//                if(s.type == SymbolType::Terminal)
//                {
//                    firstSet[u].insert(s);
//                    break;
//                }
//                else if(s.type == SymbolType::Nonterminal)
//                {
//                    auto v = s.nonterminal;
//                    calculateLL1TerminalsFirstSet(g, firstSet, v);
//                    firstSet[u].insert(firstSet[v].begin(), firstSet[v].end());
//                    if(!firstSet[v].count(EmptyStringSymbol<T>))
//                    {
//                        break;
//                    }
//
//                }
//            }
//
//        }
//    }
//}
//
//template<typename T>
//std::vector< Set<Symbol<T> > > calculateLL1TerminalsFirstSet(const Grammer<T>& g)
//{
//    auto n = g.size();
//    std::vector< Set<Symbol<T> > > firstSet(n);
//    for(auto i = 0; i < n; ++ i)
//    {
//        if(firstSet[i].empty())
//        {
//            calculateVariableFirstSet(g, firstSet, i);
//        }
//    }
//    return firstSet;
//}
//




















} // namespace lz





#endif /* LZ_COMPILER_GRAMMAR_H_ */
