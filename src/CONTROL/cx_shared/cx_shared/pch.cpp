// pch.cpp: 미리 컴파일된 헤더에 해당하는 소스 파일

#include "pch.h"

// 미리 컴파일된 헤더를 사용하는 경우 컴파일이 성공하려면 이 소스 파일이 필요합니다.
void Write_Log(LPCSTR log, ...)
{
	return;
#if 1
	TRY
	{
		CString slog;
		slog.Format("[AXIS][WriteLog] [%s]\n", log);
		//OutputDebugString(slog);

		char chfile[500]{};
		GetModuleFileName(nullptr, chfile, 260);

		CString spath, stmp;
		spath.Format("%s", chfile);
		spath.TrimRight();
		spath.Replace("axis.exe", "");

		FILE* fp;
		fopen_s(&fp, spath + "\\axis.log", "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

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