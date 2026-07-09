// ConclusionDLG.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ConclusionDLG.h"
#include "../h/axisvar.h"
#include "fxGridEx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MENU_ITEM		1000
#define USERID_GRID_MAIN	2002

const	int rowH   = 18;
const	int rowCol = 8;

// 그리드 
#define	clrGridboder	29
#define	clrFixBk	2
#define	clrFixFg	58
#define clrwhite	27
#define clrblack	0

const UINT maskHdr  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;
const UINT maskItem = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

struct	_gridHdrCON {
	char*	text;		// fixed text
	UINT	width;		// fixed width
	UINT	attr;		// fixed attribute
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr
};

struct	_gridHdrCON gridHdrCON[] = {
	{"시간",	120,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 0
	{"주문#",	80,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 0
	{"원주문#",	80,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 0
	{"계좌번호",	110,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 0
	{"종목명",	110,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 0
	{"구분",	50,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 1
	{"수량",	60,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 2
	{"가격",	60,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 3
};
/////////////////////////////////////////////////////////////////////////////
// CConclusionDLG dialog


CConclusionDLG::CConclusionDLG(CWnd* pParent /*=NULL*/, CString home)
	: CDialog(CConclusionDLG::IDD, pParent)
{
	m_grid = NULL;
	m_home = home;
}

CConclusionDLG::~CConclusionDLG()
{
	if (m_grid)	delete m_grid;
}

void CConclusionDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConclusionDLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConclusionDLG, CDialog)
	//{{AFX_MSG_MAP(CConclusionDLG)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConclusionDLG message handlers
BOOL CConclusionDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nMaxData = 101;

	initGrid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConclusionDLG::InsertMsg(CString msg)
{
	int		pos;
	CString		str;
 
	if (msg.IsEmpty())
		return;

	insertRow(1);

	pos = msg.Find('\t');
	if (pos != -1)
	{
		str = msg.Left(pos++);
		msg = msg.Mid(pos);

		m_grid->SetItemText(1, 0, str);
	}

	pos = msg.Find('\t');
	if (pos != -1)
	{
		str = msg.Left(pos++);
		msg = msg.Mid(pos);

		m_grid->SetItemText(1, 1, str);
	}

	pos = msg.Find('\t');
	if (pos != -1)
	{
		str = msg.Left(pos++);
		msg = msg.Mid(pos);

		m_grid->SetItemText(1, 2, str);
	}

	pos = msg.Find('\t');
	if (pos != -1)
	{
		str = msg.Left(pos++);
		msg = msg.Mid(pos);

		m_grid->SetItemText(1, 3, str);
	}

	pos = msg.Find('\t');
	if (pos != -1)
	{
		str = msg.Left(pos++);
		msg = msg.Mid(pos);

		m_grid->SetItemText(1, 4, str);
	}

	pos = msg.Find('\t');
	if (pos != -1)
	{
		str = msg.Left(pos++);
		msg = msg.Mid(pos);

		m_grid->SetItemText(1, 5, str);

		if (str == "매수")	m_grid->SetItemFgColor(1, 5, RGB(255,0,0));
		else			m_grid->SetItemFgColor(1, 5, RGB(0,0,255));
	}

	pos = msg.Find('\t');
	if (pos != -1)
	{
		str = msg.Left(pos++);
		msg = msg.Mid(pos);

		m_grid->SetItemText(1, 6, str);
	}

	pos = msg.Find('\t');
	if (pos != -1)
	{
		str = msg.Left(pos++);
		msg = msg.Mid(pos);
	}	
	else	str = msg;

	m_grid->SetItemText(1, 7, str);
	
	if (m_nMaxData < m_grid->GetRowCount())
		m_grid->DeleteRow(m_grid->GetRowCount() - 1);
}

void CConclusionDLG::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
	CDialog::OnClose();
}

void CConclusionDLG::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_GridFont.DeleteObject();
	((CMainFrame*)AfxGetMainWnd())->m_ConCDLG = NULL;
	
	delete this;

	CDialog::PostNcDestroy();
}

void CConclusionDLG::OnCancel() 
{
	// TODO: Add extra cleanup here

	ShowWindow(SW_HIDE);
	
	//CDialog::OnCancel();
}

void CConclusionDLG::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if (cx == 0 && cy == 0)
		return;

	if(IsWindowVisible())
	{
		RedrawCtrl(true);
		Invalidate();
	}
}

void CConclusionDLG::RedrawCtrl(bool bResize)
{
	if (m_grid != NULL)
	{
		CRect tempRt;
		GetClientRect(tempRt);
		int nsize = (tempRt.Width() - GetSystemMetrics(SM_CYVSCROLL)) / 10;

		m_grid->SetColumnWidth(0, int(nsize * 1.5));
		m_grid->SetColumnWidth(1, nsize);
		m_grid->SetColumnWidth(2, nsize);
		m_grid->SetColumnWidth(3, int(nsize * 1.5));
		m_grid->SetColumnWidth(4, nsize * 2);
		m_grid->SetColumnWidth(5, nsize);
		m_grid->SetColumnWidth(6, nsize);
		m_grid->SetColumnWidth(7, nsize);

		m_grid->SetWindowPos(NULL, tempRt.left, tempRt.top, tempRt.Width(), tempRt.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
	}
}

void CConclusionDLG::initGrid()
{
	int ii = 0;
	LOGFONT logfont;

	ZeroMemory(&logfont, sizeof(LOGFONT));

	m_GridFont.CreateFont(	   9*10,                        // nHeight
				   0,                         // nWidth
				   0,                         // nEscapement
				   0,                         // nOrientation
				   FW_NORMAL,	              // nWeight
				   FALSE,                     // bItalic
				   FALSE,                     // bUnderline
				   0,                         // cStrikeOut
				   DEFAULT_CHARSET,              // nCharSet
				   OUT_DEFAULT_PRECIS,        // nOutPrecision
				   CLIP_DEFAULT_PRECIS,       // nClipPrecision
				   DEFAULT_QUALITY,           // nQuality
				   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				   "굴림");                 // lpszFacename

	m_GridFont.GetLogFont(&logfont);

	m_grid = new CfxGridEx();
	m_grid->Create(CRect(0, 0, 0, 0) , this, USERID_GRID_MAIN, GVSC_VERT);
	m_grid->Initial(m_nMaxData, rowCol, 1, 0);
	//m_grid->SetGridDIScroll(true);

	int	width = 0;
	GVITEM	gvitem;

	for (ii = 0; ii < rowCol; ii++)
	{
		gvitem.row    = 0;
		gvitem.col    = ii;
		gvitem.state  = 0;
		gvitem.mask   = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		gvitem.format = GVFM_HEADER;
		gvitem.text   = gridHdrCON[ii].text;
		gvitem.attr   = gridHdrCON[ii].attr;
		gvitem.fgcol  = GetColor(clrFixFg);
		gvitem.bkcol  = GetColor(clrFixBk);

		CopyMemory(&gvitem.font, &logfont, sizeof(LOGFONT));

		m_grid->SetItem(&gvitem);
		m_grid->SetColumnWidth(ii, gridHdrCON[ii].width);
	}

	for (ii = 0; ii < rowCol; ii++)
	{
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= gridHdrCON[ii].format;
		gvitem.attr  = gridHdrCON[ii].attrx;
		gvitem.fgcol = GetColor(clrFixFg);
		gvitem.bkcol = GetColor(clrwhite);

		CopyMemory(&gvitem.font, &logfont, sizeof(LOGFONT));

		m_grid->SetColumnItems(ii, &gvitem);
	}

	for (ii = 0; ii < m_grid->GetRowCount(); ii++)
		m_grid->SetRowHeight(ii, rowH);

	CRect tempRt;
	GetClientRect(tempRt);

	int nsize = (tempRt.Width() - GetSystemMetrics(SM_CYVSCROLL)) / 10;

	m_grid->SetColumnWidth(0, int(nsize * 1.5));
	m_grid->SetColumnWidth(1, nsize);
	m_grid->SetColumnWidth(2, nsize);
	m_grid->SetColumnWidth(3, int(nsize * 1.5));
	m_grid->SetColumnWidth(4, nsize * 2);
	m_grid->SetColumnWidth(5, nsize);
	m_grid->SetColumnWidth(6, nsize);
	m_grid->SetColumnWidth(7, nsize);

	m_grid->SetWindowPos(NULL, tempRt.left, tempRt.top, tempRt.Width(), tempRt.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
	
	m_grid->SetGridColor(GetColor(clrGridboder));
	m_grid->SetGridFocusLine(FALSE, TRUE);
	m_grid->SetGridScroll(GVSC_BOTH);
	m_grid->Adjust();
}

void CConclusionDLG::insertRow(int row)
{
	m_grid->InsertRow("", row);
	GVITEM gvitem;

	LOGFONT logfont;

	ZeroMemory(&logfont, sizeof(LOGFONT));
	m_GridFont.GetLogFont(&logfont);

	for (int ii = 0; ii < rowCol; ii++)
	{
		gvitem.row = row;
		gvitem.col = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;
		gvitem.format= gridHdrCON[ii].format;
		gvitem.attr  = gridHdrCON[ii].attrx;
		gvitem.fgcol = GetColor(clrFixFg);
		gvitem.bkcol = GetColor(clrwhite);

		CopyMemory(&gvitem.font, &logfont, sizeof(LOGFONT));	
		m_grid->SetItem(&gvitem);
	}

	m_grid->SetRowHeight(row, rowH);
	m_grid->Adjust();
}

COLORREF CConclusionDLG::GetColor(int index)
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
	return RGB(atoi(ary.GetAt(0)), atoi(ary.GetAt(1)), atoi(ary.GetAt(2)));
}

BOOL CConclusionDLG::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	int ii = 0;
	if (!m_grid)
		return CDialog::OnNotify(wParam, lParam, pResult);
	switch (wParam)
	{
	case USERID_GRID_MAIN:
		{
			int	row, col;
			CString tmp;

			NM_GVITEM* nmgvitem = (NM_GVITEM*) lParam;

			row = nmgvitem->row;
			col = nmgvitem->col;
			
			switch (nmgvitem->hdr.code)
			{
			case GVNM_RMOUSEDOWN:
				{
					CPoint	pt;

					GetCursorPos(&pt);
					CMenu	menu;
					UINT	option = 0;

					int	maxData[6] = {51, 101, 201, 501, 1001, 2001};
					CString	maxString[6] = {"50", "100", "200", "500", "1000", "2000"};
					
					menu.CreatePopupMenu();

					menu.AppendMenu(MF_STRING, MENU_ITEM, "초기화");
					menu.AppendMenu(MF_STRING, MENU_ITEM + 1, "닫   기");
					menu.AppendMenu(MF_SEPARATOR, 0, "");

					for (ii = 0; ii < 6; ii++)
					{
						menu.AppendMenu((maxData[ii] == m_nMaxData) ?  MF_CHECKED : 0 , MENU_ITEM + ii + 2 , maxString[ii]);
					}

					int menuID = menu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, pt.x, pt.y, this);

					menu.DestroyMenu();

					switch (menuID)
					{
					case MENU_ITEM:
						m_grid->Clear();
						break;
					case MENU_ITEM + 1:
						ShowWindow(SW_HIDE);
						break;
					case MENU_ITEM + 2:
						m_nMaxData = 51;
						break;
					case MENU_ITEM + 3:
						m_nMaxData = 101;
						break;
					case MENU_ITEM + 4:
						m_nMaxData = 201;
						break;
					case MENU_ITEM + 5:
						m_nMaxData = 501;
						break;
					case MENU_ITEM + 6:
						m_nMaxData = 1001;
						break;
					case MENU_ITEM + 7:
						m_nMaxData = 2001;
						break;
					}
					
					m_grid->SetRowCount(m_nMaxData);

					for (ii = 0; ii < m_grid->GetRowCount(); ii++)
						m_grid->SetRowHeight(ii, rowH);
				}
				break;
			}
		}
		break;
	default:
		return CDialog::OnNotify(wParam, lParam, pResult);
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}