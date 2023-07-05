#ifndef GAMEFRAME_AUDIO_FFMPEG_ERROR_HPP
#define GAMEFRAME_AUDIO_FFMPEG_ERROR_HPP

#include <stdexcept>
#include <string>

#include "FFmpeg.hpp"

namespace HJUIK
{
	namespace Audio
	{
		namespace FFmpeg
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
		} // namespace FFmpeg

	} // namespace Audio

} // namespace HJUIK


#endif
