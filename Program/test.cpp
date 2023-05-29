#include <iostream>
#include "../Utilize/ResourceHolder.hpp"

struct Triple
{
	float x; float y; float z;

	Triple& operator = (Triple& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		std::cout << "Copied";
		return *this;
	}

	Triple(int a = 0, int b = 0, int c = 0)
	{
		x = a; y = b; z = c;
	}
};

#include "Application.hpp"

int main()
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
}