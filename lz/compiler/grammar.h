/*
 * grammar.h
 *
 *  Created on: 2017年3月11日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NEW_GRAMMAR_H_
#define LZ_COMPILER_NEW_GRAMMAR_H_

#include <lz/iterator.h>
#include <lz/compiler/symbol.h>
#include <lz/compiler/utility.h>



namespace lz {




template<typename _NodeProperties>
struct Grammar:
//    private
    public
std::vector<std::vector<std::vector<SymbolDescriptor>>>
{
public:
    using NodeProperties = _NodeProperties;
    using SizeType = std::size_t;
    using SemanticRuleFunc = lz::SemanticRuleType<NodeProperties>;


private:
    using RuleBody = std::vector<SymbolDescriptor>;
    using RuleBodyUnion = std::vector<RuleBody>;
    using Base = std::vector<RuleBodyUnion>;
    std::vector<SemanticRuleFunc> semanticRuleFuncs;

//    using

    using  std::vector<std::vector<std::vector<SymbolDescriptor>>>::vector;
public:


//    using NonterminalProperties = P;


    struct RuleDescriptor: public EqualityComparableFacade<RuleDescriptor>, LessThanComparableFacade<RuleDescriptor>
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

        friend bool operator<(RuleDescriptor a, RuleDescriptor b)
        {
            if(a.head != b.head) return a.head < b.head;
            return a.body < b.body;
        }
    };



    std::map<std::pair<RuleDescriptor, int> , int> priorities;

    // 设置产生式和非终结符的优先级大小
    void setPriority(RuleDescriptor rd, SymbolDescriptor terminal, int priority)
    {
        priorities[{rd, terminal}] = priority;
    }

    // 获取产生式和非终结符的优先级大小
    // 返回值 ==0 表示两者没有优先级大小关系
    int getPriority(RuleDescriptor rd, SymbolDescriptor terminal) const
    {
        if(priorities.count({rd, terminal}))
        {
            return priorities.at({rd, terminal});
        }
        return 0;
    }






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


    struct RuleSymbolIterator:
        IteratorFacade<RuleSymbolIterator, std::bidirectional_iterator_tag, SymbolDescriptor>,
        LessThanComparableFacade<RuleSymbolIterator>
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

        friend bool operator<(const RuleSymbolIterator& a, const RuleSymbolIterator &b)
        {
            return a.cur < b.cur;
//            return a.head == b.head && a.body == b.body && a.cur == b.cur;
        }

    };

    // rd 与it 所指向的字符必须是非终结符
    SymbolDescriptor calculateSemanticRule(RuleDescriptor rd, RuleSymbolIterator it) const
    {
        auto ruleRange = ruleSymbols(rd);
        int id = it.cur;
        const RuleBody& body =  (*this)[rd.head][rd.body];
        id++;

        if(id < body.size() && isSemanticRule(body[id]))
        {
            return body[id];
        }
        else return NullSymbol;
    }


    // s 代表一个action symbol
    SemanticRuleType<NodeProperties> getSemanticRuleFunc(SymbolDescriptor s) const
    {
        return semanticRuleFuncs[s - SemanticRuleSymbolBegin];
    }

    SizeType actionsNumber() const
    {
        return semanticRuleFuncs.size();
    }




    SymbolDescriptor addSemanticRuleFunc(SemanticRuleType<NodeProperties> f)
    {
        semanticRuleFuncs.push_back(f);
        return semanticRuleFuncs.size() + SemanticRuleSymbolBegin - 1;
    }

    SymbolDescriptor addNonterminal()
    {
        this->push_back({});
        return this->size() - 1;
    }

    SizeType terminalsNumber() const
    {
        return m_terminalsNumber;
    }
private:
    SizeType m_terminalsNumber = 0;
public:

    SymbolDescriptor addTerminal()
    {
        m_terminalsNumber++;
        return TerminalSymbolBegin + m_terminalsNumber - 1;
    }

    using NonterminalIterator = IntegerIterator<SymbolDescriptor>;
    using TerminalIterator = IntegerIterator<SymbolDescriptor>;
    using SymbolIterator = IntegerIterator<SymbolDescriptor>;


    IteratorRange<NonterminalIterator> nonterminals() const
    {
        return makeIteratorRange(NonterminalIterator(0), NonterminalIterator(this->size()));
    }

    IteratorRange<TerminalIterator> terminals() const
    {
        return makeIteratorRange(
                TerminalIterator(TerminalSymbolBegin),
                TerminalIterator(TerminalSymbolBegin + m_terminalsNumber));
    }

    IteratorRange<SymbolIterator> semanticRules() const
    {
        return makeIteratorRange(
            SymbolIterator(SemanticRuleSymbolBegin),
            SymbolIterator(SemanticRuleSymbolBegin + this->semanticRuleFuncs.size()));
    }

    void clear()
    {
        Base::clear();
        semanticRuleFuncs.clear();
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
    RuleDescriptor addRule(InputIterator first, InputIterator last)
    {
        Grammar & g = *this;
        SymbolDescriptor head = *first++;
        g[head].push_back({});
        for(;first != last; ++ first)
        {
            g[head].back().push_back(*first);
        }
        return RuleDescriptor(head, g[head].size() - 1);
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
    // 假定每个非终结符都至少有一个RuleBody，否则程序将不会运行正确，有待改进
    struct OutRuleIterator:IteratorFacade<OutRuleIterator, std::forward_iterator_tag, RuleDescriptor>
    {
        RuleDescriptor rd;
        OutRuleIterator():rd(RuleDescriptor()) { }

        OutRuleIterator(RuleDescriptor rd): rd(rd) { }

        OutRuleIterator& operator++()
        {
            rd.body++;
            return *this;
        }
        RuleDescriptor operator*() const
        {
            return rd;
        }

        friend bool operator==(const OutRuleIterator& a, const OutRuleIterator &b)
        {
            return a.rd == b.rd;
        }
    };

    // 有问题需要改
    IteratorRange<OutRuleIterator> outRules(SymbolDescriptor s) const
    {
        return makeIteratorRange(
                OutRuleIterator(RuleDescriptor(s, 0)),
                OutRuleIterator(RuleDescriptor(s, (*this)[s].size())));
    }


    using SymbolDescriptor = lz::SymbolDescriptor;

};





















// For output----------------------------------------------------
// NonterminalMap::Value must a string

template<typename SymbolDescriptor, typename NonterminalMap, typename TerminalMap>
struct SymbolForOutput
{
    const SymbolDescriptor& symbol;
    NonterminalMap nonterminalMap;
    TerminalMap terminalMap;

    int getNameLength() const
    {
        if(isNonterminal(symbol))
        {
            auto i = getNonterminalId(symbol);
            return nonterminalMap[i].size();
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
//        std::cout << "S::" << s << std::endl;
        if(isTerminal(s))
        {
//            std::cout << "SBterminal??" << std::endl;

            os << so.terminalMap[getTerminalId(s)];
        }
        else if(isNonterminal(s))
        {
//            std::cout << "nonononterminal??" << std::endl;
            os << so.nonterminalMap[getNonterminalId(s)];
        }
        else if(isSemanticRule(s))
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

template<typename Iterator, typename NonterminalMap, typename TerminalMap>
struct SymbolRangeForOutput
{
    using SymbolDescriptor = typename std::iterator_traits<Iterator>::value_type;

    Iterator first, last;
    NonterminalMap nonterminalMap;
    TerminalMap terminalMap;


    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const SymbolRangeForOutput& sr)
    {
        for(SymbolDescriptor s: makeIteratorRange(sr.first, sr.last))
        {
            os << SymbolForOutput<SymbolDescriptor, NonterminalMap, TerminalMap>
                {s, sr.nonterminalMap, sr.terminalMap};
            os << " " ;
        }
        return os;
    }

};

template<typename Iterator, typename NonterminalMap, typename TerminalMap>
struct RuleForOutput
{
    using SymbolDescriptor = typename std::iterator_traits<Iterator>::value_type;

    Iterator first, last;
    NonterminalMap nonterminalMap;
    TerminalMap terminalMap;

    int leftTotalWidth = -1;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, RuleForOutput ro)
    {

        SymbolForOutput<SymbolDescriptor, NonterminalMap, TerminalMap> head {*ro.first, ro.nonterminalMap, ro.terminalMap};
        os << head;
        if(ro.leftTotalWidth != -1)
        {
            os << std::string(std::max(ro.leftTotalWidth - head.getNameLength(), 0), ' ');
        }

        os << "->" << SymbolRangeForOutput<Iterator, NonterminalMap, TerminalMap>
            {++ro.first, ro.last, ro.nonterminalMap, ro.terminalMap };

        return os;
    }

};



template<typename Grammar, typename NonterminalMap, typename TerminalMap>
struct GrammerForOutput
{
    using SymbolDescriptor = typename Grammar::SymbolDescriptor;
    const Grammar& g;
    NonterminalMap nonterminalMap;
    TerminalMap terminalMap;

    bool needLeftJustified = true;

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&

    operator<<(std::basic_ostream<Char, Traits>& os, const GrammerForOutput&  g)
    {
        int maxLength = 0;
        for(auto rd: g.g.rules())
        {
            maxLength = std::max(maxLength,
                    SymbolForOutput<SymbolDescriptor, NonterminalMap, TerminalMap>{*g.g.ruleSymbols(rd).first,
                g.nonterminalMap, g.terminalMap}.getNameLength());
        }

        for(auto rd: g.g.rules())
        {
            auto ruleRange = g.g.ruleSymbols(rd);

            os << RuleForOutput<decltype(ruleRange.first), NonterminalMap, TerminalMap>
                {ruleRange.first, ruleRange.second, g.nonterminalMap, g.terminalMap,
                g.needLeftJustified ? maxLength : -1};
            os << "\n";
        }


        return os;
    }

};



}// namesapce lz

#endif /* LZ_COMPILER_NEW_GRAMMAR_H_ */
