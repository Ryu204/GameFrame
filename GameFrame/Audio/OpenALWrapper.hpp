#ifndef GAMEFRAME_AUDIO_OPENAL_WRAPPER_HPP
#define GAMEFRAME_AUDIO_OPENAL_WRAPPER_HPP

/*
	Base class for objects created within a context
*/

#include <optional>
#include <utility>

#include "ContextBasedWrapper.hpp"

namespace HJUIK
{
	namespace Audio
	{
		template <typename WrapperTrait>
		class OpenALWrapper : public ContextBasedWrapper
		{
		public:
			using Handle = typename WrapperTrait::HandleType;

			// Create an OpenAL object
			OpenALWrapper() : mHandle{WrapperTrait::create()} {}

			explicit OpenALWrapper(Handle handle) : mHandle{handle} {}
			explicit OpenALWrapper(std::nullptr_t) : mHandle{std::nullopt} {}

			OpenALWrapper(OpenALWrapper&& other) noexcept : mHandle{other.release()} {}
			OpenALWrapper(const OpenALWrapper&) = delete;

			auto operator=(OpenALWrapper&& other) noexcept -> OpenALWrapper&
			{
				OpenALWrapper(std::move(other)).swap(*this);
				return *this;
			}
			auto operator=(const OpenALWrapper&) -> OpenALWrapper& = delete;

			~OpenALWrapper() override
			{
				reset();
			}

			auto swap(OpenALWrapper& other) -> void
			{
				using std::swap;
				swap(mHandle, other.mHandle);
			}

			auto reset() -> void
			{
				if (mHandle.has_value()) {
					WrapperTrait::destroy(mHandle.value());
					mHandle.reset();
				}
			}

			auto release() -> std::optional<Handle>
			{
				return std::exchange(mHandle, std::nullopt);
			}

			auto get() const -> Handle
			{
                // NOLINTNEXTLINE(*-unchecked-optional-access)
				return mHandle.value();
			}

            auto isValid() const -> bool {
                return mHandle.has_value();
            }

		private:
			std::optional<Handle> mHandle;
		};
	} // namespace Audio
} // namespace HJUIK

#endif
