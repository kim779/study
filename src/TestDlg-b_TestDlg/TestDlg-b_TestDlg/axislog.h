#pragma once
#include <vadefs.h>
#include <stdarg.h>
#include <cstring>

//m_slog.Format("m_arGraphQue size=[%d] bIncrease=[%d]", m_arGraphQue.GetCount(), bIncrease);
//LOG_OUTP(3, "axisgmain", __FUNCTION__, m_slog);
static void LOG_OUTP(int scnt, ...)
{
	va_list argList;
	va_start(argList, scnt);

	CString sTmp, sResult;

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

	OutputDebugString("\r\n" + sResult + "\r\n");
}

static void FileLog(LPCSTR log, CString spath = "", ...)
{
#if 1
	TRY
	{
		char buf[500]{};
		GetModuleFileName(nullptr, buf, 260);

		CString spath, slog;
		spath.Format("%s", buf);
		spath.TrimRight();
		if (spath.IsEmpty())
		{
			spath.Replace("axis.exe", "axis.log");
			spath.Replace("AXIS.EXE", "axis.log");
		}
		else
		{
			spath.Replace("axis.exe", spath);
			spath.Replace("AXIS.EXE", spath);
		}


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