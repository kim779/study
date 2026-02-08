// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB611000.h"
#include "MainWnd.h"
//#include "griditem.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SIZE_TABITEMWIDTH	80
#define SIZE_TABITEMHEIGHT	19


/////////////////////////////////////////////////////////////////////////////
// CMainWnd

// CMainWnd::CMainWnd()
// {
// 	EnableAutomation();
// }

CMainWnd::CMainWnd(CWnd *parent)
{
	m_pWizard = parent;
	
	m_bFirst = FALSE;
	m_pRemainGrid = NULL;	
	
	dlg = NULL;
	m_pFont = NULL;

	m_pClientRect = CRect(0, 0,0 ,0);
	m_arGridHdrList.RemoveAll();
	m_arGridToggleList.RemoveAll();
	m_arCONTROL.RemoveAll();	
	m_bgcolor = GetIndexColor(64);
	m_gridiem.RemoveAll();
	m_grouplist.RemoveAll();

	m_igridRowHeight = GRIDROWHEIGHT;
	m_iGuideHeight = GUIDE_HEIGHT;
	m_iGridHeadrHeight = GRIDHEADER_HEIGHT;
	m_iToolheight = TOOL_HEIGHT;
	
	m_gridhdrbgcolor = GetIndexColor(78);

	m_pContentRect = CRect(0,0,0,0);
	m_iOldFont = 9;
	m_pFont = GetAxFont();
	m_clrPallete = (COLORREF)42881280;
	m_init = TRUE;
	m_dlg = FALSE;

	//m_txtcolor = RGB(255, 0, 0);
	m_nMoveSave = 0;
	m_iSelectRow = -1;
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_BTNTAB, OnSelchangeBtntab)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_MESSAGE, OnManage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {5911976D-6644-43C5-8426-F72ABF9E084B}
static const IID IID_IMainWnd =
{ 0x5911976d, 0x6644, 0x43c5, { 0x84, 0x26, 0xf7, 0x2a, 0xbf, 0x9e, 0x8, 0x4b } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

LONG CMainWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
//	int tets= wParam;
	//int key = HIBYTE(LOWORD(wParam));

	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:		// Wizard request TR Header
		// return char*
		break;
	case DLL_TRIGGER:
			break;
	case DLL_OUB:		// Received Data
		// HIBYTE(LOWORD(WPARAM)) : key, HIWORD(WPARAM) : DATA size
		// HIBYTE(LOWORD(WPARAM)) : DATA size for MSB 8bits (control FORM OUB)
		const int key = HIBYTE(LOWORD(wParam));
		TRACE("DLL_OUB : %d", key);


		CString strTemp;
		strTemp.Format("KEY:%d", key);
//		testSaveFile3("OnMessage DLL_OUB", strTemp);

		switch (key) 
		{
		case FUNDSEARCH:
			if(m_dlg == TRUE)
			{
				dlg->SendMessage(WM_USER, wParam, lParam);
				//ParsingGroup((char*)lParam, HIWORD(wParam));
			}
			else
			{
				parsingData((char*)lParam, HIWORD(wParam));
				loadcfg();
				MainQueryExcelSise(m_lastpage);
			}
			
			break;
		case FUNDNEWGROUP:
			ParsingDataNGroup((char *)lParam, HIWORD(wParam));
			
			//dlg->SendMessage(WM_USER, wParam, lParam);
			break;
		case FUNDINIT:
			GridDataParsing((char *)lParam, HIWORD(wParam));
			break;
		case FUNDCREATEGROUP:
			dlg->SendMessage(WM_USER, wParam, lParam);
			ParsingGroup((char*)lParam, HIWORD(wParam));
			break;
		case FUNDLIST:
			dlg->SendMessage(WM_USER, wParam, lParam);
			break;
		case FUNDDELETEGROUP:
			dlg->SendMessage(WM_USER, wParam, lParam);
			ParsingGroup((char*)lParam, HIWORD(wParam));
			break;
		case FUNDBACKUP:
			dlg->SendMessage(WM_USER, wParam, lParam);
			break;
		case FUNDRESTORE:
			//dlg->SendMessage(WM_USER, wParam, lParam);
			ParsingRestore((char *)lParam, HIWORD(wParam));
			break;
		case FUNDUPDATEGROUP:
			dlg->SendMessage(WM_USER, wParam, lParam);
			ParsingGroup((char*)lParam, HIWORD(wParam));
			break;
		}
		
		break;	
	}
	return TRUE;
}

LONG CMainWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	const LONG	ret = 0;

	switch ((int)(wParam))
	{
		case SETGROUPLIST:
			dlg->SendMessage(WM_USER, wParam, lParam);
			break;
		case DLGEXIT:
			m_init = FALSE;
			m_dlg = FALSE;
			//m_grouplist.RemoveAll();
			m_lastpage = lParam;
			m_pRemainGrid->Clear();
			QueryExcelSise();	
			loadTabItem();
			m_btntab.UpdateWindow();
//			MainQueryExcelSise(m_lastpage);
			//dlg = NULL;
			break;
	}
	
	return ret;
}

void CMainWnd::Init()
{
	CString m_root = Variant(homeCC);
	CRect rc;
	GetClientRect(rc);
	rc.InflateRect(0, -44);
	Variant(titleCC, "[6110] 관심펀드");
	
	m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_CONFIG); 

	m_pExpandWnd = std::make_unique<CExpandWnd>(m_pWizard);
	m_pExpandWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_EXPAND);
	m_pExpandWnd->Init();


	QueryExcelSise();
	InitBtntab();
	
	InitGrid();

}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_root = Variant(homeCC);
	m_id = Variant(userCC);
	m_user = Variant(nameCC);
	// TODO: Add your specialized creation code here
	Init();

	return 0;
}

void CMainWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
		
	dc.SetBkMode(TRANSPARENT); 
	CRect rect;
	//if( m_pClientRect.Width() <= 0)
		GetClientRect(&m_pClientRect);

	rect = m_pClientRect;
	
	dc.FillSolidRect(rect, m_bgcolor); 
	TRACE("%d ", rect.right);

	rect.bottom = rect.top + m_iToolheight+1;

//	dc.SetTextColor(m_txtcolor);

	GetClientRect(rect);
	
	rect.left += 1;
	rect.right -= 1;
	rect.top = 5;
	rect.bottom = rect.top + 23;
	DrawRoundRect(&dc, rect, GetIndexColor(66), GetIndexColor(66));				// 버튼탭 RoundRect 그리는 부분

	GetClientRect(rect);
//	TRACE("%d %d %d %d\n", rect.left, rect.right, rect.top, rect.bottom);
	rect.left += 1;
	rect.right -= 1;
	rect.top += 34;
//	rect.bottom -= 30;
	rect.bottom -= 80;
	
	DrawRoundRect(&dc, rect, GetIndexColor(68), GetIndexColor(68));			// GRId RoundRect 그리는 부분

	rect.top = rect.bottom + 3;
	rect.bottom = rect.top + 47;
	m_pExpandWnd->MoveWindow(rect);

	GetClientRect(m_errorrect);
	m_errorrect.left += 1;
	m_errorrect.right -= 1;
	m_errorrect.top = m_errorrect.bottom - 23;
	m_errorrect.bottom -= 1;
	
	DrawRoundRect(&dc, m_errorrect, RGB(221, 221, 221), RGB(221, 221, 221));		// Error Msg RoundRect 그리는 부분
	
	m_errorrect.DeflateRect(3, 1, 1, 0);
	m_pFont = (CFont *)dc.SelectObject(GetAxFont("굴림체", 9, 0));
	dc.DrawText(m_strGuideMessage, m_errorrect, DT_SINGLELINE|DT_LEFT|DT_VCENTER);

	DrawGridHeader(&dc);  

	m_btTEXT.Invalidate();
}

void CMainWnd::DrawRoundRect(CDC *pDC,int nSx,int nSy,int nEx,int nEy,long Color1,long Color2)
{
	CPen Pen, * pOldPen{};
	CBrush Brush, * pOldBrush{};
	m_LineThick = 2;

	Pen.CreatePen(PS_SOLID, m_LineThick, Color1);
	Brush.CreateSolidBrush(Color2);
	
	pOldBrush = (CBrush*)pDC->SelectObject(&Brush);
	pOldPen = (CPen*)pDC->SelectObject(&Pen);
	
	pDC->RoundRect(nSx, nSy, nEx, nEy, 10, 10);
	
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	
	Brush.DeleteObject();
	Pen.DeleteObject();
}

void CMainWnd::DrawRoundRect(CDC *pDC,RECT Rect,long Color1,long Color2)
{
	DrawRoundRect(pDC,Rect.left,Rect.top,Rect.right,Rect.bottom,Color1,Color2);
}

void CMainWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	CRect rc, rc1, rc2; GetClientRect(rc);
	rc1= rc;
	rc2 = rc;
	roundRect = CRect(0, 0, 0, 0);

	//return;

	if (m_pRemainGrid)
	{
		
		rc.top += 81;	// 헤더 높이81
		rc.left += 4;
		rc.right -= 4;
//		rc.bottom -= 29;
		rc.bottom -= 79;
		// 		if (m_bViewChart)
		// 			rc.bottom -= 200;
		// 		
		m_pRemainGrid->MoveWindow(rc);
	}

	if(m_btTEXT)
	{
	//	rc1.left = rc1.right - 41; 
		rc1.left = rc1.right - 90; 
		rc1.top = 6;
		rc1.bottom = rc1.top + 20;
		rc1.right -= 4;
		m_btTEXT.MoveWindow(rc1);
	}

	if(m_btExcel)
 	{
 		rc1.right = rc1.left - 4;
 		rc1.left = rc1.right - 20; 
 		rc1.top = 6;
 		rc1.bottom = rc1.top + 20;
 	
 		m_btExcel.MoveWindow(rc1);		
 	}
	
	if(m_btntab)
	{
		rc2.left += 4; 
		rc2.top = 6;
		rc2.bottom = rc2.top + 20;
		rc2.right -= 42;

		m_btntab.MoveWindow(rc2);
		m_btntab.SetItemSize(CSize(SIZE_TABITEMWIDTH, SIZE_TABITEMHEIGHT));
		
		if(!m_btntab.IsWindowVisible())
			m_btntab.ShowWindow(SW_SHOW);
	}
	
	Invalidate();	
}

void CMainWnd::SetGuideMessage(CString sMessage)
{
	if( sMessage.Find("그룹계좌") > -1 && sMessage.Find("존재") > -1 )
		return;
	
	int iFind = 0;
	if( (iFind=sMessage.Find("&") ) > -1) sMessage.Insert(iFind, '&'); 
	
	m_strGuideMessage = sMessage;
	InvalidateRect(CRect(m_errorrect.left, m_errorrect.top , m_errorrect.right, m_errorrect.bottom) );
}

void CMainWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;

	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;

	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != 0) {
		memDC.SelectObject(BitMap);
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	}
// 	
 	DeleteObject(BitMap);
 	memDC.DeleteDC();
}


void CMainWnd::InitGrid()
{
	m_bFirst = FALSE;

	CRect	pRect, pRect1;
	GetClientRect(&pRect);

	int itopPanelH = 30;
	
	int ileft = HORI_GAP;
	const int iright = pRect.Width() - (HORI_GAP);
	itopPanelH += HORI_GAP;
	
	const int itop = VERT_GAP + VERT_GAP;
	pRect.DeflateRect(HORI_GAP-1, itop, 1, 2);
	CRect btnRect = pRect;
	btnRect.left = pRect.right - 100;
	btnRect.bottom = btnRect.top + 23;
	
	//TOP BUTTON
	pRect1 = pRect;
	pRect1.bottom = pRect1.top + CONTROL_HEIGHT;
	m_pToolLblRect[0] = m_pToolLblRect[1] = m_pToolLblRect[2] = m_pToolLblRect[3] = pRect1;
	pRect1.left = 30;
	pRect1.right = pRect1.left + ACCOUNT_WIDTH;
	
	ileft = pRect1.right + 1;

	m_pContentRect = pRect1;
	m_pContentRect.left = HORI_GAP;
	m_pContentRect.top = pRect1.bottom +VERT_GAP;
	
	pRect1.SetRect(pRect.left,   itopPanelH + VERT_GAP+m_iGridHeadrHeight+VERT_GAP, 
		pRect.right,  pRect.bottom-m_iGuideHeight);
	m_pContentRect.bottom = pRect.bottom - VERT_GAP;
	m_pContentRect.right = pRect1.right ;
	
	//TRACE("GridRow : %d, %d, %d, %d\n", pRect1.left, pRect1.right, pRect1.top, pRect1.bottom);
	const int iscrollType = GVSC_VERT;
//	m_pRemainGrid = std::make_unique<CfxGrid>();
	m_pRemainGrid = new CfxGrid();
	m_pRemainGrid->Create(pRect1, this, IDC_CONFIGGRID, iscrollType, GVDD_NONE);

// 	m_pRemainGrid->SetTitleTips(TRUE);
 	m_pRemainGrid->Initial(0, JCOLCOUNT, 0, 0);
	
	GVITEM item;
	const int ipoint = m_iOldFont * 10;
	LOGFONT	lf = GetLogFont(ipoint);
	lf.lfWeight = 500;
	
	strcpy(lf.lfFaceName, "굴림");
	for (int ii = 0; ii < JCOLCOUNT; ii++)
	{
		m_GridHdr[ii] = gridHdr[ii];
	}
// 	
// // 	for(ii = 0; ii < JROWCOUNT+1; ii++)
// // 		insertGridRow(0);
// 	

 	m_pRemainGrid->SetGridOutline(TRUE);
// 
	m_pRemainGrid->ShowZeroData(TRUE);
	m_pRemainGrid->SetRowColResize(FALSE, TRUE);
//	m_pRemainGrid->SetConditionColor(GetIndexColor(94), GetIndexColor(95), GetIndexColor(69));
	//m_pRemainGrid->SetConditionColor(GetIndexColor(95), GetIndexColor(95), GetIndexColor(95));
	m_pRemainGrid->SetGridColor(GetIndexColor(65));
	m_pRemainGrid->SetBKSelColor(GetIndexColor(78));
	m_pRemainGrid->SetFGSelColor(RGB(77, 77, 77));
	m_pRemainGrid->SetGridFocusLine(FALSE, TRUE);
	m_pRemainGrid->SetGridLine(GVLN_VERT);
	m_pRemainGrid->SetStepColor(1, GetIndexColor(68), GetIndexColor(77));
	m_pRemainGrid->Adjust();

	m_arGridHdrList.SetSize(JCOLCOUNT);
	
///	InvalidateRect(CRect(0,0,m_pClientRect.right,m_iToolheight*3));

//	SetFocus();
}



int CMainWnd::insertGridRow(int row, bool binsert, bool bheader)
{
	if(!m_pRemainGrid) 
		return -1;
	GVITEM	item;
	bool badd = binsert;
	if(!binsert)
	{
		if( m_pRemainGrid->GetRowCount() <= row) badd = true;
	}
	
	if(badd)
		row = m_pRemainGrid->InsertRow(_T(""), row);
	
	const int ipoint = m_iOldFont * 10;

	LOGFONT	lf = GetLogFont(ipoint);
	lf.lfWeight = 500;
	
	strcpy(lf.lfFaceName, "굴림");
	item.row = row;
	const int icolor = row % 2 ;
	
	CString sTxt = "";
	int iw = 0;
//	TCHAR szTemp[256];

	for (int ii = 0; ii < JCOLCOUNT; ii++)
	{
		item.col    = ii;
		item.mask   = GVMK_FORMAT|GVMK_ATTR|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = m_GridHdr[ii].format;
		item.attr   = m_GridHdr[ii].attr|m_GridHdr[ii].attrx|GVAT_ONLYROW;

		if(bheader)
		{
			item.format |= GVFM_HEADER;
			item.attr   = m_GridHdr[ii].attr;
			
			item.bkcol  = m_gridhdrbgcolor;
			item.fgcol  = m_gridhdrtxtcolor;
		}
 		else
 		{
  			item.bkcol  = m_gridRowColor[icolor];
 			item.fgcol  = m_txtcolor;
 		}
		
		if(ii == colJNO) 
		{
			item.mask |= GVMK_PARAM;
			item.param = colJMSG;
		}
		
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		m_pRemainGrid->SetItem(&item);
		
		iw = m_GridHdr[ii].width;
		
		if(ii == 0)
			iw = iw+2 ;
		if( m_GridHdr[ii].attr & GVAT_HIDDEN) 
			iw = 0;

		if(m_GridHdr[ii].itogglecol > 0)
		{
			if(m_pRemainGrid->GetRowCount() > 1)
				iw = m_pRemainGrid->GetColumnWidth(ii);
		}

CString slog;
slog.Format("IB611000 %d\n", m_pRemainGrid->GetRowCount());
//tputDebugString(slog);
/*
		if(m_pRemainGrid->GetRowCount() % 2 == 0 && ii == 1)
		{
			m_pRemainGrid->SetColumnWidth(ii, iw );
			OutputDebugString("IB611000 11");
		}
		else if(m_pRemainGrid->GetRowCount() % 2 == 0 && ii == 2)
		{
		//	m_pRemainGrid->SetColumnWidth(ii, 1);
			m_GridHdr[ii].attr &= GVAT_HIDDEN;
			OutputDebugString("IB611000 22");
		}
		else
			m_pRemainGrid->SetColumnWidth(ii, iw);
		*/
		m_pRemainGrid->SetColumnWidth(ii, iw);
		
		sTxt = "";
		if(bheader) sTxt = m_GridHdr[ii].szhdr;
		
		m_pRemainGrid->SetItemText(row, ii, sTxt);
	}
	
	m_pRemainGrid->SetRowHeight(row, m_igridRowHeight);
	m_pRemainGrid->SetFGSelColor(RGB(77, 77, 77));
	m_pRemainGrid->Adjust();
	return row;
}

