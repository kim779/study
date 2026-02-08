// SiseWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
#include "SiseWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LPCSTR head_text[] = { "대", "거", "시", "고", "저", "미" };

CSiseWnd::CSiseWnd(CWnd *pParent)
: CGridCtrl(), CAxisExt(pParent)
{
}

BOOL CSiseWnd::Create(const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle , DWORD dwExStyle)
{
	BOOL bRet = CGridCtrl::Create(rect, parent, nID, dwStyle, dwExStyle);
	SetFont(m_pFont, FALSE);
	SetSiseGrid();
	return bRet;
}

CSiseWnd::~CSiseWnd()
{
}


BEGIN_MESSAGE_MAP(CSiseWnd, CGridCtrl)
	//{{AFX_MSG_MAP(CSiseWnd)
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CSiseWnd::OnSize(UINT nType, int cx, int cy) 
{
	CGridCtrl::OnSize(nType, cx, cy);
	EnableScrollBars(SB_BOTH, FALSE);

	if (nType==SIZE_RESTORED)
	{
		CRect rc;
		GetClientRect(&rc);
		int width = rc.Width();
		int height = rc.Height();

		if (width && GetColumnCount()>0)
		{
			int x, dx = width  / GetColumnCount();
			int tmpX;
			for(x=0; x<(GetColumnCount()-1); ++x)
			{
				if(x%2 == 0)
				{
					tmpX = dx / 2;
				}
				else
				{
					tmpX = dx + tmpX;
				}
				m_arColWidths[x] = tmpX;
				width -= tmpX;
			}
			m_arColWidths[x] = width+1;
		}
		
		if (height && GetRowCount()>0)
		{
			int y, dy = height / GetRowCount();
			for(y=0; y<(GetRowCount()-1); ++y)
			{
				m_arRowHeights[y] = dy;
				height -= dy;
			}
			m_arRowHeights[y] = height+1;
		}
	}
}

void CSiseWnd::OnNcPaint() 
{
	CWnd::OnNcPaint();
}
//LPCSTR head_text[] = { "대 비", "거래량", "시 가", "고 가", "저 가", "미결제" };
void CSiseWnd::SetData(SiseData *psd)
{
	CGridCell *pc;

	pc = (CGridCell*)GetCell(0, 1);

	if (pc->GetData() != psd->diff)
	{
		COLORREF col = GetColor(psd->gjga, psd->curr);

		// 대비
		pc = (CGridCell*)GetCell(0, 1);
		pc->SetTextClr( col );
		pc->SetText( DiffStr(psd->udgb, psd->diff) );
		//pc->SetData(psd->diff);
		RedrawCell(0, 1);

		// 시가변경
		pc = (CGridCell *)GetCell(1, 1);
		if (pc->GetData() != psd->siga)
		{
			pc->SetTextClr( GetColor(psd->gjga, psd->siga) );
			pc->SetText( Int2Str(psd->siga) );
			//pc->SetData(psd->siga);
			RedrawCell(1, 1);
		}
		
		// 고가변경
		pc = (CGridCell *)GetCell(1, 3);
		if (pc->GetData() != psd->koga)
		{
			pc = (CGridCell *)GetCell(1, 3);
			pc->SetTextClr( GetColor(psd->gjga, psd->koga) );
			pc->SetText( Int2Str(psd->koga) );
			//pc->SetData(psd->koga);
			RedrawCell(1, 3);
		}
		
		// 저가변경
		pc = (CGridCell *)GetCell(2, 1);
		if (pc->GetData() != psd->jega)
		{
			pc = (CGridCell *)GetCell(2, 1);
			pc->SetTextClr( GetColor(psd->gjga, psd->jega) );
			pc->SetText( Int2Str(psd->jega) );
			//pc->SetData(psd->jega);
			RedrawCell(2, 1);
		}
	}

	// 거래량
	pc = (CGridCell*)GetCell(0, 3);
	if (pc->GetData() != psd->gvol)
	{
		pc->SetText( Int2CommaStr(psd->gvol) );
		pc->SetData(psd->gvol);
		RedrawCell(0, 3);
	}

	// 미결제약정 변동
	pc = (CGridCell *)GetCell(2, 3);
	if (pc->GetData() != psd->mgjv)
	{
		pc->SetText( Int2CommaStr(psd->mgjv) );
		pc->SetData(psd->mgjv);
		RedrawCell(2, 3);
	}
}

