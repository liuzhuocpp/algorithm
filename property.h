#ifndef PORPERTY_H
#define PORPERTY_H




#include <iostream>
#include <typeinfo>
#include <vector>

#include <type_traits>

namespace lz {

using std::integral_constant;



struct NoProperty {};


    namespace PropertyPrivate {

        template<typename P>
        struct CountProperty:
            public integral_constant<
                size_t, 
                CountProperty<typename P::NextProperty>::value + 1
            >
        {
            // static constexpr int num = CountProperty<typename P::NextProperty>::num + 1;
        };

        template<>
        struct CountProperty<NoProperty>:
            public integral_constant<
                size_t, 
                0
            >
        {
            
        };

        template<typename... T>
        struct CountVariadic;


        template<typename Head, typename...T>
        struct CountVariadic<Head, T...>:
            public integral_constant<
                size_t, 
                CountVariadic<T...>::value + 1
            >
        {

        };

        template<>
        struct CountVariadic<>:
            public integral_constant<
                size_t,
                0
            >
        {
            // static constexpr int num = 0;
        };




    } // namespace PropertyPrivate



template<typename _Tag, typename _Type, typename _NextProperty = NoProperty>
struct Property
{
    typedef _Tag Tag;
    typedef _Type Type;
    typedef _NextProperty NextProperty;

    Property(){}

    template<typename Head, typename... Args>
    Property(const Head &_val, Args ...args): val(_val), np(args...)
    {        
        static_assert(PropertyPrivate::CountProperty<Property>::value == 
                      PropertyPrivate::CountVariadic<Head, Args...>::value,
                       "Parameters number is not equal");
    }
    _Type val;
    _NextProperty np;
};

template<typename _Tag, typename _Type>
struct Property<_Tag, _Type, NoProperty>
{
    typedef _Tag Tag;
    typedef _Type Type;
    typedef NoProperty NextProperty;
    Property(){}

    Property(const _Type &_val):val(_val){}
    _Type val;
};


    namespace PropertyPrivate {



    } // namespace PropertyPrivate




















} // namepspace lz






#endif // PORPERTY_H