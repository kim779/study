// PnWndCombo.cpp : implementation file
//

#include "stdafx.h"
#include "AxisGCom.h"
#include "PnWndCombo.h"

#include <imm.h>

#include "defInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPnWndSubEdit

CPnWndSubEdit::CPnWndSubEdit(CPnWndCombo *pCombo, CWnd* pParent, CWnd *pView, int type)
{
	m_Type = type;
	m_pCombo = pCombo;
	m_pParent = pParent;
	m_pView = pView;

	m_Unit = 0;
	m_bKeyChange = false;
}

CPnWndSubEdit::~CPnWndSubEdit()
{
}


BEGIN_MESSAGE_MAP(CPnWndSubEdit, CEdit)
	//{{AFX_MSG_MAP(CPnWndSubEdit)
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPnWndSubEdit message handlers

void CPnWndSubEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int	len;
	CString tmpS = _T("");
	GetWindowText(tmpS);

	switch (nChar)
	{
	case VK_RETURN:
		len = tmpS.GetLength();
		if (len <= 0)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			return;
		}

		switch (m_Type)
		{
		case CMB_CODE:
			break;
		case CMB_TIME:
			if (!tmpS.IsEmpty())
			{
				if (tmpS == _T("0.5"))
				{
					m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipGap), 930);
				}
				else if (atoi(tmpS) > 360)
				{
					SetWindowText("360");
					break;
				}
				else
				{
					m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipGap), atoi(tmpS));
				}
			}
			break;
		default:
			break;
		}
		break;
	case VK_TAB:
		m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipTab), m_Type);
		break;
	}
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CPnWndSubEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetSel(0, -1);	
	CEdit::OnLButtonDown(nFlags, point);
}

void CPnWndSubEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
//	SetSel(0, -1);
}

void CPnWndSubEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKeyChange = true;
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPnWndSubEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bKeyChange)
	{
		CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
		return;
	}
	m_bKeyChange = false;
	
	CString	tmpS;
	GetWindowText(tmpS);
	int	len = tmpS.GetLength();

	if (len <= 0) {
		CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
		return;
	}

	if (m_Type == CMB_TIME && len == 3)
	{
		if (tmpS != "0.5" && atoi(tmpS) > 360)
		{
			SetWindowText("360");
		}
	}

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// CPnWndCombo

CPnWndCombo::CPnWndCombo(CWnd* pParent, CWnd *pView, int type)
{
	m_Type = type;
	m_pParent = pParent;
	m_pView = pView;
	m_Unit = GU_CODE;

	m_pSubEdit = NULL;
}

CPnWndCombo::~CPnWndCombo()
{
}


BEGIN_MESSAGE_MAP(CPnWndCombo, CComboBox)
	//{{AFX_MSG_MAP(CPnWndCombo)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPnWndCombo message handlers

// 20090907 : CHANGE :
//struct _combo CUnit[4] = 
struct _combo CUnit[5] = 
{
	{GU_CODE, "주식"},
	{GU_INDEX, "업종"},
	{GU_FUTURE, "선옵"},
	{GU_FOREIGN, "해외"},
	{GU_CFUTURE, "상품"}	// 20090907 : ADD :
};


int CPnWndCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	switch (m_Type)
	{
	case CMB_UNIT:
		{
			// 20090907 : CHANGE :
			//SetData(CUnit, 4);
			SetData(CUnit, 5);
			SetCurSel(0);
		}
		break;
	case CMB_KKIND:
		{
			struct _combo CKind[9] = 
			{
				{upKospi,	"KOSPI"},
				{upKos200,	"KOSPI200"},
				{upKos100,	"KOSPI100"},
				{upKosdaq,	"KOSDAQ"},
				{upKq50,	"KOSDAQ50"},
				{upKoStar,	"KOSTAR"},	//** add macho@2005.11.03
				{upKrx100,	"KRX100"},	// Added by schbang 2006.02.20
				{upSector,	"섹터지수"},	// Added by schbang 2006.02.15
				{upFreeboard,"프리보드"}
			};

			SetData(CKind, 9);
			SetCurSel(0);
		}
		break;
	/*
	case CMB_SKIND:
		{
			struct _combo SKind[5] = 
			{
				{fnKind1, "다우 산업"},
				{fnKind2, "나스닥 종합"},
				{fnKind3, "S&P 500"},
				{fnKind4, "반도체"},
				{fnKind5, "나스닥선물"},
			};

			SetData(SKind, 5);
			SetCurSel(-1);
		}
		break;
	*/
	case CMB_TIME:
		CreateSubEdit();
		break;
	}

	return 0;
}

void CPnWndCombo::OnDestroy() 
{
	DeleteSubEdit();

	CComboBox::OnDestroy();
}

