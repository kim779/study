// CheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "CheckBox.h"
#include "resource.h"
#include "futuresdlg.h"
#include "CmmdtDlg.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckBox

CCheckBox::CCheckBox()
{
	m_pTool		= NULL; //new CTool;	
	m_hCursor	= NULL;
	m_nInit		= 40;
	m_nScroll	= 0;
	m_Key		= -1;
	m_BitmapNormal.LoadBitmap(IDB_BITMAP_NORMAL);
	m_BitmapClick.LoadBitmap(IDB_BITMAP_CLICK);
	m_Array.RemoveAll();
	m_iWnd = FUTURESDLG;
}

CCheckBox::~CCheckBox()
{		
	if (m_hCursor) ::DestroyCursor(m_hCursor);

	m_hCursor = NULL;
	
	DestroyWindow();
	
	m_BitmapNormal.DeleteObject();
	m_BitmapClick.DeleteObject();
}


BEGIN_MESSAGE_MAP(CCheckBox, CWnd)
	//{{AFX_MSG_MAP(CCheckBox)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckBox message handlers

void CCheckBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here

//	CMemDC(CDC& dc, CWnd* pWnd);
//	CMemDC(CDC& dc, const CRect& rect);

	CAxMemDC	memDC(&dc);
		m_pTool = new CTool; 

		m_pTool->SetDC(&memDC);
		Draw();

		SAFE_DELETE(m_pTool);
	
	// Do not call CWnd::OnPaint() for painting messages
}

void CCheckBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CRect rc(0, 41, 321, 225), clientRc;
	GetClientRect(&clientRc);
	int nVisibleRowCnt = clientRc.Height() / 20 - 1;

	int	scrollPos = GetScrollPos32(SB_VERT);
	int	lim = -(m_nCount - (nVisibleRowCnt + 1)) * 20;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nInit > -20 * ((m_nCount-1) - nVisibleRowCnt))
		{
			m_nInit -= 20; m_nScroll += 20;
		}

		if (m_nInit < lim)
			m_nInit = lim;

		InvalidateRect(rc);
		SetScrollPos32(SB_VERT, m_nScroll);
		break;
        case SB_LINEUP:
		if (m_nInit < 40)
		{
			m_nInit += 20; m_nScroll -= 20;
		}
		InvalidateRect(rc);
		SetScrollPos32(SB_VERT, m_nScroll);
		break;
        case SB_PAGEDOWN:
		if (m_nInit > -20*((m_nCount) - 2))
		{
			m_nInit -= 40; m_nScroll += 40;
		}

		if (m_nInit < lim)
			m_nInit = lim;

		InvalidateRect(rc);
		SetScrollPos32(SB_VERT, m_nScroll);	
	
		break;
        case SB_PAGEUP:
		if (m_nInit < 40)
		{
			m_nInit += 40; m_nScroll -= 40;
			if (m_nInit > 40)
			{	
				m_nInit = 40; m_nScroll = 0;
			}
		}
		InvalidateRect(rc);
		SetScrollPos32(SB_VERT, m_nScroll);
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
		if (/*nSBCode == SB_THUMBPOSITION || */nSBCode == SB_THUMBTRACK)
		{
			int n = nPos / 20;
			m_nInit = -(n-1) * 20;  m_nScroll = nPos;
			if (m_nScroll <= 10)
				m_nInit = 40;
			
			if (m_nInit < lim)
				m_nInit = lim;
		}
		
		InvalidateRect(rc);
		break;
        case SB_TOP:
		break;
        case SB_BOTTOM:
		break;
	default:
		break;
	}
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCheckBox::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
	// 스크롤의 위치 저장...
	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_POS;
	sinfo.nPos   = nPos;

	return SetScrollInfo(nBar, &sinfo, bRedraw);
}

