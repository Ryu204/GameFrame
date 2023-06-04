#include "Event.hpp"

namespace HJUIK
{
    Event::Data& Event::getData()
    {
        return mData;
    }

    const Event::Data& Event::getData() const
    {
        return mData;
    }
} // namespace HJUIK