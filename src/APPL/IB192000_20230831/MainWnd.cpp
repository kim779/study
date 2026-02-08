// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB192000.h"
#include "MainWnd.h"
#include "math.h"

#include "../../h/axisfire.h"
#include "../../control/fx_misc/misctype.h"

#define IDC_EXECELGRID  3345
#define ID_CTRL_SEL		3346
#define ID_CTRL_SEND	3347
#define IDC_PANEL1		3348
#define TID_CHARTDRAW	3349
#define TID_DATACHANGE	3350
#define	CURR_COLOR	// 현재가 대비색상

#define TK_SISE		123
#define randomize() srand((unsigned)time(NULL))



#define _SAFEDESTROY(window) \
	if (window) { \
	delete window; \
			window = NULL; }


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int	colRCODE = 0, colCODE = 1, colOPEN = 2, colHIGH = 18, colLOW = 19, colPCURR = 20, colPOPEN = 3, colPHIGH = 4, colPLOW = 5;
const int	colNAME = 6, colCURR = 7, colDIFF = 8, colRATE = 9, colASK = 10, colBID = 11, colPREVOL = 12, colVOL = 13, colDAYPRC = 14, colDAYVOL = 15;
const int	colUP1  = 16, colUP2 = 17, colSCURR = 21, colSVOL = 22;
const int	scrollW = ::GetSystemMetrics(SM_CYHSCROLL);

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd* pParent)
{
	m_pWizard    = pParent;
	m_pFont		 = NULL;
	m_pLine1Pen	 = NULL;
	m_pLine2Pen	 = NULL;
	m_pDeliPen   = NULL;
	m_pGridLine	 = NULL;
	m_BackColor  = GetIndexColor(66);
	m_clrChartBack = RGB(255,255,255);
	
	m_pLine1Pen = RequestPen(RGB(255,0,0), 1, PS_SOLID);
	m_pLine2Pen = RequestPen(RGB(0,255,0), 2, PS_SOLID);
	m_pDeliPen	= RequestPen(RGB(0,0,255), 1, PS_DOT);
	m_pGridLine = RequestPen(GetIndexColor(75), 1, PS_SOLID);
	m_fontsize	 = 9;
	m_gridW		 = 0;
	m_colC		 = 5;
	m_rowC		 = 41;
	m_grid		 = NULL;
	m_pBtnSel	 = NULL;
	m_pBtnSend	 = NULL;

	
	m_bThreadRTM = false;
	m_pThreadRTM = NULL;
	m_hEventRTM = NULL;
	
	m_maxidx = 0;
	m_nOldRow = -1;
	m_bfchg = false;
	m_bfhog = false;
	m_bChartChange = false;
	m_bDataChange = false;
	m_bCheck1 = true;
	m_bCheck2 = true;
	m_bViewChart = true;
	m_lastIChg = -10000;
	m_lastIVol = -10000;
	m_stime = "";
}

CMainWnd::~CMainWnd()
{
	//if (m_ExcelGrid) delete m_ExcelGrid;
	RemoveCPosAll();

	//_SAFEDESTROY(m_pBtnSel);
	//_SAFEDESTROY(m_pBtnSend);
	//_SAFEDESTROY(m_pLine1Pen);
	//_SAFEDESTROY(m_pLine2Pen);

}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_CTRL_SEL, OnBtnSel)
	ON_BN_CLICKED(ID_CTRL_SEND, OnBtnSend)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

void CMainWnd::Init()
{
	m_fontsize	 = 9;
	m_gridW		 = 0;
	m_colC		 = 5;
	m_rowC		 = 41;
	CString m_root = Variant(homeCC);	
	CRect rc; GetClientRect(rc);
	rc.InflateRect(0,-44);
	Variant(titleCC, "시가총액 상위 40종목 분석");
	//m_pFont = GetAxFont("굴림체", 9, false, 0);
	
	CRect	clientRC; 
	GetClientRect(&clientRC); 

	LOGFONT lf; 
	lf.lfHeight = m_fontsize * 10;

	//m_font->GetLogFont(&lf); 
	
	CRect cRc;
	m_pBtnSel = std::make_unique<class CfxImgButton>();
	m_pBtnSel->Create("\n가격지정", cRc, this, ID_CTRL_SEL, FALSE, TRUE);
	m_pBtnSel->SetWindowText("가격지정");
	m_pBtnSel->SetFont(m_font);
	m_pBtnSel->SetDisableDraw(FALSE);
	m_pBtnSel->SetTextColor(GetIndexColor(69));
	CString path;
	path.Format("%s\\%s\\", m_root, "image");
	
	HBITMAP hbmp	= RequestBitmap(path + "4btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= RequestBitmap(path + "4btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv	= RequestBitmap(path + "4btn_en.bmp")->operator HBITMAP();
	m_pBtnSel->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);



	m_pBtnSend = std::make_unique <class CfxImgButton>();
	m_pBtnSend->Create("\n데이터", cRc, this, ID_CTRL_SEND, FALSE, TRUE);
	m_pBtnSend->SetWindowText("데이터");
	m_pBtnSend->SetFont(m_font);
	m_pBtnSend->SetDisableDraw(FALSE);
	m_pBtnSend->SetTextColor(GetIndexColor(69));
	m_pBtnSend->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);

	
	
	cRc.left = rc.left + 30;
	cRc.right = cRc.left + 60;

	initArea();
	
	m_grid = std::make_unique <CfxGrid>(&lf);
	m_grid->Create(clientRC, this, IDC_EXECELGRID, GVSC_VERT);
	

	loadFieldInfo();
	
	initialGrid();
	
	initRTM();
	
	//*/
	/*
	CString trData; trData.Format("1301%c%s\t1021\t2023\t", 0x7f, "005930");
	SendTR("POOPPOOP", trData, TK_SISE);
	*/
	
	QueryExcelSise();

// 	SetTimer(TID_CHARTDRAW, 1000, NULL);
// 	SetTimer(TID_DATACHANGE, 800, NULL);
	SetTimer(100,800,NULL);
}

int DefStrToInt(CString str, int def_i)
{
	if (str.IsEmpty())
		return def_i;
	else
	{
		str.Remove(' ');
		str.Remove(',');
		return atoi(str);
	}	
}

int DefStrToAbsInt(CString str, int def_i)
{
	if (str.IsEmpty())
		return def_i;
	else
	{
		str.Remove(' ');
		str.Remove(',');
		return abs(atoi(str));
	}	
}

double DefStrToDouble(CString str, int def_i)
{
	if (str.IsEmpty())
		return def_i;
	else
		return atof(str);
}

double DefStrToAbsDouble(CString str, int def_i)
{
	if (str.IsEmpty())
		return def_i;
	else
	{
		str.Remove(' ');
		double rt = atof(str);
		if (rt<0) rt *= -1;
		return rt;
	}	
}

void CMainWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc, top_rc;
	dc.SelectObject(m_font);
	GetClientRect(&rc);
	top_rc = rc;
	//top_rc.bottom -= 200;
	top_rc.bottom = 50;
	top_rc.left = 942;
	if (m_BackColor)
	{	
		dc.FillSolidRect(top_rc, m_BackColor);
		top_rc.left = 0; top_rc.right = 942; top_rc.top = 0; top_rc.bottom = 2;
		dc.FillSolidRect(top_rc, m_BackColor);
		top_rc.left = 0; top_rc.right = 942; top_rc.top = 41; top_rc.bottom = 45;
		dc.FillSolidRect(top_rc, m_BackColor);
		top_rc.left = 0; top_rc.right = 2; top_rc.top = 0; top_rc.bottom = 45;
		dc.FillSolidRect(top_rc, m_BackColor);
	}
	
	dc.TextOut(1008, 25, m_stime);
	
	DrawUpData(&dc);

	chartRect = rc;
	chartRect.top = 0;
	chartRect.bottom = 200;
	drawChartRect = CRect(0, rc.bottom-200, chartRect.right , rc.bottom);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, chartRect.Width(), chartRect.Height());
	
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pbitmapOld = memDC.SelectObject(&bitmap);
	
	memDC.FillSolidRect(chartRect, RGB(255, 255, 255));
	
	DrawChart(&memDC, chartRect);
		
	dc.BitBlt(0, rc.bottom-200, chartRect.Width() , chartRect.Height(), &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(pbitmapOld);
	bitmap.DeleteObject();
	memDC.DeleteDC();
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
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

CFont* CMainWnd::GetAxFont(CString strName, int nPt, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name = (LPSTR)(LPCTSTR)strName;
	fontR.point = nPt;
	fontR.italic = bItalic;
	fontR.bold = nBold;
	
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}

void CMainWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	CRect rc; GetClientRect(rc);
	
	if (m_grid)
	{
		
		rc.top += 44;
		if (m_bViewChart)
			rc.bottom -= 200;

		m_grid->MoveWindow(rc);
	}
	CRect cRc;
	if (m_pBtnSel)
	{		
		cRc.top = 3; cRc.left = rc.right-60; cRc.bottom = 21; cRc.right = rc.right-4;	
		m_pBtnSel->MoveWindow(cRc);
	}
	cRc.OffsetRect(-60,0);
	if (m_pBtnSend)
	{
		//cRc.top = 3; cRc.left = cRc.left-60; cRc.bottom = 21; cRc.right = cRc.left-3;	
		m_pBtnSend->MoveWindow(cRc);
	}
	GetClientRect(&rc);
	m_rc_Check1 = CRect(rc.right-300,rc.bottom-198,rc.right-200, rc.bottom-198+15);
	m_rc_Check2 = CRect(rc.right-200,rc.bottom-198,rc.right-100, rc.bottom-198+15);
	// TODO: Add your message handler code here
	
}

long CMainWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		{
		struct _mod mod {};
			const int key = HIBYTE(LOWORD(wParam));
			const int len = HIWORD(wParam);
			CopyMemory(&mod, (void*)lParam, sizeof(struct _mod));
			//AfxMessageBox(mod.grid[1].code);
			//CString strData;
			//strData = (char*)lParam;
			int ii=0;
			for(int i=0; i<40; i++)
			{
				ii = i+1;
				m_grid->SetItemText(ii, colRCODE, "A" + CString(mod.grid[i].code, sizeof(mod.grid[i].code)));
//				m_grid->SetItemText(ii, colRCODE, CString(mod.grid[i].code, sizeof(mod.grid[i].code)));
				m_grid->SetItemText(ii, colNAME, CString(mod.grid[i].hnam, sizeof(mod.grid[i].hnam)));
				m_grid->SetItemText(ii, colCURR, CString(mod.grid[i].curr, sizeof(mod.grid[i].curr)));
				m_grid->SetItemText(ii, colDIFF, CString(mod.grid[i].diff, sizeof(mod.grid[i].diff)));
				m_grid->SetItemText(ii, colRATE, CString(mod.grid[i].drat, sizeof(mod.grid[i].drat)));
				m_grid->SetItemText(ii, colASK, CString(mod.grid[i].ask1, sizeof(mod.grid[i].ask1)));
				m_grid->SetItemText(ii, colBID, CString(mod.grid[i].bid1, sizeof(mod.grid[i].bid1)));
				m_grid->SetItemText(ii, colPREVOL, CString(mod.grid[i].bvol, sizeof(mod.grid[i].bvol)));
				m_grid->SetItemText(ii, colVOL, CString(mod.grid[i].gvol, sizeof(mod.grid[i].gvol)));
				m_grid->SetItemText(ii, colHIGH, CString(mod.grid[i].higp, sizeof(mod.grid[i].higp)));
				m_grid->SetItemText(ii, colLOW, CString(mod.grid[i].lowp, sizeof(mod.grid[i].lowp)));
				m_grid->SetItemText(ii, colPCURR, CString(mod.grid[i].stdp, sizeof(mod.grid[i].stdp)));

				CalcRowDiff(i);
			}
			m_choi_curr = CString(mod.f1item.fprc, sizeof(mod.f1item.fprc));
			m_choi_diff = CString(mod.f1item.fdif, sizeof(mod.f1item.fdif));
			m_medo_jan  = CString(mod.f1av, sizeof(mod.f1av));
			m_mesu_jan  = CString(mod.f1bv, sizeof(mod.f1bv));
			m_medo_geon = CString(mod.f1ac, sizeof(mod.f1ac));
			m_mesu_geon = CString(mod.f1bc, sizeof(mod.f1bc));
			m_meme_jan  = CString(mod.f1pv, sizeof(mod.f1pv));
			m_meme_geon = CString(mod.f1pa, sizeof(mod.f1pa));
			m_cha_curr  = CString(mod.f2item.fprc, sizeof(mod.f2item.fprc));
			m_cha_diff  = CString(mod.f2item.fdif, sizeof(mod.f2item.fdif));
			m_choi_code = CString(mod.f1item.fcod, sizeof(mod.f1item.fcod));
			m_cha_code  = CString(mod.f2item.fcod, sizeof(mod.f2item.fcod));
			m_migyul    = CString(mod.f2item.fopv, sizeof(mod.f2item.fopv));
			m_migyul.Remove(0); //미결데이터에 널값이 내려와서 콤마 찍기는 룰에 위배되어 따로 널 삭제 처리
			
			
			m_medo_jan  = AddComma(m_medo_jan  );
			m_mesu_jan  = AddComma(m_mesu_jan  );
			m_medo_geon = AddComma(m_medo_geon );
			m_mesu_geon = AddComma(m_mesu_geon );
			m_meme_jan  = AddComma(m_meme_jan  );
			m_meme_geon = AddComma(m_meme_geon );
			m_migyul    = AddComma(m_migyul    );
// 			for(i=1; i<=40;i++)
// 				CalcRowDiff(i);
			
			//InvalidateRect(m_rc_dataRect);
			
		}
		break;

	case DLL_TRIGGER:
		//ParsingTrigger((char*)lParam);
		break;

	case DLL_DOMINO:
		//ParsingDomino((char*)lParam);
		break;

	case DLL_SETPAL:
		//SetPallete();
		break;

	case DLL_GUIDE:
		//SetGuide((char*)lParam);
		return 1;

// 	case DLL_ALERT:
// 		{
// 			if (!m_bThreadRTM)
// 				break;
// 
// 			char *pBuf = (char*)lParam;
// 			int len = strlen(pBuf);
// 			
// 			char* pBytes = new char[len+1];
// 			memset(pBytes, 0x00, len+1);
// 			memcpy(pBytes, pBuf, len);
// 			
// 			m_syncRTM.Lock();
// 			m_arBytesRTM.Add(pBytes);
// 			m_syncRTM.Unlock();
// 			SetEvent(m_hEventRTM);
// 		}
// 		break;
	case DLL_ALERTx:
		{
			if (!m_bThreadRTM)
				break;
			
			struct _alertR* alertR;
			
			alertR = (struct _alertR*)lParam;
			
			//m_syncRTM.Lock();
			//m_arBytesXRTM.Add(alertR);
			//m_syncRTM.Unlock();
			//SetEvent(m_hEventRTM);
			doXRTM(alertR);
		}
		break;
	case DLL_SETFONT:break;
	case DLL_SETFONTx:
		//ChangeFont( HIWORD(wParam), CString((char*)lParam) );
		break;
	}
	return 0;
}

LRESULT CMainWnd::SendTR(CString strCode, CString strData, int iKey, int iStat)
{

	std::unique_ptr<char[]> pcDataBuffer = std::make_unique<char[]>(L_userTH + strData.GetLength());
	ZeroMemory(pcDataBuffer.get(), sizeof(pcDataBuffer));
	struct	_userTH* puserTH;
	puserTH = (struct _userTH *)pcDataBuffer.get();
	
	strcpy(puserTH->trc, strCode.operator LPCTSTR());
	puserTH->key = iKey;
	puserTH->stat = iStat;
	
	CopyMemory(&pcDataBuffer.get()[L_userTH], strData.operator LPCTSTR(), strData.GetLength());
	
	const LRESULT lResult = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM) pcDataBuffer.get());
	
	return lResult;
}

void CMainWnd::ParseAlert(CString pData)
{
	int	idx=0;
	CMapStringToString fms;
	CString	code, symbol, entry, datB, total_str;
	
	datB = pData;
	total_str = datB;
	datB = SplitString(total_str, "\n");
	
	while(!datB.IsEmpty())
	{
		if ((idx = datB.Find('\t')) == -1)
			return;
		code = datB.Left(idx++);
		datB = datB.Mid(idx);
		if (code.CompareNoCase("S0000") == 0)
			return;
		while (!datB.IsEmpty())
		{
			idx = datB.Find('\t');
			if (idx == -1) break;
			
			symbol = datB.Left(idx++);
			datB = datB.Mid(idx);
			idx = datB.Find('\t');
			
			if (idx == -1)
			{
				entry = datB;	
				datB = _T("");
			}
			else
			{
				entry = datB.Left(idx++); 
				datB = datB.Mid(idx);
			}
			
			if(entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0")
			{
				entry = " ";
			}
			
			fms.SetAt(symbol, entry);
		}
		if (code!="A005930") return;
		CString strFms;
		/*
		if (fms.Lookup("023", strFms))  //현재가
		{
			m_ExcelGrid->QuickSetText(1,0, strFms);
		}
		if (fms.Lookup("024", strFms))  //전일대비
		{
			m_ExcelGrid->QuickSetText(2,0, strFms);
		}
		if (fms.Lookup("033", strFms))  //등락율
		{
			m_ExcelGrid->QuickSetText(3,0, strFms);
		}
		if (fms.Lookup("025", strFms))  //매도호가
		{
			m_ExcelGrid->QuickSetText(4,0, strFms);
		}
		if (fms.Lookup("026", strFms))  //매수호가
		{
			m_ExcelGrid->QuickSetText(5,0, strFms);
		}
		if (fms.Lookup("027", strFms))  //누적거래량
		{
			m_ExcelGrid->QuickSetText(7,0, strFms);
		}
		if (fms.Lookup("030", strFms))  //고가
		{
			m_ExcelGrid->QuickSetText(12,0, strFms);
		}
		if (fms.Lookup("031", strFms))  //저가
		{
			m_ExcelGrid->QuickSetText(13,0, strFms);
		}


		m_ExcelGrid->QuickSetText(0,0, code);
		m_ExcelGrid->RedrawRow(0);
		*/
		datB = SplitString(total_str, "\n");
	}
}

BOOL CMainWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_font = GetAxFont(idTOstring(IDS_DEFAULTFONT), m_fontsize, false, 0);
	m_fontB= GetAxFont(idTOstring(IDS_DEFAULTFONT), m_fontsize, true, 0);

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

CString CMainWnd::SplitString(CString &strData, CString strToken)
{
	CString sResult = strData;
	if(strToken == "") return sResult;
	const int iFind = strData.Find(strToken); 
	if(iFind > -1 ) 
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}
	
	
	return sResult;
}

