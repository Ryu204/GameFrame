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

#include "ILoader.hpp"
#include "SoundBuffer.hpp"

namespace HJUIK
{
	namespace Audio
	{
		// FFmpeg wrapper objects
		namespace detail
		{
			class AVException : std::runtime_error
			{
			public:
				AVException(int error) : std::runtime_error{std::string{av_err2str(error)}}, mError{error} {}

				auto getErrorCode() -> int
				{
					return mError;
				}

			private:
				int mError;
			};

			inline auto avCheck(int error) -> int
			{
				if (error < 0) {
					throw AVException{error};
				}

				return error;
			}

			struct FormatContextDeleter {
				auto operator()(AVFormatContext* ctx) -> void
				{
					avformat_close_input(&ctx);
				}
			};

			using FormatContext = std::unique_ptr<AVFormatContext, FormatContextDeleter>;

			inline auto openFormatContext(const char* url) -> FormatContext
			{
				AVFormatContext* ctx = nullptr;
				avCheck(avformat_open_input(&ctx, url, nullptr, nullptr));
				return FormatContext{ctx};
			}

			struct CodecContextDeleter {
				auto operator()(AVCodecContext* ctx) -> void
				{
					avcodec_free_context(&ctx);
				}
			};

			using CodecContext = std::unique_ptr<AVCodecContext, CodecContextDeleter>;

			inline auto newCodecContext() -> CodecContext
			{
				return CodecContext{avcodec_alloc_context3(nullptr)};
			}

			struct FrameDeleter {
				auto operator()(AVFrame* frame) -> void
				{
					av_frame_free(&frame);
				}
			};

			using Frame = std::unique_ptr<AVFrame, FrameDeleter>;

			inline auto newFrame() -> Frame
			{
				return Frame{av_frame_alloc()};
			}

			struct PacketDeleter {
				auto operator()(AVPacket* packet) -> void
				{
					av_packet_free(&packet);
				}
			};

			using Packet = std::unique_ptr<AVPacket, PacketDeleter>;

			inline auto newPacket() -> Packet
			{
				return Packet{av_packet_alloc()};
			}

			struct ResampleContextDeleter {
				auto operator()(SwrContext* context)
				{
					swr_close(context);
				}
			};

			struct RescaleContext {
				std::unique_ptr<SwrContext, ResampleContextDeleter> Context;

				RescaleContext(const AVCodecParameters& params, const AVChannelLayout& outLayout,
					AVSampleFormat outFormat, int outSampleRate)
					: Context{[&] {
						  SwrContext* ctx = nullptr;
						  swr_alloc_set_opts2(&ctx, &outLayout, AV_SAMPLE_FMT_S16, outSampleRate, &params.ch_layout,
							  static_cast<AVSampleFormat>(params.format), params.sample_rate, AV_LOG_VERBOSE, nullptr);
						  return ctx;
					  }()},
					  ConvertedFrame{newFrame()}
				{
				}

				auto rescale(Frame& frame) -> void
				{
                    swr_convert(Context.get(), )
				}
			};
		} // namespace detail


		class LoaderFFmpeg : public ILoader
		{
		public:
			// Load an audio file from URL (filename is OK too)
			LoaderFFmpeg(const char* url)
				: mFormatCtx{detail::openFormatContext(url)}, mCodecCtx{detail::newCodecContext()},
				  mPacket{detail::newPacket()}, mFrame{detail::newFrame()}
			{
				using detail::avCheck;
				avCheck(avformat_find_stream_info(mFormatCtx.get(), nullptr));
				const AVCodec* codec = nullptr;
				mStreamIndex = avCheck(av_find_best_stream(mFormatCtx.get(), AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0));
				avCheck(avcodec_parameters_to_context(mCodecCtx.get(), stream().codecpar));
				avCheck(avcodec_open2(mCodecCtx.get(), codec, nullptr));
			}

			auto stream() -> AVStream&
			{
				return *mFormatCtx->streams[mStreamIndex];
			}

			auto buffer(SoundBuffer& target, size_t maxSamples) -> bool override
			{
				try {
					bool checkSeekTimestamp = true;
					size_t totalSamples		= 0;
					while (totalSamples <= maxSamples) {
						bool eof		   = false;
						const auto samples = decodePacket(target, checkSeekTimestamp, maxSamples - totalSamples, eof);
						av_packet_unref(mPacket.get());
						if (eof) {
							// the media file ended
							break;
						}

						// no need to check the timestamp after the first iteration
						checkSeekTimestamp = false;
						totalSamples += samples;
					}
				} catch (std::exception& ex) {
					av_packet_unref(mPacket.get());
					return false;
				}

				return true;
			}

