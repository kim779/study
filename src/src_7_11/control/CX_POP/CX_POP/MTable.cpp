// MTable.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CX_POP.h"
#include "MTable.h"

#include "../../../h/axisvar.h"
// CMTable

IMPLEMENT_DYNAMIC(CMTable, CWnd)
#define	CELLHEIGHT	20

CMTable::CMTable()
{
	m_pctrlWnd = NULL;
	m_pWnd = NULL;
}

CMTable::CMTable(CString home, CWnd* pWnd, CWnd* pctrlwnd)
{
	m_home  = home;
	m_pWnd = pWnd;
	m_pctrlWnd = pctrlwnd;
		
	m_headW = 0;
	m_default = true;
	maxX = 100;
	SetColor();
	m_pfont = NULL;
}

CMTable::~CMTable()
{
	m_arData.RemoveAll();
}


BEGIN_MESSAGE_MAP(CMTable, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CMTable 메시지 처리기입니다.


void CMTable::ChangeData()
{
	DrawManage();
}

void CMTable::Setfont(int ifont, CString font)
{
	if(m_pfont!= NULL)
	{
		delete m_pfont;
		m_pfont = NULL;
	}

	CClientDC dc(this);
	m_pfont = new CFont();
	m_pfont->CreatePointFont(ifont * 10, font, &dc);
}

void CMTable::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC		memDC;

	CRect	cRc;
	GetClientRect(cRc);

	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		CBitmap* pbmp = (CBitmap *) memDC.SelectObject(&bitmap);

		DrawManage(&memDC);
		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
	else
		DrawManage(&dc);
}

void CMTable::DrawManage(CDC *pDC)
{
	CClientDC	dc(this);
	if (!pDC)	
		pDC = &dc;

	CRect	cRc, hRc, wRc;
	m_pWnd->GetClientRect(hRc);
	GetClientRect(cRc);

	pDC->SelectStockObject(NULL_BRUSH);
	
	CPen	pen;
	pen.CreatePen(PS_SOLID, 1, m_lColor);
	CPen	*pOldPen = pDC->SelectObject(&pen);

	CFont*	pOldFont = pDC->SelectObject(m_pfont);

	pDC->FillSolidRect(hRc, m_hColor);
	//pDC->Rectangle(hRc);
	
	pDC->MoveTo(cRc.left, cRc.top);
	pDC->LineTo(cRc.left, cRc.bottom);

	pDC->MoveTo(cRc.left, cRc.bottom  -1);
	pDC->LineTo(cRc.right, cRc.bottom  -1);

	pDC->MoveTo(cRc.right - 1, cRc.bottom);
	pDC->LineTo(cRc.right - 1, cRc.top);

	int iposition = m_pctrlWnd->SendMessage(WM_USER + 999, 999 ,0);
	m_pctrlWnd->GetWindowRect(&wRc);
	ScreenToClient(wRc);

	CWnd* pwnd = (CWnd*)m_pctrlWnd->SendMessage(WM_USER + 999, 666 ,0);
	if(pwnd != NULL)
	{
		pwnd->GetWindowRect(&wRc);
		ScreenToClient(wRc);
		pDC->MoveTo(cRc.left , cRc.top);
		pDC->LineTo(wRc.left, cRc.top);

		pDC->MoveTo(wRc.right, cRc.top);
		pDC->LineTo(cRc.right, cRc.top);
	}				
	
	COLORREF color = pDC->SetTextColor(m_tColor);

	cRc.DeflateRect(5,0);
	cRc.OffsetRect(0, 5);

	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_data, cRc, DT_LEFT|DT_VCENTER  );	
	
	pDC->SetTextColor(color);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}


CString CMTable::Parser(CString &srcstr, CString substr)
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

void CMTable::SetTitle(char* body)
{
	CSize	size;
	CString	text;

	CClientDC	dc(this);
	CFont*	font = dc.SelectObject(m_pfont);

	CString tmps;
	int cnt = 0;
	text.Format(_T("%s"), body);
	m_data = text;
	m_arData.RemoveAll();
	while(1)
	{
		tmps = Parser(text, _T("\r"));
		if(tmps.GetLength() < 1)
			break;
		cnt++;
		m_arData.Add(tmps);
		size = dc.GetTextExtent(tmps);
		if(maxX < size.cx)
			maxX = size.cx;
	}
	dc.SelectObject(font);
	SetWindowPos(NULL, 0, 0, maxX + 10, size.cy*(1 + cnt), SWP_NOMOVE|SWP_SHOWWINDOW);
}

void CMTable::SetData(CString dat)
{
	CString	str,tmps;
	m_arData.RemoveAll();
	m_data = dat;

	while(1)
	{
		tmps = Parser(dat, _T("\r"));
		if(tmps.GetLength() < 1)
			break;
		m_arData.Add(tmps);
	}
}

void CMTable::ChangePalette()
{
	SetColor();
#if 0
	DrawBase();
	DrawHText();
	DrawBText();
	DrawBorder();
#else
	DrawManage();
#endif
}

void CMTable::SetColor()
{
	/*
	m_hColor = GetColor(2);		// head color
	m_cColor = GetColor(30);	// cell color
	m_lColor = GetColor(29);	// line color
	m_tColor = GetColor(63);	// text color
	*/

	m_hColor = RGB(249,249,249);		// head color
	m_lColor = RGB(183,187,209);	// line color
	m_tColor = RGB(75,96,139);
}

COLORREF CMTable::GetColor(int index)
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


CString CMTable::commaE(CString value, bool remove)
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


int CMTable::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	
	return 0;
}
