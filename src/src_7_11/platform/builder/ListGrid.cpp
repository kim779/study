// ListGrid.cpp : implementation file
//

#include "stdafx.h"
#include "axiswork.h"
#include "ListGrid.h"
#include "mainvar.h"
#include "awCommon/CellItem.h"
#include "awDlg/CellEdit.h"
#include "awDlg/attrDlg.h"
#include "awDlg/colorDlg.h"
#include "awDlg/GridColumnDlg.h"
#include "awDlg/mapOption.h"
#include "awDlg/cbItem.h"
#include "awDlg/publicSetDlg.h"
#include "awDlg/ControlDlg.h"
#include "awDlg/UTabPage.h"
#include "awDlg/InputKeyDlg.h"
#include "awDlg/coRGB.h"
#include "awDlg/format.h"

#include "keywords.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	WM_APPLYMULTISEL	WM_USER + 2
#define	lastRows	m_GridAttr.rows - m_StartPnt.y
#define	allCols		m_writeInfo.GetSize()

#define	CS_DEFAULT	0
#define	CS_IBEAM	1
#define	CS_SIZEWE	2
#define	CS_DROPMOVE	3
#define	CS_NODROPMOVE	4

#define CLR_MULTISEL	RGB(140, 230, 140)

struct _field {
	int num;
	char *formName;
} fieldINFO[] = {
	// Form
	{0,	"name"},
	{1,	"size"},
	{2,	"type"},
	{3,	"iok"},
	{4,	"Hkeys"},
	{5,	"Lkeys"},
	{6,	"Akeys"},
	{7,	"left"},
	{8,	"top"},
	{9,	"width"},
	{10,	"height"},
	{11,	"align"},
	{12,	"borders"},
	{13,	"index"},
	{14,	"fstyle"},
	{15,	"fpoint"},
	{16,	"fname"},
	{17,	"tRGB"},
	{18,	"pRGB"},
	{19,	"bRGB"},
	{20,	"attr"},
	{21,	"attr2"},
	{22,	"properties"},
	{23,	"checked"},
	{24,	"isHint"},
	{25,	"onHint"},
	{26,	"isEdit"},
	{27,	"onEdit"},
	{28,	"HcoRGB"},
	{29,	"LcoRGB"},
	{30,	"vals0"},
	{31,	"vals1"},
	{32,	"vals2"},
	{33,	"vals3"},
	{34,	"auxs"},
	{35,	"str"},
	{36,	"initC"},
	{37,	"refs"},
	{38,	"offs"},
	{39,	"table"},
	{40,	"rows"},
	{41,	"cols"},
	{42,	"str2"},
	{43,	"stRGB"},
	{44,	"spRGB"},
	{45,	"tabHead"},
	{46,	"tabData"},
	{47,	"tabVisible"},
	{48,	"tableHead"},
	{49,	"editS"},
	{50,	"alignImg"},
	{51,	"fixSize"},
	// Cell (grid column, table row)
	{100,	"cellname"},
	{101,	"celltype"},
	{102,	"celliok"},
	{103,	"cellkind"},
	{104,	"cellsize"},
	{105,	"cellwidth"},
	{106,	"cellhead"},
	{107,	"cellalignH"},
	{108,	"cellalignC"},
	{109,	"cellhpRGB"},
	{110,	"cellhtRGB"},
	{111,	"cellpRGB"},
	{112,	"celltRGB"},
	{113,	"cellvals"},
	{114,	"cellstr"},
	{115,	"cellauxs"},
	{116,	"cellrefs"},
	{117,	"cellproperties"},
	{118,	"cellattr"},
	{119,	"allcellwidth"},
	{120,	"cellstr2"},
	{121,	"celldat"},
	{122,	"cellkeys"},
	{123,	"cellvals2"},
	{124,	"cellalignI"},
	{125,	"cellstr3"},
	{126,	"celltxt"},
	{127,	"cellisHint"},
	{128,	"cellonHint"},
	{129,	"cellattr2"},
	// map 
	{200,	"mapcaption"},
	{201,	"mapheight"},
	{202,	"mapwidth"},
	{203,	"mapfname"},
	{204,	"mapfpoint"},
	{205,	"mapfstyle"},
	{206,	"mapvers"},
	{207,	"mapname"},
	{208,	"mapdesc"},
	{209,	"maptrxC"},
	{210,	"maptrxH"},
	{211,	"maptoPath"},
	{212,	"mapmapM"},
	{213,	"maphelpN"},
	{214,	"maprepeatV"},
	{215,	"mapwaitV"},
	{216,	"mapimageK"},
	{217,	"mapimageV"},
	{218,	"mapGroup"},
	{219,	"maptype"},
	{220,	"repeatV"},
	{221,	"publicR"},
	{222,	"mapBkClr"}
};
/////////////////////////////////////////////////////////////////////////////
// CListGrid

CListGrid::CListGrid()
{
	m_pFormItem   = NULL;
	m_pEdit       = NULL;
	m_pCombo      = NULL;
	m_pButton     = NULL;
	m_pBitmap     = NULL;
	m_bInvalidate = true;
	m_bMove       = false;
	m_StartPnt    = CPoint(0, 0);
	m_SelectedPnt = CPoint(-1, -1);
	m_InputPnt    = CPoint(0, 0);

	m_writeInfo.RemoveAll();

	m_GridAttr.headInfo  = 0;
	m_GridAttr.CellSz    = CSize(100, 16);
	m_GridAttr.HeadSz    = CSize(80, 16);
	m_GridAttr.lineColor = RGB(190, 190, 190);
	m_GridAttr.multiCol  = false;
	m_GridAttr.cols      = 0;
	m_GridAttr.rows      = 0;

	LOGFONT lf = setFont(9);
	m_font.CreatePointFontIndirect(&lf);

	for (int nCnt = 0; nCnt < sizeof(fieldINFO)/sizeof(_field); nCnt++)
		m_fieldMap.SetAt(fieldINFO[nCnt].formName, (void*)fieldINFO[nCnt].num);

	m_ctlBkColor  = ::GetSysColor(COLOR_WINDOW);
	m_ctlBrush.DeleteObject();
	m_ctlBrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));

	m_proMap.RemoveAll();

	m_NoDropMoveCursor = AfxGetApp()->LoadCursor(IDR_NO_DROPMOVE);
	VERIFY(m_NoDropMoveCursor);
	m_DropMoveCursor = AfxGetApp()->LoadCursor(IDR_DROPMOVE);
	VERIFY(m_DropMoveCursor);

	m_bDrag    = false;
	m_nDragCol = -2;
	m_pSepWnd  = NULL;
}

CListGrid::~CListGrid()
{
	m_fieldMap.RemoveAll();
	m_multiSelMap.RemoveAll();
	if (m_pBitmap)
		delete m_pBitmap;
	if (m_pEdit)
	{
		m_pEdit->DestroyWindow();
		delete m_pEdit;
	}
	if (m_pCombo)
	{
		m_pCombo->DestroyWindow();
		delete m_pCombo;
	}
	if (m_pButton)
	{
		m_pButton->DestroyWindow();
		delete m_pButton;
	}
	if (m_pFormItem)
		delete m_pFormItem;
	
	POSITION pos = m_proMap.GetStartPosition();
	while (pos)
	{
		int key;
		CProArr *value;

		m_proMap.GetNextAssoc(pos, key, value);
		delete value;
	}

	m_proMap.RemoveAll();
}


BEGIN_MESSAGE_MAP(CListGrid, CWnd)
	//{{AFX_MSG_MAP(CListGrid)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(ID_EDITCTL, OnChange)
	ON_CBN_SELENDOK(ID_COMBOCTL, OnSelEndOk)
	ON_BN_CLICKED(ID_BUTTONCTL, OnBtnClick)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 1, OnEraseCtrl)
	ON_MESSAGE(WM_APPLYMULTISEL, OnMultiSel)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CListGrid message handlers


void CListGrid::DrawGridData(CDC *pDC)
{
	long col = 0, row = 0;
	CRect rt = m_ClientRt;

	for (col = 0; col < allCols; col++)
	{
		CString tmp = (col + m_StartPnt.x < allCols ? m_writeInfo.GetAt(col + m_StartPnt.x) : ""), temp;
		if (col + m_StartPnt.x >= allCols)
		{
			CRect rc;
			rc.SetRect(m_GridAttr.HeadSz.cx + col * m_GridAttr.CellSz.cx, 
				0, m_ClientRt.right, m_ClientRt.bottom);
			DrawCellData(pDC, rc, "");
			break;
		}
		
		temp = Parser(tmp, "\t");
		
		for (row = 0; row < m_StartPnt.y; row++)
			Parser(tmp, "\t");
		for (row = 0; row < lastRows; row++)
		{
			temp = Parser(tmp, "\t");
			if (!DrawCellData(pDC, col, row, getData(col + m_StartPnt.x, row + m_StartPnt.y)))
				break;
		}
	}
}

void CListGrid::DrawHead(CDC *pDC)
{
	int ii = 0;
	if (m_GridAttr.multiCol)
	{
		for (int col = 0; col <= allCols; col++)
		{
			CRect cellRt;
			CString data;
			int value;

			if (col == 0)
			{
				data="*";
				cellRt.SetRect(0, 0, m_GridAttr.HeadSz.cx, m_GridAttr.CellSz.cy);
			}
			else
			{
				data.Format("%d", col + m_StartPnt.x);
				cellRt.SetRect(m_GridAttr.HeadSz.cx + (col - 1) * m_GridAttr.CellSz.cx, 0, m_GridAttr.HeadSz.cx + col * m_GridAttr.CellSz.cx, m_GridAttr.CellSz.cy);
			}

			if (m_SelectedPnt.y == -1 && col > 0 && m_SelectedPnt.x == col + m_StartPnt.x - 1)
				DrawCellData(pDC, cellRt, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT), DT_CENTER);
			else if (m_multiSelMap.Lookup(col + m_StartPnt.x - 1, value))
				DrawCellData(pDC, cellRt, data, CLR_MULTISEL, RGB(0,0,0), DT_CENTER);
			else
				DrawCellData(pDC, cellRt, data, RGB(255, 255, 255), RGB(0,0,0), DT_CENTER);
		}
	}
	for (int row = (m_GridAttr.multiCol?1:0); row < lastRows + (m_GridAttr.multiCol?1:0); row++)
	{
		CRect cellRt;
		CString data;
		cellRt.SetRect(0, row * m_GridAttr.CellSz.cy,
			m_GridAttr.HeadSz.cx, (row + 1) * m_GridAttr.CellSz.cy);
		data = getHeadCaption(ii++ + m_StartPnt.y);
		if (!data.IsEmpty())
		{
			if (m_SelectedPnt.y + (m_GridAttr.multiCol?1:0) == row + m_StartPnt.y)
				DrawCellData(pDC, cellRt, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT));
			else
				DrawCellData(pDC, cellRt, data);
		}
	}
}

bool CListGrid::DrawCellData(CDC *pDC, int col, int row, CString data) // col, row 상대위치
{
	CRect cellRt = m_ClientRt;
	COLORREF bkColor = RGB(255, 255, 255);

	if (m_GridAttr.multiCol)
		cellRt.SetRect(m_GridAttr.HeadSz.cx + col * m_GridAttr.CellSz.cx, 
			m_GridAttr.HeadSz.cy + row * m_GridAttr.CellSz.cy,
			m_GridAttr.HeadSz.cx + (col + 1) * m_GridAttr.CellSz.cx, 
			m_GridAttr.HeadSz.cy + (row + 1) * m_GridAttr.CellSz.cy);
	else
		cellRt.SetRect(m_GridAttr.HeadSz.cx, row * m_GridAttr.CellSz.cy,
			cellRt.right, (row + 1) * m_GridAttr.CellSz.cy);

	if (cellRt.top > m_ClientRt.Height())
		return false;

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkMode(TRANSPARENT);
	if (atoi(getValue2(row + m_StartPnt.y)) == 1002)
	{
		bkColor = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, 
						atoi(getData(col + m_StartPnt.x, row + m_StartPnt.y)));
		pDC->FillSolidRect(cellRt, bkColor);
	}
	else
		pDC->FillSolidRect(cellRt, RGB(255, 255, 255));
	cellRt.DeflateRect(1, 1, 0, 0);
	if (getType(row + m_StartPnt.y) == 4)
	{
		pDC->DrawFrameControl(cellRt, DFC_BUTTON, DFCS_BUTTONPUSH);
		pDC->DrawText("...", cellRt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		if (atoi(getValue2(row + m_StartPnt.y)) == 1002)
		{
			if ((GetRValue(bkColor) + GetGValue(bkColor) + GetBValue(bkColor)) / 3 >= 192)
				pDC->SetTextColor(RGB(0, 0, 0));
			else
				pDC->SetTextColor(RGB(255, 255, 255));
		}
		pDC->DrawText(data, cellRt, DT_LEFT);
	}
	cellRt.InflateRect(1, 1, 1, 1);
	pDC->Draw3dRect(cellRt, m_GridAttr.lineColor, m_GridAttr.lineColor);
	return true;
}

void CListGrid::DrawCellData(CDC *pDC, CRect rc, CString data, COLORREF bkcolor, COLORREF frcolor, DWORD dwStyle)
{
	pDC->SetTextColor(frcolor);
	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(rc, bkcolor);
	rc.DeflateRect(1, 0, 0, 0);
	pDC->DrawText(data, rc, dwStyle | DT_VCENTER | DT_SINGLELINE);
	rc.InflateRect(1, 0, 1, 1);
	pDC->Draw3dRect(rc, m_GridAttr.lineColor, m_GridAttr.lineColor);
}

void CListGrid::DrawBitmap(CDC *pDC)
{
	CDC	memDC;
	CBitmap*	pOldBitmap;

	calScrollSz();

	if (m_pBitmap)
		delete m_pBitmap;

	m_pBitmap = new CBitmap();

	memDC.CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, m_ClientRt.Width(), m_ClientRt.Height());
	pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);

	CFont	font, *pOldFont;
	// set font
	LOGFONT lf = setFont(9);
	font.CreatePointFontIndirect(&lf, &memDC);
	pOldFont = (CFont*)memDC.SelectObject(&font);

	memDC.FillSolidRect(m_ClientRt, RGB(255, 255, 255));
	DrawHead(&memDC);
	DrawGridData(&memDC);

	memDC.SelectObject(pOldFont);
	font.DeleteObject();
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	m_bInvalidate = false;
}

void CListGrid::DrawBitmap2(int col, int row, CRect rc)
{
	CDC *pDC = GetDC(), memDC;
	CBitmap*	pOldBitmap;

	memDC.CreateCompatibleDC(pDC);
	pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);

	CFont	font, *pOldFont;
	// set font
	LOGFONT lf = setFont(9);
	font.CreatePointFontIndirect(&lf, &memDC);
	pOldFont = (CFont*)memDC.SelectObject(&font);

	if (col >= 0 && row >= 0)
		DrawCellData(&memDC, col, row, getData(col + m_StartPnt.x, row + m_StartPnt.y));
	else
	{
		CString data;
		int value;
		if (m_GridAttr.multiCol && col >= 0)
		{
			data.Format("%d", col + m_StartPnt.x + 1);
			
			if (m_SelectedPnt.y == -1 && m_SelectedPnt.x == col + m_StartPnt.x)
				DrawCellData(&memDC, rc, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT), DT_CENTER);
			else if (m_multiSelMap.Lookup(col + m_StartPnt.x, value))
				DrawCellData(&memDC, rc, data, CLR_MULTISEL, RGB(0,0,0), DT_CENTER);
			else
				DrawCellData(&memDC, rc, data, RGB(255, 255, 255), RGB(0,0,0), DT_CENTER);
		}
		
		if (row >= 0)
		{
			data = getHeadCaption(row + m_StartPnt.y);
			if (!data.IsEmpty())
			{
				if (m_SelectedPnt.y == row + m_StartPnt.y)
					DrawCellData(&memDC, rc, data, GetSysColor(COLOR_HIGHLIGHT), GetSysColor(COLOR_HIGHLIGHTTEXT));
				else
					DrawCellData(&memDC, rc, data);
			}
		}
	}

	memDC.SelectObject(pOldFont);
	font.DeleteObject();
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	ReleaseDC(pDC);
}

//
// Window Messages
//

void CListGrid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rt = m_ClientRt;

	if (m_bInvalidate)
		DrawBitmap(&dc);
	
	if (m_pBitmap)
	{
		CDC		memDC;
		CBitmap*	pOldBitmap = NULL;
		
		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);
		
		dc.BitBlt(0, 0, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
	if (m_bMove)
	{
		rt.left = m_GridAttr.HeadSz.cx;
		rt.right = rt.left + 1;
		dc.FillSolidRect(rt, RGB(0, 0, 0));
	}

	// Do not call CWnd::OnPaint() for painting messages
}

void CListGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	GetClientRect(&m_ClientRt);
	
	EraseCtrls(FALSE);	// SDI를 위해 FALSE
	m_bInvalidate = true;
	//Invalidate();
}

void CListGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_GridAttr.rows)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}
	CPoint oldSelected = m_SelectedPnt;

	EraseCtrls();
	CRect validRt;

	m_nDragCol = -2;

	GetClientRect(&validRt);
	validRt.left = m_GridAttr.HeadSz.cx - 1;
	validRt.right = m_GridAttr.HeadSz.cx + 1;
	if (validRt.PtInRect(point))
	{
		SetCapture();
		m_bMove = true;
		Invalidate();
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}
	if (m_GridAttr.multiCol)
		validRt.SetRect(
			0, 
			0, 
			m_GridAttr.HeadSz.cx + m_GridAttr.CellSz.cx * allCols,
			m_GridAttr.HeadSz.cy + m_GridAttr.CellSz.cy * lastRows);
	else
		validRt.SetRect(
			0, 
			0, 
			m_ClientRt.Width(),
			m_GridAttr.CellSz.cy * lastRows);
	if (validRt.PtInRect(point))
	{
		if (point.x < m_GridAttr.HeadSz.cx)
			m_SelectedPnt.x = -1;
		else 
		{
			m_SelectedPnt.x = 0;
			if (m_GridAttr.multiCol)
				while((m_SelectedPnt.x + 1) * m_GridAttr.CellSz.cx + m_GridAttr.HeadSz.cx < point.x)
					m_SelectedPnt.x++;
			m_SelectedPnt.x += m_StartPnt.x;
			if (m_SelectedPnt.x >= allCols)
				m_SelectedPnt.x = -1;
		}
		
		if (m_GridAttr.multiCol && point.y < m_GridAttr.HeadSz.cy)
			m_SelectedPnt.y = -1;
		else 
		{
			m_SelectedPnt.y = 0;
			if (m_GridAttr.multiCol)
				while((m_SelectedPnt.y + 1) * m_GridAttr.CellSz.cy + m_GridAttr.HeadSz.cy < point.y)
					m_SelectedPnt.y++;
			else
				while((m_SelectedPnt.y + 1) * m_GridAttr.CellSz.cy < point.y)
					m_SelectedPnt.y++;
			m_SelectedPnt.y += m_StartPnt.y;
		}
		if (m_GridAttr.multiCol)
		{
			if (m_SelectedPnt == CPoint(-1, -1))
			{
				if (m_multiSelMap.GetCount())
					m_multiSelMap.RemoveAll();
				else
				{
					for (int nSel = 0; nSel < m_GridAttr.cols; nSel++)
						m_multiSelMap.SetAt(nSel, 1);
				}
				m_bInvalidate = true;
				Invalidate();
				return;
			}
			InvalidateRect2(oldSelected);
			InvalidateRect2(CPoint(m_SelectedPnt.x, -1));
			// column drag
			if (m_SelectedPnt.x != -1 && m_SelectedPnt.y == -1)
			{
				SetCapture();
				m_dragPnt = point;
				m_nDragCol = m_SelectedPnt.x;
				m_bDrag = false;
			}
		}

		InvalidateRect2(CPoint(-1, oldSelected.y));
		InvalidateRect2(CPoint(-1, m_SelectedPnt.y));
		InvalidateRect2(m_SelectedPnt);
		if (m_SelectedPnt.x >= 0)
			SetInputState();
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CListGrid::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_GridAttr.rows)
	{
		CWnd::OnRButtonDown(nFlags, point);
		return;
	}
	if (m_GridAttr.multiCol && point.x < m_GridAttr.HeadSz.cx && m_GridAttr.cols > 1)
	{
		OnLButtonDown(nFlags, point);
		CMenu	menu;
		
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, 1, "Common apply");
		
		ClientToScreen(&point);
		int cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD,
				point.x, point.y, this);
		switch (cmd)
		{
		case 1:	
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
			for (int ii = 1; ii < m_GridAttr.cols; ii++)
				SetLikeCell(ii);
			
			break;
		}
		
	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CListGrid::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_nDragCol >= 0 && !m_bMove && !m_bDrag && (nFlags & MK_LBUTTON) && abs((m_dragPnt.x - point.x) * (m_dragPnt.y - point.y)) > 4)
	{
		m_bDrag = true;
		m_nCursor = CS_DROPMOVE;
		::SetCursor(m_DropMoveCursor);
		if (m_pSepWnd)
		{
			m_pSepWnd->DestroyWindow();
			delete m_pSepWnd;
		}
		m_pSepWnd = new CSepWnd();
		m_pSepWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 6, m_GridAttr.HeadSz.cy + 4), GetParent(), 3);
		CWnd::OnMouseMove(nFlags, point);
		return;
	}
	
	if (m_bDrag)
	{
		int nCol = 0;
		int cx = m_GridAttr.HeadSz.cx, nTarget = -2;
		for (nCol = 0; nCol < m_GridAttr.cols; nCol++)
		{
			if (point.x <= cx)
				break;
			cx += m_GridAttr.CellSz.cx;
		}
		if (nCol > 0)
		{
			nTarget = m_StartPnt.x + nCol - 1;
			
			if (m_pSepWnd)
			{
				CRect sepRc;
				sepRc.SetRect(cx - m_GridAttr.CellSz.cx, 0, cx, m_GridAttr.HeadSz.cy);
				ClientToScreen(&sepRc);
				GetParent()->ScreenToClient(&sepRc);
				m_pSepWnd->ShowWindow(SW_SHOW);
				if (sepRc.Width() / 2 + sepRc.left < point.x)
					m_pSepWnd->MoveWnd(sepRc.right - 3, sepRc.top - 3);
				else
					m_pSepWnd->MoveWnd(sepRc.left - 3, sepRc.top - 3);
			}
			m_nCursor = CS_DROPMOVE;
			::SetCursor(m_DropMoveCursor);
			CWnd::OnMouseMove(nFlags, point);
			return;
		}
		if (m_pSepWnd)
			m_pSepWnd->ShowWindow(SW_HIDE);
		m_nCursor = CS_NODROPMOVE;
		::SetCursor(m_NoDropMoveCursor);
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	if (m_bMove)
	{
		m_GridAttr.HeadSz.cx = point.x;
		Invalidate();
		CWnd::OnMouseMove(nFlags, point);
		return;
	}
	if (m_GridAttr.rows)
	{
		CRect rc;
		m_nCursor = CS_DEFAULT;
		if (m_SelectedPnt.y >= 0)
		{
			rc.top = (m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + (m_GridAttr.multiCol?m_GridAttr.HeadSz.cy:0);
			rc.bottom = (m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy + (m_GridAttr.multiCol?m_GridAttr.HeadSz.cy:0);
			if (m_GridAttr.multiCol)
			{
				rc.left = m_GridAttr.HeadSz.cx;
				rc.right = m_GridAttr.HeadSz.cx + m_GridAttr.CellSz.cx * (allCols - m_StartPnt.x);
			}
			else
			{
				rc.left = m_GridAttr.HeadSz.cx;
				rc.right = m_ClientRt.right;
			}
			if (rc.PtInRect(point))
			{
				if (getType(m_SelectedPnt.y) == 1)
					m_nCursor = CS_IBEAM;
			}
		}
		GetClientRect(&rc);
		rc.left = m_GridAttr.HeadSz.cx - 1;
		rc.right = m_GridAttr.HeadSz.cx + 1;
		if (rc.PtInRect(point))
			m_nCursor = CS_SIZEWE;
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CListGrid::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_nDragCol >= 0)
		::ReleaseCapture();

	if (m_bDrag)
	{
		CRect rc;
		int	nTarget = -2;
		GetClientRect(&rc);
		m_nCursor = CS_DEFAULT;
		if (m_pSepWnd)
		{
			m_pSepWnd->DestroyWindow();
			delete m_pSepWnd;
			m_pSepWnd = NULL;
		}
		if (m_GridAttr.HeadSz.cy >= point.y && point.x <= rc.Width() && point.x >= m_GridAttr.HeadSz.cx)
		{
			int nCol = 0;
			int cx = m_GridAttr.HeadSz.cx + m_GridAttr.CellSz.cx / 2;
			for (nCol = 0; nCol < m_GridAttr.cols; nCol++)
			{
				if (point.x <= cx)
					break;
				cx += m_GridAttr.CellSz.cx;
			}
			nTarget = m_StartPnt.x + nCol;
			if (m_nDragCol + 1 != nTarget)
			{
				struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
				CCellItem cellItem(repR);
				cellItem.moveCell(m_nDragCol, nTarget);
				
				CString  str = m_writeInfo.GetAt(m_nDragCol);
				
				m_writeInfo.RemoveAt(m_nDragCol);
				m_writeInfo.InsertAt((m_nDragCol < nTarget? nTarget - 1: nTarget), str);
				
				m_bInvalidate = true;
				Invalidate();
			}
		}
		m_bDrag = false;
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}
	if (m_bMove)
	{
		ReleaseCapture();
		m_bInvalidate = true;
		Invalidate();
		m_bMove = false;
	}
	else
	{
		if (m_SelectedPnt.x == -1 && m_SelectedPnt.y >= 0 && !m_GridAttr.multiCol)
		{
			EraseCtrls();
			m_SelectedPnt.x = 0;
			m_bKeyDown = true;
			SetInputState();
		}
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CListGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SCROLLINFO info;
	GetScrollInfo(SB_VERT, &info);

	if (nSBCode != SB_THUMBTRACK)
		nPos = info.nPos;

	switch (nSBCode)
	{
	case SB_LINEUP:
		if (nPos == 0)
			nPos = 0;
		else
			nPos--;
		break;
	case	SB_LINEDOWN:		// move 1
		nPos++;
		break;
	case	SB_PAGEUP:
		if (nPos <= 1)
			nPos = 0;
		else
			nPos -= 2;
		break;
	case	SB_PAGEDOWN:		// move nPage
		nPos += 2;
		break;
	case	SB_THUMBTRACK:
		break;
	default:
		return;
		break;		
	}
	if (nPos > (UINT)info.nMax)
		nPos = info.nMax;
	info.nPos = nPos;
	info.fMask = SIF_POS;
	/*m_pVScroll->*/SetScrollInfo(SB_VERT, &info);
	
	m_StartPnt.y = nPos;
	EraseCtrls();
	m_bInvalidate = true;
	Invalidate();
	
	//CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CListGrid::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SCROLLINFO info;
	GetScrollInfo(SB_HORZ, &info);

	if (nSBCode != SB_THUMBTRACK)
		nPos = info.nPos;

	switch (nSBCode)
	{
	case SB_LINEUP:
		if (nPos == 0)
			nPos = 0;
		else
			nPos--;
		break;
	case	SB_LINEDOWN:		// move 1
		nPos++;
		break;
	case	SB_PAGEUP:
		if (nPos <= 1)
			nPos = 0;
		else
			nPos -= 2;
		break;
	case	SB_PAGEDOWN:		// move nPage
		nPos += 2;
		break;
	case	SB_THUMBTRACK:
		break;
	default:
		return;
		break;		
	}
	if (nPos > (UINT)info.nMax)
		nPos = info.nMax;
	info.nPos = nPos;
	info.fMask = SIF_POS;
	SetScrollInfo(SB_HORZ, &info);
	
	m_StartPnt.x = nPos;
	EraseCtrls();
	m_bInvalidate = true;
	Invalidate();
	//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CListGrid::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_EDIT)
	{
		switch (pWnd->GetDlgCtrlID())
		{
		case ID_EDITCTL:
			if ((GetRValue(m_ctlBkColor) + GetGValue(m_ctlBkColor) + GetBValue(m_ctlBkColor)) / 3 >= 192)
				pDC->SetTextColor(RGB(0, 0, 0));
			else
				pDC->SetTextColor(RGB(255, 255, 255));
			pDC->SetBkColor(m_ctlBkColor);
			hbr = (HBRUSH) m_ctlBrush;
			break;
		}
	}
	return hbr;
}

//
//	Grid Info
//
CString CListGrid::getHeadCaption(int pos)
{
	if (pos < 0)
		return "";
	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).headCaption;
	}
	return "";
}

int CListGrid::getType(int pos)
{
	if (pos < 0)
		return 0;

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).type;
	}
	return 0;
}

CString CListGrid::getValue1(int pos)
{
	if (pos < 0)
		return "";

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).value1;
	}
	return "";
}

CString CListGrid::getValue2(int pos)
{
	if (pos < 0)
		return "";

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).value2;
	}
	return "";
}

CString CListGrid::getFormM(int pos)
{
	if (pos < 0)
		return "";

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).formM;
	}
	return "";
}

CString CListGrid::getDesc(int pos)
{
	if (pos < 0)
		return "";

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).desc;
	}
	return "";
}

int CListGrid::getAttr(int pos)
{
	if (pos < 0)
		return 0;

	CProArr *proArr;
	if (m_proMap.Lookup(m_GridAttr.headInfo, proArr))
	{
		if (pos < proArr->GetSize())
			return proArr->GetAt(pos).nAttributes;
	}
	return 0;
}

int CListGrid::getVisibleRows()
{
	int nRows = 0;
	while(m_GridAttr.CellSz.cy * nRows < m_ClientRt.Height() - (m_GridAttr.multiCol?m_GridAttr.HeadSz.cy:0))
		nRows++;
	return nRows;
}

int CListGrid::getVisibleCols()
{
	int nCols = 0;
	while(m_GridAttr.CellSz.cx * nCols < m_ClientRt.Width() - m_GridAttr.HeadSz.cx)
		nCols++;
	return nCols;
}

long CListGrid::getGridHeight()
{
	if (m_GridAttr.multiCol)
		return m_GridAttr.HeadSz.cy + m_GridAttr.CellSz.cy * m_GridAttr.rows;
	else
		return m_GridAttr.CellSz.cy * m_GridAttr.rows;
}

long CListGrid::getGridWidth()
{
	return m_GridAttr.HeadSz.cx + m_GridAttr.CellSz.cx * m_GridAttr.cols;
}

//
//	Grid Methods
//
CString CListGrid::getData(int col, int row)
{
	if (col < 0 || row < 0)
		return "";
	CString tmp = (col < allCols ? m_writeInfo.GetAt(col) : ""), temp;

	if (tmp.IsEmpty())
		return "";
	
	temp = Parser(tmp, "\t");
	for (int ii = 0; ii < row; ii++)
		temp = Parser(tmp, "\t");

	return temp;
}

void CListGrid::setData(CString str, long col, long row)
{
	if (col < 0 || row < 0)
		return;
	int pos = -1;
	CString tmp, temp = "", retv = "";
	if (col < allCols)
		tmp = m_writeInfo.GetAt(col);
	else
		return;

	for (int ii = 0; ii < row; ii++)
	{
		temp = Parser(tmp, "\t");
		retv += temp;
		retv += "\t";
	}
	retv += str;
	retv += "\t";
	Parser(tmp, "\t");
	retv += tmp;

	m_writeInfo.SetAt(col, retv);
}

void CListGrid::AddCol(CString str)
{
	m_writeInfo.Add(str);
}

void CListGrid::ResetGrid()
{
	m_GridAttr.headInfo = 0;
	m_GridAttr.cols = 0;
	m_GridAttr.rows = 0;
	m_writeInfo.RemoveAll();
	//m_StartPnt.x = m_StartPnt.y = 0;
	calScrollSz();
}

//
// Utilities
//
CString CListGrid::Parser(CString &srcstr, CString substr)
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

void CListGrid::calScrollSz()
{
	CRect rc = m_ClientRt;
	if (getGridHeight() > rc.Height())
	{
		SCROLLINFO info;
		GetScrollInfo(SB_VERT, &info);
		info.cbSize    = sizeof(SCROLLINFO);
		info.fMask     = SIF_ALL;
		info.nMin      = 0;
		info.nMax      = m_GridAttr.rows - getVisibleRows() + 1;
		info.nPage     = 1;
		info.nTrackPos = 1;
		info.nPos      = m_StartPnt.y;
		SetScrollInfo(SB_VERT, &info);
		ShowScrollBar(SB_VERT);
		SetScrollRange(SB_VERT, 0, m_GridAttr.rows - getVisibleRows() + 1);
	}
	else if (getGridHeight() <= rc.Height())
		ShowScrollBar(SB_VERT, FALSE);

	if (getGridWidth() > rc.Width())
	{
		SCROLLINFO info;
		GetScrollInfo(SB_HORZ, &info);
		info.cbSize    = sizeof(SCROLLINFO);
		info.fMask     = SIF_ALL;
		info.nMin      = 0; 
		info.nMax      = m_GridAttr.cols - getVisibleCols() + 1;
		info.nPage     = 1;
		info.nTrackPos = 1;
		info.nPos      = m_StartPnt.x;
		if (info.nPos > info.nMax)
			info.nPos = info.nMax;
		SetScrollInfo(SB_HORZ, &info);
		ShowScrollBar(SB_HORZ);
		SetScrollRange(SB_HORZ, 0, m_GridAttr.cols - getVisibleCols() + 1);
	}
	else if (getGridWidth() <= rc.Width())
		ShowScrollBar(SB_HORZ, FALSE);

	//if (m_GridAttr.cols >= getVisibleCols() && m_GridAttr.cols - m_StartPnt.x <= getVisibleCols())
	//	m_StartPnt.x = m_GridAttr.cols - getVisibleCols();
}


