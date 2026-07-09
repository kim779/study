// colorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "colorDlg.h"

#include "../../h/axisvar.h"
#include "../../dll/lib/axislib.h"
#include "../mainvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	winMARGIN	5
#define	winGAP		1
#define	itemCOUNT	16
#define	itemWIDTH	20
#define	itemHEIGHT	18

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

CColorDlg::CColorDlg(CWnd* pParent, DWORD color, bool user /*=true*/)
	: CDialog(CColorDlg::IDD, pParent)
{
	CString	tmps;
	CWinApp* app = AfxGetApp();
	tmps = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	m_path.Format("%s\\%s\\%s", tmps, TABDIR, PALETTE);
	m_palette = (CAxisPalette *) NULL;
	m_draw    = (CAxisDraw *) NULL;

	m_color = color;
	m_user  = user;
	m_palette = new CAxisPalette((char *)m_path.operator LPCTSTR());
	m_draw    = new CAxisDraw();

	//{{AFX_DATA_INIT(CColorDlg)
	//}}AFX_DATA_INIT
}

CColorDlg::~CColorDlg()
{
	if (m_draw)	delete m_draw;
	if (m_palette)	delete m_palette;
}

void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers

BOOL CColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect	cRc, wRc;
	
	GetClientRect(&cRc);
	GetWindowRect(&wRc);
	int dW = wRc.Width() - cRc.Width();
	int dH = wRc.Height() - cRc.Height();
	dW += (itemCOUNT * itemWIDTH);
	dW += ((itemCOUNT - 1) * winGAP);
	dW += winMARGIN * 2;
	dH += (itemCOUNT * itemHEIGHT);
	dH += ((itemCOUNT - 1) * winGAP);
	dH += winMARGIN * 2;

	dH += winMARGIN;
	dH += (int)(itemHEIGHT * 1.5);

	SetWindowPos(NULL, 0, 0, dW, dH, SWP_NOMOVE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	rc;

	for (int ii = 0; ii < itemCOUNT; ii++)
	{
		for (int jj = 0; jj < itemCOUNT; jj++)
		{
			rc = makeRect(ii, jj);
			if (rc.PtInRect(point))
			{
				m_color = ii * itemCOUNT + jj;
				CDialog::OnOK();
				break;
			}
		}
	}
	
	if (m_user)
	{
		rc = makeRect();
		if (rc.PtInRect(point))
		{
			onUser();
			CDialog::OnOK();
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CColorDlg::OnPaint() 
{
	CRect	rc;
	CBrush*	brush;
	CPaintDC dc(this);
	bool	usePAL = (m_color & userCOLOR) ? false : true;
	
	for (int ii = 0; ii < itemCOUNT; ii++)
	{
		for (int jj = 0; jj < itemCOUNT; jj++)
		{
			rc = makeRect(ii, jj);
			brush = m_draw->SetBrush(&dc, m_palette->GetPaletteRGB(ii*itemCOUNT+jj));
			dc.FillRect(rc, brush);
			dc.DrawEdge(&rc, EDGE_SUNKEN, BF_RECT);
	
			if (usePAL && (int)m_color == ii*itemCOUNT+jj)
			{
				dc.SelectStockObject(BLACK_PEN);
				dc.SelectStockObject(NULL_BRUSH);
				dc.Rectangle(rc);
				rc.DeflateRect(1, 1);
				dc.Rectangle(rc);
			}	
		}
	}

	rc = makeRect();
	if (m_color & userCOLOR)
	{
		brush = m_draw->SetBrush(&dc, m_color);
		dc.FillRect(rc, brush);
	}

	if (m_user)
		dc.SetTextColor(PALETTERGB(0, 0, 0));
	else
		dc.SetTextColor(PALETTERGB(128, 128, 128));

	dc.DrawEdge(&rc, EDGE_ETCHED, BF_RECT);
	dc.SelectStockObject(SYSTEM_FONT);
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText("User Defined Color", &rc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
}

CRect CColorDlg::makeRect(int row, int col)
{
	CRect	cRc;

	if (row == -1 && col == -1)
	{
		cRc.left   = winMARGIN;
		cRc.top    = (itemHEIGHT + winGAP) * itemCOUNT + winGAP*4;
		cRc.right  = cRc.left + (itemWIDTH + winGAP)*itemCOUNT - winGAP;
		cRc.bottom = (int)(cRc.top + itemHEIGHT*1.5);
	}
	else
	{
		cRc.left   = ((itemWIDTH  + winGAP) * col) + winMARGIN;
		cRc.top    = ((itemHEIGHT + winGAP) * row) + winMARGIN;
		cRc.right  = cRc.left + itemWIDTH;
		cRc.bottom = cRc.top + itemHEIGHT;
	}

	return cRc;
}

void CColorDlg::onUser() 
{
	CColorDialog    dlg;
	COLORREF	color[16];

	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	if (m_color & userCOLOR)
		dlg.m_cc.rgbResult = RGB(GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
	loadUserColor(color);
	dlg.m_cc.lpCustColors = color;

	if (dlg.DoModal() == IDOK)
	{
		COLORREF clr = dlg.GetColor();
		m_color = PALETTERGB(GetRValue(clr), GetGValue(clr), GetBValue(clr));
		Invalidate();
		COLORREF* cclr = dlg.m_cc.lpCustColors;
		saveUserColor(cclr);

		CDialog::OnOK();
	}
}

void CColorDlg::loadUserColor(COLORREF* ccolor)
{
	char	wb[64];
	CString	tmps;

	for (int ii = 0; ii < 16; ii++)
	{
		tmps.Format("CustomRGB%02d", ii);
		GetPrivateProfileString(GENERALSN, tmps, "", wb, sizeof(wb), m_path);
		tmps = wb;
		if (tmps.IsEmpty())
			ccolor[ii] = 0x00ffffff;
		else
			ccolor[ii] = atoi(tmps);
	}
}

void CColorDlg::saveUserColor(COLORREF* ccolor)
{
	CString	tmps, keys;

	for (int ii = 0; ii < 16; ii++)
	{
		keys.Format("CustomRGB%02d", ii);
		tmps.Format("%d", ccolor[ii]);
		WritePrivateProfileString(GENERALSN, keys, tmps, m_path);
	}
}