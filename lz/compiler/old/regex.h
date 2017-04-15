/*
 * regex.h
 *
 *  Created on: 2017年4月13日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_REGEX_H_
#define LZ_COMPILER_REGEX_H_

#include <lz/utility.h>


namespace lz {





template<typename T>
int getPriority(T ch)
{
    if(ch == '*') return 3;
    if(ch == '.') return 2;
    if(ch == '|') return 1;

    return -1;
}


template<typename T>
void popOperator(
        std::vector<NFAFragment>& valueStack,
        std::vector<T> &operatorStack,
        std::vector<State<T>>& states)
{
    T stackOp = operatorStack.back();

    operatorStack.pop_back();

    StateId newStart;
    StateId newEnd;

    if(stackOp == '*')
    {
        states.push_back(State<T>());
        states.push_back(State<T>());

        newStart = states.size() - 2;
        newEnd = states.size() - 1;

        NFAFragment &nfa = valueStack.back();

        states[newStart].first() = nfa.start;
        states[newStart].second() = newEnd;
        states[nfa.end].first() = newEnd;
        states[nfa.end].second() = nfa.start;

        nfa = NFAFragment(newStart, newEnd);
    }
    else if(stackOp == '.')
    {
        NFAFragment nfa2 = valueStack.back();
        valueStack.pop_back();
        NFAFragment &nfa1 = valueStack.back();

        newStart = nfa1.start;
        newEnd = nfa2.end;
        states[nfa1.end].second() = nfa2.start;

        nfa1 = NFAFragment(newStart, newEnd);
    }
    else if(stackOp == '|')
    {
        states.push_back(State<T>());
        states.push_back(State<T>());

        newStart = states.size() - 2;
        newEnd = states.size() - 1;

        NFAFragment nfa2 = valueStack.back();
        valueStack.pop_back();
        NFAFragment &nfa1 = valueStack.back();

        states[newStart].first() = nfa1.start;
        states[newStart].second() = nfa2.start;
        states[nfa1.end].second() = newEnd;
        states[nfa2.end].second() = newEnd;

        nfa1 = NFAFragment(newStart, newEnd);
    }


}


template<typename T>
void pushOperator(std::vector<NFAFragment>& valueStack, std::vector<T> &operatorStack,
    std::vector<State<T>>& states, T op)
{

    while(!operatorStack.empty() &&
        getPriority(operatorStack.back()) >= getPriority(op) )
    {
        popOperator(valueStack, operatorStack, states);
    }

    operatorStack.push_back(op);


}


template<typename Iterator, typename Func = EmptyFunction>
auto
parseRegex(Iterator first, Iterator last, Func func = Func())
{
    Iterator copyFirst = first;
    auto n = last - first;
    using DiffType = typename std::iterator_traits<Iterator>::difference_type;
    using T = typename std::iterator_traits<Iterator>::value_type;

    std::vector<State<T>> states;

    std::vector<NFAFragment> valueStack;
    std::vector<T> operatorStack;

    for(;first != last; ++ first)
    {
        if(*first == '|')
        {
            pushOperator(valueStack, operatorStack, states, '|');
        }
        else if(*first == '*')
        {
            pushOperator(valueStack, operatorStack, states, '*');
        }
        else
        {
            if(first != copyFirst && *(first - 1) != '|')
            {
//                std:: cout  << "YES" << "\n";
                pushOperator(valueStack, operatorStack, states, '.');
            }

            states.push_back(State<T>());
            states.push_back(State<T>());

            StateId start = states.size() - 2;
            StateId end = states.size() - 1;
            states[start].first() = end;
            states[start].value = *first;
            valueStack.push_back(NFAFragment(start, end));
        }
    }

    while(!operatorStack.empty())
    {
        popOperator(valueStack, operatorStack, states);
    }

    assert(valueStack.size() == 1);


    NFA<T> nfa(valueStack[0].start, valueStack[0].end, std::move(states));

    return std::move(nfa);

}
















} // namespace lz





#endif /* LZ_COMPILER_REGEX_H_ */
