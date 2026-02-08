// _STLoadManager.cpp : implementation file
//

#include "stdafx.h"
#include "_STLoadManager.h"
#include "../Common_ST/LoadSTDir.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//{{ ST Macro
#define	STDATA_INDEX		0
#define	STUSER_INDEX		1

#define	STRATEGY_INDEX		0x0001
#define	STRATEGY_ORDER		0x0002
#define	STRATEGY_BULL		0x0004
#define	STRATEGY_RESERVE1	0x0008
#define	STRATEGY_RESERVE2	0x0010

#define IDC_STListCtrl		1001
//{{ ST Macro

#define		gSTGubunKey			'^'		//'@'
#define		gSTGubunKeys		"^"		//"@"


// CSTLoadManager
CSTLoadManager::CSTLoadManager()
{
	m_pOutBarCtrl = NULL;
	m_bSubFolders = TRUE;
	m_pILSTIcon = NULL;
	m_dwSTTypeToLoad = NULL;
	m_uSTIndx = 0;
	m_bSTUserLoading = FALSE;
	m_uSTUserCount = 0;

	m_parSTPath = NULL;
	m_bUseBoldFont = TRUE;
}

void CSTLoadManager::Init(BOOL bSubFolder, CString szExtName, CImageList* pILSTIcon)
{
	m_bSubFolders = bSubFolder;
	SetExtName(szExtName);
	m_pILSTIcon = pILSTIcon;
}

void CSTLoadManager::LoadST2(CWnd* pOutBarCtrl, CStringArray* parSTPath, DWORD dwSTTypeToLoad, int nDefaultSelFolder, int p_LoadOption)
{
	if(p_LoadOption==0)
	{
		LoadST(pOutBarCtrl, parSTPath, dwSTTypeToLoad, nDefaultSelFolder);
		return;
	}

	// 아웃룩바 및 파일경로가 유효한가
	if(pOutBarCtrl && pOutBarCtrl->GetSafeHwnd() && parSTPath && parSTPath->GetSize() > 0) {
		m_pOutBarCtrl = (CSTGfxOutBarCtrl*)pOutBarCtrl;
		m_parSTPath = parSTPath;
		m_dwSTTypeToLoad = dwSTTypeToLoad;

		// STIndex.dat파일에서 읽음
		if(dwSTTypeToLoad == 0)
		{
			LoadSTListFromSTIndex(p_LoadOption);
		}
		// 디렉토리에서 읽음
		else
		{
			LoadSTList();
		}

		if(m_pOutBarCtrl->GetFolderCount()>0) 
			m_pOutBarCtrl->SetSelFolder(nDefaultSelFolder);
	}
}

void CSTLoadManager::LoadST(CWnd* pOutBarCtrl, CStringArray* parSTPath, DWORD dwSTTypeToLoad, int nDefaultSelFolder)
{
	// 아웃룩바 및 파일경로가 유효한가
	if(pOutBarCtrl && pOutBarCtrl->GetSafeHwnd() && parSTPath && parSTPath->GetSize() > 0) {
		m_pOutBarCtrl = (CSTGfxOutBarCtrl*)pOutBarCtrl;
		m_parSTPath = parSTPath;
		m_dwSTTypeToLoad = dwSTTypeToLoad;

		// STIndex.dat파일에서 읽음
		if(dwSTTypeToLoad == 0)
		{
			int p_LoadOption = 0;
			LoadSTListFromSTIndex(p_LoadOption);
		}
		// 디렉토리에서 읽음
		else
		{
			LoadSTList();
		}

		if(m_pOutBarCtrl->GetFolderCount()>0) 
			m_pOutBarCtrl->SetSelFolder(nDefaultSelFolder);
	}
}

BOOL CSTLoadManager::UnloadST()
{
	if(m_pOutBarCtrl && m_pOutBarCtrl->GetSafeHwnd()) {
		int nFolderCount = m_pOutBarCtrl->GetFolderCount();

		// 전략폴더별 ListCtrl 제거
		CSTListCtrl* pSTListCtrl = NULL;
		int nItemCount = 0;
		for(int nIndx = 0; nIndx < nFolderCount; ++nIndx)
		{
			if(pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nIndx)) {
				delete pSTListCtrl;
				pSTListCtrl = NULL;
			}
		}

		// 전략폴더 제거
		m_pOutBarCtrl->DeleteAllFolder();
	}
	return TRUE;
}

