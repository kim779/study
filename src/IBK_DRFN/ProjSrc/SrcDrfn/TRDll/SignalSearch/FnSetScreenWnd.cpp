// FnSetScreenWnd.cpp: implementation of the FnSetScreenWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FnSetScreenWnd.h"
#include "ScreenWnd.h"

#include "../DR_Include/hana_typedef.h"
#include "../../../SrcSite/include/axisfire.h"
#include "../../../SrcSite/include/interSt.h"
#include "../DR_Include/MarketCode.h"

#define MAX_LINE 100

#define KOSPI_MAKRET_CNT	42
#define KOSDAQ_MARKET_CNT	46
#define KOSPI200_MARKET_CNT	9
#define	KRX_MARKET_CNT		1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFnSetScreenWnd::CFnSetScreenWnd()
{
	m_plistKOSPI200 = NULL;
	m_plistKRX = NULL;
	m_hCheckVersion = NULL;
	m_nCount = 0;
}

CFnSetScreenWnd::~CFnSetScreenWnd()
{
	POSITION pos;
	CList <hjcodex, hjcodex&>* plistGroup;
	CString strKey;
	for(pos = m_mapKospiStockMarketGruop.GetStartPosition(); pos != NULL;)
	{
		m_mapKospiStockMarketGruop.GetNextAssoc(pos, strKey, (void*&)plistGroup);
		plistGroup->RemoveAll();
		delete plistGroup;
		plistGroup = NULL;
		
		m_mapKospiStockMarketGruop.RemoveKey(strKey);
	}
	m_mapKospiStockMarketGruop.RemoveAll();
	
	for(pos = m_mapKosdaqStockMarketGroup.GetStartPosition(); pos != NULL;)
	{
		m_mapKosdaqStockMarketGroup.GetNextAssoc(pos, strKey, (void*&)plistGroup);
		plistGroup->RemoveAll();
		delete plistGroup;
		plistGroup = NULL;
		
		m_mapKosdaqStockMarketGroup.RemoveKey(strKey);
	}
	m_mapKosdaqStockMarketGroup.RemoveAll();
	
	if(m_plistKOSPI200)
	{
		m_plistKOSPI200->RemoveAll();
		delete m_plistKOSPI200;
		m_plistKOSPI200 = NULL;
	}
	
	if(m_plistKRX)
	{
		m_plistKRX->RemoveAll();
		delete m_plistKRX;
		m_plistKRX = NULL;
	}

	if(m_hCheckVersion)
	{
		Lib_Close(this);
		FreeLibrary(m_hCheckVersion);
		m_hCheckVersion = NULL;
	}
}

BEGIN_MESSAGE_MAP(CFnSetScreenWnd, CWnd)
//{{AFX_MSG_MAP(CScreenWnd)
	ON_WM_DESTROY()
//}}AFX_MSG_MAP
// 	ON_MESSAGE(WM_MANAGE, OnManage)
// 	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

void CFnSetScreenWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

BOOL CFnSetScreenWnd::SetInit(CWnd* pWndModuleMain, CWnd* pWndScreenWnd)
{
	if(!pWndModuleMain)
		return FALSE;

	if(!pWndScreenWnd)
		return FALSE;
	
	m_pWndModuleMain = pWndModuleMain;
	m_pWndScreen = pWndScreenWnd;

	DoClearDataFiles();

	m_pFontDef = GetAxFont(FONT_TYPE, 9, false, false);
	m_pBoldFont = GetAxFont(FONT_TYPE, 9, true, false);
	
	m_clrPalete = GetAxColor(64);

	SetUpMasterData();
	SetStockMasterData();

	LoadVersionManager();

	return TRUE;
}

CFont* CFnSetScreenWnd::GetAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;
	
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)m_pWndModuleMain->SendMessage(WM_USER, getFONT, (long)&fontR);
}

COLORREF CFnSetScreenWnd::GetAxColor(int nColor)
{
	if (nColor & 0x02000000)
		return nColor;
	
	return m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)nColor);
}

CString CFnSetScreenWnd::Variant(int nComm, CString strData)
{
	CString strRetValue = _T("");
	
	if(m_pWndModuleMain)
	{
		if(nComm == nameCC)
		{
			if(!m_strUserName.IsEmpty())
				return m_strUserName;
			
			LPCSTR strTemp;
			strTemp = (char*)m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), (LPARAM)(LPCTSTR)strData);
		//	m_strUserName.Format("%08u", HashKey(strTemp));
			m_strUserName.Format("%08u", HashDataAXIS(strTemp));
			
			return m_strUserName;
		}
		else
		{
			char* dta;
			dta = (char*)m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);
			
			if((long)dta > 1)
				strRetValue.Format("%s", dta);	
		}
	}
	
	return strRetValue;
}

void CFnSetScreenWnd::SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold)
{
	CString strHome = Variant(homeCC);
	
	HBITMAP hBmpBtn, hBmpBtnDN, hBmpBtnEn;
	CString strPath, strImgDir;
	strImgDir = "image";
	strPath.Format("%s\\%s\\%d%s", strHome, strImgDir, nTextLen, "BTN.BMP");	    
	hBmpBtn = GetBitmap(strPath);
	strPath.Format("%s\\%s\\%d%s", strHome, strImgDir, nTextLen, "BTN_DN.BMP");	
	hBmpBtnDN = GetBitmap(strPath);
	strPath.Format("%s\\%s\\%d%s", strHome, strImgDir, nTextLen, "BTN_EN.BMP");		
	hBmpBtnEn = GetBitmap(strPath);
	pBtn->SetImgBitmap(hBmpBtn, hBmpBtnDN, hBmpBtnEn);
	
	pBtn->SetFont((bBold == TRUE) ? GetBoldFont() :GetDefFont());
}

