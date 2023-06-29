#include <iostream>

#include "../Utilize/ResourceHolder.hpp"
#include "Application.hpp"

auto umain() -> int
{
	try
    {
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

#include "../Audio/LoaderWAV.hpp"
#include "../Audio/AudioDevice.hpp"

auto main() -> int
{
	try
    {
		HJUIK::Audio::AudioDevice device;
		HJUIK::Audio::LoaderWAV loader;
		HJUIK::Audio::SoundBuffer buffer;
		loader.loadFromFile("Sound.wav");
		loader.buffer(buffer);

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