int CCheckBox::GetScrollPos32(int nBar, BOOL bGetTrackPos)
{	
	// 스크롤의 위치 파악...
	SCROLLINFO sinfo;
	sinfo.cbSize = sizeof(SCROLLINFO);

	if (bGetTrackPos)
	{
		if (GetScrollInfo(nBar, &sinfo, SIF_TRACKPOS))
			return sinfo.nTrackPos;
	}
	else 
	{
		if (GetScrollInfo(nBar, &sinfo, SIF_POS))
			return sinfo.nPos;
	}

	return 0;
}

void CCheckBox::Draw()
{
	int row1[5] = {0, 32, 64, 96, 128};
	int row2[5] = {192, 222, 254, 288, 320};
	CStringArray aStr;
	
	CRect rc;
	
	GetClientRect(rc);
	m_pTool->FrameRect(rc, USER_BACKCOLOR2);
	
	DrawList(m_nInit);	
	
	// 헤더 ....
	rc.SetRect(0, 0, 320, 40);
	m_pTool->FrameRect(rc, USER_BACKCOLOR2);
	
	rc.SetRect(0, 0, 128, 20);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("콜옵션", rc, USER_TEXT);
	m_pTool->DrawCell(rc,USER_LINECOLOR);

	rc.SetRect(128, 0 , 192, 40);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("행사가격", rc, USER_TEXT);
	m_pTool->DrawCell(rc,USER_LINECOLOR);
	
	rc.SetRect(192, 0, 320, 20);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("풋옵션", rc, USER_TEXT);
	m_pTool->DrawCell(rc,USER_LINECOLOR);
	
	INT_PTR nCount = m_Array.GetSize();
	//		for (int ii=0; ii < ((CFuturesDlg*)m_pWnd)->GetCountMonth(); ii++)
	INT_PTR ii = 0;
	for (ii = 0; ii < nCount; ii++)
	{
		CString tmp = m_Array.GetAt(ii);
		
		rc.SetRect(row1[ii], 20, row1[ii+1], 40);
		m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
		m_pTool->WriteText(m_Array[ii], rc, USER_TEXT);
		m_pTool->DrawCell(rc,USER_LINECOLOR);
	}
	
	//		for (ii=0; ii < ((CFuturesDlg*)m_pWnd)->GetCountMonth(); ii++)
	for (ii = 0; ii < nCount; ii++)
	{
		rc.SetRect(row2[4- ii], 20, row2[3- ii], 40);
		m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
		m_pTool->WriteText(m_Array[ii], rc, USER_TEXT);
		m_pTool->DrawCell(rc,USER_LINECOLOR);
	}		
}

