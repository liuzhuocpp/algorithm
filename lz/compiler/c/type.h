/*
 * type.h
 *
 *  Created on: 2017年4月22日
 *      Author: LZ
 */

#ifndef LZ_COMPILER_C_TYPE_H_
#define LZ_COMPILER_C_TYPE_H_



namespace lz {



struct Type
{
//private:
    enum class Category
    {
        Int,
        Float,
        Double,

//        Array,
//        Struct,
        Unknown,
    };

public:

    bool isInt() const
    {
        return category == Category::Int;
    }

    bool isFloat() const
    {
        return category == Category::Float;
    }

    bool isDouble() const
    {
        return category == Category::Double;
    }

    bool isArray() const
    {
        return !arrayDimensions.empty();
    }




    Category category;

    std::vector<int> arrayDimensions;
    int getWidth() const
    {
        int ans = 1; // 暂时先设为1
        for(auto x: arrayDimensions)
        {
            ans *= x;
        }
        return ans;

    }

    Type subArray() const
    {
        Type ans(this->category);
        ans.arrayDimensions.assign(this->arrayDimensions.begin() + 1, this->arrayDimensions.end());
        return ans;
    }



    Type(Category cat = Category::Unknown):
        category(cat){}




    operator Category()
    {
        return category;
    }

    template <class Char, class Traits>
    friend std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os,
               const Type&  id)
    {

        os << "Type(";


        if(id.category == Category::Float) os << "float" ;
        else if(id.category == Category::Int) os << "int";
        else if(id.category == Category::Double) os << "double";
        else os << "unknown";

        if(!id.arrayDimensions.empty())
        os << id.arrayDimensions << ")";


        return os;
    }



};


















} // namespace lz





#endif /* LZ_COMPILER_C_TYPE_H_ */
