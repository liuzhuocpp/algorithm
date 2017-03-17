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




using SymbolDescriptor = long;


constexpr SymbolDescriptor TerminalSymbolBegin = std::numeric_limits<SymbolDescriptor>::min();
constexpr SymbolDescriptor ActionSymbolBegin = std::numeric_limits<SymbolDescriptor>::min() >> 1;
constexpr SymbolDescriptor ActionSymbolEnd = -10;


constexpr SymbolDescriptor NullSymbol = -3;
constexpr SymbolDescriptor EmptyStringSymbol = -2;
constexpr SymbolDescriptor EndTagSymbol = -1;
//从TerminalSymbolBegin开始递增是终结符号范围



bool isNonterminal(SymbolDescriptor s)
{
    return s >= 0;
}

bool isTerminal(SymbolDescriptor s)
{
    return s < ActionSymbolBegin;
}
bool isAction(SymbolDescriptor s)
{
    return s >= ActionSymbolBegin && s < EmptyStringSymbol;
}

bool isEmptyString(SymbolDescriptor s)
{
    return s == EmptyStringSymbol;
}



bool isEndTag(SymbolDescriptor s)
{
    return s == EndTagSymbol;
}


struct RuleBody: std::vector<SymbolDescriptor>
{
    using std::vector<SymbolDescriptor>::vector;
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



    struct RuleSymbolDescriptor
    {
        RuleDescriptor rule;
        int id; // 当id== -1 表示代表此rule的head
        RuleSymbolDescriptor(){}
        RuleSymbolDescriptor(RuleDescriptor rd, int j):
            rule(rd), id(j){}

        friend bool operator== (RuleSymbolDescriptor a, RuleSymbolDescriptor b)
        {
            return a.rule == b.rule && a.id == b.id;
        }

        friend bool operator!= (RuleSymbolDescriptor a, RuleSymbolDescriptor b)
        {
            return !(a == b);
        }
    };


    struct RuleSymbolIterator:IteratorFacade<RuleSymbolIterator, std::bidirectional_iterator_tag, SymbolDescriptor>
    {
        RuleSymbolDescriptor rsd;
        const Grammar* g;
        RuleSymbolIterator():rsd(RuleSymbolDescriptor()), g(nullptr){}

        RuleSymbolIterator(RuleSymbolDescriptor rd, const Grammar &g):
            rsd(rd), g(&g){}

        RuleSymbolIterator& operator++()
        {
            rsd.id ++;
            for(;rsd.id < (*g)[rsd.rule.head][rsd.rule.body].size(); rsd.id++)
            {
                SymbolDescriptor s = (*g)[rsd.rule.head][rsd.rule.body][rsd.id];
                if(g->isTerminal(s) || g->isNonterminal(s))
                    break;
            }
            return *this;
        }

        RuleSymbolIterator& operator--()
        {

            rsd.id --;
            for(;rsd.id >= 0; rsd.id--)
            {
                SymbolDescriptor s = (*g)[rsd.rule.head][rsd.rule.body][rsd.id];
                if(g->isTerminal(s) || g->isNonterminal(s))
                    break;
            }
            return *this;
        }


        SymbolDescriptor operator*() const
        {
            return g->symbol(rsd);
        }

        friend bool operator==(const RuleSymbolIterator& a, const RuleSymbolIterator &b)
        {
            return a.rsd == b.rsd && a.g == b.g;
        }
    };



    RuleSymbolIterator makeRuleSymbolIterator(RuleSymbolDescriptor rsd) const
    {
        return RuleSymbolIterator(rsd, *this);
    }

    RuleDescriptor rule(RuleSymbolDescriptor rsd) const
    {
        return rsd.rule;
    }

    SymbolDescriptor symbol(RuleSymbolDescriptor rsd) const
    {
        if(rsd.id == -1) return rsd.rule.head;
        else return (*this)[rsd.rule.head][rsd.rule.body][rsd.id];
    }

