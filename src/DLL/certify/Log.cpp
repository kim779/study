// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "Certify.h"
#include "Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog(char *logName)
{
	m_logDs = nullptr;
	m_fName = CString(logName);
}

CLog::~CLog()
{

}

void CLog::Trace(char *msg, int len, dirLog dir)
{
	errno_t	err;

	err = fopen_s(&m_logDs, (char*)m_fName.operator LPCTSTR(), (m_logDs == NULL) ? "w+" : "a+");
	if (err != 0)
		return;

	SYSTEMTIME	stime;
	GetLocalTime(&stime);
	switch (dir)
	{
	case dirSTR:
		fprintf(m_logDs, "[%s] [%02d:%02d:%02d.%03d]\n", msg,
				stime.wHour, stime.wMinute, stime.wSecond,
				stime.wMilliseconds);
		break;
	case dirRCV:
	case dirSND:
		fprintf(m_logDs, "[%c] [%02d:%02d:%02d.%03d]\n",
				dir == dirRCV ? 'R' : 'S',
				stime.wHour, stime.wMinute, stime.wSecond,
				stime.wMilliseconds);
		Parse((BYTE *) msg, len);
	}

	fprintf(m_logDs, "\n");
	fclose(m_logDs);
}

void CLog::Parse(BYTE *msg, int len)
{
	int ii;
	for (ii = 0; ii < len; )
	{
		fprintf(m_logDs, "%02x ", msg[ii++]);
		if (ii % 20)
		{
			continue;
		}
		for (int jj = ii - 20; jj < ii; jj++)
		{
			fprintf(m_logDs, "%c", msg[jj] < ' ' ? '.' : msg[jj]);
		}
		fprintf(m_logDs, "\n");
	}
	int jj = ii % 20;
	if (jj)
	{
		for (; jj < 20; jj++)
		{
			fprintf(m_logDs, "   ");
		}
		for (jj = ii - (ii % 20); jj < ii; jj++)
		{
			fprintf(m_logDs, "%c", msg[jj] < ' ' ? '.' : msg[jj]);
		}
		fprintf(m_logDs, "\n");
	}
}

