#include "stdafx.h"
#include "IB300200.h"
#include "JangoWnd.h"
#include "ControlWnd.h"
#include "../../control/fx_grid/memdc.h"
#include "../../h/jmcode.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../control/fx_misc/fxImgButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_CTRL_FUTURE1		0x01
#define ID_CTRL_FUTURE2		0x02
#define ID_CTRL_FUTURE3		0x03
#define ID_CTRL_FUTURE4		0x04
#define ID_CTRL_JSCROLL		0x05
#define ID_COMBO_OPTION		0x06
#define ID_CTRL_FUTURE5		0x07
#define ID_CTRL_FUTURE6		0x08
#define ID_CTRL_FUTURE7		0x09

#define CELL_HEIGHT	19
#define	SCROLL_WIDTH	16
#define STANDARDCNT		11	//2014.08.11 KSJ 

CJangoWnd::CJangoWnd(CWnd* pParent, CMapWnd* pMapWnd, CWnd* pView)
{
	m_pParent = pParent;
	m_pMapWnd = pMapWnd;
	m_pView = pView;

	m_iRowCnt = m_row = 0;
	m_bLBDown = false;

	for (int ii = 0; ii < STANDARDCNT; ii++)
	{
		m_pBtnFuture[ii] = nullptr;
		m_atmIdx[ii] = 0;
	}
}

CJangoWnd::~CJangoWnd()
{
	for (int ii = 0; ii < m_pFCode.GetSize(); ii++)
	{
		struct _Code *fcode = m_pFCode.GetAt(ii);
		delete fcode;	fcode = NULL;
	}
	m_pFCode.RemoveAll();
	
	for (int ii = 0; ii < STANDARDCNT; ii++)
	{
		for (int jj = 0; jj < m_pOCode[ii].GetSize(); jj++)
		{
			struct _OCode *ocode = m_pOCode[ii].GetAt(jj);
			delete ocode;	ocode = NULL;
		}
		m_pOCode[ii].RemoveAll();
	}

	for (int ii = 0; ii < 7; ii++)
	{
		if (m_pBtnFuture[ii])
		{
			m_pBtnFuture[ii] = nullptr;
		}
	}
}


