// Currently there are no support for loading an audio file periodically 

// #ifndef GAMEFRAME_AUDIO_SOUND_STREAM_HPP
// #define GAMEFRAME_AUDIO_SOUND_STREAM_HPP

// /*
//     Represent a streamed source (usually containing a large amount of data played by portions)
// */

// #include <cstdint>
// #include <array>
// #include <memory>

// #include "Utilize.hpp"
// #include "OpenALWrapper.hpp"
// #include "SoundSource.hpp"

// namespace HJUIK
// {
//     namespace Audio
//     {
//         class SoundStream : public SoundSource
//         {
//         public:
//             static constexpr std::size_t NUM_BUFFERS = 4;
//             static constexpr std::size_t SAMPLES_PER_BUFFER = 65536;

//             SoundStream()
//             {
//                 for (std::size_t i = 0; i < NUM_BUFFERS; ++i)
//                 {
//                     mBuffers[i] = std::make_unique<SoundBuffer>();
//                 }
//             }

//         private:
//             std::array<std::unique_ptr<SoundBuffer>, NUM_BUFFERS> mBuffers;
//         };
//     } // namespace Audio
// } // namespace HJUIK

// #endif