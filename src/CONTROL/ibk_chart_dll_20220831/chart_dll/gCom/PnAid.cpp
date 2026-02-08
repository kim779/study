// PnAid.cpp : implementation file
//

#include "stdafx.h"
#include "axisgcom.h"
#include "PnAid.h"
#include "PnAidInfo.h"
#include "memDC.h"
#include "PnToolTip.h"
//#include "../Common/MyTool.h"
#include "../Common/SavedHead.h"
#include "../../h/axisgobj.h"


#include "../MxTrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define RGB_BG		RGB(211,211,211)


/////////////////////////////////////////////////////////////////////////////
// CPnAid

CPnAid::CPnAid(CWnd* pwndView, CWnd* pwndGView, CFont* pFont, int iCtrlKind, char* pcInfo)
{
	m_pwndGView	= pwndGView;
	m_pwndView	= pwndView;
	m_pfont		= pFont;
	m_pfontSmall	= NULL;

	m_iFocusIndex	= -1;
	m_iLBDownIndex	= -1;

	
//	m_pMyTool = ((CAxisGComApp *)AfxGetApp())->GetMyTool(pwndView);
	m_pApp		= (CAxisGComApp*)AfxGetApp();
	m_bTracking	= FALSE;
	m_pToolTip	= NULL;
	
}

CPnAid::~CPnAid()
{
	RemoveArray();
//	RemoveMap();
}


BEGIN_MESSAGE_MAP(CPnAid, CWnd)
	//{{AFX_MSG_MAP(CPnAid)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(GEV_AID, OnAidEvent)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPnAid message handlers

int CPnAid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	
	//return 0;
	return Init();
}

int CPnAid::Init()
{
	//m_strMapName = "MG046000";
	m_strMapName = (char*)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, getDllName), NULL);
	if (!Load())
		InitAid();

	if (!m_pToolTip)
	{
		m_pToolTip = new CPnToolTip(m_pwndView, this, GEV_AID);
		m_pToolTip->Create(m_pfont);
	}

	m_pfontSmall = m_pApp->GetFont(m_pwndView, 7, "굴림", FALSE, 0);

	return 0;
}

bool CPnAid::Load()
{
	CString strFileName;
	//strFileName.Format("%s\\GEX\\%s\\%s%s", m_pMyTool->GetHome(), m_pMyTool->GetName(), m_strMapName, AID);
	strFileName.Format("%s\\GEX\\%s\\%s%s", m_pApp->GetHome(m_pwndView), m_pApp->GetName(m_pwndView), m_strMapName, AID);

	CSavedHead sh;
	DWORD dwReadLen;
	char* pcData = sh.Read(strFileName, dwReadLen);

	if (!pcData || dwReadLen < 1)
		return false;
	
	if (sh.m_sh.dwVersion < VERSION_AID || dwReadLen != sh.m_sh.dwTotalLength)
	{
		return false;
	}
	

	RemoveArray();

	for (UINT ii = 0; ii < sh.m_sh.dwDataCnt; ii++)
	{
		CPnAidInfo* pPAI = new CPnAidInfo;
		pPAI->SetStruck(pcData + ii * SZ_PNAIDINFO);

		m_arPnAidInfo.Add(pPAI);
	}

	return true;
}

bool CPnAid::Save()
{
	CString strFileName;
	//strFileName.Format("%s\\GEX\\%s\\%s%s", m_pMyTool->GetHome(), m_pMyTool->GetName(), m_strMapName, AID);
	strFileName.Format("%s\\GEX\\%s\\%s%s", m_pApp->GetHome(m_pwndView), m_pApp->GetName(m_pwndView), m_strMapName, AID);

	CSavedHead sh;
	//sh.Make(VERSION_AID, m_arPnAidInfo.GetSize(), SZ_PNAIDINFO);
	char* pcData = sh.Make(VERSION_AID, m_arPnAidInfo.GetSize(), SZ_PNAIDINFO);

	CPnAidInfo* pPnAidInfo;
	for (int ii = 0; ii < m_arPnAidInfo.GetSize(); ii++)
	{
		pPnAidInfo = m_arPnAidInfo.GetAt(ii);
		pPnAidInfo->GetStruck(pcData);
		pcData +=  SZ_PNAIDINFO;
	}

	//bool bResult = m_pMyTool->WriteFile(strFileName, pcBuf, SZ_SAVEDHEAD + m_arPnAidInfo.GetSize() * SZ_PNAIDINFO);
	bool bResult = sh.Write(strFileName);

	if (!bResult)
		return false;

	return true;
}

