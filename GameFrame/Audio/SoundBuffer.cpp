#include "SoundBuffer.hpp"
#include "Utilize.hpp"

namespace HJUIK
{
    namespace Audio
    {
        auto SoundBuffer::uploadData(const void* data, BufferFormat format, int sizeInBytes, int sampleRate) -> void
        {
            auto alID = static_cast<ALuint>(get());
            auto alFormat = static_cast<ALenum>(format);
            // `size` is the size of the vector in byte (aka 8 bit interger) 
            auto alSize = static_cast<ALsizei>(sizeInBytes);
            auto alFreq = static_cast<ALsizei>(sampleRate);

            alCheck(alBufferData(alID, alFormat, data, alSize, alFreq));
            mIsValid = true;
            mFormat = format;
        }

        auto SoundBuffer::is8Bit() const -> bool
        {
            HJUIK_ASSERT(isValid(), "Buffer is not valid yet");
            return mFormat == BufferFormat::MONO8 || mFormat == BufferFormat::STEREO8;
        }

        auto SoundBuffer::isMono() const -> bool
        {
            HJUIK_ASSERT(isValid(), "Buffer is not valid yet");
            return mFormat == BufferFormat::MONO8 || mFormat == BufferFormat::MONO16;
        }

        auto SoundBuffer::isValid() const -> bool
        {
            return mIsValid;
        }
    } // namespace Audio
} // namespace HJUIK