BEGIN_MESSAGE_MAP(CJangoWnd, CWnd)
	//{{AFX_MSG_MAP(CJangoWnd)
	ON_BN_CLICKED(ID_CTRL_FUTURE1, OnButtonFuture1)
	ON_BN_CLICKED(ID_CTRL_FUTURE2, OnButtonFuture2)
	ON_BN_CLICKED(ID_CTRL_FUTURE3, OnButtonFuture3)
	ON_BN_CLICKED(ID_CTRL_FUTURE4, OnButtonFuture4)
	ON_BN_CLICKED(ID_CTRL_FUTURE5, OnButtonFuture5)
	ON_BN_CLICKED(ID_CTRL_FUTURE6, OnButtonFuture6)
	ON_BN_CLICKED(ID_CTRL_FUTURE7, OnButtonFuture7)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELENDOK(ID_COMBO_OPTION, OnOptSelEndOk)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CJangoWnd::Init(CString home, CString user)
{
	m_home = home;
	m_user = user;

	FCodeLoad(m_home + "\\tab\\");

	CRect rc, cRc;
	GetClientRect(&rc);

	m_iRowCnt = m_row = MAX_ROW;
	m_iRowH = 19;

	const int cx = INNER_FRAME_SPACE, cy = 0;
	cRc.SetRect(cx, cy, cx + 40, cy + CTRL_HEIGHT);

	m_pBoldFont = GetAxFont("굴림", 9, false, FW_BOLD);
	m_pFont = GetAxFont("굴림", 9);

	CString str;
	for (int ii = 0; ii < 7; ii++)
	{
		struct	_Code *fCode = m_pFCode.GetAt(ii);
		str = fCode->Name.Right(4);
		switch (ii)
		{
		case 0:
			cRc.SetRect(cx, cRc.top, cx + 36, cRc.bottom);
			m_pBtnFuture[ii] = std::make_unique<CfxImgButton>();
			m_pBtnFuture[ii]->Create("\n" + fCode->Name, cRc, this, ID_CTRL_FUTURE1, FALSE, TRUE);
			m_pBtnFuture[ii]->SetWindowText(str);
			m_pBtnFuture[ii]->SetFont(m_pFont);
			break;
		case 1:
			cRc.OffsetRect(37, 0);
			m_pBtnFuture[ii] = std::make_unique<CfxImgButton>();
			m_pBtnFuture[ii]->Create("\n" + fCode->Name, cRc, this, ID_CTRL_FUTURE2, FALSE, TRUE);
			m_pBtnFuture[ii]->SetWindowText(str);
			m_pBtnFuture[ii]->SetFont(m_pFont);
			break;
		case 2:
			cRc.OffsetRect(37, 0);
			m_pBtnFuture[ii] = std::make_unique<CfxImgButton>();
			m_pBtnFuture[ii]->Create("\n" + fCode->Name, cRc, this, ID_CTRL_FUTURE3, FALSE, TRUE);
			m_pBtnFuture[ii]->SetWindowText(str);
			m_pBtnFuture[ii]->SetFont(m_pFont);
			break;
		case 3:
			cRc.OffsetRect(37, 0);
			m_pBtnFuture[ii] = std::make_unique<CfxImgButton>();
			m_pBtnFuture[ii]->Create("\n" + fCode->Name, cRc, this, ID_CTRL_FUTURE4, FALSE, TRUE);
			m_pBtnFuture[ii]->SetWindowText(str);
			m_pBtnFuture[ii]->SetFont(m_pFont);
			break;
		case 4:
			cRc.SetRect(cx, cRc.bottom + 2, cx + 36, cRc.bottom + CTRL_HEIGHT + 2);
			m_pBtnFuture[ii] = std::make_unique<CfxImgButton>();
			m_pBtnFuture[ii]->Create("\n" + fCode->Name, cRc, this, ID_CTRL_FUTURE5, FALSE, TRUE);
			m_pBtnFuture[ii]->SetWindowText(str);
			m_pBtnFuture[ii]->SetFont(m_pFont);
			break;
		case 5:
			cRc.OffsetRect(37, 0);
			m_pBtnFuture[ii] = std::make_unique<CfxImgButton>();
			m_pBtnFuture[ii]->Create("\n" + fCode->Name, cRc, this, ID_CTRL_FUTURE6, FALSE, TRUE);
			m_pBtnFuture[ii]->SetWindowText(str);
			m_pBtnFuture[ii]->SetFont(m_pFont);
			break;
		case 6:
			cRc.OffsetRect(37, 0);
			m_pBtnFuture[ii] = std::make_unique<CfxImgButton>();
			m_pBtnFuture[ii]->Create("\n" + fCode->Name, cRc, this, ID_CTRL_FUTURE7, FALSE, TRUE);
			m_pBtnFuture[ii]->SetWindowText(str);
			m_pBtnFuture[ii]->SetFont(m_pFont);
			break;
		}
	}
	setButtonBitmaps();

	cRc.SetRect(cx, cRc.bottom+1, cx + 34, cRc.bottom + 1+CTRL_HEIGHT-4);
	m_oRc1 = cRc;		cRc.OffsetRect(37, 0);
	m_oRc2 = cRc;		cRc.OffsetRect(37, 0);
	m_oRc3 = cRc;		cRc.OffsetRect(37, 0);
	m_oRc4 = cRc;		cRc.OffsetRect(37, 0);

	// 옵션 콤보
	cRc.SetRect(cx, cRc.top, CTRL_WIDTH - INNER_FRAME_SPACE-INNER_FRAME_SPACE, cRc.bottom + 202);
	m_cbOption.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST, cRc, this, ID_COMBO_OPTION);
	m_cbOption.SetFont(m_pFont);
	OCodeLoad(m_home + "\\tab\\");


	// scroll
	m_rcCombo.SetRect(rc.right - SCROLL_WIDTH, cRc.top + CTRL_HEIGHT + 2, rc.right - 1, rc.bottom);
	m_vbar.Create(WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_RIGHTALIGN, 
		m_rcCombo, this, ID_CTRL_JSCROLL);
	m_tRc2.SetRect(0, m_rcCombo.top, rc.right - SCROLL_WIDTH, m_rcCombo.bottom);
	setScroll();
}

void CJangoWnd::setButtonBitmaps()
{
	CString path = "";
	path.Format("%s\\%s\\", m_home, IMAGEDIR);

	m_hBitmap    = GetAxBitmap(path + "1btn.bmp")->operator HBITMAP();
	m_hBitmap_dn = GetAxBitmap(path + "1btn_dn.bmp")->operator HBITMAP();
	m_hBitmap_hv = GetAxBitmap(path + "1btn_en.bmp")->operator HBITMAP();

	m_pBtnFuture[0]->SetImgBitmap(m_hBitmap, m_hBitmap_dn, m_hBitmap_hv);
	m_pBtnFuture[1]->SetImgBitmap(m_hBitmap, m_hBitmap_dn, m_hBitmap_hv);
	m_pBtnFuture[2]->SetImgBitmap(m_hBitmap, m_hBitmap_dn, m_hBitmap_hv);
	m_pBtnFuture[3]->SetImgBitmap(m_hBitmap, m_hBitmap_dn, m_hBitmap_hv);
	m_pBtnFuture[4]->SetImgBitmap(m_hBitmap, m_hBitmap_dn, m_hBitmap_hv);
	m_pBtnFuture[5]->SetImgBitmap(m_hBitmap, m_hBitmap_dn, m_hBitmap_hv);
	m_pBtnFuture[6]->SetImgBitmap(m_hBitmap, m_hBitmap_dn, m_hBitmap_hv);
}

void CJangoWnd::OnButtonFuture1()
{
	const int index = 0;
	if (index >= m_pFCode.GetSize())
		return ;

	struct _Code *fcode = m_pFCode.GetAt(index);
	m_pMapWnd->SetCode(fcode->Code);
}

void CJangoWnd::OnButtonFuture2()
{
	const int index = 1;
	if (index >= m_pFCode.GetSize())
		return;

	struct _Code *fcode = m_pFCode.GetAt(index);
	m_pMapWnd->SetCode(fcode->Code);
}

