/*
 * type.h
 *
 *  Created on: 2017年4月22日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_TYPE_H_
#define LZ_COMPILER_C_TYPE_H_



namespace lz {



//struct Type
//{
//    enum class Category
//    {
//        Int,
//        Float,
//        Double,
//
//        Array,
////        Struct,
//        Unknown,
//    };
//
//
//private:
//    Category m_category;
//    std::vector<int> arrayDimensions;
//public:
//
//    Category getCategory() const
//    {
//        if(arrayDimensions.empty())
//        {
//            return m_category;
//        }
//        else
//        {
//            return Category::Array;
//        }
//    }
//    Category baseType() const
//    {
//        return m_category;
//    }
//
//
//
//
//
//    int getWidth() const
//    {
//        int ans = 1; // 暂时先设为1
//        for(auto x: arrayDimensions)
//        {
//            ans *= x;
//        }
//        return ans;
//
//    }
//
//    void addDimension(int x)
//    {
//        arrayDimensions.push_back(x);
//    }
//
//    Type subArray() const
//    {
//        Type ans(this->m_category);
//        ans.arrayDimensions.assign(this->arrayDimensions.begin() + 1, this->arrayDimensions.end());
//        return ans;
//    }
//
//
//
//    Type(Category cat = Category::Unknown):
//        m_category(cat){}
//
//
//
//
////    operator Category()
////    {
////        return m_category;
////    }
//
//    template <class Char, class Traits>
//    friend std::basic_ostream<Char, Traits>&
//    operator<<(std::basic_ostream<Char, Traits>& os,
//               const Type&  id)
//    {
//
//        os << "Type(";
//
//
//        if(id.m_category == Category::Float) os << "float" ;
//        else if(id.m_category == Category::Int) os << "int";
//        else if(id.m_category == Category::Double) os << "double";
//        else os << "unknown";
//
//        if(!id.arrayDimensions.empty())
//        {
//            auto cntRangeSplitter = currentRangeSplitter(os);
//            os <<commaRangeSplitter << id.arrayDimensions << cntRangeSplitter;
//
//        }
//        os << ")";
//
//
//        return os;
//    }
//
//
//
//};
//

















} // namespace lz





#endif /* LZ_COMPILER_C_TYPE_H_ */