void CSiseWnd::SetRealData( RealData *rp )
{
	if (rp->find(34)!=rp->end())	// 체결시간이 있으면~
	{
		SiseData sd;
		sd.curr = (int)(fabs(Str2Double( rp->find(23)->second ) * 100.0) + DOUBLE_PREFIX);
		sd.rate = (int)(fabs(Str2Double( rp->find(33)->second ) * 100.0) + DOUBLE_PREFIX);
		sd.gvol = Str2Int( rp->find(27)->second );
		sd.siga = (int)(fabs(Str2Double( rp->find(29)->second ) * 100.0) + DOUBLE_PREFIX);
		sd.koga = (int)(fabs(Str2Double( rp->find(30)->second ) * 100.0) + DOUBLE_PREFIX);
		sd.jega = (int)(fabs(Str2Double( rp->find(31)->second ) * 100.0) + DOUBLE_PREFIX);
		sd.mgjv = Str2Int( rp->find(201)->second );

		CString &diff = rp->find(24)->second.ToCStr();
		sd.udgb = diff.GetAt(0);
		sd.diff = (int)(Str2Double( diff.Mid(1) ) * 100.0);
		sd.gjga = GetGjga(sd.curr, sd.diff, sd.udgb);

		SetData(&sd);
	}
}

void CSiseWnd::SetRealData( DWORD* data )
{
	if (data[34])	// 체결시간이 있으면~
	{
		//CString s;
		SiseData sd;

		CString strCurr;
		strCurr.Format("%s",(char*)data[23]);
		sd.curr = (int)(fabs(atof( strCurr ) * 100.0) + DOUBLE_PREFIX);

		CString strRate;
		strRate.Format("%s",(char*)data[33]);
		sd.rate = (int)(fabs(atof( strRate ) * 100.0) + DOUBLE_PREFIX);

		CString strGvol;
		strGvol.Format("%s",(char*)data[27]);
		sd.gvol = atoi( strGvol );

		CString strSiga;
		strSiga.Format("%s",(char*)data[29]);
		sd.siga = (int)(fabs(atof( strSiga ) * 100.0) + DOUBLE_PREFIX);

		CString strKoga;
		strKoga.Format("%s",(char*)data[30]);
		sd.koga = (int)(fabs(atof( strKoga ) * 100.0) + DOUBLE_PREFIX);

		CString strJega;
		strJega.Format("%s",(char*)data[31]);
		sd.jega = (int)(fabs(atof( strJega ) * 100.0) + DOUBLE_PREFIX);

		CString strMgjv;
		strMgjv.Format("%s",(char*)data[201]);
		sd.mgjv = atoi( strMgjv );
		
		CString diff = (char*)data[24];
		sd.udgb = diff.GetAt(0);
		sd.diff = (int)(atof( diff.Mid(1) ) * 100.0);
		sd.gjga = GetGjga(sd.curr, sd.diff, sd.udgb);
		
		SetData(&sd);
	}
}

void CSiseWnd::Clear()
{
	if (GetColumnCount()==8 && GetRowCount()==2)
	{
		GetCell(0, 1)->SetText("");
		GetCell(0, 3)->SetText("");
		GetCell(1, 1)->SetText("");
		GetCell(1, 3)->SetText("");
		GetCell(2, 1)->SetText("");
		GetCell(2, 3)->SetText("");
	}
}

void CSiseWnd::ChangeTheme()
{
	for(int y=0; y<GetRowCount(); ++y)
	{
		for(int x=0; x<GetColumnCount(); ++x)
		{
			if (x%2==0)
			{
				SetItemBkColour(y, x, GetIndexColor(COLOR_GRIDHEAD));
				SetItemFgColour(y, x, GetIndexColor(COLOR_GRIDHEADTXT));
			}
		}
	}
	Invalidate(TRUE);
}

void CSiseWnd::SetSiseGrid()
{
	SetNoScrollBar(TRUE);
	SetColumnResize(FALSE);
	EnableSelection(FALSE);
	SetDoubleBuffering(TRUE);
	SetEditable(FALSE);
	SetFrameFocusCell(FALSE);
	
	SetGridBorderColor(RGB(0x6B,0x6B,0x6B));
	SetGridLineColor(RGB(0x6B,0x6B,0x6B));
	SetDefCellMargin(1);
	SetColumnCount(4);
	SetRowCount(3);
	SetFixedColumnCount(0);
	SetFixedRowCount(0);
	
	int hidx = 0;
	for(int y=0; y<GetRowCount(); ++y)
	{
		for(int x=0; x<GetColumnCount(); ++x)
		{
			GV_ITEM item;
			item.row = y;
			item.col = x;
			if (x%2==0) {			// 헤더 세팅	
				item.crBkClr = GetIndexColor(COLOR_GRIDHEAD);
				item.crFgClr = GetIndexColor(COLOR_GRIDHEADTXT);	
				item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_BKCLR|GVIF_FGCLR;
				item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
				item.strText = head_text[hidx++];
			}
			else
			{
				item.lParam = 0;
				item.nMargin = 5;
				item.crFgClr = GetIndexColor(COLOR_TEXT);
				item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_FGCLR|GVIF_MARGIN;
				item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
				item.strText = "";
			}
			SetItem(&item);
		}
	}
}
