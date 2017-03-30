/*
 * noterminal_proxy.h
 *
 *  Created on: 2017年3月30日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NOTERMINAL_PROXY_H_
#define LZ_COMPILER_NOTERMINAL_PROXY_H_



namespace lz{

struct EpsilonSymbol {} eps;

template<typename T, typename P>
struct NonterminalProxy;


template<typename T, typename P>
struct GrammarFactory
{
    Grammar<P> g;
    std::map<T, SymbolDescriptor> terminalMap;
    std::map<int, SymbolDescriptor> nonterminalMap;


    GrammarFactory(){}
    GrammarFactory(NonterminalProxy<T, P>& startSymbol);

    SymbolDescriptor getNonterminalAndInsert(int nonterminalProxyId)
    {
        if(nonterminalMap.count(nonterminalProxyId))
        {
            return nonterminalMap[nonterminalProxyId];
        }
        else
        {
            return nonterminalMap[nonterminalProxyId] = g.addNonterminal();
        }
    }

    std::map<SymbolDescriptor, T> calculateTerminalNames() const
    {
        std::map<SymbolDescriptor, T> ans;
        for(auto p: terminalMap)
        {
            ans[p.second] = p.first;
        }
        return ans;
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


    void connectStartNonterminal(NonterminalProxy<T, P>&);


};


    namespace Detail {


    template<typename T, typename P>
    struct UserSymbol;

    template<typename T, typename P>
    using UserRuleBody = std::vector<UserSymbol<T, P>>;

    }




template<typename T, typename P>
struct NonterminalProxy
{
private:
    static int counter;
public:
    SymbolDescriptor id;
    SemanticRuleType<P> action;
    GrammarFactory<T, P>* gf;


    NonterminalProxy(SemanticRuleType<P> func = SemanticRuleType<P>(), GrammarFactory<T, P>* gf = nullptr):
         action(func), gf(gf)
    {
        id = counter ++;
    }


    NonterminalProxy(const NonterminalProxy<T, NoProperty>&other):
        id(other.id), action(SemanticRuleType<P>()), gf(nullptr)
    {
    }


    template<typename F>
    NonterminalProxy& operator[](F f)
    {
        action = f;
        return *this;
    }

    std::vector<SymbolDescriptor> addRuleHeadAction();
    void cleanAction()
    {
        action = nullptr;
    }


    template<typename P2>
    NonterminalProxy& operator=(const Detail::UserRuleBody<T, P2>& urb);
    NonterminalProxy& operator=(T o);
    NonterminalProxy& operator=(NonterminalProxy& o);
    NonterminalProxy& operator=(EpsilonSymbol );
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




    namespace Detail {

    enum class UserSymbolType
    {
        Nonterminal,
        Terminal,
    };

    template<typename T, typename P>
    struct UserSymbol
    {
        UserSymbol(NonterminalProxy<T, P>* nonterminal):
            type(UserSymbolType::Nonterminal), nonterminal(nonterminal) {}

        UserSymbol(T terminal):
            type(UserSymbolType::Terminal), terminal(terminal) { }

        UserSymbolType type;
        NonterminalProxy<T, P> *nonterminal = nullptr;
        T terminal = T();

    };


    }





template<typename T, typename P>
auto operator>>(NonterminalProxy<T, P>& a, NonterminalProxy<T, P>& b)
{
    Detail::UserRuleBody<T, P> ans{ &a, &b };
    return ans;
}

template<typename T, typename P>
auto operator>>(NonterminalProxy<T, P>& a, T b)
{
    Detail::UserRuleBody<T, P> ans{ &a, b};

    return ans;
}

template<typename T, typename P>
auto operator>>(T a, NonterminalProxy<T, P>& b)
{
    Detail::UserRuleBody<T, P>ans{a, &b };
    return ans;
}


template<typename T, typename P>
auto operator>>(Detail::UserRuleBody<T, P> a, T b)
{
    a.push_back(Detail::UserSymbol<T, P>(b));
    return a;
}

template<typename T, typename P>
auto operator>>(Detail::UserRuleBody<T, P> a, NonterminalProxy<T, P>& b)
{
    a.push_back(&b);

    return a;
}

template<typename T>
auto operator>>(EpsilonSymbol, T b)
{
    return Detail::UserRuleBody<T, NoProperty>{ b };
}


template<typename T, typename P>
std::vector<SymbolDescriptor> NonterminalProxy<T, P>::addRuleHeadAction()
{
    std::vector<SymbolDescriptor> ans;

    ans.push_back(gf->getNonterminalAndInsert(id));
    if(action)
    {
        ans.push_back(gf->g.addSemanticRuleFunc(action));
        cleanAction();
    }
    return ans;
}

template<typename T, typename P>
template<typename P2>
NonterminalProxy<T, P>& NonterminalProxy<T, P>::operator=(const Detail::UserRuleBody<T, P2>& o)
{
    std::vector<SymbolDescriptor> rule = addRuleHeadAction();
    for(Detail::UserSymbol<T, P> ch: o)
    {
        if(ch.type == Detail::UserSymbolType::Nonterminal)
        {
            rule.push_back(gf->getNonterminalAndInsert(ch.nonterminal->id));
            if constexpr(std::is_same<P, P2>::value)
            {
                if(ch.nonterminal->action)
                {
                    rule.push_back(gf->g.addSemanticRuleFunc(ch.nonterminal->action));
                    ch.nonterminal->cleanAction();

                }
            }
            ch.nonterminal->gf = this->gf;
        }
        else if(ch.type == Detail::UserSymbolType::Terminal)
        {
            rule.push_back(gf->getTerminalSymbolAndInsert(ch.terminal));
        }
    }
    gf->g.addRule(rule.begin(), rule.end());


    return *this;
}



template<typename T, typename P>
NonterminalProxy<T, P>& NonterminalProxy<T, P>::operator=(T o)
{
    std::vector<SymbolDescriptor> rule = addRuleHeadAction();
    rule.push_back({gf->getTerminalSymbolAndInsert(o)});
    gf->g.addRule(rule.begin(), rule.end());
    return *this;
}

template<typename T, typename P>
NonterminalProxy<T, P>&  NonterminalProxy<T, P>::operator=(NonterminalProxy<T, P>& o)
{
    std::vector<SymbolDescriptor> rule = addRuleHeadAction();
    rule.push_back({gf->getNonterminalAndInsert(o.id)});
    if(o.action)
    {
        rule.push_back(gf->g.addSemanticRuleFunc(o.action));
        o.cleanAction();
    }
    o.gf = this->gf;

    gf->g.addRule(rule.begin(), rule.end());
    return *this;
}

template<typename T, typename P>
NonterminalProxy<T, P>&  NonterminalProxy<T, P>::operator=(EpsilonSymbol )
{
    std::vector<SymbolDescriptor> rule = addRuleHeadAction();
    gf->g.addRule(rule.begin(), rule.end());
    return *this;
}



}// namesapce lz

#endif /* LZ_COMPILER_NOTERMINAL_PROXY_H_ */