COLORREF CMainWnd::GetIndexColor(UINT nIndex)
{
	if (!m_pWizard)
		return 0;
	
	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
}

CString CMainWnd::Variant(int nComm, CString strData)
{
	CString strRet(_T(""));
	
	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);
	
	if ((long)pData > 1)
		strRet = pData;
	
	return strRet;
}

CFont* CMainWnd::GetAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;

	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CFont* CMainWnd::GetAxFont(struct _fontR* font)
{
	return GetAxFont(font->name, font->point, (font->bold == FW_BOLD) ? true : false, font->italic);
}

HBITMAP CMainWnd::getBitmap(CString path)
{
	CBitmap*	pBitmap = (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)path.operator LPCTSTR());

	return pBitmap->operator HBITMAP();
}

COLORREF CMainWnd::GetAxColor(int color)
{
	if (color & 0x02000000)
		return color;
	
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}



void CMainWnd::DrawGridHeader(CDC* pDC)
{
	if(!m_pRemainGrid) return;

	CRect rc;
	m_pRemainGrid->GetWindowRect(&rc);
	ScreenToClient(&rc);
//	TRACE("Rect 좌표 : %d, %d, %d, %d \n", rc.left, rc.right, rc.top, rc.bottom);
	m_gridhdrbgcolor = GetIndexColor(74);
	m_gridLinecolor = GetIndexColor(65);

	CBrush brush(m_gridhdrbgcolor);
	CBrush* pOldBrush = pDC->SelectObject(&brush); 
	CPen pen(PS_SOLID, 1, m_gridLinecolor );
	CPen* pOldPen = pDC->SelectObject(&pen); 

	const int irowH = GRIDHEADER_HEIGHT;
	CRect grc = rc, trect;
	grc.top = m_pContentRect.top + VERT_GAP;//rc.top - irowH;
	grc.bottom = rc.top + 1;
/*	grc.left += 5;*/
	grc.top += 5; 
/*	grc.right -= 5;*/
	
	pDC->Rectangle(grc); 

	int ileft = HORI_GAP + HORI_GAP-1;
	int itop = grc.top;

	const int ihalftop = grc.top + irowH/2;
	const int ilinetop = grc.top;
	trect = CRect(1, itop,  ileft, rc.top-1);
	int row_merge{}, col_merge{};
	int icolmergeleft = 0;
	int iw = 0;
	const int ibottom = rc.top;

	const bool bflag = false;
	bool badd = false;
	CPoint	pt[4];
	CRgn	rgn;
	const int		igap = 2;
	if(m_arGridToggleList.GetSize() <= 0) badd = true;
	CString sText;
	m_pFont = (CFont *)pDC->SelectObject(GetAxFont("굴림체", 9, 0));

	pDC->SetTextColor(GetIndexColor(76));
	
//	TRACE("Rect 좌표 : %d, %d, %d, %d \n", grc.left, grc.right, grc.top, grc.bottom);

	for (int ii = 0; ii < JCOLCOUNT; ii++)
	{ 
		row_merge = m_GridHdr[ii].r_merge;
		col_merge = m_GridHdr[ii].c_merge;
		
		iw = m_GridHdr[ii].width;
		if( m_GridHdr[ii].attr & GVAT_HIDDEN ) 
		{
	 		iw = 0;
		}

		trect.left = ileft;
		trect.top = itop;
		itop = grc.top;
		if( m_GridHdr[ii].c_flag2 && icolmergeleft > 0)
		{
			pDC->DrawText(m_GridHdr[ii].szhdr0, CRect(icolmergeleft, itop, ileft+iw, ihalftop) , DT_SINGLELINE|DT_CENTER|DT_VCENTER); 
			icolmergeleft = 0;
		}

		if(!row_merge) 
		{
			trect.top = ihalftop;
			if( !(m_GridHdr[ii].c_flag1 + m_GridHdr[ii].c_flag2) ) itop = ihalftop;

			pDC->MoveTo(ileft,  ihalftop);
			pDC->LineTo(ileft+iw,  ihalftop);
		}
		
		ileft += iw;
		if(m_GridHdr[ii].c_flag1)	icolmergeleft = ileft;
		sText = m_GridHdr[ii].szhdr;

		if(m_GridHdr[ii].itogglecol > 0 && m_arGridHdrList[ii] != "") sText = m_arGridHdrList[ii];
		
		if(!col_merge)
		{
			if(m_GridHdr[ii].itogglecol > 0)	
			{
				pDC->MoveTo(ileft,  itop);
				pDC->LineTo(ileft,  ibottom);
				trect.right = ileft;
				pDC->DrawText(sText, trect, DT_SINGLELINE|DT_CENTER|DT_VCENTER); 
			}
			else
			{
				pDC->MoveTo(ileft,  itop);
				pDC->LineTo(ileft,  ibottom);
				trect.right = ileft;
				pDC->DrawText(sText, trect, DT_SINGLELINE|DT_CENTER|DT_VCENTER); 
			}
		}
		else
		{
			if(strlen(m_GridHdr[ii].szhdr) )
			{
				trect.right = (ileft-iw);
				for(int c=ii; c<JCOLCOUNT; c++)
				{
					if( !m_GridHdr[c].c_merge) break;

					iw =  m_GridHdr[c].width;

					if( m_GridHdr[c].attr & GVAT_HIDDEN) iw=0;
					trect.right += iw;
				}
				pDC->DrawText(sText, trect, DT_SINGLELINE|DT_CENTER|DT_VCENTER); 
			}
		}

		if(m_GridHdr[ii].itogglecol > 0)
		{
			pt[0] = pt[3] = CPoint(trect.right-1, trect.bottom-7);	
			pt[1] = CPoint(trect.right-1, trect.bottom);
			pt[2] = CPoint(trect.right-7, trect.bottom);

			rgn.CreatePolygonRgn(pt, 4, ALTERNATE);
			CBrush	tbrush(m_gridLinecolor );
			pDC->FillRgn(&rgn, &tbrush);
			rgn.DeleteObject();
			tbrush.DeleteObject();

			if(badd) m_arGridToggleList.Add(_List(trect, ii, m_GridHdr[ii].itogglecol));
		}
	}
	pDC->SelectObject(pOldPen); 
	pDC->SelectObject(pOldBrush); 

	DeleteObject(pOldPen);
	DeleteObject(pOldBrush);
}

LOGFONT CMainWnd::GetLogFont(int fsize, bool bold, CString sFont)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize;
	
	strcpy(lf.lfFaceName, sFont);   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	if(bold)
		lf.lfWeight = FW_BOLD;
	else
		lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	return lf;
}

void CMainWnd::MainQueryExcelSise(int gno)
{
	CString grcd;

	grcd.Format("C%04d", gno);
	SendTR("pihomyfd", grcd, FUNDINIT, 0);
}


LRESULT CMainWnd::SendTR(CString strCode, CString strData, int iKey, int iStat)
{
//	testSaveFile3("SendTR", "strCode[" + strCode + "] strData[" + strData +"]");

	std::unique_ptr<char[]> pcDataBuffer = std::make_unique<char[]>(L_userTH + strData.GetLength());

//	ZeroMemory(pcDataBuffer.get(), sizeof(pcDataBuffer));
	struct	_userTH* puserTH;
	puserTH = (struct _userTH *)pcDataBuffer.get();
	
	strcpy(puserTH->trc, strCode.operator LPCTSTR());
	puserTH->key = iKey;
	puserTH->stat = iStat;
	
	CopyMemory(&pcDataBuffer.get()[L_userTH], strData.operator LPCTSTR(), strData.GetLength());
	
	const LRESULT lResult = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM) pcDataBuffer.get());
	
	pcDataBuffer = nullptr;

	return lResult;
}


