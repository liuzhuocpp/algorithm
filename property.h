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
        struct CountProperty
        : public integral_constant<size_t, CountProperty<typename P::NextProperty>::value + 1>
        { };

        template<>
        struct CountProperty<NoProperty>
        : public integral_constant<size_t, 0>
        { };

        template<typename... T>
        struct CountVariadic;

        template<typename Head, typename...T>
        struct CountVariadic<Head, T...>
        : public integral_constant< size_t, CountVariadic<T...>::value + 1>
        { };

        template<>
        struct CountVariadic<>
        : public integral_constant<size_t, 0>
        { };




    } // namespace PropertyPrivate



template<typename _Tag, typename _Value, typename _NextProperty = NoProperty>
struct Property
{
    typedef _Tag Tag;
    typedef _Value Value;
    typedef _NextProperty NextProperty;

    Property(){}

    template<typename Head, typename... Args>
    Property(const Head &value, Args ...args): m_value(value), np(args...)
    {        
        static_assert(PropertyPrivate::CountProperty<Property>::value == 
                      PropertyPrivate::CountVariadic<Head, Args...>::value,
                       "Parameters number is not equal");
    }
    _Value m_value;
    _NextProperty np;
};

template<typename _Tag, typename _Value>
struct Property<_Tag, _Value, NoProperty>
{
    typedef _Tag Tag;
    typedef _Value Value;
    typedef NoProperty NextProperty;

    Property(){}
    Property(const Value &value):m_value(value){}

    _Value m_value;    
};

    namespace PropertyPrivate {

        template<typename QueryTag, typename Tag, typename Property>
        struct Get
        {
            typedef typename Property::NextProperty  NextProperty;

            static auto get(Property &p) 
            -> decltype(Get<QueryTag, typename NextProperty::Tag, NextProperty>::get(p.np))
            {
                return Get<QueryTag, typename NextProperty::Tag, NextProperty>::get(p.np);
            }

            static auto get(const Property &p) 
            -> decltype(Get<QueryTag, typename NextProperty::Tag, NextProperty>::get(p.np))
            {
                return Get<QueryTag, typename NextProperty::Tag, NextProperty>::get(p.np);
            }    

        };

        template<typename QueryTag, typename Property>
        struct Get<QueryTag, QueryTag, Property>
        {
            typedef typename Property::Value Value;

            static Value& get(Property &p)
            {
                return p.m_value;
            }

            static const Value& get(const Property &p)
            {
                return p.m_value;
            }
        };


    } // namespace PropertyPrivate






template<typename QueryTag, typename Tag, typename Value, typename NextProperty>
auto get(Property<Tag, Value, NextProperty> & p) 
-> decltype(PropertyPrivate::Get<QueryTag, Tag, Property<Tag, Value, NextProperty> >::get(p))
{
    return PropertyPrivate::Get<QueryTag, Tag, Property<Tag, Value, NextProperty> >::get(p);
}

template<typename QueryTag, typename Tag, typename Value, typename NextProperty>
auto get(const Property<Tag, Value, NextProperty> & p) 
-> decltype(PropertyPrivate::Get<QueryTag, Tag, Property<Tag, Value, NextProperty> >::get(p))
{
    return PropertyPrivate::Get<QueryTag, Tag, Property<Tag, Value, NextProperty> >::get(p);
}



























} // namepspace lz






#endif // PORPERTY_H