// CodeReg.cpp : implementation file
//

#include "stdafx.h"
#include "CodeReg.h"
#include "Tool.h"
#include "UserDefine.h"
#include "Resource.h"
#include "../../h/AxisVar.h"
#include "../../h/axisfire.h"
#include "../../control/fx_grid/memdc.h"
#include "../../control/fx_misc/fxImgButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_CTRL_HEAD	0x01
#define IDC_CTL_TAB1	0x02
#define CLR_TAB_FORE		70
#define CLR_TAB_SELFORE		71
#define COLOR_PLUSTXT		94
#define COLOR_MINUSTXT		95
#define COLOR_FOCUS			78

#define CELL_HEIGHT	19
#define	SCROLL_WIDTH	16
/////////////////////////////////////////////////////////////////////////////
// CCodeReg

CCodeReg::CCodeReg(class CTool* pTool, bool bMenu)
{
	m_pTool		= pTool;
	m_bPopMenu	= bMenu;

	m_bLBDown	= false;
	m_iRow		= 0;
	m_pBtnHead	= NULL;	
	m_pTab = NULL;
	m_bChegUpdate = FALSE;

	m_ppenLine	= NULL;
	m_pfont		= NULL;
	m_clrBG		= RGB(255, 255, 255);
	m_clrDT		= RGB(255, 255, 255);
	m_iCellHeight = CELL_HEIGHT;
	m_iCellTop = CTRL_HEIGHT + GAP;
	m_pChegGrid = NULL;
}

CCodeReg::~CCodeReg()
{
	if (m_pBtnHead)
	{
		::DestroyWindow(m_pBtnHead->m_hWnd);
		m_pBtnHead = NULL;
	}	

	if (m_pTab)	     
	{ 
		m_pTab->DestroyWindow(); 
	}

	if (m_pChegGrid) 
	{ 
		m_pChegGrid->DestroyWindow(); 
	}
}


BEGIN_MESSAGE_MAP(CCodeReg, CWnd)
	//{{AFX_MSG_MAP(CCodeReg)
	ON_BN_CLICKED(ID_CTRL_HEAD, OnButtonHead)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_DELETE, OnMenuDelete)
	ON_COMMAND(ID_MENU_DELETEALL, OnMenuDeleteall)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CTL_TAB1, OnTabChange)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCodeReg message handlers

int CCodeReg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	Init();
	return 0;
}

void CCodeReg::Init()
{
	m_clrBG = m_pTool->GetIndexColor(CONTENTS_BACK_INDEX);
	m_clrDT	= m_pTool->GetIndexColor(COLOR_DATA);

	m_ppenLine = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	m_pfont = m_pTool->RequestFont("굴림체", 9, FALSE, 0);

	CRect rc, cRc;
	GetClientRect(&rc);

	m_iRow = (rc.Height() - CTRL_HEIGHT*2) / CELL_HEIGHT + 1;

	cRc.SetRect(0, 0, rc.right*2, CTRL_HEIGHT);
	
	m_pTab = std::make_unique<CfxTab>();
	m_pTab->Create(cRc, this, IDC_CTL_TAB1);
	m_pTab->InsertItem(0, "조회종목");
	m_pTab->InsertItem(1, "시간대별");

	m_pTab->LoadTabImage(m_pTool->Variant(homeCC, ""), false);
	m_pTab->SetStyle(0x00000001, false);
	m_pTab->SetBorderColor(m_pTool->GetIndexColor(TAB_BORDER_INDEX));
	m_pTab->SetColor(m_pTool->GetIndexColor(CLR_TAB_FORE), m_pTool->GetIndexColor(CONTENTS_BACK_INDEX), 
		m_pTool->GetIndexColor(CLR_TAB_SELFORE), m_pTool->GetIndexColor(CONTENTS_BACK_INDEX), m_pTool->GetIndexColor(CONTENTS_BACK_INDEX));
	m_pTab->SetFont(m_pfont);
	
	cRc.SetRect(0, CTRL_HEIGHT, rc.right, rc.bottom);

	m_pChegGrid = std::make_unique <CGridCtrl>();
	m_pChegGrid->Create(cRc, this, IDC_GRID_CHEG);
	m_pChegGrid->ShowWindow(SW_SHOW);
	m_pChegGrid->SetFont(m_pfont);
	SetChegGrid();

	cRc.SetRect(0, CTRL_HEIGHT, rc.right, CTRL_HEIGHT*2);

	m_pBtnHead = std::make_unique <CfxImgButton>(m_pfont);
	m_pBtnHead->Create("\n종목 추가", cRc, this, ID_CTRL_HEAD, FALSE, TRUE);
	m_pBtnHead->SetWindowText("종목 추가");
	m_pBtnHead->SetTextColor(m_pTool->GetIndexColor(69));

	CString strPath;
	strPath.Format("%s\\%s\\", m_pTool->GetHome(), IMAGEDIR);
	HBITMAP hbmp	= m_pTool->RequestBitmap(strPath + "11btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn = m_pTool->RequestBitmap(strPath + "11btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = m_pTool->RequestBitmap(strPath + "11btn_en.bmp")->operator HBITMAP();
	m_pBtnHead->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);

	LoadCode();
}