void CPnWndCombo::OnSelchange() 
{
	int index = GetCurSel();
	if (index < 0)
		return;

	CString	tmpS;
	switch (m_Type)
	{
	case CMB_UNIT:
		GetData(index);
		m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipUnit), index);
		break;
	case CMB_KKIND:
		GetData(index);
		m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipKKind), index);
		break;
	/*
	case CMB_SKIND:
		GetData(index);
		m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipSKind), index);
		break;
	*/
	case CMB_TIME:
		GetLBText(index, tmpS);
		if (tmpS == _T("0.5"))
			m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipGap), 930);
		else
			m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipGap), atoi(tmpS));
		break;
	default:
		break;
	}
}

void CPnWndCombo::SetData(struct _combo* data, int cnt)
{
	ResetContent();
	int idx;
	for (int ii = 0; ii < cnt; ii++)
	{
		idx = AddString(data[ii].name);
		SetItemData(idx, data[ii].val);
	}
}

int CPnWndCombo::SelectCombo(int data)
{
	int sel = -1;
	int ii, val;
	for (ii = 0; ii < GetCount()-1; ii++)
	{
		val = GetItemData(ii);
		if (val == data)
		{
			SetCurSel(ii);
			return ii;
		}
	}

	SetCurSel(ii);
	return sel;
}

void CPnWndCombo::GetData(int& val)
{
	val = -1;
	int sel = GetCurSel();
	if (sel < 0)
		return;
	val = GetItemData(sel);
}

CString CPnWndCombo::GetEditData()
{
	CString	sVal;
	GetWindowText(sVal);

	return sVal;
}

void CPnWndCombo::SetEditData(CString sData)
{
	m_pSubEdit->SetWindowText(sData);
	//m_pSubEdit->SetSel(-1,0);
}

void CPnWndCombo::SetUnit(int unit)
{	
	m_Unit = unit;
	if (m_Type == CMB_CODE && m_pSubEdit)
		m_pSubEdit->SetUnit(unit);
}

void CPnWndCombo::CreateSubEdit()
{
	m_pSubEdit = new CPnWndSubEdit(this, m_pParent, m_pView, m_Type);
	CWnd *pWnd = GetWindow(GW_CHILD);
	m_pSubEdit->SubclassWindow(pWnd->GetSafeHwnd());
	m_pSubEdit->SetUnit(m_Unit);
	SetFocus();
	m_pSubEdit->SetSel(-1, 0);
}

void CPnWndCombo::DeleteSubEdit()
{
	if (m_pSubEdit)
	{
		m_pSubEdit->UnsubclassWindow();
		delete m_pSubEdit;
	}
}

void CPnWndCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

	//m_pSubEdit->SetSel(-1, 0);
}

bool CPnWndCombo::IsFocusWnd()
{
	CWnd *pWnd = GetFocus();
	if (pWnd && m_pSubEdit)
	{
		if (pWnd == m_pSubEdit)
			return true;
	}

	return false;
}

void CPnWndCombo::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_TAB)
		m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipTab), m_Type);

	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

/*
void CPnWndCombo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	DrawCombo(&dc);
}

void CPnWndCombo::DrawCombo(CDC *pDC)
{
	COLORREF bg, fg;
	if (m_pParent != NULL)
	{
		bg = m_cbRGB;
		fg = m_csRGB;
	}
	else
	{
		bg = COLOR_PAL_BG;
		fg = COLOR_PAL_FG;
	}
 
	CRect WinRC, rcBtn, tRc;
	CPoint	pts[3];
	GetClientRect(&WinRC);
	pDC->FillSolidRect(WinRC, COLOR_WHITE);
	pDC->DrawEdge(&WinRC, EDGE_SUNKEN, BF_RECT);   
	
	rcBtn = WinRC;
	rcBtn.DeflateRect(0, 3, 2, 3);
	rcBtn.left = rcBtn.right - rcBtn.Height();
	tRc = rcBtn;
	tRc.DeflateRect(1, 0);
	
	CPen	cPen(PS_SOLID, 1, bg);
	CBrush	cBrush(bg);
	CPen	*sPen = pDC->SelectObject(&cPen);
	CBrush	*sBrush = pDC->SelectObject(&cBrush);
	pDC->FillRect(tRc, &cBrush);

	pDC->MoveTo(rcBtn.left, rcBtn.top+1);
	pDC->LineTo(rcBtn.left, rcBtn.bottom-1);
	pDC->MoveTo(rcBtn.right-1, rcBtn.top+1);
	pDC->LineTo(rcBtn.right-1, rcBtn.bottom-1);

	CPen	cPen1(PS_SOLID, 1, fg);
	CBrush	cBrush1(fg);
	pDC->SelectObject(&cPen1);
	pDC->SelectObject(&cBrush1);

	tRc = rcBtn;
	tRc.DeflateRect(3, tRc.Height()*2/5);
	tRc.OffsetRect(1, 0);
	tRc.right = tRc.left + (tRc.Width()/2)*2-1;
	tRc.bottom = tRc.top + tRc.Width()/2;
	pts[0].y = tRc.bottom;
	pts[1].y = tRc.top;
	pts[2].y = tRc.top;
	pts[0].x = tRc.left + (tRc.Width()/2);
	pts[1].x = tRc.right-1;
	pts[2].x = tRc.left;
	pDC->Polygon(pts, 3);

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}
*/