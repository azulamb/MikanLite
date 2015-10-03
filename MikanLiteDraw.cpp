#include "MikanLiteDraw.h"

#pragma comment(lib, "d2d1.lib")

void MikanLiteDraw::ClearScreen( void )
{
	rendertarget->Clear();
}

void MikanLiteDraw::DrawBox( int x, int y, int w, int h, unsigned long color )
{
	colorbrush->SetColor( D2D1::ColorF( color & 0xFFFFFF, ( color >> 24 ) / 255.0f ) );
	rendertarget->FillRectangle( D2D1::RectF( (float)x, (float)y, (float)( x + w ), (float)( y + h ) ), colorbrush );
}
