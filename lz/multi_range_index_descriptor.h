
#ifndef LZ_MULTI_RANGE_INDEX_DESCRIPTOR_H_
#define LZ_MULTI_RANGE_INDEX_DESCRIPTOR_H_



namespace lz {



template<typename I, I N>
struct MultiRangeIndexDescriptor
{

    using DescriptorType = I;
private:
    DescriptorType m_descriptor;

public:
    constexpr MultiRangeIndexDescriptor():MultiRangeIndexDescriptor(0, 0)
    {
    }

    constexpr MultiRangeIndexDescriptor(DescriptorType rangeId, DescriptorType index)
    {
        static_assert(std::is_unsigned<DescriptorType>::value);

        set(rangeId, index);

    }
    void set(DescriptorType rangeId, DescriptorType index)
    {
        m_descriptor = rangeBegin(rangeId) + index;
    }

    constexpr DescriptorType descriptor() const
    {
        return m_descriptor;
    }

    constexpr DescriptorType rangeIndex() const
    {
        return m_descriptor / (totalRange() / rangesNumber());
    }

    constexpr DescriptorType index() const
    {
        return m_descriptor - rangeBegin(rangeIndex());
    }

    static constexpr DescriptorType totalRange()
    {
        return std::numeric_limits<DescriptorType>::max();
    }

    static constexpr DescriptorType rangesNumber()
    {
        return N;
    }

    static constexpr DescriptorType rangeBegin(DescriptorType rangeId)
    {

        return totalRange() / rangesNumber() * rangeId;
    }
    static constexpr DescriptorType rangeSize(DescriptorType rangeId)
    {
        assert(rangeId >= 0 && rangeId < rangesNumber());

        if(rangeId + 1 < rangesNumber())
        {
            return totalRange() / rangesNumber();
        }
        else
        {
            return totalRange() - (rangesNumber() - 1) * (totalRange() / rangesNumber());
        }
    }


};







} // namespace lz





#endif /* LZ_MULTI_RANGE_INDEX_DESCRIPTOR_H_ */
