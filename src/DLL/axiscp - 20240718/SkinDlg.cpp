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

/////////////////////////////////////////////////////////////////////////////
// CSkinDlg dialog


CSkinDlg::CSkinDlg(bool (*axiscall)(int, WPARAM, LPARAM), CWnd* pParent /*=NULL*/)
	: CDialog(CSkinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkinDlg)
	//}}AFX_DATA_INIT
	m_axiscall = axiscall;
	m_skinN = 0;

	HBITMAP hBitmap;
	CString file;
	file.Format("%s\\image\\mb_skin.bmp", Axis::home);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_bitmap[0].Attach(hBitmap);

	file.Format("%s\\image\\mb_actskin.bmp", Axis::home);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_bitmap[1].Attach(hBitmap);
}

CSkinDlg::~CSkinDlg()
{
	if (m_bitmap[0].GetSafeHandle())	m_bitmap[0].DeleteObject();
	if (m_bitmap[1].GetSafeHandle())	m_bitmap[1].DeleteObject();
}

void CSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkinDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkinDlg, CDialog)
	//{{AFX_MSG_MAP(CSkinDlg)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinDlg message handlers

BOOL CSkinDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;  
}

void CSkinDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC	 memDC;
	CRect	 iRc;

	GetClientRect(iRc);
	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap* pbitmap = memDC.SelectObject(&m_bitmap[0]);
		if (pbitmap)
		{
			dc.StretchBlt(iRc.left, iRc.top, iRc.Width(), iRc.Height(),
					&memDC, 0, 0, iRc.Width(), iRc.Height(), SRCCOPY);
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

	
	CDialog::OnLButtonDown(nFlags, point);
}

void CSkinDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect	sRc, cRc;
	GetClientRect(cRc);
	const int	gap = 1;
	const int h = (cRc.Height() - gap * 2) / m_ary.GetSize();

	sRc.SetRect(cRc.left, cRc.top + gap, cRc.right, cRc.top + gap + h);

	for (int ii = 0; ii < m_ary.GetSize(); ii++)
	{
		if (!sRc.PtInRect(point))
		{
			sRc.OffsetRect(0, h);
			continue;
		}

		m_skinN = ii;
		CString skinName = m_ary.GetAt(ii);
		CString enFile, user = AfxGetApp()->GetProfileString(WORKSTATION, SIGNONID);
		const int pos  = user.Find('|');
		if (pos < 0)	user = _T("Anonymous");
		else		user = user.Mid(pos+1);
		if (user.IsEmpty())	user = _T("Anonymous");

		change_Palette(skinName);
		(*m_axiscall)(AXI_CHANGEBAR2, 0, (long)skinName.operator LPCTSTR());
		EndDialog(IDCANCEL);
		break;
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CSkinDlg::draw_ActiveSkin(CDC* pDC)
{
	if (!m_bitmap[1].GetSafeHandle())
		return;
	CRect	cRc, sRc;
	const int	gap = 1;
	int sH;
	BITMAP	bm;

	GetClientRect(cRc);
	m_bitmap[1].GetBitmap(&bm);
	sH = (bm.bmHeight - gap * 2) / m_ary.GetSize();
	if (m_skinN < m_ary.GetSize())
	{
		sRc.SetRect(cRc.left, cRc.top + gap + (sH * m_skinN),
			cRc.right, cRc.top + gap + sH * (m_skinN + 1));

		CDC	 memDC;
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* pbitmap = memDC.SelectObject(&m_bitmap[1]);
			if (pbitmap)
			{
				pDC->StretchBlt(sRc.left, sRc.top, sRc.Width(), sRc.Height(),
						&memDC, 0, sH*m_skinN + gap, sRc.Width(), sRc.Height(), SRCCOPY);
				memDC.SelectObject(pbitmap);
			}
			memDC.DeleteDC();
		}
	}
}

void CSkinDlg::ShowSkin(CPoint point, bool down)
{
	if (!m_bitmap[0].m_hObject)	return;

	BITMAP	bm;
	m_bitmap[0].GetBitmap(&bm);

	m_down = down;
	if (down)
		SetWindowPos(&wndTopMost, point.x, point.y - 1, bm.bmWidth, bm.bmHeight, SWP_SHOWWINDOW);
	else
		SetWindowPos(&wndTopMost, point.x, point.y - bm.bmHeight + 1, bm.bmWidth, bm.bmHeight, SWP_SHOWWINDOW);
}

void CSkinDlg::ChangeInfo()
{
	loadPalette();
}

void CSkinDlg::loadPalette()
{
	int	pos{};
	CString	tmpS, dat, skin;

	m_ary.RemoveAll();

	CProfile profile(pkPalette);
	skin = profile.GetString(GENERALSN, "Palette");

	dat  = profile.GetString(GENERALSN, "Palettes");
	if (dat.IsEmpty())
		return;

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

	for (int ii = 0; ii < m_ary.GetSize(); ii++)
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
	skinN.TrimRight();

	CProfile profile(pkPalette);
	profile.Write(GENERALSN, "Palette", skinN);
}

