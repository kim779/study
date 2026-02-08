// SkinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SkinDlg.h"
#include "../../h/axisvar.h"
#include "../../axis/axMsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOPMARGINE	0
#define IMGWIDTH	13
#define IMGHEIGHT	14
/////////////////////////////////////////////////////////////////////////////
// CSkinDlg dialog


CSkinDlg::CSkinDlg(bool (*axiscall)(int, WPARAM, LPARAM), CString home, CWnd* pParent /*=NULL*/)
	: CDialog(CSkinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkinDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_axiscall = axiscall;
	m_home = home;
	m_skinN = 0;
	CString		file;
	file.Format("%s\\%s\\%s", m_home, IMAGEDIR, "skin.bmp");
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_bitmap.Attach(hBitmap);
}

CSkinDlg::~CSkinDlg()
{
	m_ary.RemoveAll();
	if (m_bitmap.GetSafeHandle())	m_bitmap.DeleteObject();
}

void CSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkinDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkinDlg, CDialog)
	//{{AFX_MSG_MAP(CSkinDlg)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinDlg message handlers

BOOL CSkinDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSkinDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC	 memDC;
	CRect	 iRc;

	GetClientRect(iRc);
	if (!m_bitmap.GetSafeHandle())
		return;
	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap* pbitmap = memDC.SelectObject(&m_bitmap);
		if (pbitmap)
		{
			BITMAP	bm;
			m_bitmap.GetBitmap(&bm);
			dc.BitBlt(iRc.left, iRc.top, iRc.Width(), iRc.Height(),
					&memDC, 0, bm.bmHeight / 2, SRCCOPY);
			memDC.SelectObject(pbitmap);
		}
		memDC.DeleteDC();
	}

	draw_ActiveSkin(&dc);
	if (m_down)
	{
		iRc.SetRect(iRc.left+1, iRc.bottom-1, iRc.right-1, iRc.bottom);
		dc.FillSolidRect(iRc, RGB(64, 64, 64));
	}
	else
	{
		iRc.SetRect(iRc.left+1, iRc.top-1, iRc.right-1, iRc.top);
		dc.FillSolidRect(iRc, RGB(64, 64, 64));
	}
}


void CSkinDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	EndDialog(IDCANCEL);
}

void CSkinDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	sRc, cRc;
	const int	gap = TOPMARGINE, w = IMGWIDTH, h = IMGHEIGHT;

	GetClientRect(cRc);
	sRc.SetRect(cRc.left + gap, cRc.top + gap,
			cRc.left + gap + w, cRc.top + gap + h);

	for (int ii = 0; ii < m_ary.GetSize(); ii++)
	{
		if (!sRc.PtInRect(point))
		{
			sRc.OffsetRect(0, h+1);
			continue;
		}

		m_skinN = ii;
		CString skinName = m_ary.GetAt(ii);
		change_Palette(skinName);
		(*m_axiscall)(AXI_CHANGEBAR2, 0, (long)(const char*)skinName);
		EndDialog(IDCANCEL);
		break;
	}
	

	CDialog::OnLButtonDown(nFlags, point);
}

void CSkinDlg::draw_ActiveSkin(CDC* pDC)
{
	if (!m_bitmap.GetSafeHandle())
		return;
	CRect	cRc, sRc;
	const int	gap = TOPMARGINE;
	int sH;
	BITMAP	bm;

	GetClientRect(cRc);
	m_bitmap.GetBitmap(&bm);
	sH = bm.bmHeight/(m_ary.GetSize() * 2);	// select + non select
	if (m_skinN < m_ary.GetSize())
	{
		sRc.SetRect(cRc.left + gap, cRc.top + gap,
			cRc.left + gap + bm.bmWidth, cRc.top + gap + sH);
		sRc.OffsetRect(0, (sH)*m_skinN);
		CDC	 memDC;
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* pbitmap = memDC.SelectObject(&m_bitmap);
			if (pbitmap)
			{
				pDC->BitBlt(sRc.left, sRc.top, sRc.Width(), sRc.Height(),
						&memDC, 0, sH*m_skinN, SRCCOPY);
				memDC.SelectObject(pbitmap);
			}
			memDC.DeleteDC();
		}
	}
}

void CSkinDlg::ShowSkin(CPoint point, bool down /*= false*/)
{
	BITMAP	bm;
	m_bitmap.GetBitmap(&bm);

	m_down = down;
	if (down)
		SetWindowPos(&wndTop, point.x-1, point.y+4,
			bm.bmWidth, bm.bmHeight / 2, SWP_SHOWWINDOW);
	else
		SetWindowPos(&wndTop, point.x-1, point.y-bm.bmHeight/2-2,
		bm.bmWidth, bm.bmHeight / 2, SWP_SHOWWINDOW);
}

void CSkinDlg::ChangeInfo()
{
	loadPalette();
}

void CSkinDlg::loadPalette()
{
	int	pos{};
	char	buffer[128]{};
	CString	file, tmpS, dat, skin;

	m_ary.RemoveAll();
	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	DWORD dw = GetPrivateProfileString(GENERALSN, szPalette, "", buffer, sizeof(buffer), file);
	if (dw <= 0)	skin = _T("");
	else		skin = buffer;
	dw = GetPrivateProfileString(GENERALSN, szPalettes, "", buffer, sizeof(buffer), file);
	if (dw <= 0)	return;

	dat = buffer;
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			m_ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		m_ary.Add(tmpS);
	}

	for (int ii = 0; ii < m_ary.GetSize() && ii < MAX; ii++)
	{
		dat = m_ary.GetAt(ii);
		dat.TrimLeft();	dat.TrimRight();
		if (dat == skin)
		{
			m_skinN = ii;
			break;
		}
	}
}

void CSkinDlg::change_Palette(CString skinN)
{
	CString	file;
	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	WritePrivateProfileString(GENERALSN, szPalette, (const char *)skinN, file);
}