void CPnAid::InitAid()
{
	RemoveArray();

	struct _itemAid
	{
		char	acName[32];
		char	acText[16];
		int	iKind;
	};

	struct _itemAid	item[] = {
		{"가격 이평", "가격 이평", GK_PMA},		{"그물차트", "그물 차트", GK_NET},
		{"CCI",	"CCI", GK_CCI},				{"MACD", "MACD", GK_MACD},
		{"Fast Stochastics", "F - Stoc", GK_STOCHF},	{"Slow Stochastics", "S - Stoc", GK_STOCHS},
		{"거래량", "거래량", GK_VOL},			{"OBV",	"OBV", GK_OBV},
		{"심리도", "심리도", GK_PSY},			{"Bollinger Band", "Bollinger", GK_BOLB},
		{"Pivot Line", "Pivot", GK_PIVOT}
	};

	int iCount = sizeof (item) / sizeof(_itemAid);

	for (int ii = 0; ii < iCount; ii++)
	{
		CPnAidInfo* pPAI = new CPnAidInfo;
		pPAI->m_iKind = item[ii].iKind;
		pPAI->m_strText = CString(item[ii].acText);
		pPAI->m_strText.TrimRight();
		pPAI->m_strName = CString(item[ii].acName);
		pPAI->m_strName.TrimRight();


		m_arPnAidInfo.Add(pPAI);

	}

	ResetRect();



	

}

void CPnAid::ResetRect()
{
	for (int ii = 0; ii < m_arPnAidInfo.GetSize(); ii++)
	{
		CPnAidInfo* pPAI = m_arPnAidInfo.GetAt(ii);
		//pPAI->m_rect.SetRect(2, 2 + ii * 28, 30, 2 + (ii + 1) * 28);
		pPAI->m_rect.SetRect(2, 2 + ii * 30, 30, 2 + (ii + 1) * 30);
	}
}


void CPnAid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC	mdc(&dc);
	Draw(&mdc);
}

void CPnAid::Draw(CDC* pDC)
{
	CRect rectC, rect;
	GetClientRect(rectC);

	rect = rectC;
	CBrush	*pBGBbrush = m_pApp->GetBrush(m_pwndView, RGB_BG);
	COLORREF	clrTrans = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 181);
	pDC->FillSolidRect(&rect, clrTrans);

	CPen pn(PS_NULL, 1, clrTrans);
	CPen	*sPen = pDC->SelectObject(&pn);
	CBrush	*sBrush = pDC->SelectObject(pBGBbrush);
	pDC->RoundRect(rect,CPoint(5,5));