int CSTLoadManager::SearchST(CString szSTName, CStringArray& arFindIndx) 
{
	arFindIndx.RemoveAll();
	int nSTCount = 0;
	szSTName.MakeUpper();
	if(m_pOutBarCtrl && m_pOutBarCtrl->GetSafeHwnd()) {

		CSTListCtrl* pSTListCtrl = NULL;
		int nFolderCount = m_pOutBarCtrl->GetFolderCount();
		for(int nIndx = 0; nIndx < nFolderCount; ++nIndx)
		{
			if(pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nIndx)) {

				CString szItemName;
				CString szFindIndx;
				int nFolderIndx = -1;
				int nItemIndx = -1;
				int nItemCount = pSTListCtrl->GetItemCount();
				for(int nSubIndx = 0; nSubIndx < nItemCount; ++nSubIndx)
				{
					szItemName = pSTListCtrl->GetItemText(nSubIndx, 0);
					szItemName.MakeUpper();

					if(szItemName.Find(szSTName) != -1) {
						nFolderIndx = nIndx;
						nItemIndx = nSubIndx;
						szFindIndx.Format("%d:%d", nFolderIndx, nItemIndx);
						arFindIndx.Add(szFindIndx);
					}
				}
			}
		}

		nSTCount = arFindIndx.GetSize();
	}

	return nSTCount;
}

void CSTLoadManager::DeleteSTFile(int nUserIndx, CString szSTFile)
{
	CSTListCtrl* pSTListCtrl = NULL;
	if(pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nUserIndx))
	{
		CString szItemName;
		CString szFindIndx;
		BOOL bFound = FALSE;
		int nItemCount = pSTListCtrl->GetItemCount();
		int nIndx = 0;
		for( nIndx = 0; nIndx < nItemCount; ++nIndx)
		{
			szItemName = pSTListCtrl->GetItemText(nIndx, 0);
			int nSTType = -1;
			if((nSTType = szItemName.Find(gSTGubunKey)) != -1)
				szItemName = szItemName.Left(nSTType);

			if(szItemName.CompareNoCase(szSTFile) == 0)
			{
				bFound = TRUE;
				break;
			}
		}

		if(bFound)
			pSTListCtrl->DeleteItem(nIndx);
	}
}

BOOL CSTLoadManager::UpdateST(int nState, CString szSTPath) 
{
	int nSTIndx = 0;
	CString szSTName(szSTPath);
	if(m_pOutBarCtrl && m_pOutBarCtrl->GetSafeHwnd()) 
	{
		m_pOutBarCtrl->iLastFolderHighlighted = -1;
		m_pOutBarCtrl->iLastSelectedFolder = -1;

		CString szFolderName;
		if(nState == Ext_STChange_SDDelete || nState == Ext_STChange_SDAdd)
		{
			CString szSTType;
			IsSTTypeToLoad(szSTPath, szSTType);
			DWORD dwSTType = atol(szSTType);

			if(dwSTType == NULL)
				return FALSE;

			BOOL bAvailable = FALSE;
			if(dwSTType & STRATEGY_BULL)				// 강세약세
				szFolderName = "사용자강세약세";
			else if(dwSTType & STRATEGY_ORDER)			// 주문신호
				szFolderName = "사용자단위전략";
			else if( dwSTType & STRATEGY_INDEX)				// 지표
				szFolderName = "사용자지표";
		}
		else
			szFolderName = "사용자조합전략";

		nSTIndx = GetFolderIndexFromFolderName(szFolderName);
		if(nSTIndx < 0)
			return FALSE;

		CString szSTFilePath;
		switch(nState)
		{
		case Ext_STChange_SDDelete:
			DeleteSTFile(nSTIndx, szSTPath);	break;
		case Ext_STChange_SDAdd:
			//szSTFilePath.Format("%s\\%s\\%s%s", m_szCurrentSTPath, szFolderName, szSTPath, ".ST");
			szSTFilePath.Format("%s\\%s\\%s%s", m_szCurrentSTPath, szFolderName, szSTName, ".ST");
			AddSTFile(szSTFilePath);				break;
		case Ext_STChange_Delete:
			DeleteSTFile(nSTIndx, szSTPath);	break;
		case Ext_STChange_Add:
			//szSTFilePath.Format("%s\\%s\\%s%s", m_szCurrentSTPath, szFolderName, szSTPath, ".ST");
			szSTFilePath.Format("%s\\%s\\%s%s", m_szCurrentSTPath, szFolderName, szSTName, ".ST");
			AddSTFile(szSTFilePath);				break;
		}
	}

	//	// User폴더에서 다시 로드할 ListCtrl ID 계산
	m_pOutBarCtrl->SetSelFolder(nSTIndx);
	if(nState == Ext_STChange_SDAdd || nState == Ext_STChange_Add)
	{
		CSTListCtrl* pSTListCtrl = NULL;
		if(pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nSTIndx))
		{
			int nSTItemIndx = pSTListCtrl->GetItemCount() - 1;
			SetSelectedItem(nSTIndx, nSTItemIndx);
		}
	}

	return TRUE;
}

int CSTLoadManager::GetFolderIndexFromFolderName(CString szFolderName)
{
	CSTListCtrl* pSTListCtrl = NULL;
	int nFolderIndx = -1;
	int nFolderCount = m_pOutBarCtrl->GetFolderCount();

	for(int nFIndx = 0; nFIndx < nFolderCount; ++nFIndx)
	{
		if(pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nFIndx))
		{
			if(szFolderName.Compare(pSTListCtrl->m_szFolderName) == 0)
			{
				nFolderIndx = nFIndx;
				break;
			}
		}
	}

	return nFolderIndx;
}

