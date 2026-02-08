// GridWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "GridWnd.h"
#include "intgrid.h"
#include "../../h/grid.h"
#include "GroupWnd.h"
#include "MainWnd.h"

#include "UsefulSetupDlg.h"
#include "MoveSetupDlg.h"		// ADD PSH 20070918
#include "AutoSaveSetupDlg.h"	// ADD PSH 20070918
#include "AlertSoundDlg.h"		// ADD PSH 20070918
#include "PopupDlg.h"
#include "CodeDuplicate.h"
#include "InterDomino.h"

#include "ShowMsgWnd.h"			// ADD PSH 20070918
#include "mmsystem.h"
#include <math.h>

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__; 
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridWnd

CGridWnd::CGridWnd(CWnd* pMainWnd, int nIndex) : CBaseWnd(pMainWnd)
{
	SetIndex(nIndex);
	
	m_grid = NULL;
	m_kind = 0;
	m_szTitle = _T("");
	m_updateROW = -1;
	m_inters.RemoveAll();
	m_mapCode.RemoveAll();
	m_endsort = false;
	m_posField = false;
	m_ccField  = false;
	m_drag = m_drop = -1;
	m_nTurn = 0;
	m_seldrop = -1;
	m_bongField = -1;
	m_sonikField = -1;
	m_suikField = -1;

	// ADD PSH 20070911
	m_pTicker	= NULL;
	m_bTicker	= TRUE;
	m_bfirstStart = NULL;

	m_bMargin	= TRUE;
	m_bSorting	= FALSE;
	m_mapMargin.RemoveAll();
	m_clrMarkerBKG	= RGB(240, 240, 240);
	m_clrMarkerTXT	= RGB(142, 142, 142);

	m_bShadow	= TRUE;
	m_bAllApply	= TRUE;
	m_bMoveCfg  = FALSE;
	m_nMoveSave = 0;
	m_pMsgWnd	= NULL;
	m_bInfo		= FALSE;

	m_bAddCnd	= FALSE;
	m_bChgCnd  = FALSE;
	m_bAutoSaveAllApply = FALSE;
	m_nRowHeight	= 0;
	m_bWaveApply	= FALSE;

	m_clrKospi	  = RGB(0, 0, 0);
	m_clrKosdaq	  = RGB(128, 64, 64);
	m_bMKClrApply = FALSE;

	m_bExpect	  = FALSE;
	m_bAutoExpect = FALSE;
	m_bPopupDlg	  = FALSE;

	m_bRTS    = FALSE;
	m_bDispCode   = FALSE;
	
	m_nCodeWidth  = 80;

	m_pRCDlg	  = NULL;
	m_bMonitor	  = FALSE;
	m_bMonSym	  = FALSE;
	m_bEditWork = FALSE;

	m_bIsFirst = FALSE;
	
	m_dBase = 0;
	m_ClickRow = 0;

	for(int i=0;i<200;i++)
	{
		for(int j=0;j<5;j++)
		{
			m_iYDayVol[i][j] = "";
		}
	}

	for(int i=0 ; i< 200 ; i++)
	{
		m_irowCode[i] = 0;
	}

	m_yDayVolField = -1;			//전일거래대비율필드
	m_EqualizerField = -1;

	m_recomDataField = -1;
	m_recomBasicCostField = -1;
	m_recomSuikrateField = -1;
	m_recomBMjisuField = -1;
	m_recomBMSuikrateField = -1;
	m_recomgoalCostField = -1;

	m_nGridWidth = 0;				//그리드 넓이 
	// END ADD
	for(int cc=0 ; cc<200 ; cc++)
	{
		chg_count[cc] = 0;
	}

	m_dKosdaq = 0;
	m_dKospi = 0;
}




CGridWnd::~CGridWnd()
{
	if (NULL != m_pRCDlg)
	{
		m_pRCDlg->DestroyWindow();
		delete m_pRCDlg;
	}
}


BEGIN_MESSAGE_MAP(CGridWnd, CBaseWnd)
	//{{AFX_MSG_MAP(CGridWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_RBUTTONUP()	// ADD PSH 20070912
	ON_WM_SIZING()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridWnd message handlers


HBITMAP CGridWnd::getBitmap(CString path)
{
	CBitmap*	pBitmap = (CBitmap*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETBITMAP, (LPARAM)(char*)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

void CGridWnd::OperInit()
{
	m_pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	m_pGroupWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	m_pToolWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	m_pTreeWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	HBITMAP	hBITMAP, hBITMAP_DN, hBITMAP_HV;

	// 트리 펼치기 버튼
	CString	fileIMG;
	fileIMG.Format("%s\\%s\\", m_root, IMAGEDIR);

	LOGFONT	lf;
	m_pFont->GetLogFont(&lf);
	struct _fontR fontR;
	fontR.bold = FW_BOLD;
	fontR.italic = FALSE;
	fontR.point = ((CGroupWnd*)m_pGroupWnd)->GetFontSize();
	fontR.name = lf.lfFaceName;
	m_pFontB = GetAxFont(&fontR);
	lf.lfHeight = fontR.point * 10;


	//m_rowC = MAX_LINE + 1;
	m_rowC = 2;//BUFFET 2->1
	// END MDOIFY
	m_colC = 8;
	m_clrFOCUS[0] = RGB(255, 255, 128);
	m_clrFOCUS[1] = RGB(255, 255, 255);
	m_clrTEXT[0] = RGB(100, 100, 100);
	m_clrTEXT[1] = RGB(100, 100, 100);
	m_size.cy = ((CGroupWnd*)m_pGroupWnd)->GetRowHeight();	
	loadcfg();	
	
	m_grid = new CintGrid(m_pMainWnd, this, &lf); 
	m_grid->Create(CRect(0, 0, 0, 0), this, IDC_GRID, GVSC_BOTH, GVDD_FULL);
	
	// ADD PSH 20070911
	m_grid->SetBmpIcon(fileIMG + "MARKER.bmp", 0);
	m_grid->SetBmpIcon(fileIMG + "관리종목.bmp", 1);
	m_grid->SetBmpIcon(fileIMG + "투자유의.bmp", 2);
	m_grid->SetBmpIcon(fileIMG + "투자주의.bmp", 3);
	m_grid->SetBmpIcon(fileIMG + "투자경고.bmp", 4);
	m_grid->SetBmpIcon(fileIMG + "투자위험.bmp", 5);
	m_grid->SetBmpIcon(fileIMG + "추천종목.BMP", 6);
	//	m_btSAVE.ShowWindow(SW_SHOW);
	//	m_btCLOSE.ShowWindow(SW_SHOW);

	m_bMoveCfg  = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVECFG);
	m_nMoveSave = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVESAVE);

	m_pTicker = new CintGrid(m_pMainWnd, this, &lf); 
	m_pTicker->Create(CRect(0, 100, 0, 0), this, IDC_GRID, GVSC_NONE, GVDD_NONE);//buff
	m_pTicker->ShowWindow(m_bTicker);
	
	m_pTicker->SetBmpIcon(fileIMG + "MARKER.bmp", 0);
	m_pTicker->SetBmpIcon(fileIMG + "관리종목.bmp", 1);
	m_pTicker->SetBmpIcon(fileIMG + "투자유의.bmp", 2);
	m_pTicker->SetBmpIcon(fileIMG + "투자주의.bmp", 3);
	m_pTicker->SetBmpIcon(fileIMG + "투자경고.bmp", 4);
	m_pTicker->SetBmpIcon(fileIMG + "투자위험.bmp", 5);
	m_pTicker->SetBmpIcon(fileIMG + "추천종목.BMP", 6);

	InitPopupMenu();
	m_pTicker->Invalidate();
	
	m_btDomino.Create("\n복수현재가2 종목연동", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this , IDC_BT_DOMINO);
	m_btSAVE.Create("\n관심종목저장", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this , IDC_BT_SAVE);
	m_btCLOSE.Create("\n그룹초기화", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_ERASE);

	hBITMAP = getBitmap(fileIMG + "GTI_MATRIX2.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "GTI_MATRIX2_dn.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "GTI_MATRIX2_en.bmp");
	
	m_btDomino.SetFont(m_pFont, false);  
	m_btDomino.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	hBITMAP = getBitmap(fileIMG + "SAVEINT.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "SAVEINT_dn.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "SAVEINT_en.bmp");
	
	m_btSAVE.SetFont(m_pFont, false);  
	m_btSAVE.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	
	hBITMAP = getBitmap(fileIMG + "닫기1.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "닫기1_dn.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "닫기1_en.bmp");
	
	m_btCLOSE.SetFont(m_pFont, false);  
	m_btCLOSE.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	RemoveAll();
	FieldSetup(false);	
//	int idx;
//	idx = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETCBINDEX);
	SetTicker(m_bTicker);
//	loadInterest(1);

	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);	
}

void CGridWnd::FieldSetup(bool bDlgSetup, int iEqualizer)
{
	int	nIndex = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_GETAFIELD);
	m_bongField = -1;
	m_sonikField = -1;
	m_suikField = -1;
	m_ccField = false;
	m_posField = false;
	m_grid->Clear();
	loadField(nIndex, bDlgSetup, iEqualizer);

	initialGrid();

	// ADD PSH 20070918
	MarkerSetup();

	m_grid->MakeEqualizerBmp();
	// END ADD
}

void CGridWnd::OperDestory()
{
	savecfg();
	m_btDomino.DestroyWindow();
	m_btSAVE.DestroyWindow();
	m_btCLOSE.DestroyWindow();
	ClearInterest();
	m_grid->DestroyWindow();
	delete m_grid;
	m_pTicker->DestroyWindow();
	delete m_pTicker;
	KillTimer(1000);
}

void CGridWnd::DrawTitle(CDC* pDC)
{
	int	nCurSel = m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_GET));
	BOOL	bFocus = (nCurSel == m_nIndex) ? TRUE : FALSE;
	int	nmode = pDC->SetBkMode(TRANSPARENT);
	IH::DrawBox(pDC, m_rcTitle, RGB(200, 200, 200), BOX_ALL, (bFocus) ? m_clrFOCUS[0] : m_clrFOCUS[1], TRUE);
	COLORREF clr = pDC->SetTextColor((bFocus) ? m_clrTEXT[0] : m_clrTEXT[1]);
	CFont*	oldfont = NULL;
	if (bFocus)
		oldfont = pDC->SelectObject(m_pFontB);

	if (IsInterest())
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	CRect	rect = m_rcTitle;
	rect.OffsetRect(0, 1);
	pDC->DrawText(m_szTitle, rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	pDC->SetTextColor(clr);

	if (oldfont)
		pDC->SelectObject(oldfont);
	pDC->SetBkMode(nmode);

}

void CGridWnd::OperDraw(CDC* pDC)
{
	DrawTitle(pDC);

}

void CGridWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	SendMessage(WM_MANAGE, MK_CLICKGROUP);
	CBaseWnd::OnLButtonDown(nFlags, point);
}

// ADD PSH 20070912
void CGridWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{

	if (m_rcTitle.PtInRect(point))
	{
		ShowPopupMenu();
	}
	
	CBaseWnd::OnRButtonUp(nFlags, point);
}
// END ADD

void CGridWnd::OperResize(int cx, int cy)
{
	CRect	rect, rcButton;
	rect = m_rcTitle = m_rect;
	m_rcTitle.bottom = m_rcTitle.top + 24;//@@Zeta m_size.cy + 1; 
	rcButton = m_rcTitle;

	rcButton.right -= 1;
	rcButton.top += 2;
	rcButton.left = rcButton.right - SIZE_BUTTONS - 1;
	rcButton.bottom = rcButton.top + SIZE_BUTTONS;
	m_btCLOSE.MoveWindow(rcButton, TRUE);
	rcButton.OffsetRect(-SIZE_BUTTONS - 2, 0);
	m_btSAVE.MoveWindow(rcButton, TRUE);
	rcButton.OffsetRect(-SIZE_BUTTONS - 2, 0);
	m_btDomino.MoveWindow(rcButton, TRUE);	

	rect.top = m_rcTitle.bottom;
	/*
	int nCnt = (rect.Height() / m_nRowHeight) - m_grid->GetRowCount();
	BOOL bAddDel = TRUE;
								
	if (0 > nCnt)
	{
		nCnt = -nCnt;
		bAddDel = FALSE;
	}

	for (int i = 0; i < nCnt; i++)
	{
		if (bAddDel)
		{
			insertRow(m_rowC);
		}
		else
		{
			if ((m_grid->GetItemText(m_rowC, colNAME)).IsEmpty())
				DeleteRow(m_rowC);
		}
	}
	*/
	if(m_recomBasicCostField > 0 && ((CGroupWnd*)m_pGroupWnd)->GetIsRecommand() == true)
	{
		m_grid->SetColumnWidth(m_recomBasicCostField, 0);
	}
	
	if(m_recomBMjisuField > 0 && ((CGroupWnd*)m_pGroupWnd)->GetIsRecommand() == true)
	{
		m_grid->SetColumnWidth(m_recomBMjisuField, 0);
	}
	
	m_grid->MoveWindow(rect, TRUE);
	m_grid->Adjust();

	// ADD PSH 20070911
	m_pTicker->MoveWindow(m_rcTitle, TRUE);
	m_pTicker->Adjust();
	// END ADD
	
	if (IsInit())
	{
		// MODIFY PSH 20070911

		//m_btSAVE.ShowWindow(!m_bTicker);
		//m_btCLOSE.ShowWindow(!m_bTicker);
		// END MODIFY
		m_grid->ShowWindow(SW_SHOW);

		// ADD PSH 20070911
		m_pTicker->ShowWindow(m_bTicker);
		m_btDomino.ShowWindow(true);
		m_btSAVE.ShowWindow(true);
		m_btCLOSE.ShowWindow(true);
//		m_btSAVE.ShowWindow(SW_SHOW);
//		m_btCLOSE.ShowWindow(SW_SHOW);
		m_btDomino.Invalidate(TRUE);
		m_btSAVE.Invalidate(TRUE);
		m_btCLOSE.Invalidate(TRUE);
		// END ADD
	}
	
	InvalidateRect(m_rcTitle);
	InvalidateRect(rect);
	
	m_btDomino.ShowWindow(SW_SHOW);
	m_btSAVE.ShowWindow(SW_SHOW);
	m_btCLOSE.ShowWindow(SW_SHOW);

	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);	

	m_grid->MakeEqualizerBmp();
}

void CGridWnd::InsertRowNCode(int row)
{
//	m_bEditWork = true;
/*	_inters* pinters = m_inters.GetAt(m_inters.GetUpperBound());
	if (pinters != NULL)
	{
		CString	code = CString(pinters->code, sizeof(pinters->code));
		code.TrimLeft(); 
		code.TrimRight();

		if (strlen((char*)code.operator LPCTSTR()) > 0)
		{
			Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
			return;
		}
		// 20080118
		else if (m_inters.GetSize() > 100) 
		{
			RemoveInters(MAX_LINE);
			ReIndexing();
		}
	}
*/
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
	int rowcount = 0;
	
	if(nOver == MO_VISIBLE)
	{
		pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
		rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
	}
	else
	{
		rowcount = GetRowcount();
	}
	
	if(rowcount >= MAX_LINE)
	{
		Variant(guideCC, IH::idTOstring(IDS_GUIDE4));

		return;
	}

	insertInterest(row - 1);
	insertRow(row);
	SetLineColor();
 	m_grid->memoCheck(); 
 	m_grid->memoRefresh();
	m_grid->SetFocusCellEdit(row, colNAME, true);

	if (m_bAddCnd)
		saveInterest();
}

LONG CGridWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	switch (LOWORD(wParam))
	{
	case MK_SISECATCH:
		OperAlarm(HIWORD(wParam), (char*)lParam);
		break;
	case MK_MEMO:
		m_grid->memoCheck();
		Invalidate(FALSE);
		break;
	case MK_DELETEMEMO:
		break;
	case MK_NEWS:
		queryNewsCode();
		break;
	case MK_GETDATAKIND:
		ret = GetKind();
		break;
	case MK_CHART:
		ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_CHART);
		break;
	case MK_TURNCODE:
		{
			int	nsec = HIWORD(wParam);
			if (nsec == 0)
			{
				KillTimer(1000);
				m_nTurn = 0;				
			}		
			else
			{
				if (CAST_TREEID(m_kind)->kind == xISSUE)
					break;

				m_nTurn = 0;				
				SetTimer(1000, nsec * 1000, NULL);
			}
		}
		break;
	case MK_EXCEL:
		m_grid->RunExcel();
		break;
	case MK_HIDECODE:
		m_grid->HideCode();
		break;
	case MK_SELGROUP:
		InvalidateRect(m_rcTitle, FALSE);
		break;
	case MK_CLICKGROUP:
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)m_nIndex);
		break;
	case MK_TREEDATA:
		ParseData((class CGridData*)lParam);
		break;
	case MK_UPJONGDATA:
		ParseUpjongData((class CGridData*)lParam);
		break;
	case MK_REMAINDATA:
		ParseRemainData((class CGridData*)lParam);
		break;
	case MK_SENDTR:
		sendTransaction();
		break;
	case MK_RECVDATA:
		RecvOper(HIWORD(wParam), (CRecvData*)lParam);
		break;
	case MK_RTSDATA:
		RecvRTS((char*)lParam);
		break;
	case MK_ENDDRAG:
		m_drag = m_drop = -1;
		m_grid->FreeDragDrop();
		CintGrid::m_dropdata.Reset();
		break;
	case MK_INSERTROW:
		ret = insertRow(HIWORD(wParam));
		break;
	case MK_INSERTCODE:
		m_bEditWork = true;
		InsertRowNCode(HIWORD(wParam));
		break;
	case MK_DELETEROW:
		DeleteRow(HIWORD(wParam));
		// DEL PSH 20070918
		//insertRow(MAX_LINE);
		//saveInterest();
		// END DEL
		insertInterest(MAX_LINE - 1);
		SetLineColor();
		m_bEditWork = true;
		break;
 	case MK_TRIGGEROW:
 		triggerCode(HIWORD(wParam));
 		break;
	case MK_SAVE:
		// MODIFY PSH 20070918
		//saveInterest();
		{
			if (GVNM_ENDDRAG == lParam)
			{
				if (!m_bSorting)
				{
					saveInterest();
				}
				else
				{
					if (m_bMoveCfg && (1 == m_nMoveSave))
					{
						RemoveAllBaseInters();
						m_grid->SortBase();
						saveInterest();
					}
					else if (!m_bMoveCfg)
					{
						m_bMoveCfg  = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVECFG);
						m_nMoveSave = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVESAVE);
						
						//091013 수정
						CMoveSetupDlg dlg;
						dlg.m_nOption = m_nMoveSave;
						dlg.m_bAlert  = m_bMoveCfg;
						
						//dlg 항상 열기 옵션 체크 
						if(dlg.m_bAlert)
						{
							if (1 == dlg.m_nOption) 
							{
								RemoveAllBaseInters();
								m_grid->SortBase();
								saveInterest();
								m_bEditWork = false;
							}
							else 
							{
								m_bEditWork = true;
							}
						}
						else
						{
							if (dlg.DoModal())
							{
								if (1 == dlg.m_nOption) 
								{
									RemoveAllBaseInters();
									m_grid->SortBase();
									saveInterest();
									m_bEditWork = false;
								}
								else
								{
									m_bEditWork = true;
								}
							}
						}
						
						m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVECFG, dlg.m_bAlert);
						m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVESAVE, dlg.m_nOption);
					}
				}
			}
			else
			{
				saveInterest();
			}
		}
		// END MODIFY
		break;
	case MK_HAWKEYE:
		{
			LONG	ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
			hawkeyeAction((ret) ? true : false);
//			Invalidate(FALSE);
		}
		break;
	// ADD PSH 20070911
	case MK_HSCROLL:
		{
			m_pTicker->SetHScroll(HIWORD(wParam), lParam);
		}
		break;
	case MK_SORTBASE:
		{
			BaseSorting();
		}
		break;
	// END ADD
	default:
		ret = CBaseWnd::OnManage(wParam, lParam);
		break;

	}
	return ret;
}

void CGridWnd::triggerCode(int row)
{
	m_ClickRow = row;
	CString code = m_grid->GetItemText(row, colCODE);
	CString string; 
	int kind = 0;

	code.TrimLeft(); 
	code.TrimRight();
	
	if (code.IsEmpty()) 
		return;
	
	if (((CGroupWnd*)m_pGroupWnd)->GetSelAction() == 0)
		return;
	
	switch (GetCodeType(code))
	{
	case kospiCODE:
		string.Format("%s\t%s", HCOD, code);
		break;
	case futureCODE:
		string.Format("%s\t%s", FCOD, code);
		break;
	case optionCODE:
		string.Format("%s\t%s", OCOD, code);
		break;
	case foptionCODE:
		string.Format("%s\t%s", PCOD, code);
		break;
	case indexCODE:
		string.Format("%s\t%s", UCOD, code);
		break;
	case sinjuCODE:
		string.Format("%s\t%s", SINJUCOD, code);
		break;
	case elwCODE:
		string.Format("%s\t%s", HCOD, code);
		Variant(triggerCC, string);
		//					m_grid->SetFocus();
		string.Format("%s\t%s", ELWCOD, code);
		break;
	case thirdCODE:
		string.Format("%s\t%s", THIRDCOD, code);					
		break;
	}
	Variant(triggerCC, string);
	Variant(codeCC, string);
	
	m_grid->SetFocus();
}

void CGridWnd::RecvOper(int kind, CRecvData* rdata)
{
	struct _trkey* trkey = (struct _trkey*)&kind;
	if (trkey->group != (UINT)m_nIndex)
		return;

//	testSaveFile("GridWnd::RecvOper", (char*)rdata->m_lParam);
//	testSaveFile("GridWnd::RecvOper", "GRIDWnd~Recv~OK");
	switch (trkey->kind)
	{
	case TRKEY_GRIDUPJONG:
// 		parsingOubsUpjong((char*)rdata->m_lParam, HIWORD(rdata->m_wParam));
// 		break;
	case TRKEY_GRIDNEW:
		parsingOubs((char*)rdata->m_lParam, HIWORD(rdata->m_wParam));
		break;
	case TRKEY_GRIDNEWS:
		parsingNewsCode(rdata);
		break;
	case TRKEY_GRIDROW:
		parsingOubsOne((char*)rdata->m_lParam, HIWORD(rdata->m_wParam), m_updateROW);
		m_updateROW = -1;
		break;
	case TRKEY_GRIDSAVE:
		break;
	case TRKEY_RECOMMAND:
		parseReCommandData((char*)rdata->m_lParam, HIWORD(rdata->m_wParam));
		break;
	}
}

void CGridWnd::AddData_Overoper(int nIndex, CString sztmp)
{
	struct _inters*	pinters;
	BOOL	bInt = FALSE;
	CString	code, amount, price, bookmark;
	
	// ADD PSH 20070913
	CString strName;
	
	if (0 == nIndex)
	{
 		UINT attr = m_grid->GetItemAttr(nIndex+1, colNAME) & ~GVAT_MARKER;
		m_grid->SetItemAttr(nIndex+1, colNAME, attr);
	}
	else
	{
		insertRow(nIndex+1);
	}
	// END ADD
	
	code = IH::Parser(sztmp, P_TAB);
	
	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		bInt = TRUE;
		amount = IH::Parser(sztmp, P_TAB);
		price = IH::Parser(sztmp, P_TAB);
		// ADD PSH 20070914
		strName = IH::Parser(sztmp, P_TAB);
		bookmark = IH::Parser(sztmp, P_TAB);
		// END ADD
	}
	
	if (!bInt && code.IsEmpty())
		return;
	
	pinters = m_inters.GetAt(nIndex);
	pinters->gubn[0] = '0';
	
	strcpy(pinters->code, code);
	
	if (bInt)
	{
		strcpy(pinters->xnum, amount);
		strcpy(pinters->xprc, price);
		CopyMemory(&pinters->bookmark, bookmark, sizeof(pinters->bookmark[0]));
	}
	
	// ADD PSH 20070914
	if (!code.IsEmpty() && code.GetAt(0) == 'm')
	{
		pinters->gubn[0] = 'm';
		strcpy(pinters->name, strName);
		
 		UINT attr = m_grid->GetItemAttr(nIndex+1, colNAME) | GVAT_MARKER;
 		m_grid->SetItemAttr(nIndex+1, colNAME, attr);
 		m_grid->SetItemText(nIndex+1, colNAME, strName);
		
		return;
	}
	// END ADD
}

void CGridWnd::AddData(int nIndex, CString sztmp)
{
	struct _inters*	pinters;
	BOOL	bInt = FALSE;
	CString	code, amount, price, bookmark;

	// ADD PSH 20070913
	CString strName;

	if (0 == nIndex)
	{
		UINT attr = m_grid->GetItemAttr(nIndex , colNAME & ~GVAT_MARKER);
		m_grid->SetItemAttr(nIndex, colNAME, attr);
	}
	else
	{
		insertRow(nIndex);	
	}
	// END ADD

	code = IH::Parser(sztmp, P_TAB);

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		bInt = TRUE;
		amount = IH::Parser(sztmp, P_TAB);
		price = IH::Parser(sztmp, P_TAB);
		// ADD PSH 20070914
		strName = IH::Parser(sztmp, P_TAB);
		bookmark = IH::Parser(sztmp, P_TAB);
		// END ADD
	}

	if (!bInt && code.IsEmpty())
		return;
	
	pinters = m_inters.GetAt(nIndex);
	pinters->gubn[0] = '0';

	strcpy(pinters->code, code);

	if (bInt)
	{
		strcpy(pinters->xnum, amount);
		strcpy(pinters->xprc, price);
		CopyMemory(&pinters->bookmark, bookmark, sizeof(pinters->bookmark[0]));
	}

	// ADD PSH 20070914
	if (!code.IsEmpty() && code.GetAt(0) == 'm')
	{
		pinters->gubn[0] = 'm';
		strcpy(pinters->name, strName);

		UINT attr = m_grid->GetItemAttr(nIndex + 1, colNAME);
		m_grid->SetItemAttr(nIndex +1, colNAME, attr|GVAT_MARKER);
		m_grid->SetItemText(nIndex +1, colNAME, strName);
		
		return;
	}
	else
	{
		m_grid->SetItemAttr(nIndex + 1, colNAME, m_grid->GetItemAttr(nIndex+1, colNAME)&~GVAT_MARKER);
	}
	// END ADD
}

void CGridWnd::RemoveAll()
{
	ClearInterest();
	struct _inters* pinters = NULL;
	for (int ii = 0; ii < MAX_LINE; ii++)
	{
		pinters = (_inters *) new char[sz_inters];
		ZeroMemory(pinters, sz_inters);

		pinters->gubn[0] = '0';
		AddInters(pinters);
	}

	// ADD PSH 20070917
	int nRow = m_grid->GetRowCount() - 1;
	for (; nRow > 1; nRow--)
	{
		m_grid->DeleteRow(nRow);
		m_grid->memoDelete(nRow);
	}


	for (int i = 0; i < m_colC; i++)
	{
		m_grid->SetItemText(1, i, "");
		m_grid->SetItemData(1, i, 0);
		m_pTicker->SetItemText(0, i, "");
		m_pTicker->SetItemData(0, i, 0);
	}
	
	m_rowC = 2;		//BUFFET 2->1
	// END ADD
}

void CGridWnd::ParseRemainData(class CGridData* sdata)
{
	RemoveAll();
	
	SetKind(sdata->GetKind());
	InvalidateRect(m_rcTitle, FALSE);	
	ClearInterest();

	CString	sztmp = _T("");
	CString	code, amount, price, name, strBookmark;
	char bookmark[1];
	bookmark[0] = '0';

	struct _inters*	pinters = NULL;
	int	ncnt = sdata->GetCount();
	BOOL	bInt = FALSE;

	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		bInt = TRUE;

	int ii = 0;
	for ( ii = 0 ; ii < ncnt ; ii++ )
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, P_TAB);
		
		if (!bInt && code.IsEmpty())
			continue;
	
		pinters = (_inters *) new char[sz_inters];
		ZeroMemory(pinters, sz_inters);

		pinters->gubn[0] = '0';
		strcpy(pinters->code, code);

		if (pinters->code[0] == 'm')
		{
			pinters->gubn[0] = 'm';

		}

		if (bInt || !sztmp.IsEmpty())
		{			
			amount = IH::Parser(sztmp, P_TAB);
			price = IH::Parser(sztmp, P_TAB);
			name = IH::Parser(sztmp, P_TAB);
			strBookmark = IH::Parser(sztmp, P_TAB);
			
			// END ADD
			strcpy(pinters->xnum, amount);
			strcpy(pinters->xprc, price);
			strcpy(pinters->name, name);
			
			//북마크 추가
			CopyMemory(&pinters->bookmark, strBookmark, sizeof(pinters->bookmark[0]));
			// END ADD
		}

		AddInters(pinters);

		if (ii != 0)
		{
			insertRow(ii, FALSE);
		}
		/*
		if (pinters->code[0] == 'm')
		{			
			UINT attr = m_grid->GetItemAttr(ii +1, colCODE);
			m_grid->SetItemAttr(ii+1, colCODE, attr|GVAT_TEXT);
			m_grid->SetItemText(ii+1, colCODE, pinters->code);
			
		}
		*/
	}

	for ( ; ii < MAX_LINE ; ii++ )
	{
		pinters = (_inters *) new char[sz_inters];
		ZeroMemory(pinters, sz_inters);

		pinters->gubn[0] = '0';
		AddInters(pinters);
	}

	// ADD PSH 20070918
	MarkerSetup();
	// END ADD
}

void CGridWnd::parseReCommandData(char* datB, int datL)
{
	struct  mod
	{
		char    nrec[4];						/*	갯수			*/
		char    acod[1024][12];					/*  종목코드 리스트 */
	};
	
	#define	sz_mod	sizeof(struct mod)

	struct mod* mod = NULL;

	mod = (struct mod*)new char[sz_mod];
	ZeroMemory(mod, sz_mod);

	class CGridData sdata;

	mod = (struct mod*)datB;

	CString strCnt = CString(mod->nrec, 4);
	int nCnt = atoi(strCnt);
	
	CString code, name;

	for(int i=0 ; i<nCnt ; i++)
	{
		code = CString(mod->acod[i], 12);
		code.TrimLeft();
		code.TrimRight();
		
		if (!code.IsEmpty())
		{
			sdata.m_arDatas.Add(code);
		}
	}

	ParseData(&sdata);
	sendTransaction();
}


