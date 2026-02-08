// stdafx.cpp : source file that includes just the standard includes
//	cx_codectrl.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void Msg(char* fmt, ...)
{
#ifdef _DEBUG
	CString	tmpstr, str;

	va_list	valist;
	va_start(valist, fmt);
	tmpstr.FormatV(fmt, valist);

	str = "kjs -----" + tmpstr + "\n";
	//AfxMessageBox(tmpstr);
	//OutputDebugString(str);
	va_end(valist);
#endif
}


