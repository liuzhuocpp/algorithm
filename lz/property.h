#ifndef PORPERTY_H
#define PORPERTY_H




#include <iostream>
#include <typeinfo>
#include <vector>

#include <type_traits>

namespace lz {

using std::integral_constant;
using std::cout;
using std::endl;



struct NoProperty {
	using Tag = NoProperty;
	using ValueType = NoProperty;
	using NextProperty = NoProperty;


};
//static const NoProperty::instance = NoProperty();


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


        template<typename QueryTag, typename Tag, typename Property>
        struct Get;



    } // namespace PropertyPrivate



template<typename _Tag, typename _ValueType, typename _NextProperty = NoProperty >
struct Property: public _NextProperty
{
    template<typename QueryTag, typename Tag, typename Property>
    friend struct PropertyPrivate::Get;

    using Tag = _Tag;
    using ValueType = _ValueType;
    using NextProperty = _NextProperty;

    constexpr Property() = default;

    template<typename Head, typename... Args>
    explicit Property(const Head &value, Args ...args)
    : m_value(value), NextProperty(args...)
    {
        static_assert(PropertyPrivate::CountProperty<Property>::value ==
                      PropertyPrivate::CountVariadic<Head, Args...>::value,
                       "Parameters number is not equal");
    }


    template<typename Head, typename... Args>
    explicit Property(Head &&value, Args ...args)
    : m_value(value), NextProperty(args...)
    {

        static_assert(PropertyPrivate::CountProperty<Property>::value ==
                      PropertyPrivate::CountVariadic<Head, Args...>::value,
                       "Parameters number is not equal");
    }


    template<typename QueryTag>
    auto operator[](QueryTag tag)
    ->decltype(PropertyPrivate::Get<Property, Tag, QueryTag>::get(*this))
	{
    	return PropertyPrivate::Get<Property, Tag, QueryTag>::get(*this);
	}
    template<typename QueryTag>
	auto operator[](QueryTag tag) const
    ->decltype(PropertyPrivate::Get<Property, Tag, QueryTag>::get(*this))
	{
		return PropertyPrivate::Get<Property, Tag, QueryTag>::get(*this);
	}

    template<typename QueryTag>
    constexpr bool contains(QueryTag tag) const
    {
    	return !std::is_same<decltype((*this)[tag]), NoProperty::ValueType>::value;
    }
private:
    ValueType m_value;

};





    namespace PropertyPrivate {

        template<typename Property, typename Tag, typename QueryTag>
        struct Get
        {
            typedef typename Property::NextProperty  NextProperty;

            static auto get(Property &p)
            ->decltype(Get<NextProperty, typename NextProperty::Tag, QueryTag>::
            		   get(static_cast<NextProperty&>(p)))
            {
                return Get<NextProperty, typename NextProperty::Tag, QueryTag>::
                       get(static_cast<NextProperty&>(p));
            }

            static auto get(const Property &p)
            ->decltype(Get<NextProperty, typename NextProperty::Tag, QueryTag>::
            		   get(static_cast<const NextProperty&>(p)))
            {
                return Get<NextProperty, typename NextProperty::Tag, QueryTag>::
                       get(static_cast<const NextProperty&>(p));
            }

        };

        template<typename Property, typename QueryTag>
        struct Get<Property, QueryTag, QueryTag>
        {
            typedef typename Property::ValueType ValueType;

            static ValueType& get(Property &p)
            {
                return p.m_value;
            }

            static const ValueType& get(const Property &p)
            {
                return p.m_value;
            }
        };

        template<typename QueryTag>
        struct Get<NoProperty, NoProperty::Tag, QueryTag>
        {

            static NoProperty::ValueType get(const NoProperty &p)
            {
                return NoProperty::ValueType();
            }
        };



    } // namespace PropertyPrivate





template<typename Tag, typename ValueType, typename NextProperty, typename QueryTag>
auto get(Property<Tag, ValueType, NextProperty> & p, QueryTag tag)
-> decltype(PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p))
{
    return PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p);
}

template<typename Tag, typename ValueType, typename NextProperty, typename QueryTag>
auto get(const Property<Tag, ValueType, NextProperty> & p, QueryTag tag)
-> decltype(PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p))
{
    return PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p);
}






template<typename P>
struct ChooseDefineProperties
{
	P properties;
	void set(const P &_p) { properties = _p; }
};
template<> struct ChooseDefineProperties<NoProperty>
{
	void set(const NoProperty &_p) {  }
};





















} // namepspace lz






#endif // PORPERTY_H
