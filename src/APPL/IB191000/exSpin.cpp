#include "stdafx.h"
#include "exSpin.h"
#include <math.h>
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IDC_INPLACE_EDIT	1001
#define	IDC_INPLACE_SPIN	1002


CExSpin::CExSpin()
{
	m_spType = spNOT;
	m_attr = 0;
	m_pEdit = NULL;
	m_pSpin = NULL;
	m_font.DeleteObject();
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight         = 10 * 10;
	lf.lfWidth          = 0;
	lf.lfEscapement     = 0;
	lf.lfOrientation    = 0;
	lf.lfWeight         = FW_NORMAL;
	lf.lfItalic         = false;
	lf.lfUnderline      = false;
	lf.lfStrikeOut      = false;
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf.lfQuality        = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
	strcpy(lf.lfFaceName, "±¼¸²Ã¼");
	m_font.CreatePointFontIndirect(&lf);
}

CExSpin::~CExSpin()
{
	if (m_pEdit)
	{
		delete m_pEdit; m_pEdit = NULL;
	}
	if (m_pSpin)
	{
		delete m_pSpin; m_pSpin = NULL;
	}
}


BEGIN_MESSAGE_MAP(CExSpin, CStatic)
	//{{AFX_MSG_MAP(CExSpin)
	ON_WM_ENABLE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(SPNM_ONECHANGE, IDC_INPLACE_EDIT, OnEditChange)
	ON_NOTIFY(SPNM_ONSCHANGE, IDC_INPLACE_SPIN, OnSpinChange)
END_MESSAGE_MAP()


BOOL CExSpin::Create(DWORD attr, DWORD spType, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	m_spType = spType;
	m_attr = attr;
	m_parent = pParentWnd;
	DWORD style = WS_CHILD|WS_VISIBLE|WS_TABSTOP;
	CString lpszText = _T("");
	if (CStatic::Create(lpszText, style, rect, pParentWnd, nID) < 0)
		return FALSE;

	createControls();
	return TRUE;
}


void CExSpin::Init(DWORD attr, DWORD spType, CWnd* pParentWnd)
{
	m_spType = spType;
	m_attr = attr;
	m_parent = pParentWnd;
	createControls();
}

void CExSpin::SetAttribute(DWORD attr)
{
	if (attr & attrCOMMA)
		attr &= ~attrNUMERIC;
	m_attr = attr;
	if (m_pSpin)	m_pSpin->SetAttribute(m_attr);
	if (m_pEdit)	m_pEdit->SetAtrribute(m_attr);
}

void CExSpin::SetSpinType(DWORD remove, DWORD add)
{
	m_spType &= ~remove;
	m_spType |= add;

	
	if (!m_pSpin)	return;
	if (m_spType & (spFUTURE | spOPTION))
	{
		m_attr |= attrFLOAT;
		m_attr &= ~attrCOMMA;
		m_attr &= ~attrNUMERIC;
		SetDecimalPlaces(2);
		m_pSpin->SetAttribute(m_attr);
		m_pEdit->SetAtrribute(m_attr);
	}
	else if (m_spType & (spKOSPI | spSOPT | spKOSDAQ | spETFKOSPI | spETFKOSDAQ))
	{
		m_attr |= attrCOMMA;
		m_attr &= ~attrFLOAT;
		m_attr &= ~attrNUMERIC;
		SetDecimalPlaces(0);
		m_pSpin->SetAttribute(m_attr);
		m_pEdit->SetAtrribute(m_attr);
	}
	
	m_pSpin->SetSpinType(m_spType);
}

void CExSpin::SetColors(COLORREF crBack, COLORREF crFont, bool redraw)
{
	if (m_pEdit)
	{
		m_pEdit->SetColors(crBack, crFont, redraw);
	}
}

void CExSpin::SetDecimalPlaces(int digit, bool redraw)
{
	if (m_pSpin)	m_pSpin->SetDecimalPlaces(digit);
	if (m_pEdit)	m_pEdit->SetDecimalPlaces(digit);
	if (redraw)
		Invalidate();
}

void CExSpin::OnEnable(BOOL bEnable) 
{
	if(m_pEdit)
		m_pEdit->EnableWindow(bEnable);
	if(m_pSpin)
		m_pSpin->EnableWindow(bEnable);
	this->InvalidateRect(NULL);
}

void CExSpin::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_pEdit)
	{
		m_pEdit->SetFocus();
		m_pEdit->SetSel(0, -1);
	}
}

