#include "stdafx.h"
#include "StockCheckBox.h"
#include "resource.h"
#include "futuresdlg.h"

#include "math.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ROWCOUNT		18	//월물갯수 + 기초자산
#define ROWHIGHT		20
#define ROWVISIBLE		3	//보이는 갯수
#define ROWOFFSET		5	//컨텍스트메뉴로 들어갈 갯수

#define SFMONRECT		CRect(200, 20, 230, 40)
#define SPMONRECT		CRect(290, 20, 320, 40)

/////////////////////////////////////////////////////////////////////////////
// CCheckBox

CStockCheckBox::CStockCheckBox()
{
	m_nInit   = 40;
	m_Key     = -1;

	m_BitmapNormal.LoadBitmap(IDB_BITMAP_NORMAL);
	m_BitmapClick.LoadBitmap(IDB_BITMAP_CLICK);

	m_nSFSelect = ROWVISIBLE;
	m_nSPSelect = ROWVISIBLE;

	m_nColCount = ROWCOUNT;
	m_nSearchRow = -1;

	//초기화
	for( int ii = 0 ; ii < 50; ii++)
	{
		m_nArrSearch[ii] = -1;
	}
	m_pTool = std::make_unique<CTool>();
}

CStockCheckBox::~CStockCheckBox()
{		
	if (m_hCursor)
		::DestroyCursor(m_hCursor);
	m_hCursor = nullptr;
	
	DestroyWindow();
	m_BitmapNormal.DeleteObject();
	m_BitmapClick.DeleteObject();
}


BEGIN_MESSAGE_MAP(CStockCheckBox, CWnd)
	//{{AFX_MSG_MAP(CCheckBox)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckBox message handlers

void CStockCheckBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC	memDC(&dc);

	
	m_pTool->SetDC(&memDC);
	Draw();
	m_pTool->SetDC(nullptr);

//	delete m_pTool;
//	m_pTool = nullptr;
}

void CStockCheckBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	//CRect	rc(0,41,321,265);
	CRect rc;
	GetClientRect(&rc);
	rc.top = 41;
	CRect	clientRc;

	GetClientRect(&clientRc);
	const int nVisibleRowCnt = clientRc.Height() / 20 - 1;

	const int	scrollPos = GetScrollPos32(SB_VERT);
	const int	lim = -(m_nCount - (nVisibleRowCnt + 1)) * 20;
// 	CString s;
	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nInit > -20 * ((m_nCount-1) - nVisibleRowCnt))
		{
			m_nInit   -= 20;
			m_nScroll += 20;
		}

		if (m_nInit < lim)
			m_nInit = lim;
		InvalidateRect(rc);
		SetScrollPos32(SB_VERT, m_nScroll);
		break;

        case SB_LINEUP:
		if (m_nInit < 40)
		{
			m_nInit   += 20;
			m_nScroll -= 20;
		}
		InvalidateRect(rc);
		SetScrollPos32(SB_VERT, m_nScroll);
		break;

        case SB_PAGEDOWN:
		if (m_nInit > -20*((m_nCount) - 2))
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
			const int n = nPos / 20;
			m_nInit = -(n-1)* 20;
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

BOOL CStockCheckBox::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_POS;
	sinfo.nPos   = nPos;
	return SetScrollInfo(nBar, &sinfo, bRedraw);
}

int CStockCheckBox::GetScrollPos32(int nBar, BOOL bGetTrackPos)
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

