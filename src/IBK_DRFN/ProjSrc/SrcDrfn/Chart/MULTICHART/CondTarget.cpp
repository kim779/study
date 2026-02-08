// CondTarget.cpp: implementation of the CCondTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "StdDialog.h"
#include "CondTarget.h"

#include "../../inc/Condition_MngDef.h"

//#include "../../../SrcSite/include/axisfire.h"
#include "../../../SrcSite/include/interSt.h"

const static char *FILE_THEME =	"THEMCD.INI";

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		MAX_LINE	100

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCondTarget::CCondTarget()
{
	m_nKey = 0;
	m_pManager = NULL;
	m_strKRX100Code = "";

	IGateManager* pGateMng;
	AfxGetIGateManager(pGateMng);
	if (pGateMng)
	{
		IMainInfoManager* pManager = (IMainInfoManager*)pGateMng->GetInterface(21);
		m_strUserDir.Format("%s", pManager->GetEncUserDir());
		m_strRootDir.Format("%s", pManager->GetRootDir());
	}

	m_pManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	if (m_pManager)
	{
		CStringArray *parEngName = new CStringArray;
		CStringArray *parrCode = &m_arrKSPCode;
		CStringArray *parrName = &m_arrKSPName;
		m_pManager->GetKSPUpMst(parrCode, parrName, parEngName);

		parrCode = &m_arrKSQCode;
		parrName = &m_arrKSQName;
		m_pManager->GetKSQUpMst(parrCode, parrName, parEngName);

		CStringArray *parKRXName = new CStringArray;
		CStringArray *parKRXCode = new CStringArray;
		m_pManager->GetKRXUpMst(parKRXCode, parKRXName, parEngName);

		CString strName;
		int nSize = parKRXName->GetSize();
		for (int i=0; i<nSize; i++)
		{
			strName = parKRXName->GetAt(i);
			if (strName.CompareNoCase(_T("KRX100(통합지수)")) == 0)
			{
				m_strKRX100Code = parKRXCode->GetAt(i);
				break;
			}
		}

		parKRXName->RemoveAll();	delete parKRXName;
		parKRXCode->RemoveAll();	delete parKRXCode;
		parEngName->RemoveAll();	delete parEngName;

		DelSpaceInName();
	}
}

CCondTarget::~CCondTarget()
{
	RemoveAll();

	int	nKey;
	CGetBackData *pData;
	POSITION pos = m_mapGetBackData.GetStartPosition();
	while (pos)
	{
		m_mapGetBackData.GetNextAssoc(pos, nKey, pData);
		delete pData;
		pData = NULL;
	}

	m_mapGetBackData.RemoveAll();
}

void CCondTarget::DelSpaceInName()
{
	CString strName;
	int nSize, i;
	nSize = m_arrKSPName.GetSize();
	for (i=0; i<nSize; i++)
	{
		strName = m_arrKSPName.GetAt(i);
		strName.Replace(" ", "");

		m_arrKSPName.SetAt(i, strName);
	}

	nSize = m_arrKSQName.GetSize();
	for (i=0; i<nSize; i++)
	{
		strName = m_arrKSQName.GetAt(i);
		strName.Replace(" ", "");

		m_arrKSQName.SetAt(i, strName);
	}

	m_strKSP200Code = GetMarketCode("UMASTER", "KOSPI200");
	m_strKSPCode = GetMarketCode("UMASTER", "KOSPI(종합)");
	m_strKSQCode = GetMarketCode("KUMASTER", "KOSDAQ(종합)");
}

void CCondTarget::RemoveAll()
{
	m_arTargetResult.RemoveAll();
	m_mapExeptItem.RemoveAll();
	for (int i=0; i<SZ_MONTHARRAY; i++)
		m_saMonth[i].RemoveAll();
}

CGetBackData* CCondTarget::SetGetBackData(CString strPath)
{
	CGetBackData *pData;
	while (m_mapGetBackData.Lookup(m_nKey, pData))
		m_nKey++;

	pData = new CGetBackData(strPath, m_nKey);
	m_mapGetBackData.SetAt(m_nKey, pData);

	m_nKey++;

	return pData;
}