void CExSpin::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	if (cx == 0 && cy == 0)		return;
	if (!IsWindow(GetSafeHwnd()))	return;
	int swid = GetSystemMetrics(SM_CYHSCROLL);
	int iWidth = cx - swid;
	if (m_pEdit) m_pEdit->MoveWindow(0, 0, iWidth, cy);
	if (m_pSpin) m_pSpin->MoveWindow(iWidth, 0, cx-iWidth, cy);	
	
}

void CExSpin::OnEditChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!IsWindow(GetSafeHwnd()))
		return;
	if (pNMHDR->code == SPNM_ONECHANGE)
	{
		pNMHDR->code = SPNM_ONCHANGE;
		CWnd* pOwner = GetOwner();
		if (pOwner) pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)pNMHDR);
	}
}

void CExSpin::OnSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!IsWindow(GetSafeHwnd()))
		return;
	if (pNMHDR->code == SPNM_ONSCHANGE)
	{
		pNMHDR->code = SPNM_ONCHANGE;
		CWnd* pOwner = GetOwner();
		if (pOwner) pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)pNMHDR);
	}
}


void CExSpin::createControls()
{
	CRect rect(0,0,0,0), erect(0,0,0,0), srect(0,0,0,0);
	GetClientRect(rect);
	int nCtrlID = this->GetDlgCtrlID();
	erect = srect = rect;



	int swid = GetSystemMetrics(SM_CYHSCROLL);
	erect.right = rect.right - swid;
	srect.left = rect.right - swid;
	if (!m_pEdit) 
	{
		m_pEdit = new CSpinEdit(this, m_attr);
		m_pEdit->CreateEx(WS_EX_STATICEDGE ,_T("EDIT"), "",	// WS_EX_WINDOWEDGE, WS_EX_STATICEDGE
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL |ES_RIGHT ,  //| WS_BORDER
			erect, this, IDC_INPLACE_EDIT); 
		m_pEdit->SetFont(&m_font);
	}
	else
		return;

	if (!m_pSpin) 
	{
		m_pSpin = new CSpinCtrl(this, m_attr, m_spType);
		m_pSpin->Create(WS_CHILD|WS_VISIBLE|UDS_AUTOBUDDY, srect, this, IDC_INPLACE_SPIN);
	}
	else 
		return;	
	
//	if (m_spType & spCOLOR)
		m_pEdit->SetUseColor(true);
	
	SetAttribute(m_attr);
	SetSpinType(0, m_spType);

	m_pSpin->SetBuddy(m_pEdit);	
	m_pEdit->SetFont(&m_font);
	m_pSpin->SetRange32((int)INT_MIN, (int)INT_MAX);
	m_pSpin->SetPos(0);
}

void CExSpin::SetPos(double nPos)
{
	if (m_pSpin)
		m_pSpin->SetPos(nPos);	
}

double CExSpin::GetPos()
{
	if (!m_pSpin)
		return 0.0;
	return m_pSpin->GetPos();
}

void CExSpin::SetDirection(BOOL nFlag)
{
	if (nFlag)
		m_pSpin->SetDirection(1);
	else
		m_pSpin->SetDirection(-1);
}

void CExSpin::SetRangeEx(double lower, double upper)
{
	if (!m_pSpin || !m_pEdit)
		return;
	if (lower == UNUSEDVAL)
		lower = 0;
	if (upper ==UNUSEDVAL)
		upper = INT_MAX;
	m_pSpin->SetRange32((int)lower, (int)upper);
	m_pEdit->SetRange(lower, upper);

}

void CExSpin::GetRangeEx(double &lower, double &upper)
{
	if (!m_pSpin)	return;
	int low, upp;
	m_pSpin->GetRange32(low, upp);
	lower = (double)low;
	upper = (double)upp;
}

void CExSpin::SetDelta(double delta, bool redraw)
{
	if (!m_pSpin)	return;
	m_pSpin->SetDelta(delta);
	if (redraw)
		Invalidate();
}

double CExSpin::GetDelta()
{
	if (!m_pSpin)	return 0.0;
	return m_pSpin->GetDelta();
}

void CExSpin::SetText(CString text)
{
	if (m_pEdit)
		m_pEdit->SetText(text);
}

CString CExSpin::GetText()
{
	CString text = _T("");
	if (m_pEdit)
		text = m_pEdit->GetText();
	return text;
}

void CExSpin::SetLimitText(int len)
{
	if (m_pEdit)	m_pEdit->SetLimitText(len);
}

