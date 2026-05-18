#pragma once
#include <vadefs.h>
#include <stdarg.h>
#include <cstring>



static CString Int2CString(CString stmp, int ival)
{
	CString slog;
	slog.Format("[%s = %d]", stmp, ival);
	return slog;
}

static void LOG_OUTP(int scnt, ...)
{
	va_list argList;
	va_start(argList, scnt);

	CString sTmp, sResult;

	CTime time;
	time = CTime::GetCurrentTime();
	sResult.Format("%02d:%02d:%02d  ", time.GetHour(), time.GetMinute(), time.GetSecond());

	for (int i = 0; i < scnt; i++)
	{
		if (i == 0)
			sTmp.Format("<%s>", va_arg(argList, LPCTSTR));
		else
			sTmp.Format("[%s]", va_arg(argList, LPCTSTR));

		sResult += sTmp;
		if (i == 0)
			sResult += "   ";
		else
			sResult += " ";
	}
	va_end(argList);

	OutputDebugString("\r\n" + sResult);
}

static void FileLog(LPCSTR log, CString spath="", ...)
{
#if 1
	TRY
	{
		char buf[500]{};
		GetModuleFileName(nullptr, buf, 260);

		CString spath, slog;
		spath.Format("%s", buf);
		spath.TrimRight();

		
		spath.Replace("axis.exe", "axis.log");
		spath.Replace("AXIS.EXE", "axis.log");
	
		FILE* fp;
		fopen_s(&fp, spath, "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

		slog.Format("[FileLog] [%s]", log);
		OutputDebugString(slog);

		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");

		fclose(fp);
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH
#endif
}