void CMainWnd::initialGrid()
{

	LOGFONT lf{}, lfB{};
	m_font->GetLogFont(&lf); lf.lfHeight = m_fontsize * 10;
	m_fontB->GetLogFont(&lfB); lfB.lfHeight = m_fontsize * 10;
	
	m_grid->Initial(m_rowC, m_colC, 1, 0);
	
	GVITEM	gvitem{};
	_gridHdr gridHdr{};
	
	// column header setting
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);
		
		gvitem.row   = 0;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;
		gvitem.format= GVFM_HEADER;
		gvitem.fgcol = GetIndexColor(76);
		gvitem.bkcol = GetIndexColor(74);
		
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		
		gvitem.text  = idTOstring(gridHdr.stid);
		gvitem.attr  = gridHdr.attr;
		
		m_grid->SetItem(&gvitem);
		m_grid->SetColumnWidth(ii, gridHdr.width);
	}
	
	
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);
		
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;
		
		gvitem.format= gridHdr.format;
		gvitem.symbol= gridHdr.symbol; 
		gvitem.attr  = gridHdr.attrx;
		gvitem.fgcol = GetIndexColor(69);
		gvitem.bkcol = GetIndexColor(68);
		
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		//if (ii == colCURR) CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
		
		m_grid->SetColumnItems(ii, &gvitem);
	}
	
	m_grid->SetBKSelColor(GetIndexColor(78));
	m_grid->SetRowColResize(FALSE, TRUE);
	m_grid->SetKeepingColumn(1);		// keep column count
	m_grid->SetGridFocusLine(FALSE, TRUE);
	m_grid->SetGridColor(GetIndexColor(65));
	m_grid->SetConditionColor(GetIndexColor(94), GetIndexColor(95), GetIndexColor(69));
	m_grid->SetStepColor(1, GetIndexColor(68), GetIndexColor(77));
	m_grid->Adjust();
	

}

void CMainWnd::loadFieldInfo()
{
	const struct	_gridHdr gridHdr[] = {
		{IDS_GH_RCOD,	0,	GVAT_HIDDEN,		"1021",	GVFM_LEFT,	0,				1},	// 0
		{IDS_GH_CODE,	0,	GVAT_HIDDEN,		"1301",	GVFM_LEFT,	0,				2},	// 1
		{IDS_GH_OPEN,	0,	GVAT_HIDDEN,		"2029",	GVFM_RIGHT,	0,				3},	// 2
		{IDS_GH_PREOPEN,0,	GVAT_HIDDEN,		"2931",	GVFM_RIGHT,	0,				4},	// 6
		{IDS_GH_PREHIGH,0,	GVAT_HIDDEN,		"2932",	GVFM_RIGHT,	0,				5},	// 7
		{IDS_GH_PRELOW,	0,	GVAT_HIDDEN,		"2933",	GVFM_RIGHT,	0,				6},	// 8
		{IDS_GH_NAME,	70,	GVAT_SORTSTR,		"1022",	GVFM_LEFT,	0,				7},	// 9
#ifdef	CURR_COLOR
		{IDS_GH_CURR,	60,	GVAT_SORTABS,		"2023",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	8},	// 10
#else
		{IDS_GH_CURR,	60,	GVAT_SORTABS,		"2023",	GVFM_RIGHT,	GVAT_COMMA,			8},	// 10
#endif
		{IDS_GH_DIFF,	60,	GVAT_SORTVAL,		"2024",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST,	9},	// 11
		{IDS_GH_RATE,	46,	GVAT_SORTVAL,		"2033",	GVFM_RIGHT,	GVAT_CONDITION,			10},	// 12
		{IDS_GH_ASK,	60,	GVAT_SORTABS,		"2025",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	11},	// 13
		{IDS_GH_BID,	60,	GVAT_SORTABS,		"2026",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	12},	// 14
		{IDS_GH_PREVOL,	75,	GVAT_SORTABS,		"2927",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	13},	// 16
		{IDS_GH_VOL,	75,	GVAT_SORTABS,		"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	14},	// 16
		{IDS_GH_DAYPRC,	60,	GVAT_SORTVAL,		"DAYPRC",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,				15},	// 3
		{IDS_GH_DAYVOL,	63,	GVAT_SORTVAL,		"DAYVOL",	GVFM_RIGHT,	GVAT_COMMA,				16},	// 3
		{IDS_GH_UP1,	60,	GVAT_SORTVAL,		"UP1",	GVFM_RIGHT,	GVAT_COMMA,					17},	// 3
		{IDS_GH_UP2,	60,	GVAT_SORTVAL,		"UP2",	GVFM_RIGHT,	GVAT_COMMA,				18},	// 3
		{IDS_GH_HIGH,	60,	GVAT_SORTABS,		"2030",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,				19},	// 3
		{IDS_GH_LOW,	60,	GVAT_SORTABS,		"2031",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,				20},	// 4
		{IDS_GH_PRECURR,60,	GVAT_SORTABS,		"2934",	GVFM_RIGHT,	GVAT_COMMA,				21},	// 5
		{IDS_GH_SCURR  ,60,	GVAT_SORTABS,		"SCURR",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,				22},	// 5
		{IDS_GH_SVOL   ,60,	GVAT_SORTABS,		"SVOL",	GVFM_RIGHT,	GVAT_COMMA,				23},	// 5
	};	
	
	m_gridHdrX.RemoveAll();
	const int gridHdrC = sizeof(gridHdr) / sizeof(_gridHdr);
	
	m_gridW = 0;
	for (int ii = 0; ii < gridHdrC; ii++)
	{
		//if (gridHdr[ii].fidx >= 1)
		{
			m_gridHdrX.Add(gridHdr[ii]);
			
			if (!(gridHdr[ii].attr & GVAT_HIDDEN))
				m_gridW += gridHdr[ii].width;
		}
	}
	
	if (m_gridW > 0) m_gridW += scrollW;
	
	m_colC = m_gridHdrX.GetSize();
}

CString CMainWnd::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

UINT ParsingRTM(LPVOID lpvoid)
{
	CMainWnd* pMain = (CMainWnd *)lpvoid;
	if (pMain == NULL)
		return FALSE;
	
	char* pBytes = NULL;
	struct _alertR* alertR=NULL;
	
	while (pMain->m_bThreadRTM)
	{
#ifdef USE_DLL_ALERTx
		if (pMain->m_arBytesXRTM.GetUpperBound() < 0)
		{
			pMain->WaitRTM();
			continue;
		}
		pMain->m_syncRTM.Lock();
		alertR = pMain->m_arBytesXRTM.GetAt(0);
		pMain->m_arBytesXRTM.RemoveAt(0);
		pMain->m_syncRTM.Unlock();
		
		pMain->doXRTM(alertR);
		//delete pBytes;
		pBytes = NULL;
#else
		if (pMain->m_arBytesRTM.GetUpperBound() < 0)
		{
			pMain->WaitRTM();
			continue;
		}
		pMain->m_syncRTM.Lock();
		pBytes = pMain->m_arBytesRTM.GetAt(0);
		pMain->m_arBytesRTM.RemoveAt(0);
		pMain->m_syncRTM.Unlock();
		
		pMain->doRTM(pBytes);
		delete pBytes;
		pBytes = NULL;
#endif	
	}
	return 0;
}

void CMainWnd::doXRTM(struct _alertR* alertR)
{
	//m_sync.Lock();
	if (m_grid != NULL) parsingAlert(alertR);
	//m_sync.Unlock();
}

bool CMainWnd::WaitRTM()
{
	if (!m_pThreadRTM)
		return false;
	const DWORD res = WaitForSingleObject(m_hEventRTM, 3000);
	switch (res)
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	case WAIT_TIMEOUT:
		return false;
	}
	return true;
}

void CMainWnd::doRTM(char *pBytes)
{
	m_sync.Lock();
	if (m_grid != NULL) parsingAlert(pBytes);
	m_sync.Unlock();
}

void CMainWnd::initRTM()
{
	if (m_bThreadRTM)
		return;
	
	m_bThreadRTM = true;
	m_pThreadRTM = AfxBeginThread((AFX_THREADPROC) ParsingRTM, (LPVOID) this, THREAD_PRIORITY_NORMAL);
	m_hEventRTM = ::CreateEvent(NULL, FALSE, FALSE, NULL);	
}

void CMainWnd::stopRTM()
{
	if (!m_bThreadRTM)
		return;
	
	m_bThreadRTM = false;
	SetEvent(m_hEventRTM);
	m_pThreadRTM->PostThreadMessage(WM_QUIT, 0, 0);
	switch (WaitForSingleObject(m_pThreadRTM->m_hThread, 1000))
	{
	case WAIT_OBJECT_0:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
		break;
	case WAIT_TIMEOUT:
		TerminateThread(m_pThreadRTM->m_hThread, 0);
		break;
	}
	removeRTM();
}

void CMainWnd::removeRTM()
{
	m_syncRTM.Lock();
	//char* pBytes = NULL;
	//for (int ii = m_arBytesRTM.GetUpperBound(); ii >= 0; ii--)
	//{
	//	pBytes = m_arBytesRTM.GetAt(ii);
	//	delete pBytes;	
	//	pBytes = NULL;
	//}
	//m_arBytesRTM.RemoveAll();
	m_syncRTM.Unlock();
}

void CMainWnd::OnDestroy() 
{
	KillTimer(TID_CHARTDRAW);
	KillTimer(TID_DATACHANGE);
	stopRTM();

	//if (m_grid)
	//{
	//	delete m_grid; m_grid = NULL;
	//}
	//
	
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CMainWnd::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow(); 
	CWnd::OnClose();
}

