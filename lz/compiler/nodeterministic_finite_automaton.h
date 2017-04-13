/*
 * nodeterministic_finite_automaton.h
 *
 *  Created on: 2017年2月18日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_NODETERMINISTIC_FINITE_AUTOMATON_H_
#define LZ_COMPILER_NODETERMINISTIC_FINITE_AUTOMATON_H_

#include <iterator>
#include <lz/graph_utility.h>
#include <lz/iterator.h>

namespace lz {


namespace Private {

}




using StateId = long;
static constexpr StateId InvalidStateId = -1;



/*

    first != -1 && second != -1: 有两条边，且都是空字符
    first != -1 && second == -1: 有一条表，且是非空字符，匹配字符存在value中
    first == -1 && second != -1: 有一条边，且是空字符
 */
template<typename T>
struct State
{
    const StateId& first() const
    {
        return adjacency[0];
    }

    StateId& first()
    {
        return adjacency[0];
    }

    const StateId& second() const
    {
        return adjacency[1];
    }

    StateId& second()
    {
        return adjacency[1];
    }


    T value;
    State() = default;
    State(StateId first, StateId second, T value):
        adjacency{first, second}, value(value){}

    struct OutEdgeIterator:
        public IteratorFacade<OutEdgeIterator, std::forward_iterator_tag, StateId>
    {
        OutEdgeIterator operator++()
        {
            adjId ++;
            return *this;
        }
        StateId operator*() const
        {
            return stateId << 1 | adjId;
        }
        friend bool operator== (const OutEdgeIterator& a, const OutEdgeIterator& b)
        {
            return a.adjId == b.adjId && a.stateId == b.stateId && a.state == b.state;
        }
        OutEdgeIterator():state(nullptr), stateId(InvalidStateId), adjId(2) {}
    private:
    public:
        friend State;

        OutEdgeIterator(const State& state, StateId stateId, int adjId):
            state(&state), stateId(stateId), adjId(adjId){}

        const State* state;
        StateId stateId;
        int adjId;
    };

    const StateId* outEdges() const
    {
        return adjacency;
    }

    const T* edgeProperty(int adjId) const
    {
        if(first() != InvalidStateId && second() == InvalidStateId)
        {
            return &value;
        }
        else return nullptr;

    }

private:

//    StateId m_first = -1;
//    StateId m_second = -1;
    StateId adjacency[2] = {-1, -1};

public:
    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const State& s)
    {
        if(s.first() != -1 && s.second() != -1)
        {
            os << "(" << s.first() <<   ": ), (" << s.second() <<  ": )";
        }
        else if(s.first() != -1 && s.second() == -1)
        {
            os << "("  << s.first() << ": " << s.value <<  ")";
        }
        else if(s.first() == -1 && s.second() != -1)
        {
            os << "("  << s.second() << ": )";
        }
        return os;
    }


};





struct NFAFragment
{
    StateId start = -1, end = -1;
    NFAFragment() = default;
    NFAFragment(StateId start, StateId end):start(start), end(end){}
};


//struct NFAEdgeTag
//{
//
//};

struct NFAEdgeTag{} nfaEdgeTag;

template<typename T>
struct NFA: public std::vector<State<T>>
{
    StateId start, end;

    NFA() : start(-1), end(-1)
    {

    }
    NFA(StateId start, StateId end, std::vector<State<T>> &&data):
        start(start), end(end), std::vector<State<T>>(std::move(data))
    {

    }

