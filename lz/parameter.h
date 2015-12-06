/*
 * parameter.h
 *
 *  Created on: 2015Äê11ÔÂ30ÈÕ
 *      Author: LZ
 */

#ifndef LZ_PARAMETER_H_
#define LZ_PARAMETER_H_

#include "lz/property.h"
#include <utility>

namespace lz{

struct ParamNotFound {
	using Keyword = ParamNotFound;
	using Reference = ParamNotFound;
	using Next = ParamNotFound;


};

namespace Parameter{

template<typename ArgList, typename QueryKeyword, typename Keyword>
struct GetArgListImpl
{
	using ArgListType = typename GetArgListImpl<typename ArgList::Next,
							QueryKeyword, typename ArgList::Next::Keyword>::ArgListType;
};

template<typename ArgList, typename QueryKeyword>
struct GetArgListImpl<ArgList, QueryKeyword, QueryKeyword>
{
	using ArgListType = ArgList;
};

template<typename ArgList, typename QueryKeyword>
struct GetArgListImpl<ArgList, QueryKeyword, ParamNotFound>
{
	using ArgListType = ParamNotFound;
};

template<typename ArgList, typename QueryKeyword>
struct GetArgList
{
	using ArgListType = typename GetArgListImpl<ArgList, QueryKeyword, typename ArgList::Keyword>::ArgListType;
};



template<typename ArgList, typename QueryKeyword, typename Base = typename GetArgList<ArgList, QueryKeyword>::ArgListType >
struct GetReference
{
	static auto apply(const ArgList &ag)
	->decltype(std::forward<typename Base::Reference>(ag.Base::ref))
	{
		return std::forward<typename Base::Reference>(ag.Base::ref);
	}
};

template<typename ArgList, typename QueryKeyword>
struct GetReference<ArgList, QueryKeyword, ParamNotFound>
{
	static ParamNotFound apply(const ArgList &ag)
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
};


template<typename ArgList, typename Default,
		 typename Base = typename GetArgList<ArgList, typename Default::Keyword>::ArgListType>
struct ComputeDefault
{
	static auto apply(const ArgList &ar, Default o)
	->decltype(ar[typename Default::Keyword()])
	{
		return ar[typename Default::Keyword()];
	}
};
template<typename ArgList, typename Default>
struct ComputeDefault<ArgList, Default, ParamNotFound>
{
	static auto apply(const ArgList &ar, Default o)
	->decltype(std::forward<typename Default::Reference>(o.ref))
	{
		return std::forward<typename Default::Reference>(o.ref);
	}
};






template<typename ArgList, typename LazyDefault,
		 typename Base = typename GetArgList<ArgList, typename LazyDefault::Keyword>::ArgListType>
struct ComputeLazyDefault
{
	static auto apply(const ArgList &ar, LazyDefault o)
	->decltype(ar[typename LazyDefault::Keyword()])
	{
		return ar[typename LazyDefault::Keyword()];
	}
};
template<typename ArgList, typename LazyDefault>
struct ComputeLazyDefault<ArgList, LazyDefault, ParamNotFound>
{
	static auto apply(const ArgList &ar, LazyDefault o)
	->decltype(o.ref())
	{
		return o.ref();
	}
};

//template<typename Tag>
//struct Keyword;

template<typename _Keyword, typename _Reference, typename _Next = ParamNotFound>
struct ArgList:public _Next
{
	using Keyword = _Keyword;
	using Reference = _Reference;
	using Next = _Next;
	Reference ref;
	ArgList(Reference ref):ref(std::forward<Reference>(ref)){}
	ArgList(Reference ref, const Next &next):ref(std::forward<Reference>(ref)), Next(next) {}

	ArgList(const ArgList &o):ref(std::forward<Reference>(o.ref)), Next(*static_cast<const Next*>(&o) ){}

	template<typename OKeyword, typename OReference>
	ArgList<OKeyword, OReference, ArgList> operator,(ArgList<OKeyword, OReference> o) const
	{
		return ArgList<OKeyword, OReference, ArgList>
				(std::forward<OReference>(o.ref), *this);
	}
public:
	template<typename QueryKeyword>
	auto operator[](QueryKeyword keyword) const
	->decltype(GetReference<ArgList, QueryKeyword>::apply(*this))
	{
		return GetReference<ArgList, QueryKeyword>::apply(*this);
	}



	template<typename QueryKeyword, typename _Default>
	auto operator[](Default<QueryKeyword, _Default> o) const

	->decltype(ComputeDefault<ArgList, Default<QueryKeyword, _Default>>::apply(*this, o))
	{
		return ComputeDefault<ArgList, Default<QueryKeyword, _Default>>::apply(*this, o);
	}



	template<typename QueryKeyword, typename _Default>
	auto operator[](LazyDefault<QueryKeyword, _Default> o) const

	->decltype(ComputeLazyDefault<ArgList, LazyDefault<QueryKeyword, _Default> >::apply(*this, o))
	{
		return ComputeLazyDefault<ArgList, LazyDefault<QueryKeyword, _Default> >::apply(*this, o);
	}

};




template<typename _Tag>
struct Keyword
{
	using Tag = _Tag;

	template<typename T>
	auto operator=(T &&t) const
	->ArgList<Keyword, decltype(std::forward<T>(t))>
	{
		return ArgList<Keyword, decltype(std::forward<T>(t))>(std::forward<T>(t));
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



}} // namespace lz::Parameter


















#endif /* LZ_PARAMETER_H_ */