void CCheckBox::DrawList(int row)
{
	int col[10] = {0, 32, 64, 96, 128, 192, 222, 254, 288, 320};
	CRect rc;
	int irow = 0, icol = 0;
	CString temp;
	DATA data;
	
	// 여기서 row의 인덱스 결정...
	for (int jj = 0; jj < m_nCount; jj++)
	{	
		// 여기서 col의 인덱스 결정..
		for(int ii = 0; ii < 9; ii++)
		{
			rc.SetRect(col[ii], row, col[ii + 1], row + 20);
			m_pTool->FrameRect(rc, RGB(229,229,230));

			if (ii == 4)
			{
				m_Map.Lookup(jj * 10 + ii, data);
			
				if (data.atm == 1)
				{
					m_pTool->FrameRect(rc, RGB(148, 148, 210)); //USER_FOCUS_COR);
					m_pTool->WriteText(data.name, rc, COR_WHITE);
				}
				else
				{
					m_pTool->FrameRect(rc, COR_WHITE);//RGB(239,239,239));
					m_pTool->WriteText(data.name, rc, USER_TEXT);
						
				}
				m_pTool->DrawCell(rc,USER_LINECOLOR);
			}
			else
			{	// BOOL값 처리... 
				m_Map.Lookup((jj * 10 + ii), data); 
				
				if (data.flag)
				{
					m_BitmapNormal.Draw(m_pTool->GetDC(), rc);
					if( m_iWnd == FUTURESDLG )
					{
						if (data.flag == 2 && ((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
							&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
						{	
							m_BitmapClick.Draw(m_pTool->GetDC(), rc);
							m_Key = jj * 10 + ii;
						}
					}
					else if( m_iWnd == MMDTDLG )
					{
						if (data.flag == 2 && ((CCmmdtDlg*)m_pWnd)->GetKind() != futureCODE
							&& ((CCmmdtDlg*)m_pWnd)->GetKind() != spreadCODE)
						{	
							m_BitmapClick.Draw(m_pTool->GetDC(), rc);
							m_Key = jj * 10 + ii;
						}
					}
				}
				m_pTool->DrawCell(rc,USER_LINECOLOR);
			}

		}		
		row += 20;
	}

	ResetScrollBars();
}

void CCheckBox::ResetScrollBars()
{	
	CRect rect; 

	GetClientRect(rect);

	CRect visibleRC(0, 41, rect.right, rect.bottom);

	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_PAGE|SIF_RANGE;

	sinfo.nPage  = visibleRC.Height();
	sinfo.nMin   = 0; 
	sinfo.nMax   = m_nCount * 20;
	SetScrollInfo(SB_VERT, &sinfo, TRUE);
}

// 데이터 저장 함수 ...
void CCheckBox::AddData(int row, int col, DATA& data)
{	
	int key = row * 10 + col;

	m_Map.SetAt(key, data);
}

// 데이터 가져오는 함수..
DATA CCheckBox::GetData(int row, int col) 
{
	int key = row * 10 + col;
	DATA data;

		m_Map.Lookup(key, data);
	
		return data;
}

void CCheckBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
// 	SetItem(point);
// 	((CFuturesDlg*)m_pWnd)->EndDialog(1);

	CWnd::OnLButtonDown(nFlags, point);
}

void CCheckBox::SetItem(CPoint point)
{
	int col[10] = {0, 30, 62, 94, 130, 192, 224, 256, 290, 320};
	int val = 0;
	DATA data;
	CRect rc;


	// 이전에 선택 한것 삭제..
	m_Map.Lookup(m_Key, data);
	data.flag = TRUE;
	m_Map.SetAt(m_Key, data);
	m_Key = -1;

	data.flag = FALSE;
	data.code = "";
	data.name = "";


	CRect rect(130, 0, 192, 40);
	
	if (rect.PtInRect(point))
	{
		if( m_iWnd == FUTURESDLG )
			((CFuturesDlg*)m_pWnd)->Sort();
		else if( m_iWnd == MMDTDLG )
			((CCmmdtDlg*)m_pWnd)->Sort();
	}

	// 선택한 것
	if (m_nInit == 40)
		val = 0;
	else if (m_nInit < 40 && m_nInit > 0)
		val = m_nInit;
	else if (m_nInit <= 0)
		val = 40 + abs(m_nInit);

	if ((point.y > 40 && point.y < 319))
		for(int jj = 0; jj < m_nCount ; jj++)
			for (int ii = 0; ii < 9; ii++)
				if ((jj*20< point.y - 40 + val) && (point.y - 40 + val < 20 + jj*20) 
					&& (col[ii] < point.x) && (point.x < col[ii+1]))	
				{	
					// 마우스 이벤트 발생시...
					if (ii != 4)
					{
						rc.SetRect(col[ii], jj*20, col[ii+1], jj*20 + 20);
						m_Map.Lookup(jj*10+ii, data);
					
						if(data.flag)		data.flag = 2;
					
						if( m_iWnd == FUTURESDLG )
						{
							if (((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
								&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
							{
								((CFuturesDlg*)m_pWnd)->SetCode(data.code);
								((CFuturesDlg*)m_pWnd)->SetName(data.name);
						
								//((CFuturesDlg*)m_pWnd)->SetButton();
								((CFuturesDlg*)m_pWnd)->UpdateData(FALSE);
							}
						}
						else if( m_iWnd == MMDTDLG )
						{
							if (((CCmmdtDlg*)m_pWnd)->GetKind() != futureCODE
								&& ((CCmmdtDlg*)m_pWnd)->GetKind() != spreadCODE)
							{
								((CCmmdtDlg*)m_pWnd)->SetCode(data.code);
								((CCmmdtDlg*)m_pWnd)->SetName(data.name);
						
								//((CCmmdtDlg*)m_pWnd)->SetButton();
								((CCmmdtDlg*)m_pWnd)->UpdateData(FALSE);
							}
						}
						AddData(jj, ii, data);
						Invalidate();		
					
					}
				}
}

void CCheckBox::RemoveAll()
{
	m_Map.RemoveAll();
}

void CCheckBox::SetArray()
{
	CArray<CString, CString> *pArray;
	if( m_iWnd == FUTURESDLG )
		pArray = ((CFuturesDlg*)m_pWnd)->GetArray();
	else if( m_iWnd == MMDTDLG )
		pArray = ((CCmmdtDlg*)m_pWnd)->GetArray();

	m_Array.RemoveAll();

	for (int jj = 0 ; jj < pArray->GetSize() ; jj++)
		m_Array.Add(pArray->GetAt(jj).Right(2) + "월");

	pArray = NULL;
}

BOOL CCheckBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	POINT pt;
	
	GetCursorPos(&pt);
	
	ScreenToClient(&pt);
	if (pt.y > 40 && pt.x < 321)
		if (m_hCursor)
		{
			if ((0 < pt.x && pt.x < 130) || (321 > pt.x && pt.x > 192) )
			{
				SetItem(pt);
				::SetCursor(m_hCursor);
				return TRUE;
			}
		}


	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

DWORD CCheckBox::SetCheckCursor(int nCursorId, BOOL bRepaint)
{
	HINSTANCE	hInstResource = NULL;
	// Destroy any previous cursor
	if (m_hCursor)
	{
		::DestroyCursor(m_hCursor);
		m_hCursor = NULL;
	} // if

	// Load cursor
	if (nCursorId)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nCursorId), RT_GROUP_CURSOR);
		// Load cursor resource
		m_hCursor = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);
		// Repaint the button
		if (bRepaint) Invalidate();
		// If something wrong
		if (m_hCursor == NULL) return FALSE;
	} // if

	return TRUE;

} 

void CCheckBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if ((point.y > 40 && point.y < 319) && (0 < point.x && point.x< 130) || (point.x > 192 && point.x < 321))
	{
		if( m_iWnd == FUTURESDLG )
		{
			if (((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
				&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
			{
				OnLButtonDown(nFlags, point);
				((CFuturesDlg*)m_pWnd)->EndDialog(1);
			}
		}
		else if( m_iWnd == MMDTDLG )
		{
			if (((CCmmdtDlg*)m_pWnd)->GetKind() != futureCODE
				&& ((CCmmdtDlg*)m_pWnd)->GetKind() != spreadCODE)
			{
				OnLButtonDown(nFlags, point);
				((CCmmdtDlg*)m_pWnd)->EndDialog(1);
			}
		}
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CCheckBox::ShowRow(int row)
{
	ResetScrollBars();
	CRect rc(0,41,321,225);
			
	if (row < 5)		row = 0;
	else if (row > m_nCount - 4)	row += 4;
	else			row -=  4;
	
	SetInit(GetInit() - row * 20); 
	
	SetScroll(GetScroll() + row * 20);
	
	InvalidateRect(rc);
	SetScrollPos32(SB_VERT, GetScroll());
}

void CCheckBox::DeleteTool()
{
	if (m_pTool)
		m_pTool->DeleteDC();
}

void CCheckBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
 	SetItem(point);
	if( m_iWnd == FUTURESDLG )
		((CFuturesDlg*)m_pWnd)->EndDialog(1);
	else if( m_iWnd == MMDTDLG )
 		((CCmmdtDlg*)m_pWnd)->EndDialog(1);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CCheckBox::SetWnd(int Wnd)
{
	m_iWnd = Wnd;
}
