// CodeReg.cpp : implementation file
//

#include "stdafx.h"
#include "CodeReg.h"
#include "Tool.h"
#include "UserDefine.h"
#include "Resource.h"
#include "../../h/AxisVar.h"
#include "../../h/axisfire.h"
#include "../../control/fx_grid/memdc.h"
#include "../../control/fx_misc/fxImgButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_CTRL_HEAD	0x01
#define CELL_HEIGHT	19
#define	SCROLL_WIDTH	16
/////////////////////////////////////////////////////////////////////////////
// CCodeReg

CCodeReg::CCodeReg(class CTool* pTool, bool bMenu)
{
	m_pTool		= pTool;
	m_bPopMenu	= bMenu;

	m_bLBDown	= false;
	m_iRow		= 0;
	m_pBtnHead	= NULL;	

	m_ppenLine	= NULL;
	m_pfont		= NULL;
	m_clrBG		= RGB(255, 255, 255);
	m_clrDT		= RGB(255, 255, 255);
	m_iCellHeight = CELL_HEIGHT;
	m_iCellTop = CTRL_HEIGHT + GAP;

}

CCodeReg::~CCodeReg()
{
	if (m_pBtnHead)
	{
		::DestroyWindow(m_pBtnHead->m_hWnd);
		m_pBtnHead = nullptr;
		m_pBtnHead = NULL;
	}	
}


BEGIN_MESSAGE_MAP(CCodeReg, CWnd)
	//{{AFX_MSG_MAP(CCodeReg)
	ON_BN_CLICKED(ID_CTRL_HEAD, OnButtonHead)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_DELETE, OnMenuDelete)
	ON_COMMAND(ID_MENU_DELETEALL, OnMenuDeleteall)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCodeReg message handlers

int CCodeReg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	Init();
	return 0;
}

void CCodeReg::Init()
{
	m_clrBG = m_pTool->GetIndexColor(CONTENTS_BACK_INDEX);
	m_clrDT	= m_pTool->GetIndexColor(COLOR_DATA);

	m_ppenLine = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	m_pfont = m_pTool->RequestFont("굴림체", 9, FALSE, 0);

	CRect rc, cRc;
	GetClientRect(&rc);

	m_iRow = (rc.Height() - CTRL_HEIGHT) / CELL_HEIGHT + 1;

	cRc.SetRect(0, 0, rc.right, CTRL_HEIGHT);
	
	m_pBtnHead = std::make_unique<CfxImgButton>(m_pfont);

	m_pBtnHead->Create("\n종목 추가", cRc, this, ID_CTRL_HEAD, FALSE, TRUE);
	m_pBtnHead->SetWindowText("종목 추가");
	m_pBtnHead->SetTextColor(m_pTool->GetIndexColor(69));

	CString strPath;
	strPath.Format("%s\\%s\\", m_pTool->GetHome(), IMAGEDIR);
	HBITMAP hbmp	= m_pTool->RequestBitmap(strPath + "11btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn = m_pTool->RequestBitmap(strPath + "11btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = m_pTool->RequestBitmap(strPath + "11btn_en.bmp")->operator HBITMAP();
	m_pBtnHead->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);

	LoadCode();
}

void CCodeReg::DrawSiseText(CDC* pDC, CString strData, CRect rc, UINT nFormat)
{
	if (strData.IsEmpty())
		return;
	
	nFormat = DT_SINGLELINE | DT_VCENTER | nFormat;

	CFont* pfontOld = pDC->SelectObject(m_pfont);


	const COLORREF clrTextOld = pDC->SetTextColor(RGB(0, 0, 0));
	pDC->DrawText(strData, rc, nFormat);

	pDC->SelectObject(pfontOld);
	pDC->SetTextColor(clrTextOld);
}

//LRESULT CCodeReg::OnButtonHead()
void CCodeReg::OnButtonHead()
{
	CRect rect;
	GetDlgItem(ID_CTRL_HEAD)->GetWindowRect(rect);
	CString strCODE = m_pTool->GetJCodeByDlg(kkelwCODE, NULL, rect.left, rect.bottom + 2);
	

	const int iPos = strCODE.Find('\t');
	if (iPos > 0)
	{
		AddJCod(strCODE.Left(iPos), strCODE.Mid(iPos + 1));
	}

}

void CCodeReg::AddJCod(CString strJCode, CString strJName)
{
	strJCode.TrimLeft();
	strJCode.TrimRight();
	strJName.TrimLeft();
	strJName.TrimRight();

	if (strJCode.IsEmpty() || strJName.IsEmpty())
		return;

	struct _code Code;
	for (int ii = 0; ii < m_arCode.GetSize(); ii++)
	{
		Code = m_arCode.GetAt(ii);
		if (Code.code == strJCode)
		{
			m_arCode.RemoveAt(ii);
			break;
		}
	}
	
	Code.code = strJCode;
	Code.name = strJName;
	m_arCode.InsertAt(0, Code);

	if (m_arCode.GetSize() > m_iRow)
		m_arCode.RemoveAt(m_arCode.GetSize() - 1);

	Invalidate();
	SaveCode();
}

void CCodeReg::LoadCode()
{
	m_arCode.RemoveAll();

	CString key, section = "CODEREG", data = "", path = "";
	path.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);
	char	ssb[1024]{};

	for (int ii = 0; ii < 100; ii++)
	{
		key.Format("%02d", ii);
		GetPrivateProfileString(section, key, "", ssb, sizeof(ssb), path);
		data = ssb;
		
		const int pos = data.Find("|");
		if (pos > 0)
		{
			struct _code code;
			code.code = data.Left(pos);
			code.name = data.Mid(pos + 1);

			m_arCode.Add(code);
		}
	}
}

