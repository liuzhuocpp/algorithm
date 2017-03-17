#ifndef PORPERTY_H
#define PORPERTY_H




#include <iostream>
#include <typeinfo>
#include <vector>

#include <type_traits>
#include <tuple>
#include <lz/utility.h>
namespace lz {

using std::integral_constant;
using std::cout;
using std::endl;





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


        template <typename Property, typename CntTag, typename Tuple, size_t CntId>
		struct AssignTuple;

        template<typename Property, typename Tag, typename QueryTag>
        struct GetType;

    } // namespace PropertyPrivate


template<typename P>
struct PropertySize:public std::integral_constant<
	size_t, PropertySize<typename P::NextProperty>::value + 1> {};

template<>
struct PropertySize<NoProperty>
: public integral_constant<size_t, 0> { };


template<typename _Tag, typename _ValueType, typename _NextProperty = NoProperty >
struct Property: public _NextProperty
{
    template<typename QueryTag, typename Tag, typename Property>
    friend struct PropertyPrivate::Get;

    template <typename Property, typename CntTag, typename Tuple, size_t CntId>
    friend struct PropertyPrivate::AssignTuple;

    using Tag = _Tag;
    using ValueType = _ValueType;
    using NextProperty = _NextProperty;

    template<typename QueryTag>
    using QueryValueType = typename PropertyPrivate::GetType<Property, Tag, QueryTag>::ValueType;

    template<typename QueryTag>
    using QueryPropertyType = typename PropertyPrivate::GetType<Property, Tag, QueryTag>::PropertyType;

    Property() = default;

//    Property(const ValueType &value, const NextProperty & np):
//    	NextProperty(np),m_value(value){}

    explicit Property(ValueType &&value, const NextProperty & np):
		NextProperty(np), m_value(value)
    {
//    	cout << "FFF"<< endl;
    }

    Property(const ValueType &value): m_value(value){}
    Property(ValueType &&value): m_value(std::move(value)){}

protected:
    template<typename ...Args, size_t N>
    Property(const std::tuple<Args...> &tp, std::integral_constant<size_t, N> t):
		m_value(std::get<N>(tp)), NextProperty(tp, std::integral_constant<size_t, N + 1>())
	{

	}
public:
    template<typename ...Args>
	Property( const std::tuple<Args...> &tp):
		Property(tp, std::integral_constant<size_t, 0>())
	{
//    	cout << "SBSB" << endl;
	}



//    template <class... UTypes>
//    Property(const std::tuple<UTypes...>& tp)
//	{
//    	PropertyPrivate::AssignTuple<Property, Tag, std::tuple<UTypes...>, 0>::apply(*this, tp);
//	}

//    template <class... UTypes>
//    Property(std::tuple<UTypes...>&& tp)
//	{
//    	PropertyPrivate::AssignTuple<Property, Tag, std::tuple<UTypes...>, 0>::apply(*this, tp);
//	}

//    template <class... UTypes>
//    Property& operator=(const std::tuple<UTypes...>& tp)
//	{
//    	PropertyPrivate::AssignTuple<Property, Tag, std::tuple<UTypes...>, 0>::apply(*this, tp);
//	}
//
//    template <class... UTypes>
//    Property& operator=(std::tuple<UTypes...>&& tp)
//	{
//    	PropertyPrivate::AssignTuple<Property, Tag, std::tuple<UTypes...>, 0>::apply(*this, tp);
//	}


    template<typename QueryTag>
    QueryValueType<QueryTag>& operator[](QueryTag tag)
	{
    	using Base = QueryPropertyType<QueryTag>;
    	return this->Base::m_value;
	}
    template<typename QueryTag>
	const QueryValueType<QueryTag>& operator[](QueryTag tag) const
	{
		using Base = QueryPropertyType<QueryTag>;
		return this->Base::m_value;
	}

    template<typename QueryTag>
    constexpr bool contains(QueryTag tag) const
    {
    	return !std::is_same<decltype((*this)[tag]), NoProperty>::value;
    }

protected:
    ValueType m_value;

};





    namespace PropertyPrivate {

	template<typename Property, typename Tag, typename QueryTag>
	struct GetType
	{
	private:
		using NextProperty = typename Property::NextProperty  ;
		using NextTag = typename NextProperty::Tag;
	public:
		using ValueType = typename GetType<NextProperty, NextTag, QueryTag>::ValueType;
		using PropertyType = typename GetType<NextProperty, NextTag, QueryTag>::PropertyType;
	};

	template<typename Property, typename QueryTag>
	struct GetType<Property, QueryTag, QueryTag>
	{
		using ValueType = typename Property::ValueType;
		using PropertyType = Property;
	};

	template<typename Property, typename QueryTag>
	struct GetType<Property, NoProperty, QueryTag>
	{
		using ValueType = NoProperty;
		using PropertyType = NoProperty;
	};





	template <typename Property, typename Tag, typename Tuple, size_t Id>
	struct AssignTuple
	{
		static void apply(Property &p, const Tuple &t)
		{
//			typename std::tuple_element<Id, Tuple>::type x = std::get<Id>(t);

			p[Tag()] = std::get<Id>(t);

			AssignTuple<typename Property::NextProperty,
						typename Property::NextProperty::Tag,
						Tuple,
						Id + 1>::apply(p, t);
		}
		static void apply(Property &p, Tuple &&t)
		{
//			 typename std::tuple_element<Id, Tuple>::type x = std::get<Id>(t);

			 p[Tag()] = std::get<Id>(t);

			 AssignTuple<typename Property::NextProperty,
						 typename Property::NextProperty::Tag,
						 Tuple,
						 Id + 1>::apply(p, t);
		}

	};

	template <typename Property, typename Tuple, size_t CntId>
	struct AssignTuple<Property, NoProperty, Tuple, CntId>
	{
		static void apply(Property &p, const Tuple &t)
		{

		}
	};












    } // namespace PropertyPrivate





//template<typename Tag, typename ValueType, typename NextProperty, typename QueryTag>
//auto get(Property<Tag, ValueType, NextProperty> & p, QueryTag tag)
//-> decltype(PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p))
//{
//    return PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p);
//}
//
//template<typename Tag, typename ValueType, typename NextProperty, typename QueryTag>
//auto get(const Property<Tag, ValueType, NextProperty> & p, QueryTag tag)
//-> decltype(PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p))
//{
//    return PropertyPrivate::Get<Property<Tag, ValueType, NextProperty>, Tag, QueryTag>::get(p);
//}






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
