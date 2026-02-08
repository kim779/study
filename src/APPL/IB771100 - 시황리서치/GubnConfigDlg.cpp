// GubnConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB771100.h"
#include "GubnConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGubnConfigDlg dialog


CGubnConfigDlg::CGubnConfigDlg(CFont* pFont, COLORREF color1, COLORREF color2, COLORREF color3, COLORREF color4, CWnd* pParent /*=NULL*/)
	: CDialog(CGubnConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGubnConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pFont = pFont;
	m_txtcolor = color4;
	m_backcolor = color1;
	m_listhdrcolor = color2;
	m_listlinecolor = color3;
}


void CGubnConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGubnConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGubnConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CGubnConfigDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGubnConfigDlg message handlers

BOOL CGubnConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	int iwindoww = 0;
	int iwindowh = 0;
	int isubcnt = 0;
	const int itop = 95;
	const int ibtnh = 25;
	UINT nID= 1000;
	const int ibtnw = 110;
	const int ileft = 20;
	const int	igap = 20;

	int ibtnidx = 0;
	CRect rect(ileft, itop, ibtnw+ileft, itop+ibtnh);
//	const int icheck = 0;
	for(int i=1; i<m_arNewsList.GetSize()-4; i++) // 전체(0) 을 빼고 공시(1) 부터, 마지막(조회수상위),마지막-1(스크랩) 빼고 
	{
		if(m_arNewsList.GetAt(i).ikey >= 100) continue;
		
		isubcnt = m_arNewsList.GetAt(i).ibulruCnt;
		nID = 1000 + (i*100); // 1000 번부터 1100, 1200,1300 .....
		
		if(isubcnt > 1)
		{
			m_pNewsButton[ibtnidx].Create ("전체" , WS_CHILD |WS_VISIBLE|BS_AUTOCHECKBOX |BS_CHECKBOX, rect, this, nID);
			rect.top += 25;
			rect.bottom = rect.top + 25;
			m_pNewsButton[ibtnidx].SetFont(m_pFont);
			m_pNewsButton[ibtnidx].SetCheck( m_arNewsList.GetAt(i).iAllCheck); 
			m_pNewsButton[ibtnidx++].ShowWindow(SW_SHOW);
		}

		nID += 1;

		for(int j=0; j<isubcnt-1; j++)
		{
			m_pNewsButton[ibtnidx].Create (m_arNewsList.GetAt(i).sBulruNameList[j] , WS_CHILD |WS_VISIBLE|BS_AUTOCHECKBOX |BS_CHECKBOX, rect, this, nID + j);
			rect.top += 25;
			rect.bottom = rect.top + 25;
			m_pNewsButton[ibtnidx].SetFont(m_pFont);
			m_pNewsButton[ibtnidx].SetCheck( m_arNewsList.GetAt(i).iCheck[j]); 

			m_pNewsButton[ibtnidx++].ShowWindow(SW_SHOW);
			

		}
		m_arTypeList.Add(m_arNewsList.GetAt(i).sGubnName ); 
		m_arLblRect.Add(rect); 
		if(iwindoww < rect.right ) iwindoww = rect.right;
		if(iwindowh < rect.bottom ) iwindowh = rect.bottom;

		rect.left = rect.right+igap;
		rect.right = rect.left + ibtnw;

		rect.top = itop;
		rect.bottom = itop+ibtnh;
		
	}

	SetWindowPos(NULL,0,0, iwindoww+igap, iwindowh+igap, SWP_NOMOVE);
	GetDlgItem(IDC_SETBUTTON)->MoveWindow(iwindoww-50, igap, 50, 25);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGubnConfigDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect,  m_backcolor);
//▶ 체크선택시 해당항목들만  실시간 자동수신됩니다.
//▶ 조회뉴스분류 클릭시 전체조회인경우 해당항목만 조회됩니다

	dc.SetTextColor(m_txtcolor);

	dc.SetBkMode(TRANSPARENT); 
	CBrush brush(0xFFFFFF/*GetSysColor(COLOR_BTNFACE)*/);
	CBrush* pOldBrush = dc.SelectObject(&brush); 
	CPen pen(PS_SOLID, 1, m_listlinecolor );
	CPen* pOldPen =dc.SelectObject(&pen); 
	CFont* pOldFont = dc.SelectObject(m_pFont); 
	dc.TextOut(5, 15, "▶ 체크선택시 해당항목들만  실시간 자동수신됩니다.");
	dc.TextOut(5, 38, "▶ 조회뉴스분류 클릭시 전체조회인경우 해당항목만 조회됩니다.");