void CCodeReg::SaveCode()
{
	CString key, section = "CODEREG", data = "", path = "";
	path.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);	
	WritePrivateProfileSection(section, _T(""), path);

	for (int ii = 0; ii < m_arCode.GetSize(); ii++)
	{
		struct _code code = m_arCode.GetAt(ii);
		data.Format("%s|%s", code.code, code.name);
		key.Format("%02d", ii);
		WritePrivateProfileString(section, key, data, path);
	}	
}

void CCodeReg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBDown = true;
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CCodeReg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bLBDown)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}
	
	CRect cRc, rc;
	GetClientRect(&rc);

	const int cx = 0, cy = rc.top + CTRL_HEIGHT;

	cRc.SetRect(cx, cy, rc.right, cy + m_iCellHeight);
	for (int ii = 0; ii < m_iRow; ii++)
	{
		if (cRc.PtInRect(point))
		{
			if (ii < m_arCode.GetSize())
			{
				struct _code code = m_arCode.GetAt(ii);
				//m_pMapWnd->SetCode(code.code);
				SetCode(code.code);
			}
			break;
		}

		cRc.OffsetRect(0, m_iCellHeight - 1);
	}
	m_bLBDown = false;
	
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CCodeReg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CCodeReg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect cRc, rc;
	GetClientRect(&rc);

	const int bkMode = dc.SetBkMode(TRANSPARENT);

	dc.FillSolidRect(rc, m_clrBG);			
	CPen *pOldPen = dc.SelectObject(m_ppenLine);
	dc.SelectStockObject(NULL_BRUSH);

	const int cx = 0;
	const int cy = m_iCellTop;

	cRc.SetRect(cx, cy, rc.right, cy + m_iCellHeight);
	for (int ii = 0; ii < m_iRow; ii++)
	{
		if (ii == m_iRow)
			cRc.bottom = rc.bottom;

		dc.FillSolidRect(&cRc, m_clrDT);
		dc.Rectangle(&cRc);

		if (ii < m_arCode.GetSize())
		{
			struct _code code = m_arCode.GetAt(ii);
			CString name = code.name;
			name.Replace("&", "&&");
			DrawSiseText(&dc, name, cRc, DT_CENTER);
		}
		cRc.OffsetRect(0, m_iCellHeight - 1);
	}

	dc.SelectObject(pOldPen);
	dc.SetBkMode(bkMode);
}

void CCodeReg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (m_bPopMenu)
	{
		CRect rc;
		GetClientRect(&rc);
		rc.top = CTRL_HEIGHT;
		
		m_ptPopMenu= point;
		ScreenToClient(&m_ptPopMenu);

		if (rc.PtInRect(m_ptPopMenu))
		{
			CMenu muTemp, *pContextMenu;
			
			muTemp.LoadMenu(IDR_MENUPOPUP);
			pContextMenu = muTemp.GetSubMenu(0);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
	}
}

void CCodeReg::OnMenuDelete() 
{
	CRect cRc, rc;
	GetClientRect(&rc);

	const int cx = 0, cy = rc.top + CTRL_HEIGHT;
	cRc.SetRect(cx, cy, rc.right, cy + m_iCellHeight);

	for (int ii = 0; ii < m_iRow; ii++)
	{
		if (cRc.PtInRect(m_ptPopMenu))
		{
			if (ii < m_arCode.GetSize())
			{
				m_arCode.RemoveAt(ii);
				Invalidate();
			}
			break;
		}

		cRc.OffsetRect(0, m_iCellHeight - 1);
	}

	SaveCode();
	Invalidate();
}

void CCodeReg::OnMenuDeleteall() 
{
	m_arCode.RemoveAll();
	Invalidate();
	SaveCode();
	Invalidate();
}

void CCodeReg::OnDestroy() 
{
	CWnd::OnDestroy();
	
//	SaveCode();
//	m_arCode.RemoveAll();
	// TODO: Add your message handler code here
}

void CCodeReg::SetCode(CString strCode)
{
	GetParent()->GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_SET_CODE, NULL), (LPARAM)strCode.operator LPCTSTR());
}

void CCodeReg::Palette()
{
	m_clrBG = m_pTool->GetIndexColor(CONTENTS_BACK_INDEX);
	m_clrDT	= m_pTool->GetIndexColor(COLOR_DATA);

	m_ppenLine = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);

	Invalidate();
}


void CCodeReg::ChangeFont(double* lRate, CFont* pfont, CFont* boldFont)
{
	m_pfont = pfont;

	
	CRect wrc, rc;
	GetClientRect(&rc);
	m_iRow = (rc.Height() - CTRL_HEIGHT) / CELL_HEIGHT + 1;
//	CString ss;
//	ss.Format("%d",  m_iRow);
//AfxMessageBox(ss);
	int iw{}, ih{}, iright{};
	//int ileft = rc.left + GAP;
	const int itop = rc.top + GAP;
	

	m_pBtnHead->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);

	iright = rc.left+iw-GAP;
	wrc.SetRect(rc.left, itop, iright, itop+ih);
	m_pBtnHead->SetFont(m_pfont);
	m_pBtnHead->MoveWindow(wrc); 


	m_iCellTop = wrc.bottom + GAP;
	m_iCellHeight = (int)(m_iCellHeight + (lRate[1] * (float)m_iCellHeight));
	m_iRow = (rc.Height() - m_iCellHeight) / m_iCellHeight + 1;
	
}