void CCodeReg::ClearChegGrid()
{
	SetChegGrid();
}


const char* _code_head[] = { "시간", "체결가", "수량" };
//	int  width[] = { 38, 54, 38 };  //20210107
const int  _code_width[] = { 38, 48, 44 };

void CCodeReg::SetChegGrid()
{
	CGridCellBase *pc;
	m_pChegGrid->SetEditable(FALSE);
	m_pChegGrid->EnableSelection(FALSE);
	m_pChegGrid->SetFrameFocusCell(FALSE);
	m_pChegGrid->SetColumnResize(TRUE);  //20210107
	m_pChegGrid->SetRowResize(FALSE);
	m_pChegGrid->EnableTitleTips(FALSE);
	m_pChegGrid->SetDefCellHeight(16);
	
	m_pChegGrid->SetRowCount(1);
	m_pChegGrid->SetColumnCount(3);
	
	pc = m_pChegGrid->GetDefaultCell(FALSE, FALSE);
	if (pc)	
	{
		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_RIGHT );
//		pc->SetFormat( DT_VCENTER|DT_SINGLELINE|DT_LEFT );
		pc->SetMargin(5);
	}
	
	pc = m_pChegGrid->GetDefaultCell(TRUE, FALSE);
	if (pc) {
		
		pc->SetBackClr( m_pTool->GetIndexColor(COLOR_GRIDHEAD) );
		pc->SetTextClr( m_pTool->GetIndexColor(COLOR_GRIDHEADTXT) );
	}
	m_pChegGrid->SetBkColor(RGB(255, 255, 255));
	m_pChegGrid->SetRowCount(MAX_FC);
	m_pChegGrid->SetColumnCount(3);
	m_pChegGrid->SetFixedRowCount(1);
	


	for(int n=0; n<3; ++n)
	{
		m_pChegGrid->SetItemText(0, n, _code_head[n]);
//		m_pChegGrid->SetItemFormat(0, n, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		m_pChegGrid->SetItemFormat(0, n, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		m_pChegGrid->SetColumnWidth(n, _code_width[n]);
	}

}


void CCodeReg::DrawSiseText(CDC* pDC, CString strData, CRect rc, UINT nFormat)
{
	if (strData.IsEmpty())
		return;
	
	nFormat = DT_SINGLELINE | DT_VCENTER | nFormat;

	CFont* pfontOld = pDC->SelectObject(m_pfont);


	const COLORREF clrTextOld = pDC->SetTextColor(RGB(0, 0, 0));
	pDC->DrawText(strData, rc, nFormat);

	pDC->SelectObject(pfontOld);
	pDC->SetTextColor(clrTextOld);
}

void CCodeReg::OnButtonHead()
{
	CRect rect;
	GetDlgItem(ID_CTRL_HEAD)->GetWindowRect(rect);
	CString strCODE = m_pTool->GetJCodeByDlg(kkelwCODE, NULL, rect.left, rect.bottom + 2);
	

	const int iPos = strCODE.Find('\t');
	if (iPos > 0)
	{
		AddJCod(strCODE.Left(iPos), strCODE.Mid(iPos + 1));
	}

//	return 0;
}

