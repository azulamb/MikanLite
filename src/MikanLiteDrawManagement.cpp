#include "MikanLiteDraw.h"

#pragma comment(lib, "d2d1.lib")
//Windowscodecs.lib

MikanLiteDraw::MikanLiteDraw( void )
{
	d2dfactory = NULL;
	rendertarget = NULL;
	colorbrush = NULL;
	textures = NULL;
	texturemax = 0;
	fonts = NULL;
	fontmax = 0;
}

MikanLiteDraw::~MikanLiteDraw( void )
{
	ReleaseTexture( true );
	ReleaseFont( true );
	ReleaseRenderTarget();
	SafeRelease( d2dfactory );
	SafeRelease( writefactory );
	//SafeRelease( imagingfactory );
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

	// ImagingFactory
	CoCreateInstance( CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof( IWICImagingFactory ), (LPVOID*)&imagingfactory );

	hr = CreateRenderTarget();
	if ( FAILED( hr ) ) { return hr; }

	InitTexture( 32 );
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
	ReleaseTexture();
	ReleaseFont();
	CreateRenderTarget();
	InitTexture();
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

void MikanLiteDraw::SetColor( unsigned long color )
{
	colorbrush->SetColor( D2D1::ColorF( color & 0xFFFFFF, ( color >> 24 ) / 255.0f ) );
}

void MikanLiteDraw::InitTexture( unsigned max )
{
	unsigned int i;
	if ( textures ) { ReleaseTexture(); }
	if ( 0 < max )
	{
		textures = ( struct MIKANTEXTURE* )realloc( textures, max * sizeof( struct MIKANTEXTURE ) );
		for ( ; texturemax < max; ++texturemax )
		{
			textures[ texturemax ].filename = NULL;
			textures[ texturemax ].bitmap = NULL;
		}
	}
	for ( i = 0; i < max; ++i )
	{
		// TODO: recreate
		if ( !textures[i].filename ) { continue; }
	}
}

void MikanLiteDraw::ReleaseTexture( bool del )
{
	unsigned int i;
	for ( i = 0; i < texturemax; ++i )
	{
		if ( del ) { free( textures[ i ].filename ); }
		//SafeRelease( textures[ i ].bitmap );
		if ( textures[ i ].bitmap )
		{
			//textures[ i ].bitmap->Release();
			textures[ i ].bitmap = NULL;
		}
	}
	if ( del )
	{
		free( textures );
		texturemax = 0;
	}
}

long MikanLiteDraw::CreateTexture( unsigned int tex, const wchar_t * filename, int trc )
{
	IWICBitmapDecoder *decoder = NULL;
	IWICBitmapFrameDecode *source = NULL;
	IWICFormatConverter *converter = NULL;
	BYTE color[ 4 ];
	HRESULT hr;

	if ( texturemax <= tex )
	{
		hr = ERROR_INVALID_DATA;
		goto LAST;
	}

	hr = imagingfactory->CreateDecoderFromFilename( filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder );
	if ( FAILED( hr ) ) { goto LAST; }
	
	// Create the initial frame.
	hr = decoder->GetFrame( 0, &source );
	if ( FAILED( hr ) ) { goto LAST; }

	// Convert the image format to 32bppPBGRA
	// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
	hr = imagingfactory->CreateFormatConverter( &converter );
	if ( FAILED( hr ) ) { goto LAST; }

	hr = converter->Initialize( source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut );
	if ( FAILED( hr ) ) { goto LAST; }

	if ( trc == TRC_ZERO )
	{
		hr = source->CopyPixels( NULL, 0, 4, color );
		if ( SUCCEEDED( hr ) )
		{
			// TODO: TRC
		}
	}

	// Create a Direct2D bitmap from the WIC bitmap.
	if ( textures[ tex ].bitmap ) { SafeRelease( textures[ tex ].bitmap ); }
	hr = rendertarget->CreateBitmapFromWicBitmap( converter, NULL, &( textures[ tex ].bitmap ) );
	if ( FAILED( hr ) )
	{
		SafeRelease( textures[ tex ].bitmap );
	} else
	{
		if ( textures[ tex ].filename ) { free( textures[ tex ].filename ); }
		textures[ tex ].filename = (wchar_t *)calloc( wcslen( filename ) + 1, sizeof( wchar_t ) );
		wcscpy_s( textures[ tex ].filename, wcslen( filename ) + 1, filename );
	}

LAST:
	SafeRelease( decoder );
	SafeRelease( source );
	SafeRelease( converter );

	return hr;
}

unsigned int MikanLiteDraw::GetTextureWidth( unsigned int tex )
{
	if ( texturemax <= tex || textures[ tex ].bitmap == NULL ) { return 0; }
	return (unsigned int)textures[ tex ].bitmap->GetSize().width;
}

unsigned int MikanLiteDraw::GetTextureHeight( unsigned int tex )
{
	if ( texturemax <= tex || textures[ tex ].bitmap == NULL ) { return 0; }
	return (unsigned int)textures[ tex ].bitmap->GetSize().height;
}