int	CSTLoadManager::GetFolderST(DWORD dwInfo)
{
	CSTListCtrl* pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(m_pOutBarCtrl->GetSelFolder());
	LIST_STMNG_ITEMINFO* pInfo = (LIST_STMNG_ITEMINFO*)dwInfo;
	int nItemCount = 0;
	if(pInfo == NULL || (nItemCount = pSTListCtrl->GetItemCount()) <= 0)
		return 0;

	STMNG_ITEMINFO* pItemInfo = NULL;
	for(int nIndx = 0; nIndx < nItemCount; ++nIndx)
	{
		pItemInfo = new STMNG_ITEMINFO;
		pItemInfo->szSTPath = pSTListCtrl->m_szFullPath;

		CString szText = pSTListCtrl->GetItemText(nIndx, 0);
		int nType = (int)pSTListCtrl->GetItemData(nIndx);
		if(nType == 9999)
			pItemInfo->szSTName = szText;
		else
			pItemInfo->szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);

		pItemInfo->nSTType = pSTListCtrl->m_nSTFolderType;
		pItemInfo->szSTRData = pSTListCtrl->m_szFolderName;

		pInfo->AddTail(pItemInfo);
	}

	return nItemCount;
}

int	CSTLoadManager::GetItemST(DWORD dwInfo)
{
	CSTListCtrl* pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(m_pOutBarCtrl->GetSelFolder());
	LIST_STMNG_ITEMINFO* pInfo = (LIST_STMNG_ITEMINFO*)dwInfo;
	int nItemCount = 0;
	if(pInfo == NULL || (nItemCount = pSTListCtrl->GetItemCount()) <= 0)
		return 0;

	int nItemIndx = -1;
	STMNG_ITEMINFO* pItemInfo = NULL;
	POSITION pos = pSTListCtrl->GetFirstSelectedItemPosition();
	while(pos) {
		if((nItemIndx = pSTListCtrl->GetNextSelectedItem(pos)) >= 0) {

			pItemInfo = new STMNG_ITEMINFO;
			pItemInfo->szSTPath = pSTListCtrl->m_szFullPath;

			CString szText = pSTListCtrl->GetItemText(nItemIndx, 0);
			int nType = (int)pSTListCtrl->GetItemData(nItemIndx);

			if(nType == 9999)
				pItemInfo->szSTName = szText;
			else
				pItemInfo->szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);

			pItemInfo->nSTType = pSTListCtrl->m_nSTFolderType;
			pItemInfo->szSTRData = pSTListCtrl->m_szFolderName;

			pInfo->AddTail(pItemInfo);
		}
	}

	return 1;
}

BOOL CSTLoadManager::GetSelectedItem(STMNG_ITEMINFO& InData)
{
	if(m_pOutBarCtrl) {
		int nFolderIndx = m_pOutBarCtrl->GetSelFolder();
		int nItemIndx = -1;
		CSTListCtrl* pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nFolderIndx);
		POSITION pos = pSTListCtrl->GetFirstSelectedItemPosition();
		while(pos) {
			if((nItemIndx = pSTListCtrl->GetNextSelectedItem(pos)) >= 0) {
				CString szText = pSTListCtrl->GetItemText(nItemIndx, 0);
				int nType = (int)pSTListCtrl->GetItemData(nItemIndx);
				if(nType == 9999)
					InData.szSTName = szText;
				else
					InData.szSTName.Format(_T("%s%c%d"), szText, gSTGubunKey, nType);

				InData.szSTPath = pSTListCtrl->m_szFullPath;
				InData.nSTType = pSTListCtrl->m_nSTFolderType;
				InData.szSTRData = pSTListCtrl->m_szFolderName;
				InData.nFolderIndx = nFolderIndx;
				InData.nItemIndx = nItemIndx;
				return TRUE;
			}
			else
				return FALSE;
		}
	}

	return FALSE;
}

void CSTLoadManager::SetSelectedItem(int nFolderIndx, int nItemIndx)
{
	if(m_pOutBarCtrl) {
		if(nFolderIndx >= 0 && nFolderIndx < m_pOutBarCtrl->GetFolderCount())
			m_pOutBarCtrl->SetSelFolder(nFolderIndx);

		if(nItemIndx != -1) {
			CSTListCtrl* pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nFolderIndx);
			pSTListCtrl->SetItemState(nItemIndx, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
			pSTListCtrl->EnsureVisible(nItemIndx, FALSE);
		}
	}
}