void CListGrid::SetInputState()
{
	if (!m_GridAttr.multiCol)
		GetParent()->SendMessage(WM_USER, 2, MAKELPARAM(m_SelectedPnt.x, m_SelectedPnt.y));	// description property
	CRect cellRt;
	if (m_GridAttr.multiCol)
		cellRt.SetRect(m_GridAttr.HeadSz.cx + (m_SelectedPnt.x - m_StartPnt.x) * m_GridAttr.CellSz.cx + 1, 
			m_GridAttr.HeadSz.cy + (m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + 1,
			m_GridAttr.HeadSz.cx + (m_SelectedPnt.x - m_StartPnt.x + 1) * m_GridAttr.CellSz.cx, 
			m_GridAttr.HeadSz.cy + (m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy);

	else
		cellRt.SetRect(m_GridAttr.HeadSz.cx + 1,
			(m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + 1,
			m_ClientRt.right,
			(m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy);
	if (m_pCombo || m_pEdit)
		EraseCtrls();

	m_sBeforeVal = getData(m_SelectedPnt.x, m_SelectedPnt.y);
	switch(getType(m_SelectedPnt.y))
	{
	case 1:		// edit
		{ 
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
			CString data = getData(m_SelectedPnt.x, m_SelectedPnt.y);
			int nLimitText = 0;
			DWORD	dwStyle = WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL;
			if (!getFormM(m_SelectedPnt.y).CompareNoCase("mapname") ||
				!getHeadCaption(m_SelectedPnt.y).CompareNoCase("MapName"))
				dwStyle |= ES_UPPERCASE;
			if (!getValue1(m_SelectedPnt.y).IsEmpty() &&
				getValue1(m_SelectedPnt.y).GetAt(0) == '2')
				dwStyle |= ES_NUMBER;

			m_pEdit = new CEdit;
			m_pEdit->Create(dwStyle, cellRt, this, ID_EDITCTL);
			m_pEdit->SetFont(&m_font);
			if (!getValue1(m_SelectedPnt.y).IsEmpty() &&
				getValue1(m_SelectedPnt.y).GetAt(0) == '1')
				m_pEdit->EnableWindow(FALSE);

			// 조건부
			if (!getFormM(m_SelectedPnt.y).CompareNoCase("cellstr2"))
			{
				struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
				CCellItem cellItem(repR);
				if (m_GridAttr.multiCol)
					repR->selected = m_SelectedPnt.x;
				if (cellItem.getCellPoint(repR->selected)->kind != CK_CHECK)
					m_pEdit->EnableWindow(FALSE);
			}

			if (!getFormM(m_SelectedPnt.y).CompareNoCase("celltxt"))
			{
				struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
				CCellItem cellItem(repR);
				if (m_GridAttr.multiCol)
					repR->selected = m_SelectedPnt.x;
				if (cellItem.getCellPoint(repR->selected)->kind != CK_BUTTON)
					m_pEdit->EnableWindow(FALSE);
			}
			
			if (getValue1(m_SelectedPnt.y).GetLength() >= 3)
				nLimitText = atoi(getValue1(m_SelectedPnt.y).Mid(1, 3));

			/*if (!getFormM(m_SelectedPnt.y).CompareNoCase("mapname") ||
				!getHeadCaption(m_SelectedPnt.y).CompareNoCase("MapName"))
					nLimitText = 8;

			else if (!getFormM(m_SelectedPnt.y).Compare("mapdesc"))
				nLimitText = 20;*/
			if (nLimitText)
				m_pEdit->LimitText(nLimitText);
			m_pEdit->SetWindowText(data);
			m_pEdit->SetSel(0, -1);
			if (!m_bKeyDown)
			{
				CPoint pnt;
				GetCursorPos(&pnt);
				m_pEdit->ScreenToClient(&pnt);
				m_pEdit->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKEWPARAM(pnt.x, pnt.y));
			}
			m_pEdit->SetFocus();
			m_InputPnt = m_SelectedPnt;
			m_bKeyDown = false;
		}
		break;
	case 2: // edit combo
		{
			CString data = getData(m_SelectedPnt.x, m_SelectedPnt.y);

			cellRt.DeflateRect(0, 0, 20, 0);
			m_pEdit = new CEdit;
			m_pEdit->Create(WS_CHILD | WS_VISIBLE | ES_LEFT | ES_NOHIDESEL | ES_AUTOHSCROLL, cellRt, this, ID_EDITCTL);
			m_pEdit->SetFont(&m_font);
			if (!getValue1(m_SelectedPnt.y).IsEmpty() &&
				getValue1(m_SelectedPnt.y).GetAt(0) == '1')
				m_pEdit->EnableWindow(FALSE);
			m_pEdit->SetWindowText(data);
			if (m_pEdit->IsWindowEnabled())
				m_pEdit->SetSel(0, -1);
			//m_pEdit->SetFocus();

			m_pButton = new CNFBtn;
			if (cellRt.Width() > 20)
				cellRt.DeflateRect(cellRt.Width(), 0, -20, 0);
			m_pButton->Create(NULL, "...", WS_CHILD | WS_VISIBLE, cellRt, this, ID_BUTTONCTL);
			m_pButton->SetFont(&m_font);
			m_pButton->SetFocus();

			m_InputPnt = m_SelectedPnt;

			if (atoi(getValue2(m_InputPnt.y)) == 1002)
			{
				COLORREF rgbValue = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, 
					atoi(getData(m_InputPnt.x, m_InputPnt.y)));
				m_ctlBkColor  = rgbValue;
				m_ctlBrush.DeleteObject();
				m_ctlBrush.CreateSolidBrush(rgbValue);
			}
			m_pEdit->SetFocus();
		}
		break;
	case 3:		// combo
		{
			CString data = getData(m_SelectedPnt.x, m_SelectedPnt.y), comboStr;
			m_pCombo = new CBtnCombo;
			m_pCombo->setFont(&m_font);
			m_pCombo->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, cellRt, this, ID_COMBOCTL);
			comboStr = getValue1(m_SelectedPnt.y);
			if (comboStr.GetLength() > 0 && comboStr.GetAt(0) == '$')
				comboStr = getReservedVal1(comboStr);
			while (!comboStr.IsEmpty())
			{
				CString str = Parser(comboStr, ";");
				int iData = atoi(Parser(str, "-"));
				m_pCombo->SetItemData(m_pCombo->AddString(str), iData);
			}

			//조건부
			if (!getFormM(m_SelectedPnt.y).CompareNoCase("cellproperties") && 
				!getValue2(m_SelectedPnt.y).CompareNoCase("0x0010;0x0080"))
			{
				struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
				CCellItem cellItem(repR);
				if (m_GridAttr.multiCol)
					repR->selected = m_SelectedPnt.x;
				if (cellItem.getCellPoint(repR->selected)->kind != CK_BUTTON)
					m_pCombo->EnableWindow(FALSE);
			}
			int sel = m_pCombo->m_pCombo->FindStringExact(0, data);
			if (sel < 0)
				sel = 0;
			if (m_pCombo->m_pCombo->GetCount())
				m_pCombo->m_pCombo->SetCurSel(sel);

			m_pCombo->SetFocus();
			m_InputPnt = m_SelectedPnt;
		}
		break;
	case 4:		// only button
		{
			m_pButton = new CNFBtn;
			
			m_pButton->Create(NULL, "...", WS_CHILD | WS_VISIBLE, cellRt, this, ID_BUTTONCTL);
			m_pButton->SetFont(&m_font);
			m_pButton->SetFocus();

			m_InputPnt = m_SelectedPnt;
			const MSG *pmsg;
			pmsg = GetCurrentMessage();
			if (pmsg->message == WM_LBUTTONDOWN)
				OnBtnClick();
		}
		break;
	}
}

void CListGrid::OnChange()
{
}

void CListGrid::EraseCtrls(bool bFocus)
{
	m_ctlBkColor  = ::GetSysColor(COLOR_WINDOW);
	m_ctlBrush.DeleteObject();
	m_ctlBrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));

	if (m_pEdit)
	{
		CString tmp;
		bool	bErr = false;
		m_pEdit->GetWindowText(tmp);
		if ((m_sBeforeVal.IsEmpty() && tmp.IsEmpty()) || 
			(!m_sBeforeVal.IsEmpty() && !m_sBeforeVal.Compare(tmp)) ||
			(!tmp.IsEmpty() && !tmp.Compare(m_sBeforeVal))) // 같으면 적용 안함
		{
			m_pEdit->DestroyWindow();
			delete m_pEdit;
			m_pEdit = NULL;
		}
		else
		{
			if (!(!getFormM(m_InputPnt.y).Compare("name") && tmp.IsEmpty()))
			{
				if (!(!getFormM(m_InputPnt.y).Compare("name") && duplicateSym(tmp)))
				{
					if (!(!getFormM(m_InputPnt.y).Compare("name") && checkReservedWord(tmp)))
						setData(tmp, m_InputPnt.x, m_InputPnt.y);
					else
					{
						AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, SERR_RESERVEDKEYWORD);
						bErr = true;
					}
				}
				else
				{
					AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, SERR_DUPLICATESYMBOL);
					bErr = true;
				}
			}
			else
			{
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, SERR_NOEMPTY);
				bErr = true;
			}
			
			if (!bErr)
				SaveData(m_index, m_InputPnt.x, m_InputPnt.y);
			m_pEdit->DestroyWindow();
			delete m_pEdit;
			m_pEdit = NULL;
			
			if (!getFormM(m_InputPnt.y).Compare("left") ||
				!getFormM(m_InputPnt.y).Compare("width") ||
				!getFormM(m_InputPnt.y).Compare("top") ||
				!getFormM(m_InputPnt.y).Compare("height") ||
				!getFormM(m_InputPnt.y).Compare("mapwidth") ||
				!getFormM(m_InputPnt.y).Compare("mapheight"))
				AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REDRAWCTRL, -1);
			else
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
			
			if (!getFormM(m_InputPnt.y).Compare("name"))
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, MAKELONG(2, m_index));
		}
	}
	if (m_pCombo)
	{
		CString tmp = m_pCombo->GetText();
		if ((m_sBeforeVal.IsEmpty() && tmp.IsEmpty()) || 
			(!m_sBeforeVal.IsEmpty() && !m_sBeforeVal.Compare(tmp)) ||
			(!tmp.IsEmpty() && !tmp.Compare(m_sBeforeVal))) // 같으면 적용 안함
		{
			m_pCombo->DestroyWindow();
			delete m_pCombo;
			m_pCombo = NULL;
		}
		else
		{
			setData(tmp, m_InputPnt.x, m_InputPnt.y);
			SaveData(m_index, m_InputPnt.x, m_InputPnt.y);
			m_pCombo->DestroyWindow();
			delete m_pCombo;
			m_pCombo = NULL;
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
		}
	}
	if (m_pButton)
	{
		m_pButton->DestroyWindow();
		delete m_pButton;
		m_pButton = NULL;
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
	}
	if (bFocus)
		SetFocus();

	InvalidateRect2(m_InputPnt);
}

BOOL CListGrid::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && !(::GetKeyState(VK_CONTROL) & 0x8000))
	{
		if (m_GridAttr.multiCol)
		{
			switch (pMsg->wParam)
			{
			case VK_RETURN:
				m_bKeyDown = true;
				EraseCtrls();
				if (++m_SelectedPnt.y >= m_GridAttr.rows)
					m_SelectedPnt.y = 0;
				
				SetInputState();
				return TRUE;
			case VK_TAB:
				m_bKeyDown = true;
				EraseCtrls();
				if (++m_SelectedPnt.x >= allCols)
					m_SelectedPnt.x = 0;
				
				if (m_SelectedPnt.x + 1 > m_StartPnt.x + getVisibleCols())
				{
					OnHScroll(SB_LINEDOWN, 0, NULL);
					m_bInvalidate = true;
					Invalidate();
				}
				else if (m_SelectedPnt.x < m_StartPnt.x)
				{
					OnHScroll(SB_THUMBTRACK, 0, NULL);
					m_bInvalidate = true;
					Invalidate();
				}
				
				SetInputState();
				return TRUE;
			case VK_DELETE:
				if (m_SelectedPnt.y == -1 && m_SelectedPnt.x >= 0)
				{
					deleteCol(m_SelectedPnt.x);
					return TRUE;
				}
				break;
			case VK_INSERT:
				if (m_SelectedPnt.y == -1 && m_SelectedPnt.x >= 0)
				{
					insertCol(m_SelectedPnt.x);
					return TRUE;
				}
				break;
			}
		}
		else 
		{
			switch (pMsg->wParam)
			{
			case VK_ESCAPE:
			case VK_RETURN:
				EraseCtrls();
				return TRUE;
			case VK_TAB:
				m_bKeyDown = true;
				EraseCtrls();
				if (::GetKeyState(VK_SHIFT) & 0x8000)
				{
					if (--m_SelectedPnt.y < 0)
						m_SelectedPnt.y = m_GridAttr.rows - 1;
				}
				else
				{
					if (++m_SelectedPnt.y >= m_GridAttr.rows)
						m_SelectedPnt.y = 0;
				}
				m_SelectedPnt.x = 0;
				AdjustScroll();
				m_bInvalidate = true;
				Invalidate();
				SetInputState();
				return TRUE;
			}

			if (m_InputPnt.x >= 0 && m_InputPnt.y >= 0 && 
				(getType(m_InputPnt.y) == 2 || getType(m_InputPnt.y) == 3))
			{
				if (getType(m_InputPnt.y) == 2 && m_pEdit && pMsg->hwnd != m_pEdit->m_hWnd && atoi(getValue1(m_InputPnt.y)) != 1)
				{
					m_pEdit->SetFocus();
					m_pEdit->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
					return TRUE;
				}
			}
			/*if ((m_SelectedPnt.x == -1 || (m_SelectedPnt.x == 0 && !m_pEdit)) && m_SelectedPnt.y >= 0 && 
				getType(m_SelectedPnt.y) == 1)
			{
				if (isalnum((char)pMsg->wParam))
				{
					m_SelectedPnt.x = 0;
					AdjustScroll();
					m_bInvalidate = true;
					Invalidate();
					m_bKeyDown = true;
					SetInputState();
					if (m_pEdit)
						m_pEdit->PostMessage(WM_KEYDOWN, (char)pMsg->wParam, (LPARAM)this);
				}
			}*/
			if (m_SelectedPnt.x <= 0 && m_SelectedPnt.y >= 0 && getType(m_SelectedPnt.y) == 3)
			{
				if (isalnum((char)pMsg->wParam))
				{
					m_SelectedPnt.x = 0;
					AdjustScroll();
					m_bInvalidate = true;
					Invalidate();
					SetInputState();
					return TRUE;
				}
			}
		}
	}
	else if (pMsg->message == WM_KEYDOWN && (::GetKeyState(VK_CONTROL) & 0x8000))
	{
		if (m_pEdit)
		{
			switch (pMsg->wParam)
			{
			case 'c':
			case 'C':
				m_pEdit->Copy();
				return TRUE;
			case 'v':
			case 'V':
				m_pEdit->Paste();
				return TRUE;
			case 'x':
			case 'X':
				m_pEdit->Cut();
				return TRUE;
			case 'z':
			case 'Z':
				m_pEdit->Undo();
				return TRUE;
			}
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

LOGFONT CListGrid::setFont(int fsize, CString fname, BOOL bBold, BOOL bItalic)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	//strcpy(lf.lfFaceName, fname);   
	
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf.lfQuality        = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight         = (bBold?FW_BLACK:FW_NORMAL);
	lf.lfItalic         = bItalic;
	return lf;
}

void CListGrid::setHeadInfo(int headNum)
{
	CProArr *proArr;

	m_GridAttr.headInfo = headNum;
	m_GridAttr.rows = 0;
	
	if (m_proMap.Lookup(headNum, proArr))
		m_GridAttr.rows = proArr->GetSize();
	
	if (!m_GridAttr.multiCol)
		AddCol("");
	else
	{
		if (m_index >= 0)
		{
			struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
			CCellItem *cellItem = new CCellItem(repR);
			for (int ii = 0; ii < cellItem->getCellCount(); ii++)
				AddCol("");
			delete cellItem;
			m_GridAttr.cols = allCols;
		}
	}

	if (m_StartPnt.y > m_GridAttr.rows - getVisibleRows() + 1)
		m_StartPnt.y = m_GridAttr.rows - getVisibleRows() + 1;
	if (m_StartPnt.y < 0)
		m_StartPnt.y = 0;

	calScrollSz();

	if (!m_GridAttr.multiCol)
		GetParent()->SendMessage(WM_USER, 2, MAKELPARAM(m_SelectedPnt.x, m_SelectedPnt.y));	// description property
}

void CListGrid::setHeadInfo(int idx, CString headStr)
{
	CString temp;
	CProArr *proArr;
	if (!m_proMap.Lookup(idx, proArr))
		proArr = new CProArr();
	proArr->RemoveAll();
	
	do
	{
		temp = Parser(headStr, "\t");
		if (!temp.IsEmpty())
		{
			CProHead tmp;
			tmp.headCaption	= Parser(temp, "|");
			tmp.type	= atoi(Parser(temp, "|"));
			tmp.value1	= Parser(temp, "|");
			tmp.value2	= Parser(temp, "|");
			tmp.formM	= Parser(temp, "|");
			tmp.desc	= Parser(temp, "|");
			proArr->Add(tmp);
			temp = "^^";
		}
	} while (!temp.IsEmpty());

	m_proMap.SetAt(idx, proArr);
}

CString CListGrid::getReservedVal1(CString str)
{
	CString field = "00$bool\t01$fstyle\t02$alignment\t03$hint\t";
	int pos = field.Find(str);
	if (pos == -1)
		return "";
	switch(atoi(field.Mid(pos - 2, 2)))
	{
	case 0:	//	$bool
		return "0-False;1-True;";
	case 1:	//	$fstyle
		return "0-None;1-Normal;2-Italic;3-Bold;4-Bold/Italic;";
	case 2:	//	$alignment
		return "0-Center;1-Left;2-Right;";
	case 3:	//	$hint
		return "0-None;1-String;2-Reference;";
	}
	return "";
}

void CListGrid::LoadData(int idx, int col, int row)
{
	CString tmp, tmp2;
	int fnum = -1;
	
	for (int ii = 0; ii < m_GridAttr.rows; ii++)
	{
		tmp.Empty();
		if (row >= 0)
			ii = row;

		if (!m_fieldMap.Lookup(getFormM(ii), (void*&)fnum))
			fnum = -1;
		
		for (int iCol = 0; iCol < allCols; iCol++)
		{
			if (col	>= 0)
				iCol = col;

			if (idx >= -1)
			{
				if (m_GridAttr.multiCol)
					tmp = getFormValue(fnum, idx, ii, iCol);
				else
					tmp = getFormValue(fnum, idx, ii);
			}
			else if (idx == -2)
			{
				tmp2.Empty();
				for (int jj = m_pFormItem->getFormCount() - 1; jj >= 0; jj--)
				{
					if (m_pFormItem->getFormPoint(jj)->bCommon)
					{
						if (tmp2.IsEmpty())
						{
							tmp2 = getFormValue(fnum, jj, ii);
							tmp = tmp2;
							if (tmp2.IsEmpty())
								break;
						}
						else
						{
							tmp2 = getFormValue(fnum, jj, ii);
							if (tmp2.Compare(tmp))
							{
								tmp.Empty();
								break;
							}
						}
					}
				}
			}
			
			if (getType(ii) == 3)
			{
				CString comboStr = getValue1(ii);
				int pos = -1;
				if (comboStr.GetLength() > 0 && comboStr.GetAt(0) == '$')
					comboStr = getReservedVal1(comboStr);
				tmp += '-';
				comboStr = ";" + comboStr;
				pos = comboStr.Find(";" + tmp);

				if (pos >= 0)
				{
					pos += tmp.GetLength();
					tmp = comboStr.Mid(pos + 1, comboStr.Find(";", pos) - pos - 1);
				}
				
			}
			setData(tmp, iCol, ii);
			if (col >= 0)
				break;
		}
		if (row >= 0)
			break;
	}
}

void CListGrid::SaveData(int idx, int col, int row, bool bMulti /*= true*/)
{
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	CString tmp, data;
	int fnum = -1;

	for (int ii = 0; ii < m_GridAttr.rows; ii++)
	{
		if (row >= 0)
			ii = row;
		
		if (!m_fieldMap.Lookup(getFormM(ii), (void*&)fnum))
			fnum = -1;

		if (getType(ii) == 3)
		{
			data = "-" + getData(col, ii) + ";";
			CString comboStr = getValue1(ii);
			int pos = -1;
			if (comboStr.GetLength() > 0 && comboStr.GetAt(0) == '$')
				comboStr = getReservedVal1(comboStr);
			pos = comboStr.Find(data);
			if (pos >= 0)
			{
				while(pos >= 0 && comboStr.GetAt(pos) != ';')pos--;
				pos++;
				data = comboStr.Mid(pos, comboStr.Find(";", pos) - pos);
			}
		}
		else
			data = getData(col, ii);
		if (idx >= -1)
			setFormValue(fnum, idx, col, ii, data);
		else if (idx == -2)
		{
			for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
			{
				if (m_pFormItem->getFormPoint(jj)->bCommon)
					setFormValue(fnum, jj, col, ii, data);
			}
		}

		if (row >= 0)
		{
			if (m_GridAttr.multiCol && bMulti)
				SendMessage(WM_APPLYMULTISEL, col);
			break;
		}
	}
}

void CListGrid::Initialize(_mapH* mapH)
{
	if (m_pFormItem)
		delete m_pFormItem;
	m_pFormItem = new CFormItem(mapH);
	m_mapH = mapH;
}

bool CListGrid::setIndex(int idx)
{
	if (m_index == idx)
	{
		return false;
	}
	else
		m_index = idx;
	return true;
}

int CListGrid::_httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR *mstr = _tcsdup(value);
	_tcsupr_s(mstr, strlen(mstr) + 1);
	
	TCHAR *s = mstr;
	int result = 0;
	if (*s == '0' && *(s + 1) == 'X') s += 2;
	bool firsttime = true;
	while (*s != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*s == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = false;
	}
	free(mstr);
	return result;
}