    using OutEdgeIterator = typename State<T>::OutEdgeIterator;
    friend std::pair<OutEdgeIterator, OutEdgeIterator> outEdges(const NFA& nfa, StateId u)
    {
        if(nfa[u].first() != InvalidStateId && nfa[u].second() != InvalidStateId)
        {
            return std::make_pair(OutEdgeIterator(nfa[u], u, 0), OutEdgeIterator(nfa[u], u, 2));
        }
        else if(nfa[u].first() != InvalidStateId && nfa[u].second() == InvalidStateId)
        {
            return std::make_pair(OutEdgeIterator(nfa[u], u, 0), OutEdgeIterator(nfa[u], u, 1));
        }

        else if(nfa[u].first() == InvalidStateId && nfa[u].second() != InvalidStateId)
        {
            return std::make_pair(OutEdgeIterator(nfa[u], u, 1), OutEdgeIterator(nfa[u], u, 2));
        }
        else
            return std::make_pair(OutEdgeIterator(nfa[u], u, 0), OutEdgeIterator(nfa[u], u, 0));
    }

    friend StateId source(const NFA& nfa, StateId edge)
    {
        return edge >> 1;
    }

    friend StateId target(const NFA& nfa, StateId edge)
    {
        return nfa[edge >> 1].outEdges()[edge & 1];
    }


    template<typename Tag>
    using VertexPropertyMap = IdentityMap<StateId>;

    template<typename Tag>
    using ConstVertexPropertyMap = IdentityMap<StateId>;


    template<typename Tag>
    friend VertexPropertyMap<Tag> vertexPropertyMap(const NFA& nfa, Tag)
    {
        return VertexPropertyMap<Tag>();
    }

    using VerticesNumberType = typename NFA<T>::size_type;
    using VertexIterator = lz::IntegerIterator<StateId>;
    friend VerticesNumberType verticesNumber(const NFA& nfa)
    {
        return nfa.size();
    }
    friend std::pair<VertexIterator, VertexIterator> vertices(const NFA& nfa)
    {
        return std::make_pair(VertexIterator(0), VertexIterator(nfa.size()));
    }


    template<typename Tag>
    struct EdgePropertyMap:public MapFacade<StateId, const T*>
    {
        const T* operator[](StateId edge) const
        {
            return (*nfa)[edge >> 1].edgeProperty(edge & 1);
        }
        EdgePropertyMap(const NFA& nfa): nfa(&nfa) {}
        EdgePropertyMap(): nfa(nullptr) {}
    private:
        const NFA* nfa;
    };
    template<typename Tag>
    using ConstEdgePropertyMap = EdgePropertyMap<Tag>;




    template<typename Tag>
    friend EdgePropertyMap<Tag> edgePropertyMap(const NFA& nfa, Tag)
    {
        return EdgePropertyMap<Tag>(nfa);
    }




    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const NFA& u)
    {
        os << "start: " << u.start <<  "  end: " << u.end << "\n";
        for(std::size_t i = 0; i < u.size(); ++ i)
        {
            os << i << ": " << u[i] << "\n";
        }
        return os;
    }


};

template<typename T>
struct GraphTraits< NFA<T> >
{
    using VertexDescriptor = StateId;
    using EdgeDescriptor = StateId;
    using DirectedCategory = DirectedGraphTag;
    static constexpr VertexDescriptor nullVertex() { return  InvalidStateId; }
    static constexpr EdgeDescriptor nullEdge() { return  InvalidStateId; }

    using OutEdgeIterator = typename NFA<T>::OutEdgeIterator;

    using VerticesNumberType = typename NFA<T>::VerticesNumberType;
    using VertexIterator = typename NFA<T>::VertexIterator;



    template<typename Tag>
    using EdgePropertyMap = typename NFA<T>::template EdgePropertyMap<Tag>;

    template<typename Tag>
    using ConstEdgePropertyMap = typename NFA<T>::template ConstEdgePropertyMap<Tag>;


    template<typename Tag>
    using VertexPropertyMap = typename NFA<T>::template VertexPropertyMap<Tag>;

    template<typename Tag>
    using ConstVertexPropertyMap = typename NFA<T>::template ConstVertexPropertyMap<Tag>;

};





} // namespace lz





#endif /* LZ_COMPILER_NODETERMINISTIC_FINITE_AUTOMATON_H_ */