void CFnSetScreenWnd::SetBtnImg(CfxImgButton *pBtn, CString strFileName)
{
	CString strHome = Variant(homeCC);
	
	HBITMAP hBmpBtn, hBmpBtnDN, hBmpBtnEn;
	CString strPath, strImgDir;
	strImgDir = "image";
	strPath.Format("%s\\%s\\%s%s", strHome, strImgDir, strFileName, ".BMP");	    
	hBmpBtn = GetBitmap(strPath);
	strPath.Format("%s\\%s\\%s%s",   strHome, strImgDir, strFileName, "_DN.BMP");	
	hBmpBtnDN = GetBitmap(strPath);
	strPath.Format("%s\\%s\\%s%s", strHome, strImgDir, strFileName, "_EN.BMP");		
	hBmpBtnEn = GetBitmap(strPath);
	pBtn->SetImgBitmap(hBmpBtn, hBmpBtnDN, hBmpBtnEn);
	
	pBtn->SetFont(GetDefFont(), false);
}

HBITMAP CFnSetScreenWnd::GetBitmap(CString strPath)
{
	CBitmap* pBitmap = (CBitmap*)m_pWndModuleMain->SendMessage(WM_USER, getBITMAP, (LPARAM)strPath.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

CBrush* CFnSetScreenWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pWndModuleMain->SendMessage(WM_USER, getBRUSH, (long)clr);
}

HCURSOR CFnSetScreenWnd::GetCursor(int nKind)
{
	_ASSERT(m_pWndModuleMain);
	return (HCURSOR)m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(getCURSOR, 0), (LPARAM)nKind);
}

void CFnSetScreenWnd::SetPalete()
{
	m_clrPalete = GetAxColor(66);
	
	ST_TABCOLOR stTabColor;
	stTabColor.clrF = GetAxColor(63);
	stTabColor.clrB = GetAxColor(22);
	stTabColor.clrSF = GetAxColor(26);
	stTabColor.clrSB = GetAxColor(23);
	stTabColor.clrBACK = GetAxColor(181);
	
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pWndScreen;
	pScreenWnd->SetPalete(m_clrPalete, stTabColor);
}

void CFnSetScreenWnd::SendTR(CString strTRCode, char* szSendData, int nDataLen, int nKey, char cStat)
{
	char* szSendBuf = new char[L_userTH + nDataLen + 1];
	memset(szSendBuf, NULL, sizeof(szSendBuf));
	
	int nPos = 0;
	
	_userTH stHeader;
	memcpy(&stHeader, strTRCode, sizeof(stHeader.trc));
	stHeader.key = nKey;
	stHeader.stat = cStat;
	
	memcpy(szSendBuf + nPos, &stHeader, sizeof(_userTH));
	nPos += sizeof(_userTH);
	
	memcpy(szSendBuf + nPos, szSendData, nDataLen);
	
	if(strTRCode == TRNO_MSTCODE || strTRCode == TRNO_WORKDATE)
	{
		if(!DoPreCheck(strTRCode))
			m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nDataLen), (LPARAM)szSendBuf);
	}
	else
		m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nDataLen), (LPARAM)szSendBuf);
	
	delete szSendBuf;
}

