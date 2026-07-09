// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Wizard.h"
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
	m_logDs = NULL;
	sprintf(m_fName, "%s", logName);

//	if (::GetFileAttributes(m_fName) != 0xFFFFFFFF)
//	{
//		m_logDs = fopen(m_fName, "a+");
//		fclose(m_logDs);
//	}
}

CLog::~CLog()
{

}

void CLog::Trace(char *msg, int len, dirLog dir)
{
	m_logDs = fopen(m_fName, (m_logDs == NULL) ? "w+" : "a+");
	if (m_logDs == NULL)
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
	for (int ii = 0; ii < len; )
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

