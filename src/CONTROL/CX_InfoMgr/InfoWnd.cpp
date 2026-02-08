// InfoWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_InfoMgr.h"
#include "InfoWnd.h"
#include "InfoPopup.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoWnd

CInfoWnd::CInfoWnd(CWnd *pView, CWnd *pParent, struct _param* pParam)
{
	EnableAutomation();

	m_pView = pView;
	m_pParent = pParent;

	m_arInfo.RemoveAll();
	m_pInfoPopup = nullptr;
	m_SelInfo.key = m_SelInfo.val = "";
	m_sSection = "InfoMgr";
	m_sListHdrKey = "Key";
	m_sListHdrVal = "Val";
	m_PopWidth = 300;
	m_PopHeight = 200;

	m_Param.key   = pParam->key;
	m_Param.name  = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB  = pParam->tRGB;
	m_Param.pRGB  = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
}

CInfoWnd::~CInfoWnd()
{
	WriteSectionInfo(m_sFileName, m_sSection);
	m_arInfo.RemoveAll();
}

void CInfoWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CInfoWnd, CWnd)
	//{{AFX_MSG_MAP(CInfoWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CInfoWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CInfoWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IInfoWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {8148CB88-1196-41FE-9B83-7AB9934D07DA}
static const IID IID_IInfoWnd =
{ 0x8148cb88, 0x1196, 0x41fe, { 0x9b, 0x83, 0x7a, 0xb9, 0x93, 0x4d, 0x7, 0xda } };

BEGIN_INTERFACE_MAP(CInfoWnd, CWnd)
	INTERFACE_PART(CInfoWnd, IID_IInfoWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoWnd message handlers

CString CInfoWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char*	dta = (char*)m_pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

void CInfoWnd::CreateFont(int point, CString sFont)
{
	struct	_fontR	fontR {};

	fontR.name   = (char*)sFont.GetString();
	fontR.point  = point;
	fontR.italic = false;
	fontR.bold   = FW_NORMAL;

	m_pFont = (CFont *)m_pView->SendMessage(WM_USER, MAKEWPARAM(getFONT, 0), (LPARAM)&fontR);
}


CString CInfoWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

CString CInfoWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}

void CInfoWnd::ReadSectionInfo(CString sFile, CString sSection)
{
	CString	sKey, sTmp;
	char	pReadBuf[1024]{};

	GetPrivateProfileString(sSection, "Count", "", pReadBuf, sizeof(pReadBuf), sFile);

	_Info	info{};
	const	int	size = atoi(pReadBuf);
	const	int	sz_SEP = strlen(IS_SEP);

	m_arInfo.RemoveAll();
	for (int ii = 0, index = 0; ii < size; ii++)
	{
		sKey.Format("%03d", ii);
		GetPrivateProfileString(sSection, sKey, "", pReadBuf, sizeof(pReadBuf), sFile);

		sTmp = CString(pReadBuf, sizeof(pReadBuf));
		index = sTmp.Find(IS_SEP);
		if (index > 0)
		{
			info.key = sTmp.Left(index);
			info.val = sTmp.Mid(index+sz_SEP);
			m_arInfo.Add(info);
		}
	}
}

void CInfoWnd::WriteSectionInfo(CString sFile, CString sSection)
{
	_Info	info{};
	CString	sKey, sVal;
	const	int	size = m_arInfo.GetSize();

	sVal.Format("%03d", size);
	WritePrivateProfileString(sSection, "Count", sVal, sFile);

	for (int ii = 0; ii < size; ii++)
	{
		info = m_arInfo.GetAt(ii);
		sKey.Format("%03d", ii);
		sVal.Format("%s%s%s", info.key, IS_SEP, info.val);
		WritePrivateProfileString(sSection, sKey, sVal, sFile);
	}
}

void CInfoWnd::AddData(CString sKey, CString sVal)
{
	sKey.Trim();
	sVal.Trim();

	_Info	info{};
	const	int	size = m_arInfo.GetSize();
	for (int ii = 0; ii < size; ii++)
	{
		info = m_arInfo.GetAt(ii);
		if (info.key == sKey)	// 이미 존재하면 맨처음으로
		{
			m_arInfo.RemoveAt(ii);
			info.val = sVal;	// val 갱신
			m_arInfo.InsertAt(0, info);
			return;
		}
	}

	info.key = sKey;
	info.val = sVal;
	m_arInfo.InsertAt(0, info);
}

