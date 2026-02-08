// PopupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "PopupWnd.h"

#define SellHeight	17
#define Sper		'|'
#define Plus		'+'
#define Minus		'-'

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupWnd

CPopupWnd::CPopupWnd()
{
	m_pApp = (CIB410100App *)AfxGetApp();
}

CPopupWnd::~CPopupWnd()
{
	if (m_pBitmap)
		m_pBitmap = nullptr;
}


BEGIN_MESSAGE_MAP(CPopupWnd, CWnd)
	//{{AFX_MSG_MAP(CPopupWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPopupWnd message handlers

int CPopupWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CPopupWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (m_pBitmap)
	{
		CDC		memDC;
		CBitmap*	pOldBitmap = nullptr;
		CRect		Popuprt;

		GetClientRect(&Popuprt);
		
		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap.get());
		
		dc.BitBlt(0, 0, Popuprt.Width(), Popuprt.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}	
	// Do not call CWnd::OnPaint() for painting messages
}

void CPopupWnd::DrawPopup(int chknum, CString names, CString data)
{
	CDC		memDC, *pDC = GetDC();
	CBitmap		*pOldBitmap=nullptr;

	LOGFONT		lf{};
	CFont		font, *pOldFont=nullptr;

	CStringArray	Name;
	CString		stmp;

	CStringArray	Popdata;
	CString		Data;

	int		pos=0;
	
	if (m_pBitmap)
		m_pBitmap = nullptr;

	m_pBitmap = std::make_unique<CBitmap>();
	
	GetClientRect(&m_poprt);

	memDC.CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, m_poprt.Width(), m_poprt.Height());

	lf = m_pApp->GetLogFont(9, false);
	font.CreatePointFontIndirect(&lf, &memDC);
	pOldFont = (CFont*)memDC.SelectObject(&font);

	pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap.get());
	
	memDC.SetBkMode(TRANSPARENT);

	Name.RemoveAll();
	Popdata.RemoveAll();
	
	for (;!names.IsEmpty();)
	{
		pos = names.Find(Sper);
		if (pos != -1)
		{
			stmp = names.Left(pos);
			names  = names.Mid(pos+1);
		}
		Name.Add(stmp);
	}
		
	for (;!data.IsEmpty();)
	{
		pos = data.Find(Sper);
		if (pos != -1)
		{
			Data = data.Left(pos);
			data = data.Mid(pos+1);
		}
		else
		{
			Data = data.Mid(pos);
			data.Empty();
		}
		
		Popdata.Add(Data);
	}	
	
	int		height = 0, width = 0, point = 0;
	COLORREF	textcolor{};
	
	for (int xx = 0; xx < 2; xx++)
	{
		for (int ii = 0; ii < chknum; ii++)
		{			
			if (xx == 0)
			{
				if (Name.GetSize() > 0)
					stmp = Name.GetAt(ii);

				if (stmp == "델타")
					textcolor = RGB(255, 0, 255);
				else if (stmp == "감마")
					textcolor = RGB(128, 128, 255);
				else if (stmp == "세타")
					textcolor = RGB(0, 192, 0);
				else if (stmp == "베가")
					textcolor = RGB(255, 128, 0);

				memDC.FillSolidRect(CRect(point, m_poprt.top + height, width + 30, SellHeight +  height), RGB(230, 230, 230));

				memDC.SetTextColor(textcolor);
				memDC.DrawText(stmp, CRect(point, m_poprt.top+height + 1, width + 30, SellHeight+height + 1), DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			}
			else
			{
				if (Popdata.GetSize() > 0)
				{
					CString tmp = _T("");
					
					Data = Popdata.GetAt(ii);

					memDC.FillSolidRect(CRect(point, m_poprt.top+height, width + 40, SellHeight+height), RGB(255, 255, 255));

					memDC.SetTextColor(RGB(0, 0, 0));
					memDC.DrawText(Data, CRect(point, m_poprt.top+height+1, width + 35, SellHeight+height+1), DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
				}
			}

			memDC.Draw3dRect(CRect(point, m_poprt.top + height, width + 40, SellHeight + height), RGB(192, 192, 192), RGB(192, 192, 192));
			height += SellHeight - 1;
			const CRect tmprt = CRect(point, m_poprt.top + height, width + 40, SellHeight + height);
		}

		point  = 30 - 1;
		width  = 40;
		height = 0;
	}

	Invalidate();
	memDC.SelectObject(pOldFont);
	font.DeleteObject();
	
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	ReleaseDC(pDC);
}