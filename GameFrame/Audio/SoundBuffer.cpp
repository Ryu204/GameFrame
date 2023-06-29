#include "SoundBuffer.hpp"
#include "Error.hpp"

namespace HJUIK
{
    namespace Audio
    {
        SoundBuffer::SoundBuffer()
        {
            alCheck(alGenBuffers(1, &mID));
        }

        SoundBuffer::~SoundBuffer()
        {
            alCheck(alDeleteBuffers(1, &mID));
        }

        auto SoundBuffer::getInfo() const -> Info
        {
            return mInfo;
        }

        auto SoundBuffer::getRawData() const -> const void*
        {
            if (mInfo.Format == BufferFormat::MONO8 || mInfo.Format == BufferFormat::STEREO8)
            {
                return static_cast<const void*>(mData.V8Bit.data());
            }
            return static_cast<const void*>(mData.V16Bit.data());
        }

        auto SoundBuffer::getVectorData() -> Data&
        {
            return mData;
        }

        auto SoundBuffer::bufferData(ILoader& loader) -> void
        {
            loader.buffer(*this);
        }
    } // namespace Audio
} // namespace HJUIK