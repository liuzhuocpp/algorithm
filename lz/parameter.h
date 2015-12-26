/*
 * parameter.h
 *
 *  Created on: 2015Äê11ÔÂ30ÈÕ
 *      Author: LZ
 */

#ifndef LZ_PARAMETER_H_
#define LZ_PARAMETER_H_

//#include "lz/property.h"
#include <utility>

namespace lz{


struct ParamNotFound {
	using Keyword = ParamNotFound;
	using Reference = ParamNotFound;
	using Next = ParamNotFound;
};
struct EmptyParamPack;


namespace Parameter{



template<typename _Tag>
struct Keyword;
template<typename _Keyword, typename _Default>
struct Default;

template<typename _Keyword, typename _Default>
struct LazyDefault;

template<typename ArgList, typename QueryKeyword, typename Keyword>
struct GetParamPackImpl
{
	using ParamPackType = typename GetParamPackImpl<typename ArgList::Next,
							QueryKeyword, typename ArgList::Next::Keyword>::ParamPackType;
};

template<typename ArgList, typename QueryKeyword>
struct GetParamPackImpl<ArgList, QueryKeyword, QueryKeyword>
{
	using ParamPackType = ArgList;
};

template<typename ArgList, typename QueryKeyword>
struct GetParamPackImpl<ArgList, QueryKeyword, EmptyParamPack>
{
	using ParamPackType = EmptyParamPack;
};

template<typename ArgList, typename QueryKeyword>
struct GetParamPack
{
	using ParamPackType = typename GetParamPackImpl<ArgList, QueryKeyword, typename ArgList::Keyword>::ParamPackType;
};



template<typename ArgList, typename QueryKeyword, typename Base = typename GetParamPack<ArgList, QueryKeyword>::ParamPackType >
struct GetReference
{
	static auto apply(const ArgList &ag)
	->decltype(std::forward<typename Base::Reference>(ag.Base::ref))
	{
//		cout << "HH " << endl;
//		cout << ag.ref << endl;
		return std::forward<typename Base::Reference>(ag.Base::ref);
	}
};

template<typename ParamPack, typename QueryKeyword>
struct GetReference<ParamPack, QueryKeyword, EmptyParamPack>
{
	static ParamNotFound apply(const ParamPack &ag)
	{
		return ParamNotFound();
	}
};

template<typename _Keyword, typename _Default>
struct Default
{
	using Keyword =_Keyword;
	using Reference =_Default;

	Reference ref;
	Default(Reference ref):ref(std::forward<Reference>(ref)) {}

	Default(const Default &o)
	:ref(std::forward<Reference>(o.ref)) {}
};

template<typename _Keyword, typename _Default>
struct LazyDefault
{
	using Keyword =_Keyword;
	using Reference =_Default;

	Reference ref;

	LazyDefault(Reference ref):ref(std::forward<Reference>(ref)) {}

	LazyDefault(const LazyDefault &o)
	:ref(std::forward<Reference>(o.ref)) {}
};


template<typename ParamPack, typename Default,
		 typename Base = typename GetParamPack<ParamPack, typename Default::Keyword>::ParamPackType>
struct ComputeDefault
{
	static auto apply(const ParamPack &ar, Default o)
	->decltype(ar[typename Default::Keyword()])
	{
		return ar[typename Default::Keyword()];
	}
};
template<typename ParamPack, typename Default>
struct ComputeDefault<ParamPack, Default, EmptyParamPack>
{
	static auto apply(const ParamPack &ar, Default o)
	->decltype(std::forward<typename Default::Reference>(o.ref))
	{
		return std::forward<typename Default::Reference>(o.ref);
	}
};



template<typename ParamPack, typename LazyDefault,
		 typename Base = typename GetParamPack<ParamPack, typename LazyDefault::Keyword>::ParamPackType>
struct ComputeLazyDefault
{
	static auto apply(const ParamPack &ar, LazyDefault o)
	->decltype(ar[typename LazyDefault::Keyword()])
	{
		return ar[typename LazyDefault::Keyword()];
	}
};
template<typename ParamPack, typename LazyDefault>
struct ComputeLazyDefault<ParamPack, LazyDefault, EmptyParamPack>
{
	static auto apply(const ParamPack &ar, LazyDefault o)
	->decltype(o.ref())
	{
		return o.ref();
	}
};





} // namespace Parameter



struct EmptyParamPack
{

	using Keyword = EmptyParamPack;
	using Reference = EmptyParamPack;
	using Next = EmptyParamPack;

	template<typename QTag>
	ParamNotFound operator[](Parameter::Keyword<QTag>) const
	{
		return ParamNotFound();
	}

	template<typename QK, typename D>
	auto operator[](Parameter::Default<QK, D> o) const
	->decltype(std::forward<decltype(o.ref)>(o.ref))
	{
		return std::forward<decltype(o.ref)>(o.ref);
	}


