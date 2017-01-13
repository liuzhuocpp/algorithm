/*
 * iterator.h
 *
 *  Created on: 2015��11��4��
 *      Author: LZ
 */

#ifndef LZ_ITERATOR_H_
#define LZ_ITERATOR_H_
#include <iterator>
#include "lz/utility.h"
namespace lz{




/*
 * You should implement the below method:


forward iterator requires:
	Derived& operator++() ;
	typename Derived::reference operator*() const;
	bool operator==(Derived const& o) const;

bidirectional_iterator requires:
	Derived& operator--();

random_access_iterator requires:
	Derived& operator+=(typename Derived::difference_type n);
	typename Derived::difference_type operator-(Derived const& o) const;
	bool operator<(Derived const& o) const;


 */


template <
	class Derived,
	class Category,
	class T,
	class Distance = std::ptrdiff_t,
	class Pointer = T*,
	class Reference = T&
>
class IteratorFacade;

/*
 * Implementation for forward traversal iterators
 */

template <
	class Derived,
	class T,
	class Distance ,
	class Pointer,
	class Reference
>
class IteratorFacade<Derived, std::forward_iterator_tag, T, Distance, Pointer, Reference>
		: public std::iterator<std::forward_iterator_tag, T, Distance, Pointer, Reference>,
		  public lz::FacadeBase<Derived>,
		  public lz::EqualityComparableFacade<Derived>
{
	using Base = std::iterator<std::forward_iterator_tag, T, Distance, Pointer, Reference>;
public:
  	friend Derived operator++(Derived& a, int)
	{
		Derived	tmp(a);
		++a;
		return tmp;
	}

	typename Base::pointer operator->() const
	{
		return &*this->derived();
	}

//	bool operator!=(const Derived &o) const
//	{
//		return !(this->derived() == o);
//	}
};

/*
 * Implementation for bidirectional traversal iterators
 */

template <
	class Derived,
	class T,
	class Distance,
	class Pointer,
	class Reference
>
class IteratorFacade<Derived, std::bidirectional_iterator_tag, T, Distance, Pointer, Reference>
		: public IteratorFacade<Derived, std::forward_iterator_tag, T, Distance, Pointer, Reference>
{
	using Base = IteratorFacade<Derived, std::forward_iterator_tag, T, Distance, Pointer, Reference>;
public:
	using iterator_category = std::bidirectional_iterator_tag;

	friend Derived operator--(Derived&a, int)
	{
		Derived	tmp(a);
		--a;
		return tmp;
	}
};

// Implementation for random access traversal iterators
template <
	class Derived,
	class T,
	class Distance,
	class Pointer,
	class Reference
>
class IteratorFacade<Derived, std::random_access_iterator_tag, T, Distance, Pointer, Reference>
		: public IteratorFacade<Derived,std::bidirectional_iterator_tag, T, Distance, Pointer, Reference>
		, public lz::LessThanComparableFacade<Derived>
{
	using Base = IteratorFacade<Derived,std::bidirectional_iterator_tag, T, Distance, Pointer, Reference>;

public:
	using iterator_category = std::random_access_iterator_tag;

	friend Derived operator+(const Derived &a, typename Base::difference_type b)
	{
		Derived res = a;
		res += b;
		return res;
	}
	friend Derived operator+(typename Base::difference_type b, const Derived &a)
	{
		Derived res = a;
		res += b;
		return res;
	}
	Derived operator-(typename Base::difference_type b) const
	{
		Derived res = this->derived();
		res += -b;
		return res;
	}
	Derived& operator-=(typename Base::difference_type o)
	{
		this->derived() += -o;
		return this->derived();
	}

	typename Base::reference operator[](typename Base::difference_type o) const
	{
		return *(this->derived() + o);
	}


//	bool operator>(const Derived &o) const
//	{
//		return o < this->derived();
//	}
//	bool operator<=(const Derived &o) const
//	{
//		return !(o < this->derived());
//	}
//	bool operator>=(const Derived &o) const
//	{
//		return !(this->derived() < o);
//	}

};


template<typename Integer>
class IntegerIterator: public IteratorFacade<
							IntegerIterator<Integer>,
							std::random_access_iterator_tag,
							Integer,
							std::ptrdiff_t,
							Integer*,
							Integer>
{
	Integer m_val;
	using Base = IteratorFacade<
			IntegerIterator<Integer>,
			std::random_access_iterator_tag,
			Integer,
			std::ptrdiff_t,
			Integer*,
			Integer>;
	using Derived = IntegerIterator<Integer>;
public:
	IntegerIterator() = default;
	explicit IntegerIterator(Integer val):m_val(val) {}

	//forward iterator requires:
	Derived& operator++()
	{
		++ m_val;
		return this->derived();
	}
	typename Base::reference operator*() const
	{
		return m_val;
	}
	bool operator==(Derived const& f) const
	{
		return m_val == f.m_val;
	}

	//bidirectional_iterator requires:
	Derived& operator--()
	{
		-- m_val;
		return this->derived();
	}

	//random_access_iterator requires:
	Derived& operator+=(typename Base::difference_type n)
	{
		m_val += n;
		return this->derived();
	}
	typename Base::difference_type operator-(Derived const& f) const
	{
		return m_val - f.m_val;
	}

	bool operator<(IntegerIterator const& f) const
	{
		return m_val < f.m_val;
	}



};

/*

You should implement the below method:
forward iterator requires:
    Derived& operator++() ;
    typename Derived::reference operator*() const;
    bool operator==(Derived const& o) const;

bidirectional_iterator requires:
    Derived& operator--();

random_access_iterator requires:
    Derived& operator+=(typename Derived::difference_type n);
    typename Derived::difference_type operator-(Derived const& o) const;
    bool operator<(Derived const& o) const;
 */


// Derived must extend the IteratorAdapter, Base must be an iterator

//template <
//    class Derived,
//    class Base,
//    class Category,
//    class T,
//    class Distance = std::ptrdiff_t,
//    class Pointer = T*,
//    class Reference = T&
//>
//class IteratorAdapter;


// Should write IteratorAdapter By Different iterator Category
template <
    class Derived,
    class Base,
    class Category = typename std::iterator_traits<Base>::iterator_category,
    class T = typename std::iterator_traits<Base>::value_type,
    class Distance = typename std::iterator_traits<Base>::difference_type,
    class Pointer = typename std::iterator_traits<Base>::pointer,
    class Reference = typename std::iterator_traits<Base>::reference
>
class IteratorAdapter
        :public IteratorFacade<Derived, Category, T, Distance, Pointer, Reference>

{
public:
    IteratorAdapter() = default;
    IteratorAdapter(const Base& base):m_base(base) {}

    using BaseType = Base;
    const Base& base() const
    {
        return m_base;
    }
    Base& base()
    {
        return m_base;
    }
    Derived& operator++()
    {
        ++m_base;
        return this->derived();
    }
    Reference operator*() const
    {
        return *m_base;
    }
    bool operator==(Derived const& o) const
    {
        return m_base == o.m_base;
    }


    Derived& operator--()
    {
        --m_base;
        return this->derived();
    }


    Derived& operator+=(Distance n)
    {
        m_base += n;
        return this->derived();
    }
    Distance operator-(Derived const& o) const
    {
        return m_base - o.m_base;
    }
    bool operator<(Derived const& o) const
    {
        return m_base < o.m_base;
    }


private:
    Base m_base;
};






} // namespace lz



#endif /* LZ_ITERATOR_H_ */