void CMainWnd::SplitStringArray(CString &strData, CString strToken,CStringArray &ar) //
{
	//CStringArray arStr;
	
	CString sResult = strData;
	if(strToken == "") return;
	int iFind = strData.Find(strToken); 
	
	if(iFind < 0)
	{
		ar.Add(sResult);
		return;
	}
	
	while(iFind > -1)
	{
		if(iFind > -1 ) 
		{
			sResult = strData.Left(iFind);
			strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
			
			ar.Add(sResult);
			
			iFind = strData.Find(strToken);
		}
	}
	
	sResult = strData;
	
	ar.Add(sResult);
}

void CMainWnd::parsingAlert(CString datB)
{
	int	idx=0;
	int	xrow=0;
	CMapStringToString fms;
	CString	code, symbol, entry, entry2;
	//AfxMessageBox(datB);

	//TRACE("%s\n",datB);
	if ((idx = datB.Find('\t')) == -1)
		return;

	CStringArray arTokens;

	SplitStringArray(datB,"\t",arTokens);
	
//  	code = datB.Left(idx++);
//  	datB = datB.Mid(idx);
	code = arTokens.GetAt(0);
	if ((code == m_choi_code)||(code == m_cha_code))
	{
		
		bool m_bchoi = false;
		m_bchoi = (code == m_choi_code);
		
// 		while (!datB.IsEmpty())
// 		{
// 			idx = datB.Find('\t');
// 			if (idx == -1) break;
// 			
// 			symbol = datB.Left(idx++);
// 			datB = datB.Mid(idx);
// 			
// 			idx = datB.Find('\t');
// 			if (idx == -1)
// 			{
// 				entry = datB;	datB = _T("");
// 			}
// 			else
// 			{
// 				entry = datB.Left(idx++); datB = datB.Mid(idx);
// 			}
// 			fms.SetAt(symbol, entry);
// 		}

		for(int i=1;i<arTokens.GetSize();i=i+2)
		{
			symbol = arTokens.GetAt(i);
			symbol.TrimLeft();
			
			if(symbol == "")
				break;
			
			entry = arTokens.GetAt(i+1);
			fms.SetAt(symbol,entry);
		}


		
		if (fms.Lookup("023", entry))  //선물 현재가
			if (m_bchoi)
			{
				m_choi_curr = entry;
				if (fms.Lookup("034", entry)) //체결시간
				{					
					AddChartData(entry,m_choi_curr,DefStrToAbsDouble(m_choi_curr,0),m_maxidx);
					m_maxidx++;
				}
			}
			else
				m_cha_curr = entry;

		if (fms.Lookup("024", entry))  //선물 전일대비
			if (m_bchoi)
				m_choi_diff = entry;
			else
				m_cha_diff = entry;
			

				
		if (m_bchoi)
		{
			if (fms.Lookup("040", entry)) //호가시간
			{
				if (fms.Lookup("120", entry2)) //매도수량차이
				{
					AddChartData(entry,entry2,DefStrToDouble(entry2,0),m_maxidx, true);
					m_maxidx++;
				}
			}
		}
		
		if (m_bchoi)
		{
			if (fms.Lookup("101", entry))  //선물 매도총잔량
				m_medo_jan = AddComma(entry);
			if (fms.Lookup("106", entry))  //선물 매수총잔량
				m_mesu_jan = AddComma(entry);
			if (fms.Lookup("103", entry))  //선물 매도총건수
				m_medo_geon = AddComma(entry);
			if (fms.Lookup("108", entry))  //선물 매수총건수
				m_mesu_geon = AddComma(entry);
			
			//선물 매매순잔량
			m_meme_jan.Format("%d", DefStrToAbsInt(m_mesu_jan,0)-DefStrToAbsInt(m_medo_jan,0));
			//선물 매매순건수
			m_meme_geon.Format("%d", DefStrToAbsInt(m_mesu_geon,0)-DefStrToAbsInt(m_medo_geon,0));
			m_meme_jan  = AddComma(m_meme_jan);
			m_meme_geon = AddComma(m_meme_geon);
						

		}else
		{
			if (fms.Lookup("201", entry))  //선물 매수총건수
				m_migyul = AddComma(entry);
		}
		m_bDataChange = true;
		
		//InvalidateRect(m_rc_dataRect);
		//return;
	}	
	else
	{
		if (checkRealTimeCode(code, xrow) <= 0)
			return;

		for(int i=1;i<arTokens.GetSize();i=i+2)
		{
			symbol = arTokens.GetAt(i);
			symbol.TrimLeft();

			if(symbol == "")
				break;

			entry = arTokens.GetAt(i+1);
			fms.SetAt(symbol,entry);
		}

		const int countX = m_gridHdrX.GetSize(); _gridHdr gridHdr{};
		for (int ii = 3; ii < countX; ii++)
		{
			gridHdr = m_gridHdrX.GetAt(ii);
			symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));
			if (symbol.GetLength() >= 3) symbol = symbol.Right(3);
			
			if (!fms.Lookup(symbol, entry))
				continue;
			
			
			
			if (xrow == -1) continue;
			
			if (ii == colCURR && entry.GetLength() > 0)
			{
#ifdef	CURR_COLOR
				// entry.Remove('+'); entry.Remove('-');
#else
				entry.Remove('+'); entry.Remove('-');
#endif
			}
			
			// change TOfabs -> TOf
			if (TOf(entry) == TOf(m_grid->GetItemText(xrow, ii)))
				continue;
			
			m_grid->SetItemText(xrow, ii, entry);
			
		}
		CalcRow(xrow);
	}
	
// 	while (!datB.IsEmpty())
// 	{
// 		idx = datB.Find('\t');
// 		if (idx == -1) break;
// 		
// 		symbol = datB.Left(idx++);
// 		datB = datB.Mid(idx);
// 		
// 		idx = datB.Find('\t');
// 		if (idx == -1)
// 		{
// 			entry = datB;	datB = _T("");
// 		}
// 		else
// 		{
// 			entry = datB.Left(idx++); datB = datB.Mid(idx);
// 		}
// 		fms.SetAt(symbol, entry);
// 	}
}

void CMainWnd::parsingAlert(struct _alertR* alertR)
{
	//int	idx=0;
	int	xrow=0;
	CMapStringToString fms;
	CString	code, symbol, entry, entry2;
	
 	//CString s;
 	//s.Format("XRTM : %s\n",alertR->code);
// 	OutputDebugString(s);

	DWORD* data=nullptr;
	//AfxMessageBox(datB);

//	for(int i=0;i<alertR->size;i++)
	for (int i = alertR->size - 1; i > -1; --i)
	{
		data = (DWORD*)alertR->ptr[i];

 		//s.Format("CODE : %s		CHOI_CODE : %s\n",alertR->code,m_choi_code);
 		//OutputDebugString(s);
	
		if ((alertR->code == m_choi_code)||(alertR->code == m_cha_code))
		{
			bool m_bchoi = false;
			m_bchoi = (alertR->code == m_choi_code);

			if (data[23])  //선물 현재가
			{
				if (m_bchoi)
				{
					entry = (char*)data[23];
					m_choi_curr = entry;
					if (data[34]) //체결시간
					{					
						entry = (char*)data[34];
						AddChartData(entry,m_choi_curr,DefStrToAbsDouble(m_choi_curr,0),m_maxidx);
						m_maxidx++;
					}
				}
				else
					m_cha_curr = entry;
			}

			if (data[24])  //선물 전일대비
			{
				entry = (char*)data[24];

				if (m_bchoi)
					m_choi_diff = entry;
				else
					m_cha_diff = entry;
			}
				

					
			if (m_bchoi)
			{
				if (data[40]) //호가시간
				{
					if (data[120]) //매도수량차이
					{
						entry2 = (char*)data[120];
						AddChartData(entry,entry2,DefStrToDouble(entry2,0),m_maxidx, true);
						m_maxidx++;
					}
				}
			}
			
			if (m_bchoi)
			{
				if (data[101])  //선물 매도총잔량
					m_medo_jan = AddComma((char*)data[101]);
				if (data[106])  //선물 매수총잔량
					m_mesu_jan = AddComma((char*)data[106]);
				if (data[103])  //선물 매도총건수
					m_medo_geon = AddComma((char*)data[103]);
				if (data[108])  //선물 매수총건수
					m_mesu_geon = AddComma((char*)data[108]);
				
				//선물 매매순잔량
				m_meme_jan.Format("%d", DefStrToAbsInt(m_mesu_jan,0)-DefStrToAbsInt(m_medo_jan,0));
				//선물 매매순건수
				m_meme_geon.Format("%d", DefStrToAbsInt(m_mesu_geon,0)-DefStrToAbsInt(m_medo_geon,0));
				m_meme_jan  = AddComma(m_meme_jan);
				m_meme_geon = AddComma(m_meme_geon);
							

			}else
			{
				if (data[201])  //선물 매수총건수
					m_migyul = AddComma((char*)data[201]);
			}
			m_bDataChange = true;
			
			InvalidateRect(m_rc_dataRect);
			return;
		}	

		if (checkRealTimeCode(alertR->code, xrow) <= 0)
			return;

		const int countX = m_gridHdrX.GetSize(); _gridHdr gridHdr{};
		for (int ii = 3; ii < countX; ii++)
		{
			gridHdr = m_gridHdrX.GetAt(ii);
			symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));
			if (symbol.GetLength() >= 3) symbol = symbol.Right(3);
			
			if (!data[atoi(symbol)])
				continue;
			
			entry = (char*)data[atoi(symbol)];

			if (xrow == -1) continue;
			
			if (ii == colCURR && entry.GetLength() > 0)
			{
	#ifdef	CURR_COLOR
				// entry.Remove('+'); entry.Remove('-');
	#else
				entry.Remove('+'); entry.Remove('-');
	#endif
			}
			
			// change TOfabs -> TOf
			if (TOf(entry) == TOf(m_grid->GetItemText(xrow, ii)))
				continue;
			
			m_grid->SetItemText(xrow, ii, entry);

		}
		CalcRow(xrow);
	}
}

