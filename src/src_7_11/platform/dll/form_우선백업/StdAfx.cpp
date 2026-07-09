// stdafx.cpp : source file that includes just the standard includes
//	axisform10.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void AFXAPI Msg(char* fmt, ...)
{
	CString	tmpstr, str;

	va_list	valist;
	va_start(valist, fmt);
	tmpstr.FormatV(fmt, valist);
	va_end(valist);

	if (tmpstr == "")
		str = "********************************************************************************************************";
	else
	{
		CTime time = CTime::GetCurrentTime();
		CString sTime;

		sTime.Format("[%02d%02d-%02d:%02d:%02d] ", time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		str = "<Debug>" + sTime + tmpstr;
	}

	OutputDebugString(str + "\n");
	/*
	ofstream dFile(_T("c:\\Debug.log"), std::ios::app);
	dFile << str << endl;
	dFile.close();
	*/
}