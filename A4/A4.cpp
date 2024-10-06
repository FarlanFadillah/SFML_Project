#include <SFML/Graphics.hpp>
#include <iostream>
#include "Vec2.hpp"
#include "GameEngine.hpp"
#include <fstream>

int main()
{
	std::ios::sync_with_stdio(false); // Disable synchronization
	std::cout.tie(nullptr); // Untie cin and cout for faster output
	GameEngine ge("res/config/config.ini"); 

	ge.run();

	return 0;
}
