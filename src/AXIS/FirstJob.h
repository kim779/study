#ifndef __FIRSTJOB_H__
#define __FIRSTJOB_H__

class CFirstJob
{
protected:
	void DeleteUserFile(const char* root, const char* fileName);
	void DeleteGarbageFiles();

	void IncreaseDate(CString& date, CString today);
	void DeleteFile(const char* fileName);
	CString MakeMapFileName(const char* fileName);
public:
	CFirstJob();
};

#endif // __FIRSTJOB_H__