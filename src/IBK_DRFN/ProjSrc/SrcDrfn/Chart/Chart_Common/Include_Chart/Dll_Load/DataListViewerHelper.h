// DataListViewerHelper.h: interface for the CDataListViewerHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATALISTVIEWERHELPER_H__5D2A506F_7779_11D5_81A5_0050FC28B229__INCLUDED_)
#define AFX_DATALISTVIEWERHELPER_H__5D2A506F_7779_11D5_81A5_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadDll.h"
//////////////////////////////////////////////////////////////////////////////
//Dll에서 구현되어 있는 함수 형태의 포인터 형 선언..
class CMainBlock;
class CPacketList;

// (2004.03.04, 배승원) Main이 Dialog Base인 경우, 화면단위의 Modal이 형성되고,
//		다른 Chart 화면에서 한번 더 Data View Dialog를 실행시킬 수 있도록 Global로 관리하지 않는다.
typedef void ( WINAPI *fpLoadDataListViewer)( CWnd*, CString&, const CString&, CString&, CString&, CString&, bool, CPacketList*, CWnd *&p_pDataViewer);
typedef void ( WINAPI *fpRealUpdate)( CWnd *p_pDataViewer);

class CDataListViewerHelper : public CLoadDllLib  
{
public:
	fpLoadDataListViewer	m_pfpLoadDataListViewer; 
	fpRealUpdate			m_pfpRealUpdate; 
	BOOL					m_delAuto;
	CWnd *					m_pDataViewer;

	CDataListViewerHelper(LPCSTR _szDll)
	{
		m_pDataViewer =  NULL;

		m_delAuto = TRUE;
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll,"DataListViewer.dll");
		else
			m_szDllName = _T("DataListViewer.dll");
		m_hLib = NULL;
		MakeNull(m_pfpLoadDataListViewer);
		MakeNull(m_pfpRealUpdate);
		LoadLib();

		// (2006/2/20 - Seung-Won, Bae) if it does not exist in Bin.
		if(m_hLib == NULL)
		{
			m_szDllName = g_strOcxPath + m_szDllName;
			LoadLib();
		}
	}

	CDataListViewerHelper(BOOL deleteAuto)
	{
		m_pDataViewer =  NULL;

		m_delAuto = TRUE;
		m_szDllName = _T("DataListViewer.dll");
		m_hLib = NULL;
		MakeNull(m_pfpLoadDataListViewer);
		MakeNull(m_pfpRealUpdate);
		LoadLib();

		// (2006/2/20 - Seung-Won, Bae) if it does not exist in Bin.
		if(m_hLib == NULL)
		{
			m_szDllName = g_strOcxPath + m_szDllName;
			LoadLib();
		}

		m_delAuto = deleteAuto;
	}

	CDataListViewerHelper()
	{
		m_pDataViewer =  NULL;

		m_delAuto = TRUE;
		m_szDllName = _T("DataListViewer.dll");
		m_hLib = NULL;
		MakeNull(m_pfpLoadDataListViewer);
		MakeNull(m_pfpRealUpdate);
		LoadLib();

		// (2006/2/20 - Seung-Won, Bae) if it does not exist in Bin.
		if(m_hLib == NULL)
		{
			m_szDllName = g_strOcxPath + m_szDllName;
			LoadLib();
		}
	}

	~CDataListViewerHelper()
	{
		if( m_pDataViewer != NULL) delete m_pDataViewer;
	}

	int LoadDataListViewer(CWnd* pOcxWnd, CString& strTitle, const CString& strHorzScale,
									   CString& strPacketNamesPerBlock,
									   CString& strPacketNames,
									   CString& strSaveFilePath,
									   bool bUseExcel,
									   CPacketList* pPacketList)
	{
		if(!m_hLib) 
			return m_bState;//CS_LOADERROR;

		m_pfpLoadDataListViewer	= (fpLoadDataListViewer)::GetProcAddress(m_hLib, "LoadDataListViewer");
		if(m_pfpLoadDataListViewer==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return GetState();//CS_LOADERROR;
		}
		m_pfpLoadDataListViewer(pOcxWnd, strTitle, strHorzScale, strPacketNamesPerBlock,
								strPacketNames, strSaveFilePath, bUseExcel, pPacketList, m_pDataViewer);
		MAKESTATE(LDS_OK, LDR_READY);
		return GetState();//nRet;
	}

	int RealUpdate()
	{
		if(!m_hLib) 
			return m_bState;//CS_LOADERROR;

		m_pfpRealUpdate	= (fpRealUpdate)::GetProcAddress(m_hLib, "RealUpdate");
		if(m_pfpRealUpdate==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return GetState();//CS_LOADERROR;
		}
		m_pfpRealUpdate( m_pDataViewer);
		MAKESTATE(LDS_OK, LDR_READY);
		return GetState();//nRet;
	}
};

#endif // !defined(AFX_DATALISTVIEWERHELPER_H__5D2A506F_7779_11D5_81A5_0050FC28B229__INCLUDED_)
