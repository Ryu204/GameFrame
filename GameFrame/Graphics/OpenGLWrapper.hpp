#ifndef GAMEFRAME_GRAPHICS_OPENGL_WRAPPER_HPP
#define GAMEFRAME_GRAPHICS_OPENGL_WRAPPER_HPP

#include <functional>
#include <set>
#include <stdexcept>
#include <utility>

namespace HJUIK
{
	// API based on std::unique_ptr
	template <typename WrapperTrait, typename PossiblyBoundObjectType = void>
	class OpenGLWrapper
	{
	public:
		using Handle						  = typename WrapperTrait::HandleType;
		static constexpr Handle NULL_HANDLE	  = static_cast<Handle>(0);
		static constexpr bool SUPPORT_BINDING = !std::is_same_v<PossiblyBoundObjectType, void>;

		// Create an OpenGL object
		OpenGLWrapper() : mHandle{WrapperTrait::create()} {}

		//
		explicit OpenGLWrapper(Handle handle) : mHandle{handle} {}
		explicit OpenGLWrapper(std::nullptr_t) : mHandle{NULL_HANDLE} {}

		OpenGLWrapper(OpenGLWrapper&& other) noexcept : mHandle(other.release()) {}
		OpenGLWrapper(const OpenGLWrapper&) = delete;

		auto operator=(OpenGLWrapper&& other) noexcept -> OpenGLWrapper&
		{
			OpenGLWrapper(std::move(other)).swap(*this);
			return *this;
		}
		auto operator=(const OpenGLWrapper&) -> OpenGLWrapper& = delete;

		virtual ~OpenGLWrapper()
		{
			reset();
		}

		auto reset() -> void
		{
			if (mHandle != NULL_HANDLE) {
				WrapperTrait::destroy(std::exchange(mHandle, NULL_HANDLE));
			}
		}

		auto swap(OpenGLWrapper& other) -> void
		{
			using std::swap;
			swap(mHandle, other.mHandle);
		}

		auto release() -> Handle
		{
			return std::exchange(mHandle, NULL_HANDLE);
		}

		auto get() const -> Handle
		{
			return mHandle;
		}

		explicit operator bool() const
		{
			return mHandle != NULL_HANDLE;
		}

		template <typename... BindArgs, bool Cond = SUPPORT_BINDING, typename = std::enable_if_t<Cond>>
		auto bind(BindArgs... args) const -> PossiblyBoundObjectType
		{
			static_assert(Cond == SUPPORT_BINDING);
			const auto maybeBound = maybeBind();
			maybeBound.forceBind();
			return std::move(maybeBound);
		}

		template <typename... BindArgs, bool Cond = SUPPORT_BINDING, typename = std::enable_if_t<Cond>>
		auto maybeBind(BindArgs... args) const -> PossiblyBoundObjectType
		{
			static_assert(Cond == SUPPORT_BINDING);
			return PossiblyBoundObjectType{get(), args...};
		}

		template <typename... BindArgs, bool Cond = SUPPORT_BINDING, typename = std::enable_if_t<Cond>>
		static auto getCurrentBound() -> GLuint
		{
			static_assert(Cond == SUPPORT_BINDING);
			return WrapperTrait::getCurrentBound();
		}

	private:
		Handle mHandle;
	};

	// reduce boilerplates in glGen*/glGet* calls
	template <typename ReturnType, typename Func, typename... Args>
	inline auto callGLGet(Func&& glGetFunc, Args&&... args) -> ReturnType
	{
		ReturnType value;
		std::forward<Func>(glGetFunc)(std::forward<Args>(args)..., &value);
		return value;
	}

	// reduce boilerplates in glGen* calls
	template <typename ReturnType, typename Func, typename... Args>
	inline auto callGLGen(Func&& glGenFunc, Args&&... args) -> ReturnType
	{
		return callGLGet<ReturnType>(std::forward<Func>(glGenFunc), std::forward<Args>(args)..., 1);
	}

	template <typename WrapperTrait, typename... Args>
	class PossiblyBoundOpenGLWrapper
	{
	public:
		using HandleType = typename WrapperTrait::HandleType;

		explicit PossiblyBoundOpenGLWrapper(HandleType handle, Args... args)
			: mHandle{handle}, mArgs{args...}, mCurrentBound{}
		{
		}

		PossiblyBoundOpenGLWrapper(const PossiblyBoundOpenGLWrapper&)					   = delete;
		auto operator=(const PossiblyBoundOpenGLWrapper&) -> PossiblyBoundOpenGLWrapper& = delete;

		PossiblyBoundOpenGLWrapper(PossiblyBoundOpenGLWrapper&&) noexcept					   = default;
		auto operator=(PossiblyBoundOpenGLWrapper&&) noexcept -> PossiblyBoundOpenGLWrapper& = default;

		~PossiblyBoundOpenGLWrapper()
		{
			if (mBound) {
				forceUnbind();
			}
		}

		auto getArgs() const -> const std::tuple<Args...>&
		{
			return mArgs;
		}

		auto getHandle() const -> HandleType
		{
			return mHandle;
		}

		auto forceBind() const -> void
		{
			if (mBound) {
				return;
			}

			mCurrentBound = std::apply(WrapperTrait::getCurrentBound, mArgs);

			if (mAlreadyBoundTargets().find(mArgs) != mAlreadyBoundTargets().end()) {
				throw std::runtime_error("another bind guard already in effect");
			}

			mAlreadyBoundTargets().insert(mArgs);
			std::apply([&](auto... args) { WrapperTrait::bind(mHandle, args...); }, mArgs);
			mBound = true;
		}

		auto forceUnbind() const -> void
		{
			if (!mBound) {
				return;
			}

			mAlreadyBoundTargets().erase(mArgs);
			std::apply([&](auto... args) { WrapperTrait::bind(mCurrentBound, args...); }, mArgs);
			mBound = false;
		}

		auto isBound() const -> bool
		{
			return mBound;
		}

	private:
		HandleType mHandle;
		std::tuple<Args...> mArgs;
		mutable HandleType mCurrentBound{static_cast<HandleType>(0)};
		mutable bool mBound{false};

		static auto mAlreadyBoundTargets() -> std::set<std::tuple<Args...>>&
		{
			static std::set<std::tuple<Args...>> alreadyBoundTargets{};
			return alreadyBoundTargets;
		}
	};

	inline auto supportsDSA() -> bool
	{
		return GLAD_GL_VERSION_4_5 != 0;
	}
} // namespace HJUIK

#endif
