#ifndef __LOAD_DIRECTORY_FO_STFILE__HEADER__
#define __LOAD_DIRECTORY_FO_STFILE__HEADER__

// #include "LoadSTDir"
#include <direct.h>

enum STLevel
{
	STLevel_Basic=0,		// 초급
	STLevel_Ext,			// 고급
};
enum STType
{
	STType_System = 0,		// 제공되는 전략
	STType_User,			// 사용자가 만든전략
};

enum STTreeType
{
	STTreeType_Folder=0,
	STTreeType_Item,
};

typedef struct {
	int	nType;				// 0:Folder, 1:Item
} STTREEBASE_INFO;

typedef struct : public STTREEBASE_INFO
{
	int	nSTLevel;			// 0:초급, 1:고급
	int	nSTType;			// 제공전략, 사용자가 만든전략
	CString	szBaseFolder;	// 폴더의 위치
} STTREEFOLDER_INFO;

typedef struct  : public STTREEBASE_INFO
{
	CString	szRawName;				// .ST를 뺀 전략의 이름, @들어가 있을 수 있음 ex)@2.
	STTREEFOLDER_INFO*	pFolder;	//STTREEFOLDER_INFO정보의 포인트
} STTREEITEM_INFO;
	
class CLoadSTFile
{
public:
	// 트리에 ST파일을 로드한다.
	static void LoadST(LPCSTR aTarDir, LPCSTR pszExt, BOOL bSub, CTreeCtrl* pTree, HTREEITEM hItem)
	{
		char aFile[MAX_PATH];
		sprintf(aFile, "%s\\%s", aTarDir, pszExt);

		char aDelFile[MAX_PATH]={0,};
		CString strComponent;
		CFileFind finder;
		BOOL bWorking = finder.FindFile( aFile );
		int nFind=-1;
		while (bWorking){
			bWorking = finder.FindNextFile();
			strComponent = finder.GetFileName();

			if(!finder.IsDirectory())  
			{
				nFind = strComponent.Find("@");
				if(nFind>0)
				{
					strComponent = strComponent.Left(nFind);
					pTree->InsertItem(strComponent, hItem);
				}
			}
			else if(bSub && strComponent[0]!='.') {
				HTREEITEM hNewItem = pTree->InsertItem(strComponent, hItem);

				char aSubDir[MAX_PATH]={0,};
				sprintf(aSubDir, "%s\\%s", aTarDir, strComponent);
				LoadST(aSubDir, pszExt, bSub, pTree, hNewItem);
			}
		}
	}

	#define		SZSTBASICDIR		"\\STData\\STBasic"
	#define		SZSTEXT				".ST"
	#define		SZSTBASICEXT0		"@9"
	#define		SZSTBASICEXT		"@9."
	#define		SZSTBASICEXT2		"@9.ST"
	#define		SZSTBASICAPPNAME	"StrategyList"
	#define		SZUSERFOLDER		"사용자전략"
	static int GetUserSTFile(LPCSTR szRootDir, LPCSTR szSaveName, CString& rString)
	{
		rString.Empty();

		CString szUserDir;
		szUserDir.Format("%s%s", szRootDir , "\\User");

		rString.Format("%s%s\\%s\\%s%s", szUserDir, SZSTBASICDIR, SZUSERFOLDER, szSaveName, SZSTBASICEXT2);
		return rString.GetLength();
	}
	
	static int GetTempSTFile(LPCSTR szRootDir, LPCSTR szSaveName, CString& rString)
	{
		rString.Empty();

		rString.Format("%s%s%s%s", szRootDir, "\\User\\STData\\Temp\\", szSaveName, SZSTBASICEXT2);
		return rString.GetLength();
	}

	static int GetBasicDirInfo(LPCSTR szRootDir, CString& szFolder, CString& rString)
	{
		rString.Empty();
		if(szFolder.CompareNoCase(SZUSERFOLDER)==0)
		{
			rString.Format("%s%s%s", szRootDir, "\\User\\STData\\STBasic\\", szFolder);
		}
		else
		{
			rString.Format("%s%s%s", szRootDir, "\\Data\\STData\\STBasic\\", szFolder);
		}
		return rString.GetLength();
	}

	static int GetSTFile(LPCSTR szRootDir, LPCSTR szFolder, LPCSTR szSaveName, CString& rString)
	{
		rString.Empty();

		CString szUserDir;
		szUserDir.Format("%s%s", szRootDir , "\\Data");
		rString.Format("%s%s\\%s\\%s%s", szUserDir, SZSTBASICDIR, szFolder, szSaveName, SZSTBASICEXT2);
		return rString.GetLength();
	}

	// 주어진 디렉토리에서 폴더명 리스트를 얻어온다.
	static int GetSTFolderList(LPCSTR aTarDir, CStringArray &arr)
	{
		arr.RemoveAll();

		char aFilePath[MAX_PATH];
		sprintf(aFilePath, "%s\\%s", aTarDir, "*.*");

		CString strComponent;
		CFileFind finder;
		BOOL bWorking = finder.FindFile( aFilePath );
		while (bWorking){
			bWorking = finder.FindNextFile();
			strComponent = finder.GetFileName();

			if(finder.IsDirectory() && !finder.IsDots())
			{
				arr.Add(strComponent);
			}
		}
		return arr.GetSize();
	}

