#ifndef _C_LOG_H_
#define _C_LOG_H_

#include "stdafx.h"

class CLog 
{
public:
	CLog();
	~CLog();

	void write(int ii);
	void write(CString strIn, int ii);
	void write(CString str);

protected:

	CString GetTime(void);

	CStdioFile file;
};

extern CLog g_log;

#endif