void CJangoWnd::OnButtonFuture3()
{
	const int index = 2;
	if (index >= m_pFCode.GetSize())
		return;

	struct _Code *fcode = m_pFCode.GetAt(index);
	m_pMapWnd->SetCode(fcode->Code);
}

void CJangoWnd::OnButtonFuture4()
{
	const int index = 3;
	if (index >= m_pFCode.GetSize())
		return;

	struct _Code *fcode = m_pFCode.GetAt(index);
	m_pMapWnd->SetCode(fcode->Code);
}

void CJangoWnd::OnButtonFuture5()
{
	const int index = 4;
	if (index >= m_pFCode.GetSize())
		return ;
	
	struct _Code *fcode = m_pFCode.GetAt(index);
	m_pMapWnd->SetCode(fcode->Code);
}

void CJangoWnd::OnButtonFuture6()
{
	const int index = 5;
	if (index >= m_pFCode.GetSize())
		return ;
	
	struct _Code *fcode = m_pFCode.GetAt(index);
	m_pMapWnd->SetCode(fcode->Code);
}

void CJangoWnd::OnButtonFuture7()
{
	const int index = 6;
	if (index >= m_pFCode.GetSize())
		return ;
	
	struct _Code *fcode = m_pFCode.GetAt(index);
	m_pMapWnd->SetCode(fcode->Code);
	
}

void CJangoWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBDown = true;
	CWnd::OnLButtonDown(nFlags, point);
}

void CJangoWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bLBDown)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}
	
	m_bLBDown = false;

	if (m_oRc1.PtInRect(point))
	{
		struct _Code *fcode = m_pFCode.GetAt(0);
	//	((CControlWnd *)m_pParent)->SetJmcn(fcode->Code, fcode->Jango);
		return;
	}
	else if (m_oRc2.PtInRect(point))
	{
		struct _Code *fcode = m_pFCode.GetAt(1);
		//	((CControlWnd *)m_pParent)->SetJmcn(fcode->Code, fcode->Jango);
		return;
	}
	else if (m_oRc3.PtInRect(point))
	{
		struct _Code *fcode = m_pFCode.GetAt(2);
		//	((CControlWnd *)m_pParent)->SetJmcn(fcode->Code, fcode->Jango);
		return;
	}
	else if (m_oRc4.PtInRect(point))
	{
		struct _Code *fcode = m_pFCode.GetAt(3);
		//	((CControlWnd *)m_pParent)->SetJmcn(fcode->Code, fcode->Jango);
		return; 
	}
	else if (m_oRc5.PtInRect(point))
	{
		struct _Code *fcode = m_pFCode.GetAt(4);
		//	((CControlWnd *)m_pParent)->SetJmcn(fcode->Code, fcode->Jango);
		return;
	}
	else if (m_oRc6.PtInRect(point))
	{
		struct _Code *fcode = m_pFCode.GetAt(5);
		//	((CControlWnd *)m_pParent)->SetJmcn(fcode->Code, fcode->Jango);
		return;
	}
	else if (m_oRc7.PtInRect(point))
	{
		struct _Code *fcode = m_pFCode.GetAt(6);
		//	((CControlWnd *)m_pParent)->SetJmcn(fcode->Code, fcode->Jango);
		return; 
	}

	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	const int	nPos = info.nPos;
	int	index = m_cbOption.GetCurSel();
	
	struct _OCode* ocode = NULL;
	for (int ii = 1; ii <= m_row; ii++)
	{
		if (m_rcCall[ii].PtInRect(point))
		{
			struct _OCode otmp;
			if (ii + nPos <= m_pOCode[index].GetSize())
			{
				ocode = m_pOCode[index].GetAt(nPos + ii - 1);
			//	((CControlWnd *)m_pParent)->SetJmcn(ocode->Call.Code, ocode->Call.Jango);
				break;
			}
			else
				break;
		}
		else if (m_rcPut[ii].PtInRect(point))
		{
			struct _OCode otmp;
			if (ii + nPos <= m_pOCode[index].GetSize())
			{
				ocode = m_pOCode[index].GetAt(nPos + ii - 1);
			//	((CControlWnd *)m_pParent)->SetJmcn(ocode->Put.Code, ocode->Put.Jango);
				break;
			}
			else
				break;
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CJangoWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	const int	nPos = info.nPos;
	int	index = m_cbOption.GetCurSel();
	
	struct _OCode* ocode = NULL;
	for (int ii = 1; ii <= m_row; ii++)
	{
		if (m_rcCall[ii].PtInRect(point))
		{
			struct _OCode otmp;
			if (ii + nPos <= m_pOCode[index].GetSize())
			{
				ocode = m_pOCode[index].GetAt(nPos + ii - 1);
				m_pMapWnd->SetCode(ocode->Call.Code);
				break;
			}
			else
				break;
		}
		else if (m_rcPut[ii].PtInRect(point))
		{
			struct _OCode otmp;
			if (ii + nPos <= m_pOCode[index].GetSize())
			{
				ocode = m_pOCode[index].GetAt(nPos + ii - 1);
				m_pMapWnd->SetCode(ocode->Put.Code);
				break;
			}
			else
				break;
		}
	}
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

BOOL CJangoWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CJangoWnd::OnPaint() 
{
	CPaintDC dc(this); 

	CRect rc;
	GetClientRect(&rc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pbitmapOld = memDC.SelectObject(&bitmap);

	SCROLLINFO	info{};
	int	nPos{}, col[3]{};
	const int height = 18;
	struct _Code *fcode = NULL;

	const int bkMode = memDC.SetBkMode(TRANSPARENT);

	memDC.FillSolidRect(rc, GetIndexColor(CONTENTS_BACK_INDEX));			
//	memDC.FillSolidRect(rc, 0xFF0000);	

	CPen *pPen = GetAxPen(GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	CPen *pOldPen = memDC.SelectObject(pPen);

	CString strDraw;

	memDC.FillSolidRect(&m_oRc1, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc1);
	fcode = m_pFCode.GetAt(0);
	if (fcode->Micheg==0) strDraw = fcode->Jango;
	else strDraw.Format("%s(%d)", fcode->Jango, fcode->Micheg);
	DrawSiseText(&memDC, strDraw, m_oRc1, DT_RIGHT, ST_COLOR | ST_REMOVE);

	memDC.FillSolidRect(&m_oRc2, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc2);
	fcode = m_pFCode.GetAt(1);
	if (fcode->Micheg==0) strDraw = fcode->Jango;
	else strDraw.Format("%s(%d)", fcode->Jango, fcode->Micheg);
	DrawSiseText(&memDC, strDraw, m_oRc2, DT_RIGHT, ST_COLOR | ST_REMOVE);

	memDC.FillSolidRect(&m_oRc3, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc3);
	fcode = m_pFCode.GetAt(2);
	if (fcode->Micheg==0) strDraw = fcode->Jango;
	else strDraw.Format("%s(%d)", fcode->Jango, fcode->Micheg);
	DrawSiseText(&memDC, strDraw, m_oRc3, DT_RIGHT, ST_COLOR | ST_REMOVE);

	memDC.FillSolidRect(&m_oRc4, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc4);
	fcode = m_pFCode.GetAt(3);
	if (fcode->Micheg==0) strDraw = fcode->Jango;
	else strDraw.Format("%s(%d)", fcode->Jango, fcode->Micheg);
	DrawSiseText(&memDC, strDraw, m_oRc4, DT_RIGHT, ST_COLOR | ST_REMOVE);

	memDC.FillSolidRect(&m_oRc5, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc5);
	fcode = m_pFCode.GetAt(4);
	if (fcode->Micheg==0) strDraw = fcode->Jango;
	else strDraw.Format("%s(%d)", fcode->Jango, fcode->Micheg);
	DrawSiseText(&memDC, strDraw, m_oRc5, DT_RIGHT, ST_COLOR | ST_REMOVE);
	
	memDC.FillSolidRect(&m_oRc6, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc6);
	fcode = m_pFCode.GetAt(5);
	if (fcode->Micheg==0) strDraw = fcode->Jango;
	else strDraw.Format("%s(%d)", fcode->Jango, fcode->Micheg);
	DrawSiseText(&memDC, strDraw, m_oRc6, DT_RIGHT, ST_COLOR | ST_REMOVE);
	
	memDC.FillSolidRect(&m_oRc7, GetIndexColor(COLOR_DATA));	memDC.Rectangle(&m_oRc7);
	fcode = m_pFCode.GetAt(6);
	if (fcode->Micheg==0) strDraw = fcode->Jango;
	else strDraw.Format("%s(%d)", fcode->Jango, fcode->Micheg);
	DrawSiseText(&memDC, strDraw, m_oRc7, DT_RIGHT, ST_COLOR | ST_REMOVE);

	if (m_vbar.IsWindowVisible())
	{
		col[0] = (rc.Width() - SCROLL_WIDTH) / 3;
		col[1] = col[0] + (rc.Width() - SCROLL_WIDTH) / 3 - 2;
		col[2] = rc.Width() - SCROLL_WIDTH - 1;
	}
	else
	{
		col[0] = rc.Width() / 3;
		col[1] = col[0] + rc.Width() / 3 - 2;
		col[2] = rc.right - 1;
	}

	int index = m_cbOption.GetCurSel();

	CRect wrc;
	m_cbOption.GetWindowRect(&wrc);
	ScreenToClient(wrc);
	const int itop = wrc.bottom + GAP;

	m_rcCall[0].SetRect(0, itop, col[0], itop + m_iRowH);
	m_rcHsga[0].SetRect(col[0] - 1, itop, col[1], itop + m_iRowH);
	m_rcPut[0].SetRect(col[1] - 1, itop, col[2], itop + m_iRowH);

//	m_rcCall[0].SetRect(0, m_oRc4.bottom + 23, col[0], m_oRc4.bottom + 23 + height);
//	m_rcHsga[0].SetRect(col[0] - 1, m_oRc4.bottom + 23, col[1], m_oRc4.bottom + 23 + height);
//	m_rcPut[0].SetRect(col[1] - 1, m_oRc4.bottom + 23, col[2], m_oRc4.bottom + 23 + height);

	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	nPos = info.nPos;
	struct _OCode* ocode = NULL;
	int	jj = 0;

	for (int ii = nPos; ii <= nPos + m_row; ii++)
	{
		struct _OCode otmp;
		if (ii <= m_pOCode[index].GetSize() && ii > 0)
			ocode = m_pOCode[index].GetAt(ii - 1);
		else
		{
			ocode = &otmp;
			ocode->atm = false;
		}

		if (ii == nPos)
		{
			memDC.FillSolidRect(&m_rcCall[jj], GetIndexColor(COLOR_MSB));	
			memDC.Rectangle(&m_rcCall[jj]);
			

			DrawSiseText(&memDC, "Call", m_rcCall[jj], DT_CENTER);
			memDC.FillSolidRect(&m_rcHsga[jj], GetIndexColor(COLOR_GRIDHEAD));	
			memDC.Rectangle(&m_rcHsga[jj]);
			DrawSiseText(&memDC, "행사가", m_rcHsga[jj], DT_CENTER);

			memDC.FillSolidRect(&m_rcPut[jj], GetIndexColor(COLOR_MDB));	
			memDC.Rectangle(&m_rcPut[jj]);
			DrawSiseText(&memDC, "Put", m_rcPut[jj], DT_CENTER);
		}
		else
		{
			memDC.FillSolidRect(&m_rcCall[jj], GetIndexColor(COLOR_MSB));	
			memDC.Rectangle(&m_rcCall[jj]);
			if (ocode->Call.Micheg==0) strDraw = ocode->Call.Jango;
			else strDraw.Format("%s(%d)", ocode->Call.Jango, ocode->Call.Micheg);
			DrawSiseText(&memDC, strDraw, &m_rcCall[jj], DT_RIGHT, ST_COLOR | ST_REMOVE);

			memDC.FillSolidRect(&m_rcHsga[jj], GetIndexColor(COLOR_DATA));	
			memDC.Rectangle(&m_rcHsga[jj]);
			if (ocode->atm)
				DrawSiseText(&memDC, "+" + ocode->Hsga, &m_rcHsga[jj], DT_CENTER, ST_COLOR | ST_REMOVE);
			else
				DrawSiseText(&memDC, ocode->Hsga, &m_rcHsga[jj], DT_CENTER);

			memDC.FillSolidRect(&m_rcPut[jj], GetIndexColor(COLOR_MDB));	
			memDC.Rectangle(&m_rcPut[jj]);
			if (ocode->Put.Micheg==0) strDraw = ocode->Put.Jango;
			else strDraw.Format("%s(%d)", ocode->Put.Jango, ocode->Put.Micheg);
			DrawSiseText(&memDC, strDraw, &m_rcPut[jj], DT_RIGHT, ST_COLOR | ST_REMOVE);
		}

		if (jj + 1 <= m_row)
		{
			m_rcCall[jj + 1] = m_rcCall[jj];	
			m_rcCall[jj + 1].top = m_rcCall[jj].bottom - 1 ;
			m_rcCall[jj + 1].bottom = m_rcCall[jj + 1].top + height ;
			//m_rcCall[jj + 1].OffsetRect(0, height - 1);

			m_rcHsga[jj + 1] = m_rcHsga[jj];	
			m_rcHsga[jj + 1].top = m_rcHsga[jj].bottom - 1;
			m_rcHsga[jj + 1].bottom = m_rcHsga[jj + 1].top + height ;

		//	m_rcHsga[jj + 1].OffsetRect(0, height - 1);

			m_rcPut[jj + 1] = m_rcPut[jj];		
			m_rcPut[jj + 1].top = m_rcPut[jj].bottom - 1 ;
			m_rcPut[jj + 1].bottom = m_rcPut[jj + 1].top + height ;

			//m_rcPut[jj + 1].OffsetRect(0, height - 1);
		}
		jj++;
	}
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pOldPen);
	memDC.SetBkMode(bkMode);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

bool CJangoWnd::FCodeLoad(CString tabPath)
{
	if (m_pFCode.GetSize() > 0)
		return true;

	char	buf[1024]{};
	int	res=0;
	DWORD	dw = 0;
	int	bufsize = 0;
	struct	fjcode	*pFjCode=nullptr;
	
	CString path = tabPath + "fjcode.dat";
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		::MessageBox(NULL, "선물 화일을 읽을 수 없습니다!", "선물옵션 클릭주문", MB_ICONWARNING);
		CloseHandle(fh);
		return false;
	}
	
	gsl::owner<struct	_Code*>	fCode = NULL;
	bufsize = sizeof(struct fjcode);
	for (int ii = 0; ii < 7; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pFjCode = (struct fjcode *)buf;

		fCode = new struct _Code;
		fCode->Code = CString(pFjCode->cod2, FCodeLen);
		fCode->Code.TrimRight();
		fCode->Name = CString(pFjCode->hnam, FNameLen);
		fCode->Name.TrimRight();
		fCode->Micheg = 0;
		m_pFCode.Add(fCode);
	}

	CloseHandle(fh);

	return true;
}

bool CJangoWnd::OCodeLoad(CString tabPath)
{
	CFile file;
	struct  ojcodh  OJCodh;
	struct  ojcode  OJCode;
	ULONGLONG codeN=0, len=0;
	
	CString	path = tabPath + "opcode.dat";
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		MessageBox("파일이 존재하지 않습니다.");
		return FALSE;
	}
	len = file.Read(&OJCodh, sizeof(struct ojcodh));
	
	for (int jj = 0; jj < STANDARDCNT; jj++)
	{
		CString str = CString(OJCodh.cjym[jj], 6).Right(4);
		m_cbOption.InsertString(0, str);
	}
	m_cbOption.SetCurSel(0);


	CString	tmp;
	int	res{};
	bool bChkAtm[11]{};
	bChkAtm[0] = bChkAtm[1] = bChkAtm[2] = bChkAtm[3] = bChkAtm[4] = bChkAtm[5] = false;
	bChkAtm[6] = bChkAtm[7] = bChkAtm[8] = bChkAtm[9] = bChkAtm[10] = false;

	codeN = (file.GetLength() - len) / sizeof(struct ojcode);
	
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&OJCode, sizeof(struct ojcode));
		
		tmp = CString(OJCode.price, OPriceLen);
		oCode.Price.Format("%3.1f", float(atoi(tmp)/ 100.0));
		oCode.Atm = OJCode.atmg;

		for (int jj = 0; jj < STANDARDCNT; jj++)
		{
			oCode.Call[jj].Exist = OJCode.call[10-jj].yorn;
			oCode.Call[jj].Code = CString(OJCode.call[10-jj].cod2, OCodeLen -1);	//2014.06.27 KSJ 여기서 9로 세팅해주면 GetLength() 함수에서 사이즈가 9로 나온다.
			oCode.Call[jj].Name = CString(OJCode.call[10-jj].hnam, ONameLen);
			res = oCode.Call[jj].Name.Find('\0');
			if (res != -1)
				oCode.Call[jj].Name = oCode.Call[jj].Name.Left(res);
			oCode.Call[jj].Name.TrimRight();
			
			oCode.Put[jj].Exist = OJCode.put[jj].yorn;
			oCode.Put[jj].Code = CString(OJCode.put[jj].cod2, OCodeLen -1);		//2014.06.27 KSJ 여기서 9로 세팅해주면 GetLength() 함수에서 사이즈가 9로 나온다.
			oCode.Put[jj].Name = CString(OJCode.put[jj].hnam, ONameLen);
			res = oCode.Put[jj].Name.Find('\0');
			if (res != -1)
				oCode.Put[jj].Name = oCode.Put[jj].Name.Left(res);
			oCode.Put[jj].Name.TrimRight();
			
			if (oCode.Call[jj].Exist == '1' && oCode.Put[jj].Exist == '1')
			{
				gsl::owner<struct _OCode*> tOCode = new struct _OCode;
				
				tOCode->Hsga = oCode.Price;
				
				if (!bChkAtm[jj])
					m_atmIdx[jj]++;
				
				if (OJCode.atmg == 1)
				{
					tOCode->atm = true;
					bChkAtm[jj] = true;
				}
				else 
					tOCode->atm = false;
				
				tOCode->Call.Code = oCode.Call[jj].Code;
				tOCode->Call.Name = oCode.Call[jj].Name;
				tOCode->Call.Micheg = 0;
				
				tOCode->Put.Code = oCode.Put[jj].Code;
				tOCode->Put.Name = oCode.Put[jj].Name;
				tOCode->Put.Micheg = 0;
				
// 				OutputDebugString("[KSJ] [" + oCode.Call[jj].Code + "]");
				m_pOCode[jj].InsertAt(0, tOCode);
			}
		}
	}
	
	file.Close();
	return true;
}

