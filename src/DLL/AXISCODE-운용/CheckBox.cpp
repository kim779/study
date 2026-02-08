// CheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "CheckBox.h"
#include "resource.h"
#include "futuresdlg.h"

#include "math.h"
#include "../../h/axisfire.h"

#define COMBOBOX_CALL	3000
#define COMBOBOX_PUT	3001
#define CALLMONRECT		CRect(0, 20, 32, 40)
#define PUTMONRECT		CRect(288, 20, 320, 40)
#define STANDARDNUM		11
#define ROWCOUNT		23
#define ROWHIGHT		20
#define STARTNUM		4
#define STARTWNUM		2

#define WCALL_FAR	 9
#define WCALL_CLOSE  10
#define WPUT_CLOSE   12
#define WPUT_FAR     13

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckBox

CCheckBox::CCheckBox()
{
	m_nInit   = 40;
	m_Key     = -1;

	m_BitmapNormal.LoadBitmap(IDB_BITMAP_NORMAL);
	m_BitmapClick.LoadBitmap(IDB_BITMAP_CLICK);

	m_bCall = true;
	m_nCallSelect = STARTNUM;
	m_nPutSelect = STARTNUM;
	m_nWCallSelect = STARTWNUM;
	m_nWPutSelect = STARTWNUM;
	m_bWeek = false;
	m_pTool = std::make_unique<CTool>();
}

CCheckBox::~CCheckBox()
{		
	if (m_hCursor)
		::DestroyCursor(m_hCursor);
	m_hCursor = nullptr;
	
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
	CMemDC	memDC(&dc);

	m_pTool->SetDC(&memDC);
	if(m_bWeek)
		DrawWeek();
	else
		Draw();

	m_pTool->SetDC(nullptr);
}

void CCheckBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
//	CRect	rc(0,41,321,265), clientRc;
	CRect rc, clientRc;

	if(m_bWeek)
		rc.SetRect(0, 41, 321, 315);
	else
		rc.SetRect(0, 41, 321, 265);

	GetClientRect(&clientRc);
	const int nVisibleRowCnt = gsl::narrow_cast<int>(clientRc.Height() / ROWHIGHT - 1);

	const int	scrollPos = GetScrollPos32(SB_VERT);
	const int	lim = -(m_nCount - (nVisibleRowCnt + 1)) * ROWHIGHT;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nInit > -ROWHIGHT * ((m_nCount-1) - nVisibleRowCnt))
		{
			m_nInit   -= ROWHIGHT;
			m_nScroll += ROWHIGHT;
		}

		if (m_nInit < lim)
			m_nInit = lim;

		InvalidateRect(rc);
		SetScrollPos32(SB_VERT, m_nScroll);
		break;

    case SB_LINEUP:
		if (m_nInit < 40)
		{
			m_nInit   += ROWHIGHT;
			m_nScroll -= ROWHIGHT;
		}
		InvalidateRect(rc);
		SetScrollPos32(SB_VERT, m_nScroll);
		break;

    case SB_PAGEDOWN:
		if (m_nInit > -ROWHIGHT*((m_nCount) - 2))
		{
			m_nInit   -= 40;
			m_nScroll += 40;
		}

		if (m_nInit < lim)
			m_nInit = lim;

		InvalidateRect(rc);
		SetScrollPos32(SB_VERT, m_nScroll);	
		break;

        case SB_PAGEUP:
		if (m_nInit < 40)
		{
			m_nInit   += 40;
			m_nScroll -= 40;
			if (m_nInit > 40)
			{	
				m_nInit   = 40;
				m_nScroll = 0;
			}
		}
		InvalidateRect(rc);
		SetScrollPos32(SB_VERT, m_nScroll);
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
		if (nSBCode == SB_THUMBTRACK)
		{
			const int n = nPos / ROWHIGHT;
			m_nInit = -(n-1)* ROWHIGHT;
			m_nScroll = nPos;

			if (m_nScroll <= 10)
				m_nInit = 40;
			
			if (m_nInit < lim)
				m_nInit = lim;
		}
		InvalidateRect(rc);
		break;

        case SB_TOP:
        case SB_BOTTOM:
	default:
		break;
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCheckBox::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_POS;
	sinfo.nPos   = nPos;
	return SetScrollInfo(nBar, &sinfo, bRedraw);
}

