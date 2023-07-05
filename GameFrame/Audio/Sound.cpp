#include "Sound.hpp"

namespace HJUIK
{
	namespace Audio
	{
		Sound::Sound(std::size_t numSources, ILoader& loader)
		{
			loader.buffer(mBuffer, SIZE_MAX);
			static_assert(std::is_move_constructible_v<OpenALWrapper<detail::BufferTrait>>);
			for (std::size_t i = 0; i < numSources; ++i) {
				SoundSource source{};
				source.setBuffer(mBuffer);
				mSources.push_back(std::move(source));
			}
		}
		auto Sound::getPlayableSource() -> SoundSource*
		{
			for (auto& source : mSources) {
				if (source.getState() == SourceState::INITIAL || source.getState() == SourceState::STOPPED) {
					return &source;
				}
			}

			return nullptr;
		}
	} // namespace Audio

} // namespace HJUIK
