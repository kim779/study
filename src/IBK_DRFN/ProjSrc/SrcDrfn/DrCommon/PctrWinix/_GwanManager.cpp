// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PctrWinix.h"
#include "../../../SrcSite/include/axisgenv.h"

#define MAX_GWANSIM 100
/////////////////////////////////////////////////////////////////////////////
// CAUGwanManager
CAUGwanManager::CAUGwanManager()
{
}

CAUGwanManager::~CAUGwanManager()
{
	_DoClearList();
}

CWinixGwanItem* CAUGwanManager::_GetKeyItem(long dwRet)
{
	if(dwRet != NULL)
	{
		//CWinixGwanItem* pGItem = m_List.GetAt((POSITION)dwRet);
		//return pGItem;
		return m_List.GetAt((POSITION)dwRet);
	}
	return NULL;
}

void CAUGwanManager::_DoClearList()
{
	for(POSITION pos=m_List.GetHeadPosition(); pos;)
	{
		CWinixGwanItem* pItem = m_List.GetNext(pos);
		if(pItem) delete pItem;
	}
	m_List.RemoveAll();
}

long CAUGwanManager::Advise(LPCSTR _szKey)
{
	CString szKey(_szKey); szKey+='\t';
	CString szKeyData[4];
	int nPos;
	{
		nPos=szKey.Find('\t');
		if(nPos<0) return NULL;
		szKeyData[0] = szKey.Left(nPos);

		//-----------------------
		szKey = szKey.Mid(nPos+1);

		nPos=szKey.Find('\t');
		if(nPos<0) return NULL;
		szKeyData[1] = szKey.Left(nPos);

		//-----------------------
		szKey = szKey.Mid(nPos+1);
		
		nPos=szKey.Find('\t');
		if(nPos<0) return NULL;
		szKeyData[2] = szKey.Left(nPos);

		//-----------------------
		szKey = szKey.Mid(nPos+1);
		
		nPos=szKey.Find('\t');
		if(nPos<0) return NULL;
		szKeyData[3] = szKey.Left(nPos);
	}

	HWND hWinix, hDrfnWrap, hGwanReciever;
	sscanf(szKeyData[0], "%08X", &hWinix);
	sscanf(szKeyData[1], "%08X", &hDrfnWrap);
	sscanf(szKeyData[2], "%08X", &hGwanReciever);

	CWinixGwanItem* pGItem = new CWinixGwanItem(hWinix, hDrfnWrap, hGwanReciever);
	pGItem->DoInitialize(TRUE);
	// 2012.2.7 박성경
	//pGItem->RequestGwansimGroup();

	POSITION pos = m_List.AddTail(pGItem);
	return (long)pos;
}

void CAUGwanManager::UnAdvise(long dwRet)
{
	if(dwRet != NULL)
	{
		POSITION pos = (POSITION)dwRet;
		CWinixGwanItem* pGItem = m_List.GetAt(pos);
		if(pGItem)
		{
			//if(IsWindow(pGItem->m_hGwanReciever)
			delete pGItem;
		}
		m_List.RemoveAt(pos);
	}
}

long CAUGwanManager::GetGwansimDataAll(long dwRet, int nType, LPCSTR szGroup)
{
	CWinixGwanItem* pItem = _GetKeyItem(dwRet);
	return NULL;
}

void CAUGwanManager::RequestGwansimGroup(long dwRet)
{
	CWinixGwanItem* pItem = _GetKeyItem(dwRet);
	if(!pItem) return;

	pItem->RequestGwansimGroup();
}

CString Parser(CString &sSrc, CString sDelimiter)
{
	if (sSrc.Find(sDelimiter) == -1)
	{
		CString sTmp = sSrc;
		sSrc.Empty();
		return sTmp;
	}
	else
	{
		CString sTmp = sSrc.Left(sSrc.Find(sDelimiter));
		sSrc = sSrc.Mid(sSrc.Find(sDelimiter) + sDelimiter.GetLength());
		return sTmp;
	}
	
	return "";
}

