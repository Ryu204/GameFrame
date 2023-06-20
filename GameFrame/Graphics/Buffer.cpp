#include "Buffer.hpp"

#include <optional>
#include <stdexcept>

#include "../Utilize/CallAssert.hpp"

namespace HJUIK
{
	namespace Graphics
	{
		auto detail::BufferTrait::create() -> GLuint
		{
			return Utilize::throwIfZero(
				supportsDSA() ? callGLGen<GLuint>(glCreateBuffers) : callGLGen<GLuint>(glGenBuffers),
				"unable to create buffer");
		}

		auto detail::BufferTrait::destroy(GLuint handle) -> void
		{
			glDeleteBuffers(1, &handle);
		}

		auto detail::BufferTrait::bind(GLuint handle, BufferTarget target) -> void
		{
			glBindBuffer(static_cast<GLenum>(target), handle);
		}

		auto detail::BufferTrait::getCurrentBound(BufferTarget target) -> GLuint
		{
			return static_cast<GLuint>(callGLGet<GLint>(glGetIntegerv, getBufferBindingTarget(target)));
		}

		auto getBufferBindingTarget(BufferTarget target) -> GLenum
		{
			switch (target) {
			case BufferTarget::ARRAY:
				return GL_ARRAY_BUFFER_BINDING;
			case BufferTarget::ATOMIC_COUNTER:
				return GL_ATOMIC_COUNTER_BUFFER_BINDING;
			case BufferTarget::COPY_READ:
				return GL_COPY_READ_BUFFER_BINDING;
			case BufferTarget::COPY_WRITE:
				return GL_COPY_WRITE_BUFFER_BINDING;
			case BufferTarget::DISPATCH_INDIRECT:
				return GL_DISPATCH_INDIRECT_BUFFER_BINDING;
			case BufferTarget::DRAW_INDIRECT:
				return GL_DRAW_INDIRECT_BUFFER_BINDING;
			case BufferTarget::ELEMENT_ARRAY:
				return GL_ELEMENT_ARRAY_BUFFER_BINDING;
			case BufferTarget::PIXEL_PACK:
				return GL_PIXEL_PACK_BUFFER_BINDING;
			case BufferTarget::PIXEL_UNPACK:
				return GL_PIXEL_UNPACK_BUFFER_BINDING;
			case BufferTarget::QUERY:
				return GL_QUERY_BUFFER_BINDING;
			case BufferTarget::SHADER_STORAGE:
				return GL_SHADER_STORAGE_BUFFER_BINDING;
			case BufferTarget::TEXTURE:
				return GL_TEXTURE_BUFFER_BINDING;
			case BufferTarget::TRANSFORM_FEEDBACK:
				return GL_TRANSFORM_FEEDBACK_BUFFER_BINDING;
			case BufferTarget::UNIFORM:
				return GL_UNIFORM_BUFFER_BINDING;
			default:
				throw std::runtime_error("invalid buffer type");
			}
		}
		auto BufferUsage::asBufferStorageFlags() const -> GLbitfield
		{
			auto flags = static_cast<GLbitfield>(Permission) | static_cast<GLbitfield>(PersistentMapping);
			if (DynamicStorage) {
				flags |= static_cast<GLbitfield>(GL_DYNAMIC_STORAGE_BIT);
			}
			if (ClientStorage) {
				flags |= static_cast<GLbitfield>(GL_CLIENT_STORAGE_BIT);
			}
			return flags;
		}
		auto BufferUsage::asBufferDataUsage() const -> GLenum
		{
			return static_cast<GLenum>(Access) + static_cast<GLenum>(Frequency);
		}
		auto BufferMapAccess::isAdvancedAccess() const -> bool
		{
			return ExplicitFlush || Unsynchronized || PersistentMode != BufferMapPersistentMode::NONE
				|| InvalidateMode != BufferMapInvalidateMode::NONE;
		}
		auto BufferMapAccess::asEnum() const -> GLenum
		{
			switch (Permission) {
			case BufferMapPermission::READ:
				return GL_READ_ONLY;
			case BufferMapPermission::WRITE:
				return GL_WRITE_ONLY;
			case BufferMapPermission::READ_WRITE:
				return GL_READ_WRITE;
			default:
				throw std::runtime_error("invalid map permission");
			}
		}
		auto BufferMapAccess::asBitfield() const -> GLbitfield
		{
			auto flags = static_cast<GLbitfield>(Permission) | static_cast<GLbitfield>(PersistentMode)
					   | static_cast<GLbitfield>(InvalidateMode);
			if (ExplicitFlush) {
				flags |= static_cast<GLbitfield>(GL_MAP_FLUSH_EXPLICIT_BIT);
			}
			if (Unsynchronized) {
				flags |= static_cast<GLbitfield>(GL_MAP_UNSYNCHRONIZED_BIT);
			}
			return flags;
		}
		// NOLINTNEXTLINE(*-easily-swappable-parameters)a
		auto Buffer::bindBase(BufferTarget target, std::size_t index, std::size_t offset, std::size_t size) const
			-> void
		{
			if (offset == 0 && size == SIZE_MAX) {
				glBindBufferBase(static_cast<GLenum>(target), index, get());
				return;
			}

			const BoundBuffer bound{get(), TEMP_BUFFER_TARGET};
			const auto [glOffset, glSize] = bound.checkRange(offset, size);
			glBindBufferRange(static_cast<GLenum>(target), index, get(), glOffset, glSize);
		}
		auto Buffer::unbindBase(BufferTarget target, std::size_t index) -> void
		{
			glBindBufferBase(static_cast<GLenum>(target), index, 0);
		}
		auto Buffer::setLabel(const char* name) const -> void
		{
			if (GLAD_GL_VERSION_4_3 != 0) {
				// only binds if this is not created with glCreate*
				// i.e. DSA is not supported
				std::optional<BoundBuffer> guard;
				if (supportsDSA()) {
					guard = bind(TEMP_BUFFER_TARGET);
					guard.value().forceBind();
				}
				glObjectLabel(GL_BUFFER, get(), -1, name);
			}
		}
		auto Buffer::invalidate(std::size_t offset, std::size_t size) const -> void
		{
			if (offset == 0 && size == SIZE_MAX) {
				glInvalidateBufferData(get());
			}

			const BoundBuffer bound		  = this->bind(TEMP_BUFFER_TARGET);
			const auto [glOffset, glSize] = bound.checkRange(offset, size);
			glInvalidateBufferSubData(get(), glOffset, glSize);
		}