    static RuleSymbolDescriptor nullRuleSymbol()
    {
        return RuleSymbolDescriptor(RuleDescriptor(-1, -1), -1);
    }

    int calculateActionId(RuleSymbolDescriptor rsd) const
    {

        rsd = calulateAction(rsd);
        if(rsd == nullRuleSymbol()) return -1;
        return symbol(rsd) - ActionSymbolBegin;
    }


    // rsd 必须指向一个action符号, 否则行为未定义
    int getActionId(SymbolDescriptor sd) const
    {
        return sd - ActionSymbolBegin;
    }

    SymbolDescriptor calculateAction(RuleDescriptor rd, RuleSymbolIterator it) const
    {
        RuleSymbolDescriptor rsd = calulateAction(it.rsd);
        if(rsd == nullRuleSymbol()) return NullSymbol;
        else return symbol(rsd);
    }



    // rsd 必须指向一个action符号, 否则行为未定义
    int getActionId(RuleSymbolDescriptor rsd) const
    {
        return symbol(rsd) - ActionSymbolBegin;
    }



    // rsd 必须对应某个非终结符，否则返回结果未定义
    //返回结果是rsd 对应 symbol 对应的action的index 值，文法中的对每一个nonterminal的action赋予了唯一index，
    // 返回是[0, n)其中n是文法中的所有nonterminal的action的数量

    RuleSymbolDescriptor calulateAction(RuleSymbolDescriptor rsd) const
    {
        const RuleBody& body =  (*this)[rsd.rule.head][rsd.rule.body];
        if(rsd.id == -1)
        {
            if(!body.empty() && isAction(body[0]))
            {
                rsd.id ++;
                return rsd;
            }
        }
        else
        {
            if(rsd.id + 1 < body.size() && isAction(body[rsd.id + 1]))
            {
                rsd.id ++;
                return rsd;
            }
        }
        return nullRuleSymbol();
    }

    // s 必须是一个 terminal Symbol,返回结果是ternimal Symbol 的index 值，文法中的对每一个terminal赋予了唯一index，
    // 返回是[0, n)其中n是文法中的所有terminal的数量
    //否则返回结果未定义
    int getTerminalId(SymbolDescriptor s) const
    {
        return s - TerminalSymbolBegin;
    }

    // s 必须是一个 nonterminal Symbol,返回结果是nonternimal Symbol 的index 值，文法中的对每一个nonterminal赋予了唯一index，
    // 返回是[0, n)其中n是文法中的所有nonterminal的数量
    //否则返回结果未定义
    int getNonterminalId(SymbolDescriptor sd) const
    {
        return sd;
    }


    bool isNonterminal(SymbolDescriptor s) const
    {
        return s >= 0;
    }

    bool isTerminal(SymbolDescriptor s) const
    {
        return s < ActionSymbolBegin;
    }
    bool isAction(SymbolDescriptor s) const
    {
        return s >= ActionSymbolBegin && s < EmptyStringSymbol;
    }


    bool isNonterminal(RuleSymbolDescriptor rsd) const
    {
        return isNonterminal(symbol(rsd));
    }

    bool isTerminal(RuleSymbolDescriptor rsd) const
    {
        return isTerminal(symbol(rsd));
    }

    bool isAction(RuleSymbolDescriptor rsd)  const
    {
        return isAction(symbol(rsd));
    }




    IteratorRange<RuleSymbolIterator> ruleSymbols(RuleDescriptor rd) const
    {
        return makeIteratorRange(
                RuleSymbolIterator(RuleSymbolDescriptor(rd, -1), *this), // -1 表示head
                RuleSymbolIterator(RuleSymbolDescriptor(rd, (*this)[rd.head][rd.body].size()), *this)
                );
    }





