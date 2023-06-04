#ifndef GAMEFRAME_WINDOW_EVENT_MANAGER_HPP
#define GAMEFRAME_WINDOW_EVENT_MANAGER_HPP

/*
    Manage event processing and let other objects register callback
    The number of maximum available handlers in total is defined
*/

#include "Event.hpp"
#include "../Utilize/IDGenerator.hpp"

#include <vector>
#include <functional>

namespace HJUIK
{
    // Central class for managing callbacks 
    class EventManager
    {
    public:
        // Type is specific event type
        using Handler = std::function<void(const Event::Data&)>;
        // Process event based on registered handlers
        auto processEvent(const Event& event) -> void const;
        // Register handler for callback when event Type happens
        // Returns the ID of this handler
        template <typename Type>
        auto registerHandler(Handler& handler) -> std::size_t;
        // Delete the handler with corresponding ID
        template <typename Type>
        auto deleteHandler(std::size_t ID) -> void;
    private:
        std::array<std::unordered_map<std::size_t, Handler>, Event::getTypeCount()> mHandlers;
        static constexpr std::size_t MAX_HANDLERS_COUNT = 200;
        Utilize::IDGenerator<MAX_HANDLERS_COUNT> mIDGenerator;
    };

    template <typename Type>
    auto EventManager::registerHandler(Handler& handler) -> std::size_t
    {
        std::size_t typeIndex = Utilize::VariantHelper<Type, Event::Data>::getIndex();
        mHandlers[typeIndex].emplace(mIDGenerator.generate(), handler);
    }

    template <typename Type>
    auto EventManager::deleteHandler(std::size_t ID) -> void
    {
        std::size_t typeIndex = Utilize::VariantHelper<Type, Event::Data>::getIndex();
        mHandlers[typeIndex].erase(ID);
    }
} // namespace HJUIK

#endif