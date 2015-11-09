/*
 * iterator.h
 *
 *  Created on: 2015Äê11ÔÂ4ÈÕ
 *      Author: LZ
 */

#ifndef LZ_ITERATOR_H_
#define LZ_ITERATOR_H_
#include <iterator>

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
		: public std::iterator<std::forward_iterator_tag, T, Distance, Pointer, Reference>
{

	using Base = std::iterator<std::forward_iterator_tag, T, Distance, Pointer, Reference>;
public:

//** You should implement the function
//	Derived& operator++();

	friend Derived operator++(Derived& a, int)
	{
		Derived	tmp(a);
		++a;
		return tmp;
	}

//** You should implement the function
//	typename Base::reference operator*() const;

	typename Base::pointer operator->() const
	{
		return &*this->derived();
	}

//** You should implement the function
//	bool operator==(const Derived &o) const;

	bool operator!=(const Derived &o) const
	{
		return !(this->derived() == o);
	}


protected:

	Derived& derived()
	{
		return *static_cast<Derived*>(this);
	}

	Derived const& derived() const
	{
		return *static_cast<Derived const*>(this);
	}
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

//** You should implement the function
//	Derived& operator--();

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
{
	using Base = IteratorFacade<Derived,std::bidirectional_iterator_tag, T, Distance, Pointer, Reference>;

public:
	using iterator_category = std::random_access_iterator_tag;

//** You should implement the function
//	Derived& operator+=(typename Base::difference_type o) const;

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


//** You should implement the function
//	typename Base::difference_type operator-(const Derived &a) const;

//** You should implement the function
//	bool operator<(const Derived &o) const;

;	bool operator>(const Derived &o) const
	{
		return o < this->derived();
	}
	bool operator<=(const Derived &o) const
	{
		return !(o < this->derived());
	}
	bool operator>=(const Derived &o) const
	{
		return !(this->derived() < o);
	}

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



} // namespace lz



#endif /* LZ_ITERATOR_H_ */