void CCondTarget::RemoveAtGetBackData(int nKey)
{
	CGetBackData *pData;
	m_mapGetBackData.Lookup(nKey, pData);
	if (pData)
	{
		delete pData;
		pData = NULL;

		m_mapGetBackData.RemoveKey(nKey);
	}
}

int CCondTarget::GetProfileInt(CString strGKey, CString strKey, CString strSavePath)
{
	char szInt[64];
	::GetPrivateProfileString(strGKey, strKey, "0", szInt, sizeof(szInt), strSavePath);
	return atoi(szInt);
}

void CCondTarget::LoadTdfData(CGetBackData *pData)
{
	CString strFullPath = pData->m_strPath;
	int nKey = pData->m_nIndex;

	CString szExt = strFullPath.Right(4);
	if (szExt.CompareNoCase(".xml") !=0)
		return;

	CString strPath = strFullPath;
	strPath = strFullPath.Left(strFullPath.GetLength()-4);
	strPath += _T(".tdf");

	CFileFind ff;
	if (!ff.FindFile(strPath))
		return;

	CString strGKey = "Except_JM";
	char szTemp[1024];

	m_bIssuesForAdministration		= GetProfileInt(strGKey, "Issue", strPath);
	m_bDealStop						= GetProfileInt(strGKey, "CheckDealStop", strPath);
	m_bTidyDeal						= GetProfileInt(strGKey, "TidyDeal", strPath);
	m_bInsincerityNotice			= GetProfileInt(strGKey, "InsincerityNotice", strPath);
	m_bAttentionInvestment			= GetProfileInt(strGKey, "AttentionInvestment", strPath);
	m_bNoticeAttentionInvestment	= GetProfileInt(strGKey, "NoticeAttentionInvestment", strPath);
	m_bPreferenceStock				= GetProfileInt(strGKey, "CheckPreferenceStock", strPath);
	m_bDeposit						= GetProfileInt(strGKey, "CheckDeposit", strPath);
	m_bDangerInvestment				= GetProfileInt(strGKey, "DangerInvestment", strPath);
	m_bWarningInvestment			= GetProfileInt(strGKey, "WarningInvestment", strPath);
	m_bInvestAttention				= GetProfileInt(strGKey, "InvestAttention", strPath);
	m_bETF							= GetProfileInt(strGKey, "ETF", strPath);

	strGKey = "Month";
	m_nSelRadioMonth = GetProfileInt(strGKey, "SelRadioMonth", strPath);
	m_nIndexMonthCombo = GetProfileInt(strGKey, "IndexMonthCombo", strPath);
	
	strGKey = "Tree_Upjong";
	::GetPrivateProfileString(strGKey, "TargetItem", "0", szTemp, sizeof(szTemp), strPath);
	m_strEditSubjectSelectItemText.Format("%s", szTemp);

	::GetPrivateProfileString(strGKey, "TargetParnetItem", "0", szTemp, sizeof(szTemp), strPath);
	m_strEditSubjectSelectParentItemText.Format("%s", szTemp);

	::GetPrivateProfileString("Tree_JangGubun", "Index", "1000", szTemp, sizeof(szTemp), strPath);
	m_strJangGubun.Format("%s", szTemp);

	SetEditSubjectTarGetBit();

	RemoveAtGetBackData(nKey);
}

