// FirstJob.cpp
//

#include "stdafx.h"
#include "FirstJob.h"

CFirstJob::CFirstJob()
{
	//** delete usertool.ini in user area
	
	CProfile profile(pkAxis);
	const BOOL bFirst = profile.GetInt("Stamp", "InitUserTool", 1);

	if (bFirst)
	{
		DeleteUserFile(Format("%s\\%s", Axis::home, "user"), "usertool.ini");
		profile.Write("Stamp", "InitUserTool", 0);
	}

	DeleteGarbageFiles();
}


void CFirstJob::DeleteUserFile(const char* root, const char* fileName)
{
	CString folder(root);

	if (folder.IsEmpty())
		return;
	
	if (folder[folder.GetLength() - 1] != '\\')
		folder += '\\';

	CFileFind ff;
	BOOL exist = FALSE;

	if (ff.FindFile(folder + "*.*"))
	{
		while (ff.FindNextFile())
		{
			if (ff.IsDirectory() && !ff.IsDots())
			{
				DeleteUserFile(ff.GetFilePath(), fileName);
			}
			else if (!ff.IsDirectory()) 
			{
				if (ff.GetFileName().CompareNoCase(fileName) == 0)
					exist = TRUE;
			}
		}
		if (ff.IsDirectory() && !ff.IsDots())
		{
			DeleteUserFile(ff.GetFilePath(), fileName);
		}
		else if (!ff.IsDirectory()) 
		{
			if (ff.GetFileName().CompareNoCase(fileName) == 0)
				exist = TRUE;
		}

		ff.Close();
	}

	if (exist)
	{
		TRY
		{
			CFile::Remove(folder + fileName);
		}
		CATCH(CFileException, e)
		{
		}
		END_CATCH;
	}
}

void CFirstJob::IncreaseDate(CString& date, CString today)
{
	int year{}, month{}, day{};
	
	if (date.GetLength() != 10)
	{
		date = today;
		return;
	}
	year = atoi(date.Mid(0, 4));
	month = atoi(date.Mid(5, 2));
	day = atoi(date.Mid(8, 2));

	if (year * month * day == 0)
	{
		date = today;
		return;
	}

	CTime tm(year, month, day, 0, 0, 0);

	const CTimeSpan ts(1, 0, 0, 0);
	tm += ts;

	date = tm.Format("%Y.%m.%d");
}

void CFirstJob::DeleteFile(const char* fileName)
{
	CString file(fileName), path;
	CString pathSign;

	const int find = file.Find('/');
	if (find > 0)
	{
		pathSign = file.Left(find);
		file.Delete(0, find + 1);
	}
	else
		return;

	pathSign.MakeUpper();

	if (pathSign == "MAP")
		path = MakeMapFileName(file);
	else
		path.Format("%s\\%s\\%s", Axis::home, pathSign, file);

	if (path.IsEmpty())
		return;

	TRY 
	{
		TRACE("Deleting file = %s\n", path);
		CFile::Remove(path);
	}
	CATCH (CFileException, e)
	{
		
	}
	END_CATCH;		
}

CString CFirstJob::MakeMapFileName(const char* fileName)
{
	CString file(fileName);
	file.MakeUpper();

	if (file.Left(2) != "IB")
		return "";

	return Format("%s\\MAP\\%s\\%s\\%s", Axis::home, "IB", file.Left(3), file);
}

void CFirstJob::DeleteGarbageFiles()
{
	CProfile profile(pkUserConfig);
	CString lastDate = profile.GetString("FIRSTJOB", "FileDeleteDate", "2008.06.12");
	CProfile delfile(Format("%s\\%s\\deletefile.ini", Axis::home, TABDIR));
	CString today(CTime::GetCurrentTime().Format("%Y.%m.%d"));
	CString lastUpdate = delfile.GetString("SET", "LastDate", today);
	CString buffer, line;
	int find{};

	while (lastDate < lastUpdate)
	{
		IncreaseDate(lastDate, today);

		buffer = delfile.GetSectionLF(lastDate);

		while (!buffer.IsEmpty())
		{
			find = buffer.Find('\n');
			if (find < 0)
			{
				line = buffer;
				buffer.Empty();
			}
			else
			{
				line = buffer.Left(find);
				buffer.Delete(0, find + 1);
			}

			line = buffer.Left(find);

			line.Replace("\r", "");
			line.Replace("\n", "");

			Trim(line);

			if (!line.IsEmpty())
				DeleteFile(line);
		}
	}

	profile.Write("FIRSTJOB", "FileDeleteDate", today);
}

