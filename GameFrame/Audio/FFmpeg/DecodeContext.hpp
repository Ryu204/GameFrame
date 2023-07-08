#ifndef GAMEFRAME_AUDIO_FFMPEG_DECODE_CONTEXT_HPP
#define GAMEFRAME_AUDIO_FFMPEG_DECODE_CONTEXT_HPP

#include <optional>

#include "Wrappers.hpp"

namespace HJUIK
{
	namespace Audio
	{
		namespace FFmpeg
		{
			class DecodeContext
			{
			public:
				DecodeContext(const char* url) : mFormatCtx{newFormatContext(url)}
				{
					avCheck(avformat_find_stream_info(mFormatCtx.get(), nullptr));
					const AVCodec* codec = nullptr;
					mStreamIndex =
						avCheck(av_find_best_stream(mFormatCtx.get(), AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0));
					avCheck(avcodec_parameters_to_context(mCodecCtx.get(), stream().codecpar));
					avCheck(avcodec_open2(mCodecCtx.get(), codec, nullptr));
				}

				auto stream() -> AVStream&
				{
					return *mFormatCtx->streams[mStreamIndex];
				}

				auto getCodecParams() -> const AVCodecParameters&
				{
					return *stream().codecpar;
				}

				auto getFrame() -> Frame&
				{
					return mFrame;
				}

				/** The decoding process is as follows:
				 * Read frames from the format context via `av_read_frame` to get the raw packet data.
				 * Send the packet to the codec context via `avcodec_send_packet`.
				 * In a loop, get the decoded frame via `avcodec_receive_frame`.
				 *
				 * This method returns false if there are no more frames left.
				 */
				auto decodeFrame() -> bool
				{
					if (!mPacketGuard.has_value()) {
						// no packet guard means that either `sendPacket` hasn't been called or
						// all of the frames of the packet has been read

						// 1. Read frames, get packet data and send packet to the codec context
						sendPacket();
					}

					try {
						// 2. Receive the decoded frame and returns true
						avCheck(avcodec_receive_frame(mCodecCtx.get(), mFrame.get()));
						return true;
					} catch (AVException& ex) {
						// AVERROR(EAGAIN) -> must send new input (i.e. send new packet)
						// AVERROR_EOF -> no more output frames
						if (ex.getErrorCode() == AVERROR(EAGAIN)) {
							mPacketGuard.reset();
							return decodeFrame();
						} else if (ex.getErrorCode() == AVERROR_EOF) {
							mPacketGuard.reset();
							return false;
						} else {
							throw;
						}
					}

					return false;
				}

				auto sendPacket() -> void
				{
					avCheck(av_read_frame(mFormatCtx.get(), mPacket.get()));
					// basically a RAII guard for av_packet_unref
					// this must be valid while frames are decoding from this packet
					// (it will be reset if last frame of the packet is decoded)
					do {
						mPacketGuard.reset();
						avCheck(avcodec_send_packet(mCodecCtx.get(), mPacket.get()));
						mPacketGuard.emplace(mPacket.get());
					} while (mPacket->stream_index != mStreamIndex);
				}

			private:
				FormatContext mFormatCtx;
				CodecContext mCodecCtx;
				Packet mPacket;
				Frame mFrame;
				int mStreamIndex;

				std::optional<PacketRefGuard> mPacketGuard = std::nullopt;
			};
		} // namespace FFmpeg
	}	  // namespace Audio
} // namespace HJUIK


#endif
