#include "MikanLiteDraw.h"


MikanLiteDraw::MikanLiteDraw( void )
{
	d2dfactory = NULL;
	rendertarget = NULL;
	colorbrush = NULL;
	fonts = NULL;
	fontmax = 0;
}

MikanLiteDraw::~MikanLiteDraw( void )
{
	ReleaseFont( true );
	ReleaseRenderTarget();
	SafeRelease( d2dfactory );
	SafeRelease( writefactory );
}

long MikanLiteDraw::Init( HWND window )
{
	HRESULT hr;

	this->window = window;

	// Direct2D
	hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dfactory );
	if ( FAILED( hr ) ) { return hr; }

	// DirectWrite
	hr = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof( writefactory ), reinterpret_cast<IUnknown **>( &writefactory ) );
	if ( FAILED( hr ) ) { return hr; }

	hr = CreateRenderTarget();
	if ( FAILED( hr ) ) { return hr; }

	InitFont( 32 );
	CreateFont( 0 );

	rendertarget->SetTransform( DPI2DPI() );

	return hr;
}

ID2D1Factory * MikanLiteDraw::GetD2DFactory( void )
{
	return d2dfactory;
}

IDWriteFactory * MikanLiteDraw::GetWriteFactory( void )
{
	return writefactory;
}

ID2D1HwndRenderTarget * MikanLiteDraw::GetRenderTarget( void )
{
	return rendertarget;
}

void MikanLiteDraw::Begin( void )
{
	rendertarget->BeginDraw();
}

long MikanLiteDraw::End( void )
{
	HRESULT hr;

	hr = rendertarget->EndDraw();

	// DeviceLost
	if ( hr == D2DERR_RECREATE_TARGET )
	{
		Recover();
	}

	return hr;
}

void MikanLiteDraw::Recover( void )
{
	ReleaseRenderTarget();
	ReleaseFont();
	CreateRenderTarget();
	InitFont();
}

long MikanLiteDraw::CreateRenderTarget( void )
{
	HRESULT hr;
	RECT rect;

	GetClientRect( window, &rect );

	hr = d2dfactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties( window, D2D1::SizeU( rect.right - rect.left, rect.bottom - rect.top ) ),
		&rendertarget
		);

	if ( FAILED( hr ) ) { return hr; }

	rendertarget->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::White ), &colorbrush );

	return hr;
}

void MikanLiteDraw::ReleaseRenderTarget()
{
	SafeRelease( rendertarget );
	SafeRelease( colorbrush );
}

D2D1_MATRIX_3X2_F MikanLiteDraw::DPI2DPI( void )
{
	D2D1_MATRIX_3X2_F ret;
	float x, y;

	ret = D2D1::IdentityMatrix();

	d2dfactory->GetDesktopDpi( &x, &y );
	if ( x == 0 || y == 0 ) { return D2D1_MATRIX_3X2_F(); }

	ret._11 = 96.0f / x;
	ret._22 = 96.0f / y;

	return ret;
}
