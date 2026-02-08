// CCustomCheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "CustomCheckBox.h"
#include "resource.h"
#include "Customfuturesdlg.h"

#include "math.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomCheckBox

CCustomCheckBox::CCustomCheckBox()
{
	m_nInit   = 40;
	m_Key     = -1;
	m_BitmapNormal.LoadBitmap(IDB_BITMAP_NORMAL);
	m_BitmapClick.LoadBitmap(IDB_BITMAP_CLICK);
	m_pTool = std::make_unique<CTool>();
}

CCustomCheckBox::~CCustomCheckBox()
{		
	if (m_hCursor)
		::DestroyCursor(m_hCursor);
	m_hCursor = nullptr;
	
	DestroyWindow();
	m_BitmapNormal.DeleteObject();
	m_BitmapClick.DeleteObject();
}


BEGIN_MESSAGE_MAP(CCustomCheckBox, CWnd)
	//{{AFX_MSG_MAP(CCustomCheckBox)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomCheckBox message handlers

void CCustomCheckBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC	memDC(&dc);
	m_pTool->SetDC(&memDC);
	Draw();

	m_pTool->SetDC(nullptr);
}

void CCustomCheckBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CRect	rc(0,41,321,225), clientRc;
	GetClientRect(&clientRc);
	const int nVisibleRowCnt = clientRc.Height() / 20 - 1;

	const int	scrollPos = GetScrollPos32(SB_VERT);
	const int	lim = -(m_nCount - (nVisibleRowCnt + 1)) * 20;

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

BOOL CCustomCheckBox::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
	SCROLLINFO sinfo;

	sinfo.cbSize = sizeof(SCROLLINFO);
	sinfo.fMask  = SIF_POS;
	sinfo.nPos   = nPos;
	return SetScrollInfo(nBar, &sinfo, bRedraw);
}

int CCustomCheckBox::GetScrollPos32(int nBar, BOOL bGetTrackPos)
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

void CCustomCheckBox::Draw()
{
	CRect	rc;
	CStringArray aStr;
	const int	row1[5] = {0, 32, 64, 96, 128};
	const int	row2[5] = {192, 222, 254, 288, 320};

	GetClientRect(rc);
	m_pTool->FrameRect(rc, USER_BACKCOLOR2);

	DrawList(m_nInit);	

	// header
	rc.SetRect(0, 0, 320, 40);
	m_pTool->FrameRect(rc, USER_BACKCOLOR2);
	
	rc.SetRect(0, 0, 128, 20);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("妮可记", rc);
	m_pTool->DrawCell(rc);

	rc.SetRect(128, 0 , 192, 40);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("青荤啊拜", rc);
	m_pTool->DrawCell(rc);

	rc.SetRect(192, 0, 320, 20);
	m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
	m_pTool->WriteText("遣可记", rc);
	m_pTool->DrawCell(rc);

	const int nCount = m_Array.GetSize();

	for (int ii = 0; ii < nCount; ii++)
	{
		CString tmp = m_Array.GetAt(ii);

		rc.SetRect(row1[ii], 20, row1[ii+1], 40);
		m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
		m_pTool->WriteText(m_Array[ii], rc);
		m_pTool->DrawCell(rc);
	}
	
	for (int ii=0; ii < nCount; ii++)
	{
		rc.SetRect(row2[4- ii], 20, row2[3- ii], 40);
		m_pTool->FrameRect(rc, USER_BT_NORMALCOR);
		m_pTool->WriteText(m_Array[ii], rc);
		m_pTool->DrawCell(rc);
	}	
}
	