//	pDC->FillSolidRect(rectC, RGB_BG);
	
	int		sMode = pDC->SetBkMode(TRANSPARENT);
	CFont	*sFont = pDC->SelectObject(m_pfontSmall);

	for (int ii = 0; ii < m_arPnAidInfo.GetSize(); ii++)
	{
		CPnAidInfo* pPAI = m_arPnAidInfo.GetAt(ii);
		rect = pPAI->m_rect;
		rect.DeflateRect(1, 3);

		if (ii == m_iLBDownIndex)
		{
			pDC->Draw3dRect(rect, RGB(0, 0, 0), RGB(255, 255, 255));
		}
		else if (ii == m_iFocusIndex)
		{
			pDC->Draw3dRect(rect, RGB(255, 255, 255), RGB(0, 0, 0));
		}

		if (pPAI->m_strText.Find(' ') > 0)
			pDC->DrawText(pPAI->m_strText, rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
		else
			pDC->DrawText(pPAI->m_strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

/*	rect = rectC;
	rect.DeflateRect(1, 1, 1, 0);
	pDC->Draw3dRect(rect, RGB(255, 255, 255), RGB(0, 0, 0));*/

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
	pDC->SelectObject(sFont);
	pDC->SetBkMode(sMode);
}

void CPnAid::AidConfig()
{
	int iCount = m_arPnAidInfo.GetSize();
	char* pcBuffer = new char[SZ_PNAIDINFO * iCount];

	CPnAidInfo* pPnAidInfo;

	for (int ii = 0; ii < iCount; ii++)
	{
		pPnAidInfo = m_arPnAidInfo.GetAt(ii);
		pPnAidInfo->GetStruck(pcBuffer + SZ_PNAIDINFO * ii);
	}

	struct _aidConfig aidConfig;
	aidConfig.pcBuf = pcBuffer;
	aidConfig.iCount = iCount;

	if (m_pwndGView)
	{
		BOOL bResult = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, DlgAidCfg), (LPARAM)&aidConfig);
		if (bResult)
		{
			RemoveArray();

			for (int ii = 0; ii < aidConfig.iCountResult; ii++)
			{
				CPnAidInfo* pPAI = new CPnAidInfo;
				pPAI->SetStruck(aidConfig.pcBufResult + SZ_PNAIDINFO * ii);
				m_arPnAidInfo.Add(pPAI);
			}

			if (aidConfig.iCount > 0)
			{
				delete aidConfig.pcBufResult;
			}

			ResetRect();
			Save();
			Invalidate();
		}
	}

	delete pcBuffer;
}

void CPnAid::RemoveArray()
{
	for (int ii = 0; ii < m_arPnAidInfo.GetSize(); ii++)
	{
		CPnAidInfo* pPAI = m_arPnAidInfo.GetAt(ii);
		delete pPAI;
	}
	m_arPnAidInfo.RemoveAll();
}
/*
void CPnAid::RemoveMap()
{
	CPnAidInfo* pPAI;
	int iKey;
	for (POSITION pos = m_mapPnAidInfo.GetStartPosition(); pos;)
	{
		m_mapPnAidInfo.GetNextAssoc(pos, iKey, pPAI);
		delete pPAI;
	}
	m_mapPnAidInfo.RemoveAll();
}
*/

