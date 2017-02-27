/*
 * ll1_grammer.h
 *
 *  Created on: 2017年2月26日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_LL1_GRAMMAR_H_
#define LZ_COMPILER_LL1_GRAMMAR_H_

#include <lz/compiler/grammar.h>

namespace lz {




template<typename T>
using Set = std::set<T>;


template<typename T>
void calculateRuleBodyFirstSet(const RuleBody<T>& ruleBody,
    const std::vector<Set<Symbol<T>>>& nonterminalsFirstSet, Set<Symbol<T>>& ruleBodyFirstSet);


template<typename T>
std::vector< Set<Symbol<T> > > calculateTernimalFirstSets(const Grammar<T>& g)
{
    auto n = g.size();
    std::vector< Set<Symbol<T> > > ans(n);
    while(true)
    {
        bool hasNew = false;
        for(NonterminalType A: irange(n))
        {
            for(const RuleBody<T>& ruleBody: g[A])
            {
                auto oldSize = ans[A].size();

                calculateRuleBodyFirstSet(ruleBody, ans, ans[A]);
                if(ans[A].size() > oldSize) hasNew = true;
            }
        }
        if(!hasNew) break;
    }

    return ans;
}


template<typename T>
void calculateRuleBodyFirstSet(const RuleBody<T>& ruleBody,
    const std::vector<Set<Symbol<T>>>& nonterminalsFirstSet, Set<Symbol<T>>& ruleBodyFirstSet)
{
    bool allHasEmptyString = true;
    for(Symbol<T> u: ruleBody)
    {
        if(u.isNonterminal())
        {
            bool hasEmptyString = false;
            for(Symbol<T> firstSymbol: nonterminalsFirstSet[u.nonterminal])
            {
                if(firstSymbol.isEmptyString())
                {
                    hasEmptyString = true;
                }
                else
                {
                    ruleBodyFirstSet.insert(firstSymbol);
                }
            }
            if(!hasEmptyString)
            {
                allHasEmptyString = false; break;
            }
        }
        else if(u.isTerminal())
        {
            ruleBodyFirstSet.insert(u);
            allHasEmptyString = false;
            break;
        }
        else if(u.isEmptyString())
        {
            ruleBodyFirstSet.insert(u);
            break;
        }
        else
        {
            std::cout << "error" << "\n";
        }
    }
    if(allHasEmptyString) ruleBodyFirstSet.insert(EmptyStringSymbol<T>);
//    return ans;
}



template<typename T>
Set<Symbol<T>> calculateRuleBodyFirstSet(const RuleBody<T>& ruleBody,
    const std::vector<Set<Symbol<T>>>& nonterminalsFirstSet)
{
    Set<Symbol<T>> ans;

    calculateRuleBodyFirstSet(ruleBody, nonterminalsFirstSet, ans);
    return ans;
}





template<typename T>
std::vector< Set<Symbol<T> > > calculateTernimalFollowSets(const Grammar<T>& g, const std::vector< Set<Symbol<T> > >& firstSet)
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
struct LL1ParsingTable:std::vector<Map<Symbol<T>, RuleBody<T>>>
{

};





template<typename InputIterator1, typename InputIterator2>
bool isSetIntersection(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2)
{
    for(;first1 != last1 && first2 != last2;)
    {
        if(*first1 < *first2)
        {
            first1 ++;
        }
        else if(*first2 < *first1)
        {
            first2 ++;
        }
        else
        {
            return true;
        }
    }
    return false;
}


template<typename Range1, typename Range2>
bool isSetIntersection(const Range1 & r1, const Range2& r2)
{
    return isSetIntersection(std::begin(r1), std::end(r1), std::begin(r2), std::end(r2));
}





    namespace Detail {


    template<typename T>
    bool isLL1Grammar(const Grammar<T>& g,
        const std::vector<Set<Symbol<T>>>& firstSets,
        const std::vector<Set<Symbol<T>>>& followSets)
    {
        for(NonterminalType A: irange(g.size()))
        {
            for(auto i: irange(g[A].size()))
            {
                for(auto j: irange(i + 1, g[A].size()))
                {
                    auto firstSetI = calculateRuleBodyFirstSet(g[A][i], firstSets);
                    auto firstSetJ = calculateRuleBodyFirstSet(g[A][j], firstSets);
                    if(isSetIntersection(firstSetI, firstSetJ))
                    {
                        return false;
                    }

                    if(firstSetI.count(EmptyStringSymbol<T>))
                    {
                        if(isSetIntersection(followSets[A], firstSetJ))
                        {
                            return false;
                        }
                    }

                    if(firstSetJ.count(EmptyStringSymbol<T>))
                    {
                        if(isSetIntersection(followSets[A], firstSetI))
                        {
                            return false;
                        }
                    }

                }
            }
        }
        return true;
    }



    template<typename T>
    LL1ParsingTable<T> constructLL1ParsingTable(const Grammar<T>& g,
        const std::vector<Set<Symbol<T>>>& firstSet,
        const std::vector<Set<Symbol<T>>>& followSet)
    {
        LL1ParsingTable<T> table;
        auto n = g.size();
        table.resize(n);

        for(int i = 0; i < n; ++ i)
        {
            for(int j = 0; j < g[i].size(); ++ j)
            {
                auto ruleBody = g[i][j];

                Set<Symbol<T>> ruleBodyFirstSet = calculateRuleBodyFirstSet(ruleBody, firstSet);
                for(auto a: ruleBodyFirstSet)
                {
                    if(!a.isEmptyString())
                    {
                        table[i][a] = ruleBody;
                    }
                    else
                    {
                        for(auto b: followSet[i])
                        {
                            table[i][b] = ruleBody;
                        }
                    }
                }
             }
        }
        return table;
    }



    } // Detail



template<typename T>
void eliminateLeftRecursion(Grammar<T>& g)
{

}

template<typename T>
void leftFactor(Grammar<T>& g)
{

}



template<typename T>
bool isLL1Grammar(const Grammar<T>& g)
{
    auto firstSets = calculateTernimalFirstSets(g);
    auto followSets = calculateTernimalFollowSets(g, firstSets);

    return Detail::isLL1Grammar(g, firstSets, followSets);
}


template<typename T>
LL1ParsingTable<T> constructLL1ParsingTable(const Grammar<T>& g)
{
    auto firstSets = calculateTernimalFirstSets(g);
    auto followSets = calculateTernimalFollowSets(g, firstSets);

    return Detail::constructLL1ParsingTable(g, firstSets, followSets);
}

//template<typename T>
//struct PredictiveParsingTable: std::vector<Map<Symbol<T>, RuleBody<T>>>
//{
//    PredictiveParsingTable(const Grammer<T>& g)
//    {
//        auto firstSet = calculateTernimalFirstSets(g);
//        auto followSet = calculateTernimalFollowSets(g, firstSet);
//        construct(g, firstSet, followSet);
//
//    }
//
//
//private:
//    void construct(const Grammer<T>& g,
//        const std::vector<Set<Symbol<T>>>& firstSet,
//        const std::vector<Set<Symbol<T>>>& followSet)
//    {
//        auto n = g.size();
//        this->clear();
//        this->resize(n);
//
//        for(int i = 0; i < n; ++ i)
//        {
//            for(int j = 0; j < g[i].size(); ++ j)
//            {
//                auto ruleBody = g[i][j];
//
//                Set<Symbol<T>> ruleBodyFirstSet = calculateRuleBodyFirstSet(ruleBody, firstSet);
//                for(auto a: ruleBodyFirstSet)
//                {
//                    if(!a.isEmptyString())
//                        (*this)[i][a] = ruleBody;
//                    else
//                    {
//                        for(auto b: followSet[i])
//                        {
//                            (*this)[i][b] = ruleBody;
//                        }
//                    }
//                }
//
//             }
//        }
//    }
//};





template<typename Iterator>
void parseLL1Grammar(const LL1ParsingTable<typename std::iterator_traits<Iterator>::value_type> &table,
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






template<typename T>
struct LL1ParsingTableForOutput
{
    const LL1ParsingTable<T>& table;
    const std::vector<std::string>& names;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os, const LL1ParsingTableForOutput&  o)
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











} // namespace lz





#endif /* LZ_COMPILER_LL1_GRAMMAR_H_ */
