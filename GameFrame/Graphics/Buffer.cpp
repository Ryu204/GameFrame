#include "Buffer.hpp"

#include <stdexcept>

namespace HJUIK
{
	namespace Graphics
	{
		auto detail::BufferTrait::create() -> GLuint
		{
			return callGLGen<GLuint>(glGenBuffers);
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
				return GL_ATOMIC_COUNTER_BUFFER;
			case BufferTarget::COPY_READ:
				return GL_COPY_READ_BUFFER;
			case BufferTarget::COPY_WRITE:
				return GL_COPY_WRITE_BUFFER;
			case BufferTarget::DISPATCH_INDIRECT:
				return GL_DISPATCH_INDIRECT_BUFFER;
			case BufferTarget::DRAW_INDIRECT:
				return GL_DRAW_INDIRECT_BUFFER;
			case BufferTarget::ELEMENT_ARRAY:
				return GL_ELEMENT_ARRAY_BUFFER;
			case BufferTarget::PIXEL_PACK:
				return GL_PIXEL_PACK_BUFFER;
			case BufferTarget::PIXEL_UNPACK:
				return GL_PIXEL_UNPACK_BUFFER;
			case BufferTarget::QUERY:
				return GL_QUERY_BUFFER;
			case BufferTarget::SHADER_STORAGE:
				return GL_SHADER_STORAGE_BUFFER;
			case BufferTarget::TEXTURE:
				return GL_TEXTURE_BUFFER;
			case BufferTarget::TRANSFORM_FEEDBACK:
				return GL_TRANSFORM_FEEDBACK_BUFFER;
			case BufferTarget::UNIFORM:
				return GL_UNIFORM_BUFFER;
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
				const BoundBuffer guard = this->bind(TEMP_BUFFER_TARGET);
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
			return getSize(getTarget());
		}

		auto BoundBuffer::allocate(const BufferUsage& usage, std::size_t size, const void* initialData) const -> void
		{
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
			if (offset == 0 && size == SIZE_MAX && !access.isAdvancedAccess()) {
				pointer = glMapBuffer(getTargetEnum(), access.asEnum());
			} else {
				const auto [glOffset, glSize] = checkRange(offset, size);
				pointer = glMapBufferRange(getTargetEnum(), glOffset, glSize, access.asBitfield());
			}

			if (pointer == nullptr) {
				throw std::runtime_error("unable to map buffer");
			}

			return pointer;
		}
		auto BoundBuffer::unmap() const -> bool
		{
			return glUnmapBuffer(getTargetEnum()) != GL_FALSE;
		}
		auto BoundBuffer::flushMappedRange(std::size_t offset, std::size_t size) const -> void
		{
			const auto [glOffset, glSize] = checkRange(offset, size);
			glFlushMappedBufferRange(getTargetEnum(), glOffset, glSize);
		}

		auto BoundBuffer::memCopy(std::size_t destOffset, const void* src, std::size_t size) const -> void
		{
			const auto [glOffset, glSize] = checkRange(destOffset, size);
			glBufferSubData(getTargetEnum(), glOffset, glSize, src);
		}

		auto BoundBuffer::memClear(std::size_t offset, std::size_t size) const -> void
		{
			if (offset == 0 && size == SIZE_MAX) {
				glClearBufferData(getTargetEnum(), GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			}

			const auto [glOffset, glSize] = checkRange(offset, size);
			glClearBufferSubData(getTargetEnum(), glOffset, glSize, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}

		auto BoundBuffer::memCopyFromBuffer(
			BufferTarget srcTarget, std::size_t destOffset, std::size_t srcOffset, std::size_t size) const -> void
		{
			const auto [glSrcOffset, glSrcSize]	  = checkRange(srcOffset, size, srcTarget);
			const auto [glDestOffset, glDestSize] = checkRange(destOffset, size);
			if (glSrcSize != glDestSize) {
				throw std::runtime_error("buffer size mismatch");
			}

			glCopyBufferSubData(static_cast<GLenum>(srcTarget), getTargetEnum(), glSrcOffset, glDestOffset, glDestSize);
		}

		auto BoundBuffer::getSize(BufferTarget target) -> std::size_t
		{
			return static_cast<std::size_t>(
				callGLGet<GLint64>(glGetBufferParameteri64v, static_cast<GLenum>(target), GL_BUFFER_SIZE));
		}

		// NOLINTNEXTLINE(*-member-functions-to-static)
		auto BoundBuffer::checkRange(std::size_t offset, std::size_t size, std::optional<BufferTarget> target) const
			-> std::tuple<GLintptr, GLsizeiptr>
		{
			const auto bufTarget  = target.value_or(getTarget());
			size				  = std::min(size, getSize(bufTarget));
			const auto bufferSize = getSize(bufTarget);
			if (offset >= bufferSize) {
				throw std::runtime_error("offset out of bounds");
			}
			return {static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(std::min(bufferSize - offset, size))};
		}
	} // namespace Graphics
} // namespace HJUIK