    template<typename Iterator>
    int getNonterminalsNumber(Iterator first, Iterator last) const
    {
        int ans = 0;
        for(;first != last; first++)
        {
            SymbolDescriptor s = *first;
            if(isNonterminal(s)) ans ++;
        }
        return ans;
    }

//    template<typename Iterator>
    int getNonterminalsNumber(RuleSymbolIterator first, RuleSymbolIterator last) const
    {
        int ans = 0;
        for(;first != last; first++)
        {
            if(isNonterminal(*first)) ans ++;
//            std::cout << "HH " <<std::endl;
        }
        return ans;
    }






    SymbolDescriptor ruleHead(RuleDescriptor rd) const
    {
        return rd.head;
    }
    const RuleBody& ruleBody(RuleDescriptor rd) const
    {
        return (*this)[rd.head][rd.body];
    }





    std::pair<RuleIterator, RuleIterator> rules() const
    {
        return std::make_pair(
            RuleIterator(RuleDescriptor(0, 0), *this),
            RuleIterator(RuleDescriptor(size(), 0), *this));
    }

    std::pair<RuleIterator, RuleIterator> rules(SymbolDescriptor s) const
    {
        return std::make_pair(
            RuleIterator(RuleDescriptor(s, 0), *this),
            RuleIterator(RuleDescriptor(s, (*this)[s].size()), *this));
    }



};




struct EpsilonSymbol {} eps;
struct NoProperty{};


template<typename T, typename P>
struct UserNonterminal;



template<typename P>
using ActionType = std::function<void(const std::vector<P>&, P&)>;

template<typename T, typename P>
struct GrammarFactory
{
    Grammar g;
    std::map<T, SymbolDescriptor> terminalMap;
//    using ActionType = std::function<void(std::vector<P>, P&)>;
    std::vector<ActionType<P>> actions;



    SymbolDescriptor getActionSymbolAndInsert(ActionType<P> action)
    {
        actions.push_back(action);
        return actions.size() + ActionSymbolBegin - 1;
    }


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
    std::array<UserNonterminal<T, P>, N> makeNonternimals();


};




template<typename T, typename P>
struct UserSymbol;

template<typename T, typename P>
using UserRuleBody = std::vector<UserSymbol<T, P>>;


template<typename T, typename P>
struct UserNonterminal
{
//    using ActionType = typename GrammarFactory<T, P>::ActionType;

    SymbolDescriptor id;
    ActionType<P> action;
    GrammarFactory<T, P>* gf;


    UserNonterminal(SymbolDescriptor id = 0, ActionType<P> func = ActionType<P>(), GrammarFactory<T, P>* gf = nullptr):
        id(id), action(func), gf(gf)
    {

    }


    UserNonterminal(const UserNonterminal<T, NoProperty>&other):
        id(other.id), action(ActionType<P>()), gf(nullptr)
    {

    }


    template<typename F>
    UserNonterminal& operator[](F f)
    {
        action = f;
        return *this;
    }

    void addRuleHeadAction();
    void cleanAction()
    {
        action = nullptr;
    }


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
};


template<typename T, typename P>
struct UserSymbol
{
    UserSymbol(UserNonterminal<T, P> nonterminal):
        type(UserSymbolType::Nonterminal), nonterminal(nonterminal) {}

    UserSymbol(T terminal):
        type(UserSymbolType::Terminal), terminal(terminal) { }

    UserSymbolType type;
    UserNonterminal<T, P> nonterminal = UserNonterminal<T, P>();
    T terminal = T();

};





template<typename T, typename P>
auto operator>>(UserNonterminal<T, P>& a, UserNonterminal<T, P>& b)
{
    UserRuleBody<T, P> ans{ UserSymbol<T, P>(a), UserSymbol<T, P>(b) };
    a.cleanAction();
    b.cleanAction();
    return ans;
}

template<typename T, typename P>
auto operator>>(UserNonterminal<T, P>& a, T b)
{
    UserRuleBody<T, P> ans{ UserSymbol<T, P>(a), UserSymbol<T, P>(b) };
    a.cleanAction();
    return ans;
}