void CGridWnd::ParseUpjongData(class CGridData* sdata)
{
	RemoveAll();
	ClearInterest();

	CString	sztmp = _T("");
	CString	code;
	
	struct _inters*	pinters = NULL;
	int	ncnt = sdata->GetCount();
	BOOL	bInt = FALSE;

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, P_TAB);
		
		if (code.IsEmpty())
			continue;

		pinters = (_inters *) new char[sz_inters];
		ZeroMemory(pinters, sz_inters);
		
		pinters->gubn[0] = '0';
		strcpy(pinters->code, code);

		AddInters(pinters);

		if (ii != 0)
		{
			insertRow(ii, FALSE);
		}
	}


	for ( ; ii < MAX_LINE ; ii++ )
	{
		pinters = (_inters *) new char[sz_inters];
		ZeroMemory(pinters, sz_inters);
		
		pinters->gubn[0] = '0';
		AddInters(pinters);
	}

	sendTransaction(200);
	

}



void CGridWnd::ParseData(class CGridData* sdata)
{	
	RemoveAll();
	
	SetKind(sdata->GetKind());
	InvalidateRect(m_rcTitle, FALSE);	
	ClearInterest();

	CString	sztmp = _T("");
	CString	code, amount, price, name, strBookmark, futureGubn, creditPrc, maeipPrc;
	char bookmark[1];
	bookmark[0] = '0';

	struct _inters*	pinters = NULL;
	int	ncnt = sdata->GetCount();
	BOOL	bInt = FALSE;

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
		bInt = TRUE;
	int ii = 0;
	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, P_TAB);
		
		if (!bInt && code.IsEmpty())
			continue;
	
		pinters = (_inters *) new char[sz_inters];
		ZeroMemory(pinters, sz_inters);

		pinters->gubn[0] = '0';
		strcpy(pinters->code, code);

		if (pinters->code[0] == 'm')
		{
			pinters->gubn[0] = 'm';

		}

		if (bInt || !sztmp.IsEmpty())
		{			
			amount = IH::Parser(sztmp, P_TAB);
			price = IH::Parser(sztmp, P_TAB);
			name = IH::Parser(sztmp, P_TAB);
			strBookmark = IH::Parser(sztmp, P_TAB);
			futureGubn = IH::Parser(sztmp, P_TAB);
			creditPrc = IH::Parser(sztmp, P_TAB);
			maeipPrc = IH::Parser(sztmp, P_TAB);

			// END ADD
			strcpy(pinters->xnum, amount);
			strcpy(pinters->xprc, price);
			strcpy(pinters->name, name);
			strcpy(pinters->futureGubn, futureGubn);
			pinters->creditPrc = atof(creditPrc);
 			pinters->maeipPrc = atof(maeipPrc);

			//북마크 추가
// 			bookmark = atoi(strBookmark);
// 			pinters->bookmark = bookmark;
			CopyMemory(&pinters->bookmark, strBookmark, sizeof(pinters->bookmark[0]));
			// END ADD
		}

		AddInters(pinters);

		if (ii != 0)
		{
			insertRow(ii, FALSE);
		}
		/*
		if (pinters->code[0] == 'm')
		{			
			UINT attr = m_grid->GetItemAttr(ii +1, colCODE);
			m_grid->SetItemAttr(ii+1, colCODE, attr|GVAT_TEXT);
			m_grid->SetItemText(ii+1, colCODE, pinters->code);
			
		}
		*/
	}

	for ( ; ii < MAX_LINE ; ii++ )
	{
		pinters = (_inters *) new char[sz_inters];
		ZeroMemory(pinters, sz_inters);

		pinters->gubn[0] = '0';
		AddInters(pinters);
	}

	// ADD PSH 20070918
	MarkerSetup();
	// END ADD
}

int  CGridWnd::loadcfg_data()
{
	int result = GetPrivateProfileInt(m_section, KEY_DATA, 0, m_fileCFG);
			
	return result;
}

void CGridWnd::savecfg_data(CString keydata)
{
	WritePrivateProfileString(m_section, KEY_DATA, keydata.operator LPCTSTR(), m_fileCFG);
}

void CGridWnd::loadcfg()
{

 	int confirm_start = GetPrivateProfileInt(m_section, KEY_DATA, 9999, m_fileCFG);
	
	if(confirm_start == 9999)
	{
		m_bfirstStart = TRUE;
	}
	else
	{
		m_bfirstStart = FALSE;
	}

	// data kind
	m_kind = GetPrivateProfileInt(m_section, KEY_DATA, 0, m_fileCFG);	
	

	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		m_kind = 0;
	
	char	buf[1024];
	
	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(m_section, KEY_FIELD, "", buf, sizeof(buf), m_fileCFG); 
	m_szFIELD.Format("%s", buf);

	memset(buf, 0x00, sizeof(buf));	
	GetPrivateProfileString(m_section, KEY_WIDTH, "", buf, sizeof(buf), m_fileCFG); 
	m_szWIDTH.Format("%s", buf);

	// ADD PSH 20070912
	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(m_section, "MARGIN", "", buf, sizeof(buf), m_fileCFG); 

	if(m_bfirstStart == TRUE)
	{
		m_bMargin = 1;
	}
	else
	{
		m_bMargin = atoi(buf);
	}


	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(m_section, "TICKER", "", buf, sizeof(buf), m_fileCFG); 
	
	if(m_bfirstStart == TRUE)
	{
		m_bTicker = 1;
	}
	else
	{
		m_bTicker = atoi(buf);
	}
	


	DWORD dwRes;

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "BKGCLR", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_clrMarkerBKG = RGB(240, 240, 240);;
	}
	else
	{
		m_clrMarkerBKG = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TXTCLR", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_clrMarkerTXT = RGB(142, 142, 142);
	}
	else
	{
		m_clrMarkerTXT = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TXTSHD", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_bShadow = TRUE;
	}
	else
	{
		m_bShadow = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "ALLAPPL", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_bAllApply = TRUE;
	}
	else
	{
		m_bAllApply = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "SYMINFO", "", buf, sizeof(buf), m_fileCFG); 

	if(m_bfirstStart == TRUE)
	{
		m_bInfo = 1;
	}
	else
	{
		if (0 == dwRes)
		{
			m_bInfo = FALSE;
		}
		else
		{
			m_bInfo = atoi(buf);
			dwRes = 0;
		}
	}






	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKAPPL", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_bApply = FALSE;
	}
	else
	{
		m_tkConfig.m_bApply = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKVOL", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_vol = 0;
	}
	else
	{
		m_tkConfig.m_vol = atoi(buf);
		dwRes = 0;
	}
	
	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKAMT", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_amt = 0;
	}
	else
	{
		m_tkConfig.m_amt = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKAND", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_and = FALSE;
	}
	else
	{
		m_tkConfig.m_and = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKPRC", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_price = FALSE;
	}
	else
	{
		m_tkConfig.m_price = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKSPRC", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_sprc = 0;
	}
	else
	{
		m_tkConfig.m_sprc = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKEPRC", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_eprc = 0;
	}
	else
	{
		m_tkConfig.m_eprc = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKULIM", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_ulimit = TRUE;
	}
	else
	{
		m_tkConfig.m_ulimit = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKUP", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_up = TRUE;
	}
	else
	{
		m_tkConfig.m_up = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKFLAT", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_flat = TRUE;
	}
	else
	{
		m_tkConfig.m_flat = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKDLIM", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_dlimit = TRUE;
	}
	else
	{
		m_tkConfig.m_dlimit = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKDOWN", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_tkConfig.m_down = TRUE;
	}
	else
	{
		m_tkConfig.m_down = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "ALLAUTO", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_bAutoSaveAllApply = FALSE;
	}
	else
	{
		m_bAutoSaveAllApply = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "ADDCND", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_bAddCnd = FALSE;
	}
	else
	{
		m_bAddCnd = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "CHGCND", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_bChgCnd = FALSE;
	}
	else
	{
		m_bChgCnd = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "CODEWIDTH", "", buf, sizeof(buf), m_fileCFG); 

	if (0 == dwRes)
	{
		m_nCodeWidth = 80;
	}
	else
	{
		m_nCodeWidth = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "GRIDWIDTH", "", buf, sizeof(buf), m_fileCFG); 

	if(0 == dwRes)
	{
		m_nGridWidth = 0;
	}
	else
	{
		m_nGridWidth = atoi(buf);
		dwRes = 0;
	}
	// END ADD
}

void CGridWnd::savecfg()
{
	// data kind
	CString	str;

	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		m_kind = 0;

	str.Format("%d", m_kind);
	WritePrivateProfileString(m_section, KEY_DATA, str.operator LPCTSTR(), m_fileCFG);	

	int	nWidth = 0;
	int	ncnt = m_gridHdrX.GetSize();
	struct _gridHdr	grdHdr;
	CString	sztmp, szWIDTH = _T(""), szFIELD = _T("");

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		grdHdr = m_gridHdrX.GetAt(ii);
		nWidth = m_grid->GetColumnWidth(ii);
		
		if (ii >= colNAME)
		{
			sztmp.Format("%d,", nWidth);
			szWIDTH += sztmp;
		}
		else if (ii == colCODE && 0 < nWidth)
		{
			m_nCodeWidth = nWidth;
		}

		if (ii > colCURR)
		{
			sztmp.Format("%s,", grdHdr.symbol);
			if (sztmp.Compare("2111,") == 0)
				sztmp.Format("2023,");
			else if (sztmp.Compare("2115,") == 0)
				sztmp.Format("2024,");
			else if (sztmp.Compare("2116,") == 0)
				sztmp.Format("2033,");
			else if (sztmp.Compare("2112,") == 0)
				sztmp.Format("2027,");

			if (grdHdr.needs == 1)
			{
				if (sztmp.Compare("2029,") == 0)
				{
					sztmp = "#1##,";
				}
				else if (sztmp.Compare("2030,") == 0)
				{
					sztmp = "#2##,";
				}
				else if (sztmp.Compare("2031,") == 0)
				{
					sztmp = "#3##,";
				}
			}
			else if (grdHdr.needs == 3)
			{
				if (sztmp.Compare("2029,") == 0)
				{
					sztmp = "#4##,";
				}
				else if (sztmp.Compare("2030,") == 0)
				{
					sztmp = "#5##,";
				}
				else if (sztmp.Compare("2031,") == 0)
				{
					sztmp = "#6##,";
				}
			}

			szFIELD += sztmp;
		}
	}

	WritePrivateProfileString(m_section, KEY_WIDTH, szWIDTH.operator LPCTSTR(), m_fileCFG);
	WritePrivateProfileString(m_section, KEY_FIELD, szFIELD.operator LPCTSTR(), m_fileCFG);

	// ADD PSH 20070912
	sztmp.Format("%d", m_bMargin);
	WritePrivateProfileString(m_section, "MARGIN", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bTicker);
	WritePrivateProfileString(m_section, "TICKER", sztmp.operator LPCTSTR(), m_fileCFG);


	sztmp.Format("%d", m_clrMarkerBKG);
	WritePrivateProfileString(m_section, "BKGCLR", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_clrMarkerTXT);
	WritePrivateProfileString(m_section, "TXTCLR", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bShadow);
	WritePrivateProfileString(m_section, "TXTSHD", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bAllApply);
	WritePrivateProfileString(m_section, "ALLAPPL", sztmp.operator LPCTSTR(), m_fileCFG);

	//종목 특이사항보기
	sztmp.Format("%d", m_bInfo);
	WritePrivateProfileString(m_section, "SYMINFO", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_tkConfig.m_bApply);
	WritePrivateProfileString(m_section, "TKAPPL", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_vol);			// 체결량 범위   (vol * 1,000)
	WritePrivateProfileString(m_section, "TKVOL", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_amt);			// 체결금액 범위 (amt * 10,000,000)
	WritePrivateProfileString(m_section, "TKAMT", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_and);			// and / or
	WritePrivateProfileString(m_section, "TKAND", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_price);			// 가격설정 flag
	WritePrivateProfileString(m_section, "TKPRC", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_sprc);			// 가격설정 시작값
	WritePrivateProfileString(m_section, "TKSPRC", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_eprc);			// 가격설정 끝값
	WritePrivateProfileString(m_section, "TKEPRC", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_ulimit);		// 상한
	WritePrivateProfileString(m_section, "TKULIM", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_up);			// 상승
	WritePrivateProfileString(m_section, "TKUP", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_flat);			// 보합
	WritePrivateProfileString(m_section, "TKFLAT", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_dlimit);		// 하한
	WritePrivateProfileString(m_section, "TKDLIM", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_down);			// 하락
	WritePrivateProfileString(m_section, "TKDOWN", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bAutoSaveAllApply);
	WritePrivateProfileString(m_section, "ALLAUTO", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bAddCnd);
	WritePrivateProfileString(m_section, "ADDCND", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bChgCnd);
	WritePrivateProfileString(m_section, "CHGCND", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_nCodeWidth);
	WritePrivateProfileString(m_section, "CODEWIDTH", sztmp.operator LPCTSTR(), m_fileCFG);
	// END ADD

	//가로크기 저장
	CRect rc;
	GetWindowRect(rc);
	sztmp.Format("%d", rc.Width());
	WritePrivateProfileString(m_section, "GRIDWIDTH", sztmp.operator LPCTSTR(), m_fileCFG);
}

BOOL CGridWnd::IsInterest()
{
	struct _treeID*	treeID = (struct _treeID*)&m_kind;
	if (treeID->kind == xINTEREST)
		return TRUE;
	return FALSE;
}

//테스트용

void CGridWnd::testSaveFile(CString gubn, CString jongmok, CString currPrc, CString maipPrc,  CString PyugPrc, CString sonikRate)
{
	CString strPath(_T("")), m_root, m_user, titletime;
	strPath.Format("%s/test.dat", m_root);

	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 
	
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

	if(gubn == "장중")
	{
		titletime.Format("[%s][%s]%s -> 매입단가 : %s 현재가 : %s 평가손익 : %s 손익률 : %s\n", strCurTime, gubn, jongmok, maipPrc, currPrc, PyugPrc, sonikRate);
	}
	else
	{
		titletime.Format("[%s][%s]%s -> 매입단가 : %s 예상가 : %s 평가손익 : %s 손익률 : %s\n", strCurTime, gubn, jongmok, maipPrc, currPrc, PyugPrc, sonikRate);
	}
	
	file.WriteString(titletime);
	file.Close();
}

void CGridWnd::testSaveFile2(CString code, CString time, CString symbol, CString EvaPrc)
{
	CString strPath(_T("")), m_root, m_user, titletime;
	strPath.Format("%s/test2.dat", m_root);
	
	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 

	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

	titletime.Format("[%s]종목번호 : %s 심볼명 : %s 현재가 : %s \n", time, code, symbol, EvaPrc);
	
	file.WriteString(titletime);
	file.Close();
}


void CGridWnd::testSaveFile3(CString code, CString datB)
{
	CString strPath(_T("")), m_root, m_user, titletime;
	strPath.Format("%s/test3.dat", m_root);

	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());

	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 	
	
	titletime.Format("[%s][%s]%s\n", time, code, datB);

	file.WriteString(titletime);
	file.Close();	
}




void CGridWnd::sendTransaction(int update)
{
	int	sendL = 0;
	char	tempB[64];
	int	bufSize = sizeof(struct _gridHi) + 50 + m_gridHdrX.GetSize()*5 + m_inters.GetSize()*12 + 12;
	char	*sendB = new char[bufSize];

	FillMemory(sendB, bufSize, ' ');

	BOOL	bExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
	m_bAutoExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);

	if(bExpect)
	{
		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
	}
	else
	{
		if(m_bAutoExpect)
		{
			sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
		}
		else
		{
			sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 0, P_TAB);
		}
	}

//	sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, (bExpect) ? 1 : 0, P_TAB);

	CopyMemory(&sendB[sendL], tempB, strlen(tempB));	
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", gSYMBOL, P_DELI);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));	
	sendL += strlen(tempB);

	struct	_gridHi* gridHi;

	gridHi = (struct _gridHi *) &sendB[sendL];
	sendL += sizeof(struct _gridHi);

	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", m_inters.GetSize());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir  = '1';
	gridHi->sort = '0';
 
	struct	_gridHdr gridHdr;
	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "2321%c", P_NEW);					//전일거래량
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2022%c", P_NEW);					//새로 추가된 심볼의 현재가
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	if(((CGroupWnd*)m_pGroupWnd)->GetIsRecommand())
	{
		sprintf(tempB, "1915%c", P_NEW);					//지수 구분
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}
	else
	{
		sprintf(tempB, "1910%c", P_NEW);					//추천일자
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
		
		sprintf(tempB, "1911%c", P_NEW);					//추천종목 기준가
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);

		sprintf(tempB, "1912%c", P_NEW);					//수익률
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
		
		sprintf(tempB, "1913%c", P_NEW);					//추천종목 기준 BM지수
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
		
		sprintf(tempB, "1914%c", P_NEW);					//BM수익률
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
		
		sprintf(tempB, "1915%c", P_NEW);					//지수 구분
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}


	/////
	sprintf(tempB, "%s%c", CCOD, P_DELI);				// 종목코드
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct _inters* pinters;
	int	ncnt = 0;
	int nInterCnt = m_inters.GetSize();
	CString tempStr;
	int pos = -1;

// 	sprintf(tempB, "%s%c", "001", P_DELI);					//kospi
// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
// 	sendL += strlen(tempB);
// 	
// 	sprintf(tempB, "%s%c", "201", P_DELI);					//kosdaq
// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
// 	sendL += strlen(tempB);

	for (int ii = 0; ii < nInterCnt; ii++)
	{
		pinters = m_inters.GetAt(ii);
		
		if (strlen(pinters->code))
		{
			tempStr= CString(pinters->code);
			/*
			if (pinters->code[0] == 'm')
			{
				m_grid->SetItemText(ii+1, colCODE, tempStr);
			}
			*/
			pos = tempStr.Find(" ");
			if(pos != -1)
			{
//				strcpy(temp, tempStr.Mid(0, pos));
				strcpy(pinters->code, tempStr.Mid(0, pos));
			}
			
			ncnt++;
		}
		
		sprintf(tempB, "%s%c", strlen(pinters->code) <= 0 ? " " : pinters->code, P_DELI);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}




	sendB[sendL] = P_NEW; 
	sendL += 1;
	sendB[sendL] = P_TAB; 
	sendL += 1;
	sendB[sendL] = 0x00;
	CSendData	sdata;
	char	key;
	_trkey* trkey = (struct _trkey*)&key;

	trkey->group = m_nIndex;

	if (update < 0)
	{
		trkey->kind = TRKEY_GRIDNEW;
	}
	else if(update >= 0 && update < 200)
	{
		trkey->kind = TRKEY_GRIDROW;
	}
	else
	{
		trkey->kind = TRKEY_GRIDUPJONG;
	}

	m_updateROW = update;

	sdata.SetData("pooppoop", key, sendB, sendL, "");
	
//	CString dt;
//	dt.Format("key :: %d  SendBuffer :: %s SendLength :: %d", key, sendB, sendL);
//	testSaveFile("GridWnd::SendTransaction", dt);

	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
	m_endsort = false;
	
	delete[] sendB;
}

