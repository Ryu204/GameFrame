#ifndef GAMEFRAME_AUDIO_OPENAL_WRAPPER_HPP
#define GAMEFRAME_AUDIO_OPENAL_WRAPPER_HPP

/*
	Base class for objects created within a context
*/

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
			using Handle						= typename WrapperTrait::HandleType;
			static constexpr Handle NULL_HANDLE = static_cast<Handle>(0);

			// Create an OpenAL object
			OpenALWrapper() : mHandle{WrapperTrait::create()} {}

			explicit OpenALWrapper(Handle handle) : mHandle{handle} {}
			explicit OpenALWrapper(std::nullptr_t) : mHandle{NULL_HANDLE} {}

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
				if (mHandle != NULL_HANDLE) {
					WrapperTrait::destroy(mHandle);
					mHandle = NULL_HANDLE;
				}
			}

			auto release() -> Handle
			{
				return std::exchange(mHandle, NULL_HANDLE);
			}

			auto get() const -> Handle
			{
				return mHandle;
			}

		private:
			Handle mHandle;
		};
	} // namespace Audio
} // namespace HJUIK

#endif