void CCodeReg::AddJCod(CString strJCode, CString strJName)
{
	strJCode.TrimLeft();
	strJCode.TrimRight();
	strJName.TrimLeft();
	strJName.TrimRight();

	if (strJCode.IsEmpty() || strJName.IsEmpty())
		return;

	struct _code Code;
	for (int ii = 0; ii < m_arCode.GetSize(); ii++)
	{
		Code = m_arCode.GetAt(ii);
		if (Code.code == strJCode)
		{
			m_arCode.RemoveAt(ii);
			break;
		}
	}
	
	Code.code = strJCode;
	Code.name = strJName;
	m_arCode.InsertAt(0, Code);

	if (m_arCode.GetSize() > m_iRow)
		m_arCode.RemoveAt(m_arCode.GetSize() - 1);

	Invalidate();
	CString strData;
	strData.Format("1301\t%s", strJCode);
	m_pTool->SetHistory(strData);	//2012.11.12 종목히스토리 추가
	m_pTool->SetTrigger(strData);
	
// 	SaveCode();
}

void CCodeReg::LoadCode()
{
	CString path = "", strData ="";

	strData = m_pTool->GetHistory("1301");
	m_arCode.RemoveAll();

	path.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);

	struct _code code;
	CString	Str;
	int	index=0;
	for (;;)
	{
		index = strData.Find("\t");
		if (index <= 0)
			break;
		
		Str = strData.Left(index);
		strData = strData.Mid(index + 1);
		
		index = Str.Find(" ");
		if (index > 0)
		{
			code.code = Str.Left(index);
			code.name = Str.Mid(index + 1);
			
			if (code.code[0]=='9') continue;
			if (code.code.GetLength()!=6) continue;
			
			m_arCode.Add(code);
		}
	}

	//load tab select
	const int iSel = GetPrivateProfileInt("TAB1", "SELECT", 0, (LPCSTR)path);
	m_pTab->SetCurSel(iSel);

	NMHDR hdr;
	hdr.idFrom = IDC_CTL_TAB1;
	OnTabChange(&hdr, NULL);
}

void CCodeReg::SaveCode()
{

}

void CCodeReg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBDown = true;
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CCodeReg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bLBDown)
	{
		CWnd::OnLButtonUp(nFlags, point);
		return;
	}
	
	CRect cRc, rc;
	CString strParam;
	GetClientRect(&rc);
	
	const int cx = 0, cy = rc.top + CTRL_HEIGHT + CTRL_HEIGHT;

	cRc.SetRect(cx, cy, rc.right, cy + m_iCellHeight);
	for (int ii = 0; ii < m_iRow; ii++)
	{
		if (cRc.PtInRect(point))
		{
			if (ii < m_arCode.GetSize())
			{
				struct _code code = m_arCode.GetAt(ii);
				//m_pMapWnd->SetCode(code.code);
				SetCode(code.code);
				
				strParam.Format("1301\t%s", code.code);
				
				m_pTool->SetHistory(strParam);
				m_pTool->Trigger(strParam);
			}
			break;
		}

		cRc.OffsetRect(0, m_iCellHeight - 1);
	}
	m_bLBDown = false;
	
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CCodeReg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CCodeReg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect cRc, rc;
	GetClientRect(&rc);

	const int bkMode = dc.SetBkMode(TRANSPARENT);

	dc.FillSolidRect(rc, m_clrBG);			
	CPen *pOldPen = dc.SelectObject(m_ppenLine);
	dc.SelectStockObject(NULL_BRUSH);

	const int cx = 0, cy = m_iCellTop + CTRL_HEIGHT;

	cRc.SetRect(cx, cy, rc.right, cy + m_iCellHeight);
	for (int ii = 0; ii < m_iRow; ii++)
	{
		if (ii == m_iRow)
			cRc.bottom = rc.bottom;

		dc.FillSolidRect(&cRc, m_clrDT);
		dc.Rectangle(&cRc);

		if (ii < m_arCode.GetSize())
		{
			struct _code code = m_arCode.GetAt(ii);
			CString name = code.name;
			name.Replace("&", "&&");
			DrawSiseText(&dc, name, cRc, DT_CENTER);
		}
		cRc.OffsetRect(0, m_iCellHeight - 1);
	}

	dc.SelectObject(pOldPen);
	dc.SetBkMode(bkMode);
}