void CMainWnd::InitBtntab()
{
	HBITMAP	hBITMAP{}, hBITMAP_DN{}, hBITMAP_HV{}, hBITMAP2{}, hBITMAP2_DN{}, hBITMAP2_HV{};

	m_fileIMG.Format("%s\\%s\\", m_root, IMAGEDIR);
	
	m_cbArrange.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_ARRANGE);
	m_cbArrange.SetFont(m_pFont);	 

	m_cbFIELD.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_FIELD);
	m_cbFIELD.SetFont(m_pFont);
	
	m_cbGROUP.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_GROUP);
	m_cbGROUP.SetFont(m_pFont);
	
	CFont *font;
	font = GetAxFont("굴림",9,true);
	
	/*m_btntab.ShowWindow(SW_SHOW);*/
	hBITMAP2 = getBitmap(m_fileIMG + "엑셀.bmp");
	hBITMAP2_DN = getBitmap(m_fileIMG + "엑셀_DN.bmp");
	hBITMAP2_HV = getBitmap(m_fileIMG + "엑셀_EN.bmp");
	
	m_btExcel.Create("", CRect(0, 0, 20, SIZE_BUTTONS), this, IDC_BT_EXCEL, false);
	m_btExcel.SetFont(font, false);  
	m_btExcel.SetImgBitmap(hBITMAP2, hBITMAP2_DN, hBITMAP2_HV);
 	m_btExcel.SetTextColor(GetAxColor(186));//BUFFET
	
	m_btntab.Create(WS_CHILD|TCS_BUTTONS|TCS_OWNERDRAWFIXED|TCS_RAGGEDRIGHT, CRect(0, 0, 0, 0), this, IDC_BTNTAB);
	m_btntab.SetFont(m_pFont, false);
	m_btntab.SetColor(RGB(255, 255, 255), GetAxColor(14), RGB(255, 255, 32), GetAxColor(14), GetAxColor(66));
	m_btntab.m_bitmapT_nm = getBitmap(m_fileIMG + "관심TAB.bmp");
	m_btntab.m_bitmapT_dn = getBitmap(m_fileIMG + "관심TAB_dn.bmp");
	m_btntab.m_bitmapT_hv = getBitmap(m_fileIMG + "관심TAB_en.bmp");

	hBITMAP = getBitmap(m_fileIMG + "6BTN_조회_EN.BMP");
	hBITMAP_DN = getBitmap(m_fileIMG + "6BTN_조회_DN.BMP");
	hBITMAP_HV = getBitmap(m_fileIMG + "6BTN_조회_DN.BMP");
	
	const UINT	ids[] = {IDC_BT_CONFIG, IDC_BT_EXPECT};
	const CString texts[] = {"비교펀드등록\n비교펀드설정", "예상\n예상체결가"};	
	//CString textExpect = "자동\n자동으로 예상체결가보기";
	
	const bool	bchecks[] = {false, true};

	//m_btTEXT.Create(texts[0], CRect(0, 0, 38, SIZE_BUTTONS), this, ids[0], bchecks[0]);
	m_btTEXT.Create(texts[0], CRect(0, 0, 90, SIZE_BUTTONS), this, ids[0], bchecks[0], true);
	m_btTEXT.SetFont(font, false);  
	m_btTEXT.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btTEXT.SetTextColor(GetAxColor(186));//BUFFET
}


void CMainWnd::OnDestroy() 
{
	CWnd::OnDestroy();
		
	if(dlg)
	{
		dlg->CloseWindow();
		dlg = nullptr;
	}

 	//DeleteGridItem();
//	m_gridiem.RemoveAll();
//	m_grouplist.RemoveAll();

	if(m_btntab)
	{
//		m_btntab.DestroyWindow();
//		delete m_btntab;
	}

//	delete m_pRemainGrid;
 	m_pRemainGrid = NULL;
/*
	m_btTEXT.DestroyWindow();
	delete m_btTEXT;

 	m_btExcel.DestroyWindow();
 	delete m_btExcel;
	
	m_cbArrange.DestroyWindow();
	delete m_cbArrange;
	m_cbGROUP.DestroyWindow();
	delete m_cbGROUP;
	m_cbFIELD.DestroyWindow();
	delete m_cbFIELD;
*/


// 	
//	m_arGridToggleList.RemoveAll();
//	m_arGridHdrList.RemoveAll();
		
// 	if(m_pRemainGrid)
// 	{
// //		m_pRemainGrid->DeleteAllItems();
// 		delete m_pRemainGrid;
// 		m_pRemainGrid = NULL;
// 	}
		
}

CString	CMainWnd::GetGroupName(int gno)
{
	_grouplist* grouplist{};
	int ncnt = 0;
	CString groupNo;

	ncnt = m_grouplist.GetSize();
	
	for(int ii = 0; ii < ncnt ; ii++)
	{
		grouplist = m_grouplist.GetAt(ii);
		
		groupNo.Format("%04d", gno);

		if(strcmp(groupNo, grouplist->gno))
			return grouplist->gnm;
	}

	return "";
}

void CMainWnd::loadcfg()
{
	// MODIFY PSH 20070918
	//BOOL	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_EXPAND, 0, m_fileCFG);
	BOOL	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_EXPAND, 1, m_fileCFG);
	// END MODIFY

	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_AUTO, 1, m_fileCFG);
	
	// ADD PSH 20070918
	m_bMoveCfg  = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_MOVECFG, 0, m_fileCFG);
	m_nMoveSave = GetPrivateProfileInt(SEC_TOOL, KEY_MOVESAVE, 0, m_fileCFG);
	// END ADD
	
	const int	nmode = (int)GetPrivateProfileInt(SEC_TOOL, KEY_FILLMODE, MO_SELECT, m_fileCFG);
	
	SelectMode(nmode);
	
	m_bBig = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_BIG, 1, m_fileCFG);
	const bool m_bIsEqual = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_EQUALIZER, 0, m_fileCFG);
	
	int	ncount = (int)GetPrivateProfileInt(SEC_GROUP, KEY_COUNT, 1, m_fileCFG);
	
	if (ncount <= 0)
		ncount = 1;
	
	if (ncount > MAX_GROUP)
		ncount = MAX_GROUP;
	
	const int	nval = GetPrivateProfileInt(SEC_TOOL, KEY_TIME, 3, m_fileCFG);
	CString	sztmp;
	sztmp.Format("%d", nval);
	//m_spinTIME->SetText(sztmp);
	
	m_fileFIELD.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);	
	m_activefield = loadFieldnm(loadActiveField());
	
	m_filePPOLIO.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_GROUP);	
	loadTabItem();	
	
	CString cb[] = {"등록순", "업종순", "종목명순", "종목코드순"};
	
	int nTemp{}, jj{};
	for (int ii = 0; ii<4 ; ii++)
	{
		nTemp = m_cbArrange.AddString(cb[ii]);
		jj = ii+1;
		m_cbArrange.SetItemData(nTemp, jj);
	}
	
	//pyt
	// 	ncount = (int)GetPrivateProfileInt(SEC_TOOL, KEY_CBARRANGE, 0, m_fileCFG);
	m_cbArrange.SetCurSel(0);
}

void CMainWnd::SelectMode(int nmode)
{
// 	m_btAddSelect.SetCheck(false);
// 	m_btAddVisible.SetCheck(false);
// 	m_btAddFull.SetCheck(false);
	
	switch (nmode)
	{
	case MO_VISIBLE:
//		m_btAddVisible.SetCheck(true);
		break;
	case MO_FULL:
		//		m_btAddFull.SetCheck(true);
		//		break;
	case MO_SELECT:
	default:
//		m_btAddSelect.SetCheck(true);
		break;
	}
}