BOOL CGridWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_GVITEM* nmgv = (NM_GVITEM*) lParam;

	TRACE("\n nmgv row : %d \n", nmgv->row);

	if (wParam != IDC_GRID)
		return CBaseWnd::OnNotify(wParam, lParam, pResult);	

	switch (nmgv->hdr.code)
	{
	case GVNM_REFRESH:
		{
			m_bEditWork = FALSE;
			m_pToolWnd->SendMessage(WM_MANAGE, MK_REFRESH);
		}
		break;
	case GVNM_CHGCOLSIZE:
		{
			int	ncnt = m_gridHdrX.GetSize();
			CString	sztmp = _T("");
			int	nWidth;
			m_szWIDTH = _T("");
			struct _gridHdr	grdHdr;
			for ( int ii = 0 ; ii < ncnt ; ii++ )
			{
				grdHdr = m_gridHdrX.GetAt(ii);
				nWidth = m_grid->GetColumnWidth(ii);
				
				if (ii >= colNAME)
//				if (ii >= colCODE)
				{
					sztmp.Format("%d,", nWidth);
					m_szWIDTH += sztmp;

					if (ii == colCODE && nWidth != 0) m_nCodeWidth = nWidth;
				}
				
				// ADD PSH 20070911
				if (ii == nmgv->col)
				{
					m_pTicker->SetColumnWidth(ii, nWidth);

					m_pTicker->Adjust();
				}
				// END ADD
			}
		}
		break;
	case GVNM_CHANGECODE:
		{		
			m_grid->SetFocus();
			CString code, oldname, name;
			code = m_grid->GetItemText(nmgv->row, colCODE);

			if (code.GetLength() == 6 || code.GetLength() == 8 || code.GetLength() == 9)
			{
				name = GetCodeName(code);
			}

			struct	_inters* pinters;
			int	count = m_inters.GetSize();
			int	xrow  = nmgv->row - 1;
			
			//종목 중복 등록 허용
			if(((CGroupWnd*)GetParent())->GetOverLapAction() == FALSE)		
			{
				if (code.GetLength() && IsDuplicateCode(code, nmgv->row))
				{
					CCodeDuplicate dlg;

					//dlg 항상 열기 옵션 체크 
					if(dlg.DoModal())
					{
						if(dlg.m_bAlert == FALSE)
						{
//							Variant(guideCC, IH::idTOstring(IDS_GUIDE2));
// 							struct _inters* pInter = m_inters.GetAt(nmgv->row - 1);
 							m_grid->SetItemText(nmgv->row, colCODE, "");
// 							DeleteRow(nmgv->row);	
// 							insertInterest(MAX_LINE - 1);
				
							if (m_bAddCnd) 
							{	
								saveInterest();
							}
							break;
						}
						else
						{
							//파일에 저장
							((CGroupWnd*)GetParent())->saveOverLap(TRUE);
						}
					}
					else
					{
						struct _inters* pInter = m_inters.GetAt(nmgv->row - 1);
						m_grid->SetItemText(nmgv->row, colCODE, pInter->code);
						DeleteRow(nmgv->row);	
						insertInterest(MAX_LINE - 1);
						if (m_bAddCnd) 
						{	
							saveInterest();
						}
						break;
					}
				}
			}

			if (count > xrow)
			{
				pinters = m_inters.GetAt(xrow);

				// ADD PSH 20070913
				int nRowCnt = m_arrBaseInters.GetSize();
				if (m_bSorting)
				{
					if (strlen(pinters->code))
					{
						CopyInter(m_arrBaseInters.GetAt(xrow), pinters);
					}
					else
					{
						_inters* pBaseInter = (_inters *) new char[sz_inters];
						ZeroMemory(pBaseInter, sz_inters);

						pBaseInter->gubn[0] = '0';
						strcpy(pBaseInter->code, code);
						strcpy(pBaseInter->name, name);

						m_arrBaseInters.Add(pBaseInter);
					}
				}
				// END ADD

				pinters->gubn[0] = '0';
				strcpy(pinters->code, code);
				strcpy(pinters->name, name);
				m_grid->SetItemText(xrow + 1, colNAME, name);
				m_inters.SetAt(xrow, pinters);

				
			}
			else
			{
				for (int ii = count; ii < xrow; ii++)
				{
					pinters = (_inters *) new char[sz_inters];
					ZeroMemory(pinters, sz_inters);

					pinters->gubn[0] = '0';
					AddInters(pinters);
				}

				pinters = (_inters *) new char[sz_inters];
				ZeroMemory(pinters, sz_inters);

				pinters->gubn[0] = '0';
				strcpy(pinters->code, code);
				strcpy(pinters->name, name);
				m_grid->SetItemText(xrow + 1, colNAME, name);
				AddInters(pinters);
			}

			if (code.GetLength() <= 0)
			{
				for (int ii = 0; ii < m_colC; ii++)
					m_grid->SetItemText(nmgv->row, ii, "");
				m_grid->memoCheck(nmgv->row);
				break;
			}


			m_bEditWork = TRUE;
			if (m_bChgCnd) 
			{
				saveInterest();
			}

			sendTransaction(xrow);			
		}
		break;
	case GVNM_ENDEDIT: case GVNM_ENDRETURN:
		{		
			m_grid->SetFocus();


			int	nRow = nmgv->row, nCol = nmgv->col;			

			CString	symbol, curr;
			symbol = m_grid->GetItemSymbol(nRow, nCol);
			curr = m_grid->GetItemText(nRow, colCURR);
			_inters* pInters = GetData(nRow - 1);
			CString	value = m_grid->GetItemText(nRow, nCol);
			
			CString zeroValue = "0";

			if (symbol.CompareNoCase("###1") == 0) // 보유수량
			{
				value = m_grid->GetItemText(nRow, nCol);
				memset(pInters->xnum, 0x00, sizeof(pInters->xnum));
				memcpy(pInters->xnum, value, min(value.GetLength(), 10));

				if (m_bChgCnd) 
				{
					saveInterest();
				}
			}
			else if (symbol.CompareNoCase("###2") == 0) // 보유단가
			{
				memset(pInters->xprc, 0x00, sizeof(pInters->xprc));
				memcpy(pInters->xprc, value, min(value.GetLength(), 10));

				if (m_bChgCnd) 
				{
					saveInterest();
				}
			}
			else
				Invalidate();
				break;

			
			

			if (m_sonikField > colCURR)
			{
				value = CalcuPyungaSonik(pInters, curr);
				m_grid->SetItemText(nRow, m_sonikField, value);
			}
			
			if (m_suikField > colCURR)
			{
				value = CalcuSuik(pInters, curr);
				m_grid->SetItemText(nRow, m_suikField, value);
			}
			m_bEditWork = TRUE;
		}
		break;

	case GVNM_COLUMNMOVE:
		{
			if (nmgv->row >= m_gridHdrX.GetSize()
					|| nmgv->col >= m_gridHdrX.GetSize())
				break;

			m_pTicker->MoveColAction(nmgv->col, nmgv->row);
			m_pTicker->ResetScrollBars(); 
			m_pTicker->Invalidate();
			m_btDomino.Invalidate(TRUE);
			m_btSAVE.Invalidate(TRUE);
			m_btCLOSE.Invalidate(TRUE);	
			_gridHdr gridHdr;
			
			gridHdr = m_gridHdrX.GetAt(nmgv->col);
			m_gridHdrX.InsertAt(nmgv->row, gridHdr);

			if (nmgv->col > nmgv->row)
				nmgv->col += 1;

			m_gridHdrX.RemoveAt(nmgv->col);
			ReIndexSpecial();

			int	ncnt = m_gridHdrX.GetSize();
			CString	sztmp, szFIELD = _T("");
			int	nWidth;
			m_szWIDTH = _T("");			
			m_szFIELD = _T("");
			for ( int ii = 0 ; ii < ncnt ; ii++ )
			{
				gridHdr = m_gridHdrX.GetAt(ii);				
				nWidth = m_grid->GetColumnWidth(ii);
				if (ii >= colNAME)
				{
					sztmp.Format("%d,", nWidth);
					m_szWIDTH += sztmp;
				}
				if (ii > colCURR)
				{
					sztmp.Format("%s,", gridHdr.symbol);
					if (sztmp.Compare("2111,") == 0)
						sztmp.Format("2023,");
					else if (sztmp.Compare("2115,") == 0)
						sztmp.Format("2024,");
					else if (sztmp.Compare("2116,") == 0)
						sztmp.Format("2033,");
					else if (sztmp.Compare("2112,") == 0)
						sztmp.Format("2027,");

					m_szFIELD += sztmp;
				}
			}

			SaveAsInterField();
		}
		break;

	case GVNM_BEGINDRAG:
		m_drag = nmgv->row; m_drop = -1;
		m_strCode = m_grid->GetItemText(m_drag, colCODE);
		
		TRACE("BEGINDRAG nmgv row : %d ", nmgv->row);

		if (CAST_TREEID(m_kind)->kind == xISSUE)
			CintGrid::m_dropdata.SetCode(m_grid->GetItemText(m_drag, colCODE)); 
		
		break;
	case GVNM_OUTDRAG:
		{	
			if (m_grid->m_bOutPos)
			{
 				DeleteRow(m_drag);
 				m_bEditWork = TRUE;
 				m_grid->m_bOutPos = false;
			}
		}
		break;
	case GVNM_ENDEND:
		{
			//드래그 카운트용 호출
			((CGroupWnd*)m_pGroupWnd)->AddDragInCount();

			int xdrag = m_drag, xdrop = nmgv->row; m_drop = nmgv->row;
			int nIdx = 0;
			CString	code, name;
			
			m_grid->m_bOutPos = false;

			code = CintGrid::m_dropdata.GetCode();
			name = CintGrid::m_dropdata.GetCode();
			
			//종목 중복 등록 허용
			if(((CGroupWnd*)GetParent())->GetOverLapAction() == FALSE)		
			{
				if (!code.IsEmpty() && IsDuplicateCode(code, m_drop, true))
				{
						CCodeDuplicate dlg;
						
						//dlg 항상 열기 옵션 체크 
						if(dlg.DoModal())
						{
							if(dlg.m_bAlert == FALSE)
							{
//								Variant(guideCC, IH::idTOstring(IDS_GUIDE2));
								m_grid->Invalidate(FALSE);
								break;
							}
							else
							{
								//파일에 저장
								((CGroupWnd*)GetParent())->saveOverLap(TRUE);
							}
						}
						else
						{
							m_grid->Invalidate(FALSE);
							break;
						}
				}
			}

			
			if ( m_drop < 0 ) m_drop = m_grid->GetRowCount();
			else if ( m_drop == 0 ) m_drop = 1;

			if ( m_drop - 2 < 0 ) nIdx = 0;
			else nIdx  = m_drop - 2 ;
			
			_inters* pinters = m_inters.GetAt(nIdx);
			if (strlen(pinters->code) != 0)
			{
//				pinters = m_inters.GetAt(m_inters.GetUpperBound());
//				if (strlen(pinters->code) > 0)
				CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
				int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
				int rowcount = 0;
				
				if(nOver == MO_VISIBLE)
				{
					pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
					rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
				}
				else
				{
					rowcount = GetRowcount();
				}
				
				if(rowcount >= MAX_LINE)
				{
	
					Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
					m_grid->Invalidate(FALSE);
					break;
				}
				else
				{
					DeleteRow(MAX_LINE);
				}
				// END DEL
				
				insertInterest(m_drop - 1);
				insertRow(m_drop);
				m_drop -= 1;
				pinters = m_inters.GetAt(m_drop);
			}

			struct _inters InterData;
			memset(&InterData,0x20,sizeof(struct _inters));
			
			InterData.gubn[0] = '0';
			memcpy(InterData.code,code,sizeof(InterData.code));
			memcpy(InterData.name,name,sizeof(InterData.name));
			
			memcpy(pinters, &InterData, sizeof(struct _inters));
			
			m_bEditWork = TRUE;//BUFFET

			m_drag = m_drop = -1;			
			m_seldrop = xdrop;
			sendTransaction();
			m_pGroupWnd->SendMessage(WM_MANAGE, MK_ENDDRAG);

		}
		break;
	case GVNM_ENDDRAG:
		{		
			if (CAST_TREEID(m_kind)->kind == xISSUE)
			{
				m_grid->Invalidate(FALSE);
				break;
			}

			m_grid->m_bOutPos = false;

			CDropData*	pDropData = &CintGrid::m_dropdata;	// drop information
			CintGrid*	pGrid = pDropData->GetGrid();	// drag grid
			if (pGrid == NULL)
				break;
		
			int		nDGrow = pDropData->GetRow();	// drag row
			CGridWnd*	pDropGrid = (CGridWnd*)pDropData->GetGrid()->GetParent();	// drag gridwnd	
			CGroupWnd*	pDropGroup = (CGroupWnd*)pDropGrid->GetParent();		// drag groupwnd
			int xdrag = m_drag, xdrop = nmgv->row;
			int nIdx = 0 ;//BUFFET
			CString xCODE;
			CString	code, name;
			BOOL	bNewsDrop = FALSE;
			UINT	kind = (UINT)pDropGrid->SendMessage(WM_MANAGE, MK_GETDATAKIND);
			if (CAST_TREEID(kind)->kind == xISSUE)
				bNewsDrop = TRUE;
			if (bNewsDrop)
				xCODE = CintGrid::m_dropdata.GetCode();
			m_drop = nmgv->row;
				
			if (pDropData->GetGrid()->m_hWnd == m_grid->m_hWnd) // m_drag > 0 && m_drop > 0)		// 동일한 화면에서 drop
			{
				if ( m_drop < 0 ) m_drop = m_grid->GetRowCount()-1;
				else if ( m_drop == 0 ) m_drop = 1;

				if (m_drag == m_drop)
					break;
				bool	bUp = false;
				bool	bEmpty = false;
				LPARAM	lDrag = 0;
				lDrag = m_grid->GetItemData(m_drag, colSIG);
				struct _inters*	pDrag = m_inters.GetAt(m_drag - 1);
				struct _inters	dragInter;
				memcpy(&dragInter, pDrag, sizeof(struct _inters));
				
				if (m_drag > m_drop)
					bUp = true;

				_inters* pTemp = m_inters.GetAt(m_drop - 1);
				
				if (strlen(pTemp->code) == 0)
					bEmpty = true;

				// ADD PSH 20070914
				UINT uAttr;
				// END ADD

				if (bEmpty)
				{
					pTemp = m_inters.GetAt(m_drop - 1);
					memcpy(pTemp, &dragInter, sizeof(struct _inters));
					m_grid->SetItemData(m_drop, colSIG, lDrag);
					memset(pDrag, 0x00, sizeof(struct _inters));
					m_grid->SetItemData(m_drag, colSIG, 0);
				}
				else
				{
					if (bUp)
					{
						insertInterest(m_drop - 1);
						insertRow2(m_drop, m_drag);    //이동 이전의 값을 이용해서 북마크 칠해야할지 결정
						
						m_drag++;
						// ADD PSH 20070914
						uAttr = m_grid->GetItemAttr(m_drag, colNAME);
						m_grid->SetItemAttr(m_drop, colNAME, uAttr);

						if (GVAT_MARKER & uAttr)
						{
							m_grid->SetItemText(m_drop, colNAME, dragInter.name);
						}

						// END ADD
						DeleteRow(m_drag);
						pTemp = m_inters.GetAt(m_drop - 1);
					}
					else
					{
						m_drop++;

						insertInterest(m_drop - 1);
						insertRow2(m_drop, m_drag);    //이동 이전의 값을 이용해서 북마크 칠해야할지 결정

						// ADD PSH 20070914
						uAttr = m_grid->GetItemAttr(m_drag, colNAME);
						m_grid->SetItemAttr(m_drop, colNAME, uAttr);

						if (GVAT_MARKER & uAttr)
						{
							m_grid->SetItemText(m_drop, colNAME, dragInter.name);
						}

						// END ADD
						pTemp = m_inters.GetAt(m_drop - 1);
					}
					
					memcpy(pTemp, &dragInter, sizeof(struct _inters));
					m_grid->SetItemData(m_drop, colSIG, lDrag);
					
					if (!bUp)
						DeleteRow(m_drag);
				}
			}
			else if (pDropData->GetGrid()->m_hWnd != m_grid->m_hWnd)//(m_drag < 0 && m_drop > 0)	// 다른 화면에서 drop
			{
				
				code = (bNewsDrop) ? xCODE : pGrid->GetItemText(nDGrow, colCODE);
				name = pGrid->GetItemText(nDGrow, colNAME);

				if ( m_drop < 0 ) m_drop = m_grid->GetRowCount();
				else if ( m_drop == 0 ) m_drop = 1;

				//종목 중복 등록 허용
				if(((CGroupWnd*)GetParent())->GetOverLapAction() == FALSE)		
				{
					if (!code.IsEmpty() && IsDuplicateCode(code, m_drop, true))
					{
						CCodeDuplicate dlg;
						
						//dlg 항상 열기 옵션 체크 
						if(dlg.DoModal())
						{
							if(dlg.m_bAlert == FALSE)
							{
//								Variant(guideCC, IH::idTOstring(IDS_GUIDE2));
								m_grid->Invalidate(FALSE);
								break;
							}
							else
							{
								//파일에 저장
								((CGroupWnd*)GetParent())->saveOverLap(TRUE);
							}
						}
						else
						{
							m_grid->Invalidate(FALSE);
							break;
						}
					}
				}

				if ( m_drop - 2 < 0 ) nIdx = 0;
				else nIdx  = m_drop - 2 ;

				_inters* pinters = m_inters.GetAt(nIdx);
//				_inters* pinters = m_inters.GetAt(m_drop - 1);
				if (strlen(pinters->code) != 0)
				{
//					pinters = m_inters.GetAt(m_inters.GetUpperBound());

//					if (strlen(pinters->code) > 0)
					CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
					int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
					int rowcount = 0;
					
					if(nOver == MO_VISIBLE)
					{
						pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
						rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
					}
					else
					{
						rowcount = GetRowcount();
					}

					if(rowcount >= MAX_LINE)
					{
						Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
						m_grid->Invalidate(FALSE);
						break;
					}
					else
					{
						DeleteRow(MAX_LINE);
					}
					// END DEL
					
					insertInterest(m_drop - 1);
					insertRow(m_drop);
					m_drop -= 1;
					pinters = m_inters.GetAt(m_drop);
				}
				//그리드에 한줄도 없을 경우 임의로 한줄 추가 생성
				else
				{
					if(nIdx == 0 && m_rowC < 2)
					{
						insertRow(m_drop);
					}
				}

				struct _inters*	pInterDrag = pDropGrid->GetData(nDGrow - 1);
				memcpy(pinters, pInterDrag, sizeof(struct _inters));

				// ADD PSH 20070918
				UINT uAttr;
				if ('m' == pinters->gubn[0])
				{
					uAttr = m_grid->GetItemAttr(m_drop+1, colNAME) | GVAT_MARKER;
					m_grid->SetItemAttr(m_drop+1, colNAME, uAttr);
					m_grid->SetItemText(m_drop+1, colNAME, pinters->name);
				}
				else
				{
					uAttr = m_grid->GetItemAttr(m_drop+1, colNAME) & ~GVAT_MARKER;
					m_grid->SetItemAttr(m_drop+1, colNAME, uAttr);
					m_grid->SetItemText(m_drop+1, colNAME, pinters->name);
				}
				// END ADD
				
				LPARAM	data = pGrid->GetItemData(nDGrow, colSIG);
				m_grid->SetItemData(xdrop, colSIG, data);

				// index가 하나 차이가 나므로
				if (pDropGroup->m_hWnd == m_pGroupWnd->m_hWnd)
				{
					if (!bNewsDrop)
						pDropGrid->SendMessage(WM_MANAGE, MAKEWPARAM(MK_DELETEROW, nDGrow));
				}
				
//				pDropGrid->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SAVE, nDGrow), GVNM_ENDDRAG);
			}

			if (!m_bSorting)
			{
//				saveInterest();
				m_bEditWork = TRUE;

			}
			else
			{
// 				CString test;
// 				test.Format("m_bMoveCfg %d m_nMoveSave %d", m_bMoveCfg, m_nMoveSave);
	

				if (m_bMoveCfg && (1 == m_nMoveSave))
				{
					RemoveAllBaseInters();
					m_grid->SortBase();
					saveInterest();

				}
				else if (!m_bMoveCfg)
				{
					m_bMoveCfg  = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVECFG);
					m_nMoveSave = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVESAVE);

					//091013 수정
					CMoveSetupDlg dlg;
					dlg.m_nOption = m_nMoveSave;
					dlg.m_bAlert  = m_bMoveCfg;
					
					//dlg 항상 열기 옵션 체크 
					if(dlg.m_bAlert)
					{
						if (1 == dlg.m_nOption) 
						{
							RemoveAllBaseInters();
							m_grid->SortBase();
							saveInterest();
							m_bEditWork = false;
						}
						else 
						{
							m_bEditWork = true;
						}
					}
					else
					{
						if (dlg.DoModal())
						{
							if (1 == dlg.m_nOption) 
							{
								RemoveAllBaseInters();
								m_grid->SortBase();
								saveInterest();
								m_bEditWork = false;
							}
							else
							{
								m_bEditWork = true;
							}
						}
					}

					m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVECFG, dlg.m_bAlert);
					m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVESAVE, dlg.m_nOption);

				}
			}
			
			m_drag = m_drop = -1;			
			m_seldrop = xdrop;
			sendTransaction();
			m_pGroupWnd->SendMessage(WM_MANAGE, MK_ENDDRAG);
		}
		break;
	case GVNM_SELCHANGED:
		{
			CString code, string; 
			int kind = 0;
			if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
			{
				code = m_grid->GetItemText(nmgv->row, colCODE);
				code.TrimLeft(); 
				code.TrimRight();

				if (code.IsEmpty()) break;
				
				if (((CGroupWnd*)m_pGroupWnd)->GetSelAction() == 0)
					break;

				switch (GetCodeType(code))
				{
				case kospiCODE:
					string.Format("%s\t%s", HCOD, code);
					 break;
				case futureCODE:
					string.Format("%s\t%s", FCOD, code);
					break;
				case optionCODE:
					string.Format("%s\t%s", OCOD, code);
					break;
				case foptionCODE:
					string.Format("%s\t%s", PCOD, code);
					break;
				case indexCODE:
					string.Format("%s\t%s", UCOD, code);
					break;
				case sinjuCODE:
					string.Format("%s\t%s", SINJUCOD, code);
					break;
				case elwCODE:
					string.Format("%s\t%s", HCOD, code);
					Variant(triggerCC, string);
					string.Format("%s\t%s", ELWCOD, code);
					m_grid->SetFocus();
					break;
				case thirdCODE:
					string.Format("%s\t%s", THIRDCOD, code);					
					break;
				}
				
				TRACE("\n select nmgv row : %d \n", nmgv->row);
				m_grid->SetFocus();
				
				break;
			}
			break;
		}

	case GVNM_TRIGGER:
		{
			CString code = m_grid->GetItemText(nmgv->row, colCODE);
			CString string; 
			int kind = 0;
			//if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
			{
				//code = m_grid->GetItemText(nmgv->row, colCODE);
				code.TrimLeft(); 
				code.TrimRight();

				if (code.IsEmpty()) 
					break;
				
				if (((CGroupWnd*)m_pGroupWnd)->GetSelAction() == 0)
					break;

				switch (GetCodeType(code))
				{
				case kospiCODE:
					string.Format("%s\t%s", HCOD, code);
					 break;
				case futureCODE:
					string.Format("%s\t%s", FCOD, code);
					break;
				case optionCODE:
					string.Format("%s\t%s", OCOD, code);
					break;
				case foptionCODE:
					string.Format("%s\t%s", PCOD, code);
					break;
				case indexCODE:
					string.Format("%s\t%s", UCOD, code);
					break;
				case sinjuCODE:
					string.Format("%s\t%s", SINJUCOD, code);
					break;
				case elwCODE:
					string.Format("%s\t%s", HCOD, code);
					Variant(triggerCC, string);
//					m_grid->SetFocus();
					string.Format("%s\t%s", ELWCOD, code);
					break;
				case thirdCODE:
					string.Format("%s\t%s", THIRDCOD, code);					
					break;
				}
				Variant(triggerCC, string);
				Variant(codeCC, string);
	
				
				TRACE("\nClick point is %d, %d, %s\n\n", nmgv->row, m_ClickRow, string);
 				if(nmgv != NULL)
 				{
 					if(nmgv->row != m_ClickRow)
 					{
 						m_grid->SetFocus();
 						m_ClickRow = nmgv->row;
 					}
 				}

// 				HCURSOR hCursor;
// 				hCursor = AfxGetApp()->LoadStandardCursor(IDC_IBEAM); 
// 				
// 				if(GetCursor() == hCursor)
// 				{	
// 					TRACE("\nIBEAM\n");
// 				}
// 				else
// 				{
// 					TRACE("\nARROW\n");
// 					m_grid->SetFocus();
// 				}
				break;
			}
		}
		break;
	case GVNM_DBLCLICKED:
		{
			CString code, string; int kind = 0;
			if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
			{
				code = m_grid->GetItemText(nmgv->row, colCODE);
				code.TrimLeft(); code.TrimRight();
				if (code.IsEmpty()) break;

				switch (((CGroupWnd*)m_pGroupWnd)->GetDblAction())
				{
				case dblCURR:
					switch (GetCodeType(code))
					{
					case kospiCODE:
						string.Format("%s /S/t0/d%s\t%s", kcurrMAP, HCOD, code);
						openView(typeVIEW, string); break;
					case futureCODE:
						string.Format("%s /S/t0/d%s\t%s", fcurrMAP, FCOD, code);
						openView(typeVIEW, string); break;
					case optionCODE:
						string.Format("%s /S/t0/d%s\t%s", ocurrMAP, OCOD, code);
						openView(typeVIEW, string); break;
					case foptionCODE:
						string.Format("%s /S/t0/d%s\t%s", focurrMAP,PCOD, code);
						openView(typeVIEW, string); break;
					case indexCODE:
						string.Format("%s /S/t0/d%s\t%s", icurrMAP, UCOD, code);
						openView(typeVIEW, string); break;
					case elwCODE:
						string.Format("%s /S/t0/d%s\t%s", ecurrMAP, ELWCOD, code);
						openView(typeVIEW, string); break;
					}
					break;
				case dblCHART:
					string.Format("%s /S /d%s\t%s", chartMAP, CCOD, code);
					openView(typeVIEW, string);
					break;				
				case dblUSER:
					{
						CString	mapname = ((CGroupWnd*)m_pGroupWnd)->GetLinkmap();
						if (mapname.IsEmpty())
							break;
							
						switch (GetCodeType(code))
						{
						case kospiCODE:
							string.Format("%s /S/t0/d%s\t%s", mapname, HCOD, code);
							openView(typeVIEW, string); break;
						case futureCODE:
							string.Format("%s /S/t0/d%s\t%s", mapname, FCOD, code);
							openView(typeVIEW, string); break;
						case optionCODE:
							string.Format("%s /S/t0/d%s\t%s", mapname, OCOD, code);
							openView(typeVIEW, string); break;
						case foptionCODE:
							string.Format("%s /S/t0/d%s\t%s", mapname, PCOD, code);
							openView(typeVIEW, string); break;
						case indexCODE:
							string.Format("%s /S/t0/d%s\t%s", mapname, UCOD, code);
							openView(typeVIEW, string); break;
						}
						break;
					}
				}
			}
		}
		break;
	case GVNM_ENDSORT:
		{
			m_bEditWork = TRUE;
			m_endsort = true; 
			m_grid->memoCheck();
			rebuildInterest();
			COLORREF bkColor;
			for (int ii = 0; ii < m_grid->GetRowCount(); ii++)
			{
			//	m_grid->SetRowHeight(ii, ((CGroupWnd*)m_pGroupWnd)->GetRowHeight());
				if (!ii) continue;

				bkColor = ((CGroupWnd*)m_pGroupWnd)->GetBkColor1();
				if (ii > 1 && ((ii -1)/((CGroupWnd*)m_pGroupWnd)->GetLine()) % 2)
					bkColor = ((CGroupWnd*)m_pGroupWnd)->GetBkColor2();
				for (int jj = 0; jj < m_colC; jj++)
					m_grid->SetItemBkColor(ii, jj, bkColor);
			}

			LONG	ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
			hawkeyeAction((ret) ? true : false);
			// DEL PSH 20070913
			//saveInterest();
			// END DEL

			// ADD PSH 20070913
			m_bSorting = TRUE;
			// END ADD
		}
		break;

	case GVNM_RMOUSEDOWN:
		// MODIFY PSH 20070912
		//RbuttonAction(nmgv->row);
		{
			if (0 < nmgv->row)
			{
				RbuttonAction(nmgv->row);
			}
			else
			{
				ShowPopupMenu();
			}
		}
		// END MODIFY
		break;
	// ADD PSH 20070914
	case GVNM_CHANGEMARKER:
		{		
			m_grid->SetFocus();
			CString strMarker, strMarkerCode;
			strMarker = m_grid->GetItemText(nmgv->row, colNAME);

			struct	_inters* pInter;
			int	nMarkerRow  = nmgv->row - 1;

			strMarkerCode.Format("m%05d", nMarkerRow);

			if (!m_bSorting)
			{
				pInter = m_inters.GetAt(nMarkerRow);
				ZeroMemory(pInter->code, sizeof(pInter->code));
				ZeroMemory(pInter->name, sizeof(pInter->name));
				CopyMemory(pInter->code, strMarkerCode.GetBuffer(0), strMarkerCode.GetLength());
				CopyMemory(pInter->name, strMarker.GetBuffer(0), strMarker.GetLength());				

				saveInterest();
			}
		}
		break;
	// END ADD
	}

	return CBaseWnd::OnNotify(wParam, lParam, pResult);
}
bool CGridWnd::IsDuplicateCode(CString code, int nrow, bool drag )
{
	CString string;
	int nMax = m_grid->GetRowCount();

	for (int ii = 1; ii < nMax/*m_rowC*/; ii++)
	{
		if (!drag)
		{
			if (ii == nrow) continue;
		}

		string = m_grid->GetItemText(ii, colCODE);
		if (code == string)
			return true;
	}

	return false;
}

int CGridWnd::GetCodeType(CString code)
{
	int codeL = code.GetLength();
	if (codeL <= 0) return 0;

	int result = m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);

	switch (result)
	{
	case kospiType:				// kospi code
	case kosdaqType:			// kosdaq code
	case etfType:				// etf code
		return	kospiCODE;;
	case elwType:				// ELW
		return	elwCODE;
	case futureType:
		return	futureCODE;		// 선물
	case sfutureType:
		return sfCODE;		
	case callType:				// call option
	case putType:				// put  option
		return  optionCODE;
	case indexType:				// 업종
		return	indexCODE;
	case thirdType:				// 제3시장
		return	thirdCODE;
	case sinjuType:				// 신주인수권
		return	sinjuCODE;
	case mufundType:			// 뮤츄얼펀드
	case reitsType:				// 리츠
	case hyfundType:			// 하이일드펀드
		return 200;
	}

	return 0;
}


int CGridWnd::openView(int type, CString data)
{
	return m_pView->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

void CGridWnd::hawkeyeAction(bool toggle)
{
	CString	code;

	if (toggle)	// hawkeye color
	{
		for (int ii = 1; ii < m_rowC; ii++)
		{
			code = m_grid->GetItemText(ii, colCODE);
			code.TrimLeft(); code.TrimRight();
			if (!code.IsEmpty()) 
				hawkEyeColor(ii);
			else
				SetLineColor(ii);
		}
	}
	else
		SetLineColor();

	m_grid->m_nAlmRow = -1;//BUFFET
	m_grid->memoRefresh();
}


void CGridWnd::RbuttonAction(int row)
{
	if (row == 0)	return;

	const	int userBASE = 100;
	const	int menuBASE = 200;

	m_grid->SetSelectRow(row);
	

	CPoint	point; 
	GetCursorPos(&point);

	int	index = -1, offs;
	char	*ptr, wb[1024];;
	CStringArray items; items.RemoveAll();
	CString string, path, domino, mapN = _T("IB202200");

	CString	code;

	if (m_bPopupDlg)
	{
		CPopupDlg dlg;
		dlg.m_nX = point.x;
		dlg.m_nY = point.y;

		path.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, "IB20");

		DWORD	rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		if (rc <= 0)
		{
			path.Format("%s/%s/%s", m_root, MTBLDIR, "IB20");
			rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		}

		ptr = wb;
		for ( ; rc > 0; ptr++, rc--)
		{
			string = ptr;
			ptr   += string.GetLength();
			rc    -= string.GetLength();
			if (string.GetLength() <= L_MAPN)
			{
				if (string.Find('-') == 0)
					continue;
			}
			mapN   = string.Left(L_MAPN);
			string = string.Mid(L_MAPN);
			offs   = string.Find('=');
			if (offs != -1)
				string = string.Mid(offs+1);

			index = items.Add(mapN);
			dlg.AddMenu(menuBASE+index, string);
		}

		code = m_grid->GetItemText(row, colCODE); 
		code.TrimRight();

		Variant(triggerCC, code);
		m_grid->SetFocus();

		int	dataidx = CAST_TREEID(m_kind)->kind;

		if (dataidx != xISSUE)
		{
			dlg.AddMenu(userBASE+0, "종목 추가");
			dlg.AddMenu(userBASE+1, "종목 삭제");
			dlg.AddMenu(userBASE+2, "종목 변경");
		
			if (!code.IsEmpty())
			{
				dlg.AddMenu(userBASE+3, "메모 입력");
				dlg.AddMenu(userBASE+11, "메모 삭제");
				dlg.AddMenu(userBASE+12, "북마크 지정");
				dlg.AddMenu(userBASE+13, "북마크 삭제");
			}

			dlg.AddMenu(userBASE+9, "책갈피 추가");
			dlg.AddMenu(userBASE+10, "책갈피 삭제");
		}
		
//		dlg.AddMenu(userBASE+14, "복수현재가2 종목연동");
		
		if (dataidx == xINTEREST)
			dlg.AddMenu(userBASE+6, "관심그룹저장");
		else
			dlg.AddMenu(userBASE+7, "관심그룹추가");

		dlg.AddMenu(userBASE+8, "그룹초기화");
		
		dlg.AddMenu(userBASE+4, "연결화면 편집...");
		
		dlg.AddMenu(userBASE+5,  "엑셀로 보내기");

		if (-1 == dlg.DoModal()) return;

		index = dlg.m_nResult;
	}
	else
	{
		CMenu* popM = new CMenu(); 
		popM->CreatePopupMenu();

		path.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, "IB20");

		DWORD	rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		if (rc <= 0)
		{
			path.Format("%s/%s/%s", m_root, MTBLDIR, "IB20");
			rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		}

		ptr = wb;
		for ( ; rc > 0; ptr++, rc--)
		{
			string = ptr;
			ptr   += string.GetLength();
			rc    -= string.GetLength();
			if (string.GetLength() <= L_MAPN)
			{
				if (string.Find('-') == 0)
					popM->AppendMenu(MF_SEPARATOR, 0);
				continue;
			}
			mapN   = string.Left(L_MAPN);
			string = string.Mid(L_MAPN);
			offs   = string.Find('=');
			if (offs != -1)
				string = string.Mid(offs+1);

			index = items.Add(mapN);
			popM->AppendMenu(MF_STRING|MF_ENABLED, menuBASE+index, string);
		}

		if (index >= 0)
		{
			index++;
			popM->AppendMenu(MF_SEPARATOR, 0);
		}

		code = m_grid->GetItemText(row, colCODE); code.TrimRight();

		int	dataidx = CAST_TREEID(m_kind)->kind;

		if (dataidx != xISSUE)
		{
			popM->AppendMenu(MF_STRING, userBASE+0, "종목 추가<INSERT>");
			popM->AppendMenu(MF_STRING, userBASE+1, "종목 삭제<DELETE>");
			popM->AppendMenu(MF_STRING, userBASE+2, "종목 변경");
		
			if (!code.IsEmpty())
			{
				popM->AppendMenu(MF_STRING, userBASE+3, "메모 입력");
				popM->AppendMenu(MF_STRING, userBASE+11, "메모 삭제");
				popM->AppendMenu(MF_STRING, userBASE+12, "북마크 지정");
				popM->AppendMenu(MF_STRING, userBASE+13, "북마크 삭제");
			}
			// MODIFY PSH 20070914
			//popM->AppendMenu(MF_STRING, userBASE+9, "빈줄추가");
			popM->AppendMenu(MF_STRING, userBASE+9, "책갈피 추가");
			popM->AppendMenu(MF_STRING, userBASE+10, "책갈피 삭제");
			// END MODIFY
			popM->AppendMenu(MF_STRING, userBASE+14, "복수현재가2 종목연동");

			popM->AppendMenu(MF_SEPARATOR, 0);
		}
		if (dataidx == xINTEREST)
			popM->AppendMenu(MF_STRING, userBASE+6, "관심그룹저장");
		else
			popM->AppendMenu(MF_STRING, userBASE+7, "관심그룹추가");
		popM->AppendMenu(MF_STRING, userBASE+8, "그룹초기화");
		popM->AppendMenu(MF_SEPARATOR, 0);
		popM->AppendMenu(MF_STRING, userBASE+4, "연결화면 편집...");
		popM->AppendMenu(MF_SEPARATOR, 0);
		popM->AppendMenu(MF_STRING, userBASE+5,  "엑셀로 보내기");

		index = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, point.x, point.y, m_pView);
		delete popM;
	}

	domino = _T("");
	if (!code.IsEmpty())
	{
		switch (GetCodeType(code))
		{
		case kospiCODE:		domino.Format("%s\t%s", HCOD, code);	break;
		case futureCODE:	domino.Format("%s\t%s", FCOD, code);	break;
		case optionCODE:	domino.Format("%s\t%s", OCOD, code);	break;
		case foptionCODE:	domino.Format("%s\t%s", PCOD, code);	break;
		case indexCODE:		domino.Format("%s\t%s", UCOD, code);	break;
		case elwCODE:		domino.Format("%s\t%s", HCOD, code);	break;
		}
	}

	if (index >= menuBASE)
	{
		index -= menuBASE;
		if (index < items.GetSize())
		{
			string = items.GetAt(index);
			string += "/s/t0/d";
			string += domino;
			openView(typeVIEW, string);
		}
		items.RemoveAll();
		return;
	}
	items.RemoveAll();

	switch (index)
	{
	case userBASE+0:	// 추가
		{	
			m_bEditWork = true;
			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
			int rowcount = 0;

			if(nOver == MO_VISIBLE)
			{
				pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
				rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
			}
			else
			{
				rowcount = GetRowcount();
			}
			
			if(rowcount >= MAX_LINE)
			{

				Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
				return;
			}

			insertInterest(row - 1);
			insertRow(row);
			SetLineColor();
 			m_grid->memoCheck(); 
//			m_grid->memoCheck(row+1);
 			m_grid->memoRefresh();
			m_grid->SetFocusCellEdit(row, colNAME, true);

			if (m_bAddCnd) 
			{	
				saveInterest();
			}

		}
		break;
	case userBASE+1:	// 삭제
		{
			m_bEditWork = true;
			if (m_inters.GetSize() >= row -1)
			{
				if (!(m_grid->GetItemAttr(row, colNAME) & GVAT_MARKER))
				{
					DeleteRow(row);	
					insertInterest(MAX_LINE - 1);
					// DEL PSH 20070914
					//insertRow(m_grid->GetRowCount());
					// END DEL
//					saveInterest();
				}
				// END MODIFY
			}
		}
		break;
	case userBASE+2:
		m_grid->SetFocusCellEdit(row, colNAME, true);
		break;
	case userBASE+3:
		m_grid->memoWindowEdit(row);
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_MEMO);
		break;
	case userBASE+4:
		mapN = _T("IB202200");
		m_pView->SendMessage(WM_USER, MAKEWPARAM(linkDLL, 0), (LPARAM)(LPCTSTR)mapN);
		break;
	case userBASE+5:
		if (m_grid) m_grid->RunExcel();
		break;
	case userBASE+6:// 관심그룹저장
		saveInterestX();
		break;
	case userBASE+7:// 관심그룹추가
//		saveInterestX();
		break;
	case userBASE+8:// 그룹화면초기화
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			Reset(false);
		else
			Reset(true);
		break;
	case userBASE+9:// 책갈피 추가
		{	
			m_bEditWork = true;
		
			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
			int rowcount = 0;
			
			if(nOver == MO_VISIBLE)
			{
				pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
				rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
			}
			else
			{
				rowcount = GetRowcount();
			}

			if(rowcount >= MAX_LINE)
			{

				Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
				break;
			}

			insertInterest(row - 1);
			insertRow(row);
			SetLineColor();
			m_grid->memoCheck(); 
//			m_grid->memoCheck(row+1);
			m_grid->memoRefresh();

			UINT attr = m_grid->GetItemAttr(row, colNAME) | GVAT_MARKER;
			m_grid->SetItemAttr(row, colNAME, attr);

			_inters* pInter = m_inters.GetAt(row - 1);

			CString strMarker;
			strMarker.Format("m%05d", row);

			if (pInter)
			{
				pInter->gubn[0] = 'm';
				memcpy(pInter->code, strMarker.GetBuffer(0), strMarker.GetLength());
			}

			m_grid->SetItemText(row, colCODE, strMarker);
			
			m_grid->SetFocusCellEdit(row, colNAME, true);

			if (m_bAddCnd) 
			{
				saveInterest();
			}
		}
		break;
	case userBASE+10:	// 책갈피삭제
		{
			m_bEditWork = true;

			if (m_inters.GetSize() >= row -1)
			{
				if (m_grid->GetItemAttr(row, colNAME) & GVAT_MARKER)
				{
					DeleteRow(row);	
					insertInterest(MAX_LINE - 1);
//					saveInterest();
				}
			}
		}
		break;
	case userBASE+11:	//메모 삭제 091013 수정
		{
			if (MessageBox("메모를 삭제하시겠습니까?\n", "메모 삭제 확인창", MB_YESNO) == IDYES)
			{
				m_grid->delectMemo(row);
			}
		}
		break;
	case userBASE+12:	//북마크 지정 091102 
		{
			UINT uAttr = m_grid->GetItemAttr(row, colNAME);
			m_grid->SetItemAttr(m_drop, colNAME, uAttr);
			
			if (GVAT_MARKER & uAttr)
			{
				
			}
			else
			{
				setBookmark(row);
			}
		}
		break;

	case userBASE+13:	//북마크 삭제
		{
			UINT uAttr = m_grid->GetItemAttr(row, colNAME);
			m_grid->SetItemAttr(m_drop, colNAME, uAttr);
			
			if (GVAT_MARKER & uAttr)
			{
				
			}
			else
			{
				deleteBookmark(row);
			}
			
		}
		break;
	case userBASE+14:	//복수현재가2에 종목 연동하기
		sendtoMutiHoga();
		break;
	// END ADD
	// END ADD
	}
}



