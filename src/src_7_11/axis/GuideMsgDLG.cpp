// GuideMsgDLG.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "GuideMsgDLG.h"
#include "../h/axisvar.h"
#include "MainFrm.h"

#include "fxGridEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MENU_ITEM		1000
#define USERID_GRID_MAIN	2001

const	int rowH   = 18;
const	int rowCol = 4;

// 그리드 
#define	clrGridboder	29
#define	clrFixBk	2
#define	clrFixFg	58
#define clrwhite	27
#define clrblack	0

const UINT maskHdr  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;
const UINT maskItem = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

struct	_gridHdr {
	char*	text;		// fixed text
	UINT	width;		// fixed width
	UINT	attr;		// fixed attribute
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr
};

struct	_gridHdr gridHdr[] = {
	{"화면번호",	65,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 0
	{"구분",	50,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 1
	{"메시지",	184,	GVAT_TEXT,	GVFM_LEFT,	GVAT_TEXT},			// 2
	{"시간",	90,	GVAT_TEXT,	GVFM_CENTER,	GVAT_TEXT},			// 3
};

/////////////////////////////////////////////////////////////////////////////
// CGuideMsgDLG dialog

//#define MAXROW	20

CGuideMsgDLG::CGuideMsgDLG(CWnd* pParent /*=NULL*/, CString home)
	: CDialog(CGuideMsgDLG::IDD, pParent)
{
	m_grid = NULL;
	m_home = home;
}

CGuideMsgDLG::~CGuideMsgDLG()
{
	if (m_grid)	delete m_grid;
}

void CGuideMsgDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGuideMsgDLG)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGuideMsgDLG, CDialog)
	//{{AFX_MSG_MAP(CGuideMsgDLG)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGuideMsgDLG message handlers

BOOL CGuideMsgDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_nMaxData = 101;

	initGrid();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGuideMsgDLG::InsertMsg(CString msg, int level)
{
	CString ScreenNum, data, SubLevel;
	int pos;
	COLORREF color;

	switch(level)
	{
	case 1:
		SubLevel = "정상";
		color = RGB(0, 0, 0);
		break;
	case 2:
		SubLevel = "에러";
		color = RGB(255, 0, 0);
		break;
	case 3:
		SubLevel = "정상";
		color = RGB(0, 0, 0);
		break;
	default:
		SubLevel = "정상";
		color = RGB(0, 0, 0);
		break;
	}

	if (msg.Left(1) == "{")		// SBC
	{
		SubLevel = "정상";
		color = RGB(0, 0, 0);
	}
	else if (msg.Left(1) == "}")
	{
		SubLevel = "거부";
		color = RGB(255, 0, 0);
	}
	
	
	pos = msg.Find("\t");

	ScreenNum = msg.Left(pos);
	if (ScreenNum.Left(1) == "{" || ScreenNum.Left(1) == "}")
		ScreenNum = ScreenNum.Mid(1);
	data = msg.Mid(pos+1);
	
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString whour, text;
	
	whour = (st.wHour < 12) ? "오전" : "오후";

	if(st.wHour > 12)
		st.wHour %= 12;
	
	text.Format("%s %02d:%02d:%02d", whour, st.wHour, st.wMinute, st.wSecond);

	insertRow(1);
	
	m_grid->SetItemText(1, 0, ScreenNum);
	m_grid->SetItemText(1, 1, SubLevel);
	m_grid->SetItemText(1, 2, data);
	m_grid->SetItemText(1, 3, text);

	for (int ii = 0; ii < 4; ii++)
		m_grid->SetItemFgColor(1,ii, color);

	if (m_nMaxData < m_grid->GetRowCount())
		m_grid->DeleteRow(m_grid->GetRowCount() - 1);

	if (level == 2 || msg.Left(1) == "}") ShowWindow(SW_SHOW);
}

void CGuideMsgDLG::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
	CDialog::OnClose();
}

void CGuideMsgDLG::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	m_GridFont.DeleteObject();
	((CMainFrame *)AfxGetMainWnd())->m_GuideDLG = NULL;
	
	delete this;

	CDialog::PostNcDestroy();
}

void CGuideMsgDLG::OnCancel() 
{
	// TODO: Add extra cleanup here

	ShowWindow(SW_HIDE);
	
	//CDialog::OnCancel();
}

void CGuideMsgDLG::OnSize(UINT nType, int cx, int cy) 
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

void CGuideMsgDLG::RedrawCtrl(bool bResize)
{
	if (m_grid != NULL)
	{
		CRect tempRt;
		GetClientRect(tempRt);

		int gap = GetSystemMetrics(SM_CYVSCROLL);

		m_grid->SetColumnWidth(0, 65);
		m_grid->SetColumnWidth(1, 50);
		m_grid->SetColumnWidth(2, tempRt.Width() - 65 - 50 - 90 - gap);
		m_grid->SetColumnWidth(3, 90);

		m_grid->SetWindowPos(NULL, tempRt.left, tempRt.top, tempRt.Width(), tempRt.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
	}
}

void CGuideMsgDLG::initGrid()
{
	LOGFONT logfont;
	int ii = 0;
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
		gvitem.text   = gridHdr[ii].text;
		gvitem.attr   = gridHdr[ii].attr;
		gvitem.fgcol  = GetColor(clrFixFg);
		gvitem.bkcol  = GetColor(clrFixBk);
		gvitem.format = GVFM_CENTER;

		CopyMemory(&gvitem.font, &logfont, sizeof(LOGFONT));

		m_grid->SetItem(&gvitem);
		m_grid->SetColumnWidth(ii, gridHdr[ii].width);
	}

	for (ii = 0; ii < rowCol; ii++)
	{
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= gridHdr[ii].format;
		gvitem.attr  = gridHdr[ii].attrx;
		gvitem.fgcol = GetColor(clrFixFg);
		gvitem.bkcol = GetColor(clrwhite);

		CopyMemory(&gvitem.font, &logfont, sizeof(LOGFONT));

		m_grid->SetColumnItems(ii, &gvitem);
	}

	for (ii = 0; ii < m_grid->GetRowCount(); ii++)
		m_grid->SetRowHeight(ii, rowH);

	CRect tempRt;
	GetClientRect(tempRt);

	int gap = GetSystemMetrics(SM_CYVSCROLL);

	m_grid->SetColumnWidth(0, 65);
	m_grid->SetColumnWidth(1, 50);
	m_grid->SetColumnWidth(2, tempRt.Width() - 65 - 50 - 90 - gap);
	m_grid->SetColumnWidth(3, 90);

	m_grid->SetWindowPos(NULL, tempRt.left, tempRt.top, tempRt.Width(), tempRt.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);

	m_grid->SetGridColor(GetColor(clrGridboder));
	m_grid->SetGridFocusLine(FALSE, TRUE);
	m_grid->SetGridScroll(GVSC_BOTH);
	m_grid->Adjust();
}

void CGuideMsgDLG::insertRow(int row)
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
		gvitem.format= gridHdr[ii].format;
		gvitem.attr  = gridHdr[ii].attrx;
		gvitem.fgcol = GetColor(clrFixFg);
		gvitem.bkcol = GetColor(clrwhite);

		CopyMemory(&gvitem.font, &logfont, sizeof(LOGFONT));	
		m_grid->SetItem(&gvitem);
	}

	m_grid->SetRowHeight(row, rowH);
	m_grid->Adjust();
}

COLORREF CGuideMsgDLG::GetColor(int index)
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

BOOL CGuideMsgDLG::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