	template<typename QK, typename D>
	auto operator[](Parameter::LazyDefault<QK, D> o) const
	->decltype(o.ref())
	{
		return o.ref();
	}


//	template<typename QTag>
//	auto operator[](Parameter::Keyword<QTag>) const
//	->decltype(Parameter::GetReference<EmptyParamPack, Parameter::Keyword<QTag> >::apply(*this))
//	{
//		return Parameter::GetReference<EmptyParamPack, Parameter::Keyword<QTag> >::apply(*this);
//	}
//
//	template<typename QK, typename D>
//	auto operator[](Parameter::Default<QK, D> o) const
//
//	->decltype(Parameter::ComputeDefault<EmptyParamPack, Parameter::Default<QK, D>>::apply(*this, o))
//	{
//		return Parameter::ComputeDefault<EmptyParamPack, Parameter::Default<QK, D>>::apply(*this, o);
//	}
//
//
//
//	template<typename QK, typename D>
//	auto operator[](Parameter::LazyDefault<QK, D> o) const
//	->decltype(Parameter::ComputeLazyDefault<EmptyParamPack, Parameter::LazyDefault<QK, D> >::apply(*this, o))
//	{
//		return Parameter::ComputeLazyDefault<EmptyParamPack, Parameter::LazyDefault<QK, D> >::apply(*this, o);
//	}



};



namespace Parameter{






template<typename _Keyword, typename _Reference, typename _Next = EmptyParamPack>
struct ParamPack:public _Next
{


	template<typename QK>
	using QueryReference = decltype(GetReference<ParamPack, typename std::decay<QK>::type>::apply(

			std::declval<ParamPack>()
			   ) );

	_Reference ref;
	ParamPack(_Reference _ref):ref(std::forward<_Reference>(*&_ref)  )
	{
//		cout << std::is_same<int&&, decltype(ref)>::value << "|||||||" <<  endl;
//		cout << "CON" <<  _ref << endl;
//		cout << "CON" <<  ref << endl;
//		cout << "REF: " << &ref << endl;
	}
	ParamPack(_Reference ref, const _Next &next):ref(std::forward<_Reference>(ref)), _Next(next) {}

	ParamPack(const ParamPack &o)
	:ref(std::forward<_Reference>(o.ref)), _Next(*static_cast<const _Next*>(&o) )
//	:ParamPack(std::forward<_Reference>(o.ref), o.Next)
	{
//		cout << "HHEHEHEEH" << endl;

	}

	template<typename OKeyword, typename OReference>
	ParamPack<OKeyword, OReference, ParamPack> operator,(ParamPack<OKeyword, OReference> o) const
	{
		return ParamPack<OKeyword, OReference, ParamPack>
				(std::forward<OReference>(o.ref), *this);
	}

	template<typename QTag>
	auto operator[](Parameter::Keyword<QTag>) const
	->decltype(GetReference<ParamPack, Parameter::Keyword<QTag> >::apply(*this))
	{
//		cout << "SB " << ref << endl;
//		cout << "REF: " << &ref << endl;
		return GetReference<ParamPack, Parameter::Keyword<QTag> >::apply(*this);
	}

	template<typename QK, typename D>
	auto operator[](Default<QK, D> o) const

	->decltype(ComputeDefault<ParamPack, Default<QK, D>>::apply(*this, o))
	{
		return ComputeDefault<ParamPack, Default<QK, D>>::apply(*this, o);
	}



	template<typename QK, typename D>
	auto operator[](LazyDefault<QK, D> o) const
	->decltype(ComputeLazyDefault<ParamPack, LazyDefault<QK, D> >::apply(*this, o))
	{
		return ComputeLazyDefault<ParamPack, LazyDefault<QK, D> >::apply(*this, o);
	}

	using Keyword = _Keyword;
	using Reference = _Reference;
	using Next = _Next;

};




template<typename _Tag>
struct Keyword
{
	using Tag = _Tag;

	template<typename T>
	auto operator=(T &&t) const
	->ParamPack<Keyword, decltype(std::forward<T>(t))>
	{
//		cout <<" T: " << t << endl;
		return ParamPack<Keyword, decltype(std::forward<T>(t))>(std::forward<T>(t));
	}


	template<typename T>
	auto operator|(T &&t) const
	->Default<Keyword, decltype(std::forward<T>(t)) >
	{
		return Default<Keyword, decltype(std::forward<T>(t)) >(std::forward<T>(t));
	}

	template<typename T>
	auto operator||(T &&t) const
	->LazyDefault<Keyword, decltype(std::forward<T>(t))>
	{
		return LazyDefault<Keyword, decltype(std::forward<T>(t))>(std::forward<T>(t));
	}




	static const Keyword<Tag> instance;
};
template<typename _Tag>
const Keyword<_Tag> Keyword<_Tag>::instance = {};












#define LZ_PARAMETER_KEYWORD(tag_namespace,name)                    \
    namespace tag_namespace                                         \
    {                                                               \
		struct name                                                 \
		{                                                           \
		};                                                          \
    }                                                               \
    namespace                                                       \
    {                                                               \
		::lz::Parameter::Keyword<tag_namespace::name> const& name   \
		= ::lz::Parameter::Keyword<tag_namespace::name>::instance;  \
    }																\



#define LZ_PARAMETER_KEYWORD_TAG_NAMESPACE(name)   \
	LZ_PARAMETER_KEYWORD(tag, name)



}// namespace lz::Parameter



//template<typename ArgList, typename QueryKeyword>
//struct GetParamPack
//{
//	using ParamPackType = typename GetParamPackImpl<ArgList, QueryKeyword, typename ArgList::Keyword>::ParamPackType;
//};



template<typename ParamPack, typename KeywordType, typename Default = ParamNotFound>
using ChooseParamType =	typename std::conditional<

	!std::is_same<typename Parameter::GetParamPack<ParamPack, KeywordType>::ParamPackType::Reference, ParamNotFound >::value,

	typename Parameter::GetParamPack<ParamPack, KeywordType>::ParamPackType::Reference,


	Default>::type;




} // namespace lz


















#endif /* LZ_PARAMETER_H_ */