CString CMainWnd::loadFieldnm(int fno)
{
	CString ret = _T("");
	m_cbFIELD.ResetContent();
//	RemoveAllMenu();
	
	CFileFind finder;
	if (!finder.FindFile(m_fileFIELD))
	{
		CString	string = DEF_FIELD_CONTENT1;
		WritePrivateProfileString(SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
	}
	
	int	readL{};
	char	readB[1024]{};
	CString	fieldnm, fieldno, string, stringx;
	int	index = 0, active = -1;
	
	// ADD PSH 20070911
	const int nCnt = 0;
	// END ADD
	
	for (int ii = 0; ii < MAX_FIELD; ii++)
	{
		fieldno.Format("%02d", ii);
		
		readL = GetPrivateProfileString(SEC_FIELDS, fieldno, "", readB, sizeof(readB), m_fileFIELD);
		if (readL > 0)
		{
			string = CString(readB, readL);
			stringx = IH::Parser(string, ";");		// fields name			
			if (!stringx.IsEmpty())
			{
				index = m_cbFIELD.AddString(stringx);
				m_cbFIELD.SetItemData(index, ii); 
				if (ii == fno) active = index;
				
				// ADD PSH 20070911
//				m_menuField.AppendMenu(MF_STRING, IDC_MENU_FIELD + nCnt++, stringx);
				// END ADD
			}
		}
	}
	
	if (m_cbFIELD.GetCount() > 0)
	{
		if (active < 0) 
			active = 0;
		
		m_cbFIELD.SetCurSel(active);
		const int	ndata = m_cbFIELD.GetItemData(active);
		CString key;
		key.Format("%02d", ndata);
		readL = GetPrivateProfileString(SEC_FIELDS, key, "", readB, sizeof(readB), m_fileFIELD);
		ret.Format("%s", readB);		
		IH::Parser(ret, ";");		
		
		// ADD PSH 20070911
		m_cbFIELD.GetLBText(active, key);
// 		m_btField.SetWindowText(key);
// 		m_menuField.CheckMenuItem(IDC_MENU_FIELD + active, MF_CHECKED);
		// END ADD
	}
	else
	{
		CString	string = DEF_FIELD_CONTENT1;
		WritePrivateProfileString(SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
		loadFieldnm();
	}
	
	return ret;
}

int CMainWnd::loadActiveField()
{
	CString tempFile; tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);
	
	CFileFind finder; if (!finder.FindFile(tempFile)) return -1;
	
	int	readL = 0;
	char	readB[256];
	
	readL = GetPrivateProfileString(SEC_FIELDS, KEY_ACTIVE, "00", readB, sizeof(readB), tempFile);

	return atoi(CString(readB, readL));
}

void CMainWnd::loadTabItem()
{
	//int bIndex = m_btntab.GetCurSel();
	m_btntab.DeleteAllItems(); 
	m_cbGROUP.ResetContent();
	m_selectedTab = m_lastpage;
	
// 	int readL = 0, idx =  0, nTemp = 0;
// 	char readB[1024];
	CString	string, gno, gname;	
	int idx{}, idx2{};
	
	const int ncnt = m_grouplist.GetSize();
	_grouplist* grouplist{};

	for(int ii = 0; ii < ncnt; ii++)
	{
		grouplist = m_grouplist.GetAt(ii);
		
		gno = CString(grouplist->gno, sizeof(grouplist->gno));
		gname = CString(grouplist->gnm, sizeof(grouplist->gnm));
// 		strcpy(gno, grouplist->fdcd);
// 		strcpy(gname, grouplist->fdnm);

		if(!gname.IsEmpty())
		{
			idx = atoi(gno);
			
		//	m_gubn.SetItemData(idx, gubncnt);
			
			m_btntab.InsertItem(ii, " " + gname);
			idx2 = m_cbGROUP.AddString(gname);
			m_cbGROUP.SetItemData(idx2, ii);
		}
	}

	//	TRACE("Tab iNdex : %d\n", bIndex);
	if (m_lastpage >= 0 && m_lastpage < m_cbGROUP.GetCount())
	{
		m_btntab.SetCurSel(m_lastpage);
// 		if(m_lastpage == m_cbGROUP.GetCount()-1)
// 		{
// 			m_btntab.SetCurSel(m_lastpage-3);
// 		}
		
		m_cbGROUP.SetCurSel(m_lastpage);
		
		//		SendTree(bIndex);
	}
}

CRect CMainWnd::MoveButton(CRect rcPrev, CSize size, CSize gap, CfxImgButton* pButton)
{
	CRect	rect;
	rect = GetNextRect(rcPrev, size, gap);
	pButton->MoveWindow(rect, FALSE);
	pButton->ShowWindow(SW_SHOW);
	pButton->Invalidate();
	m_arCONTROL.Add((CWnd*)pButton);

	return rect;
}

CRect CMainWnd::GetNextRect(CRect rcPrev, CSize size, CSize gap)
{
	CRect	rect;
	
	rect.left = (gap.cx) ? rcPrev.right + gap.cx : rcPrev.left;
	rect.right = rect.left + size.cx;
	rect.top = (gap.cy) ? rcPrev.bottom + gap.cy : rcPrev.top;
	rect.bottom = rect.top + size.cy;
	
	return rect;
}

void CMainWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonDown(nFlags, point);


//	MessageBox("마우스 클릭", NULL, MB_OK);
}


BOOL CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	const BOOL	bCheck = FALSE;
	CString	sztmp = _T("");
	const int	GroupNumber = 0;
	//int	nOver = 0;

	switch (LOWORD(wParam))
	{
	case  IDC_BT_CONFIG:
	//	dlg.DoModal();
		dlg = nullptr;
		dlg = std::make_unique<CDlg>(m_pWizard, this);
		m_dlg = TRUE;
		dlg->DoModal();

		dlg = nullptr;
// 		dlg->Create(IDD_IBSETUP, this);
// 		dlg->ShowWindow(SW_SHOW);
//		dlg.Create("관심펀드  설정", m_pWizard);
		break;
	case IDC_BT_EXCEL:
//		if (m_pRemainGrid) m_pRemainGrid->RunExcel();
		ExcelList();
		break;
	}

	return CWnd::OnCommand(wParam, lParam);
}

CString CMainWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_root, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

void CMainWnd::GridDataParsing(char* datB, int datL)
{
//	testSaveFile3("GridDataParsing", CString(datB, datL));

	char *buf = datB;
	struct _mod3 *mod3;
	mod3 = (struct _mod3*)buf;
	CString fdcd, fdnm, sprc, dtrt, wkrt, rt01, rt03, rt06, rt12, rt36, yrat, rate, idnm, term, stdt, strt, ivnm, cfName, tmps;
	CString nrce;
	int ncnt = 0;
	item3* griditem{};

	nrce.Format("%.*s", sizeof(mod3->nrce), mod3->nrce);
	ncnt = atoi(nrce);

 	if(!m_init)
	{
		m_gridiem.RemoveAll();	
	}
	
	for(int ii = 0; ii < ncnt; ii++)
	{
		griditem = new item3();
		
		fdcd= CString(mod3->list[ii].fdcd, sizeof(mod3->list[ii].fdcd));
		fdcd.TrimRight();
		fdnm= CString(mod3->list[ii].fdnm, sizeof(mod3->list[ii].fdnm));
		fdnm.TrimRight();
		sprc= CString(mod3->list[ii].sprc, sizeof(mod3->list[ii].sprc));
		sprc.TrimLeft();
		dtrt= CString(mod3->list[ii].dtrt, sizeof(mod3->list[ii].dtrt));
		dtrt.TrimLeft();
		wkrt= CString(mod3->list[ii].wkrt, sizeof(mod3->list[ii].wkrt));
		wkrt.TrimLeft();
		rt01= CString(mod3->list[ii].rt01, sizeof(mod3->list[ii].rt01));
		rt01.TrimLeft();
		rt03= CString(mod3->list[ii].rt03, sizeof(mod3->list[ii].rt03));
		rt03.TrimLeft();
		rt06= CString(mod3->list[ii].rt06, sizeof(mod3->list[ii].rt06));
		rt06.TrimLeft();
		rt12= CString(mod3->list[ii].rt12, sizeof(mod3->list[ii].rt12));
		rt12.TrimLeft();
		rt36= CString(mod3->list[ii].rt36, sizeof(mod3->list[ii].rt36));
		rt36.TrimLeft();
		yrat= CString(mod3->list[ii].yrat, sizeof(mod3->list[ii].yrat));
		yrat.TrimLeft();
		rate= CString(mod3->list[ii].rate, sizeof(mod3->list[ii].rate));
		rate.TrimLeft();
		idnm= CString(mod3->list[ii].idnm, sizeof(mod3->list[ii].idnm));
		idnm.TrimRight();
		term= CString(mod3->list[ii].term, sizeof(mod3->list[ii].term));
		term.TrimLeft();
		stdt= CString(mod3->list[ii].stdt, sizeof(mod3->list[ii].stdt));
		strt= CString(mod3->list[ii].strt, sizeof(mod3->list[ii].strt));
		strt.TrimLeft();
		ivnm= CString(mod3->list[ii].ivnm, sizeof(mod3->list[ii].ivnm));
		ivnm.TrimRight();

		tmps= CString(mod3->list[ii].cfName, sizeof(mod3->list[ii].cfName));
		tmps.TrimRight();
		tmps.TrimLeft();
/*
CString slog;
slog.Format(" --[%s]", tmps);
OutputDebugString("IB611000 " + slog  );
*/
		if(tmps != "")
			cfName.Format("[%s]", tmps);
		else
			cfName.Format("%s", tmps);

OutputDebugString("IB611000" + cfName  );
		strcpy(griditem->fdcd, fdcd);
		strcpy(griditem->fdnm, fdnm);
		strcpy(griditem->sprc, sprc);
		strcpy(griditem->dtrt, dtrt);
		strcpy(griditem->wkrt, wkrt);
		strcpy(griditem->rt01, rt01);
		strcpy(griditem->rt03, rt03);
		strcpy(griditem->rt06, rt06);
		strcpy(griditem->rt12, rt12);
		strcpy(griditem->rt36, rt36);
		strcpy(griditem->yrat, yrat);
		strcpy(griditem->rate, rate);
		strcpy(griditem->idnm, idnm);
		strcpy(griditem->term, term);
		strcpy(griditem->stdt, stdt);
		strcpy(griditem->strt, strt);
		strcpy(griditem->ivnm, ivnm);
		strcpy(griditem->cfName, cfName);

		m_gridiem.Add(griditem);
	}

//	testSaveFile3("GridDataParsing", "END");

	GridData();	
}

