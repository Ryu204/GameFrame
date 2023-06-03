#ifndef GAMEFRAME_GRAPHICS_OPENGL_WRAPPER_HPP
#define GAMEFRAME_GRAPHICS_OPENGL_WRAPPER_HPP

#include <stdexcept>
#include <utility>

namespace HJUIK
{
  // API based on std::unique_ptr
  template <typename WrapperTrait>
  class OpenGLWrapper
  {
  public:
    using Handle = WrapperTrait::HandleType;
    static constexpr Handle NULL_HANDLE = static_cast<Handle>(0);

    // Create an OpenGL object
    OpenGLWrapper() : mHandle{WrapperTrait::create()} {}

    //
    explicit OpenGLWrapper(Handle handle) : mHandle{handle} {}
    explicit OpenGLWrapper(std::nullptr_t) : mHandle{NULL_HANDLE} {}

    OpenGLWrapper(OpenGLWrapper &&other) : mHandle(other.release()) {}
    OpenGLWrapper(const OpenGLWrapper &) = delete;

    auto operator=(OpenGLWrapper &&other) -> OpenGLWrapper &
    {
      OpenGLWrapper(std::move(other)).swap(*this);
      return *this;
    }
    auto operator=(const OpenGLWrapper &) -> OpenGLWrapper & = delete;

    virtual ~OpenGLWrapper() { reset(); }

    auto reset() -> void
    {
      if (mHandle != NULL_HANDLE)
      {
        WrapperTrait::destroy(std::exchange(mHandle, NULL_HANDLE));
      }
    }

    auto swap(OpenGLWrapper &other) -> void
    {
      using std::swap;
      swap(mHandle, other.mHandle);
    }

    auto release() -> Handle { return std::exchange(mHandle, NULL_HANDLE); }

    auto get() const -> Handle { return mHandle; }

    operator bool() const { return mHandle != NULL_HANDLE; }

  private:
    Handle mHandle;
  };
} // namespace HJUIK

// reduce boilerplates in glGen* calls
template <typename ReturnType, typename Func, typename... Args>
inline auto callGLGen(Func &&glGenFunc, Args &&...args) -> ReturnType
{
  ReturnType value;
  std::forward<Func>(glGenFunc)(std::forward<Args>(args)..., 1, value);
  return value;
}

template <typename Type>
inline auto checkNonZero(Type &&value, const char *errorMessage)
    -> decltype(auto)
{
  if (value == 0)
  {
    throw std::runtime_error(errorMessage);
  }

  return std::forward<Type>(value);
}

#endif