void CJangoWnd::setScroll()
{
	CRect rc;
	GetClientRect(&rc);

	const int index = m_cbOption.GetCurSel();
	if (index < 0 || index > 10)
		return;
	const int nPage = (int)m_row, nMax = m_pOCode[index].GetSize() - 1;
	int nPos = nMax - m_atmIdx[index]-2;
	if (nPos < 0 || nPos > nMax)	nPos = 0;
	SCROLLINFO	info;
	
	info.cbSize	= sizeof(SCROLLINFO);
	info.fMask	= SIF_ALL | SIF_DISABLENOSCROLL; 
	info.nMin	= 0;
	info.nMax	= nMax;
	info.nPage	= nPage;
	info.nPos	= nPos;
	info.nTrackPos	= 0;

	m_vbar.SetScrollInfo(&info);
	if (nMax < nPage)
		m_vbar.ShowWindow(SW_HIDE);
	else
		m_vbar.ShowWindow(SW_SHOW);

	Invalidate();
}

void CJangoWnd::OnOptSelEndOk()
{
	setScroll();
	Invalidate();
}

void CJangoWnd::SetMicheg(CString code, int cnt)
{
	if (!code.IsEmpty() && code.GetAt(0)=='1')
	{
		for (int ii = 0; ii < m_pFCode.GetSize(); ii++)
		{
			struct _Code* fcode = m_pFCode.GetAt(ii);
			if (fcode->Code == code)
			{
				fcode->Micheg = cnt;
				return;
			}
		}
	}
	for (int ii = 0; ii < STANDARDCNT; ii++)
	{
		for (int jj = 0; jj < m_pOCode[ii].GetSize(); jj++)
		{
			struct _OCode* ocode = m_pOCode[ii].GetAt(jj);
			if (ocode->Call.Code == code)
			{
				ocode->Call.Micheg = cnt;
				return;
			}
			else if (ocode->Put.Code == code)
			{
				ocode->Put.Micheg = cnt;
				return;
			}
		}
	}
}