void CGridWnd::InsertEmpty(int row)
{
/*	_inters*	pinters = m_inters.GetAt(m_inters.GetUpperBound());
	if (strlen(pinters->code) > 0)
	{*/
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
	int rowcount = 0;
	
	if(nOver == MO_VISIBLE)
	{
		pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
		rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
	}
	else
	{
		rowcount = GetRowcount();
	}

	if(rowcount >= MAX_LINE)
	{
		
		Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
		return;
	}	

	DeleteRow(MAX_LINE);
	insertRow(row);
}

void CGridWnd::hawkEyeColor(int row)
{
	double	curr, open, high, low;
	double	pcurr, popen, phigh, plow, dval;

	curr  = IH::TOfabs(m_grid->GetItemText(row, colCURR));
	open  = IH::TOfabs(m_grid->GetItemText(row, colOPEN));
	high  = IH::TOfabs(m_grid->GetItemText(row, colHIGH));
	low   = IH::TOfabs(m_grid->GetItemText(row, colLOW));

	pcurr = IH::TOfabs(m_grid->GetItemText(row, colPCURR));
	popen = IH::TOfabs(m_grid->GetItemText(row, colPOPEN));
	phigh = IH::TOfabs(m_grid->GetItemText(row, colPHIGH));
	plow  = IH::TOfabs(m_grid->GetItemText(row, colPLOW));
	
	if (curr <= 0) curr = pcurr;
	if (curr <= 0) return;

	// colNAME foreground
	if (open >= phigh)		// 상승갭
	{
		m_grid->SetItemFgColor(row, colNAME, colorKKRED);
	}
	else if (open <= plow)	// 하락갭
	{
		m_grid->SetItemFgColor(row, colNAME, colorKKBLUE);
	}
	else
	{
		m_grid->SetItemFgColor(row, colNAME, GetAxColor(69));
	}

	// colNAME background
	if (curr >= high)
	{
		m_grid->SetItemBkColor(row, colNAME, colorLTRED);
	}
	else if (curr <= low)
	{
		m_grid->SetItemBkColor(row, colNAME, colorLTBLUE);
	}
	else
	{
		m_grid->SetItemBkColor(row, colNAME, GetAxColor(68));
	}

	dval = (phigh - plow) / 3.;

	// colCURR background
	if (curr >= phigh)
	{
		m_grid->SetItemBkColor(row, colCURR, colorDKRED);
	}
	else if (curr >= (plow + dval*2))
	{
		m_grid->SetItemBkColor(row, colCURR, colorLTRED);
	}
	else if (curr >= (plow + dval))
	{
		m_grid->SetItemBkColor(row, colCURR, GetAxColor(68));
	}
	else if (curr >= plow)
	{
		m_grid->SetItemBkColor(row, colCURR, colorLTBLUE);
	}
	else if (curr < plow)
	{
		m_grid->SetItemBkColor(row, colCURR, colorDKBLUE);
	}
	else // default
	{
		TRACE("check  error....\n");
	}
}

void CGridWnd::DeleteRow(int row)
{
	m_grid->DeleteRow(row);
	m_grid->memoDelete(row);
	m_rowC--;	
	RemoveInters(row - 1);
	ReIndexing();
}

void CGridWnd::insertInterest(int idx)
{
	_inters* pinters = (_inters *) new char[sz_inters];
	ZeroMemory(pinters, sz_inters);

	pinters->gubn[0] = '0';
	m_inters.InsertAt(idx, pinters);
}

void CGridWnd::initialGrid(bool size)
{
	if (m_nIndex == 0)
	{
		CintGrid::m_dropdata.Reset();
	}

	m_grid->Initial(m_rowC, m_colC, 1, 0);//colCURR + 1);//BUFFET SCROLL BAR

	// ADD PSH 20070911
	m_grid->SetMarginRatio(m_bMargin);
	m_grid->SetBkMarkerColor(m_clrMarkerBKG);
	m_grid->SetMarkerColor(m_clrMarkerTXT);
	m_grid->SetMarkerShadow(m_bShadow);
	m_grid->SetInfo(m_bInfo);
//	m_grid->SetGridLine(GVLN_VERT);//BUFFET LINE
//	m_grid->SetStepColor(1, GetAxColor(68), GetAxColor(77));//BUFF
////////////////////////////////////////////////////
	char use = ((CGroupWnd*)m_pGroupWnd)->GetBkUse();
	if ( use == '1' || use == '2' )
	{
		int nLine = ((CGroupWnd*)m_pGroupWnd)->GetLine();
		COLORREF bkColor = ((CGroupWnd*)m_pGroupWnd)->GetBkColor1();
		COLORREF bkCol2  = ((CGroupWnd*)m_pGroupWnd)->GetBkColor2();

		if ( nLine <= 0 ) nLine = 1;
		m_grid->SetStepColor(nLine, bkColor, bkCol2);//BUFF
	}else m_grid->SetStepColor(1, GetAxColor(68), GetAxColor(77));//BUFF

///////////////////////////////////////////////////////////	
	m_pTicker->Initial(1, m_colC, 0, colCURR + 1);
	m_pTicker->SetHeader(FALSE);
	m_pTicker->SetEnableSelect(FALSE);
	m_pTicker->SetMarginRatio(m_bMargin);
	m_pTicker->SetInfo(m_bInfo);

	m_nRowHeight = m_grid->GetRowHeight(0);
	// END ADD

	LOGFONT lf, lfB;
	int	fontsize = ((CGroupWnd*)m_pGroupWnd)->GetFontSize();
	CString	fontname = ((CGroupWnd*)m_pGroupWnd)->GetFontName();
	// MODIFY PSH 20070917
	//BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	BOOL	bCurrBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	BOOL	bNameBold = ((CGroupWnd*)m_pGroupWnd)->GetNameBold();
	// END MODIFY

	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);
		
	m_pFont->GetLogFont(&lf); lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	m_pFontB->GetLogFont(&lfB); lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	int	width = 0;
	GVITEM	gvitem; 
	_gridHdr gridHdr;

 	// column header setting
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row   = 0;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		gvitem.format= GVFM_HEADER;
		gvitem.fgcol = GetAxColor(76);	
		gvitem.bkcol = GetAxColor(74);

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		gvitem.text  = IH::idTOstring(gridHdr.stid);
		gvitem.attr  = gridHdr.attr;

		m_grid->SetItem(&gvitem);
		if (ii == colCODE)
		{
			m_grid->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);
			m_pTicker->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);
		}
		else if (ii == colSIG)
		{
			m_grid->SetColumnWidth(ii, ((CGroupWnd*)GetParent())->GetRowHeight());
		}
		else
		{
			if (size)
				m_grid->SetColumnWidth(ii, gridHdr.width);
		}
	}

	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= gridHdr.format;
		gvitem.symbol= gridHdr.symbol; 
		gvitem.attr  = gridHdr.attrx;

		gvitem.fgcol = GetAxColor(69);
		// MODIFY PSH 20070912
		//gvitem.bkcol = GetAxColor(68);		
		gvitem.bkcol = ((CGroupWnd*)m_pGroupWnd)->GetRTMColor();		
		// END MODIFY		

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		// MODIFY PSH 20070911
		if ((ii == colCODE) || (ii == colNAME) || (ii == colCURR))
		// END MODIFY
		{	// 20070706 kwon
			gvitem.attr |= GVAT_CONDITIONx;

			// MODIFY PSH 20070917
			//if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold))
			// END MODIFY
			{
				//gvitem.attr &= ~GVAT_CONDITIONx;
				CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
			}
//			else
//				gvitem.attr |= GVAT_CONDITIONx;
		}

		// ADD PSH 20070911
		m_pTicker->SetColumnItems(ii, &gvitem);

		gvitem.bkcol = GetAxColor(68);
		// END ADD

		if (ii == colNAME)
		{
			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}

		m_grid->SetColumnItems(ii, &gvitem);

		if (ii == colCODE)
		{
			m_grid->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);
			m_pTicker->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);

			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}
		else if (ii == colSIG)
		{
			m_grid->SetColumnWidth(ii, ((CGroupWnd*)GetParent())->GetRowHeight());

			// ADD PSH 20070911
			m_pTicker->SetColumnWidth(ii, ((CGroupWnd*)GetParent())->GetRowHeight());
			// END ADD
		}
		else
		{
			if (size)
			{
				m_grid->SetColumnWidth(ii, gridHdr.width);

				// ADD PSH 20070911
				m_pTicker->SetColumnWidth(ii, gridHdr.width);
				// END ADD
			}
		}
	}
	
	// ADD PSH 20070911
	SetLineColor(0, m_pTicker);	
	m_pTicker->SetBkColor(((CGroupWnd*)m_pGroupWnd)->GetRTMColor());
	m_pTicker->SetBKSelColor(((CGroupWnd*)m_pGroupWnd)->GetRTMColor()); 
	m_pTicker->SetRowColResize(FALSE, TRUE);
	m_pTicker->SetKeepingColumn(colCURR +1);		// keep column count
	m_pTicker->SetGridFocusLine(FALSE, TRUE);
	m_pTicker->SetGridColor(GetAxColor(65));
	m_pTicker->SetConditionColor(GetAxColor(94), GetAxColor(95), GetAxColor(69));
	//m_pTicker->SetTitleTips(TRUE);
	m_pTicker->Adjust();
	// END PSH

	SetLineColor();	
	// ADD PSH 20070918
	m_grid->SetBkColor(RGB(255,255,255));//BUFFET FOR SAME BACKCOLOR
	//m_grid->SetBkColor(GetAxColor(64));
	m_grid->SetFixBkColor(GetAxColor(74));
	// END ADD
	m_grid->SetBKSelColor(GetAxColor(78)); 
	m_grid->SetRowColResize(FALSE, TRUE);
	m_grid->SetKeepingColumn(colCURR +1);		// keep column count
	m_grid->SetGridFocusLine(FALSE, TRUE);
	m_grid->SetGridColor(GetAxColor(65));
	m_grid->SetConditionColor(GetAxColor(CLR_UP), GetAxColor(CLR_DN), GetAxColor(CLR_TEXT));	
	m_grid->SetTitleTips(TRUE);
//	m_grid->SetUseOLE(TRUE);	// Ctrl + C Ctrl + V사용을 위한 옵션 잠시 보류 
	m_grid->Adjust();

	if(m_recomBasicCostField > 0 && ((CGroupWnd*)m_pGroupWnd)->GetIsRecommand() == true)
	{
		m_grid->SetColumnWidth(m_recomBasicCostField, 0);
	}

	if(m_recomBMjisuField > 0 && ((CGroupWnd*)m_pGroupWnd)->GetIsRecommand() == true)
	{
		m_grid->SetColumnWidth(m_recomBMjisuField, 0);
	}
}

// ADD PSH 20070912
void CGridWnd::InitPopupMenu()
{
	m_menuSubHeader.CreatePopupMenu();
	m_menuSubHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND1, "항목추가시 자동저장");
	m_menuSubHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND2, "항목변경시 자동저장");

	m_menuHeader.CreatePopupMenu();
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INTERSAVE, "관심종목 등록하기");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INTERTAKE, "관심종목 덮어쓰기");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_BASESORT, "등록순으로 정렬");
	m_menuHeader.AppendMenu(MF_SEPARATOR);
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MULTICHART, "선택종목 멀티차트 보기");
	m_menuHeader.AppendMenu(MF_SEPARATOR);
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_SHOWRTSWND, "체결리스트 보기");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_CODE,   "종목코드 보기");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INFO,	  "종목특이사항 보기");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MARGIN,   "주식증거금 보기");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_TICKER,   "관심티커 보기");
	m_menuHeader.AppendMenu(MF_SEPARATOR);
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_TICKER_SETUP,   "관심티커 설정");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND1,		"편의기능 설정");
	//m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND1,   "자동저장 설정");
	//m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND2,   "알림소리 설정");
	//m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MARKET_COLOR,   "시장구분 설정");
	//m_menuHeader.InsertMenu(9, MF_POPUP|MF_BYPOSITION, (UINT)m_menuSubHeader.m_hMenu, "자동저장 설정");
	//m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MARKER,			"책갈피 설정");

	m_menuHeader.CheckMenuItem(IDC_MENU_SHOWRTSWND, m_bRTS ? MF_CHECKED : MF_UNCHECKED);
	m_menuHeader.CheckMenuItem(IDC_MENU_CODE, m_bDispCode ? MF_CHECKED : MF_UNCHECKED);
	m_menuHeader.CheckMenuItem(IDC_MENU_INFO, m_bInfo ? MF_CHECKED : MF_UNCHECKED);
	m_menuHeader.CheckMenuItem(IDC_MENU_MARGIN, m_bMargin ? MF_CHECKED : MF_UNCHECKED);
	m_menuHeader.CheckMenuItem(IDC_MENU_TICKER, m_bTicker ? MF_CHECKED : MF_UNCHECKED);
	//m_menuHeader.CheckMenuItem(IDC_MENU_SAVE_CND1, m_bAddCnd ? MF_CHECKED : MF_UNCHECKED);
	//m_menuHeader.CheckMenuItem(IDC_MENU_SAVE_CND2, m_bChgCnd ? MF_CHECKED : MF_UNCHECKED);

	m_menuHeader.SetSelectDisableMode(TRUE);
}

void CGridWnd::SetTicker(BOOL bTicker /* = TRUE */)
{
	m_bTicker = bTicker;

	m_pTicker->ShowWindow(m_bTicker);
	CRect rect;
	m_btSAVE.GetWindowRect(&rect);
	if (m_bTicker)
	{
		m_btDomino.SetParent(m_pTicker);
		m_btSAVE.SetParent(m_pTicker);
		m_btCLOSE.SetParent(m_pTicker);
		//m_btSAVE.SetWindowPos(0, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW );
	}
	else
	{
		//m_btSAVE.SetParent(m_pGroupWnd);
		//m_btCLOSE.SetParent(m_pGroupWnd);
		m_btDomino.SetParent(this);
		m_btSAVE.SetParent(this);
		m_btCLOSE.SetParent(this);
		//m_btSAVE.SetWindowPos(0, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW );
	}

	m_btDomino.ShowWindow(SW_SHOW);	
	m_btDomino.Invalidate();
//	m_btSAVE.BringWindowToTop();
	m_btSAVE.ShowWindow(SW_SHOW);	
	m_btSAVE.Invalidate();
//	m_btCLOSE.BringWindowToTop();
	m_btCLOSE.ShowWindow(SW_SHOW); 
	m_btCLOSE.Invalidate();

	CRect rcWnd;
	GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);

	InvalidateRect(rcWnd);
}

void CGridWnd::SetInfo(BOOL bInfo /* = TRUE */)
{
	m_bInfo = bInfo;

	m_pTicker->ShowWindow(m_bInfo);
	m_grid->ShowWindow(m_bInfo);

	CRect rcWnd;
	GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);

	InvalidateRect(rcWnd);
}

void CGridWnd::ShowPopupMenu(int nX /* = -1 */, int nY /* = -1 */)
{
	if ((nX == -1) || (nY == -1))
	{
		CPoint	point;
		GetCursorPos(&point);

		nX = point.x;
		nY = point.y;
	}

	int nResult;

	if (m_bPopupDlg)
	{
		CPopupDlg dlg;
		dlg.m_nX = nX;
		dlg.m_nY = nY;

		int nResultID[] = {
			IDC_MENU_SAVE_CND1, 
			IDC_MENU_SAVE_CND2,
			IDC_MENU_INTERSAVE,
			IDC_MENU_INTERTAKE,
			IDC_MENU_BASESORT,
			IDC_MENU_MULTICHART,
			IDC_MENU_SHOWRTSWND,
			IDC_MENU_INFO,
			IDC_MENU_MARGIN,
			IDC_MENU_TICKER,
			IDC_MENU_TICKER_SETUP,
			IDC_MENU_SAVE_CND1,
		};

		int nBase = 100;

		dlg.AddMenu(nBase + 2, "관심종목 등록하기");

		if (!m_szTitle.IsEmpty())
			dlg.AddMenu(nBase + 3, "관심종목 덮어쓰기");

		dlg.AddMenu(nBase + 4, "등록순으로 정렬");

		dlg.AddMenu(nBase + 5, "선택종목 멀티차트 보기");
		dlg.AddMenu(nBase + 6, "체결리스트 보기");
		
		dlg.AddMenu(nBase + 7, "종목특이사항 보기", m_bInfo);
		dlg.AddMenu(nBase + 8, "주식증거금 보기", m_bMargin);
		dlg.AddMenu(nBase + 9, "관심티커 보기", m_bTicker);
		
		dlg.AddMenu(nBase + 10, "관심티커 설정");
		dlg.AddMenu(nBase + 11, "편의기능 설정");

		if (-1 == dlg.DoModal()) return;

		nResult = nResultID[dlg.m_nResult - nBase];
	}
	else
	{
		m_menuHeader.EnableMenuItem(IDC_MENU_INTERTAKE, m_szTitle.IsEmpty() ? MF_DISABLED : MF_ENABLED);
		m_menuHeader.EnableMenuItem(IDC_MENU_MARKER, m_bSorting ? MF_DISABLED : MF_ENABLED);

		nResult = m_menuHeader.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, nX, nY, this);
	}

	switch(nResult)
	{
	case IDC_MENU_INTERSAVE:
		{
			saveInterestX();
		}
		break;
	case IDC_MENU_INTERTAKE:
		{
			saveInterest();
		}
		break;
	case IDC_MENU_BASESORT:
		{
			BaseSorting();
		}
		break;
	case IDC_MENU_MULTICHART:
		{
			MultiChart();
		}
		break;
	case IDC_MENU_SHOWRTSWND:
		{
			ShowRTSWnd();
			m_menuHeader.CheckMenuItem(IDC_MENU_SHOWRTSWND, ((CMainWnd*)m_pMainWnd)->GetRTSVisible() ? MF_CHECKED : MF_UNCHECKED);
		}
		break;
	case IDC_MENU_CODE:
		{
			m_pGroupWnd->SendMessage(WM_MANAGE, MK_MARKETCODE, !m_bDispCode);
			m_menuHeader.CheckMenuItem(IDC_MENU_CODE, m_bDispCode ? MF_CHECKED : MF_UNCHECKED);
		}
		break;
	case IDC_MENU_INFO:
		{
			m_bInfo = !m_bInfo;
			m_menuHeader.CheckMenuItem(IDC_MENU_INFO, m_bInfo ? MF_CHECKED : MF_UNCHECKED);
			m_grid->SetInfo(m_bInfo);
			m_pTicker->SetInfo(m_bInfo);
		}
		break;
	case IDC_MENU_MARGIN:
		{
			m_bMargin = !m_bMargin;
			m_menuHeader.CheckMenuItem(IDC_MENU_MARGIN, m_bMargin ? MF_CHECKED : MF_UNCHECKED);
			m_grid->SetMarginRatio(m_bMargin);
			m_grid->Invalidate(false);
			m_pTicker->SetMarginRatio(m_bMargin);
			m_pTicker->Invalidate(false);
		}
		break;
	case IDC_MENU_TICKER:
		{
			SetTicker(!m_bTicker);
			m_menuHeader.CheckMenuItem(IDC_MENU_TICKER, m_bTicker ? MF_CHECKED : MF_UNCHECKED);
		}
		break;
	case IDC_MENU_TICKER_SETUP:
		{
			Csetup dlg(NULL, &m_tkConfig);
			dlg.DoModal();
		}
		break;
	case IDC_MENU_SAVE_CND1:
		{
			CUsefulSetupDlg dlg;

			dlg.m_bClrApply		= m_bMKClrApply;
			dlg.m_clrKospi		= m_clrKospi;
			dlg.m_clrKosdaq		= m_clrKosdaq;

			dlg.m_clrMarkerBKG	= m_clrMarkerBKG;
			dlg.m_clrMarkerTXT	= m_clrMarkerTXT;
			dlg.m_bShadow		= m_bShadow;
			dlg.m_bMKAllApply	= m_bAllApply;

			dlg.m_strFile		= m_strWaveFile;
			dlg.m_bWaveApply	= m_bWaveApply;

			dlg.m_bAllApply		= m_bAutoSaveAllApply;
			dlg.m_bAddCnd		= m_bAddCnd;
			dlg.m_bChgCnd		= m_bChgCnd;
			
			if (dlg.DoModal())
			{
				if (-1 != dlg.m_strFile.Find("psh0419"))
				{
					CString strTemp;

					if (NULL == m_pRCDlg)
					{
						m_pRCDlg = new CRealCheckDlg;
						m_pRCDlg->Create(IDD_REALTIME_CHECK);
						m_pRCDlg->ShowWindow(SW_SHOW);

						m_pRCDlg->m_stcTitle.SetWindowText(m_szTitle);
						m_pRCDlg->m_pGridWnd = this;
					}
					else
					{
						m_pRCDlg->ShowWindow(SW_SHOW);
					}

					int nCnt = m_pRCDlg->m_listGrid.GetCount();

					for (int i = nCnt - 1; i >= 0; i--)
					{
						m_pRCDlg->m_listGrid.DeleteString(i);
					}

					m_pRCDlg->m_edSymbol.SetWindowText("");
					m_pRCDlg->m_edData.SetWindowText("");

					for (int i = 1; i < m_rowC; i++)
					{
						strTemp.Format("%03d : %s - %s", i, m_grid->GetItemText(i, colRCOD), m_grid->GetItemText(i, colCODE));
						m_pRCDlg->m_listGrid.AddString(strTemp);
					}

					break;
				}

				if (dlg.m_bClrApply	!= m_bMKClrApply ||
					dlg.m_clrKospi	!= m_clrKospi ||
					dlg.m_clrKosdaq	!= m_clrKosdaq)
				{
					m_pGroupWnd->SendMessage(WM_MANAGE, MK_MARKETCOLOR, (LPARAM)&dlg);
				}

				if (dlg.m_clrMarkerBKG != m_clrMarkerBKG ||
					dlg.m_clrMarkerTXT != m_clrMarkerTXT ||
					dlg.m_bShadow	   != m_bShadow ||
					dlg.m_bMKAllApply  != m_bAllApply)
				{
					_marker mkProp;
					mkProp.clrMarkerBKG = dlg.m_clrMarkerBKG;
					mkProp.clrMarkerTXT = dlg.m_clrMarkerTXT;
					mkProp.bShadow		= dlg.m_bShadow;
					mkProp.bAllApply	= dlg.m_bMKAllApply;

					if (dlg.m_bMKAllApply)
					{
						m_pGroupWnd->SendMessage(WM_MANAGE, MK_MKPROP, (LPARAM)&mkProp);
					}
					else
					{
						SetMarkerProp(&mkProp);
					}
				}

				if (dlg.m_strFile	 != m_strWaveFile ||
					dlg.m_bWaveApply != m_bWaveApply)
				{
					m_pGroupWnd->SendMessage(WM_MANAGE, MK_ALERTWAVE, (LPARAM)&dlg);
				}

				if (dlg.m_bAllApply != m_bAutoSaveAllApply ||
					dlg.m_bAddCnd	!= m_bAddCnd ||
					dlg.m_bChgCnd	!= m_bChgCnd)
				{
					m_pGroupWnd->SendMessage(WM_MANAGE, MK_AUTOSAVE, (LPARAM)&dlg);
				}
			}
		}
		break;
	}
}

bool CGridWnd::GetCodeMap(CMap <CString, LPCTSTR, int, int>& arCodeMap)
{
	CString code;
	if(m_rowC <= 1) return false;

	for (int ii = 1; ii < m_rowC; ii++)
	{
		code = m_grid->GetItemText(ii, colCODE);
		if (code != "")
		{
			arCodeMap.SetAt(code, ii);
		}
		
	}

	return true;

}


void CGridWnd::BaseSorting()
{

	CMap <CString, LPCTSTR, int, int> mapBaseSort, mapCurCode;
	CString strIndex, strCode;

	int nCnt = m_arrBaseInters.GetSize();
	if (1 > nCnt) return;
	if( !GetCodeMap(mapCurCode)) return;


	_inters* pInter = NULL;
	for (int i = 0; i < nCnt; i++)
	{
		if (pInter = m_arrBaseInters.GetAt(i))
		{
			strCode = pInter->code;
		}

		if (!strCode.IsEmpty())
		{
			mapBaseSort.SetAt(strCode, i);
		}
	}

	
	m_grid->SortBase(mapBaseSort, mapCurCode);
	/*
	CMapStringToString mapBaseSort;
	_inters* pInter = NULL;
	CString strIndex, strCode;

	int nCnt = m_arrBaseInters.GetSize();

	if (1 > nCnt)
	{
		return;
	}

	for (int i = 0; i < nCnt; i++)
	{
		if (pInter = m_arrBaseInters.GetAt(i))
		{
			strCode = pInter->code;
		}

		if (!strCode.IsEmpty())
		{
			strIndex.Format("%d", i + 1);
			mapBaseSort.SetAt(strCode, strIndex);
			strCode.Empty();
		}

		delete pInter;
	}

	
	m_grid->SortBase(&mapBaseSort);
*/
	m_bSorting = FALSE;
	m_arrBaseInters.RemoveAll();
}
// END ADD

void CGridWnd::SetLineColor()
{
	for (int ii = 0; ii < m_grid->GetRowCount(); ii++)
	{
		SetLineColor(ii);
	}
}

// MODIFY PSH 20070911
/*void CGridWnd::SetLineColor(int nRow)
{	
	COLORREF	bkColor;
	m_grid->SetRowHeight(nRow, ((CGroupWnd*)m_pGroupWnd)->GetRowHeight());
	if (!nRow)
		return;

	bkColor = ((CGroupWnd*)m_pGroupWnd)->GetBkColor1();
	if (nRow > 1 && ((nRow -1)/((CGroupWnd*)m_pGroupWnd)->GetLine()) % 2)
		bkColor = ((CGroupWnd*)m_pGroupWnd)->GetBkColor2();
	
	for (int jj = 0; jj < m_colC; jj++)
	{
		if (jj == colNAME)
			m_grid->SetItemFgColor(nRow, colNAME, GetAxColor(69));
		
		m_grid->SetItemBkColor(nRow, jj, bkColor);
	}
}
*/

void CGridWnd::SetLineColor(int nRow, CintGrid* pGrid)
{	
	if (NULL == pGrid) pGrid = m_grid;

	COLORREF	bkColor;
	pGrid->SetRowHeight(nRow, ((CGroupWnd*)m_pGroupWnd)->GetRowHeight());

	if (!nRow)	return;

	bkColor = ((CGroupWnd*)m_pGroupWnd)->GetBkColor1();

	if (nRow > 1 && ((nRow -1)/((CGroupWnd*)m_pGroupWnd)->GetLine()) % 2)
		bkColor = ((CGroupWnd*)m_pGroupWnd)->GetBkColor2();
	
	for (int jj = 0; jj < m_colC; jj++)
	{
		if (jj == colNAME)
			pGrid->SetItemFgColor(nRow, colNAME, GetAxColor(69));
		
		pGrid->SetItemBkColor(nRow, jj, bkColor);
	}
}
// END MODIFY

//북마크 색 지정하기
void CGridWnd::setBookmark(int row)
{
	LOGFONT lf, lfB;
	int	fontsize = ((CGroupWnd*)m_pGroupWnd)->GetFontSize();
	CString	fontname = ((CGroupWnd*)m_pGroupWnd)->GetFontName();
	
	BOOL	bCurrBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	BOOL	bNameBold = ((CGroupWnd*)m_pGroupWnd)->GetNameBold();
	
	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);
	
	m_pFont->GetLogFont(&lf); 
	lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	
	m_pFontB->GetLogFont(&lfB); 
	lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	
	GVITEM	gvitem; 
	_gridHdr gridHdr;
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);
		
		gvitem.row   = row;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;
		
		gvitem.format= gridHdr.format;
		gvitem.symbol= gridHdr.symbol; 
		gvitem.attr  = gridHdr.attrx;
		gvitem.fgcol = ((CGroupWnd*)m_pGroupWnd)->GetBookMarkPenColor();
		gvitem.bkcol = ((CGroupWnd*)m_pGroupWnd)->GetBookMarkColor();
		
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		
		if ((ii == colCODE) || (ii == colCURR) || (ii == colNAME))
		{	// 20070706 kwon
			gvitem.attr |= GVAT_CONDITIONx;
			
			// MODIFY PSH 20070917
			//if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold))
				// END MODIFY
			{
				//gvitem.attr &= ~GVAT_CONDITIONx;
				CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
			}
			//			else
			//				gvitem.attr |= GVAT_CONDITIONx;
		}
		if (ii == colNAME)
		{
			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}
		
		m_grid->SetItem(&gvitem);
	}
	
	m_grid->SetGridFocusLine(FALSE, TRUE);
	
	
	//북마크 지정
	struct _inters*	pInter; 
	pInter = m_inters.GetAt(row-1);
	pInter->bookmark[0] = '1';
	m_inters.SetAt(row-1, pInter);
	
	saveInterest();
	sendTransaction(row-1);
}


void CGridWnd::deleteBookmark(int row)
{
	LOGFONT lf, lfB;
	int	fontsize = ((CGroupWnd*)m_pGroupWnd)->GetFontSize();
	CString	fontname = ((CGroupWnd*)m_pGroupWnd)->GetFontName();
	
	BOOL	bCurrBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	BOOL	bNameBold = ((CGroupWnd*)m_pGroupWnd)->GetNameBold();
	
	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);
	
	m_pFont->GetLogFont(&lf); 
	lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	
	m_pFontB->GetLogFont(&lfB); 
	lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	
	GVITEM	gvitem; 
	_gridHdr gridHdr;
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);
		
		gvitem.row   = row;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;
		
		gvitem.format= gridHdr.format;
		gvitem.symbol= gridHdr.symbol; 
		gvitem.attr  = gridHdr.attrx;
		gvitem.fgcol = GetAxColor(69);
		gvitem.bkcol = GetAxColor(68);	
		
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		
		if ((ii == colCODE) || (ii == colCURR) || (ii == colNAME))
		{	// 20070706 kwon
			gvitem.attr |= GVAT_CONDITIONx;
			
			// MODIFY PSH 20070917
			//if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold))
				// END MODIFY
			{
				//gvitem.attr &= ~GVAT_CONDITIONx;
				CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
			}
			//			else
			//				gvitem.attr |= GVAT_CONDITIONx;
		}
		if (ii == colNAME)
		{
			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}
		
		m_grid->SetItem(&gvitem);
	}
	
	m_grid->SetGridFocusLine(FALSE, TRUE);

	//북마크 지정
	struct _inters*	pInter; 
	pInter = m_inters.GetAt(row-1);
	pInter->bookmark[0] = '0';
	m_inters.SetAt(row-1, pInter);
	
	saveInterest();
	m_grid->Invalidate(FALSE);
	sendTransaction(row-1);
}

COLORREF CGridWnd::GetBookMarkColor()
{
	return ((CGroupWnd*)m_pGroupWnd)->GetBookMarkColor();
}

COLORREF CGridWnd::GetBookMarkPenColor()
{
	return ((CGroupWnd*)m_pGroupWnd)->GetBookMarkPenColor();
}