LONG CFnSetScreenWnd::ReceiveTR(WPARAM wParam, LPARAM lParam)
{
	LONG lRet = 0;
	int nKind = (HIBYTE(LOWORD(wParam)));
	
	CScreenWnd* pScreenWnd = (CScreenWnd*)m_pWndScreen;
	switch(nKind)
	{
	case TRKEY_MSTCODE :
		DoPreReceiveData(TRNO_MSTCODE, (LPVOID)lParam, (long)HIWORD(wParam));
		pScreenWnd->ReceiveTR(TRNO_MSTCODE, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	case TRKEY_WORKDATE :
		DoPreReceiveData(TRNO_WORKDATE, (LPVOID)lParam, (long)HIWORD(wParam));
		pScreenWnd->ReceiveTR(TRNO_WORKDATE, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	case TRKEY_CONDITION :
		pScreenWnd->ReceiveTR(TRNO_CONDITION, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	case TRKEY_LANGUAGE :
		pScreenWnd->ReceiveTR(TRNO_LANGUAGE, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	case TRKEY_CONDIONECLK_RESULT :
		pScreenWnd->ReceiveTR(TRNO_CONDIONECLK_RESULT, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	case TRKEY_ALERT_REALREG :
		pScreenWnd->ReceiveTR(TRNO_ALERT_REALREG, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	case TRKEY_DRAW_RESULT :
		pScreenWnd->ReceiveTR(TRNO_DRAW_RESULT, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	case TRKEY_GROUPTOBASE_UP :
		pScreenWnd->ReceiveTR(TRNO_GROUPTOBASE_UP, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	case TRKEY_GROUPTOBASE :
		pScreenWnd->ReceiveTR(TRNO_GROUPTOBASE, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	case TRKEY_SET_REAL :
		pScreenWnd->ReceiveTR(TRNO_SET_REAL, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	case TRKEY_VALUE_PREDICTION :
		pScreenWnd->ReceiveTR(TRNO_VALUE_PREDICTION, (LPVOID)lParam, (long)HIWORD(wParam));
		break;
	}
	
	return lRet;
}

void CFnSetScreenWnd::DoClearDataFiles()
{
	CString strDataDir;
	strDataDir.Format("%s%s\\%s", Variant(homeCC), FOLDER_DATA, gszCacheDir);
	CFileFind ff;
	if(!ff.FindFile(strDataDir))
		CreateDirectory(strDataDir, NULL);
	
	CString strIni;
	strIni.Format("%s\\%s", strDataDir, "RequestToday.ini");
	
	CTime tm = CTime::GetCurrentTime();
	CString strKey;
	strKey.Format("%04d%02d%02d", tm.GetYear(), tm.GetMonth(), tm.GetDay());
	
	CString strFilePath;
	bool bSkip = false;
	char aBuff[ 30+1 ]={0,};	
	{
		bSkip = false;
		if(::GetPrivateProfileString(gszDataKey_MstCode, "Today", "", aBuff, 30, strIni) > 0)
		{
			if(strKey.Compare(aBuff) == 0) bSkip = true;
		}

		int nVer = ::GetPrivateProfileInt(gszDataKey_MstCode, "Version", 0, strIni);
		if (nVer < COND_MST_VER)
			bSkip = false;

		if(bSkip == false)
		{
			::WritePrivateProfileString(gszDataKey_MstCode, "Today", "", strIni);
			strFilePath.Format("%s%s", strDataDir, "\\MstCode.dat");
			DoFileRemove(strFilePath);
		}
	}
	
	{
		bSkip = false;
		if(::GetPrivateProfileString(gszDataKey_WorkDate, "Today", "", aBuff, 30, strIni) > 0)
		{
			if(strKey.Compare(aBuff) == 0) bSkip = true;
		}
		if(bSkip == false)
		{
			::WritePrivateProfileString(gszDataKey_WorkDate, "Today", "", strIni);
			strFilePath.Format("%s%s", strDataDir, "\\WorkDate.dat");
			DoFileRemove(strFilePath);
		}
	}
}

BOOL CFnSetScreenWnd::DoFileRemove(LPCSTR pszSrc)
{
	BOOL bRtn = ::DeleteFile(pszSrc);
	return bRtn;
}

void CFnSetScreenWnd::DoPreReceiveData(CString strTRCode, LPVOID pData, int nBodyLen)
{
	CString strDataDir;
	strDataDir.Format("%s%s\\%s", Variant(homeCC), FOLDER_DATA, gszCacheDir);
	
	CString strIni;
	strIni.Format("%s\\%s", strDataDir, "RequestToday.ini");
	
	CTime tm = CTime::GetCurrentTime();
	CString strKey;
	strKey.Format("%04d%02d%02d", tm.GetYear(), tm.GetMonth(), tm.GetDay());
	
	CFileFind ff;
	if(!ff.FindFile(strDataDir))
		CreateDirectory(strDataDir, NULL);
	
	char buf[8];
	memset(buf, 0x00, sizeof(buf));
	memcpy(buf, pData, 4);
	int nTotCnt = atoi(buf);
	
	if(strTRCode == TRNO_MSTCODE)
	{
		CString strFile;
		strFile.Format("%s\\%s", strDataDir, "MstCode.dat");
				
		nBodyLen = nTotCnt * sizeof(MSTCode) + 4;

		FILE* fp = fopen(strFile, "w");
		if(fp)
		{
			fwrite(pData, 1, nBodyLen, fp);
			fclose(fp);
			
			::WritePrivateProfileString(gszDataKey_MstCode, "Today", strKey, strIni);
			CString strVer;
			strVer.Format(_T("%05d"), COND_MST_VER);
			::WritePrivateProfileString(gszDataKey_MstCode, "Version", strVer, strIni);
		}
	}
	else if(strTRCode == TRNO_WORKDATE)
	{
		CString strFile;
		strFile.Format("%s\\%s", strDataDir, "WorkDate.dat");
		
		FILE* fp = fopen(strFile, "w");
		if(fp)
		{
			fwrite(pData, 1, nBodyLen, fp);
			fclose(fp);
			
			::WritePrivateProfileString(gszDataKey_WorkDate, "Today", strKey, strIni);
		}
	}
}

int CFnSetScreenWnd::DoPreCheck(CString strTRCode)
{
	if(strcmp(strTRCode, TRNO_MSTCODE) == 0)
		return DoPreCheck_MSTCode();
	else if(strcmp(strTRCode, TRNO_WORKDATE) == 0)
		return DoPreCheck_WorkDate();
	
	return 0;
}

int CFnSetScreenWnd::DoPreCheck_MSTCode()
{
	CString strDataFile;
	strDataFile.Format("%s%s\\%s\\%s", Variant(homeCC), FOLDER_DATA, gszCacheDir, "MstCode.dat");
	
	{
		CFileFind find;
		if(find.FindFile(strDataFile) == FALSE)
			return 0;
	}
	
	CFile file;
	if(file.Open(strDataFile, CFile::modeRead | CFile::typeBinary)==TRUE)
	{
		long nDataLen = file.GetLength();
		if(nDataLen > 0)
		{
			CString rValue;
			rValue.GetBufferSetLength(nDataLen);
			file.Read((char*)(LPCSTR)rValue, nDataLen);
			file.Close();			
			rValue.ReleaseBuffer();

			LPBYTE pDataBuf = (LPBYTE)(LPCSTR)rValue;
			
			CScreenWnd* pScreenWnd = (CScreenWnd*)m_pWndScreen;
			pScreenWnd->ReceiveTR(TRNO_MSTCODE, pDataBuf, nDataLen);
			
			return 1;
		}
	}
	
	return 0;
}

int CFnSetScreenWnd::DoPreCheck_WorkDate()
{
	CString strDataFile;
	strDataFile.Format("%s%s\\%s\\%s", Variant(homeCC), FOLDER_DATA, gszCacheDir, "\\WorkDate.dat");
	{
		CFileFind find;
		if(find.FindFile(strDataFile) == FALSE)
			return 0;
	}
	
	CFile file;
	if(file.Open(strDataFile, CFile::modeRead | CFile::typeBinary) == TRUE)
	{
		long nDataLen = file.GetLength();
		if(nDataLen>0)
		{
			CString rValue;
			rValue.GetBufferSetLength(nDataLen);
			file.Read((char*)(LPCSTR)rValue, nDataLen);
			file.Close();			
			rValue.ReleaseBuffer();

			LPBYTE pDataBuf = (LPBYTE)(LPCSTR)rValue;
		
			CScreenWnd* pScreenWnd = (CScreenWnd*)m_pWndScreen;
			pScreenWnd->ReceiveTR(TRNO_WORKDATE, pDataBuf, nDataLen);
			
			return 1;
		}
	}
	
	return 0;
}

int CFnSetScreenWnd::SetUserMaster(LPCSTR szKey, int nCount, CStringArray *pArrCode, CStringArray *pArrName)
{
	CString szDataFile;
	
	if(strcmp(szKey, usrMasterCondition)==0)
	{
		szDataFile.Format("%s%s%s\\%s\\%s", Variant(homeCC), USRDIR, Variant(nameCC), "FindData", "findTemp.dat");
		
		CFile file;
		if(file.Open(szDataFile, CFile::modeWrite|CFile::modeCreate|CFile::typeBinary)==TRUE)
		{
			CArchive ar(&file, CArchive::store);
			
			ar << nCount;
			
			pArrCode->Serialize(ar);
			pArrName->Serialize(ar);
			
			ar.Close();
			file.Close();
		}
	}
	
	return 0;
}

int CFnSetScreenWnd::GetUserMaster(LPCSTR szKey, CStringArray &arrCode, CStringArray &arrName)
{
	CString szDataFile;
	
	if(strcmp(szKey, usrMasterCondition)==0)
	{
		szDataFile.Format("%s%s%s\\%s", Variant(homeCC), USRDIR, Variant(nameCC), "FindData", "findTemp.dat");
		
		CFile file;
		if(!file.Open(szDataFile, CFile::typeBinary|CFile::modeRead))
			return 0;
		
		int nCount = 0;
		
		CArchive ar(&file, CArchive::load);
		ar >> nCount;
		arrCode.Serialize(ar);
		arrName.Serialize(ar);
		file.Close();
		
		return nCount;	
	}
	
	return 0;
}

void CFnSetScreenWnd::DoParsingCode(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName, bool bInsert, int nStartIdx)
{
	if(szCodeList.GetLength()==0) return;
	
	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	
	int nDataLen = szCodeList.GetLength();
	if(nStartIdx==0)
	{
		while(nPos1>=0 && nOffset<=nDataLen)
		{
			nPos1 = szCodeList.Find('\n', nOffset);
			if(nPos1<0) break;
			
			szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
			nOffset = (nPos1+1);
			
			nPos2 = szTmp.Find('\t');
			if(nPos2<0) break;
			szCode		= szTmp.Left(nPos2);
			if(szCode.Left(1) == "A") szCode = szCode.Mid(1);
			szCodeName	= szTmp.Mid(nPos2+1);
			
			szCodeName.TrimLeft();
			if(bInsert)
			{
				arrCode.InsertAt(0, szCode);
				arrName.InsertAt(0, szCodeName);
			}
			else
			{
				arrCode.Add(szCode);
				arrName.Add(szCodeName);
			}
		}
	}
	else if(nStartIdx==1)
	{
		while(nPos1>=0 && nOffset<=nDataLen)
		{
			nPos1 = szCodeList.Find('\n', nOffset);
			if(nPos1<0) break;
			
			szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
			nOffset = (nPos1+1);
			
			nPos2 = szTmp.Find('\t');
			if(nPos2<0) break;
			szCode		= szTmp.Left(nPos2);
			if(szCode.Left(1) == "A") szCode = szCode.Mid(1);
			szCodeName	= szTmp.Mid(nPos2+1);
			if(szCodeName[0]=='#' ||szCodeName[0]=='&')
				szCodeName = szCodeName.Mid(1);
			
			if(bInsert)
			{
				arrCode.InsertAt(0, szCode);
				arrName.InsertAt(0, szCodeName);
			}
			else
			{
				arrCode.Add(szCode);
				arrName.Add(szCodeName);
			}
		}
	}
}

BOOL CFnSetScreenWnd::GetKSPUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();
	
// 	struct _codeR codeR;
// 	ZeroMemory(codeR.code, sizeof(codeR.code));
// 	if(nOption==0)	codeR.kind = CDKIND_KOSPI;
// 	else			codeR.kind = CDKIND_INDEX_KOSPI;
// 	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
// 	CString strRet = (char*)(m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR));
// 	
// 	DoParsingCode(strRet, *pastrUpCode, *pastrUpName);
	
	return TRUE;
}

BOOL CFnSetScreenWnd::GetKSQUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

// 	struct _codeR codeR;
// 	ZeroMemory(codeR.code, sizeof(codeR.code));
// 	if(nOption==0)	codeR.kind = CDKIND_KOSDAQ;
// 	else			codeR.kind = CDKIND_INDEX_KOSDAQ;
// 	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
// 	CString strRet = (char*)(m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR));
// 	
// 	DoParsingCode(strRet, *pastrUpCode, *pastrUpName);
	
	return TRUE;
}

void CFnSetScreenWnd::OpenScreen(CString strScreenNo, CStringArray* pSaCode, CString strOtherData)
{
	if(strScreenNo.IsEmpty())
		return;

	if(strScreenNo == "7112")
		strScreenNo = "IB711200";
	else if(strScreenNo == "2001")
		strScreenNo = "IB200100";
	else if(strScreenNo == "7129")
		strScreenNo = "IB712900";
	else if(strScreenNo == "관심등록")
	{
		RegPortItem(pSaCode);
		return;
	}
	else if(strScreenNo == "7128")
		strScreenNo = "IB712800";
	else if(strScreenNo == "7134")
		strScreenNo = "IB713400";
	
	CString strSendData;
	CString strCode;
	if(pSaCode)
		strCode	= pSaCode->GetAt(0);
	if(strCode.IsEmpty())
		strSendData = strScreenNo;
	else
		strSendData.Format("%s /t/s/p5/d1301\t%s", strScreenNo, strCode);
	
	if(!strOtherData.IsEmpty())
		strSendData += "\n" + strOtherData;
	
	m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strSendData);
}

CString CFnSetScreenWnd::Parser(CString &sSrc, CString sDelimiter)
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

void CFnSetScreenWnd::GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName)
{
	CString sFile;
	sFile.Format("%s%s%s%s", Variant(homeCC), USRDIR, Variant(nameCC), FILE_GROUP);	
	
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

		saKey.Add(strGNo);
		saGroupName.Add(strGName);
	}
}

BOOL CFnSetScreenWnd::GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey)
{
	BOOL bResult = FALSE;
	saCode.RemoveAll();
	saName.RemoveAll();
	
	CString	strFilePath;
	strFilePath.Format("%s%s%s//portfolio.i%02d", Variant(homeCC), USRDIR, Variant(nameCC), nKey);
	
	DWORD	fLen = 0;
	CFileFind finder;
	if (!finder.FindFile(strFilePath))
		return bResult;
	
	UINT	readL;
	CString	code = _T(""), amount = _T(""), price = _T(""), name = _T("");
	struct	_inters interest;
	
	CFileStatus fs;
	if (CFile::GetStatus(strFilePath, fs) == FALSE)
		return bResult;
	
	CFile	fileH(strFilePath, CFile::modeRead);
	if (fileH.m_hFile == CFile::hFileNull)
		return bResult;
	
	for (int nIndex = 0; nIndex < MAX_LINE ; nIndex++)
	{
		readL = fileH.Read(&interest, sizeof(struct _inters));
		if (readL < sizeof(struct _inters))
			break;
		
		code.Format("%.*s", sizeof(interest.code), interest.code);
		code.TrimLeft(), code.TrimRight();
		name.Format("%.*s", sizeof(interest.name), interest.name);		
		name.TrimLeft(), name.TrimRight();

		if(name.IsEmpty())
			continue;

		saCode.Add(code);
		saName.Add(name);
		bResult = TRUE;
	}
	fileH.Close();
	
	return bResult;
}

void CFnSetScreenWnd::RegPortItem(CStringArray* pSaItemCode)
{
	if(pSaItemCode->GetSize() <= 0)
		return;

	CString strCodeList;
	int nSize = pSaItemCode->GetSize();
	for(int i = 0; i < nSize; i++)
	{
		CString strBuf;
		strBuf.Format("%s%c", pSaItemCode->GetAt(i), CH_TAB);
		strCodeList = strCodeList + strBuf;
	}

	CString strGroupName;
	strGroupName = "";
	
	CString strData;
	strData.Format("%s%c%s", strGroupName, CH_TAB, strCodeList);

	CString sParam;
	sParam.Format("%s /p5 /S/d%s%s%c", "IB202201", "appendITEMS", strData, CH_LF);
	m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUP), (LPARAM)(LPCTSTR)sParam);
}

void CFnSetScreenWnd::ShareCode(CString strCode)
{
	CString strShareCode;
	strShareCode.Format("%s\t%s", "1301", strCode);
	Variant(triggerCC, strShareCode);
}

void CFnSetScreenWnd::ReceiveReal(WPARAM wParam, LPARAM lParam)
{
	struct _alertR* alertR = (struct _alertR*)lParam;
	CString strKey = alertR->code;

	if(strKey == "FNDS")
	{
		DWORD* pArrData = NULL;
		CString strTemp, strKey, strData;
		CScreenWnd* pScreenWnd = (CScreenWnd*)m_pWndScreen;

		if(m_nCount == 0)
			m_strSendData = "FNDS";

		for (int i=alertR->size-1; i>=0; i--)
		{
			pArrData = (DWORD*)alertR->ptr[i];
			strTemp = (char*)pArrData[270];
			strData.Format("%s", strTemp.Mid(12));
			strKey.Format("%s", strTemp.Left(12));

			if(strKey.Left(1) == "2")
			{
				m_strSendData += strData;
				pScreenWnd->ReceiveReal(wParam, (LPARAM)(LPCSTR)m_strSendData);
				m_strSendData = "FNDS";
				m_nCount = 0;
				continue;
			}

			m_strSendData += strData;
			m_nCount++;
		}
	}
	else if(strKey.Left(1) == "A")
	{
		char* pszData = (char*) lParam;
		UINT uDataSize = (int) HIWORD(wParam);
		CScreenWnd* pScreenWnd = (CScreenWnd*)m_pWndScreen;
		// 박성경
		//pScreenWnd->ReceiveChartReal(pszData, uDataSize, wParam);
	}
}

CString CFnSetScreenWnd::PeekCode(CString strData)
{
	CString strFirst = Parser(strData, "\n");
	
	if(strFirst.GetLength())
	{
		CString strCode = Parser(strFirst, char(CH_TAB));
		return strCode;
	}
	
	return "";
}

CString CFnSetScreenWnd::GetFormCode()
{
	return (LPCTSTR)m_pWndModuleMain->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 0));
}

