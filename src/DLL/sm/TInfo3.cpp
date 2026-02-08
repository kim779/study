// TInfo3.cpp : implementation file
//

#include "stdafx.h"
#include "TInfo3.h"
#include "resource.h"
#include "SimpleSplitter.h"
#include "Pane.h"
#include "smdefine.h"
#include "nbutton.h"
#include "../../h/axisvar.h"
#include "../../axis/axmsg.hxx"
#include "skindlg.h"
#include "MenuXP.h"

#define	WGAP		2
#define MINPANE		20
#define ID_TOOLTIP		1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTInfo3

CTInfo3::CTInfo3(bool (*callback)(int, WPARAM, LPARAM))
{
	m_axiscall	= callback;
	m_height	= LISTBAR_HEIGHT;

	m_bmpBack = Axis::GetBitmap("BAR_BG");
	m_col = -1;

	SetCols();

	m_btnClose = NULL;
	//m_ToolTip2 = NULL;

	m_blinkInterval = 300;
	m_blinkStep = 0;
	m_blinkColor = RGB(0xFF, 0, 0);
}

CTInfo3::~CTInfo3()
{
	XMSG();
	DELETE_WND(m_btnClose);
}


BEGIN_MESSAGE_MAP(CTInfo3, CDialogBar)
	//{{AFX_MSG_MAP(CTInfo3)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTInfo3 message handlers
void CTInfo3::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

CSize CTInfo3::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	CSize	size;
	CRect	fRc;

	size.cy = m_height;
	size.cy += (rect.Height());

	if (IsFloating())
	{
		GetDockingFrame()->GetParent()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}
	else
	{
		GetDockingFrame()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}

	return size;
}

void CTInfo3::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (m_bitmap.m_hObject == NULL)
	{
		MakeBaseBitmap(&dc);
	}

	CDC mdc;
	CRect rc;
	GetClientRect(rc);

	mdc.CreateCompatibleDC(&dc);
	CBitmap* oldBmp = mdc.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);

	for (int i = 0; i < m_cols; i++)
		DrawData(&dc, i);

	mdc.SelectObject(oldBmp);
}

void CTInfo3::MakeBaseBitmap(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);

	CDC mdc;
	mdc.CreateCompatibleDC(pDC);

	if (m_bitmap.m_hObject)
		m_bitmap.DeleteObject();
	m_bitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());

	CBitmap* oldBmp = mdc.SelectObject(&m_bitmap);

	CDC tempDC;
	tempDC.CreateCompatibleDC(pDC);
	CBitmap* oldTempBmp = tempDC.SelectObject(m_bmpBack);

	BITMAP bm;
	m_bmpBack->GetBitmap(&bm);

	mdc.StretchBlt(0, 0, rc.Width(), bm.bmHeight, 
			&tempDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	mdc.SelectObject(oldBmp);
	tempDC.SelectObject(oldTempBmp);
}

void CTInfo3::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);

	CWindowDC dc(this);
	MakeBaseBitmap(&dc);

	if (m_btnClose)
		m_btnClose->MoveWindow(cx - MINPANE, 2, m_buttonSize, m_buttonSize);

	Invalidate();
}

void CTInfo3::OnDestroy() 
{
	XMSG();
	/*
	if (m_ToolTip2)
	{
		if (m_ToolTip2->m_hWnd)
		{
			m_ToolTip2->Activate(FALSE);
			m_ToolTip2->DelTool(this);
			m_ToolTip2->RelayEvent(NULL);
		}
		delete m_ToolTip2;
	}
	*/
	CDialogBar::OnDestroy();	
}

void CTInfo3::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CDialogBar::OnLButtonDblClk(nFlags, point);
}

void CTInfo3::change_Palette(CString skinN)
{

}

void CTInfo3::make_Ctrl()
{
	m_btnClose = CreateNButton(CTRL_HIDE, "체결툴바 닫기", "tk_close", FALSE);

	CBitmap* bmp = Axis::GetBitmap("tk_close");
	if (bmp)
	{
		BITMAP bm;
		bmp->GetBitmap(&bm);

		m_buttonSize = bm.bmHeight;
	}
}

