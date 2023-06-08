#ifndef GAMEFRAME_GRAPHICS_BUFFER_HPP
#define GAMEFRAME_GRAPHICS_BUFFER_HPP

#include "glad/glad.h"
#include <algorithm>
#include <tuple>
#include <type_traits>
#include <utility>

#include "OpenGLWrapper.hpp"

namespace HJUIK
{
  namespace Graphics
  {
    namespace detail
    {
      struct BufferTrait {
        using HandleType = GLuint;
        static auto create() -> GLuint;
        static auto destroy(GLuint handle) -> void;
      };
    } // namespace detail

    enum class BufferTarget {
      ARRAY              = GL_ARRAY_BUFFER,
      ATOMIC_COUNTER     = GL_ATOMIC_COUNTER_BUFFER,
      COPY_READ          = GL_COPY_READ_BUFFER,
      COPY_WRITE         = GL_COPY_WRITE_BUFFER,
      DISPATCH_INDIRECT  = GL_DISPATCH_INDIRECT_BUFFER,
      DRAW_INDIRECT      = GL_DRAW_INDIRECT_BUFFER,
      ELEMENT_ARRAY      = GL_ELEMENT_ARRAY_BUFFER,
      PIXEL_PACK         = GL_PIXEL_PACK_BUFFER,
      PIXEL_UNPACK       = GL_PIXEL_UNPACK_BUFFER,
      QUERY              = GL_QUERY_BUFFER,
      SHADER_STORAGE     = GL_SHADER_STORAGE_BUFFER,
      TEXTURE            = GL_TEXTURE_BUFFER,
      TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
      UNIFORM            = GL_UNIFORM_BUFFER,
    };

    auto getBufferBindingTarget(BufferTarget target) -> GLenum;

    // these values are picked in a way that `BufferAccess` and
    // `BufferAccessFrequency` add up to the glBufferData's usage arg
    // e.g. STREAM + READ = GL_STREAM_DRAW + 1 = GL_STREAM_READ
    // (not sure if this is standard behavior though)
    enum class BufferAccess {
      DRAW = 0,
      READ = 1,
      COPY = 2,
    };

    enum class BufferAccessFrequency {
      STREAM  = GL_STREAM_DRAW,
      STATIC  = GL_STATIC_DRAW,
      DYNAMIC = GL_DYNAMIC_DRAW,
    };

    enum class BufferMapPermission : std::make_unsigned_t<GLenum> {
      NONE       = 0,
      READ       = GL_MAP_READ_BIT,
      WRITE      = GL_MAP_WRITE_BIT,
      READ_WRITE = READ | WRITE,
    };

    enum class BufferMapPersistentMode {
      NONE       = 0,
      PERSISTENT = GL_MAP_PERSISTENT_BIT,
      COHERENT   = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT,
    };

    enum class BufferMapInvalidateMode {
      NONE   = 0,
      RANGE  = GL_MAP_INVALIDATE_RANGE_BIT,
      BUFFER = GL_MAP_INVALIDATE_BUFFER_BIT,
    };

    struct BufferUsage {
      // if `Immutable` is true and the OpenGL version >= 4.1,
      // buffer will be allocated via the newer glBufferStorage API
      // otherwise, it will fallback to older glBufferData API
      // Note: the newer API does not support reallocation, this is
      // a case when one could consider setting this to false
      // btw `Immutable` means that you can't reallocate the buffer,
      // not the buffer content being unmodifiable
      bool Immutable = true;
      // these two properties are only used if buffer was allocated via glBufferData
      // these are not that important, since the driver can ignore them completely
      // see here: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml
      // basically in 99% of the cases, Access is DRAW, and
      // if the buffer data will be modified, Frequency is DYNAMIC,
      // otherwise, frequency can be DYNAMIC
      BufferAccess Access             = BufferAccess::DRAW;
      BufferAccessFrequency Frequency = BufferAccessFrequency::STATIC;
      // the remaining properties are only used if buffer was allocated via glBufferStorage
      // these are important, specifying wrong values can cause some issues
      // see here: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferStorage.xhtml

      // access permissions if one wants to use the `Buffer` mapping API
      // (map/unmap methods)
      BufferMapPermission Permission = BufferMapPermission::NONE;
      // true if one wants to modify the buffer using the `memCopy/memSet` methods
      // (which uses glBufferSubData and glClearBuffer*Data under the hood)
      bool DynamicStorage = false;
      // persistent mapping, these are relatively complex, so read the docs:
      // https://www.khronos.org/opengl/wiki/Buffer_Object#Persistent_mapping
      BufferMapPersistentMode PersistentMapping = BufferMapPersistentMode::NONE;
      // hint to the driver that the storage for the buffer should come from "client"
      // memory (i.e. on the CPU)
      bool ClientStorage = false;

