#include "EventManager.hpp"
#include <iostream>

namespace HJUIK
{
	auto EventManager::processEvent(const Event& event) const -> void
	{
		const auto eventIndex = event.getData().index();
		for (const auto& func : mHandlers[eventIndex]) {
			func.second(event);
		}
	}

	auto EventManager::deleteHandler(std::size_t ID) -> void // NOLINT
	{
		const auto index = mIDToTypeIndex[ID];
        mHandlers[index].erase(ID);
        mIDGenerator.erase(ID);
	}
} // namespace HJUIK
