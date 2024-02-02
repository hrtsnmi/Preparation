#include "pch.h"

#include "Application.h"

#include <execution> // for execution::par

extern Application* kApp = nullptr;

LRESULT CALLBACK Win32IWindowWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( kApp )
	{
		LRESULT lr = kApp->WinProcCallBack( hwnd, msg, wParam, lParam );
		if( lr != -1 )
		{
			return lr;
		}
	}

	return DefWindowProc( hwnd, msg, wParam, lParam );
}

Application::Application()
	: _isApplicationAlive( true )
	, _isApplicationInited( false )
	, _Backbuffer( nullptr )
	, finalPixelmap( nullptr )
{
	kApp = this;


	#if MT
	//set itterators
	_imageVerticalIterator.resize(this->_Window.getSize().y);
	_imageHorizontalIterator.resize(this->_Window.getSize().x);

	for (uint32_t i = 0; i < this->_Window.getSize().y; ++i)
	{
		_imageVerticalIterator[i] = i;
	}

	for (uint32_t i = 0; i < this->_Window.getSize().x; ++i)
	{
		_imageHorizontalIterator[i] = i;
	}
	#else

	#endif
}

Application::~Application()
{
	if( this->_Backbuffer )
	{
		delete[] this->_Backbuffer;
		this->_Backbuffer = nullptr;
	}
	if( this->finalPixelmap )
	{
		delete[] this->finalPixelmap;
		this->finalPixelmap = nullptr;
	}

	this->_isApplicationInited = false;
}


void Application::Init( const char* commandLine, const HINSTANCE& hInst )
{
	this->_Backbuffer = new float3[ 1024 * 768 ];

	this->finalPixelmap = new COLORREF[ 1024 * 768 ];

	// initing window 
	this->_Window.Init( hInst, &Win32IWindowWndProc, "Simple Sample" );

	this->_isApplicationInited = true;
}

void Application::EntryMessage()
{
	MSG msg;
	ZeroMemory( &msg, sizeof( MSG ) );

	while( this->_isApplicationAlive )
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

}

void Application::Close()
{
	this->_isApplicationAlive = false;
}

LRESULT Application::WinProcCallBack( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	

	switch( msg )
	{
	case WM_PAINT:
		{
		if( this->_isApplicationInited )
		{
			HDC hdc = BeginPaint( hwnd, &ps );



			// our draw
			this->Frame();


			
			this->CopyColorToWindow( hdc );


			EndPaint( hwnd, &ps );
			//else
			//{
			//	// let's sleep for a while
			//	Sleep( 50 );
			//}
		}
		}
		return 0;

	

	// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		( (MINMAXINFO* )lParam )->ptMinTrackSize.x = 1024;
		( (MINMAXINFO* )lParam )->ptMinTrackSize.y = 768;
		( ( MINMAXINFO* )lParam )->ptMaxTrackSize.x = 1024;
		( ( MINMAXINFO* )lParam )->ptMaxTrackSize.y = 768;
		( ( MINMAXINFO* )lParam )->ptMaxSize.x = 1024;
		( ( MINMAXINFO* )lParam )->ptMaxSize.y = 768;
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT( 0, MNC_IGNORE );

	case WM_CLOSE:
		this->_isApplicationAlive = false;
		return 0;

	case WM_DESTROY:
		::PostQuitMessage( 0 );
		break;
	}
	return -1;

}

void Application::CleanBackbuffer( const float3& color )
{
	// for every texel in backbuffer need to setup color
	// not so efficient, but, don't care for now
	unsigned int uiTexels = 1024U * 768U;

	for( unsigned int i = 0; i < uiTexels; ++i )
	{
		this->_Backbuffer[ i ] = color;
	}
}

