/*
 * simulate_nfa.h
 *
 *  Created on: 2017年2月19日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_SIMULATE_NFA_H_
#define LZ_COMPILER_SIMULATE_NFA_H_

#include <lz/breadth_first_search.h>

namespace lz {



template<typename NFA, typename EdgePropertyMap>
void
calucateEmptyStringClosure(
    const NFA& nfa,
    typename GraphTraits<NFA>::VertexDescriptor s,
    EdgePropertyMap edgeMap,
    std::vector<bool> &flag,
    std::vector<typename GraphTraits<NFA>::VertexDescriptor> &answer)
{
    using Vertex = typename GraphTraits<NFA>::VertexDescriptor;
    std::vector<Vertex> buffer;

    buffer.push_back(s);
    while(buffer.size())
    {
        Vertex u = buffer.back();
        buffer.pop_back();
        answer.push_back(u);
        for(auto e: outEdges(nfa, u))
        {
            Vertex to = target(nfa, e);
            if(!flag[to] && edgeMap[e] == nullptr)
            {
                flag[to] = 1;
                buffer.push_back(to);
            }
        }
    }

}




template<typename NFA, typename EdgePropertyMap>
auto
calucateEmptyStringClosure(
    const NFA &nfa,
    std::vector<typename GraphTraits<NFA>::VertexDescriptor> oldVertices,
    EdgePropertyMap edgeMap)
{
    using Vertex = typename GraphTraits<NFA>::VertexDescriptor;
    auto n = verticesNumber(nfa);
    std::vector<bool> flag(n, 0);

    std::vector<Vertex> answer;
    for(auto u : oldVertices)
    {
        calucateEmptyStringClosure(nfa, u, edgeMap, flag, answer);
    }
    return answer;
}

template<typename NFA, typename EdgePropertyMap>
auto moveStates(
    const NFA &nfa,
    const std::vector<typename GraphTraits<NFA>::VertexDescriptor>& oldVertices,
    EdgePropertyMap edgeMap,
    std::remove_pointer_t<typename MapTraits<EdgePropertyMap>::ValueType> a)
{
    using Vertex = typename GraphTraits<NFA>::VertexDescriptor;
    auto n = verticesNumber(nfa);
    std::vector<bool> flag(n, 0);
    std::vector<Vertex> answer;

    for(auto u : oldVertices)
    {
        for(auto e: outEdges(nfa, u))
        {

            Vertex to = target(nfa, e);
            if(edgeMap[e] != nullptr && *edgeMap[e] == a && !flag[to])
            {
                flag[to] = 1;
                answer.push_back(to);
            }
        }
    }
    return answer;

}


template<typename NFA, typename Iterator>
bool simulateNFA(const NFA &nfa, Iterator first, Iterator last)
{
    using Vertex = typename GraphTraits<NFA>::VertexDescriptor;

    std::vector<Vertex> states;

    auto n = verticesNumber(nfa);
    std::vector<bool> flag(n, 0);
    auto edgeMap = edgePropertyMap(nfa, NFAEdgeTag());
    calucateEmptyStringClosure(nfa, nfa.start, edgeMap, flag, states);
    for(;first != last; ++ first)
    {
        states = moveStates(nfa, states, edgeMap, *first);
        states = calucateEmptyStringClosure(nfa, states, edgeMap);
    }

    return std::any_of(states.begin(), states.end(), [&](auto i){ return i == nfa.end; } );



}









} // namespace lz





#endif /* LZ_COMPILER_SIMULATE_NFA_H_ */
