/*
 * grammar.h
 *
 *  Created on: 2017年3月11日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NEW_GRAMMAR_H_
#define LZ_COMPILER_NEW_GRAMMAR_H_

#include <lz/iterator.h>

namespace lz {




using Symbol = long;


constexpr Symbol TerminalSymbolBegin = std::numeric_limits<Symbol>::min();
constexpr Symbol ActionSymbolBegin = std::numeric_limits<Symbol>::min() >> 1;
constexpr Symbol EmptyStringSymbol = -2;
constexpr Symbol EndTagSymbol = -1;
//从TerminalSymbolBegin开始递增是终结符号范围



bool isNonterminal(Symbol s)
{
    return s >= 0;
}

bool isTerminal(Symbol s)
{
    return s < ActionSymbolBegin;
}
bool isAction(Symbol s)
{
    return s >= ActionSymbolBegin && s < EmptyStringSymbol;
}

bool isEmptyString(Symbol s)
{
    return s == EmptyStringSymbol;
}



bool isEndTag(Symbol s)
{
    return s == EndTagSymbol;
}


struct RuleBody: std::vector<Symbol>
{
    using std::vector<Symbol>::vector;
};

struct RuleBodyUnion: std::vector<RuleBody>
{
    using std::vector<RuleBody>::vector;
};

struct Grammar: std::vector<RuleBodyUnion>
{
    using std::vector<RuleBodyUnion>::vector;

    struct RuleDescriptor
    {
        using RuleBodyIndexType = std::vector<RuleDescriptor>::difference_type;
        Symbol A;
        RuleBodyIndexType i;

        RuleDescriptor():A(Symbol()), i(RuleBodyIndexType()) {}

        RuleDescriptor(Symbol A, RuleBodyIndexType i):A(A), i(i) {}
        friend bool operator== (RuleDescriptor a, RuleDescriptor b)
        {
            return a.A == b.A && a.i == b.i;
        }

        friend bool operator!= (RuleDescriptor a, RuleDescriptor b)
        {
            return !(a == b);
        }
    };


    struct RuleSymbolDescriptor
    {
        RuleDescriptor ruleDescriptor;
        int pos;
        RuleSymbolDescriptor(){}
        RuleSymbolDescriptor(RuleDescriptor rd, int j):
            ruleDescriptor(rd), pos(j){}

    };

    template<typename Iterator>
    int getNonterminalsNumber(Iterator first, Iterator last) const
    {
        int ans = 0;
        for(;first != last; first++)
        {
            Symbol s = *first;
            if(isNonterminal(s)) ans ++;
        }
        return ans;
    }



//
//    // need improve
//    Symbol ruleHeadAction(RuleDescriptor rd)
//    {
//        const RuleBody &body = ruleBody(rd);
//        if(!body.empty() && isAction(body[0]))
//        {
//            return body[0];
//        }
//        else return EmptyStringSymbol;
//    }
//
//
//    // need improve
//    Symbol ruleBodySymbolAction(RuleDescriptor rd, int j)
//    {
//        const RuleBody &body = ruleBody(rd);
//        if(j + 1 < body.size() && isAction(body[j + 1]))
//        {
//            return body[j + 1];
//        }
//        else
//        {
//            return EmptyStringSymbol;
//        }
//    }


    Symbol ruleHead(RuleDescriptor rd) const
    {
        return rd.A;
    }
    const RuleBody& ruleBody(RuleDescriptor rd) const
    {
        return (*this)[rd.A][rd.i];
    }

    // 假定每个非终结符都至少有一个RuleBody，否则程序将不会运行正确，有待改进
    struct RuleIterator:IteratorFacade<RuleIterator, std::forward_iterator_tag, RuleDescriptor>
    {
        RuleDescriptor rd;
        const Grammar* g;
        RuleIterator():rd(RuleDescriptor()), g(nullptr)
        {

        }

        RuleIterator(RuleDescriptor rd, const Grammar &g):
            rd(rd), g(&g)
        {

        }


        RuleIterator& operator++()
        {
            rd.i++;
            if(rd.i == (*g)[rd.A].size())
            {
                rd.A ++;
                rd.i = 0;
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




    std::pair<RuleIterator, RuleIterator> rules() const
    {
        return std::make_pair(
            RuleIterator(RuleDescriptor(0, 0), *this),
            RuleIterator(RuleDescriptor(size(), 0), *this));
    }


};




struct EpsilonSymbol {} eps;
struct NoProperty{};


template<typename T, typename P>
struct UserNonterminal;



template<typename T, typename P>
struct GrammarFactory
{
    Grammar g;
    std::map<T, Symbol> terminalMap;
    using ActionType = std::function<void(std::vector<P>, P&)>;
    std::vector<ActionType> actions;


    Symbol getActionSymbol()
    {
        return actions.size() + ActionSymbolBegin;
    }

    std::map<Symbol, T> calculateTerminalNames()
    {
        std::map<Symbol, T> ans;
        for(auto p: terminalMap)
        {
            ans[p.second] = p.first;
        }
        return ans;
    }

    Symbol getTerminalSymbol(T ch)
    {
        if(terminalMap.count(ch))
        {
            return terminalMap[ch];
        }
        else
        {
            Symbol newId = TerminalSymbolBegin + terminalMap.size();
            terminalMap[ch] = newId;
            return newId;

        }
    }

    template<std::size_t N>
    std::array<UserNonterminal<T, P>, N> makeNonternimals();


};




template<typename T, typename P>
struct UserSymbol;

template<typename T, typename P>
using UserRuleBody = std::vector<UserSymbol<T, P>>;


template<typename T, typename P>
struct UserNonterminal
{
    using ActionType = std::function<void(std::vector<P>, P&)>;

    UserNonterminal(Symbol id = 0, ActionType func = ActionType(), GrammarFactory<T, P>* gf = nullptr):
        id(id), action(func), gf(gf)
    {

    }


    UserNonterminal(const UserNonterminal<T, NoProperty>&other):
        id(other.id), action(ActionType()), gf(nullptr)
    {

    }

    Symbol id;
    std::function<void(std::vector<P>, P&)> action;
    GrammarFactory<T, P>* gf;

    template<typename F>
    UserNonterminal& operator[](F f)
    {
        std::cout << "CAO action" << std::endl;
        action = f;
        return *this;
    }

    template<typename P2>
    RuleBody calculateRuleBodyWithRuleHeadAction(UserRuleBody<T, P2>);


    template<typename P2>
    UserNonterminal& operator=(const UserRuleBody<T, P2>& urb);
    UserNonterminal& operator=(T o);
    UserNonterminal& operator=(UserNonterminal o);
    UserNonterminal& operator=(EpsilonSymbol );


};






enum class UserSymbolType
{
    Nonterminal,
    Terminal,
    EmptyString,
};


template<typename T, typename P>
struct UserSymbol
{
    UserSymbol(UserNonterminal<T, P> nonterminal):
        type(UserSymbolType::Nonterminal), nonterminal(nonterminal)
    {

    }

    UserSymbol(T terminal):
        type(UserSymbolType::Terminal), terminal(terminal)
    {

    }

    UserSymbol():
        type(UserSymbolType::EmptyString)
    {

    }


    UserSymbolType type;
    UserNonterminal<T, P> nonterminal = UserNonterminal<T, P>();
    T terminal = T();

};




//template<typename P1, typename P2>
//using PropertiesType = std::conditional_t<std::is_same<P1, NoProperty>::value, P2, P1>;


template<typename T, typename P, typename P2>
RuleBody convertToRuleBody(UserRuleBody<T, P> a, GrammarFactory<T, P2> & gf)
{
    RuleBody ans;
    for(UserSymbol<T, P> ch: a)
    {
        if(ch.type == UserSymbolType::Nonterminal)
        {
            ans.push_back(ch.nonterminal.id);

            if constexpr(std::is_same<P, P2>::value)
            {
                if(ch.nonterminal.action)
                {
                    ans.push_back(ActionSymbolBegin + gf.actions.size());
                    gf.actions.push_back(ch.nonterminal.action);

                }

            }
        }
        else if(ch.type == UserSymbolType::Terminal)
        {
            ans.push_back(gf.getTerminalSymbol(ch.terminal));
        }
    }
    return ans;
}



template<typename T, typename P>
auto operator>>(UserNonterminal<T, P> a, UserNonterminal<T, P> b)
{
    return UserRuleBody<T, P>{ UserSymbol<T, P>(a), UserSymbol<T, P>(b) };
}

template<typename T, typename P>
auto operator>>(UserNonterminal<T, P> a, T b)
{
    return UserRuleBody<T, P>{ UserSymbol<T, P>(a), UserSymbol<T, P>(b) };
}

template<typename T, typename P>
auto operator>>(T a, UserNonterminal<T, P> b)
{
    return UserRuleBody<T, P>{ UserSymbol<T, P>(a), UserSymbol<T, P>(b) };
}


template<typename T, typename P>
auto operator>>(UserRuleBody<T, P> a, T b)
{
    a.push_back(UserSymbol<T, P>(b) );
    return a;
}

template<typename T, typename P>
auto operator>>(UserRuleBody<T, P> a, UserNonterminal<T, P> b)
{
    a.push_back(UserSymbol<T, P>(b));
    return a;
}


template<typename T>
auto operator>>(EpsilonSymbol, T b)
{
    return UserRuleBody<T, NoProperty>{ UserSymbol<T, NoProperty>(b) };
}




template<typename T, typename P>
template<typename P2>
RuleBody UserNonterminal<T, P>::calculateRuleBodyWithRuleHeadAction(UserRuleBody<T, P2> o)
{
    RuleBody ans = convertToRuleBody(o, *gf);
    if(action)
    {
        ans.insert(ans.begin(), gf->getActionSymbol());
        gf->actions.push_back(action);
    }
    return ans;
}


template<typename T, typename P>
template<typename P2>
UserNonterminal<T, P>& UserNonterminal<T, P>::operator=(const UserRuleBody<T, P2>& o)
{
    gf->g[id].push_back(calculateRuleBodyWithRuleHeadAction(o));
    return *this;
}



template<typename T, typename P>
UserNonterminal<T, P>& UserNonterminal<T, P>::operator=(T o)
{
//    RuleBody rule
    gf->g[id].push_back({gf->getTerminalSymbol(o)});
    return *this;
}

template<typename T, typename P>
UserNonterminal<T, P>&  UserNonterminal<T, P>::operator=(UserNonterminal<T, P> o)
{
    gf->g[id].push_back({o.id});
    return *this;
}

template<typename T, typename P>
UserNonterminal<T, P>&  UserNonterminal<T, P>::operator=(EpsilonSymbol )
{
    gf->g[id].push_back({});

    if(action)
    {
        gf->g[id].back().push_back(gf->getActionSymbol());
//        ans.insert(ans.begin(), gf->getActionSymbol());
        gf->actions.push_back(action);
    }

    return *this;
}








template<typename T, typename P>
template<std::size_t N>
std::array<UserNonterminal<T, P>, N>

GrammarFactory<T, P>::makeNonternimals()
{
    std::array<UserNonterminal<T, P>, N> ans;
    using DiffType = typename std::array<UserNonterminal<T, P>, N>::difference_type;
    for(DiffType i: lz::irange(N))
    {
        ans[i].id = i;
        ans[i].gf = this;
    }


    terminalMap.clear();
    g.resize(N);
    actions.clear();
    return ans;
}






















// For output----------------------------------------------------
template<typename T>
struct SymbolForOutput
{
    const Symbol& symbol;
    const std::vector<std::string>& nonterminalNames;
    std::map<Symbol, T> ternimalNames;

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
        const Symbol& s = so.symbol;

//        if(s.isEmptyString())
//        {
//            os << "#"  ; // 暂时用#号代替空字符串字符
//        }
//        else
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

template<typename T>
struct RuleBodyForOutput
{
    const RuleBody &ruleBody;

    const std::vector<std::string>& nonterminalNames;
    const std::map<Symbol, T>& ternimalNames;


    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const RuleBodyForOutput& r)
    {
        for(auto s: r.ruleBody)
        {
            os << SymbolForOutput<T>{s, r.nonterminalNames, r.ternimalNames};
            os << " " ;
        }
        return os;
    }

};

template<typename T>
struct RuleForOutput
{
    Symbol ruleHead;
    const RuleBody &ruleBody;

    const std::vector<std::string>& nonterminalNames;
    const std::map<Symbol, T>& ternimalNames;

    int leftTotalWidth = -1;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const RuleForOutput& ro)
    {

        SymbolForOutput<T> head {ro.ruleHead, ro.nonterminalNames, ro.ternimalNames};
        os << head;
        if(ro.leftTotalWidth != -1)
        {
            os << std::string(std::max(ro.leftTotalWidth - head.getNameLength(), 0), ' ');
        }
        os << "->" << RuleBodyForOutput<T>{ro.ruleBody, ro.nonterminalNames, ro.ternimalNames};

        return os;
    }

};


template<typename T>
struct RuleUnionForOutput
{
    const Symbol A;
    const RuleBodyUnion& ru;
//    const std::vector<std::string>& names;
    const std::vector<std::string>& nonterminalNames;
    const std::map<Symbol, T>& ternimalNames;


    int  leftTotalWidth = -1; // 确定左边终结符号加上空格之后，总共的宽度

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const RuleUnionForOutput&  ruo)
    {

        for(auto i: irange(ruo.ru.size()))
        {
            os << RuleForOutput<T>{ruo.A, ruo.ru[i], ruo.nonterminalNames,ruo.ternimalNames,  ruo.leftTotalWidth} << "\n";
        }

        return os;
    }

};



template<typename T>
struct GrammerForOutput
{
    const Grammar& g;

    const std::vector<std::string>& nonterminalNames;
    const std::map<Symbol, T>& ternimalNames;

    bool needLeftJustified = true;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const GrammerForOutput&  g)
    {
        int maxLength = 0;
        for(Symbol i: irange(g.g.size()))
        {
            maxLength = std::max(maxLength, SymbolForOutput<T>{i, g.nonterminalNames, g.ternimalNames}.getNameLength());
        }

        for(Symbol i: irange(g.g.size()))
        {
            os << RuleUnionForOutput<T>{i, g.g[i], g.nonterminalNames, g.ternimalNames,
                g.needLeftJustified ? maxLength : -1};
        }


        return os;
    }

};



}// namesapce lz

#endif /* LZ_COMPILER_NEW_GRAMMAR_H_ */