		auto BoundBuffer::getTarget() const -> BufferTarget
		{
			return std::get<0>(getArgs());
		}

		auto BoundBuffer::getTargetEnum() const -> GLenum
		{
			return static_cast<GLenum>(getTarget());
		}

		auto BoundBuffer::getSize() const -> std::size_t
		{
			if (supportsDSA()) {
				return static_cast<std::size_t>(
					callGLGet<GLint64>(glGetNamedBufferParameteri64v, getHandle(), GL_BUFFER_SIZE));
			}

			forceBind();
			return static_cast<std::size_t>(
				callGLGet<GLint64>(glGetBufferParameteri64v, getTargetEnum(), GL_BUFFER_SIZE));
		}

		auto BoundBuffer::allocate(const BufferUsage& usage, std::size_t size, const void* initialData) const -> void
		{
			if (supportsDSA()) {
				if (usage.Immutable && GLAD_GL_VERSION_4_4 != 0) {
					glNamedBufferStorage(
						getHandle(), static_cast<GLsizeiptr>(size), initialData, usage.asBufferStorageFlags());
				} else {
					glNamedBufferData(
						getHandle(), static_cast<GLsizeiptr>(size), initialData, usage.asBufferDataUsage());
				}

				return;
			}

			forceBind();
			if (usage.Immutable && GLAD_GL_VERSION_4_4 != 0) {
				glBufferStorage(
					getTargetEnum(), static_cast<GLsizeiptr>(size), initialData, usage.asBufferStorageFlags());
			} else {
				glBufferData(getTargetEnum(), static_cast<GLsizeiptr>(size), initialData, usage.asBufferDataUsage());
			}
		}
		auto BoundBuffer::map(const BufferMapAccess& access, std::size_t offset, std::size_t size) const -> void*
		{
			void* pointer = nullptr;
			if (supportsDSA()) {
				if (offset == 0 && size == SIZE_MAX && !access.isAdvancedAccess()) {
					pointer = glMapNamedBuffer(getTargetEnum(), access.asEnum());
				} else {
					const auto [glOffset, glSize] = checkRange(offset, size);
					pointer = glMapNamedBufferRange(getTargetEnum(), glOffset, glSize, access.asBitfield());
				}
			} else {
				if (offset == 0 && size == SIZE_MAX && !access.isAdvancedAccess()) {
					pointer = glMapBuffer(getTargetEnum(), access.asEnum());
				} else {
					const auto [glOffset, glSize] = checkRange(offset, size);
					pointer = glMapBufferRange(getTargetEnum(), glOffset, glSize, access.asBitfield());
				}
			}

			if (pointer == nullptr) {
				throw std::runtime_error("unable to map buffer");
			}

			return pointer;
		}
		auto BoundBuffer::unmap() const -> bool
		{
			if (supportsDSA()) {
				return glUnmapNamedBuffer(getHandle()) != GL_FALSE;
			}

			forceBind();
			return glUnmapBuffer(getTargetEnum()) != GL_FALSE;
		}
		auto BoundBuffer::flushMappedRange(std::size_t offset, std::size_t size) const -> void
		{
			const auto [glOffset, glSize] = checkRange(offset, size);
			if (supportsDSA()) {
				glFlushMappedNamedBufferRange(getHandle(), glOffset, glSize);
				return;
			}

			forceBind();
			glFlushMappedBufferRange(getTargetEnum(), glOffset, glSize);
		}