void CCondTarget::ReceiveData(LPVOID aTRData, long dwTRDateLen)
{
	RemoveAll();

	CStringArray saMSTCode;
	MSTCode stMSTCode;
	char *psData = (char*)aTRData;
	int nPos=0;

	int nDataCnt = dwTRDateLen / sizeof(MSTCode);

	char szTotCodeCnt[5] = {0, };
	memcpy(&szTotCodeCnt, (char*)&psData[nPos], 4); //종목카운트 패킷 앞의 4바이트
	nDataCnt = atoi(szTotCodeCnt);

	CString strCode;
	char	buf[4];

	CString strData;

	BOOL bInsert;
	for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
	{
		bInsert = FALSE;
		strData = "";
		memset(&stMSTCode, 0x00, sizeof(MSTCode));
		memcpy(&stMSTCode, (char*)&psData[nPos] + 4, sizeof(MSTCode));
		nPos += sizeof(MSTCode);
		if (nPos >= dwTRDateLen)
			break;

		strCode.Format("%-7.7s", stMSTCode.sCode);
		strCode.TrimLeft();
		strCode.TrimRight();
		saMSTCode.Add(strCode);

		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, stMSTCode.sUnder, sizeof(stMSTCode.sUnder));
		if(atoi(buf) == 1)
		{
			strData += "1";
			bInsert = TRUE;
		}	
		else
			strData += "0";		

		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, stMSTCode.sStop, sizeof(stMSTCode.sStop));
		if(atoi(buf) == 1)
		{
			strData += "1";
			bInsert = TRUE;
		}	
		else
			strData += "0";

		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, stMSTCode.sJrmm, sizeof(stMSTCode.sJrmm));
		if(atoi(buf) == 1)
		{
			strData += "1";
			bInsert = TRUE;
		}	
		else
			strData += "0";

		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, stMSTCode.sDishonest, sizeof(stMSTCode.sDishonest));
		if(atoi(buf) == 1)
		{
			strData += "1";
			bInsert = TRUE;
		}	
		else
			strData += "0";

		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, stMSTCode.sMrktalrm, sizeof(stMSTCode.sMrktalrm));
		switch(atoi(buf))
		{
		case 1 : strData += "1"; bInsert = TRUE; break;
		case 2 : strData += "2"; bInsert = TRUE; break;
		case 3 : strData += "3"; bInsert = TRUE; break;
		case 4 : strData += "4"; bInsert = TRUE; break;
		default: strData += "0";
		}

		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, stMSTCode.sUsjg, sizeof(stMSTCode.sUsjg));
		if(atoi(buf) == 1)
		{
			strData += "1";
			bInsert = TRUE;
		}	
		else
			strData += "0";

		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, stMSTCode.sDrat, sizeof(stMSTCode.sDrat));
		if(atoi(buf) == 100)
		{
			strData += "1";
			bInsert = TRUE;
		}	
		else
			strData += "0";

		if (stMSTCode.sETF[0] == '1')
		{
			strData += "1";
			bInsert = TRUE;
		}
		else
			strData += "0";

		if(bInsert)
			m_mapExeptItem.SetAt(strCode, strData);

		memset(buf, 0x00, sizeof(buf));
		memcpy(buf, stMSTCode.sMonth, sizeof(stMSTCode.sMonth));
		switch(atoi(buf))
		{
			case 1 :	m_saMonth[0].Add(strCode);		break;
			case 2 :	m_saMonth[1].Add(strCode);		break;
			case 3 :	m_saMonth[2].Add(strCode);		break;
			case 4 :	m_saMonth[3].Add(strCode);		break;
			case 5 :	m_saMonth[4].Add(strCode);		break;
			case 6 :	m_saMonth[5].Add(strCode);		break;
			case 7 :	m_saMonth[6].Add(strCode);		break;
			case 8 :	m_saMonth[7].Add(strCode);		break;
			case 9 :	m_saMonth[8].Add(strCode);		break;
			case 10 :	m_saMonth[9].Add(strCode);		break;
			case 11 :	m_saMonth[10].Add(strCode);		break;
			case 12 :	m_saMonth[11].Add(strCode);		break;
		}
	}
}