void CPnAid::OnDestroy()
{
	if (m_pToolTip)
		WND_DELETE(m_pToolTip);

	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CPnAid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	LButtonDown(point);
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CPnAid::LButtonDown(CPoint point) 
{
	int iIndex = FindIndex(point);
	if (iIndex == m_iLBDownIndex)
		return;

	if (iIndex >= 0)
	{
		if (m_iLBDownIndex >= 0 && m_iLBDownIndex < m_arPnAidInfo.GetSize())
		{
			CPnAidInfo* pPAI = m_arPnAidInfo.GetAt(m_iLBDownIndex);
			InvalidateRect(pPAI->m_rect);
		}

		m_iLBDownIndex = iIndex;

		if (m_iLBDownIndex >= 0 && m_iLBDownIndex < m_arPnAidInfo.GetSize())
		{
			CPnAidInfo* pPAI = m_arPnAidInfo.GetAt(m_iLBDownIndex);
			InvalidateRect(pPAI->m_rect);
		}
	}
	else
	{
		if (m_iLBDownIndex >= 0 && m_iLBDownIndex < m_arPnAidInfo.GetSize())
		{
			CPnAidInfo* pPAI = m_arPnAidInfo.GetAt(m_iLBDownIndex);
			InvalidateRect(pPAI->m_rect);
		}
		m_iLBDownIndex = -1;
	}
}

void CPnAid::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	LButtonUp(point);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CPnAid::LButtonUp(CPoint point) 
{
	int iLBDownIndex = m_iLBDownIndex;
	m_iLBDownIndex = -1;
	m_iFocusIndex = -1;
	CPnAidInfo* pPAI = NULL;
	if (iLBDownIndex >= 0 && iLBDownIndex < m_arPnAidInfo.GetSize())
	{
		pPAI = m_arPnAidInfo.GetAt(iLBDownIndex);
		InvalidateRect(pPAI->m_rect);
	}

	int iIndex = FindIndex(point);
	if (iIndex != iLBDownIndex || !pPAI)
		return;

	struct _envInfo* pEnvInfo = (struct _envInfo*)m_pwndGView->SendMessage(GEV_CHART, MAKELPARAM(CHART_DATA, REQUEST_CUR_ENV), NULL);
	switch (pEnvInfo->datainfo.wMainGrpKind)
	{
	case GK_CLOCK:
	case GK_PV:
	case GK_THREE:
	case GK_PNF:
	case GK_SWING:
	case GK_KAGI:
	case GK_RENKO:
	case GK_EQV:
	case GK_CDV:

		return;
	}
	// click process
	//m_pwndGView->SendMessage(GEV_VIEW, MAKELPARAM(viewNotify, aidAddCfg), pPAI->m_iKind);
	m_pwndGView->SendMessage(GEV_VIEW, MAKELPARAM(viewNotify, aidAddChart), pPAI->m_iKind);
}

void CPnAid::OnMouseMove(UINT nFlags, CPoint point) 
{
	MouseMove(point);
	
	CWnd::OnMouseMove(nFlags, point);
}

void CPnAid::mouseLeaveAction(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	m_iFocusIndex = -1;
	m_iLBDownIndex = -1;

	if (m_pToolTip)
	{
		m_pToolTip->m_dispStr = _T("");
		m_pToolTip->hideToolTip();
	}


	Invalidate();
}

void CPnAid::MouseMove(CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;	//HOVER_DEFAULT;
		m_bTracking = _TrackMouseEvent(&tme);
	}


	int iIndex = FindIndex(point);
	if (iIndex == m_iFocusIndex || m_iLBDownIndex >= 0)
		return;

	if (m_iFocusIndex >= 0 && m_iFocusIndex < m_arPnAidInfo.GetSize())
	{
		CPnAidInfo* pPAI = m_arPnAidInfo.GetAt(m_iFocusIndex);
		InvalidateRect(pPAI->m_rect);

	}

	m_iFocusIndex = iIndex;

	if (m_iFocusIndex >= 0 && m_iFocusIndex < m_arPnAidInfo.GetSize())
	{
		CPnAidInfo* pPAI = m_arPnAidInfo.GetAt(m_iFocusIndex);
		InvalidateRect(pPAI->m_rect);

	}

}

int CPnAid::FindIndex(CPoint point) 
{
	for (int ii = 0; ii < m_arPnAidInfo.GetSize(); ii++)
	{
		CPnAidInfo* pPAI = m_arPnAidInfo.GetAt(ii);
		if (pPAI->m_rect.PtInRect(point))
			return ii;
	}
	return -1;
}

LRESULT CPnAid::OnAidEvent(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case AID_CONFIG:
		AidConfig();
		break;

	case AID_GET_POS_COMMENT:
		m_strComment = GetPosComment(CPoint(LOWORD(lParam), HIWORD(lParam)));
		return (LRESULT)(LPCTSTR)m_strComment;

	}

	return 0;
}

LRESULT CPnAid::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (m_pToolTip)
		m_pToolTip->RelayEvent(message);	
	
	return CWnd::WindowProc(message, wParam, lParam);
}

CString CPnAid::GetPosComment(CPoint pt)
{
	ScreenToClient(&pt);
	CString strReturn;
	CPnAidInfo* pPnAidInfo;
	for (int ii = 0; ii < m_arPnAidInfo.GetSize(); ii++)
	{
		pPnAidInfo = m_arPnAidInfo.GetAt(ii);

		if (pPnAidInfo->m_rect.PtInRect(pt))
		{
			strReturn = CString(pPnAidInfo->m_strName);
			strReturn.TrimRight();
			return strReturn;
		}
	}

	return _T("");
}