void CAUGwanManager::GetGwansimGroupName(long dwRet, CStringArray &arrKey, CStringArray &arrName)
{
	CWinixGwanItem* pItem = _GetKeyItem(dwRet);
#if (0)//tour2k
	// 박성경
	if(pItem /*&& pItem->m_pIM*/)
	{
		// 박성경
		//pItem->m_pIM->GetGroupList(&rArr);
		HWND hWnd = pItem->m_hWinix;
		if(!hWnd) return;

		CString strData, strRoot, strUserName;
		strRoot = (char*)::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, homeCC), (LPARAM)(LPCSTR)strData);
		strUserName = (char*)::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, nameCC), (LPARAM)(LPCSTR)strData);

		CString sFile;
		sFile.Format("%s\\%s\\%s%s", strRoot, USRDIR, strUserName, "\\portfolio.ini");	
		
		int nReadL = 0, nIdx = 0, nTemp = 0;
		char szReadB[1024];
		CString	strBuf, strGNo, strGName;
		
		nReadL = GetPrivateProfileString("GROUPORDER", "00", "", szReadB, sizeof(szReadB), sFile);
		strBuf = CString(szReadB, nReadL);
		
		for (int i = 0; !strBuf.IsEmpty(); i++)
		{
			strGNo = Parser(strBuf, ";");
			nReadL = GetPrivateProfileString("GROUPNAME", strGNo, "", szReadB, sizeof(szReadB), sFile);
			strGName = CString(szReadB, nReadL);

			if(strGName.IsEmpty())
				continue;
			
			arrKey.Add(strGNo);
			arrName.Add(strGName);
		}
#else
	arrKey.Copy(m_GwanSimGroup.strKey);
	arrName.Copy(m_GwanSimGroup.strGroupName);

#endif
		// 2012.2.7 박성경: 관심리스트 로드 솔로몬 >>
// 		CStringArray rArr;
// 
// 		if(rArr.GetSize()>0)
// 		{
// 			CString szStr, szName;
// 			for(int i=0; i<rArr.GetSize(); i++)
// 			{
// 				szStr = rArr.GetAt(i);
// 				{
// 					int nPos1 = szStr.Find('\t');
// 					int nPos2 = szStr.Find('\t', nPos1+1);
// 					if(nPos1>0 && nPos2>nPos1)
// 					{
// 						szName = szStr.Mid(nPos1+1, nPos2-nPos1-1);
// 						arrName.Add(szName);
// 					}
// 				}
// 				arrKey.Add(szStr);
// 			}
// 		}
		// 2012.2.7 박성경: 관심리스트 로드 솔로몬 <<
//	}
//	pItem->m_pIM->GetGroupList(CStringArray* pArr);
}

void CAUGwanManager::GetGwansimItem(long dwRet, CString strKey, CStringArray& arrCode, CStringArray& arrName)
{
	CWinixGwanItem* pItem = _GetKeyItem(dwRet);
	if(!pItem) return;

	if(strKey.GetLength()==0) return;

	arrCode.RemoveAll();
	arrName.RemoveAll();

#if(0)//tour2k file read ==> request tr
	HWND hWnd = pItem->m_hWinix;
	if(!hWnd) return;

	CString strData, strRoot, strUserName;
	strRoot = (char*)::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, homeCC), (LPARAM)(LPCSTR)strData);
	strUserName = (char*)::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, nameCC), (LPARAM)(LPCSTR)strData);
	
	CString	strFilePath;
	strFilePath.Format("%s\\%s\\%s\\portfolio.i%02d", strRoot, USRDIR, strUserName, atoi(strKey));
	
	DWORD	fLen = 0;
	CFileFind finder;
	if (!finder.FindFile(strFilePath))
		return ;
	
	UINT	readL;
	CString	code = _T(""), amount = _T(""), price = _T(""), name = _T("");
	struct	_inters interest;
	
	CFileStatus fs;
	if (CFile::GetStatus(strFilePath, fs) == FALSE)
		return ;
	
	CFile	fileH(strFilePath, CFile::modeRead);
	if (fileH.m_hFile == CFile::hFileNull)
		return ;
	
	for (int nIndex = 0; nIndex < MAX_GWANSIM ; nIndex++)
	{
		readL = fileH.Read(&interest, sizeof(struct _inters));
		if (readL < sizeof(struct _inters))
			break;
		
		code.Format("%.*s", sizeof(interest.code), interest.code);
		code.TrimLeft(), code.TrimRight();
		name.Format("%.*s", sizeof(interest.name), interest.name);		
		name.TrimLeft(), name.TrimRight();

		if(!code.IsEmpty())
		{
			arrCode.Add(code);
			arrName.Add(name);
		}
	}
	fileH.Close();

	// 2012.2.7 박성경: 관심종목로드 솔로몬 >>
	/*
	CString szQueryKey;
	int nPos1 = strKey.Find('\t');
	if(nPos1==0) return;
	if(nPos1>1)
		szQueryKey = strKey.Left(nPos1);
	else
		szQueryKey = strKey;

	{
		CString strTRData = "";
		strTRData.Format("%s\t%08X\t%d\t%d\t%s"	
			,"GwanSim"
			, &pItem->m_xTrCommSite
			, TRKEY_GWANSIMITEM
			, 0			//Size
			, " ");		//Data
		CWnd* pDRfnWrap = CWnd::FromHandle(pItem->m_hDrfnWrap);
		//pDRfnWrap->GetParent()->SendMessage(RMSG_SENDMESSAGE, MAKEWPARAM(1, 0), (LPARAM)(LPCSTR)strTRData);
		pDRfnWrap->SendMessage(RMSG_SENDMESSAGE, MAKEWPARAM(1, 0), (LPARAM)(LPCSTR)strTRData);
	}
	//m_nQueryState = wgi_queryitem;
	// 박성경
	//pItem->m_pIM->QueryGroup(TRKEY_GWANSIMITEM, szQueryKey);
	
	*/
	// 2012.2.7 박성경: 관심종목로드 솔로몬 <<
