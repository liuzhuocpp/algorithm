/*
 * grammar.h
 *
 *  Created on: 2017年3月11日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NEW_GRAMMAR_H_
#define LZ_COMPILER_NEW_GRAMMAR_H_



namespace lz {




using Symbol = long;


constexpr Symbol TerminalSymbolBegin = std::numeric_limits<Symbol>::min();
constexpr Symbol ActionSymbolBegin = std::numeric_limits<Symbol>::min() >> 1;
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
    return s >= ActionSymbolBegin && s < EndTagSymbol;
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
};




struct EpsilonSymbol {} eps;

template<typename T>
struct Nonterminal;




template<typename T>
struct GrammarFactory
{
    Grammar g;
    std::map<T, Symbol> terminalMap;
    Symbol cntTerminalId;

    std::map<Symbol, T> calculateTerminalNames()
    {
        std::map<Symbol, T> ans;
        for(auto p: terminalMap)
        {
            ans[p.second] = p.first;
        }
        return ans;
    }

    Symbol getTerminalId(T ch)
    {
        if(terminalMap.count(ch))
        {
            return terminalMap[ch];
        }
        else
        {
            terminalMap[ch] = cntTerminalId;
            return cntTerminalId ++;
        }
    }

    template<std::size_t N>
    std::array<Nonterminal<T>, N> makeNonternimals();


};


template<typename T>
struct Nonterminal
{

    enum class UserSymbolType
    {
        Nonterminal,
        Terminal,
        EmptyString,
    };


    struct UserSymbol
    {
        UserSymbolType type;
        union
        {
            Symbol nonterminal;
            T terminal;
        };
    };


    static UserSymbol makeNonterminal(Symbol s)
    {
        UserSymbol us;
        us.type = UserSymbolType::Nonterminal;
        us.nonterminal = s;
        return us;
    }

    static UserSymbol makeTerminal(Symbol s)
    {
        UserSymbol us;
        us.type = UserSymbolType::Terminal;
        us.terminal = s;
        return us;
    }

    static UserSymbol makeEmptyString()
    {
        UserSymbol us;
        us.type = UserSymbolType::EmptyString;
        return us;
    }

    using UserRuleBody = std::vector<UserSymbol>;


    friend UserRuleBody operator>>(Nonterminal a, Nonterminal b)
    {
        return UserRuleBody{makeNonterminal(a.id), makeNonterminal(b.id)};
    }

    friend UserRuleBody operator>>(Nonterminal a, T b)
    {
        return UserRuleBody {makeNonterminal(a.id), makeTerminal(b)};
    }

    friend UserRuleBody operator>>(T a, Nonterminal b)
    {
        return UserRuleBody {makeTerminal(a), makeNonterminal (b.id)};
    }

    friend UserRuleBody  operator>>(UserRuleBody a, Nonterminal b)
    {
        a.push_back(makeNonterminal(b.id));
        return a;
    }


    RuleBody getRuleBody( UserRuleBody a)
    {
        RuleBody ans;
        for(UserSymbol ch: a)
        {
            if(ch.type == UserSymbolType::Nonterminal)
            {
                ans.push_back(ch.nonterminal);
            }
            else if(ch.type == UserSymbolType::Terminal)
            {
                ans.push_back(gf->getTerminalId(ch.nonterminal));
            }
        }
        return ans;
    }






    Nonterminal& operator=(const UserRuleBody& o)
    {
        gf->g[id].push_back(getRuleBody(o));
        return *this;
    }

    Nonterminal& operator=(Nonterminal o)
    {
        gf->g[id].push_back({o.id});
        return *this;
    }

    Nonterminal& operator=(EpsilonSymbol )
    {
        gf->g[id].push_back({});
        return *this;
    }

    Nonterminal& operator=(T o)
    {
        gf->g[id].push_back({gf->getTerminalId(o)});
        return *this;
    }
//


//private:
    Symbol id;
    GrammarFactory<T>* gf;

};




template<typename T>
 typename Nonterminal<T>::UserRuleBody  operator>>(EpsilonSymbol a, T b)
{

    return typename  Nonterminal<T>::UserRuleBody{Nonterminal<T>::makeTerminal(b)};
}






//template<typename T>
//typename Nonterminal<T>::UserRuleBody  operator>>(T a, EpsilonSymbol b)
//{
//
//    return typename  Nonterminal<T>::UserRuleBody{Nonterminal<T>::makeTerminal(a)};
//}




template<typename T>
typename Nonterminal<T>::UserRuleBody  operator>>(typename Nonterminal<T>::UserRuleBody a, T b)
{
    a.push_back(Nonterminal<T>::makeTerminal(b));
    return a;
}










template<typename T>
template<std::size_t N>
std::array<Nonterminal<T>, N>

GrammarFactory<T>::makeNonternimals()
{
    std::array<Nonterminal<T>, N> ans;
    using DiffType = typename std::array<Nonterminal<T>, N>::difference_type;
    for(DiffType i: lz::irange(N))
    {
        ans[i].id = i;
        ans[i].gf = this;
    }

    cntTerminalId = TerminalSymbolBegin;
    terminalMap.clear();
    g.resize(N);
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
        else if(isEndTag(s))
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
