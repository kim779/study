#include "stdafx.h"
#include "KTBChart.h"
#include "KTBChartCtl.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"			// for CAddInManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for ML_LANGUAGE_RES
#include "OutsideCommonInterface.h"							// for UWM_FIREGSHAREDATA

// (2004.06.08, 배승원) Global 공유 기능을 추가한다. (Input OCX에서 Porting함)
//////////////////////////////////////////////////////////////////////////////////
// Global 공유데이터
// Global 할때 아래 두 부분을 수정해서 사용하세요.
// 2000. 11. 29 이준옥(JunOk Lee)  alzio@dooriic.co.kr

BOOL CKTBChartCtrl::CGSharedCli::ChangedData(LPCSTR _szName, LPCSTR _szValue)
{
	METHOD_PROLOGUE(CKTBChartCtrl, GSharedCli)
		
	POSITION pos = FindShareData( _szName);
	if(pos)
	{
		TRACE("CGSharedCli::ChangedData [SHAREDNAME=%s] [VALUE=%s]\n",_szName,_szValue);
		// 공유데이터가 바뀌었다.
		// 여기에서 원하는 처리를 한다.
		if( m_pGSharedSvr)
		{
			m_bGetGData = TRUE;

#ifdef _DEBUG
			_MHWND( pThis->m_hOcxWnd);
			ML_SET_LANGUAGE_RES();
			CString strFormat;
			strFormat.LoadString( IDS_FORMAT_SHARED_DATA_LOG);
			CString szTest;
			szTest.Format( strFormat, _szName, _szValue);
			TRACE(szTest);
//			AfxMessageBox(szTest);
#endif

			m_szGSharedData = _szValue;
			m_szGSharedData.TrimLeft();
			m_szGSharedData.TrimRight();
			if( m_szGSharedData.GetLength() == 0) m_bGetGData = FALSE;

			pThis->OnGSharedDataChange( _szName, m_szGSharedData);

			// 박성진 2002.08.09
			// 공유가 끝나고 나면 FALSE로 만들어 줘야한다.
			m_bGetGData = FALSE;
		}
	}

	return TRUE;
}

BOOL CKTBChartCtrl::CGSharedCli::GetData(LPCSTR _szName, CString& szValue)
{
	METHOD_PROLOGUE(CKTBChartCtrl, GSharedCli)

	if(m_pGSharedSvr)
		return m_pGSharedSvr->GetData(_szName, szValue);
	return FALSE;
}

BOOL CKTBChartCtrl::CGSharedCli::SetData(LPCSTR _szName, LPCSTR _szValue)
{
	METHOD_PROLOGUE(CKTBChartCtrl, GSharedCli)

	if(m_pGSharedSvr)
		return m_pGSharedSvr->SetData(_szName, _szValue, this);
	return FALSE;
}

BOOL CKTBChartCtrl::CGSharedCli::Advise(LPCSTR _szName, CGlobalSharedClient* _pClient)
{
	METHOD_PROLOGUE(CKTBChartCtrl, GSharedCli)

	if(m_pGSharedSvr)
		return m_pGSharedSvr->Advise(_szName, this);
	return FALSE;
}

BOOL CKTBChartCtrl::CGSharedCli::UnAdvise(LPCSTR _szName, CGlobalSharedClient* _pClient)
{
	METHOD_PROLOGUE(CKTBChartCtrl, GSharedCli)

	if(m_pGSharedSvr)
		return m_pGSharedSvr->UnAdvise(_szName, this);
	return FALSE;
}

