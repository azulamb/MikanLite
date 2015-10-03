#include "MikanLiteDraw.h"
#include <wchar.h>

#pragma comment(lib, "dwrite.lib")

void MikanLiteDraw::InitFont( unsigned int max )
{
	unsigned int i;
	if ( fonts ) { ReleaseFont(); }
	if ( 0 < max )
	{
		fonts = ( struct MIKANFONT* )realloc( fonts, max * sizeof( struct MIKANFONT ) );
		for ( ; fontmax < max; ++fontmax )
		{
			fonts[ fontmax ].format = NULL;
		}
	}
	for ( i = 0; i < max; ++i )
	{
		// TODO: recreate
		if ( !fonts[ i ].format ) { continue; }
	}
}

void MikanLiteDraw::ReleaseFont( bool del )
{
	unsigned int i;
	for ( i = 0; i < fontmax; ++i )
	{
		SafeRelease( fonts[ i ].format );
	}
	if ( del )
	{
		free( fonts );
		fontmax = 0;
	}
}

void MikanLiteDraw::CreateFont( unsigned int font, const wchar_t *fontname, int size, unsigned long color, int weight, int italic )
{
	HRESULT hr;

	if ( fontmax <= font ) { return; }

	fonts[ font ].color = 0xFFFFFFFF;

	hr = writefactory->CreateTextFormat(
		L"メイリオ",
		NULL, // Font collection (NULL=System font)
		DWRITE_FONT_WEIGHT_NORMAL, // weight
		DWRITE_FONT_STYLE_NORMAL, // italic
		DWRITE_FONT_STRETCH_NORMAL, //
		10.0f,//文字の大きさ
		L"ja-jp",//ロケール(地域)/ja-jp/en-us
		&( fonts[ font ].format )
		);
}

void MikanLiteDraw::Print( unsigned int font, int x, int y, const wchar_t * str )
{
	D2D1_RECT_F rect;

	if ( fontmax <= font || !fonts[ font ].format ) { font = 0; }

	rect.left = (float)x;
	rect.top = (float)y;
	// TODO: Calc size.
	rect.right = (float)( rect.left + 3000 );
	rect.bottom = (float)( rect.top + 3000 );

	colorbrush->SetColor( D2D1::ColorF( fonts[ font ].color & 0xFFFFFF, ( fonts[ font ].color >> 24 ) / 255.0f ) );

	rendertarget->DrawTextW( str, wcslen( str ), fonts[ font ].format, &rect, colorbrush );
}

void MikanLiteDraw::Print( unsigned int font, int x, int y, unsigned long color, const wchar_t * str )
{
	D2D1_RECT_F rect;

	if ( fontmax <= font || !fonts[ font ].format ) { font = 0; }

	rect.left = (float)x;
	rect.top = (float)y;
	// TODO: Calc size.
	rect.right = (float)( rect.left + 3000 );
	rect.bottom = (float)( rect.top + 3000 );

	colorbrush->SetColor( D2D1::ColorF( color & 0xFFFFFF, ( color >> 24 ) / 255.0f ) );

	rendertarget->DrawTextW( str, wcslen( str ), fonts[ font ].format, &rect, colorbrush );
}

void MikanLiteDraw::Printf( unsigned int font, int x, int y, const wchar_t * format, ... )
{
	wchar_t str[ MAX_BUFFER_SIZE ];
	//TODO: va_list
	_vsnwprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &format + 1 ) );
	return Print( font, x, y, str );
}

void MikanLiteDraw::Printf( unsigned int font, int x, int y, unsigned long color, const wchar_t * format, ... )
{
	wchar_t str[ MAX_BUFFER_SIZE ];
	//TODO: va_list
	_vsnwprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &format + 1 ) );
	return Print( font, x, y, color, str );
}