void CMainWnd::GridData()
{
//	testSaveFile3("GridData", "START");

	int ncnt = m_gridiem.GetSize();
	
	ncnt *= 2;
	item3* griditem{};
	int rowCnt = m_pRemainGrid->GetRowCount();
	double dtrt{}, wkrt{}, rt01{}, rt03{}, rt06{}, rt12{}, rt36{}, yrat{}, rate{};
	CString stdt;
	m_pRemainGrid->Clear();	

	if(!m_init)
	{
// 		for(int i = 0; i < rowCnt; i++)
// 			m_pRemainGrid->DeleteRow(i);
		for(; rowCnt >= 0 ; rowCnt--)
			m_pRemainGrid->DeleteRow(rowCnt);		
	}
	
	if(ncnt <= 20)
	{
		for(int ii =0; ii < JROWCOUNT; ii++)
			insertGridRow(0);
	}
	else
	{
		for(int ii = 0; ii < ncnt; ii++)
			insertGridRow(0);
	}

	for (int ii = 0; ii < ncnt; ii++)
	{
		if(ii % 2 == 1) 
		{ 
			m_pRemainGrid->SetItemText(ii, FDNM, CString(griditem->cfName, sizeof(griditem->cfName))); 
			m_pRemainGrid->SetItemFgColor(ii, FDNM, RGB(170, 170, 170));
			continue;
		}
//		griditem = m_gridiem.GetAt(ii);	
		griditem = m_gridiem.GetAt(ii/2);	
		
		m_pRemainGrid->SetItemText(ii, FDCD, CString(griditem->fdcd, sizeof(griditem->fdcd)));
		m_pRemainGrid->SetItemFgColor(ii, FDCD, RGB(77, 77, 77));
		m_pRemainGrid->SetItemText(ii, FDNM, CString(griditem->fdnm, sizeof(griditem->fdnm))); 
		m_pRemainGrid->SetItemFgColor(ii, FDNM, RGB(77, 77, 77));
		m_pRemainGrid->SetItemText(ii, SPRC, CString(griditem->sprc, sizeof(griditem->sprc)));
		m_pRemainGrid->SetItemFgColor(ii, SPRC, RGB(77, 77, 77));
		m_pRemainGrid->SetItemText(ii, DTRT, CString(griditem->dtrt, sizeof(griditem->dtrt)));
		
		dtrt = atof(griditem->dtrt);
		if(dtrt >0)
			m_pRemainGrid->SetItemFgColor(ii, DTRT, RGB(255, 0, 0));
		else if(dtrt == 0)
			m_pRemainGrid->SetItemFgColor(ii, DTRT, RGB(77, 77, 77));
		else
			m_pRemainGrid->SetItemFgColor(ii, DTRT, RGB(0, 0, 255));

		m_pRemainGrid->SetItemText(ii, WKRT, CString(griditem->wkrt, sizeof(griditem->wkrt)));

		wkrt = atof(griditem->wkrt);

		if(wkrt >0)
			m_pRemainGrid->SetItemFgColor(ii, WKRT, RGB(255, 0, 0));
		else if(wkrt == 0)
			m_pRemainGrid->SetItemFgColor(ii, WKRT, RGB(77, 77, 77));
		else
			m_pRemainGrid->SetItemFgColor(ii, WKRT, RGB(0, 0, 255));

		m_pRemainGrid->SetItemText(ii, RT01, CString(griditem->rt01, sizeof(griditem->rt01)));
	
		rt01 = atof(griditem->rt01);
		if(rt01 >0)
			m_pRemainGrid->SetItemFgColor(ii, RT01, RGB(255, 0, 0));
		else if(rt01 == 0)
			m_pRemainGrid->SetItemFgColor(ii, RT01, RGB(77, 77, 77));
		else
			m_pRemainGrid->SetItemFgColor(ii, RT01, RGB(0, 0, 255));

		m_pRemainGrid->SetItemText(ii, RT03, CString(griditem->rt03, sizeof(griditem->rt03)));

		rt03 = atof(griditem->rt03);
		if(rt03 >0)
			m_pRemainGrid->SetItemFgColor(ii, RT03, RGB(255, 0, 0));
		else if(rt03 == 0)
			m_pRemainGrid->SetItemFgColor(ii, RT03, RGB(77, 77, 77));
		else
			m_pRemainGrid->SetItemFgColor(ii, RT03, RGB(0, 0, 255));
		
		m_pRemainGrid->SetItemText(ii, RT06, CString(griditem->rt06, sizeof(griditem->rt06)));

		rt06 = atof(griditem->rt06);
		if(rt06 >0)
			m_pRemainGrid->SetItemFgColor(ii, RT06, RGB(255, 0, 0));
		else if(rt06 == 0)
			m_pRemainGrid->SetItemFgColor(ii, RT06, RGB(77, 77, 77));
		else
			m_pRemainGrid->SetItemFgColor(ii, RT06, RGB(0, 0, 255));

		m_pRemainGrid->SetItemText(ii, RT12, CString(griditem->rt12, sizeof(griditem->rt12)));

		rt12 = atof(griditem->rt12);
		if(rt12 >0)
			m_pRemainGrid->SetItemFgColor(ii, RT12, RGB(255, 0, 0));
		else if(rt12 == 0)
			m_pRemainGrid->SetItemFgColor(ii, RT12, RGB(77, 77, 77));
		else
			m_pRemainGrid->SetItemFgColor(ii, RT12, RGB(0, 0, 255));

		m_pRemainGrid->SetItemText(ii, RT36, CString(griditem->rt36, sizeof(griditem->rt36)));
		rt36 = atof(griditem->rt36);
		if(rt36 >0)
			m_pRemainGrid->SetItemFgColor(ii, RT36, RGB(255, 0, 0));
		else if(rt36 == 0)
			m_pRemainGrid->SetItemFgColor(ii, RT36, RGB(77, 77, 77));
		else
			m_pRemainGrid->SetItemFgColor(ii, RT36, RGB(0, 0, 255));
		
		m_pRemainGrid->SetItemText(ii, YRAT, CString(griditem->yrat, sizeof(griditem->yrat)));
		yrat = atof(griditem->yrat);
		if(yrat >0)
			m_pRemainGrid->SetItemFgColor(ii, YRAT, RGB(255, 0, 0));
		else if(yrat == 0)
			m_pRemainGrid->SetItemFgColor(ii, YRAT, RGB(77, 77, 77));
		else
			m_pRemainGrid->SetItemFgColor(ii, YRAT, RGB(0, 0, 255));
		m_pRemainGrid->SetItemText(ii, RATE, CString(griditem->rate, sizeof(griditem->rate)));

		rate = atof(griditem->rate);
		if(rate >0)
			m_pRemainGrid->SetItemFgColor(ii, RATE, RGB(255, 0, 0));
		else if(rate == 0)
			m_pRemainGrid->SetItemFgColor(ii, RATE, RGB(77, 77, 77));
		else
			m_pRemainGrid->SetItemFgColor(ii, RATE, RGB(0, 0, 255));

		m_pRemainGrid->SetItemText(ii, IDNM, CString(griditem->idnm, sizeof(griditem->idnm)));
		m_pRemainGrid->SetItemFgColor(ii, IDNM, RGB(77, 77, 77));
		m_pRemainGrid->SetItemText(ii, TERM, CString(griditem->term, sizeof(griditem->term)));
		m_pRemainGrid->SetItemFgColor(ii, TERM, RGB(77, 77, 77));
		stdt = CString(griditem->stdt, sizeof(griditem->stdt));
		stdt.Insert(4, "/");
		stdt.Insert(7, "/");
		
		m_pRemainGrid->SetItemText(ii, STDT, stdt);
		m_pRemainGrid->SetItemFgColor(ii, STDT, RGB(77, 77, 77));
		m_pRemainGrid->SetItemText(ii, STRT, CString(griditem->strt, sizeof(griditem->strt)));
		m_pRemainGrid->SetItemFgColor(ii, STRT, RGB(77, 77, 77));
		m_pRemainGrid->SetItemText(ii, IVNM, CString(griditem->ivnm, sizeof(griditem->ivnm)));
		m_pRemainGrid->SetItemFgColor(ii, IVNM, RGB(77, 77, 77));
	//	TRACE("%s %s %s ", griditem->fdcd, griditem->fdnm, griditem->sprc);
	}

//	testSaveFile3("GridData", "END");
}