CNButton* CTInfo3::CreateNButton(unsigned int id, const char* title, const char* button, BOOL eachButton)
{
#ifdef DF_USESTL_TINFO3
	auto btn = std::make_unique<CNButton>(id);
	const DWORD	dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	if (!btn->Create(NULL, title, dwStyle, CRect(0, 0, 0, 0), this, -1))
		return NULL;
	else
	{
		btn->set_ToolTipText(title);
		btn->set_ImageType(eachButton ? IT_EACH : IT_INDEX);
		if (eachButton)
			btn->set_Image(CAxButtonName(button));
		else
			btn->set_Image(button);

		btn->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		return btn.release();
	}
#else
	CNButton*	btn = new CNButton(id);
	const DWORD	dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	if (!btn->Create(NULL, title, dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete btn;
		return NULL;
	}
	else
	{
		btn->set_ToolTipText(title);
		btn->set_ImageType(eachButton ? IT_EACH : IT_INDEX);
		if (eachButton)
			btn->set_Image(CAxButtonName(button));
		else
			btn->set_Image(button);

		btn->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

		return btn;
	}
#endif
	
}

LONG CTInfo3::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_HIDE:
		(*m_axiscall)(AXI_HIDETICKER, 0, 5);	
		break;
	}

	return 0;
}

int CTInfo3::GetCols()
{
	return m_cols;
}

void CTInfo3::SetCols(int cols)
{
	m_cols = cols;
	m_datas.SetSize(m_cols);
	m_acnts.SetSize(m_cols);
	for (int i = 0; i < m_cols; i++)
	{
		m_datas[i] = "";
		m_acnts[i] = "";
	}
	if (GetSafeHwnd())
		Invalidate();
}

CString CTInfo3::Comma(CString value)
{
	CString		tmp, gb, tail;
	int		pos{};

	value.TrimLeft();	
	value.TrimRight();
	if (value.IsEmpty())	
		return _T("");
	
	pos = value.Find('.');
	if (pos != -1)
	{
		tail = value.Mid(pos);
		value = value.Left(pos);
	}

	if (value.GetLength() < 3)
		return value + tail;

	for (int ii = value.GetLength()-1, jj = 0; ii >= 0; jj++)
	{
		if (jj != 0 && jj % 3 == 0)
			tmp.Insert(0, ',');
		tmp.Insert(0, value.GetAt(ii));
		ii--;
	}
	return tmp + tail;
}

BOOL CTInfo3::AddData(CString data, BOOL bFail)
{
	//if (data.Find("체결") < 0)
	//	return FALSE;

	CString time(Axis::Parse(data));

	CWindowDC dc(this);

	if (m_blinkStep % 2)
	{
		m_blinkStep = 0;
		DrawData(&dc, m_col);
	}

	m_col++;
	m_col %= m_cols;

	CString orderNo(Axis::Parse(data));
	CString orgOrderNo(Axis::Parse(data));
	CString acctNo(Axis::Parse(data));
	CString acctNm(Axis::Parse(data));
	CString itemNm(Axis::Parse(data));
	CString noticeKind(Axis::Parse(data));
	CString volume(Axis::Parse(data));
	CString price(Axis::Parse(data));
	
	if (!noticeKind.IsEmpty())
	{
		/*
		if (noticeKind[0] == '+')
			m_blinkColor = RGB(0xFF, 0, 0);
		else
			m_blinkColor = RGB(0, 0, 0xFF);
		*/
		noticeKind.Delete(0, 1);
	}
	

	price = Comma(price);

	m_datas[m_col] = Format("%s %s %s %s %s주 %s", 
		acctNm, noticeKind, itemNm, noticeKind.Left(4), volume, price);
	m_acnts[m_col] = acctNo;
	DrawData(&dc, m_col);

	m_blinkStep = 0;
	SetTimer(1, m_blinkInterval, NULL);
	
	return TRUE;
}

