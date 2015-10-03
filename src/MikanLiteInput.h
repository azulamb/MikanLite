#ifndef __MIKAN_LITE_INPUT
#define __MIKAN_LITE_INPUT

#include "Common.h"

struct MOUSEDATA
{
	POINT point;
	bool buf[ 3 ];
	int frame[ 3 ];
};

class MikanLiteInput :public MikanInput, InputManagement
{
private:
	// Mouse.
	struct MOUSEDATA mouse;
public:
	MikanLiteInput( void );
	virtual void Update( void );

	virtual void UpdateMouse( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	virtual void ReflectMouse();
	virtual int GetMouseX( void );
	virtual int GetMouseY( void );
	virtual void SetMousePosition( int x, int y );
	virtual int GetMouseFrame( unsigned int button );

	virtual void UpdateKeyboard( void );


};

#endif
