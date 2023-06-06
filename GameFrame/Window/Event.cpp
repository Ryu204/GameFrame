#include "Event.hpp"

namespace HJUIK
{
    Event::Event()
        : mData(EventType::Unknown { })
    { }

    auto Event::getData() -> Event::Data&
    {
        return mData;
    }

    auto Event::getData() const -> const Event::Data&
    {
        return mData;
    }
} // namespace HJUIK