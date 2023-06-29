#include "SoundBuffer.hpp"
#include "Error.hpp"

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
            loader.buffer(*this);
            // Conversion uint <-> int
            auto identifier = static_cast<ALuint>(get());
            auto format = static_cast<ALenum>(mData.Format);
            const auto* data = is8Bit() ? static_cast<const ALvoid*>(mData.V8Bit.data()) : static_cast<const ALvoid*>(mData.V16Bit.data());
            auto size = static_cast<ALsizei>(sizeof(data));
            auto freq = static_cast<ALsizei>(mData.SamplePerSecond);

            alCheck(alBufferData(identifier, format, data, size, freq));
        }

        auto SoundBuffer::is8Bit() const -> bool
        {
            return mData.Format == BufferFormat::MONO8 || mData.Format == BufferFormat::STEREO8;
        }
    } // namespace Audio
} // namespace HJUIK