void CFnSetScreenWnd::SetStockMasterData()
{
	CFile	fileH;
	CString	filePath, strTemp;
	hjcodex	HJCode;
	
	filePath.Format("%s/%s/%s", Variant(homeCC), "tab", "hjcode3.dat");
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("종목코드 파일을 읽을 수 없습니다.");
		return;
	}
	
	int nCount = fileH.GetLength() / sizeof(HJCode);

	CList <hjcodex, hjcodex&>* plistKospiStock = new CList <hjcodex, hjcodex&>;
	CList <hjcodex, hjcodex&>* plistKosdaqStock = new CList <hjcodex, hjcodex&>;

	m_plistKOSPI200 = new CList <hjcodex, hjcodex&>;
	m_plistKRX = new CList<hjcodex, hjcodex&>;
	
	for (int i = 0; i < nCount; i++)
	{
		fileH.Read(&HJCode, sizeof(HJCode));
		strTemp = HJCode.code;
		if(strTemp.Left(1) == "A")
		{
			strTemp = HJCode.code;
			memset(HJCode.code, 0x00, sizeof(HJCode.code));
			memcpy(HJCode.code, strTemp.Mid(1,6), 6);
			
			if((UINT)HJCode.kosd == 0)			// kospi
			{
				plistKospiStock->AddTail(HJCode);
			}
			else if((UINT)HJCode.kosd == 10)	// kosdaq
			{
				plistKosdaqStock->AddTail(HJCode);
			}
			
			if(((UINT)HJCode.kpgb == 1 || (UINT)HJCode.kpgb == 2 || (UINT)HJCode.kpgb == 3) && (UINT)HJCode.kosd != 10)		// kospi200
			{
				m_plistKOSPI200->AddTail(HJCode);
			}
			
			if((UINT)HJCode.unio == 1)		// krx
			{
				m_plistKRX->AddTail(HJCode);
			}
 		}
	}
	fileH.Close();

	SetMakrketGroup(plistKospiStock, plistKosdaqStock);

	plistKospiStock->RemoveAll();
	delete plistKospiStock;
	plistKospiStock = NULL;

	plistKosdaqStock->RemoveAll();
	delete plistKosdaqStock;
	plistKosdaqStock = NULL;
}

