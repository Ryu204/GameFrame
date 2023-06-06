#include "EventManager.hpp"

namespace HJUIK
{
    auto EventManager::processEvent(const Event& event) const -> void
    {
        std::size_t eventIndex = event.getData().index();
        for (const auto& func : mHandlers[eventIndex])
        {
            func.second(event);
        }
    }

    auto EventManager::deleteHandler(std::size_t ID) -> void // NOLINT
    {
        std::size_t index = mIDToTypeIndex[ID];
        mHandlers[index].erase(ID);
    }
} // namespace HJUIK