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
		try{
            mHandlers[index].erase(ID);
            mIDGenerator.erase(ID);
        } catch (std::exception& e){
            // TODO: Create a proper debug class to deal with exceptions
            throw(std::runtime_error("HJUIK::EventManager: Cannot delete handler " +
                std::to_string(ID) + ":\n\t" + e.what()));
        }
	}
} // namespace HJUIK