void Application::Frame()
{
	// clean back buffer with color
	this->CleanBackbuffer( { 0.0f, 0.2f, 0.0f } );

	const Math::int2& sizeWindow = this->_Window.getSize();

	//camera is looking along Z axis (lefthand sys)
	// be bouble awere with this
	float3 fCameraDirection = float3::kAxisZ;

	const float3& camepra_pos = fCameraDirection * -1.f;

	const float aspect_ratio = float(sizeWindow.x) / float(sizeWindow.y);
	float fMiddleAngle = 90.0f;
	float fov = 45.0f;
	float fHalfFov = fov * 0.5f;

	float cosmin = cosf(Math::AngleToRadian(fMiddleAngle - fHalfFov));
	float cosmax = cosf(Math::AngleToRadian(fMiddleAngle + fHalfFov));

	float3 horStart(cosmax * aspect_ratio, 0.0f, 0.0f);
	float3 horEnd(cosmin * aspect_ratio, 0.0f, 0.0f);

	float3 verStart(0.0f, cosmin, 0.0f);
	float3 verEnd(0.0f, cosmax, 0.0f);

	#if MT
	std::for_each(std::execution::par, _imageVerticalIterator.begin(), _imageVerticalIterator.end(), [this, &sizeWindow, &horStart, &horEnd, &verStart, &verEnd, &camepra_pos, &fCameraDirection](uint32_t y)
		{
			std::for_each(std::execution::par, _imageHorizontalIterator.begin(), _imageHorizontalIterator.end(), [this, y, &sizeWindow, &horStart, &horEnd, &verStart, &verEnd, &camepra_pos, &fCameraDirection](uint32_t x)
				{
					float u = float(y) / float(sizeWindow.x - 1);
					float v = float(x) / float(sizeWindow.y - 1);
					
					//total distribution
					//x-> [-1, 1]
					//y-> [1, -1]
					float3 dir = Math::Lerp(horStart, horEnd, u) + Math::Lerp(verStart, verEnd, v) + fCameraDirection;
					
					Ray r;
	
					r._direction = dir;
					r._position = camepra_pos;
					//r.setPostion(camepra_pos);
					//r.setDirection(dir);

					//calc colo from ray
					float3 col = this->RayTracing(r);

					//clamping extra energy
					col.x = Math::Saturate(col.x);
					col.y = Math::Saturate(col.y);
					col.z = Math::Saturate(col.z);

					//put color into backbuffer
					unsigned int uiIndex = x + y * sizeWindow.x;
					this->_Backbuffer[uiIndex] = col;
				});
		});
			
					
	#else
	for (uint32_t y = 0; y < sizeWindow.y; ++y)
	{
		for (uint32_t x = 0; x < sizeWindow.x; ++x)
		{
			float u = float(y) / float(sizeWindow.x - 1);
			float v = float(x) / float(sizeWindow.y - 1);

			//total distribution
			//x-> [-1, 1]
			//y-> [1, -1]
			float3 dir = Math::Lerp(horStart, horEnd, u);

			Ray r;
			r._direction = dir;
			r._position = camepra_pos;
			//r.setPostion(camepra_pos);
			//r.setDirection(dir);

			////calc colo from ray
			float3 col = this->RayTracing(r);

			////clamping extra energy
			//col.x = Math::Saturate(col.x);
			//col.y = Math::Saturate(col.y);
			//col.z = Math::Saturate(col.z);

			////put color into backbuffer
			unsigned int uiIndex = x + y * sizeWindow.x;
			this->_Backbuffer[uiIndex] = col;
		}
	}
	#endif

#pragma message( "void Application::Frame(), YOUR FUTURE RAY TRACING WILL BE HERE" )
}

void Application::CopyColorToWindow( HDC hdc )
{
	// THIS IS really unimportant part of the code,
	// current application should just draw smth, and this code
	// ... well, it should be better, but common
	// GDI...
	//
	// if you're really strong and promise to yerself to not be
	// scared by Windows95 API
	// you're allowed to read it

	// fillup current color ref buffer( buffer with RGBA or ARGB layout )
	const unsigned int uiTexels = 1024U * 768U;
	for( unsigned int i = 0; i < uiTexels; ++i )
	{
		finalPixelmap[ i ] = RGB(
			this->_Backbuffer[ i ].x * 255.0f,
			this->_Backbuffer[ i ].y * 255.0f,
			this->_Backbuffer[ i ].z * 255.0f );
	}

	// creatign new bitmap with size of screen

	HBITMAP hBitmap = CreateBitmap( 1024, 768, 1, 32, finalPixelmap );

	// creating local DC( in GDI+ it's called 'Graphics' )
	HDC hLocalDC = CreateCompatibleDC( hdc );

	// selecting our bitmap in GDI+'s Graphics
	HBITMAP hOldBmp = ( HBITMAP )SelectObject( hLocalDC, hBitmap );

	// copy from localDC( Graphics ) to our window one
	BitBlt( hdc, 0, 0, 1024, 768, hLocalDC, 0, 0, SRCCOPY );

	// selecting literaly nothing( or system's value )
	// to aviod memory linked resources( can be kind of leak )
	SelectObject( hLocalDC, hOldBmp );

	// dropping localDC(Grapchics )
	DeleteDC( hLocalDC );

	// dropping bitmap
	DeleteObject( hBitmap );
}

float3 Application::RayTracing(const Ray& ray)
{
	ray._direction.Norm();
	float LerpL{}, LerpR{ 1.0f };
	
	float r{0.f}, g{0.f}, b{1.0f};

	//r = Math::Lerp(LerpL, LerpR, ray._direction.x + 1.f);
	//g = Math::Lerp(LerpL, LerpR, 2.0f - ray._direction.y - 2.f);
	
	
	return float3(b, g, r);
}

