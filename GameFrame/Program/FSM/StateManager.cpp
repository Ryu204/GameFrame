#include "StateManager.hpp"
#include "../../Utilize/CallAssert.hpp"

namespace HJUIK
{
    namespace FSM
    {
        auto StateManager::update(Utilize::Time deltaTime) -> void
        {
			mTotalTime += deltaTime;

            while (!mTimeline.empty())
            {
				SPtr leastTimeLeft = *mTimeline.begin();
                // If no more state needs to be destroyed, stop
                if (leastTimeLeft->Removal > mTotalTime)
                {
					break;
				}
				// Now we are sure this element is to be destroyed
                mInactive[leastTimeLeft->Identifier].erase(leastTimeLeft);
				mTimeline.erase(leastTimeLeft);
				mIDGenerator.erase(leastTimeLeft->HashID);
			}
		}

		auto StateManager::create(const IState::ID& identifier) -> std::pair<IState*, std::size_t>
        {
			HJUIK_ASSERT(mCreators.find(identifier) != mCreators.end(), "Unregistered FSM state ID \"", identifier, "\"");

			// If there are no inactive state, create one
			if (mInactive[identifier].empty())
            {
                auto newState = mCreators[identifier]();
				newState->State->load();
				IState* res = newState->State.get();

				mActiveHash[newState->HashID] = newState;
                mActive[identifier].insert(newState);

				return {res, newState->HashID};
			}

            // Else we just take one from the pool
            SPtr pooled = *mInactive[identifier].begin();

            // Safely moved the Info object from inactive to active
            mInactive[identifier].erase(pooled);
            mTimeline.erase(pooled);

            mActive[identifier].insert(pooled);
            mActiveHash[pooled->HashID] = pooled;

            pooled->State->load();

            // Return the state pointer and its ID
			IState* res = pooled->State.get();
			return { res, pooled->HashID };
		}

		auto StateManager::retrieve(std::size_t ID) -> void // NOLINT
        {
			HJUIK_ASSERT(mActiveHash.count(ID), "No state with specified ID created (", ID, ")");

            SPtr toBeInactive = mActiveHash[ID];

            // Move the deactivated state to the inactive pool
			mActiveHash.erase(ID);
			mActive[toBeInactive->Identifier].erase(toBeInactive);
			toBeInactive->State->unload();

			mInactive[toBeInactive->Identifier].insert(toBeInactive);
			mTimeline.insert(toBeInactive);

            // Mark its death moment
			toBeInactive->Removal = mTotalTime + toBeInactive->Duration;
		}
	} // namespace FSM
} // namespace HJUIK