void CCondTarget::SetEditSubjectTarGetBit()
{
	CStringArray arrMasterItemCode, arrTargetItemName, arrFilterMonthItemCode;

	if (m_strEditSubjectSelectParentItemText == "관심종목")
		GetData_PortItem(arrMasterItemCode, arrTargetItemName, _ttoi(m_strJangGubun));
	else if (m_strEditSubjectSelectParentItemText == "테마그룹")
		GetItemCodeFromThemeGroup(arrMasterItemCode, m_strJangGubun);
	else
		GetStockMasterData(arrMasterItemCode, arrTargetItemName);

	CStringArray arrTargetItemCode;
	
	CString strExceptDetail;
	for(int nIndex = 0; nIndex < arrMasterItemCode.GetSize(); nIndex++)
	{
		if(m_mapExeptItem.Lookup(arrMasterItemCode.GetAt(nIndex), strExceptDetail))
		{
			if ((strExceptDetail.GetLength() != 8) ||
				(m_bIssuesForAdministration		&& strExceptDetail.GetAt(0) == '1') ||	// 관리종목
				(m_bDealStop					&& strExceptDetail.GetAt(1) == '1') ||	// 거래정지
				(m_bTidyDeal					&& strExceptDetail.GetAt(2) == '1') ||	// 정리매매
				(m_bInsincerityNotice			&& strExceptDetail.GetAt(3) == '1') ||	// 불성실공시
				(m_bAttentionInvestment			&& strExceptDetail.GetAt(4) == '1') ||	// 투자주의
				(m_bWarningInvestment			&& strExceptDetail.GetAt(4) == '2') ||	// 투자경고
				(m_bDangerInvestment			&& strExceptDetail.GetAt(4) == '3') ||	// 투자위험
				(m_bNoticeAttentionInvestment	&& strExceptDetail.GetAt(4) == '4') ||	// 투자위험예고
				(m_bPreferenceStock				&& strExceptDetail.GetAt(5) == '1') ||	// 우선주
				(m_bDeposit						&& strExceptDetail.GetAt(6) == '1') ||	// 증거금
//				(m_bETN							&& strExceptDetail.GetAt(7) == '1') ||	// ETN
				(m_bETF							&& strExceptDetail.GetAt(7) == '1'))	// ETF
			{
				continue;
			}
		}

		arrTargetItemCode.Add(arrMasterItemCode.GetAt(nIndex));
	}

	if (m_nSelRadioMonth)
	{
		CStringArray saMonthSettlement;

		int nMonth;

		if (m_nSelRadioMonth == 1)			nMonth = 3;
		else if (m_nSelRadioMonth == 2)		nMonth = 6;
		else if (m_nSelRadioMonth == 3)		nMonth = 9;
		else if (m_nSelRadioMonth == 4)		nMonth = 12;
		else if (m_nSelRadioMonth == 5)		nMonth = m_nIndexMonthCombo + 1;

		saMonthSettlement.Copy(m_saMonth[nMonth - 1]);
		
		for (int nIndex = 0; nIndex < saMonthSettlement.GetSize(); nIndex++)
		{
			CString strItemCodeMonth;
			strItemCodeMonth = saMonthSettlement.GetAt(nIndex);

			for (int nTargetIndex = 0; nTargetIndex < arrTargetItemCode.GetSize(); nTargetIndex++)
			{
				if (strItemCodeMonth == arrTargetItemCode.GetAt(nTargetIndex))
				{
					arrFilterMonthItemCode.Add(arrTargetItemCode.GetAt(nTargetIndex));
					break;
				}
			}
		}
	}
	else
		arrFilterMonthItemCode.Copy(arrTargetItemCode);

	m_arTargetResult.RemoveAll();
	m_arTargetResult.Copy(arrFilterMonthItemCode);
}

BOOL CCondTarget::CheckTargetResult(CString strCode)
{
	CString strRet;
	int nSize = m_arTargetResult.GetSize();
	for (int i=0; i<nSize; i++)
	{
		strRet = m_arTargetResult.GetAt(i);
		if (strRet.CompareNoCase(strCode) == 0)
			return TRUE;
	}

	return FALSE;
}


CString CCondTarget::GetMarketCode(CString strType, CString strName)
{
	strName.Replace(" ", "");
	CString strCode = "";
	CStringArray *pArrName=NULL, *pArrCode=NULL;
	if (strType.CompareNoCase(_T("UMASTER")) == 0)
	{
		pArrName = &m_arrKSPName;
		pArrCode = &m_arrKSPCode;
	}
	else if (strType.CompareNoCase(_T("KUMASTER")) == 0)
	{
		pArrName = &m_arrKSQName;
		pArrCode = &m_arrKSQCode;
	}
	else if (strType.CompareNoCase(_T("KRXUMASTER")) == 0)
	{
	}

	if (pArrCode == NULL)
		return "";

	int nSize = pArrName->GetSize();
	for (int i=0; i<nSize; i++)
	{
		if (pArrName->GetAt(i).CompareNoCase(strName) == 0)
		{
			strCode = pArrCode->GetAt(i);
			break;
		}
	}

	return strCode;
}