void CKTBChartCtrl::CGSharedCli::MakeList(CString szList)
{
	m_ListShare.RemoveAll();

	if(szList.GetLength()==0)		return;

	CString szData = szList;
	szData += ";";
	int nIndex = szData.Find(";");
	CString szShareName;

	BOOL	bStart, bSet, bGet;
	char	aTmp[2]={0,};
	int		nLen, nCnt=0, nTmp;

	while(nIndex>0)
	{
		bStart = bSet = bGet = FALSE;
		szShareName = szData.Left(nIndex);
		szData = szData.Mid(nIndex+1);

		nLen = szShareName.GetLength();
		aTmp[0] = szShareName[nLen-1];
		szShareName = szShareName.Left(nLen-1);

		nTmp = atoi(aTmp);
		if(nTmp & 1) bStart = TRUE;
		if(nTmp & 2) bSet=TRUE;
		if(nTmp & 4) bGet=TRUE;

		ST_GSHAREDFORMAT SDF;
		SDF.m_bStart = bStart;
		SDF.m_bSet = bSet;
		SDF.m_bGet = bGet;
		SDF.m_szSharedName = szShareName;
		m_ListShare.AddTail(SDF);

		nIndex = szData.Find(";");
	}
}

POSITION CKTBChartCtrl::CGSharedCli::FindShareData(LPCSTR _szName)
{
	CString szShareList = _szName;
	szShareList += ";";
	CString szTmp;

	ST_GSHAREDFORMAT SDF;
	POSITION tmpPos;
	for(POSITION pos=m_ListShare.GetHeadPosition(); pos; )
	{
		tmpPos=pos;
		SDF = m_ListShare.GetNext(pos);
		if(SDF.m_bGet)
		{
			szTmp.Format("%s;", SDF.m_szSharedName);
			szTmp.Remove(';');
			szShareList.Remove(';');
			if(szShareList.CompareNoCase(szTmp) == 0)
				return tmpPos;
		}
	}

	return NULL;
}

void CKTBChartCtrl::CGSharedCli::CallCheckSharedData(LPCSTR _szData)
{
	METHOD_PROLOGUE(CKTBChartCtrl, GSharedCli)

	_MHWND( pThis->m_hOcxWnd);
	ML_SET_LANGUAGE_RES();
	CString strFormat;
	if(m_bGetGData == TRUE) 
	{
		strFormat.LoadString( IDS_FORMAT_SHARED_DATA_LOG2);
		m_bGetGData = FALSE;
		TRACE( strFormat, _szData);
		return;
	}

	strFormat.LoadString( IDS_FORMAT_SHARED_DATA_LOG3);
	TRACE( strFormat, _szData);
	ST_GSHAREDFORMAT SDF;
	CString szShareNameList;
	for(POSITION pos=m_ListShare.GetHeadPosition(); pos; )
	{
		SDF = m_ListShare.GetNext(pos);
		if(SDF.m_bSet)
		{
			szShareNameList += SDF.m_szSharedName;
			szShareNameList += ";";
		}
	}

	if(szShareNameList.GetLength() > 0)
		SetData(szShareNameList, _szData);
}

int  CKTBChartCtrl::CGSharedCli::AddSharedNameToServer()
{
	// 현재는 클라이언트 포인터만 등록한다.
	// 공유가 발생할 경우 무조건 온다.
	if(!m_pGSharedSvr) return 0;

	m_pGSharedSvr->Advise("*", this);

	int nRet=1;
	return nRet;
}

int  CKTBChartCtrl::CGSharedCli::RemoveSharedNameToServer()
{
	// 현재는 클라이언트 포인터만 등록한다.
	// 공유가 발생할 경우 무조건 온다.
	if(!m_pGSharedSvr) return 0;

	m_pGSharedSvr->UnAdvise("*", this);

	int nRet=1;
	return nRet;
}

