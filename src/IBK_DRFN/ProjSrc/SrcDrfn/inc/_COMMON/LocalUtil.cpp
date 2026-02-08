// CodeUtil.cpp : implementation file
//

#include "stdafx.h"
#include "LocalUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		TrimAll(xxVal)			xxVal.TrimLeft(); xxVal.TrimRight();

/////////////////////////////////////////////////////////////////////////////
// CCodeUtil

CCodeUtil::CCodeUtil()
{
}

CCodeUtil::~CCodeUtil()
{
}

int CCodeUtil::Read_File(LPCSTR szFile, CStringList& list)
{
	list.RemoveAll();

	CStdioFile file;
	if(!file.Open(szFile, CFile::modeRead))
		return 0;

	CString rString;
	while(file.ReadString(rString))
		list.AddTail(rString);

	file.Close();
	return list.GetCount();
}

void CCodeUtil::GetFileName(LPCSTR szKey, CString& szName, LPCSTR _szINI)
{
	char tmp[30];
	::GetPrivateProfileString("FILE", szKey, "", tmp, 30, _szINI);
	if(strlen(tmp)==0)
		sprintf(tmp, "%s.dat", szKey);
	szName = tmp;
}

// 박성진 2002.06.17
// 코드 5 자리에서 7자리로 변경.
// #define	ONESIZE_JMASTER	(1+1+18+6+7+3+1+1+3+1)
BOOL CJMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_JMASTER-3)
		return FALSE;

	int nOff=0;
	
	m_szUnder	= szTmp.Mid(nOff,1);	nOff+=1;
	m_szWarn	= szTmp.Mid(nOff,1);	nOff+=1;
	m_szName	= szTmp.Mid(nOff, 18);  nOff+=18;
	m_szCode	= szTmp.Mid(nOff, 6);	nOff+=6;
	m_szSymbol	= szTmp.Mid(nOff, 7);	nOff+=7;
//	m_szGubun	= szTmp.Mid(nOff,1);	nOff+=1;
	m_szUpCode	= szTmp.Mid(nOff, 3);	nOff+=3;
	nOff += 1; //업종코드 다음 빈칸 by LEJ, 2004.01.15
	m_szS		= szTmp.Mid(nOff, 1);	nOff+=1;
	m_szC		= szTmp.Mid(nOff, 1);	nOff+=1;
	//if(szTmp.GetLength() > nOff)
	m_szG = szTmp.Mid(nOff, 3);		nOff+=3;
	if(szTmp.GetLength() > nOff)
		m_manuf = szTmp.Mid(nOff, 1);		

	TrimAll(m_szName);
	TrimAll(m_szSymbol);
	TrimAll(m_szCode);

	return TRUE;
}

// 박성진 2002.06.17
// 코드5자리에서 7자리 변경
// #define	ONESIZE_KMASTER	(18+7+3)
// 박성진 2002.09.15
// JMaster 에 그룹사 양식 추가.(파일을 JMaster 와 동일)
//#define	ONESIZE_KMASTER	(1+1+18+7+7+3+1+1+3)
BOOL CKMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	//TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_KMASTER)
		return FALSE;
	int nOff=0;
	m_szUnder	= szTmp.Mid(nOff,1);	nOff+=1;
	m_szWarn	= szTmp.Mid(nOff,1);	nOff+=1;
	m_szName	= szTmp.Mid(nOff, 18);  nOff+=18;
	m_szCode	= szTmp.Mid(nOff, 6);	nOff+=6;
										nOff+=7;	// 심볼
//	m_szGubun	= szTmp.Mid(nOff,1);	nOff+=1;
	m_szUpCode	= szTmp.Mid(nOff, 3);	nOff+=3;
	nOff+=1; //업종코드 다음 빈칸 
	m_szUpCodem = szTmp.Mid(nOff, 3);	nOff+=3;
	m_szUpCodel = szTmp.Mid(nOff, 3);	nOff+=3;
	m_szCapsize = szTmp.Mid(nOff, 1);	nOff+=1;
	m_szVen = szTmp.Mid(nOff, 1);	nOff+=1;
	m_stargb = szTmp.Mid(nOff, 1);	
//005 2004.01.29	nOffset+=2;	// 부,대중소 
//005 2004.01.29	JWSTRPARSING(m_szG, _szStr, 3)
//005 2004.01.29
//										nOff+=2;	// 부,대중소 
//	m_szG		= szTmp.Mid(nOff, 3);	nOff+=3;
//	m_szVen = szTmp.Mid(nOff, 1);  nOff+=1;

	TrimAll(m_szName);
	TrimAll(m_szCode);
