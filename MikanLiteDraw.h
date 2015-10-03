#ifndef __MIKAN_LITE_DRAW
#define __MIKAN_LITE_DRAW

#include "MikanLite.h"
#include "Common.h"
#include <D2d1.h>
#include <D2d1helper.h>
#include <Dwrite.h>

#define MAX_BUFFER_SIZE 1000

struct MIKANFONT
{
	IDWriteTextFormat *format;
	unsigned long color;
};

class MikanLiteDraw :public MikanDraw, DrawManagement
{
private:
	HWND window;

	ID2D1Factory *d2dfactory;
	IDWriteFactory *writefactory;
	ID2D1HwndRenderTarget *rendertarget;

	ID2D1SolidColorBrush *colorbrush;

	struct MIKANFONT *fonts;
	unsigned int fontmax;
public:
	// Draw management.
	MikanLiteDraw( void );
	virtual ~MikanLiteDraw( void );
	virtual long Init( HWND window );

	virtual struct ID2D1Factory *GetD2DFactory( void );
	virtual struct IDWriteFactory *GetWriteFactory( void );
	virtual struct ID2D1HwndRenderTarget *GetRenderTarget( void );

	virtual void Begin( void );
	virtual long End( void );

	virtual void Recover( void );

	virtual long CreateRenderTarget( void );
	virtual void ReleaseRenderTarget( void );

	virtual D2D1_MATRIX_3X2_F DPI2DPI( void );

	// Draw.
	virtual void ClearScreen( void );

	virtual void DrawBox( int x, int y, int w, int h, unsigned long color );

	// Draw font.
	virtual void InitFont( unsigned int max = 0 );
	virtual void ReleaseFont( bool del = false );

	virtual void CreateFont( unsigned int font, const wchar_t *fontname = L"Meiryo", int size = 10, unsigned long color = 0xFFFFFFFF, int weight = DWRITE_FONT_WEIGHT_NORMAL, int italic = DWRITE_FONT_STYLE_NORMAL );
	virtual void Print( unsigned int font, int x, int y, const wchar_t *str );
	virtual void Print( unsigned int font, int x, int y, unsigned long color, const wchar_t *str );
	virtual void Printf( unsigned int font, int x, int y, const wchar_t *format, ... );
	virtual void Printf( unsigned int font, int x, int y, unsigned long color, const wchar_t *format, ... );
};

#endif
