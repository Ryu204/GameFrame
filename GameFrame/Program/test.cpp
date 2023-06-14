#include <iostream>

#include "../Utilize/ResourceHolder.hpp"
#include "Application.hpp"

auto main1() -> int
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

// testing
// NOLINTBEGIN
#include "../Graphics/ImageLoader.hpp"

auto main() -> int
{
    try
    {
		auto data = HJUIK::Graphics::Image2DLoader::loadFromFile("hello.pg");

        if (!HJUIK::Graphics::Image2DLoader::succeeded())
        {
			// throw std::runtime_error("cannot load image");
		}
        else
			std::cout << "Image loaded!\n";

		int i	  = 0;
		for (auto row = 0; row < data.Dimensions.y; ++row) {
			for (auto col = 0; col < data.Dimensions.x; ++col)
            {
				std::cout << (int)data.Value[i] << ',' <<
                    (int)data.Value[i + 1] << ',' << (int)data.Value[i + 2] << (int)data.Value[i + 3] << "\t\t";
				i += 4;
			}
			std::cout << '\n';
		}
		return 0;
	} catch (std::exception& e) {
		std::cerr << e.what() << '\n';
		return -1;
	} catch (...) {
		return -2;
	}
}
// NOLINTEND