
#include "Engine/LynxWindow.hpp"

int main()
{
	lynx::LynxWindow window;

	while (window.isOpen())
	{
		window.handleEvents();
		window.clear();

		window.display();
	}

	return 0;
}