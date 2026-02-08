// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BrowserCtrl.h"
#include "ControlWnd.h"
#include "SimpleBrowser.h"
#include "CEdgeBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT constCtrlID		= 20900;
const UINT constBrowserCtrlID	= 1000;
const UINT constEdgeCtrlID	= 2000;

#define TAG_CODEVALIDATIONKEY "cate=item&key="
#define TAG_TERMVALIDATIONKEY "cate=term&key="
#define TAG_HELPVALIDATIONKEY "cate=help&key="
#define TAG_STOCKITEM_KEY	  "stockitem?code="

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pParent = nullptr;
	m_pWebBrowser = nullptr;

	m_strURL = "";
	m_strMap1 = "CK202000";
	m_strMap2 = "CK240091";

	m_strClassName = AfxRegisterWndClass(0);

	m_SData = "";
	m_sTime = "";
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_USER + 2401, OnOpenScreen)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "visible", m_visible, OnVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Text", m_strURL, OnTextChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Left", m_nLeft, OnLeftChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Top", m_nTop, OnTopChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Right", m_nRight, OnRightChanged, VT_I4)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Bottom", m_nBottom, OnBottomChanged, VT_I4)
	DISP_FUNCTION(CControlWnd, "viewURL", viewURL, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Print", Print, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "Save", Save, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "PrintData", _PrintData, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "navigate", navigate, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Refresh", Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetRect", SetRect, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {58553D20-F769-4AB6-AF78-3CAA7E115074}
static const IID IID_IControlWnd =
{ 0x58553d20, 0xf769, 0x4ab6, { 0xaf, 0x78, 0x3c, 0xaa, 0x7e, 0x11, 0x50, 0x74 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pEdgeBrowser = std::make_unique<CEdgeBrowser>(m_pParent);
	if (!m_pEdgeBrowser->Create(WS_BORDER | WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, constEdgeCtrlID))
		m_pEdgeBrowser = nullptr;

	m_pEdgeBrowser->SetParentWnd(m_bParentWnd);
	m_pEdgeBrowser->SetVisible(true);

	if (m_pEdgeBrowser->InitializeWebView())
		m_bEdgeBrowser = true;
	else
		m_bEdgeBrowser = false;

	m_pWebBrowser = std::make_unique<SimpleBrowser>(m_pParent, m_strMap1, m_strMap2);
	if (!m_pWebBrowser->Create(WS_BORDER | WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, constBrowserCtrlID))
	{
		m_pWebBrowser = nullptr;
	}

	if (!m_bEdgeBrowser)
	{
		if (m_pWebBrowser && !m_strURL.IsEmpty())
			m_pWebBrowser->Navigate(m_strURL);
	}

	return 0;
}

void CControlWnd::SetParam(_param *pParam)
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
	sToken = _T("/u");	// URL
	int nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		CString sTmp = m_Param.options.Mid(nPos + sToken.GetLength());
		nPos = sTmp.Find("/");
		if (nPos < 0)
			m_strURL = sTmp;
		else
			m_strURL = sTmp.Left(nPos);
	}

	sToken = _T("/a");	// 링크할 현재가맵
	nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		CString sTmp = m_Param.options.Mid(nPos + sToken.GetLength());
		nPos = sTmp.Find("/");
		if (nPos < 0)
			m_strMap1 = sTmp;
		else
			m_strMap1 = sTmp.Left(nPos);
	}

	sToken = _T("/b");	// 링크할 도움말맵
	nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
	{
		CString sTmp = m_Param.options.Mid(nPos + sToken.GetLength());
		nPos = sTmp.Find("/");
		if (nPos < 0)
			m_strMap2 = sTmp;
		else
			m_strMap2 = sTmp.Left(nPos);
	}

	m_bParentWnd = false;
	sToken = _T("/t");
	nPos = m_Param.options.Find(sToken, 0);
	if (nPos >= 0)
		m_bParentWnd = true;

	if (!m_strURL.IsEmpty())
	{
		if (m_strURL.Find("http://") < 0)
			m_strURL = "http://" + m_strURL;
	}
}

long CControlWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
	{
		const _extTH* extTH = (_extTH*)lParam;
		const char* pExData = (char*)(lParam + L_extTH);

		CString strData = CString(pExData, extTH->size);
		CString strHome = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0L);
		CString strName = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0L);
		CString strPath(_T(""));
		strPath.Format("%s\\user\\%s\\NewsContents.html", strHome, strName);

		CFile file;
		CFileException ex;
		file.Open((LPCTSTR)strPath, CFile::modeWrite |
			CFile::shareExclusive | CFile::modeCreate, &ex);

		file.Write((LPCTSTR)strData, strData.GetLength());
		file.Close();

		if (m_bEdgeBrowser)
		{
			if (m_pEdgeBrowser)
				m_pEdgeBrowser->Navigate((LPCTSTR)strPath);
		}
		else
		{
			if (m_pWebBrowser)
				m_pWebBrowser->Navigate((LPCTSTR)strPath);
		}
	}
	break;
	}

	return 0;
}

void CControlWnd::OnDestroy() 
{
	if (m_pWebBrowser != nullptr)
		m_pWebBrowser->DestroyWindow();

	if (m_pEdgeBrowser != nullptr)
		m_pEdgeBrowser->DestroyWindow();

	CWnd::OnDestroy();
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	ReCalcLayout();
}

void CControlWnd::ReCalcLayout()
{
	CRect rcClient;
	GetClientRect(rcClient);

	if (m_bEdgeBrowser)
	{
		if (m_pEdgeBrowser)
			m_pEdgeBrowser->MoveWindow(&rcClient);
	}
	else
	{
		if (m_pWebBrowser)
			m_pWebBrowser->MoveWindow(&rcClient);
	}
}

void CControlWnd::viewURL(LPCTSTR url) 
{
	m_strURL.Format("%s", url);

	if (m_bEdgeBrowser)
	{
		if (m_pEdgeBrowser)
			m_pEdgeBrowser->Navigate(m_strURL);
	}
	else
	{
		if (m_pWebBrowser)
			m_pWebBrowser->Navigate(m_strURL);
	}
}

void CControlWnd::Print() 
{
	if (m_bEdgeBrowser)
	{
		if (m_pEdgeBrowser)
			m_pEdgeBrowser->Print();
	}
	else
	{
		if (m_pWebBrowser)
			m_pWebBrowser->Print(m_SData, m_sTime);
	}
}

void CControlWnd::Save(LPCTSTR strType, LPCTSTR strTitle, LPCTSTR strCodeName, LPCTSTR strDate, LPCTSTR strTime, LPCTSTR strSubType, LPCTSTR strCode, LPCTSTR strKey) 
{
	CBrowserCtrlApp	*pApp = (CBrowserCtrlApp*)AfxGetApp();
	CString strSaveData(_T(""));
	CString strNewsPath(_T(""));
	CString strPath(_T(""));
	CString strTmp(_T(""));
	CString strValue(_T(""));

	CString strTmp1, strTmp2;
	if (strlen(strDate) == 8)
		strTmp1.Format("%.4s/%.2s/%.2s", strDate, &strDate[4], &strDate[6]);
	else
		strTmp1 = strDate;

	if (strlen(strTime) == 6)
		strTmp2.Format("%.2s:%.2s:%.2s", strTime, &strTime[2], &strTime[4]);
	else
		strTmp2 = strTime;

	strSaveData.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", strType, strTitle, strCodeName, 
		strTmp2, strTmp1, strSubType, strCode, strKey);
	
	strNewsPath.Format("%s\\user\\%s\\newsconf.ini", pApp->GetRoot(m_pParent), pApp->GetName(m_pParent));
	int nStartIndex = GetPrivateProfileInt(_T("SCRAP"), _T("STARTGROUP"), 0, (LPCTSTR)strNewsPath);
	
	strPath.Format("%s\\user\\%s\\newsscrap.i%02d", pApp->GetRoot(m_pParent), pApp->GetName(m_pParent), nStartIndex);
	
	int nScrapCount = GetPrivateProfileInt(_T("COMMON"), _T("COUNT"), 0, (LPCTSTR)strPath);
	strTmp.Format("%d", nScrapCount++);
	WritePrivateProfileString(_T("SCRAP"), (LPCTSTR)strTmp, (LPCTSTR)strSaveData, (LPCTSTR)strPath);
	
	if (nScrapCount == 100)
	{
		strValue.Format("%d", nScrapCount);
		WritePrivateProfileString(_T("COMMON"), _T("COUNT"), (LPCTSTR)strValue, (LPCTSTR)strPath);
		
		strValue.Format("%d", ++nStartIndex);
		WritePrivateProfileString(_T("SCRAP"), _T("STARTGROUP"), (LPCTSTR)strValue, (LPCTSTR)strNewsPath);
		
		strPath.Format("%s\\user\\%s\\newsscrap.i%02d", pApp->GetRoot(m_pParent), pApp->GetName(m_pParent), nStartIndex);
		
		CFileFind ff;
		if (ff.FindFile(strPath) != 0)
			DeleteFile((LPCTSTR)strPath);
		
		nScrapCount = 0;
	}
	
	strValue.Format("%d", nScrapCount);
	WritePrivateProfileString(_T("COMMON"), _T("COUNT"), (LPCTSTR)strValue, (LPCTSTR)strPath);
}