/////////////////////////////////////////////////////////////////////
// Spin Button
CSpinCtrl::CSpinCtrl(CWnd* pWnd, DWORD attr, DWORD spType)
{
	m_pWnd = pWnd;
	m_spType = spType;
	m_attr = attr;
	m_direction = 1;	
	m_digit = 0;
	m_delta = 1;
}

CSpinCtrl::~CSpinCtrl()
{
}

BEGIN_MESSAGE_MAP(CSpinCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CSpinCtrl)
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CSpinCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSpinButtonCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetAttribute(m_attr);
	
	return 0;
}

void CSpinCtrl::SetSpinType(DWORD spType, bool redraw)
{
	m_spType = spType;
	if (redraw)
		Invalidate();
}

void CSpinCtrl::SetAttribute(DWORD attr, bool redraw)
{
	m_attr = attr;

	if (m_attr & attrCOMMA)
		ModifyStyle(0, UDS_NOTHOUSANDS);
	else
		ModifyStyle(UDS_NOTHOUSANDS, 0);

	if (m_attr & attrFLOAT)
	{
		ModifyStyle(UDS_NOTHOUSANDS, 0);
	}

	if (redraw)
		Invalidate();
}

void CSpinCtrl::SetDirection(int direction)
{
	m_direction = direction;
}

void CSpinCtrl::SetDecimalPlaces(int digit)
{
	m_digit = digit;
}

void CSpinCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	int lower, upper;
	GetRange32(lower, upper);
	CWnd *pWnd = GetBuddy();
	double val = 0;
	double pos = GetPos();
	if (pNMUpDown->iDelta < 0) 
	{
		checkDelta(FALSE);	//down
		val = pos - (m_delta * m_direction);
		if (val > upper || val < lower)
			return;
	}
	else 
	{
		checkDelta(TRUE);	//up
		val = pos + (m_delta * m_direction);		
		if (val > upper || val < lower)
			return;
	}
	SetPos(val);	

	NMHDR nmhdr;
	nmhdr.hwndFrom = GetSafeHwnd();
	nmhdr.idFrom   = GetDlgCtrlID();
	nmhdr.code     = SPNM_ONSCHANGE;
	if (m_pWnd) m_pWnd->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);	
	*pResult = 0;
}

void CSpinCtrl::SetPos(double nPos)
{
	CWnd* pWnd = GetBuddy();
	if (!pWnd)	return;
	CString tmpS;
	tmpS.Format("%.*f", m_digit, nPos);
	if (m_attr & attrCOMMA)
		formatComma(tmpS);
	
	pWnd->SetWindowText(tmpS);
}

double CSpinCtrl::GetPos()
{
	CWnd* pWnd = GetBuddy();
	if (!pWnd)	return 0.0;
	
	CString text;
	pWnd->GetWindowText(text);
	text.Remove(',');
	return atof(text);
}

void CSpinCtrl::formatComma(CString& text)
{
	
	if (m_attr & attrCOMMA && !text.IsEmpty())
	{
		bool bMinus = false;
		if (atof(text) < 0)
		{
			bMinus = true;
			text = text.Mid(1);
		}
		CString tmpS = _T("");
		text.Remove(',');
		text.MakeReverse();
		for (int ii = 0; ii < text.GetLength(); ii++)
		{
			if (ii && !(ii % 3))
				tmpS += ',';
			tmpS += text[ii];
		}
		tmpS.MakeReverse();
		text = tmpS;

		if (bMinus)
			text = "-" + text;
	}
	
}


