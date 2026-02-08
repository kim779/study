// AxisGCom.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AxisGCom.h"
#include "pnInput.h"
#include "pnSise.h"
#include "pnCtrl.h"
#include "pnTool.h"
#include "pnTable.h"
#include "PnAid.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

__declspec(dllexport) CWnd* WINAPI axCreateCtrl(int iCtrlKind, CWnd* pwndView, CWnd* pwndGView, char* pcInfo, CFont* pFont)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisGComApp* pApp = (CAxisGComApp*)AfxGetApp();

	switch (iCtrlKind)
	{
	case PN_INPUT:
		{
			CPnInput* pInput = new CPnInput(pwndView, pwndGView, pFont, iCtrlKind, pcInfo);
			pInput->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
				CRect(0, 0, 0, 0), pwndGView, 0);
			return pInput;
		}
	case PN_SISE:
		{
			CPnSise* pSise = new CPnSise(pwndView, pwndGView, pFont, iCtrlKind, pcInfo);
			pSise->Create(NULL, NULL, WS_CHILD | WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
				CRect(0, 0, 0, 0), pwndGView, 0);
			return pSise;
		}
	case PN_CTRL:
		{
			CPnCtrl* pCtrl = new CPnCtrl(pwndView, pwndGView, pFont, iCtrlKind, pcInfo);
			pCtrl->Create(NULL, NULL, WS_CHILD | WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
				CRect(0, 0, 0, 0), pwndGView, 0);
			return pCtrl;
		}
	case PN_TOOL:
		{
			CPnTool* pPnTool = new CPnTool(pwndView, pwndGView, pFont, iCtrlKind, pcInfo);
			pPnTool->Create(NULL, NULL, WS_CHILD | WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
				//CRect(0, 0, 0, 0), pwndGView, 0);
				CRect(0, 0, 32, 32), pwndGView, 0);
			return pPnTool;
		}
//	case PN_CFG:
//		return NULL;
	case PN_TABLE:
		{
			CPnTable* pTable = new CPnTable(pwndView, pwndGView, pFont, iCtrlKind, pcInfo);
			pTable->Create(NULL, NULL, WS_CHILD | WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
				CRect(0, 0, 0, 0), pwndGView, 0);
			return pTable;
		}
//	case PN_INFO:
//		return NULL;

	case PN_AID:	// 보조지표 툴바
		{
			CPnAid* pPnAid = new CPnAid(pwndView, pwndGView, pFont, iCtrlKind, pcInfo);
			pPnAid->Create(NULL, NULL, WS_CHILD | WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
				CRect(0, 0, 32, 32), pwndGView, 0);
			return pPnAid;
		}
		break;
	}
	return (CWnd *)NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CAxisGComApp