bool CListGrid::checkReservedWord(CString name)
{
	CString sKeywords = szKeywords;
	name.MakeLower();
	if (sKeywords.Find(" " + name + " ") >= 0)
		return true;

	sKeywords = szConstants;

	if (sKeywords.Find(" " + name + " ") >= 0)
		return true;

	return false;
}

void CListGrid::OnSelEndOk()
{
	if (m_pCombo)
	{
		setData(m_pCombo->GetText(), m_InputPnt.x, m_InputPnt.y);
		SaveData(m_index, m_InputPnt.x, m_InputPnt.y);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
	}
}


BOOL CListGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTCLIENT && pWnd->GetSafeHwnd() == this->GetSafeHwnd())
	{
		switch(m_nCursor)	
		{
		case CS_IBEAM:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_IBEAM));
			return TRUE;
		case CS_SIZEWE:
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			return TRUE;
		case CS_DROPMOVE:
			::SetCursor(m_DropMoveCursor);
			return TRUE;
		case CS_NODROPMOVE:
			::SetCursor(m_NoDropMoveCursor);
			return TRUE;
		}
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

CString CListGrid::getFormValue(int fnum, int idx, int row, int col)
{
	CString tmp;
	switch(fnum)
	{
	case 0:	//	name
		tmp = m_pFormItem->getFormStr(idx, "name");
		break;
	case 1:	//	size
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->size);
		break;
	case 2:	//	type
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->type);
		break;
	case 3:	//	iok
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->iok);
		break;
	case 4:	//	Hkeys
		tmp.Format("%d", HIWORD(m_pFormItem->getFormPoint(idx)->keys));
		break;
	case 5:	//	Lkeys
		tmp.Format("%d", LOWORD(m_pFormItem->getFormPoint(idx)->keys));
		break;
	case 6:	//	Akeys
		if (m_pFormItem->getFormPoint(idx)->kind == FM_TABLE && m_pFormItem->getFormPoint(idx)->keys == 0)
		{
			struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(idx)->auxR;
			if (repR)
				m_pFormItem->getFormPoint(idx)->keys = (m_pFormItem->getFormPoint(idx)->bottom - m_pFormItem->getFormPoint(idx)->top) / repR->rows;
		}
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->keys);
		break;
	case 7:	//	left
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->left);
		if (m_pFormItem->getFormPoint(idx)->tabN)
			tmp.Format("%d", m_pFormItem->getFormPoint(idx)->left - 
			m_pFormItem->getFormPoint(idx)->tabN->left);
		break;
	case 8:	//	top
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->top);
		if (m_pFormItem->getFormPoint(idx)->tabN)
			tmp.Format("%d", m_pFormItem->getFormPoint(idx)->top - 
			m_pFormItem->getFormPoint(idx)->tabN->top);
		break;
	case 9:	//	width
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->right - m_pFormItem->getFormPoint(idx)->left);
		break;
	case 10:	//	height
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->bottom - m_pFormItem->getFormPoint(idx)->top);
		break;
	case 11:	//	align
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->alignment);
		break;
	case 12:	//	borders
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->borders);
		break;
	case 13:	//	index
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->index);
		break;
	case 14:	//	fstyle
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->fStyle);
		break;
	case 15:	//	fpoint
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->fPoint);
		if (atoi(tmp) == 0 && m_pFormItem->getFormPoint(idx)->fStyle == FS_NONE)
			tmp.Format("%d", m_mapH->fPoint);
		break;
	case 16:	//	fname
		tmp = m_pFormItem->getFormStr(idx, "fName");
		if (tmp.IsEmpty() && m_pFormItem->getFormPoint(idx)->fStyle == FS_NONE)
			tmp = m_mapH->fName;
		break;
	case 17:	//	tRGB
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->tRGB);
		break;
	case 18:	//	pRGB
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->pRGB);
		break;
	case 19:	//	bRGB
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->bRGB);
		break;
	case 20:	//	attr
		if (_httoi(getValue2(row)) & m_pFormItem->getFormPoint(idx)->attr)
			tmp = "1";
		else
			tmp = "0";
		break;
	case 21:	//	attr2
		if (m_pFormItem->getFormPoint(idx)->kind == FM_EDIT) 
		{
			if (m_pFormItem->getFormPoint(idx)->attr & FA_DATE)
				tmp.Format("%d", (m_pFormItem->getFormPoint(idx)->attr2 & SA_DATE));
			else
				tmp.Format("%d", SA_DATE);
			break;
		}

		if (_httoi(getValue2(row)) & m_pFormItem->getFormPoint(idx)->attr2)
			tmp = "1";
		else
			tmp = "0";
		break;
	case 22:	//	properties
		{
			CString str = getValue2(row);
			int nCnt = 1;
			tmp = "0";
			while(!str.IsEmpty())
			{
				CString str2 = Parser(str, ";");
				if (_httoi(str2) & m_pFormItem->getFormPoint(idx)->properties)
					tmp.Format("%d", nCnt);
				nCnt++;
			}
		}
		break;
	case 23:	//	checked
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->checked);
		break;
	case 24:	//	isHint
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->isHint);
		break;
	case 25:	//	onHint
		tmp = m_pFormItem->getFormStr(idx, "help");
		break;
	case 26:	//	isEidt
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->isEdit);
		break;
	case 27:	//	onEdit
		tmp.Format("%ld", m_pFormItem->getFormPoint(idx)->onEdit);
		break;
	case 28:	//	HcoRGB
		tmp.Format("%d", HIWORD(m_pFormItem->getFormPoint(idx)->onEdit));
		break;
	case 29:	//	LcoRGB
		tmp.Format("%d", LOWORD(m_pFormItem->getFormPoint(idx)->onEdit));
		break;
	case 30:	//	vals0
		tmp.Format("%d", LOWORD(m_pFormItem->getFormPoint(idx)->vals0));
		break;
	case 34:	//	auxs
		tmp = m_pFormItem->getFormStr(idx, "auxS");
		break;
	case 35:	//	str
		tmp = m_pFormItem->getFormStr(idx, "str");
		break;
	case 36:	//	initC
		if (m_pFormItem->getFormPoint(idx)->initC)
			tmp = "1";
		else
			tmp = "0";
		break;
	case 37:	//	refs
		if (m_pFormItem->getFormPoint(idx)->properties & FA_REF)
			tmp = m_pFormItem->getFormStr(idx, "refS");
		break;
	case 38:	//	offs			
		tmp.Format("%d", min(m_pFormItem->getFormPoint(idx)->size, m_pFormItem->getFormPoint(idx)->offs));
		break;
	case 39:	//	table
		if (m_pFormItem->getFormPoint(idx)->attr2 & GO_TABLE)
			tmp = "1";
		else
			tmp = "0";
		break;
	case 40:	//	rows
		{
			struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(idx)->auxR;
			if (repR)
				tmp.Format("%d", repR->rows);
		}
		break;
	case 41:	//	cols
		{
			struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(idx)->auxR;
			if (repR)
				tmp.Format("%d", repR->cols);
		}
		break;
	case 42:	//	str2
		tmp = m_pFormItem->getFormStr(idx, "str2");
		break;
	case 43:	//	stRGB
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->stRGB);
		break;
	case 44:	//	spRGB
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->spRGB);
		break;
	case 45:	//	tabHead
		{
			if (m_pFormItem->getFormPoint(idx)->keys)
			{
				CString temp = m_pFormItem->getFormStr(idx, "dat");
				for (int jj = 0; jj < m_pFormItem->getFormPoint(idx)->checked + 1; jj++)
					tmp = Parser(temp, "\t");
			}
		}
		break;
	case 46:	//	tabData
		{
			if (m_pFormItem->getFormPoint(idx)->keys)
			{
				CString temp = m_pFormItem->getFormStr(idx, "str");
				for (int jj = 0; jj < m_pFormItem->getFormPoint(idx)->checked + 1; jj++)
					tmp = Parser(temp, "\t");
				Parser(tmp, ":");
			}
		}
		break;
	case 47:	//	tabVisible
		{
			if (m_pFormItem->getFormPoint(idx)->keys)
			{
				CString temp = m_pFormItem->getFormStr(idx, "str");
				for (int jj = 0; jj < m_pFormItem->getFormPoint(idx)->checked + 1; jj++)
					tmp = Parser(temp, "\t");
				tmp = Parser(tmp, ":");
			}
		}
		break;
	case 48:	//	tableHead
		{
			if (m_pFormItem->getFormPoint(idx)->attr2 & GO_XHEAD)
				tmp = '2';
			else if (m_pFormItem->getFormPoint(idx)->attr2 & GO_MULTI)
				tmp = '0';
			else
				tmp = '1';
		}
		break;
	case 49:	//	editS
		tmp = m_pFormItem->getFormStr(idx, "editS");
		break;
	case 50:	//	alignImg
		tmp.Format("%d", m_pFormItem->getFormPoint(idx)->alignImage);
		break;
	case 51:	//	fixSize
		if (m_pFormItem->getFormPoint(idx)->bFixedSize)
			tmp = '1';
		else 
			tmp = '0';
		break;
	case 119:	//	allcellwidth
		{
			struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(idx)->auxR;
			if (repR->rows > 0)
			{
				CCellItem *cellItem = new CCellItem(repR);
				tmp.Format("%d", cellItem->getCellPoint(0)->width);
				delete cellItem;
			}
			else
				tmp = "80";
		}
		break;
	}
	if (fnum >= 100 && fnum < 200)
	{
		struct _repR *repR   = (idx >= 0?(struct _repR *)m_pFormItem->getFormPoint(idx)->auxR:NULL);
		if (repR != NULL && (repR->selected >= 0 || col >= 0))
		{
			if (col < 0)
				col = repR->selected;
			
			CCellItem *cellItem = new CCellItem(repR);
			struct _cellR *cellR = cellItem->getCellPoint(col);
			switch(fnum)
			{
			case 100:	//	cellname
				tmp = cellR->name;
				break;
			case 101:	//	celltype
				tmp.Format("%d", cellR->type);
				break;
			case 102:	//	celliok
				if (cellR->iok == EIO_OUTPUT && cellR->kind == CK_CHECK)
					cellR->iok = EIO_INOUT;
				if ((cellR->iok == EIO_OUTPUT || cellR->iok == EIO_INOUT || cellR->iok == EIO_INPUT) && cellR->kind == CK_BUTTON)
					cellR->iok = EIO_NOP;
				tmp.Format("%d", cellR->iok);
				break;
			case 103:	//	cellkind
				if (cellR->iok == EIO_OUTPUT && cellR->kind == CK_CHECK)
					cellR->kind = CK_NORMAL;
				if ((cellR->iok == EIO_OUTPUT || cellR->iok == EIO_INOUT || cellR->iok == EIO_INPUT) && cellR->kind == CK_BUTTON)
					cellR->kind = CK_NORMAL;
				tmp.Format("%d", cellR->kind);
				break;
			case 104:	//	cellsize
				tmp.Format("%d", cellR->size);
				break;
			case 105:	//	cellwidth
				tmp.Format("%d", cellR->width);
				break;
			case 106:	//	cellhead
				tmp = cellR->head;
				break;
			case 107:	//	cellalignH
				tmp.Format("%d", cellR->alignH);
				break;
			case 108:	//	cellalignC
				tmp.Format("%d", cellR->alignC);
				break;
			case 109:	//	cellhpRGB
				tmp.Format("%d", cellR->hpRGB);
				break;
			case 110:	//	cellhtRGB
				tmp.Format("%d", cellR->htRGB);
				break;
			case 111:	//	cellpRGB
				tmp.Format("%d", cellR->pRGB);
				break;
			case 112:	//	celltRGB
				tmp.Format("%d", cellR->tRGB);
				break;
			case 113:	//	cellvals
				tmp.Format("%d", cellR->vals);
				break;
			case 114:	//	cellstr
				tmp = cellR->str;
				break;
			case 115:	//	cellauxs
				tmp = cellR->auxS;
				break;
			case 116:	//	cellrefs
				tmp = cellR->refS;
				break;
			case 117:	//	cellproperties
				{
					CString str = getValue2(row);
					int nCnt = 1;
					tmp = "0";
					while(!str.IsEmpty())
					{
						CString str2 = Parser(str, ";");
						if (_httoi(str2) & cellR->properties)
							tmp.Format("%d", nCnt);
						nCnt++;
					}
				}
				break;
			case 118:	//	cellattr
				if (_httoi(getValue2(row)) & cellR->attr)
					tmp = "1";
				else
					tmp = "0";
				break;
			case 120:	//	cellstr2
				tmp = cellR->str2;
				break;
			case 121:	//	celldat
				tmp = cellR->dat;
				break;
			case 122:	//	cellkeys
				tmp.Format("%d", cellR->keys);
				break;
			case 123:	//	cellvals2
				tmp.Format("%ld", cellR->vals2);
				break;
			case 124:	//	cellalignI
				tmp.Format("%d", cellR->alignI);
				break;
			case 125:	//	cellstr3
				tmp = cellR->str3;
				break;
			case 126:	//	celltxt
				tmp = cellR->txt;
				break;
			case 127:	//	cellisHint
				tmp.Format("%d", cellR->isHint);
				break;
			case 128:	//	cellonHint
				tmp = cellR->onHint;
				break;
			case 129:	//	cellattr2
				if ((cellR->iok == EIO_INOUT || cellR->iok == EIO_INPUT) && cellR->kind == CK_NORMAL)	// edit
				{
					if (cellR->attr & FA_DATE)
						tmp.Format("%d", (cellR->attr2 & SA_DATE));
					else
						tmp.Format("%d", SA_DATE);
					break;
				}
				tmp.Format("%d", SA_DATE);
				break;
			}
			delete cellItem;
		}
	}
	
	if (fnum >= 200)
	{
		switch(fnum) // form
		{
		case 200:	//	mapcaption
			tmp = m_mapH->caption;
			break;
		case 201:	//	mapheight
			tmp.Format("%d", m_mapH->height);
			break;
		case 202:	//	mapwidth
			tmp.Format("%d", m_mapH->width);
			break;
		case 203:	//	mapfname
			tmp = m_mapH->fName;
			break;
		case 204:	//	mapfpoint
			tmp.Format("%d", m_mapH->fPoint);
			break;
		case 205:	//	mapfstyle
			tmp.Format("%d", m_mapH->fStyle);
			break;
		case 206:	//	mapvers
			tmp.Format("%d", m_mapH->vers);
			break;
		case 207:	//	mapname
			tmp = m_mapH->mapN;
			break;
		case 208:	//	mapdesc
			if (strlen(m_mapH->mapdesc) == 0)
			{
				tmp = m_mapH->caption;
				CopyMemory(m_mapH->mapdesc, m_mapH->caption, min(strlen(m_mapH->caption), 20));	
			}
			else
				tmp = m_mapH->mapdesc;
			break;
		case 209:	//	maptrxC
			tmp = m_mapH->trxC;
			break;
		case 210:	//	maptrxH
			tmp = m_mapH->trxH;
			break;
		case 211:	//	maptoPath
			tmp.Format("%d", m_mapH->toPath);
			break;
		case 212:	//	mapmapM
			tmp.Format("%d", m_mapH->mapM);
			break;
		case 213:	//	maphelpN
			tmp = m_mapH->helpN;
			break;
		case 214:	//	maprepeatV
			tmp.Format("%d", m_mapH->repeatV);
			break;
		case 215:	//	mapwaitV
			tmp.Format("%d", m_mapH->waitV);
			break;
		case 216:	//	mapimageK
			tmp.Format("%d", m_mapH->imageK);
			break;
		case 217:	//	mapimageV
			tmp = m_mapH->imageV;
			break;
		case 218:	//	mapGroup
			tmp.Format("%d", m_mapH->mapGroup);
			break;
		case 219:	//	maptype
			tmp.Format("%d", m_mapH->type);
			break;
		case 220:	//	repeatV
			tmp.Format("%d", m_mapH->repeatV);
			break;
		case 221:	//	publicR
			tmp = m_mapH->publicR;
			break;
		case 222:	//	mapBkClr
			tmp.Format("%d", m_mapH->bkClr);
			break;
		}
	}
	
	return tmp;
}