BOOL CCondTarget::GetStockMasterData(CStringArray& saCode, CStringArray& saName)
{
	UINT Jgub=0, Upgub=0;
	CString strTitle, strMarketCode, strType;
	if (m_strEditSubjectSelectParentItemText == "")
	{
		if (m_strEditSubjectSelectItemText.CompareNoCase("거래소") == 0)
		{
			strType = "UMASTER";
			strMarketCode = m_strKSPCode;
		}
		else if (m_strEditSubjectSelectItemText.CompareNoCase("코스닥") == 0)
		{
			strType = "KUMASTER";
			strMarketCode = m_strKSQCode;
		}
		else if (m_strEditSubjectSelectItemText.CompareNoCase("KOSPI200") == 0)
		{
			strType = "UMASTER";
			strMarketCode = m_strKSP200Code;
		}
		else if (m_strEditSubjectSelectItemText.CompareNoCase("KRX100") == 0)
		{
			strType = "KRXUMASTER";
			strMarketCode = m_strKRX100Code;
		}
		else if (m_strEditSubjectSelectItemText.CompareNoCase("전체") == 0)
		{
			if (m_pManager)
				m_pManager->GetAllStock(saCode, saName);

			return TRUE;
		}
	}
	else
	{
		if (m_strEditSubjectSelectParentItemText.CompareNoCase("거래소") == 0)
		{
			strType = "UMASTER";
			strMarketCode = GetMarketCode(strType, m_strEditSubjectSelectItemText);
		}
		else if (m_strEditSubjectSelectParentItemText.CompareNoCase("코스닥") == 0)
		{
			strType = "KUMASTER";
			strMarketCode = GetMarketCode(strType, m_strEditSubjectSelectItemText);
		}
		else
			return FALSE;
	}

	if (m_pManager)
		m_pManager->GetMasterDataInUpDetails(strType, saCode, saName, strMarketCode);

	return TRUE;
}

BOOL CCondTarget::GetData_PortItem(CStringArray& saCode, CStringArray& saName, int nKey)
{
	BOOL bResult = FALSE;
	saCode.RemoveAll();
	saName.RemoveAll();
	
	CString	strFilePath;
	strFilePath.Format("%s\\portfolio.i%02d", m_strUserDir, nKey);
	
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

void CCondTarget::GetItemCodeFromThemeGroup(CStringArray &arrCode, CString strThemeIndex)
{
	arrCode.RemoveAll();

	CString sFile;
	sFile.Format("%s\\tab\\%s", m_strRootDir, FILE_THEME);
	
	for (int i = 0; ; i++)
	{
		char szBuf[BUFSIZ] = { 0x00, };
		CString sKey;
		
		if (i < 100)
			sKey.Format("%02d", i);
		else
			sKey.Format("%03d", i);
		
		GetPrivateProfileString("FOLDER", sKey, "", szBuf, sizeof(szBuf), sFile);
		
		if (strlen(szBuf) <= 0)
			break;
		
		CString sData = szBuf;
		
		if (sData.GetLength() < 33)
			continue;
		
		CString sName = sData.Left(30);
		CString sTheCode = sData.Right(3);
		
		sName.TrimLeft(); sName.TrimRight();
		sTheCode.TrimLeft(); sTheCode.TrimRight();
		
		if(strThemeIndex != sTheCode)
			continue;
		
		char szCList[BUFSIZ] = { 0x00, };
		GetPrivateProfileString("TMCODE", sTheCode, "", szCList, sizeof(szCList), sFile);
		
		CString sCList = szCList;
		while (1)
		{
			CString sCode = Parser(sCList, ";");
			sCode.TrimLeft(); sCode.TrimRight();
			
			if (sCode.GetLength() <= 0)
				break;
			
			arrCode.Add(sCode);
		}
		
		if (arrCode.GetSize() <= 0)
		{
			return ;
		}
		
		break;
	}
}

CString CCondTarget::Parser(CString &sSrc, CString sDelimiter)
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
