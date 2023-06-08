#ifndef GAMEFRAME_WINDOW_EVENT_MANAGER_HPP
#define GAMEFRAME_WINDOW_EVENT_MANAGER_HPP

/*
	Manage event processing and let other objects register callback
	The number of maximum available handlers in total is defined
*/

#include <functional>
#include <vector>

#include "../Utilize/IDGenerator.hpp"
#include "Event.hpp"

namespace HJUIK
{
	// Central class for managing events callbacks
	class EventManager
	{
	public:
		using Handler = std::function<void(const Event&)>;
		// Process event based on registered handlers
		auto processEvent(const Event& event) const -> void;
		// Register handler for callback when event Type happens
		// Returns the ID of this handler
		template <typename Type, typename Func>
		auto registerHandler(Func&& handler) -> std::size_t;
		// Delete the handler with corresponding ID
		auto deleteHandler(std::size_t ID) -> void; // NOLINT
	private:
		std::array<std::unordered_map<std::size_t, Handler>, Event::getTypeCount()> mHandlers;
		std::unordered_map<std::size_t, std::size_t> mIDToTypeIndex;
		static constexpr std::size_t MAX_HANDLERS_COUNT = 200;
		Utilize::IDGenerator<MAX_HANDLERS_COUNT> mIDGenerator;
	};

	template <typename Type, typename Func>
	auto EventManager::registerHandler(Func&& handler) -> std::size_t
	{
		std::size_t typeIndex = Utilize::VariantHelper<Type, Event::Data>::index();
		std::size_t res		  = mIDGenerator.generate();
		mHandlers[typeIndex].emplace(res,
			[handler = std::forward<Func>(handler)](const Event& event) { handler(std::get<Type>(event.getData())); });
		mIDToTypeIndex[res] = typeIndex;
		return res;
	}
} // namespace HJUIK

#endif