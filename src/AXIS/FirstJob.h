#ifndef __FIRSTJOB_H__
#define __FIRSTJOB_H__

class CFirstJob
{
	void PatchModules();
	static bool FileExists(const CString& path);
	static bool CopyFileSafe(const CString& src, const CString& dst);
public:
	CFirstJob();
};

#endif // __FIRSTJOB_H__