int CGridWnd::insertRow2(int afterRow, int beforeRow, BOOL bInsertInter)
{
	if (afterRow > m_grid->GetRowCount())
		afterRow = m_grid->GetRowCount();
	
	// ADD PSH 20070914
	m_rowC++;
	// END ADD
	
	int	ret = m_grid->InsertRow("", afterRow);
	
	struct _inters*	pInter; 
	pInter = m_inters.GetAt(beforeRow);
	
	//	if (bInsertInter)
	//		insertInterest(row - 1);
	
	LOGFONT lf, lfB;
	int	fontsize = ((CGroupWnd*)m_pGroupWnd)->GetFontSize();
	CString	fontname = ((CGroupWnd*)m_pGroupWnd)->GetFontName();

	BOOL	bCurrBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	BOOL	bNameBold = ((CGroupWnd*)m_pGroupWnd)->GetNameBold();
	
	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);
	
	m_pFont->GetLogFont(&lf); 
	lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	m_pFontB->GetLogFont(&lfB); 
	lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	
	GVITEM	gvitem; 
	_gridHdr gridHdr;
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);
		
		gvitem.row   = afterRow;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;
		
		gvitem.format= gridHdr.format;
		gvitem.symbol= gridHdr.symbol; 
		gvitem.attr  = gridHdr.attrx;
		gvitem.fgcol = GetAxColor(69);
		
		//북마크되어있는지 확인
		if(pInter->bookmark[0] == '1')
		{
			gvitem.bkcol = ((CGroupWnd*)m_pGroupWnd)->GetBookMarkColor();	
		}
		else
		{
			gvitem.bkcol = ((CGroupWnd*)m_pGroupWnd)->GetBookMarkPenColor();	
		}
		
		
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		
		if ((ii == colCODE) || (ii == colCURR) || (ii == colNAME))
		{	// 20070706 kwon
			gvitem.attr |= GVAT_CONDITIONx;
			
			// MODIFY PSH 20070917
			//if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold))
				// END MODIFY
			{
				//gvitem.attr &= ~GVAT_CONDITIONx;
				CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
			}
			//			else
			//				gvitem.attr |= GVAT_CONDITIONx;
		}
		if (ii == colNAME)
		{
			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}
		
		m_grid->SetItem(&gvitem);
	}
	
	m_grid->SetGridFocusLine(FALSE, TRUE);
	
	return ret;
}

int CGridWnd::insertRow(int row, BOOL bInsertInter /* = TRUE */)
{
	if (row > m_grid->GetRowCount())
		row = m_grid->GetRowCount();

	// ADD PSH 20070914
	m_rowC++;
	// END ADD

	LOGFONT lf, lfB;
	int	fontsize = ((CGroupWnd*)m_pGroupWnd)->GetFontSize();
	CString	fontname = ((CGroupWnd*)m_pGroupWnd)->GetFontName();
	// MODIFY PSH 20070917
	//BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	BOOL	bCurrBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	BOOL	bNameBold = ((CGroupWnd*)m_pGroupWnd)->GetNameBold();

	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);
	
	m_pFont->GetLogFont(&lf); lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	m_pFontB->GetLogFont(&lfB); lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	int	ret = m_grid->InsertRow("", row);

	struct _inters*	pInter; 
	
	if(row >= m_inters.GetSize())
	{
		return 0;
	}

	pInter = m_inters.GetAt(row);
	

//	if (bInsertInter)
//		insertInterest(row - 1);
	
	// END MODIFY
	

	GVITEM	gvitem; 
	_gridHdr gridHdr;
	for (int ii = 0; ii < m_colC; ii++)
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row   = row;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= gridHdr.format;
		gvitem.symbol= gridHdr.symbol; 
		gvitem.attr  = gridHdr.attrx & ~GVAT_MARKER;
		gvitem.fgcol = GetAxColor(69);
		
		//북마크되어있는지 확인
		if(pInter->bookmark[0] == '1')
		{
			gvitem.bkcol = ((CGroupWnd*)m_pGroupWnd)->GetBookMarkColor();	
		}
		else
		{
			gvitem.bkcol = GetAxColor(68);	
		}
		/*
		if(pInter->code[0] == 'm')
		{
			if (ii == colCODE ||ii == colRCOD)
			{
				gvitem.text = pInter->code;
			}
		}
		*/
	
		
		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		if ((ii == colCODE) || (ii == colCURR) || (ii == colNAME))
		{	// 20070706 kwon
			gvitem.attr |= GVAT_CONDITIONx;
			
			// MODIFY PSH 20070917
			//if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold))
			// END MODIFY
			{
				//gvitem.attr &= ~GVAT_CONDITIONx;
				CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
			}
//			else
//				gvitem.attr |= GVAT_CONDITIONx;
		}
		if (ii == colNAME)
		{
			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}

		m_grid->SetItem(&gvitem);
	}

	m_grid->SetGridFocusLine(FALSE, TRUE);

	return ret;
}




void CGridWnd::parsingField(CString symbol, bool bDlgSetup/* = false*/)
{
	CString	string;
	const int gridHdrC = sizeof(gridHdr) / sizeof(_gridHdr);
	m_gridHdrX.RemoveAll();
	

	for (int ii = 0; ii < gridHdrC; ii++)
	{
		if (gridHdr[ii].needs == 2)
		{
			m_gridHdrX.Add(gridHdr[ii]);
		}
	}

	if (symbol.IsEmpty())
	{
		m_szFIELD = _T(""), m_szWIDTH = _T("");
		for (int ii = 0; ii < gridHdrC; ii++)
		{
			if (gridHdr[ii].needs == 1)	// default
				m_gridHdrX.Add(gridHdr[ii]);
		}
		return;
	}

	if (!symbol.IsEmpty()) IH::Parser(symbol, deliMITER);		// field name 제거
	
	m_posField = false; 
	m_ccField = false;
	int	nIndex = 0;
	CString	sztmp = _T(""), sztmp2;
	CMapStringToString	mapSymbol;
	CWordArray		arWidth;
	CStringArray		arSymbol;	
	
	int	ncnt = m_gridHdrX.GetSize();
	int	wcnt = 0;
	struct _gridHdr	gHdr;
	bool	bField = true;

	sztmp.Format("%s", symbol);
	m_szFIELD = sztmp;

	while (!sztmp.IsEmpty())
	{
		string = IH::Parser(sztmp, deliMITER);
		arSymbol.Add(string);
	}

	sztmp.Format("%s", m_szWIDTH);
	while (sztmp.GetLength())
	{
		string = IH::Parser(sztmp, ",");

		if (string.IsEmpty())
			continue;

		arWidth.Add(atoi(string));
	}

	wcnt = arWidth.GetSize();
	sztmp.Format("%s", m_szFIELD);
	while (sztmp.GetLength())
	{
		string = IH::Parser(sztmp, ",");
		if (string.IsEmpty())
			continue;

		mapSymbol.SetAt(string, "");
	}		

	wcnt = mapSymbol.GetCount();

	if (!bDlgSetup && wcnt == arSymbol.GetSize())
	{
		bField = true;

		CString strSym;
		for ( int ii = 0 ; ii < wcnt ; ii++ )
		{
			strSym = arSymbol.GetAt(ii);
			if (!mapSymbol.Lookup(strSym, sztmp2))
			{
				if ((strSym == "#1##") || (strSym == "#4##") ||
					(strSym == "#2##") || (strSym == "#5##") ||
					(strSym == "#3##") || (strSym == "#6##"))
				{
					continue;
				}

				bField = false;
				break;
			}			
		}	
		
		if (bField)
		{
			symbol.Format("%s", m_szFIELD);
			symbol.Replace(",", ";");
		}

	}
	
	while (!symbol.IsEmpty())
	{
		string = IH::Parser(symbol, deliMITER);
		if (string.IsEmpty()) continue;

		for (int ii = 0; ii < gridHdrC; ii++)
		{
			if (gridHdr[ii].needs == 2)
				continue;

			if (string.Compare(gridHdr[ii].symbol))
				continue;

			nIndex = m_gridHdrX.Add(gridHdr[ii]);
			sztmp.Format("%d", gridHdr[ii].stid);			

			if ((gridHdr[ii].needs == 1) || (gridHdr[ii].needs == 3))
			{
				if ((string.Compare("#1##") == 0) || (string.Compare("#4##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2029");
				}
				else if ((string.Compare("#2##") == 0) || (string.Compare("#5##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2030");
				}
				else if ((string.Compare("#3##") == 0) || (string.Compare("#6##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2031");
				}
			}

			if (gridHdr[ii].needs == 9)
			{
				m_posField = true;
				
				if (string.Compare("###3") == 0)
					m_sonikField = nIndex;
				if (string.Compare("###4") == 0)
					m_suikField = nIndex;	
			}

			if (gridHdr[ii].needs == 8)
				m_ccField = true;						
			
			if (gridHdr[ii].needs == 7)
				m_bongField = nIndex;

			if (gridHdr[ii].needs == 10)
				m_yDayVolField = nIndex;

			if (gridHdr[ii].needs == 11)
				m_EqualizerField = nIndex;
			
			if (gridHdr[ii].needs == 12)
				m_recomDataField = nIndex;

			if (gridHdr[ii].needs == 13)
				m_recomBasicCostField = nIndex;

			if (gridHdr[ii].needs == 14)
				m_recomSuikrateField = nIndex;

			if (gridHdr[ii].needs == 15)
				m_recomBMjisuField = nIndex;

			if (gridHdr[ii].needs == 16)
				m_recomBMSuikrateField = nIndex;

			if (gridHdr[ii].needs == 17)
				m_recomgoalCostField = nIndex;
			break;
		}
	}


	if (bField)
	{		
		ncnt = m_gridHdrX.GetSize();
		wcnt = arWidth.GetSize();

		if ((ncnt - colNAME) == wcnt)
		{
			for (int  ii = 0 ; ii < wcnt ; ii++ )
			{
				gHdr = m_gridHdrX.GetAt(ii + colNAME);
				gHdr.width = arWidth.GetAt(ii);
				m_gridHdrX.SetAt(ii + colNAME, gHdr);
			}
		}
	}
}


bool CGridWnd::loadField(int fno /* = -1*/, bool bDlgSetup/* = false*/, int iEqualizer)
{
	CString tempFile; 
	tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);

	CFileFind finder;
	if (!finder.FindFile(tempFile))
	{
		parsingField("", bDlgSetup); 
		m_colC = m_gridHdrX.GetSize();
		return true;
	}

	if (fno < 0)
		return false;

	int	readL = 0;
	char	readB[1024];
	CString	string, fieldno;

	fieldno.Format("%02d", fno);
	
	readL  = GetPrivateProfileString("FIELDS", fieldno, "", readB, sizeof(readB), tempFile);
	
	string = CString(readB, readL);	

	CString eq = "7778;";
	CString recom = "1910;1911;1916;1912;1913;1914;";

	if(iEqualizer == 1)
	{
		string += eq;
	}
	else if(iEqualizer == 2)
	{
		string.Replace(eq, "");
		string.Replace(recom, "");
	}
	else if(iEqualizer == 3)
	{
		string += recom;
	}

	int index = string.Find(eq);

	if(index > 0)
	{
		m_pToolWnd->SendMessage(WM_MANAGE, MK_EQUALIZER, (LPARAM)1);
		m_grid->m_isEqualizerField = true;
	}
	else
	{
		m_pToolWnd->SendMessage(WM_MANAGE, MK_EQUALIZER, (LPARAM)0);
		m_grid->m_isEqualizerField = false;
	}

	parsingField(string, bDlgSetup); 
	m_colC = m_gridHdrX.GetSize();
	
	WritePrivateProfileString("FIELDS", "ACTIVE", fieldno, tempFile);
	return true;
}

void CGridWnd::SaveAsInterField()
{
	if (m_szFIELD.IsEmpty())
		return;

	CString tempFile; tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);

	CFileFind finder;
	if (!finder.FindFile(tempFile))
		return;

	int	nIndex = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_GETAFIELD);
	if (nIndex < 0) return;

	int	readL = 0;
	char	readB[1024];
	CString	sField, sNewField, sFieldName, sFieldNo;

	sFieldNo.Format("%02d", nIndex);

	readL  = GetPrivateProfileString("FIELDS", sFieldNo, "", readB, sizeof(readB), tempFile);
	sField = CString(readB, readL);	
	int	index = sField.Find(deliMITER);
	if (index == -1)	return;

	sFieldName = sField.Left(index);
	sField.Format("%s", m_szFIELD);
	sField.Replace(",", deliMITER);
	sNewField.Format("%s%s%s", sFieldName, deliMITER, sField);

	WritePrivateProfileString("FIELDS", sFieldNo, sNewField, tempFile);
}


void CGridWnd::parsingOubsUpjong(char* datB, int datL)
{
	int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL) return;

	CString	string, stringx, entry;

	string = CString(&datB[gridHoL], datL - gridHoL);


	CString strCode;
	CString tempData, tempStr;
	
	if (!string.IsEmpty())
	{
		stringx = IH::Parser(string, P_NEW);
		tempData = IH::Parser(stringx, P_TAB);
		tempData = IH::Parser(stringx, P_TAB);
		tempData = IH::Parser(stringx, P_TAB);
		tempData = IH::Parser(stringx, P_TAB);
		m_dKospi = atof(tempData);
		tempStr.Format("%.2f", m_dKospi);
		m_dKospi = atof(tempStr);

		stringx = IH::Parser(string, P_NEW);
		tempData = IH::Parser(stringx, P_TAB);
		tempData = IH::Parser(stringx, P_TAB);
		tempData = IH::Parser(stringx, P_TAB);
		tempData = IH::Parser(stringx, P_TAB);
		m_dKosdaq = atof(tempData);
		tempStr.Format("%.2f", m_dKosdaq);
		m_dKosdaq = atof(tempStr);		
	}
}


void CGridWnd::parsingOubs(char* datB, int datL)
{
	int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL) return;
	
	if (m_seldrop < 0 )
	{
		m_grid->Clear(); 
		m_grid->MessageToGrid(WM_VSCROLL, SB_TOP);
	}

	CString	string, stringx, entry;
	CString	bongdata = _T("");
	BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	string = CString(&datB[gridHoL], datL - gridHoL);
	m_arExist.RemoveAll();
	int	kind = CAST_TREEID(m_kind)->kind;
	LPARAM	lParam = 0;

	if (kind == xISSUE)
	{
		lParam = MAKELPARAM(0, 1);
	}

	CString strCode, strMargin, strTodayVolumn;
	CString tempData, recommand1, recommand2, recommand3, recommand4, recommand5;

	if (!string.IsEmpty())
	{
		for (int ii = 0; ii < m_inters.GetSize(); ii++)
		{
			tempData = "";
			recommand1 = "";
			recommand2 = "";
			recommand3 = "";
			recommand4 = "";
			recommand5 = "";


			strTodayVolumn = "";
			stringx = IH::Parser(string, P_NEW);
//			TRACE("stringx : %s\n", stringx);

			if (!stringx.IsEmpty())
			{
				CString tempOnebyte = stringx.Left(1);
				tempOnebyte.TrimLeft();

// 				if(tempOnebyte.IsEmpty() == false && m_seldrop < 0)
// 				{
// 					if(ii>m_rowC-2)
// 					{
// 						insertRow(ii, FALSE);
// 					}
// 				}
			
				int iHeadersymbol = 0;		//그리드 헤더심볼 값
				
				for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
				{
					entry = IH::Parser(stringx, P_TAB); 
					 
					entry.TrimRight();
					entry.TrimLeft();
//					TRACE("jj : %d\t entry : %s\n", jj, entry);
					
					if(entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0")
					{
						entry = " ";
					}

					if (jj == colCODE && !entry.IsEmpty())
					{
						CString	sztmp1, sztmp2;
						sztmp1 = m_grid->GetItemText(ii + 1, 0);
						
						sztmp2.Format("%d", ii);
						m_arExist.SetData(sztmp1, sztmp2);
						if (m_seldrop < 0)
							m_grid->SetItemData(ii + 1, colSIG, lParam);			
						
						strCode = entry;
					}



					_gridHdr gridHdr = m_gridHdrX.GetAt(jj);
					struct _inters* pinters = m_inters.GetAt(ii);
					
					if (jj == colNAME)
					{
						UINT attr;

						if (pinters->gubn[0] == 'm')
						{
							entry.Format("%s", pinters->name);
							
							//책갈피 형태로 변경
							attr = m_grid->GetItemAttr(ii+1, colNAME) | GVAT_MARKER;
							m_grid->SetItemAttr(ii+1, colNAME, attr);
							m_grid->SetItemText(ii+1, colCODE, pinters->code);
								
						}
						else
						{
							attr = m_grid->GetItemAttr(ii+1, colNAME) & ~GVAT_MARKER;
							m_grid->SetItemAttr(ii+1, colNAME, attr);
						}

						strMargin = m_grid->GetItemText(ii + 1, colMAR);
						m_mapMargin.SetAt(strMargin, strMargin);
						m_grid->SetItemData(ii + 1, colNAME, atoi(strMargin.GetBuffer(0)));
					}
					else if (jj == colMAR)
					{
						entry.TrimLeft();
					}
					else if (!entry.IsEmpty() && jj == colRATE && (gridHdr.attr & GVAT_HIDDEN))
//					else if (!entry.IsEmpty() && jj == colRATE)
					{
//						TRACE("colRATE : %s\n ", entry);
						int nType = 0;
						switch(entry[0])
						{
						case '1':
							nType = 1;
							break;
						case '+':
						case '2':
							nType = 2;
							break;
						case '4':
							nType = 4;
							break;
						case '-':
						case '5':
							nType = 5;
							break;
						default:
							nType = 3;
						}

						m_grid->SetItemData(ii+1, jj, nType);
											
					}
					else if (jj == colCODE)
					{
						switch (m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)entry))
						{
						case kospiType:
							m_grid->SetItemData(ii+1, jj, 1);
							break;
						case kosdaqType:
							m_grid->SetItemData(ii+1, jj, 0);
							break;
						}
					}
					
					if (!(gridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
					{
						if ((strcmp("2029", gridHdr.symbol) == 0) ||
							(strcmp("2030", gridHdr.symbol) == 0) ||
							(strcmp("2031", gridHdr.symbol) == 0) )
						{
							if ((1 == gridHdr.needs) || (3 == gridHdr.needs))
							{
								CString strVal  = entry;
								CString strDiff = strVal;
								
								if (strVal[0] == '+' || strVal[0] == '-')
								{
									strDiff = strVal.Mid(1);
								}
								
								double dDiffOpen = 0.0;
								double dPClose, dVal;
								dVal	= atof(strDiff.GetBuffer(0));
								dPClose = atof( (m_grid->GetItemText(ii + 1, colPCURR)).GetBuffer(0) );
								
								if (dVal != 0 && dPClose != 0)
								{
									dDiffOpen = (dVal - dPClose) / dPClose * 100;
									
									if (1 == gridHdr.needs)
									{
										if (0 > dDiffOpen)
										{
											entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
										}
										else
										{
											entry.Format("%s( %0.2f%c)", strVal, dDiffOpen, P_PER);
										}
									}
									else if (3 == gridHdr.needs)
									{
										if (0 > dDiffOpen)
										{
											entry.Format("%0.2f%c", dDiffOpen, P_PER);
										}
										else if (0 == dDiffOpen)       
										{
											entry.Format(" %0.2f%c", dDiffOpen, P_PER);
										}
										else
										{
											entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
										}
									}
								}
							}
						}				
					}
					// END ADD
					
//					TRACE("symbol%s %d : %s\n",gridHdr.symbol, jj, entry);
					
	 				m_grid->SetItemText(ii+1, jj, entry);
					
					//전일종가를 저장(2320심볼)colPCURR
					CString tempYvol;

					if(jj == colPCURR)
					{
						tempYvol = entry;
						tempYvol.Remove('+');
						tempYvol.Remove('-');
						m_iYDayVol[ii][YDAYLASTVALUE] = tempYvol;						
					}

				
					if(jj == colVOL)
					{
						tempYvol = entry;
						tempYvol.Remove('+');
						tempYvol.Remove('-');
						strTodayVolumn = tempYvol;
					}

					if(m_yDayVolField >= 0)
					{		
						if(m_yDayVolField == jj)
						{
							entry.Remove('+'), entry.Remove('-');
							m_grid->SetItemText(ii + 1, jj, entry);
						}
					}

					if (m_bongField >= 0)
					{
						switch (jj)
						{
						case colHIGH:
							bongdata.Format("%s%c", entry, P_TAB);
							break;
						case colLOW:
							bongdata += entry;
							break;
						}
						
						if (m_bongField == jj)
						{
							bongdata.Remove('+'), bongdata.Remove('-');
							m_grid->SetItemText(ii + 1, jj, bongdata);					
						}
					}
			
					//체결 이퀄라이저 필드가 있을 경우
					if(m_EqualizerField >= 0)
					{
						
					}

					if(jj == colCURR)
					{
						int iHeadersymbol = atoi(gridHdr.symbol);

						CString tempStr;

						if(strCode == "001")
						{	
							m_dKospi = atof(entry);
							tempStr.Format("%.2f", m_dKospi);
							m_dKospi = atof(tempStr);
						}
						else if(strCode == "201")
						{
							m_dKosdaq = atof(entry);
							tempStr.Format("%.2f", m_dKosdaq);
							m_dKosdaq = atof(tempStr);
						}
					}


				}

				
				tempData = IH::Parser(stringx, P_TAB); 
				tempData.TrimRight();
				tempData.TrimLeft();

				//전일거래량을 저장(2321심볼)
				m_iYDayVol[ii][JMCODE] = strCode;
				m_iYDayVol[ii][YDAYVOLUMN] = tempData;
				
				//현재가를 저장(2022심볼)
				tempData = IH::Parser(stringx, P_TAB); 
				tempData.TrimRight();
				tempData.TrimLeft();
				tempData.Remove('+');
				tempData.Remove('-');
				m_iYDayVol[ii][TDAYVALUE] = tempData;

				if(((CGroupWnd*)m_pGroupWnd)->GetIsRecommand())
				{
					//지수구분(1915심볼)
					tempData = IH::Parser(stringx, P_TAB); 
					tempData.TrimRight();
					tempData.TrimLeft();		
					m_iYDayVol[ii][JISUGUBN] = tempData;
				}
				else
				{
					//추천일자(1910심볼)
					recommand1 = IH::Parser(stringx, P_TAB); 
					recommand1.TrimRight();
					recommand1.TrimLeft();
					
					//추천종목 기준가(1911심볼)
					recommand2 = IH::Parser(stringx, P_TAB); 
					recommand2.TrimRight();
					recommand2.TrimLeft();
					
					//수익률(1912심볼)
					recommand3 = IH::Parser(stringx, P_TAB); 
					recommand3.TrimRight();
					recommand3.TrimLeft();
					
					//추천종목 기준 BM지수(1913심볼)
					recommand4 = IH::Parser(stringx, P_TAB); 
					recommand4.TrimRight();
					recommand4.TrimLeft();
					
					//BM수익률(1914심볼)
					recommand5 = IH::Parser(stringx, P_TAB); 
					recommand5.TrimRight();
					recommand5.TrimLeft();
					
					//지수구분(1915심볼)
					tempData = IH::Parser(stringx, P_TAB); 
					tempData.TrimRight();
					tempData.TrimLeft();		
					m_iYDayVol[ii][JISUGUBN] = tempData;
				}

					
				double	dval1, dval2, dval3, creditPrc;
				int iDval = 0;

				CString	str;
				dval1 = dval2 = dval3 = creditPrc = 0.0;
				CString futurnGubn, mCode;
				int sizeCode;
				

				//추천일자
				if(m_recomDataField > 0)
				{	
					CString rDate, fDate;
					
					rDate = m_grid->GetItemText(ii + 1, m_recomDataField);
					rDate.TrimLeft();	
					rDate.TrimRight();	

					if(!rDate.IsEmpty())
					{
						fDate.Format("%s/%s/%s", rDate.Left(4), rDate.Mid(4, 2), rDate.Mid(6,2));
						m_grid->SetItemText(ii+1,m_recomDataField,fDate);
					}
				}

				for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
				{
					_gridHdr gridHdr = m_gridHdrX.GetAt(jj);
					struct _inters* pinters = m_inters.GetAt(ii);
						
					mCode = CString(pinters->code, sizeof(pinters->code));
					sizeCode = strlen(mCode);

					if (!m_posField) 
						continue;   //매입단가 등 특정필드가 있을경우 아래로 넘어감
					
					if (gridHdr.needs != 9)
						continue;
						
					switch (gridHdr.symbol[3])
					{
					case '1':		// 보유수량
						entry = IH::charTOstring(pinters->xnum, sizeof(pinters->xnum));
						break;
					case '2':		// 매입단가
						dval1 = IH::TOfabs(IH::charTOstring(pinters->xprc, sizeof(pinters->xprc)));
						if(sizeCode == 6 || sizeCode == 9)
						{
							iDval = Round(dval1);
							entry.Format("%d", iDval);
						}
						else
						{
							entry.Format("%.2f", dval1);	
						}
						break;
					case '3':		// 평가손익
						dval1 = IH::TOfabs(IH::charTOstring(pinters->xnum, sizeof(pinters->xnum)));
						dval2 = IH::TOfabs(IH::charTOstring(pinters->xprc, sizeof(pinters->xprc)));
						dval3 = IH::TOfabs(m_grid->GetItemText(ii+1, colCURR));
						
						if (dval1 <= 0 || dval2 <= 0) continue;
						
						if(sizeCode == 6 || sizeCode == 9)
						{
							if(iHeadersymbol == 2023)
							{	
								entry = CalcuPyungaSonik(pinters, m_grid->GetItemText(ii+1, colCURR));
							}
							else
							{
								entry = CalcuPyungaSonik(pinters, m_iYDayVol[ii][TDAYVALUE]);
							}
						}
						else
						{
							futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
							entry = CalFutureEvalPrice(pinters, strCode, futurnGubn, dval3, dval2, dval1);
//							entry = setPyungaSonik(pinters);
						}
						break;
					case '4':		// 수익율
						dval1 = IH::TOfabs(IH::charTOstring(pinters->xnum, sizeof(pinters->xnum)));
						dval2 = IH::TOfabs(IH::charTOstring(pinters->xprc, sizeof(pinters->xprc)));
						dval3 = IH::TOfabs(m_grid->GetItemText(ii+1, colCURR));
						
						if (dval1 <= 0 || dval2 <= 0) continue;
						
						if(sizeCode == 6 || sizeCode == 9)
						{
							
//							entry = CalJusikEvalRate(dval3, dval2, dval1);
							if(iHeadersymbol == 2023)
							{	
								entry = CalcuSuik(pinters, m_grid->GetItemText(ii+1, colCURR));
							}
							else
							{
								entry = CalcuSuik(pinters, m_iYDayVol[ii][TDAYVALUE]);
							}
						}
						else
						{
							futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
							entry = CalFutureEvalRate(pinters, strCode, futurnGubn, dval3, dval2, dval1);
//							entry = setsuikRate(pinters);
						}

						break;
					default:
						continue;
					}
					m_grid->SetItemText(ii+1, jj, entry);
				}

				strCode = "";

			}
		///////
		
		// ADD PSH 20070927
		/*
			entry = IH::Parser(stringx, P_TAB);	// 주식d증거금
		
		  if (!entry.IsEmpty())
		  {
		  m_mapMargin.SetAt(strCode, entry);
		  m_grid->SetItemData(ii + 1, colNAME, atoi(entry.GetBuffer(0)));
		  }
	    */

		  entry = m_grid->GetItemText(ii + 1, colINFO);	// 종목특이사항
		  CString strName = m_grid->GetItemText(ii + 1, colNAME);

		  //현재는 종목앞에 기호에 따라 이미지를 보여주고 있다
		  //이를 시장정보종목에만 이미지 보여주도록 변경
		  if (!strName.IsEmpty())
		  {
			  if ("투자주의" == entry)
			  {
				  m_grid->SetItemData(ii + 1, colINFO, 3);
			  }
			  else if ("투자경고" == entry)
			  {
				  m_grid->SetItemData(ii + 1, colINFO, 4);
			  }
			  else if ("투자위험" == entry)
			  {
				  m_grid->SetItemData(ii + 1, colINFO, 5);
			  }	
		  }
		}
	}

	
	if (m_ccField) calcInClient();
	LONG	ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
	hawkeyeAction((ret) ? true : false);

	m_grid->memoCheck();
	if (m_seldrop >= 0)
		m_grid->SetSelectRow(m_seldrop), m_seldrop = -1;

	if (NULL != m_pMsgWnd)
	{
		delete m_pMsgWnd;
		m_pMsgWnd = NULL;
	}

	//	m_bEditWork = FALSE;
	// DEL PSH 20070913
	//saveInterest();
	// END DEL
}

void CGridWnd::getBase(CString m_code)
{
	if (m_code.IsEmpty())
		return;
	int	nkind = getJKind(m_code);
	
	m_dBase = 100000;
	
	switch (nkind)
	{
	case JK_JCALLOPT:
	case JK_JPUTOPT:
		m_dBase = 1;
		break;
	case JK_SFUTURE:
	case JK_SFSPREAD:
		m_dBase = 10;
		break;
	case JK_FUTURE:
	case JK_SPREAD:
		m_dBase = 500000;
		break;
	case JK_STAR:
		m_dBase = 10000;
		break;
	case JK_KTBFUTURE:
		m_dBase = 1000000;
		break;
	case JK_USDFUTURE:
		m_dBase = 10000;
		break;
	case JK_GOLDFUTURE:
		m_dBase = 1000;
		break;
	}
}

CString CGridWnd::getFormatData(CString src)
{
	CString sRtn = src;
	if (!sRtn.IsEmpty() && sRtn.FindOneOf("+-") == 0)
		sRtn = sRtn.Mid(1);
	sRtn.TrimLeft();
	return sRtn;
}

int CGridWnd::getJKind(CString m_code)
{
	if (m_code.GetLength() != 8)
		return -1;
	TCHAR	ch1 = m_code.GetAt(0);
	TCHAR	ch2 = m_code.GetAt(1);
	TCHAR	ch3 = m_code.GetAt(2);
	
	switch (ch1)
	{
	case '1':	// future
		{
			if (ch2 == '0')
			{
				if (ch3 == '1')
					return JK_FUTURE;
				else
					return JK_STAR;
			}
			else if (ch2 == '6') 
				return JK_KTBFUTURE;
			else if (ch2 == '7')
				return JK_USDFUTURE;
			else if (ch2 == '8')
				return JK_GOLDFUTURE;
			else
				return JK_SFUTURE;			
		}
	case '4':	// future spread
		if (ch2 == '0')
			return JK_SPREAD;
		else
			return JK_SFSPREAD;
	case '2':	// call option
		if (m_code.GetAt(1) == '0')
			return JK_CALLOPT;
		else
			return JK_JCALLOPT;
		break;
	case '3':	// put option
		if (m_code.GetAt(1) == '0')
			return JK_PUTOPT;
		else
			return JK_JPUTOPT;
		break;
	}
	return -1;
}