void CMainWnd::QueryExcelSise()
{
	CString strData, grcd;
	char sendB[1]{};
	struct _4mid1* mid1 = (struct _4mid1 *)&sendB[0];
	
	FillMemory(&mid1, sizeof(struct _4mid1), ' ');
	FillMemory(sendB, sizeof(sendB), ' ');
	
	CopyMemory(sendB, "A", sizeof(mid1->gubn));
	
	//strData = CString((char *)&mid2, sizeof(struct __mid2));
	//TRACE("Main data : %s\n", strData);
	//	SendTR("pihomyfd", strData, FUNDINIT, 0);
	SendTR("pihomyfd",  sendB, FUNDSEARCH, 0);

//	testSaveFile3("QueryExcelSise", "pihomyfd");
}

void CMainWnd::parsingData(char* datB, int datL)
{
	struct item1
	{
		char type[1];
		char data[40];
	};
	
	struct mod1
	{
		char nrec[4];
		char ltcd[4];
		
		struct item1 item1[MYFD_DEFMAXCNT];
	}*mod1{};
	
	char *buf = datB;
	mod1 = (struct mod1*)buf;
	CString nrec, ltcd;
	CString type, data;
	CString fdcd, fdnm;

	CString s;
	s.Format("6110 [%s]\n",datB);
	OutputDebugString(s);

	int ncnt = 0;
	//int idx = 0;
	//int gubncnt = 0;
	//int ivgd = 0;
	_grouplist* grouplist{};
	
	nrec.Format("%.*s", sizeof(mod1->nrec), mod1->nrec);
	ncnt = atoi(nrec);
	
	ltcd.Format("%.*s", sizeof(mod1->ltcd), mod1->ltcd);
	ltcd.TrimLeft();

	const int _ltcd= atoi(ltcd);

	if(_ltcd == -1)
		m_lastpage = 0;
	else
		m_lastpage = _ltcd;

	m_grouplist.RemoveAll();

	for(int ii = 0; ii < ncnt; ii++)
	{
		type.Format("%.*s", sizeof(mod1->item1[ii].type), mod1->item1[ii].type);
		type.TrimLeft();
		
		data.Format("%.*s", sizeof(mod1->item1[ii].data), mod1->item1[ii].data);
		data.TrimRight();
	
		if(strcmp(type, "3") == 0)
		{
			grouplist = new _grouplist();
			
			fdcd = data.Mid(0, 4);
			const int len = strlen(data);
			fdnm = data.Mid(4, len);
			
			strcpy(grouplist->gno, fdcd);
			strcpy(grouplist->gnm, fdnm);
 			
// 			CopyMemory(grouplist->gno, fdcd, strlen(fdcd));
// 			CopyMemory(grouplist->gnm, fdnm, strlen(fdnm));
			
			m_grouplist.Add(grouplist);
		}
	//	TRACE("GroupNO: %s, name : %s \n", grouplist->fdcd, grouplist->fdnm);
	}
	
	loadTabItem();
}

int CMainWnd::GetLastPage()
{
	return m_lastpage;
}

void CMainWnd::OnSelchangeBtntab(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	testSaveFile3("OnSelchangeBtntab", "START");

	m_btntab.SetFocus();
	
	SetGroupWndActiveChk();
	
	clickBtn();
	*pResult = 0;

//	testSaveFile3("OnSelchangeBtntab", "END");
	//	testSaveFile();
}

void CMainWnd::SetGroupWndActiveChk()
{	
	m_cbArrange.SetCurSel(0);
	//pyt
// 	CWnd* pgWnd = (CWnd*)m_pWizard->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
// 	pgWnd->SendMessage(WM_MANAGE, MK_SETARRANGEACTIVE);
}

void CMainWnd::clickBtn()
{
//	testSaveFile3("clickBtn", "START");

	const int	nIndex = m_btntab.GetCurSel();
	if (nIndex >= 0 )
	{
		if(m_selectedTab != -1)
		{
			//saveGroupwhenClose(m_selectedTab);
		}
		m_selectedTab = nIndex;
// 		if(nIndex == -1)
// 			nIndex = 0;
		MainQueryExcelSise(nIndex);

//		testSaveFile3("clickBtn", "MainQueryExcelSise");

		//		gno = GetGroupNo(nIndex, gnm);
		
	//	SendTree(nIndex);
	}
	m_init = FALSE;

//	testSaveFile3("clickBtn", "END");
}

CString CMainWnd::GetGroupNo(int nIndex, CString gname)
{
	_grouplist* grouplist{};
	int ncnt = 0;
	ncnt = m_grouplist.GetSize();

	for(int ii = 0; ii < ncnt ; ii++)
	{
		grouplist = m_grouplist.GetAt(ii);

		if(ii >= nIndex && strcmp(gname, grouplist->gnm))
			return grouplist->gno;
	}

	return "";
}

void CMainWnd::ParsingGroup(char* datB, int datL)
{
	char *buf = datB;

	struct _mod2* mod2{};
	_grouplist* grouplist{};

	mod2 = (struct _mod2*)buf;

	CString rest, ercd, nrec;
	CString ngrd, grnm;
	int ncnt = 0;

	m_grouplist.RemoveAll();

	nrec.Format("%.*s", sizeof(mod2->nrec), mod2->nrec);
	ncnt = atoi(nrec);

	for(int ii= 0; ii < ncnt; ii++)
	{
		grouplist = new _grouplist();

		ngrd = CString(mod2->list[ii].ngrd, sizeof(mod2->list->ngrd));

		grnm = CString(mod2->list[ii].grnm, sizeof(mod2->list->grnm));
		grnm.TrimRight();
		
		strcpy(grouplist->gno, ngrd);
		strcpy(grouplist->gnm, grnm);

		m_grouplist.Add(grouplist);
	}
}

void CMainWnd::ParsingDataNGroup(char* datB, int datL)
{
	struct _mod1* mod1{};
	char *buf = datB;
	mod1 = (struct _mod1*)buf;
	CString rest, ercd;
	CString str;

	rest.Format("%.*s", sizeof(mod1->rest), mod1->rest);
	
	ercd.Format("%.*s", sizeof(mod1->ercd), mod1->ercd);
	
	str = "관심펀드가 서버에 저장되지 않았습니다. Error CODE : " + ercd;
	if(strcmp(rest, "N") == 0)
		SetGuideMessage(str);
}

void CMainWnd::ParsingRestore(char* datB, int datL)
{
	struct _modB3* mod3{};
	
	char *buf = datB;
	mod3 = (struct _modB3*)buf;
	
	CString rest(mod3->rest, sizeof(mod3->rest));
	CString ercd(mod3->ercd, sizeof(mod3->ercd));
	
	// 	rest.Format("%.s", sizeof(mod3->rest), mod3->rest);
	// 	ercd.Format("%.s", sizeof(mod3->ercd), mod3->ercd);
	 CString str = "데이터가 복구되지 않았습니다. Error CODE : " + ercd;
	if(strcmp(rest, "N") == 0)
		SetGuideMessage(str);
			
	//DestroyWindow();
}

BOOL CMainWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_GVITEM* nmgv = (NM_GVITEM*) lParam;
		 
	if( wParam != IDC_CONFIGGRID && wParam != IDC_BTNTAB) return 0;

	CString string, code;

	m_iSelectRow = nmgv->row;
	switch (nmgv->hdr.code)
	{
		case GVNM_DBLCLICKED:
//			code = m_pRemainGrid->GetItemText(nmgv->row, FDCD); 
			if(nmgv->row % 2 == 1)
				code = m_pRemainGrid->GetItemText(nmgv->row - 1, FDCD);
			else
				code = m_pRemainGrid->GetItemText(nmgv->row, FDCD);

			code.TrimLeft(); code.TrimRight();
			if(strcmp(code, "") == 0)
			{
				SetGuideMessage("선택된 관심펀드가 없습니다. 관심펀드를 선택해 주세요.");
				return 0;
			}
			string.Format("%s /S/t0/d%s\t%s", DetailFund, "fundcode", code);
			openView(typeVIEW, string);
			break;
	}
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

int CMainWnd::openView(int type, CString data)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

