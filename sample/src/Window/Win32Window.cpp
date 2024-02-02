#include "pch.h"

#include "Win32Window.h"

Win32Window::Win32Window()
	: _pHwnd( nullptr )
	, _hInstance( nullptr )
	, _Windowsize( 1024, 768 )
{
	memset( &this->_Win32Desc, 0, sizeof( this->_Win32Desc ) );
}

Win32Window::~Win32Window()
{
	this->Clean();
}

void Win32Window::Init( HINSTANCE hInstance, WNDPROC proc, const std::string& sClassName )
{
	if( this->_pHwnd )
	{
		return;
	}
	this->_hInstance = hInstance;

	WNDCLASSA cl;
	BOOL res = GetClassInfoA( hInstance, sClassName.c_str(), &cl );
	if( res )
	{
		// class exist!
		memcpy( &this->_Win32Desc, &cl, sizeof( cl ) );
	}
	else
	{
		// need to create new class !
		memset( &this->_Win32Desc, 0, sizeof( this->_Win32Desc ) );
		this->_Win32Desc.cbClsExtra = 0;
		this->_Win32Desc.cbWndExtra = 0;
		this->_Win32Desc.cbSize = sizeof( WNDCLASSEXA );
		this->_Win32Desc.style = CS_HREDRAW | CS_VREDRAW;
		this->_Win32Desc.lpszMenuName = "";

		this->_Win32Desc.hbrBackground = CreateSolidBrush( RGB( 0, 0, 0 ) );
		this->_Win32Desc.hCursor = LoadCursor( nullptr, IDC_ARROW );
		this->_Win32Desc.hIcon = LoadIcon( nullptr, IDI_APPLICATION );
		this->_Win32Desc.hIconSm = LoadIcon( nullptr, IDI_APPLICATION );

		this->_Win32Desc.hInstance = hInstance;
		this->_Win32Desc.lpszClassName = sClassName.c_str();

		this->_Win32Desc.lpfnWndProc = proc;

		ATOM result = RegisterClassExA( &this->_Win32Desc );
		if( !result )
		{
			//Assert( false, "Can't register wnd class" );
			return;
		}
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, (LONG)this->_Windowsize.x, (LONG)this->_Windowsize.y };
	AdjustWindowRect( &R, WS_OVERLAPPEDWINDOW, false );
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	this->_pHwnd = ::CreateWindowExA(
		0,
		this->_Win32Desc.lpszClassName,
		this->_Win32Desc.lpszClassName, //name window, title
		WS_OVERLAPPEDWINDOW,
		0, 0, // place
		width, height, // size
		nullptr /*parent hwnd*/,
		( HMENU )0/* menu */,
		this->_Win32Desc.hInstance,
		this );

	ShowWindow( this->_pHwnd, SW_SHOW );
	UpdateWindow( this->_pHwnd );
}

void Win32Window::Clean()
{	
}

void Win32Window::setHwnd( HWND hw )
{
	if( this->_pHwnd )
	{
		return;
	}
	this->_pHwnd = hw;
}

HWND Win32Window::getHwnd() const
{
	return this->_pHwnd;
}

const HINSTANCE& Win32Window::getHInstance() const
{
	return this->_hInstance;
}

void Win32Window::setSize( const Math::int2& size )
{
	this->_Windowsize = size;
}

const Math::int2& Win32Window::getSize() const
{
	return this->_Windowsize;
}