void CTInfo3::DrawData(CDC* dc, int col)
{
	CRect rc(GetCellRect(col));

	const COLORREF backColors[5] = 
	{
		0x00EBEBFE, 0x00FEF2D8, 0x00F9FCF0, 0x00F7F7F7, 0x00F1F7FF
	};
	const COLORREF borderColor = 0x00DFEAFB;
	CPen pen(PS_SOLID, 1, m_blinkStep % 2  ? m_blinkColor: borderColor);
	CPen pen2(PS_SOLID, 1, m_blinkColor);
	CPen* oldPen{};
	if ((m_col == col) && (m_blinkStep==0)) //asdf
		oldPen = dc->SelectObject(&pen2);
	else
		oldPen = dc->SelectObject(&pen);
	CBrush brush(backColors[col]);
	CBrush* oldBrush = dc->SelectObject(&brush);

	dc->Rectangle(rc);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);

	dc->SetBkMode(TRANSPARENT);

	rc.left += 3;
	rc.right -= 3;

	CFont* oldFont = dc->SelectObject(&Axis::font);
	rc.top++;
	if (m_datas[col].Find("매수")>0) dc->SetTextColor(RGB(255,0,0));
	else if (m_datas[col].Find("매도")>0) dc->SetTextColor(RGB(0,0,255));
	else dc->SetTextColor(RGB(0,0,0));
	//if (m_datas[col].Find("정정")>0) dc->SetTextColor(RGB(0,255,0));
	dc->DrawText(m_datas[col], rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	dc->SelectObject(oldFont);
}

CRect CTInfo3::GetCellRect(int col)
{
	CRect rc;
	GetClientRect(rc);

	rc.top = 2;
	rc.bottom -= 2;
	rc.right -= m_buttonSize + 5;
	rc.left += 3;

	const int cellWidth = (int)(rc.Width() / (m_cols * 1.0));

	rc.left += cellWidth * col;
	rc.right = rc.left + cellWidth - 1;

	return rc;
}

void CTInfo3::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		CWindowDC dc(this);
		
		if (m_blinkStep == 0)
			for (int i = 0; i < m_cols; i++)
				DrawData(&dc, i);
		else
			DrawData(&dc, m_col);


		m_blinkStep++;
		if (m_blinkStep > 10)
		{
			KillTimer(1);
			m_blinkStep = 0;
			for (int i = 0; i < m_cols; i++)
				DrawData(&dc, i);
		}
	}
	CDialogBar::OnTimer(nIDEvent);
}

void CTInfo3::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialogBar::OnLButtonDown(nFlags, point);
}

void CTInfo3::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rc;
	bool cfind = false;
	int icfind = 0;
	CString sAcnt(_T(""));
	CString sgood = "10";
	for (int i = 0; i < m_cols; i++)
	{
		rc = GetCellRect(i);
		if (rc.PtInRect(point)!=0)
		{
			cfind = true;
			icfind = i;
			break;
		}
	}
	if (cfind)
	{
		sAcnt = m_acnts[icfind];
		
		sAcnt.TrimRight();
		if (!sAcnt.IsEmpty())
		{
			CString m_trcode;
			sgood = sAcnt.Mid(3,2);
			if ((sgood == "10")||(sgood == "11")||(sgood.Mid(1,1) == "3"))
				m_trcode = "IB110200AN1A\t";
			else if((sgood == "20")||(sgood == "21"))
				m_trcode = "IB310200AN20\t";
			else return;
			m_trcode += sAcnt;
			(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)m_trcode);
		}
	}

	CDialogBar::OnLButtonUp(nFlags, point);
}

void CTInfo3::InitToolTip()
{
	/*
	if (!m_ToolTip2)
	{
		m_ToolTip2 = new CToolTipCtrl();
		m_ToolTip2->Create(this, TTS_ALWAYSTIP);
		m_ToolTip2->Activate(TRUE);
	}
	*/
}

void CTInfo3::SetToolTipText(CString text, BOOL bActivate)
{
	/*
	if (text.IsEmpty())	return;
	
	InitToolTip();
	if (m_ToolTip2->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip2->AddTool(this, text, CRect(0, 0, 100, 100), ID_TOOLTIP);
	}
	
	m_ToolTip2->UpdateTipText(text, this, ID_TOOLTIP);
	m_ToolTip2->Activate(bActivate);
	*/
}

