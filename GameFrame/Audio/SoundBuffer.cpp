#include "SoundBuffer.hpp"

#include "Utilize.hpp"

namespace HJUIK
{
	namespace Audio
	{
        // NOLINTNEXTLINE(*-easily-swappable-parameters)
		auto SoundBuffer::uploadData(const void* data, BufferFormat format, int sizeInBytes, int sampleRate) -> void
		{
			auto alID	  = static_cast<ALuint>(get());
			auto alFormat = static_cast<ALenum>(format);
			// `size` is the size of the vector in byte (aka 8 bit interger)
			auto alSize = static_cast<ALsizei>(sizeInBytes);
			auto alFreq = static_cast<ALsizei>(sampleRate);

			alCheck(alBufferData(alID, alFormat, data, alSize, alFreq));
		}

		auto SoundBuffer::isValid() const -> bool
		{
			return alGet<ALint>(alGetBufferi, get(), AL_SIZE) > 0;
		}
	} // namespace Audio
} // namespace HJUIK
