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
struct KeywordWithDefault
{
	using Keyword =_Keyword;
	using Reference =_Default;

	Reference ref;
	KeywordWithDefault(Reference ref):ref(std::forward<Reference>(ref)) {}

	KeywordWithDefault(const KeywordWithDefault &o)
	:ref(std::forward<Reference>(o.ref)) {}
};

template<typename _Keyword, typename _Default>
struct KeywordWithLazyDefault
{
	using Keyword =_Keyword;
	using Reference =_Default;

	Reference ref;

	KeywordWithLazyDefault(Reference ref):ref(std::forward<Reference>(ref)) {}
};


template<typename ArgList, typename KeywordWithDefault,
		 typename Base = typename GetArgList<ArgList, typename KeywordWithDefault::Keyword>::ArgListType>
struct ComputeDefault
{
	static auto apply(const ArgList &ar, KeywordWithDefault o)
	->decltype(ar[typename KeywordWithDefault::Keyword()])
	{
//		cout << "JJJ " <<endl;
		return ar[typename KeywordWithDefault::Keyword()];
	}
};
template<typename ArgList, typename KeywordWithDefault>
struct ComputeDefault<ArgList, KeywordWithDefault, ParamNotFound>
{
	static auto apply(const ArgList &ar, KeywordWithDefault o)
	->decltype(std::forward<typename KeywordWithDefault::Reference>(o.ref))
	{
//		cout << "JJJ------------ " <<endl;
		return std::forward<typename KeywordWithDefault::Reference>(o.ref);
	}
};

template<typename Tag>
struct Keyword;

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
//		cout << "HEHEH" << endl;
		return GetReference<ArgList, QueryKeyword>::apply(*this);
	}

//	template<typename Tag>
//	auto operator[](Keyword<Tag>) const
//	->decltype(GetReference<ArgList, Keyword<Tag>>::apply(*this))
//	{
//		cout << "HEHEH" << endl;
//		return GetReference<ArgList, Keyword<Tag>>::apply(*this);
//	}

	template<typename QueryKeyword, typename Default>
	auto operator[](KeywordWithDefault<QueryKeyword, Default> o) const

	->decltype(ComputeDefault<ArgList, KeywordWithDefault<QueryKeyword, Default>>::apply(*this, o))
	{
//		cout << "SBFFF" << endl;
		return ComputeDefault<ArgList, KeywordWithDefault<QueryKeyword, Default>>::apply(*this, o);
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
	->KeywordWithDefault<Keyword, decltype(std::forward<T>(t)) >
	{
		cout << "FJJFJFJF" << endl;
		return KeywordWithDefault<Keyword, decltype(std::forward<T>(t)) >(std::forward<T>(t));
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