void CSpinCtrl::checkDelta(BOOL nFlag)
{
	CWnd *pWnd = GetBuddy();
	if (!pWnd)	return;

	CString tmpS;
	pWnd->GetWindowText(tmpS);
	tmpS.Remove(',');
	double	spVal = atof(tmpS);
	if (m_spType & spKOSPI)
	{
		if (spVal < 5000)
			m_delta = 5;
		else if (spVal >= 5000 && spVal < 10000)
			m_delta = 10;
		else if (spVal >= 10000 && spVal < 50000)
			m_delta = 50;
		else if (spVal >= 50000 && spVal < 100000)
			m_delta = 100;
		else if (spVal >= 100000 && spVal < 500000)
			m_delta = 500;
		else
			m_delta = 1000;
	}
	else if (m_spType & spKOSDAQ)
	{
		if (spVal < 5000)
			m_delta = 5;
		else if (spVal >= 5000 && spVal < 10000)
			m_delta = 10;
		else if (spVal >= 10000 && spVal < 50000)
			m_delta = 50;
		else
			m_delta = 100;
	}
	else if (m_spType & spETFKOSPI)
	{
		if (spVal < 5000)
			m_delta = 5;
		else if (spVal >= 5000 && spVal < 50000)
			m_delta = 10;
		else if (spVal >= 50000 && spVal < 100000)
			m_delta = 100;
		else if (spVal >= 100000 && spVal < 500000)
			m_delta = 500;
		else
			m_delta = 1000;
	}
	else if (m_spType & spETFKOSDAQ)
	{
		if (spVal < 5000)
			m_delta = 5;
		else if (spVal >= 5000 && spVal < 10000)
			m_delta = 10;
		else if (spVal >= 10000 && spVal < 50000)
			m_delta = 50;
		else
			m_delta = 100;
	}
	else if (m_spType & spFUTURE)
	{
		m_delta = 0.05;
	}
	else if (m_spType & spOPTION)
	{
		if(nFlag) 
		{
			if (spVal < 3)
				m_delta = 0.01;		
			else
				m_delta = 0.05;
		} 
		else 
		{
			if (spVal <= 3)
				m_delta = 0.01;		
			else
				m_delta = 0.05;
		}
	}
	else if (m_spType & spSOPT)
	{
		if (spVal < 1000)
			m_delta = 10;
		else if (spVal >= 1000 && spVal < 2000)
			m_delta = 20;
		else if (spVal >= 2000 && spVal < 5000)
			m_delta = 50;
		else if (spVal >= 5000 && spVal < 10000)
			m_delta = 100;
		else
			m_delta = 200;
	}
	else if (m_spType & spRATE)
	{
		m_delta = 0.01;	
	}
}

double CSpinCtrl::GetDelta()
{
	return m_delta;
}

void CSpinCtrl::SetDelta(double delta)
{
	m_delta = delta;
}

/////////////////////////////////////////////////////////////////////
// Spin Edit
CSpinEdit::CSpinEdit(CWnd* pWnd, DWORD attr)
{
	m_bColor = false;
	m_pWnd = pWnd;
	m_attr = attr;
	m_clrBACK   = RGB(255, 255, 255);
	m_clrFORE   = RGB(  0,   0,   0);
	m_brBACK.CreateSolidBrush(m_clrBACK);
	m_lower = INT_MIN;
	m_upper = INT_MAX;
	m_digit = 0;
	m_bFocus = false;
}

CSpinEdit::~CSpinEdit()
{
	m_brBACK.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSpinEdit, CEdit)
	//{{AFX_MSG_MAP(CSpinEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


HBRUSH CSpinEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if (m_bColor)
	{
		pDC->SetTextColor(m_clrFORE);
		pDC->SetBkColor(m_clrBACK);
		return m_brBACK;
	}
	return NULL;
}


void CSpinEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	bool isHan = false, bDot = false, bMinus = false;
	CString tmpS = _T("");
	
	int jj = 0;
	int sel = -1;

	CString text;
	GetWindowText(text);
	char* tmp = (LPSTR)text.operator LPCTSTR();

	switch (m_attr)
	{
	case attrCOMMA:
		for (jj = 0; jj < text.GetLength(); jj++)
		{
			if (tmp[jj] & 0x80)
			{
				if (sel == -1)
					sel = jj;
				isHan = true;
			}
			else if (tmp[jj] == '-')
			{
				sel = jj;
				bMinus = true;
			}
			else
			{
				tmpS += text.GetAt(jj);
			}
		}
		if ((nChar >= 0x30 && nChar <= 0x39) || nChar == VK_BACK)
		;
		else if (nChar == '-')
		{
			if (bMinus)
				return;
		}
		else if (isHan)
		{
			text = tmpS;
			SetWindowText(text);
			SetSel(sel, sel);
			return;
		}
		else
			return;	
		break;
	case attrFLOAT:
	case attrNUMERIC:
		for (jj = 0; jj < text.GetLength(); jj++)
		{
			if (tmp[jj] & 0x80)
			{
				if (sel == -1)
					sel = jj;
				isHan = true;
			}
			else if (tmp[jj] == '-')
			{
				sel = jj;
				bMinus = true;
			}
			else if (tmp[jj] == '.')
			{
				tmpS += text.GetAt(jj);
			 	bDot = true;
			}
			else
			{
				tmpS += text.GetAt(jj);
			}
		}

		if (bDot && nChar == '.')
			return;
		else if ((nChar >= 0x30 && nChar <= 0x39) 
			|| nChar == VK_BACK 
			|| (!bDot && nChar == '.'))
		;
		else if (nChar == '-')
		{
			if (bMinus)	return;
		}
		else if (isHan)
		{
			text = tmpS;
			SetWindowText(text);
			SetSel(sel, sel);
			return;
		}
		else
			return;	
		break;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
	CString realS;
	GetWindowText(realS);
	
	int pos = realS.Find('-');
	if (pos > 0)
	{
		realS.Remove('-');
		SetWindowText(realS);
		SetSel(realS.GetLength(), realS.GetLength());
	}
		
	if (m_attr & attrCOMMA)
		commaText();
	
	tmpS = GetText();
	
	double val = atof(tmpS);
	if (val < m_lower)
	{
		tmpS.Format("%.f", m_lower);
		SetText(tmpS);
		SetSel(realS.GetLength(), realS.GetLength());
	}
	else if (val > m_upper)
	{
		tmpS.Format("%.f", m_upper);
		SetText(tmpS);
		SetSel(realS.GetLength(), realS.GetLength());
	}
	NMHDR nmhdr;
	nmhdr.hwndFrom = GetSafeHwnd();
	nmhdr.idFrom   = GetDlgCtrlID();
	nmhdr.code     = SPNM_ONECHANGE;
	if (m_pWnd) m_pWnd->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);
}

void CSpinEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	if (m_attr & attrCOMMA)
	{
		switch (nChar)
		{
		case VK_DELETE:
			{
				commaText();
				NMHDR nmhdr;
				nmhdr.hwndFrom = GetSafeHwnd();
				nmhdr.idFrom   = GetDlgCtrlID();
				nmhdr.code     = SPNM_ONECHANGE;
				if (m_pWnd) m_pWnd->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmhdr);
			}
			break;
		}
	}
}



void CSpinEdit::SetUseColor(bool bColor)
{
	m_bColor = bColor;
}

void CSpinEdit::SetColors(COLORREF clrBACK, COLORREF clrFORE, bool redraw)
{
	m_clrFORE = clrFORE;
	m_clrBACK = clrBACK;
	m_brBACK.DeleteObject();
	m_brBACK.CreateSolidBrush(m_clrBACK);

	if (redraw)
		Invalidate();
}

void CSpinEdit::SetRange(double lower, double upper)
{
	m_upper = upper;
	m_lower = lower;
}

void CSpinEdit::SetText(CString text, bool redraw)
{
	if (m_attr & attrCOMMA)
		formatComma(text);

	SetWindowText(text);
	if (redraw)
		Invalidate();
}

CString CSpinEdit::GetText()
{
	CString text;
	GetWindowText(text);
	if (m_attr & attrCOMMA)
		text.Remove(',');

	if (m_digit > 0)
	{
		double val = atof(text);
		text.Format("%.*f", m_digit, val);
	}
	return text;
}

void CSpinEdit::SetAtrribute(DWORD attr, bool redraw)
{
	m_attr = attr;
	if (redraw)
		Invalidate();
}

void CSpinEdit::formatComma(CString& text)
{
	if (m_attr & attrCOMMA && !text.IsEmpty())
	{
		bool bSign = false;
		CString tmpS = _T("");
		int pos = text.Find('-');
		if (pos >= 0)
		{
			bSign = true;
			text.Remove('-');
		}
		text.Remove(',');
		text.MakeReverse();

		for (int ii = 0; ii < text.GetLength(); ii++)
		{
			if (ii && !(ii % 3))
				tmpS += ',';
			tmpS += text[ii];
		}
		tmpS.MakeReverse();
		text = tmpS;
		if (bSign)
			text = "-" + text;
	}
}

void CSpinEdit::commaText()
{
	CString tmpS = GetText();
	SetText(tmpS);
	GetWindowText(tmpS);
	SetSel(tmpS.GetLength(), tmpS.GetLength());
}

void CSpinEdit::OnChange() 
{
	return;
	CString tmpS;
	tmpS = GetText();
	double val = atof(tmpS);
	if (val < m_lower)
	{
		tmpS.Format("%.f", m_lower);
		SetText(tmpS);
	}
	else if (val > m_upper)
	{
		tmpS.Format("%.f", m_upper);
		SetText(tmpS);
	}
}
void CSpinEdit::SetDecimalPlaces(int digit)
{
	m_digit = digit;
}

void CSpinEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CEdit::OnLButtonDown(nFlags, point);
	
}

void CSpinEdit::OnKillFocus(CWnd* pNewWnd) 
{
	if (m_bFocus)
		m_bFocus = false;
	CEdit::OnKillFocus(pNewWnd);
}

void CSpinEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonUp(nFlags, point);
	if (!m_bFocus)
	{
		SetSel(0, -1);
		m_bFocus = true;
	}
}