void CListGrid::setFormValue(int fnum, int idx, int col, int row, CString data)
{
	if (m_pFormItem->getFormCount() <= idx)
		return;

	m_mapH->modified = true;
	switch(fnum)
	{
	case 0:	//	name
		m_pFormItem->setFormStr(idx, "name", data);
		break;
	case 1:	//	size
		m_pFormItem->getFormPoint(idx)->size = atoi(data);
		break;
	case 2:	//	type
		m_pFormItem->getFormPoint(idx)->type = atoi(data);
		break;
	case 3:	//	iok
		m_pFormItem->getFormPoint(idx)->iok = atoi(data);
		//AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SORTORDER);
		break;
	case 4:	//	Hkeys
		m_pFormItem->getFormPoint(idx)->keys = MAKELONG(LOWORD(m_pFormItem->getFormPoint(idx)->keys), atoi(data));
		break;
	case 5:	//	Lkeys
		m_pFormItem->getFormPoint(idx)->keys = MAKELONG(atoi(data), HIWORD(m_pFormItem->getFormPoint(idx)->keys));
		break;
	case 6:	//	keys
		if (m_pFormItem->getFormPoint(idx)->kind == FM_TAB || m_pFormItem->getFormPoint(idx)->kind == FM_UTAB)
		{
			if (atoi(data) <= 0)
				return;
			struct _formR *formR = m_pFormItem->getFormPoint(idx);
			for (int ii = formR->keys ; ii < atoi(data); ii++)
			{
				CString tmp;
				tmp.Format("Tab%d\t", ii);
				m_pFormItem->setFormStr(idx, "dat", m_pFormItem->getFormStr(idx, "dat") + tmp);
				if (m_pFormItem->getFormPoint(idx)->kind == FM_UTAB)
				{
					CString tt;
					tt.Format("1:%d\t", m_pFormItem->getFormPoint(idx)->keys);
					m_pFormItem->setFormStr(idx, "str", m_pFormItem->getFormStr(idx, "str") + tt);
				}
			}
			if (atoi(data) < (int)formR->keys)
			{
				CString tmp = m_pFormItem->getFormStr(idx, "dat"), 
					tmp2 = m_pFormItem->getFormStr(idx, "str"),
					rTmp = "", rTmp2 = "";
				int ii = 0;
				for (ii = 0; ii < atoi(data); ii++)
				{
					rTmp += Parser(tmp, "\t");
					rTmp += '\t';
					if (formR->kind == FM_UTAB)
					{
						rTmp2 += Parser(tmp2, "\t");
						rTmp2 += '\t';
					}
				}
				m_pFormItem->setFormStr(idx, "dat", rTmp);
				if (formR->kind == FM_UTAB)
					m_pFormItem->setFormStr(idx, "str", rTmp2);
				for (ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
					if (m_pFormItem->getFormPoint(ii)->tabN == formR &&
						m_pFormItem->getFormPoint(ii)->pageN >= atoi(data))
						m_pFormItem->removeForm(ii);
				formR->checked = 0;
			}
			GetParent()->PostMessage(WM_USER, 1, idx);	// Change Property
		}
		m_pFormItem->getFormPoint(idx)->keys = atoi(data);
		break;
	case 7:	//	left
		m_pFormItem->getFormPoint(idx)->right += atoi(data) - m_pFormItem->getFormPoint(idx)->left;
		m_pFormItem->getFormPoint(idx)->left = atoi(data);
		if (m_pFormItem->getFormPoint(idx)->tabN)
		{
			m_pFormItem->getFormPoint(idx)->right += m_pFormItem->getFormPoint(idx)->tabN->left;
			m_pFormItem->getFormPoint(idx)->left += m_pFormItem->getFormPoint(idx)->tabN->left;
		}
		break;
	case 8:	//	top
		m_pFormItem->getFormPoint(idx)->bottom += atoi(data) - m_pFormItem->getFormPoint(idx)->top;
		m_pFormItem->getFormPoint(idx)->top = atoi(data);
		if (m_pFormItem->getFormPoint(idx)->tabN)
		{
			m_pFormItem->getFormPoint(idx)->bottom += m_pFormItem->getFormPoint(idx)->tabN->top;
			m_pFormItem->getFormPoint(idx)->top += m_pFormItem->getFormPoint(idx)->tabN->top;
		}
		break;
	case 9:	//	width
		m_pFormItem->getFormPoint(idx)->right = atoi(data) + m_pFormItem->getFormPoint(idx)->left;
		break;
	case 10:	//	height
		m_pFormItem->getFormPoint(idx)->bottom = atoi(data) + m_pFormItem->getFormPoint(idx)->top;
		break;
	case 11:	//	align
		m_pFormItem->getFormPoint(idx)->alignment = atoi(data);
		break;
	case 12:	//	borders
		m_pFormItem->getFormPoint(idx)->borders = atoi(data);
		break;
	case 13:	//	index
		m_pFormItem->getFormPoint(idx)->index = atoi(data);
		break;
	case 14:	//	fstyle
		if (m_pFormItem->getFormPoint(idx)->fStyle == FS_NONE && atoi(data) != FS_NONE)
		{
			m_pFormItem->setFormStr(idx, "fName", m_mapH->fName);
			m_pFormItem->getFormPoint(idx)->fPoint = m_mapH->fPoint;
			AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHPRO, -1);
		}
		if (m_pFormItem->getFormPoint(idx)->fStyle != FS_NONE && atoi(data) == FS_NONE)
		{
			m_pFormItem->setFormStr(idx, "fName", "");
			m_pFormItem->getFormPoint(idx)->fPoint = 0;
			AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHPRO, -1);
		}
		m_pFormItem->getFormPoint(idx)->fStyle = atoi(data);
		break;
	case 15:	//	fpoint
		m_pFormItem->getFormPoint(idx)->fPoint = atoi(data);
		break;
	case 16:	//	fname
		m_pFormItem->setFormStr(idx, "fName", data);
		break;
	case 17:	//	tRGB
		m_pFormItem->getFormPoint(idx)->tRGB = atoi(data);
		break;
	case 18:	//	pRGB
		m_pFormItem->getFormPoint(idx)->pRGB = atoi(data);
		break;
	case 19:	//	bRGB
		m_pFormItem->getFormPoint(idx)->bRGB = atoi(data);
		break;
	case 20:	//	attr
		if (data.GetAt(0) == '0')
			m_pFormItem->getFormPoint(idx)->attr &= ~_httoi(getValue2(row));
		else
		{
			m_pFormItem->getFormPoint(idx)->attr |= _httoi(getValue2(row));
			if (_httoi(getValue2(row)) & FA_XFLOAT)
			{
				m_pFormItem->getFormPoint(idx)->offs = 0;
				GetParent()->PostMessage(WM_USER, 1, idx);	// Change Property
			}
		}
		break;
	case 21:	//	attr2
		if (m_pFormItem->getFormPoint(idx)->kind == FM_EDIT)
		{
			if ((atoi(data) & SA_DATE) == SA_DATE)	// None
			{
				m_pFormItem->getFormPoint(idx)->attr &= ~FA_DATE;
				m_pFormItem->getFormPoint(idx)->attr2 &= ~SA_DATE;
			}
			else 
			{
				m_pFormItem->getFormPoint(idx)->attr |= FA_DATE;
				m_pFormItem->getFormPoint(idx)->attr2 &= ~SA_DATE;
				m_pFormItem->getFormPoint(idx)->attr2 |= (atoi(data) & SA_DATE);
			}
			break;
		}
		if (data.GetAt(0) == '0')
		{
			m_pFormItem->getFormPoint(idx)->attr2 &= ~_httoi(getValue2(row));
			if (_httoi(getValue2(row)) & GO_FLEX)
			{
				m_pFormItem->getFormPoint(idx)->attr2 &= ~GO_EXPAND;
				GetParent()->PostMessage(WM_USER, 1, idx);	// Change Property
			}
		}
		else
		{
			m_pFormItem->getFormPoint(idx)->attr2 |= _httoi(getValue2(row));
			if ((_httoi(getValue2(row)) & GO_EXPAND) &&
				!(m_pFormItem->getFormPoint(idx)->attr2 & GO_FLEX))
			{
				m_pFormItem->getFormPoint(idx)->attr2 &= ~GO_EXPAND;
				GetParent()->PostMessage(WM_USER, 1, idx);	// Change Property
			}
		}
		
		gridMultiLineCheck(idx);
		break;
	case 22:	//	properties
		{
			CString str = getValue2(row), dta = data;
			dta = Parser(dta, "-");
			int nCnt = 1;
			while(!str.IsEmpty())
			{
				CString str2 = Parser(str, ";");
				if (nCnt == atoi(dta))
					m_pFormItem->getFormPoint(idx)->properties |= _httoi(str2);
				else
					m_pFormItem->getFormPoint(idx)->properties &= ~_httoi(str2);
				nCnt++;
			}
		}
		break;
	case 23:	//	checked
		m_pFormItem->getFormPoint(idx)->checked = atoi(data);
		if (m_pFormItem->getFormPoint(idx)->kind == FM_TAB || m_pFormItem->getFormPoint(idx)->kind == FM_UTAB)
			GetParent()->PostMessage(WM_USER, 1, idx);	// Change Property
		break;
	case 24:	//	isHint
		m_pFormItem->getFormPoint(idx)->isHint = atoi(data);
		break;
	case 25:	//	onHint
		m_pFormItem->setFormStr(idx, "help", data);
		break;
	case 26:	//	isEidt
		m_pFormItem->getFormPoint(idx)->isEdit = atoi(data);
		break;
	case 27:	//	onEdit
		m_pFormItem->getFormPoint(idx)->onEdit = atoi(data);
		break;
	case 28:	//	HcoRGB
		m_pFormItem->getFormPoint(idx)->onEdit = MAKELONG(atoi(data), HIWORD(m_pFormItem->getFormPoint(idx)->onEdit));
		break;
	case 29:	//	LcoRGB
		m_pFormItem->getFormPoint(idx)->onEdit = MAKELONG(LOWORD(m_pFormItem->getFormPoint(idx)->onEdit), atoi(data));
		break;
	case 30:	//	vals0
		m_pFormItem->getFormPoint(idx)->vals0 = atoi(data);
		break;
	case 34:	//	auxS
		m_pFormItem->setFormStr(idx, "auxS", data);
		break;
	case 35:	//	str
		if (m_pFormItem->getFormPoint(idx)->kind == FM_OBJECT && !data.IsEmpty())
			if (!data.CompareNoCase(m_mapH->mapN))
			{
				GetParent()->PostMessage(WM_USER, 1, idx);	// Change Property
				m_errStr = "Object Name must different from Map Name!!";
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, (long)m_errStr.operator LPCTSTR());
				break;
			}
		m_pFormItem->setFormStr(idx, "str", data);
		break;
	case 36:	//	initC
		if (data.GetAt(0) == '0')
			m_pFormItem->getFormPoint(idx)->initC = false;
		else
			m_pFormItem->getFormPoint(idx)->initC = true;
		break;
	case 37:	//	refs
		if (data.IsEmpty())
		{
			m_pFormItem->getFormPoint(idx)->attr &= ~FA_REF;
			m_pFormItem->setFormStr(idx, "refS", "");
		}
		else
		{
			m_pFormItem->getFormPoint(idx)->attr |= FA_REF;
			m_pFormItem->setFormStr(idx, "refS", data);
		}
		break;
	case 38:	//	offs
		if (!atoi(data))
		{
			m_pFormItem->getFormPoint(idx)->attr &= ~FA_FLOAT;
			m_pFormItem->getFormPoint(idx)->offs = 0;
		}
		else
		{
			m_pFormItem->getFormPoint(idx)->offs = min(m_pFormItem->getFormPoint(idx)->size, atoi(data));
			if (m_pFormItem->getFormPoint(idx)->size < atoi(data))
			{
				GetParent()->PostMessage(WM_USER, 1, idx);	// Change Property
				m_errStr = "Data Length less then float digit!";
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, (long)m_errStr.operator LPCTSTR());
			}
		}
		break;
	case 39:	//	table
		if (data.GetAt(0) == '0')
		{
			m_pFormItem->getFormPoint(idx)->attr2 &= ~GO_TABLE;
		}
		else
		{
			m_pFormItem->getFormPoint(idx)->attr2 |= GO_TABLE;
			m_pFormItem->getFormPoint(idx)->attr  &= ~FA_ENUM;
			m_pFormItem->getFormPoint(idx)->attr2 &= ~GO_HEADER;
		}
		break;
	case 40:	//	rows
		{
			if (atoi(data) < 0)
				data = "0";
			struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(idx)->auxR;
			CCellItem *cellItem = new CCellItem(repR);
			struct _cellR *cellR;
			if (m_pFormItem->getFormPoint(idx)->kind == FM_TABLE && cellItem->getCellCount() < atoi(data))
			{
				for (int ii = cellItem->getCellCount(); ii < atoi(data); ii++)
				{
					cellR = cellItem->getCellPoint(cellItem->addCell());
					cellR->pRGB = basINPUT;
					cellR->tRGB = 232;
					if (repR->rows)
					{
						cellR->hpRGB = cellItem->getCellPoint(repR->rows - 1)->hpRGB;
						cellR->htRGB = cellItem->getCellPoint(repR->rows - 1)->htRGB;
						cellR->width = cellItem->getCellPoint(repR->rows - 1)->width;
					}
					else
					{
						cellR->hpRGB = basPRGB;
						cellR->htRGB = 232;
						cellR->width = 80;
					}
					cellR->size = 0;
					//cellR->headN = 1;
					cellR->type = IO_NUM;
					strcpy_s(cellR->name, getInitSymbol("Row"));
					cellR->alignC = AL_RIGHT;
					cellR->properties |= PR_VISIBLE;
				}
			}
			if (m_pFormItem->getFormPoint(idx)->kind == FM_TABLE)
			{
				while(atoi(data) < cellItem->getCellCount())
					cellItem->removeLastCell(atoi(data));
			}
			repR->rows = atoi(data);
			delete cellItem;
		}
		break;
	case 41:	//	cols
		{
			if (atoi(data) < 0)
				data = "0";
			struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(idx)->auxR;
			CCellItem *cellItem = new CCellItem(repR);
			struct _cellR *cellR;
			if (m_pFormItem->getFormPoint(idx)->kind == FM_GRID && cellItem->getCellCount() < atoi(data))
			{
				for (int ii = cellItem->getCellCount(); ii < atoi(data); ii++)
				{
					cellR = cellItem->getCellPoint(cellItem->addCell());
					cellR->pRGB = basINPUT;
					cellR->tRGB = 232;
					if (repR->cols)
					{
						cellR->hpRGB = cellItem->getCellPoint(repR->cols - 1)->hpRGB;
						cellR->htRGB = cellItem->getCellPoint(repR->cols - 1)->htRGB;
						cellR->width = cellItem->getCellPoint(repR->cols - 1)->width;
					}
					else
					{
						cellR->hpRGB = basPRGB;
						cellR->htRGB = 232;
						cellR->width = 80;
					}
					cellR->size = 0;
					//cellR->headN = 1;
					cellR->type = IO_NUM;
					cellR->iok = EIO_OUTPUT;
					cellR->kind = CK_NORMAL;
					strcpy_s(cellR->name, getInitSymbol("Col"));
					cellR->alignC = AL_RIGHT;
					cellR->properties |= PR_VISIBLE;
				}
			}
			if (m_pFormItem->getFormPoint(idx)->kind == FM_GRID)
			{
				while(atoi(data) < cellItem->getCellCount())
					cellItem->removeLastCell(atoi(data));
			}
			repR->cols = atoi(data);
			delete cellItem;
			gridMultiLineCheck(idx);
		}
		break;
	case 42:	//	str2
		m_pFormItem->setFormStr(idx, "str2", data);
		break;
	case 43:	//	stRGB
		m_pFormItem->getFormPoint(idx)->stRGB = atoi(data);
		break;
	case 44:	//	spRGB
		m_pFormItem->getFormPoint(idx)->spRGB = atoi(data);
		break;
	case 45:	//	tabHead
		{
			if (m_pFormItem->getFormPoint(idx)->checked >= m_pFormItem->getFormPoint(idx)->keys)
				break;
			CString temp = _T(""), tmp = m_pFormItem->getFormStr(idx, "dat"), retv = _T("");
			int cnt = m_pFormItem->getFormPoint(idx)->checked;
			for (int jj = 0; jj < cnt; jj++)
			{
				temp = Parser(tmp, "\t");
				retv += temp;
				retv += "\t";
			}
			retv += data;
			retv += "\t";
			Parser(tmp, "\t");
			retv += tmp;
			m_pFormItem->setFormStr(idx, "dat", retv);
		}
		break;
	case 46:	//	tabData
		{
			if (m_pFormItem->getFormPoint(idx)->checked >= m_pFormItem->getFormPoint(idx)->keys)
				break;
			CString temp = _T(""), tmp = m_pFormItem->getFormStr(idx, "str"), retv = _T("");
			int cnt = m_pFormItem->getFormPoint(idx)->checked;
			for (int jj = 0; jj < cnt; jj++)
			{
				temp = Parser(tmp, "\t");
				retv += temp;
				retv += "\t";
			}
			temp = Parser(tmp, "\t");
			if (temp.GetLength() && (temp.GetAt(0) == '0' ||  temp.GetAt(0) == '1'))
			{
				retv += temp.GetAt(0);
				retv += ':';
				retv += data;
				retv += '\t';
			}
			retv += tmp;
			m_pFormItem->setFormStr(idx, "str", retv);
		}
		break;
	case 47:	//	tabVisible
		{
			if (m_pFormItem->getFormPoint(idx)->checked >= m_pFormItem->getFormPoint(idx)->keys)
				break;
			CString temp = _T(""), tmp = m_pFormItem->getFormStr(idx, "str"), retv = _T("");
			int cnt = m_pFormItem->getFormPoint(idx)->checked;
			for (int jj = 0; jj < cnt; jj++)
			{
				temp = Parser(tmp, "\t");
				retv += temp;
				retv += "\t";
			}
			temp = Parser(tmp, "\t");
			Parser(temp, ":");
			if (data.GetAt(0) == '0')
				retv += "0:";
			else
				retv += "1:";
			retv += temp;
			retv += "\t";
			
			retv += tmp;
			m_pFormItem->setFormStr(idx, "str", retv);
		}
		break;
	case 48:	//	tableHead
		{
			m_pFormItem->getFormPoint(idx)->attr2 &= ~(GO_MULTI | GO_XHEAD);
			m_pFormItem->getFormPoint(idx)->iok = EIO_NOP;
			switch(atoi(data))
			{
			case 0:
				m_pFormItem->getFormPoint(idx)->attr2 |= GO_MULTI;
				m_pFormItem->getFormPoint(idx)->iok = EIO_OUTPUT;
				break;
			case 1:	break;
			case 2:
				m_pFormItem->getFormPoint(idx)->attr2 |= GO_XHEAD;
				m_pFormItem->getFormPoint(idx)->iok = EIO_OUTPUT;
				break;
			}
		}
		break;
	case 49:	//	editS
		m_pFormItem->setFormStr(idx, "editS", data);
		break;
	case 50:	//	alignImg
		m_pFormItem->getFormPoint(idx)->alignImage = atoi(data);
		break;
	case 51:	//	fixSize
		if (data.GetAt(0) == '0')
			m_pFormItem->getFormPoint(idx)->bFixedSize = false;
		else
			m_pFormItem->getFormPoint(idx)->bFixedSize = true;
		break;
	case 119:	//	allcellwidth
		{
			struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(idx)->auxR;
			CCellItem *cellItem = new CCellItem(repR);
			for (int ii = 0; ii < cellItem->getCellCount(); ii++)
				cellItem->getCellPoint(ii)->width = atoi(data);
			delete cellItem;
		}
		break;
	}
		
	struct _repR *repR   = (idx >= 0?(struct _repR *)m_pFormItem->getFormPoint(idx)->auxR:NULL);
	if (repR != NULL && (repR->selected >= 0 || col >= 0))
	{
		if (col < 0 || !m_GridAttr.multiCol)
			col = repR->selected;
		if (col == -1)
			return;
		CCellItem *cellItem = new CCellItem(repR);
		struct _cellR *cellR = cellItem->getCellPoint(col);
		switch(fnum)
		{
		case 100:	//	cellname
			strcpy_s(cellR->name, data);
			break;
		case 101:	//	celltype
			cellR->type = atoi(data);
			if (cellR->type != IO_NUM)
				cellR->attr &= ~(FA_COMMA|FA_RIGHT);

			break;
		case 102:	//	celliok
			if (atoi(data) == EIO_OUTPUT && cellR->kind == CK_CHECK)
			{
				cellR->iok = EIO_INOUT;
				LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
				UpdateCombo();
			}
			else if ((atoi(data) == EIO_OUTPUT || atoi(data) == EIO_INOUT || atoi(data) == EIO_INPUT) && cellR->kind == CK_BUTTON)
			{
				cellR->iok = EIO_NOP;
				LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
				UpdateCombo();
			}
			else
				cellR->iok = atoi(data);

			if (!((cellR->iok == EIO_INOUT || cellR->iok == EIO_INPUT) && cellR->kind == CK_NORMAL))	// No edit
			{
				cellR->attr  &= ~FA_DATE;
				cellR->attr2 &= ~SA_DATE;
			}
			break;
		case 103:	//	cellkind
			if (cellR->iok == EIO_OUTPUT && atoi(data) == CK_CHECK)
			{
				cellR->kind = CK_NORMAL;
				LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
				UpdateCombo();
			}
			else if ((cellR->iok == EIO_OUTPUT || cellR->iok == EIO_INOUT || cellR->iok == EIO_INPUT) && atoi(data) == CK_BUTTON)
			{
				cellR->kind = CK_NORMAL;
				LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
				UpdateCombo();
			}
			else
				cellR->kind = atoi(data);
			if (!((cellR->iok == EIO_INOUT || cellR->iok == EIO_INPUT) && cellR->kind == CK_NORMAL))	// No edit
			{
				cellR->attr  &= ~FA_DATE;
				cellR->attr2 &= ~SA_DATE;
			}

			break;
		case 104:	//	cellsize
			cellR->size = atoi(data);
			break;
		case 105:	//	cellwidth
			cellR->width = atoi(data);
			break;
		case 106:	//	cellhead
			{
				CString head = data;
				strcpy_s(cellR->head, head);
			}
			break;
		case 107:	//	cellalignH
			cellR->alignH = atoi(data);
			break;
		case 108:	//	cellalignC
			cellR->alignC = atoi(data);
			break;
		case 109:	//	cellhpRGB
			cellR->hpRGB = atoi(data);
			break;
		case 110:	//	cellhtRGB
			cellR->htRGB = atoi(data);
			break;
		case 111:	//	cellpRGB
			cellR->pRGB = atoi(data);
			break;
		case 112:	//	celltRGB
			cellR->tRGB = atoi(data);
			break;
		case 113:	//	cellvals
			cellR->vals = atoi(data);
			break;
		case 114:	//	cellstr
			strcpy_s(cellR->str, data);
			break;
		case 115:	//	cellauxs
			strcpy_s(cellR->auxS, data);
			break;
		case 116:	//	cellrefs
			strcpy_s(cellR->refS, data);
			break;
		case 117:	//	cellproperties
			{
				CString str = getValue2(row), dta = data;
				dta = Parser(dta, "-");
				int nCnt = 1;
				while(!str.IsEmpty())
				{
					CString str2 = Parser(str, ";");
					if (nCnt == atoi(dta))
						cellR->properties |= _httoi(str2);
					else
						cellR->properties &= ~_httoi(str2);
					nCnt++;
				}
			}
			break;
		case 118:	//	cellattr
			if (data.GetAt(0) == '0')
				cellR->attr &= ~_httoi(getValue2(row));
			else
				cellR->attr |= _httoi(getValue2(row));
			break;
		case 120:	//	cellstr2
			strcpy_s(cellR->str2, data);
			break;
		case 121:	//	celldat
			strcpy_s(cellR->dat, data);
			break;
		case 122:	//	cellkeys
			cellR->keys = atoi(data);
			break;
		case 123:	//	cellvals2
			cellR->vals2 = atoi(data);
			break;
		case 124:	//	cellalignI
			cellR->alignI = atoi(data);
			break;
		case 125:	//	cellstr3
			strcpy_s(cellR->str3, data);
			break;
		case 126:	//	celltxt
			strcpy_s(cellR->txt, data);
			break;
		case 127:	//	cellisHint
			cellR->isHint = atoi(data);
			break;
		case 128:	//	cellonHint
			strcpy_s(cellR->onHint, data);
			break;
		case 129:	//	cellattr2
			if ((cellR->iok == EIO_INOUT || cellR->iok == EIO_INPUT) && cellR->kind == CK_NORMAL)	// edit
			{
				if ((atoi(data) & SA_DATE) == SA_DATE)
				{
					cellR->attr &= ~FA_DATE;
					cellR->attr2 &= ~SA_DATE;
				}
				else 
				{
					cellR->attr |= FA_DATE;
					cellR->attr2 &= ~SA_DATE;
					cellR->attr2 |= (atoi(data) & SA_DATE);
				}
				break;
			}

			cellR->attr &= ~FA_DATE;
			cellR->attr2 &= ~SA_DATE;
			LoadData(m_index, m_InputPnt.x, m_InputPnt.y);
			UpdateCombo();
			break;
		}
		delete cellItem;
	}
		
	switch(fnum) // form
	{
	case 200:	//	mapcaption
		strcpy_s(m_mapH->caption, data);
		if (m_mapH->mapK == MK_NORM)
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETCAPTION, 0);
		break;
	case 201:	//	mapheight
		m_mapH->height = atoi(data);
		break;
	case 202:	//	mapwidth
		m_mapH->width = atoi(data);
		break;
	case 203:	//	mapfname
		strcpy_s(m_mapH->fName, data);
		break;
	case 204:	//	mapfpoint
		m_mapH->fPoint = atoi(data);
		break;
	case 205:	//	mapfstyle
		m_mapH->fStyle = atoi(data);
		break;
	case 206:	//	mapvers
		m_mapH->vers = atoi(data);
		break;
	case 207:	//	mapname
		strcpy_s(m_mapH->mapN, data);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETMAPNAME);
		break;
	case 208:	//	mapdesc
		strcpy_s(m_mapH->mapdesc, data);
		if (m_mapH->mapK == MK_TEMPLATE)
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETCAPTION, 0);
		break;
	case 209:	//	maptrxC
		strcpy_s(m_mapH->trxC, data);
		break;
	case 210:	//	maptrxH
		strcpy_s(m_mapH->trxH, data);
		break;
	case 211:	//	maptoPath
		m_mapH->toPath = atoi(data);
		break;
	case 212:	//	mapmapM
		m_mapH->mapM = atoi(data);
		break;
	case 213:	//	maphelpN
		strcpy_s(m_mapH->helpN, data);
		break;
	case 214:	//	maprepeatV
		m_mapH->repeatV = atoi(data);
		break;
	case 215:	//	mapwaitV
		m_mapH->waitV = atoi(data);
		break;
	case 216:	//	mapimageK
		m_mapH->imageK = atoi(data);
		break;
	case 217:	//	mapimageV
		strcpy_s(m_mapH->imageV, data);
		break;
	case 218:	//	mapGroup
		m_mapH->mapGroup = atoi(data);
		break;
	case 219:	//	maptype
		m_mapH->type = atoi(data);
		break;
	case 220:	//	repeatV
		m_mapH->repeatV = atoi(data);
		break;
	case 221:	//	publicR
		strcpy_s(m_mapH->publicR, data);
		break;
	case 222:	//	mapBkClr
		m_mapH->bkClr = atoi(data);
		break;
	}
}

