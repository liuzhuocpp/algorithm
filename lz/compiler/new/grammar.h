/*
 * grammar.h
 *
 *  Created on: 2017年3月11日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NEW_GRAMMAR_H_
#define LZ_COMPILER_NEW_GRAMMAR_H_

#include <lz/iterator.h>
#include <lz/compiler/new/symbol.h>

namespace lz {


template<typename P>
using ActionType = std::function<void(const std::vector<P>&, P&)>;


template<typename P>
struct Grammar: private std::vector<std::vector<std::vector<SymbolDescriptor>>>
{
private:
    using RuleBody = std::vector<SymbolDescriptor>;
    using RuleBodyUnion = std::vector<RuleBody>;
    using Base = std::vector<RuleBodyUnion>;
    std::vector<ActionType<P>> actions;
    using  std::vector<std::vector<std::vector<SymbolDescriptor>>>::vector;
public:

    using SizeType = std::size_t;

    using NonterminalProperties = P;


    struct RuleDescriptor
    {
        using RuleBodyIndexType = typename std::vector<RuleDescriptor>::difference_type;
        SymbolDescriptor head;
        RuleBodyIndexType body;

        RuleDescriptor():head(SymbolDescriptor()), body(RuleBodyIndexType()) {}

        RuleDescriptor(SymbolDescriptor A, RuleBodyIndexType i):head(A), body(i) {}
        friend bool operator== (RuleDescriptor a, RuleDescriptor b)
        {
            return a.head == b.head && a.body == b.body;
        }

        friend bool operator!= (RuleDescriptor a, RuleDescriptor b)
        {
            return !(a == b);
        }
    };

    // 假定每个非终结符都至少有一个RuleBody，否则程序将不会运行正确，有待改进
    struct RuleIterator:IteratorFacade<RuleIterator, std::forward_iterator_tag, RuleDescriptor>
    {
        RuleDescriptor rd;
        const Grammar* g;
        RuleIterator():rd(RuleDescriptor()), g(nullptr) { }

        RuleIterator(RuleDescriptor rd, const Grammar &g):
            rd(rd), g(&g) { }

        RuleIterator& operator++()
        {
            rd.body++;
            if(rd.body == (*g)[rd.head].size())
            {
                rd.head ++;
                rd.body = 0;
            }
            return *this;
        }
        RuleDescriptor operator*() const
        {
            return rd;
        }

        friend bool operator==(const RuleIterator& a, const RuleIterator &b)
        {
            return a.rd == b.rd && a.g == b.g;
        }
    };


    struct RuleSymbolIterator:IteratorFacade<RuleSymbolIterator, std::bidirectional_iterator_tag, SymbolDescriptor>
    {
        SymbolDescriptor head;
        const RuleBody* body;
        int cur;

        RuleSymbolIterator(){}

        RuleSymbolIterator(SymbolDescriptor head, const RuleBody& body, int cur):
            head(head), body(&body), cur(cur){}

        RuleSymbolIterator& operator++()
        {
            cur++;
            if(cur < body->size())
            {
                SymbolDescriptor s = (*body)[cur];
                if(!isNonterminal(s) && !isTerminal(s))
                {
                    cur ++;
                }
            }
            return *this;
        }

        RuleSymbolIterator& operator--()
        {
            cur --;
            if(cur >= 0)
            {
                SymbolDescriptor s = (*body)[cur];
                if(!isNonterminal(s) && !isTerminal(s))
                {
                    cur --;
                }

            }
            return *this;
        }

        SymbolDescriptor operator*() const
        {
            if(cur == -1) return head;
            else return (*body)[cur];
        }

        friend bool operator==(const RuleSymbolIterator& a, const RuleSymbolIterator &b)
        {
            return a.head == b.head && a.body == b.body && a.cur == b.cur;
        }
    };

    // rd 与it 所指向的字符必须是非终结符
    SymbolDescriptor calculateAction(RuleDescriptor rd, RuleSymbolIterator it) const
    {
        auto ruleRange = ruleSymbols(rd);
        int id = it.cur;
        const RuleBody& body =  (*this)[rd.head][rd.body];
        id++;

        if(id < body.size() && isAction(body[id]))
        {
            return body[id];
        }
        else return NullSymbol;
    }


    // s 代表一个action symbol
    ActionType<P> getActionFunc(SymbolDescriptor s) const
    {
        return actions[s - ActionSymbolBegin];
    }

    SizeType actionsNumber() const
    {
        return actions.size();
    }



    SymbolDescriptor addActionFunc(ActionType<P> f)
    {
        actions.push_back(f);
        return actions.size() + ActionSymbolBegin - 1;
    }

    SymbolDescriptor addNonterminal()
    {
        this->push_back({});
        return this->size() - 1;
    }

    void clear()
    {
        Base::clear();
        actions.clear();
    }

    void assginNonterminalsNumber(int n)
    {
        clear();
        this->resize(n);
    }
    SizeType nonterminalsNumber() const
    {
        return this->size();
    }

    template<typename InputIterator>
    void addRule(InputIterator first, InputIterator last)
    {
        Grammar & g = *this;
        SymbolDescriptor head = *first++;
        g[head].push_back({});
        for(;first != last; ++ first)
        {
            g[head].back().push_back(*first);
        }
    }


    IteratorRange<RuleSymbolIterator> ruleSymbols(RuleDescriptor rd) const
    {

        const RuleBody& body = (*this)[rd.head][rd.body];
        return makeIteratorRange(
                RuleSymbolIterator(rd.head, body, -1), // -1 表示head
                RuleSymbolIterator(rd.head, body, body.size())
                );
    }

    std::pair<RuleIterator, RuleIterator> rules() const
    {
        return std::make_pair(
            RuleIterator(RuleDescriptor(0, 0), *this),
            RuleIterator(RuleDescriptor(size(), 0), *this));
    }

    std::pair<RuleIterator, RuleIterator> outRules(SymbolDescriptor s) const
    {
        return std::make_pair(
            RuleIterator(RuleDescriptor(s, 0), *this),
            RuleIterator(RuleDescriptor(s, (*this)[s].size()), *this));
    }

};




//struct NoProperty{};

struct EpsilonSymbol {} eps;



template<typename T, typename P>
struct NonterminalProxy;




template<typename T, typename P>
struct GrammarFactory
{
    Grammar<P> g;
    std::map<T, SymbolDescriptor> terminalMap;

    std::map<SymbolDescriptor, T> calculateTerminalNames()
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
            return terminalMap[ch];
        }
        else
        {
            SymbolDescriptor newId = TerminalSymbolBegin + terminalMap.size();
            terminalMap[ch] = newId;
            return newId;

        }
    }

    template<std::size_t N>
    std::array<NonterminalProxy<T, P>, N> makeNonternimalProxies();


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
    SymbolDescriptor id;
    ActionType<P> action;
    GrammarFactory<T, P>* gf;


    NonterminalProxy(SymbolDescriptor id = 0, ActionType<P> func = ActionType<P>(), GrammarFactory<T, P>* gf = nullptr):
        id(id), action(func), gf(gf)
    {
    }


    NonterminalProxy(const NonterminalProxy<T, NoProperty>&other):
        id(other.id), action(ActionType<P>()), gf(nullptr)
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
    NonterminalProxy& operator=(NonterminalProxy o);
    NonterminalProxy& operator=(EpsilonSymbol );
};


    namespace Detail {

    enum class UserSymbolType
    {
        Nonterminal,
        Terminal,
    };

    template<typename T, typename P>
    struct UserSymbol
    {
        UserSymbol(NonterminalProxy<T, P> nonterminal):
            type(UserSymbolType::Nonterminal), nonterminal(nonterminal) {}

        UserSymbol(T terminal):
            type(UserSymbolType::Terminal), terminal(terminal) { }

        UserSymbolType type;
        NonterminalProxy<T, P> nonterminal = NonterminalProxy<T, P>();
        T terminal = T();

    };


    }





template<typename T, typename P>
auto operator>>(NonterminalProxy<T, P>& a, NonterminalProxy<T, P>& b)
{
    Detail::UserRuleBody<T, P> ans{ Detail::UserSymbol<T, P>(a), Detail::UserSymbol<T, P>(b) };
    a.cleanAction();
    b.cleanAction();
    return ans;
}

template<typename T, typename P>
auto operator>>(NonterminalProxy<T, P>& a, T b)
{
    Detail::UserRuleBody<T, P> ans{ Detail::UserSymbol<T, P>(a), Detail::UserSymbol<T, P>(b) };
    a.cleanAction();
    return ans;
}

template<typename T, typename P>
auto operator>>(T a, NonterminalProxy<T, P>& b)
{
    Detail::UserRuleBody<T, P>ans{ Detail::UserSymbol<T, P>(a), Detail::UserSymbol<T, P>(b) };
    b.cleanAction();
    return ans;
}


template<typename T, typename P>
auto operator>>(Detail::UserRuleBody<T, P> a, T b)
{
    return a;
}

template<typename T, typename P>
auto operator>>(Detail::UserRuleBody<T, P> a, NonterminalProxy<T, P>& b)
{
    a.push_back(Detail::UserSymbol<T, P>(b));
    b.cleanAction();

    return a;
}

template<typename T>
auto operator>>(EpsilonSymbol, T b)
{
    return Detail::UserRuleBody<T, NoProperty>{ Detail::UserSymbol<T, NoProperty>(b) };
}


template<typename T, typename P>
std::vector<SymbolDescriptor> NonterminalProxy<T, P>::addRuleHeadAction()
{
    std::vector<SymbolDescriptor> ans;
    ans.push_back(id);
    if(action)
    {
        ans.push_back(gf->g.addActionFunc(action));
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
            rule.push_back(ch.nonterminal.id);
            if constexpr(std::is_same<P, P2>::value)
            {
                if(ch.nonterminal.action)
                {
                    rule.push_back(gf->g.addActionFunc(ch.nonterminal.action));
                }

            }
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
NonterminalProxy<T, P>&  NonterminalProxy<T, P>::operator=(NonterminalProxy<T, P> o)
{
    std::vector<SymbolDescriptor> rule = addRuleHeadAction();
    rule.push_back({o.id});
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

template<typename T, typename P>
template<std::size_t N>
std::array<NonterminalProxy<T, P>, N>

GrammarFactory<T, P>::makeNonternimalProxies()
{
    std::array<NonterminalProxy<T, P>, N> ans;
    using DiffType = typename std::array<NonterminalProxy<T, P>, N>::difference_type;
    for(DiffType i: lz::irange(N))
    {
        ans[i].id = i;
        ans[i].gf = this;
    }


    terminalMap.clear();
    g.clear();
    g.assginNonterminalsNumber(N);

    return ans;
}






















// For output----------------------------------------------------
template<typename T>
struct SymbolForOutput
{
    const SymbolDescriptor& symbol;
    const std::vector<std::string>& nonterminalNames;
    std::map<SymbolDescriptor, T> ternimalNames;

    int getNameLength() const
    {
        if(isNonterminal(symbol))
        {
            auto i = symbol;
            if(i < nonterminalNames.size() && !nonterminalNames[i].empty())
                return nonterminalNames[i].size();
            else
                return std::to_string(i).size();
        }
        else
        {
            return 1; // Could be more improved
        }
    }



    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, SymbolForOutput so)
    {
        const SymbolDescriptor& s = so.symbol;

        if(isTerminal(s))
        {
            os << (so.ternimalNames)[s];
        }
        else if(isNonterminal(s))
        {
            auto i = s;

            if(i < so.nonterminalNames.size() && !so.nonterminalNames[i].empty())
                os << so.nonterminalNames[i];
            else
                os << std::to_string(i);
        }
        else if(isAction(s))
        {
            os << '#'; // action
        }
        else if(isEndTag(s))
        {
            os << "$";
        }
        else if(isEmptyString(s))
        {
            os <<"eps";
        }
        else
        {

        }

        return os;
    }

};

template<typename Iterator, typename T>
struct SymbolRangeForOutput
{
    Iterator first, last;
    const std::vector<std::string>& nonterminalNames;
    const std::map<SymbolDescriptor, T>& ternimalNames;


    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const SymbolRangeForOutput& sr)
    {
        for(auto s: makeIteratorRange(sr.first, sr.last))
        {
            os << SymbolForOutput<T>{s, sr.nonterminalNames, sr.ternimalNames};
            os << " " ;
        }
        return os;
    }

};

template<typename Iterator, typename T>
struct RuleForOutput
{

    Iterator first, last;

    const std::vector<std::string>& nonterminalNames;
    const std::map<SymbolDescriptor, T>& ternimalNames;

    int leftTotalWidth = -1;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const RuleForOutput& ro)
    {

        SymbolForOutput<T> head {*ro.first, ro.nonterminalNames, ro.ternimalNames};
        os << head;
        if(ro.leftTotalWidth != -1)
        {
            os << std::string(std::max(ro.leftTotalWidth - head.getNameLength(), 0), ' ');
        }
        auto nextFirst = ro.first;

        os << "->" << SymbolRangeForOutput<Iterator, T>{++nextFirst, ro.last, ro.nonterminalNames, ro.ternimalNames };

        return os;
    }

};



template<typename T, typename Grammar>
struct GrammerForOutput
{
    const Grammar& g;

    const std::vector<std::string>& nonterminalNames;
    const std::map<SymbolDescriptor, T>& ternimalNames;

    bool needLeftJustified = true;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const GrammerForOutput&  g)
    {
        int maxLength = 0;


        for(auto rd: g.g.rules())
        {
            maxLength = std::max(maxLength, SymbolForOutput<T>{*g.g.ruleSymbols(rd).first,
                g.nonterminalNames, g.ternimalNames}.getNameLength());
        }

        for(auto rd: g.g.rules())
        {
            auto ruleRange = g.g.ruleSymbols(rd);

            os << RuleForOutput<decltype(ruleRange.first), T>{ruleRange.first, ruleRange.second, g.nonterminalNames, g.ternimalNames,
                g.needLeftJustified ? maxLength : -1};
            os << "\n";
        }


        return os;
    }

};



}// namesapce lz

#endif /* LZ_COMPILER_NEW_GRAMMAR_H_ */
