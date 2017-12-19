#pragma once
#if ( _MSC_VER >= 1700 )
#	if defined( _DEBUG )
#		pragma comment(lib, "debug/zlibstat.lib")
#	else
#		pragma comment(lib, "release/zlibstat.lib")
#	endif
#endif
#ifndef ZLIB_WINAPI
#	define ZLIB_WINAPI
#endif
#include <zlib.h>