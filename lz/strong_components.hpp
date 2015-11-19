



#ifndef STRONG_COMPONENTS_H
#define STRONG_COMPONENTS_H

#include <utility>
#include <functional>
namespace lz {

using std::swap;
using std::less;



    namespace StrongComponentsPrivate {

    }

    template<typename G>
    struct StrongComponentsParams{
    	discoverTimeMap(){

    	}


    };

    template<typename G, typename ComponentMap, typename SCP = StrongComponentsParams>
    MapTraits<ComponentMap>::ValueType
	strongComponents(const G &g, ComponentMap cmap, SCP &&scp = StrongComponentsParams() )
    {

    }



}



#endif // STRONG_COMPONENTS_H