double CGridWnd::Round(double data ) //(반올림)
{
    return (int)(data + 0.5);
}

CString	CGridWnd::CalJusikEvalPrice(struct _inters* pinters, double m_curr, double m_maip, double m_remian)
{
	double result;
	CString entry, m_rprice;

	double rPrice = m_curr * m_remian;
	double mPrice = pinters->maeipPrc;
	
	result = rPrice - mPrice;
	
	if(result > 0)
	{
		entry.Format("%+.0f", result);
	}
	else if(result < 0)
	{
		entry.Format("-%.0f", result);
	}
	
	return entry;
}

CString	CGridWnd::CalJusikEvalRate(struct _inters* pinters, double m_curr, double m_maip, double m_remian)
{
	CString entry;
	double result;
	double  creditPrc = pinters->creditPrc;
	double resultVal = 0;

//  	m_maip *= m_remian; 
//  	m_curr *= m_remian; 

	double rPrice = m_curr * m_remian;
	double mPrice = pinters->maeipPrc;

	result = rPrice - mPrice;

//  	result = (int)result;
//  	m_maip = (int)m_maip;
	
	entry.Format("%+.2f", (result / (mPrice - creditPrc)) * 100);

	return entry;
}



CString CGridWnd::setPyungaSonik(struct _inters* pinters)
{
	double  pyunggaPrc = pinters->creditPrc;
	
	CString result;
	
	if (pyunggaPrc > 0)
		result.Format("+%.0f", pyunggaPrc);
	else if (pyunggaPrc < 0)
		result.Format("-%.0f", pyunggaPrc);
	
	return result;
}


CString CGridWnd::setsuikRate(struct _inters* pinters)
{
	double  sonikRate = pinters->maeipPrc;
	
	CString result;
	
	result.Format("%.2f", sonikRate);

	if (sonikRate < 0)
		result = "" + result;
	else if (sonikRate > 0)
		result = "+" + result;
	
	return result;
}	


void CGridWnd::parsingOubsOne(char* datB, int datL, int update)
{
	if (update < 0)
		return;
	int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL) return;
	
	CString	string, stringx, entry;
	CString	bongdata = _T("");
	string = CString(&datB[gridHoL], datL - gridHoL);
	BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();

	CString strCode, strMargin; 
	CString tempData, recommand1, recommand2, recommand3, recommand4, recommand5;

	for (int ii = 0; ii < m_inters.GetSize(); ii++)
	{
		if (string.IsEmpty())
			break;

			tempData = "";
			recommand1 = "";
			recommand2 = "";
			recommand3 = "";
			recommand4 = "";
			recommand5 = "";
			
			stringx = IH::Parser(string, P_NEW);
			if (ii < update) continue;
			
			for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
			{
				if (stringx.IsEmpty())
					break;
				
				entry = IH::Parser(stringx, P_TAB); 
			
			entry.TrimRight();
			entry.TrimLeft();

			if(entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0")
			{
				entry = " ";
			}

			// ADD PSH 20070912
			_gridHdr gridHdr = m_gridHdrX.GetAt(jj);
			struct _inters* pinters = m_inters.GetAt(ii);

			if (jj == colCODE)
			{
				strCode = entry;
			}
			else if (jj == colNAME)
			{
				if (pinters->gubn[0] == 'm')
				{
					entry.Format("%s", pinters->name);
				}

				strMargin = m_grid->GetItemText(ii + 1, colMAR);
				m_mapMargin.SetAt(strCode, strMargin);
				m_grid->SetItemData(ii + 1, colNAME, atoi(strMargin.GetBuffer(0)));
			}
			else if (!entry.IsEmpty() && jj == colRATE && (gridHdr.attr & GVAT_HIDDEN))
			{
				int nType = 0;
				switch(entry[0])
				{
				case '1':
					nType = 1;
					break;
				case '+':
				case '2':
					nType = 2;
					break;
				case '4':
					nType = 4;
					break;
				case '-':
				case '5':
					nType = 5;
					break;
				default:
					nType = 3;
				}
				m_grid->SetItemData(ii+1, jj, nType);
			}
			else if (jj == colCODE)
			{
				switch (m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)entry))
				{
				case kospiType:
					m_grid->SetItemData(ii+1, jj, 1);
					break;
				case kosdaqType:
					m_grid->SetItemData(ii+1, jj, 0);
					break;
				}
			}

			if (!(gridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
			{
				if ((strcmp("2029", gridHdr.symbol) == 0) ||
					(strcmp("2030", gridHdr.symbol) == 0) ||
					(strcmp("2031", gridHdr.symbol) == 0) )
				{
					if ((1 == gridHdr.needs) || (3 == gridHdr.needs))
					{
						CString strVal  = entry;
						CString strDiff = strVal.Mid(1);
						double dDiffOpen, dPClose, dVal;
						dVal	= atof(strDiff.GetBuffer(0));
						dPClose = atof( (m_grid->GetItemText(ii + 1, colPCURR)).GetBuffer(0) );

						dDiffOpen = (dVal - dPClose) / dPClose * 100;
					
						if (1 == gridHdr.needs)
						{
							if (0 > dDiffOpen)
							{
								entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
							}
							else
							{
								entry.Format("%s( %0.2f%c)", strVal, dDiffOpen, P_PER);
							}
						}
						else if (3 == gridHdr.needs)
						{
							if (0 > dDiffOpen)
							{
								entry.Format("%0.2f%c", dDiffOpen, P_PER);
							}
							else if (0 == dDiffOpen)
							{
								entry.Format(" %0.2f%c", dDiffOpen, P_PER);
							}
							else
							{
								entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
							}
						}
					}
				}				
			}
			// END ADD

			m_grid->SetItemText(ii+1, jj, entry);

			if (m_bongField >= 0)
			{
				switch (jj)
				{
				case colHIGH:
					bongdata.Format("%s%c", entry, P_TAB);
					break;
				case colLOW:
					bongdata += entry;
					break;
				}

				if (m_bongField == jj)
				{
					bongdata.Remove('+'), bongdata.Remove('-');
					m_grid->SetItemText(ii + 1, jj, bongdata);					
				}
			}


			//보유수량필드가 있는 확인
			if (!m_posField) continue;		

			// DEL PSH 20070912
			//_gridHdr gridHdr = m_gridHdrX.GetAt(jj);
			// END DEL

			if (gridHdr.needs != 9)
				continue;

			// DEL PSH 20070912
			//struct _inters* pinters = m_inters.GetAt(ii);
			// END DEL
			
			switch (gridHdr.symbol[3])
			{
			case '1':		// 보유수량
				entry = IH::charTOstring(pinters->xnum, sizeof(pinters->xnum));
				break;
			case '2':		// 매입단가
				entry = IH::charTOstring(pinters->xprc, sizeof(pinters->xprc));
				break;
			case '3':		// 평가손익
				entry = CalcuPyungaSonik(pinters, m_grid->GetItemText(ii+1, colCURR));
				break;
			case '4':		// 수익율
				entry = CalcuSuik(pinters, m_grid->GetItemText(ii+1, colCURR));
				break;
			default:
				continue;
			}
			m_grid->SetItemText(ii+1, jj, entry);
		}
		
		//전일거래량을 저장(2321심볼)
		tempData = IH::Parser(stringx, P_TAB); 
		tempData.TrimRight();
		tempData.TrimLeft();
		m_iYDayVol[ii][JMCODE] = strCode;
		m_iYDayVol[ii][YDAYVOLUMN] = tempData;
		//save the Current Value
		m_iYDayVol[ii][YDAYLASTVALUE] = m_grid->GetItemText(ii+1, colCURR);

		//현재가를 저장(2022심볼)
		tempData = IH::Parser(stringx, P_TAB); 
		
		//추천일자(1910심볼)
		recommand1 = IH::Parser(stringx, P_TAB); 
		recommand1.TrimRight();
		recommand1.TrimLeft();
		
		//추천종목 기준가(1911심볼)
		recommand2 = IH::Parser(stringx, P_TAB); 
		recommand2.TrimRight();
		recommand2.TrimLeft();
		
		//수익률(1912심볼)
		recommand3 = IH::Parser(stringx, P_TAB); 
		recommand3.TrimRight();
		recommand3.TrimLeft();
		
		//추천종목 기준 BM지수(1913심볼)
		recommand4 = IH::Parser(stringx, P_TAB); 
		recommand4.TrimRight();
		recommand4.TrimLeft();
		
		//BM수익률(1914심볼)
		recommand5 = IH::Parser(stringx, P_TAB); 
		recommand5.TrimRight();
		recommand5.TrimLeft();

		//지수구분(1915심볼)
		tempData = IH::Parser(stringx, P_TAB); 
		tempData.TrimRight();
		tempData.TrimLeft();		
		m_iYDayVol[ii][JISUGUBN] = tempData;

		// ADD PSH 20070927
		entry = m_grid->GetItemText(ii + 1, colINFO);	// 종목특이사항
		{
			CString strName = m_grid->GetItemText(ii + 1, colNAME);
			
			if(!recommand1.IsEmpty())
			{
				m_grid->SetItemData(ii + 1, colSIG, 6);
			}

			if (!strName.IsEmpty())
			{
				if ("투자주의" == entry)
				{
					m_grid->SetItemData(ii + 1, colINFO, 3);
				}
				else if ("투자경고" == entry)
				{
					m_grid->SetItemData(ii + 1, colINFO, 4);
				}
				else if ("투자위험" == entry)
				{
					m_grid->SetItemData(ii + 1, colINFO, 5);
				}
			}
		}
		// END ADD

		if (m_ccField) calcInClient(ii+1);
		LONG	ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		if (ret) hawkEyeColor(ii+1);
		if (m_grid->memoCheck(ii+1)) m_grid->memoRefresh();
		break;
	}

	// ADD PSH 20070927
		
	stringx = IH::Parser(string, P_TAB);

	if (!stringx.IsEmpty())
	{
		//strCode = stringx.Mid(1, 11);
		//strCode.TrimRight();

		//strMargin = stringx.Mid(153, 6);
		//strMargin.TrimRight();

		m_mapMargin.SetAt(strCode, stringx);
	}
	
	// END ADD

	m_grid->memoCheck(m_updateROW + 1);
	if (m_seldrop >= 0)
		m_grid->SetSelectRow(m_seldrop), m_seldrop = -1;

	// DEL PSH 20070913
	//saveInterest();
	// END DEL
	ReIndexing();
}

void CGridWnd::InsertNews(CString datB)
{
	int	idx = 0;
	CString	code = _T(""), szTitle = _T(""), symbol = _T(""), entry = _T("");

	while (!datB.IsEmpty())
	{
		idx = datB.Find('\t');
		if (idx == -1) break;

		symbol = datB.Left(idx++);
		datB = datB.Mid(idx);

		idx = datB.Find('\t');
		if (idx == -1)
		{
			entry = datB;	datB = _T("");
		}
		else
		{
			entry = datB.Left(idx++); datB = datB.Mid(idx);
		}

		if (symbol.Compare("015") == 0)
			szTitle = entry;

		if (symbol.Compare("301") == 0)
		{
			code = entry;
			code.TrimRight();
			break;
		}
	}
		
	if (!code.IsEmpty())
	{
		m_szTitle.Format(" %s", (char*)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, m_kind));
		m_szTitle += ("- " + szTitle); 
		InvalidateRect(m_rcTitle, FALSE);
		int	ncnt = m_inters.GetSize();
		struct _inters* pinters = NULL;
		int	nIndex = -1;
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			pinters = m_inters.GetAt(ii);
			if (strlen(pinters->code) == 0)
			{
				break;	
			}

			if (strcmp(pinters->code, (char*)code.operator LPCTSTR()) == 0)
			{
				nIndex = ii;			
				break;
			}
		}

		if (nIndex < 0)
		{
			DeleteRow(MAX_LINE);		
			
		}
		else
		{
			DeleteRow(nIndex + 1);	
		}

		insertRow(1);
		pinters = m_inters.GetAt(0);
		m_grid->SetItemData(1, colSIG, MAKEWPARAM(0, 1));
		strcpy(pinters->code, (char*)code.operator LPCTSTR());
		sendTransaction();
	}	
}

void CGridWnd::ReplaceSymbol(CMapStringToString& fms, CString szold, CString sznew)
{
	CString	entry = _T("");
	if (fms.Lookup(szold, entry))
	{
		fms.RemoveKey(szold);
		fms.SetAt(sznew, entry);
	}
}

CString SplitString(CString &strData, CString strToken)
{
	CString sResult = strData;
	if(strToken == "") return sResult;
	int iFind = strData.Find(strToken); 
	if(iFind > -1 ) 
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}
	
	
	return sResult;
}

void CGridWnd::SettingGridHeaderName(int index)
{
	struct	_symbol {
		UINT	stid1;
		char*	symb1;
		UINT	stid2;
		char*	symb2;
	} chksym [] = {
		{ IDS_GH_CURR,	"2023",	IDS_GH_ANTIPRC,	"2111" },
		{ IDS_GH_DIFF,	"2024",	IDS_GH_DIFF,	"2115" },
		{ IDS_GH_RATE,	"2033",	IDS_GH_RATE,	"2116" },
		{ IDS_GH_VOL,	"2027",	IDS_GH_ANTIVOL,	"2112" },
	};
	
	const	int chksymC = sizeof(chksym) / sizeof(_symbol);

	if(index == 1)
	{
		_gridHdr gridHdr;
		for (int ii = 0; ii < chksymC; ii++)
		{
			for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);
				if (atoi(gridHdr.symbol) != atoi(chksym[ii].symb1))
					continue;
				
				gridHdr.stid = chksym[ii].stid2;
				m_gridHdrX.SetAt(jj, gridHdr);
			}
		}
		for ( int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
		{
				gridHdr = m_gridHdrX.GetAt(jj);
		}
	}
	else
	{
		_gridHdr gridHdr;
		for (int ii = 0; ii < chksymC; ii++)
		{
			for (int jj = colCURR ; jj < m_gridHdrX.GetSize() ; jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);
				if (atoi(gridHdr.symbol) != atoi(chksym[ii].symb2))
					continue;
				
				gridHdr.stid = chksym[ii].stid1;
				m_gridHdrX.SetAt(jj, gridHdr);
				break;
			}
		}
	}
}

void CGridWnd::parsingAlert(CString datA) 
{
	int	idx, xrow;
	CMapStringToString fms;
	CString	code, symbol, entry, datB, total_str, dataExceptCode;
	
	datB = datA;
	total_str = datB;
	datB = SplitString(total_str, "\n");
	

	
	while(!datB.IsEmpty())
	{
		if ((idx = datB.Find('\t')) == -1)
			return;
		
		

		code = datB.Left(idx++);
		datB = datB.Mid(idx);
		dataExceptCode = datB;			//종목 중복때문에 임시 저장

		if (code.CompareNoCase("S0000") == 0)
		{
			if (CAST_TREEID(m_kind)->kind == xISSUE)
			{
				InsertNews(datB);
				return;
			}
			else
				parsingNews(datB);
		}	
		
//		testSaveFile3(code, datB);

		int count = 0;
		count = CheckRealTimeCode(code);
		
		if (count == 0) return;
	
		CString	szssss;
		szssss.Format("%s", datB);

		//그룹 종목 중복 허용일 경우,
		for(int rowPosition = 0 ; rowPosition<count ; rowPosition++)
		{
			xrow = m_irowCode[rowPosition]; 
			datB = dataExceptCode;


			if (m_bMonitor)
			{
				if (m_bMonSym && m_pRCDlg->m_strSymbol == code)
				{
					CString strTemp;
					strTemp.Format(" ========== Row : %d ==========\r\n", xrow);
					m_strMonData += "\r\n" + strTemp + total_str;
					m_pRCDlg->m_edData.SetWindowText(m_strMonData);
				}
				else if (!m_bMonSym)
				{
					m_strMonData += "\r\n" + total_str;
					m_pRCDlg->m_edData.SetWindowText(m_strMonData);
				}
			}


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

			entry = _T("");
			CString	oldEXP = m_grid->GetItemText(xrow, colEXPECT);
			CString	newEXP = _T("");
			BOOL	bManual = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
			BOOL	bAutoCheck = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);

			CString strTime, expect, real, excep;
			CString strBeginTime = "081000";
			CString strEndTime = "090000";
 			CString codeExceptA;

// 			CString test, test2;
// 			fms.Lookup("111", test2);
// 			TRACE("111 RealData is : %s\n", test2);
// 			fms.Lookup("023", test);
// 			TRACE("023 RealData is : %s\n", test);


			//변경이 있을때마다 배열에 저장해 둔 현재가 데이타 업데이트
			CString en2, saveData;

			if(fms.Lookup("111", saveData))
			{
				fms.Lookup("034", en2);
//				testSaveFile2(code, en2, "111", saveData);	
			}	
			else if (fms.Lookup("023", saveData))
			{
				fms.Lookup("034", en2);
		
				if(en2)
				{
					for(int i=0 ; i<MAX_LINE ; i++)
					{
						if(code.GetLength() == 7)
						{
							codeExceptA = code.Mid(1);
						}
						else
						{
							codeExceptA = code;
						}
						
						//종목번호로 매칭해서 데이터를 보여준다
						if(codeExceptA == m_iYDayVol[i][JMCODE])
						{
							m_iYDayVol[i][TDAYVALUE] = saveData;
//							testSaveFile2(code, en2, "023", saveData);
						}
					}
				}
			}
			/////////////////여기까지


			//내려오는 걸 기준으로 끊기
			//111이 내려오는지 023이 내려오는지 판단해서 사전 차단
				
			if (fms.Lookup("111", entry))	// 예상가 적용
			{
				if (!entry.IsEmpty())
				{
					m_grid->SetItemText(xrow, colEXPECT, "1");
				}
				
				//현재 예상버튼이 안눌러져 있을때, 강제로 심볼을 바꾸어서 그리드에 데이터를 보여주고 있다 
				//(현재가 심볼로 되어있기 때문에 )
				//그럼 자동 체크 눌러있을때만, 보여주면 되고
				//자동 체크 해지시엔, 예상가 안보여주면 된다
				
				if (!bManual)					//예상 버튼 안눌린 상태
				{
					// replace symbol
					if(bAutoCheck)				// 자동에 체크된 상태
					{
						ReplaceSymbol(fms, "111", "023");
						ReplaceSymbol(fms, "112", "027");
						CString	szval1 = _T("");
						if (fms.Lookup("115", szval1))
						{
							szval1 = szval1.Mid(1);

							if (!szval1.IsEmpty())
							{
								ReplaceSymbol(fms, "115", "024");
							}
						}
						
						szval1 = _T("");
						if (fms.Lookup("116", szval1))
						{

							if (!szval1.IsEmpty())
							{
								ReplaceSymbol(fms, "116", "033");
							}
						}
					}
					else
					{
						if (!entry.IsEmpty())
						{
							m_grid->SetItemText(xrow, colEXPECT, "0");
						}
					}
				}
			}
			else if (fms.Lookup("023", entry))
			{
				
				
				if (!bManual)	//예상 버튼 안눌린 상태
				{
//					//변경이 있을때마다 배열에 저장해 둔 현재가 데이타 업데이트
// 					for(int i=0 ; i<MAX_LINE ; i++)
// 					{
// 						if(code.GetLength() == 7)
// 						{
// 							codeExceptA = code.Mid(1);
// 						}
// 						else
// 						{
// 							codeExceptA = code;
// 						}
// 						
// 						//종목번호로 매칭해서 데이터를 보여준다
// 						if(codeExceptA == m_iYDayVol[i][JMCODE])
// 						{
// 							m_iYDayVol[i][TDAYVALUE] = entry;
// 						}
// 					}


					
					if(bAutoCheck)				// 자동에 체크된 상태
					{
						fms.Lookup("034", strTime);
						
						if (strBeginTime <= strTime && strEndTime >= strTime)
						{
							m_grid->SetItemText(xrow, colEXPECT, "1");
						}
						else
						{
							m_grid->SetItemText(xrow, colEXPECT, "0");
						}
					}
				}
				else
				{
					if (!entry.IsEmpty())
					{
						m_grid->SetItemText(xrow, colEXPECT, "1");
					}
				}
			}
		

			newEXP = m_grid->GetItemText(xrow, colEXPECT);
			BOOL	bForceDraw = (newEXP == oldEXP) ? FALSE : TRUE;
			BOOL	bExpect = (BOOL)atoi(m_grid->GetItemText(xrow, colEXPECT));	// 위에서 설정한 0 이나 1 값 return
			bool	updatePoss = false;
			const int countX = m_gridHdrX.GetSize(); 
			_gridHdr gridHdr;
			LONG	ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
			
			COLORREF rtmColor = ((CGroupWnd*)m_pGroupWnd)->GetRTMColor();
			int	rtmAction = ((CGroupWnd*)m_pGroupWnd)->GetRtmAction();
			BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();


			for (int ii = 2; ii < countX; ii++)
			{
				gridHdr = m_gridHdrX.GetAt(ii);
				
				symbol = CString(gridHdr.symbol, strlen(gridHdr.symbol));
				

				if (symbol.GetLength() >= 3) 
					symbol = symbol.Right(3);

				if (!fms.Lookup(symbol, entry))
					continue;


				if (!bForceDraw && IH::TOf(entry) == IH::TOf(m_grid->GetItemText(xrow, ii)))
					continue;


				
				if (!(gridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
				{
					if ((strcmp("2029", gridHdr.symbol) == 0) ||
						(strcmp("2030", gridHdr.symbol) == 0) ||
						(strcmp("2031", gridHdr.symbol) == 0) )
					{
						if ((1 == gridHdr.needs) || (3 == gridHdr.needs))
						{
							CString strVal  = entry;
							CString strDiff = strVal;

							if (strVal[0] == '+' || strVal[0] == '-')
							{
								strDiff = strVal.Mid(1);
							}

							double dDiffOpen = 0.0;
							double dPClose, dVal;
							dVal	= atof(strDiff.GetBuffer(0));
							dPClose = atof( (m_grid->GetItemText(xrow, colPCURR)).GetBuffer(0) );

							if (dVal != 0 && dPClose != 0)
							{
								dDiffOpen = (dVal - dPClose) / dPClose * 100;

								if (1 == gridHdr.needs)
								{
									if (0 > dDiffOpen)
									{
										entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
									}
									else
									{
										entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
									}
								}
								else if (3 == gridHdr.needs)
								{
									if (0 > dDiffOpen)
									{
										entry.Format("%0.2f%c", dDiffOpen, P_PER);
									}
									else if (0 == dDiffOpen)
									{
										entry.Format(" %0.2f%c", dDiffOpen, P_PER);
									}
									else
									{
										entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
									}
								}
							}
						}
					}	
				}

				if ((ii == colCURR) && bExpect && !m_bExpect)		// 예상버튼이 안눌려져있고, 동시호가때
				{
					entry.TrimLeft();
					entry.TrimRight();

					if ((entry == "0") || (entry == "-0") || (entry == "+0"))
					{
						entry = " ";
						entry = m_grid->GetItemText(xrow, colCURR);
						return;
					}
				}
				
				CString tempStr;

				if( ii== colCURR)
				{
					if(code.Find("K0001") > -1)
					{	
						m_dKospi = atof(entry);
						tempStr.Format("%.2f", m_dKospi);
						m_dKospi = atof(tempStr);
					}
					else if(code.Find("KQ001") > -1)
					{
						m_dKosdaq = atof(entry);
						tempStr.Format("%.2f", m_dKosdaq);
						m_dKosdaq = atof(tempStr);
					}
				}

				if((strcmp("2115", gridHdr.symbol) == 0) || (strcmp("2024", gridHdr.symbol) == 0))
				{
					if(entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0" || entry == "30")
					{
						entry = " ";
					}
				}
				else
				{
					if(entry == "0" || entry == "-0" || entry == "0.00" || entry == "+0" || entry == " 0")
					{
						entry = " ";
					}
				}
			


				m_grid->SetItemText(xrow, ii, entry);


				
				//전일거래대비율 실시간 계산
				CString str2027, str2403;
				double int2027 = 0;
				double int2403 = 0;
				double int2321 = 0;
				
				if(fms.Lookup("027", str2027))
				{
					fms.Lookup("027", str2027);
					int2027 = atoi(str2027);
					for(int i=0 ; i<100 ; i++)
					{
						if(code.GetLength() == 7)
						{
							code = code.Mid(1);
						}
						
						//종목번호로 매칭해서 데이터를 보여준다
						if(code == m_iYDayVol[i][JMCODE])
						{
							int2321 = atoi(m_iYDayVol[i][YDAYVOLUMN]);
//							break;
						}
					}
//					int2321 = atoi(m_iYDayVol[xrow-1]);
					
					if(int2321 != 0)
					{
						int2403 = (int2027 / int2321) * 100;
					}
					else
					{
						int2403 = 0;
					}
					
					if(!bExpect && m_yDayVolField >= 0)
					{
						entry.Format("%.2f", int2403);
						m_grid->SetItemText(xrow, m_yDayVolField, entry);
					}
				}
				
				//추천 수익률 계산 수익률 = 100.0 * (현재가 - 기준가) / 기준가
				if(m_recomSuikrateField >= 0 && !bExpect)
				{
					CString strBasicCost, str023, suik;
					double dBasicCost = 0;
					double	d023 = 0;
					double result = 0;
					
					strBasicCost = m_grid->GetItemText(xrow, m_recomBasicCostField);
					dBasicCost = atoi(strBasicCost);
					
					fms.Lookup("023", str023);
					d023 = abs(atof(str023));
					
					if(d023 != 0)
					{
						result = ((d023 - dBasicCost) / dBasicCost) * 100;

						if(result > 0)
						{
							suik.Format("+%.2f", result);
						}
						else
						{
							suik.Format("%.2f", result);
						}
						
						m_grid->SetItemText(xrow, m_recomSuikrateField, suik);
					}
				}
				
				//추천 BM수익률 계산
				//수익률 = 100.0 * (현재지수 - BM지수) / BM지수
				if(m_recomBMSuikrateField >= 0 && !bExpect)
				{
					CString strBMjisu, suik;
					double	dBMjisu = 0;
					double result = 0;
					
					strBMjisu = m_grid->GetItemText(xrow, m_recomBMjisuField);
					dBMjisu = atoi(strBMjisu);
					
					CString upjong;
					
					if(xrow > 0)
					{
						upjong = GetRecommandInfo(xrow);
					}
					
						
					m_dKospi = ((CGroupWnd*)GetParent())->GetGridWndUpjn()->m_dKospi;
					m_dKosdaq = ((CGroupWnd*)GetParent())->GetGridWndUpjn()->m_dKosdaq;
					
					if(!upjong.IsEmpty() && m_dKospi > 0 && m_dKosdaq > 0)
					{
						if(atoi(upjong) == 1 && m_dKospi > 0)
						{
							result = ((m_dKospi - dBMjisu) / dBMjisu) * 100;
						}
						else if(atoi(upjong) == 2 && m_dKosdaq > 0)
						{
							result = ((m_dKosdaq - dBMjisu) / dBMjisu) * 100;
						}
						
						if(result > 0)
						{
							suik.Format("+%.2f", result);
						}
						else
						{
							suik.Format("%.2f", result);
						}

						m_grid->SetItemText(xrow, m_recomBMSuikrateField, suik);
					}
				}

				if (ii == colCURR)
				{
					if (!bExpect && m_bongField >= 0)
					{
						CString	bongdata = _T(""), open = _T(""), high = _T(""), low = _T("");
						fms.Lookup("029", open);
						m_grid->SetItemText(xrow, colOPEN, open);
						fms.Lookup("030", high);
						high.Remove('+'), high.Remove('-');
						fms.Lookup("031", low);
						low.Remove('+'), low.Remove('-');
						bongdata.Format("%s%c%s", high, P_TAB, low);
						m_grid->SetItemText(xrow, m_bongField, bongdata);
					}

					if (m_posField && !bExpect) 
					{
						updatePoss = true;
					}
				}
				
				CString cheg;
				if(m_EqualizerField >= 0 && fms.Lookup("032", cheg))
				{
					TRACE("Cheg : %d\n", cheg);
					m_grid->SetItemText(xrow, m_EqualizerField, cheg);
				}				

				switch (rtmAction)
				{
				case rtmNONE:
					break;
				case rtmCELL:
					{				
						if (ret && ii <= colCURR)
							break;

						m_grid->Blink(xrow, ii, rtmColor);
					}
					break;
				case rtmROW:
					{
						int xx = colSIG;
						if (ret) xx = colCURR + 1;
						for (; xx < countX; xx++)
							m_grid->Blink(xrow, xx, rtmColor);
					}
					break;
				}
			}

			// MODIFY PSH 20070912
			//for (int jj = 0; updatePoss && jj < m_gridHdrX.GetSize(); jj++)

			BOOL bRealData = TRUE;
			CString strTemp;
			int  nTemp = 0;

			// ADD PSH 20070912
			if (m_tkConfig.m_bApply)
			{
				bRealData = FALSE;

				nTemp = m_grid->GetItemData(xrow, colRATE);

				if ((m_tkConfig.m_ulimit) && (nTemp == 1))
				{
					bRealData = TRUE;
				}

				if ((m_tkConfig.m_up) && (nTemp == 2))
				{
					bRealData = TRUE;
				}

				if ((m_tkConfig.m_flat) && (nTemp == 3))
				{
					bRealData = TRUE;
				}

				if ((m_tkConfig.m_dlimit) && (nTemp == 4))
				{
					bRealData = TRUE;
				}

				if ((m_tkConfig.m_down) && (nTemp == 5))
				{
					bRealData = TRUE;
				}

				if (bRealData)
				{
					strTemp = m_grid->GetItemText(xrow, colTVOL);
					nTemp = atoi(strTemp.GetBuffer(0));

					if (0 > nTemp) nTemp = -nTemp;

					if (nTemp >= m_tkConfig.m_vol * 1000)
					{
						bRealData = TRUE;
					}
					else
					{
						bRealData = FALSE;
					}

					if (bRealData || !m_tkConfig.m_and)
					{
						if (m_tkConfig.m_and)
						{
							strTemp = m_grid->GetItemText(xrow, colCURR);
							nTemp = nTemp * atoi(strTemp.GetBuffer(0));

							if (nTemp < 0) nTemp = -nTemp;

							if (nTemp >= m_tkConfig.m_amt * 10000000)
							{
								bRealData = TRUE;
							}
							else
							{
								bRealData = FALSE;
							}
						}

						if (bRealData)
						{
							if (m_tkConfig.m_price)
							{
								strTemp = m_grid->GetItemText(xrow, colCURR);
								nTemp = atoi(strTemp.GetBuffer(0));

								if (0 > nTemp) nTemp = -nTemp;

								if ((nTemp >= m_tkConfig.m_sprc) && (nTemp <= m_tkConfig.m_eprc))
								{
									bRealData = TRUE;
								}
								else
								{
									bRealData = FALSE;
								}
							}
						}
					}
				}
			}

			if (m_ccField) 
				calcInClient(xrow);

			for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
			{
			// END MODIFY
				
				if (bRealData)
				{
					m_pTicker->SetItemText(0, jj, m_grid->GetItemText(xrow, jj));

					if ((colSIG == jj) || (colNAME == jj) || (colINFO == jj))
					{
						m_pTicker->SetItemData(0, jj, m_grid->GetItemData(xrow, jj));
					}

					m_btDomino.Invalidate(TRUE);
					m_btSAVE.Invalidate(TRUE);
					m_btCLOSE.Invalidate(TRUE);	

				}


				double	dval1, dval2, dval3;
				//		char	szTemp[16];
				CString	str, mCode;
				int sizeCode;

				dval1 = dval2 = dval3 = 0.0;
				
				_gridHdr gridHdr = m_gridHdrX.GetAt(jj);

				if (gridHdr.needs != 9)
					continue;

				if (m_posField)
				{
				// END ADD
					double	dval1, dval2, dval3;
			//		char	szTemp[16];
					CString	str;
					dval1 = dval2 = dval3 = 0.0;
					CString futurnGubn, mCode, strCurr;
					
					_gridHdr gridHdr = m_gridHdrX.GetAt(jj);
					if (gridHdr.needs != 9) 
						continue;
					
					struct _inters* pinters = m_inters.GetAt(xrow -1);

					mCode = CString(pinters->code, sizeof(pinters->code));
					sizeCode = strlen(mCode);

					if(bExpect == FALSE)		// 장중
					{
						switch (gridHdr.symbol[3])
						{
						case '3':		// 평가손익
							dval1 = IH::TOfabs(IH::charTOstring(pinters->xnum, sizeof(pinters->xnum)));
							dval2 = IH::TOfabs(IH::charTOstring(pinters->xprc, sizeof(pinters->xprc)));
							dval3 = IH::TOfabs(m_grid->GetItemText(xrow, colCURR));

							if (dval1 <= 0 || dval2 <= 0) continue;
							
							if(sizeCode == 6 || sizeCode == 9)
							{
	
//								entry = CalJusikEvalPrice(pinters, dval3, dval2, dval1);
								entry = CalcuPyungaSonik(pinters, m_grid->GetItemText(xrow, colCURR));
							}
							else
							{
								futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
								entry = CalFutureEvalPrice(pinters, code, futurnGubn, dval3, dval2, dval1);
							}
							
//							testSaveFile("장중", GetCodeName(mCode), m_grid->GetItemText(xrow, colCURR), dval2, entry, "");
							break;

						case '4':		// 수익율
							dval1 = IH::TOfabs(IH::charTOstring(pinters->xnum, sizeof(pinters->xnum)));
							dval2 = IH::TOfabs(IH::charTOstring(pinters->xprc, sizeof(pinters->xprc)));
							dval3 = IH::TOfabs(m_grid->GetItemText(xrow, colCURR));
							
							if (dval1 <= 0 || dval2 <= 0) continue;

							if(sizeCode == 6 || sizeCode == 9)
							{
								
//								entry = CalJusikEvalRate(pinters, dval3, dval2, dval1);
								entry = CalcuSuik(pinters, m_grid->GetItemText(xrow, colCURR));
							}
							else
							{
								futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
								entry = CalFutureEvalRate(pinters, code, futurnGubn, dval3, dval2, dval1);
							}

//							testSaveFile("장중", GetCodeName(mCode), m_grid->GetItemText(xrow, colCURR), dval2, "", entry);

							break;

						default:
							continue;
						}
						m_grid->SetItemText(xrow, jj, entry);
						
					}
					else		//동시호가 시간
					{
						if(bAutoCheck)
						{
							int ii = 0;
							switch (gridHdr.symbol[3])
							{
							case '3':		// 평가손익
								
								dval1 = IH::TOfabs(IH::charTOstring(pinters->xnum, sizeof(pinters->xnum)));
								dval2 = IH::TOfabs(IH::charTOstring(pinters->xprc, sizeof(pinters->xprc)));
								

								for(ii=0 ; ii<100 ; ii++)
								{
									if(code.GetLength() == 7)
									{
										code = code.Mid(1);
									}
									
									//종목번호로 매칭해서 데이터를 보여준다
									if(code == m_iYDayVol[ii][JMCODE])
									{
										strCurr = m_iYDayVol[ii][TDAYVALUE];
										//TRACE("RealTime 023 Symbol : %s is %dth located value is %s\n", m_iYDayVol[ii][JMCODE], ii, m_iYDayVol[ii][TDAYVALUE]);
									}
								}

								if (dval1 <= 0 || dval2 <= 0) continue;
								
								if(sizeCode == 6 || sizeCode == 9)
								{
		
//									entry = CalJusikEvalPrice(pinters, dval3, dval2, dval1);
									entry = CalcuPyungaSonik(pinters, strCurr);
								}
								else
								{
									futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
									entry = CalFutureEvalPrice(pinters, code, futurnGubn, dval3, dval2, dval1);
								}

//								testSaveFile("동시", GetCodeName(mCode), strCurr, dval2, entry, "");

								break;

							case '4':		// 수익율
								dval1 = IH::TOfabs(IH::charTOstring(pinters->xnum, sizeof(pinters->xnum)));
								dval2 = IH::TOfabs(IH::charTOstring(pinters->xprc, sizeof(pinters->xprc)));
								for(ii=0 ; ii<100 ; ii++)
								{
									if(code.GetLength() == 7)
									{
										code = code.Mid(1);
									}
									
									//종목번호로 매칭해서 데이터를 보여준다
									if(code == m_iYDayVol[ii][JMCODE])
									{
										strCurr = m_iYDayVol[ii][TDAYVALUE];
//										TRACE("RealTime 023 Symbol : %s is %dth located\n", m_iYDayVol[ii][JMCODE], ii);
									}
								}


								if (dval1 <= 0 || dval2 <= 0) continue;
								
								if(sizeCode == 6 || sizeCode == 9)
								{
//									entry = CalJusikEvalRate(pinters, dval3, dval2, dval1);
									entry = CalcuSuik(pinters, strCurr);
								}
								else
								{
									futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
									entry = CalFutureEvalRate(pinters, code, futurnGubn, dval3, dval2, dval1);
								}

//								testSaveFile("동시", GetCodeName(mCode), strCurr, dval2, "", entry);


								break;
							default:
								continue;
							}

							m_grid->SetItemText(xrow, jj, entry);
						}
						
						
					}
					// ADD PSH 20070912
					if (rtmAction == rtmCELL)
						m_grid->Blink(xrow, jj, rtmColor);	
				}

				// END ADD
			}


			
			if (ret)
			{
				hawkEyeColor(xrow);
				m_grid->memoRefresh();
			}

			datB = SplitString(total_str, "\n");
		}

	}

}



void CGridWnd::calcInClient()
{
	CString	code;
	for (int ii = 1; ii < m_rowC; ii++)
	{
		code = m_grid->GetItemText(ii, colCODE);
		code.TrimLeft(); code.TrimRight();
		if (code.IsEmpty()) continue;

		calcInClient(ii);
	}
}

void CGridWnd::calcInClient(int row)
{
	CString	string, stringx;
	double	curr, open, high, low;

	curr  = IH::TOfabs(m_grid->GetItemText(row, colCURR));
	open  = IH::TOfabs(m_grid->GetItemText(row, colOPEN));
	high  = IH::TOfabs(m_grid->GetItemText(row, colHIGH));
	low   = IH::TOfabs(m_grid->GetItemText(row, colLOW));

	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		_gridHdr gridHdr = m_gridHdrX.GetAt(ii);

		if (gridHdr.needs != 8)
			continue;

		string = _T("");

		switch (gridHdr.symbol[2])
		{
		case '1':	// 시가대비(원)
			if (curr <= 0. || open <= 0.)
				break;
			string.Format("%+.0f", curr - open);
			break;
		case '2':	// 시가대비(%)
			if (curr <= 0. || open <= 0.)
				break;
			string.Format("%+.2f%%", (curr - open) / curr * 100);
			break;
		case '3':	// 고가대비(원)
			if (curr <= 0. || high <= 0.)
				break;
			string.Format("%+.0f", curr - high);
			break;
		case '4':	// 고가대비(%)
			if (curr <= 0. || high <= 0.)
				break;
			string.Format("%+.2f%%", (curr - high) / curr * 100);
			break;
		case '5':	// 저가대비(원)
			if (curr <= 0. || low <= 0.)
				break;
			string.Format("%+.0f", curr - low);
			break;
		case '6':	// 저가대비(%)
			if (curr <= 0. || low <= 0.)
				break;
			string.Format("%+.2f%%", (curr - low) / curr * 100);
			break;
		}
		stringx = m_grid->GetItemText(row, ii);
		if (atof(stringx) != atof(string))
		{
			m_grid->SetItemText(row, ii, string);
			if (((CGroupWnd*)m_pGroupWnd)->GetRtmAction() == rtmCELL)
				m_grid->Blink(row, ii, ((CGroupWnd*)m_pGroupWnd)->GetRTMColor());
		}
	}
}

int CGridWnd::CheckRealTimeCode(CString code)
{
	const int realtimeCol = 0; 
	CString string;

	for (int ii = 0; ii < 200 ; ii++)
	{
		m_irowCode[ii] = 0;
	}

	int count = 0;

	for (int ii = 1; ii < m_rowC; ii++)
	{
		string = m_grid->GetItemText(ii, realtimeCol);

		if (string == code) 
		{
			m_irowCode[count] = ii;
			count++;
		}

	}

	return count;
}

/*
int CGridWnd::loadInterest(int gno)
{
	ClearInterest();

	struct _oldinters
	{
		char	gubn[1];	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
		char	code[12];	// 종목코드
		char	name[32];	// 종목명
		char	xprc[10];	// 보유단가
		char	xnum[10];	// 보유수량
		char	xupnum[16];	// 상장 주식수
		char	filler[24];	// Reserved
	};

	struct	_inters* pinters;
	struct  _oldinters oldInter;
	struct  _bookmarkinfo* bInfo;

	UINT newSize = sizeof(_inters);
	UINT oldSize = sizeof(_oldinters);
	UINT fileSize = 0;

	bool isOld = false;			//새 구조체인지 판단

	CString	filePath, fileBook; 

	filePath.Format("%s/%s/%s/%s%02d", m_root, USRDIR, m_user, FILE_SUBDATA, gno);
	fileBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_user, gno);

	if (!ExistFile(gno))
	{
		for (int ii = 0; ii < MAX_LINE ; ii++)
		{
			pinters = (_inters *) new char[sz_inters];
			ZeroMemory(pinters, sz_inters);

			pinters->gubn[0] = '0';
			pinters->filler[0] = 0;

			AddInters(pinters);
		}
		return 0;
	}

	UINT	readL;
	bool	endfile = false;
	bool	endfile2 = false;
	bool    isFile = false;

	CFile	fileH;
	CFile	fileB;
	
	fileH.Open(filePath, CFile::modeRead);

	if( !fileB.Open(fileBook, CFile::modeRead) )
	{
		isFile = true;		//파일 없을경우
	}
	// 
	//파일 사이즈로 옛날 inter인지 신규 inter인지 판단
	fileSize = fileH.GetLength();

	for (int ii = 0; ii < MAX_LINE; ii++)
	{
		pinters = (_inters *) new char[sz_inters];
		oldInter = (_oldinters *) new char[sizeof(_oldinters)];
		bInfo = (_bookmarkinfo *) new char[sizeof(_bookmarkinfo)];

		ZeroMemory(pinters, sz_inters);
		ZeroMemory(oldInter, sizeof(_oldinters));
		ZeroMemory(bInfo, sizeof(_bookmarkinfo));

		if (!endfile)
		{
			if(fileSize%newSize == 0)
			{
				readL = fileH.Read(pinters, sz_inters);
				
				if (readL < sz_inters)
					endfile = true;
			}
			else
			{
				readL = fileH.Read(&oldInter, sizeof(_oldinters));
				isOld = true; 

				if (readL < sizeof(_oldinters))
					endfile = true;
			}
		}
		

		if (endfile2 == false && isFile == false)
		{
			readL = fileB.Read(bInfo, sizeof(_bookmarkinfo));
			
			if(readL < sizeof(_bookmarkinfo))
				endfile2 = true;
		}
	
		if(isOld == true)
		{
			CopyMemory(pinters->code, oldInter.code, sizeof(pinters->code));
			CopyMemory(pinters->name, oldInter.name, sizeof(pinters->name));
			CopyMemory(pinters->gubn, oldInter.gubn, sizeof(pinters->gubn));
			CopyMemory(pinters->xnum, oldInter.xnum, sizeof(pinters->xnum));
			CopyMemory(pinters->xprc, oldInter.xprc, sizeof(pinters->xprc));
			CopyMemory(pinters->xupnum, oldInter.xupnum, sizeof(pinters->xupnum));
			CopyMemory(pinters->filler, oldInter.filler, sizeof(pinters->filler));
		}

		//북마크 세팅
		pinters->filler[0] = bInfo->bookmark;
//		AfxMessageBox(pinters);
		
		AddInters(pinters);
//		TRACE("%s %d\n", pinters->name, pinters->bookmark);
	}

	fileH.Close();

	if(isFile == false)
	{
		fileB.Close();
	}

//	MarkerSetup();
	
	return m_inters.GetSize();
}

int CGridWnd::loadInterest(CString codes)
{
	if (codes.IsEmpty()) return 0;

	ClearInterest();

	CString	code, name;
	struct	_inters* pinters;

	for (int ii = 0; ii < MAX_LINE; ii++)
	{
		code = name = _T("");

		if (!codes.IsEmpty())
		{
			code = IH::Parser(codes, P_TAB);
			if (code.IsEmpty()) continue;

			if (code.GetLength() == 6 || code.GetLength() == 8 || code.GetLength() == 9)
			{
				char wB[64]; 
				strcpy(wB, code);
				char* xxP = (char *)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)wB);
				name = xxP; 
				name.TrimLeft(); 
				name.TrimRight();
			}

			if (name.IsEmpty()) continue;
		}

		pinters = (_inters *) new char[sz_inters];
		ZeroMemory(pinters, sz_inters);

		if (!code.IsEmpty())
		{
			pinters->gubn[0] = '0';	
			strcpy(pinters->code, code);
			strcpy(pinters->name, name);
		}

		AddInters(pinters);
	}

	return m_inters.GetSize();
}
*/

void CGridWnd::ClearInterest()
{
	for (int ii = 0; ii < m_inters.GetSize(); ii++)
		delete m_inters.GetAt(ii);

	m_inters.RemoveAll();
}


bool CGridWnd::ExistFile(int gno)
{
	CString	filePath;

	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, gno);

	DWORD	fLen = 0;
	CFileFind finder;
	if (!finder.FindFile(filePath))
		return false;

	return true;
}

