#include <iostream>

#include "../Utilize/ResourceHolder.hpp"
#include "Application.hpp"
#include "../Audio/SoundBuffer.hpp"
#include "../Audio/LoaderWAV.hpp"

auto main() -> int
{
	try
    {
		HJUIK::Audio::LoaderWAV loader;
		loader.loadFromFile("build/Sound.wav");
		HJUIK::Audio::SoundBuffer buffer;
		buffer.bufferData(loader);
		
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