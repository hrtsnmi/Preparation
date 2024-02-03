#pragma once

#include "Window/Win32Window.h"

#define MT 1
#define HG 1

struct Ray;

class Application final
{
public:
	// {ctor} / {dtor}
	//
	Application();
	~Application();

	// lock copy
	//
	Application( const Application& other ) = delete;
	Application& operator=( const Application& other ) = delete;

	// init window and Directx
	//
	void Init( const char* commandLine, const HINSTANCE& hInst );
	
	// game loop starts here
	//
	void EntryMessage();

	// drop main window
	//
	void Close();

	// messages from system
	//
	LRESULT WinProcCallBack( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );


	// clean backbufer
	//
	void CleanBackbuffer( const float3& color );

protected:
	// frame 
	//
	void Frame();

	void CopyColorToWindow( HDC hdc );

	// application
protected:
	// color bitmap for current window
	// it is a kind of back buffer, but without dx\vulkan API
	// you, can be sure, size is unchangable, so 1024x768!
	//
	float3* _Backbuffer;


	COLORREF* finalPixelmap;

	// system
protected:

	// window of Microsoft OS instance
	//
	Win32Window _Window;

	// is application is ready to draw smth
	//
	bool _isApplicationInited;

	// is allive application
	//
	bool _isApplicationAlive;

#if MT
protected:
	//adt iterators to use for_each for setting pixels
	std::vector<uint32_t> _imageHorizontalIterator;
	std::vector<uint32_t> _imageVerticalIterator;
#else

#endif

protected:
	float3 RayTracing(const Ray& ray);
};


struct Ray
{
	float3 _position;
	float3 _direction;
};