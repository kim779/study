// MTable.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "MTable.h"
#include "resource.h"
#include "axmsg.hxx"

#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMTable

CMTable::CMTable(CString home, int viewN, bool hLine)
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
}

CMTable::~CMTable()
{
	m_font.DeleteObject();
	m_arData.RemoveAll();
}


BEGIN_MESSAGE_MAP(CMTable, CWnd)
	//{{AFX_MSG_MAP(CMTable)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMTable message handlers

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

void CMTable::SetTitle(char* body)
{
	CSize	size;
	CString	text;

	CClientDC	dc(this);
	CFont*	font = dc.SelectObject(&m_font);

	text = body;
	size = dc.GetTextExtent(text);
	dc.SelectObject(font);

	m_arMultiData.RemoveAll();
	SetWindowPos(NULL, 0, 0, size.cx, CELLHEIGHT*4, SWP_NOMOVE|SWP_SHOWWINDOW);
}

void CMTable::SetContent(CString strdata)
{
	CSize	size;
	CString	text;

	CClientDC	dc(this);
	CFont*	font = dc.SelectObject(&m_font);

	size = dc.GetTextExtent("일이삼사오육칠팔구십일이삼사오가나다라마바사아자차");
	dc.SelectObject(font);

	int cnt = 0;
	m_arMultiData.RemoveAll();
	while(1)
	{
		m_arMultiData.Add(Parser(strdata,_T("#@")));
		if(strdata.IsEmpty())
			break;
	}
	cnt = m_arMultiData.GetSize() - 1;

	SetWindowPos(NULL, 0, 0, size.cx, CELLHEIGHT*(4+cnt), SWP_NOMOVE|SWP_SHOWWINDOW);
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

// 통보제목\t통보내용
void CMTable::SetData(CString dat)
{
	CString	str;
	m_arData.RemoveAll();

	while(1)
	{
		if(dat.GetLength() <=0 )
			break;
		str = Parser(dat, _T("\t"));
	//	m_arData.Add(Parser(dat, _T("\t")));
		m_arData.Add(str);
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

void CMTable::ChangeData()
{
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
	m_hColor = RGB(255,0,0);		// head color
	m_cColor = GetColor(30);	// cell color
	m_lColor = RGB(140,140,140);	// line color
	m_tColor = GetColor(63);	// text color
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

void CMTable::DrawManage(CDC *pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

//	if (m_arData.GetSize() < 2)	
	//	return;
	/*
	CString text1, text2;
	text1 = m_arData.GetAt(1);
	if (text1.Find('|') >= 0)
	{
		int pos = text1.Find('|');
		text2 = text1.Mid(pos + 1);
		text1 = text1.Left(pos);
	}
*/
	CRect	cRc, hRc;
	GetClientRect(cRc);

	pDC->SelectStockObject(NULL_BRUSH);
	
	CPen	pen;
	pen.CreatePen(PS_SOLID, 5, m_lColor);
	CPen	*pOldPen = pDC->SelectObject(&pen);

	CFont*	pOldFont = pDC->SelectObject(&m_font);

	pDC->FillSolidRect(cRc, RGB(255,255,255));
	pDC->Rectangle(cRc);

	COLORREF color = pDC->SetTextColor(RGB(250,84,0));

	hRc.SetRect(cRc.left + 10, cRc.top + 12, cRc.right, cRc.top + 12 + CELLHEIGHT);
	pDC->DrawText(m_arData.GetAt(0), hRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	if(m_arData.GetAt(2) == _T("Y"))
	{
		pDC->SetTextColor(RGB(70,70,70));
		hRc.OffsetRect(0, CELLHEIGHT);	
		CString strdata;
		strdata = m_arData.GetAt(1);
		CStringArray arr;
		int ileft = strdata.GetLength() % 20;
		int cnt= 0 ;
		CString temps;
		
		if(m_arMultiData.GetSize() > 0)
		{
			for(int ii = 0 ; ii < m_arMultiData.GetSize() ; ii++)
			{
				pDC->DrawText(m_arMultiData.GetAt(ii), hRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);	
				hRc.OffsetRect(0, CELLHEIGHT);	
			}
		}
		else
		{
			pDC->DrawText(m_arData.GetAt(1), hRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);	
		}
	}

	pDC->SetTextColor(color);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}

void CMTable::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_arData.GetSize() > 2)
	{
		CString strtemp;
		strtemp.Format(_T("%s\t%s"),  m_arData.GetAt(m_arData.GetSize() - 2),m_arData.GetAt(m_arData.GetSize() -1));
		AfxGetMainWnd()->SendMessage(WM_USER+500, MAKEWPARAM(axFITSMNG, 0), (LPARAM)strtemp.operator LPCSTR());
	}

	CWnd::OnLButtonDown(nFlags, point);
}