void CKTBChartCtrl::CGSharedCli::IniStartGShareData()
{
	METHOD_PROLOGUE(CKTBChartCtrl, GSharedCli)
	ST_GSHAREDFORMAT SDF;

	for(POSITION pos=m_ListShare.GetHeadPosition(); pos; )
	{
		SDF = m_ListShare.GetNext(pos);
		if(SDF.m_bStart)
		{
			m_bGetGData =TRUE;
			m_szGSharedData = GetData2(SDF.m_szSharedName);
#ifdef _DEBUG
			_MHWND( pThis->m_hOcxWnd);
			ML_SET_LANGUAGE_RES();
			CString strFormat;
			strFormat.LoadString( IDS_FORMAT_SHARED_DATA_LOG4);
			CString szTest;
			szTest.Format( strFormat, SDF.m_szSharedName, m_szGSharedData);
			TRACE(szTest);
#endif
			m_szGSharedData.TrimLeft();
			m_szGSharedData.TrimRight();

			if(m_szGSharedData.GetLength() <= 0) 
				m_bGetGData = FALSE;

			pThis->OnGSharedDataChange( SDF.m_szSharedName, m_szGSharedData, TRUE);

			// 박성진 2002.08.09
			// 공유가 끝나고 나면 FALSE로 만들어 줘야한다.
			m_bGetGData = FALSE;
		}
	}

}

LPCSTR CKTBChartCtrl::CGSharedCli::GetData2(LPCSTR _szName)
{
	METHOD_PROLOGUE(CKTBChartCtrl, GSharedCli)

	if(m_pGSharedSvr)
		return m_pGSharedSvr->GetData2(_szName);
	return "";
}

// (2004.06.11, 배승원) 공유의 Get/Set 상관없이 공유 설정 정보를 조회하는 Interface를 추가한다.
ST_GSHAREDFORMAT CKTBChartCtrl::CGSharedCli::FindShareInfo( LPCSTR _szName)
{
	ST_GSHAREDFORMAT SDF;
	for( POSITION pos = m_ListShare.GetHeadPosition(); pos; )
	{
		SDF = m_ListShare.GetNext( pos);
		if( SDF.m_szSharedName.CompareNoCase( _szName) == 0) break;
	}
	return SDF;
}

// Global 공유데이터
//////////////////////////////////////////////////////////////////////////////////

// (2004.06.11, 배승원) 다른 Class에서 Global 공유를 발생시킬 수 있도록 Interface를 제공한다.
// (2004.11.04, 배승원) CGSharedCli를 직접 Open하지 않는다.
BOOL CKTBChartCtrl::FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData)
{
	// (2004.11.05, 배승원) DLL Container가 있는 경우의 Global 공유 발생은, DLL Container에게 알려,
	//		소속 다른 Map들에게도 전달되도록 한다.
	if( m_hContainerDllWnd) SendEventMsgToDLL( UWM_FIREGSHAREDATA, 0,
		( long)( const char *)( CString( p_szGSharedName) + ";" + p_szGSharedData));

	// (2006/1/24 - Seung-Won, Bae) Add ';' for Share Platform
	return m_xGSharedCli.SetData( p_szGSharedName + CString( ';'), p_szGSharedData);
}

// (2004.11.04, 배승원) 주어진 Global 공유가 설정되어 있는지 확인하는 Interface를 제공한다.
ST_GSHAREDFORMAT CKTBChartCtrl::FindGSharedDataInfo( const char *p_szGSharedName)
{
	return m_xGSharedCli.FindShareInfo( p_szGSharedName);
}

// (2006/1/11 - Seung-Won, Bae) Check GShared Setting
BOOL CKTBChartCtrl::CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet)
{
	ST_GSHAREDFORMAT SDF = FindGSharedDataInfo( p_szGSharedName);
	if( SDF.m_szSharedName.IsEmpty()) return FALSE;
	return SDF.m_bStart && p_bStart || SDF.m_bGet && p_bGet || SDF.m_bSet && p_bSet;
}

// (2004.06.08, 배승원) Global 공유를 받는 기능을 제공한다.
//		Start 공유를 처리한다.
LRESULT CKTBChartCtrl::RmsgFormInializeComplete(WPARAM wParam, LPARAM lParam)
{
	m_xGSharedCli.IniStartGShareData();
	return 0L;	
}

// (2004.06.10, 배승원) Global 공유를 받는 기능을 제공한다.
void CKTBChartCtrl::OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)
{
	// (2004.10.11, 배승원) AddIn에게 이를 전달한다.
	if( m_pAddInManager) m_pAddInManager->OnGSharedDataChange( p_szGShareKey, p_szGShareValue, p_bStartShare);
}
