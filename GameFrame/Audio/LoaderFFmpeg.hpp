#ifndef GAMEFRAME_AUDIO_LOADER_FFMPEG_HPP
#define GAMEFRAME_AUDIO_LOADER_FFMPEG_HPP

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswresample/swresample.h>
}

#include <memory>
#include <stdexcept>
#include <string>

#include "FFmpeg/DecodeContext.hpp"
#include "FFmpeg/Resampler.hpp"
#include "ILoader.hpp"
#include "SoundBuffer.hpp"

namespace HJUIK
{
	namespace Audio
	{
		/*
		 * DECODING PIPELINE:
		 * 1. DecodeContext -> AVFrames
		 * 2. AVFrame -> Sample data
		 * 3. Sample data -> audio sample buffer
		 * 4. audio sample buffer -> SoundBuffer
		 *
		 * First, the DecodeContext (which contains FFmpeg contexts like the AVFormatContext
		 * and the AVCodecContext), uses the classic av_read_frame/avcodec_send_packet/avcodec_receive_frame
		 * way to receive audio frames.
		 *
		 * These audio frames contain audio data in AVFrame::data. However, a problem here is that it may be
		 * the case that the AVFrame is incompatible with OpenAL supported formats, so there may be an extra
		 * resampling step.
		 *
		 * The sample data extracted/converted from the AVFrame can now be buffered in an audio sample buffer,
		 * which is basically a std::vector<std::uint8_t>. This is needed due to the fact that FFmpeg does
		 * not allow decoding precisely up to a certain sample index, so one has to buffer the sample data.
		 *
		 * Every call to `buffer` would take some data from the audio sample buffer, until the buffer is empty.
		 * Then, the pipeline will go back to step 1 or 2 to retrieve new sample data. This process would
		 * continue until EOF was reached.
		 */
		class LoaderFFmpeg : public ILoader
		{
		public:
			// Load an audio file from URL (filename is OK too)
			LoaderFFmpeg(const char* url) : mDecodeCtx{url}, mResampler{mDecodeCtx.getCodecParams()} {}

			auto channels() -> std::size_t override
			{
				return static_cast<std::size_t>(mDecodeCtx.getCodecParams().channels);
			}

			auto sampleRate() -> std::size_t override
			{
				return static_cast<std::size_t>(mDecodeCtx.getCodecParams().sample_rate);
			}
			// seek temporarily not implemented to reduce complexity

			auto buffer(SoundBuffer& target, std::size_t maxSamples) -> std::size_t override
			{
				auto upload = [&, this](void* ptr, std::size_t numSamples) {
					target.uploadData(ptr, mResampler.getOpenALFormat(), numSamples * mResampler.getSampleSize(),
						mDecodeCtx.getCodecParams().sample_rate);
					return numSamples;
				};
				while (mSampleBuffer.size() < maxSamples * mResampler.getSampleSize()) {
					if (!mDecodeCtx.decodeFrame()) {
						break;
					}

					auto& frame		 = mDecodeCtx.getFrame();
					auto dataOnFrame = mResampler.getSampleData(frame, mSampleBuffer);
					// to directly load the audio sample from AVFrame to ALBuffer, these conditions must be satisfied:
					// - `mSampleBuffer` is empty
					// - `dataOnFrame` is true
					// - there is more (or as much as) data on AVFrame than the ALBuffer can handle
					// tbh these conditions are somewhat unlikely, so i don't expect much performance gain from this
					// optimization
					if (mSampleBuffer.empty() && dataOnFrame && frame->nb_samples * frame->channels >= maxSamples) {
						return upload(frame->data[0], frame->nb_samples * frame->channels);
					}

					// (there is also another case: the third condition is not satisfied, but this is the last frame,
					// but that's kind of a hassle to implement)
				}

				if (mSampleBuffer.empty()) {
					return 0;
				}

				const auto numSamples = std::min(maxSamples, mSampleBuffer.size() / mResampler.getSampleSize());
				upload(mSampleBuffer.data(), numSamples);
				mSampleBuffer.erase(
					mSampleBuffer.begin(), mSampleBuffer.begin() + numSamples * mResampler.getSampleSize());
				return numSamples;
			}

		private:
			FFmpeg::DecodeContext mDecodeCtx;
			FFmpeg::Resampler mResampler;
			std::vector<std::uint8_t> mSampleBuffer;
		};
	} // namespace Audio

} // namespace HJUIK


#endif
