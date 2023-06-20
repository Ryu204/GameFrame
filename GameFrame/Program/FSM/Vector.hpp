#ifndef GAMEFRAME_PROGRAM_FSM_STACK_HPP
#define GAMEFRAME_PROGRAM_FSM_STACK_HPP

#include "IState.hpp"
#include "StateManager.hpp"
#include "../../Utilize/CallAssert.hpp"
#include <vector>
#include <variant>
#include <unordered_map>
#include <functional>

namespace HJUIK
{
    namespace FSM
    {
        namespace Request
        {
            struct Push
            {
				IState::ID Identifier;
			};
            struct ErasePtr
            {
                IState* Pointer;
			};
			struct EraseIndex {
				int Index;
			};
			struct EraseIdentifier{
				IState::ID Identifier;
				enum Modifier { ALL, LAST } Modifier;
			};
            struct Clear {};
			using Type = std::variant<Push, ErasePtr, EraseIndex, EraseIdentifier, Clear>;
		} // namespace Request

		class StateVector
        {
        public:
            // Elements management...

            auto clear() -> void;
			auto push(const IState::ID& identifier) -> void;
            // Erase the element with corresponding index
            // Allow negative index (start from -1 as last element)
			auto erase(int index) -> void;
            // Erase the specific element by pointer
			auto erase(IState* element) -> void;
			// Erase the element with correspoding identifier name
			// If `all` is set to false, only the element with greatest index is deleted
			auto erase(const IState::ID& identifier, bool all = true) -> void;
			auto isEmpty() const -> bool;
			auto getSize() const -> std::size_t;

			// Logic functions...
			auto getRequest(const Request::Type& request) -> void;
			auto update(Utilize::Time deltaTime) -> void;
			auto processInput(const Event& event) -> void;
            auto render(Graphics::IOpenGLContext& context) -> void;

            // Register a StateType object with corresponding identifier
            // `poolTime` is how long (in seconds) the state will be kept in the pool before destroyed
            // `args...` are StateType's constructor parameters
			template <typename StateType, typename... Args>
			auto registerState(const IState::ID& identifier, Utilize::Time poolTime, const Args&... args) -> void;

		private:
            auto flushRequests() -> void;
			auto eraseAtIndex(std::size_t index) -> void;

			struct Pack
            {
				IState::ID Identifier;
				std::size_t ID;
				IState* State;
				StateManager* Manager;

				Pack(IState::ID identifier, StateManager* manager)
                    : Identifier(std::move(identifier))
                    , Manager(manager)
                {
					auto newState = Manager->create(Identifier);
					State		  = newState.first;
					ID			  = newState.second;
				}
				Pack(Pack&&) = default;
				Pack(const Pack&) = default;
				auto operator=(Pack&&) -> Pack& = default;
				auto operator=(const Pack&) -> Pack& = default;
				~Pack()								 = default;

				auto returnID() const -> void
                {
					Manager->retrieve(ID);
				}
			};

			std::vector<Request::Type> mPendingChanges;
			std::vector<Pack> mVector;
			StateManager mManager;
		};

		template <typename StateType, typename... Args>
        auto StateVector::registerState(const IState::ID& identifier, Utilize::Time poolTime, const Args&... args) -> void
        {
			mManager.registerState<StateType, StateVector*, Args...>(identifier, poolTime, this, args...);
		}
	} // namespace FSM
} // namespace HJUIK

#endif