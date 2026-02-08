#if !defined(AFX_AZIO_UTIL_FILMANAGER_BY_JUNOK_LEE__INCLUDED_)
#define AFX_AZIO_UTIL_FILMANAGER_BY_JUNOK_LEE__INCLUDED_

// #include "../../inc/AZUtilFileMng.h"
#include <io.h>				// for _chmod() function supports
#include <sys/stat.h>		// for _chmod() function supports
#include <DIRECT.H>

class CAZUtil_FileManager
{
public:
	static int	DoDirFileRemove(LPCSTR pszDir, LPCSTR pszExt, BOOL bSub)
	{
		char aFile[MAX_PATH];
		sprintf(aFile, "%s\\%s", pszDir, pszExt);
		
		char aDelFile[MAX_PATH];
		CString strComponent;
		CFileFind finder;
		BOOL bWorking = finder.FindFile( aFile );
		while (bWorking){
			bWorking = finder.FindNextFile();
			strComponent = finder.GetFileName();
			
			if(!finder.IsDirectory())  {
				sprintf(aDelFile, "%s\\%s", pszDir, strComponent);
				DoFileRemove( aDelFile);
				//TRACE("FileRemove =>  %s\n", aDelFile);
			}
			else if(bSub && strComponent[0]!='.') 
			{
				// 디렉토리일 경우 재귀호출을 콜한다.
				char aSubDir[MAX_PATH];
				sprintf(aSubDir, "%s\\%s", pszDir, strComponent);
				DoDirFileRemove(aSubDir, pszExt, bSub);
			}
		}
		return 0;
	}

	static int	DoDirectoryMove(LPCSTR pszFromDir, LPCSTR pszToDir, LPCSTR pszExt, BOOL bSub, BOOL bDirRemove)
	{
		DoMakeDirectory(pszToDir);

		char aFile[MAX_PATH];
		sprintf(aFile, "%s\\%s", pszFromDir, pszExt);

		char aCopyFileFrom[MAX_PATH], aCopyFileTo[MAX_PATH];
		BOOL bRet=FALSE	;

		CString strComponent;
		CFileFind finder;
		BOOL bWorking = finder.FindFile( aFile );
		while (bWorking){
			bWorking = finder.FindNextFile();
			strComponent = finder.GetFileName();
			
			if(!finder.IsDirectory())  {
				sprintf(aCopyFileFrom,	"%s\\%s", pszFromDir,	strComponent);
				sprintf(aCopyFileTo,	"%s\\%s", pszToDir,		strComponent);

				DoFileMove( aCopyFileFrom, aCopyFileTo);
			}
			else if(bSub && strComponent[0]!='.') {
				// RecurceiveCall
				char aSubFileFrom[MAX_PATH], aSubFileTo[MAX_PATH];
				sprintf(aSubFileFrom, "%s\\%s", pszFromDir, strComponent);
				sprintf(aSubFileTo, "%s\\%s", pszToDir, strComponent);

				//DoMakeDirectory(aSubFileTo);
				DoDirectoryMove(aSubFileFrom, aSubFileTo, pszExt, bSub, bDirRemove);
			}
		}

		if(bDirRemove)
		{
			bRet = DoRemoveDirectory(pszFromDir);
		}

		return 0;
	}

	//20060920S02001-090401 alzioyes
	static int	DoDirectoryDelete(LPCSTR pszTarDir)
	{
		LPCSTR pszExt = "*.*";
		char aFile[MAX_PATH], aCopyFileFrom[MAX_PATH];
		sprintf(aFile, "%s\\%s", pszTarDir, pszExt);

		CString strComponent;
		CFileFind finder;
		BOOL bWorking = finder.FindFile( aFile );
		while (bWorking){
			bWorking = finder.FindNextFile();
			strComponent = finder.GetFileName();

			if(!finder.IsDirectory())  {
				sprintf(aCopyFileFrom,	"%s\\%s", pszTarDir, strComponent);
				DoFileRemove( aCopyFileFrom);
			}
			else if(strComponent[0]!='.') {
				// RecurceiveCall
				char aSubDir[MAX_PATH];
				sprintf(aSubDir, "%s\\%s", pszTarDir, strComponent);
				DoDirectoryDelete(aSubDir);
			}
		}

		DoRemoveDirectory(pszTarDir);

		return 0;
	}

