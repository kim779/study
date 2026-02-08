// DrSplitterCtrl.cpp: implementation of the CDrSplitterCtrl class.
//
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//
// DrSplitterCtrl.cpp

#include "stdafx.h"
#include "DrSplitterCtrl.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define CX_BORDER   1
#define CY_BORDER   1

static BOOL bNotWin4;

BEGIN_MESSAGE_MAP(CDrSplitterCtrl, CSplitterWnd)
//{{AFX_MSG_MAP(CDrSplitterCtrl)
ON_WM_SIZE()
ON_WM_ERASEBKGND()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
//
// CDrSplitterCtrl
CDrSplitterCtrl::CDrSplitterCtrl()
{
	m_nResizeRow = -1;
	m_bFirstSizing = false;
	
	m_cxBorder = 0;
	m_cyBorder = 0;
 	m_cxSplitter = m_cySplitter = 3;
 	m_cxSplitterGap = m_cySplitterGap = 3;
	
	DWORD dwVersion = ::GetVersion();
	BOOL bWin4 = (BYTE)dwVersion >= 4;
	bNotWin4 = 1 - bWin4;   // for convenience
	
	m_upBorder = 3;
	m_bWhiteLine = true;

	m_clrSplitBar = COLOR_SPLITTER;
}

void CDrSplitterCtrl::SetShow_Col(BOOL bHide, int nCol)
{
	if (bHide == TRUE)
		SetHide_Col(nCol);
	else
		SetShow_Col(nCol);
}

void CDrSplitterCtrl::SetShow_Col(int nCol)
{
	if (m_nCols >= m_nMaxCols)
		return;

	ASSERT_VALID(this);
	ASSERT(nCol < m_nMaxCols);
	
	int cxNew = m_pColInfo[m_nCols].nCurSize;
	m_nCols++;  // add a column
	ASSERT(m_nCols == m_nMaxCols);
	
	// fill the hided column
	int nIndex_Col, nIndex_Row;

	for (nIndex_Row = 0; nIndex_Row < m_nRows; nIndex_Row++)
	{
		CWnd* pPaneShow = GetDlgItem(AFX_IDW_PANE_FIRST + nIndex_Row * 16 + m_nCols);
		ASSERT(pPaneShow != NULL);
		pPaneShow->ShowWindow(SW_SHOWNA);
		
		for (nIndex_Col = m_nCols - 2; nIndex_Col >= nCol; nIndex_Col--)
		{
			CWnd* pPane = GetPane(nIndex_Row, nIndex_Col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(nIndex_Row, nIndex_Col + 1));
		}
		
		pPaneShow->SetDlgCtrlID(IdFromRowCol(nIndex_Row, nCol));
	}
	
	// new panes have been created -- recalculate layout
	for (nIndex_Col = nCol + 1; nIndex_Col < m_nCols; nIndex_Col++)
		m_pColInfo[nIndex_Col].nIdealSize = m_pColInfo[nIndex_Col - 1].nCurSize;
	
	m_pColInfo[nCol].nIdealSize = cxNew;
	RecalcLayout();
}

void CDrSplitterCtrl::SetHide_Col(int nCol)
{
	ASSERT_VALID(this);
	
	ASSERT(m_nCols > 1);
	ASSERT(nCol < m_nCols);
	
	// if the column has an active window -- change it
	int rowActive, colActive;
	if (GetActivePane(&rowActive, &colActive) != NULL && colActive == nCol)
	{
		if (++colActive >= m_nCols)
			colActive = 0;
		SetActivePane(rowActive, colActive);
	}
	
	// hide all column panes
	int nIndex_Col, nIndex_Row;
	for (nIndex_Row = 0; nIndex_Row < m_nRows; nIndex_Row++)
	{
		CWnd* pPaneHide = GetPane(nIndex_Row, nCol);
		ASSERT(pPaneHide != NULL);
		pPaneHide->ShowWindow(SW_HIDE);
		pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + nIndex_Row * 16 + m_nCols);
		
		for (nIndex_Col = nCol + 1; nIndex_Col < m_nCols; nIndex_Col++)
		{
			CWnd* pPane = GetPane(nIndex_Row, nIndex_Col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(nIndex_Row, nIndex_Col - 1));
		}
	}
	
	m_nCols--;
	m_pColInfo[m_nCols].nCurSize = m_pColInfo[nCol].nCurSize;
	RecalcLayout();
}

void CDrSplitterCtrl::SetShow_Row(BOOL bHide, int nRow)
{
	if (bHide == TRUE)
		SetHide_Row(nRow);
	else
		SetShow_Row(nRow);
}

void CDrSplitterCtrl::SetHide_Row(int nRow)
{
	if (nRow >= m_nMaxRows)
		return;
	
	ASSERT_VALID(this);
	
	ASSERT(m_nRows > 1);
	ASSERT(nRow < m_nRows);
	
	int rowActive, colActive;
	if (GetActivePane(&rowActive, &colActive) != NULL && rowActive == nRow)
	{
		if (++rowActive >= m_nRows)
			rowActive = 0;
		SetActivePane(rowActive, colActive);
	}

	int nIndex_Col, nIndex_Row;
	for (nIndex_Col = 0; nIndex_Col < m_nCols; nIndex_Col++)
	{
		CWnd* pPaneHide = GetPane(nRow, nIndex_Col);
		ASSERT(pPaneHide != NULL);
		pPaneHide->ShowWindow(SW_HIDE);
		pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + m_nRows * 16 + nIndex_Col);
		
		for (nIndex_Row = nRow + 1; nIndex_Row < m_nRows; nIndex_Row++)
		{
			CWnd* pPane = GetPane(nIndex_Row, nIndex_Col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(nIndex_Row - 1, nIndex_Col));
		}
	}
	
	m_nRows--;
	m_pColInfo[m_nRows].nCurSize = m_pColInfo[nRow].nCurSize;
	RecalcLayout();
}

