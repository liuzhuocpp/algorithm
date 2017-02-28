/*
 * grammar_design.h
 *
 *  Created on: 2017年2月28日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_GRAMMAR_DESIGN_H_
#define LZ_COMPILER_GRAMMAR_DESIGN_H_



/*
 *
 包含了一些对文法重新设计、改造的算法，包括 计算first集、follow集，消除左递归，提取左公共因子
 */



namespace lz {




template<typename T>
using Set = std::set<T>;


template<typename T>
void calculateRuleBodyFirstSet(const RuleBody<T>& ruleBody,
    const std::vector<Set<Symbol<T>>>& nonterminalsFirstSet, Set<Symbol<T>>& ruleBodyFirstSet);


template<typename T>
std::vector< Set<Symbol<T> > > calculateNonternimalFirstSets(const Grammar<T>& g)
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
std::vector< Set<Symbol<T> > > calculateNonternimalFollowSets(const Grammar<T>& g, const std::vector< Set<Symbol<T> > >& firstSet)
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






template<typename T>
void eliminateDirectLeftRecursion(Grammar<T>& g, NonterminalType A)
{
    using DiffType = typename Grammar<T>::difference_type;
    DiffType n = g.size();
    std::vector<DiffType> sames;

    for(auto i: irange(g[A].size()))
    {
        if(g[A][i][0] == makeNonterminal<T>(A))
        {
            sames.push_back(i);
        }
    }
    if(sames.empty()) return ;
    if(sames.size() == g[A].size())
    {
        std::cout << "error" << "\n";
        return ;
    }
    NonterminalType _A = g.size();
    g.push_back(RuleBodyUnion<T>());
    g[_A].push_back(RuleBody<T>{EmptyStringSymbol<T>});
    for(DiffType i: sames)
    {
        RuleBody<T> newBody(g[A][i].begin() + 1, g[A][i].end());
        newBody.push_back(makeNonterminal<T>(_A));
        g[_A].push_back(std::move(newBody));
    }

    DiffType k = 0;
    for(DiffType i = 0, j = 0; j < g[A].size();)
    {
        if(i < sames.size() && sames[i] == j)
        {
            i ++;
            j ++;
        }
        else
        {
            g[A][k ++] = g[A][j];
            j ++;
        }
    }
    g[A].resize(k);
    for(DiffType i = 0; i < k; ++ i)
    {
        if(g[A][i][0].isEmptyString())
            g[A][i][0] = makeNonterminal<T>(_A);
        else
            g[A][i].push_back(makeNonterminal<T>(_A));
    }

}

template<typename T>
void eliminateLeftRecursion(Grammar<T>& g)
{
    using DiffType = typename Grammar<T>::difference_type;
    DiffType n = g.size();
    for(DiffType i = 0; i < n; ++ i)
    {
        for(DiffType j = 0; j < i; ++ j) // could be more efficient
        {

            std::vector<DiffType> needDeleted;
            for(DiffType I = g[i].size() - 1; I >= 0; -- I)
            {
                if(g[i][I][0] == makeNonterminal<T>(j))
                {
                    needDeleted.push_back(I);

                    DiffType iRulesNumber = g[i].size();
                    g[i].insert(g[i].end(), g[j].begin(), g[j].end());

                    for(DiffType k = iRulesNumber; k < g[i].size(); ++ k)
                    {
                        if(g[i][k][0].isEmptyString())
                        {
                            g[i][k].assign(g[i][I].begin() + 1, g[i][I].end());
                        }
                        else
                        {
                            g[i][k].insert(g[i][k].end(), g[i][I].begin() + 1, g[i][I].end());
                        }
                    }
                }
            }

            for(DiffType I: needDeleted)
            {
                g[i].erase(g[i].begin() + I);
            }

        }
        eliminateDirectLeftRecursion(g, i);
    }
}

template<typename T>
void leftFactor(Grammar<T>& g, NonterminalType A) // could be more efficent
{
    using DiffType = typename Grammar<T>::difference_type;
    std::sort(g[A].begin(), g[A].end());
    RuleBodyUnion<T> newARuleBodyUnion;
    for(DiffType i = 0, j; i < g[A].size(); i = j)
    {
        for(j = i + 1; j < g[A].size() && g[A][j][0] == g[A][i][0]; ++ j);
        if(j - i == 1)
        {
            newARuleBodyUnion.push_back(g[A][i]);
            continue;
        }
        DiffType l;
        for(l = 1; ; ++ l)
        {
            for(DiffType k = i + 1; k < j; ++ k)
            {
                if(!(l < g[A][k].size() && g[A][k][l] == g[A][i][l]))
                {
                    goto XXX;
                }
            }

        }
        XXX:
        NonterminalType _A = g.size();
        g.push_back(RuleBodyUnion<T>());
        for(auto k: irange(i, j))
        {
            g[_A].push_back(RuleBody<T>(g[A][k].begin() + l, g[A][k].end()));
            if(g[_A][k].empty())
            {
                g[_A][k].push_back(EmptyStringSymbol<T>);
            }
        }
        newARuleBodyUnion.push_back(RuleBody<T>(g[A][i].begin(), g[A][i].begin() + l) );
        newARuleBodyUnion.back().push_back(makeNonterminal<T>(_A));
        leftFactor(g, _A);

    }
    g[A] = newARuleBodyUnion;

}




} // namespace lz





#endif /* LZ_COMPILER_GRAMMAR_DESIGN_H_ */
