#ifndef GAMEFRAME_AUDIO_FFMPEG_RESAMPLER_HPP
#define GAMEFRAME_AUDIO_FFMPEG_RESAMPLER_HPP

#include <cstdint>
#include <optional>
#include <vector>

#include "../SoundBuffer.hpp"
#include "Wrappers.hpp"

namespace HJUIK
{
	namespace Audio
	{
		namespace FFmpeg
		{
			// more high-level wrapper of the ResampleContext
			class Resampler
			{
			public:
				// from the codec params, the `Resampler` will figure out:
				// - the OpenAL buffer format (sample rate, channel layout, etc.)
				// - if it's needed for one to actually do the resampling
				// - where the sample data belong to
				Resampler(const AVCodecParameters& srcCodecParams)
				{
					const auto& inChannelLayout = srcCodecParams.ch_layout;
					const auto inSampleRate		= srcCodecParams.sample_rate;
					const auto inSampleFormat	= static_cast<AVSampleFormat>(srcCodecParams.format);

					const auto [outChannelLayout, commonLayout] = correspondingALChannelLayout(inChannelLayout);
					// the sample rate is kept the same
					const auto outSampleRate = inSampleRate;
					outSampleFormat			 = correspondingALSampleFormat(inSampleFormat, inChannelLayout.nb_channels);
					outNumChannels			 = outChannelLayout.nb_channels;

					if (!commonLayout || !outSampleFormat) {
						// conversion is needed
						mResampleCtx = newResampleContext(outChannelLayout, outSampleFormat, outSampleRate,
							inChannelLayout, inSampleFormat, inSampleRate);
					}

					// otherwise, keep mResampleCtx as nullptr
				}

				auto getBufferSize(int numSamples) -> int
				{
					return av_samples_get_buffer_size(nullptr, outNumChannels, numSamples, outSampleFormat, 1);
				}

                // return false if frame data does not need to be resampled and can be used directly
				auto getSampleData(Frame& frame, std::vector<std::uint8_t>& resampledBuffer) -> bool
				{
					if (mResampleCtx != nullptr) {
						// maximum number of out samples that could be gotten from
						const auto maxOutSamples = swr_get_out_samples(mResampleCtx.get(), frame->nb_samples);
						const auto oldBufferSize = resampledBuffer.size();
						// this may expand the buffer over the necessary amount...
						resampledBuffer.resize(static_cast<std::size_t>(getBufferSize(maxOutSamples)) + oldBufferSize);
						auto out = &resampledBuffer[oldBufferSize];
						const auto numOutSamples =
							swr_convert(mResampleCtx.get(), &out, maxOutSamples, frame->data, frame->nb_samples);
						// ...so we would have to shrink it back to it supposed size
						resampledBuffer.resize(static_cast<std::size_t>(getBufferSize(numOutSamples)) + oldBufferSize);
						return true;
					}

					return false;
				}

				auto getOpenALFormat() -> BufferFormat
				{
					if (outNumChannels == 1) {
						if (outSampleFormat == AV_SAMPLE_FMT_U8 || outSampleFormat == AV_SAMPLE_FMT_U8P) {
							return BufferFormat::MONO8;
						} else {
							return BufferFormat::MONO16;
						}
					} else {
						if (outSampleFormat == AV_SAMPLE_FMT_U8) {
							return BufferFormat::STEREO8;
						} else {
							return BufferFormat::STEREO16;
						}
					}
				}

				auto getSampleSize() -> std::size_t
				{
					return static_cast<std::size_t>(av_get_bytes_per_sample(outSampleFormat));
				}

			private:
				ResampleContext mResampleCtx;
				std::size_t mSampleBufferOffset;
				int outNumChannels;
				AVSampleFormat outSampleFormat;

				static auto correspondingALChannelLayout(const AVChannelLayout& srcLayout)
					-> std::tuple<AVChannelLayout, bool>
				{
					// temporarily will default to AV_CHANNEL_LAYOUT_STEREO
					if (srcLayout.order != AV_CHANNEL_ORDER_NATIVE) {
						return std::make_tuple<AVChannelLayout, bool>(AV_CHANNEL_LAYOUT_STEREO, false);
					}

					auto layoutsEqual = [](const AVChannelLayout& lhs, const AVChannelLayout& rhs) {
						// assuming lhs.order == AV_CHANNEL_ORDER_NATIVE due to
						// AV_CHANNEL_LAYOUT_STEREO/MONO both have this order
						return lhs.nb_channels == rhs.nb_channels && lhs.order == AV_CHANNEL_ORDER_NATIVE
							&& lhs.order == rhs.order && lhs.u.mask == rhs.u.mask;
					};

					if (layoutsEqual(AV_CHANNEL_LAYOUT_MONO, srcLayout)) {
						return std::make_tuple<AVChannelLayout, bool>(AV_CHANNEL_LAYOUT_MONO, true);
					}

					return std::make_tuple<AVChannelLayout, bool>(
						AV_CHANNEL_LAYOUT_STEREO, layoutsEqual(AV_CHANNEL_LAYOUT_STEREO, srcLayout));
				}

				static auto correspondingALSampleFormat(AVSampleFormat srcFormat, int numChannels) -> AVSampleFormat
				{
					// maybe supports other sample formats (might be supported by OpenAL extensions)
					// in the future, but for now, we'll only consider u8 and i16 interleaved sample formats
					// however, if `numChannels == 1`, the planar and interleaved formats are the same
					// so we'll also have to consider that
					if (srcFormat == AV_SAMPLE_FMT_U8 || (srcFormat == AV_SAMPLE_FMT_U8P && numChannels == 1)
						|| srcFormat == AV_SAMPLE_FMT_S16 || (srcFormat == AV_SAMPLE_FMT_S16P && numChannels == 1)) {
						return srcFormat;
					}

					// default to AV_SAMPLE_FMT_S16
					return AV_SAMPLE_FMT_S16;
				}
			};
		} // namespace FFmpeg
	}	  // namespace Audio
} // namespace HJUIK
#endif
