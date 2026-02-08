// IEWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_WebView.h"
#include "IEWnd.h"
#include "AxBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIEWnd

CIEWnd::CIEWnd()
{
	EnableAutomation();
	m_pHtml = nullptr;
	m_sURL = _T("");
	//m_pApp = (Ccx_THtmlExApp*)AfxGetApp();
	m_bScroll = true;
	m_scrGap = 0;
}

CIEWnd::~CIEWnd()
{
}

void CIEWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CIEWnd, CWnd)
	//{{AFX_MSG_MAP(CIEWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CIEWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CIEWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CIEWnd, "Navigate", dispidNavigate, Navigate, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IIEWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {B8018BF3-6CFA-460A-BA46-591A5C5D28E8}
static const IID IID_IIEWnd =
{ 0xb8018bf3, 0x6cfa, 0x460a, { 0xba, 0x46, 0x59, 0x1a, 0x5c, 0x5d, 0x28, 0xe8 } };

BEGIN_INTERFACE_MAP(CIEWnd, CWnd)
	INTERFACE_PART(CIEWnd, IID_IIEWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIEWnd message handlers


void CIEWnd::SetParam(_param* pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString	sToken;
	sToken = _T("/wurl");
	int nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		CString sTmp = m_Param.options.Mid(nPos + sToken.GetLength());
		nPos = sTmp.Find("/");
		if (nPos < 0)
			m_sURL = sTmp;
		else
			m_sURL = sTmp.Left(nPos);
	}

	sToken = _T("/d");
	nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		CString sTmp = m_Param.options.Mid(nPos + sToken.GetLength());
		if (sTmp == "1")
			m_bScroll = true;
		else
			m_bScroll = false;
	}
	else
		m_bScroll = false;
}

void CIEWnd::Navigate(BSTR sUrl)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_pHtml)
	{
		CString url;
		m_pHtml->SetSilent(TRUE);
		url.Format("%s", sUrl);
		url.TrimRight();
		url.TrimLeft();
		m_pHtml->Navigate2(url);
	}
}

int CIEWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_bScroll)
	{
		m_Htmlrc.SetRect(-2, -2, m_Param.rect.Width() + 2, m_Param.rect.Height() + 2);
		m_pHtml = new CAxBrowser;
		m_pHtml->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_Htmlrc, this, 0);
	}
	else
	{
		std::unique_ptr<CWnd> pRefView = std::make_unique<CWnd>();
		pRefView->Create(NULL, NULL, WS_CHILD | WS_VSCROLL, CRect(0, 0, 50, 50), this, 0);

		CRect	crc;
		pRefView->GetClientRect(&crc);
		pRefView.reset();

		m_scrGap = 50 - crc.Width();

		m_Htmlrc.SetRect(-2, -2, m_Param.rect.Width() + 2 + m_scrGap, m_Param.rect.Height() + 2 + m_scrGap);
		m_pHtml = new CAxBrowser;
		m_pHtml->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_Htmlrc, this, 0);
		m_pHtml->SetParentView(m_pParent);
		m_pHtml->m_scrGap = m_scrGap;
	}
	if (IsDev())
	{
		m_sURL.Replace("www.ibks.com", "test.ibks.com");
	}

	if (!m_sURL.IsEmpty())
		m_pHtml->Navigate2(m_sURL);

	return 0;
}

BOOL CIEWnd::IsDev()
{
	CString strdata, name;
	name = Variant(nameCC, "");

	int readL = 0;
	char readB[1024];
	CString userD;
	userD.Format("%s\\%s\\%s\\%s.ini", m_sRoot, "user", name, name);
	readL = GetPrivateProfileString("MODE", "DEV", "", readB, sizeof(readB), userD);

	if (readL <= 0)
		return 0;

	CString sDEV;
	sDEV.Format("%s", readB);
	if (sDEV == "1")
		return TRUE;
	else
		return FALSE;
}

void CIEWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (!m_pHtml)
		return;

	CRect	rc;
	GetWindowRect(&rc);

	m_pHtml->SetWindowPos(NULL, -2, -2, rc.Width() + 2 + m_scrGap, rc.Height() + 2 + m_scrGap, NULL);
	Invalidate();
}

CString CIEWnd::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}