template<typename T, typename P>
auto operator>>(T a, UserNonterminal<T, P>& b)
{
    UserRuleBody<T, P>ans{ UserSymbol<T, P>(a), UserSymbol<T, P>(b) };
    b.cleanAction();
    return ans;
}


template<typename T, typename P>
auto operator>>(UserRuleBody<T, P> a, T b)
{
    return a;
}

template<typename T, typename P>
auto operator>>(UserRuleBody<T, P> a, UserNonterminal<T, P>& b)
{
    a.push_back(UserSymbol<T, P>(b));
    b.cleanAction();

    return a;
}


template<typename T>
auto operator>>(EpsilonSymbol, T b)
{
    return UserRuleBody<T, NoProperty>{ UserSymbol<T, NoProperty>(b) };
}


template<typename T, typename P>
void UserNonterminal<T, P>::addRuleHeadAction()
{
    gf->g[id].push_back({});
    if(action)
    {
        gf->g[id].back().push_back(gf->getActionSymbolAndInsert(action));
        action = nullptr;
    }
}


template<typename T, typename P>
template<typename P2>
UserNonterminal<T, P>& UserNonterminal<T, P>::operator=(const UserRuleBody<T, P2>& o)
{
    addRuleHeadAction();



    for(UserSymbol<T, P> ch: o)
    {
        if(ch.type == UserSymbolType::Nonterminal)
        {
            gf->g[id].back().push_back(ch.nonterminal.id);
            if constexpr(std::is_same<P, P2>::value)
            {
                if(ch.nonterminal.action)
                {
                    gf->g[id].back().push_back(gf->getActionSymbolAndInsert(ch.nonterminal.action));
                }

            }
        }
        else if(ch.type == UserSymbolType::Terminal)
        {
            gf->g[id].back().push_back(gf->getTerminalSymbolAndInsert(ch.terminal));
        }
    }


    return *this;
}



template<typename T, typename P>
UserNonterminal<T, P>& UserNonterminal<T, P>::operator=(T o)
{
    addRuleHeadAction();
    gf->g[id].back().push_back({gf->getTerminalSymbolAndInsert(o)});
    return *this;
}

template<typename T, typename P>
UserNonterminal<T, P>&  UserNonterminal<T, P>::operator=(UserNonterminal<T, P> o)
{
    addRuleHeadAction();
    gf->g[id].back().push_back({o.id});
    return *this;
}

template<typename T, typename P>
UserNonterminal<T, P>&  UserNonterminal<T, P>::operator=(EpsilonSymbol )
{
    addRuleHeadAction();
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
    const std::map<SymbolDescriptor, T>& ternimalNames;


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
    SymbolDescriptor ruleHead;
    const RuleBody &ruleBody;

    const std::vector<std::string>& nonterminalNames;
    const std::map<SymbolDescriptor, T>& ternimalNames;

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
    const SymbolDescriptor A;
    const RuleBodyUnion& ru;
//    const std::vector<std::string>& names;
    const std::vector<std::string>& nonterminalNames;
    const std::map<SymbolDescriptor, T>& ternimalNames;


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
    const std::map<SymbolDescriptor, T>& ternimalNames;

    bool needLeftJustified = true;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const GrammerForOutput&  g)
    {
        int maxLength = 0;
        for(SymbolDescriptor i: irange(g.g.size()))
        {
            maxLength = std::max(maxLength, SymbolForOutput<T>{i, g.nonterminalNames, g.ternimalNames}.getNameLength());
        }

        for(SymbolDescriptor i: irange(g.g.size()))
        {
            os << RuleUnionForOutput<T>{i, g.g[i], g.nonterminalNames, g.ternimalNames,
                g.needLeftJustified ? maxLength : -1};
        }


        return os;
    }

};



}// namesapce lz

#endif /* LZ_COMPILER_NEW_GRAMMAR_H_ */