void CListGrid::OnBtnClick()
{
	int dlgnum = atoi(getValue2(m_InputPnt.y));
	switch(dlgnum)
	{
	case 1001:	//	font dialog
		{
			char	*pFname = NULL;
			WORD	*pFstyle = NULL;
			WORD	*pFpoint = NULL;
			CFontDialog	fontD;
			if (m_index == -1)
			{
				pFname = (char*)&m_mapH->fName;
				pFstyle = &m_mapH->fStyle;
				pFpoint = &m_mapH->fPoint;
			}
			else if (m_index == -2)
			{
				for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
				{
					if (m_pFormItem->getFormPoint(jj)->bCommon)
					{
						pFname = m_pFormItem->getFormPoint(jj)->fName;
						pFstyle = &m_pFormItem->getFormPoint(jj)->fStyle;
						pFpoint = &m_pFormItem->getFormPoint(jj)->fPoint;
						break;
					}
				}
			}
			else
			{
				pFname = m_pFormItem->getFormPoint(m_index)->fName;
				pFstyle = &m_pFormItem->getFormPoint(m_index)->fStyle;
				pFpoint = &m_pFormItem->getFormPoint(m_index)->fPoint;
			}

			fontD.m_cf.lStructSize = sizeof(CHOOSEFONT);
			
			ZeroMemory(fontD.m_cf.lpLogFont, sizeof(LOGFONT));
			if (pFname)
				strcpy_s(fontD.m_cf.lpLogFont->lfFaceName, pFname);
			else
				strcpy_s(fontD.m_cf.lpLogFont->lfFaceName, m_mapH->fName);
			
			if (*pFstyle == FS_ITALIC || *pFstyle == FS_IBOLD)
				fontD.m_cf.lpLogFont->lfItalic = TRUE;
			else
				fontD.m_cf.lpLogFont->lfItalic = FALSE;
			
			if (*pFstyle == FS_BOLD || *pFstyle == FS_IBOLD)
				fontD.m_cf.lpLogFont->lfWeight = FW_BOLD;
			else
				fontD.m_cf.lpLogFont->lfWeight = FW_NORMAL;

			CDC *pDC = GetDC();
			fontD.m_cf.lpLogFont->lfHeight = MulDiv(-(int)*pFpoint, pDC->GetDeviceCaps(LOGPIXELSY), 72);
			ReleaseDC(pDC);

			fontD.m_cf.lpLogFont->lfCharSet = DEFAULT_CHARSET;
			fontD.m_cf.hDC = 0;
			fontD.m_cf.Flags  = CF_ENABLEHOOK | CF_FORCEFONTEXIST | CF_SCREENFONTS;
			fontD.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT|CF_NOVERTFONTS;
			fontD.m_cf.nFontType  = SCREEN_FONTTYPE;

			if (fontD.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				int jj = 0;
				do
				{
					if (m_index == -2 && !m_pFormItem->getFormPoint(jj)->bCommon)
					{
						jj++;
						if (jj >= m_pFormItem->getFormCount())
							break;
						continue;
					}
					else if (m_index == -2)
					{
						pFstyle = &m_pFormItem->getFormPoint(jj)->fStyle;
						pFpoint = &m_pFormItem->getFormPoint(jj)->fPoint;
					}

					*pFstyle = FS_NORM;
					if (fontD.IsItalic())
						*pFstyle = FS_ITALIC;
					if (fontD.IsBold())
						*pFstyle += FS_BOLD - FS_NORM;
					
					if (m_index == -1)
						strcpy_s(pFname, fontD.GetFaceName().GetLength() + 1, (char *)fontD.GetFaceName().operator LPCTSTR());
					else if (m_index == -2)
						m_pFormItem->setFormStr(jj, "fName", fontD.GetFaceName());
					else
						m_pFormItem->setFormStr(m_index, "fName", fontD.GetFaceName());
					*pFpoint = fontD.GetSize()/10;
					
					if (m_pEdit)
					{
						m_pEdit->SetSel(0, -1);
						m_pEdit->ReplaceSel(fontD.GetFaceName());
					}
					
					jj++;
					if (jj >= m_pFormItem->getFormCount())
						break;
				}while (m_index == -2);
			}
		}
		break;
	case 1002:	//	Color Dialog
		{
			DWORD rgb = atol(getData(m_InputPnt.x, m_InputPnt.y));
			CRect cellRt;
			if (m_GridAttr.multiCol)
				cellRt.SetRect(m_GridAttr.HeadSz.cx + (m_SelectedPnt.x - m_StartPnt.x) * m_GridAttr.CellSz.cx + 1, 
				m_GridAttr.HeadSz.cy + (m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + 1,
				m_GridAttr.HeadSz.cx + (m_SelectedPnt.x - m_StartPnt.x + 1) * m_GridAttr.CellSz.cx, 
				m_GridAttr.HeadSz.cy + (m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy);
			
			else
				cellRt.SetRect(m_GridAttr.HeadSz.cx + 1,
				(m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + 1,
				m_ClientRt.right,
				(m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy);
			ClientToScreen(&cellRt);
			CColorDlg *clDlg = new CColorDlg(this, rgb, true, cellRt);
			if (clDlg->DoModal() == IDOK)
			{
				m_mapH->modified = true;
				if (m_pEdit)
				{
					CString tmp;
					tmp.Format("%d", clDlg->m_color);
					m_pEdit->SetSel(0, -1);
					m_pEdit->ReplaceSel(tmp);
					setData(tmp, m_InputPnt.x, m_InputPnt.y);

					COLORREF rgbValue = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, 
						atoi(getData(m_InputPnt.x, m_InputPnt.y)));
					m_ctlBkColor  = rgbValue;
					m_ctlBrush.DeleteObject();
					m_ctlBrush.CreateSolidBrush(rgbValue);
				}
				SaveData(m_index, m_InputPnt.x, m_InputPnt.y);
			}
			clDlg->DestroyWindow();
			delete clDlg;
		}
		break;
	case 1003:	//	File Dialog
		{
			CFileDialog	fileD(TRUE, NULL, NULL,
				OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
				"Bitmap Files (*.bmp)|*.bmp|Gif Files (*.gif)|*.gif||");
			if (fileD.DoModal() != IDOK)
				return;
			if (m_pEdit)
			{
				m_mapH->modified = true;
				m_pEdit->SetSel(0, -1);
				m_pEdit->ReplaceSel(fileD.GetPathName());
			}
		}
		break;
	case 1103:	// File Dialog avi
		{
			CFileDialog	fileD(TRUE, NULL, NULL,
				OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
				"Windows Media Files (asf)(*.bmp; *.wm; *.wma)|*.bmp; *.wm; *.wma|Windows AVI Files (avi)(*.avi; *.wmv)|*.avi; *.wmv|Windows Audio Files(*.wav; *.snd; *.au)|*.wav; *.snd; *.au|Mpeg Files(*.mpeg; *.mpg; *.m1v; *.mp2; *.mp3)|*.mpeg; *.mpg; *.m1v; *.mp2; *.mp3|All Files(*.*)|*.*||");
			if (fileD.DoModal() != IDOK)
				return;
			if (m_pEdit)
			{
				m_mapH->modified = true;
				m_pEdit->SetSel(0, -1);
				m_pEdit->ReplaceSel(fileD.GetPathName());
			}
		}
		break;
	case 1004:	//	attributes
		if (m_index >= 0 || m_index == -2)
		{
			int idx = m_index;
			if (idx == -2)
				for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
					if (m_pFormItem->getFormPoint(jj)->bCommon)
					{
						idx = jj;
						break;
					}

			if (m_pFormItem->getFormPoint(idx)->kind == FM_GRID || m_pFormItem->getFormPoint(idx)->kind == FM_TABLE)
			{
				struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(idx)->auxR;
				if (m_GridAttr.multiCol)
					repR->selected = m_SelectedPnt.x;

				CCellItem cellItem(repR);
				
				CAttrDlg attrDlg(this, m_pFormItem->getFormPoint(idx)->kind, cellItem.getCellPoint(repR->selected)->type, cellItem.getCellPoint(repR->selected)->iok,
					cellItem.getCellPoint(repR->selected)->attr, cellItem.getCellPoint(repR->selected)->attr2);	
				if (attrDlg.DoModal() == IDOK)
				{
					m_mapH->modified = true;
					cellItem.getCellPoint(repR->selected)->attr = attrDlg.m_attr;
					cellItem.getCellPoint(repR->selected)->attr2 = attrDlg.m_attr2;
				}

				if (m_pFormItem->getFormPoint(idx)->kind == FM_GRID)
					gridMultiLineCheck(idx);
			}
			else
			{
				CAttrDlg attrDlg(this, m_pFormItem->getFormPoint(idx)->kind, m_pFormItem->getFormPoint(idx)->type, m_pFormItem->getFormPoint(idx)->iok, m_pFormItem->getFormPoint(idx)->attr, m_pFormItem->getFormPoint(idx)->attr2);
				
				if (attrDlg.DoModal() == IDOK)
				{
					m_mapH->modified = true;
					do
					{
						if (m_index == -2 && !m_pFormItem->getFormPoint(idx)->bCommon)
						{
							idx++;
							if (idx >= m_pFormItem->getFormCount())
								break;
							continue;
						}
						m_pFormItem->getFormPoint(idx)->attr  = attrDlg.m_attr;
						m_pFormItem->getFormPoint(idx)->attr2 = attrDlg.m_attr2;
						idx++;
						if (idx >= m_pFormItem->getFormCount())
							break;
					}while(m_index == -2);
				}
			}
			if (m_GridAttr.multiCol)
				SendMessage(WM_APPLYMULTISEL, m_SelectedPnt.x);
		}
		break;
	case 1005:	// combo str
		{
			int idx = m_index;
			if (idx == -2)
				for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
					if (m_pFormItem->getFormPoint(jj)->bCommon)
					{
						idx = jj;
						break;
					}
			CComboItem	cbItem(this, LOWORD(m_pFormItem->getFormPoint(idx)->keys), m_pFormItem->getFormPoint(idx)->size);
			if (m_pFormItem->getFormPoint(idx)->dat)
				cbItem.m_disB = m_pFormItem->getFormPoint(idx)->dat;
			if (m_pFormItem->getFormPoint(idx)->str)
				cbItem.m_datB =m_pFormItem->getFormPoint(idx)->str;
			if (cbItem.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				do
				{
					if (m_index == -2 && !m_pFormItem->getFormPoint(idx)->bCommon)
						continue;
					
					if (m_pEdit)
					{
						CString tmp;
						tmp.Format("%d", cbItem.m_count);
						m_pEdit->SetSel(0, -1);
						m_pEdit->ReplaceSel(tmp);
					}
					m_pFormItem->getFormPoint(idx)->keys = MAKELONG((WORD)cbItem.m_count, (WORD)cbItem.m_count);
					m_pFormItem->setFormStr(idx, "dat", cbItem.m_disB);
					m_pFormItem->setFormStr(idx, "str", cbItem.m_datB);
					if (idx >= m_pFormItem->getFormCount())
						break;
				}while(m_index == -2);
			}
		}
		break;
	case 1007:	// edit format
		{
			struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
			CCellItem cellItem(repR);
			if (m_GridAttr.multiCol)
				repR->selected = m_InputPnt.x;
			CCellEdit cellEdit(this, FM_GRID, cellItem.getCellPoint(repR->selected)->type, 
				cellItem.getCellPoint(repR->selected)->isEdit,
				cellItem.getCellPoint(repR->selected)->onEdit, 
				cellItem.getCellPoint(repR->selected)->editS,  
				cellItem.getCellPoint(repR->selected)->name,
				cellItem.getCellPoint(repR->selected)->attr,   
				cellItem.getCellPoint(repR->selected)->size);
			if (cellEdit.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				cellItem.getCellPoint(repR->selected)->attr   = cellEdit.m_attr;
				cellItem.getCellPoint(repR->selected)->isEdit = cellEdit.m_isEdit;
				cellItem.getCellPoint(repR->selected)->onEdit = cellEdit.m_onEdit;
				strcpy_s(cellItem.getCellPoint(repR->selected)->editS, (char *)cellEdit.m_format.operator LPCTSTR());
			}
			if (m_GridAttr.multiCol)
				SendMessage(WM_APPLYMULTISEL, m_SelectedPnt.x);
		}
		break;
	case 1008:	//	map option dialog
		{
			CMapOption Dlg(m_mapH);
			if (Dlg.DoModal() == IDOK)
			{
				m_mapH->modified = true;
			}
		}
		break;
	case 1009:	// column edit Dialog
		if (m_index >= 0)
		{
			CGridColumnDlg	Dlg(this, m_mapH, m_index);
			Dlg.DoModal();
			m_mapH->modified = true;
		}
		break;
	case 1010:	// publicDlg
		{
			CPublicSetDlg Dlg;
			
			m_pEdit->GetWindowText(Dlg.m_publicR);
			//Dlg.m_publicR = m_mapH->publicR;
			if (Dlg.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				strcpy_s(m_mapH->publicR, Dlg.m_publicR);
				m_pEdit->SetSel(0, -1);
				m_pEdit->ReplaceSel(Dlg.m_publicR);
				m_pEdit->SetSel(0, 0);
			}
		}
		break;
	case 1011:	// cell combo str
		{
			int idx = m_index;

			if (m_index < 0 || !m_pFormItem->getFormPoint(m_index)->auxR)
				break;
			
			struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
			CCellItem cellItem(repR);
			if (m_GridAttr.multiCol)
				repR->selected = m_InputPnt.x;
			if (repR->selected < 0)
				repR->selected = m_InputPnt.x;
			if (cellItem.getCellPoint(repR->selected)->kind != CK_COMBO)
				break;

			CComboItem	cbItem(this, cellItem.getCellPoint(repR->selected)->keys, cellItem.getCellPoint(repR->selected)->size);
			if (cellItem.getCellPoint(repR->selected)->dat)
				cbItem.m_disB = cellItem.getCellPoint(repR->selected)->dat;
			if (cellItem.getCellPoint(repR->selected)->str)
				cbItem.m_datB =cellItem.getCellPoint(repR->selected)->str;
			if (cbItem.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
				
				if (m_pEdit)
				{
					CString tmp;
					tmp.Format("%d", cbItem.m_count);
					m_pEdit->SetSel(0, -1);
					m_pEdit->ReplaceSel(tmp);
				}
				cellItem.getCellPoint(repR->selected)->keys = cbItem.m_count;
				strcpy_s(cellItem.getCellPoint(repR->selected)->dat, cbItem.m_disB);
				strcpy_s(cellItem.getCellPoint(repR->selected)->str, cbItem.m_datB);
			}
		}
		break;
	case 1012:	// control setup dialog
		{
			CControlDlg Dlg;
			
			m_pEdit->GetWindowText(Dlg.m_ctrlName);
			Dlg.m_ctrlParam = m_pFormItem->getFormStr(m_index, "str2");
			if (Dlg.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				m_pFormItem->setFormStr(m_index, "str", Dlg.m_ctrlName);
				m_pFormItem->setFormStr(m_index, "str2", Dlg.m_ctrlParam);

				m_pEdit->SetSel(0, -1);
				m_pEdit->ReplaceSel(Dlg.m_ctrlName);
				m_pEdit->SetSel(0, 0);
				AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, 0); // control event refresh
			}
		}
		break;
	case 1013:	// utabpage
		{
			CUTabPage	Dlg(this, m_mapH, m_index);
			Dlg.DoModal();
		}
		break;
	case 1014:	// inputkey combo
		{
			CRect cellRt;
			if (m_GridAttr.multiCol)
				cellRt.SetRect(m_GridAttr.HeadSz.cx + (m_SelectedPnt.x - m_StartPnt.x) * m_GridAttr.CellSz.cx + 1, 
				m_GridAttr.HeadSz.cy + (m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + 1,
				m_GridAttr.HeadSz.cx + (m_SelectedPnt.x - m_StartPnt.x + 1) * m_GridAttr.CellSz.cx, 
				m_GridAttr.HeadSz.cy + (m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy);
			
			else
				cellRt.SetRect(m_GridAttr.HeadSz.cx + 1,
				(m_SelectedPnt.y - m_StartPnt.y) * m_GridAttr.CellSz.cy + 1,
				m_ClientRt.right,
				(m_SelectedPnt.y - m_StartPnt.y + 1) * m_GridAttr.CellSz.cy);

			ClientToScreen(&cellRt);

			CInputKeyDlg	Dlg(this, cellRt);
			if (Dlg.DoModal() == IDOK)
			{
				if (m_pEdit)
				{
					CString tmp;
					tmp.Format("%ld", Dlg.m_keyVal);
					m_pEdit->SetSel(0, -1);
					m_pEdit->ReplaceSel(tmp);
				}
			}
		}
		break;
	case 1015:	// Edit Format
		if (m_index >= 0 || m_index == -2)
		{
			int idx = m_index;
			if (idx == -2)
				for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
					if (m_pFormItem->getFormPoint(jj)->bCommon)
					{
						idx = jj;
						break;
					}
			if (!((m_pFormItem->getFormPoint(idx)->kind == FM_EDIT || m_pFormItem->getFormPoint(idx)->kind == FM_OUT || m_pFormItem->getFormPoint(idx)->kind == FM_COMBO)
				&& (m_pFormItem->getFormPoint(idx)->type == IO_NUM || m_pFormItem->getFormPoint(idx)->type == IO_ALPHA)
				&& (m_pFormItem->getFormPoint(idx)->isEdit == IE_EDIT || m_pFormItem->getFormPoint(idx)->isEdit == IE_NOP)))
			{
				m_errStr = "Edit Format is not able to do!!";
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, (long)m_errStr.operator LPCTSTR());
				break;
			}
			CString name = m_pFormItem->getFormStr(idx, "name"),
				editS = m_pFormItem->getFormStr(idx, "editS");
			CFormat*	format;
			format = new CFormat(this, m_pFormItem->getFormPoint(idx)->isEdit, (LPSTR)(LPCTSTR)editS,
				m_pFormItem->getFormPoint(idx)->attr, (LPSTR)(LPCTSTR)name, m_pFormItem->getFormPoint(idx)->size);
			if (format->DoModal() == IDOK)
			{
				m_mapH->modified = true;
				do
				{
					if (m_index == -2 && !m_pFormItem->getFormPoint(idx)->bCommon)
					{
						idx++;
						if (idx >= m_pFormItem->getFormCount())
							break;
						continue;
					}

					if (!((m_pFormItem->getFormPoint(idx)->kind == FM_EDIT || m_pFormItem->getFormPoint(idx)->kind == FM_OUT || m_pFormItem->getFormPoint(idx)->kind == FM_COMBO)
						&& (m_pFormItem->getFormPoint(idx)->type == IO_NUM || m_pFormItem->getFormPoint(idx)->type == IO_ALPHA)
						&& (m_pFormItem->getFormPoint(idx)->isEdit == IE_EDIT || m_pFormItem->getFormPoint(idx)->isEdit == IE_NOP)))
					{
						idx++;
						if (idx >= m_pFormItem->getFormCount())
							break;
						continue;
					}

					m_pFormItem->getFormPoint(idx)->attr   = format->m_attr;
					m_pFormItem->getFormPoint(idx)->size   = format->m_size;
					m_pFormItem->getFormPoint(idx)->isEdit = format->m_isEdit;
					m_pFormItem->setFormStr(idx, "editS", format->m_edit);
					idx++;
					if (idx >= m_pFormItem->getFormCount())
						break;
				}while(m_index == -2);
			}
			delete format;
		}
		break;
	case 1016:	// CoRGB
		if (m_index >= 0 || m_index == -2)
		{
			int idx = m_index;
			if (idx == -2)
				for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
					if (m_pFormItem->getFormPoint(jj)->bCommon)
					{
						idx = jj;
						break;
					}
			if (!((m_pFormItem->getFormPoint(idx)->kind == FM_OUT 
				|| (m_pFormItem->getFormPoint(idx)->kind == FM_EDIT && m_pFormItem->getFormPoint(idx)->iok == EIO_INOUT))
				&& (m_pFormItem->getFormPoint(idx)->isEdit == IE_CORGB || m_pFormItem->getFormPoint(idx)->isEdit == IE_NOP)))
			{
				m_errStr = "Edit CoRGB is not able to do!!";
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSERR, (long)m_errStr.operator LPCTSTR());
				break;
			}
			CCoRGB*		coRGB;
			if (m_pFormItem->getFormPoint(idx)->isEdit == IE_CORGB)
			{
				coRGB = new CCoRGB(this, m_pFormItem->getFormStr(idx, "name"), m_pFormItem->getFormPoint(idx)->isEdit,
					HIWORD(m_pFormItem->getFormPoint(idx)->onEdit), LOWORD(m_pFormItem->getFormPoint(idx)->onEdit));
			}
			else
			{
				long lCorgb = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCORGB);
				coRGB = new CCoRGB(this, m_pFormItem->getFormStr(idx, "name"), 
						m_pFormItem->getFormPoint(idx)->isEdit, LOWORD(lCorgb), HIWORD(lCorgb));
			}
			
			if (coRGB->DoModal() == IDOK)
			{
				m_mapH->modified = true;
				do
				{
					if (m_index == -2 && !m_pFormItem->getFormPoint(idx)->bCommon)
					{
						idx++;
						if (idx >= m_pFormItem->getFormCount())
							break;
						continue;
					}

					if (!((m_pFormItem->getFormPoint(idx)->kind == FM_OUT 
						|| (m_pFormItem->getFormPoint(idx)->kind == FM_EDIT && m_pFormItem->getFormPoint(idx)->iok == EIO_INOUT))
						&& (m_pFormItem->getFormPoint(idx)->isEdit == IE_CORGB || m_pFormItem->getFormPoint(idx)->isEdit == IE_NOP)))
					{
						idx++;
						if (idx >= m_pFormItem->getFormCount())
							break;
						continue;
					}
						
					m_pFormItem->getFormPoint(idx)->isEdit = coRGB->m_isEdit;
					if (m_pFormItem->getFormPoint(idx)->isEdit == IE_CORGB)
					{
						m_pFormItem->getFormPoint(idx)->onEdit = MAKELONG(coRGB->m_dnRGB, coRGB->m_upRGB);
						m_pFormItem->getFormPoint(idx)->attr   |= FA_CORGB;
					}
					else
					{
						m_pFormItem->getFormPoint(idx)->onEdit = 0;
						m_pFormItem->getFormPoint(idx)->attr  &= ~FA_CORGB;
					}
					idx++;
					if (idx >= m_pFormItem->getFormCount())
						break;
				}while(m_index == -2);
			}
			delete coRGB;
		}
		break;
	}
	if (!m_GridAttr.multiCol)
		LoadData(m_index, 0);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_index);
	m_bInvalidate = true;
	Invalidate();
}

