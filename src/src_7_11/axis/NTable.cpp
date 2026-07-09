// NTable.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "NTable.h"
#include "resource.h"

#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNTable

CNTable::CNTable(CString home, int viewN, bool hLine)
{
	m_home  = home;
	m_viewN = viewN;
	m_hLine = hLine;
	m_headW = 0;
	m_default = true;
	SetColor();
	CString	fontName;
	fontName.LoadString(ST_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);
	m_bManage = FALSE;
}

CNTable::~CNTable()
{
	m_font.DeleteObject();
	m_arTitle.RemoveAll();
	m_arData.RemoveAll();
}


BEGIN_MESSAGE_MAP(CNTable, CWnd)
	//{{AFX_MSG_MAP(CNTable)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNTable message handlers

void CNTable::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	DrawBase(&dc);
	DrawHText(&dc);
	DrawBText(&dc);
	DrawBorder(&dc);
}

void CNTable::DrawBase(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CRect	cRc, hRc, bRc;
	GetClientRect(cRc);

	hRc.SetRect(0, cRc.top, m_headW, cRc.bottom);
	pDC->FillSolidRect(hRc, m_hColor);

	bRc.SetRect(m_headW, cRc.top, cRc.right, cRc.bottom);
	pDC->FillSolidRect(bRc, m_cColor);

	CPen	Pen, *pen;
	Pen.CreatePen(PS_SOLID, 0, m_lColor);
	pen = dc.SelectObject(&Pen);

	dc.MoveTo(m_headW, 0);
	dc.LineTo(m_headW, cRc.bottom);

//	horizontal line
	for (int ii = 0; ii < m_viewN && m_hLine; ii++)
	{
		dc.MoveTo(0, CELLHEIGHT*(ii+1));
		dc.LineTo(cRc.right, CELLHEIGHT*(ii+1));
	}
	dc.SelectObject(pen);
	Pen.DeleteObject();
}

void CNTable::DrawHText(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	
	CRect	tRc;
	CString	str;
	CFont*	font = pDC->SelectObject(&m_font);
	pDC->SetBkMode(TRANSPARENT);
	for (int ii = 0; ii < m_arTitle.GetSize(); ii++)
	{
		tRc.SetRect(0, CELLHEIGHT*ii, m_headW, CELLHEIGHT*(ii+1));
		if (m_default)
			str = m_arTitle.GetAt(ii);
		else	str =m_arCT.GetAt(ii);

		pDC->DrawText(str, tRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	pDC->SelectObject(font);
}

void CNTable::DrawBText(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CRect	tRc, cRc;
	CFont*	font = pDC->SelectObject(&m_font);

	GetClientRect(cRc);
	pDC->SetBkMode(TRANSPARENT);
#if 1
	for (int ii = 0; ii < m_arData.GetSize(); ii++)
	{
		tRc.SetRect(m_headW, CELLHEIGHT*ii, cRc.right, CELLHEIGHT*(ii+1));
		switch (ii)
		{
		default:
		case 0:	// 계좌번호
		case 1:	// 종목명
			if (m_bManage)
			{
				int	nalign = 0;
					
				if (ii == 0)
				{
					nalign = DT_CENTER;
					pDC->FillSolidRect(tRc, m_hColor);
				}
				else
				{
					tRc.left += 6;
					nalign = DT_LEFT;
				}
				pDC->DrawText(m_arData.GetAt(ii) + "  ", tRc, nalign|DT_VCENTER|DT_SINGLELINE);
			}
			else
				pDC->DrawText(m_arData.GetAt(ii) + "  ", tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			break;
		case 2:	// 매매구분
			{
				CString text = m_arData.GetAt(ii);
				if (text.GetLength() && (text.GetAt(0) == '+' || text.GetAt(0) == '-'))
					text = text.Mid(1);
				pDC->DrawText(text + "  ", tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			}
			break;
		case 3:	// 체결수량 or 확인수량
		case 4:	// 체결단가 or 주문단가
			pDC->DrawText(commaE(m_arData.GetAt(ii))+"  ", tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			break;
		}
	}
#else
	for (int ii = 0; ii < m_arData.GetSize(); ii++)
	{
		tRc.SetRect(m_headW, CELLHEIGHT*ii, cRc.right, CELLHEIGHT*(ii+1));
		CString	tmps, text = m_arData.GetAt(ii);
		COLORREF color = pDC->SetTextColor(GetColor(69));
		switch (ii)
		{
		case 0:	// 계좌번호
			tmps = text;
			text.Format("%s-%s ", tmps.Left(3), tmps.Mid(3));
			break;
		default:
		case 1:	// 종목명
			text += " ";
			break;
		case 2:	// 매매구분
			if (text.GetLength() < 1)	break;

			switch (text.GetAt(0))
			{
			case 'D':	pDC->SetTextColor(GetColor(111));	break;
			case 'O':	pDC->SetTextColor(GetColor(112));	break;
			case 'C':	pDC->SetTextColor(GetColor(113));	break;
			case 'M':	pDC->SetTextColor(GetColor(114));	break;
			default:	break;
			}
			text = text.Mid(1) + " ";
			break;
		case 3:	// 수량
		case 4:	// 가격
			text.Format("%s ", commaE(text));
			break;
		}
		pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		pDC->SetTextColor(color);
	}
#endif
	pDC->SelectObject(font);
}

void CNTable::DrawBorder(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CRect	cRc;
	CPen	Pen, *pen;
	GetClientRect(cRc);

	Pen.CreatePen(PS_SOLID, 0, m_lColor);
	pen = pDC->SelectObject(&Pen);

	pDC->MoveTo(cRc.left, cRc.top);
	pDC->LineTo(cRc.right-1, cRc.top);
	pDC->LineTo(cRc.right-1, cRc.bottom-1);
	pDC->LineTo(cRc.left, cRc.bottom-1);
	pDC->LineTo(cRc.left, cRc.top);

	pDC->SelectObject(&pen);
	Pen.DeleteObject();
}

void CNTable::SetTitle(CStringArray& ary, char* body)
{
	int ii = 0;
	int	idx = 0, gap;
	CSize	size;
	CString	text;
	m_arTitle.RemoveAll();

	for (ii = 0; ii < ary.GetSize(); ii++)
		m_arTitle.Add(ary.GetAt(ii));
	//while (title[idx])	m_arTitle.Add(title[idx++]);

	CClientDC	dc(this);
	CFont*	font = dc.SelectObject(&m_font);
	gap = dc.GetTextExtent("    ").cx;
	for (ii = 0; ii < m_arTitle.GetSize(); ii++)
	{
		text = m_arTitle.GetAt(ii);
		size = dc.GetTextExtent(text);
		if (size.cx <= m_headW)	continue;
		m_headW = size.cx;
	}

	text = body;
	size = dc.GetTextExtent(text);
	dc.SelectObject(font);

	m_headW += gap;
	SetWindowPos(NULL, 0, 0, m_headW + size.cx, CELLHEIGHT*m_viewN+1, SWP_NOMOVE|SWP_SHOWWINDOW);

	m_arCT.Copy(m_arTitle);
}

void CNTable::SetWidth(bool head, int width)
{
	if (head)	m_headW = width;
	else
	{
		if (!IsWindow(m_hWnd))
			return;
		SetWindowPos(NULL, 0, 0, m_headW + width, 
			(CELLHEIGHT-1)*m_viewN, SWP_NOZORDER|SWP_SHOWWINDOW);
	}
}

// 통보제목\t계좌\t종목명\t매매구분\t체결수량\t체결가격
void CNTable::SetData(CString dat)
{
	CString	str;
	m_arData.RemoveAll();
	int pos = dat.Find('\t');
	if (pos != -1)		// 계좌번호
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
		m_arData.Add(str);
	}

	pos = dat.Find('\t');
	if (pos != -1)		// 종목명
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
		str.TrimRight();
		m_arData.Add(str);
	}

	pos = dat.Find('\t');
	if (pos != -1)		// 매매구분
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);

		m_arData.Add(str);
	}

	pos = dat.Find('\t');
	if (pos != -1)		// 체결가격
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
		m_arData.Add(str);
	}
	m_arData.Add(dat);	// 체결수량
}