void CSTLoadManager::SetSelectedItem(STMNG_ITEMINFO* pInData)
{
	if(pInData == NULL)
		return;

	CString szFolderName = pInData->szSTRData;
	CString szItemName = pInData->szSTName;

	//szFolderName.Delete(0, szFolderName.ReverseFind('\\') + 1);
	int nSTType = -1;
	if((nSTType = szItemName.Find(gSTGubunKey)) != -1)
		szItemName = szItemName.Left(nSTType);

	int nFolderIndx = -1;
	int nItemIndx = -1;
	if(m_pOutBarCtrl) 
	{
		CSTListCtrl* pSTListCtrl = NULL;
		int nFolderCount = m_pOutBarCtrl->GetFolderCount();
		int nItemCount = 0;
		for(int nFIndx = 0; nFIndx < nFolderCount; ++nFIndx)
		{
			if(pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nFIndx))
			{
				if(szFolderName.Compare(pSTListCtrl->m_szFolderName) == 0)
				{
					nFolderIndx = nFIndx;
					if(nItemCount = pSTListCtrl->GetItemCount())
					{
						CString szName;
						for(int nIIndx = 0; nIIndx < nItemCount; ++nIIndx)
						{
							szName = pSTListCtrl->GetItemText(nIIndx, 0);
							if(szName.Compare(szItemName) == 0)
							{
								nItemIndx = nIIndx;
								SetSelectedItem(nFolderIndx, nItemIndx);
							}
						}
					}
				}
			}
		}
	}
}

void CSTLoadManager::LoadSTList(BOOL bOnlyUser/*=FALSE*/)
{
	// Data폴더의 STData
	CFileFind finder;
	if(!bOnlyUser) {
		m_szCurrentSTPath = m_parSTPath->GetAt(STDATA_INDEX);

		BOOL bExist = finder.FindFile(m_szCurrentSTPath);
		if(bExist)
		{
			m_bSearching = TRUE;
			DirectorySearch(m_szCurrentSTPath, "*.*", m_bSubFolders);
			m_bSearching = FALSE;
		}
	}

	// User폴더의 STData

	if(m_parSTPath->GetSize()>1)//KHD: 예외 처리 : 성과분석으로 인하여..
	{
		m_szCurrentSTPath = m_parSTPath->GetAt(STUSER_INDEX);
		m_uSTUserCount = 0;
		BOOL bExist = finder.FindFile(m_szCurrentSTPath);
		if(bExist)
		{
			m_bSearching = TRUE;
			m_bSTUserLoading = TRUE;
			DirectorySearch(m_szCurrentSTPath, "*.*", m_bSubFolders);
			m_bSTUserLoading = FALSE;
			m_bSearching = FALSE;
		}
	}
}

