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