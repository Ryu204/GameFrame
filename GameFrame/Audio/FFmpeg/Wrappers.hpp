#ifndef GAMEFRAME_AUDIO_FFMPEG_WRAPPERS_HPP
#define GAMEFRAME_AUDIO_FFMPEG_WRAPPERS_HPP

#include <memory>

#include "Error.hpp"
#include "FFmpeg.hpp"

namespace HJUIK
{
	namespace Audio
	{
		namespace FFmpeg
		{
#define HJUIK_MAKE_UNIQUE_PTR_TYPEDEF(name, type, deleteExpr) \
	namespace detail                                          \
	{                                                         \
		struct name##Deleter {                                \
			auto operator()(auto ptr)                         \
			{                                                 \
				deleteExpr;                                   \
			}                                                 \
		};                                                    \
	}                                                         \
	using name = std::unique_ptr<type, detail::name##Deleter>

			HJUIK_MAKE_UNIQUE_PTR_TYPEDEF(FormatContext, AVFormatContext, avformat_close_input(&ptr));
			HJUIK_MAKE_UNIQUE_PTR_TYPEDEF(CodecContext, AVCodecContext, avcodec_free_context(&ptr));
			HJUIK_MAKE_UNIQUE_PTR_TYPEDEF(Frame, AVFrame, av_frame_free(&ptr));
			HJUIK_MAKE_UNIQUE_PTR_TYPEDEF(Packet, AVPacket, av_packet_free(&ptr));
			HJUIK_MAKE_UNIQUE_PTR_TYPEDEF(ResampleContext, SwrContext, swr_close(ptr));
			HJUIK_MAKE_UNIQUE_PTR_TYPEDEF(FrameRefGuard, AVFrame, av_frame_unref(ptr));
			HJUIK_MAKE_UNIQUE_PTR_TYPEDEF(PacketRefGuard, AVPacket, av_packet_unref(ptr));

			inline auto newFormatContext(const char* url) -> FormatContext
			{
				AVFormatContext* context = nullptr;
				avCheck(avformat_open_input(&context, url, nullptr, nullptr));
				return FormatContext{context};
			}

			inline auto newCodecContext() -> CodecContext
			{
				return CodecContext{avcodec_alloc_context3(nullptr)};
			}

			inline auto newFrame() -> Frame
			{
				return Frame{av_frame_alloc()};
			}

			inline auto newPacket() -> Packet
			{
				return Packet{av_packet_alloc()};
			}

			inline auto newResampleContext(const AVChannelLayout& outChannelLayout, AVSampleFormat outSampleFormat,
				int outSampleRate, const AVChannelLayout& inChannelLayout, AVSampleFormat inSampleFormat,
				int inSampleRate) -> ResampleContext
			{
				SwrContext* context = nullptr;
				avCheck(swr_alloc_set_opts2(&context, &outChannelLayout, outSampleFormat, outSampleRate,
					&inChannelLayout, inSampleFormat, inSampleRate, AV_LOG_WARNING, nullptr));
				return ResampleContext{context};
			}
		} // namespace FFmpeg

	} // namespace Audio

} // namespace HJUIK


#endif
