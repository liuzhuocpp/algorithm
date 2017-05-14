/*
 * named_enum.h
 *
 *  Created on: 2017年5月12日
 *      Author: LZ
 */

#ifndef LZ_NAMED_ENUM_H_
#define LZ_NAMED_ENUM_H_

#include <lz/optional.h>


#define OVERLOAD_DISPATCHER(_1, _2, NAME, ... ) NAME
#define make_enum_element_name_X(...) OVERLOAD_DISPATCHER(__VA_ARGS__, make_enum_element_name_X2, make_enum_element_name_X1)(__VA_ARGS__)

#define make_enum_element_name_X2(key, name) name,
#define make_enum_element_name_X1(key) #key,


#define make_enum_element_X(key, ...) key,


#define LZ_MAKE_NAMED_ENUM(enum_class_name, enum_names_name, enum_list) \
    enum class enum_class_name \
    {\
        enum_list(make_enum_element_X) \
    };\
    static const char* enum_names_name(enum_class_name enum_obj) {\
        static const char* A[]= {\
            enum_list(make_enum_element_name_X) \
        };\
        return A[static_cast<unsigned>(enum_obj)];\
    }



#define name_to_enum_first_X(...) OVERLOAD_DISPATCHER(__VA_ARGS__, name_to_enum_first_X2, name_to_enum_first_X1)(__VA_ARGS__)
#define name_to_enum_second_X(...) OVERLOAD_DISPATCHER(__VA_ARGS__, name_to_enum_second_X2, name_to_enum_second_X1)(__VA_ARGS__)

// 第一次展开恰好有1个参数的
#define name_to_enum_first_X1(key)    #key) != a && !hasFound) ansId ++; else { hasFound = true; }
#define name_to_enum_first_X2(key, name) #key) == ""); if(!hasFound) ansId++;

// 第二次展开恰好有有2个参数的
#define name_to_enum_second_X1(key)   if(!hasFound) ansId++;
#define name_to_enum_second_X2(key, name) if(name != a && !hasFound) ansId ++; else hasFound = true;

// 第三次获取对应ansId的那个enum
#define name_to_enum_third_X(key, ...)   key;
#define name_to_enum_third_X_prefix  if(cntId++ == ansId) ans =


#define name_to_enum_first_X_prefix if(

#define name_to_enum_first_X_prefix2 (

#define lz_name_to_enum(name_to_enum, enum_class, enum_list, _tranFunc) \
    static lz::Optional<enum_class> name_to_enum(std::string a) {\
        int ansId = 0; bool hasFound = false; enum_class ans; auto tranFunc = _tranFunc;\
        enum_list( name_to_enum_first_X_prefix tranFunc  name_to_enum_first_X_prefix2 name_to_enum_first_X)\
        if(!hasFound) ansId = 0, hasFound = false;\
        enum_list( name_to_enum_second_X)\
        \
        int cntId = 0;\
        enum_list( name_to_enum_third_X_prefix enum_class:: name_to_enum_third_X ) \
        if(hasFound) return lz::Optional<enum_class>(ans);\
        return lz::Optional<enum_class>();\
    };


//        static const std::unordered_map<std::string, enum_class> A = {\
//            enum_list(name_to_enum_first_X1)
//        };\
//        return A[a];\



#define get_list_range_element_X(key, ...) key;
#define get_list_range_begin_element_X(enum_class_name) if(!beginInit) beginInit = true, begin = enum_class_name::
#define get_list_range_end_element_X(enum_class_name)   end = enum_class_name::

#define LZ_GET_ENUM_LIST_RANGE(enum_class_name, enum_list) \
    []() {\
        bool beginInit = false;   enum_class_name begin, end;\
        enum_list(get_list_range_begin_element_X(enum_class_name) get_list_range_element_X )\
        enum_list(get_list_range_end_element_X(enum_class_name) get_list_range_element_X )\
        return std::make_pair(begin, end);\
    }()




namespace lz {


} // namespace lz





#endif /* LZ_NAMED_ENUM_H_ */