//	TrimAll(m_szG);
//	m_szVen = szTmp.Mid(nOff, 1);  nOff+=1;


	return TRUE;
}

// #define	ONESIZE_UMASTER	(18+3)
BOOL CUMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_UMASTER)
		return FALSE;

	int nOff=0;
	m_szName = szTmp.Mid(nOff, 18);  nOff+=18;
//	m_szGubun = szTmp.Mid(nOff,1);	nOff+=1;
	m_szCode = szTmp.Mid(nOff, 3);  nOff+=3;

	TrimAll(m_szName);

	return TRUE;
}

// #define	ONESIZE_UPMASTER	(18+3)
BOOL CUPMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_UPMASTER)
		return FALSE;

	int nOff=0;
	m_szName = szTmp.Mid(nOff, 18);  nOff+=18;
//	m_szGubun = szTmp.Mid(nOff,1);	nOff+=1;
	m_szCode = szTmp.Mid(nOff, 3);  nOff+=3;

	TrimAll(m_szName);

	return TRUE;
}

// #define	ONESIZE_KUMASTER	(18+3)
BOOL CKUMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_KUMASTER)
		return FALSE;

	int nOff=0;
	m_szName = szTmp.Mid(nOff, 18);  nOff+=18;
//	m_szGubun = szTmp.Mid(nOff,1);	nOff+=1;
	m_szCode = szTmp.Mid(nOff, 3);  nOff+=3;

	TrimAll(m_szName);

	return TRUE;
}

// #define	ONESIZE_UDMASTER	(18+1+3)
BOOL CUDMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_UDMASTER)
		return FALSE;

	int nOff=0;
	m_szName  = szTmp.Mid(nOff, 18); nOff+=18;
//	m_szGubun = szTmp.Mid(nOff, 1);  nOff+=1;
	m_szCode  = szTmp.Mid(nOff, 3);  nOff+=3;

	TrimAll(m_szName);

	return TRUE;
}

// 박성진 2002.06.17
// 코드5자리에서 7자리 변경
// #define	ONESIZE_TMASTER	(40+7)
BOOL CTMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	//TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_TMASTER)
		return FALSE;

	int nOff=0;
	m_szName = szTmp.Mid(nOff, 40);  nOff+=40;
	m_szCode = szTmp.Mid(nOff, 6);  nOff+=6;

	TrimAll(m_szName);
	TrimAll(m_szCode);

	return TRUE;
}


// #define	ONESIZE_GMASTER	(40+3)
BOOL CGMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_GMASTER)
		return FALSE;

	int nOff=0;
	m_szName = szTmp.Mid(nOff, 40);  nOff+=40;
//	m_szGubun= szTmp.Mid(nOff,1);	nOff+=1;
	m_szCode = szTmp.Mid(nOff, 3);  nOff+=3;

	TrimAll(m_szCode);
	TrimAll(m_szName);

	return TRUE;
}

// #define	ONESIZE_FMASTER	(20+5)
BOOL CFMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_FMASTER)
		return FALSE;

	int nOff=0;
//	m_szName = szTmp.Mid(nOff, 20);  nOff+=20;
//	m_szCode = szTmp.Mid(nOff, 5);  nOff+=5;
	m_szName = szTmp.Mid(nOff, NAMELEN_F);  nOff+=NAMELEN_F;
	m_szCode = szTmp.Mid(nOff, CODELEN_F);  nOff+=CODELEN_F;
	TrimAll(m_szName);

	return TRUE;
}

// #define	ONESIZE_PMASTER	(14+8)
BOOL CPMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_PMASTER)
		return FALSE;

	int nOff=0;
	m_szName = szTmp.Mid(nOff, 14);  nOff+=14;
	m_szCode = szTmp.Mid(nOff, 8);  nOff+=8;

	TrimAll(m_szName);

	return TRUE;
}


#define	ONESIZE_THMASTER	(40+3)
BOOL CTHMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_THMASTER)
		return FALSE;

	int nOff=0;
	m_szName = szTmp.Mid(nOff, 40);  nOff+=40;
//	m_szGubun = szTmp.Mid(nOff, 1);  nOff+=1;
	m_szCode = szTmp.Mid(nOff, 3);  nOff+=3;

	TrimAll(m_szName);

	return TRUE;
}


// * 결재년월
// #define		ONESIZE_DMaster			(7)
BOOL CDMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_DMaster)
		return FALSE;

	int nOff=0;
	m_szDate = szTmp.Mid(nOff, 7);  nOff+=7;

	TrimAll(m_szDate);

	return TRUE;
}


