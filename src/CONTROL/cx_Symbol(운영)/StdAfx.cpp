// stdafx.cpp : source file that includes just the standard includes
//	CX_Symbol.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"



#ifdef _DEBUG_PRINT_

#include <sys/timeb.h>

void _add_log(CString str)
{
	struct tm* t;
	struct _timeb tb;

	_ftime( &tb );
	t = localtime( &tb.time );

	CString strFile = "cx_symbol.txt";

	CFileFind find;
	find.FindFile( strFile );

	CTime t1, t2;
	find.GetLastWriteTime( t1 );

	t2 = CTime::GetCurrentTime();

	CFile file;
	if (t1.GetDay() != t2.GetDay() ) 
	{
		if (!file.Open( strFile, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate ) )
			return;
	}
	else
	{
		if (!file.Open( strFile, CFile::modeCreate|CFile::modeWrite ) )
			return;
	}
	

	CString strTemp;
	strTemp.Format("[%02d/%02d %02d:%02d:%02d %03d] %s\n", 
		t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tb.millitm, str );	

	file.Seek(0,CFile::end);
	file.Write( strTemp, strTemp.GetLength() );

	file.Close();
}

#endif