      auto asBufferStorageFlags() const -> GLbitfield;
      auto asBufferDataUsage() const -> GLenum;
    };

    // see docs: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glMapBufferRange.xhtml
    struct BufferMapAccess {
      BufferMapPermission Permission         = BufferMapPermission::READ_WRITE;
      BufferMapPersistentMode PersistentMode = BufferMapPersistentMode::NONE;
      BufferMapInvalidateMode InvalidateMode = BufferMapInvalidateMode::NONE;
      bool ExplicitFlush                     = false;
      bool Unsynchronized                    = false;

      // return true if this uses features like persistent mapping, explicit flush, etc.,
      // which could not be done with plain old glMapBuffer
      auto isAdvancedAccess() const -> bool;
      auto asEnum() const -> GLenum;
      auto asBitfield() const -> GLbitfield;
    };

    class Buffer : public OpenGLWrapper<detail::BufferTrait>
    {
    public:
      using OpenGLWrapper::OpenGLWrapper;
      using OpenGLWrapper::operator=;
      constexpr static BufferTarget TEMP_BUFFER_TARGET = BufferTarget::COPY_READ;

      auto bind(BufferTarget target) const -> void;
      static auto unbind(BufferTarget target) -> void;

      auto getSize() const -> std::size_t;

      // wrapper for glBindBufferBase/glBindBufferRange
      // NOLINTNEXTLINE(*-easily-swappable-parameters)
      auto bindBase(BufferTarget target, std::size_t index, std::size_t offset = 0, std::size_t size = SIZE_MAX) const
          -> void;
      static auto unbindBase(BufferTarget target, std::size_t index) -> void;

      // get handle to currently bound Buffer
      // this can be used to restore binding state after
      // temporarily binding a Buffer
      static auto getCurrentBound(BufferTarget target) -> GLuint;

      // set a label for this Buffer via `glObjectLabel`.
      // this label may show up in debug callback or an external OpenGL
      // debugger (e.g. RenderDoc)
      // (only have effect in OpenGL 4.3+)
      auto setLabel(const char* name) const -> void;

      // allocate memory on the GPU
      // set the `initialData` arg to nullptr to skip uploading initial data
      // set the `immutable` arg to false to force the allocation to use glBufferData API instead of
      // glBufferStorage. Even if `immutable` is true, if glBufferStorage is not supported (OpenGL
      // version <= 4.1), it will fallback to glBufferData
      static auto allocate(
          BufferTarget target, const BufferUsage& usage, std::size_t size, const void* initialData = nullptr) -> void;

      // depending on what you specified `usage` when allocating the buffer,
      // these APIs may not be available
      static auto map(BufferTarget target, const BufferMapAccess& access, std::size_t offset = 0,
          std::size_t size = SIZE_MAX) -> void*;
      // the return value of `unmap` is false if some sort of buffer corruption happened
      // see here: https://www.khronos.org/opengl/wiki/Buffer_Object#Buffer_Corruption
      static auto unmap(BufferTarget target) -> bool;
      static auto flushMappedRange(BufferTarget target, std::size_t offset, std::size_t size) -> void;
      auto invalidate(std::size_t offset, std::size_t size) const -> void;
      static auto memCopy(BufferTarget target, std::size_t destOffset, const void* src, std::size_t size) -> void;
      // TODO: add support for custom clearing
      static auto memClear(BufferTarget target, std::size_t offset = 0, std::size_t size = SIZE_MAX) -> void;
      static auto memCopyFromBuffer(BufferTarget destTarget, BufferTarget srcTarget, std::size_t destOffset,
          std::size_t srcOffset, std::size_t size) -> void;

      template <typename Container>
      static auto allocate(BufferTarget target, const BufferUsage& usage, const Container& container) -> void
      {
        allocate(target, usage, container.size() * sizeof(*container.data()), container.data());
      }

      template <typename Container>
      static auto memCopy(BufferTarget target, std::size_t destOffset, const Container& container) -> void
      {
        memCopy(target, destOffset, container.data(), container.size() * sizeof(*container.data()));
      }

    private:
      static auto getSize(BufferTarget target) -> std::size_t;
      static auto checkRange(std::size_t offset, std::size_t size, BufferTarget target) -> std::tuple<GLintptr, GLsizeiptr>;
      auto checkRange(std::size_t offset, std::size_t size) const -> std::tuple<GLintptr, GLsizeiptr>;
    };

  } // namespace Graphics

} // namespace HJUIK


#endif