void CJangoWnd::SetJango(CString code, CString jgvl, CString pgdg)
{
	if (!code.IsEmpty() && code.GetAt(0) == '1')
	{
		for (int ii = 0; ii < m_pFCode.GetSize(); ii++)
		{
			struct _Code* fcode = m_pFCode.GetAt(ii);
			if (fcode->Code == code)
			{
				fcode->Jango = jgvl;
				fcode->Pgga = pgdg;
				invalidateRC();
				return;
			}
		}
	}

	for (int ii = 0; ii < STANDARDCNT; ii++)
	{
		for (int jj = 0; jj < m_pOCode[ii].GetSize(); jj++)
		{
			struct _OCode* ocode = m_pOCode[ii].GetAt(jj);
			if (ocode->Call.Code == code)
			{
				ocode->Call.Jango = jgvl;
				ocode->Call.Pgga = pgdg;
				invalidateRC();
				return;
			}
			else if (ocode->Put.Code == code)
			{
				ocode->Put.Jango = jgvl;
				ocode->Put.Pgga = pgdg;
				invalidateRC();
				return;
			}
		}
	}
}

void CJangoWnd::ClearMicheg()
{
	int	ii=0, jj=0;
	
	for (ii = 0; ii < m_pFCode.GetSize(); ii++)
	{
		struct _Code* fcode = m_pFCode.GetAt(ii);
		fcode->Micheg = 0;
	}
	
	for (ii = 0; ii < STANDARDCNT; ii++)
	{
		for (jj = 0; jj < m_pOCode[ii].GetSize(); jj++)
		{
			struct _OCode* ocode = m_pOCode[ii].GetAt(jj);
			ocode->Call.Micheg = 0;
			ocode->Put.Micheg = 0;
		}
	}
}

