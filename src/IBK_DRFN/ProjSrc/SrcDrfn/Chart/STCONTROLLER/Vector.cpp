// Vector.cpp: implementation of the CVector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void raise_exception(int _L, const char* _F, ...)
{
	int bytes;
	char what[256];

	va_list argptr;

	va_start(argptr, _F);
	bytes = sprintf(what, "Calc Exception(L=%d): ", _L);
	vsprintf(&what[bytes], _F, argptr);
	va_end(argptr);

	throw std::exception(what);
}
