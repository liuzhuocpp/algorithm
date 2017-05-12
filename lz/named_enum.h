/*
 * named_enum.h
 *
 *  Created on: 2017年5月12日
 *      Author: LZ
 */

#ifndef LZ_NAMED_ENUM_H_
#define LZ_NAMED_ENUM_H_




#define make_enum_element_X(key, ...) key,
#define make_enum_element_name_X(key, ...) #key,

#define make_enum(enum_class_name, enum_names_name, enum_list) \
    enum class enum_class_name \
    {\
        enum_list(make_enum_element_X) \
    };\
    static constexpr const char * enum_names_name[] = {\
        enum_list(make_enum_element_name_X) \
    };


#define get_list_range_element_X(key, ...) key;
#define get_list_range_begin_element_X(enum_class_name) if(!beginInit) beginInit = true, begin = enum_class_name::
#define get_list_range_end_element_X(enum_class_name)   end = enum_class_name::
#define get_list_range(enum_class_name, enum_list) \
    []() {\
        bool beginInit = false;   enum_class_name begin, end;\
        enum_list(get_list_range_begin_element_X(enum_class_name) get_list_range_element_X )\
        enum_list(get_list_range_end_element_X(enum_class_name) get_list_range_element_X )\
        return std::make_pair(begin, end);\
    }()




namespace lz {


} // namespace lz





#endif /* LZ_NAMED_ENUM_H_ */
