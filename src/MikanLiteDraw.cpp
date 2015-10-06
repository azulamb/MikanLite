#include "MikanLiteDraw.h"

void MikanLiteDraw::ClearScreen( void )
{
	rendertarget->Clear();
}

void MikanLiteDraw::DrawBox( int x, int y, int w, int h, unsigned long color )
{
	SetColor( color );
	rendertarget->FillRectangle( D2D1::RectF( (float)x, (float)y, (float)( x + w ), (float)( y + h ) ), colorbrush );
}

void MikanLiteDraw::DrawBoxC( int cx, int cy, int w, int h, unsigned long color )
{
	SetColor( color );
	rendertarget->FillRectangle( D2D1::RectF( (float)( cx - w / 2 ), (float)( cy - h / 2 ), (float)( cx + w / 2 ), (float)( cy + h / 2 ) ), colorbrush );
}

void MikanLiteDraw::DrawTexture( unsigned int tex, int rx, int ry, int w, int h, int dx, int dy )
{
	D2D1_RECT_F draw, read;
	if ( texturemax <= tex || textures[ tex ].bitmap == NULL ) { return; }
	draw.left = (float)( dx );
	draw.right = (float)( dx + w );
	draw.top = (float)( dy );
	draw.bottom = (float)( dy + h );
	read.left = (float)( rx );
	read.right = (float)( rx + w );
	read.top = (float)( ry );
	read.bottom = (float)( ry + h );
	rendertarget->DrawBitmap( textures[ tex ].bitmap, draw, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, read );
}

void MikanLiteDraw::DrawTextureC( unsigned int tex, int rx, int ry, int w, int h, int dx, int dy )
{
	D2D1_RECT_F draw, read;
	if ( texturemax <= tex || textures[ tex ].bitmap == NULL ) { return; }
	draw.left = (float)( dx - w / 2 );
	draw.right = (float)( dx + w / 2 );
	draw.top = (float)( dy - h / 2 );
	draw.bottom = (float)( dy + h / 2 );
	read.left = (float)( rx );
	read.right = (float)( rx + w );
	read.top = (float)( ry );
	read.bottom = (float)( ry + h );
	rendertarget->DrawBitmap( textures[ tex ].bitmap, draw, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, read );
}