void CControlWnd::_PrintData(LPCTSTR sData, LPCTSTR sTime) 
{
	m_SData = sData;
	m_sTime = sTime;
}

LRESULT CControlWnd::OnOpenScreen(WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = S_FALSE;
	CString strUrl, strCode, strParam;
	strUrl = (LPCTSTR)lParam;
	bool	bflag = FALSE;
	bool	bfpop = FALSE;

	if (strUrl.Find(TAG_CODEVALIDATIONKEY) != -1)
	{
		strCode = strUrl.Mid(strUrl.Find(TAG_CODEVALIDATIONKEY) + strlen(TAG_CODEVALIDATIONKEY) + 1);
		strParam.Format("%s/S/p5/d 1301\t%s", m_strMap1, strCode);
		lResult = S_OK;
		bfpop = TRUE;
	}
	else if (strUrl.Find(TAG_STOCKITEM_KEY) != -1)
	{
		strCode = strUrl.Mid(strUrl.Find(TAG_STOCKITEM_KEY) + strlen(TAG_STOCKITEM_KEY) + 1);
		strParam.Format("%s/S/p5/d 1301\t%s", m_strMap1, strCode);
		lResult = S_OK;
		bfpop = TRUE;
	}
	else if (strUrl.Find(TAG_TERMVALIDATIONKEY) != -1)
	{
		strCode = strUrl.Mid(strUrl.Find(TAG_TERMVALIDATIONKEY) + strlen(TAG_TERMVALIDATIONKEY));
		strParam.Format("%s/S/p5/d GUBN\t91\nSEQN\t%s", m_strMap2, strCode);
		lResult = S_OK;
		bflag = TRUE;
	}
	else if (strUrl.Find(TAG_HELPVALIDATIONKEY) != -1)
	{
		strCode = strUrl.Mid(strUrl.Find(TAG_HELPVALIDATIONKEY) + strlen(TAG_HELPVALIDATIONKEY));
		strParam.Format("%s/S/p5/d GUBN\t92\nSEQN\t%s", m_strMap2, strCode);
		lResult = S_OK;
	}

	if (lResult == S_OK)
	{
		if (!bflag)
		{
			if (!bfpop)
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strParam);
			else
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUP), (LPARAM)(LPCTSTR)strParam);

		}
		else
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUPX), (LPARAM)(LPCTSTR)strParam);
	}


	return lResult;
}

void CControlWnd::navigate(LPCTSTR url)
{
	viewURL(url);
}

void CControlWnd::Refresh()
{
	if (m_bEdgeBrowser)
		m_pEdgeBrowser->Invalidate();
	else
		m_pWebBrowser->Invalidate();
}

void CControlWnd::SetRect(long Left, long top, long right, long bottom)
{

}

void CControlWnd::OnVisibleChanged()
{	
	if (m_bEdgeBrowser)
	{
		if (m_pEdgeBrowser)
			m_pEdgeBrowser->SetVisible(m_visible);
	}
	::ShowWindow(GetSafeHwnd(), m_visible);
}

void CControlWnd::OnTextChanged()
{
	viewURL(m_strURL);
}

void CControlWnd::OnLeftChanged()
{

}

void CControlWnd::OnTopChanged()
{

}

void CControlWnd::OnRightChanged()
{

}

void CControlWnd::OnBottomChanged()
{

}