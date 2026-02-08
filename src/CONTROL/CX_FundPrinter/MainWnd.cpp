// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FundPrinter.h"
#include "MainWnd.h"

#include "../../h/fxCommonx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd* pParent)
{
	EnableAutomation();

	m_pParent = pParent;
	m_tokenSize = 0;
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "SetData", SetData, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {059274D8-E747-48DB-BA58-CCF05AD951D0}
static const IID IID_IMainWnd =
{ 0x59274d8, 0xe747, 0x48db, { 0xba, 0x58, 0xcc, 0xf0, 0x5a, 0xd9, 0x51, 0xd0 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
//CMainWnd message handlers

void CMainWnd::SetParam(_param *pParam)
{
	m_Param.key     = pParam->key;
	m_Param.name    = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect    = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts   = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point   = pParam->point;
	m_Param.style   = pParam->style;
	m_Param.tRGB    = pParam->tRGB;
	m_Param.pRGB    = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
}

void CMainWnd::SetProperties(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here
	m_Param.options = str;
}

BSTR CMainWnd::GetProperties() 
{
	CString strResult = m_Param.options;
	return strResult.AllocSysString();
}

void CMainWnd::GetSplit(CString sText, TCHAR sSplit) 
{
	 //m_printData.clear();
	 CString sResult;

	 if(sText != "")
	 {
		 // afxwin.h을 이용하여 AfxExtractSubString함수를 이용한다.
		 while(AfxExtractSubString(sResult, sText, m_tokenSize, sSplit)) {
			if(sResult.GetLength() > 0)
			{
				m_printData[m_tokenSize] = sResult;
			}

			m_tokenSize += 1;
		 }
	 }
}

void CMainWnd::SetData(LPCTSTR data) 
{
	GetSplit(data,'|');

	if(m_tokenSize < 21)
	{
		MessageBox(_T("데이타가 정상적으로 수신되지 않았읍니다."), "IBK투자증권", MB_OK|MB_ICONERROR);
	}
	else
	{
		CPrintDlg* pDlg = new CPrintDlg(m_pParent);

		pDlg->SetData(data);

		if(pDlg->DoModal() == IDOK)
		{
			
		}
		
		pDlg->DestroyWindow();

		delete pDlg;
		pDlg = NULL;
	}
// 	if (!pDlg->Create(IDD_DIALOG2, NULL))
// 	{
// 		delete pDlg;
// 		pDlg = NULL;
// 	}	

// 	m_printDlg = pDlg;
// 
// 	m_printDlg->SetData(data);
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

CString CMainWnd::Variant(int comm, CString data)
{
	return VariantA(m_pParent, comm, data);
}