int CCheckBox::GetScrollPos32(int nBar, BOOL bGetTrackPos)
{	
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

void CCheckBox::DrawWeek()
{
	CRect	rc;
	CStringArray aStr;
	//int	row1[5] = {0, 32, 64, 96, 128};
	const int	row1[3] = {0, 64, 128};
	//int	row2[5] = {192, 222, 254, 288, 320};
	const int	row2[3] = {192, 254, 320};
	
	GetClientRect(rc);
	m_pTool->FrameRect(rc, USER_BACKCOLOR2);
	
	DrawWeekList(m_nInit);
	
	// header
	rc.SetRect(0, 0, 320, 40);
	m_pTool->FrameRect(rc, USER_BACKCOLOR2);
	
	rc.SetRect(0, 0, 128, 20);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("콜옵션", rc);
	m_pTool->DrawCell(rc);
	
	rc.SetRect(128, 0 , 192, 40);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("행사가격", rc);
	m_pTool->DrawCell(rc);
	
	rc.SetRect(192, 0, 320, 20);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("풋옵션", rc);
	m_pTool->DrawCell(rc);
	
	const int nCount = m_Array.GetSize();
	CString tmp;
	
	if(nCount == 0)	
		return;	//2015.07.14 KSJ 사이즈가 0이면 그리지 않음.
	for (int ii = 0; ii < 2; ii++)
	{
		rc.SetRect(row1[ii], 20, row1[ii+1], 40);
		
		if(ii == 0)
		{
			tmp = m_Array.GetAt(m_nWCallSelect == 0 ? 0 : STANDARDNUM - m_nWCallSelect); 
			if (tmp == "0000" || tmp == "000000")
				tmp.Empty();
			else if (tmp.GetLength() == 6)
				tmp = tmp.Left(2) + "." + tmp.Mid(2, 2) + "." + tmp.Right(2);

			m_pTool->FrameRect(CRect(0, 20, 64, 40), USER_BT_NORMALCOR1);
			m_pTool->WriteText(tmp, CRect(0, 20, 64, 40));
			m_pTool->DrawCell(CRect(0, 20, 64, 40));
		}
		else
		{	
			m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
			tmp = m_Array.GetAt(10);
			if (tmp == "0000" || tmp == "000000")
				tmp.Empty();
			else if (tmp.GetLength() == 6)
				tmp = tmp.Left(2) + "." + tmp.Mid(2, 2) + "." + tmp.Right(2);

			m_pTool->WriteText(tmp, rc);
			m_pTool->DrawCell(rc);
		}
	}
	for (int ii=0; ii < 2; ii++)
	{
		rc.SetRect(row2[2- ii], 20, row2[1- ii], 40);
		
		if(ii == 0)
		{
			tmp = m_Array.GetAt(m_nWPutSelect == 0 ? 0 : STANDARDNUM - m_nWPutSelect);
			if (tmp == "0000" || tmp == "000000")
				tmp.Empty();
			else if (tmp.GetLength() == 6)
				tmp = tmp.Left(2) + "." + tmp.Mid(2, 2) + "." + tmp.Right(2);

			m_pTool->FrameRect(CRect(254, 20, 320, 40), USER_BT_NORMALCOR1);
			m_pTool->WriteText(tmp, CRect(254, 20, 320, 40));
			m_pTool->DrawCell(CRect(254, 20, 320, 40));
		}
		else
		{	
			m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
		//	m_pTool->WriteText(m_Array[7 +ii], rc);
			tmp = m_Array.GetAt(10);
			if (tmp == "0000" || tmp == "000000")
				tmp.Empty();
			else if (tmp.GetLength() == 6)
				tmp = tmp.Left(2) + "." + tmp.Mid(2, 2) + "." + tmp.Right(2);
	
			m_pTool->WriteText(tmp, rc);
			m_pTool->DrawCell(rc);
		}
	}
}

void CCheckBox::Draw()
{
	CRect	rc;
	CStringArray aStr;
	const int row1[5] = {0, 32, 64, 96, 128};
	const int row2[5] = {192, 222, 254, 288, 320};

	GetClientRect(rc);
	m_pTool->FrameRect(rc, USER_BACKCOLOR2);

	DrawList(m_nInit);	

	// header
	rc.SetRect(0, 0, 320, 40);
	m_pTool->FrameRect(rc, USER_BACKCOLOR2);
	
	rc.SetRect(0, 0, 128, 20);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("콜옵션", rc);
	m_pTool->DrawCell(rc);

	rc.SetRect(128, 0 , 192, 40);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("행사가격", rc);
	m_pTool->DrawCell(rc);

	rc.SetRect(192, 0, 320, 20);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("풋옵션", rc);
	m_pTool->DrawCell(rc);

	const int nCount = m_Array.GetSize();
	CString tmp;

	if(nCount == 0)	return;	//2015.07.14 KSJ 사이즈가 0이면 그리지 않음.

	//2014.08.11 KSJ 여긴 그리는 부분이기 떄문에 4개만 그림
	for (int ii = 0; ii < 4; ii++)
	{
		rc.SetRect(row1[ii], 20, row1[ii+1], 40);

		if(ii == 0)
		{
			tmp = m_Array.GetAt(m_nCallSelect == 0 ? 0:STANDARDNUM - m_nCallSelect);
			m_pTool->FrameRect(CALLMONRECT, USER_BT_NORMALCOR1);
			m_pTool->WriteText(tmp, CALLMONRECT);
			m_pTool->DrawCell(CALLMONRECT);
		}
		else
		{	
			m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
			m_pTool->WriteText(m_Array[7 + ii], rc);
			m_pTool->DrawCell(rc);
		}
	}
	
	for (int ii=0; ii < 4; ii++)
	{
		rc.SetRect(row2[4- ii], 20, row2[3- ii], 40);

		if(ii == 0)
		{
			tmp = m_Array.GetAt(m_nPutSelect == 0 ? 0:STANDARDNUM - m_nPutSelect);
			m_pTool->FrameRect(PUTMONRECT, USER_BT_NORMALCOR1);
			m_pTool->WriteText(tmp, PUTMONRECT);
			m_pTool->DrawCell(PUTMONRECT);
		}
		else
		{	
			m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
			m_pTool->WriteText(m_Array[7 +ii], rc);
			m_pTool->DrawCell(rc);
		}
	}
}

void CCheckBox::DrawWeekList(int row)
{
	const int nRow = row;
	CRect	rc, tmprc;
	CString	temp;
	DATA	data;
	const int	col[6] = {0, 64, 128, 192, 254, 320};
	const int	irow = 0, icol = 0;
	
	const int nCount = m_Array.GetSize();
	
	if(nCount == 0)
	{
		ResetScrollBars();
		return;	//2015.07.14 KSJ 사이즈가 0이면 그리지 않음.
	}
	
	//2014.08.11 KSJ 여긴 그리는 부분이기 떄문에 4개만 그림
	
	// row index
	for (int jj = 0; jj < m_nCount; jj++)
	{	
		tmprc.SetRect(col[1], row, col[4], row+ROWHIGHT);
		m_pTool->FrameRect(tmprc, RGB(229,229,230));
	
		int icnt = 0;

		for (int ii = WCALL_FAR; ii < WPUT_FAR; ii++)
		{
			if (ii == STANDARDNUM)   //행사가격 처리
			{
				tmprc.SetRect(col[2], row, col[3], row+ROWHIGHT);
				m_Map.Lookup(jj*ROWCOUNT+ii, data);
				

				if (data.atm == 1)
				{
					m_pTool->FrameRect(tmprc, RGB(148, 148, 210)); //USER_FOCUS_COR);
					m_pTool->WriteText(data.name, tmprc, COR_WHITE);
				}
				else
				{
					m_pTool->FrameRect(tmprc, COR_WHITE);//RGB(239,239,239));
					m_pTool->WriteText(data.name, tmprc);
				}
				m_pTool->DrawCell(tmprc);
				
			}
			else if(  ii ==WCALL_CLOSE || ii == WPUT_CLOSE  )
			{	// BOOL값 처리... 
				const int cnt = jj*ROWCOUNT+ii;
				m_Map.Lookup((jj*ROWCOUNT+ii), data); 

				if (data.flag)
				{
					icnt++;
					if(ii == WCALL_CLOSE)
					{
						rc.SetRect(col[1], row, col[2], row+ROWHIGHT);
						tmprc = rc;
						tmprc.top += 2;
						tmprc.bottom -= 2;
						tmprc.left += 16;
						tmprc.right -= 16;
						m_BitmapNormal.Draw(m_pTool->GetDC(), tmprc);
					}
					else if(ii == WPUT_CLOSE)
					{
						rc.SetRect(col[3], row, col[4], row+ROWHIGHT);
						tmprc = rc;
						tmprc.top += 2;
						tmprc.bottom -= 2;
						tmprc.left += 16;
						tmprc.right -= 16;
						m_BitmapNormal.Draw(m_pTool->GetDC(), tmprc);
					}
					
					if (data.flag == 2 && ((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
						&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
					{	
						m_BitmapClick.Draw(m_pTool->GetDC(), rc);
						m_Key = jj*ROWCOUNT+ii;
					}			
				}	
			}
		}	
		
		rc.SetRect(col[1], row, col[2], row+ROWHIGHT);
		m_pTool->DrawCell(rc);
		
		rc.SetRect(col[3], row, col[4], row+ROWHIGHT);
		m_pTool->DrawCell(rc);

		row += ROWHIGHT;
	}
	
	SetWeekMonth(nRow);

	ResetScrollBars();
}
	
void CCheckBox::DrawList(int row)
{
	const int nRow = row;
	CRect	rc;
	CString	temp;
	DATA	data;
	const int	col[10] = {0, 32, 64, 96, 128, 192, 222, 254, 288, 320};
	const int	irow = 0, icol = 0;
	
	const int nCount = m_Array.GetSize();
	
	if(nCount == 0)
	{
		ResetScrollBars();
		return;	//2015.07.14 KSJ 사이즈가 0이면 그리지 않음.
	}
	
	//2014.08.11 KSJ 여긴 그리는 부분이기 떄문에 4개만 그림

	// row index
	for (int jj = 0; jj < m_nCount; jj++)
	{	
		// col index
		for (int ii = 8; ii < 15; ii++)
		{
			rc.SetRect(col[ii - 7], row, col[ii - 6], row+ROWHIGHT);
			m_pTool->FrameRect(rc, RGB(229,229,230));

			if (ii == STANDARDNUM)
			{
				m_Map.Lookup(jj*ROWCOUNT+ii, data);

				if (data.atm == 1)
				{
					m_pTool->FrameRect(rc, RGB(148, 148, 210)); //USER_FOCUS_COR);
					m_pTool->WriteText(data.name, rc, COR_WHITE);
				}
				else
				{
					m_pTool->FrameRect(rc, COR_WHITE);//RGB(239,239,239));
					m_pTool->WriteText(data.name, rc);
				}
				m_pTool->DrawCell(rc);
				
			}
			else
			{	// BOOL값 처리... 
				m_Map.Lookup((jj*ROWCOUNT+ii), data); 
				if (data.flag)
				{
					m_BitmapNormal.Draw(m_pTool->GetDC(), rc);

					if (data.flag == 2 && ((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
						&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
					{	
						m_BitmapClick.Draw(m_pTool->GetDC(), rc);
						m_Key = jj*ROWCOUNT+ii;
					}
				}
				m_pTool->DrawCell(rc);
			}
		}		
		
		row += ROWHIGHT;
	}

	SetMonth(nRow);
	ResetScrollBars();
}

void CCheckBox::ResetScrollBars()
{	
	CRect	rect;
	CRect	visibleRC;
	SCROLLINFO sinfo;
		
	GetClientRect(rect);
	visibleRC.SetRect(0, 41, rect.right, rect.bottom);

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_PAGE|SIF_RANGE;
	sinfo.nPage  = visibleRC.Height();
	sinfo.nMin   = 0; 
	sinfo.nMax   = m_nCount * ROWHIGHT; 
	SetScrollInfo(SB_VERT, &sinfo, TRUE);
}

void CCheckBox::AddData(int row, int col, const DATA& data)
{	
	m_Map.SetAt(row*ROWCOUNT + col, data);
}

DATA CCheckBox::GetData(int row, int col)
{
	DATA data;

	m_Map.Lookup(row*ROWCOUNT + col, data);
	return data;
}

void CCheckBox::weeklbtnClick(UINT nFlags, CPoint point)
{
	CString stmp{};
	DATA	data;
	int	val{};
	int	nIndex{};
	const int col[10] = { 0, 62, 130, 192, 256, 320 };

	m_Map.Lookup(m_Key, data);
	data.flag = TRUE;
	m_Map.SetAt(m_Key, data);
	m_Key = -1;

	data.flag = FALSE;
	data.code = "";
	data.name = "";

	if (m_nInit == 40)
		val = 0;
	else if (m_nInit < 40 && m_nInit > 0)
		val = m_nInit;
	else if (m_nInit <= 0)
		val = 40 + abs(m_nInit);

	if (point.y > 40 && point.y < 319)
	{
		for(int jj = 0; jj < m_nCount ; jj++)
		{
			for (int ii = 0; ii < 5 ; ii++)
			{
				if (jj*ROWHIGHT< point.y - 40 + val && point.y - 40 + val < ROWHIGHT + jj*ROWHIGHT 
					&& col[ii] < point.x && point.x < col[ii+1])
				{	
					if (ii == 0 || ii == 4)
					{
						if (ii == 0)
							nIndex = STANDARDNUM - m_nWCallSelect;
						else
							nIndex = m_nWPutSelect + STANDARDNUM;
					}
					else if (ii != 2)
					{
						nIndex = ii + 9;
					}
					m_Map.Lookup(jj * ROWCOUNT + nIndex, data);

					if (data.flag)
						data.flag = 2;
					
					if (((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
						&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
					{
						((CFuturesDlg*)m_pWnd)->SetCode(data.code);
						((CFuturesDlg*)m_pWnd)->SetName(data.name);
						
						m_sCode = data.code;
						
						((CFuturesDlg*)m_pWnd)->SetButton();
						((CFuturesDlg*)m_pWnd)->UpdateData(FALSE);
					}
					AddData(jj, nIndex, data);
					Invalidate();		
				}
			}
		}
	}

	if ((point.y > 40 && point.y < 319) && (0 < point.x && point.x< 130) || (point.x > 192 && point.x < 321))
	{
		if (((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
			&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
		{
			if (!m_sCode.IsEmpty())
				((CFuturesDlg*)m_pWnd)->EndDialog(1);
		}
	}

	int nSelect = 0;

	//2014.07.29 KSJ 컨텍스트 메뉴 띄우기
	if ((point.y > 20 && point.y < 40) && (0 < point.x && point.x < 64))
	{
		m_bCall = true;

		std::unique_ptr<CMenu> popM = std::make_unique<CMenu>();
		popM->CreatePopupMenu();
		for (int ii = 2; ii < m_Array.GetSize(); ii++)
		{
			if (m_Array[m_Array.GetSize() - ii].Compare("0000") && m_Array[m_Array.GetSize() - ii].Compare("000000"))	//2015.07.02 KSJ 0000일때는 넣어주지 않는다.
			{
				stmp = m_Array[m_Array.GetSize() - ii];
				stmp = stmp.Left(2) + "." + stmp.Mid(2, 2) + "." + stmp.Right(2);
				popM->AppendMenu(MF_STRING, ii - 2, stmp);
			}
		}

		CPoint pot;
		GetCursorPos(&pot);
		nSelect = popM->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pot.x, pot.y, this, CRect(0, 0, 40, 80));

		nSelect += STARTWNUM;

		m_nWCallSelect = nSelect;
		Invalidate();
	}
	else if ((point.y > 20 && point.y < 40) && (point.x > 254 && point.x < 320))
	{
		m_bCall = false;

		std::unique_ptr<CMenu> popM = std::make_unique<CMenu>();
		popM->CreatePopupMenu();
		for (int ii = 2; ii < m_Array.GetSize(); ii++)
		{
			if (m_Array[m_Array.GetSize() - ii].Compare("0000") && m_Array[m_Array.GetSize() - ii].Compare("000000"))	//2015.07.02 KSJ 0000일때는 넣어주지 않는다.
			{
				stmp = m_Array[m_Array.GetSize() - ii];
				stmp = stmp.Left(2) + "." + stmp.Mid(2, 2) + "." + stmp.Right(2);
				popM->AppendMenu(MF_STRING, ii - 2, stmp);
			}
		}

		CPoint pot;
		GetCursorPos(&pot);
		nSelect = popM->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pot.x, pot.y, this, CRect(0, 0, 40, 80));

		nSelect += STARTWNUM;

		m_nWPutSelect = nSelect;
		Invalidate();
	}
}

void CCheckBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CCheckBox::RemoveAll()
{
	m_Map.RemoveAll();
	m_nPutSelect = STARTNUM;
	m_nCallSelect = STARTNUM;
	m_nWPutSelect = STARTWNUM;
	m_nWCallSelect = STARTWNUM;
}

void CCheckBox::SetArray()
{
	CArray<CString, CString> *pArray;

	pArray = ((CFuturesDlg*)m_pWnd)->GetArray();
	m_Array.RemoveAll();

	for (int jj = 0 ; jj < pArray->GetSize() ; jj++)
		m_Array.Add(pArray->GetAt(jj));
	
	pArray = nullptr;
}

BOOL CCheckBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	POINT	pt;
	
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	if (pt.y > 40 && pt.x < 321)
	{
		if (m_hCursor)
		{
			if ((0 < pt.x && pt.x < 130) || (321 > pt.x && pt.x > 192) )
			{
				::SetCursor(m_hCursor);
				return TRUE;
			}
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

DWORD CCheckBox::SetCheckCursor(int nCursorId, BOOL bRepaint)
{
	HINSTANCE hInstResource = nullptr;

	if (m_hCursor)
	{
		::DestroyCursor(m_hCursor);
		m_hCursor = nullptr;
	}

	if (nCursorId)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nCursorId), RT_GROUP_CURSOR);
		m_hCursor = (HCURSOR)::LoadImage(hInstResource, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);
		if (bRepaint)
			Invalidate();
		if (m_hCursor == nullptr)
			return FALSE;
	}

	return TRUE;
} 

void CCheckBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if ((point.y > 40 && point.y < 319) && (0 < point.x && point.x< 130) || (point.x > 192 && point.x < 321))
	{
		if (((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
			&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
		{
			OnLButtonDown(nFlags, point);
			if (!m_sCode.IsEmpty())
				((CFuturesDlg*)m_pWnd)->EndDialog(1);
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CCheckBox::ShowRow(int row)
{
	ResetScrollBars();
	CRect	rc(0,41,321,265);

	if (row < 5)
		row = 0;
	else if (row > m_nCount - 4)
		row += 4;
	else	row -= 4;

	SetInit(GetInit() - row*ROWHIGHT); 
	SetScroll(GetScroll() + row*ROWHIGHT);
	InvalidateRect(rc);
	SetScrollPos32(SB_VERT, GetScroll());
}

void CCheckBox::DeleteTool()
{
	if (m_pTool)
		m_pTool->DeleteDC();
}

void CCheckBox::SetMonth(int row)
{
	int nRow = row, nIndex = 0;
	CRect	rc;
	DATA	data;
	const int	col[10] = {0, 32, 64, 96, 128, 192, 222, 254, 288, 320};
	int	icol = 0;

	CString strData;


	const int nCount = m_Array.GetSize();
	
	if(nCount == 0)	return;	//2015.07.14 KSJ 사이즈가 0이면 그리지 않음.


	{
		icol = 0;
		nIndex = STANDARDNUM - m_nCallSelect;

		strData = m_Array.GetAt(m_nCallSelect == 0 ? 0:nIndex);
		m_pTool->FrameRect(CALLMONRECT, USER_BT_NORMALCOR1);
		m_pTool->WriteText(strData, CALLMONRECT);
		m_pTool->DrawCell(CALLMONRECT);

		for (int jj = 0; jj < m_nCount; jj++)
		{	
			// col index
			rc.SetRect(col[icol], nRow, col[icol + 1], nRow+ROWHIGHT);
			m_pTool->FrameRect(rc, RGB(229,229,230));
			
			// BOOL값 처리... 
			m_Map.Lookup((jj*ROWCOUNT + icol + nIndex), data); 
			if (data.flag)
			{
				m_BitmapNormal.Draw(m_pTool->GetDC(), rc);
				
				if (data.flag == 2 && ((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
					&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
				{	
					m_BitmapClick.Draw(m_pTool->GetDC(), rc);
					m_Key = jj*ROWCOUNT + icol + nIndex;
				}
			}
			m_pTool->DrawCell(rc);
			nRow += ROWHIGHT;
		}
	}
	
	{
		nRow = row;
		icol = 8;
		nIndex = m_nPutSelect + STANDARDNUM -1;

		strData = m_Array.GetAt(abs(m_nPutSelect == 0 ? 0:m_nPutSelect - STANDARDNUM));
		m_pTool->FrameRect(PUTMONRECT, USER_BT_NORMALCOR1);
		m_pTool->WriteText(strData, PUTMONRECT);
		m_pTool->DrawCell(PUTMONRECT);

		for (int jj = 0; jj < m_nCount; jj++)
		{	
			// col index
			rc.SetRect(col[icol], nRow, col[icol + 1], nRow+ROWHIGHT);
			m_pTool->FrameRect(rc, RGB(229,229,230));
			
			// BOOL값 처리... 
			m_Map.Lookup((jj*ROWCOUNT + nIndex), data); 
			if (data.flag)
			{
				m_BitmapNormal.Draw(m_pTool->GetDC(), rc);
				
				if (data.flag == 2 && ((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
					&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
				{	
					m_BitmapClick.Draw(m_pTool->GetDC(), rc);
					m_Key = jj*ROWCOUNT + nIndex;
				}
			}
			m_pTool->DrawCell(rc);
			nRow += ROWHIGHT;
		}
	}
}

void CCheckBox::SetWeekMonth(int row)
{
	int nRow = row, nIndex = 0;
	CRect	rc, tmprc;
	DATA	data;
	const int	col[6] = {0, 64, 128, 192, 254, 320};
	int	icol = 0;
	
	CString strData;
	
	
	const int nCount = m_Array.GetSize();
	
	if(nCount == 0)	return;	//2015.07.14 KSJ 사이즈가 0이면 그리지 않음.
	{
		icol = 0;
		nIndex = STANDARDNUM - m_nWCallSelect;
		strData = m_Array.GetAt(m_nWCallSelect == 0 ? 0 : nIndex); 
		m_pTool->FrameRect(CRect(0, 20, 64, 40), USER_BT_NORMALCOR1);
		m_pTool->WriteText(strData, CRect(0, 20, 64, 40));
		m_pTool->DrawCell(CRect(0, 20, 64, 40));
		
		for (int jj = 0; jj < m_nCount; jj++)
		{	
			// col index
			rc.SetRect(col[icol], nRow, col[icol + 1], nRow+ROWHIGHT);
			m_pTool->FrameRect(rc, RGB(229,229,230));
		
			// BOOL값 처리...  
			m_Map.Lookup((jj*ROWCOUNT + icol + nIndex), data);
			if (data.flag)
			{
				tmprc = rc;
				tmprc = rc;
				tmprc.top += 2;
				tmprc.bottom -= 2;
				tmprc.left += 16;
				tmprc.right -= 16;

				m_BitmapNormal.Draw(m_pTool->GetDC(), tmprc);
				
				if (data.flag == 2 && ((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
					&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
				{	
					m_BitmapClick.Draw(m_pTool->GetDC(), rc);
					m_Key = jj*ROWCOUNT + icol + nIndex;
				}
			}
			m_pTool->DrawCell(rc);
			
			nRow += ROWHIGHT;
		}
	}
	
	{
		nRow = row;
		icol = 4;
		nIndex = m_nWPutSelect + STANDARDNUM;
		strData = m_Array.GetAt(abs(m_nWPutSelect == 0 ? 0 : m_nWPutSelect - STANDARDNUM));
		m_pTool->FrameRect(CRect(254, 20, 320, 40), USER_BT_NORMALCOR1);
		m_pTool->DrawCell(CRect(254, 20, 320, 40));
		
		for (int jj = 0; jj < m_nCount; jj++)
		{	
			// col index
			rc.SetRect(col[icol], nRow, col[icol + 1], nRow+ROWHIGHT);
			m_pTool->FrameRect(rc, RGB(229,229,230));
			
			// BOOL값 처리... 
			m_Map.Lookup((jj*ROWCOUNT + nIndex), data);
			if (data.flag)
			{
				tmprc = rc;
				tmprc = rc;
				tmprc.top += 2;
				tmprc.bottom -= 2;
				tmprc.left += 16;
				tmprc.right -= 16;
				m_BitmapNormal.Draw(m_pTool->GetDC(), tmprc);
				
				if (data.flag == 2 && ((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
					&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
				{	
					m_BitmapClick.Draw(m_pTool->GetDC(), rc);
					m_Key = jj*ROWCOUNT + nIndex;
				}
			}
			m_pTool->DrawCell(rc);
			nRow += ROWHIGHT;
		}
	}
}

void CCheckBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bWeek)
	{
		weeklbtnClick(nFlags, point);
	}
	else
	{
		DATA	data;
		int	val{};
		const int col[10] = { 0, 30, 62, 94, 130, 192, 224, 256, 290, 320 };
		int nIndex{};

		m_Map.Lookup(m_Key, data);
		data.flag = TRUE;
		m_Map.SetAt(m_Key, data);
		m_Key = -1;

		data.flag = FALSE;
		data.code = "";
		data.name = "";

		if (m_nInit == 40)
			val = 0;
		else if (m_nInit < 40 && m_nInit > 0)
			val = m_nInit;
		else if (m_nInit <= 0)
			val = 40 + abs(m_nInit);

		if (point.y > 40 && point.y < 319)
		{
			for(int jj = 0; jj < m_nCount ; jj++)
			{
				for (int ii = 0; ii < 9 ; ii++)
				{
					if (jj*ROWHIGHT< point.y - 40 + val && point.y - 40 + val < ROWHIGHT + jj*ROWHIGHT 
						&& col[ii] < point.x && point.x < col[ii+1])
					{	
						if(ii == 0 || ii == 8)
						{
							if(ii == 0)	nIndex = STANDARDNUM - m_nCallSelect + ii;
							else		nIndex = STANDARDNUM + m_nPutSelect;
							
						}
						else if (ii != 4)
						{
							nIndex = ii + 7;	//2014.08.11 KSJ 가운데에 가까운 부분만 보이기 때문에
						}

						m_Map.Lookup(jj*ROWCOUNT + nIndex, data);
						if (data.flag)
							data.flag = 2;
						
						if (((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
							&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
						{
							((CFuturesDlg*)m_pWnd)->SetCode(data.code);
							((CFuturesDlg*)m_pWnd)->SetName(data.name);
							
							m_sCode = data.code;
							
							((CFuturesDlg*)m_pWnd)->SetButton();
							((CFuturesDlg*)m_pWnd)->UpdateData(FALSE);
						}
						AddData(jj, nIndex, data);
						Invalidate();		
					}
				}
			}
		}

		if ((point.y > 40 && point.y < 319) && (0 < point.x && point.x< 130) || (point.x > 192 && point.x < 321))
		{
			if (((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
				&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
			{
				if (!m_sCode.IsEmpty())
					((CFuturesDlg*)m_pWnd)->EndDialog(1);
			}
		}

		int nSelect = 0;
		nIndex = 0;

		//2014.07.29 KSJ 컨텍스트 메뉴 띄우기
		if ((point.y > 20 && point.y < 40) && (0 < point.x && point.x< 32))
		{
			m_bCall = true;

			std::unique_ptr<CMenu> popM = std::make_unique<CMenu>();
			popM->CreatePopupMenu();
			for(int ii = 3; ii < m_Array.GetSize(); ii++)
			{
				if(m_Array[m_Array.GetSize()-(ii+1)].Compare("0000"))	//2015.07.02 KSJ 0000일때는 넣어주지 않는다.
					popM->AppendMenu(MF_STRING, ii - 3, m_Array[m_Array.GetSize()-(ii+1)]);
			}
			
			CPoint pot;
			GetCursorPos(&pot);
			nSelect = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, pot.x, pot.y, this, CRect(0,0,40,80));
			
			nSelect += STARTNUM;

			m_nCallSelect = nSelect;
			Invalidate();
		}
		else if((point.y > 20 && point.y < 40) && (point.x > 288 && point.x < 320))
		{
			m_bCall = false;

			std::unique_ptr<CMenu> popM = std::make_unique<CMenu>();
			popM->CreatePopupMenu();
			for(int ii = 3; ii < m_Array.GetSize(); ii++)
			{
				if(m_Array[m_Array.GetSize()-(ii+1)].Compare("0000"))	//2015.07.02 KSJ 0000일때는 넣어주지 않는다.
					popM->AppendMenu(MF_STRING, ii - 3, m_Array[m_Array.GetSize()-(ii+1)]);
			}
			
			CPoint pot;
			GetCursorPos(&pot);
			nSelect = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, pot.x, pot.y, this, CRect(0,0,40,80));
			
			nSelect += STARTNUM;

			m_nPutSelect = nSelect;
			Invalidate();
		}
	}
	CWnd::OnLButtonUp(nFlags, point);
}