void CFnSetScreenWnd::SetMakrketGroup(CList <hjcodex, hjcodex&>* plistKospiStock,
									  CList <hjcodex, hjcodex&>* plistKosdaqStock)
{
	CString strMaketCode;

	CList <hjcodex, hjcodex&>* plistGroupKospi[KOSPI_MAKRET_CNT];
	int i = 0;
	for( i = 0; i < KOSPI_MAKRET_CNT; i++)
		plistGroupKospi[i] = new CList<hjcodex, hjcodex&>;
	
	hjcodex JCode;
	POSITION pos;
	for(i = 0; i <  plistKospiStock->GetCount(); i++)
	{
		pos = plistKospiStock->FindIndex(i);
		JCode = plistKospiStock->GetAt(pos);
		
		if((UINT)JCode.kosd == 0)
		{
			CString strCode;
			strCode.Format("%s", JCode.code);
			strCode = strCode.Mid(0, 6);
			if(strCode == "004370" || strCode == "097950" || strCode == "078930" || strCode == "002380" || strCode == "033780" ||
				strCode == "003550" || strCode == "034220" || strCode == "002550" || strCode == "010120" || strCode == "005490")
				int ss = 0;
			
			for(int j = 0; j < KOSPI_MAKRET_CNT; j++)
			{
				int nMarketCode = CheckKospiStock(j + 1, JCode);
				if(nMarketCode == -1 || nMarketCode == 0)
					continue;
				
				plistGroupKospi[nMarketCode - 1]->AddTail(JCode);
				
			}
		}
		
	}
	
	for(i = 0; i < KOSPI_MAKRET_CNT; i++)
	{
		strMaketCode.Format("0%2d", i + 1);
		strMaketCode.Replace(' ', '0');
		m_mapKospiStockMarketGruop.SetAt(strMaketCode, plistGroupKospi[i]);
	}

	CList <hjcodex, hjcodex&>* plistGroupKosdaq[KOSDAQ_MARKET_CNT];
	for(i = 0; i < KOSDAQ_MARKET_CNT; i++)
		plistGroupKosdaq[i] = new CList<hjcodex, hjcodex&>;
	
	
	for(i = 0; i < plistKosdaqStock->GetCount(); i++)
	{
		pos = plistKosdaqStock->FindIndex(i);
		JCode = plistKosdaqStock->GetAt(pos);
		if((UINT)JCode.kosd == 10)
		{
			for(int j = 0; j < KOSDAQ_MARKET_CNT; j++)
			{
				int nMarketCode = CheckKosdaqStock(j + 1, JCode);
				if(nMarketCode == -1 || nMarketCode == 0)
					continue;
				
				plistGroupKosdaq[nMarketCode - 1]->AddTail(JCode);
			}
		}
	}

	for(i = 0; i < KOSDAQ_MARKET_CNT; i++)
	{
		strMaketCode.Format("2%2d", i + 1);
		strMaketCode.Replace(' ', '0');
		m_mapKosdaqStockMarketGroup.SetAt(strMaketCode, plistGroupKosdaq[i]);
	}
}

