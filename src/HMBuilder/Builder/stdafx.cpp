
// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// axisMBuilder.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.


#include "stdafx.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;


void AFXAPI Msg(char* fmt, ...)
{
#ifdef _DEBUG

//	ofstream dFile(_T("c:\\Debug.log"), std::ios::app);
	CString	tmpstr, str;

	va_list	valist;
	va_start(valist, fmt);
	tmpstr.FormatV(fmt, valist);
	va_end(valist);
	
	if (tmpstr == "")
		str = "***************************************************************";
	else
	{
		CTime time = CTime::GetCurrentTime();
		CString sTime;

		sTime.Format("[%02d%02d-%02d:%02d:%02d] ", time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		str = "<Debug>" + sTime + tmpstr;
	}

	OutputDebugString(str + "\n");
//	dFile << str << endl;
//	dFile.close();

#endif
}

CString AFXAPI FORMAT(const char* formatString, ...)
{
	CString	s;

	va_list	valist;
	va_start(valist, formatString);
	s.FormatV(formatString, valist);
	va_end(valist);
	TRIM(s);
	return s;
}

int Split(std::vector<std::string> &elems, const std::string &s, char delim)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
		elems.push_back(item);
    
	return elems.size();
}
