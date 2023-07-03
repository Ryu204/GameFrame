#include <iostream>
#include <thread>
#include <chrono>

#include "../Utilize/ResourceHolder.hpp"
#include "Application.hpp"
#include "../Audio/SoundBuffer.hpp"
#include "../Audio/SoundSource.hpp"
#include "../Audio/LoaderWAV.hpp"

auto main() -> int
{
	try
    {
		using ld = HJUIK::Audio::LoaderWAV;
		using ss = HJUIK::Audio::SoundSource;
		using sb = HJUIK::Audio::SoundBuffer;

		ld loader;
		loader.loadFromFile("build/Sound.wav");
		sb buffer;
		ss source;
		buffer.bufferData(loader);
		source.setBuffer(buffer);
		source.play();

		HJUIK::Application app1;
		app1.run();
		return 0;
	}
    catch (std::exception& e)
    {
		std::cerr << e.what() << '\n';
		return -1;
	}
    catch (...)
    {
		return -2;
	}
}