void CMainWnd::ExcelList()
{
	_Application app;
	Workbooks books;
	_Workbook book;
	
	Worksheets sheets;
	_Worksheet sheet;
	Range range;
	Font font;
	Range cols;
	Interior interior;
	Borders borders;
	
	int ii{}, size{};
	char r_end[4]{};
	memset (r_end, 0x00, sizeof(r_end));
	
	// Commonly used OLE variants.
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	
	if(!app.CreateDispatch("Excel.Application"))
	{
		AfxMessageBox("엑셀 파일을 열수 없습니다.");
		return;
	}
 
	//Get a new workbook.
	books = app.GetWorkbooks();
	book = books.Add (covOptional);
	
	//Get the first sheet.
	sheets =book.GetSheets();
	sheet = sheets.GetItem(COleVariant((short)1));
	
	//Fill cells A1, B1, C1, and D1 one cell at a time with "headers".
	range = sheet.GetRange(COleVariant("A1"),COleVariant("B2"));
	range.SetMergeCells((COleVariant)"TRUE");
	range.SetValue2(COleVariant("펀드명"));
	range.SetHorizontalAlignment(COleVariant((short)-4108)); //xlVAlignCenter
	
	range = sheet.GetRange(COleVariant("C1"),COleVariant("C2"));
	range.SetMergeCells((COleVariant)"TRUE");
	range.SetValue2(COleVariant("기준가"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));

 	range = sheet.GetRange(COleVariant("D1"),COleVariant("L1"));
 	range.SetMergeCells((COleVariant)"TRUE");
 	range.SetValue2(COleVariant("수익률"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));

	range = sheet.GetRange(COleVariant("D2"),COleVariant("D2"));
	range.SetValue2(COleVariant("1일"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("E2"),COleVariant("E2"));
	range.SetValue2(COleVariant("1주"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("F2"),COleVariant("F2"));
	range.SetValue2(COleVariant("1개월"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("G2"),COleVariant("G2"));
	range.SetValue2(COleVariant("3개월"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("H2"),COleVariant("H2"));
	range.SetValue2(COleVariant("6개월"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("I2"),COleVariant("I2"));
	range.SetValue2(COleVariant("1년"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("J2"),COleVariant("J2"));
	range.SetValue2(COleVariant("3년"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("K2"),COleVariant("K2"));
	range.SetValue2(COleVariant("연초"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("L2"),COleVariant("L2"));
	range.SetValue2(COleVariant("누적"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("M1"),COleVariant("M2"));
	range.SetMergeCells((COleVariant)"TRUE");
	range.SetValue2(COleVariant("간접투자유형"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("N1"),COleVariant("N2"));
	range.SetMergeCells((COleVariant)"TRUE");
	range.SetValue2(COleVariant("기간(일)"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("O1"),COleVariant("O2"));
	range.SetMergeCells((COleVariant)"TRUE");
	range.SetValue2(COleVariant("설정일"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));	
	range = sheet.GetRange(COleVariant("P1"),COleVariant("P2"));
	range.SetMergeCells((COleVariant)"TRUE");
	range.SetValue2(COleVariant("주식편입비율"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));
	range = sheet.GetRange(COleVariant("Q1"),COleVariant("Q2"));
	range.SetMergeCells((COleVariant)"TRUE");
	range.SetValue2(COleVariant("투자등급명"));
	range.SetHorizontalAlignment(COleVariant((short)-4108));

	range = sheet.GetRange(COleVariant("A1"), COleVariant("Q2"));
	font = range.GetFont();
	font.SetBold(covTrue);
	interior = range.GetInterior();
    interior.SetColorIndex(COleVariant((short)36));
	range.SetVerticalAlignment(COleVariant((short)-4108));   //xlVAlignCenter = -4108

	//Fill A3:R% with an array of values (First & Last Names).
	size = m_pRemainGrid->GetRowCount();

	CString code, name, curr, day, week, month, month3, month6, year, year3, year_first, sum, gubn, period, setting, rate, grade;

	if(size > 0)
	{
		CString idx;
		for (ii =0; ii < size; ii++)
		{
			code = m_pRemainGrid->GetItemText(ii, 0);
			name = m_pRemainGrid->GetItemText(ii, 1);
			curr = m_pRemainGrid->GetItemText(ii, 2);
			day  = m_pRemainGrid->GetItemText(ii, 3);
			week  = m_pRemainGrid->GetItemText(ii, 4);
			month  = m_pRemainGrid->GetItemText(ii, 5);
			month3  = m_pRemainGrid->GetItemText(ii, 6);
			month6  = m_pRemainGrid->GetItemText(ii, 7);
			year  = m_pRemainGrid->GetItemText(ii, 8);
			year3  = m_pRemainGrid->GetItemText(ii, 9);
			year_first  = m_pRemainGrid->GetItemText(ii, 10);
			sum  = m_pRemainGrid->GetItemText(ii, 11);
			gubn  = m_pRemainGrid->GetItemText(ii, 13);
			period  = m_pRemainGrid->GetItemText(ii, 14);
			setting  = m_pRemainGrid->GetItemText(ii, 15);
			rate  = m_pRemainGrid->GetItemText(ii, 16);
			grade  = m_pRemainGrid->GetItemText(ii, 17);
			idx.Format("%d", ii+3);

			range = sheet.GetRange(COleVariant("A"+idx),COleVariant("A"+idx)); range.SetValue2(COleVariant(code));
			range = sheet.GetRange(COleVariant("B"+idx),COleVariant("B"+idx)); range.SetValue2(COleVariant(name));
			range = sheet.GetRange(COleVariant("C"+idx),COleVariant("C"+idx)); range.SetValue2(COleVariant(curr));
			range = sheet.GetRange(COleVariant("D"+idx),COleVariant("D"+idx)); range.SetValue2(COleVariant(day));
			range = sheet.GetRange(COleVariant("E"+idx),COleVariant("E"+idx)); range.SetValue2(COleVariant(week));
			range = sheet.GetRange(COleVariant("F"+idx),COleVariant("F"+idx)); range.SetValue2(COleVariant(month));
			range = sheet.GetRange(COleVariant("G"+idx),COleVariant("G"+idx)); range.SetValue2(COleVariant(month3));
			range = sheet.GetRange(COleVariant("H"+idx),COleVariant("H"+idx)); range.SetValue2(COleVariant(month6));
			range = sheet.GetRange(COleVariant("I"+idx),COleVariant("I"+idx)); range.SetValue2(COleVariant(year));
			range = sheet.GetRange(COleVariant("J"+idx),COleVariant("J"+idx)); range.SetValue2(COleVariant(year3));
			range = sheet.GetRange(COleVariant("K"+idx),COleVariant("K"+idx)); range.SetValue2(COleVariant(year_first));
			range = sheet.GetRange(COleVariant("L"+idx),COleVariant("L"+idx)); range.SetValue2(COleVariant(sum));
			range = sheet.GetRange(COleVariant("M"+idx),COleVariant("M"+idx)); range.SetValue2(COleVariant(gubn));
			range = sheet.GetRange(COleVariant("N"+idx),COleVariant("N"+idx)); range.SetValue2(COleVariant(period));
			range = sheet.GetRange(COleVariant("O"+idx),COleVariant("O"+idx)); range.SetValue2(COleVariant(setting));
			range = sheet.GetRange(COleVariant("P"+idx),COleVariant("P"+idx)); range.SetValue2(COleVariant(rate));
			range = sheet.GetRange(COleVariant("Q"+idx),COleVariant("Q"+idx)); range.SetValue2(COleVariant(grade));
		}
		range = sheet.GetRange(COleVariant("A1"), COleVariant("Q"+idx));
		cols = range.GetEntireColumn();
		cols.AutoFit();
	}

	borders = range.GetBorders();
    borders.SetWeight(COleVariant((short)2));   //xlThin = 2
	
	Border bottomborder;
	bottomborder = borders.GetItem((long)9);
	bottomborder.SetLineStyle(COleVariant((short)-4119));   //xlDouble = -4119
	bottomborder.SetWeight(COleVariant((short)4));       //xlThick = 4
	
	app.SetVisible(TRUE);
    app.SetUserControl(TRUE);
}

void CMainWnd::testSaveFile3(CString strSec, CString strDat)
{
	// 	if(m_user.Compare("q123356"))	return;	//2014.10.10 KSJ 관심종목 저장하는 부분
	
	CString strPath(_T("")), titletime;
	strPath.Format("%s\\6110Log.dat", m_root);
	
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	
	CString strCurTime;
	strCurTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	
	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 	
	
	titletime.Format("[%s][%s][%s]\n", strCurTime, strSec, strDat);
	
	file.WriteString(titletime);
	file.Close();	
}