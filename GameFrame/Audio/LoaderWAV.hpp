#ifndef GAMEFRAME_AUDIO_LOADER_WAV_HPP
#define GAMEFRAME_AUDIO_LOADER_WAV_HPP

#include <algorithm>
#include <cstdint>
#include <exception>
#include <iterator>

#include "../Utilize/CallAssert.hpp"
#include "ILoader.hpp"
#include "SoundBuffer.hpp"
#include "deps/AudioFile.hpp"

namespace HJUIK
{
	namespace Audio
	{
		class LoaderWAV : public ILoader
		{
		public:
			// If the load fails, throw an exception
			auto loadFromFile(const char* filename) -> void
			{
				mInternalLoader.shouldLogErrorsToConsole(/*logErrors=*/false);
				if (!mInternalLoader.load(std::string(filename))) {
					throw std::runtime_error("Cannot load WAV file in " + std::string(filename));
				}
			}

			auto buffer(SoundBuffer& target, std::size_t maxSamples) -> std::size_t override
			{
				// Format & Data ==================================================
				int bitdepth = mInternalLoader.getBitDepth();
				// Make sure openAL can handle file format ========================
				// NOLINTBEGIN(*-magic-numbers)
				if (bitdepth != 8 && bitdepth != 16) {
					throw std::runtime_error("WAV file bit depth is not 8 or 16: " + std::to_string(bitdepth));
				}
				int channelNum = mInternalLoader.getNumChannels();
				if (channelNum != 1 && channelNum != 2) {
					throw std::runtime_error("WAV file channel count is not 1 or 2" + std::to_string(channelNum));
				}

				const std::size_t numSamplesPerChannel = std::min<std::size_t>(
					maxSamples / channelNum, mInternalLoader.getNumSamplesPerChannel() - mSampleOffset);

				// ================================================================
				if (mInternalLoader.isMono()) {
					if (bitdepth == 8) {
						mono8DataBuffer(target, numSamplesPerChannel);
					} else {
						mono16DataBuffer(target, numSamplesPerChannel);
					}
				} else {
					if (bitdepth == 8) {
						stereo8DataBuffer(target, maxSamples);
					} else {
						stereo16DataBuffer(target, maxSamples);
					}
				}

				// NOLINTEND(*-magic-numbers)
				// free memory
				mSampleOffset += numSamplesPerChannel;
				mInternalLoader.samples.clear();
				return numSamplesPerChannel * mInternalLoader.getNumChannels();
			}

			auto channels() -> std::size_t override
			{
				return static_cast<std::size_t>(mInternalLoader.getNumChannels());
			}

			auto sampleRate() -> std::size_t override
			{
				return static_cast<std::size_t>(mInternalLoader.getSampleRate());
			}

			auto seek(std::size_t sampleIndex) -> void override
			{
				mSampleOffset = sampleIndex;
			}

		private:
			template <typename VectorType>
			auto upload(SoundBuffer& target, const VectorType& data, BufferFormat format,
				std::size_t numSamplesPerChannel) const -> void
			{
				target.uploadData(&data[mSampleOffset * mInternalLoader.getNumChannels()], format,
					numSamplesPerChannel * mInternalLoader.getNumChannels() * sizeof(data[0]),
					mInternalLoader.getSampleRate());
			}

			auto mono8DataBuffer(SoundBuffer& target, std::size_t numSamplesPerChannel) const -> void
			{
				std::vector<std::uint8_t> targetData{};
				HJUIK_ASSERT(mInternalLoader.samples.size() == 1, "Invalid call");
				const auto& data = mInternalLoader.samples[0];
				targetData.reserve(data.size());
				std::transform(data.begin(), data.begin() + numSamplesPerChannel, std::back_inserter(targetData),
					[](auto sample) { return static_cast<std::uint8_t>(sample + INT8_MAX); });
				upload(target, targetData, BufferFormat::MONO8, numSamplesPerChannel);
			}

			auto mono16DataBuffer(SoundBuffer& target, std::size_t numSamplesPerChannel) const -> void
			{
				HJUIK_ASSERT(mInternalLoader.samples.size() == 1, "Invalid call");
				upload(target, mInternalLoader.samples[0], BufferFormat::MONO16, numSamplesPerChannel);
			}

			auto stereo8DataBuffer(SoundBuffer& target, std::size_t numSamplesPerChannel) const -> void
			{
				const auto& loadedData = mInternalLoader.samples;
				HJUIK_ASSERT(loadedData.size() == 2, "Invalid call");
				std::vector<std::uint8_t> targetData{};
				targetData.reserve(numSamplesPerChannel * 2);
				for (int i = 0; i < numSamplesPerChannel; ++i) {
					targetData.push_back(static_cast<std::uint8_t>(loadedData[0][i] + INT8_MAX));
					targetData.push_back(static_cast<std::uint8_t>(loadedData[1][i] + INT8_MAX));
				}
				upload(target, targetData, BufferFormat::STEREO8, numSamplesPerChannel);
			}

			auto stereo16DataBuffer(SoundBuffer& target, std::size_t numSamplesPerChannel) const -> void
			{
				const auto& loadedData = mInternalLoader.samples;
				HJUIK_ASSERT(loadedData.size() == 2, "Invalid call");
				std::vector<std::int16_t> targetData{};
				targetData.reserve(numSamplesPerChannel * 2);
				for (int i = 0; i < numSamplesPerChannel; ++i) {
					targetData.push_back(loadedData[0][i]);
					targetData.push_back(loadedData[1][i]);
				}
				upload(target, targetData, BufferFormat::STEREO16, numSamplesPerChannel);
			}

			AudioFile<std::int16_t> mInternalLoader;
			std::size_t mSampleOffset = 0;
		};
	} // namespace Audio
} // namespace HJUIK

#endif
