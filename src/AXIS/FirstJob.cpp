// FirstJob.cpp
//

#include "stdafx.h"
#include "FirstJob.h"

CFirstJob::CFirstJob()
{
	PatchModules();
}

bool CFirstJob::FileExists(const CString& path)
{
	return GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES;
}

bool CFirstJob::CopyFileSafe(const CString& src, const CString& dst)
{
	return ::CopyFile(src, dst, FALSE) != FALSE;
}

// Controlled by {home}\tab\devpatch.ini
//   [PATCH]
//   activate=original  ; trans=apply patch, original=restore original
//   state=original     ; managed by this code, do not edit manually
//
//   [MODULES]
//   IB202700.dll=IB202200.dll
//   cx_interest.dll=cx_interest.dll
void CFirstJob::PatchModules()
{
    CString iniPath;
    iniPath.Format("%s\\%s\\devpatch.ini", Axis::home, TABDIR);

    if (!FileExists(iniPath))
        return;

    char buf[16] = { 0 };
    GetPrivateProfileString("PATCH", "activate", "original", buf, sizeof(buf), iniPath);
    const CString activate(buf);

    GetPrivateProfileString("PATCH", "state", "original", buf, sizeof(buf), iniPath);
    const CString state(buf);

    if (activate == state)
        return;

    char sectionBuf[4096] = { 0 };
    GetPrivateProfileSection("MODULES", sectionBuf, sizeof(sectionBuf), iniPath);

    bool allOk = true;
    for (const char* p = sectionBuf; *p; p += strlen(p) + 1)
    {
        CString entry(p);
        const int sep = entry.Find('=');
        if (sep <= 0)
            continue;

        CString dstPath = entry.Left(sep);
        CString srcPath = entry.Mid(sep + 1);
        dstPath.TrimRight(); srcPath.TrimRight();
        if (dstPath.IsEmpty() || srcPath.IsEmpty())
            continue;

        CString dstFile, srcFile, bakFile;
        dstFile.Format("%s\\%s", Axis::home, dstPath);
        srcFile.Format("%s\\%s", Axis::home, srcPath);
        bakFile.Format("%s\\%s.org", Axis::home, dstPath);

        if (activate == "trans")  //변환
        {
            if (!FileExists(srcFile))
            {
                allOk = false;
                continue;
            }
            if (FileExists(dstFile) && !CopyFileSafe(dstFile, bakFile))
            {
                allOk = false;
                continue;
            }
            if (!CopyFileSafe(srcFile, dstFile))
                allOk = false;
        }
        else   //복원
        {
            if (!FileExists(bakFile))
                continue;
            if (!CopyFileSafe(bakFile, dstFile))
                allOk = false;
        }
    }

    if (allOk)
        WritePrivateProfileString("PATCH", "state", activate, iniPath);
}