void CStockCheckBox::Draw()
{
	CRect	rc;
	CStringArray aStr;
// 	int	row1[5] = {96, 128, 160, 192, 224};
// 	int	row2[4] = {224, 256, 288, 320};
	const 	int	row1[5] = {110, 140, 170, 200, 230};
	const 	int	row2[4] = {230, 260, 290, 320};

	GetClientRect(rc);
	m_pTool->FrameRect(rc, USER_BACKCOLOR2);

	DrawList(m_nInit);	

	// header
	rc.SetRect(0, 0, 320, 40);
	m_pTool->FrameRect(rc, USER_BACKCOLOR2);

	rc.SetRect(0, 0, 110, 40);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("기초자산", rc);
	m_pTool->DrawCell(rc);

	rc.SetRect(110, 0, 230, 20);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("기본종목", rc);
	m_pTool->DrawCell(rc);

	rc.SetRect(230, 0, 320, 20);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("스프레드종목", rc);
	m_pTool->DrawCell(rc);

	//2014.08.24 KSJ 그리는 부분이므로 4개가 맞음.
	//int nCount = m_Array.GetSize();
	for (int ii = 0; ii < 4; ii++)
	{
		CString tmp = m_Array.GetAt(ii);

		rc.SetRect(row1[ii], 20, row1[ii+1], 40);

		if(ii == 3)
		{
			tmp = m_Array.GetAt(m_nSFSelect);
			m_pTool->FrameRect(SFMONRECT, USER_BT_NORMALCOR1);
			m_pTool->WriteText(tmp, SFMONRECT);
			m_pTool->DrawCell(SFMONRECT);
		}
		else
		{
			m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
			m_pTool->WriteText(m_Array[ii], rc);
			m_pTool->DrawCell(rc);
		}
	}
	
	CString spreadText;

	for (int ii=0; ii < 3; ii++)
	{
		rc.SetRect(row2[ii], 20, row2[ii+1], 40);

		if(ii == 2)
		{
			spreadText.Format("제 %d",m_nSPSelect);

			m_pTool->FrameRect(SPMONRECT, USER_BT_NORMALCOR1);
			m_pTool->WriteText(spreadText, SPMONRECT);
			m_pTool->DrawCell(SPMONRECT);
		}
		else
		{
			spreadText.Format("제 %d",ii + 1);

			m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
			m_pTool->WriteText(spreadText, rc);
			m_pTool->DrawCell(rc);
		}
	}	
}