// ADD PSH 20070913
void CGridWnd::CopyInter(_inters* pCpy, _inters* pSrc)
{
	memcpy(pCpy->gubn,		pSrc->gubn,	sizeof(pSrc->gubn));
	memcpy(pCpy->code,		pSrc->code,	sizeof(pSrc->code));
	memcpy(pCpy->name,		pSrc->name,	sizeof(pSrc->name));
	memcpy(pCpy->xprc,		pSrc->xprc,	sizeof(pSrc->xprc));
	memcpy(pCpy->xnum,		pSrc->xnum,	sizeof(pSrc->xnum));
	memcpy(pCpy->xupnum,	pSrc->xupnum, sizeof(pSrc->xupnum));
	memcpy(pCpy->bookmark,	pSrc->bookmark, sizeof(pSrc->bookmark));
	memcpy(pCpy->filler,	pSrc->filler, sizeof(pSrc->filler));
}

void CGridWnd::SetMarkerProp(_marker* pProp)
{
	if (NULL != pProp)
	{
		BOOL bChanged = FALSE;

		if ( (m_clrMarkerBKG != pProp->clrMarkerBKG) ||
			 (m_clrMarkerTXT != pProp->clrMarkerTXT) ||
			 (m_bShadow != pProp->bShadow) )
		{
			bChanged = TRUE;
		}

		if (bChanged)
		{
			m_clrMarkerBKG	= pProp->clrMarkerBKG;
			m_clrMarkerTXT	= pProp->clrMarkerTXT;
			m_bShadow		= pProp->bShadow;

			m_grid->SetBkMarkerColor(m_clrMarkerBKG);
			m_grid->SetMarkerColor(m_clrMarkerTXT);
			m_grid->SetMarkerShadow(m_bShadow);

			m_grid->Invalidate();
		}

		m_bAllApply = pProp->bAllApply;
	}
}

void CGridWnd::MarkerSetup()
{
	_inters* pInter = NULL;

	int nCnt = m_inters.GetSize();

	for (int nRow = 1; nRow < nCnt; nRow++)
	{
		pInter = m_inters.GetAt(nRow - 1);

		if (pInter)
		{
			UINT attr;
			
			if ('m' == pInter->gubn[0])
			{
				attr = m_grid->GetItemAttr(nRow, colNAME) | GVAT_MARKER;
			}
			else
			{
				attr = m_grid->GetItemAttr(nRow, colNAME) & ~GVAT_MARKER;
			}

			m_grid->SetItemAttr(nRow, colNAME, attr);
		}
	}
}

CWnd* CGridWnd::ShowMessage(CString strMsg, int nWidth /* = 100 */, int nHeight /* = 30 */, UINT nDelay /* = 1000 */, BOOL bProgress /* = FALSE */)
{
	return CShowMsgWnd::ShowMessage(m_pMainWnd->GetParent(), this, nWidth, nHeight, strMsg, nDelay, bProgress);
}

void CGridWnd::RemoveAllBaseInters()
{
	int nCnt = m_arrBaseInters.GetSize();
	_inters* pInter = NULL;

	for (int nRow = 0; nRow < nCnt; nRow++)
	{
		pInter = m_arrBaseInters.GetAt(nRow);

		if (pInter)
		{
			delete pInter;
			pInter = NULL;
		}
	}

	m_arrBaseInters.RemoveAll();
}
// END ADD

int CGridWnd::AddInters(struct _inters* pinters)
{ 
	if (strlen(pinters->code))
		m_mapCode.SetAt(pinters->code, pinters->name);

	return m_inters.Add(pinters);
}


void CGridWnd::RemoveInters(int nIndex)
{
	struct _inters* pinters = m_inters.GetAt(nIndex);
	if (strlen(pinters->code))
	{
		// ADD PSH 20070913
		if (!m_bSorting)
		{
			int nRowCnt = m_arrBaseInters.GetSize();
			_inters* pInter = NULL;

			for (int nRow = 0; nRow < nRowCnt; nRow++)
			{
				pInter = m_arrBaseInters.GetAt(nRow);

				if ((pInter) && !strcmp(pInter->code, pinters->code))
				{
					m_arrBaseInters.RemoveAt(nIndex);
					delete pInter;

					break;
				}
			}
		}

		m_mapMargin.RemoveKey(pinters->code);
		// END ADD

		m_mapCode.RemoveKey(pinters->code);
	}

	m_inters.RemoveAt(nIndex);
	delete pinters;

	insertInterest(MAX_LINE - 1);
}

void CGridWnd::SetKind(UINT kind)
{
	m_kind = kind;
	m_szTitle.Format(" %s", (char*)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, kind));

	if (kind != 0)
		((CMainWnd*)m_pMainWnd)->SetRTSTreeData(this);

	InvalidateRect(m_rcTitle, FALSE);
	m_grid->SetSortColumn(-1);
}

CString CGridWnd::FindTitle(UINT kind)
{
	CString title = _T("");
	if (m_pTreeWnd)
		title.Format(" %s",(char*)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, kind));

	return title;
}
void CGridWnd::RecvRTS(char* ptr)
{
	parsingAlert(ptr);
}

void CGridWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1000)
	{
		int	ncnt = m_arExist.GetCount();
		if (ncnt < 0)
		{
			KillTimer(1000);
			return;
		}
		int	nIndex = atoi(m_arExist.GetValue(m_nTurn));
		CString	code = m_grid->GetItemText(nIndex + 1, colCODE);		
		CString	string;

		switch (GetCodeType(code))
		{
		case kospiCODE:
			string.Format("%s\t%s", HCOD, code);
			break;
		case futureCODE:
			string.Format("%s\t%s", FCOD, code);
			break;
		case optionCODE:
			string.Format("%s\t%s", OCOD, code);
			break;
		case foptionCODE:
			string.Format("%s\t%s", PCOD, code);
			break;
		case indexCODE:
			string.Format("%s\t%s", UCOD, code);
			break;
		case sinjuCODE:
			string.Format("%s\t%s", SINJUCOD, code);
			break;
		case elwCODE:
			string.Format("%s\t%s", ELWCOD, code);			
			break;
		}
		
		Variant(triggerCC, string); 
		m_grid->SetFocus();
		m_grid->SetSelectRow(nIndex + 1);
		m_nTurn++;
		m_nTurn = m_nTurn % ncnt;
	}
	CBaseWnd::OnTimer(nIDEvent);
}


void CGridWnd::rebuildInterest()
{
	CString	code, name;

	// ADD PSH 20070913
/*	if (1 > m_arrBaseInters.GetSize())
	{
		_inters* pInter = NULL;
		int nRowCnt = m_inters.GetSize();

		for (int nRow = 0; nRow < nRowCnt; nRow++)
		{
			pInter = m_inters.GetAt(nRow);
			if ( (pInter) && !(pInter->gubn[0] == '0' && pInter->gubn[1] == '\0') )
			{
				_inters* pBaseInter = (_inters *) new char[sz_inters];
				ZeroMemory(pBaseInter, sz_inters);

				CopyInter(pBaseInter, pInter);

				m_arrBaseInters.Add(pBaseInter);
			}
		}
	}*/
	// END ADD

	struct	_inters* pinters = NULL;
	for ( int ii = 0; ii < MAX_LINE ; ii++ )
	{
		pinters = m_inters.GetAt(ii);
		code = m_grid->GetItemText(ii+1, colCODE);
		
		

// 		if(m_grid->GetItemAttr(ii, colNAME) & GVAT_MARKER)
// 		{
// 			if(code.IsEmpty())
// 			{
// 				code.Format("m%05d", ii);
// 			}
// 		}
// 		else
// 		{
// 			code = m_grid->GetItemText(ii+1, colCODE);
// 		}
		code = m_grid->GetItemText(ii+1, colCODE);
		name = m_grid->GetItemText(ii+1, colNAME);
		code.TrimRight();
		
		memset(pinters, 0x00, sizeof(struct _inters));
		pinters->gubn[0] = '0';
		strcpy(pinters->code, code);
		strcpy(pinters->name, name);
	}
}

void CGridWnd::Reset(bool bAll)
{
	KillTimer(1000);
	RemoveAll();
	int	nActive = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ACTIVE));
	if (nActive == m_nIndex)
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET));
	m_grid->Clear();

	if (bAll)
	{
		m_szTitle = _T("");
		SetKind(0);		
	}
}


//화면 번호 tab 개수 tab 종목 번호 | 종목 번호 | 종목 번호

void CGridWnd::sendtoMutiHoga()
{
	struct _inters* pInters;
	int nScnt = 0;
	CString code(_T("")), string(_T("")), data(_T("")), string_code(_T(""));
	char chTab = P_TAB;
	char chLf = P_NEW;

	int i = 0;

	for (i = m_inters.GetUpperBound(); i >= 0; i--)
	{
		pInters = m_inters.GetAt(i);
		
		if (strlen(pInters->code) > 0)
			break;
	}
	
	if (i < 0)
		nScnt = 0;
	else
		nScnt = i;
	
// 	if(nScnt > 32)
// 	{
// 		MessageBox("최대 32개까지만 종목 연동이 가능합니다", "IBK투자증권");
// 		nScnt = 32;
// 	}
	
	int rowCount;

	rowCount = 0;

	
	for (int i = 0; i <= nScnt; i++)
	{
		pInters = m_inters.GetAt(i);

		if(pInters->code[0] != 'm')
		{

			if(pInters->code[0] != NULL)
			{
				string_code += pInters->code;
				string_code += "|";
				rowCount++;
			}

		}
	}

	CInterDomino dlgInter(m_pMainWnd, rowCount, string_code);
	
	dlgInter.DoModal();

//	string += chLf;
	

}

void CGridWnd::saveInterestVisible(int gno)
{
	//파일의 내용을 읽어와서 서버에 저장한다.
	CSendData sData;
	CString strPath(_T("")), strTemp(_T("")), strBook(_T(""));
	CString strSendData(_T(""));
	
	strPath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, gno);
	
	char szTemp[10];
	int nScnt = 0;
	
	strPath += ".tmp";	
	struct _inters* pInters;

	if (!ExistFile(gno))
	{
		return;
	}
	
	UINT	readL;
	bool	endfile = false;
	bool    isFile = false;

	CFile	fileH;
	
	fileH.Open(strPath, CFile::modeRead);

	struct _updn updn;
	
	FillMemory(&updn, sizeof(_updn), ' ');
	ZeroMemory(&updn, sizeof(_updn));
	
	CopyMemory(&updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'G';
	CopyMemory(updn.uinfo.nblc, _T("00001"), sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'O';

	sprintf(szTemp, "%02d", gno);
	CString strGrouptName = GetGroupName(gno);

	CopyMemory(updn.ginfo.gnox, szTemp, sizeof(updn.ginfo.gnox));
	CopyMemory(updn.ginfo.gnam, (LPCTSTR)strGrouptName, min(sizeof(updn.ginfo.gnam), strGrouptName.GetLength()));

	int fileSize = fileH.GetLength();
	UINT newSize = sizeof(_inters);

	int nrec = fileSize/newSize;
	sprintf(szTemp, "%04d", nrec + 1); 

	CopyMemory(updn.ginfo.jrec, szTemp, sizeof(updn.ginfo.jrec));
	
	strSendData = CString((char*)&updn, sizeof(_updn));

	for (int i = 0; i < MAX_LINE; i++)
	{
		pInters = (_inters *) new char[sz_inters];
		ZeroMemory(pInters, sz_inters);

		struct _jinfo jinfo;
		FillMemory(&jinfo, sizeof(_jinfo), ' ');
		
		if(!endfile)
		{
			readL = fileH.Read(pInters, sz_inters);
			
			if (readL < sz_inters)
				endfile = true;
		}

		jinfo.gubn[0] = pInters->gubn[0];
		
		if(strlen(pInters->code) == 0)
		{
			CopyMemory(jinfo.code, "      ", sizeof(jinfo.code));
			CopyMemory(jinfo.xprc, "      ", sizeof(jinfo.xprc));
			CopyMemory(jinfo.xnum, "      ", sizeof(jinfo.xnum));
		}
		else
		{
			CopyMemory(jinfo.code, pInters->code, sizeof(jinfo.code));
			CopyMemory(jinfo.xprc, pInters->xprc, sizeof(jinfo.xprc));
			CopyMemory(jinfo.xnum, pInters->xnum, sizeof(jinfo.xnum));
		}
		
		strSendData += CString((char*)&jinfo, sizeof(_jinfo));
	}
	fileH.Close();
	
	char	key;
	_trkey* trkey = (struct _trkey*)&key;
	trkey->kind = TRKEY_GRIDSAVE;
	trkey->group = m_nIndex;
	
	sData.SetData(trUPDOWN, key, (LPSTR)(LPCTSTR)strSendData, strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
	m_pMainWnd->SendMessage(WM_MANAGE, MK_PROCDLL);
	
	CString string = "OnPortfolio\tok";
	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR) string);
	m_bEditWork = TRUE;

	OnAllsave();
}


void CGridWnd::saveInterest(bool btmp, int gno )
{
	if (gno < 0)
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			gno = CAST_TREEID(m_kind)->value;
		else
			return;
	}
	
	CWnd*	pTreeWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	int	nOver = (int)pTreeWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	//분할로 보기모드일 경우
	if(nOver == MO_VISIBLE)
	{
		//분할로 보여지고 있는 그룹 저장하기
		int gno = ((CGroupWnd*)m_pGroupWnd)->WriteFileSumtoEachGroup(m_kind);		

		//서버에 올리기
		saveInterestVisible(gno);
		return;
	}

	CSendData sData;
	CString strPath(_T("")), strTemp(_T("")), strBook(_T(""));
	CString strSendData(_T(""));
	
	strPath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, gno);
	strBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_user, gno);

	char szTemp[10];
	int nScnt = 0;
	
	if(btmp)
		strPath += ".tmp";
	
	struct _inters* pInters;

	int i = 0;
	for (int i = m_inters.GetUpperBound(); i >= 0; i--)
	{
		pInters = m_inters.GetAt(i);
		
		if (strlen(pInters->code) > 0)
			break;
	}
	
	if (i < 0)
		nScnt = 0;
	else
		nScnt = i;
	
	::DeleteFile(strPath);
	CFile	file(strPath, CFile::modeWrite|CFile::modeCreate);

	//북마크만 따로 파일로 관리 <임시>
	::DeleteFile(strBook);
	CFile	file2(strBook, CFile::modeWrite|CFile::modeCreate);

	struct _updn updn;
	
	FillMemory(&updn, sizeof(_updn), ' ');
	ZeroMemory(&updn, sizeof(_updn));

	CopyMemory(&updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'G';
	CopyMemory(updn.uinfo.nblc, _T("00001"), sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'O';
	
	sprintf(szTemp, "%02d", gno);
	CString strGrouptName = GetGroupName(gno);
	
	CopyMemory(updn.ginfo.gnox, szTemp, sizeof(updn.ginfo.gnox));
	CopyMemory(updn.ginfo.gnam, (LPCTSTR)strGrouptName, min(sizeof(updn.ginfo.gnam), strGrouptName.GetLength()));
	sprintf(szTemp, "%04d", i + 1); 
	CopyMemory(updn.ginfo.jrec, szTemp, sizeof(updn.ginfo.jrec));
	
	strSendData = CString((char*)&updn, sizeof(_updn));
	
	for (i = 0; i <= nScnt; i++)
	{
		pInters = m_inters.GetAt(i);
		
		if (strlen(pInters->code) && strlen(pInters->name) == 0)
		{
			CString strName = m_grid->GetItemText(i + 1, colNAME);
			strcpy(pInters->name, strName);
		}
		
		struct _jinfo jinfo;
		FillMemory(&jinfo, sizeof(_jinfo), ' ');
		
		jinfo.gubn[0] = pInters->gubn[0];

		if(strlen(pInters->code) == 0)
		{
			CopyMemory(jinfo.code, "      ", sizeof(jinfo.code));
			CopyMemory(jinfo.xprc, "      ", sizeof(jinfo.xprc));
			CopyMemory(jinfo.xnum, "      ", sizeof(jinfo.xnum));
			CopyMemory(pInters->code, "            ", sizeof(pInters->code));
			CopyMemory(pInters->name, "            ", sizeof(pInters->name));
		}
		else
		{
			CopyMemory(jinfo.code, pInters->code, sizeof(jinfo.code));
			CopyMemory(jinfo.xprc, pInters->xprc, sizeof(jinfo.xprc));
			CopyMemory(jinfo.xnum, pInters->xnum, sizeof(jinfo.xnum));
		}

		strSendData += CString((char*)&jinfo, sizeof(_jinfo));

		//북마크를 제외하고 저장하기 위해!
		file.Write(pInters, sz_inters);

		//북마크만 따로 파일로 관리
		struct _bookmarkinfo binfo;
//		FillMemory(&binfo, sizeof(_bookmarkinfo), ' ');
		ZeroMemory(&binfo, sizeof(_bookmarkinfo));

		if(strlen(pInters->code) == 0)
		{
			CopyMemory(binfo.code, "            ", sizeof(binfo.code));
			CopyMemory(binfo.name, "            ", sizeof(binfo.name));
			binfo.bookmark[0] = '0';
		}
		else
		{
			CopyMemory(binfo.code, pInters->code, sizeof(binfo.code));
			CopyMemory(binfo.name, pInters->name, sizeof(binfo.name));
			binfo.bookmark[0] = pInters->bookmark[0];
		}

		file2.Write(&binfo, sizeof(_bookmarkinfo));

	}
	file2.Close();
	file.Close();
	

	char	key;
	_trkey* trkey = (struct _trkey*)&key;
	trkey->kind = TRKEY_GRIDSAVE;
	trkey->group = m_nIndex;
	
	sData.SetData(trUPDOWN, key, (LPSTR)(LPCTSTR)strSendData, strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
	m_pMainWnd->SendMessage(WM_MANAGE, MK_PROCDLL);
	
	CString string = "OnPortfolio\tok";
	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR) string);
	m_bEditWork = TRUE;

	//081215 :: 오른쪽 팜업메뉴로 관심그룹 저장시 저장 안되던 문제 해결
	OnAllsave();
}


