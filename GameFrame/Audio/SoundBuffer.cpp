#include "SoundBuffer.hpp"
#include "Utilize.hpp"

namespace HJUIK
{
    namespace Audio
    {
        auto SoundBuffer::getData() -> Data&
        {
            return mData;
        }

        auto SoundBuffer::bufferData(ILoader& loader) -> void
        {
            // If the loader fails to buffer this instance
            if (!loader.buffer(*this))
            {
                mIsValid = false;
                return;
            }
            // Conversion uint <-> int
            auto identifier = static_cast<ALuint>(get());
            auto format = static_cast<ALenum>(mData.Format);
            const auto* data = is8Bit() ? static_cast<const ALvoid*>(mData.V8Bit.data()) : static_cast<const ALvoid*>(mData.V16Bit.data());
            // `size` is the size of the vector in byte (aka 8 bit interger) 
            auto size = static_cast<ALsizei>(is8Bit() ? mData.V8Bit.size() : 2 * mData.V16Bit.size());
            auto freq = static_cast<ALsizei>(mData.SamplePerSecond);

            alCheck(alBufferData(identifier, format, data, size, freq));
            mIsValid = true;
            // Free the memory
            mData = {};
        }

        auto SoundBuffer::is8Bit() const -> bool
        {
            return mData.Format == BufferFormat::MONO8 || mData.Format == BufferFormat::STEREO8;
        }

        auto SoundBuffer::isValid() const -> bool
        {
            return mIsValid;
        }
    } // namespace Audio
} // namespace HJUIK