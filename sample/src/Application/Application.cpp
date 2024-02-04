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


#if _HAS_CXX17
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

#if ComparePerfomance
	Timer t1;
#endif

#if _HAS_CXX17
	std::for_each(std::execution::par, _imageVerticalIterator.begin(), _imageVerticalIterator.end(),
		[this, &sizeWindow, &horStart, &horEnd, &verStart, &verEnd, &camepra_pos, &fCameraDirection](uint32_t y)
		{
			std::for_each(std::execution::par, _imageHorizontalIterator.begin(), _imageHorizontalIterator.end(),
			[this, y, &sizeWindow, &horStart, &horEnd, &verStart, &verEnd, &camepra_pos, &fCameraDirection](uint32_t x)
				{
					float u = float(x) / float(sizeWindow.x - 1);
					float v = float(y) / float(sizeWindow.y - 1);

					//total distribution
					//x-> [-1, 1]
					//y-> [1, -1]
					float3 dir = Math::Lerp(horStart, horEnd, u) + Math::Lerp(verStart, verEnd, v) + fCameraDirection;

					Ray r;
					r._direction = dir;
					r._position = camepra_pos;

					//r.setPostion(camepra_pos);
					//r.setDirection(dir);

					////calc colo from ray
					float3 col = this->RayTracing(r);

					////clamping extra energy
					col.x = Math::Saturate(col.x);
					col.y = Math::Saturate(col.y);
					col.z = Math::Saturate(col.z);

					////put color into backbuffer
					unsigned int uiIndex = x + y * sizeWindow.x;
					this->_Backbuffer[uiIndex] = col;
				});
		});
						
	#else
	for (uint32_t y = 0; y < sizeWindow.y; ++y)
	{
		for (uint32_t x = 0; x < sizeWindow.x; ++x)
		{
			float u = float(x) / float(sizeWindow.x - 1);
			float v = float(y) / float(sizeWindow.y - 1);

			//total distribution
			//x-> [-1, 1]
			//y-> [1, -1]
			float3 dir = Math::Lerp(horStart, horEnd, u) + Math::Lerp(verStart, verEnd, v) + fCameraDirection;

			Ray r;
			r._direction = dir;
			r._position = camepra_pos;

			//r.setPostion(camepra_pos);
			//r.setDirection(dir);

			////calc colo from ray
			float3 col = this->RayTracing(r);

			////clamping extra energy
			col.x = Math::Saturate(col.x);
			col.y = Math::Saturate(col.y);
			col.z = Math::Saturate(col.z);

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
	float3 fSpherePos(0.0f, 0.0f, 1.0f);

	auto hitSphere = [](const float3& center, const Ray& ray, float radius = 0.5f) -> float
	{
		float3 AC = ray._position - center;
		float a{ float3::Dot(ray._direction, ray._direction) };
		float b{ 2.f * float3::Dot(AC, ray._direction) };
		float c{ float3::Dot(AC, AC) - radius * radius };
		float discriminant{ b * b - 4.f * a * c };

		if (discriminant < 0.0f)
		{
			return -1.0f;
		}

		float sol1{ -b / (2.f * a) }, sol2{ sol1 };
		float semiResult = (std::sqrt(discriminant)) / (2.f * a);
		sol1 += semiResult;
		sol2 -= semiResult;

		return Math::Min(sol1, sol2);
	};

	float fDistance = hitSphere(fSpherePos, ray);

	if (fDistance > 0.f)
	{
		float3 posAt = ray.PointAt(fDistance);

		/*float forBox = posAt.x + posAt.y + posAt.z;
		if (forBox <= 3.f * 0.5f)
		{
			return BoxColor[0];
		}*/

		float3 normal = (posAt - fSpherePos).Norm();
		normal.z = -normal.z;
		
		//normal.x = 0.f;
		//normal.y = 0.f;
		//normal.z = 0.f;

		return 0.5f * (float3::kOne + normal);
	}

	float3 vCoord{ ray._direction + ray._position };

	float LerpL{ 1.0f }, LerpR{ 0.0f };

	std::array<float, 3> bgr;

	float Gradient{};

#if HG
	Gradient = 0.5f * (vCoord.x + 1.f);

#else
	Gradient = 0.5f * (-vCoord.y + 1.f);
#endif

#if !NDEBUG
	TCHAR szDebugString[256];
	sprintf_s(szDebugString, sizeof(szDebugString), "Gradient: %f\n",
		Gradient);
	OutputDebugString(szDebugString);
#else

#endif

	Gradient = Math::Lerp(LerpL, LerpR, Gradient);

	for (unsigned short i = 0; i < 3; i++)
	{
		bgr[i] = Gradient;
	}

	return float3(bgr[0], bgr[1], bgr[2]);
}

float3 Ray::PointAt(float Distance) const
{
	//return float3();
	return this->_direction * Distance + this->_position;
}


#if ComparePerfomance
Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end - start;

	TCHAR szDebugString[256];
	sprintf_s(szDebugString, sizeof(szDebugString), "Timer took: %f ms\n",
		duration.count() * 1000.f);
	OutputDebugString(szDebugString);
}
#endif