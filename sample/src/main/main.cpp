#include "pch.h"

#include "Application/Application.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nSHowCmd )
{
	Application app;

	app.Init( lpCmdLine, hInstance );
	app.EntryMessage();

	return 0;
}