#include "pch.h"

#include "Application.h"

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