void CStockCheckBox::DrawList(int row)
{
	const int nRow = row;
	CRect	rc;
	CString	temp;
	DATA	data;
//	int	col[9] = {0, 96, 128, 160, 192, 224, 256, 288, 320};
	const int	col[9] = {0, 110, 140, 170, 200, 230, 260, 290, 320};

 	CString s;
	
	// row index
	for (int jj = 0; jj < m_nCount; jj++)
	{	
		// col index
		for (int ii = 0; ii < 8; ii++)
		{
			rc.SetRect(col[ii], row, col[ii+1], row+ROWHIGHT);
			m_pTool->FrameRect(rc, RGB(229,229,230));

			if (ii == 0)
			{
				data.name.TrimLeft();

				m_Map.Lookup(jj*m_nColCount+ii, data);

				//2014.08.27 KSJ 찾았을때 색깔 표시
				if(m_nSearchRow == jj)
				{
					m_pTool->FrameRect(rc, COR_BLACK);//RGB(239,239,239));
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
				if(ii > 4)
				{
					m_Map.Lookup((jj*m_nColCount+ii + 5), data);
				}
				else
					m_Map.Lookup((jj*m_nColCount+ii), data); 

				if (data.flag)
				{
					m_BitmapNormal.Draw(m_pTool->GetDC(), rc);
					
					if (data.flag == 2 && ((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
						&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
					{	
						m_BitmapClick.Draw(m_pTool->GetDC(), rc);
						m_Key = jj*m_nColCount+ii;
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

//2015.09.09 KSJ nNext 종목서치(0:현재(처음조회), 1:앞으로, -1:뒤로)
void CStockCheckBox::SearchStock(CString strName, int nNext)
{
	DATA	data;

	if(nNext == 0)
	{
		//초기화
		for( int ii = 0 ; ii < 50; ii++)
		{
			m_nArrSearch[ii] = -1;
		}

		//검색된거 저장
		for (int jj = 0, nIndex = 0; jj < m_nCount; jj++)
		{	
			// col index
			m_Map.Lookup(jj*m_nColCount, data);
			
			CString strUpperS, strUpperO;
			strUpperS = strName;
			strUpperO = data.name;
			strUpperS.MakeUpper(); strUpperS.TrimLeft(); strUpperS.TrimRight();
			strUpperO.MakeUpper(); strUpperO.TrimLeft(); strUpperO.TrimRight();
			
			if (strUpperO.Find(strUpperS) < 0 || strUpperS.IsEmpty())
				continue;			
			
			if(strUpperS.IsEmpty())		m_nSearchRow = -1;
			else						m_nSearchRow = jj;	//2014.08.27 색깔지정할 row
			
			m_nArrSearch[nIndex++] = jj;
			//ShowRow(jj);
			//break;
		}
		
		m_nCurrIndex = 0;
		m_nSearchRow = m_nArrSearch[m_nCurrIndex];
		ShowRow(m_nSearchRow);
	}
	else if( nNext == 1) // 앞으로
	{
		m_nCurrIndex++;

		if(m_nCurrIndex < 50 && m_nArrSearch[m_nCurrIndex] != -1)
		{
			m_nSearchRow = m_nArrSearch[m_nCurrIndex];
			ShowRow(m_nSearchRow);
		}
		else
		{
			m_nCurrIndex--;			
		}
	}
	else if(nNext == -1) // 뒤로
	{
		m_nCurrIndex--;
		
		if(m_nCurrIndex > -1 && m_nArrSearch[m_nCurrIndex] != -1)
		{
			m_nSearchRow = m_nArrSearch[m_nCurrIndex];
			ShowRow(m_nSearchRow);
		}
		else
		{
			m_nCurrIndex++;			
		}
	}
}

void CStockCheckBox::ResetScrollBars()
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

void CStockCheckBox::AddData(int row, int col, const DATA& data)
{	
	m_Map.SetAt(row*m_nColCount + col, data);

}

DATA CStockCheckBox::GetData(int row, int col)
{
	DATA data;

	m_Map.Lookup(row*m_nColCount + col, data);
	return data;
}

void CStockCheckBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	DATA	data;

	//	int	val, col[9] = {0, 96, 128, 160, 192, 224, 256, 288, 320};
	const int col[9] = {0, 110, 140, 170, 200, 230, 260, 290, 320};
	int nIndex{}, val{};

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

	if ((point.y > 40 && point.y < 319) && (109 < point.x && point.x< 321))
	{
		for(int jj = 0; jj < m_nCount ; jj++)
		{
			for (int ii = 0; ii < 8; ii++)
			{
				if (jj*20< point.y - 40 + val && point.y - 40 + val < 20 + jj*ROWHIGHT 
					&& col[ii] < point.x && point.x < col[ii+1])
				{	
					if(ii == 4 || ii == 7)
					{
						if(ii == 4)	nIndex = m_nSFSelect + ii - ROWVISIBLE;
						else		nIndex = m_Array.GetSize() == 7 ? m_nSPSelect + ii : m_nSPSelect + ii + ROWVISIBLE - 1;	//2015.12.07 KSJ 실제 선택한것과 보이는것 차이가 2난다.
					}
					else if (ii != 0)
					{
						if(ii < 4)	nIndex = ii;
						else		nIndex = m_Array.GetSize() == 7 ? ii + ROWVISIBLE : ii + ROWOFFSET;
					}

					m_Map.Lookup(jj*m_nColCount+ nIndex, data);
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

	if ((point.y > 40 && point.y < 319) && (109 < point.x && point.x< 321))
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
	if ((point.y > 20 && point.y < 40) && (200 < point.x && point.x< 230))
	{
		m_bSpread = false;
		
		std::unique_ptr<CMenu> popM = std::make_unique<CMenu>(); 
		popM->CreatePopupMenu();
		for(int ii = 0; ii < m_Array.GetSize() - ROWVISIBLE; ii++)	//2014.08.26 KSJ 보이는 갯수가 4개임.
		{
			popM->AppendMenu(MF_STRING, ii, m_Array[ii + ROWVISIBLE]);
		}
		
		CPoint pot;
		GetCursorPos(&pot);
		nSelect = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, pot.x, pot.y, this, CRect(0,0,40,80));
		nSelect += ROWVISIBLE;
		//delete popM;
		
		m_nSFSelect = nSelect;
		Invalidate();
	}
	else if((point.y > 20 && point.y < 40) && (point.x > 290 && point.x < 320))
	{
		m_bSpread = TRUE;
		CString strText;

		std::unique_ptr<CMenu> popM = std::make_unique<CMenu>();
		popM->CreatePopupMenu();
		for(int ii = 0; ii < m_Array.GetSize() - ROWVISIBLE; ii++)	//2014.08.26 KSJ 보이는 갯수가 3개임. 스프레드가 선물보다 하나적음.
		{
			strText.Format("제 %d",ii + ROWVISIBLE);
			popM->AppendMenu(MF_STRING, ii, strText);
		}
		
		CPoint pot;
		GetCursorPos(&pot);
		nSelect = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, pot.x, pot.y, this, CRect(0,0,40,80));
		
		nSelect += ROWVISIBLE;
		//delete popM;
		
		m_nSPSelect = nSelect;
		Invalidate();
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CStockCheckBox::RemoveAll()
{
	m_Map.RemoveAll();
}

void CStockCheckBox::SetArray()
{
	CArray<CString, CString> *pArray;

	pArray = ((CFuturesDlg*)m_pWnd)->GetArray();
	m_Array.RemoveAll();

	for (int jj = 0 ; jj < pArray->GetSize() ; jj++)
	{
		m_Array.Add(pArray->GetAt(jj));
	}

	pArray = nullptr;
}

BOOL CStockCheckBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	POINT	pt;
	
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	if (pt.y > 40 && pt.x < 321)
	{
		if (m_hCursor)
		{
			if ((110 < pt.x && pt.x < 140) || (321 > pt.x && pt.x > 200) )
			{
				::SetCursor(m_hCursor);
				return TRUE;
			}
		}
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

DWORD CStockCheckBox::SetCheckCursor(int nCursorId, BOOL bRepaint)
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

void CStockCheckBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if ((point.y > 40 && point.y < 319) && (110 < point.x && point.x< 140) || (point.x > 200 && point.x < 321))
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

void CStockCheckBox::ShowRow(int row)
{
	//2015.09.09 KSJ -1이오면 0으로 세팅해준다.
	if(row == -1) row = 0;

	ResetScrollBars();

	CRect rc;
	GetClientRect(&rc);
	rc.top = 41;

	CRect	clientRc;
	
	GetClientRect(&clientRc);
	const int nVisibleRowCnt = clientRc.Height() / 20 - 1;
	
	const int	scrollPos = GetScrollPos32(SB_VERT);
	const int	lim = -(m_nCount - (nVisibleRowCnt + 1)) * ROWHIGHT;

	const int nBottom = (m_nCount -1)* ROWHIGHT;	//2015.07.08 KSJ 스크롤 제한.

// 	if (row < 12)
// 		row = 0;
// 	else if (row > 12)
// 		row -= 6;

	SetInit(40 - row*ROWHIGHT); 

	if (m_nInit < lim)
		m_nInit = lim;

// 	if(m_nInit < -920)
// 		m_nInit = -920;

	SetScroll(row*ROWHIGHT);

	if(m_nScroll > nBottom)
		m_nScroll = nBottom;

	InvalidateRect(rc);
	SetScrollPos32(SB_VERT, GetScroll());
}

void CStockCheckBox::DeleteTool()
{
	if (m_pTool)
		m_pTool->DeleteDC();
}

void CStockCheckBox::SetMonth(int row)
{
	int nRow = row, nIndex = 0;
	CRect	rc;
	DATA	data;

	const int	col[9] = {0, 110, 140, 170, 200, 230, 260, 290, 320};
	int	icol = 0;
	
	CString strData;
	
	{
		icol = 4;
		nIndex = m_nSFSelect;

 		CString s;
		
		strData = m_Array.GetAt(m_nSFSelect == 0 ? 1+ROWVISIBLE:nIndex);

		m_pTool->FrameRect(SFMONRECT, USER_BT_NORMALCOR1);
		m_pTool->WriteText(strData, SFMONRECT);
		m_pTool->DrawCell(SFMONRECT);
		
		for (int jj = 0; jj < m_nCount; jj++)
		{	
			// col index
			rc.SetRect(col[icol], nRow, col[icol + 1], nRow+ROWHIGHT);
			m_pTool->FrameRect(rc, RGB(229,229,230));
			
			// BOOL값 처리... 
			m_Map.Lookup((jj*m_nColCount + nIndex + 1), data); 
			if (data.flag)
			{
				m_BitmapNormal.Draw(m_pTool->GetDC(), rc);
				
				if (data.flag == 2 && ((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
					&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
				{	
					m_BitmapClick.Draw(m_pTool->GetDC(), rc);
					m_Key = jj*m_nColCount + nIndex;
				}
			}
			m_pTool->DrawCell(rc);
			nRow += ROWHIGHT;
		}
	}
	
	{
		nRow = row;
		icol = 7;
		nIndex = m_nSPSelect;
		
		strData.Format("제 %d",nIndex);
		m_pTool->FrameRect(SPMONRECT, USER_BT_NORMALCOR1);
		m_pTool->WriteText(strData, SPMONRECT);
		m_pTool->DrawCell(SPMONRECT);
		
		for (int jj = 0; jj < m_nCount; jj++)
		{	
			// col index
			rc.SetRect(col[icol], nRow, col[icol + 1], nRow+ROWHIGHT);
			m_pTool->FrameRect(rc, RGB(229,229,230));
			
			// BOOL값 처리... 
			m_Map.Lookup((jj*m_nColCount + icol + 2 + nIndex/* + ROWOFFSET -1*/), data); 
			if (data.flag)
			{
				m_BitmapNormal.Draw(m_pTool->GetDC(), rc);
				
				if (data.flag == 2 && ((CFuturesDlg*)m_pWnd)->GetKind() != futureCODE
					&& ((CFuturesDlg*)m_pWnd)->GetKind() != spreadCODE)
				{	
					m_BitmapClick.Draw(m_pTool->GetDC(), rc);
					m_Key = jj*m_nColCount + icol + nIndex/* + ROWOFFSET -1*/;
				}
			}
			m_pTool->DrawCell(rc);
			nRow += ROWHIGHT;
		}
	}
}
