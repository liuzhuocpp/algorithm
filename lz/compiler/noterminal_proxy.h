/*
 * noterminal_proxy.h
 *
 *  Created on: 2017年3月30日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NOTERMINAL_PROXY_H_
#define LZ_COMPILER_NOTERMINAL_PROXY_H_

#include <lz/compiler/grammar.h> // can be not use it

namespace lz {

    namespace Detail {

    template<typename Pair>
    struct IsStdPair
    {
        constexpr static bool value = 0;
    };

    template<typename A, typename B>
    struct IsStdPair<std::pair<A, B>>
    {
        constexpr static bool value = 1;
    };

    template<typename T, typename ...Args>
    auto addOrCheckPrecedencePair(std::tuple<Args...> a)
    {
        constexpr int Size = std::tuple_size<decltype(a)>::value;
        if constexpr(!Detail::IsStdPair<typename std::tuple_element_t<Size - 1, decltype(a)>>:: value)
        {
            return std::tuple_cat(a, std::make_tuple(std::make_pair(std::vector<T>(), std::vector<T>())));
        }
        else return a;
    }

    // 优先级符号的关系存储在tuple的最后一个element中，最后一个element是一个pair，
    // 每个pair是一个vector<T>，第一是比当前规则高的优先符号，第二个是第当前规则低的优先符号
    template<int Index, typename Tuple, typename T>
    auto addPrecedenceRelation(Tuple & ruleBody, T a)
    {
        auto newRuleBody = addOrCheckPrecedencePair<T>(ruleBody);
        constexpr auto Size = std::tuple_size<decltype(newRuleBody)>::value;
        std::get<Index>(std::get<Size - 1>(newRuleBody)).push_back(a);
        return newRuleBody;
    }

    template<int i, typename Tuple, typename Func>
    void applyTuple(const Tuple& a, Func f)
    {
        f(std::get<i>(a));
        if constexpr(i + 1 < std::tuple_size<Tuple>::value)
        {
            applyTuple<i+1>(a, f);
        }
    }

    template<typename Tuple, typename Func>
    void applyTuple(const Tuple& a, Func f)
    {
        applyTuple<0>(a, f);
    }

    } // Detail


struct EpsilonSymbol { } eps;

template<typename T, typename P>
struct NonterminalProxy;

template<typename T, typename P>
struct GrammarFactory
{
    friend NonterminalProxy<T, P>;

    Grammar<P> g;
private:
    std::map<T, SymbolDescriptor> terminalMap;
    std::map<int, SymbolDescriptor> nonterminalMap;
    std::map<T, int> terminalToIndexMap;
public:

    GrammarFactory(){}
    GrammarFactory(NonterminalProxy<T, P>& startSymbol)
    {
        this->connectStartNonterminal(startSymbol);
    }

    void connectStartNonterminal(NonterminalProxy<T, P>&);

    // 返回一个Map,key是terminal的id，value是对应的terminal
    auto getIndexToTerminalMap() const
    {
        SharedArrayMap<T> ans(terminalMap.size());
        for(auto p: terminalMap)
        {
            ans[getTerminalId(p.second)] = p.first;
        }
        return ans;
    }
    auto getTerminalToIndexMap()
    {
        terminalToIndexMap.clear();
        for(auto& _pair: terminalMap)
        {
            terminalToIndexMap[_pair.first] = lz::getTerminalId(_pair.second);
        }

        ConstAssociativeMap<decltype(terminalToIndexMap)> ans(terminalToIndexMap);
        return ans;

    }
private:

    //nonterminalProxyId仅仅作为一种标识，并不一定是从0开始的连续的id
    SymbolDescriptor getNonterminalAndInsert(int nonterminalProxyId)
    {
        if(nonterminalMap.count(nonterminalProxyId))
        {
            return nonterminalMap.at(nonterminalProxyId);
        }
        else
        {
            return nonterminalMap[nonterminalProxyId] = g.addNonterminal();
        }
    }

    SymbolDescriptor getTerminalSymbolAndInsert(T ch)
    {
        if(terminalMap.count(ch))
        {
            return terminalMap.at(ch);
        }
        else
        {
            return terminalMap[ch] = g.addTerminal();
        }
    }
};

template<typename T, typename P>
struct NonterminalProxy
{
private:
    friend GrammarFactory<T, P>;
    static int counter;
    SymbolDescriptor id;
    GrammarFactory<T, P>* gf;
public:
    NonterminalProxy(GrammarFactory<T, P>* gf = nullptr):
         gf(gf)
    {
        id = counter ++;
    }

    std::vector<SymbolDescriptor> addRuleHead()
    {
        std::vector<SymbolDescriptor> ans;
        ans.push_back(gf->getNonterminalAndInsert(id));
        return ans;
    }

    template<typename...Args>
    NonterminalProxy<T, P>& operator=(const std::tuple<Args...>& o)
    {
        std::vector<SymbolDescriptor> rule = addRuleHead();
        std::vector<T> highPrecedence, lowPrecedence;

        auto pushSymbol = [&](auto ch)
        {
            if constexpr(std::is_same<decltype(ch), NonterminalProxy<T, P>* >::value)
            {
                rule.push_back(this->gf->getNonterminalAndInsert(ch->id));
                ch->gf = this->gf;
            }
            else if constexpr(std::is_convertible<decltype(ch), T>::value)
            {
                rule.push_back(this->gf->getTerminalSymbolAndInsert(ch));
            }
            else if constexpr(Detail::IsStdPair<decltype(ch)>::value)
            {
                for(auto t: ch.first) {
                    highPrecedence.push_back(t);
                }

                for(auto t: ch.second) {
                    lowPrecedence.push_back(t);
                }

//                highPrecedence = std::move(ch.first);
//                lowPrecedence = std::move(ch.second);
            }
            else if constexpr(std::is_convertible<decltype(ch), SemanticRuleType<P> >::value)
            {
                rule.push_back(gf->g.addSemanticRuleFunc(ch));
            }
            else
            {

//                std::cout << (std::is_same<decltype(ch), std::string>::value) << "\n";
                assert(false);
//                static_assert(false);
            }
        };
        Detail::applyTuple(o, pushSymbol);

        std::vector<SymbolDescriptor> newRule;
        newRule.push_back(rule[0]);
        if(isSemanticRule( rule.back()) )
        {
            newRule.push_back(rule.back());
            rule.pop_back();
        }
        for(std::size_t i = 1; i < rule.size();)
        {
            if(isSemanticRule(rule[i]))
            {
                newRule.push_back(rule[i + 1]);
                newRule.push_back(rule[i]);
                i+=2;
            }
            else
            {
                newRule.push_back(rule[i]);
                i ++;
            }
        }


        auto rd = gf->g.addRule(newRule.begin(), newRule.end());


        for(T realTerminal: highPrecedence)
        {
            SymbolDescriptor terminal = gf->getTerminalSymbolAndInsert(realTerminal);
            gf->g.setPriority(rd, terminal, -1);
        }

        for(T realTerminal: lowPrecedence)
        {
            SymbolDescriptor terminal = gf->getTerminalSymbolAndInsert(realTerminal);
            gf->g.setPriority(rd, terminal, 1);
        }
        return *this;
    }

    NonterminalProxy& operator=(T o)
    {
        std::vector<SymbolDescriptor> rule = addRuleHead();
        rule.push_back({gf->getTerminalSymbolAndInsert(o)});
        gf->g.addRule(rule.begin(), rule.end());
        return *this;
    }

    NonterminalProxy& operator=(NonterminalProxy& o)
    {
        std::vector<SymbolDescriptor> rule = addRuleHead();
        rule.push_back({gf->getNonterminalAndInsert(o.id)});
        o.gf = this->gf;
        gf->g.addRule(rule.begin(), rule.end());
        return *this;
    }

    NonterminalProxy& operator=(EpsilonSymbol )
    {
        std::vector<SymbolDescriptor> rule = addRuleHead();
        gf->g.addRule(rule.begin(), rule.end());
        return *this;
    }

    NonterminalProxy& operator=(const SemanticRuleType<P>& func)
    {
        std::vector<SymbolDescriptor> rule = addRuleHead();
        rule.push_back(gf->g.addSemanticRuleFunc(func));
        gf->g.addRule(rule.begin(), rule.end());
        return *this;
    }
};

template<typename T, typename P>
int NonterminalProxy<T, P>::counter = 0;

template<typename T, typename P>
void GrammarFactory<T, P>::connectStartNonterminal(NonterminalProxy<T, P>& start)
{
    start.gf = this;
    terminalMap.clear();
    g.clear();
}

template<typename T, typename P>
auto operator>>(NonterminalProxy<T, P>& a, NonterminalProxy<T, P>& b)
{
    return std::make_tuple(&a, &b );
}

template<typename T, typename P>
auto operator>>(NonterminalProxy<T, P>& a, T b)
{
    return std::make_tuple(&a, b);
}

template<typename T, typename P, typename F>
auto operator>>(NonterminalProxy<T, P>& a, F b)
{
    return std::make_tuple(&a, b);
}

template<typename T, typename P>
auto operator>>(T a, NonterminalProxy<T, P>& b)
{
    return std::make_tuple(a, &b);
}

template<typename Func, typename T, typename P>
auto operator>>(Func a, NonterminalProxy<T, P>& b)
{
    return std::make_tuple(a, &b);
}

template<typename TerminalOrFunc1, typename TerminalOrFunc2>
auto operator>>(TerminalOrFunc1 a, TerminalOrFunc2 b)
{
    return std::make_tuple(a, b);
}

template<typename... Arg, typename Func>
auto operator>>(std::tuple<Arg...> a, Func b)
{
    return std::tuple_cat(a, std::make_tuple(b));
}

template<typename... Arg, typename T, typename P>
auto operator>>(std::tuple<Arg...> a, NonterminalProxy<T, P> & b)
{
    return std::tuple_cat(a, std::make_tuple(&b));
}

template<typename T>
auto operator>>(EpsilonSymbol, T b)
{
    return std::make_tuple(b);
}

template<typename...Args, typename T>
auto operator>(std::tuple<Args...> a, T b)
{
    return Detail::addPrecedenceRelation<1>(a, b);
}

template<typename...Args, typename T>
auto operator<(std::tuple<Args...> a, T b)
{
    return Detail::addPrecedenceRelation<0>(a, b);
}


}// namesapce lz

#endif /* LZ_COMPILER_NOTERMINAL_PROXY_H_ */