void CInfoWnd::RemoveData(CString sKey)
{
	sKey.Trim();

	_Info	info{};
	const	int	size = m_arInfo.GetSize();
	for (int ii = 0; ii < size; ii++)
	{
		info = m_arInfo.GetAt(ii);
		if (info.key == sKey)
		{
			m_arInfo.RemoveAt(ii);
			return;
		}
	}
}

void CInfoWnd::RemoveAllData()
{
	m_arInfo.RemoveAll();
}

void CInfoWnd::ShowData()
{
	if (m_pInfoPopup)
		m_pInfoPopup.reset();

	CRect	rc;
	CString sClassName = AfxRegisterWndClass(0);

	GetWindowRect(&rc);
	m_pInfoPopup = std::make_unique<CInfoPopup>(this);
	if (!m_pInfoPopup->CreateEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_DLGMODALFRAME, sClassName, nullptr,
			WS_POPUP|WS_BORDER, CRect(rc.right, rc.top, rc.right + m_PopWidth, rc.top + m_PopHeight),
			nullptr, 0, nullptr))
	{
		m_pInfoPopup = nullptr;
		return;
	}

	m_SelInfo.key = "";
	m_SelInfo.val = "";
	m_pInfoPopup->ShowWindow(SW_SHOW);
}

void CInfoWnd::SelectData(_Info info)
{
	m_pInfoPopup->ShowWindow(SW_HIDE);

	m_SelInfo.key = info.key;
	m_SelInfo.val = info.val;
	m_pView->PostMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, 12)), (LPARAM)m_Param.name.GetString());
}

CString CInfoWnd::GetSelectKey()
{
	return m_SelInfo.key;
}

CString CInfoWnd::GetRecentKey()
{
	if (m_arInfo.GetSize() > 0)
	{
		_Info	info = m_arInfo.GetAt(0);
		return info.key;
	}

	return _T("");
}

CString CInfoWnd::GetSelectVal()
{
	return m_SelInfo.val;
}

CString CInfoWnd::GetRecentVal()
{
	if (m_arInfo.GetSize() > 0)
	{
		_Info	info = m_arInfo.GetAt(0);
		return info.val;
	}

	return _T("");
}

CString CInfoWnd::GetAllData()
{
	CString	sItem, retStr = _T("");
	for (int ii = 0; ii < m_arInfo.GetSize(); ii++)
	{
		_Info	info = m_arInfo.GetAt(ii);
		sItem.Format("%s|%s#", info.key, info.val);

		retStr += sItem;
	}

	return retStr;
}

int CInfoWnd::GetListCount()
{
	return m_arInfo.GetSize();
}

CString CInfoWnd::GetListHdrKey()
{
	return m_sListHdrKey;
}

CString	CInfoWnd::GetListHdrVal()
{
	return m_sListHdrVal;
}

CString CInfoWnd::GetListDataKey(int index)
{
	if (index < 0 && m_arInfo.GetSize() <= index)
		return _T("");

	return m_arInfo.GetAt(index).key;
}

CString	CInfoWnd::GetListDataVal(int index)
{
	if (index < 0 && m_arInfo.GetSize() <= index)
		return _T("");

	return m_arInfo.GetAt(index).val;
}

int CInfoWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateFont(m_Param.point, m_Param.fonts);

	CString tmpS  = m_Param.options;
	CString	sHome = Variant(homeCC, "");
	CString	sName = Variant(nameCC, "");
	m_sFileName.Format("%s\\user\\%s\\%s", sHome, sName, "InfoMgr.ini");

	CString	tmp = OptionParser(tmpS, "/sn");if (!tmp.IsEmpty())	m_sSection = tmp;
	tmp = OptionParser(tmpS, "/lk");	if (!tmp.IsEmpty())	m_sListHdrKey = tmp;
	tmp = OptionParser(tmpS, "/lv");	if (!tmp.IsEmpty())	m_sListHdrVal = tmp;
	tmp = OptionParser(tmpS, "/w");		if (atoi(tmp) > 0)	m_PopWidth = atoi(tmp);
	tmp = OptionParser(tmpS, "/h");		if (atoi(tmp) > 0)	m_PopHeight = atoi(tmp);

	ReadSectionInfo(m_sFileName, m_sSection);
	
	return 0;
}

void CInfoWnd::OnDestroy() 
{
	if (m_pInfoPopup)
		m_pInfoPopup.reset();

	CWnd::OnDestroy();
}

void CInfoWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);

	ShowData();
}

void CInfoWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	rc;

	GetClientRect(&rc);
	dc.FillSolidRect(&rc, RGB(255,0,0));	
}