// p_LoadOption :	0 이면 LoadST(..) 그대로 호출
//					1 이면 조합가능한 전략만 로딩(지표,강세/약세,조건등 제외)
//					2 이면 사용자 전략 로딩 안함.
//					And(&) 조합가능함.
void CSTLoadManager::LoadSTListFromSTIndex(int p_LoadOption, BOOL bOnlyUser/*=FALSE*/)
{
	// Data폴더의 STData
	if(!bOnlyUser) 
	{
		CString szOptimizerPath ;
		szOptimizerPath.Format("%s\\STData\\VarOptima.cfg",(LPCSTR)AfxGetPctrData(12));

		CString szSTDataPath = m_parSTPath->GetAt(STDATA_INDEX);
		m_szCurrentSTPath = szSTDataPath;

		char szSTIndexPath[MAX_PATH];
		sprintf(szSTIndexPath, "%s\\%s", szSTDataPath, "STIndex.dat");

		LPCSTR szSecKey = "LIST";
		int nFolderCnt = ::GetPrivateProfileInt(szSecKey, "Count", 0, szSTIndexPath);
		int nFileCnt = 0;

		char szKeyFolderName[MAX_PATH]={0,}, szKeyFileName[MAX_PATH]={0,}, szKeyRealFolderName[MAX_PATH]={0,};

		char szKey[5];
		CString szSTPath;
		int	nVersion=0;
		CString szFolderName;
		CString szRealFolderName;
		for(int nFolder = 0; nFolder < nFolderCnt; ++nFolder)
		{
			sprintf(szKey, "%d", nFolder+1);

			if(::GetPrivateProfileString(szSecKey, szKey, "", szKeyFolderName, MAX_PATH, szSTIndexPath) > 0)
			{
				szFolderName = szKeyFolderName;
				if(p_LoadOption == 1 && szFolderName.Compare("강세약세") == 0)
					continue;

				::GetPrivateProfileString(szKeyFolderName, "RealFolderName", "", szKeyRealFolderName, MAX_PATH, szSTIndexPath);
				//szSTPath = szSTDataPath + "\\" + szKeyFolderName;
				//AddFolder(szSTPath, szKeyFolderName);
				szRealFolderName = szKeyRealFolderName;
				szSTPath = szSTDataPath + "\\" + szRealFolderName;
				AddFolder(szSTPath, szFolderName);

				nFileCnt = ::GetPrivateProfileInt(szKeyFolderName, "Count", 0, szSTIndexPath);
				nVersion = ::GetPrivateProfileInt(szKeyFolderName, "Version", 0, szSTIndexPath);
				if(nVersion==0)
				{
					for(int nFile = 0; nFile < nFileCnt; ++nFile)
					{
						CString szSection;
						sprintf(szKey, "%d", nFile+1);
						if(::GetPrivateProfileString(szKeyFolderName, szKey, "", szKeyFileName, MAX_PATH, szSTIndexPath) > 0)
						{
							if(m_parSTPath->GetSize()==1)//변수 최적화시 : KHD 
							{
								szSection.Format("%s",szKeyFileName);

								int nFind = szSection.Find(gSTGubunKeys);
								if(nFind>0) szSection = szSection.Left(nFind);
								int nCount = ::GetPrivateProfileInt(szSection, "Count", 0, szOptimizerPath);
								if(nCount==0) continue;
							}
							CString szSTFilePath;
							szSTFilePath.Format(_T("%s\\%s.ST"), szSTPath, szKeyFileName);
							AddSTFile(szSTFilePath);
						}
					}
				}
				else if(nVersion==1)
				{
					int nFind;
					CString szKeyFileName1, szKeyFileName2, szTmp;
					for(int nFile = 0; nFile < nFileCnt; ++nFile)
					{
						CString szSection;
						sprintf(szKey, "%d", nFile+1);
						if(::GetPrivateProfileString(szKeyFolderName, szKey, "", szKeyFileName, MAX_PATH, szSTIndexPath) > 0)
						{
							if(m_parSTPath->GetSize()==1)//변수 최적화시 : KHD 
							{
								szSection.Format("%s",szKeyFileName);

								int nFind = szSection.Find(gSTGubunKeys);
								if(nFind>0) szSection = szSection.Left(nFind);
								int nCount = ::GetPrivateProfileInt(szSection, "Count", 0, szOptimizerPath);
								if(nCount==0) continue;
							}

							szTmp = szKeyFileName;
							nFind = szTmp.Find(';');
							if(nFind<0) continue;

							szKeyFileName1 = szTmp.Left(nFind);
							szKeyFileName2 = szTmp.Mid(nFind+1);

							CString szSTFilePath;
							szSTFilePath.Format(_T("%s\\%s.ST"), szSTPath, szKeyFileName2);
							AddSTFile(szSTFilePath, szKeyFileName1);
						}
					}
				}
			}
		}
	}

	// User폴더의 STData
	CFileFind finder;
	if(m_parSTPath->GetSize()>1)//KHD: 예외 처리 : 성과분석으로 인하여..
	{
		m_szCurrentSTPath = m_parSTPath->GetAt(STUSER_INDEX);
		m_uSTUserCount = 0;

		/* 기존 루틴을 코멘트 처리하고 새로운 루틴으로 대체함.
		사용자폴더가 더 세분화 되어짐.
		기존루틴 START
		BOOL bExist = finder.FindFile(m_szCurrentSTPath);
		if(bExist)
		{
			m_bSearching = TRUE;
			m_bSTUserLoading = TRUE;
			DirectorySearch(m_szCurrentSTPath, "*.*", m_bSubFolders);
			m_bSTUserLoading = FALSE;
			m_bSearching = FALSE;
		}
		기존루틴 END */

		// 여기서 부터 새로운 루틴 START
		// 폴더명
		//		1.사용자단위전략((@1) ^1
		//		2.사용자조합전략(.ST인 파일들)	
		//		3.사용자강세/약세(@4,@5)	^4, ^5
		//		4.사용자지표(@1) ^1
		m_bSearching = TRUE;
		m_bSTUserLoading = TRUE;

		CString szSTDir;
		LPCSTR  szDirKey;

		BOOL bWizardType = FALSE;
		if(p_LoadOption==0) bWizardType = FALSE;
		else if(p_LoadOption==1) bWizardType = TRUE;

		//#. 사용자단위전략을 로딩한다. @2,@3,@7
		szDirKey = "사용자단위전략";
		szSTDir = m_szCurrentSTPath + "\\" + szDirKey;
		LoadSTFolderList(szDirKey, szSTDir, 237);

		//#. 사용자조합전략을 로딩한다.
		szDirKey = "사용자조합전략";
		szSTDir = m_szCurrentSTPath + "\\" + szDirKey;
		LoadSTFolderList(szDirKey, szSTDir, 999);

		//#. 사용자지표를 로딩한다. 사용자 단위전략에 있는 파일중에서 @1 로딩.
		szDirKey = "사용자단위전략";
		szSTDir = m_szCurrentSTPath + "\\" + szDirKey;
		if(!bWizardType)LoadSTFolderList("사용자지표", szSTDir, 1);

		//#. 사용자강세약세를 로딩한다. 사용자 단위전략에 있는 파일중에서 @5 로딩.
		//szDirKey = "사용자단위전략";
		//szSTDir = m_szCurrentSTPath + "\\" + szDirKey;
		if(!bWizardType)LoadSTFolderList("사용자강세약세", szSTDir, 45);

		m_bSTUserLoading = FALSE;
		m_bSearching = FALSE;
		// 새로운 루틴 END
	}
}