BOOL CFnSetScreenWnd::GetStockMasterData(CStringArray &arrCode, CStringArray &arrName, UINT Jgub, UINT Upgub)
{
	CString strItemCode;
	CString	strItemName;
	hjcodex stJCode;

	CMapStringToPtr* pmapMarket = NULL;
	
	switch(Jgub)
	{
	case GETMARKET_ALL		:
		{
			CList<hjcodex, hjcodex&>* plist = NULL;
			
			CString strMarketCode = "001";
			pmapMarket = &m_mapKospiStockMarketGruop;
			if(pmapMarket->Lookup(strMarketCode, (void*&)plist))
			{
				POSITION pos;
				for(int i = 0; i < plist->GetCount(); i++)
				{
					pos = plist->FindIndex(i);
					stJCode = plist->GetAt(pos);
					
					strItemCode.Format("%s", stJCode.code);
					strItemCode = strItemCode.Mid(0, 6);
					
					strItemName.Format("%s", stJCode.hnam);
					strItemName = strItemName.Mid(0, 40);
					strItemName.TrimLeft();
					strItemName.TrimRight();
					
					arrCode.Add(strItemCode);
					arrName.Add(strItemName);
				}
			}

			strMarketCode = "201";
			pmapMarket = &m_mapKosdaqStockMarketGroup;
			if(pmapMarket->Lookup(strMarketCode, (void*&)plist))
			{
				POSITION pos;
				for(int i = 0; i < plist->GetCount(); i++)
				{
					pos = plist->FindIndex(i);
					stJCode = plist->GetAt(pos);
					
					strItemCode.Format("%s", stJCode.code);
					strItemCode = strItemCode.Mid(0, 6);
					
					strItemName.Format("%s", stJCode.hnam);
					strItemName = strItemName.Mid(0, 40);
					strItemName.TrimLeft();
					strItemName.TrimRight();
					
					arrCode.Add(strItemCode);
					arrName.Add(strItemName);
				}
			}
		}
	case GETMARKET_KOSPI	:	pmapMarket = &m_mapKospiStockMarketGruop;	break;
	case GETMARKET_KOSDAQ	:	pmapMarket = &m_mapKosdaqStockMarketGroup;	break;
	case GETMARKET_KOSPI200	:	
		{
			POSITION pos;
			for(int i = 0; i < m_plistKOSPI200->GetCount(); i++)
			{
				pos = m_plistKOSPI200->FindIndex(i);
				stJCode = m_plistKOSPI200->GetAt(pos);

				strItemCode.Format("%s", stJCode.code);
				strItemCode = strItemCode.Mid(0, 6);

				strItemName.Format("%s", stJCode.hnam);
				strItemName = strItemName.Mid(0, 40);
				strItemName.TrimLeft();
				strItemName.TrimRight();

				arrCode.Add(strItemCode);
				arrName.Add(strItemName);
			}
			
			return TRUE;
		}
		
	case GETMARKET_KRX		:	
		{
			POSITION pos;
			for(int i = 0; i < m_plistKRX->GetCount(); i++)
			{
				pos = m_plistKRX->FindIndex(i);
				stJCode = m_plistKRX->GetAt(pos);
				
				strItemCode.Format("%s", stJCode.code);
				strItemCode = strItemCode.Mid(0, 6);
				
				strItemName.Format("%s", stJCode.hnam);
				strItemName = strItemName.Mid(0, 40);
				strItemName.TrimLeft();
				strItemName.TrimRight();
				
				arrCode.Add(strItemCode);
				arrName.Add(strItemName);
			}
			
			return TRUE;
		}
	}
	
	CString strMarketCode;
	
	if(Upgub == 0)
	{
		Upgub = 1;
		
		if(GETMARKET_KOSPI == Jgub || GETMARKET_KOSDAQ == Jgub)
		{
			strMarketCode.Format("%3d", Jgub + Upgub);
			strMarketCode.Replace(' ', '0');
		}
	}
	else
	{
		strMarketCode.Format("%3d", Upgub);
		strMarketCode.Replace(' ', '0');
	}
	
	CList<hjcodex, hjcodex&>* plist = NULL;
	if(pmapMarket->Lookup(strMarketCode, (void*&)plist))
	{
		POSITION pos;
		for(int i = 0; i < plist->GetCount(); i++)
		{
			pos = plist->FindIndex(i);
			stJCode = plist->GetAt(pos);
			
			strItemCode.Format("%s", stJCode.code);
			strItemCode = strItemCode.Mid(0, 6);
			
			strItemName.Format("%s", stJCode.hnam);
			strItemName = strItemName.Mid(0, 40);
			strItemName.TrimLeft();
			strItemName.TrimRight();
			
			arrCode.Add(strItemCode);
			arrName.Add(strItemName);
		}
	}
	
	return TRUE;
}

