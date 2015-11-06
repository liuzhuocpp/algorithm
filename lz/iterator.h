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

//using std::iterator;


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
	/*
	 * You should implement the below method:

	 reference dereference() const;
	 void increment();
	 void decrement();
	 bool equal(Derived const& f) const;
	 bool less(Derived const& f) const;
	 void advance(typename difference_type n) const;
	 difference_type distance(Derived const& f) const;
	 */
	using Base = std::iterator<std::forward_iterator_tag, T, Distance, Pointer, Reference>;
public:
	Derived& operator++()
	{
		this->derived().increment();
		return this->derived();
	}
	Derived operator++(int)
	{
		Derived	tmp(this->derived().derived());
		--*this;
		return tmp;
	}
	bool operator==(const Derived &o) const
	{
		return this->derived().equal(o);
	}
	bool operator!=(const Derived &o) const
	{
		return !(*this == o);
	}
	typename Base::reference operator*() const
	{
		return this->derived().dereference();
	}
	typename Base::pointer operator->() const
	{
		return &this->derived().dereference();
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

	Derived& operator--()
	{
		this->derived().decrement();
		return this->derived();
	}
	Derived operator--(int)
	{
		Derived	tmp(this->derived());
		--*this;
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

	friend Derived operator+(const Derived &a, typename Base::difference_type b)
	{
		Derived res = a;
		res.advance(b);
		return res;
	}
	friend Derived operator+(typename Base::difference_type b, const Derived &a)
	{
		Derived res = a;
		res.advance(b);
		return res;
	}
	typename Base::difference_type operator-(const Derived &a) const
	{
		return this->derived().distance(a);
	}
	Derived operator-(typename Base::difference_type b) const
	{
		Derived res = *this;
		res.advance(-b);
		return res;
	}
	bool operator<(const Derived &o) const
	{
		return this->derived().less(o);
	}
	bool operator>(const Derived &o) const
	{
		return o < *this;
	}
	bool operator<=(const Derived &o) const
	{
		return !(o < *this);
	}
	bool operator>=(const Derived &o) const
	{
		return !(*this < o);
	}
	Derived& operator+=(typename Base::difference_type o) const
	{
		this->derived().advance(o);
		return this->derived();
	}
	Derived& operator-=(typename Base::difference_type o) const
	{
		this->derived().advance(-o);
		return this->derived();
	}
	typename Base::reference operator[](typename Base::difference_type o) const
	{
		return this->derived().dereference(*this + o);
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
public:
	IntegerIterator() = default;
	explicit IntegerIterator(Integer val):m_val(val) {}

	typename Base::reference dereference() const
	{
		return m_val;
	}
	void increment()
	{
		++ m_val;
	}
	void decrement()
	{
		--m_val;
	}
	bool equal(IntegerIterator const& f) const
	{
		return m_val == f.m_val;
	}
	bool less(IntegerIterator const& f) const
	{
		return m_val < f.m_val;
	}
	void advance(typename Base::difference_type n) const
	{
		m_val += n;
	}
	typename Base::difference_type distance(IntegerIterator const& f) const
	{
		return m_val - f.m_val;
	}


};
//bool equal(Derived const& f) const;
//	 * bool less(Derived const& f) const;
//	 * void advance(typename difference_type n) const;
//	 * difference_type distance(Derived const& f) const;



} // namespace lz



#endif /* LZ_ITERATOR_H_ */
