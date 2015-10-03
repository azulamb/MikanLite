#ifndef __MIKAN_LITE_INPUT
#define __MIKAN_LITE_INPUT

#include "Common.h"

struct MOUSEDATA
{
	POINT point;
	bool buf[ 3 ];
	int frame[ 3 ];
};

struct KEYDATA
{
	unsigned char last;
	bool buf[ 0xFF ];
	int frame[ 0xFF ];
};

class MikanLiteInput :public MikanInput, InputManagement
{
private:
	// Mouse.
	struct MOUSEDATA mouse;
	struct KEYDATA key;
public:
	MikanLiteInput( void );
	virtual void Update( void );

	virtual void UpdateMouse( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void ReflectMouse( void );
	virtual int GetMouseX( void );
	virtual int GetMouseY( void );
	virtual void SetMousePosition( int x, int y );
	virtual int GetMouseFrame( unsigned int button );

	virtual void UpdateKeyboard( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void InitKeyboard( void );
	virtual void ReflectKeyboard( void );
	virtual int GetKeyFrame( unsigned char keycode );
	virtual unsigned char GetWhichKey( void );

};

#endif