			auto channels() -> size_t override
			{
				return static_cast<size_t>(stream().codecpar->channels);
			}

			auto sampleRate() -> size_t override
			{
				return static_cast<size_t>(stream().codecpar->sample_rate);
			}

			auto seekSecond(float second) -> void override
			{
				// idk what this mean i just copied from here
				// https://stackoverflow.com/questions/53015621/ffmpeg-library-how-to-precisely-seek-in-an-audio-file
				const auto targetTimestamp = av_rescale_q(second * AV_TIME_BASE, AV_TIME_BASE_Q, stream().time_base);

				// Using AVSEEK_FLAG_BACKWARD to make sure we're
				// always *before* requested timestamp.
				detail::avCheck(av_seek_frame(mFormatCtx.get(), mStreamIndex, targetTimestamp, AVSEEK_FLAG_BACKWARD));
				mSeekTimestamp = targetTimestamp;

				// reset leftover decode data
				resetLastDecodeData();
			}

		private:
			detail::FormatContext mFormatCtx;
			detail::CodecContext mCodecCtx;
			detail::Packet mPacket;
			detail::Frame mFrame;
			detail::RescaleContext mRescaleContext;
			int mStreamIndex;
			int64_t mSeekTimestamp = -1;

			std::vector<uint8_t> mLastDecodeData;
			size_t mLastDecodeOffset  = 0;
			size_t mLastDecodeSamples = 0;

			auto resetLastDecodeData() -> void
			{
				mLastDecodeOffset  = 0;
				mLastDecodeSamples = 0;
				mLastDecodeData.clear();
			}

			auto decodePacket(SoundBuffer& target, bool checkSeekTimestamp, size_t maxSamples, bool& eof) -> size_t
			{
				// there is left-over data from the last `decodePacket` call
				if (!mLastDecodeData.empty()) {
					const auto numSamples = std::min(mLastDecodeSamples, maxSamples);
					upload(&mLastDecodeData[mLastDecodeOffset], numSamples);
					mLastDecodeOffset += numSamples;
					if (mLastDecodeOffset == mLastDecodeSamples) {
						resetLastDecodeData();
					}
					return numSamples;
				}

				try {
					detail::avCheck(av_read_frame(mFormatCtx.get(), mPacket.get()));
				} catch (detail::AVException& ex) {
					if (ex.getErrorCode() == AVERROR_EOF) {
						// reach end-of-file
						eof = true;
						return 0;
					} else {
						// rethrow if unknown exception
						throw;
					}
				}

				if (checkSeekTimestamp) {
					if (mPacket->pts > 0 && mSeekTimestamp >= 0 && mPacket->pts < mSeekTimestamp) {
						// see
						// https://github.com/barsoosayque/libgdx-oboe/blob/master/library/src/cpp/mediacodec/audio_decoder.cpp
						const auto delta = (mSeekTimestamp - mPacket->pts) / stream().time_base.den;
						const auto numSkipFrames =
							static_cast<uint32_t>(delta * mCodecCtx->time_base.den / mCodecCtx->time_base.num);
						auto* skipData = av_packet_get_side_data(mPacket.get(), AV_PKT_DATA_SKIP_SAMPLES, nullptr);

						// AV_PKT_DATA_SKIP_SAMPLES layout:
						// u32le number of samples to skip from start of this packet
						// u32le number of samples to skip from end of this packet
						// u8    reason for start skip
						// u8    reason for end   skip (0=padding silence, 1=convergence)
						skipData[8] = 0;

						// store `numSkipFrames` in `skipData[0..3]` in little endian format
						skipData[0] = numSkipFrames >> 24;
						skipData[1] = (numSkipFrames >> 16) & 0xFF;
						skipData[2] = (numSkipFrames >> 8) & 0xFF;
						skipData[3] = numSkipFrames & 0xFF;
					}
				}

				detail::avCheck(avcodec_send_packet(mCodecCtx.get(), mPacket.get()));

				while (true) {
					try {
						detail::avCheck(avcodec_receive_frame(mCodecCtx.get(), mFrame.get()));
					} catch (detail::AVException& ex) {
						if (ex.getErrorCode() == AVERROR(EAGAIN) || ex.getErrorCode() == AVERROR_EOF) {
							break;
						}

						throw;
					}

					uploadFrame(mFrame.get());
				}
			}

			auto upload(const uint8_t* data, size_t size) -> void {}
		};
	} // namespace Audio

} // namespace HJUIK


#endif
