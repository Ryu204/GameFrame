#ifndef GAMEFRAME_PROGRAM_FSM_STACK_HPP
#define GAMEFRAME_PROGRAM_FSM_STACK_HPP

#include "FSMState.hpp"
#include "../Utilize/CallAssert.hpp"
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
			struct EraseIdentifier
            {
				IState::ID Identifier;
				enum Modifier { ALL, LAST } Modifier;
			};
            struct Clear {};
			using Type = std::variant<Push, ErasePtr, EraseIndex, EraseIdentifier, Clear>;
		} // namespace Request

		class StateVector
        {
        public:
            using UPtr = std::unique_ptr<IState>;
            using CreatorFn = std::function<UPtr()>;

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

			template <typename StateType, typename... Args>
			auto registerState(const IState::ID& identifier, const Args&... args) -> void;

		private:
            auto flushRequests() -> void;

			std::vector<Request::Type> mPendingChanges;
			std::vector<std::pair<IState::ID, UPtr>> mVector;
            std::unordered_map<IState::ID, CreatorFn> mRegistered;
        };

        template <typename StateType, typename... Args>
        auto StateVector::registerState(const IState::ID& identifier, const Args&... args) -> void
        {
			HJUIK_ASSERT(mRegistered.find(identifier) == mRegistered.end(), "State ID already registered: ", identifier);

			mRegistered.emplace(identifier, [this, identifier, args...]() -> UPtr {
				return std::make_unique<StateType>(this, args...);
            });
		}
    } // namespace FSM
} // namespace HJUIK

#endif