int CFnSetScreenWnd::CheckKospiStock(UINT Upgub, hjcodex JCode)
{
	/*	<Upgub>
		1:KOSPI(종합)	2:대형주		3:중형주
		4:소형주		5:음식료업		6:섬유의복
		7:종이목재		8:화학			9:의약품
		10:비금속광물	11:철강금속		12:기계
		13:전기전자		14:의료정밀		15:운수장비
		16:유통업		17:전기가스		18:건설업
		19:운수창고		20:통신업		21:금융업
		22:은행			24:증권			25:보험업
		26:서비스업		27:제조업		41:배당지수
		42:지배구조지수
	*/

	switch(JCode.ssgb)
	{
	case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
		break;
	default:
		if(Upgub == 1)
		{
			return Upgub;
		}
		if(Upgub < 5)
		{
			if((UINT)JCode.size == Upgub)
				return Upgub;
		}

		else if(Upgub < 27)
		{
#if(0)
			if((UINT)JCode.ucds == Upgub || (UINT)JCode.ucdm == Upgub)
#else
			if (strncmp(g_szKospi[Upgub], JCode.acds, 6) == 0 ||
				strncmp(g_szKospi[Upgub], JCode.acdm, 6) == 0)
#endif
				return Upgub;
		}
		else if(Upgub == 27)
		{
			if((UINT)JCode.jjug == Upgub)
				return Upgub;
		}
		else if(Upgub < 41)
		{
			if((UINT)JCode.jsiz == Upgub)
				return Upgub;
		}
		else if(Upgub == 41)
		{
			if((UINT)JCode.jsiz == Upgub)
				return Upgub;
		}
		else if(Upgub == 42)
		{
			if((UINT)JCode.wsgb == 1)
				return Upgub;
		}
		break;
	}

	return -1;
}

