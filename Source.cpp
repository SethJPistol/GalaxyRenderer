#include "Application.h"

int main()
{
	//Checks for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Application* app = new Application();	//Create the application

	app->Run();		//Start the game loop

	delete app;		//When the game loop has been exited, clear memory

	return 0;
}