// * 거래원코드
// #define		ONESIZE_SMaster		(20 + 3+1)
BOOL CSMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
//	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_SMaster)
		return FALSE;

	int nOff=0;
	m_szName = szTmp.Mid(nOff, 20);  nOff+=20;
	m_szCode = szTmp.Mid(nOff, 3);  nOff+=3;
	m_szGubun = szTmp.Mid(nOff, 1);  nOff+=1;

	TrimAll(m_szName);

	return TRUE;
}

// * 채권
// #define		ONESIZE_CMaster			(45+9)
BOOL CCMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_CMaster)
		return FALSE;
	
	int nOff=0;
	m_szName = szTmp.Mid(nOff, 45);  nOff+=45;
	m_szCode = szTmp.Mid(nOff, 9);  nOff+=9;
	
	TrimAll(m_szName);
	
	return TRUE;
}

// #define ONESIZE_KFMASTER		(30 + 11)
BOOL CKFMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	if(szTmp.GetLength()<ONESIZE_KFMASTER)
		return FALSE;
	
	int nOff=0;
	m_szName = szTmp.Mid(nOff,30);	nOff+=30;
	m_szCode = szTmp.Mid(nOff,11);	nOff+=11;
	
	TrimAll(m_szName);
	m_szCode = m_szName;
	
	return TRUE;
}

// * 코스닥선물합성
// #define	ONESIZE_KFTSMaster			(15)
BOOL  CKFTSMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp = _szStr;
	TrimAll(m_szCode);
	if(szTmp.GetLength()<ONESIZE_KFTSMASTER)
		return FALSE;
	
	m_szName = szTmp.Mid(0,15);
	TrimAll(m_szName);
	m_szCode = m_szName;
	
	return TRUE;
}

BOOL CBMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp = _szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_BMASTER)
		return FALSE;
	
	int nOff = 0;
	m_szName = szTmp.Left(20);
	m_szCode = szTmp.Right(9);
	
	TrimAll(m_szName);
	TrimAll(m_szCode);
	return TRUE;
}

BOOL CBEMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp = _szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_BEMASTER)
		return FALSE;
	
	int nOff = 0;
	m_szName = szTmp.Left(20);
	m_szCode = szTmp.Right(9);
	
	TrimAll(m_szName);
	TrimAll(m_szCode);
	return TRUE;
}

BOOL CBCMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp = _szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_BCMASTER)
		return FALSE;
	
	int nOff = 0;
	m_szName = szTmp.Left(20);
	m_szCode = szTmp.Right(9);
	
	TrimAll(m_szName);
	TrimAll(m_szCode);
	return TRUE;
}

BOOL CSTKOPT::ReadData(LPCSTR _szStr)
{
	CString szTmp = _szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_STKOPT)
		return FALSE;
	
	int nOff=0;
	m_szName = szTmp.Mid(nOff, 20);  nOff+=20;
	m_szCode = szTmp.Mid(nOff, 2);  nOff+=2;
	m_szATM = szTmp.Mid(nOff, 10);  nOff+=10;
	m_szBaseCode = szTmp.Mid(nOff, 5);  nOff+=5;
	
	TrimAll(m_szName);
	TrimAll(m_szCode);
	TrimAll(m_szATM);
	TrimAll(m_szBaseCode);
	return TRUE;
}

// 주식옵션 결제월
BOOL CSTKDMASTER::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_STKDMASTER)
		return FALSE;

	int nOff=0;
	m_szDate = szTmp.Mid(nOff, 7);  nOff+=7;

	TrimAll(m_szDate);

	return TRUE;
}

// * ETF(상장지수펀드)
// #define		ONESIZE_ETFMASTER	(20+7+1)
BOOL CETFMaster::ReadData(LPCSTR _szStr)
{
	CString szTmp=_szStr;
//	TrimAll(szTmp);
	if(szTmp.GetLength()<ONESIZE_ETFMASTER)
		return FALSE;

	int nOff=0;
	m_szName	= szTmp.Mid(nOff, 20);  nOff+=20;
	m_szCode	= szTmp.Mid(nOff, 6);	nOff+=6;
	m_szGubun	= szTmp.Mid(nOff, 1);	nOff+=1;

	TrimAll(m_szName);
	TrimAll(m_szCode);

	return TRUE;
}


