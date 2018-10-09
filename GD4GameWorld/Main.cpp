#include "Game.hpp"

#include <stdexcept>
#include <iostream>

int main()
{
	try
	{
		Game game;
		game.run();
	}
	catch (std::exception e)
	{
		std::cout << "\n EXCEPTION: " << e.what() << std::endl;
	}
}