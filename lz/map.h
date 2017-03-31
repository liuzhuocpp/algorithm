/*
 * map.h
 *
 *  Created on: 2015��8��5��
 *      Author: LZ
 */

/*
 * Map concept is similar to boost property map library,
 * but Map concept is sample. It only contain operator[] for
 * a Map object.
 */
#ifndef LZ_MAP_H_
#define LZ_MAP_H_

//#include <iostream>
#include <memory>
#include <type_traits>
namespace lz {

//using std::cout;
//using std::endl;

template<typename Map>
struct MapTraits
{
    using KeyType = typename Map::KeyType;
    using ValueType = typename Map::ValueType;
};

template<typename Key, typename Value>
struct MapFacade
{
    using KeyType = Key;
    using ValueType = Value;
};

template<typename _ValueType>
struct SharedArrayMap: public MapFacade<std::ptrdiff_t, _ValueType>
{
private:
    std::shared_ptr<_ValueType> sp;
public:
    SharedArrayMap(std::shared_ptr<_ValueType> sp) :
            sp(sp)
    {
    }
    SharedArrayMap() :
            sp(nullptr)
    {
    }

    SharedArrayMap(size_t n) :
            sp(new _ValueType[n], std::default_delete<_ValueType[]>())
    {
    }

    SharedArrayMap(size_t n, const _ValueType &val) :
                sp(new _ValueType[n], std::default_delete<_ValueType[]>())
    {

        std::fill(sp.get(), sp.get() + n, val);
    }

    auto operator[](ptrdiff_t d) const ->decltype(sp.get()[d])
    {
        return sp.get()[d];
    }
};



//template<typename _ValueType>
//struct UniqueArrayMap: public MapFacade<std::ptrdiff_t, _ValueType>
//{
//private:
//    std::unique_ptr<_ValueType, std::default_delete<_ValueType[]> > sp;
//public:
//
//    UniqueArrayMap(): sp(nullptr) {}
//
//    UniqueArrayMap(size_t n) :
//            sp(new _ValueType[n])
//    {
//    }
//
//    UniqueArrayMap(size_t n, const _ValueType &val) :
//                sp(new _ValueType[n])
//    {
//
//        std::fill(sp.get(), sp.get(), val);
//    }
//
//    auto operator[](ptrdiff_t d) const ->decltype(sp.get()[d])
//    {
//        return sp.get()[d];
//    }
//};




template<typename Key>
struct IdentityMap: public MapFacade<Key, Key>
{
    const Key& operator[](const Key &key) const
    {
        return key;
    }
};

template<typename I>
class IteratorMap: public MapFacade<
//Key
        typename std::iterator_traits<I>::difference_type,
        typename std::iterator_traits<I>::value_type
//Value

>
{
    I i;
public:
    IteratorMap() = default;
    IteratorMap(I i) :
            i(i)
    {
    }

    using Iterator = I;
    Iterator iterator() const
    {
        return i;
    }

    auto operator[](
            typename std::iterator_traits<I>::difference_type key) const ->decltype(i[key])
    {

        return i[key];
    }
};
template<typename I>
IteratorMap<I> makeIteratorMap(I i)
{
    return IteratorMap<I>(i);
}

template<typename FM, typename SM>
class ComposeMap: public MapFacade<typename MapTraits<FM>::KeyType,
        typename MapTraits<SM>::ValueType>
{
    FM fm;
    SM sm;
    using Base = MapFacade<typename MapTraits<FM>::KeyType, typename MapTraits<SM>::ValueType>;
public:
    using FirstMap = FM;
    using SecondMap = SM;

    explicit ComposeMap() = default;
    explicit ComposeMap(const FM &fm, const SM &sm) :
            fm(fm), sm(sm)
    {
    }

    FirstMap firstMap() const
    {
        return fm;
    }
    SecondMap secondMap() const
    {
        return sm;
    }

    template<typename Key>
    auto operator[](Key && key) const ->decltype(sm[fm[key]])
    {
        return sm[fm[key]];
    }
};

template<typename FM, typename SM>
ComposeMap<FM, SM> makeComposeMap(const FM &fm, const SM &sm)

{
    return ComposeMap<FM, SM>(fm, sm);
}



template<typename UniquePairAssociativeContainer>
struct AssociativeMap:
    MapFacade<
    typename UniquePairAssociativeContainer::key_type,
    typename UniquePairAssociativeContainer::mapped_type>
{
public:
    AssociativeMap(){}

    AssociativeMap(UniquePairAssociativeContainer &associativeContainer )
        :associativeContainer(&associativeContainer)
    {

    }

    template<typename KeyType>
    decltype(auto) operator[](KeyType && key) const
    {
        return (*associativeContainer)[key];
    }
private:
    UniquePairAssociativeContainer * associativeContainer = nullptr;

};


template<typename UniquePairAssociativeContainer>
struct ConstAssociativeMap:
    MapFacade<
    typename UniquePairAssociativeContainer::key_type,
    typename UniquePairAssociativeContainer::mapped_type>
{
public:
    ConstAssociativeMap(){}

    ConstAssociativeMap(const UniquePairAssociativeContainer &associativeContainer )
        :associativeContainer(&associativeContainer)
    {

    }

    template<typename KeyType>
    decltype(auto) operator[](KeyType && key) const
    {
        return associativeContainer->at(key);
    }
private:
    const UniquePairAssociativeContainer * associativeContainer = nullptr;

};







} // namespace lz

#endif /* LZ_MAP_H_ */