/*
class CSTKOPT	: public CMasterBase
{
public:
	virtual BOOL ReadData(LPCTSTR _szStr);
	CString m_szName;		// 종목명
	CString m_szCode;		// 코드
	CString m_szATM;		 // ATM 가격
	CString m_szBaseCode;	// 기초자산 코드
}
*/
/*
/////////////////////////////////////////////////////////////////////////////
// CSortClass
//소팅을 하기위한 클래스  2000.11.28 code by nykim
CSortClass::CSortClass(CListCtrl * _pWnd, const int _iCol, const bool _bIsNumeric)
{
	iCol = _iCol;
	pWnd = _pWnd;
	bIsNumeric = _bIsNumeric;
	
	ASSERT(pWnd);
	int max = pWnd->GetItemCount();
	DWORD dw;
	CString txt;
	if (bIsNumeric)
	{
		for (int t = 0; t < max; t++)
		{
			dw = pWnd->GetItemData(t);
			txt = pWnd->GetItemText(t, iCol);
			pWnd->SetItemData(t, (DWORD) new CSortItemInt(dw, txt));
		}
	}
	else
	{
		for (int t = 0; t < max; t++)
		{
			dw = pWnd->GetItemData(t);
			txt = pWnd->GetItemText(t, iCol);
			pWnd->SetItemData(t, (DWORD) new CSortItem(dw, txt));
		}
	}
}

CSortClass::~CSortClass()
{
	ASSERT(pWnd);
	int max = pWnd->GetItemCount();
	if (bIsNumeric)
	{
		CSortItemInt * pItem;
		for (int t = 0; t < max; t++)
		{
			pItem = (CSortItemInt *) pWnd->GetItemData(t);
			ASSERT(pItem);
			pWnd->SetItemData(t, pItem->dw);
			delete pItem;
		}
	}
	else
	{
		CSortItem * pItem;
		for (int t = 0; t < max; t++)
		{
			pItem = (CSortItem *) pWnd->GetItemData(t);
			ASSERT(pItem);
			pWnd->SetItemData(t, pItem->dw);
			delete pItem;
		}
	}
}

void
CSortClass::Sort(const bool bAsc)
{
	if (bIsNumeric)
	{
		if (bAsc)
			pWnd->SortItems(CompareAscI, 0L);
		else
			pWnd->SortItems(CompareDesI, 0L);
	}
	else
	{
		if (bAsc)
			pWnd->SortItems(CompareAsc, 0L);
		else
			pWnd->SortItems(CompareDes, 0L);
	}
}

int CALLBACK CSortClass::CompareAsc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortItem * i1 = (CSortItem *) lParam1;
	CSortItem * i2 = (CSortItem *) lParam2;
	ASSERT(i1 && i2);
	CString s1,s2;
	s1 = i1->txt;
	s2 = i2->txt;

	int n;
	while((n = s1.Find('.'))!=-1)
		s1 = s1.Left(n) + s1.Mid(n+1);

	while((n = s2.Find('.'))!=-1)
		s2 = s2.Left(n) + s2.Mid(n+1);

	return s1.CompareNoCase(s2);
}

int CALLBACK CSortClass::CompareDes(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortItem * i1 = (CSortItem *) lParam1;
	CSortItem * i2 = (CSortItem *) lParam2;
	ASSERT(i1 && i2);
	CString s1,s2;
	s1 = i1->txt;
	s2 = i2->txt;

	int n;
	while((n = s1.Find('.'))!=-1)
		s1 = s1.Left(n) + s1.Mid(n+1);

	while((n = s2.Find('.'))!=-1)
		s2 = s2.Left(n) + s2.Mid(n+1);

	return s2.CompareNoCase(s1);
}

int CALLBACK CSortClass::CompareAscI(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortItemInt * i1 = (CSortItemInt *) lParam1;
	CSortItemInt * i2 = (CSortItemInt *) lParam2;
	ASSERT(i1 && i2);
	if (i1->iInt == i2->iInt) return 0;
	return i1->iInt > i2->iInt ? 1 : -1;
}

int CALLBACK CSortClass::CompareDesI(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortItemInt * i1 = (CSortItemInt *) lParam1;
	CSortItemInt * i2 = (CSortItemInt *) lParam2;
	ASSERT(i1 && i2);
	if (i1->iInt == i2->iInt) return 0;
	return i1->iInt < i2->iInt ? 1 : -1;
}

CSortClass::CSortItem::CSortItem(const DWORD _dw, const CString & _txt)
{
	dw = _dw;
	txt = _txt;
}

CSortClass::CSortItem::~CSortItem()
{
}

CSortClass::CSortItemInt::CSortItemInt(const DWORD _dw, const CString & _txt)
{
	iInt = atoi(_txt);
	dw = _dw;
}
*/