	// 주어진 디렉토리에서 초급전략에 ST파일 리스트를 얻어온다.
	static int GetSTFileList(LPCSTR aTarDir, CStringArray &arr)
	{
		arr.RemoveAll();
		
		char aFile[MAX_PATH];
		sprintf(aFile, "%s\\%s", aTarDir, "*.ST");
		char* aFilePath = _strupr(aFile);

		CString strComponent;
		int nFind;
		CFileFind finder;
		BOOL bWorking = finder.FindFile( aFilePath );
		while (bWorking){
			bWorking = finder.FindNextFile();
			strComponent = finder.GetFileName();

			if(!finder.IsDirectory())  
			{
				//nFind = strComponent.Find(SZSTBASICEXT);
				//if(nFind>0)
				//{
				//	strComponent = strComponent.Left(nFind);
				//	arr.Add(strComponent);
				//}
				//else
				//{
					nFind = strComponent.Find(SZSTEXT);
					if(nFind>0)
					{
						strComponent = strComponent.Left(nFind);
						arr.Add(strComponent);
					}
				//}
			}
		}
		return arr.GetSize();
	}

	// Exam : 종가,14
	// 종가와 14를 따로 저장한다
	// szData는 구분값이 ","임. 끝에 , 없음.
	static int SaveVarList(LPCSTR szSec, int nStart, CString szData, LPCSTR szSTFile)
	{
		int nFind=szData.Find(",");
		if(nFind<0) return 0;

		szData += ",";

		char aSub[MAX_PATH], aData[MAX_PATH];

		int nRtn=nStart;
		while(nFind>0)
		{
			sprintf(aSub, "Input%d", nRtn++);
			sprintf(aData, "%s", szData.Left(nFind));
			::WritePrivateProfileString(szSec, aSub, aData, szSTFile);
			szData = szData.Mid(nFind+1);
			nFind=szData.Find(",");
		}

		return nRtn-nStart;
	}

	// szData는 구분값이 ";"임. 끝에 ; 있음.
	// nType이 1이면 삼성에서 추가된 봉, 이내/이전/연속 처리를 위한 타입
	static int SaveInputList(LPCSTR szSec, int nStart, CString szData, int nType, LPCSTR szSTFile)
	{
		int nFind=szData.Find(";");
		if(nFind<0) return 0;

		char aSub[MAX_PATH], aData[MAX_PATH];
		CString szUseExt;

		CStringList szExtDataList;
		int nRtn=nStart;		
		while(nFind>0)
		{
			sprintf(aSub, "Input%d", nRtn++);
			sprintf(aData, "%s", szData.Left(nFind));
			szExtDataList.AddTail(aData);
			if(nType==1)
			{
				szUseExt += aData; szUseExt += ";";
			}
			::WritePrivateProfileString(szSec, aSub, aData, szSTFile);
			szData = szData.Mid(nFind+1);
			nFind=szData.Find(";");
		}
		if(nType==1)
		{
			//::WritePrivateProfileString(szSec, "UseExtData", szUseExt, szSTFile);

			int nExtCnt = szExtDataList.GetCount();
			if(nExtCnt>=2)
			{
				POSITION pos2=szExtDataList.GetTailPosition();
				POSITION pos1 = pos2;
				szExtDataList.GetPrev(pos1);

				szUseExt.Format("%s;%s;",	szExtDataList.GetAt(pos1),
											szExtDataList.GetAt(pos2) );
				::WritePrivateProfileString(szSec, "UseExtData", szUseExt, szSTFile);
			}
		}

		return nRtn-nStart;
	}

	// 삼성에서는 버전처리시에 같은 이름이 있어서는 안된다고 해서 만든 해결책.
	// 제공되는 초급전략명에 "STB_" 라는 이름을 붙여서 버전처리하고 
	// 실제사용할 때는 "STB_"를 삭제하고 사용한다.
	#define		SSANGSUNG_PRE			"STB_"
	#define		SSANGSUNG_PRESIZE		4
	static void Convert2BasicST(LPCSTR aTarDir, LPCSTR pszExt, BOOL bSub=TRUE)
	{
		char aFile[MAX_PATH];
 		sprintf(aFile, "%s\\%s", aTarDir, pszExt);

		char aDelFile[MAX_PATH]={0,};
		CString strComponent, szNewStr;
		CString szMTar, szMSrc;
		CFileFind finder;
		BOOL bWorking = finder.FindFile( aFile );
		int nFind=-1;
		while (bWorking){
			bWorking = finder.FindNextFile();
			strComponent = finder.GetFileName();

			if(!finder.IsDirectory())  
			{
				strComponent.MakeUpper();
				if(strComponent.GetLength()>3 && strComponent.Find(SSANGSUNG_PRE)==0)
				{
					szNewStr = strComponent.Mid(SSANGSUNG_PRESIZE);
					szMSrc.Format("%s\\%s", finder.GetRoot(), strComponent);
					szMTar.Format("%s\\%s", finder.GetRoot(), szNewStr);
					// MoveFile(strComponent, szNewStr);
					MoveFile(szMSrc, szMTar);
				}
			}
			else if(bSub && strComponent[0]!='.') {
				char aSubDir[MAX_PATH]={0,};
				sprintf(aSubDir, "%s\\%s", aTarDir, strComponent);
				Convert2BasicST(aSubDir, pszExt, bSub);
			}
		}
	}

};

#endif // __LOAD_DIRECTORY_FO_STFILE__HEADER__

