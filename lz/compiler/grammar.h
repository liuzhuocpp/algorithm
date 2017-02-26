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
//    LeftParenthesis,
//    RightParenthesis,

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
};


// 为了给用户更加方便的编写文法的方式，最终文法将会存储在Grammer里，此类仅仅作为 各类操作符的重定义，使用户可以用接近EBNF的语法书写文法。
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
       RuleBody<T> res;
       res.push_back(makeNonterminal<T>(a.id));
       res.push_back(makeNonterminal<T>(b.id));
       return res;
    }

    friend RuleBody<T> operator>>(NonterminalProxy a, T b)
    {
       RuleBody<T> res;
       res.push_back(makeNonterminal<T>(a.id));
       res.push_back(makeTerminal<T>(b));
       return res;
    }

    friend RuleBody<T> operator>>(T a, NonterminalProxy b)
    {
       RuleBody<T> res;
       res.push_back(makeTerminal<T>(a));
       res.push_back(makeNonterminal<T>(b.id));
       return res;
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












template<typename T>
using Set = std::set<T>;

template<typename T>
void calculateVariableFirstSet(const Grammer<T>& g, std::vector<Set<Symbol<T>> >& firstSet , NonterminalType u)
{
    if(!firstSet[u].empty()) return;

    for(auto ruleBody: g[u])
    {
        Symbol<T> r0 = ruleBody[0];
        if(r0.isEmptyString())
        {
            firstSet[u].insert(r0);
        }
        else if(r0.isTerminal())
        {
            firstSet[u].insert(r0);
        }
        else if(r0.isNonterminal())
        {
            for(auto s: ruleBody)
            {
                if(s.type == SymbolType::Terminal)
                {
                    firstSet[u].insert(s);
                    break;
                }
                else if(s.type == SymbolType::Nonterminal)
                {
                    auto v = s.nonterminal;
                    calculateVariableFirstSet(g, firstSet, v);
                    firstSet[u].insert(firstSet[v].begin(), firstSet[v].end());
                    if(!firstSet[v].count(EmptyStringSymbol<T>))
                    {
                        break;
                    }

                }
            }

        }
    }
}

template<typename T>
std::vector< Set<Symbol<T> > > calculateFirstSet(const Grammer<T>& g)
{
    auto n = g.size();
    std::vector< Set<Symbol<T> > > firstSet(n);
    for(auto i = 0; i < n; ++ i)
    {
        if(firstSet[i].empty())
        {
            calculateVariableFirstSet(g, firstSet, i);
        }
    }
    return firstSet;
}


template<typename T>
std::vector< Set<Symbol<T> > > calculateFollowSet(const Grammer<T>& g, const std::vector< Set<Symbol<T> > >& firstSet)
{
    auto n = g.size();
    std::vector< Set<Symbol<T> > > followSet(n);
    followSet[0].insert(EndTagSymbol<T>);

    while(true)
    {
        bool hasNew = false;
        for(auto A: irange(n))
        {
            for(auto ruleBody: g[A])
            {
                for(auto i: irange(ruleBody.size()))
                {
                    Symbol<T> B = ruleBody[i];
                    if(B.isNonterminal())
                    {
                        bool needRecursive = false;
                        auto sizeB = followSet[B.nonterminal].size();
                        if(i + 1 < ruleBody.size())
                        {
                            Symbol<T> next = ruleBody[i + 1];
                            if(next.isNonterminal())
                            {
                                followSet[B.nonterminal].insert(firstSet[next.nonterminal].begin(), firstSet[next.nonterminal].end());
                                if(firstSet[next.nonterminal].count(EmptyStringSymbol<T>))
                                {
                                    followSet[B.nonterminal].erase(EmptyStringSymbol<T>);
                                    needRecursive = true;
                                }
                            }
                            else if(next.isTerminal())
                            {
                                followSet[B.nonterminal].insert(next);
                            }
                            else
                            {
                                std::cout << "ERROR " <<"\n";
                            }
                        }
                        else needRecursive = true;

                        if(needRecursive)
                        {
                            followSet[B.nonterminal].insert(followSet[A].begin(), followSet[A].end());
                        }

                        if(followSet[B.nonterminal].size() > sizeB)
                        {
                            hasNew = true;
                        }
                    }
                }
            }
        }
        if(!hasNew) break;
    }
    return followSet;
}

template<typename K, typename V>
using Map = std::map<K, V>;

template<typename T>
struct PredictiveParsingTable: std::vector<Map<Symbol<T>, RuleBody<T>>>
{
    PredictiveParsingTable(const Grammer<T>& g)
    {
        auto firstSet = calculateFirstSet(g);
        auto followSet = calculateFollowSet(g, firstSet);
        construct(g, firstSet, followSet);

    }


private:
    void construct(const Grammer<T>& g,
        const std::vector<Set<Symbol<T>>>& firstSet,
        const std::vector<Set<Symbol<T>>>& followSet)
    {
        auto n = g.size();
        this->clear();
        this->resize(n);

        for(int i = 0; i < n; ++ i)
        {
            for(int j = 0; j < g[i].size(); ++ j)
            {
                auto ruleBody = g[i][j];
                Symbol<T> s = g[i][j][0];
                bool addFollow = false;
                if(s.isNonterminal())
                {
                    for(auto a: firstSet[s.nonterminal])
                    {
                        (*this)[i][a] = ruleBody;
                    }
                    if(firstSet[s.nonterminal].count(EmptyStringSymbol<T> ))
                    {
                        (*this)[i].erase(EmptyStringSymbol<T>);
                        addFollow = true;
                    }
                }
                else if(s.isTerminal())
                {
                    (*this)[i][s] = ruleBody;
                }
                else if(s.isEmptyString())
                {
                    addFollow = true;
                }

                if(addFollow)
                {
                    for(auto b: followSet[i])
                    {
                        (*this)[i][b] = ruleBody;
                    }
                    if(followSet[i].count(EndTagSymbol<T>))
                    {
                        (*this)[i][EndTagSymbol<T>] = ruleBody;
                    }

                }
            }
        }
    }
};





// For output
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

template<typename T>
struct PredictiveParsingTableForOutput
{
    const PredictiveParsingTable<T>& table;
    const std::vector<std::string>& names;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os, const PredictiveParsingTableForOutput&  o)
    {

        for(int i = 0; i < o.table.size(); ++ i)
        {
            auto iout = SymbolForOutput<T>{Symbol<T>(SymbolType::Nonterminal, i), o.names};
            for(auto pi: o.table[i])
            {
                os <<std::left << std::setw(3) <<  iout << ", " << SymbolForOutput<T>{pi.first, o.names} << ":   " << iout << "->";
                for(auto symbol: pi.second)
                {
                    os << SymbolForOutput<T>{symbol, o.names};
                    os << " ";
                }
                os << "\n";
            }
        }
        return os;
    }

};