void CJangoWnd::ClearJango()
{
	int	ii=0, jj=0;
	
	for (ii = 0; ii < m_pFCode.GetSize(); ii++)
	{
		struct _Code* fcode = m_pFCode.GetAt(ii);
		fcode->Jango = "";
		fcode->Pgga = "";
		fcode->Micheg = 0;
	}
	
	for (ii = 0; ii < STANDARDCNT; ii++)
	{
		for (jj = 0; jj < m_pOCode[ii].GetSize(); jj++)
		{
			struct _OCode* ocode = m_pOCode[ii].GetAt(jj);

			ocode->Call.Jango = "";
			ocode->Call.Pgga = "";
			ocode->Call.Micheg = 0;

			ocode->Put.Jango = "";
			ocode->Put.Pgga = "";
			ocode->Put.Micheg = 0;
		}
	}
}

void CJangoWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	ClientToScreen(&pt);
	if (m_pMapWnd->GetCode().IsEmpty())
		m_mouseDLL.Format("IB300200\n%d\t%d\n", pt.x, pt.y);
	else
		m_mouseDLL.Format("IB300200\n%d\t%d\ned_focod\t%s\n", pt.x, pt.y, m_pMapWnd->GetCode());
	m_pView->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (long)m_mouseDLL.operator LPCTSTR());
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CJangoWnd::SetSize(bool bExpand)
{
	CRect rc;
	GetClientRect(rc);
	if (bExpand)
	{
		m_row = MAX_ROW;
		m_rcCombo.bottom = rc.bottom;
	}
	else 
	{
		m_row = MIN_ROW;
		m_rcCombo.bottom = rc.bottom;
	}
	
	setScroll();
	m_vbar.MoveWindow(m_rcCombo);
	
	Invalidate();
}

void CJangoWnd::invalidateRC()
{
	InvalidateRect(m_oRc1);
	InvalidateRect(m_oRc2);
	InvalidateRect(m_oRc3);
	InvalidateRect(m_oRc4);
	InvalidateRect(m_oRc5);
	InvalidateRect(m_oRc6);
	InvalidateRect(m_oRc7);
	InvalidateRect(m_tRc2);
}