#else	
	CString szQueryKey;
	int nPos1 = strKey.Find('\t');
	if(nPos1==0) return;
	if(nPos1>1)
		szQueryKey = strKey.Left(nPos1);
	else
		szQueryKey = strKey;

	int sendL = 0;
	char sendB[16 * 1024] = { 0, };
	int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';
	memcpy((char*)&sendB[sz_uinfo], strKey , 2);

	//	strWinix.Format("%s%80s%s%40s%s","MYUj"," ","00001U"," ",strKey);
	CString strTRData = "";
	strTRData.Format("%s\t%08X\t%d\t%d\t%s"	
			, TRNO_FAVORITE
			, &pItem->m_xTrCommSite
			, TRKEY_GWANSIMITEM
			, sendL //Size
			, sendB);		//Data

		CWnd* pDRfnWrap = CWnd::FromHandle(pItem->m_hDrfnWrap);
		//pDRfnWrap->GetParent()->SendMessage(RMSG_SENDMESSAGE, MAKEWPARAM(1, 0), (LPARAM)(LPCSTR)strTRData);
		pDRfnWrap->SendMessage(RMSG_SENDMESSAGE, MAKEWPARAM(1, 0), (LPARAM)(LPCSTR)strTRData);
#endif
}
//KHD : 관심등록 차트 수정 
int CAUGwanManager::ShowGwansimDlg(long dwRet, CWnd* pCaller, CString strCodeList /*=""*/)
{
	CWinixGwanItem* pItem = _GetKeyItem(dwRet);
	if(pItem && pItem->m_hWinix)
	{
		CString szData = _T("IB202201 /p5 /S/d");
		HWND hWnd = pItem->m_hWinix;
		::SendMessage(hWnd, WM_USER, MAKEWPARAM(viewDLL,typePOPUP), (LPARAM)(LPCSTR)szData);
	}

	return 0;
}

//KHD : 관심등록 차트 수
#define CH_TAB			0x09
#define CH_LF			0x0A
BOOL CAUGwanManager::ADDGwasimItem(long dwRet, CString strKey, CString strItem, BOOL bFirst)
{
	CString szRKey(strKey);
	szRKey = szRKey.Left(2);

	CWinixGwanItem* pItem = _GetKeyItem(dwRet);
	if(pItem && pItem->m_hWinix)
	{
		CString strBuf;
		strBuf.Format("%s%c%s%c", strKey, CH_TAB, strItem, CH_TAB);

		CString strData;
		strData.Format("%s%c%s", "", CH_TAB, strBuf);
 
		CString sParam;
		sParam.Format("%s /p5 /S/d%s%s%c", "IB202201", "appendITEMS", strData, CH_LF);
		HWND hWnd = pItem->m_hWinix;
		::SendMessage(hWnd, WM_USER, MAKEWPARAM(viewDLL,typePOPUP), (LPARAM)(LPCSTR)sParam);
	}

	return FALSE;
}

// BOOL CAUGwanManager::ADDGwasimItems(long dwRet, CString strKey, CStringArray& arrItem)
// {
//		CWinixGwanItem* pItem = _GetKeyItem(dwRet);
// 	return FALSE;
// }

int CAUGwanManager::SetRegGwansim(long dwRet, CWnd* pComboWnd, int nType)
{
	CWinixGwanItem* pItem = _GetKeyItem(dwRet);
	return 0;
}

