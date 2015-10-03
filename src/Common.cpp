#include "Common.h"

#include <locale.h>
#include <stdlib.h>

wchar_t *Wchar2Char( const char *str )
{
	size_t len = 0;
	wchar_t *wstr;

	setlocale( LC_ALL, "japanese" );

	mbstowcs_s( &len, NULL, 0, str, 0 );
	++len;

	wstr = (wchar_t *)malloc( sizeof( wchar_t ) * len );

	mbstowcs_s( &len, wstr, len, str, _TRUNCATE );

	return wstr;
}