int CMainWnd::checkRealTimeCode(CString code, int &xrow)
{
	CString string;
	int	count = 0;
	const int realtimeCol = 0;
	
	xrow = -1;
	
	CString strText(_T(""));
	
	for (int ii = 1; ii <= m_rowC; ii++)
	{
		strText = m_grid->GetItemText(ii, realtimeCol);
		
		if (!strText.IsEmpty() && strText == code)
		{
			xrow = ii; count++;
			break;
		}
	}
	

	// escdream check A005930 예외처리
	if (code == "A005930") 
		xrow = 1;
	
	return xrow;
}

double CMainWnd::TOf(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;
	
	dval = atof(string);
	return dval;
}

double CMainWnd::TOfabs(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;
	
	dval = fabs(atof(string));
	return dval;
}

void CMainWnd::QueryExcelSise()
{
		
	CString strData;
	struct _mid mid;
	FillMemory(&mid, sizeof(struct _mid), ' ');
	memset(&mid, ' ', sizeof(mid));
	strncpy(mid.fcod[0], "AAAAAAAA", 8);
	strData = CString((char *)&mid, sizeof(struct _mid));
	
	SendTR("pibo1920", strData, TK_SISE);
}

void CMainWnd::OnBtnSel()
{
	CString t_curr, t_vol;
	for(int i=1; i<=40; i++ )
	{
		
		t_curr = m_grid->GetItemText(i, colCURR);
		t_vol  = m_grid->GetItemText(i, colVOL);
		
		/*
		//개발때처리하기위해 
		t_curr = m_grid->GetItemText(i, colPCURR);
		t_vol  = m_grid->GetItemText(i, colPREVOL);
		*/
		m_grid->SetItemText(i, colSCURR, t_curr);
		m_grid->SetItemText(i, colSVOL, t_vol);
		CalcRow(i);	
		
	}
	//InvalidateRect(m_rc_dataRect);
	m_bDataChange = true;
	CTime time;
	time = CTime::GetCurrentTime();
	CString str;
	str.Format("%02d:%02d:%02d", time.GetHour(), time.GetMinute(), time.GetSecond());
	m_stime = str;
	AddChartData(str,"",-1,m_maxidx);
	m_maxidx++;
	Invalidate();
}



void CMainWnd::CalcRow(int row)
{
	CString t_curr, t_vol, c_curr, c_vol, c_diff;
	int it_curr=0, it_vol=0, ic_curr=0, ic_vol=0, ic_diff=0;
	c_curr = m_grid->GetItemText(row, colCURR);		ic_curr = DefStrToAbsInt(c_curr, 0);	
	c_vol  = m_grid->GetItemText(row, colVOL);		ic_vol = DefStrToAbsInt(c_vol, 0);
	t_curr = m_grid->GetItemText(row, colSCURR);	it_curr = DefStrToAbsInt(t_curr, 0);
	t_vol  = m_grid->GetItemText(row, colSVOL);		it_vol = DefStrToAbsInt(t_vol, 0);
	c_diff  = m_grid->GetItemText(row, colPCURR);		ic_diff = DefStrToAbsInt(c_diff, 0);
	
	it_curr = ic_curr-it_curr; 
	it_vol = ic_vol-it_vol;
	ic_diff = ic_curr - ic_diff;
	CString data;
	data.Format("%d",it_curr);
	if (it_curr>=0)
		data = "+"+data;
	else
		data = "-"+data;
	m_grid->SetItemText(row, colDAYPRC, data);
	data.Format("%d",it_vol);
	/*
	if (it_vol>=0)
		data = "+"+data;
	else
		data = "-"+data;
	*/	
	m_grid->SetItemText(row, colDAYVOL, data);
	if (it_curr>=0)
		m_grid->SetItemText(row, colUP1, "1");
	else
		m_grid->SetItemText(row, colUP1, "-1");
	if (ic_diff>=0)
		m_grid->SetItemText(row, colUP2, "1");
	else
		m_grid->SetItemText(row, colUP2, "-1");
	it_curr = 0;
	it_vol = 0;
	for (int i = 1; i<=40; i++)
	{
		c_curr = m_grid->GetItemText(i, colUP1);		ic_curr = DefStrToInt(c_curr, 0);
		it_curr += ic_curr;
		c_vol = m_grid->GetItemText(i, colUP2);		ic_vol = DefStrToInt(c_vol, 0);
		it_vol += ic_vol;
	}
	m_sum_up1.Format("%d", it_curr);
	m_sum_up2.Format("%d", it_vol);

	//InvalidateRect(m_rc_sum_up1);
	//InvalidateRect(m_rc_sum_up2);
	

}

void CMainWnd::OnBtnSend()
{
	m_bViewChart = !m_bViewChart;
	CRect rc; GetClientRect(rc);
	
	if (m_bViewChart)
		m_pBtnSend->SetWindowText("데이터");
	else
		m_pBtnSend->SetWindowText("차트");
	if (m_grid)
	{
		
		rc.top += 44;
		if (m_bViewChart)
			rc.bottom -= 200;
		
		m_grid->MoveWindow(rc);
	}

}