int CAUGwanManager::RegiestToImsiGroup(long dwRet, CStringArray& arrCode, CWnd* pWnd)
{
	CWinixGwanItem* pItem = _GetKeyItem(dwRet);
	return 0;
}

LPCSTR CAUGwanManager::GetGwanKeyFromName(long dwRet, LPCSTR szGwanGroupName)
{
	CWinixGwanItem* pItem = _GetKeyItem(dwRet);
	return "";
}

int CAUGwanManager::ShowGwansimDlgWithList(long dwRet, CWnd* pCaller, CStringArray& arrItem)
{
	CWinixGwanItem* pItem = _GetKeyItem(dwRet);
	return 0;
}


int CAUGwanManager::ParseGwanList(WPARAM wParam, LPARAM lParam, CString &szGroup, CStringList &szItemList)
{
//KHD
// 	CString strCode(_T(""));
// 	struct _pfgroup* pGroup = (struct _pfgroup*)lParam;
// 	if(!pGroup) return 0;
// 
// 	szGroup.Format("%*.*s\t%*.*s", sizeof(pGroup->key), sizeof(pGroup->key), pGroup->key,
// 							 sizeof(pGroup->name), sizeof(pGroup->name), pGroup->name);
// 	szGroup.TrimRight();
// 
// 	int nCount = atoi((LPCTSTR)CString(pGroup->nrec, sizeof(pGroup->nrec)));
// 	
// 	//m_strCodeList = _T("");
// 	
// 	for (int i = 0; i < nCount; i++)
// 	{
// 		struct _pfcode* pCode = &pGroup->grid[i];
// 		strCode = CString(pCode->code, sizeof(pCode->code));
// 		strCode.TrimLeft();
// 		strCode.TrimRight();
// 		szItemList.AddTail(strCode);
// 	}
//END

	char* pdata = (char*)lParam;

	CString strseq = CString(pdata, 2);
	CString strgname = CString(&pdata[2], 20);
	int cnt = atoi(CString(&pdata[22], 4));
	int parseL = 26;

	struct _jinfo {
		char gubn[1]; // 종목구분 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
		char code[12]; // 종목코드
		char xprc[10]; // 보유단가
		char xnum[10]; // 보유수량
	};

	struct _jinfo* jinfo;
	char* pgubn, * pCode, * pxprc, * pxnum;
	pgubn = new char[sizeof(jinfo->gubn) + 1];
	pCode = new char[sizeof(jinfo->code) + 1];

	for (int ii = 0; ii < cnt; ii++)
	{
		jinfo = (struct _jinfo*)&pdata[parseL];
		memset(pgubn, 0x00, sizeof(jinfo->gubn) + 1);
		memcpy(pgubn, jinfo->gubn, sizeof(jinfo->gubn));  // 종목구분 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
		memset(pCode, 0x00, sizeof(jinfo->code) + 1);
		memcpy(pCode, jinfo->code, sizeof(jinfo->code));  // 종목코드[12]

		szItemList.AddTail(CString(pCode));

		parseL += sizeof(struct _jinfo);
	}

	delete[] pgubn; pgubn = nullptr;
	delete[] pCode; pCode = nullptr;

	return szItemList.GetCount();
}

int CAUGwanManager::ParseGroup(LPARAM lParam, CStringArray &arrKey, CStringArray &arrName)
{
	char* pdata = (char*)lParam;
	int cnt = atoi(CString(pdata, 4));
	int parseL = 4;

	struct _grSearch
	{
		char ngrs[2];
		char grseq[2];
		char gname[30];
	};

	struct _grSearch* stgr;
	char* pSeq, * pgame;
	pSeq = new char[sizeof(stgr->grseq) + 1];
	pgame = new char[sizeof(stgr->gname) + 1];

	m_GwanSimGroup.strGroupName.RemoveAll();
	m_GwanSimGroup.strKey.RemoveAll();

	for (int ii = 0; ii < cnt; ii++)
	{
		stgr = (struct _grSearch*)&pdata[parseL];
		memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
		memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
		arrKey.Add(pSeq);
		m_GwanSimGroup.strKey.Add(pSeq);
		
		memset(pgame, 0x00, sizeof(stgr->gname) + 1);
		memcpy(pgame, stgr->gname, sizeof(stgr->gname));

		m_GwanSimGroup.strGroupName.Add(pgame);
		arrName.Add(pgame);

		parseL += sizeof(struct _grSearch);

	}

	delete[] pSeq; pSeq = nullptr;
	delete[] pgame; pgame = nullptr;

	return cnt;
}

