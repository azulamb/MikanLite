#ifndef __MIKAN_LITE_COMMON_LIB
#define __MIKAN_LITE_COMMON_LIB

#include "../MikanLite.h"

#define SafeRelease(x) if(x){x->Release(); x=NULL;}

wchar_t *Wchar2Char( const char *str );

class DrawManagement
{
public:
	virtual void Begin( void ) = 0;
	virtual long End( void ) = 0;
};

class InputManagement
{
public:
	virtual void Update() = 0;
};

#endif
