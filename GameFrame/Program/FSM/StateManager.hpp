#ifndef GAMEFRAME_PROGRAM_FSM_STATE_MANAGER_HPP
#define GAMEFRAME_PROGRAM_FSM_STATE_MANAGER_HPP

/*
    Class for state creation, registration and pooling
    An ID represents a template object AND expiration time
    ======================================================
    Our data structure must be able to meet those requirements:
    1. Create a state pointer with registered ID
    2. Retrieve a pointer created before
    3. In 1, use a deactivated state if possible
    4. If a state is inactive for too long, destroy it
    ======================================================
    1.
        In order to accomplsih 1, we use a map from ID
    to a std::function<IState*()> (`mCreators`)
    2.
        In order to accomplish 4, we need a timer for each object.
    However, updating time of every state inside the pool might
    be inefficient. So we will instead use a global timeline
    (`mTotalTime`). The idea is whenever a state become inactive,
    we use its lifetime and `mTotalTime` to calculate it end-of-
    life moment. Then in every update call, we simply discards
    those of which EOL time is less than `mTotalTime`
        This can be done by using a set to keep track of inactive
    states that sorts them by removal time (mTimeline)
    3.
        How do we check if it's possible to pool a state from
    inactive pool to use? Then we need to know for each ID, how
    many states is still kept (have not met their EOL yet). If yes,
    we take one out and make it actice. Here a mapping from ID to
    inactive states of that ID is used (`mInactive`). So each time
    we remove/add a state pointer to `mTimeline` or `mInactive`, we
    must do the same for the other
    4.
        When retrieving a state pointer, we simply query its ID
    (using `mActiveHash`), push the corresponding object from active
    to inactive pool, then update `mTimeline` and let its life
    running down

*/

#include "IState.hpp"
#include "../../Utilize/Time.hpp"
#include "../../Utilize/CallAssert.hpp"
#include "../../Utilize/IDGenerator.hpp"
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory>

namespace HJUIK
{
    namespace FSM
    {
        namespace detail
        {
            // NOLINTBEGIN(*-transparent-functors)
            // State ID, expiration time and the state itself
            struct Info
            {
				IState::ID Identifier;
				std::unique_ptr<IState> State;
				Utilize::Time Removal; // Life expectancy
				Utilize::Time Duration; // The time the state lives in inactive mode
				std::size_t HashID; // Used for sorting those with same `Removal`

				Info(IState::ID identifier, Utilize::Time duration, std::unique_ptr<IState> state, std::size_t hashID)
                    : Identifier(std::move(identifier))
                    , State(std::move(state))
                    , Duration(duration)
                    , HashID(hashID)
                {}

                auto operator < (const Info& right) const -> bool
                {
                    if (Removal != right.Removal)
                    {
						return Removal < right.Removal;
					}
					return HashID < right.HashID;
				}
			};

			// Comparer according to time
            struct SPtrCmpLess
            {
                auto operator() (const std::shared_ptr<Info>& first, const std::shared_ptr<Info>& last) const -> bool
                {
					return (*first) < (*last);
				}
			};
            // NOLINTEND(*-transparent-functors)
		} // namespace detail

		class StateManager
        {
        public:
			using SPtr		= std::shared_ptr<detail::Info>;
			using CreatorFn = std::function<SPtr()>;
			using CustomSet = std::set<SPtr, detail::SPtrCmpLess>;
			static constexpr std::size_t MAX_STATE_AVAILABLE = 200;

			auto update(Utilize::Time deltaTime) -> void;
			auto create(const IState::ID& identifier) -> std::pair<IState*, std::size_t>;
			// DONOT use the pointer after calling this function
            auto retrieve(std::size_t ID) -> void; // NOLINT

			template <typename StateType, typename... Args>
			auto registerState(const IState::ID& identifier, Utilize::Time exprTime, const Args&... args) -> void
            {
				HJUIK_ASSERT(mCreators.find(identifier) == mCreators.end(), "ID \"", identifier, "\" registered twice");
				mCreators.emplace(identifier, [=] {
					std::unique_ptr<IState> state = std::make_unique<StateType>(args...);
					return std::make_shared<detail::Info>(identifier, exprTime, std::move(state), mIDGenerator.generate());
				});
			}

		private:
			Utilize::Time mTotalTime;

			std::unordered_map<IState::ID, CreatorFn> mCreators;
			Utilize::IDGenerator<MAX_STATE_AVAILABLE> mIDGenerator;

			std::unordered_map<IState::ID, std::unordered_set<SPtr>> mActive;
			std::unordered_map<std::size_t, SPtr> mActiveHash;

            std::unordered_map<IState::ID, CustomSet> mInactive;
			CustomSet mTimeline;
		};
	} // namespace FSM
} // namespace HJUIK

#endif