#include "stdafx.h"

#include "clog.h"

CLog g_log;

CLog::CLog()
{
	file.Open("C:\\temp.txt",CFile::modeWrite|CFile::modeCreate|CFile::shareDenyNone);
}

CLog::~CLog()
{
	file.Close();
}

void CLog::write(int ii)
{
	file.Seek(0,CFile::end);

	CString str = GetTime();
	
	CString temp;
	temp.Format("%s (%d)\n", str, ii);

	file.WriteString(temp);
}

void CLog::write(CString strIn, int ii)
{
	file.Seek(0,CFile::end);

	CString str = GetTime();
	
	CString temp;
	temp.Format("%s (%s %d)\n", str, strIn, ii);

	file.WriteString(temp);

}

CString CLog::GetTime()
{
	CString str;
	CONST	CTime t = CTime::GetCurrentTime();

	str.Format("[%02d-%02d %02d:%02d:%02d] ", t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	return str;
}

void CLog::write(CString str)
{
	file.Seek(0,CFile::end);

	CString strTime = GetTime();
	
	CString temp;
	temp.Format("%s (%s)\n", strTime, str);

	file.WriteString(temp);
	
}