void CCodeReg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if (m_bPopMenu)
	{
		CRect rc;
		GetClientRect(&rc);
		rc.top = CTRL_HEIGHT;
		
		m_ptPopMenu= point;
		ScreenToClient(&m_ptPopMenu);

		if (rc.PtInRect(m_ptPopMenu))
		{
			CMenu muTemp, *pContextMenu;
			
			muTemp.LoadMenu(IDR_MENUPOPUP);
			pContextMenu = muTemp.GetSubMenu(0);
			pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		}
	}
}

void CCodeReg::OnMenuDelete() 
{
	CRect cRc, rc;
	GetClientRect(&rc);

	const int cx = 0, cy = rc.top + CTRL_HEIGHT + CTRL_HEIGHT;
	cRc.SetRect(cx, cy, rc.right, cy + m_iCellHeight);

	CString strTemp;
	for (int ii = 0; ii < m_iRow; ii++)
	{
 		//strTemp.Format("[KSJ] cRc[%d, %d, %d, %d], m_ptPopMenu[%d, %d], ii[%d, %s], PtInRect[%s]", cRc.left, cRc.top, cRc.right, cRc.bottom, m_ptPopMenu.x, m_ptPopMenu.y, ii, m_arCode.GetAt(ii).name, cRc.PtInRect(m_ptPopMenu) ? "TRUE":"FALSE");
     	//OutputDebugString(strTemp);

		if (cRc.PtInRect(m_ptPopMenu))
		{
			if (ii < m_arCode.GetSize())
			{
				CString strData;
				strData.Format("1301\t%s", m_arCode.GetAt(ii).code);
				m_pTool->DelHistory(strData);	//2012.11.12 종목히스토리 삭제
				m_arCode.RemoveAt(ii);
				Invalidate();
			}
			break;
		}

		cRc.OffsetRect(0, m_iCellHeight - 1);
	}

//	SaveCode();
	Invalidate();
}

void CCodeReg::OnMenuDeleteall() 
{
	m_arCode.RemoveAll();
	Invalidate();
	m_pTool->DelHistory("1301\t");
// 	SaveCode();
	Invalidate();
}

void CCodeReg::OnDestroy() 
{
	CWnd::OnDestroy();
	
//	SaveCode();
//	m_arCode.RemoveAll();
	// TODO: Add your message handler code here
}

void CCodeReg::SetCode(CString strCode)
{
	GetParent()->GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_SET_CODE, NULL), (LPARAM)strCode.operator LPCTSTR());
}

void CCodeReg::Palette()
{
	m_clrBG = m_pTool->GetIndexColor(CONTENTS_BACK_INDEX);
	m_clrDT	= m_pTool->GetIndexColor(COLOR_DATA);

	m_ppenLine = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);

	Invalidate();
}


void CCodeReg::ChangeFont(double* lRate, CFont* pfont, CFont* boldFont)
{
	m_pfont = pfont;

	
	CRect wrc, rc;
	GetClientRect(&rc);
	m_iRow = (rc.Height() - CTRL_HEIGHT) / CELL_HEIGHT + 1;
//	CString ss;
//	ss.Format("%d",  m_iRow);
//AfxMessageBox(ss);
	int iw{}, ih{}, iright{};
	const int ileft = rc.left + GAP;
	const int itop = rc.top + GAP;
	

	m_pBtnHead->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw);
	ih = ih + (int)(lRate[1] * (float)ih);

	iright = rc.left+iw-GAP;
	wrc.SetRect(rc.left, itop, iright, itop+ih);
	m_pBtnHead->SetFont(m_pfont);
	m_pBtnHead->MoveWindow(wrc); 


	m_iCellTop = wrc.bottom + GAP;
	m_iCellHeight = m_iCellHeight + (int)(lRate[1] * (float)m_iCellHeight);
	m_iRow = (rc.Height() - m_iCellHeight) / m_iCellHeight + 1;
	
}

void CCodeReg::OnTabChange( NMHDR *pHeader, LRESULT *pResult )
{
	CString key, section = "CODEREG", data = "", path = "";
	path.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);
	
	if (pHeader->idFrom==IDC_CTL_TAB1 && m_pTab)
	{
		int nCmd1 = SW_SHOW, nCmd2 = SW_SHOW;
		if (m_pTab->GetCurSel()==0)
		{
			nCmd1 = SW_SHOW;
			nCmd2 = SW_HIDE;
			m_bChegUpdate = FALSE;
		}
		else if (m_pTab->GetCurSel()==1) 
		{
			nCmd1 = SW_HIDE;
			nCmd2 = SW_SHOW;
			m_bChegUpdate = TRUE;
		}
		else 
		{
			return;
		}
		
		{
			CString strVal;
			strVal.Format("%d", m_pTab->GetCurSel());
			WritePrivateProfileString("TAB1", "SELECT", (LPCSTR)strVal, (LPCSTR)path);
		}
		
		m_pBtnHead->ShowWindow(nCmd1);
		m_pChegGrid->ShowWindow(nCmd2);
	}
}