void CDrSplitterCtrl::SetShow_Row(int nRow)
{
	if (m_nRows >= m_nMaxRows)
		return;
	
	ASSERT_VALID(this);
	int cxNew = m_pRowInfo[m_nRows].nCurSize;
	m_nRows++;
	ASSERT(m_nRows == m_nMaxRows);
	
	int nIndex_Col, nIndex_Row;
	for (nIndex_Col = 0; nIndex_Col < m_nCols; nIndex_Col++)
	{
		CWnd* pPaneShow = GetDlgItem(AFX_IDW_PANE_FIRST + m_nRows * 16 + nIndex_Col);
		ASSERT(pPaneShow != NULL);
		pPaneShow->ShowWindow(SW_SHOWNA);
		
		for (nIndex_Row = m_nRows - 2; nIndex_Row >= nRow; nIndex_Row--)
		{
			CWnd* pPane = GetPane(nIndex_Row, nIndex_Col);
			ASSERT(pPane != NULL);
			pPane->SetDlgCtrlID(IdFromRowCol(nIndex_Row + 1, nIndex_Col));
		}
		
		pPaneShow->SetDlgCtrlID(IdFromRowCol(nRow, nIndex_Col));
	}
	
	// new panes have been created -- recalculate layout
	for (nIndex_Row = nRow + 1; nIndex_Row < m_nRows; nIndex_Row++)
	{
		m_pRowInfo[nIndex_Row].nIdealSize = m_pRowInfo[nIndex_Row - 1].nCurSize;
	}
	
	m_pRowInfo[nRow].nIdealSize = cxNew;
	RecalcLayout();
}

void CDrSplitterCtrl::OnSize(UINT nType, int cx, int cy)
{
	int nCount, nLength;
	int nTotalIdealSize = 0, nTotalCurSize = 0;
	
	if( m_nResizeRow != -1 )
	{
		m_bFirstSizing = true;
		nLength = m_nRows;
		
		for(nCount = 0; nCount < nLength; nCount++ )
		{
			nTotalIdealSize += m_pRowInfo[nCount].nIdealSize;
			nTotalCurSize   += m_pRowInfo[nCount].nCurSize;
		}
		
		if( nTotalIdealSize != 0 && nTotalCurSize != 0  ) 
		{
			CRect rcClient;
			GetClientRect(rcClient);
			
			nTotalIdealSize = 0;
			
			for(nCount = 0; nCount < nLength; nCount++ )
			{
				if( nCount != m_nResizeRow )
				{
					m_pRowInfo[nCount].nIdealSize = m_pRowInfo[nCount].nCurSize + 11;
					nTotalIdealSize += m_pRowInfo[nCount].nIdealSize;
					break;
				} 
				else 
				{
					m_pRowInfo[nCount].nIdealSize = m_pRowInfo[nCount].nCurSize;
					nTotalIdealSize += m_pRowInfo[nCount].nIdealSize;
				}
			}
			
			for(nCount++ ;nCount < nLength; nCount++ ) 
			{
				m_pRowInfo[nCount].nIdealSize = m_pRowInfo[nCount].nCurSize;
				nTotalIdealSize += m_pRowInfo[nCount].nIdealSize;
			}
			
			nTotalIdealSize -= m_pRowInfo[m_nResizeRow].nIdealSize;
			
			if( cy <= nTotalIdealSize ) 
			{
				m_pRowInfo[m_nResizeRow].nIdealSize = 0;
			} 
			else
			{
				m_pRowInfo[m_nResizeRow].nIdealSize = cy - nTotalIdealSize;
			}
		}
	}
	
	CSplitterWnd::OnSize(nType, cx, cy);
}

void CDrSplitterCtrl::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	CRect rcSplitter = rect;	
	
	if (pDC == NULL)
	{
		RedrawWindow(rcSplitter, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
		return;
	}
	ASSERT_VALID(pDC);

	switch (nType)
	{
	case splitBorder:
		rcSplitter.DeflateRect(1, 1);
		return;
	case splitBox:
		pDC->Draw3dRect(rect, GetSysColor(COLOR_3DFACE), GetSysColor(COLOR_WINDOWFRAME));
		break;
	}
	
	COLORREF clr = m_clrSplitBar;
	pDC->FillSolidRect(rcSplitter, clr);
}

BOOL CDrSplitterCtrl::OnEraseBkgnd(CDC* pDC) 
{
	CRect rectClient;
	GetClientRect(rectClient);
	rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
	rectClient.bottom = rectClient.top + m_upBorder;
	
	pDC->FillSolidRect(rectClient, m_clrSplitBar);
	
	return CSplitterWnd::OnEraseBkgnd(pDC);
}