void CMainWnd::DrawUpData(CDC *pdc)
{
	if (pdc == NULL)
	{
		pdc = &CPaintDC(this);
	}
	const int HIGHT_TABLE = 42;
	pdc->SelectObject(m_font);
	pdc->FillSolidRect(2,2, 940,14, GetIndexColor(74));
	pdc->FillSolidRect(2,14, 60,28, GetIndexColor(74));
	clrText = GetIndexColor(76);
	pdc->SetTextColor(clrText);
	pdc->DrawText(m_basis, m_rc_basis,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	
	
	m_rc_title = m_rc_choi_curr;
	m_rc_title.OffsetRect(0,-27);
	pdc->DrawText("현재가", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_choi_diff;
	m_rc_title.OffsetRect(0,-27);
	pdc->DrawText("전일대비", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_cha_migyul;
	m_rc_title.OffsetRect(0,-14);
	pdc->DrawText("미결제수량", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_medo_geon;
	m_rc_title.OffsetRect(0,-27);
	pdc->DrawText("매도총건수", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_mesu_geon;
	m_rc_title.OffsetRect(0,-27);
	pdc->DrawText("매수총건수", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_medo_jan;
	m_rc_title.OffsetRect(0,-27);
	pdc->DrawText("매도총잔량", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_mesu_jan;
	m_rc_title.OffsetRect(0,-27);
	pdc->DrawText("매수총잔량", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_meme_jan;
	m_rc_title.OffsetRect(0,-27);
	pdc->DrawText("총호가순잔량", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_meme_geon;
	m_rc_title.OffsetRect(0,-27);
	pdc->DrawText("총호가순건수", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_choi_curr;
	m_rc_title.OffsetRect(-57,0);
	pdc->DrawText("KP최근월", m_rc_title,  DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_cha_curr;
	m_rc_title.OffsetRect(-57,0);
	pdc->DrawText("KP차근월", m_rc_title,  DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_cha_curr;
	m_rc_title.OffsetRect(-57,-14);
	pdc->DrawText("월물구분", m_rc_title,  DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_sum_up1;
	m_rc_title.OffsetRect(0,-27);
	pdc->DrawText("지정 상승", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_rc_title = m_rc_sum_up2;
	m_rc_title.OffsetRect(0,-27);
	pdc->DrawText("전일 상승", m_rc_title,  DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

	pdc->FillSolidRect(60,15, 882,27, GetIndexColor(68));
	//pdc->FillSolidRect(60,28, 812,14, GetIndexColor(77));
	
	clrText = GetIndexColor(69);
	pdc->SetTextColor(clrText);
	pdc->DrawText(m_sum_up1, m_rc_sum_up1,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	pdc->DrawText(m_sum_up2, m_rc_sum_up2,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	double itemp = atof(m_choi_diff);
	if (itemp > 0)
		clrText = GetIndexColor(94);
	else if (itemp < 0)
		clrText = GetIndexColor(95);
	else
		clrText = GetIndexColor(69);
	pdc->SetTextColor(clrText);
	CString strtcurr;
	strtcurr = m_choi_curr; strtcurr.Remove('-'); strtcurr.Remove('+');
	pdc->DrawText(strtcurr, m_rc_choi_curr,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_choi_diff.Remove(' ');
	pdc->DrawText(m_choi_diff, m_rc_choi_diff,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	clrText = GetIndexColor(69);
	pdc->SetTextColor(clrText);
	pdc->DrawText(m_medo_jan, m_rc_medo_jan,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	pdc->DrawText(m_mesu_jan, m_rc_mesu_jan,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	pdc->DrawText(m_medo_geon, m_rc_medo_geon,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	pdc->DrawText(m_mesu_geon, m_rc_mesu_geon,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	pdc->DrawText(m_meme_jan, m_rc_meme_jan,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	pdc->DrawText(m_meme_geon, m_rc_meme_geon,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	itemp = atof(m_cha_diff);
	if (itemp > 0)
		clrText = GetIndexColor(94);
	else if (itemp < 0)
		clrText = GetIndexColor(95);
	else
		clrText = GetIndexColor(69);
	pdc->SetTextColor(clrText);
	strtcurr = m_cha_curr; strtcurr.Remove('-'); strtcurr.Remove('+');
	pdc->DrawText(strtcurr, m_rc_cha_curr,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	m_cha_diff.Remove(' ');
	pdc->DrawText(m_cha_diff, m_rc_cha_diff,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
	clrText = GetIndexColor(69);
	pdc->SetTextColor(clrText);
	pdc->DrawText(m_migyul, m_rc_cha_migyul,  DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

	pdc->SelectObject(m_pGridLine);
	pdc->MoveTo(m_rc_choi_curr.left, 2);
	pdc->LineTo(m_rc_choi_curr.left, HIGHT_TABLE);
	pdc->MoveTo(m_rc_choi_diff.left, 2);
	pdc->LineTo(m_rc_choi_diff.left, HIGHT_TABLE);
	pdc->MoveTo(m_rc_cha_migyul.left, 2);
	pdc->LineTo(m_rc_cha_migyul.left, HIGHT_TABLE);
	pdc->MoveTo(m_rc_medo_geon.left, 2);
	pdc->LineTo(m_rc_medo_geon.left, HIGHT_TABLE);
	pdc->MoveTo(m_rc_mesu_geon.left, 2);
	pdc->LineTo(m_rc_mesu_geon.left, HIGHT_TABLE);
	pdc->MoveTo(m_rc_medo_jan.left, 2);
	pdc->LineTo(m_rc_medo_jan.left, HIGHT_TABLE);
	pdc->MoveTo(m_rc_mesu_jan.left, 2);
	pdc->LineTo(m_rc_mesu_jan.left, HIGHT_TABLE);
	pdc->MoveTo(m_rc_meme_jan.left, 2);
	pdc->LineTo(m_rc_meme_jan.left, HIGHT_TABLE);
	pdc->MoveTo(m_rc_meme_geon.left, 2);
	pdc->LineTo(m_rc_meme_geon.left, HIGHT_TABLE);
	pdc->MoveTo(m_rc_sum_up1.left, 2);
	pdc->LineTo(m_rc_sum_up1.left, HIGHT_TABLE);
	pdc->MoveTo(m_rc_sum_up2.left, 2);
	pdc->LineTo(m_rc_sum_up2.left, HIGHT_TABLE);
	pdc->MoveTo(2, m_rc_choi_curr.top-1);
	pdc->LineTo(942, m_rc_choi_curr.top-1);
	pdc->MoveTo(2, m_rc_cha_curr.top-1);
	pdc->LineTo(942, m_rc_cha_curr.top-1);
	/*
	itemp = atof(m_basis);
	if (itemp > 0)
		clrText = GetIndexColor(94);
	else if (itemp < 0)
		clrText = GetIndexColor(95);
	else
		clrText = GetIndexColor(69);
	*/	
	
}

void CMainWnd::initArea()
{
	CRect rc;
	GetClientRect(&rc);
	const int START_POS = 60, WIDTH_CURR = 65, WIDTH_DIFF = 50, WIDTH_ITEM = 78, I_HEIGHT = 15, I_GAP = 2;
	const int LINE2_TOP = 29, LINE1_TOP = 16;
	m_rc_choi_curr.left = rc.left+START_POS; m_rc_choi_curr.right = m_rc_choi_curr.left+WIDTH_CURR;
	m_rc_choi_curr.top = LINE2_TOP; m_rc_choi_curr.bottom = m_rc_choi_curr.top+I_HEIGHT;
	
	m_rc_choi_diff.left = m_rc_choi_curr.right+I_GAP; m_rc_choi_diff.right = m_rc_choi_diff.left + WIDTH_DIFF;
	m_rc_choi_diff.top = LINE2_TOP; m_rc_choi_diff.bottom = m_rc_choi_diff.top+I_HEIGHT;
	
	m_rc_medo_jan.left = m_rc_choi_diff.right+I_GAP+I_GAP+WIDTH_ITEM; m_rc_medo_jan.right = m_rc_medo_jan.left + WIDTH_ITEM;
	m_rc_medo_jan.top = LINE2_TOP; m_rc_medo_jan.bottom = m_rc_medo_jan.top+I_HEIGHT;
	
	m_rc_mesu_jan.left = m_rc_medo_jan.right+I_GAP; m_rc_mesu_jan.right = m_rc_mesu_jan.left + WIDTH_ITEM;
	m_rc_mesu_jan.top = LINE2_TOP; m_rc_mesu_jan.bottom = m_rc_mesu_jan.top+I_HEIGHT;

	m_rc_medo_geon.left = m_rc_mesu_jan.right+I_GAP; m_rc_medo_geon.right = m_rc_medo_geon.left + WIDTH_ITEM;
	m_rc_medo_geon.top = LINE2_TOP; m_rc_medo_geon.bottom = m_rc_medo_geon.top+I_HEIGHT;

	m_rc_mesu_geon.left = m_rc_medo_geon.right+I_GAP; m_rc_mesu_geon.right = m_rc_mesu_geon.left + WIDTH_ITEM;
	m_rc_mesu_geon.top = LINE2_TOP; m_rc_mesu_geon.bottom = m_rc_mesu_geon.top+I_HEIGHT;

	m_rc_meme_jan.left = m_rc_mesu_geon.right+I_GAP; m_rc_meme_jan.right = m_rc_meme_jan.left + WIDTH_ITEM+20;
	m_rc_meme_jan.top = LINE2_TOP; m_rc_meme_jan.bottom = m_rc_meme_jan.top+I_HEIGHT;

	m_rc_meme_geon.left = m_rc_meme_jan.right+I_GAP; m_rc_meme_geon.right = m_rc_meme_geon.left + WIDTH_ITEM+20;
	m_rc_meme_geon.top = LINE2_TOP; m_rc_meme_geon.bottom = m_rc_meme_geon.top+I_HEIGHT;

	m_rc_sum_up1.left = m_rc_meme_geon.right+I_GAP; m_rc_sum_up1.right = m_rc_sum_up1.left + WIDTH_ITEM;
	m_rc_sum_up1.top = LINE2_TOP; m_rc_sum_up1.bottom = m_rc_sum_up1.top+I_HEIGHT;

	m_rc_sum_up2.left = m_rc_sum_up1.right+I_GAP; m_rc_sum_up2.right = m_rc_sum_up2.left + WIDTH_ITEM;
	m_rc_sum_up2.top = LINE2_TOP; m_rc_sum_up2.bottom = m_rc_sum_up2.top+I_HEIGHT;

	m_rc_cha_curr.left = rc.left+START_POS; m_rc_cha_curr.right = m_rc_cha_curr.left+WIDTH_CURR;
	m_rc_cha_curr.top = LINE1_TOP; m_rc_cha_curr.bottom = m_rc_cha_curr.top+I_HEIGHT;
	
	m_rc_cha_diff.left = m_rc_cha_curr.right+I_GAP; m_rc_cha_diff.right = m_rc_cha_diff.left + WIDTH_DIFF;
	m_rc_cha_diff.top = LINE1_TOP; m_rc_cha_diff.bottom = m_rc_cha_diff.top+I_HEIGHT;
	
	m_rc_cha_migyul.left = m_rc_cha_diff.right+I_GAP; m_rc_cha_migyul.right = m_rc_cha_migyul.left + WIDTH_ITEM;
	m_rc_cha_migyul.top = LINE1_TOP; m_rc_cha_migyul.bottom = m_rc_cha_migyul.top+I_HEIGHT;

	m_rc_dataRect.left = rc.left+START_POS; m_rc_dataRect.right = m_rc_sum_up2.right;
	m_rc_dataRect.top = LINE1_TOP; m_rc_dataRect.bottom = 40;
	
	
}

void CMainWnd::DrawChart(CDC *pDC, CRect rc)
{
	/*
	if (pDC == NULL)
	{
		pDC = &CPaintDC(this);
	}
	*/
	if (!m_bViewChart) return;	
	
	//m_sync.Lock();
	pDC->SetTextColor(RGB(0,0,0));
	const int LEFT_SIDE_GAB = 60, RIGHT_SIDE_GAB = 70, TOP_GAB = 20, BOTTOM_GAB = 18;
	CRect in_rc, teduri_rc;
	in_rc = rc;
	in_rc.left += LEFT_SIDE_GAB; in_rc.right -= RIGHT_SIDE_GAB;
	in_rc.top += TOP_GAB; in_rc.bottom -= BOTTOM_GAB;
	teduri_rc = in_rc;
	teduri_rc.InflateRect(4,4);
	pDC->Rectangle(teduri_rc);
	const int cnt = m_arCPoint.GetSize();
	pDC->SelectObject(m_font);
	int x_expos = 60;
	CPen* oldpen;
	oldpen = pDC->SelectObject(m_pLine2Pen);
	pDC->MoveTo(x_expos, 8);	x_expos+=20;
	pDC->LineTo(x_expos, 8);	x_expos+=4;
	pDC->TextOut(x_expos, 2, "총호가순잔량"); x_expos+=80;
	pDC->SelectObject(m_pLine1Pen);
	pDC->MoveTo(x_expos, 8);	x_expos+=20;
	pDC->LineTo(x_expos, 8);	x_expos+=4;
	pDC->TextOut(x_expos, 2, "KP최근월물가");	x_expos+=80;
	pDC->SelectObject(m_pDeliPen);
	pDC->MoveTo(x_expos, 8);	x_expos+=20;
	pDC->LineTo(x_expos, 8);	x_expos+=4;
	pDC->TextOut(x_expos, 2, "지정가시점 구분선");
	//CRect m_rc_check = CRect(rc.right-50, rc.top+2, rc.right, rc.top+17);

	CRect checkRc;
	checkRc = m_rc_Check1; checkRc.OffsetRect(0, (m_rc_Check1.bottom-16)*-1); checkRc.right = checkRc.left+15;
	pDC->DrawFrameControl(&checkRc, DFC_BUTTON, DFCS_BUTTONCHECK | (m_bCheck1 ? DFCS_CHECKED : 0));
	pDC->TextOut(checkRc.right+2, checkRc.top, "KP최근월물가");
	/*
	pDC->Rectangle(checkRc);
	CPoint pos; GetCursorPos(&pos);
	CString stmp; stmp.Format("pos.y: %d <> checkRc.top: %d <> m_rc_Check1.top: %d", pos.y, checkRc.top, m_rc_Check1.top);
	pDC->TextOut(checkRc.left+20-300, checkRc.top, stmp);
	*/
	checkRc = m_rc_Check2; checkRc.OffsetRect(0, (m_rc_Check2.bottom-16)*-1); checkRc.right = checkRc.left+15;
	pDC->DrawFrameControl(&checkRc, DFC_BUTTON, DFCS_BUTTONCHECK | (m_bCheck2 ? DFCS_CHECKED : 0));
	pDC->TextOut(checkRc.right+2, checkRc.top, "총호가순잔량");
	if (cnt > 1)
	{
		const int draw_width = in_rc.Width();
		const int draw_height = in_rc.Height();
		class ChartPoint *pCPoint = NULL;
		int x=0, ypos=0;
		double y = 0;
		CString str;
		const double w_unit = (double)draw_width/(cnt-1);
		double h_unit{};
		bool bfirst = false;
		
		pDC->SelectObject(m_pLine2Pen);
		
		//return;
		h_unit = (double)draw_height/(m_maxy2val-m_miny2val);
		
		CPoint lastPoint;
		//if (cnt>=10000) istartpos = cnt-10000;
		//else istartpos = 0;
		int i=0;
		const int istartpos = 0;
		CRect minmaxtrc;
		if (m_bCheck2)
		{
			minmaxtrc = CRect(rc.left+2,in_rc.bottom-7, rc.left+2+52, in_rc.bottom+5);
			//str.Format("%d",(int)m_miny2val);
			str = AddCommai((int)m_miny2val);
			pDC->DrawText(str, minmaxtrc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
			ypos = (int)((m_maxy2val-m_miny2val)*h_unit);
			minmaxtrc = CRect(rc.left+2,in_rc.bottom-ypos-7, rc.left+2+52, in_rc.bottom-ypos+5);
			//str.Format("%d",(int)m_maxy2val);
			str = AddCommai((int)m_maxy2val);
			pDC->DrawText(str, minmaxtrc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
			
			for (i=istartpos; i<cnt;i++)
			{
				pCPoint = m_arCPoint.GetAt(i).get();
				if (pCPoint)
				if(pCPoint->bhoga)
				{

						x = i;//pCPoint->idx;
						y = pCPoint->data;
						//TRACE("%04d/%04d) i: %d, w_unit: %f, xpos: %d\n",i, cnt, i, w_unit, (int)(i*w_unit));
						pCPoint->xpos = (int)(x*w_unit);
						ypos = (int)((y-m_miny2val)*h_unit);
						//TRACE("%05d) 현재가: %f, 최소: %f, 위치: %f\n",i, y, m_minyval, ypos);
						lastPoint.x = pCPoint->xpos+in_rc.left;
						lastPoint.y = in_rc.bottom-ypos;
						if (!bfirst)
						{
							bfirst = true;
							pDC->MoveTo(lastPoint.x, lastPoint.y);
						}	
						else
							pDC->LineTo(lastPoint.x, lastPoint.y);
				}
				
			}
			//if (i==m_lastHVOL) 마지막 순호가잔량
			{
				ypos = (int)((m_lastIVol-m_miny2val)*h_unit);
				CRect trc = CRect(rc.left+2,in_rc.bottom-ypos-7, rc.left+2+52, lastPoint.y-7+13);
				trc.OffsetRect(-1,-1);
				pDC->FillSolidRect(trc, RGB(0,255,0));
				pDC->SetTextColor(RGB(0,0,0));
				trc.OffsetRect(1,1);
				pDC->DrawText(AddCommai(m_lastIVol), trc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
				
				//pDC->TextOut(rc.left+2,in_rc.bottom-ypos-7, pCPoint->sdata);
			}
		}
		pDC->SetTextColor(RGB(0,0,0));
		h_unit = (double)draw_height/(m_maxyval-m_minyval);
		pDC->SelectObject(m_pLine1Pen);
		bfirst = false;
		
		minmaxtrc = CRect(in_rc.right+8,in_rc.bottom-7, in_rc.right+60, in_rc.bottom+5);
		str.Format("%.2f",m_minyval);
		pDC->FillSolidRect(minmaxtrc, RGB(255,255,255));
		pDC->DrawText(str, minmaxtrc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
		ypos = (int)((m_maxyval-m_minyval)*h_unit);
		minmaxtrc = CRect(in_rc.right+8,in_rc.bottom-ypos-7, in_rc.right+60, in_rc.bottom-ypos+5);
		str.Format("%.2f",m_maxyval);
		pDC->DrawText(str, minmaxtrc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);

		for (i=istartpos; i<cnt;i++)
		{
			pCPoint = m_arCPoint.GetAt(i).get();
			
			if (pCPoint)
			if(!pCPoint->bhoga)
			{
				if (pCPoint->data == -1)
				{
					pDC->FillSolidRect(0,0,1,1, RGB(255,255,255));
					x = i;//pCPoint->idx;
					y = pCPoint->data;
					//TRACE("%04d/%04d) i: %d, w_unit: %f, xpos: %d\n",i, cnt, i, w_unit, (int)(i*w_unit));
					pCPoint->xpos = (int)(x*w_unit);
					ypos = (int)((y-m_minyval)*h_unit);
					//TRACE("%05d) 현재가: %f, 최소: %f, 위치: %f\n",i, y, m_minyval, ypos);
					x = pCPoint->xpos+in_rc.left;
					y = in_rc.bottom-ypos;
					pDC->SelectObject(m_pDeliPen);
					pDC->MoveTo(x, in_rc.top+2);
					pDC->LineTo(x, in_rc.bottom-2);
					pDC->SelectObject(m_pLine1Pen);
					pDC->MoveTo(lastPoint.x, lastPoint.y);
					pDC->TextOut(x-27, in_rc.bottom+4, pCPoint->stime);
					
				}
				else
				{
					if (m_bCheck1)
					{
						x = i;//pCPoint->idx;
						y = pCPoint->data;
						//TRACE("%04d/%04d) i: %d, w_unit: %f, xpos: %d\n",i, cnt, i, w_unit, (int)(i*w_unit));
						pCPoint->xpos = (int)(x*w_unit);
						ypos = (int)((y-m_minyval)*h_unit);
						lastPoint.x = pCPoint->xpos+in_rc.left;
						lastPoint.y = in_rc.bottom-ypos;
						//TRACE("%05d) 현재가: %f, 최소: %f, 위치: %f\n",i, y, m_minyval, ypos);
						if (!bfirst)
						{
							bfirst = true;
							pDC->MoveTo(lastPoint.x,lastPoint.y);
						}	
						else
							pDC->LineTo(lastPoint.x,lastPoint.y);
						/*
						if (i==m_lastCheg)
						{
							CRect trc = CRect(in_rc.right+8,in_rc.bottom-ypos-7, in_rc.right+60, in_rc.bottom-ypos-7+13);
							trc.OffsetRect(-1,-1);
							pDC->FillSolidRect(trc, RGB(255,0,0));
							pDC->SetTextColor(RGB(255,255,255));
							trc.OffsetRect(1,1);
							pDC->DrawText(pCPoint->sdata, trc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
							//pDC->TextOut(rc.left+2,in_rc.bottom-ypos-7, pCPoint->sdata);
						}
						*/
					}
				}
				
			}
			
		}
		{
			ypos = (int)((m_lastIChg-m_minyval)*h_unit);
			//CRect trc = CRect(rc.left+2,in_rc.bottom-ypos-7, rc.left+2+52, lastPoint.y-7+13);
			CRect trc = CRect(in_rc.right+8,in_rc.bottom-ypos-7, in_rc.right+60, in_rc.bottom-ypos-7+13);
			trc.OffsetRect(-1,-1);
			pDC->FillSolidRect(trc, RGB(255,0,0));
			pDC->SetTextColor(RGB(255,255,255));
			trc.OffsetRect(1,1);
			str.Format("%.2f",m_lastIChg);
			pDC->DrawText(str, trc, DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX);
			//pDC->TextOut(rc.left+2,in_rc.bottom-ypos-7, pCPoint->sdata);
		}		
		pDC->SelectObject(oldpen);	
	}
	//m_sync.Unlock();
		
}

void CMainWnd::AddChartData(CString time, CString sval, double yval, int idx, bool bhoga)
{
	m_sync.Lock();
	
	//class ChartPoint* pChartPoint = new class ChartPoint;
	std::shared_ptr<ChartPoint> pChartPoint = std::make_shared<ChartPoint>();


	pChartPoint->idx = m_maxidx;
	pChartPoint->data = yval;
	if (m_arCPoint.GetSize()>=10000)
	{
		std::shared_ptr<ChartPoint> delPoint;
		delPoint = m_arCPoint.GetAt(0);
		delPoint.reset();
		m_arCPoint.RemoveAt(0);

		//class ChartPoint* pDelPoint;
		//pDelPoint = m_arCPoint.GetAt(0);
		//delete pDelPoint;
		//m_arCPoint.RemoveAt(0);
	}	
	if (bhoga)
	{
		pChartPoint->sdata.Format("%d", (int)yval);
	}else{	
		pChartPoint->sdata.Format("%.2f", yval);
	}
	pChartPoint->stime = time;
	pChartPoint->bhoga = bhoga;
	if (!bhoga & !m_bfchg)
	{
		if (yval!=-1)
		{
			m_bfchg = true;
			m_maxyval = yval;
			m_minyval = yval;
			m_lastIVol = (int)yval;
		}
	}
	else if (bhoga & !m_bfhog)
	{
		
		m_bfhog = true;
		m_maxy2val = yval;
		m_miny2val = yval;			
		m_lastIChg = yval;
	
	}

	
	
	if (!bhoga)
	{
		if (yval!=-1)
		{
			if (m_maxyval < yval)
				m_maxyval = yval;
			if (m_minyval > yval)
				m_minyval = yval;
			m_lastCheg = idx;
			m_lastIChg = yval;
		}
	}
	else
	{
		if (m_maxy2val < yval)
			m_maxy2val = yval;
		if (m_miny2val > yval)
			m_miny2val = yval;
		m_lastHVOL = idx;
		m_lastIVol = (int)yval;
	}

	m_arCPoint.Add(pChartPoint);
	m_bChartChange = true;
	m_sync.Unlock();
	
	//InvalidateRect(drawChartRect, false);
}

BOOL CMainWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return false;
	return CWnd::OnEraseBkgnd(pDC);
}

CPen* CMainWnd::RequestPen(COLORREF rgbLine, int iWidth, int iStyle)
{
	struct	_penR	penR;
	penR.clr = rgbLine;
	penR.width = iWidth;
	penR.style = iStyle;
	
	return (CPen*)m_pWizard->SendMessage(WM_USER, (WPARAM)MAKEWORD(getPEN, NULL), (LPARAM)&penR);
}

void CMainWnd::RemoveCPosAll()
{
	class ChartPoint* pCPoint = nullptr;
	std::shared_ptr<class ChartPoint> pPoint = nullptr;
	const int istartpos = m_arCPoint.GetSize()-1;

	for (int i=istartpos; i>=0;i--)
	{
		pPoint = m_arCPoint.GetAt(i);
		pPoint.reset();
		//pCPoint = m_arCPoint.GetAt(i);
		//m_arCPoint.RemoveAt(i);
		//delete pCPoint;
	}
}

void CMainWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//OutputDebugString("OnTimer\n");
// 	switch(nIDEvent)
// 	{
// 	case TID_CHARTDRAW:
// 		if (m_bChartChange)
// 		{
// 			m_bChartChange = false;
// 			InvalidateRect(drawChartRect, false);
// 		}		
// 		break;
// 	case TID_DATACHANGE:
// 		if (m_bDataChange)
// 		{
// 			m_bDataChange = false;
// 			InvalidateRect(m_rc_dataRect, false);
// 		}
// 		break;
// 	}	

	if(m_bDataChange || m_bChartChange)
	{
		Invalidate();
	}

	if (m_bChartChange)
	{
		m_bChartChange = false;
		//InvalidateRect(drawChartRect, false);
	}

	if (m_bDataChange)
	{
		m_bDataChange = false;
		//InvalidateRect(m_rc_dataRect, false);
	}


	CWnd::OnTimer(nIDEvent);
}

void CMainWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_rc_Check1.PtInRect(point))
	{
		m_bCheck1 = !m_bCheck1;
		InvalidateRect(drawChartRect, false);
	}
	else if (m_rc_Check2.PtInRect(point))
	{
		m_bCheck2 = !m_bCheck2;
		InvalidateRect(drawChartRect, false);
	}
	CWnd::OnLButtonUp(nFlags, point);
}

CBitmap* CMainWnd::RequestBitmap(CString stPath)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)stPath.operator LPCTSTR());
}

CString CMainWnd::AddComma(CString strSrc)
{
	strSrc.TrimLeft();
	strSrc.TrimRight();
	strSrc.Remove(',');
	
	CString strFloat;
	CString strNatural;
	const int iPos = strSrc.Find('.');
	if (iPos >= 0)
	{
		strNatural = strSrc.Left(iPos);
		strFloat = strSrc.Mid(iPos);
	}
	else
	{
		strNatural = strSrc;
		strFloat.Empty();
	}
	
	CString strReturn;
	int jj = -1;
	for (int ii = strNatural.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (strNatural.GetAt(0) != '-' && strNatural.GetAt(0) != '+')
				strReturn = ',' + strReturn;
			else if (ii != 0)
				strReturn = ',' + strReturn;
			
			jj = 0;
		}
		strReturn = strNatural.GetAt(ii) + strReturn;
	}
	strReturn += strFloat;
	
	return strReturn;
}

CString CMainWnd::AddCommai(int iSrc)
{
	CString strSrc;
	strSrc.Format("%d", iSrc);
	//	strSrc.TrimLeft();
	//	strSrc.TrimRight();
	
	CString strFloat;
	CString strNatural;
	const int iPos = strSrc.Find('.');
	if (iPos >= 0)
	{
		strNatural = strSrc.Left(iPos);
		strFloat = strSrc.Mid(iPos);
	}
	else
	{
		strNatural = strSrc;
		strFloat.Empty();
	}
	
	CString strReturn;
	
	int jj = -1;
	for (int ii = strNatural.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (strNatural.GetAt(0) != '-' && strNatural.GetAt(0) != '+')
				strReturn = ',' + strReturn;
			else if (ii != 0)
				strReturn = ',' + strReturn;
			
			jj = 0;
		}
		strReturn = strNatural.GetAt(ii) + strReturn;
	}
	strReturn += strFloat;
	
	return strReturn;
}

BOOL CMainWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	NM_GVITEM* nmgv = (NM_GVITEM*) lParam;
	if (wParam != IDC_EXECELGRID)
		return CWnd::OnNotify(wParam, lParam, pResult);	
	
	switch (nmgv->hdr.code)
	{
	case GVNM_LMOUSEDOWN:
		{
			if (nmgv->row == 0)
			{
				//CIdCell icell;
				CString str;
				//icell = m_grid->GetCell(nmgv->row, nmgv->col);
				str = m_grid->GetItemText(nmgv->row, nmgv->col);
				//CString tmp; tmp.Format("▽ %d, △ %d", str.Find("▽"), str.Find("△"));
				//AfxMessageBox(tmp);
				//MessageToParent(nmgv->col, 0, GVNM_ENDSORT);
				/*
				if (str.Find("▽")!=-1)
				{
					//str.Remove('△');
					str.Replace("▽","△");
					m_grid->SetItemText(nmgv->row, nmgv->col, str);
					m_grid->Sort(nmgv->col, true);
				}else if (str.Find("△")!=-1){
					//str.Remove('▽');
					str.Replace("△","▽");
					m_grid->SetItemText(nmgv->row, nmgv->col, str);
					m_grid->Sort(nmgv->col, false);
				}else{
					str.Insert(0, "△");
					m_grid->SetItemText(nmgv->row, nmgv->col, str);
					m_grid->Sort(nmgv->col, true);
					
					
				}
				*/
				

			}
			
		}		
		break;
	case GVNM_LMOUSEUP:
		if (nmgv->row > -1)
		{
			CString code = m_grid->GetItemText(nmgv->row, colRCODE);
			code.TrimLeft();
			code.TrimRight();
			code.Remove('A');
			
			CString strTrigger(_T(""));
			
			//AfxMessageBox(code);
			if(code.GetLength() != 6)
				break;
			
			strTrigger.Format("1301\t%s", code);
			
			Variant(triggerCC, strTrigger);
		}
				
		break;
	}	
	//return CWnd::OnNotify(wParam, lParam, pResult);

	// escdream check 
	return FALSE;
}


void CMainWnd::CalcRowDiff(int row)
{
	CString t_curr, t_vol, c_curr, c_vol, c_diff;
	int it_curr=0, it_vol=0, ic_curr=0, ic_vol=0, ic_diff=0;
	c_curr = m_grid->GetItemText(row, colCURR);		ic_curr = DefStrToAbsInt(c_curr, 0);	
	c_diff  = m_grid->GetItemText(row, colPCURR);		ic_diff = DefStrToAbsInt(c_diff, 0);
	
	ic_diff = ic_curr - ic_diff;
	if (ic_diff>=0)
		m_grid->SetItemText(row, colUP2, "1");
	else
		m_grid->SetItemText(row, colUP2, "-1");
	it_curr = 0;
	it_vol = 0;
	for (int i = 1; i<=40; i++)
	{
		c_vol = m_grid->GetItemText(i, colUP2);		ic_vol = DefStrToInt(c_vol, 0);
		it_vol += ic_vol;
	}
	m_sum_up2.Format("%d", it_vol);

	//InvalidateRect(m_rc_sum_up2);
	

}