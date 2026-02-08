// ThemePreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThemePreviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThemePreviewDlg dialog


CThemePreviewDlg::CThemePreviewDlg(int currIndex, CWnd* pParent /*=NULL*/)
	: CDialog(CThemePreviewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThemePreviewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_index = m_oldIndex = currIndex;
}


void CThemePreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThemePreviewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CThemePreviewDlg, CDialog)
	//{{AFX_MSG_MAP(CThemePreviewDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThemePreviewDlg message handlers

BOOL CThemePreviewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//m_changer = new CControlChanger(this);
	m_changer = std::make_unique <CControlChanger>(this);

	LoadBitmaps();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CThemePreviewDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	//delete m_changer;
	
	FreeBitmaps();	
}

void CThemePreviewDlg::LoadBitmaps()
{
	CString skin;
	CProfile profile(pkPalette);
	CString currentSkin = profile.GetString(szGeneral, szPalette);
	CString buffer = profile.GetString(szGeneral, szPalettes);

	int pos{};

	while (!buffer.IsEmpty())
	{
		pos = buffer.Find(',');
		if (pos < 0)
		{
			Trim(buffer);
			m_themes.Add(buffer);
			break;
		}

		skin = buffer.Left(pos);
		buffer = buffer.Mid(pos + 1);
		Trim(skin);
		m_themes.Add(skin);
	}

//	CFileBitmap* bitmap{};

	for (int i = 0; i < m_themes.GetSize(); i++)
	{
		//bitmap = new CFileBitmap(Axis::home, Format("demo_%s", m_themes[i]));
		auto bitmap = std::make_shared<CFileBitmap>(Axis::home, Format("demo_%s", m_themes[i]));
	//	m_bitmaps.Add(bitmap);
		m_bitmaps.emplace_back(bitmap);
	}
}

void CThemePreviewDlg::FreeBitmaps()
{
	/*for (int i = 0; i < m_bitmaps.GetSize(); i++)
	{
		delete m_bitmaps[i];
	}*/
	m_bitmaps.clear();
}

void CThemePreviewDlg::OnPrev() 
{
	m_oldIndex = m_index--;
	if (m_index < 0)
		m_index = m_themes.GetSize() - 1;

	DrawSample();
}

void CThemePreviewDlg::OnNext() 
{
	m_oldIndex = m_index++;
	if (m_index >= m_themes.GetSize())
		m_index = 0;

	DrawSample();
}

void CThemePreviewDlg::OnPaint() 
{
	CPaintDC dc(this); 
	
	DrawSample(&dc);
}


void CThemePreviewDlg::DrawSample(CDC* pDC)
{
	//if (m_index < 0 || m_index >= m_bitmaps.GetSize())
	if (m_index < 0 || m_index >= m_bitmaps.size())
		return;

	//CDC* tempDC = NULL;
	std::unique_ptr<CDC> tempDC;
	if (pDC == NULL)
	{
	//	tempDC = new CClientDC(this);
		tempDC = std::make_unique<CClientDC>(this);
	//	pDC = tempDC;
		pDC = tempDC.get();
		GetDlgItem(IDC_THEME)->Invalidate();
	}

	CRect rc, crc;
	GetDlgItem(IDC_THEME)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	crc = rc;
	crc -= crc.TopLeft();

	CDC mdc;
	mdc.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	CBitmap* oldBmp = mdc.SelectObject(&bmp);

	DrawGradient(&mdc, crc, RGB(127, 127, 127), RGB(255, 255, 255), false);

	CFont font;
	font.CreatePointFont(140, "Comic Sans MS");
	CFont* oldFont = mdc.SelectObject(&font);

	mdc.SetBkMode(TRANSPARENT);
	mdc.DrawText(m_themes[m_index], crc, DT_BOTTOM | DT_SINGLELINE | DT_CENTER);
	mdc.SelectObject(oldFont);

	if (m_bitmaps[m_index]->m_hObject == NULL)
	{
		mdc.DrawText(CString("샘플이미지가 없습니다."), crc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	else
	{
		CDC dc;
		dc.CreateCompatibleDC(pDC);

		//CBitmap* old = (CBitmap*)dc.SelectObject(m_bitmaps[m_index]);
		CBitmap* old = (CBitmap*)dc.SelectObject(m_bitmaps[m_index].get());

		BITMAP bm;
		m_bitmaps[m_index]->GetBitmap(&bm);

		const int x = crc.Width() / 2 - bm.bmWidth / 2;
		const int y = crc.Height() / 2 - bm.bmHeight / 2;

		mdc.BitBlt(x, y, bm.bmWidth, bm.bmHeight, &dc, 0, 0, SRCCOPY);

		dc.SelectObject(old);
	}
	pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);

	if (tempDC) {
		//delete tempDC;
		GetDlgItem(IDC_PREV)->Invalidate();
		GetDlgItem(IDC_NEXT)->Invalidate();
	}
}

void CThemePreviewDlg::DrawGradient(CDC* pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect	rect;
	int	nWidth{}, nHeight{};

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs{}, gs{}, bs{};
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re{}, ge{}, be{};
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep{}, gStep{}, bStep{};
	CPen* sPen{};
	if (bRight)
	{
		if (!nWidth)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nWidth);
		gStep = float(gs - ge)/float(nWidth);
		bStep = float(bs - be)/float(nWidth);

		if (!nHeight)
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left + ii, drawRC.top);
				pDC->LineTo(drawRC.left + ii + 1, drawRC.top);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nHeight);
		gStep = float(gs - ge)/float(nHeight);
		bStep = float(bs - be)/float(nHeight);

		if (!nWidth)
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left, drawRC.top + ii);
				pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
}

int CThemePreviewDlg::GetIndex()
{
	return m_index;
}

BOOL CThemePreviewDlg::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;	
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CThemePreviewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