void CSTLoadManager::LoadSTFolderList(LPCSTR p_szGroupName, LPCSTR p_szTargetDir, int p_nSTType)
{
	List_STFILEITEM arrSTList;
	CSTLoadManager::GetSTFileList(p_szTargetDir, p_nSTType, &arrSTList);

//	if(arrSTList.GetCount()==0) return;

	// 리스트를 추가한다.
	AddFolder(p_szTargetDir, p_szGroupName);

	// 새로운 리스트에 아이템추가
	CString szSTFilePath;
	STFILEITEM*  pItem = NULL;

	for(POSITION pos=arrSTList.GetHeadPosition(); pos;)
	{
		pItem = arrSTList.GetNext(pos);
		szSTFilePath.Format("%s\\%s%s", p_szTargetDir, pItem->szRawName, ".ST");
		AddSTFile(szSTFilePath);
	}

	Delete_List_STFILEITEM(arrSTList);
}

BOOL CSTLoadManager::IsSTTypeToLoad(CString& szFileTitle, CString& szSTType)
{
	if(szFileTitle.IsEmpty())
		return FALSE;

	szSTType.Empty();
	int nSTTypePosition = szFileTitle.Find(gSTGubunKey);

	// 조합전략일 경우
	if(nSTTypePosition == -1)
		return TRUE;

	// 조합전략 이외의 경우
	szSTType = szFileTitle;
	szFileTitle = szFileTitle.Mid(0, nSTTypePosition);
	szSTType = szSTType.Mid(nSTTypePosition + 1, 1);
	DWORD dwSTType = atol(szSTType);
	if(dwSTType == NULL)
		return FALSE;

	BOOL bAvailable = FALSE;
	if( dwSTType & STRATEGY_INDEX ||				// 지표
		dwSTType & STRATEGY_ORDER ||				// 주문신호
	    dwSTType & STRATEGY_BULL)					// 강세약세
		bAvailable = TRUE;

//	if(!bAvailable && (dwSTType & STRATEGY_INDEX))	// 지수
//		return FALSE;

	return TRUE;
}

void CSTLoadManager::GetFolderName(CString szPath, CString& szSTFolder)
{
	szSTFolder.Empty();
	if(szPath.IsEmpty())
		return;

	// 경로에서 전략폴더명 얻기
	int nLastSlashPos = szPath.ReverseFind('\\');
	szSTFolder = szPath.Mid(nLastSlashPos + 1);
}

