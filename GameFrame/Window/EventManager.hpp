#ifndef GAMEFRAME_WINDOW_EVENT_MANAGER_HPP
#define GAMEFRAME_WINDOW_EVENT_MANAGER_HPP

/*
    Manage event processing and let other objects register callback
*/

#include <vector>
#include <functional>

namespace HJUIK
{
    class EventManager
    {
    public:
        // Type is specific event type
        template <typename Type>
        using Handler = std::function<void(const Type&)>;
    
    };
}

#endif