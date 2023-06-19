#include "Vector.hpp"
#include "../../Utilize/CallAssert.hpp"
#include "../../Utilize/Variant.hpp"

namespace HJUIK
{
    namespace FSM
    {
        auto StateVector::clear() -> void
        {
			mVector.clear();
		}

        auto StateVector::push(const IState::ID& identifier) -> void
        {
            HJUIK_ASSERT(mRegistered.find(identifier) != mRegistered.end(), "Unregistered FSM state");
			mVector.emplace_back(std::pair<IState::ID, UPtr>{identifier, mRegistered[identifier]()});
		}

        auto StateVector::erase(int index) -> void
        {
            HJUIK_ASSERT(!mVector.empty(), "Cannot erase element from empty vector");
			int newIndex = index < 0 ? index + static_cast<int>(mVector.size()) : index;
			HJUIK_ASSERT(0 <= newIndex && newIndex < mVector.size(), "Vector index out of range: ", index);
			mVector.erase(mVector.begin() + newIndex);
		}

        auto StateVector::erase(IState* element) -> void
        {
            HJUIK_ASSERT(!mVector.empty(), "Cannot erase element from empty vector");
			bool found = false;
			int index = 0;
			for ( ; index < mVector.size(); ++index)
            {
                if (mVector[index].second.get() == element)
                {
					found = true;
					break;
				}
			}
			HJUIK_ASSERT(found, "Element to erase with pointer not found");
			mVector.erase(mVector.begin() + index);
		}

		auto StateVector::erase(const IState::ID& identifier, bool all) -> void
        {
            HJUIK_ASSERT(!mVector.empty(), "Cannot erase element from empty vector");
			std::vector<int> erasedIndices;
			for (int i = 0; i < mVector.size(); ++i)
            {
				if (mVector[i].first == identifier)
                {
					erasedIndices.push_back(i);
				}
			}
			HJUIK_ASSERT(!erasedIndices.empty(), "State to erase with ID \"", identifier, "\" not found");
            // Delete last element
            if (!all)
            {
				mVector.erase(mVector.begin() + erasedIndices.back());
			}
            // Delete all element
            else
            {
                for (auto i = erasedIndices.rbegin(); i != erasedIndices.rend(); ++i)
                {
					mVector.erase(mVector.begin() + (*i));
				}
			}
		}

		auto StateVector::isEmpty() const -> bool
        {
			return mVector.empty();
		}

		auto StateVector::getSize() const -> std::size_t
        {
			return mVector.size();
		}

		auto StateVector::getRequest(const Request::Type& request) -> void
        {
            mPendingChanges.push_back(request);
        }

		auto StateVector::flushRequests() -> void
        {
            static const auto visitor = Utilize::VariantVisitor
            {
                // Push a new state
                [this](const Request::Push& call)
                {
                    push(call.Identifier);
                },
                // Erase a state
                [this](const Request::ErasePtr& call)
                {
                    erase(call.Pointer);
                },
                [this](const Request::EraseIndex& call)
                {
                    erase(call.Index);
                },
                [this](const Request::EraseIdentifier& call)
                {
                    erase(call.Identifier, call.Modifier == Request::EraseIdentifier::ALL);
                },
                // Clear the stack
                [this](const Request::Clear&)
                {
                    clear();
                }
            };
            for (const auto& req : mPendingChanges)
            {
                std::visit(visitor, req);
            }
            mPendingChanges.clear();
        }

        auto StateVector::update(Utilize::Time deltaTime) -> void
        {
			for (auto itr = mVector.rbegin(); itr != mVector.rend(); itr++) {
				// If the current state blocks update of underneath state
                if (!(itr->second->update(deltaTime)))
                {
                    break;
                }
		    }
			flushRequests();
        }

        auto StateVector::processInput(const Event& event) -> void
        {
            for (auto itr = mVector.rbegin(); itr != mVector.rend(); itr++)
            {
                // If the current state blocks events for lower state
                if (!(itr->second->processInput(event)))
                {
                    break;
                }
            }
            flushRequests();
        }

        auto StateVector::render(Graphics::IOpenGLContext& context) -> void
        {
            for (auto itr = mVector.rbegin(); itr != mVector.rend(); itr++)
            {
                // If the current state is the last state that should be drawn
                if (!(itr->second->render(context)))
                {
                    break;
                }
            }
            flushRequests();
        }
    } // namespace FSM
} // namespace HJUIK