int	CGridWnd::GetRowcount()
{
	struct _inters* pInters;
	int maxRow = m_inters.GetUpperBound();

	for (int i = maxRow ; i >= 0; i--)
	{
		pInters = m_inters.GetAt(i);
		
		if (strlen(pInters->code) > 0)
		{
			break;
		}
	}

	return i+1;
}

CString CGridWnd::GetcolName(int index)
{ 
	return m_grid->GetItemText(index + 1, colNAME); 
}


CString	CGridWnd::GetGroupName(int gno)
{
	CFileFind finder; CString filePath;
	
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_GROUP);
	
	CString	string; string.Format("%02d", gno);
	int	wbL = 0; char	wbX[128];
	wbL = GetPrivateProfileString("GROUPNAME", string, NULL, wbX, sizeof(wbX), filePath);
	
	if (wbL <= 0)
	{
		sprintf(wbX, "%s%02d", "관심종목", gno);
		wbL = strlen(wbX);
	}
	
	return CString(wbX, wbL);
}

//081215 :: 오른쪽 팜업메뉴로 관심그룹 저장시 저장 안되던 문제 해결
void CGridWnd::OnAllsave()
{
	DWORD	fLen = 0; 
	CFileFind finder;
	CString	filePath, fileTemp;
	int itemcount = 0;
	
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, "portfolio.ini");
	fileTemp.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, "portfolio.ini.tmp");
	
	if (finder.FindFile(fileTemp))
		CopyFile(fileTemp, filePath, FALSE);
	
	for (int ii = 0 ; ii < 100 ; ii++)
	{
		filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_user, ii);
		fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_user, ii);
		
		if (!finder.FindFile(fileTemp))
			continue;
		
		CopyFile(fileTemp, filePath, FALSE);
	}
}



// 관심종목저장 버튼 선택시
void CGridWnd::saveInterestX()
{
	CString	code, bookmark, string, stringx;
 	struct	_inters* pinters;

	CWnd*	pTreeWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	CWnd*	pGroupWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	int	nOver = (int)pTreeWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	

	int scnt = 0;
	for (scnt = m_inters.GetUpperBound() ; scnt >= 0 ; scnt--)
	{
		pinters = m_inters.GetAt(scnt);
		if (strlen(pinters->code) > 0)
			break;
	}

	if (scnt < 0)
	{
		Variant(guideCC, "선택된 그룹에 종목이 없습니다.");
		return;
	}

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{

		if(m_bEditWork)
		{
			saveInterest(true);

			m_bEditWork = FALSE;
		}
		else
		{
			string = "해당 관심그룹의 변경된 내용이 없습니다";
			m_pMainWnd->SendMessage(WM_MANAGE, MK_SHOWTOOLTIP, (LPARAM)string.operator LPCTSTR());
		}
	}
	else
	{

		//그룹명 검색해서 내관심종목에 vol 추가하기
		stringx = makeGroupName();
//		stringx.Format("%s%c", _T("내관심종목"), P_DELI);
		
		for (int ii = 0; ii <= scnt; ii++)
		{
			pinters = m_inters.GetAt(ii);

			code = CString(pinters->code, sizeof(pinters->code));
			code.TrimLeft(); 
			code.TrimRight();
			
			if(pinters->bookmark[0] == '\0')		//NULL
			{
				pinters->bookmark[0] = '0';
			}

			string.Format("%s%c%c%c",code, P_DELI ,pinters->bookmark[0], P_DELI);
			stringx += string;
		}
		
		CString tem; tem.Format("[%s]", stringx);

		string.Format("%s /p5 /S/d%s%c%s%c", MAP_CONFIG, "appendParentGROUP", P_TAB, stringx, P_NEW);
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_OPENSCR, typePOPUP), (LPARAM)string.operator LPCTSTR());
	}
}

CString	CGridWnd::makeGroupName()
{
	CString stringx, string, gno, gname, gnameLTrim, gnameRTrim, strPath;
	int readL = 0, idx = 0, makeGNo = 0;
	char readB[1024];
	bool isTrue = false;
	CString name = _T("내관심종목");
	strPath.Format("%s/%s/%s/portfolio.ini", m_root, USRDIR, m_user);

	readL = GetPrivateProfileString(SEC_GROUPORDER, "00", "", readB, sizeof(readB), strPath);
	string = CString(readB, readL);

	for(int ii=0 ; !string.IsEmpty() ; ii++)
	{
		gno = IH::Parser(string, ";");
		readL = GetPrivateProfileString(SEC_GROUPNAME, gno, "", readB, sizeof(readB), strPath);
		gname = CString(readB, readL);

		//내관심종목일 경우 그룹 넘버링 메이킹
		if(!gname.IsEmpty())
		{
			gnameLTrim = gname.Left(10);
			
			if(gnameLTrim == "내관심종목")
			{
				isTrue = true;

				if(gname.GetLength() > 11)
				{
					gnameRTrim = gname.Mid(10, 2);
				}
				else
				{
					gnameRTrim = "0";
				}
				
				
				if(!gnameLTrim.IsEmpty())
				{
					idx = atoi(gnameRTrim);

					if(idx > makeGNo)
					{
						makeGNo = idx;
					}
				}
			}
		}
	}
	
	if(isTrue == true)
	{
		makeGNo++;

		stringx.Format("%s%02d%c", name, makeGNo, P_DELI);
	}
	else
	{
		makeGNo = 1;
		stringx.Format("%s%02d%c", name, makeGNo, P_DELI);
	}
	
	
	return stringx;
}

LRESULT CGridWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_ERASEBKGND)
		return 1;
	return CBaseWnd::WindowProc(message, wParam, lParam);
}

// int counts = 0;
// CString datas[6] = {"A000660\t023\t+3333\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t111\t+2222\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t111\t-2222\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t023\t+4444\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t023\t+3334\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t111\t+2223\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t111\t-2223\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t023\t+4445\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t023\t+3335\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t111\t+2224\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t111\t-2224\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t023\t+4446\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n"};
BOOL CGridWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	switch (LOWORD(wParam))
	{
	case IDC_BT_DOMINO:
		sendtoMutiHoga();
		break;

	case IDC_BT_SAVE:
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)m_nIndex);
		saveInterestX();
		break;
	case IDC_BT_ERASE:
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_CLOSEGRID, m_nIndex));
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_INITGRID);
		break;
	}
	return CBaseWnd::OnCommand(wParam, lParam);
}

void CGridWnd::DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect	rect;
	int	nWidth, nHeight;

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs,gs,bs;
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re,ge,be;
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep, gStep, bStep;
	CPen	*sPen;
	if (bRight)
	{
		if (!nWidth)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nWidth);
		gStep = float(gs - ge)/float(nWidth);
		bStep = float(bs - be)/float(nWidth);

		if (!nHeight)
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left + ii, drawRC.top);
				pDC->LineTo(drawRC.left + ii + 1, drawRC.top);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nHeight);
		gStep = float(gs - ge)/float(nHeight);
		bStep = float(bs - be)/float(nHeight);

		if (!nWidth)
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left, drawRC.top + ii);
				pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
}

void CGridWnd::Assign(CGridWnd* pGrid)
{
//	Reset();
	int* nRowCount = NULL;
	int temp = 0;
    nRowCount = &temp;

	int	nActive = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ACTIVE));
	if (nActive == m_nIndex)
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET));
	m_grid->Clear();

	m_szTitle = _T("");
	SetKind(0);		

	SetKind(pGrid->GetKind());
	m_gridHdrX.RemoveAll();

	//그리드의 row 재설정
	RemoveAll();

	pGrid->GetHearder(m_gridHdrX);
	pGrid->GetData(m_inters, nRowCount);	//m_inters의 row개수를 반환
	
	for(int ii=1 ; ii<*nRowCount ; ii++)
	{
		insertRow(ii, FALSE);
	}

	MarkerSetup();

	
/*	int nRow = m_grid->GetRowCount() - 1;
	for (; nRow > 1; nRow--)
	{
		m_grid->DeleteRow(nRow);
		m_grid->memoDelete(nRow);
	}
	
	for (int i = 0; i < m_colC; i++)
	{
		m_grid->SetItemText(1, i, "");
		m_grid->SetItemData(1, i, 0);
		m_pTicker->SetItemText(0, i, "");
		m_pTicker->SetItemData(0, i, 0);
	}
	m_rowC = 1;

	pGrid->GetHearder(m_gridHdrX);
	//pGrid->GetData(m_inters);
	pGrid->GetData(m_inters, nRowCount);	//m_inters의 row개수를 반환
	
	for(int ii=0 ; ii<*nRowCount ; ii++)
	{
		insertRow(ii, FALSE);
	}	
*/
}

void CGridWnd::GetHearder(CArray< _gridHdr, _gridHdr >& arHeader)
{
	arHeader.Copy(m_gridHdrX);
}

void CGridWnd::GetData(CArray <_inters*, _inters*>& arData)
{
	int	ncnt = arData.GetSize();
	int nTot = m_inters.GetSize();
	int nMax = 0;

	struct _inters*	pInter1 = NULL;
	struct _inters*	pInter2 = NULL;

	if ( ncnt >= nTot ) nMax = nTot;
	else nMax = ncnt;

	for ( int ii = 0 ; ii < nMax ; ii++ )
	{
		pInter1 = arData.GetAt(ii);
		pInter2 = m_inters.GetAt(ii);
		memcpy(pInter1, pInter2, sizeof(struct _inters));
	}
}

void CGridWnd::GetData(CArray <_inters*, _inters*>& arData, int* nRowCount)
{
	int	ncnt = arData.GetSize();
	int nTot = m_inters.GetSize();
	int nMax = 0;
	char tCode[12];
	int rCnt = 0;
	
	struct _inters*	pInter1 = NULL;
	struct _inters*	pInter2 = NULL;
	
	if ( ncnt >= nTot ) nMax = nTot;
	else nMax = ncnt;
	
	for (int ii = 0 ; ii < nMax ; ii++ )
	{
		pInter1 = arData.GetAt(ii);
		pInter2 = m_inters.GetAt(ii);
		
		ZeroMemory(tCode, sizeof(tCode));
		CopyMemory(tCode, m_inters.GetAt(ii)->code, sizeof(tCode));
		
		if(CString(tCode).IsEmpty() == false)
		{
			rCnt++;
		}

		memcpy(pInter1, pInter2, sizeof(struct _inters));
	}
	
	*nRowCount = rCnt;
}

void CGridWnd::toggleAction(bool toggle)
{
	if (!GetKind())
		return;

	m_bExpect = toggle;

	struct	_symbol {
		UINT	stid1;
		char*	symb1;
		UINT	stid2;
		char*	symb2;
	} chksym [] = {
		{ IDS_GH_CURR,	"2023",	IDS_GH_ANTIPRC,	"2111" },
		{ IDS_GH_DIFF,	"2024",	IDS_GH_DIFF,	"2115" },
		{ IDS_GH_RATE,	"2033",	IDS_GH_RATE,	"2116" },
		{ IDS_GH_VOL,	"2027",	IDS_GH_ANTIVOL,	"2112" },
	};
	const	int chksymC = sizeof(chksym) / sizeof(_symbol);


	if (toggle)		// 예상체결가 형태로 변경
	{
		_gridHdr gridHdr;
		for (int ii = 0; ii < chksymC; ii++)
		{
			for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);
				if (atoi(gridHdr.symbol) != atoi(chksym[ii].symb1))
					continue;
			
				gridHdr.stid = chksym[ii].stid2;
				strcpy(gridHdr.symbol, chksym[ii].symb2);

				m_gridHdrX.SetAt(jj, gridHdr);
				break;
			}
		}
		
		for ( int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
		{
				gridHdr = m_gridHdrX.GetAt(jj);
				
		}
	}
	else
	{
		_gridHdr gridHdr;
		for (int ii = 0; ii < chksymC; ii++)
		{
			for (int jj = colCURR ; jj < m_gridHdrX.GetSize() ; jj++)
			{
				gridHdr = m_gridHdrX.GetAt(jj);
				if (atoi(gridHdr.symbol) != atoi(chksym[ii].symb2))
					continue;
			
				gridHdr.stid = chksym[ii].stid1;
				strcpy(gridHdr.symbol, chksym[ii].symb1);

				m_gridHdrX.SetAt(jj, gridHdr);
				break;
			}
		}
	}

	initialGrid(false); 
	sendTransaction();
}

void CGridWnd::toggleAutoAction(bool toggle)
{
	if (!GetKind())
		return;
	
	m_bAutoExpect = toggle;
	sendTransaction();
}

void CGridWnd::queryNewsCode()
{
	struct  mid {
	    char    func[1];        /* '1':최근100종목      */
	    char    date[8];        /* 날짜                 */
	} mid;

	memset(&mid, ' ', sizeof(mid));
	mid.func[0] = '1';
	CString senddata = CString((char*)&mid, sizeof(mid));
	CSendData	sdata;
	char	key;
	_trkey* trkey = (struct _trkey*)&key;
	trkey->kind = TRKEY_NEWS;
	trkey->group = m_nIndex;
	sdata.SetData("PIBO2022", key, (char*)senddata.operator LPCTSTR(), senddata.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
}

void CGridWnd::parsingNewsCode(CRecvData* data)
{
	struct grid
	{
		char cod2[12];
	};

	struct  mod 
	{
	    char    nrec[4];
	    struct grid	grid[1];
	}* mod;

	char*	buf = (char*)data->m_lParam;
	mod = (struct mod*)buf;
	CString	nrec, code;
	int	ncnt = 0;
	int	nsize = sizeof(struct grid);
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);

	SetKind(MAKE_TREEID(xISSUE));

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'A')
				code = code.Mid(1);
			strcpy(m_inters.GetAt(ii)->code, code);
		}
	}
	
	sendTransaction();
}

void CGridWnd::parsingNews(CString datB)
{
	int	idx = 0;
	CString	code = _T(""), szTitle = _T(""), symbol = _T(""), entry = _T("");

	while (!datB.IsEmpty())
	{
		idx = datB.Find('\t');
		if (idx == -1) break;

		symbol = datB.Left(idx++);
		datB = datB.Mid(idx);

		idx = datB.Find('\t');
		if (idx == -1)
		{
			entry = datB;	datB = _T("");
		}
		else
		{
			entry = datB.Left(idx++); datB = datB.Mid(idx);
		}
		
		if (symbol.Compare("301") == 0)
		{
			code = entry;
			code.TrimRight();
			break;
		}
	}	

	if (!code.IsEmpty())
	{
		int	ncnt = m_arExist.GetCount();
		int	nIndex = 0;
		struct _inters*	pInter = NULL;

		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			nIndex = atoi(m_arExist.GetValue(ii));
			pInter = m_inters.GetAt(nIndex);
			
			if (strcmp(pInter->code, (char*)code.operator LPCTSTR()) == 0)
			{
				LPARAM	lParam = m_grid->GetItemData(nIndex + 1, colSIG);
				WORD	low = LOWORD(lParam);
				WORD	high = 1;
				m_grid->SetItemData(nIndex + 1, colSIG, MAKELPARAM(low, high));
				if (m_bWaveApply) PlayWave();
//				break;		-> 종목 중복 등록 허용으로 인해 수정
			}
		}		
	}
}

CString CGridWnd::GetCodeName(CString code)
{
	CString	ret = _T("");
	CString	sztmp;
	sztmp.Format("%s", code);
	ret.Format("%s", (char *)m_pView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)sztmp.operator LPCTSTR()));
	ret.TrimLeft(), ret.TrimRight();
	return ret;
}

void CGridWnd::OperAlarm(int kind, char* lParam)
{
	LPARAM	data = 0;
	WORD	hval = 0;
	int	ncnt = m_arExist.GetCount();
	int	nindex = -1;

	if (kind == MO_INIT)
	{		
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			nindex = atoi(m_arExist.GetValue(ii));
			data = m_grid->GetItemData(nindex + 1, colSIG);
			hval = HIWORD(data);
			data = MAKELPARAM(0, hval);
			m_grid->SetItemData(nindex + 1, colSIG, data);
		}
	}
	else
	{
		CString	strA, strB;
		strA.Format("%s", lParam);
		if (m_arExist.GetCount())
		{
			strB = m_arExist.GetData(strA);
			strB.TrimRight();
			if ( strB.IsEmpty() ) return;
			nindex = atoi(strB);			
			data = m_grid->GetItemData(nindex + 1, colSIG);
			hval = HIWORD(data);
			data = MAKELPARAM(1, hval);
			m_grid->SetItemData(nindex + 1, colSIG, data);
			if ( nindex >= 0 ){
			m_grid->m_nAlmRow = nindex + 1;
			m_grid->Invalidate();
			}
		}
	}
}

void CGridWnd::ReIndexing()
{
	m_arExist.RemoveAll();
	CString	sztmp, szindex;
	for ( int ii = 0 ; ii < MAX_LINE ; ii++ )
	{
		sztmp = m_grid->GetItemText(ii+1, 0);
		if (!sztmp.IsEmpty())
		{	
			szindex.Format("%d", ii);
			m_arExist.SetData(sztmp, szindex);
		}
	}
}

int CGridWnd::GetIndex(CString symbol)
{
	int	ncnt = m_gridHdrX.GetSize();	
	struct _gridHdr	gridHdr;
	int	ret = -1;

	for ( int ii = colCURR + 1 ; ii < ncnt ; ii++ )
	{
		gridHdr = m_gridHdrX.GetAt(ii);

		if (symbol.CompareNoCase(gridHdr.symbol) == 0)
		{
			ret = ii;
			break;
		}
	}

	return ret;
}


CString CGridWnd::CalcuPyungaSonik(struct _inters* pinters, CString curr)
{
	CString	ret = _T("");
	double	dval1 = IH::TOfabs(IH::charTOstring(pinters->xnum, sizeof(pinters->xnum)));
	double	dval2 = IH::TOfabs(IH::charTOstring(pinters->xprc, sizeof(pinters->xprc)));
	double	dval3 = IH::TOfabs(curr);
	double	maeipPrc = pinters->maeipPrc;
	double  creditPrc = pinters->creditPrc;
	double	result = 0;
	double rPrice = 0;

	if (dval1 <= 0 || dval2 <= 0)
		return ret;

	
//	평가손익 = 평가금액(신용금액 차감) - 매입금액
	rPrice = (dval3 * dval1) - creditPrc;					 
	
	if(maeipPrc != 0)
	{
		result = rPrice - maeipPrc;								
	}
	else
	{
		result = rPrice - (dval2 * dval1);					
	}


	if(result > 0)
	{
		ret.Format("%+.0f", result);
	}
	else if(result < 0)
	{
		ret.Format("-%.0f", result);
	}

	return ret;
}

CString CGridWnd::CalcuSuik(struct _inters* pinters, CString curr)
{
	CString	ret = _T("");
	double	dval1 = IH::TOfabs(IH::charTOstring(pinters->xnum, sizeof(pinters->xnum)));
	double	dval2 = IH::TOfabs(IH::charTOstring(pinters->xprc, sizeof(pinters->xprc)));
	double	dval3 = IH::TOfabs(curr);
	double  creditPrc = pinters->creditPrc;
	double	maeipPrc = pinters->maeipPrc;
	double	result = 0;
	double resultVal = 0;
	double rPrice = 0;

	if (dval1 <= 0 || dval2 <= 0)
		return ret;

//	평가손익 = 평가금액(신용금액 차감) - 매입금액
	rPrice = (dval3 * dval1) - creditPrc;					 
	
	if(maeipPrc != 0)
	{
		result = rPrice - maeipPrc;								
	}
	else
	{
		result = rPrice - (dval2 * dval1);					
	}
	
//	ret.Format("%+.0f", result);

//	resultVal = atof(ret);

	//  수익률 = 평가손익 * 100 / 매입가
	if(maeipPrc != 0)
	{
		ret.Format("%+.2f", ((result * 100.0) / maeipPrc));	
	}
	else
	{
		ret.Format("%+.2f", ((result * 100.0) / (dval2 * dval1)));	
	}

	return ret;
}

CString CGridWnd::CalFutureEvalPrice(struct _inters* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian)
{
	//	if (!m_dBase)	
	getBase(m_code);
	
	double maeipPrc = pinters->maeipPrc;	//원장에서 받은 매입금액
	
	double dSonik = 0, dGubn = 1, dTemp = 0.;
	bool	bMd = false;
	CString m_pprice, m_pgsonik;
	double m_pmaeip = 0;
	
	if(m_curr == 0)
	{
		return "";
	}
	
	int kind = getJKind(m_code);
	if (m_gubn == "1")
		bMd = true;
	if (bMd)	dGubn = -1;
	
	
	
	m_pprice.Format("%.f", m_remian * m_curr * m_dBase);
	
	if(maeipPrc != 0)
	{
		m_pmaeip = maeipPrc;
	}
	else
	{
		m_pmaeip = m_remian * m_maip * m_dBase;
	}
	
	dSonik = atof(getFormatData(m_pprice)) - m_pmaeip;
	dSonik *= dGubn;
	
	TRACE("현재가 : %f * 잔고 : %f * 기본단위 : %f = %s, 매입금액 = %f,  \n", m_curr, m_remian, m_dBase, m_pprice, m_pmaeip);
	//	TRACE("현재가 : %f 기본단위 : %f 평가금액 : %s - 매입금액 : %f = 평가손익 : %f\n", m_curr, m_dBase, m_pprice, m_pmaeip, dSonik);
	if (dSonik > 0)
		m_pgsonik.Format("+%.0f", dSonik);
	else if (dSonik < 0)
		m_pgsonik.Format("-%.0f", dSonik);
	
	return m_pgsonik;
}

CString CGridWnd::CalFutureEvalRate(struct _inters* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian)
{
	getBase(m_code);
	
	double maeipPrc = pinters->maeipPrc;	//원장에서 받은 매입금액

	double dSonik = 0, dGubn = 1, dTemp = 0.;
	bool	bMd = false;
	CString m_pprice, m_pgsonik, m_suik;
	double m_pmaeip = 0;
	
	if(m_curr == 0)
	{
		return "";
	}
	
	int kind = getJKind(m_code);
	if (m_gubn == "1")
		bMd = true;
	if (bMd)	dGubn = -1;
	
	m_pprice.Format("%.f", m_remian * m_curr * m_dBase);

	if(maeipPrc != 0)
	{
		m_pmaeip = maeipPrc;
	}
	else
	{
		m_pmaeip = m_remian * m_maip * m_dBase;
	}
	
	dSonik = atof(getFormatData(m_pprice)) - m_pmaeip;	
	dSonik *= dGubn;
	
	// 수익률
	dSonik *= 100;
	dSonik /= m_pmaeip;//(atof(m_rprice) * m_dBase * atof(m_remian));
	m_suik.Format("%.2f", dSonik);
	if (dSonik < 0)
		m_suik = "" + m_suik;
	else if (dSonik > 0)
		m_suik = "+" + m_suik;
	
	return m_suik;
}

void CGridWnd::SetGridBkColor(COLORREF color)
{ 
	color = RGB(255,255,255);////BUFFET FOR FIXED BACKCOLOR

	if (m_grid)
	{
		m_grid->SetBkColor(color);

		// ADD PSH 20070911
		m_grid->SetGridColor(GetAxColor(65));

		m_pTicker->SetFixBkColor(color);
		m_pTicker->SetGridColor(GetAxColor(65));

		m_grid->Invalidate();
		m_pTicker->Invalidate();

		m_btDomino.Invalidate(TRUE);
		m_btSAVE.Invalidate(TRUE);
		m_btCLOSE.Invalidate(TRUE);	
		// END ADD

		initialGrid(false);//BUFFET
	}
}

void CGridWnd::ReIndexSpecial()
{
	int	ncnt = m_gridHdrX.GetSize();
	UINT	needs = 0;
	CString	string;
	for (int ii = 0; ii < ncnt; ii++)
	{
		needs = m_gridHdrX.GetAt(ii).needs;
		if (needs == 2)
			continue;

		string.Format("%s", m_gridHdrX.GetAt(ii).symbol);

		if (needs == 9)
		{
			if (string.Compare("###3") == 0)
				m_sonikField = ii;
			if (string.Compare("###4") == 0)
				m_suikField = ii;
		}

		if (needs == 7)
			m_bongField = ii;
		
		//전일거래대비율 컬럼 index 변경
		if (needs == 10)
			m_yDayVolField = ii;

		if (needs == 11)
			m_EqualizerField = ii;
	}	
}

void CGridWnd::PlayWave(CString strFile /* = "" */)
{
	if (strFile.IsEmpty())
	{
		strFile = m_strWaveFile;
	}

	CFileFind ff;

	if (ff.FindFile(strFile))
		sndPlaySound(strFile, SND_ASYNC);
}

void CGridWnd::SetMarketColor(BOOL bApply, COLORREF clrKospi, COLORREF clrKosdaq)
{
	m_bMKClrApply = bApply;
	m_clrKospi    = clrKospi;
	m_clrKosdaq   = clrKosdaq;

	m_grid->m_bMKClrApply = m_bMKClrApply;
	m_grid->m_clrKospi  = m_clrKospi;
	m_grid->m_clrKosdaq = m_clrKosdaq;
	
	m_pTicker->m_bMKClrApply = m_bMKClrApply;
	m_pTicker->m_clrKospi  = m_clrKospi;
	m_pTicker->m_clrKosdaq = m_clrKosdaq;
}

void CGridWnd::SetDispCode(BOOL bDispCode)
{
	int nWidth = 0;
	m_bDispCode = bDispCode;
	
	if (m_bDispCode) nWidth = m_nCodeWidth;

	m_grid->SetColumnWidth(colCODE, nWidth);
	m_grid->ResetScrollBars();
	m_grid->Invalidate();

	m_pTicker->SetColumnWidth(colCODE, nWidth);
	m_pTicker->ResetScrollBars();
	m_pTicker->Invalidate();

	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);	
}

void CGridWnd::MultiChart()
{
	CString strData(_T(""));
	strData.Format("%s /S/t0/d%s\t", mchartMAP, MCSYMBOL);

	int nCount = m_grid->GetRowCount();

	for (int i = m_grid->GetFixedRowCount(); i < nCount; i++)
	{
		if (!m_grid->IsSelectedRow(i))
			continue;

		strData += m_grid->GetItemText(i, colCODE);
	}

	openView(typeVIEW, strData);
}

void CGridWnd::ShowRTSWnd()
{
	m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RTSVISIBLE, ((CMainWnd*)m_pMainWnd)->GetRTSVisible() ? MO_HIDE : MO_SHOW));
	((CMainWnd*)m_pMainWnd)->SetRTSTreeData(this);
}

BOOL CGridWnd::IsValidCode(CString strCode)
{
	BOOL bFind = FALSE;
	if (m_grid)
	{
		for (int i = 1; i < m_grid->GetRowCount(); i++)
		{
			CString strGridCode(m_grid->GetItemText(i, 0));

			if (strGridCode.GetLength() == 7)
				strGridCode.Delete(0);

			if (strGridCode == strCode)
			{
				bFind = TRUE;
				break;
			}
		}
	}

	return bFind;
}



void CGridWnd::OnSizing(UINT fwSide, LPRECT pRect) 
{
	CBaseWnd::OnSizing(fwSide, pRect);
	
	// TODO: Add your message handler code here
	
		OperResize(pRect->right - pRect->left, pRect->bottom - pRect->top);
}

int	CGridWnd::GetGroupCount(int gno)
{
	int result = 0;
	struct _inters* pInters;
	int nInterCnt = m_inters.GetSize();
	CString tempStr;
	int pos = -1;
	CString	filePath, fileBook; 
	bool	endfile = false;

	filePath.Format("%s/%s/%s/%s%02d", m_root, USRDIR, m_user, FILE_SUBDATA, gno);

	if (!ExistFile(gno))
	{
		return 0;
	}

	CFile	fileH;
	UINT	readL;

	fileH.Open(filePath, CFile::modeRead);

	for (int ii = 0; ii < MAX_LINE; ii++)
	{
		pInters = (_inters *) new char[sz_inters];
		ZeroMemory(pInters, sz_inters);

		if (!endfile)
		{
			readL = fileH.Read(pInters, sz_inters);
			
			if (readL < sz_inters)
			{
					endfile = true;
			}
			else
			{
				result++;
			}

		}
	}
	
	fileH.Close();

	return result;
}

int	CGridWnd::GetScrollCtrl()
{
//	return m_grid->GetGridScroll();
	return m_grid->GetHscroll();
}

int	CGridWnd::GetGridWidth()
{	
	int childWidth = m_grid->GetGridWidth();

	return childWidth;
}

int	CGridWnd::GetWidth()
{
	int width;
	CRect rc;
	GetWindowRect(rc);

	width = rc.Width();

	if(width <= 0)
	{
		width = m_nGridWidth;
	}

	return width;
}

CString CGridWnd::GetRecommandInfo(int row)
{ 
	return m_iYDayVol[row-1][4]; 
}