void CSTLoadManager::AddFolder(LPCTSTR szSTFolderPath, LPCSTR szFolerName)
{
	CString szPath = szSTFolderPath;
	if(!szPath.Compare(m_szCurrentSTPath))
		return;

	// ListCtrl 생성 및 초기화
	CSTListCtrl* pSTListCtrl = new CSTListCtrl;

	CString szRealFolderName(szSTFolderPath);
	szRealFolderName.Delete(0, szRealFolderName.ReverseFind('\\') + 1);
	pSTListCtrl->m_szFolderName = szFolerName;
	pSTListCtrl->m_szRealFolderName = szRealFolderName;
	pSTListCtrl->m_szFullPath = szSTFolderPath;
	pSTListCtrl->m_nSTFolderType = (int)m_bSTUserLoading;
	pSTListCtrl->m_nFolderIndx = m_uSTIndx;


	if(!pSTListCtrl->Create(
		WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_NOCOLUMNHEADER, 
		CRect(0,0,0,0), 
		m_pOutBarCtrl, 
		IDC_STListCtrl + m_uSTIndx++)) 
	{
		delete pSTListCtrl;
		pSTListCtrl = NULL;
		return;
	}

	pSTListCtrl->InitFont(m_bUseBoldFont);
	pSTListCtrl->SetExtendedStyle(pSTListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	pSTListCtrl->SetImageList(m_pILSTIcon, LVSIL_SMALL);
	pSTListCtrl->InsertColumn(0, _T(""), LVCFMT_LEFT, 200);

	// 전략폴더 경로로부터 폴더명 얻기
//	CString szSTFolder;
//	GetFolderName(szPath, szSTFolder);

	// 전략폴더에 ListCtrl(ST파일) Add
	//m_pOutBarCtrl->AddFolderBar(szFolerName, pSTListCtrl);
	m_pOutBarCtrl->AddFolderBar(szFolerName, pSTListCtrl);

	// ListCtrl ID 계산을 위해 Counting
	if(m_bSTUserLoading)
		m_uSTUserCount++;
}

void CSTLoadManager::AddSTFile(LPCTSTR szSTFilePath, LPCSTR p_szShowName)
{
	int nFolderCount = m_pOutBarCtrl->GetFolderCount();
	
	if(nFolderCount > 0) {


		CString szPath(szSTFilePath);
		CString szExt;
		int nPosExt = szPath.ReverseFind('.');
		int nPosFileTitle = szPath.ReverseFind(('\\'));
		szExt = szPath.Mid(nPosExt);

		// 파일 확장자가 ".ST"가 아닌 경우 리턴

		if(szExt.CompareNoCase(".ST"))
			return;

		CString szFileTitle;
		if(p_szShowName) 
		{
			szFileTitle = p_szShowName;
		}
		else{
			 szFileTitle = szPath.Mid(nPosFileTitle+1);
			 szFileTitle = szFileTitle.Left(szFileTitle.GetLength()-3);
		}

		CString szFolderName;
		GetFolderName(szPath.Left(nPosFileTitle), szFolderName);

		CString szSTType;
		CSTListCtrl* pSTListCtrl = NULL;
		if(IsSTTypeToLoad(szFileTitle, szSTType))
		{
			if(m_bSearching)
				pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nFolderCount - 1);
			else
			{
				for(int nIndx = 0; nIndx < nFolderCount; ++nIndx)
				{
					pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nIndx);
					if(szFolderName.CompareNoCase(pSTListCtrl->m_szFolderName) == 0)
						break;
				}
			}

			if(pSTListCtrl == NULL)
				return;

			pSTListCtrl->InsertItem(pSTListCtrl->GetItemCount(), szFileTitle, 0);

			// STType 정보
			if(szSTType.GetLength())
			{
				DWORD dwType = (DWORD)atoi(szSTType);
//				if(dwType==4 || dwType==5 || dwType==1)
//					pSTListCtrl->SetItemData(pSTListCtrl->GetItemCount() - 1, (DWORD)9990);
//				else
					pSTListCtrl->SetItemData(pSTListCtrl->GetItemCount() - 1, dwType);
			}
			else
			{
				pSTListCtrl->SetItemData(pSTListCtrl->GetItemCount() - 1, (DWORD)9999);
			}

		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// 만든이            : 이준옥(Jun-Ok, Lee)
// Email             : alzioyes@gmail.com
// 만든 날짜         : 2006/08/17
// 함수 이름         : int	DirectorySearch
// 리턴 타입         : void
// 파라미터
//		LPCSTR pszStartDir : 시작디렉토리
//		LPCSTR pszExt : 파일확장자
//		BOOL bSub : 서브디렉토리 찾기 여부
// 함수 설명         : 
///////////////////////////////////////////////////////////////////////////////
int	CSTLoadManager::DirectorySearch(LPCSTR pszStartDir, LPCSTR pszExt, BOOL bSub)
{
	char aFile[MAX_PATH];
	sprintf(aFile, "%s\\%s", pszStartDir, pszExt);

	CString strComponent;
	CFileFind finder;
	BOOL bWorking = finder.FindFile( aFile );
	int nCnt = 0;
	//KHD : 전략 Count 찾기..
	CString szOptimizerPath ;
	szOptimizerPath.Format("%s\\STData\\VarOptima.cfg",(LPCSTR)AfxGetPctrData(12));

	while (bWorking){
		bWorking = finder.FindNextFile();
		strComponent = finder.GetFileName();		// 파일명

		if(!finder.IsDirectory())  {
			// 파일이름이므로 해당폴더에 추가한다.
			// 여기에서 필요한 작업을 한다.
			// 여기...
			//
			TRACE("CSTLoadManager::DirectorySearch(..) FileInfo => [Dir=%s][FileName=%s]\n", pszStartDir, strComponent);
			if(m_parSTPath->GetSize()==1)//변수 최적화시 : KHD 
			{
				CString szSection;

				szSection.Format("%s",strComponent);

				int nFind = szSection.Find(gSTGubunKeys);
				if(nFind>0) szSection = szSection.Left(nFind);
				int nCount = ::GetPrivateProfileInt(szSection, "Count", 0, szOptimizerPath);
				if(nCount==0) continue;
			}
			CString szSTFilePath;
			szSTFilePath.Format(_T("%s\\%s"), pszStartDir, strComponent);
			AddSTFile(szSTFilePath);
			nCnt++;
		}
		//else if(bSub && strComponent[0]!='.') {
		else if(finder.IsDots()==FALSE && bSub) {
			// 디렉토리일 경우 재귀호출을 콜한다.
			char aSubDir[MAX_PATH]={0,};
			sprintf(aSubDir, "%s\\%s", pszStartDir, strComponent);
			TRACE("CSTLoadManager::DirectorySearch(..) Directory => %s\n", aSubDir);
			CString szSTFolderPath;
			szSTFolderPath.Format(_T("%s\\%s"), pszStartDir, strComponent);
			AddFolder(aSubDir, strComponent);
			nCnt += DirectorySearch(aSubDir, pszExt, bSub);
		}
	}
	return nCnt;
}

//	p_nSTType은 ST파일중에서 @다음의 숫자를 가지고 리스트를 얻는 키.
//		1	 : @1	-> 지표
//		237 : @2,@3,@7 -> 지표, 강세약세 제외
//		5	 : @5	-> 강세약세
//		999 : 전부로딩
int CSTLoadManager::GetSTFileList(LPCSTR szDir, int p_nSTType, List_STFILEITEM *p_arrSTList)
{
	Delete_List_STFILEITEMP(p_arrSTList);

	CStringArray arr;
	CSTLoadManager::GetSTFileList(szDir, arr);
	
	CString szText;
	int nFind = -1;
	STFILEITEM* pItemInfo = NULL;

	for(int i=0; i<arr.GetSize(); i++)
	{
		szText = arr.GetAt(i); 

		nFind = szText.Find(gSTGubunKeys);
		if(nFind>0)
		{
			if(p_nSTType==1)
			{
				if(szText[nFind+1]=='1') 
				{
					pItemInfo = new STFILEITEM;
					pItemInfo->szRawName = szText;

					szText = szText.Left(nFind);
					pItemInfo->szSTName = szText;

				}
				else continue;
			}
			else if(p_nSTType==45)
			{
				if(szText[nFind+1]=='5' || szText[nFind+1]=='4') 
				{
					pItemInfo = new STFILEITEM;
					pItemInfo->szRawName = szText;

					szText = szText.Left(nFind);
					pItemInfo->szSTName = szText;
				}
				else continue;
			}
			else if(p_nSTType==237)
			{
				if(szText[nFind+1]=='2' || szText[nFind+1]=='3' ||szText[nFind+1]=='7') 
				{
					pItemInfo = new STFILEITEM;
					pItemInfo->szRawName = szText;

					szText = szText.Left(nFind);
					pItemInfo->szSTName = szText;
				}
				else continue;
			}
			else if(p_nSTType=999)
			{
				pItemInfo = new STFILEITEM;
				pItemInfo->szRawName = szText;

				szText = szText.Left(nFind);
				pItemInfo->szSTName = szText;
			}
		}
		else
		{
			if(p_nSTType=999)
			{
				pItemInfo = new STFILEITEM;
				pItemInfo->szRawName = szText;
				pItemInfo->szSTName = szText;
			}
			else
				continue;
		}

		p_arrSTList->AddTail(pItemInfo);
	}

	return p_arrSTList->GetCount();
}

int CSTLoadManager::GetSTFileList(LPCSTR p_aTarDir, CStringArray &p_arr)
{
	p_arr.RemoveAll();

	char aFile[MAX_PATH];
	sprintf(aFile, "%s\\%s", p_aTarDir, "*.ST");
	char* aFilePath = _strupr(aFile);

	CString strComponent, szTmp;
	int nFind;
	CFileFind finder;
	BOOL bWorking = finder.FindFile( aFilePath );
	while (bWorking){
		bWorking = finder.FindNextFile();
		strComponent = finder.GetFileName();
		szTmp = strComponent;
		szTmp.MakeUpper();

		if(!finder.IsDirectory())  
		{
			nFind = szTmp.Find(SZSTEXT);
			if(nFind>0)
			{
				strComponent = strComponent.Left(nFind);
				p_arr.Add(strComponent);
			}
		}
	}
	return p_arr.GetSize();
}


BOOL CSTLoadManager::GetRunSTItemInfo(CString szSTName, STMNG_ITEMINFO& InData)
{
	szSTName.MakeUpper();

	CString szFolderName(InData.szSTRData);
	CString szItemName;
	int nFolderIndx = 0;
	int nItemIndx = 0;
	if(m_pOutBarCtrl && m_pOutBarCtrl->GetSafeHwnd()) 
	{
		CSTListCtrl* pSTListCtrl = NULL;
		BOOL bFound = FALSE;
		int nFolderCount = m_pOutBarCtrl->GetFolderCount();
		for(int nIndx = 0; nIndx < nFolderCount && !bFound; ++nIndx)
		{
			pSTListCtrl = (CSTListCtrl*)m_pOutBarCtrl->GetFolderChild(nIndx);
			if(szFolderName.Compare(pSTListCtrl->m_szRealFolderName) == 0) 
			{
				int nItemCount = pSTListCtrl->GetItemCount();
				for(int nSubIndx = 0; nSubIndx < nItemCount; ++nSubIndx)
				{
					szItemName = pSTListCtrl->GetItemText(nSubIndx, 0);
					szItemName.MakeUpper();

					if(szItemName.Compare(szSTName) == 0) {
						nFolderIndx = nIndx;
						nItemIndx = nSubIndx;
						bFound = TRUE;
						break;
					}
				}
			}
		}

		if(pSTListCtrl && bFound)
		{
			InData.nFolderIndx = nFolderIndx;
			InData.nItemIndx = nItemIndx;
			InData.nSTType = pSTListCtrl->m_nSTFolderType;
//			InData.szSTPath = pSTListCtrl->m_szFullPath;
			InData.szSTRData = pSTListCtrl->m_szFolderName;

			return TRUE;
		}
	}

	return FALSE;
}

