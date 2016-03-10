#ifndef SHARED_PTR_H
#define SHARED_PTR_H



#include <cstddef>
#include <algorithm>
namespace lz {

	namespace SharedPtrPrivate {




	}// namespace SharedPtrPrivate


template<typename T>
class SharedPtr
{
	using Data = std::pair<T*, std::size_t>;
	Data *data;
public:
	constexpr SharedPtr(): data(nullptr) {}

	explicit SharedPtr(T * ptr):data(new Data(ptr, 1))
	{

	}
	SharedPtr(const SharedPtr & o)
	{
		data = o.data;
		data->second ++;
	}
	SharedPtr(SharedPtr && o)
	{
		data = o.data;
		o.data = nullptr;
	}

	~SharedPtr()
	{
		data->second --;
		if(data->second == 0)
		{
			delete data->first;
			delete data;
		}
	}



};





}// namespace lz

#endif // SHARED_PTR_H