void CCustomCheckBox::DrawList(int row)
{
	CRect	rc;
	CString	temp;
	_DATA	data;
	const int	col[10] = {0, 32, 64, 96, 128, 192, 222, 254, 288, 320};
	
	// row index
	for (int jj = 0; jj < m_nCount; jj++)
	{	
		// col index
		for (int ii = 0; ii < 9; ii++)
		{
			rc.SetRect(col[ii], row, col[ii+1], row+20);
			m_pTool->FrameRect(rc, RGB(229,229,230));

			if (ii == 4)
			{
				m_Map.Lookup(jj*10+ii, data);
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
			{	// BOOL蔼 贸府... 
				m_Map.Lookup((jj*10+ii), data); 
				if (data.flag && !m_bEmpty)
				{
					m_BitmapNormal.Draw(m_pTool->GetDC(), rc);

					if (data.flag == 2)
					{	
						m_BitmapClick.Draw(m_pTool->GetDC(), rc);
						m_Key = jj*10+ii;
					}
				}
				m_pTool->DrawCell(rc);
			}
		}		
		
		row += 20;
	}

	ResetScrollBars();
}

void CCustomCheckBox::ResetScrollBars()
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
	sinfo.nMax   = m_nCount * 20; 
	SetScrollInfo(SB_VERT, &sinfo, TRUE);
}

void CCustomCheckBox::AddData(int row, int col, const _DATA& data)
{	
	m_Map.SetAt(row*10 + col, data);
}

_DATA CCustomCheckBox::GetData(int row, int col)
{
	_DATA data;

	m_Map.Lookup(row*10 + col, data);
	return data;
}

void CCustomCheckBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	rc;
	_DATA	data;
	int	val{};
	const int col[10] = { 0, 30, 62, 94, 130, 192, 224, 256, 290, 320 };

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
			for (int ii = 0; ii < 9; ii++)
			{
				if (jj*20< point.y - 40 + val && point.y - 40 + val < 20 + jj*20 
					&& col[ii] < point.x && point.x < col[ii+1])
				{	
					if (ii != 4)
					{
						rc.SetRect(col[ii], jj*20, col[ii+1], jj*20 + 20);
						m_Map.Lookup(jj*10+ii, data);
						if (data.flag)
							data.flag = 2;
					
						((CCustomFuturesDlg*)m_pWnd)->SetCode(data.code);
						((CCustomFuturesDlg*)m_pWnd)->SetName(data.name);
					
						m_sCode = data.code;

						((CCustomFuturesDlg*)m_pWnd)->SetButton();
						((CCustomFuturesDlg*)m_pWnd)->UpdateData(FALSE);
						AddData(jj, ii, data);
						Invalidate();		
					}
				}
			}
		}
	}

	if ((point.y > 40 && point.y < 319) && (0 < point.x && point.x< 130) || (point.x > 192 && point.x < 321))
	{
		if (!m_sCode.IsEmpty())
			((CCustomFuturesDlg*)m_pWnd)->EndDialog(1);		
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CCustomCheckBox::RemoveAll()
{
	m_Map.RemoveAll();
}

void CCustomCheckBox::SetArray()
{
	CArray<CString, CString> *pArray;

	pArray = ((CCustomFuturesDlg*)m_pWnd)->GetArray();
	m_Array.RemoveAll();

	for (int jj = 0 ; jj < pArray->GetSize() ; jj++)
		m_Array.Add(pArray->GetAt(jj).Right(2) + "岿");

	pArray = nullptr;
}

BOOL CCustomCheckBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
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

DWORD CCustomCheckBox::SetCheckCursor(int nCursorId, BOOL bRepaint)
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

void CCustomCheckBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if ((point.y > 40 && point.y < 319) && (0 < point.x && point.x< 130) || (point.x > 192 && point.x < 321))
	{
		OnLButtonDown(nFlags, point);
		if (!m_sCode.IsEmpty())
			((CCustomFuturesDlg*)m_pWnd)->EndDialog(1);
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CCustomCheckBox::ShowRow(int row)
{
	ResetScrollBars();
	CRect	rc(0,41,321,225);

	if (row < 5)
		row = 0;
	else if (row > m_nCount - 4)
		row += 4;
	else	row -= 4;

	SetInit(GetInit() - row*20); 
	SetScroll(GetScroll() + row*20);
	InvalidateRect(rc);
	SetScrollPos32(SB_VERT, GetScroll());
}

void CCustomCheckBox::DeleteTool()
{
	if (m_pTool)
		m_pTool->DeleteDC();
}
