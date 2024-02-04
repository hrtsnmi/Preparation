#pragma once

#include "Window/Win32Window.h"

#define HG 0
#define ComparePerfomance 0

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

#if _HAS_CXX17
protected:
	//adt iterators to use for_each for setting pixels
	std::vector<uint32_t> _imageHorizontalIterator;
	std::vector<uint32_t> _imageVerticalIterator;
#endif

protected:
	float3 RayTracing(const Ray& ray);
};


struct Ray
{
	float3 _position;
	float3 _direction;

	float3 PointAt(float Distance) const;
};

#if ComparePerfomance
#include <chrono>

class Timer
{
public:
	Timer();
	~Timer();

private:

	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
};
#endif

const float3 BoxColor[6]
{
	float3::kAxisX,				//right
	float3::kAxisZ,				//front
	float3(1.0f, 0.0f, 1.0f),	//left
	float3(0.0f, 1.0f, 1.0f),	//back
	float3::kAxisY,				//top
	float3(1.0f, 1.0f, 0.0f),	//bottom
};