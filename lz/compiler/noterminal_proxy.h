/*
 * noterminal_proxy.h
 *
 *  Created on: 2017年3月30日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NOTERMINAL_PROXY_H_
#define LZ_COMPILER_NOTERMINAL_PROXY_H_



namespace lz{




template<typename T, typename P>
struct NonterminalProxy;


template<typename T, typename P>
struct GrammarFactory
{
    template<typename _T, typename _P>
    friend struct NonterminalProxy;

    Grammar<P> g;
private:

    std::map<T, SymbolDescriptor> terminalMap;
    std::map<T, int> terminalToIndexMap;

public:

    GrammarFactory(){}
    GrammarFactory(NonterminalProxy<T, P>& startSymbol);

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


    SymbolDescriptor getNonterminalAndInsert(int nonterminalProxyId)
    {
        int cntId = g.nonterminalsNumber();
        cntId --;
        for(;cntId < nonterminalProxyId; cntId++)
        {
            g.addNonterminal();
        }
        return lz::makeNonterminal(nonterminalProxyId);
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
    auto addOrCheckPrecedenceVector(std::tuple<Args...> a)
    {
        constexpr int Size = std::tuple_size<decltype(a)>::value;

        if constexpr(!Detail::IsStdPair<typename std::tuple_element_t<Size - 1, decltype(a)>>:: value)
        {
            return std::tuple_cat(a, std::make_tuple(std::make_pair(std::vector<T>(), std::vector<T>())));
        }
        else return a;
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




    }


struct EpsilonSymbol {




} eps;


template<typename T, typename P>
struct NonterminalProxy
{
private:
    static int counter;
public:
    SymbolDescriptor id;

    GrammarFactory<T, P>* gf;


    NonterminalProxy(GrammarFactory<T, P>* gf = nullptr):
         gf(gf)
    {
        id = counter ++;
    }


    NonterminalProxy(const NonterminalProxy<T, NoProperty>&other):
        id(other.id),  gf(nullptr)
    {
    }

    std::vector<SymbolDescriptor> addRuleHead();

    template<typename...Args>
    NonterminalProxy<T, P>& operator=(const std::tuple<Args...>& o);

    NonterminalProxy& operator=(T o);
    NonterminalProxy& operator=(NonterminalProxy& o);
    NonterminalProxy& operator=(EpsilonSymbol );
    NonterminalProxy& operator=(const SemanticRuleType<P>& func);

};

template<typename T, typename P>
int NonterminalProxy<T, P>::counter = 0;



template<typename T, typename P>
GrammarFactory<T, P>::GrammarFactory(NonterminalProxy<T, P>& startSymbol)
{
    this->connectStartNonterminal(startSymbol);
}

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
    auto newA = Detail::addOrCheckPrecedenceVector<T>(a);

    constexpr int Size = std::tuple_size<decltype(newA)>::value;
    auto& precedence = std::get<Size-1>(newA);
    precedence.second.push_back(b);
    return newA;
}


template<typename...Args, typename T>
auto operator<(std::tuple<Args...> a, T b)
{
    auto newA = Detail::addOrCheckPrecedenceVector<T>(a);
    constexpr int Size = std::tuple_size<decltype(newA)>::value;
    auto& precedence = std::get<Size-1>(newA);
    precedence.first.push_back(b);
    return newA;
}







template<typename T, typename P>
std::vector<SymbolDescriptor> NonterminalProxy<T, P>::addRuleHead()
{
    std::vector<SymbolDescriptor> ans;
    ans.push_back(gf->getNonterminalAndInsert(id));
    return ans;
}



template<typename T, typename P>
template<typename ...Args>
NonterminalProxy<T, P>& NonterminalProxy<T, P>::operator=(const std::tuple<Args...>& o)
{
    std::vector<SymbolDescriptor> rule = addRuleHead();
    std::vector<T> highPrecedence, lowPrecedence;

    auto pushSymbol = [&](auto ch)
    {

        if constexpr(std::is_same<decltype(ch), T>::value)
        {
            rule.push_back(this->gf->getTerminalSymbolAndInsert(ch));
        }
        else if constexpr(std::is_same<decltype(ch), NonterminalProxy<T, P>* >::value)
        {
            rule.push_back(this->gf->getNonterminalAndInsert(ch->id));
            ch->gf = this->gf;
        }
        else if constexpr(Detail::IsStdPair<decltype(ch)>::value)
        {
            highPrecedence = std::move(ch.first);
            lowPrecedence = std::move(ch.second);
        }
        else
        {
            rule.push_back(gf->g.addSemanticRuleFunc(ch));
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



template<typename T, typename P>
NonterminalProxy<T, P>& NonterminalProxy<T, P>::operator=(T o)
{
    std::vector<SymbolDescriptor> rule = addRuleHead();
    rule.push_back({gf->getTerminalSymbolAndInsert(o)});
    gf->g.addRule(rule.begin(), rule.end());
    return *this;
}

template<typename T, typename P>
NonterminalProxy<T, P>&  NonterminalProxy<T, P>::operator=(NonterminalProxy<T, P>& o)
{
    std::vector<SymbolDescriptor> rule = addRuleHead();
    rule.push_back({gf->getNonterminalAndInsert(o.id)});
    o.gf = this->gf;
    gf->g.addRule(rule.begin(), rule.end());
    return *this;
}

template<typename T, typename P>
NonterminalProxy<T, P>&  NonterminalProxy<T, P>::operator=(EpsilonSymbol )
{
    std::vector<SymbolDescriptor> rule = addRuleHead();
    gf->g.addRule(rule.begin(), rule.end());
    return *this;
}

template<typename T, typename P>
NonterminalProxy<T, P>& NonterminalProxy<T, P>::operator=(const SemanticRuleType<P>& func)
{
    std::vector<SymbolDescriptor> rule = addRuleHead();
    rule.push_back(gf->g.addSemanticRuleFunc(func));
    gf->g.addRule(rule.begin(), rule.end());
    return *this;

}




}// namesapce lz

#endif /* LZ_COMPILER_NOTERMINAL_PROXY_H_ */