void CCodeReg::SetRealData(CString time, CString curr, CString cvol)
{
	CString pTime;
	if (m_pChegGrid)
	{
		m_pChegGrid->InsertRow(NULL, 1);
		m_pChegGrid->DeleteRow(MAX_FC);	//2012.11.19 KSJ 마지막라인은 삭제함.
		
		m_pChegGrid->SetItemFormat(1, 0, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		pTime.Format("%.2s:%.2s", time.Mid(0,2), time.Mid(2,2));
		m_pChegGrid->SetItemText(1, 0, pTime);

		curr.TrimLeft();
		curr.TrimRight();
		cvol.TrimLeft();
		cvol.TrimRight();
		
// 		CString ddd;
// 		ddd.Format("curr:%s Length:%d", curr.GetLength());
// 		MessageBox(ddd);

		if(curr.Left(1) == "+" || curr.Left(1) == "-" || curr.Left(1) == " ")
		{
			if(curr.GetLength() > 4 && curr.GetLength() < 8)
				curr = m_pTool->AddComma(curr);
		}
		else
		{
			if(curr.GetLength() < 7)
				curr = m_pTool->AddComma(curr);
		}

		
//		if (curr.GetLength() > 7)	curr.Remove(',');	//2012.07.12 KSJ 100만원이상일때 콤마 없애기
		

		switch(curr.GetAt(0))
		{
		case '+': 
			m_pChegGrid->SetItemFgColour(1, 1, m_pTool->GetIndexColor(COLOR_PLUSTXT));
			m_pChegGrid->SetItemText(1, 1, curr.Mid(1));
			break;
		case '-': 
			m_pChegGrid->SetItemFgColour(1, 1, m_pTool->GetIndexColor(COLOR_MINUSTXT)); 
			m_pChegGrid->SetItemText(1, 1, curr.Mid(1));
			break;
		default : 
			m_pChegGrid->SetItemFgColour(1, 1, RGB(0, 0, 0));
			m_pChegGrid->SetItemText(1, 1, curr.Mid(0));
			break;
		}
		
		
		cvol = m_pTool->AddComma(cvol);
		switch(cvol.GetAt(0))
		{
			
		case '+':
			m_pChegGrid->SetItemFgColour(1, 2, m_pTool->GetIndexColor(COLOR_PLUSTXT));
			m_pChegGrid->SetItemBkColour(1, 2, RGB(255, 255, 255));
			m_pChegGrid->SetItemText(1, 2, cvol.Mid(1));
			break;
		case '-':
			m_pChegGrid->SetItemFgColour(1, 2, m_pTool->GetIndexColor(COLOR_MINUSTXT));
			m_pChegGrid->SetItemBkColour(1, 2, RGB(255, 255, 255));
			m_pChegGrid->SetItemText(1, 2, cvol.Mid(1));
			break;
		case '1': 
			m_pChegGrid->SetItemFgColour(1, 2, RGB(255, 255, 255));
			m_pChegGrid->SetItemBkColour(1, 2, m_pTool->GetIndexColor(COLOR_PLUSTXT));
			m_pChegGrid->SetItemText(1, 2, cvol.Mid(1));
			break;
		case '4':
			m_pChegGrid->SetItemFgColour(1, 2, RGB(255, 255, 255));
			m_pChegGrid->SetItemBkColour(1, 2, m_pTool->GetIndexColor(COLOR_MINUSTXT));
			m_pChegGrid->SetItemText(1, 2, cvol.Mid(1));
			break;
		default:
			m_pChegGrid->SetItemText(1, 2, cvol.Mid(0));
		}
		
		if (m_bChegUpdate)
			m_pChegGrid->Refresh();
		
 		if (m_pChegGrid->GetRowCount()>200)
 			m_pChegGrid->SetRowCount(200);
	}
}