BEGIN_MESSAGE_MAP(CAxisGComApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisGComApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisGComApp construction
CAxisGComApp::CAxisGComApp()
{
	m_pImgDate = NULL;
	m_pImgTime = NULL;
	m_pImgNTime = NULL;
	m_pImgNBtn = NULL;
	m_pImgCode = NULL;
	m_pImgIEtc = NULL;
	m_pImgIOpt = NULL;
	m_pImgICfg = NULL;
	m_pImgTool = NULL;
	m_pImgTool2 = NULL;
	m_pImgCtrl = NULL;
	m_pImgCtrl2 = NULL;
	//** macho add
	m_pImgUnit = NULL;

	m_root = _T("");

//	m_pMyTool = NULL;
}

CAxisGComApp::~CAxisGComApp()
{
	if (m_pImgDate)		IMG_DELETE(m_pImgDate);
	if (m_pImgTime)		IMG_DELETE(m_pImgTime);
	if (m_pImgNTime)	IMG_DELETE(m_pImgNTime);
	if (m_pImgNBtn)		IMG_DELETE(m_pImgNBtn);
	if (m_pImgCode)		IMG_DELETE(m_pImgCode);
	if (m_pImgIEtc)		IMG_DELETE(m_pImgIEtc);
	if (m_pImgIOpt)		IMG_DELETE(m_pImgIOpt);
	if (m_pImgICfg)		IMG_DELETE(m_pImgICfg);
	if (m_pImgTool)		IMG_DELETE(m_pImgTool);
	if (m_pImgTool2)	IMG_DELETE(m_pImgTool2);
	if (m_pImgCtrl)		IMG_DELETE(m_pImgCtrl);
	if (m_pImgCtrl2)	IMG_DELETE(m_pImgCtrl2);
	//** macho add
	if (m_pImgUnit)		IMG_DELETE(m_pImgUnit);

//	if (m_pMyTool)
//		LIB_DELETE(m_pMyTool);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAxisGComApp object

CAxisGComApp theApp;

CString CAxisGComApp::GetImagePath(CWnd* pwndView, CString imgName)
{
	CString imgPath = _T("");
	CString root = GetHome(pwndView);
	imgPath.Format("%s\\%s\\%s", root, IMAGEDIR, imgName);
	return imgPath;
}

CString CAxisGComApp::GetUserPath(CWnd* pwndView)
{
	CString root = GetHome(pwndView);
	CString user = GetName(pwndView);

	CreateDirectory(root + "\\User", NULL);
	CString path;
	path.Format("%s\\User\\%s\\", root, user);
	CreateDirectory(path, NULL);
	return path;
}

CImageList* CAxisGComApp::GetImage(CString path, int Width, int Height, UINT flag, COLORREF crMask)
{
	CImageList* pImage = NULL;
	HBITMAP hbitmap = (HBITMAP) LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hbitmap)
	{
		CBitmap	bitmap;
		bitmap.DeleteObject();
		bitmap.Attach(hbitmap);
		pImage = new CImageList();
		pImage->Create(Width, Height, flag, 0, 1);
		pImage->Add(&bitmap, crMask);
		bitmap.DeleteObject();
	}
	return pImage;
}

CImageList* CAxisGComApp::GetImage(int imgType, CString path)
{
	switch (imgType)
	{
	case imgDate:
		if (!m_pImgDate)
			m_pImgDate = GetImage(path, szDateImgX, szDateImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgDate;
	case imgTime:
		if (!m_pImgTime)
			m_pImgTime = GetImage(path, szTimeImgX, szTimeImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgTime;
	case imgNTime:
		if (!m_pImgNTime)
			m_pImgNTime = GetImage(path, szTimeImgX, szTimeImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgNTime;
	case imgNBtn:
		if (!m_pImgNBtn)
			m_pImgNBtn = GetImage(path, szIBtnImgX, szIBtnImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgNBtn;
	case imgCode:
		if (!m_pImgCode)
			m_pImgCode = GetImage(path, szCodeImgX, szCodeImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgCode;
	case imgIEtc:
		if (!m_pImgIEtc)
			m_pImgIEtc = GetImage(path, szIEtcImgX, szIEtcImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgIEtc;
	case imgIOpt:
		if (!m_pImgIOpt)
			m_pImgIOpt = GetImage(path, szIOptImgX, szIOptImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgIOpt;
	case imgICfg:
		if (!m_pImgICfg)
			m_pImgICfg = GetImage(path, szICfgImgX, szICfgImgY,
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgICfg;
	case imgTool:
		if (!m_pImgTool)
			m_pImgTool = GetImage(path, szToolImgX, szToolImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgTool;
	case imgTool2:
		if (!m_pImgTool2)
			m_pImgTool2 = GetImage(path, szTool2ImgX, szTool2ImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgTool2;
	case imgCtrl:
		if (!m_pImgCtrl)
			m_pImgCtrl = GetImage(path, szCtrlImgX, szCtrlImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgCtrl;
	case imgCtrl2:
		if (!m_pImgCtrl2)
			m_pImgCtrl2 = GetImage(path, szCtrl2ImgX, szCtrl2ImgY, 
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgCtrl2;
	//** macho add
	case imgIUnit:
		if (!m_pImgUnit)
			m_pImgUnit = GetImage(path, 49, 18, //szUnitImgX, szUnitImgY,
			ILC_COLOR24|ILC_MASK, RGB(255, 0, 255));
		return m_pImgUnit;
	}
	return NULL;
}


char* CAxisGComApp::GetHome(CWnd* pwndView)
{
	return (char*)pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), NULL);
}


char* CAxisGComApp::GetName(CWnd* pwndView)
{
	return (char*)pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), NULL);
}

CFont* CAxisGComApp::GetFont(CWnd* pwndView, int iPoint, CString strName, int iBold, bool bItalic)
{
	struct	_fontR fontR;

	fontR.name	= (char*)(strName.operator LPCTSTR());
	fontR.point	= iPoint;
	fontR.italic	= bItalic;
	fontR.bold	= iBold;

	return (CFont*)pwndView->SendMessage(WM_USER, (WPARAM)MAKEWORD(getFONT, NULL), (LPARAM)&fontR);
}

CPen* CAxisGComApp::GetPen(CWnd* pwndView, int iStyle, int iWidth, COLORREF rgbLine)
{
	struct	_penR	penR;
	penR.clr = rgbLine;
	penR.width = iWidth;
	penR.style = iStyle;

	return (CPen*)pwndView->SendMessage(WM_USER, (WPARAM)MAKEWORD(getPEN, NULL), (LPARAM)&penR);
}

CBrush* CAxisGComApp::GetBrush(CWnd* pwndView, COLORREF rgbBrush)
{
	return (CBrush*)pwndView->SendMessage(WM_USER, (WPARAM)MAKEWORD(getBRUSH, NULL), (LPARAM)rgbBrush);
}

CBitmap* CAxisGComApp::GetBitmap(CWnd* pwndView, CString strPath)
{
	return (CBitmap*)pwndView->SendMessage(WM_USER, getBITMAP, (LPARAM)strPath.operator LPCTSTR());
}