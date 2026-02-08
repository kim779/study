// CodeReg.cpp : implementation file
//

#include "stdafx.h"
#include "IB300200.h"
#include "CodeReg.h"
#include "../../h/memDC.h"

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

CCodeReg::CCodeReg(CWnd* pParent, CMapWnd* pMapWnd, CWnd* pView)
{
	m_pParent = pParent;
	m_pMapWnd = pMapWnd;
	m_pView = pView;

	m_row = 0;
	m_bLBDown = false;
	m_bPopMenu = false;
}

CCodeReg::~CCodeReg()
{
	SaveCode();
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCodeReg message handlers

COLORREF CCodeReg::GetIndexColor(int index)
{
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CFont* CCodeReg::GetAxFont(CString fName, int point, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name   = (LPSTR)fName.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = bItalic;
	fontR.bold   = nBold;
	return (CFont*)m_pView->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* CCodeReg::GetAxBitmap(CString path)
{
	return (CBitmap*)m_pView->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

CPen* CCodeReg::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pView->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* CCodeReg::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pView->SendMessage(WM_USER, getBRUSH, (long)clr);
}


CString CCodeReg::SetComma(CString src)
{
	CString temp, floatStr;
	int	ii, jj = -1;

	src.TrimLeft();
	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (ii = temp.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}

void CCodeReg::DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat, UINT nType)
{
	rc.DeflateRect(5, 0);
	nFormat = DT_SINGLELINE | DT_VCENTER | nFormat;
	COLORREF color, clrText = GetIndexColor(COLOR_TEXT);
	int	pos = -1;
	CString value, giho = "", percent = "";

	if (data.IsEmpty())	return;

	CFont *pOldFont;
	if (nType & ST_BOLD)
		pOldFont = pDC->SelectObject(GetAxFont("굴림", 9, false, FW_BOLD));
	else
		pOldFont = pDC->SelectObject(GetAxFont("굴림", 9));

	if (nType & ST_TEXT)
	{
		pDC->DrawText(data, rc, nFormat);
		return;
	}

	if (nType & ST_SHGA)
	{
		if (data[0] == '1')
		{
			giho = "↑ ";
			clrText = GetIndexColor(94);
		}
		else if (data[0] == '2')
		{
			giho = "▲ ";
			clrText = GetIndexColor(94);
		}
		else if (data[0] == '4')
		{
			giho = "↓ ";
			clrText = GetIndexColor(95);
		}
		else if (data[0] == '5')
		{
			giho = "▼ ";	
			clrText = GetIndexColor(95);
		}

		data = data.Mid(1);
	}

	if (nType & ST_COLOR)
	{
		pos = data.Find('+');
		if (pos >= 0)
			clrText = GetIndexColor(94);

		pos = data.Find('-');
		if (pos >= 0)
			clrText = GetIndexColor(95);
	}

	if (nType & ST_PERCENT)
		percent = " %";

	if (nType & ST_REMOVE)
	{
		data.Remove('+');
		data.Remove('-');
	}

	if (nType & ST_NORMAL)
	{
		CString pre = _T("");
		pos = data.Find('+');
		if (pos >= 0)
		{
			pre = "+";
			data.Remove('+');
		}

		pos = data.Find('-');
		if (pos >= 0)
		{
			pre = "-";
			data.Remove('-');
		}

		data.Remove(' ');
		pos = data.Find('.');
		if (pos >= 0)
			value.Format("%.2f", atof(data));
		else
			value.Format("%d", atoi(data));

		if (atof(data) == 0.0)
			data = "";
		else
			data = pre + value;
	}

	if (nType & ST_COMMA)
	{
		data = SetComma(data);	
	}

	color = pDC->SetTextColor(clrText);
	pDC->DrawText(giho + data + percent, rc, nFormat);

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(color);
}

CString CCodeReg::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CCodeReg::Init(CString home, CString user, bool bMenu)
{
	m_home = home;
	m_user = user;
	m_bPopMenu = bMenu;

	CRect rc, cRc;
	GetClientRect(&rc);

	m_row = (rc.Height() - CTRL_HEIGHT) / (double)CELL_HEIGHT;

	int cx = 0, cy = 0;

	cRc.SetRect(cx, cy, rc.right, cy + CTRL_HEIGHT);
	m_pBtnHead = new CfxImgButton();
	m_pBtnHead->Create("\n종목 등록", cRc, this, ID_CTRL_HEAD);
	m_pBtnHead->SetWindowText("종목 등록");
	m_pBtnHead->SetFont(GetAxFont());

	LoadCode();
}

void CCodeReg::setButtonBitmaps()
{
	CString path = "";
	path.Format("%s\\%s\\", m_home, IMAGEDIR);

	m_hBitmap    = GetAxBitmap(path + "7btn.bmp")->operator HBITMAP();
	m_hBitmap_dn = GetAxBitmap(path + "7btn_dn.bmp")->operator HBITMAP();
	m_hBitmap_hv = GetAxBitmap(path + "7btn_en.bmp")->operator HBITMAP();
	m_pBtnHead->SetImgBitmap(m_hBitmap, m_hBitmap_dn, m_hBitmap_hv);
}

LRESULT CCodeReg::OnButtonHead()
{
	CRect rect;
	GetDlgItem(ID_CTRL_HEAD)->GetWindowRect(rect);
	CString strCODE = (char*) m_pView->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(kkCODE, NULL)), MAKELPARAM(rect.left, rect.bottom + 2));
	struct _code code;

	int pos = strCODE.Find('\t');
	if (pos > 0)
	{
		code.code = strCODE.Left(pos);
		code.code.TrimLeft();	code.code.TrimRight();
		code.name = strCODE.Mid(pos + 1);
		code.name.TrimLeft();	code.name.TrimRight();

		m_arCode.Add(code);

		Invalidate();
	}

	return 0;
}

void CCodeReg::LoadCode()
{
	m_arCode.RemoveAll();

	CString key, section = "CODEREG", data = "", path = "";
	path.Format("%s\\%s\\%s\\IB300200.ini", m_home, USRDIR, m_user);	
	char	ssb[1024];

	for (int ii = 0; ii < 100; ii++)
	{
		key.Format("%02d", ii);
		GetPrivateProfileString(section, key, "", ssb, sizeof(ssb), path);
		data = ssb;
		
		int pos = data.Find("|");
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
	path.Format("%s\\%s\\%s\\IB300200.ini", m_home, USRDIR, m_user);	

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

	int cx = 0, cy = rc.top + CTRL_HEIGHT;

	cRc.SetRect(cx, cy, rc.right, cy + CELL_HEIGHT);
	for (int ii = 0; ii < m_row; ii++)
	{
		if (cRc.PtInRect(point))
		{
			if (ii < m_arCode.GetSize())
			{
				struct _code code = m_arCode.GetAt(ii);
				m_pMapWnd->SetCode(code.code);
			}
			break;
		}

		cRc.OffsetRect(0, CELL_HEIGHT - 1);
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

	int bkMode = dc.SetBkMode(TRANSPARENT);

	dc.FillSolidRect(rc, GetIndexColor(COLOR_BK));			

	CPen *pPen = GetAxPen(GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	CPen *pOldPen = dc.SelectObject(pPen);
	dc.SelectStockObject(NULL_BRUSH);

	int cx = 0, cy = rc.top + CTRL_HEIGHT;

	cRc.SetRect(cx, cy, rc.right, cy + CELL_HEIGHT);
	for (int ii = 0; ii < m_row; ii++)
	{
		if (ii == (int)m_row)
			cRc.bottom = rc.bottom;

		dc.FillSolidRect(&cRc, GetIndexColor(COLOR_DATA));	dc.Rectangle(&cRc);

		if (ii < m_arCode.GetSize())
		{
			struct _code code = m_arCode.GetAt(ii);
			DrawSiseText(&dc, code.name, cRc, DT_CENTER);
		}
		cRc.OffsetRect(0, CELL_HEIGHT - 1);
	}

	dc.SelectObject(pOldPen);
	dc.SetBkMode(bkMode);
}

void CCodeReg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (m_bPopMenu)
	{
		CRect cRc, rc;
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

	int cx = 0, cy = rc.top + CTRL_HEIGHT;

	cRc.SetRect(cx, cy, rc.right, cy + CELL_HEIGHT);
	for (int ii = 0; ii < m_row; ii++)
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

		cRc.OffsetRect(0, CELL_HEIGHT - 1);
	}
}

void CCodeReg::OnMenuDeleteall() 
{
	m_arCode.RemoveAll();

	Invalidate();
}
