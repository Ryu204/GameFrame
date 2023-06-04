#include "EventManager.hpp"

namespace HJUIK
{
    auto EventManager::processEvent(const Event& event) -> void const
    {
        std::size_t eventIndex = event.getData().index();
        for (const auto& func : mHandlers[eventIndex])
            func.second(event.getData());
    }
} // namespace HJUIK