BOOL CListGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (!m_GridAttr.multiCol)
	{
		if (zDelta < 0)
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEDOWN, 0), 0);
		else
			SendMessage(WM_VSCROLL, MAKEWPARAM(SB_PAGEUP, 0), 0);
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

bool CListGrid::IsGroupSameValue(int nGroup, int fnum, int row)
{
	CString tmp2 = "", tmp = "";
	for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
	{
		if (m_pFormItem->getFormPoint(jj)->group == (struct _formR*)nGroup)
		{
			if (tmp2.IsEmpty())
			{
				tmp2 = getFormValue(fnum, jj, row);
				tmp = tmp2;
			}
			else
			{
				tmp2 = getFormValue(fnum, jj, row);
				if (tmp2.Compare(tmp))
					return false;
			}
		}
	}
	return true;
}

void CListGrid::SetMulti(bool bMulti)
{
	m_GridAttr.multiCol = bMulti;
}

void CListGrid::InvalidateRect2(CPoint pnt)
{
	CRect cellRt;
	GetClientRect(&cellRt);
	if (pnt.x >= 0)
		pnt.x -= m_StartPnt.x;
	if (pnt.y >= 0)
		pnt.y -= m_StartPnt.y;
	if (m_GridAttr.multiCol)
		cellRt.SetRect(m_GridAttr.HeadSz.cx + pnt.x * m_GridAttr.CellSz.cx, 
			m_GridAttr.HeadSz.cy + pnt.y * m_GridAttr.CellSz.cy,
			m_GridAttr.HeadSz.cx + (pnt.x + 1) * m_GridAttr.CellSz.cx, 
			m_GridAttr.HeadSz.cy + (pnt.y + 1) * m_GridAttr.CellSz.cy);
	else
		cellRt.SetRect(m_GridAttr.HeadSz.cx, pnt.y * m_GridAttr.CellSz.cy,
			cellRt.right, (pnt.y + 1) * m_GridAttr.CellSz.cy);
	if (pnt.x < 0)
	{
		cellRt.left = 0;
		cellRt.right = m_GridAttr.HeadSz.cx;
	}
	if (pnt.y < 0)
	{
		cellRt.top = 0;
		cellRt.bottom = m_GridAttr.HeadSz.cy;
	}
	
	if (pnt.x < -1 || pnt.y < -1)
		return;
	DrawBitmap2(pnt.x, pnt.y, cellRt);
	m_bInvalidate = false;
	InvalidateRect(&cellRt);
}

