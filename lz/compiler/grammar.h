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

//NonterminalSymbol

enum class SymbolType
{
    Variable,
    LiteralUnit,
    EmptyString,
    EndTag,


//    Cat,
//    Alternative,
//    KleenStar,
//    LeftParenthesis,
//    RightParenthesis,

};




using VariableType = long;

template<typename T>
struct Symbol
{
    SymbolType type;
    VariableType variable;
    T literalUnit;

    bool operator<(const Symbol & o) const
    {
        if(type != o.type) return type < o.type;
        if(type == SymbolType::Variable) return variable < o.variable;
        if(type == SymbolType::LiteralUnit) return literalUnit < o.literalUnit;
//        if(type == SymbolType::EndTag) return

        return false;
    }

    Symbol(SymbolType type):type(type)
    {

    }

    template<typename Any> // need improve
    Symbol(SymbolType type, Any any):type(type)
    {
        if(type == SymbolType::Variable)
        {
            variable = any;
        }
        else if(type == SymbolType::LiteralUnit)
        {
            literalUnit = any;
        }
        else if(type == SymbolType::EmptyString)
        {

        }
        else {}
    }

    friend std::ostream&
    operator<<(std::ostream& os, const Symbol&  s)
    {
        if(s.type == SymbolType::EmptyString)
        {
            os << "#"  ; // 暂时用#号代替空字符串字符
        }
        else if(s.type == SymbolType::LiteralUnit)
        {
            os << s.literalUnit;
        }
        else if(s.type == SymbolType::Variable)
        {
            os << s.variable;
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

//    template <class Char, class Traits>
//    friend std::basic_ostream<Char, Traits>&
//    operator<<(std::basic_ostream<Char, Traits>& os,
//               const Symbol&  s)
//    {
//        if(s.type == SymbolType::EmptyString)
//        {
//            os << "{E} 我\u2500" <<"ф"  ;
//        }
//        else if(s.type == SymbolType::LiteralUnit)
//        {
//            os << s.literalUnit;
//        }
//        else if(s.type == SymbolType::Variable)
//        {
//            os << s.variable;
//        }
//        else if(s.type == SymbolType::EndTag)
//        {
//            os << "$";
//        }
//        else
//        {
//
//        }
//
//
//        return os;
//    }

};




template<typename T>
Symbol<T> EmptyStringSymbol(SymbolType::EmptyString);

template<typename T>
Symbol<T> EndTagSymbol(SymbolType::EndTag);


//template<typename T>
//using RuleBody = std::vector<Symbol<T>>;
// A->α1 | α2 | α3 | ...
template<typename T>
struct RuleBodyUnion: std::vector< std::vector<Symbol<T>>  >
{
    using std::vector< std::vector<Symbol<T>>  >::vector;
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
                    if(g[i][j][k].type == SymbolType::Variable &&
                        g[i][j][k].variable < g.names.size() &&
                        !g.names[g[i][j][k].variable].empty())
                    {
                        os << g.names[g[i][j][k].variable];
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


//    auto variablesNumber() const
//    {
//        return size();
//    }
};







template<typename T>
using Set = std::set<T>;

template<typename T>
void
calculateVariableFirstSet(const Grammer<T>& g, std::vector<Set<Symbol<T>> >& mem , VariableType u)
{
    if(!mem[u].empty()) return;

    for(auto ruleBody: g[u])
    {
        if(ruleBody[0].type == SymbolType::EmptyString)
        {
            mem[u].insert(ruleBody[0]);
        }
        else if(ruleBody[0].type == SymbolType::LiteralUnit)
        {
            mem[u].insert(ruleBody[0]);
        }
        else if(ruleBody[0].type == SymbolType::Variable)
        {
            for(auto s: ruleBody)
            {
                if(s.type == SymbolType::LiteralUnit)
                {
                    mem[u].insert(s);
                    break;
                }
                else if(s.type == SymbolType::Variable)
                {
                    auto v = s.variable;
                    calculateVariableFirstSet(g, mem, v);
                    mem[u].insert(mem[v].begin(), mem[v].end());
                    if(!mem[v].count(Symbol<T>(SymbolType::EmptyString)))
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
    std::vector< Set<Symbol<T> > > mem(n);
    for(auto i = 0; i < n; ++ i)
    {
        if(mem[i].empty())
        {
            calculateVariableFirstSet(g, mem, i);
        }
    }

    return mem;

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
                    if(B.type == SymbolType::Variable)
                    {
                        bool needRecursive = false;
                        auto sizeB = followSet[B.variable].size();
                        if(i + 1 < ruleBody.size())
                        {
                            auto next = ruleBody[i + 1];
                            if(next.type == SymbolType::Variable)
                            {
                                followSet[B.variable].insert(firstSet[next.variable].begin(), firstSet[next.variable].end());
                                if(firstSet[next.variable].count(Symbol<T>(SymbolType::EmptyString)))
                                {
                                    followSet[B.variable].erase(Symbol<T>(SymbolType::EmptyString));
                                    needRecursive = true;
                                }
                            }
                            else if(next.type == SymbolType::LiteralUnit)
                            {
                                followSet[B.variable].insert(next);
                            }
                            else
                            {
                                std::cout << "ERROR " <<"\n";
                            }
                        }
                        else needRecursive = true;



                        if(needRecursive)
                        {
                            followSet[B.variable].insert(followSet[A].begin(), followSet[A].end());
                        }

                        if(followSet[B.variable].size() > sizeB)
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
std::vector<Map<Symbol<T>, int>>
constructPredictiveParsingTable(const Grammer<T>& g,
    const std::vector<Set<Symbol<T>>>& firstSet,
    const std::vector<Set<Symbol<T>>>& followSet)
{

    auto n = g.size();
    std::vector<Map<Symbol<T>, int> > table(n);


    for(int i = 0; i < n; ++ i)
    {
        for(int j = 0; j < g[i].size(); ++ j)
//        for(int j = g[i].size() - 1; j >= 0; --j)
        {
            auto ruleBody = g[i][j];
//            std::cout << "before : " << "\n";
            Symbol<T> s = g[i][j][0];
            bool addFollow = false;
            if(s.type == SymbolType::Variable)
            {
                for(auto a: firstSet[s.variable])
                {
                    table[i][a] = j;
                }
                if(firstSet[s.variable].count(EmptyStringSymbol<T> ))
                {
                    table[i].erase(EmptyStringSymbol<T>);
                    addFollow = true;
                }
            }
            else if(s.type == SymbolType::LiteralUnit)
            {
                table[i][s] = j;
            }
            else if(s.type == SymbolType::EmptyString)
            {
                addFollow = true;
            }

            if(addFollow)
            {
                for(auto b: followSet[i])
                {
                    table[i][b] = j;
                }
                if(followSet[i].count(EndTagSymbol<T>))
                {
                    table[i][EndTagSymbol<T>] = j;
                }

            }
        }
    }

    return table;
}






template<typename T>
struct RuleBody:std::vector<Symbol<T>>
{

};


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
       res.push_back(Symbol<T>(SymbolType::Variable, a.id));
       res.push_back(Symbol<T>(SymbolType::Variable, b.id));
       return res;
    }

    friend RuleBody<T> operator>>(NonterminalSymbol a, T b)
    {
       RuleBody<T> res;
       res.push_back(Symbol<T>(SymbolType::Variable, a.id));
       res.push_back(Symbol<T>(SymbolType::LiteralUnit, b));
       return res;
    }

    friend RuleBody<T> operator>>(T a, NonterminalSymbol b)
    {
       RuleBody<T> res;
       res.push_back(Symbol<T>(SymbolType::LiteralUnit, a));
       res.push_back(Symbol<T>(SymbolType::Variable, b.id));
       return res;
    }

//    friend RuleBody<T> operator>>(Symbol<T> a, NonterminalSymbol b)
//    {
//       RuleBody<T> res;
//       res.push_back(a);
//       res.push_back(Symbol<T>(SymbolType::Variable, b));
//       return res;
//    }
//
//    friend RuleBody<T> operator>>(NonterminalSymbol a, Symbol<T> b)
//    {
//       RuleBody<T> res;
//       res.push_back(Symbol<T>(SymbolType::Variable, a));
//       res.push_back(b);
//       return res;
//    }


//
    friend RuleBody<T> operator>>(RuleBody<T> a, NonterminalSymbol b)
    {
        a.push_back(Symbol<T>(SymbolType::Variable, b.id));
        return a;
    }

    std::vector<std::vector<Symbol<T>>> data;
    NonterminalSymbol& operator=(RuleBody<T> o)
    {
        data.push_back(o);
        return *this;
    }

    NonterminalSymbol& operator=(NonterminalSymbol o)
    {
        data.push_back(std::vector<Symbol<T>> {Symbol<T>(SymbolType::Variable, o.id)});
        return *this;
    }

    NonterminalSymbol& operator=(Symbol<T> o)
    {
        data.push_back(std::vector<Symbol<T>> {o});
        return *this;
    }


    NonterminalSymbol& operator=(T o)
    {
        data.push_back(std::vector<Symbol<T>> {Symbol<T>(SymbolType::LiteralUnit, o)});
        return *this;
    }

};
template<typename T>
int NonterminalSymbol<T>::gloldId = 0;




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
//        std::cout << nonterminal.name << "---- " << "\n";
    }


    return ans;

}

































} // namespace lz





#endif /* LZ_COMPILER_GRAMMAR_H_ */
