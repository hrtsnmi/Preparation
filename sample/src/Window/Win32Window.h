#pragma once


class Win32Window
{
public:
	// defualt {ctor} and {dtor}
	//
	Win32Window();
	virtual ~Win32Window();

	// init default params
	//
	virtual void Init( HINSTANCE hInstance, WNDPROC proc, const std::string& sClassName );

	// clean directx resources
	//
	void Clean();

	// setter / getter HWND
	//
	void setHwnd( HWND hw );
	HWND getHwnd()const;

	// getting instance handle
	//
	const HINSTANCE& getHInstance()const;

	// setter / getter window size
	//
	void setSize( const Math::int2& size );
	const Math::int2& getSize()const;


protected:
	// HWND instance
	//
	HWND _pHwnd;

	// hInst 
	//
	HINSTANCE _hInstance;

	// OS window properties
	//
	Math::int2 _Windowsize;

	// win32 class desc
	//
	WNDCLASSEXA _Win32Desc;
};