		auto BoundBuffer::memCopy(std::size_t destOffset, const void* src, std::size_t size) const -> void
		{
			const auto [glOffset, glSize] = checkRange(destOffset, size);
			if (supportsDSA()) {
				glNamedBufferSubData(getHandle(), glOffset, glSize, src);
				return;
			}

			forceBind();
			glBufferSubData(getTargetEnum(), glOffset, glSize, src);
		}

		auto BoundBuffer::memClear(std::size_t offset, std::size_t size) const -> void
		{
			if (offset == 0 && size == SIZE_MAX) {
				if (supportsDSA()) {
					glClearNamedBufferData(getHandle(), GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
					return;
				}

				forceBind();
				glClearBufferData(getTargetEnum(), GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				return;
			}

			const auto [glOffset, glSize] = checkRange(offset, size);
			if (supportsDSA()) {
				glClearNamedBufferSubData(getHandle(), glOffset, glSize, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				return;
			}

			forceBind();
			glClearBufferSubData(getTargetEnum(), glOffset, glSize, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}

		auto BoundBuffer::memCopyFromBuffer(
			const BoundBuffer& srcBuffer, std::size_t destOffset, std::size_t srcOffset, std::size_t size) const -> void
		{
			const auto [glSrcOffset, glSrcSize]	  = srcBuffer.checkRange(srcOffset, size);
			const auto [glDestOffset, glDestSize] = checkRange(destOffset, size);
			if (glSrcSize != glDestSize) {
				throw std::runtime_error("buffer size mismatch");
			}

			if (supportsDSA()) {
				glCopyNamedBufferSubData(srcBuffer.getHandle(), getHandle(), glSrcOffset, glDestOffset, glDestSize);
				return;
			}

			HJUIK_ASSERT(srcBuffer.getTarget() != getTarget(), "could not copy between buffers of the same target");
			srcBuffer.forceBind();
			forceBind();
			glCopyBufferSubData(srcBuffer.getTargetEnum(), getTargetEnum(), glSrcOffset, glDestOffset, glDestSize);
		}

		// NOLINTNEXTLINE(*-member-functions-to-static)
		auto BoundBuffer::checkRange(std::size_t offset, std::size_t size) const -> std::tuple<GLintptr, GLsizeiptr>
		{
			const auto bufferSize = getSize();
			size				  = std::min(size, bufferSize);
			if (offset >= bufferSize) {
				throw std::runtime_error("offset out of bounds");
			}
			return {static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(std::min(size - offset, size))};
		}
	} // namespace Graphics
} // namespace HJUIK
