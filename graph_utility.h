#ifndef GRAPH_UTILITY_H
#define GRAPH_UTILITY_H


namespace lz {

/*
	this header file define some base classes and functions
*/
struct NoProperty{};

	namespace GraphUtilityPrivate {
	}



	

template<typename T, typename P>
struct PropertyMap
{
    typedef T Type;
    typedef P Properties;
    T P::*map;
    explicit PropertyMap(T P::*_map):map(_map) {}
};

template<typename T, typename D>
PropertyMap<T, D> propertyMap(T D::*p)
{
    return PropertyMap<T, D>(p);
}
		






}




#endif // GRAPH_UTILITY_H
