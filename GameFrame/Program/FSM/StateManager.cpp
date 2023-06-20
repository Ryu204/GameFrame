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
			}
		}

		auto StateManager::create(const IState::ID& identifier) -> IState*
        {
			HJUIK_ASSERT(mCreators.find(identifier) != mCreators.end(), "Unregistered FSM state ID \"", identifier, "\"");
			// If there are no inactive state, create one
            if (mInactive[identifier].empty())
            {
                auto newState = mCreators[identifier]();
                mActive[identifier].insert(newState);
				IState* res = newState->State.get();
				mActiveHash[res] = newState;
				return res;
			}
			// Else we just take one from the pool
            SPtr pooled = *mInactive[identifier].begin();
            // Safely moved the Info object from inactive to active
            mInactive[identifier].erase(pooled);
            mTimeline.erase(pooled);
            mActive[identifier].insert(pooled);
            // Return
			// pooled->State->reset();
			IState* res = pooled->State.get();
			mActiveHash[res] = pooled;
			return res;
		}

		auto StateManager::retrieve(IState* state) -> void
        {
			HJUIK_ASSERT(mActiveHash.find(state) != mActiveHash.end(), "No state with specified pointer created");
			SPtr toBeInactive = mActiveHash[state];
			mActiveHash.erase(state);
			toBeInactive->Removal = mTotalTime + toBeInactive->Duration;
			mInactive[toBeInactive->Identifier].insert(toBeInactive);
			mTimeline.insert(toBeInactive);
		}
	} // namespace FSM
} // namespace HJUIK