COLORREF CJangoWnd::GetIndexColor(int index)
{
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CFont* CJangoWnd::GetAxFont(CString fName, int point, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name   = (LPSTR)fName.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = bItalic;
	fontR.bold   = nBold;
	return (CFont*)m_pView->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* CJangoWnd::GetAxBitmap(CString path)
{
	return (CBitmap*)m_pView->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

CPen* CJangoWnd::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pView->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* CJangoWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pView->SendMessage(WM_USER, getBRUSH, (long)clr);
}


CString CJangoWnd::SetComma(CString src)
{
	CString temp, floatStr;
	int	ii{}, jj = -1;

	src.TrimLeft();
	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (ii = temp.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}


CString CJangoWnd::Parser(CString &srcstr, CString substr)
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

void CJangoWnd::DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat, UINT nType)
{
	rc.DeflateRect(2, 0);
	nFormat = DT_SINGLELINE | DT_VCENTER | nFormat;
	COLORREF color{}, clrText = GetIndexColor(COLOR_TEXT);
	int	pos = -1;
	CString value, giho = "", percent = "";

	if (data.IsEmpty())	return;

	CFont* pOldFont{};
	if (nType & ST_BOLD)
		pOldFont = pDC->SelectObject(m_pBoldFont);
	else
		pOldFont = pDC->SelectObject(m_pFont);

	if (nType & ST_TEXT)
	{
		pDC->DrawText(data, rc, nFormat);
		return;
	}

	if (nType & ST_SHGA)
	{
		if (data[0] == '1')
		{
			giho = "↑ ";
			clrText = GetIndexColor(94);
		}
		else if (data[0] == '2')
		{
			giho = "▲ ";
			clrText = GetIndexColor(94);
		}
		else if (data[0] == '4')
		{
			giho = "↓ ";
			clrText = GetIndexColor(95);
		}
		else if (data[0] == '5')
		{
			giho = "▼ ";	
			clrText = GetIndexColor(95);
		}
		data = data.Mid(1);
	}

	if (nType & ST_COLOR)
	{
		pos = data.Find('+');
		if (pos >= 0)	
			clrText = GetIndexColor(94);

		pos = data.Find('-');
		if (pos >= 0)
			clrText = GetIndexColor(95);
	}

	if (nType & ST_PERCENT)
		percent = " %";

	if (nType & ST_REMOVE)
	{
		data.Remove('+');
		//data.Remove('-');
	}

	if (nType & ST_NORMAL)
	{
		CString pre = _T("");
		pos = data.Find('+');
		if (pos >= 0)
		{
			pre = "+";
			data.Remove('+');
		}

		pos = data.Find('-');
		if (pos >= 0)
		{
			pre = "-";
			data.Remove('-');
		}

		data.Remove(' ');
		pos = data.Find('.');
		if (pos >= 0)
			value.Format("%.2f", atof(data));
		else
			value.Format("%d", atoi(data));

		if (atof(data) == 0.0)
			data = "";
		else
			data = pre + value;
	}

	if (nType & ST_COMMA)
	{
		data = SetComma(data);	
	}

	color = pDC->SetTextColor(clrText);
	pDC->DrawText(giho + data + percent, rc, nFormat);

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(color);
}


void CJangoWnd::ChangeFont(double* lRate, CFont* pfont, CFont* boldFont)
{
	m_pFont = pfont;
	m_pBoldFont = boldFont;
	CRect rc, wrc;
	GetClientRect(&rc);
	int iw{}, ih{}, ileft{}, itop{};
	const int igap = 1;

	m_pBtnFuture[0]->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	itop = wrc.top;
	ileft = wrc.left ;

	iw = ( rc.Width() - ileft - (GAP*3) - igap)  / 4;
	ih = wrc.Height() ;
	ih = ih + (int)(lRate[1] * (float)ih );

	for(int i=0; i<7; i++)
	{
		wrc = CRect(ileft, itop, ileft+iw, itop+ih);
		m_pBtnFuture[i]->SetFont(pfont);
		m_pBtnFuture[i]->MoveWindow(wrc);

		wrc.top = itop + ih + BUTTON_SPACE;
		wrc.bottom = wrc.top + ih;
		if(i == 0)	m_oRc1 = wrc;
		else if(i == 1)	m_oRc2 = wrc;
		else if(i == 2)	m_oRc3 = wrc;
		else if(i == 3) m_oRc4 = wrc;
		else if(i == 4) m_oRc5 = wrc;
		else if(i == 5) m_oRc6 = wrc;
		else	m_oRc7 = wrc;
		ileft += (iw + BUTTON_SPACE);
	}

	itop = m_oRc7.bottom + GAP;
	m_cbOption.GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	ih = wrc.Height() ;
	ih = ih + (int)(lRate[1] * (float)ih );

	wrc.top = itop;
	wrc.bottom = itop + ih;
	wrc.right = rc.right - igap;
	m_cbOption.SetFont(pfont);
	m_cbOption.MoveWindow(wrc);

	m_tRc2.top = wrc.bottom + 1;
	m_tRc2.bottom = rc.bottom - GAP;
	m_tRc2.right = wrc.right - SCROLL_WIDTH-GAP;

//	m_vbar.GetWindowRect( &wrc);
//	ScreenToClient(&wrc);
	
	wrc.left = m_tRc2.right;
	wrc.right = wrc.left + SCROLL_WIDTH;
	wrc.top = m_tRc2.top;
	wrc.bottom = m_tRc2.bottom;
	
	m_vbar.MoveWindow(wrc);
	m_iRowH = (int)(wrc.Height() / m_row);
}