int CFnSetScreenWnd::CheckKosdaqStock(UINT Upgub, hjcodex JCode)
{
	/*	<Upgub>
		1:KODAQ(종합)		3:기타서비스	4:IT종합
		6:제조				7:건설			8:유통
		9:숙박/음식			10:운송			11:금융
		12:통신/방송		13:IT S/W		14:IT H/W
		15:음식료 담배		16:섬유/의류	17:종이/목재
		18:출판매체			19:화학			20:제약
		21:비금속			22:금속			23:기계장비
		24:전기/전자		25:의료/정밀	26:운송/부품
		27:기타제조			28:통신서비스	29:방송서비스
		30:인터넷			31:디지털		32:소프트웨어
		33:컴퓨터서비스		34:통신장비		35:정보기기
		36:반도체			37:IT 부품		38:KOSDAQ100
		39:KOSDAQ MID 300	40:KOSDAQ SMALL	41:오락/문화
		42:프리미어			43:우량기업		44:벤처기업
		45:중견기업			46:신성장기업
	*/

	if (JCode.kosd != jmKOSDAQ)
		return -1;

	switch (JCode.ssgb)
	{
	case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
		break;
	default:
		if(Upgub == 1)
		{
			return Upgub;
		}
		if(Upgub == 2)
		{
			if((UINT)JCode.jjug == Upgub)
				return Upgub;
		}
		if(Upgub < 5)
		{
			if((UINT)JCode.size == Upgub)
				return Upgub;
		}
		else if(Upgub == 5)
		{
			if((UINT)JCode.size == 4 && (UINT)JCode.jjug == 2)
				return Upgub;
		}
		else if(Upgub < 38 || Upgub == 41)//15~37
		{
			if (strncmp(g_szKosdaq[Upgub], JCode.acdm, 6) == 0 ||
				strncmp(g_szKosdaq[Upgub], JCode.acds, 6) == 0)
				return Upgub;
		}
		else if(Upgub < 41)
		{
			if((UINT)JCode.jsiz == Upgub - 37)
				return Upgub;
		}
		else if(Upgub == 42)
		{
			if((UINT)JCode.prmr == Upgub - 41)
				return Upgub;
		}
		else if(Upgub == 43)	// 우량기업
		{
			if ((UINT)JCode.uc20 == 1)
				return Upgub;
		}
		else if(Upgub == 44)	// 벤처기업
		{
			if ((UINT)JCode.uc20 == 2)
				return Upgub;
		}
		else if(Upgub == 45)	// 중견기업
		{
			if ((UINT)JCode.uc20 == 4)
				return Upgub;
		}
		else if(Upgub == 46)	// 신성장기업
		{
			if ((UINT)JCode.uc20 == 8)
				return Upgub;
		}
		break;
	}
	
	return -1;
}

void CFnSetScreenWnd::SetUpMasterData()
{
	CFile	fileH;
	CString	filePath;
	Hana_UpMaster	upCode;

	filePath.Format("%s/%s/%s", Variant(homeCC), "tab", "upcode.dat");
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("업종코드 파일을 읽을 수 없습니다.");
		return;
	}
	
	int nCount = fileH.GetLength() / sizeof(upCode);
	m_mapMarketMaster.RemoveAll();
	
	CString strMarketCode;
	CString strMarketName;
	
	for (int i = 0; i < nCount; i++)
	{
		fileH.Read(&upCode, sizeof(upCode));
			
		strMarketCode.Format("%d%2d", upCode.jgub, upCode.ucod);
		strMarketCode.Replace(' ', '0');
		strMarketName.Format("%s", upCode.hnam);

		m_mapMarketMaster.SetAt((LPCTSTR)strMarketCode, (LPCTSTR)strMarketName);

	}
	fileH.Close();
}

BOOL CFnSetScreenWnd::GetUpMasterData(CStringArray *&psaCode, CStringArray *&psaName, int jgub)
{
	CString			strMarketCode;
	CString			strMarketName;

	int nMarket;
	int nMarketCnt;
	int i = 0;
	switch(jgub)
	{
	case GETMARKET_ALL		:
		{
			for( i = 0; i < KOSPI_MAKRET_CNT; i++)
			{
				strMarketCode.Format("0%2d", i + 1);
				strMarketCode.Replace(' ', '0');
				if(m_mapMarketMaster.Lookup(strMarketCode, strMarketName))
				{
					psaCode->Add(strMarketCode);
					psaName->Add(strMarketName);
				}
			}

			for(i = 0; i < KOSDAQ_MARKET_CNT; i++)
			{
				strMarketCode.Format("2%2d", i + 1);
				strMarketCode.Replace(' ', '0');
				if(m_mapMarketMaster.Lookup(strMarketCode, strMarketName))
				{
					psaCode->Add(strMarketCode);
					psaName->Add(strMarketName);
				}
			}
		}
	case GETMARKET_KOSPI	:	nMarket = 0; nMarketCnt = KOSPI_MAKRET_CNT;		break;
	case GETMARKET_KOSPI200 :	nMarket = 1; nMarketCnt = KOSPI200_MARKET_CNT;	break;
	case GETMARKET_KOSDAQ	:	nMarket = 2; nMarketCnt = KOSDAQ_MARKET_CNT;	break;
	case GETMARKET_KRX		:	
		strMarketCode = "999";
		if(m_mapMarketMaster.Lookup(strMarketCode, strMarketName))
		{
			psaCode->Add(strMarketCode);
			psaName->Add(strMarketName);
			
			return TRUE;
		}
		return FALSE;
	}
	
	for(int i = 0; i < nMarketCnt; i++)
	{
		strMarketCode.Format("%d%2d", nMarket, i + 1);
		strMarketCode.Replace(' ', '0');
		if(m_mapMarketMaster.Lookup(strMarketCode, strMarketName))
		{
			psaCode->Add(strMarketCode);
			psaName->Add(strMarketName);
		}
	}

	return TRUE;
}

CString CFnSetScreenWnd::GetMarketNameToCode(CString strMarketCode)
{
	CString strMarketName;
	if(m_mapMarketMaster.Lookup(strMarketCode, strMarketName))
	{
		return strMarketName;
	}
	return "";
}

CPen* CFnSetScreenWnd::GetAxPen(COLORREF clr, int nWidth, int nStyle)
{
	_penR penR;
	penR.clr = clr;
	penR.width = nWidth;
	penR.style = nStyle;
	return (CPen*)m_pWndModuleMain->SendMessage(WM_USER, getPEN, (long)&penR);
}

void CFnSetScreenWnd::LoadVersionManager()
{
	CString strPath;
	strPath.Format("%s%s", Variant(homeCC), "\\dev\\CSVerMng.dll");
	m_hCheckVersion = ::LoadLibrary(strPath);
	
	if(m_hCheckVersion)
	{
		Lib_Open = (_DLL_Open)::GetProcAddress(m_hCheckVersion, "_DLL_Open");
		Lib_Close = (_DLL_Close)::GetProcAddress(m_hCheckVersion, "_DLL_Close");

		Lib_Open(GetFormCode(), this, GetAxFont());
	}
}
