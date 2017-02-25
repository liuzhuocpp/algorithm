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
    NonterminalType nonterminal;
    T terminal;

    bool operator<(const Symbol & o) const
    {
        if(type != o.type) return type < o.type;
        if(type == SymbolType::Nonterminal) return nonterminal < o.nonterminal;
        if(type == SymbolType::Terminal) return terminal < o.terminal;
//        if(type == SymbolType::EndTag) return

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

    friend std::ostream& operator<<(std::ostream& os, const Symbol&  s)
    {
        if(s.type == SymbolType::EmptyString)
        {
            os << "#"  ; // 暂时用#号代替空字符串字符
        }
        else if(s.type == SymbolType::Terminal)
        {
            os << s.terminal;
        }
        else if(s.type == SymbolType::Nonterminal)
        {
            os << s.nonterminal;
        }
        else if(s.type == SymbolType::EndTag)
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
Symbol<T> EmptyStringSymbol(SymbolType::EmptyString);

template<typename T>
Symbol<T> EndTagSymbol(SymbolType::EndTag);



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

// T 表示终结符号
template<typename T>
struct Grammer : std::vector<RuleBodyUnion<T> >
{
    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const Grammer&  g)
    {
        for(int i = 0; i < g.size(); ++ i)
        {
            for(int j = 0; j < g[i].size(); ++ j)
            {
                if(i < g.names.size() && !g.names[i].empty())
                {
                    os << g.names[i];
                }
                else
                    os << i;
                os << "->";
                for(int k = 0; k < g[i][j].size(); ++ k)
                {
                    if(g[i][j][k].type == SymbolType::Nonterminal &&
                        g[i][j][k].nonterminal < g.names.size() &&
                        !g.names[g[i][j][k].nonterminal].empty())
                    {
                        os << g.names[g[i][j][k].nonterminal];
                    }
                    else
                        os << g[i][j][k];
                    os << " " ;
                }
                os << "\n";
            }
        }
        return os;
    }
    std::vector<std::string> names;

};







template<typename T>
using Set = std::set<T>;

template<typename T>
void calculateVariableFirstSet(const Grammer<T>& g, std::vector<Set<Symbol<T>> >& firstSet , NonterminalType u)
{
    if(!firstSet[u].empty()) return;

    for(auto ruleBody: g[u])
    {
        if(ruleBody[0].type == SymbolType::EmptyString)
        {
            firstSet[u].insert(ruleBody[0]);
        }
        else if(ruleBody[0].type == SymbolType::Terminal)
        {
            firstSet[u].insert(ruleBody[0]);
        }
        else if(ruleBody[0].type == SymbolType::Nonterminal)
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
                    if(!firstSet[v].count(Symbol<T>(SymbolType::EmptyString)))
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
    followSet[0].insert(Symbol<T>(SymbolType::EndTag));

    while(true)
    {
        bool hasNew = false;
        for(int A = 0; A < n; ++ A)
        {
            for(auto ruleBody: g[A])
            {
                for(int i = 0; i < ruleBody.size(); ++ i)
                {
                    auto B = ruleBody[i];
                    if(B.type == SymbolType::Nonterminal)
                    {
                        bool needRecursive = false;
                        auto sizeB = followSet[B.nonterminal].size();
                        if(i + 1 < ruleBody.size())
                        {
                            auto next = ruleBody[i + 1];
                            if(next.type == SymbolType::Nonterminal)
                            {
                                followSet[B.nonterminal].insert(firstSet[next.nonterminal].begin(), firstSet[next.nonterminal].end());
                                if(firstSet[next.nonterminal].count(Symbol<T>(SymbolType::EmptyString)))
                                {
                                    followSet[B.nonterminal].erase(Symbol<T>(SymbolType::EmptyString));
                                    needRecursive = true;
                                }
                            }
                            else if(next.type == SymbolType::Terminal)
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
    std::vector<std::string> names;
    PredictiveParsingTable(const Grammer<T>& g)
    {
        auto firstSet = calculateFirstSet(g);
        auto followSet = calculateFollowSet(g, firstSet);
        construct(g, firstSet, followSet);
        names = g.names;
    }

    std::string getName(NonterminalType i) const
    {
        if(i < names.size() && !names[i].empty())
        {
            return names[i];
        }
        else return std::to_string(i);
    }


    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const PredictiveParsingTable&  g)
    {
        std::size_t maxLen = 0;
        for(auto i: lz::irange(g.size())) maxLen = std::max(maxLen, g.getName(i).size());

//        os << maxLen;

        for(int i = 0; i < g.size(); ++ i)
        {

            for(auto pi: g[i])
            {
                os << g.getName(i) << std::string(maxLen - g.getName(i).size(), ' ') << ", "
                    << pi.first << ":   " << g.getName(i) << "->";
                for(auto symbol: pi.second)
                {
                    if(symbol.type == SymbolType::Nonterminal)
                    {
                        os << g.getName(symbol.nonterminal);
                    }
                    else os << symbol;
                    os << " ";
                }
                os << "\n";
            }
        }
        return os;
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
//                std::cout << ruleBody << "---" << "\n";
                Symbol<T> s = g[i][j][0];
                bool addFollow = false;
                if(s.type == SymbolType::Nonterminal)
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
                else if(s.type == SymbolType::Terminal)
                {
                    (*this)[i][s] = ruleBody;
                }
                else if(s.type == SymbolType::EmptyString)
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



// need improve
template<typename T>
void outRule(NonterminalType i, const RuleBody<T>& ruleBody, const std::vector<std::string>& names)
{
    auto getName = [=](NonterminalType i) ->std::string
    {
        if(i < names.size() && !names[i].empty())
        {
            return names[i];
        }
        else return std::to_string(i);
    };

    std::cout << getName(i) << "->";

    for(auto symbol: ruleBody)
    {
        if(symbol.type == SymbolType::Nonterminal)
        {
            std::cout << getName(symbol.nonterminal);
        }
        else std::cout << symbol;
        std::cout << " ";
    }

}

template<typename Iterator>
void predictivePasringLL1(const PredictiveParsingTable<typename std::iterator_traits<Iterator>::value_type> &table,
    Symbol<typename std::iterator_traits<Iterator>::value_type> startSymbol, Iterator first, Iterator last)
{
    using T = typename std::iterator_traits<Iterator>::value_type;
    std::vector<Symbol<T>> stack;
    stack.push_back(startSymbol);

    while(!stack.empty() )
    {
        Symbol<T> x = stack.back();
        if(x.type == SymbolType::Terminal)
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
        else if(x.type == SymbolType::Nonterminal)
        {
            Symbol<T> cntFirst(SymbolType::EndTag);
            if(first != last)
                cntFirst = Symbol<T>(SymbolType::Terminal, *first);

            if(table[x.nonterminal].count(cntFirst))
            {
                stack.pop_back();
                auto ruleBody = table[x.nonterminal].at(cntFirst);
                for(int i = ruleBody.size() - 1; i >= 0; -- i)
                {
                    if(ruleBody[i].type != SymbolType::EmptyString)
                    stack.push_back(ruleBody[i]);
                }

                std::cout << "output:";
                outRule(x.nonterminal, ruleBody, table.names);
                std::cout << "\n";
            }
            else
            {
                std::cout << "error" << "\n";

            }
        }
//        else if(x.type == SymbolType::EndTag)
//        {
//
//        }

    }
}







//
//template<typename T>
//std::vector<Map<Symbol<T>, int>>
//constructPredictiveParsingTable(const Grammer<T>& g,
//    const std::vector<Set<Symbol<T>>>& firstSet,
//    const std::vector<Set<Symbol<T>>>& followSet)
//{
//
//    auto n = g.size();
//    std::vector<Map<Symbol<T>, int> > table(n);
//
//    for(int i = 0; i < n; ++ i)
//    {
//        for(int j = 0; j < g[i].size(); ++ j)
//        {
//            auto ruleBody = g[i][j];
//            Symbol<T> s = g[i][j][0];
//            bool addFollow = false;
//            if(s.type == SymbolType::Nonterminal)
//            {
//                for(auto a: firstSet[s.nonterminal])
//                {
//                    table[i][a] = j;
//                }
//                if(firstSet[s.nonterminal].count(EmptyStringSymbol<T> ))
//                {
//                    table[i].erase(EmptyStringSymbol<T>);
//                    addFollow = true;
//                }
//            }
//            else if(s.type == SymbolType::Terminal)
//            {
//                table[i][s] = j;
//            }
//            else if(s.type == SymbolType::EmptyString)
//            {
//                addFollow = true;
//            }
//
//            if(addFollow)
//            {
//                for(auto b: followSet[i])
//                {
//                    table[i][b] = j;
//                }
//                if(followSet[i].count(EndTagSymbol<T>))
//                {
//                    table[i][EndTagSymbol<T>] = j;
//                }
//
//            }
//        }
//    }
//
//    return table;
//}




// 为了给用户更加方便的编写文法的方式，最终文法将会存储在Grammer里，此类仅仅作为 各类操作符的重定义，使用户可以用接近EBNF的语法书写文法。
template<typename T>
struct NonterminalSymbol
{
    static int gloldId;
    static void reset()
    {
        gloldId = 0;
    }

    int id;
    std::string name;
    NonterminalSymbol(std::string name = std::string()):id(gloldId ++), name(name) { }




    friend RuleBody<T> operator>>(NonterminalSymbol a, NonterminalSymbol b)
    {
       RuleBody<T> res;
       res.push_back(Symbol<T>(SymbolType::Nonterminal, a.id));
       res.push_back(Symbol<T>(SymbolType::Nonterminal, b.id));
       return res;
    }

    friend RuleBody<T> operator>>(NonterminalSymbol a, T b)
    {
       RuleBody<T> res;
       res.push_back(Symbol<T>(SymbolType::Nonterminal, a.id));
       res.push_back(Symbol<T>(SymbolType::Terminal, b));
       return res;
    }

    friend RuleBody<T> operator>>(T a, NonterminalSymbol b)
    {
       RuleBody<T> res;
       res.push_back(Symbol<T>(SymbolType::Terminal, a));
       res.push_back(Symbol<T>(SymbolType::Nonterminal, b.id));
       return res;
    }


    friend RuleBody<T> operator>>(RuleBody<T> a, NonterminalSymbol b)
    {
        a.push_back(Symbol<T>(SymbolType::Nonterminal, b.id));
        return a;
    }

    RuleBodyUnion<T> data;

    NonterminalSymbol& operator=(RuleBody<T> o)
    {
        data.push_back(o);
        return *this;
    }

    NonterminalSymbol& operator=(NonterminalSymbol o)
    {
        data.push_back(std::vector<Symbol<T>> {Symbol<T>(SymbolType::Nonterminal, o.id)});
        return *this;
    }

    NonterminalSymbol& operator=(Symbol<T> o)
    {
        data.push_back(std::vector<Symbol<T>> {o});
        return *this;
    }


    NonterminalSymbol& operator=(T o)
    {
        data.push_back(std::vector<Symbol<T>> {Symbol<T>(SymbolType::Terminal, o)});
        return *this;
    }
};

template<typename T>
int NonterminalSymbol<T>::gloldId = 0;


template<typename T>
RuleBody<T> operator>>(RuleBody<T> a, T b)
{
    a.push_back(Symbol<T>(SymbolType::Terminal, b));
    return a;
}


template<typename T>
Grammer<T> makeGrammer(const std::vector<NonterminalSymbol<T>>& nonterminals)
{
    Grammer<T> ans;
    auto n = nonterminals.size();
    ans.resize(n);
    ans.names.resize(n);
    for(NonterminalSymbol<T> nonterminal : nonterminals)
    {
        ans[nonterminal.id] = nonterminal.data;
        ans.names[nonterminal.id] = nonterminal.name;
    }
    return ans;

}

































} // namespace lz





#endif /* LZ_COMPILER_GRAMMAR_H_ */
