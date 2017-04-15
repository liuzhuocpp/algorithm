/*
 * regex.h
 *
 *  Created on: 2017年4月13日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_REGEX_H_
#define LZ_COMPILER_REGEX_H_

#include <lz/utility.h>
#include <lz/adjacency_list.h>

namespace lz {


    namespace Detail {

    template<typename T>
    constexpr T epsilon = -1;

    }

using StateId = long;
static constexpr StateId InvalidStateId = -1;

struct NFAFragment
{
    StateId start = -1, end = -1;
    NFAFragment() = default;
    NFAFragment(StateId start, StateId end):start(start), end(end){}
};


template<typename T>
int getPriority(T ch)
{
    if(ch == '*') return 3;
    if(ch == '.') return 2;
    if(ch == '|') return 1;

    return -1;
}



template<typename T, typename Graph>
void popOperator(
        std::vector<NFAFragment>& valueStack,
        std::vector<T> &operatorStack,
        Graph &nfa)
{
    using Vertex = typename GraphTraits<Graph>::VertexDescriptor;

    Vertex newStart, newEnd;

    T stackOp = operatorStack.back();
    operatorStack.pop_back();


    if(stackOp == '*')
    {
//        newStart = addVertex(nfa);
//        newEnd = addVertex(nfa);


        NFAFragment frag = valueStack.back();

        addEdge(nfa, frag.start, frag.end, Detail::epsilon<T>);
        addEdge(nfa, frag.end, frag.start, Detail::epsilon<T>);

//        valueStack.pop_back();

//        addEdge(nfa, newStart, frag.start, Detail::epsilon<T>);
//        addEdge(nfa, frag.end, newEnd, Detail::epsilon<T>);
//        addEdge(nfa, newEnd, newStart, Detail::epsilon<T>);

//        valueStack.push_back(NFAFragment{newStart, newEnd});

    }
    else if(stackOp == '.')
    {
        NFAFragment frag2 = valueStack.back();
        valueStack.pop_back();
        NFAFragment frag1 = valueStack.back();
        valueStack.pop_back();


        addEdge(nfa, frag1.end, frag2.start, Detail::epsilon<T>);
        valueStack.push_back(NFAFragment{frag1.start, frag2.end});
    }
    else if(stackOp == '|')
    {
        newStart = addVertex(nfa);
        newEnd = addVertex(nfa);

        NFAFragment frag2 = valueStack.back();
        valueStack.pop_back();
        NFAFragment frag1 = valueStack.back();
        valueStack.pop_back();

        addEdge(nfa, newStart, frag1.start, Detail::epsilon<T>);
        addEdge(nfa, newStart, frag2.start, Detail::epsilon<T>);
        addEdge(nfa, frag1.end, newEnd, Detail::epsilon<T>);
        addEdge(nfa, frag2.end, newEnd, Detail::epsilon<T>);

        valueStack.push_back(NFAFragment{newStart, newEnd});
    }


}


template<typename T, typename NFA>
void pushOperator(std::vector<NFAFragment>& valueStack, std::vector<T> &operatorStack,
    NFA &nfa, T op)
{
    if(op == '(')
    {
        operatorStack.push_back(op);
        return ;
    }

    if(op == ')')
    {
        while(!operatorStack.empty() && operatorStack.back() != '(')
        {
            popOperator(valueStack, operatorStack, nfa);
        }
        operatorStack.pop_back();
        return ;
    }


    while(!operatorStack.empty() &&
        operatorStack.back() != '(' &&
        (getPriority(operatorStack.back()) >= getPriority(op) ))
    {
//        std::cout << "ggg" << std::endl;
        popOperator(valueStack, operatorStack, nfa);
    }

    operatorStack.push_back(op);


}

int getType(char ch)
{
    if(ch >= 'A' && ch <= 'Z') return 0;
    if(ch >= 'a' && ch <= 'z') return 1;
    if(ch >= '0' && ch <= '9') return 2;
    assert(0);
    return -1;

}

template<typename NFA, typename Iterator>
auto
parseRegex(NFA &nfa, Iterator first, Iterator last)
{
    Iterator copyFirst = first;
    auto n = last - first;
    using DiffType = typename std::iterator_traits<Iterator>::difference_type;
    using T = typename std::iterator_traits<Iterator>::value_type;

    using Vertex = typename GraphTraits<NFA>::VertexDescriptor;

    std::vector<NFAFragment> valueStack;
    std::vector<T> operatorStack;

    for(;first != last; ++ first)
    {
        auto needCat = [&]() {
            if(first != copyFirst)
            {
                if(first[-1] != '|' && first[-1] != '(')
                {
                    return true;
                }
                else if(first - 2 >= copyFirst && first[-2] == '\\')
                {
                    return true;
                }
            }
            return false;

        };

        if(*first == '|')
        {
            pushOperator(valueStack, operatorStack, nfa, '|');
        }
        else if(*first == '*')
        {
            pushOperator(valueStack, operatorStack, nfa, '*');
        }
        else if(*first == '(')
        {
            if(needCat())
            {
                pushOperator(valueStack, operatorStack, nfa, '.');
            }

            pushOperator(valueStack, operatorStack, nfa, '(');
        }
        else if(*first == ')')
        {
            pushOperator(valueStack, operatorStack, nfa, ')');
        }
        else if(*first == '[')
        {
            if(needCat())
            {
                pushOperator(valueStack, operatorStack, nfa, '.');
            }



            auto rightBracket = ++first;
            for(;rightBracket != last && *rightBracket != ']'; rightBracket++)
            {
            }

            if(rightBracket == last)
            {
                assert(0);
                break;
            }
            assert((rightBracket - first) % 3 == 0);

            Vertex realStart = addVertex(nfa);
            Vertex realEnd = addVertex(nfa);

            for(;first != rightBracket; first += 3)
            {
                assert(getType(first[0]) == getType(first[2]));
                assert(first[1] == '-');
                assert(first[0] < first[2]);

                for(T ch = first[0]; ch <= first[2]; ++ ch)
                {
                    Vertex start = addVertex(nfa);
                    Vertex end = addVertex(nfa);
                    addEdge(nfa, start, end, ch);
                    addEdge(nfa, realStart, start, Detail::epsilon<T>);
                    addEdge(nfa, end, realEnd, Detail::epsilon<T>);
                }
            }
            valueStack.push_back(NFAFragment(realStart, realEnd));

            first = rightBracket;


        }
        else
        {
            if(needCat())
            {
                pushOperator(valueStack, operatorStack, nfa, '.');
            }

            if(*first == '\\' )
            {
                assert(first != last);
                 ++ first;
            }

            Vertex start = addVertex(nfa);
            Vertex end = addVertex(nfa);
            addEdge(nfa, start, end, *first);
            valueStack.push_back(NFAFragment(start, end));
        }
    }

    while(!operatorStack.empty())
    {
        popOperator(valueStack, operatorStack, nfa);
    }

    assert(valueStack.size() == 1);
    return std::make_tuple(valueStack[0].start, valueStack[0].end);
}





template<typename NFA, typename String >
auto
parseRegex( NFA &nfa, const String & input)
{
    return parseRegex(nfa, std::begin(input), std::end(input));
}


template<typename NFA>
auto parseMultiRegex(NFA& nfa,
    const std::vector<std::pair<std::string, std::function<void(std::string::iterator, std::string::iterator)>>>& regexAndFuncs)
{
    using Vertex = typename GraphTraits<NFA>::VertexDescriptor;
    Vertex realStart = addVertex(nfa);
    std::map<Vertex, std::function<void(std::string::iterator, std::string::iterator)>> vertexToFunc;
    for(auto regexAndFunc: regexAndFuncs)
    {
        auto [start, end] = parseRegex( nfa, std::get<0>(regexAndFunc));
        addEdge(nfa, realStart, start, Detail::epsilon<char>);
        vertexToFunc[end] = std::get<1>(regexAndFunc);
    }


    return std::make_pair(realStart, vertexToFunc);
}







} // namespace lz





#endif /* LZ_COMPILER_REGEX_H_ */