// multi column 관련
void CListGrid::deleteCol(int col)
{
	if (col == -1)
		col = m_SelectedPnt.x;
	if (!m_GridAttr.multiCol || m_SelectedPnt.y != -1 || m_SelectedPnt.x == -1 || m_GridAttr.cols <= 0 || col >= m_GridAttr.cols)
		return;
	
	m_bKeyDown = true;
	EraseCtrls();
	struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	cellItem->removeCell(col);
	delete cellItem;
	m_writeInfo.RemoveAt(col);
	if (m_pFormItem->getFormPoint(m_index)->kind == FM_GRID)
		repR->cols--;
	else
		repR->rows--;
	m_GridAttr.cols--;
	repR->selected--;
	m_SelectedPnt.x--;

	int value;
	for (int ii = col; ii < m_GridAttr.cols; ii++)
	{
		if (m_multiSelMap.Lookup(ii + 1, value))
			m_multiSelMap.SetAt(ii, value);
		else
			m_multiSelMap.RemoveKey(ii);
	}
	while (m_StartPnt.x > 0 && m_GridAttr.cols - m_StartPnt.x < getVisibleCols())
		m_StartPnt.x--;
	//AdjustScroll();
	m_bInvalidate = true;
	Invalidate();
}

int CListGrid::insertCol(int pos)
{
	if (!m_GridAttr.multiCol || pos < -2)
		return -1;

	m_bKeyDown = true;
	EraseCtrls();
	struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
	CCellItem cellItem(repR);
	struct _cellR *cellR;
	if (m_pFormItem->getFormPoint(m_index)->kind == FM_GRID)
	{
		CString temp;
		if (pos == -1 || (pos == -2 && (m_SelectedPnt.x < 0 || m_SelectedPnt.y != -1)))
			pos = cellItem.getCellCount();
		else if (pos == -2)
			pos = m_SelectedPnt.x;
		cellR = cellItem.getCellPoint(cellItem.insertCell(pos));
		cellR->pRGB = getDefaultValue("GRID", "DPRGB", basINPUT);
		cellR->tRGB = getDefaultValue("GRID", "DTRGB", 232);
		cellR->hpRGB = getDefaultValue("GRID", "HPRGB", basPRGB);
		cellR->htRGB = getDefaultValue("GRID", "HTRGB", 232);
		cellR->size = 0;
		cellR->type = IO_NUM;
		cellR->iok = EIO_OUTPUT;
		cellR->kind = CK_NORMAL;
		strcpy_s(cellR->name, getInitSymbol("Col"));
		if (pos == 0)
			cellR->width = getDefaultValue("GRID", "COLWIDTH", 80);
		else
			cellR->width = getDefaultValue("GRID", "COLWIDTH", cellItem.getCellPoint(0)->width);
		cellR->alignC = AL_RIGHT;
		cellR->properties |= PR_VISIBLE;
		
		m_writeInfo.InsertAt(pos, "");
		repR->cols++;
		m_GridAttr.cols++;
		LoadData(m_index, pos);
	}
	else
	{
		CString temp;
		if (pos == -1 || (pos == -2 && (m_SelectedPnt.x < 0 || m_SelectedPnt.y != -1)))
			pos = cellItem.getCellCount();
		else if (pos == -2)
			pos = m_SelectedPnt.x;
		
		cellR = cellItem.getCellPoint(cellItem.insertCell(pos));
		cellR->pRGB = getDefaultValue("TABLE", "DPRGB", basINPUT);
		cellR->tRGB = getDefaultValue("TABLE", "DTRGB", 232);
		cellR->hpRGB = getDefaultValue("TABLE", "HPRGB", basPRGB);
		cellR->htRGB = getDefaultValue("TABLE", "HTRGB", 232);
		cellR->size = 0;
		cellR->type = IO_NUM;
		strcpy_s(cellR->name, getInitSymbol("Row"));
		if (pos == 0)
			cellR->width = getDefaultValue("TABLE", "ROWWIDTH", 80);
		else
			cellR->width = getDefaultValue("TABLE", "ROWWIDTH", cellItem.getCellPoint(0)->width);
		cellR->alignC = AL_RIGHT;
		cellR->properties |= PR_VISIBLE;
		
		m_writeInfo.InsertAt(pos, "");
		LoadData(m_index, pos);
		repR->rows++;
		m_GridAttr.cols++;
	}

	int value;
	for (int ii = m_GridAttr.cols - 2; ii >= pos; ii--)
	{
		if (m_multiSelMap.Lookup(ii, value))
			m_multiSelMap.SetAt(ii + 1, value);
		else
			m_multiSelMap.RemoveKey(ii + 1);
	}
	m_multiSelMap.RemoveKey(pos);

	m_bInvalidate = true;
	Invalidate();

	return pos;
}

void CListGrid::gridMultiLineCheck(int idx)
{
	if (m_pFormItem->getFormPoint(idx)->kind == FM_GRID && (m_pFormItem->getFormPoint(idx)->attr2 & GO_MULTI))
	{
		if (m_pFormItem->getFormPoint(idx)->attr2 & GO_MERGE)
		{
			m_pFormItem->getFormPoint(idx)->attr2 &= ~GO_MULTI;
			return;
		}
		struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(idx)->auxR;
		CCellItem *cellItem = new CCellItem(repR);
		if (repR)
		{
			int	line, colN;
			line = colN = 0;
			for (int ii = 0; ii < repR->cols; ii ++)
			{
				colN++;
				cellItem->getCellPoint(ii)->lineN = (BYTE)line;
				if (cellItem->getCellPoint(ii)->attr & FA_WRAP)
					line++;
			}
		}
		delete cellItem;
	}
}

void CListGrid::SetKeyIn(char nChar)
{
	for (int ii = 0; ii < m_GridAttr.rows; ii++)
	{
		if (atoi(getValue2(ii)) == 1)
		{
			m_SelectedPnt.y = ii;
			m_SelectedPnt.x = 0;
			AdjustScroll();
			m_bInvalidate = true;
			Invalidate();
			m_bKeyDown = true;
			SetInputState();
			if (m_pEdit)
			{
				m_pEdit->PostMessage(WM_KEYDOWN, nChar, (LPARAM)this);
				m_pEdit->SetFocus();
			}
			break;
		}
	}
}

void CListGrid::AdjustScroll()
{
	if (m_GridAttr.rows < 0 || m_GridAttr.cols < 0)
		return;
	if (m_GridAttr.multiCol)
	{
		if (m_SelectedPnt.x + 1 > m_StartPnt.x + getVisibleCols())
		{
			OnHScroll(SB_LINEDOWN, 0, NULL);
			m_bInvalidate = true;
			Invalidate();
		}
		else if (m_SelectedPnt.x < m_StartPnt.x)
		{
			OnHScroll(SB_THUMBTRACK, 0, NULL);
			m_bInvalidate = true;
			Invalidate();
		}
	}
	while(m_StartPnt.y < m_GridAttr.rows - 1 && m_SelectedPnt.y + 2 > m_StartPnt.y + getVisibleRows())
		OnVScroll(SB_LINEDOWN, 0, NULL);

	while(m_StartPnt.y > 0 && m_SelectedPnt.y < m_StartPnt.y)
		OnVScroll(SB_LINEUP, 0, NULL);

	m_bInvalidate = true;
	Invalidate();
}

void CListGrid::refreshData()
{
	int lastRow = m_StartPnt.y + getVisibleRows();
	for (int ii = m_StartPnt.y; ii <= lastRow; ii++)
	{
		LoadData(m_index, 0, ii);
		InvalidateRect2(CPoint(0, ii));
	}
}

bool CListGrid::duplicateSym(CString sym)
{
	CString symbolName = "\t";
	for (int kk = 0; kk < m_pFormItem->getFormCount(); kk++)
	{
		symbolName += m_pFormItem->getFormStr(kk, "name");
		symbolName += '\t';
	}
	if (symbolName.Find("\t" + sym + "\t") >= 0)
		return true;
	return false;
}

CString CListGrid::getInitSymbol(CString sSymbol)
{
	CString tSym, sSymbolList;
	int	initN;
	struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
	if (!repR)
		return sSymbol;
	CCellItem cellItem(repR);

	for (int kk = 0; kk < cellItem.getCellCount(); kk++)
	{
		if (cellItem.getCellPoint(kk)->name[0] != NULL)
		{
			sSymbolList += cellItem.getCellPoint(kk)->name;
			sSymbolList += '\t';
		}
	}

	initN = 0;
	sSymbolList.MakeUpper();
	for (;;)
	{
		tSym.Format("%s%d", sSymbol, initN);
		tSym.MakeUpper();
		if (sSymbolList.Find(tSym + "\t") == -1)
			break;
		else
			initN++;
	}
	
	tSym.Format("%s%d", sSymbol, initN);
	return tSym;
}

void CListGrid::RedrawGrid()
{
	m_bInvalidate = true;
	Invalidate();
}

int CListGrid::getDefaultValue(CString sSection, CString sKey, int nDefault)
{
	CString sTabDir;
	sTabDir.Format("%s\\tab\\", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR));
	return GetPrivateProfileInt(sSection, sKey, nDefault, sTabDir + "awDefault.ini");
}

void CListGrid::UpdateCombo()
{
	if (m_pCombo)
	{
		int sel = m_pCombo->m_pCombo->FindStringExact(0, getData(m_InputPnt.x, m_InputPnt.y));
		if (sel < 0)
			sel = 0;
		if (m_pCombo->m_pCombo->GetCount())
			m_pCombo->m_pCombo->SetCurSel(sel);
	}
}

void CListGrid::SetCellSize(long cx, long cy)
{
	m_GridAttr.CellSz.cx = cx;
	m_GridAttr.CellSz.cy = cy;
}


void CListGrid::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_SPACE)
	{
		if (m_SelectedPnt.y == -1 && m_SelectedPnt.x >= 0)
		{
			int value;
			if (m_multiSelMap.Lookup(m_SelectedPnt.x, value))
				m_multiSelMap.RemoveKey(m_SelectedPnt.x);
			else
				m_multiSelMap.SetAt(m_SelectedPnt.x, 1);
		}
	}
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CListGrid::SetLikeCell(int nCol, int nSrcCol/* = 0*/)
{
	if (getType(m_SelectedPnt.y) != 4)
	{
		CString tmp = getData(nSrcCol, m_SelectedPnt.y);
		setData(tmp, nCol, m_SelectedPnt.y);
		SaveData(m_index, nCol, m_SelectedPnt.y, false);
		InvalidateRect2(CPoint(nCol, m_SelectedPnt.y));
	}
	else
	{
		switch(atoi(getValue2(m_SelectedPnt.y)))
		{
		case 1004:	// attributes
			{
				struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
				
				CCellItem cellItem(repR);
				CAttrDlg attrDlg(this, m_pFormItem->getFormPoint(m_index)->kind, cellItem.getCellPoint(nSrcCol)->type, cellItem.getCellPoint(nSrcCol)->iok,
					cellItem.getCellPoint(nSrcCol)->attr, cellItem.getCellPoint(nSrcCol)->attr2);
				
				cellItem.getCellPoint(nCol)->attr = attrDlg.m_attr;
				cellItem.getCellPoint(nCol)->attr2 = attrDlg.m_attr2;
			}
			break;
		case 1007:	// edit format
			{
				struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_index)->auxR;
				CCellItem cellItem(repR);
				
				CCellEdit cellEdit(this, FM_GRID, cellItem.getCellPoint(nSrcCol)->type, 
					cellItem.getCellPoint(nSrcCol)->isEdit,
					cellItem.getCellPoint(nSrcCol)->onEdit, 
					cellItem.getCellPoint(nSrcCol)->editS,  
					cellItem.getCellPoint(nSrcCol)->name,
					cellItem.getCellPoint(nSrcCol)->attr,   
					cellItem.getCellPoint(nSrcCol)->size);
				
				cellItem.getCellPoint(nCol)->attr   = cellEdit.m_attr;
				cellItem.getCellPoint(nCol)->isEdit = cellEdit.m_isEdit;
				cellItem.getCellPoint(nCol)->onEdit = cellEdit.m_onEdit;
				strcpy_s(cellItem.getCellPoint(nCol)->editS, (char *)cellEdit.m_format.operator LPCTSTR());
			}
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//
//	event message
//
/////////////////////////////////////////////////////////////////////////////
LRESULT CListGrid::OnMultiSel(WPARAM wParam, LPARAM lParam)
{
	int col = wParam;
	int value, key;
	if (m_multiSelMap.Lookup(col, value) && m_multiSelMap.GetCount())
	{
		POSITION pos = m_multiSelMap.GetStartPosition();
		while (pos)
		{
			m_multiSelMap.GetNextAssoc(pos, key, value);
			if (key != col)
				SetLikeCell(key, col);
		}
	}
	return 0;
}

LRESULT CListGrid::OnEraseCtrl(WPARAM wParam, LPARAM lParam)
{
	if (wParam == VK_ESCAPE || wParam == VK_RETURN)
		EraseCtrls();
	return 0;
}