void CNTable::ChangePalette()
{
	SetColor();
	DrawBase();
	DrawHText();
	DrawBText();
	DrawBorder();
}

void CNTable::ChangeData()
{
	DrawBase();
	DrawHText();
	DrawBText();
	DrawBorder();
}

void CNTable::SetColor()
{
	m_hColor = GetColor(2);		// head color
	m_cColor = GetColor(30);	// cell color
	m_lColor = GetColor(29);	// line color
	m_tColor = GetColor(63);	// text color
}

COLORREF CNTable::GetColor(int index)
{
	int		pos;
	char		buffer[128];
	CString		file, tmpS, dat, section, key;
	CStringArray	ary;

	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	GetPrivateProfileString(GENERALSN, "Palette", "Palette", buffer, sizeof(buffer), file);

	section = buffer;
	key.Format("RGB%03d", index);
	DWORD dw = GetPrivateProfileString(section, key, "", buffer, sizeof(buffer), file);
	if (dw <= 0)
	{
		if (!section.Compare("Palette"))
			return RGB(0, 0, 0);
		section = _T("Palette");
		dw = GetPrivateProfileString(section, key, "", buffer, sizeof(buffer), file);
		if (dw <= 0)	return RGB(0, 0, 0);
	}

	dat = buffer;
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmpS);
	}

	if (ary.GetSize() < 3)
		return RGB(215, 215, 215);
	int	r = atoi(ary.GetAt(0)); 
	int	g = atoi(ary.GetAt(1));
	int	b = atoi(ary.GetAt(2));
	return RGB(r, g, b);
}

CString CNTable::commaE(CString value, bool remove)
{
	CString		tmpS_, gb, tail;
	int		pos;

	value.TrimLeft();	value.TrimRight();
	if (value.IsEmpty())	return _T("");

	if (value.Left(1) == "+" || value.Left(1) == "-")
	{
		gb = value.Left(1);
		value = value.Mid(1);
	}
	
	pos = value.Find('.');
	if (pos != -1)
	{
		tail = value.Mid(pos);
		value = value.Left(pos);
	}

	if (remove)	gb = _T("");
	if (value.GetLength() < 3)
		return gb + value + tail;

	for (int ii = value.GetLength()-1, jj = 0; ii >= 0; jj++)
	{
		if (jj != 0 && jj % 3 == 0)
			tmpS_.Insert(0, ',');
		tmpS_.Insert(0, value.GetAt(ii));
		ii--;
	}
	return gb + tmpS_ + tail;
}

void CNTable::ChangeTitle(int index, CString title)
{
	if (index == -1)
	{
		m_default = true;
		m_arCT.Copy(m_arTitle);
		return;
	}

	m_default = false;
	if (0 < index && m_arCT.GetSize() > index)
		m_arCT.SetAt(index, title);
}
