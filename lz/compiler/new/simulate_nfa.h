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
    using EdgeWeight = typename MapTraits<EdgePropertyMap>::ValueType;
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
            if(!flag[to] && edgeMap[e] == Detail::epsilon<EdgeWeight>)
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
            if(edgeMap[e] == a && !flag[to])
            {
                flag[to] = 1;
                answer.push_back(to);
            }
        }
    }
    return answer;

}


template<typename Iterator, typename NFA, typename VertexToFunc>
auto simulateNFA(Iterator first, Iterator last, const NFA &nfa,
        typename GraphTraits<NFA>::VertexDescriptor start,
        VertexToFunc vertexToFunc)
{
    using Vertex = typename GraphTraits<NFA>::VertexDescriptor;

    std::vector<Vertex> states;

    auto n = verticesNumber(nfa);
    std::vector<bool> flag(n, 0);
    auto edgeMap = edgePropertyMap(nfa, edgeWeightTag);

    calucateEmptyStringClosure(nfa, start, edgeMap, flag, states);
    auto firstCopy = first;
    auto ret = last;
    StateId retStateId;
    for(;first != last; ++ first)
    {
        states = moveStates(nfa, states, edgeMap, *first);
        if(states.empty()) break;
        states = calucateEmptyStringClosure(nfa, states, edgeMap);

        auto ans = std::find_if(states.begin(), states.end(), [&](auto i) {

                if(vertexToFunc.count(i))
                {
                    ret = first;
                    retStateId = i;
                    return true;
                }
                else return false;
        });
    }



    if(ret != last)
    {
        ++ret;
//        std::cout << "kkkkkjjj " <<  (firstCopy == ret) << endl;
        vertexToFunc[retStateId](firstCopy, ret);
    }
    else
    {
        ret = firstCopy;
    }
    return ret;



}









} // namespace lz





#endif /* LZ_COMPILER_SIMULATE_NFA_H_ */