	static BOOL DoMakeDirectory(LPCSTR pszSrc)
	{
		return(mkdir(pszSrc)==0);
	}

	static BOOL DoRemoveDirectory(LPCSTR pszSrc)
	{
		{
			CString szDebug;
			szDebug.Format("%s[%s]", "DoRemoveDirectory", pszSrc);
			//OutputDebugString(szDebug);
		}

//		CString szMoveDir;
//		szMoveDir.Format("\"%s\0\"", pszSrc);
//		//szMoveDir+='\0';
		_chmod(pszSrc, _S_IWRITE);
		return ::RemoveDirectory(pszSrc);		
		//return(rmdir(pszSrc)==0);
	}

	static BOOL DoFileMove(LPSTR pszSrc, LPSTR pszDes)
	{
		_chmod(pszSrc, _S_IWRITE);
		_chmod(pszDes, _S_IWRITE);

		if(::CopyFile(pszSrc, pszDes, FALSE))
			return ::DeleteFile(pszSrc);

		return FALSE;
	}

	static BOOL DoFileRemove(LPCSTR pszSrc)
	{
		if(_chmod(pszSrc, _S_IWRITE)==-1) 
		{
			TRACE("[%s]  Attirbute Changing is Failed.\n", pszSrc);
		}

		BOOL bRtn = ::DeleteFile(pszSrc);
		return bRtn;
	}

	static BOOL DoCheckFileExist(LPCSTR pszSrc)
	{
		char szUserPath[MAX_PATH] = {0, };
		sprintf(szUserPath, "%s\\*.*", pszSrc);

		CString strUser;

		CFileFind ff;
		if(ff.FindFile(szUserPath))
		{
			BOOL bFind = TRUE;
			while(bFind)
			{
				bFind = ff.FindNextFile();
				if(ff.IsDirectory())
				{
					strUser = ff.GetFileName();
					if(strUser == "." || strUser == ".." || strUser.GetLength() < 2)
					continue;

					return TRUE;
				}
			}		
		}
		return FALSE;
	}

	static int	DoDirFileCopy(LPCSTR pszSDir, LPCSTR pszTDir, LPCSTR pszExt, BOOL bSub)
	{
		char aFile[MAX_PATH];
		sprintf(aFile, "%s\\%s", pszSDir, pszExt);
		
		char aFromFile[MAX_PATH], aToFile[MAX_PATH];
		CString strComponent;
		CFileFind finder;
		BOOL bWorking = finder.FindFile( aFile );
		while (bWorking){
			bWorking = finder.FindNextFile();
			strComponent = finder.GetFileName();
			
			if(!finder.IsDirectory())  {
				sprintf(aFromFile, "%s\\%s", pszSDir, strComponent);
				sprintf(aToFile, "%s\\%s",  pszTDir, strComponent);
				DoFileCopy(aFromFile, aToFile);
				//TRACE("FileRemove =>  %s\n", aDelFile);
			}
			else if(bSub && strComponent[0]!='.') 
			{
				// 디렉토리일 경우 재귀호출을 콜한다.
				char aSubFromDir[MAX_PATH], aSubToDir[MAX_PATH];
				sprintf(aSubFromDir, "%s\\%s", pszSDir, strComponent);
				sprintf(aSubToDir, "%s\\%s", pszTDir, strComponent);
				DoDirFileCopy(aSubFromDir, aSubToDir, pszExt, bSub);
			}
		}
		return 0;
	}

	static BOOL DoFileCopy(LPCSTR pszSFile, LPCSTR pszTFile)
	{
		//DoCheckFileExist(pszSFile);
		BOOL bRtn = ::CopyFile(pszSFile, pszTFile, FALSE);
		return bRtn;
	}
};

#endif //AFX_AZIO_UTIL_FILMANAGER_BY_JUNOK_LEE__INCLUDED_