//▶ 조회뉴스분류 클릭시 전체조회인경우 해당항목만 조회됩니다


	const int itop = 90;
	rect.top = 65;
	rect.DeflateRect(5, 0, 5, 10); 
	dc.Rectangle(rect); 
	CRect rhdr = rect;
	

	dc.MoveTo( rect.left, itop);    
	dc.LineTo( rect.right, itop);
	
	rhdr.bottom = itop;
	rhdr.DeflateRect(1,1,1,0); 
	dc.FillSolidRect(rhdr, m_listhdrcolor);

	CRect trect;
	const int iloop = m_arLblRect.GetSize();
	for(int i=0; i<iloop; i++)
	{
		trect = CRect( m_arLblRect.GetAt(i).left, rect.top, m_arLblRect.GetAt(i).right, itop);
		dc.DrawText(m_arTypeList.GetAt(i),   m_arTypeList.GetAt(i).GetLength(), trect, DT_SINGLELINE|DT_CENTER|DT_VCENTER); 

		if(iloop-1 > i)
		{
			dc.MoveTo( m_arLblRect.GetAt(i).right+12, rect.top);    
			dc.LineTo( m_arLblRect.GetAt(i).right+12, rect.bottom);
		}
	}

	dc.SelectObject(pOldFont); 
	DeleteObject(pOldFont);
	
	dc.SelectObject(pOldBrush); 
	DeleteObject(pOldBrush);

	dc.SelectObject(pOldPen); 
	DeleteObject(pOldPen);

	
	// Do not call CDialog::OnPaint() for painting messages
}

void CGubnConfigDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


//	CDialog::OnOK(); 
BOOL CGubnConfigDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	try
	{
		int nID = LOWORD(wParam);
		if(nID == IDCANCEL)
		{
			CDialog::OnCancel();  
			return TRUE;
		}
		
		if(nID == IDC_SETBUTTON)
		{
			CDialog::OnOK(); 
			return TRUE;
		}
		
		CButton* pButton = (CButton*)GetDlgItem(nID);
		if(!pButton) 
			return TRUE;
		
		const int igidx = (nID - 1000) / 100;
		if(m_arNewsList.GetSize() <=  igidx) 
			return TRUE;
		
		NewsList pList = m_arNewsList.GetAt(igidx);  
		const int ichk = pButton->GetCheck();  // 현재 체크된 버튼 상태 
		
		if( (nID % 100) == 0) // 현재 체크된 버튼이 전체 버튼 (첫번째버튼) 이라면 
		{
			//전체
			int idx = nID+1;
			
			while(1)
			{
				pButton = (CButton*)GetDlgItem(idx);
				if( !pButton) 
					break;
				
				pButton->SetCheck(ichk); 
				idx++;
				
				if( (idx % 100) == 0 ) 
					break;
			} // 나머지 버튼 모두 같은 값으로 셋팅 
			
			pList.iAllCheck =ichk; 
			
			for(int i=0; i<pList.ibulruCnt; i++)
				pList.iCheck[i] = ichk;
			
		}
		else
		{
			int isubidx = nID % 100;
			nID -= isubidx;
			
			
			isubidx -= 1;
			pList.iCheck[isubidx] = ichk;
			if(!ichk)
			{
				pButton = (CButton*)GetDlgItem(nID); //전체버튼
				if(pButton) pButton->SetCheck(ichk); 
				pList.iAllCheck = ichk;
			}
			else
			{
				int idx = nID+1;
				bool ball = true;
				while(1)
				{
					pButton = (CButton*)GetDlgItem(idx);
					if( !pButton) break;
					
					if( !pButton->GetCheck()) 
					{
						ball = false;
						break;
					}
					
					idx++;
					if( (idx % 100) == 0 ) break;
				}
				
				if(ball)
				{
					if( (pButton = (CButton*)GetDlgItem(nID)) )
					{
						pButton->SetCheck(ball); 
					} //전체버튼
				}
			}
		}
		
		m_arNewsList.SetAt(igidx,  pList);
	}
	catch (const CException* err)
	{
		const CException* a = err;
		//err		
	}

	return CDialog::OnCommand(wParam, lParam);
}


HBRUSH CGubnConfigDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC  )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_txtcolor);
		return CreateSolidBrush(0xFFFFFF);
	}

	
	if(nCtlColor == CTLCOLOR_BTN)
	{
		pDC->SetBkMode(TRANSPARENT);
		return CreateSolidBrush(0xFFFFFF);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