template<typename Iterator>
void predictivePasringLL1(const PredictiveParsingTable<typename std::iterator_traits<Iterator>::value_type> &table,
    Iterator first, Iterator last, Symbol<typename std::iterator_traits<Iterator>::value_type> startSymbol,
    const std::vector<std::string>& names)
{
    using T = typename std::iterator_traits<Iterator>::value_type;
    std::vector<Symbol<T>> stack;
    stack.push_back(startSymbol);

    while(!stack.empty() )
    {
        Symbol<T> x = stack.back();
        if(x.isTerminal())
        {
            if(x.terminal == *first)
            {
                stack.pop_back();
                first ++;
                std::cout << "match :" << *first;
                std::cout << "\n";
            }
            else
            {
                std::cout << "error" << "\n";
            }
        }
        else if(x.isNonterminal())
        {
            auto cntFirst = EndTagSymbol<T>;
            if(first != last)
                cntFirst = makeTerminal(*first);

            if(table[x.nonterminal].count(cntFirst))
            {
                stack.pop_back();
                auto ruleBody = table[x.nonterminal].at(cntFirst);
                for(int i = ruleBody.size() - 1; i >= 0; -- i)
                {
                    if(!ruleBody[i].isEmptyString())
                    stack.push_back(ruleBody[i]);
                }

                std::cout << "output:";

                std::cout << RuleForOutput<T>{x.nonterminal, ruleBody, names};
                std::cout << "\n";
            }
            else
            {
                std::cout << "error" << "\n";

            }
        }
    }
}
































} // namespace lz





#endif /* LZ_COMPILER_GRAMMAR_H_ */
