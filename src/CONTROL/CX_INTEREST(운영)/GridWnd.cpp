// GridWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "GridWnd.h"
#include "intgrid.h"
#include "../../h/grid.h"
#include "GroupWnd.h"
#include "MainWnd.h"
#include "UsefulSetupDlg.h"
#include "MoveSetupDlg.h"		// ADD PSH 20070918
#include "AutoSaveSetupDlg.h"		// ADD PSH 20070918
#include "AlertSoundDlg.h"		// ADD PSH 20070918
#include "PopupDlg.h"
#include "CodeDuplicate.h"
#include "InterDomino.h"
#include "ShowMsgWnd.h"			// ADD PSH 20070918
#include "mmsystem.h"
#include <math.h>
#include "resource.h"
#include "axMenu.h"
#include "CX_INTERGRID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridWnd

static const std::unordered_map<int, int>& FwdMap()
{
	static const std::unordered_map<int, int> m = {
		{ 23, 623 }, { 24, 624 }, { 27, 627 }, { 28, 628 },
		{ 29, 629 }, { 30, 630 }, { 31, 631 }, { 32, 632 },
		{ 33, 633 }, { 36, 634 }, { 41, 641 }, { 51, 651 },
		{ 61, 661 }, { 71, 671 }, { 101, 601 }, { 104, 604 },
		{ 106, 606 }, { 109, 609 }, { 146, 646 }, { 181, 681 },
		{ 111, 611 }, { 112, 612 }, { 115, 615 }, { 116, 616 },
	};
	return m;
}

static int ToMapped(int idx)
{
	const auto& m = FwdMap();
	auto it = m.find(idx);
	return (it != m.end()) ? it->second : idx;
}

CGridWnd::CGridWnd(CWnd* pMainWnd, int nIndex) : CBaseWnd(pMainWnd)
{
	_pApp = dynamic_cast<CCX_INTERGRIDApp*>(AfxGetApp());
	SetIndex(nIndex);

	m_grid = nullptr;
	m_kind = 0;
	m_szTitle = _T("");
	m_updateROW = -1;
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
	//m_pTicker	= nullptr;
	m_bTicker	= TRUE;
	m_bfirstStart = 0;

	m_bMargin	= TRUE;
	m_bSorting	= FALSE;
	m_mapMargin.RemoveAll();
	m_clrMarkerBKG	= RGB(240, 240, 240);
	m_clrMarkerTXT	= RGB(142, 142, 142);

	m_bShadow	= TRUE;
	m_bAllApply	= TRUE;
	m_bMoveCfg  = FALSE;
	m_nMoveSave = 0;
	m_pMsgWnd	= nullptr;
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
	m_bDiff		  = FALSE;
	m_bAutoExpect = FALSE;
	m_bPopupDlg	  = FALSE;

	m_bRTS    = FALSE;
	m_bDispCode   = FALSE;

	m_nCodeWidth  = 80;

	m_pRCDlg	  = nullptr;
	m_bMonitor	  = FALSE;
	m_bMonSym	  = FALSE;
	m_bEditWork = FALSE;

	m_bIsFirst = FALSE;

	m_dBase = 0;
	m_ClickRow = 0;

	for(int i=0 ; i< 200 ; i++)
	{
		m_irowCode[i] = 0;
	}

	m_yDayVolField = -1;			//전일거래대비율필드
	m_EqualizerField = -1;

	m_recomBasicCostField = -1;
	m_recomSuikrateField = -1;
	m_recomBMjisuField = -1;
	m_recomBMSuikrateField = -1;
	m_upjongCode = -1;

	m_nGridWidth = 0;				//그리드 넓이
	// END ADD
	for(int cc=0 ; cc<200 ; cc++)
	{
		chg_count[cc] = 0;
	}

	m_dKosdaq = 0;
	m_dKospi = 0;
	m_SendKey = -1;
	m_UpdateGroup = -1;

	m_iFieldType = 0;
	m_PrevGroupIndex = -1;

	m_selectRow = -1;
	m_keyMode = -1;

	newsFms.RemoveAll();

	m_bContinue = FALSE;
	m_bSecond = FALSE;

	m_bSingle = FALSE;
	m_bSort = FALSE;

	m_pGridData = nullptr;

	m_nGridNumber = 0;
	m_nSelectedRow = -1;
	m_nScrollPos = -1;

	m_pGridMarker.RemoveAll();

	m_bSaveInterestForOub = TRUE;
	m_nID = -1;

	m_recomArray.RemoveAll();
	m_recomCodeToData.RemoveAll();

	m_pSearchMap.RemoveAll();
	m_mapCurValue.RemoveAll();

	m_strBeginTime = "081000";
	m_strEndTime = "085959";
	_sClass = "grid";
}

CGridWnd::~CGridWnd()
{
	if (nullptr != m_pRCDlg)
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
	const CBitmap*	pBitmap = (CBitmap*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETBITMAP, (LPARAM)(char*)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

void CGridWnd::OperInit()
{
	m_pView     = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	m_pGroupWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	m_pToolWnd  = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	m_pTreeWnd  = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	//HBITMAP	hBITMAP, hBITMAP_DN, hBITMAP_HV;

	// 트리 펼치기 버튼
	CString	fileIMG;
	fileIMG.Format("%s\\%s\\", m_root, IMAGEDIR);

	LOGFONT	lf;
	m_pFont->GetLogFont(&lf);
	struct _fontR fontR;
	fontR.bold = FW_NORMAL;
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

	m_grid = std::make_unique<CintGrid>(m_pMainWnd, this, &lf);
	m_grid->Create(CRect(0, 0, 0, 0), this, IDC_GRID, GVSC_BOTH, GVDD_FULL);

	// ADD PSH 20070911
	m_grid->SetBmpIcon(fileIMG + "MARKER.bmp", 0);
	m_grid->SetBmpIcon(fileIMG + "관.bmp", 1);
	m_grid->SetBmpIcon(fileIMG + "투자유의.bmp", 2);
	m_grid->SetBmpIcon(fileIMG + "주.bmp", 3);
	m_grid->SetBmpIcon(fileIMG + "경.bmp", 4);
	m_grid->SetBmpIcon(fileIMG + "위2.bmp", 5);
	m_grid->SetBmpIcon(fileIMG + "추천종목.BMP", 6);

	m_grid->SetBmpIcon(fileIMG + "환.BMP", 7);
	m_grid->SetBmpIcon(fileIMG + "이상급등.BMP", 8);
	m_grid->SetBmpIcon(fileIMG + "불.BMP", 9);
	m_grid->SetBmpIcon(fileIMG + "정2.BMP", 10);	//정지
	m_grid->SetBmpIcon(fileIMG + "정1.BMP",11);		//정리
	m_grid->SetBmpIcon(fileIMG + "배.BMP", 12);	//배분
	m_grid->SetBmpIcon(fileIMG + "임.BMP",13);		//임의
	m_grid->SetBmpIcon(fileIMG + "과.BMP",14);	//2012.10.31 KSJ 단기과열 추가
	m_grid->SetBmpIcon(fileIMG + "위1.BMP",15);	//2013.03.18 KSJ 위험예정
	m_grid->SetBmpIcon(fileIMG + "권.BMP",16);	//2013.03.18 KSJ 권리
	m_grid->SetBmpIcon(fileIMG + "배1.BMP",17);	//2013.03.18 KSJ 배당
	m_grid->SetBmpIcon(fileIMG + "액.BMP",18);	//2013.03.18 KSJ 액면
	m_grid->SetBmpIcon(fileIMG + "병.BMP",19);	//2013.03.18 KSJ 병합
	m_grid->SetBmpIcon(fileIMG + "합.BMP",20);	//2013.03.18 KSJ 합병
	m_grid->SetBmpIcon(fileIMG + "감1.BMP",21);	//2013.03.18 KSJ 감자
	m_grid->SetBmpIcon(fileIMG + "감.BMP",22);	//2013.03.18 KSJ 감리
	m_grid->SetBmpIcon(fileIMG + "유.BMP",23);	//2016.06.14 KSJ 초저유동성

	m_bMoveCfg  = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVECFG);
	m_nMoveSave = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVESAVE);

	m_grid->SetTitleTips(TRUE);

	RemoveAll();
	FieldSetup(false);
}

//////////////////////////////////////////////////////////////////////////

CString CGridWnd::CalMaketTime(CString strTime, bool bEnd)
{
	CString strData;

	strTime.Replace("시", "");
	strTime.Replace("분", "");

	CString strTemp;

	if(bEnd)
	{
		if(strTime.IsEmpty())
			strTime = "90";

		int nTime = atoi(strTime) -1;

		if(nTime%10 == 9)
			nTime -= 4;

		strData.Format("%03d959", nTime);	//085959, 092959, 095959
	}
	else
	{
		if(strTime.IsEmpty())
			strTime = "80";

		strData.Format("%04d00", atoi(strTime));	//081000, 084000, 091000
	}

	return strData;
}


void CGridWnd::FieldSetup(bool bDlgSetup, int iEqualizer)
{
	//int	nIndex = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_GETAFIELD);
	m_bongField = -1;
	m_sonikField = -1;
	m_suikField = -1;
	m_ccField = false;
	m_posField = false;
	m_grid->Clear();
	loadField(0, bDlgSetup, iEqualizer);

	initialGrid();

	// ADD PSH 20070918
	MarkerSetup();
	initRSymbol();
//	m_grid->MakeEqualizerBmp();
	// END ADD
}

void CGridWnd::OperDestory()
{
	savecfg();
	m_btDomino.DestroyWindow();
	m_btSAVE.DestroyWindow();
	m_btCLOSE.DestroyWindow();
	ClearInterest();

	if (m_grid)
		m_grid->DestroyWindow();
// 	m_pTicker->DestroyWindow();
// 	delete m_pTicker;
	KillTimer(1000);
}

void CGridWnd::DrawTitle(CDC* pDC)
{
	const int	nCurSel = m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_GET));
	const BOOL	bFocus = (nCurSel == m_nIndex) ? TRUE : FALSE;
	const int	nmode = pDC->SetBkMode(TRANSPARENT);
	IH::DrawBox(pDC, m_rcTitle, RGB(200, 200, 200), BOX_ALL, (bFocus) ? m_clrFOCUS[0] : m_clrFOCUS[1], TRUE);
	const COLORREF clr = pDC->SetTextColor((bFocus) ? m_clrTEXT[0] : m_clrTEXT[1]);
	CFont*	oldfont = nullptr;
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
//	DrawTitle(pDC);

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
	CRect	rect;
	rect = m_rcTitle = m_rect;

	m_grid->MoveWindow(rect, TRUE);
	m_grid->Adjust();

	if (IsInit())
	{
		m_grid->ShowWindow(SW_SHOW);
	}

	//뉴스컬럼 숨기기
	//m_grid->SetColumnWidth(colSIG, 0);

	InvalidateRect(rect);
}

void CGridWnd::InsertRowNCode(int row)
{
	//2011.11.09 KSJ
	//row가 0이면 (티커)임..
	if(row <= 0)
		return;

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	int rowcount = 0;

	if(nOver == MO_VISIBLE)
	{
		pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
		rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
	}
	else
	{
		rowcount = GetRowcountVisibleMode();
	}

	if(rowcount >= MAX_LINE)
	{
		Variant(guideCC, IH::idTOstring(IDS_GUIDE4));

		return;
	}

	insertInterest(row - 1);
	insertRow(row);
	SetLineColor();
 	m_grid->getmemoCheck();
 	m_grid->memoRefresh();
	m_grid->SetFocusCellEdit(row, colNAME, true);

	if (m_bAddCnd)
		saveInterest();
}

LONG CGridWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int retArrange = pWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);

// 	CString strTemp;

	switch (LOWORD(wParam))
	{
	case MK_SISECATCH:
		OperAlarm(HIWORD(wParam), (char*)lParam);
		break;
	case MK_MEMO:
		m_grid->getmemoCheck();
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
// 	case MK_TURNCODE:
// 		{
// 			int	nsec = HIWORD(wParam);
// 			if (nsec == 0)
// 			{
// 				KillTimer(1000);
// 				m_nTurn = 0;
// 			}
// 			else
// 			{
// 				if (CAST_TREEID(m_kind)->kind == xISSUE)
// 					break;
//
// 				m_nTurn = 0;
// 				SetTimer(1000, nsec * 1000, nullptr);
// 			}
// 		}
// 		break;
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
	case MK_RECVDATA2:
		RecvOper2(HIWORD(wParam), (CRecvData*)lParam);
		break;
	//case MK_RTSDATA:
	//	RecvRTS((char*)lParam);
	//	break;
	case MK_ENDDRAG:
		m_drag = m_drop = -1;
		m_grid->FreeDragDrop();
		CintGrid::m_dropdata.Reset();
		break;
	case MK_INSERTROW:
		ret = insertRow(HIWORD(wParam));
		break;
	case MK_INSERTCODE:
		{
			const CWnd* ptoolWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int   nOver = (int)ptoolWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
			int rowcount = 0;

			m_selectRow = HIWORD(wParam);		//선택한 열을 저장
			m_keyMode = 1;
			//				SetTimer(2000, 500, nullptr);

			if(nOver == MO_VISIBLE)
			{
				rowcount = GetRowcountVisibleMode();
			}
			else
			{
				rowcount = m_rowC - 1;
			}

			if(rowcount >= MAX_LINE)
			{
				Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
				break;
			}


			insertInterest(HIWORD(wParam) - 1);
			insertRow(HIWORD(wParam));
			SetLineColor();
			m_grid->getmemoCheck();
			m_grid->memoRefresh();

			m_grid->SetFocusCellEdit(HIWORD(wParam), colNAME, true);

// 			if(nOver == MO_VISIBLE)
// 			{
// 				m_tempCode = m_grid->GetItemText(HIWORD(wParam), colCODE);
// 				m_grid->SetFocusCellEdit(HIWORD(wParam), colNAME, true);
// 			}
// 			else
// 			{
// 				insertInterest(HIWORD(wParam) - 1);
// 				insertRow(HIWORD(wParam));
// 				SetLineColor();
// 				m_grid->memoCheck();
// 				m_grid->memoRefresh();
//
// 				m_grid->SetFocusCellEdit(HIWORD(wParam), colNAME, true);
// 			}

			saveInterest(true);
		}
// 		if(retArrange == 0)
// 		{
//
// 			if (CAST_TREEID(m_kind)->kind == xINTEREST)
// 				m_bEditWork = true;
//
// 			InsertRowNCode(HIWORD(wParam));
// 		}
		break;
	case MK_DELETEROW:
		{
			if(((CMainWnd*)m_pMainWnd)->m_bRemain == TRUE)
			{
				break;
			}

			const int nRow = HIWORD(wParam);

			const CWnd*	ptoolWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int	nOver = (int)ptoolWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
			//int pageNumber = pWnd->SendMessage(WM_MANAGE, MK_GETVIEWPAGE);
			m_pGroupWnd->SendMessage(WM_MANAGE,MK_EDITWORK,TRUE);

			m_selectRow = nRow;		//선택한 열을 저장
			//m_selectRow = m_selectRow - 1;

			if (gsl::narrow_cast<int>(m_inters.size()) >= nRow -1)
			{
				if (!(m_grid->GetItemAttr(nRow, colNAME) & GVAT_MARKER))
				{
					DeleteRow(nRow);

					m_nSelectedRow = m_selectRow;
 					m_nScrollPos = m_grid->GetScrollPos32(SB_VERT);

					insertInterest(MAX_LINE - 1);
					saveInterest(true);
					if(nOver == MO_VISIBLE)
					{
						//pWnd->SendMessage(WM_MANAGE, MK_SETPAGEVIEW, pageNumber);
						pWnd->SendMessage(WM_MANAGE, MK_RELOADLIST);
					}
					else
					{
						m_grid->SetSelectRow(m_nSelectedRow);
					}

					SetLineColor();
				}

			}
		}
		break;
 	case MK_TRIGGEROW:
 		triggerCode(HIWORD(wParam));
 		break;
	case MK_SAVE:
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
				m_nID = (int)lParam;

				saveInterest();
			}
		}
		// END MODIFY
		break;
	case MK_HAWKEYE:
		{
			const LONG value = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
			hawkeyeAction((value) ? true : false);
//			Invalidate(FALSE);
		}
		break;
	// ADD PSH 20070911
	case MK_HSCROLL:
		{
			//m_pTicker->SetHScroll(HIWORD(wParam), lParam);
		}
		break;
	case MK_SORTBASE:
		{
			BaseSorting();
		}
		break;
	case MK_GROUPSAVE2:	//화면 뷰모드 변경시 저장
		{
			saveInterestInverse();
		}
		break;
	// END ADD
	case MK_CHANGEVIEWTYPE:
		{
			if(((int)lParam) == SMALLTYPE_FILE)
			{
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_SMALLCONFIG);
			}
			else
			{
				m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_LARGECONFIG);
			}

			/*loadcfg();*/

			break;
		}
		break;
	case MK_SAVEFOROUB:
		{
			m_bSaveInterestForOub = (BOOL)lParam;
		}
		break;
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

	code.TrimLeft();
	code.TrimRight();

	if (code.IsEmpty())
		return;

//	m_sync.Lock();

	if (((CGroupWnd*)m_pGroupWnd)->GetSelAction() == 0)
		return;

	switch (GetCodeType(code))
	{
	case kospiCODE:
		string.Format("%s\t%s", HCOD, code);
		break;
	case futureCODE:
		string.Format("%s\t%s", "ed_focod", code);	//2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
		Variant(triggerCC, string);

		string.Format("%s\t%s", FCOD, code);
		Variant(triggerCC, string);
		break;
	case optionCODE:
		string.Format("%s\t%s", "ed_focod", code);	//2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
		Variant(triggerCC, string);

		string.Format("%s\t%s", OCOD, code);
		Variant(triggerCC, string);
		break;
	case foptionCODE:
		string.Format("%s\t%s", "ed_focod", code);	//2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
		Variant(triggerCC, string);

		string.Format("%s\t%s", PCOD, code);
		Variant(triggerCC, string);
		break;
	case indexCODE:
		string.Format("%s\t%s", UCOD, code);
		break;
	case sinjuCODE:
		string.Format("%s\t%s", SINJUCOD, code);
		break;
	case elwCODE:
		string.Format("%s\t%s", HCOD, code);
		activeTrigger(code);
		//Variant(triggerCC, string);
		//					m_grid->SetFocus();
		string.Format("%s\t%s", ELWCOD, code);
		break;
	case thirdCODE:
		string.Format("%s\t%s", THIRDCOD, code);
		break;
	case 0:		//2012.10.11 KSJ 주식, 금리, 통화선물 추가
		if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물  //파생상품 코드개편
		{
			int nGubn = atoi(code.Mid(1,2));
			CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ 주식선물 신규종목 연동

			if(nGubn > 10 && nGubn < 60)	//주식선물
			{
				string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
				Variant(triggerCC, string);

				string.Format("%s\t%s", SFOD, code);
				break;
			}
			else if(nGubn > 60 && nGubn <= 70)	//금리선물
			{
				string.Format("%s\t%s", SFOD, code);
				break;
			}
			else if(nGubn > 70 && nGubn < 80)	//통화선물
			{
				string.Format("%s\t%s", SFOD, code);
				break;
			}
			else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
				|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
				|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z"))
			{
				string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
				Variant(triggerCC, string);

				// m_slog.Format("[cx_interest][SF][%s]<%d> code=[%s] string=[%s]", __FUNCTION__, __LINE__, code, string);
				// OutputDebugString(m_slog);

				string.Format("%s\t%s", SFOD, code);
				break;
			}
		}
	}

	activeTrigger(code);
// 	Variant(triggerCC, string);
	Variant(codeCC, string);

//	m_sync.Unlock();

	//m_grid->SetFocus();
}

void CGridWnd::activeTrigger(CString data)
{
	const int key = ((CMainWnd*)m_pMainWnd)->GetParamKey();
	CString name = ((CMainWnd*)m_pMainWnd)->GetParamName();
	CString string, string2;

	//2013.02.19 KSJ 종합화면에서 자기맵에서 온 Settrigger함수 인지 체크하기위하여 핸들을 포함하여 보냄.
	CString send;
	send.Format("%s%c%s,%s", "Settrigger", P_TAB, data, ((CMainWnd*)m_pMainWnd)->m_sMapHandle);

	m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)send);
	m_grid->SetFocus();
}


void CGridWnd::RecvOper(int kind, CRecvData* rdata)
{
	struct _extTHx* ex = (struct _extTHx*)rdata->m_lParam;
	m_grid->setReal(false);
	if(m_SendKey == TRKEY_GRIDNEW)
	{
		parsingOubs(ex->data, ex->size);
		ClearSearchMap();	//2011.12.29 KSJ
		ReSetSearchMap();	//2011.12.29 KSJ
	}
	else if(m_SendKey == TRKEY_GRIDROW)
	{
		parsingOubsOne(ex->data, ex->size, m_updateROW);
		m_updateROW = -1;
		ClearSearchMap();	//2011.12.29 KSJ
		ReSetSearchMap();	//2011.12.29 KSJ
	}
	else if(m_SendKey == TRKEY_MARKETTIME)
	{
	}


	if(m_nID != -1 && m_nID < 101)
	{
// 		CString send;
// 		send.Format("%s%c%d", "CallClickTuja", P_TAB, m_nID);
//
// 		m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)send);
//		m_pMainWnd->PostMessage(WM_CLOSE,0,0);
		m_nID = -1;
	}
	else
	{
		const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

		if(nOver == MO_SELECT)
		{
			if(m_selectRow != -1)
			{
				m_grid->SetSelectRow(m_selectRow);
				m_selectRow = -1;
 			}
		}
	}
	m_grid->setReal(true);
}

void CGridWnd::RecvOper2(int kind, CRecvData* rdata)
{
	struct _extTHx* ex = (struct _extTHx*)rdata->m_lParam;

	if (ex->size <= 0)
		return;
	m_grid->setReal(false);
	if(m_SendKey == TRKEY_GRIDNEW)
	{
		parsingOubs(ex->data, ex->size, kind);
		ClearSearchMap();	//2011.12.29 KSJ
		ReSetSearchMap();	//2011.12.29 KSJ
	}
	else if(m_SendKey == TRKEY_GRIDROW)
	{
		parsingOubsOne(ex->data, ex->size, kind, m_updateROW);
		m_updateROW = -1;
		ClearSearchMap();	//2011.12.29 KSJ
		ReSetSearchMap();	//2011.12.29 KSJ
	}
	else if(m_SendKey == TRKEY_MARKETTIME)
	{
	}

	if(m_nID != -1 && m_nID < 101)
	{
		m_nID = -1;
	}
	else
	{
		const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

		if(nOver == MO_VISIBLE)
		{
			{
				if(m_selectRow != -1)
				{
					m_grid->SetSelectRow(m_selectRow);
					m_selectRow = -1;
 				}
			}
		}
	}
	m_grid->setReal(true);
}

void CGridWnd::AddData_Overoper(int nIndex, CString sztmp)
{
//	struct _inters* pinters{};
	BOOL	bInt = FALSE;
	CString	code, amount, price, bookmark;

	// ADD PSH 20070913
	CString strName;

	if (0 == nIndex)
	{
		const UINT attr = m_grid->GetItemAttr(nIndex+1, colNAME) & ~GVAT_MARKER;
		m_grid->SetItemAttr(nIndex+1, colNAME, attr);
	}
	else
	{
		insertRow(nIndex+1);
	}
	// END ADD

	code = IH::Parser(sztmp, PTAB);

	code.TrimLeft(); code.TrimRight();

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		bInt = TRUE;
		amount = IH::Parser(sztmp, PTAB);
		price = IH::Parser(sztmp, PTAB);
		// ADD PSH 20070914
		strName = IH::Parser(sztmp, PTAB);
		bookmark = IH::Parser(sztmp, PTAB);
		// END ADD
	}

	if (!bInt && code.IsEmpty())
		return;

	auto& pinters = m_inters.at(nIndex);
	pinters.get()->gubn = '0';
	pinters.get()->code = code;

	if (bInt)
	{
		pinters.get()->xnum = amount;
		pinters.get()->xprc = price;
		pinters.get()->bookmark = bookmark[0];
	}

	// ADD PSH 20070914
	if (!code.IsEmpty() && code.GetAt(0) == 'm')
	{
		pinters.get()->gubn = ROW_COMMENT;
		pinters.get()->name = strName;

		const UINT attr = m_grid->GetItemAttr(nIndex+1, colNAME) | GVAT_MARKER;
 		m_grid->SetItemAttr(nIndex+1, colNAME, attr);
 		m_grid->SetItemText(nIndex+1, colNAME, strName);

		return;
	}
	// END ADD
}

void CGridWnd::AddData(int nIndex, CString sztmp)
{
	BOOL	bInt = FALSE;
	vector<CString> vitem;
	const enum { eamount, eprice, ename, ebookmark, efutureGubn, ecreditPrc, emaeipPrc, ebookrgb, ememoYn };

	CString strName;
	m_grid->setReal(false);
	if (0 == nIndex)
	{
		const UINT attr = m_grid->GetItemAttr(nIndex , colNAME & ~GVAT_MARKER);
		m_grid->SetItemAttr(nIndex, colNAME, attr);
	}
	else
	{
		insertRow(nIndex);
	}
	// END ADD

	CString code = IH::Parser(sztmp, PTAB);
	code.Trim();
	
	AxStd::SplitSub(vitem, sztmp, PTAB);
	if (vitem.size() > 8)
		bInt = TRUE;		

	const int count = m_inters.size();
	if (!bInt && code.IsEmpty())	return;

	if (count <= nIndex)
		return;
	
	auto& pinters = m_inters.at(nIndex);
	pinters.get()->gubn = '0';
	pinters.get()->code = code;

	if(bInt && vitem.size() > 8 && CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		pinters.get()->xnum       = vitem[eamount];
		pinters.get()->xprc       = vitem[eprice];
		pinters.get()->name       = vitem[ename];
		pinters.get()->bookmark   = vitem[ebookmark][0];
		pinters.get()->futureGubn = vitem[efutureGubn][0];
		pinters.get()->creditPrc  = atof(vitem[ecreditPrc]);
		pinters.get()->maeipPrc   = atof(vitem[emaeipPrc]);
		pinters.get()->bookrgb    = vitem[ebookrgb];
		pinters.get()->bMemo      = vitem[ememoYn];

		pinters.get()->bookmark = vitem[ebookrgb].CompareNoCase("00") ? '1':'0';
		if (vitem[ebookrgb].IsEmpty())
			pinters.get()->bookmark = '0';

		if (vitem[ebookmark] == "1")
			pinters.get()->gubn = ROW_BOOKMARK;
	}

	if (!code.IsEmpty() && code.GetAt(0) == 'm')
	{
		pinters.get()->gubn = ROW_COMMENT;
		pinters.get()->name = strName;

		const UINT attr = m_grid->GetItemAttr(nIndex + 1, colNAME);
		m_grid->SetItemAttr(nIndex +1, colNAME, attr|GVAT_MARKER);
		m_grid->SetItemText(nIndex +1, colNAME, strName);

		return;
	}
	else
	{
		m_grid->SetItemAttr(nIndex + 1, colNAME, m_grid->GetItemAttr(nIndex+1, colNAME)&~GVAT_MARKER);  //test 20230209  확인
	}
	m_grid->setReal(true);
}

void CGridWnd::RemoveAll()
{
	ClearInterest();	

	for (int ii = 0; ii < MAX_LINE; ii++)
	{
		auto& pinters = m_inters.emplace_back(std::make_shared<_intersx>());
		pinters.get()->empty();
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
// 		m_pTicker->SetItemText(0, i, "");
// 		m_pTicker->SetItemData(0, i, 0);
	}

	m_rowC = 2;		//BUFFET 2->1
	// END ADD
	//2012.03.22 KSJ 같은그룹을 다른 화면에서 띄울때 첫번째 줄에서 실시간 데이터를 받았음.
	ClearSearchMap();	//2011.12.29 KSJ
	ReSetSearchMap();	//2011.12.29 KSJ
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

	const int ncnt = sdata->GetCount();
	BOOL	bInt = FALSE;

	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		bInt = TRUE;

	int ii = 0;
	for ( ii = 0 ; ii < ncnt ; ii++ )
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, PTAB);

		code.TrimLeft(); code.TrimRight();

		if (!bInt && code.IsEmpty())
			continue;

		auto& pinters = m_inters.emplace_back(std::make_shared<_intersx>());
		pinters.get()->gubn = '0';
		pinters.get()->code = code;
		if (pinters.get()->code[0] == 'm')
			pinters.get()->gubn = ROW_COMMENT;

		if (bInt || !sztmp.IsEmpty())
		{
			amount = IH::Parser(sztmp, PTAB);
			price = IH::Parser(sztmp, PTAB);
			name = IH::Parser(sztmp, PTAB);
			strBookmark = IH::Parser(sztmp, PTAB);

			// END ADD
			pinters.get()->xnum = amount;
			pinters.get()->xprc = price;
			pinters.get()->name = name;

			//북마크 추가
			pinters.get()->bookmark = strBookmark[0];
			// END ADD
		}

		if (ii != 0)
		{
			insertRow(ii, FALSE);
		}
	}

	for (; ii < MAX_LINE ; ii++ )
	{
		auto pinters = m_inters.emplace_back(std::make_shared<_intersx>());
		pinters.get()->empty();
	}

	// ADD PSH 20070918
	MarkerSetup();
	// END ADD
}

//void CGridWnd::parseReCommandData(char* datB, int datL)
//{
//	struct  mod
//	{
//		char    nrec[4];						/*	갯수			*/
//		char    acod[1024][12];					/*  종목코드 리스트 */
//	};
//
//	#define	sz_mod	sizeof(struct mod)
//
//	struct mod* mod = nullptr;
//
//	mod = (struct mod*)new char[sz_mod];
//	ZeroMemory(mod, sz_mod);
//
//	class CGridData sdata;
//
//	mod = (struct mod*)datB;
//
//	CString strCnt = CString(mod->nrec, 4);
//	const int nCnt = atoi(strCnt);
//
//	CString code, name;
//
//	for(int i=0 ; i<nCnt ; i++)
//	{
//		code = CString(mod->acod[i], 12);
//		code.TrimLeft();
//		code.TrimRight();
//
//		if (!code.IsEmpty())
//		{
//			sdata.m_arDatas.Add(code);
//		}
//	}
//
//	ParseData(&sdata);
//	sendTransaction();
//}



void CGridWnd::ParseUpjongData(class CGridData* sdata)
{
	RemoveAll();

	CString	sztmp = _T("");
	CString	code;

	int	sendL = 0;
	char	tempB[64]{};
	const int	bufSize = sizeof(struct _gridHi) + 50 + m_gridHdrX.GetSize()*5 + m_inters.size()*12 + 12;
	std::string sendB;
	sendB.resize(bufSize, ' ');


	const BOOL	bExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
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
	sprintf(tempB, "%04d", m_inters.size());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir  = '1';
	gridHi->sort = '0';

	//그리드 심볼
	sprintf(tempB, "2023%c", P_NEW);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "1910%c", P_NEW);					//추천일자
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "1911%c", P_NEW);					//추천종목 기준가
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);


	sprintf(tempB, "1915%c", P_NEW);					//지수 구분
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", CCOD, P_DELI);				// 종목코드
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	const int	ncnt = sdata->GetCount();

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, PTAB);

		code.TrimLeft(); code.TrimRight();

		if (code.IsEmpty())
			continue;

		sprintf(tempB, "%s%c", strlen(code) <= 0 ? " " : code, P_DELI);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sendB[sendL] = P_NEW;
	sendL += 1;
	sendB[sendL] = P_TAB;
	sendL += 1;
	sendB[sendL] = 0x00;
	CSendData	sendata;
	char	key{};
	_trkey* trkey = (struct _trkey*)&key;

	trkey->group = m_nIndex;
	trkey->kind = TRKEY_GRIDUPJONG;

	m_updateROW = -1;

	sendata.SetData("pooppoop", key, sendB.data(), sendL, "");

	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sendata);
	m_endsort = false;

}


void CGridWnd::ParseData(class CGridData* sdata)
{
	RemoveAll();

	SetKind(sdata->GetKind());
	InvalidateRect(m_rcTitle, FALSE);
	ClearInterest();

	CString	sztmp = _T("");
	CString	code, amount, price, name, strBookmark, futureGubn, creditPrc, maeipPrc, bookrgb, MemoYN;
	char bookmark[1];
	bookmark[0] = '0';

	const int	ncnt = sdata->GetCount();
	BOOL	bInt = FALSE;

	const UINT nKind = CAST_TREEID(m_kind)->kind;
	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		bInt = FALSE;
	else
		bInt = TRUE;

	int ii = 0;
	for ( ii = 0 ; ii < ncnt ; ii++ )
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, PTAB);

		code.TrimLeft(); code.TrimRight();

		if (!bInt && code.IsEmpty())
			continue;

		auto& pinters = m_inters.emplace_back(std::make_shared<_intersx>());
		pinters.get()->gubn = '0';
		pinters.get()->code = code;

		if (pinters.get()->code[0] == 'm')
			pinters.get()->gubn = ROW_COMMENT;

		if (bInt || !sztmp.IsEmpty())
		{
			amount = IH::Parser(sztmp, PTAB);
			price = IH::Parser(sztmp, PTAB);
			name = IH::Parser(sztmp, PTAB);
			strBookmark = IH::Parser(sztmp, PTAB);
			futureGubn = IH::Parser(sztmp, PTAB);
			creditPrc = IH::Parser(sztmp, PTAB);
			maeipPrc = IH::Parser(sztmp, PTAB);
			bookrgb = IH::Parser(sztmp, PTAB);
			MemoYN = IH::Parser(sztmp, PTAB);

			// END ADD
			pinters.get()->xnum = amount;
			pinters.get()->xprc = price;
			pinters.get()->name = name;
			pinters.get()->futureGubn = futureGubn[0];
			pinters.get()->creditPrc = atof(creditPrc);
 			pinters.get()->maeipPrc = atof(maeipPrc);
			pinters.get()->bookrgb = bookrgb;
			pinters.get()->bMemo = MemoYN;

			strBookmark = bookrgb.CompareNoCase("00") ? "1":"0";
			if (bookrgb.IsEmpty())
				strBookmark = "0";

			pinters.get()->bookmark = strBookmark[0];
			if(strBookmark == "1")
				pinters.get()->gubn = ROW_BOOKMARK;
			// END ADD
		}

		if (ii != 0)
		{
			insertRow(ii, FALSE);
		}
	}

	for ( ; ii < MAX_LINE ; ii++ )
	{
		auto& pinters = m_inters.emplace_back(std::make_shared<_intersx>());
		pinters.get()->empty();
	}

	//m_arrBaseInters.RemoveAll();
	//rebuildInterest();	//2013.05.21 KSJ 여기서 m_inter와 baseinter를 같게 만들어준다.
	//// ADD PSH 20070918
	//MarkerSetup();

	//Invalidate();
	// END ADD
}

//2011.12.20 KSJ 컨트롤을 종료하지 않고 GridWnd의 m_bSorting을 false로 만들어 준다.
void CGridWnd::SetInitSortingValue()
{
	m_bSorting = FALSE;
}
// KSJ

int  CGridWnd::loadcfg_data()
{
	const int result = GetPrivateProfileInt(m_section, KEY_DATA, 0, m_fileCFG);

	return result;
}

void CGridWnd::savecfg_data(CString keydata)
{
	WritePrivateProfileString(m_section, KEY_DATA, keydata.operator LPCTSTR(), m_fileCFG);
}

void CGridWnd::loadcfg()
{
	const int confirm_start = GetPrivateProfileInt(m_section, KEY_DATA, 9999, m_fileCFG);

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
// 	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
// 	{
// 		m_kind = GetPrivateProfileInt(m_section, KEY_DATA, 0, m_fileCFG);
// 	}
// 	else
// 	{
// 		m_kind = GetPrivateProfileInt(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_DATA, 0, m_fileCFG);
// 	}


	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		m_kind = 0;

	char	buf[1024];

	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(m_section, KEY_FIELD, "", buf, sizeof(buf), m_fileCFG);
	m_szFIELD.Format("%s", buf);

	memset(buf, 0x00, sizeof(buf));

	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
	{
		GetPrivateProfileString(m_section, KEY_WIDTH, "", buf, sizeof(buf), m_fileCFG);
	}
	else
	{
		GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH, "", buf, sizeof(buf), m_fileCFG);
	}

	m_szWIDTH.Format("%s",buf);
	m_szWIDTH.TrimLeft();

	//2013.07.12 KSJ ,0,0,0,0이 들어가있으면 지우고 저장한다.
	//7852~7855까지 추가한걸 잘못 반영했다가 생긴 어쩔수 없는 코딩
	if(m_szWIDTH.Find(",0,0,0,0"))
	{
		m_szWIDTH.Replace(",0,0,0,0", "");
		WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH, m_szWIDTH, m_fileCFG);
	}
	// KSJ

	CString tmpCfg;
	tmpCfg.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_SMALLCONFIG);

	memset(buf,0x00, sizeof(buf));

	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
	{
		GetPrivateProfileString(m_section, KEY_WIDTH2, "", buf, sizeof(buf), tmpCfg);
	}
	else
	{
		GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH2, "", buf, sizeof(buf), tmpCfg);
	}

	m_MultiViewCols.Format("%s",buf);
	m_MultiViewCols.TrimLeft();

	//2013.07.12 KSJ ,0,0,0,0이 들어가있으면 지우고 저장한다.
	//7852~7855까지 추가한걸 잘못 반영했다가 생긴 어쩔수 없는 코딩
	if(m_MultiViewCols.Find(",0,0,0,0"))
	{
		m_MultiViewCols.Replace(",0,0,0,0", "");
		if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
			WritePrivateProfileString(m_section, KEY_WIDTH2, m_MultiViewCols, tmpCfg);
		else
			WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH2, m_MultiViewCols, tmpCfg);
	}
	// KSJ

	if(m_szWIDTH == "")
	{
		if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
		{
			int nColWidth = ((CMainWnd*)m_pMainWnd)->m_param.rect.Width() - 30;
			int nCols = 0;

	// 		if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
	// 			m_szWIDTH = "69,57,60,42,63,0";
	// 		else
	// 			m_szWIDTH = "110,67,61,63,77,59,62,61,60,0";
			if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
			{
	// 			nColWidth -= 7;
	// 			nCols = 5;
				nColWidth -= 63;
				nCols = 4;
			}
			else
			{
				nColWidth -= 6;
				nCols = 9;
			}

			if(((CMainWnd*)m_pMainWnd)->m_param.options == "WIDE")
			{
				m_MultiViewCols = _T("");

				const int nMultiWidth = ((((CMainWnd*)m_pMainWnd)->m_param.rect.Width() - 38)/ 2 - 20) / 5;
				const int nRemMulti = ((nColWidth + 6) / 2) % nCols;

				for(int i=0;i<5;i++)
				{
					CString strM;

					if(i==0)
						strM.Format("%d",nMultiWidth+nRemMulti);
					else if(i == 3)
						strM.Format("%d",nMultiWidth-10);
					else if(i==4)
						strM.Format("%d",nMultiWidth+10);
					else
						strM.Format("%d",nMultiWidth);


					m_MultiViewCols += strM + ",";
				}

				m_MultiViewCols += "0";
			}

			const int nWidth = nColWidth / nCols;
			const int nRem = nColWidth % nCols;

			CString strWidth;

			if(nCols == 4)
			{
				nCols += 1;
			}

			for(int i=0;i<nCols;i++)
			{
				CString str;

				if(nCols == 5 && i==0)
				{
					str.Format("%d",56);
				}
				else if(nCols == 5 && i==4)
				{
					str.Format("%d",nWidth+nRem + 10);
				}
				else if(nCols == 5 && i==3)
				{
					str.Format("%d",nWidth+nRem - 12);
				}
				else if(nCols != 5 && i == 0)
				{
					str.Format("%d",nWidth+nRem);
				}
				else
				{
					str.Format("%d",nWidth);
				}


				strWidth += str + ",";
			}

			if(strWidth.Find("0,0,0,0,0") > -1)
			{
				if(nCols == 5)
					strWidth = "63,57,57,37,77";
				else
					strWidth = "75,68,68,68,68,68,68,66,68";
			}

			//strWidth.TrimRight(",");

			m_szWIDTH = strWidth + "0";

			WritePrivateProfileString(m_section, KEY_WIDTH, m_szWIDTH.operator LPCTSTR(), m_fileCFG);
			WritePrivateProfileString(m_section, KEY_WIDTH2, m_MultiViewCols.operator LPCTSTR(), tmpCfg);

			WritePrivateProfileString(SEC_MAIN, KEY_VERSION, "1", tmpCfg);
		}
		else
		{
			if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
			{
				m_szWIDTH = ((CMainWnd*)m_pMainWnd)->m_SmallDefWidth;
				m_MultiViewCols = ((CMainWnd*)m_pMainWnd)->m_SmallDefWidth;
			}
			else
			{
				m_szWIDTH = ((CMainWnd*)m_pMainWnd)->m_LargeDefWidth;
				m_MultiViewCols = ((CMainWnd*)m_pMainWnd)->m_SmallDefWidth;
			}

			WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH, m_szWIDTH.operator LPCTSTR(), m_fileCFG);
			WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH2, m_MultiViewCols.operator LPCTSTR(), tmpCfg);

			WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName+SEC_MAIN, KEY_VERSION, "1", tmpCfg);
		}
	}

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
	const int	ncnt = m_gridHdrX.GetSize();
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

// 	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
// 	{
// 		WritePrivateProfileString(m_section, KEY_WIDTH, szWIDTH.operator LPCTSTR(), m_fileCFG);
// 		WritePrivateProfileString(m_section, KEY_FIELD, szFIELD.operator LPCTSTR(), m_fileCFG);
// 	}
// 	else
// 	{
// 		WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH, szWIDTH.operator LPCTSTR(), m_fileCFG);
// 		WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_FIELD, szFIELD.operator LPCTSTR(), m_fileCFG);
// 	}

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
	const struct _treeID*	treeID = (struct _treeID*)&m_kind;
	if (treeID->kind == xINTEREST)
		return TRUE;
	return FALSE;
}

//테스트용

//void CGridWnd::testSaveFile(CString gubn, CString jongmok, CString currPrc, CString maipPrc,  CString PyugPrc, CString sonikRate)
//{
//	CString strPath(_T("")), m_root, m_user, titletime;
//	strPath.Format("%s/test.dat", m_root);
//
//	CStdioFile file;
//
//	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
//	file.SeekToEnd();
//
//	COleDateTime oTime;
//	oTime = COleDateTime::GetCurrentTime();
//	CString strCurTime;
//	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
//
//	if(gubn == "장중")
//	{
//		titletime.Format("[%s][%s]%s -> 매입단가 : %s 현재가 : %s 평가손익 : %s 손익률 : %s\n", strCurTime, gubn, jongmok, maipPrc, currPrc, PyugPrc, sonikRate);
//	}
//	else
//	{
//		titletime.Format("[%s][%s]%s -> 매입단가 : %s 예상가 : %s 평가손익 : %s 손익률 : %s\n", strCurTime, gubn, jongmok, maipPrc, currPrc, PyugPrc, sonikRate);
//	}
//
//	file.WriteString(titletime);
//	file.Close();
//}

//void CGridWnd::testSaveFile2(CString code, CString time, CString symbol, CString EvaPrc)
//{
//	CString strPath(_T("")), m_root, m_user, titletime;
//	strPath.Format("%s/test2.dat", m_root);
//
//	CStdioFile file;
//
//	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
//	file.SeekToEnd();
//
//	COleDateTime oTime;
//	oTime = COleDateTime::GetCurrentTime();
//	CString strCurTime;
//	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
//
//	titletime.Format("[%s]종목번호 : %s 심볼명 : %s 현재가 : %s \n", time, code, symbol, EvaPrc);
//
//	file.WriteString(titletime);
//	file.Close();
//}


//void CGridWnd::testSaveFile3(CString code, CString datB)
//{
//	CString strPath(_T("")), m_root, m_user, titletime;
//	strPath.Format("%s/test3.dat", m_root);
//
//	COleDateTime oTime;
//	oTime = COleDateTime::GetCurrentTime();
//	CString strCurTime;
//	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
//
//	CStdioFile file;
//
//	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
//	file.SeekToEnd();
//
//	titletime.Format("[%s][%s]%s\n", time, code, datB);
//
//	file.WriteString(titletime);
//	file.Close();
//}

void CGridWnd::SendWhenVisibleMode(CGridData* sdata)
{
	m_pGridArray.RemoveAll();
	char tempB[64]{};

	//////////////////////////////////////
	struct	_gridHdr xgridHdr;
	for (int ii = 0 ; ii < m_gridHdrX.GetSize() ; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		sprintf(tempB, "%s%c", xgridHdr.symbol, P_NEW);
		m_pGridArray.Add(tempB);
	}

	//////////////////////////////////////
	// 	CWnd* m_pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	// 	m_pWnd->SendMessage(WM_MANAGE, MK_GETTREEITEM);

	if(m_pGridData->GetCount() == 0)
	{
		return;
	}

	if(m_pGridArray.GetSize() > GRIDMAXNUM)
	{
		const int nRem = m_pGridArray.GetSize() - GRIDMAXNUM;

		if(!m_bContinue && !m_bSecond)
		{
			m_bContinue = TRUE;

			SendWhenVisibleModeTR(m_pGridData,0,GRIDMAXNUM);
		}
	}
	else
	{
		m_bContinue = FALSE;
		m_bSingle = TRUE;

		SendWhenVisibleModeTR(m_pGridData,0,m_pGridArray.GetSize());
	}
}

void CGridWnd::SendWhenVisibleModeTR(CGridData* sdata, int nStart, int nEnd, int update, int updateGroup)
{
	int	sendL = 0;
	char	tempB[64]{};
	const int bufSize = sizeof(struct _gridHi) + 50 + m_gridHdrX.GetSize()*5 + m_inters.size()*12 + 12;
	std::string sendB;
	sendB.resize(bufSize, ' ');

	const BOOL	bExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
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

	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	//2011.12.08 KSJ 2013.08.28 KSJ 이제 일반시세도 필터링되서 내려오기 때문에 따로 받을 필요 없음.
// 	sprintf(tempB, "%s%c%d%c", gFILTERING, P_DELI, 1, P_TAB);
// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
// 	sendL += strlen(tempB);
	//

	int type = m_pMainWnd->SendMessage(WM_MANAGE, MK_MARKET);
	// 중요..  여기서 NXT시장 구분값을 날려줌...
	sprintf(tempB, "%s%c%d%c", gNXT, P_DELI, type, P_TAB);				// 시장구분자   KRX :1 NXT :2  TOTAL : 3  NXT / KRX : 4 
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c%d%c", "90999", P_DELI, 1, P_TAB);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", gSYMBOL, P_DELI);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);


	struct	_gridHi* gridHi;

	gridHi = (struct _gridHi *) &sendB[sendL];
	sendL += sizeof(struct _gridHi);

	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", m_inters.size());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir  = '1';
	gridHi->sort = '0';

// 	struct	_gridHdr gridHdr;
// 	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
// 	{
// 		gridHdr = m_gridHdrX.GetAt(ii);
//
// 		sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
// 		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
// 		sendL += strlen(tempB);
// 	}
	for (int ii = nStart ; ii < nEnd ; ii++)
	{
		//gridHdr = m_gridHdrX.GetAt(ii);

		//sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
		strcpy(tempB,(LPTSTR)(LPCTSTR)m_pGridArray.GetAt(ii));
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "2321%c", P_NEW);					//전일거래량
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2022%c", P_NEW);					//새로 추가된 심볼의 현재가
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "1910%c", P_NEW);					//추천일자
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "1911%c", P_NEW);					//추천종목 기준가
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

// 	sprintf(tempB, "1912%c", P_NEW);					//수익률
// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
// 	sendL += strlen(tempB);
//
// 	sprintf(tempB, "1913%c", P_NEW);					//추천종목 기준 BM지수
// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
// 	sendL += strlen(tempB);
//
// 	sprintf(tempB, "1914%c", P_NEW);					//BM수익률
// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
// 	sendL += strlen(tempB);

	sprintf(tempB, "1915%c", P_NEW);					//지수 구분
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", CCOD, P_DELI);				// 종목코드
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);


	const int	nInterCnt = sdata->GetCount();
	CString data, code;
	int	ncnt = 0;
	CString tempStr;
	int pos = -1;

	for (int ii = 0; ii < nInterCnt; ii++)
	{
		data = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(data, PTAB);

		if (strlen(code))
		{
			tempStr = code;

			if(ii == 0 && ((CGroupWnd*)m_pGroupWnd)->m_commIndex < 2)
			{
				((CGroupWnd*)m_pGroupWnd)->m_commInfo[((CGroupWnd*)m_pGroupWnd)->m_commIndex][0] = tempStr;
			}

			pos = tempStr.Find(" ");
			if(pos != -1)
			{
				//				strcpy(temp, tempStr.Mid(0, pos));
				code = tempStr.Mid(0, pos);
			}

// 			sprintf(tempB, "%s%c", strlen(code) <= 0 ? " " : code, P_DELI);
// 			CopyMemory(&sendB[sendL], tempB, strlen(tempB));
// 			sendL += strlen(tempB);

			ncnt++;
		}

		sprintf(tempB, "%s%c", strlen(code) <= 0 ? " " : code, P_DELI);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	if(ncnt == 0)
		return;

	sendB[sendL] = P_NEW;
	sendL += 1;
	sendB[sendL] = P_TAB;
	sendL += 1;
	sendB[sendL] = 0x00;
	CSendData	ssdata;
	char	key{};
	key = 1;
	const _trkey* trkey = (struct _trkey*)&key;
	//struct _trkey* trkey = (struct _trkey*)&((CGroupWnd*)m_pGroupWnd)->m_pTrkey;

	m_UpdateGroup = updateGroup;

	if(update < 0)
	{
		m_SendKey = TRKEY_GRIDNEW;
	}
	else
	{
		m_SendKey = TRKEY_GRIDROW;
	}


	if(((CGroupWnd*)m_pGroupWnd)->m_commIndex < 2)
	{
		CString strTemp;
		strTemp.Format("%d", m_nIndex);

		((CGroupWnd*)m_pGroupWnd)->m_commInfo[((CGroupWnd*)m_pGroupWnd)->m_commIndex][1] = strTemp;

		strTemp.Format("%d", trkey->kind);
		((CGroupWnd*)m_pGroupWnd)->m_commInfo[((CGroupWnd*)m_pGroupWnd)->m_commIndex][2] = strTemp;

		((CGroupWnd*)m_pGroupWnd)->m_commIndex++;
	}

	m_updateROW = update;

	ssdata.SetData("pooppoop", key, sendB.data(), sendL, "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&ssdata);
	m_endsort = false;

}

void CGridWnd::SetInitRecommandInfo()
{
	// for(int i=0 ; i<200 ; i++)
	// {
	// 	m_iYDayVol[i][JISUGUBN] = "";
	// }
}

void CGridWnd::sendTransactionTR(int update,int nStart,int nEnd)
{
	int	sendL = 0;
	char	tempB[64]{};
	const int	bufSize = sizeof(struct _gridHi) + 50 + m_gridHdrX.GetSize()*5 + m_inters.size()*12 + 12;
	std::string sendB;
	sendB.resize(bufSize, ' ');

//	SetInitRecommandInfo();

	const BOOL	bExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
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

	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	//2011.12.08 KSJ 2013.08.28 KSJ 이제 일반시세도 필터링되서 내려오기 때문에 따로 받을 필요 없음.
// 	sprintf(tempB, "%s%c%d%c", gFILTERING, P_DELI, 1, P_TAB);
// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
// 	sendL += strlen(tempB);
	//

	int type = m_pMainWnd->SendMessage(WM_MANAGE, MK_MARKET);
	// 중요..  여기서 NXT시장 구분값을 날려줌...
	sprintf(tempB, "%s%c%d%c", gNXT, P_DELI, type, P_TAB);				// 시장구분자   KRX :1 NXT :2  TOTAL : 3  NXT / KRX : 4 
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c%d%c", "90999", P_DELI, 1, P_TAB);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", gSYMBOL, P_DELI);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct	_gridHi* gridHi;

	gridHi = (struct _gridHi *) &sendB[sendL];
	sendL += sizeof(struct _gridHi);

	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", m_inters.size());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir  = '1';
	gridHi->sort = '0';

	//////////////////////////////////////
	//struct	_gridHdr gridHdr;
	for (int ii = nStart ; ii < nEnd ; ii++)
	{
		//gridHdr = m_gridHdrX.GetAt(ii);

		//sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
		strcpy(tempB,(LPTSTR)(LPCTSTR)m_pGridArray.GetAt(ii));
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "2321%c", P_NEW);					//전일거래량
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2022%c", P_NEW);					//새로 추가된 심볼의 현재가
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "1910%c", P_NEW);					//추천일자
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "1911%c", P_NEW);					//추천종목 기준가
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	// 	sprintf(tempB, "1912%c", P_NEW);					//수익률
	// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	// 	sendL += strlen(tempB);
	//
	// 	sprintf(tempB, "1913%c", P_NEW);					//추천종목 기준 BM지수
	// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	// 	sendL += strlen(tempB);
	//
	// 	sprintf(tempB, "1914%c", P_NEW);					//BM수익률
	// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	// 	sendL += strlen(tempB);

	sprintf(tempB, "1915%c", P_NEW);					//지수 구분
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2950%c", P_NEW);						//2012.06.19 KSJ 배분정보
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2951%c", P_NEW);						//2012.06.19 KSJ 임의종료
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", CCOD, P_DELI);				// 종목코드
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	//struct _inters* pinters{};
	int	ncnt = 0;
	const int nInterCnt = m_inters.size();
	CString tempStr;
	int pos = -1;

	for (int ii = 0; ii < nInterCnt; ii++)
	{
		auto& pinters = m_inters.at(ii);

		if (strlen(pinters.get()->code))
		{
			tempStr= CString(pinters.get()->code);

			pos = tempStr.Find(" ");
			if(pos != -1)
			{
				pinters.get()->code = tempStr.Mid(0, pos);
			}

			ncnt++;
		}

		sprintf(tempB, "%s%c", strlen(pinters.get()->code) <= 0 ? " " : pinters.get()->code, P_DELI);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	if(ncnt == 0)
	{
		m_bContinue = FALSE;	//2013.07.11	KSJ	보유종목이나 빈그룹을 조회했을때 ncnt가 0이 된다. 그래서 초기화 해준다.
		m_bSecond = FALSE;	//2013.07.11	KSJ 보유종목이나 빈그룹을 조회했을때 ncnt가 0이 된다. 그래서 초기화 해준다.
		return;
	}

	sendB[sendL] = P_NEW;
	sendL += 1;
	sendB[sendL] = P_TAB;
	sendL += 1;
	sendB[sendL] = 0x00;
	CSendData	sdata;
	char	key{};
	_trkey* trkey = (struct _trkey*)&key;

	trkey->group = m_nIndex;

	if (m_staticUpdate < 0)
	{
		trkey->kind = TRKEY_GRIDNEW;
	}
	else if(m_staticUpdate >= 0 && m_staticUpdate < 200)
	{
		trkey->kind = TRKEY_GRIDROW;
	}
	else
	{
		trkey->kind = TRKEY_GRIDUPJONG;
	}

	m_updateROW = m_staticUpdate;

	sdata.SetData("pooppoop", key, sendB.data(), sendL, "");

	m_SendKey = TRKEY_GRIDNEW;
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
	m_endsort = false;
	m_grid->initFilter();

}

void CGridWnd::sendTransaction(int update)
{
	m_pGridArray.RemoveAll();
	m_staticUpdate = update;

	char tempB[64]{};

	//////////////////////////////////////
	struct	_gridHdr xgridHdr;
	for (int ii = 0 ; ii < m_gridHdrX.GetSize() ; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		sprintf(tempB, "%s%c", xgridHdr.symbol, P_NEW);
		m_pGridArray.Add(tempB);
	}

	// if(m_pGridArray.GetSize() > GRIDMAXNUM)
	// {
	// 	const int nRem = m_pGridArray.GetSize() - GRIDMAXNUM;

	// 	if(!m_bContinue && !m_bSecond)
	// 	{
	// 		m_bContinue = TRUE;

	// 		sendTransactionTR(m_staticUpdate,0,GRIDMAXNUM);
	// 	}
	// }
	// else
	{
		m_bContinue = FALSE;
		m_bSingle = TRUE;

		sendTransactionTR(m_staticUpdate,0,m_pGridArray.GetSize());
	}
}

void CGridWnd::SaveWidth()
{

}

BOOL CGridWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	NM_GVITEM* nmgv = (NM_GVITEM*) lParam;


	if (wParam != IDC_GRID)
		return CBaseWnd::OnNotify(wParam, lParam, pResult);

	switch (nmgv->hdr.code)
	{
	case GVNM_LMOUSEDOWN:
		{
			CString code, string;
			CString strINI;
			strINI.Format("%s\\User\\%s\\userconf.ini", m_root, m_user);

			if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col == colSIG)
			{
				//2015.01.15 KSJ 빈값이 들어가므로 trim 해준다.
				CString sRecommand;
				sRecommand = GetRecommandInfo(nmgv->row);
				sRecommand.TrimRight(); sRecommand.TrimLeft();

				if(sRecommand != "")
				{
					code = m_grid->GetItemText(nmgv->row, colCODE);
					code.TrimLeft(); code.TrimRight();
					if (code.IsEmpty()) break;

					WritePrivateProfileString("IB202600", "POPUP_FLAG", "1", strINI);
					WritePrivateProfileString("IB202600", "POPUP_CODE", code, strINI);
					string.Format("IB202600 /S/t0/d%s\t%s",HCOD, code);
					openView(typeVIEW, string);
					CString codepopup;
					codepopup.Format("%s\t%s", HCOD, code);

				}

			}
		}
		break;
	case GVNM_REFRESH:
		{
			m_bEditWork = FALSE;
			m_pToolWnd->SendMessage(WM_MANAGE, MK_REFRESH);

			ClearSearchMap();	//2011.12.29 KSJ
			ReSetSearchMap();	//2011.12.29 KSJ
		}
		break;
	case GVNM_CHGCOLSIZE:
		{
		const int	ncnt = m_gridHdrX.GetSize();
			CString	sztmp = _T("");
			int	nWidth{};
			m_szWIDTH = _T("");

			struct _gridHdr	grdHdr;
			for ( int ii = 0 ; ii < ncnt ; ii++ )
			{
				grdHdr = m_gridHdrX.GetAt(ii);
				nWidth = m_grid->GetRealColumnWidth(ii);	//2015.05.25 KSJ

				if (ii >= colNAME)
//				if (ii >= colCODE)
				{
					sztmp.Format("%d,", nWidth);
					m_szWIDTH += sztmp;

					if (ii == colCODE && nWidth != 0) m_nCodeWidth = nWidth;
				}

				// ADD PSH 20070911
// 				if (ii == nmgv->col)
// 				{
// 					m_pTicker->SetColumnWidth(ii, nWidth);
//
// 					m_pTicker->Adjust();
// 				}
				// END ADD
			}

			if(((CMainWnd*)m_pMainWnd)->m_param.options == "WIDE")
			{
				if(m_fileCFG.Find("intercfg3.ini") > -1)
				{
					m_MultiViewCols = m_szWIDTH;
				}

				if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
				{
					WritePrivateProfileString(m_section, KEY_WIDTH2, m_MultiViewCols.operator LPCTSTR(), m_fileCFG);
				}
				else
				{
					WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH2, m_MultiViewCols.operator LPCTSTR(), m_fileCFG);
				}
			}

			if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
			{
				WritePrivateProfileString(m_section, KEY_WIDTH, m_szWIDTH.operator LPCTSTR(), m_fileCFG);
			}
			else
			{
				WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH, m_szWIDTH.operator LPCTSTR(), m_fileCFG);
			}

			CString send;
			send.Format("%s%c%d", "RefreshInterest", P_TAB, ((CMainWnd*)m_pMainWnd)->m_param.key);

			m_pViewWnd->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)send);
		}
		break;
	case GVNM_CHANGECODE:
		{
			if(((CMainWnd*)m_pMainWnd)->m_bRemain == TRUE)
			{
				return 0;
			}

			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int pageNumber = pWnd->SendMessage(WM_MANAGE, MK_GETVIEWPAGE);
			const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

			if(nOver == MO_VISIBLE && m_tempCode != "")
			{
//				if(m_tempCode != "")
				{
					CString code, oldname, name;

					code = m_grid->GetItemText(nmgv->row, colCODE);
					const int idx = nmgv->row;
					insertInterest(nmgv->row - 1);
					insertRow(nmgv->row);


					m_selectRow = nmgv->row;		//선택한 열을 저장


					m_grid->SetFocus();

					m_grid->SetItemText(nmgv->row, colCODE, code);
					m_grid->SetItemText(nmgv->row+1, colCODE, m_tempCode);

					if(code.GetLength() == 6 || code.GetLength() == 8 || code.GetLength() == 9)
					{
						name = GetCodeName(code);
					}

					struct	_inters* pinters{};
					const int	count = m_inters.size();
					const int	xrow  = nmgv->row - 1;

					//종목 중복 등록 허용
					if(((CGroupWnd*)GetParent())->GetOverLapAction() == FALSE)
					{
						const bool bDup = m_pGroupWnd->SendMessage(WM_MANAGE,MAKEWPARAM(MK_DUPLICATE,nmgv->row),(LPARAM)code.operator LPCTSTR());

						//if (code.GetLength() && IsDuplicateCode(code, nmgv->row))
						if(code.GetLength() && bDup)
						{
							CCodeDuplicate dlg;

							//dlg 항상 열기 옵션 체크
							if(dlg.DoModal())
							{
								if(dlg.m_bAlert == FALSE)
								{
 									m_grid->SetItemText(nmgv->row, colCODE, "");

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
								const auto& pInter = m_inters.at(nmgv->row - 1);
								m_grid->SetItemText(nmgv->row, colCODE, pInter.get()->code);
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
						auto& pinterrow = m_inters.at(xrow);

						// ADD PSH 20070913
						const int nRowCnt = m_arrBaseInters.size();
						if (m_bSorting)
						{
							if (!pinterrow.get()->code.IsEmpty())
							{
								//2011.12.19 KSJ 소팅하고나서 종목을 바꾸면 여기서 에러남.
								//m_arrBaseInters 사이즈가 0임..
								//rebuildInterest()함수의 주석 처리된 부분 해제함.
								//CopyInter(m_arrBaseInters.GetAt(xrow), pinters);
								m_arrBaseInters.at(xrow) = pinterrow;
							}
							else
							{
								auto& pBaseInter = m_arrBaseInters.emplace_back(std::make_shared<_intersx>());

								pBaseInter.get()->gubn = '0';
								pBaseInter.get()->code = code;
								pBaseInter.get()->name = name;

							}
						}
						// END ADD

						pinterrow.get()->gubn = '0';
						pinterrow.get()->code = code;
						pinterrow.get()->name = name;
						m_grid->SetItemText(xrow + 1, colNAME, name);
						//m_inters.at(xrow) =  pinters;
					}
					else
					{
						for (int ii = count; ii < xrow; ii++)
						{
							auto& xpinters = m_inters.emplace_back(std::make_shared<_intersx>());
							xpinters.get()->empty();
						}

						auto& pintersx = m_inters.emplace_back(std::make_shared<_intersx>());
						pintersx.get()->gubn = '0';
						pintersx.get()->code = code;
						pintersx.get()->name = name;
						m_grid->SetItemText(xrow + 1, colNAME, name);
						//AddInters(pinters);
					}

					if (code.GetLength() <= 0)
					{
						for (int ii = 0; ii < m_colC; ii++)
							m_grid->SetItemText(nmgv->row, ii, "");
						m_grid->setmemoCheck(nmgv->row);
						break;
					}
				}


				m_tempCode = "";


			}
			else
			{
				m_selectRow = nmgv->row;		//선택한 열을 저장


				m_grid->SetFocus();
				CString code, oldname, name;
				code = m_grid->GetItemText(nmgv->row, colCODE);

				if (code.GetLength() == 6 || code.GetLength() == 8 || code.GetLength() == 9)
				{
					name = GetCodeName(code);
				}

				//struct	_inters* pinters{};
				const int	count = m_inters.size();
				const int	xrow  = nmgv->row - 1;

				//종목 중복 등록 허용
				if(((CGroupWnd*)GetParent())->GetOverLapAction() == FALSE)
				{
					const bool bDup = m_pGroupWnd->SendMessage(WM_MANAGE,MAKEWPARAM(MK_DUPLICATE,nmgv->row),(LPARAM)code.operator LPCTSTR());

					//if (code.GetLength() && IsDuplicateCode(code, nmgv->row))
					if(code.GetLength() && bDup)
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
							const auto& pInter = m_inters.at(nmgv->row - 1);
							m_grid->SetItemText(nmgv->row, colCODE, pInter.get()->code);
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
					auto& pinters = m_inters.at(xrow);

					// ADD PSH 20070913
					const int nRowCnt = m_arrBaseInters.size();
					if (m_bSorting)
					{
						if (!pinters.get()->code.IsEmpty())
						{
							m_arrBaseInters.at(xrow) = pinters;
						}
						else
						{
							auto& pBaseInter = m_arrBaseInters.emplace_back(std::make_shared<_intersx>());
							pBaseInter.get()->gubn = '0';
							pBaseInter.get()->code = code;
							pBaseInter.get()->name = name;
						}
					}
					// END ADD

					pinters.get()->gubn = '0';
					pinters.get()->code = code;
					pinters.get()->name = name;
					m_grid->SetItemText(xrow + 1, colNAME, name);
				//	m_inters.at(xrow) = pinters;
				}
				else
				{
					for (int ii = count; ii < xrow; ii++)
					{
						auto& pinters = m_inters.emplace_back(std::make_shared<_intersx>());
						pinters.get()->gubn = '0';
					}

					auto& pinters = m_inters.emplace_back(std::make_shared<_intersx>());
					//ZeroMemory(pinters, sz_inters);

					pinters.get()->gubn = '0';
					pinters.get()->code = code;
					pinters.get()->name = name;
					m_grid->SetItemText(xrow + 1, colNAME, name);
					//AddInters(pinters);
				}

				if (code.GetLength() <= 0)
				{
					for (int ii = 0; ii < m_colC; ii++)
						m_grid->SetItemText(nmgv->row, ii, "");
					m_grid->setmemoCheck(nmgv->row);
					break;
				}
			}

			//if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_pGroupWnd->SendMessage(WM_MANAGE,MK_EDITWORK,TRUE);

			saveInterest(true);

			pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));

			m_nSelectedRow = nmgv->row;
			m_nScrollPos = m_grid->GetScrollPos32(SB_VERT);

			if(nOver == MO_VISIBLE)
			{
				pWnd->SendMessage(WM_MANAGE, MK_RELOADLIST);
			}
			else
			{
				sendTransaction(nmgv->row);
			}

			ClearSearchMap();	//2011.12.29 KSJ
			ReSetSearchMap();	//2011.12.29 KSJ
		}
		break;
	case GVNM_ENDEDIT: case GVNM_ENDRETURN:
		{
			m_grid->SetFocus();
			const int	nRow = nmgv->row, nCol = nmgv->col;

			CString	symbol, curr;
			symbol = m_grid->GetItemSymbol(nRow, nCol);
			curr = m_grid->GetItemText(nRow, colCURR);
			const auto& pInters = GetData(nRow - 1);
			CString	value = m_grid->GetItemText(nRow, nCol);

			CString zeroValue = "0";

			//2011.12.20 KSJ
			if(symbol.IsEmpty())
				break;
			//KSJ

			if (symbol.CompareNoCase("###1") == 0) // 보유수량
			{
				value = m_grid->GetItemText(nRow, nCol);
				pInters->xnum = value;

				if (m_bChgCnd)
				{
					saveInterest();
				}
			}
			else if (symbol.CompareNoCase("###2") == 0) // 보유단가
			{
				pInters->xprc = value;

				if (m_bChgCnd)
				{
					saveInterest();
				}
			}
			else
			//2011.12.20 KSJ 중괄호 되어 있지 않아서 추가함.
			{
				Invalidate();
				break;
			}
			// KSJ

			//if (m_sonikField > colCURR)
			//{
			//	value = CalcuPyungaSonik(pInters.get(), curr);
			//	m_grid->SetItemText(nRow, m_sonikField, value);
			//}
			//
			//if (m_suikField > colCURR)
			//{
			//	value = CalcuSuik(pInters.get(), curr);
			//	m_grid->SetItemText(nRow, m_suikField, value);
			//}
			m_bEditWork = TRUE;
		}

		ClearSearchMap();	//2011.12.29 KSJ
		ReSetSearchMap();	//2011.12.29 KSJ

		break;

	case GVNM_COLUMNMOVE:
		{
			if (nmgv->row >= m_gridHdrX.GetSize()
					|| nmgv->col >= m_gridHdrX.GetSize())
				break;

// 			m_pTicker->MoveColAction(nmgv->col, nmgv->row);
// 			m_pTicker->ResetScrollBars();
//			m_pTicker->Invalidate();
//			m_btDomino.Invalidate(TRUE);  //test 20230209
//			m_btSAVE.Invalidate(TRUE);
//			m_btCLOSE.Invalidate(TRUE);
			_gridHdr xgridHdr;

			xgridHdr = m_gridHdrX.GetAt(nmgv->col);
			m_gridHdrX.InsertAt(nmgv->row, xgridHdr);

			if (nmgv->col > nmgv->row)
				nmgv->col += 1;

			m_gridHdrX.RemoveAt(nmgv->col);
			ReIndexSpecial();

			const int	ncnt = m_gridHdrX.GetSize();
			CString	sztmp, szFIELD = _T("");
			int	nWidth{};
			m_szWIDTH = _T("");
			m_szFIELD = _T("");
			for ( int ii = 0 ; ii < ncnt ; ii++ )
			{
				xgridHdr = m_gridHdrX.GetAt(ii);
				nWidth = m_grid->GetColumnWidth(ii);
				if (ii >= colNAME)
				{
					sztmp.Format("%d,", nWidth);
					m_szWIDTH += sztmp;
				}
				if (ii > colCURR)
				{
					sztmp.Format("%s,", xgridHdr.symbol);
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

			if(((CMainWnd*)m_pMainWnd)->m_param.options == "WIDE")
			{
				if(m_fileCFG.Find("intercfg3.ini") > -1)
				{
					m_MultiViewCols = m_szWIDTH;
				}
			}

			SaveAsInterField();
		}
		break;

	case GVNM_BEGINDRAG:
		m_drag = nmgv->row; m_drop = -1;
		m_strCode = m_grid->GetItemText(m_drag, colCODE);

		if (CAST_TREEID(m_kind)->kind == xISSUE)
			CintGrid::m_dropdata.SetCode(m_grid->GetItemText(m_drag, colCODE));

		break;
	case GVNM_OUTDRAG:

		{
			if (((CMainWnd*)m_pMainWnd)->m_bRemain == FALSE && m_grid->m_bOutPos)
			{
				DeleteRow(m_drag);
				m_pGroupWnd->SendMessage(WM_MANAGE,MK_EDITWORK,TRUE);
				m_grid->m_bOutPos = false;

				saveInterest(true);
			}
		}
		break;
	case GVNM_ENDEND:
		{
			if(((CMainWnd*)m_pMainWnd)->m_bRemain == TRUE)
			{
				return 0;
			}
			//드래그 카운트용 호출
			((CGroupWnd*)m_pGroupWnd)->AddDragInCount();
			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int pageNumber = pWnd->SendMessage(WM_MANAGE, MK_GETVIEWPAGE);

			const int xdrag = m_drag, xdrop = nmgv->row; m_drop = nmgv->row;
			int nIdx = 0;
			CString	code, name;

			m_grid->m_bOutPos = false;

			code = CintGrid::m_dropdata.GetCode();
			name = GetCodeName(code);//CintGrid::m_dropdata.GetCodeName();


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


			if ( m_drop < 0 )
				m_drop = m_grid->GetRowCount();
			else if ( m_drop == 0 )
				m_drop = 1;

			if ( m_drop - 2 < 0 )
				nIdx = 0;
			else
				nIdx  = m_drop - 2 ;

			//auto& pinters = m_inters.at(nIdx);  // //20231107 drag&drop code
			auto pinters = m_inters.at(nIdx);
			if (!pinters.get()->code.IsEmpty())
			{
//				pinters = m_inters.GetAt(m_inters.GetUpperBound());
//				if (strlen(pinters->code) > 0)
				const CWnd*	ptoolWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
				const int	nOver = (int)ptoolWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
				int rowcount = 0;

				if(nOver == MO_VISIBLE)
				{
					rowcount = GetRowcountVisibleMode();
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

				m_selectRow = m_drop;
				insertInterest(m_drop - 1);
				insertRow(m_drop);
				m_drop -= 1;
				pinters = m_inters.at(m_drop);
			}

			pinters.get()->gubn = '0';
			pinters.get()->code = code;
			pinters.get()->name = name;

			//struct _inters InterData;
			//memset(&InterData,0x20,sizeof(struct _inters));

			//InterData.gubn[0] = '0';
			//memcpy(InterData.code,code,sizeof(InterData.code));
			//memcpy(InterData.name,name,sizeof(InterData.name));
			//memcpy(pinters, &InterData, sizeof(struct _inters));

			m_pGroupWnd->SendMessage(WM_MANAGE,MK_EDITWORK,TRUE);

			m_drag = m_drop = -1;
			m_seldrop = xdrop;
			saveInterest(true);   //드래그해서 종목 넣을때도 서버저장하려면 saveinterest() 해줘야 함

			pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

			if(nOver == MO_VISIBLE)
			{
				pWnd->SendMessage(WM_MANAGE, MK_RELOADLIST);
			}
			else
			{
				sendTransaction();
			}


			m_pGroupWnd->SendMessage(WM_MANAGE, MK_ENDDRAG);

		}
		break;
	case GVNM_ENDDRAG:
		{
			if(((CMainWnd*)m_pMainWnd)->m_bRemain == TRUE)
			{
				return 0;
			}

			if (((CMainWnd*)m_pMainWnd)->m_bChangeGroup == TRUE)
			{
				return 0;
			}

			m_grid->m_bOutPos = false;

			CDropData*	pDropData = &CintGrid::m_dropdata;	// drop information
			CintGrid*	pGrid = pDropData->GetGrid();	        // drag grid
			if (pGrid == nullptr)
				break;

			const int nDGrow = pDropData->GetRow();	// drag row
			const int nR = pDropData->GetGrid()->GetRowCount() - 1;
			const int ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);

			if(ret != 0)		//등록순이 아닐경우 드래그 허용 안함
			{
				m_grid->SetSelectRow(nDGrow);
				return 0;
			}

			CGridWnd*	 pDropGrid = (CGridWnd*)pDropData->GetGrid()->GetParent();	// drag gridwnd
			const CGroupWnd* pDropGroup = (CGroupWnd*)pDropGrid->GetParent();		// drag groupwnd
			const int xdrag = m_drag, xdrop = nmgv->row;
			int nIdx = 0 ;//BUFFET
			CString xCODE;
			CString	code, name;
			BOOL	bNewsDrop = FALSE;
			const UINT	kind = (UINT)pDropGrid->SendMessage(WM_MANAGE, MK_GETDATAKIND);

			if (CAST_TREEID(kind)->kind == xISSUE)
				bNewsDrop = TRUE;

			if (bNewsDrop)
				xCODE = CintGrid::m_dropdata.GetCode();

			m_drop = nmgv->row;

			m_selectRow	= m_drop;
			const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int pageNumber = pWnd->SendMessage(WM_MANAGE, MK_GETVIEWPAGE);

			if (pDropData->GetGrid()->m_hWnd == m_grid->m_hWnd) // m_drag > 0 && m_drop > 0)		// 동일한 화면에서 drop
			{
				if (m_drop < 0)
					m_drop = m_grid->GetRowCount() - 1;
				else if (m_drop == 0)
					m_drop = 1;

				if (m_drag == m_drop)
					break;

				code = m_grid->GetItemText(m_drag, colCODE);
				if (code.TrimRight().GetLength() == 0) break;
				if (code.Find("m00") >= 0) break;

				auto pDrag = m_inters.at(m_drag - 1);
				if (m_drag > m_drop)   //test 확인 20230209   밑에서 올릴때
				{
					m_inters.erase(m_inters.begin() + m_drag - 1);
					m_inters.insert(m_inters.begin() + m_drop - 1, pDrag);
				}
				else   //위에서 내릴때 
				{
					m_inters.insert(m_inters.begin() + m_drop, pDrag);
					m_inters.erase(m_inters.begin() + m_drag - 1);
					//	m_inters.insert(m_inters.begin() + m_drop - 1, pDrag);
					//	m_inters.erase(m_inters.begin() + m_drag - 1);
				}
				//m_inters.erase(m_inters.begin() + m_drag - 1);
				//m_inters.insert(m_inters.begin() + m_drop - 1, pDrag);
			}
			else if (pDropData->GetGrid()->m_hWnd != m_grid->m_hWnd)//(m_drag < 0 && m_drop > 0)	// 다른 화면에서 drop
			{		
				if (GetRowcount() == 0) break;

				code = (bNewsDrop) ? xCODE : pGrid->GetItemText(nDGrow, colCODE);
				name = pGrid->GetItemText(nDGrow, colNAME);

				if (code.TrimRight().GetLength() == 0) break;
				if (code.Find("m00") >= 0) break;

				if (m_drop < 0)
					m_drop = m_grid->GetRowCount();
				else if (m_drop == 0)
					m_drop = 1;


				//if (pDropData->GetGrid()->m_hWnd != m_grid->m_hWnd)
				//{
				//	//종목 중복 등록 허용
				//	if (((CGroupWnd*)GetParent())->GetOverLapAction() == FALSE)
				//	{
				//		if (!code.IsEmpty() && IsDuplicateCode(code, m_drop, true))
				//		{
				//			CCodeDuplicate dlg;
				//			// dlg 항상 열기 옵션 체크
				//			if (dlg.DoModal())
				//			{
				//				if (dlg.m_bAlert == FALSE)
				//				{
				//					m_grid->Invalidate(FALSE);
				//					break;
				//				}
				//				else
				//				{
				//					//파일에 저장
				//					((CGroupWnd*)GetParent())->saveOverLap(TRUE);
				//				}
				//			}
				//			else
				//			{
				//				m_grid->Invalidate(FALSE);
				//				break;
				//			}
				//		}
				//	}
				//}

				if (m_drop - 2 < 0)
					nIdx = 0;
				else
					nIdx = m_drop - 2;

				const auto& pinters = m_inters.at(nIdx);
				//if (!pinters->code.IsEmpty())  //test 20230209
				{
					//CWnd* pgroupWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
					//const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
					int rowcount = GetRowcount();
					//int maxcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();

					if (rowcount >= MAX_LINE)
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
				}
				//그리드에 한줄도 없을 경우 임의로 한줄 추가 생성
				/*else
				{
					if (nIdx == 0 && m_rowC < 2)
					{
						insertRow(m_drop);
					}
				}*/

				auto& pDrop = m_inters.at(m_drop);
				// 2013.01.22 KSJ 정렬후에는 그리드의 데이터를 가져올 때 반드시 현재 m_inters의 현재의치를 찾아야 한다.
				const auto& pInterDrag = pDropGrid->GetData(pDropGrid->GetInter(code));
				pDrop = pInterDrag;

				// ADD PSH 20070918
				UINT uAttr{};
				if ('m' == pinters.get()->code[0] || pinters.get()->gubn == ROW_COMMENT) //test mod 기존이 먼가 이상?
				{
					uAttr = m_grid->GetItemAttr(m_drop + 1, colNAME) | GVAT_MARKER;
					m_grid->SetItemAttr(m_drop + 1, colNAME, uAttr);
					m_grid->SetItemText(m_drop + 1, colNAME, pinters.get()->name);
				}
				else
				{
					uAttr = m_grid->GetItemAttr(m_drop + 1, colNAME) & ~GVAT_MARKER;
					m_grid->SetItemAttr(m_drop + 1, colNAME, uAttr);
					m_grid->SetItemText(m_drop + 1, colNAME, pinters.get()->name);
				}
				// END ADD

				const LPARAM data = pGrid->GetItemData(nDGrow, colSIG);
				m_grid->SetItemData(xdrop, colSIG, data);

				// index가 하나 차이가 나므로
				if (pDropGroup->m_hWnd == m_pGroupWnd->m_hWnd)
				{
					if (!bNewsDrop)
						pDropGrid->SendMessage(WM_MANAGE, MAKEWPARAM(MK_DELETEROW, nDGrow));
				}
			}

		//	return 0;


			if (!m_bSorting)
			{
				saveInterest(true);
				m_pGroupWnd->SendMessage(WM_MANAGE,MK_EDITWORK,TRUE);

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

			m_grid->SetSelectRow(m_drop);

			m_drag = m_drop = -1;
			m_seldrop = xdrop;
			//pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int nOver = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
			if(nOver == MO_VISIBLE)
			{
				//pWnd->SendMessage(WM_MANAGE, MK_SETPAGEVIEW, pageNumber);
				pWnd->SendMessage(WM_MANAGE, MK_RELOADLIST);
			}
			else
			{
//				pWnd->SendMessage(WM_MANAGE, MK_SENDTREE);
				sendTransaction();
			}

			m_pGroupWnd->SendMessage(WM_MANAGE, MK_ENDDRAG);
		}
		break;
	case GVNM_SELCHANGED:
		{
			CString code, string;

			if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
			{
				code = m_grid->GetItemText(nmgv->row, colCODE);
				code.TrimLeft();
				code.TrimRight();

				if (code.IsEmpty()) break;

				if (((CGroupWnd*)m_pGroupWnd)->GetSelAction() == 0)
					break;

// 				if(m_nIndex == 0)
// 				{
// 					((CGroupWnd*)m_pGroupWnd)->SetKillFocus(1);
// 				}
// 				else
// 				{
// 					((CGroupWnd*)m_pGroupWnd)->SetKillFocus(0);
// 				}


				switch (GetCodeType(code))
				{
				case kospiCODE:
					string.Format("%s\t%s", HCOD, code);
					 break;
				case futureCODE:
					string.Format("%s\t%s", "ed_focod", code);	//2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
					Variant(triggerCC, string);

					string.Format("%s\t%s", FCOD, code);
					break;
				case optionCODE:
					string.Format("%s\t%s", "ed_focod", code);	//2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
					Variant(triggerCC, string);

					string.Format("%s\t%s", OCOD, code);
					break;
				case foptionCODE:
					string.Format("%s\t%s", "ed_focod", code);	//2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
					Variant(triggerCC, string);

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
					activeTrigger(code);
					//Variant(triggerCC, string);
					string.Format("%s\t%s", ELWCOD, code);
					m_grid->SetFocus();
					break;
				case thirdCODE:
					string.Format("%s\t%s", THIRDCOD, code);
					break;
				case 0:		//2012.10.11 KSJ 주식, 금리, 통화선물 추가
					if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물 //파생상품 코드개편
					{
						int nGubn = atoi(code.Mid(1,2));
						CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ 주식선물 신규종목 연동

						if(nGubn > 10 && nGubn < 60)	//주식선물
						{
							string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
							Variant(triggerCC, string);

							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if(nGubn > 60 && nGubn <= 70)	//금리선물
						{
							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if(nGubn > 70 && nGubn < 80)	//통화선물
						{
							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
							|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
							|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z"))
						{
							string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
							Variant(triggerCC, string);

							// m_slog.Format("[cx_interest][SF][%s]<%d> code=[%s] string=[%s]", __FUNCTION__, __LINE__, code, string);
							// OutputDebugString(m_slog);

							string.Format("%s\t%s", SFOD, code);
							break;
						}
					}
				}

 	//			Variant(triggerCC, string);
				m_grid->SetFocus();

				break;
			}
			// Map 초기화 KSI
			ClearSearchMap();
			ReSetSearchMap();	//2011.12.29 KSJ

			break;
		}

	case GVNM_TRIGGER:
		{
			CString code = m_grid->GetItemText(nmgv->row, colCODE);
			CString string;
//			if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
			{
//				code = m_grid->GetItemText(nmgv->row, colCODE);
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
					activeTrigger(code);
					//Variant(triggerCC, string);
//					m_grid->SetFocus();
					string.Format("%s\t%s", ELWCOD, code);
					break;
				case thirdCODE:
					string.Format("%s\t%s", THIRDCOD, code);
					break;
				case 0:		//2012.10.11 KSJ 주식, 금리, 통화선물 추가
					if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물  //파생상품 코드개편
					{
						int nGubn = atoi(code.Mid(1,2));
						CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ 주식선물 신규종목 연동

						if(nGubn > 10 && nGubn < 60)	//주식선물
						{
							string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
							Variant(triggerCC, string);

							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if(nGubn > 60 && nGubn <= 70)	//금리선물
						{
							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if(nGubn > 70 && nGubn < 80)	//통화선물
						{
							string.Format("%s\t%s", SFOD, code);
							break;
						}
						else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
							|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
							|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z"))
						{
							string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
							Variant(triggerCC, string);

							// m_slog.Format("[cx_interest][SF][%s]<%d> code=[%s] string=[%s]", __FUNCTION__, __LINE__, code, string);
							// OutputDebugString(m_slog);

							string.Format("%s\t%s", SFOD, code);
							break;
						}
					}
				}

				activeTrigger(code);
// 				Variant(triggerCC, string);
				Variant(codeCC, string);

// 				TRACE("\nClick point is %d, %d, %s\n\n", nmgv->row, m_ClickRow, string);
 				if(nmgv != nullptr)
 				{
 					if(nmgv->row != m_ClickRow)
 					{
 						m_grid->SetFocus();
 						m_ClickRow = nmgv->row;

						((CGroupWnd*)m_pGroupWnd)->m_strCode = code;

						SetTimer(5500,500,nullptr);
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
// 				break;
			}
		}
		break;
	case GVNM_DBLCLICKED:
		{
			CString code, string;
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
					case thirdCODE:	//2013.08.07 프리보드일때도 2001번을 띄워준다.
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
					case 0:		//2012.10.11 KSJ 주식, 금리, 통화선물 추가
						if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물 파생상품 코드개편
						{
							int nGubn = atoi(code.Mid(1,2));
							CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ 주식선물 신규종목 연동

							if(nGubn > 10 && nGubn < 60)	//주식선물
							{
								string.Format("%s /S/t0/d%s\t%s", sfcurrMAP, SFOD, code);
								openView(typeVIEW, string); break;
							}
							else if(nGubn > 60 && nGubn <= 70)	//금리선물
							{
								string.Format("%s /S/t0/d%s\t%s", cccurrMAP, SFOD, code);
								openView(typeVIEW, string); break;
							}
							else if(nGubn > 70 && nGubn < 80)	//통화선물
							{
								string.Format("%s /S/t0/d%s\t%s", cccurrMAP, SFOD, code);
								openView(typeVIEW, string); break;
							}
							else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
								|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
								|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z"))
							{
								string.Format("%s /S/t0/d%s\t%s", sfcurrMAP, SFOD, code);
								openView(typeVIEW, string); break;

								// m_slog.Format("[cx_interest][SF][%s]<%d> code=[%s] string=[%s]", __FUNCTION__, __LINE__, code, string);
								// OutputDebugString(m_slog);
							}
						}
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
						case 0:		//2012.10.11 KSJ 주식, 금리, 통화선물 추가
							if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물 파생상품 코드개편
							{
								int nGubn = atoi(code.Mid(1,2));
								const CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ 주식선물 신규종목 연동

								if(nGubn > 10 && nGubn < 60)	//주식선물
								{
									string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
									openView(typeVIEW, string); break;
								}
								else if(nGubn > 60 && nGubn <= 70)	//금리선물
								{
									string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
									openView(typeVIEW, string); break;
								}
								else if(nGubn > 70 && nGubn < 80)	//통화선물
								{
									string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
									openView(typeVIEW, string); break;
								}
								else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
									|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
									|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z"))
								{
									string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
									openView(typeVIEW, string); break;

									// m_slog.Format("[cx_interest][SF][%s]<%d> code=[%s] string=[%s]", __FUNCTION__, __LINE__, code, string);
									// OutputDebugString(m_slog);
								}
							}
						}
						break;
					}
				}

				activeTrigger(code);
			}
		}
		break;
	case GVNM_ENDSORT:
		{
			if (CAST_TREEID(m_kind)->kind == xINTEREST)
			{
				// 				m_bEditWork = true;		//2012.02.13 KSJ 변경한것 없어도 소트하고나면 저장하라고해서 주석처리함.
			}

			m_endsort = true;
			m_grid->getmemoCheck();

			rebuildInterest();

			COLORREF bkColor{};
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

			// ADD PSH 20070913
			m_bSorting = TRUE;
			// END ADD

			// Map 초기화 KSI
			ClearSearchMap();
			ReSetSearchMap();	//2011.12.29 KSJ
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
		}
		// END MODIFY
		break;
	// ADD PSH 20070914
	case GVNM_CHANGEMARKER:
		{
			m_grid->SetFocus();
			CString strMarker, strMarkerCode;
			strMarker = m_grid->GetItemText(nmgv->row, colNAME);
			const int nMarkerRow  = nmgv->row - 1;
			strMarkerCode.Format("m%05d", nMarkerRow);

			if (!m_bSorting)
			{
				auto& pInter = m_inters.at(nMarkerRow);
				pInter.get()->code = strMarkerCode;
				pInter.get()->name = strMarker;

				saveInterest();
			}
		}
		break;
	// END ADD
	}

	return CBaseWnd::OnNotify(wParam, lParam, pResult);
}

void CGridWnd::SelectRowClear()
{
	m_grid->m_bDrag = TRUE;
}

void CGridWnd::SaveAsInterField()
{
	if (m_szFIELD.IsEmpty())
		return;

	CString tempFile;

	if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
		tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_SMALLFIELD);
	else
		tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_LARGEFIELD);

	CFileFind finder;
	if (!finder.FindFile(tempFile))
		return;

	const int	nIndex = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_GETAFIELD);
	if (nIndex < 0) return;

	int	readL = 0;
	char	readB[1024]{};
	CString	sField, sNewField, sFieldName, sFieldNo;

	sFieldNo.Format("%02d", nIndex);

	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
		readL  = GetPrivateProfileString("FIELDS", sFieldNo, "", readB, sizeof(readB), tempFile);
	else
		readL  = GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + "FIELDS", sFieldNo, "", readB, sizeof(readB), tempFile);

	sField = CString(readB, readL);
	const int	index = sField.Find(deliMITER);
	if (index == -1)	return;

	sFieldName = sField.Left(index);
	sField.Format("%s", m_szFIELD);
	sField.Replace(",", deliMITER);
	sNewField.Format("%s%s%s", sFieldName, deliMITER, sField);

	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
		WritePrivateProfileString("FIELDS", sFieldNo, sNewField, tempFile);
	else
		WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + "FIELDS", sFieldNo, sNewField, tempFile);
}

bool CGridWnd::IsDuplicateCode(CString code, int nrow, bool drag )
{
	CString string;
	const int nMax = m_grid->GetRowCount();

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
	const int codeL = code.GetLength();
	if (codeL <= 0) return 0;

	const int result = m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);

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

	const int userBASE = 100;
	const int subBASE =  200;
	const int menuBASE = 300;

	m_grid->SetSelectRow(row);


	CPoint	point;
	GetCursorPos(&point);

	int	index = -1, offs{};
	char* ptr{}, wb[1024]{};
	CStringArray items; items.RemoveAll();
	CString string, path, domino, mapN = _T("IB202200");

	CString	code;
	int itype{};

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

		activeTrigger(code);
		//Variant(triggerCC, code);
		m_grid->SetFocus();

		const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int ret = pWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);

		const int	dataidx = CAST_TREEID(m_kind)->kind;

		if (dataidx != xISSUE && ret == 0)
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

// 			dlg.AddMenu(userBASE+9, "책갈피 추가");
//			dlg.AddMenu(userBASE+10, "책갈피 삭제");
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
		code = m_grid->GetItemText(row, colCODE);  //test menupop
		code.TrimRight();

		itype = GetCodeType(code);

		auto popM = std::make_unique<CMenu>();
		popM->CreatePopupMenu();

		path.Format("%s/%s/%s", m_root, TABDIR, "intermenu.txt");
		switch (itype)
		{
		case thirdCODE:  //110  K-OTC
		case kospiCODE: //100  코스피 
			mapN = "IB202200_100";
			break;
		case futureCODE:  //101 지수선물
			mapN = "IB202200_101";
			break;
		case optionCODE: //102 지수옵션
			mapN = "IB202200_102";
			break;
		case indexCODE:   //104 업종지수
			mapN = "IB202200_104";
			break;
		case elwCODE:   //120  ELW
			mapN = "IB202200_120";
			break;
		case sfCODE:   //122 주식선물
			mapN = "IB202200_SF";
			break;
		case 0:
			if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물//파생상품 코드개편
			{
				int nGubn = atoi(code.Mid(1, 2));
				CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ 주식선물 신규종목 연동

				if (nGubn > 60 && nGubn <= 70) //금리선물
				{
					mapN = "IB202200_CF";
					break;
				}
				else if (nGubn > 70 && nGubn < 80) //통화선물
				{
					mapN = "IB202200_CF";
					break;
				}
				else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
					|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
					|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z"))
				{
					mapN = "IB202200_CF";

					// m_slog.Format("[cx_interest][SF][%s]<%d> code=[%s]", __FUNCTION__, __LINE__, code);
					// OutputDebugString(m_slog);

					break;
				}
			}
		}

		DWORD	rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		if (rc <= 0)
		{
			mapN = "IB202200";
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


		auto subMenu = std::make_unique<CAxMenu>();
		subMenu->CreatePopupMenu();
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 1, "1");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 2, "2");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 3, "3");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 4, "4");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 5, "5");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 6, "6");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 7, "7");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 8, "8");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 9, "9");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 10, "10");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 11, "11");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 12, "12");


		code = m_grid->GetItemText(row, colCODE); code.TrimRight();
		const int	dataidx = CAST_TREEID(m_kind)->kind;

		const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		//const int ret = pWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
		const int ret = m_kind;
		if (dataidx != xISSUE  && ret == 0)
		{
			if(((CMainWnd*)m_pMainWnd)->m_bRemain == FALSE && nOver == MO_SELECT)
			{
				popM->AppendMenu(MF_STRING, userBASE+0, "종목 추가<INSERT>");
				popM->AppendMenu(MF_STRING, userBASE+1, "종목 삭제<DELETE>");
				popM->AppendMenu(MF_STRING, userBASE+2, "종목 변경");
			}

			if (!code.IsEmpty())
			{
				popM->AppendMenu(MF_STRING, userBASE+3, "메모 입력");
				popM->AppendMenu(MF_STRING, userBASE+11, "메모 삭제");

				popM->InsertMenu(userBASE + 12, MF_POPUP | MF_BYPOSITION, (UINT_PTR)subMenu->GetSafeHmenu(), _T("북마크 지정"));
				popM->AppendMenu(MF_STRING, userBASE+13, "북마크 삭제");
			}
			// MODIFY PSH 20070914
//			popM->AppendMenu(MF_STRING, userBASE+9, "빈줄추가");

  			popM->AppendMenu(MF_STRING, userBASE+15, "책갈피 추가");
 			popM->AppendMenu(MF_STRING, userBASE+16, "책갈피 삭제");
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

		index = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, point.x, point.y, this);
		popM->DestroyMenu();
		subMenu->DestroyMenu();
	}



	domino = _T("");
	if (!code.IsEmpty())
	{
		switch (itype)
		{
		case kospiCODE:		domino.Format("%s\t%s", HCOD, code);	break;
		case futureCODE:	domino.Format("%s\t%s", FCOD, code);	break;
		case optionCODE:	domino.Format("%s\t%s", OCOD, code);	break;
		case foptionCODE:	domino.Format("%s\t%s", PCOD, code);	break;
		case indexCODE:		domino.Format("%s\t%s", UCOD, code);	break;
		case elwCODE:		domino.Format("%s\t%s", HCOD, code);	break;
		case 0:		//2012.10.11 KSJ 주식, 금리, 통화선물 추가
			if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물 파생상품 코드개편
			{
				int nGubn = atoi(code.Mid(1,2));
				CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ 주식선물 신규종목 연동

				if(nGubn > 10 && nGubn < 60)	//주식선물
				{
					string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
					Variant(triggerCC, string);

					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if(nGubn > 60 && nGubn <= 70)	//금리선물
				{
					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if(nGubn > 70 && nGubn < 80)	//통화선물
				{
					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
					|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
					|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z"))
				{
					string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
					Variant(triggerCC, string);

					// m_slog.Format("[cx_interest][SF][%s]<%d> code=[%s] string=[%s]", __FUNCTION__, __LINE__, code, string);
					// OutputDebugString(m_slog);

					string.Format("%s\t%s", SFOD, code);
					break;
				}
			}
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
			if(((CMainWnd*)m_pMainWnd)->m_bRemain == TRUE)
			{
				return;
			}

			if (CAST_TREEID(m_kind)->kind == xINTEREST)
				m_pGroupWnd->SendMessage(WM_MANAGE,MK_EDITWORK,TRUE);

			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
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

			m_recomArray.InsertAt(row-1,"");

			insertInterest(row - 1);
			insertRow(row);
			SetLineColor();
			m_grid->getmemoCheck();
			//			m_grid->memoCheck(row+1);
			m_grid->memoRefresh();
			m_grid->SetFocusCellEdit(row, colNAME, true);

			if (m_bAddCnd)
			{
				saveInterest(true);
			}
		}

		break;
	case userBASE+1:	// 삭제
		{
			if(((CMainWnd*)m_pMainWnd)->m_bRemain == TRUE)
			{
				return;
			}

			const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

			m_pGroupWnd->SendMessage(WM_MANAGE,MK_EDITWORK,TRUE);
			if (gsl::narrow_cast<int>(m_inters.size()) >= row -1)
			{
				if (!(m_grid->GetItemAttr(row, colNAME) & GVAT_MARKER))
				{
					m_selectRow = row;		//선택한 열을 저장
					m_nSelectedRow = m_selectRow;

					DeleteRow(row);
					insertInterest(MAX_LINE - 1);
					saveInterest(true);
					if(nOver == MO_VISIBLE)
					{
						//pWnd->SendMessage(WM_MANAGE, MK_SETPAGEVIEW, pageNumber);
						pWnd->SendMessage(WM_MANAGE, MK_RELOADLIST);
					}
					else
					{
//						pWnd->SendMessage(WM_MANAGE, MK_SENDTREE);
						m_grid->SetSelectRow(m_nSelectedRow);
					}
				}
				// END MODIFY
			}
		}
		break;
	case userBASE+2:
		if(((CMainWnd*)m_pMainWnd)->m_bRemain == TRUE)
		{
			return;
		}

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
	case userBASE+9://빈줄 추가
		{
			if(((CMainWnd*)m_pMainWnd)->m_bRemain == TRUE)
			{
				return;
			}

			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
			//int pageNumber = pWnd->SendMessage(WM_MANAGE, MK_GETVIEWPAGE);
			int rowcount = 0;

			if(nOver == MO_VISIBLE)
			{
				rowcount = GetRowcountVisibleMode();
			}
			else
			{
				rowcount = m_rowC - 1;
			}

			if(rowcount >= MAX_LINE)
			{

				Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
				return;
			}

			m_selectRow = row;		//선택한 열을 저장

			m_pGroupWnd->SendMessage(WM_MANAGE,MK_EDITWORK,TRUE);

			m_recomArray.InsertAt(row-1,"");
			insertInterest(row - 1);
			insertRow(row);
			SetLineColor();
			m_grid->getmemoCheck();
			m_grid->memoRefresh();

			saveInterest(true);

			pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			if(nOver == MO_VISIBLE)
			{
				//pWnd->SendMessage(WM_MANAGE, MK_SETPAGEVIEW, pageNumber);
				pWnd->SendMessage(WM_MANAGE, MK_RELOADLIST);
			}
			else
			{
//				pWnd->SendMessage(WM_MANAGE, MK_SENDTREE);
			}
		}
		break;
	case userBASE+15:// 책갈피 추가
		{
			m_pGroupWnd->SendMessage(WM_MANAGE,MK_EDITWORK,TRUE);

			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
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

			m_recomArray.InsertAt(row-1,"");
			insertInterest(row - 1);
			insertRow(row);
			SetLineColor();
			m_grid->getmemoCheck();
//			m_grid->memoCheck(row+1);
			m_grid->memoRefresh();

			const UINT attr = m_grid->GetItemAttr(row, colNAME) | GVAT_MARKER;
			m_grid->SetItemAttr(row, colNAME, attr);
			auto& pInter = m_inters.at(row - 1);

			CString strMarker;
			strMarker.Format("m%05d", row - 1);

			m_pGridMarker.Add(strMarker);

			if (pInter)
			{
				pInter.get()->gubn = ROW_COMMENT;
				pInter.get()->code = strMarker;
			}

			m_grid->SetItemText(row, colCODE, strMarker);
			m_grid->SetFocusCellEdit(row, colNAME, true);

			if (m_bAddCnd)
			{
				saveInterest(true);
			}
		}
		break;
	case userBASE+16:	// 책갈피삭제
		{
			m_pGroupWnd->SendMessage(WM_MANAGE,MK_EDITWORK,TRUE);
			if (gsl::narrow_cast<int>(m_inters.size()) >= row -1)
			{
				if (m_grid->GetItemAttr(row, colNAME) & GVAT_MARKER)
				{
					DeleteRow(row);
					insertInterest(MAX_LINE - 1);
					saveInterest(true);
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
		const UINT uAttr = m_grid->GetItemAttr(row, colNAME);
			m_grid->SetItemAttr(m_drop, colNAME, uAttr);

			if (GVAT_MARKER & uAttr)
			{

			}
			else
			{
				//setBookmark(row);
			}
		}
		break;

	case userBASE+13:	//북마크 삭제
		{
		const UINT uAttr = m_grid->GetItemAttr(row, colNAME);
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

	case subBASE + 1:
	case subBASE + 2:
	case subBASE + 3:
	case subBASE + 4:
	case subBASE + 5:
	case subBASE + 6:
	case subBASE + 7:
	case subBASE + 8:
	case subBASE + 9:
	case subBASE + 10:
	case subBASE + 11:
	case subBASE + 12:
		if (index > subBASE)
		{
			m_bEditWork = true;
			setBookmark(row, AxStd::FORMAT(_T("%02d"), index - subBASE));
		}
	break;	
	default:
		break;
	}
}

void CGridWnd::RecoverMarker()
{
	CString string;
	int nCnt = 0;

	const int nMax = m_grid->GetRowCount();

	for (int ii = 1; ii < nMax/*m_rowC*/; ii++)
	{
		string = m_grid->GetItemText(ii, colCODE);

		if(string == "")
			continue;

		if(string.GetAt(0) == 'm')
		{
			for(int i=0;i<m_pGridMarker.GetSize();i++)
			{
				CString strMarker = m_pGridMarker.GetAt(i);

				if(string == strMarker)
				{
					DeleteRow(ii);
					insertInterest(MAX_LINE - 1);

					nCnt++;

					break;
				}
			}
		}

		if(nCnt == m_pGridMarker.GetSize())
			break;
	}

	if(m_pGridMarker.GetSize() > 0)
	{
		saveInterest();

		m_pGridMarker.RemoveAll();
	}
}


void CGridWnd::InsertEmpty(int row)
{
/*	_inters*	pinters = m_inters.GetAt(m_inters.GetUpperBound());
	if (strlen(pinters->code) > 0)
	{*/
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	int rowcount = 0;

	if(nOver == MO_VISIBLE)
	{
		rowcount = GetRowcountVisibleMode();
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
	double	curr{}, open{}, high{}, low{};
	double	pcurr{}, popen{}, phigh{}, plow{}, dval{};

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
	//2011.11.09 KSJ
	//GetRowCount가 2이면 실제 Row가 하나 남은 것이므로 지우면 안됨.
	//지우면 Insert가 되지 않음.
	//row가 0이면 (티커)임..
	if(m_grid->GetRowCount() <= 2 || row <= 0)
		return;
	/*	2012.12.18 KSJ 수정전
	m_recomArray.RemoveAt(row-1);	//2012.05.11 KSJ 추천종목도 같이 삭제되어야함

	BOOL b = m_grid->DeleteRow(row);
	m_grid->memoDelete(row);
	m_rowC--;
	RemoveInters(row - 1);
	*/

	//2012.12.18 KSJ 정렬을 한상태에서 삭제하면 해당하는 row와 m_inters의 값이 맞지 않다.
	//m_inters의 값은 baseSorting 기준으로 되어 있기 때문임.
	//그래서 코드를 가지고 위치를 찾아야 한다.
	CString strCode;
	strCode = m_grid->GetItemText(row, colCODE);

	if(row <= m_recomArray.GetSize())	//2013.06.11 KSJ MAX_LINE을 삭제할때 사이즈 체크를 함.
		m_recomArray.RemoveAt(row-1);	//2012.05.11 KSJ 추천종목도 같이 삭제되어야함

	m_grid->DeleteRow(row);
	m_grid->memoDelete(row);
	m_rowC--;

	//2013.01.28 KSJ 코드를 찾아서 지우면 빈줄일때는 지워지지 않는다.
	//그래서 코드가 빈값일때는 row -1을 지운다.
	strCode.TrimLeft(); strCode.TrimRight();
	if(!strCode.IsEmpty())
		RemoveInters(GetInter(strCode)); //RemoveInters(row -1);
	else
		RemoveInters(row -1);
	//2013.01.28 KSJ End
	//2012.12.18 KSJ End

	ReIndexing();

	// Map 초기화 KSI
	ClearSearchMap();
	ReSetSearchMap();	//2011.12.29 KSJ
}

void CGridWnd::insertInterest(int idx)
{
	m_inters.emplace(m_inters.begin() + idx, std::make_shared<_intersx>());
}

void CGridWnd::initialGrid(bool size)
{
	if (m_nIndex == 0)
	{
		CintGrid::m_dropdata.Reset();
	}

	m_grid->Initial(m_rowC, m_colC, 1, 0);//colCURR + 1);//BUFFET SCROLL BAR

	// ADD PSH 20070911
	m_grid->SetMarginRatio(FALSE);
	m_grid->SetBkMarkerColor(m_clrMarkerBKG);
	m_grid->SetMarkerColor(m_clrMarkerTXT);
	m_grid->SetMarkerShadow(m_bShadow);
	m_grid->SetInfo(m_bInfo);
	m_grid->m_iTime = ((CMainWnd*)m_pMainWnd)->m_iTime;
//	m_grid->SetGridLine(GVLN_VERT);//BUFFET LINE
//	m_grid->SetStepColor(1, GetAxColor(68), GetAxColor(77));//BUFF
////////////////////////////////////////////////////
	const char use = ((CGroupWnd*)m_pGroupWnd)->GetBkUse();
	if ( use == '1' || use == '2' )
	{
		int nLine = ((CGroupWnd*)m_pGroupWnd)->GetLine();
		const COLORREF bkColor = ((CGroupWnd*)m_pGroupWnd)->GetBkColor1();
		const COLORREF bkCol2  = ((CGroupWnd*)m_pGroupWnd)->GetBkColor2();

		if ( nLine <= 0 ) nLine = 1;
		m_grid->SetStepColor(nLine, bkColor, bkCol2);//BUFF
	}else m_grid->SetStepColor(1, GetAxColor(68), GetAxColor(77));//BUFF

///////////////////////////////////////////////////////////
// 	m_pTicker->Initial(1, m_colC, 0, colCURR + 1);
// 	m_pTicker->SetHeader(FALSE);
// 	m_pTicker->SetEnableSelect(FALSE);
// 	m_pTicker->SetMarginRatio(m_bMargin);
// 	m_pTicker->SetInfo(m_bInfo);

	m_nRowHeight = m_grid->GetRowHeight(0);
	// END ADD

	LOGFONT lf{}, lfB{};
	const int	fontsize = ((CGroupWnd*)m_pGroupWnd)->GetFontSize();
	CString	fontname = ((CGroupWnd*)m_pGroupWnd)->GetFontName();
	// MODIFY PSH 20070917
	//BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	const BOOL	bCurrBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	const BOOL	bNameBold = ((CGroupWnd*)m_pGroupWnd)->GetNameBold();
	const BOOL	bAllBold = ((CGroupWnd*)m_pGroupWnd)->GetAllBold();
	// END MODIFY

	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);

	m_pFont->GetLogFont(&lf); lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	m_pFontB->GetLogFont(&lfB); lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	GVITEM	gvitem;
	_gridHdr xgridHdr;

 	// column header setting
	for (int ii = 0; ii < m_colC; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row   = 0;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		gvitem.format= GVFM_HEADER;
		gvitem.fgcol = GetAxColor(76);
		gvitem.bkcol = GetAxColor(74);

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		gvitem.text  = IH::idTOstring(xgridHdr.stid);

		if(gvitem.text.Find("외인소진") > -1)
		{
			gvitem.text = "외 인";
		}

		gvitem.attr  = xgridHdr.attr;

		m_grid->SetItem(&gvitem);
		if (ii == colCODE)
		{
			m_grid->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);
			//m_pTicker->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);
		}
		else if (ii == colSIG)
		{
//			m_grid->SetColumnWidth(ii, ((CGroupWnd*)GetParent())->GetRowHeight());
			m_grid->SetColumnWidth(ii, 0);
		}
		else
		{
			if (size)
				m_grid->SetColumnWidth(ii, xgridHdr.width);
		}
	}

	for (int ii = 0; ii < m_colC; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= xgridHdr.format;
		gvitem.symbol= xgridHdr.symbol;
		gvitem.attr  = xgridHdr.attrx;

		gvitem.fgcol = GetAxColor(69);
		// MODIFY PSH 20070912
		//gvitem.bkcol = GetAxColor(68);
		gvitem.bkcol = ((CGroupWnd*)m_pGroupWnd)->GetRTMColor();
		// END MODIFY

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		// MODIFY PSH 20070911
		if ((ii == colCODE) || (ii == colNAME) || (ii == colCURR) || bAllBold)
		// END MODIFY
		{	// 20070706 kwon
			if(gvitem.symbol.Compare("2024"))	//2012.11.29 KSJ 대비일때는 GVAT_CONDITIONx하면 색지정이 안됨
				gvitem.attr |= GVAT_CONDITIONx;

			// MODIFY PSH 20070917
			//if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold)  || bAllBold)
			// END MODIFY
			{
				//gvitem.attr &= ~GVAT_CONDITIONx;
				CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
			}
//			else
//				gvitem.attr |= GVAT_CONDITIONx;
		}

		// ADD PSH 20070911
		//m_pTicker->SetColumnItems(ii, &gvitem);

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
			//m_pTicker->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);

			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}
		else if (ii == colSIG)
		{
			m_grid->SetColumnWidth(ii, ((CGroupWnd*)GetParent())->GetRowHeight());

			// ADD PSH 20070911
			//m_pTicker->SetColumnWidth(ii, ((CGroupWnd*)GetParent())->GetRowHeight());
			// END ADD
		}
		else
		{
			if (size)
			{
				m_grid->SetColumnWidth(ii, xgridHdr.width);

				// ADD PSH 20070911
				//m_pTicker->SetColumnWidth(ii, gridHdr.width);
				// END ADD
			}
		}
	}

	// ADD PSH 20070911
	//SetLineColor(0, m_pTicker);
// 	m_pTicker->SetBkColor(((CGroupWnd*)m_pGroupWnd)->GetRTMColor());
// 	m_pTicker->SetBKSelColor(((CGroupWnd*)m_pGroupWnd)->GetRTMColor());
// 	m_pTicker->SetRowColResize(FALSE, TRUE);
// 	m_pTicker->SetKeepingColumn(colCURR +1);		// keep column count
// 	m_pTicker->SetGridFocusLine(FALSE, TRUE);
// 	m_pTicker->SetGridColor(GetAxColor(65));
// 	m_pTicker->SetConditionColor(GetAxColor(94), GetAxColor(95), GetAxColor(69));
// 	//m_pTicker->SetTitleTips(TRUE);
// 	m_pTicker->Adjust();
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
	//m_grid->SetTitleTips(TRUE);
//	m_grid->SetUseOLE(TRUE);	// Ctrl + C Ctrl + V사용을 위한 옵션 잠시 보류
	m_grid->Adjust();

// 	m_grid->SetColumnWidth(colSIG, 0);
// 	m_grid->SetColumnWidth(colRATE, 0);
}


void CGridWnd::ChangeField(bool type)
{
	m_iFieldType = type;

	if(m_iFieldType == 1)
	{
		m_grid->SetColumnWidth(colRATE, 60);
		m_grid->SetColumnWidth(colMAR, 0);
	}
	else
	{
		m_grid->SetColumnWidth(colRATE, 0);
		m_grid->SetColumnWidth(colMAR, 60);
	}


	m_grid->Invalidate(FALSE);
}

// ADD PSH 20070912
// void CGridWnd::InitPopupMenu()
// {
// 	m_menuSubHeader.CreatePopupMenu();
// 	m_menuSubHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND1, "항목추가시 자동저장");
// 	m_menuSubHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND2, "항목변경시 자동저장");
//
// 	m_menuHeader.CreatePopupMenu();
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INTERSAVE, "관심종목 등록하기");
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INTERTAKE, "관심종목 덮어쓰기");
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_BASESORT, "등록순으로 정렬");
// 	m_menuHeader.AppendMenu(MF_SEPARATOR);
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MULTICHART, "선택종목 멀티차트 보기");
// 	m_menuHeader.AppendMenu(MF_SEPARATOR);
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_SHOWRTSWND, "체결리스트 보기");
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_CODE,   "종목코드 보기");
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INFO,	  "종목특이사항 보기");
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MARGIN,   "주식증거금 보기");
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_TICKER,   "관심티커 보기");
// 	m_menuHeader.AppendMenu(MF_SEPARATOR);
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_TICKER_SETUP,   "관심티커 설정");
// 	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND1,		"편의기능 설정");
// 	//m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND1,   "자동저장 설정");
// 	//m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND2,   "알림소리 설정");
// 	//m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MARKET_COLOR,   "시장구분 설정");
// 	//m_menuHeader.InsertMenu(9, MF_POPUP|MF_BYPOSITION, (UINT)m_menuSubHeader.m_hMenu, "자동저장 설정");
// 	//m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MARKER,			"책갈피 설정");
//
// 	m_menuHeader.CheckMenuItem(IDC_MENU_SHOWRTSWND, m_bRTS ? MF_CHECKED : MF_UNCHECKED);
// 	m_menuHeader.CheckMenuItem(IDC_MENU_CODE, m_bDispCode ? MF_CHECKED : MF_UNCHECKED);
// 	m_menuHeader.CheckMenuItem(IDC_MENU_INFO, m_bInfo ? MF_CHECKED : MF_UNCHECKED);
// 	m_menuHeader.CheckMenuItem(IDC_MENU_MARGIN, m_bMargin ? MF_CHECKED : MF_UNCHECKED);
// 	m_menuHeader.CheckMenuItem(IDC_MENU_TICKER, m_bTicker ? MF_CHECKED : MF_UNCHECKED);
// 	//m_menuHeader.CheckMenuItem(IDC_MENU_SAVE_CND1, m_bAddCnd ? MF_CHECKED : MF_UNCHECKED);
// 	//m_menuHeader.CheckMenuItem(IDC_MENU_SAVE_CND2, m_bChgCnd ? MF_CHECKED : MF_UNCHECKED);
//
// 	m_menuHeader.SetSelectDisableMode(TRUE);
// }

// void CGridWnd::SetTicker(BOOL bTicker /* = TRUE */)
// {
// 	m_bTicker = bTicker;
//
// 	m_pTicker->ShowWindow(m_bTicker);
// 	CRect rect;
// 	m_btSAVE.GetWindowRect(&rect);
// 	if (m_bTicker)
// 	{
// 		m_btDomino.SetParent(m_pTicker);
// 		m_btSAVE.SetParent(m_pTicker);
// 		m_btCLOSE.SetParent(m_pTicker);
// 		//m_btSAVE.SetWindowPos(0, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW );
// 	}
// 	else
// 	{
// 		//m_btSAVE.SetParent(m_pGroupWnd);
// 		//m_btCLOSE.SetParent(m_pGroupWnd);
// 		m_btDomino.SetParent(this);
// 		m_btSAVE.SetParent(this);
// 		m_btCLOSE.SetParent(this);
// 		//m_btSAVE.SetWindowPos(0, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW );
// 	}
//
// 	m_btDomino.ShowWindow(SW_SHOW);
// 	m_btDomino.Invalidate();
// //	m_btSAVE.BringWindowToTop();
// 	m_btSAVE.ShowWindow(SW_SHOW);
// 	m_btSAVE.Invalidate();
// //	m_btCLOSE.BringWindowToTop();
// 	m_btCLOSE.ShowWindow(SW_SHOW);
// 	m_btCLOSE.Invalidate();
//
// 	CRect rcWnd;
// 	GetWindowRect(rcWnd);
// 	ScreenToClient(rcWnd);
//
// 	InvalidateRect(rcWnd);
// }

void CGridWnd::SetInfo(BOOL bInfo /* = TRUE */)
{
	m_bInfo = bInfo;

	//m_pTicker->ShowWindow(m_bInfo);
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

	int nResult{};

	if (m_bPopupDlg)
	{
		CPopupDlg dlg;
		dlg.m_nX = nX;
		dlg.m_nY = nY;

		const int nResultID[] = {
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

		const int nBase = 100;

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
	case IDC_MENU_TICKER_SETUP:
		{
			Csetup dlg(nullptr, &m_tkConfig);
			dlg.DoModal();
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
	return; //test 20230208 우선 소트 원복 기능은 막아둔다
	CMap <CString, LPCTSTR, int, int> mapBaseSort, mapCurCode;
	CString strIndex, strCode;

	const int nCnt = gsl::narrow_cast<int>(m_arrBaseInters.size());
	if (1 > nCnt) return;
	if( !GetCodeMap(mapCurCode)) return;


//inters* pInter = nullptr;
	for (int i = 0; i < nCnt; i++)
	{
		auto& pInter = m_arrBaseInters.at(i);
		strCode = pInter->code;
		strCode.Trim();
		if (!strCode.IsEmpty())
			mapBaseSort.SetAt(strCode, i);
	}

	m_grid->SortBase(mapBaseSort, mapCurCode);
	m_bSorting = FALSE;
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

void CGridWnd::SetLineColor(int nRow)
{
	COLORREF	bkColor{};
	if (m_grid)
		m_grid->SetRowHeight(nRow, ((CGroupWnd*)m_pGroupWnd)->GetRowHeight());

	if (!nRow)	return;

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
// END MODIFY

//북마크 색 지정하기
void CGridWnd::setBookmark(int row, CString indexrgb)
{
	const int rowinter = row -1;
	const int count = m_inters.size();
	if (rowinter > count)
		return;

	auto&pInter = m_inters.at(rowinter);
	pInter.get()->bookmark = '1';  
	pInter.get()->bookrgb = indexrgb;
	pInter.get()->gubn = ROW_BOOKMARK;
	
	m_nBookMarkRow = rowinter;
//	saveInterest(false, -1, true);

	m_grid->Adjust();
}


void CGridWnd::deleteBookmark(int row)
{
	const int rowinter = row - 1;
	const int count = m_inters.size();

	if (rowinter > count)
		return;

	//북마크 지정
	auto& pInter = m_inters.at(row - 1);
	pInter.get()->bookrgb = "00";
	pInter.get()->bookmark = '0';  
	m_nBookMarkRow = row - 1;
	//saveInterest(false, -1, true);

	//sendTransaction(row - 1);
	m_grid->Adjust();
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

	const int ret = m_grid->InsertRow("", afterRow);
	auto& pInter = m_inters.at(beforeRow);

	//	if (bInsertInter)
	//		insertInterest(row - 1);

	LOGFONT lf{}, lfB{};
	const int fontsize = ((CGroupWnd*)m_pGroupWnd)->GetFontSize();
	CString	fontname = ((CGroupWnd*)m_pGroupWnd)->GetFontName();

	const BOOL	bCurrBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	const BOOL	bNameBold = ((CGroupWnd*)m_pGroupWnd)->GetNameBold();
	const BOOL	bAllBold = ((CGroupWnd*)m_pGroupWnd)->GetAllBold();

	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);

	m_pFont->GetLogFont(&lf);
	lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	m_pFontB->GetLogFont(&lfB);
	lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	GVITEM	gvitem;
	_gridHdr xgridHdr;
	for (int ii = 0; ii < m_colC; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row   = afterRow;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= xgridHdr.format;
		gvitem.symbol= xgridHdr.symbol;
		gvitem.attr  = xgridHdr.attrx;
		gvitem.fgcol = GetAxColor(69);

		//북마크되어있는지 확인
		if(pInter.get()->bookmark == '1')
		{
			gvitem.bkcol = ((CGroupWnd*)m_pGroupWnd)->GetBookMarkColor();
		}
		else
		{
			gvitem.bkcol = GetAxColor(68);
		}


		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		if ((ii == colCODE) || (ii == colCURR) || (ii == colNAME) || bAllBold)
		{	// 20070706 kwon
			if(gvitem.symbol.Compare("2024"))	//2012.11.29 KSJ 대비일때는 GVAT_CONDITIONx하면 색지정이 안됨
				gvitem.attr |= GVAT_CONDITIONx;

			// MODIFY PSH 20070917
			//if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
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

	// Map 초기화 KSI
	ClearSearchMap();
	ReSetSearchMap();	//2011.12.29 KSJ

	return ret;
}

int CGridWnd::insertRow(int row, BOOL bInsertInter /* = TRUE */, BOOL bLineAdd)
{
	//2011.11.09 KSJ
	//row가 0이면 (티커)임..
	if(row <= 0)
		return 0;

	if (row > m_grid->GetRowCount())
		row = m_grid->GetRowCount();

	// ADD PSH 20070914
	m_rowC++;
	// END ADD

	LOGFONT lf{}, lfB{};
	const int	fontsize = ((CGroupWnd*)m_pGroupWnd)->GetFontSize();
	CString	fontname = ((CGroupWnd*)m_pGroupWnd)->GetFontName();

	const BOOL	bCurrBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	const BOOL	bNameBold = ((CGroupWnd*)m_pGroupWnd)->GetNameBold();
	const BOOL	bAllBold = ((CGroupWnd*)m_pGroupWnd)->GetAllBold();

	m_pFont = GetAxFont(fontname, fontsize);
	m_pFontB = GetAxFont(fontname, fontsize, true);

	m_pFont->GetLogFont(&lf); lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	m_pFontB->GetLogFont(&lfB); lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);


	int ret{};

	if(bLineAdd == FALSE)
	{
		ret = row;
	}
	else
	{
		ret = m_grid->InsertRow("", row, bInsertInter);
	}

	if(row >= gsl::narrow_cast<int>(m_inters.size()))
	{
		return 0;
	}

	auto& pInter = m_inters.at(row);

	GVITEM	gvitem;
	_gridHdr xgridHdr;
	for (int ii = 0; ii < m_colC; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row   = row;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_SYMBOL;

		gvitem.format= xgridHdr.format;
		gvitem.symbol= xgridHdr.symbol;
		gvitem.attr  = xgridHdr.attrx & ~GVAT_MARKER;
		gvitem.fgcol = GetAxColor(69);

		//북마크되어있는지 확인
		if(pInter.get()->bookmark == '1')
		{
			gvitem.bkcol = ((CGroupWnd*)m_pGroupWnd)->GetBookMarkColor();
		}
		else
		{
			gvitem.bkcol = GetAxColor(68);
		}

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		if ((ii == colCODE) || (ii == colCURR) || (ii == colNAME) || bAllBold)
		{
			if(gvitem.symbol.Compare("2024"))	//2012.11.29 KSJ 대비일때는 GVAT_CONDITIONx하면 색지정이 안됨
				gvitem.attr |= GVAT_CONDITIONx;

			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
			{
				CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
			}
		}
		if (ii == colNAME)
		{
			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}

		m_grid->SetItem(&gvitem);
	}

	m_grid->SetGridFocusLine(FALSE, TRUE);

	// Map 초기화 KSI
	ClearSearchMap();
	ReSetSearchMap();	//2011.12.29 KSJ

	return ret;
}



void CGridWnd::parsingField(CString symbol, bool bDlgSetup/* = false*/)
{
	CString	string;
	const int gridHdrC = sizeof(gridHdr) / sizeof(_gridHdr);
	m_gridHdrX.RemoveAll();

	char	buf[1024];

	memset(buf, 0x00, sizeof(buf));

	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
	{
		GetPrivateProfileString(m_section, KEY_WIDTH, "", buf, sizeof(buf), m_fileCFG);
	}
	else
	{
		GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH, "", buf, sizeof(buf), m_fileCFG);
	}

	m_szWIDTH.Format("%s",buf);

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

	if(((CMainWnd*)m_pMainWnd)->m_param.options == "WIDE")
	{
		if(m_fileCFG.Find("intercfg3.ini") > -1)
		{
			sztmp.Format("%s", m_MultiViewCols);
		}
		else
		{
			sztmp.Format("%s", m_szWIDTH);
		}
	}
	else
	{
		sztmp.Format("%s", m_szWIDTH);
	}

	while (sztmp.GetLength())
	{
		string = IH::Parser(sztmp, ",");

		if (string.IsEmpty())
			continue;

		const int nColWidth = atoi(string);

		arWidth.Add(nColWidth);

		//nTotalCols += nColWidth;
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

			if (gridHdr[ii].needs == 13)
				m_recomBasicCostField = nIndex;

			if (gridHdr[ii].needs == 14)
				m_recomSuikrateField = nIndex;

			/* 2011.12.22 KSJ
			if (gridHdr[ii].needs == 15)
				m_recomBMjisuField = nIndex;

			if (gridHdr[ii].needs == 16)
				m_recomBMSuikrateField = nIndex;
			KSJ */

			if (gridHdr[ii].needs == 17)
				m_upjongCode = nIndex;

			if (gridHdr[ii].needs == 18)
				m_recomJisuGubnField = nIndex;
			break;
		}
	}


	if (bField)
	{
		ncnt = m_gridHdrX.GetSize();
		wcnt = arWidth.GetSize();

		CString strSymbol;
// 		if ((ncnt - colNAME) == wcnt)
// 		{
// 			for ( ii = 0 ; ii < wcnt ; ii++ )
// 			{
// 				gHdr = m_gridHdrX.GetAt(ii + colNAME);
// 				gHdr.width = arWidth.GetAt(ii);
// 				m_gridHdrX.SetAt(ii + colNAME, gHdr);
// 			}
// 		}

		if ((ncnt - colNAME) == wcnt)
		{
			for (int ii = 0 ; ii < wcnt ; ii++ )
			{
				if(ii + colNAME >= m_gridHdrX.GetSize())
					continue;
				gHdr = m_gridHdrX.GetAt(ii + colNAME);

// 				//2013.07.03 KSJ 7852 ~ 7855 일때 칼럼표시 안해주기 위해서..
// 				strSymbol = CString(gHdr.symbol, strlen(gHdr.symbol));
//
// 				if(!strSymbol.Compare("7852") || !strSymbol.Compare("7853") || !strSymbol.Compare("7854") || !strSymbol.Compare("7855"))
// 					continue;

				gHdr.width = arWidth.GetAt(ii);
				m_gridHdrX.SetAt(ii + colNAME, gHdr);
			}
		}
		else if((ncnt - colNAME) > wcnt)
		{
// 			int jj = 0;
			for (int ii = 0 ; ii < wcnt - 1 ; ii++ )
			{
				if(ii + colNAME >= m_gridHdrX.GetSize())
					continue;
				gHdr = m_gridHdrX.GetAt(ii + colNAME);

// 				//2013.07.03 KSJ 7852 ~ 7855 일때 칼럼표시 안해주기 위해서..
// 				strSymbol = CString(gHdr.symbol, strlen(gHdr.symbol));
//
// 				if(!strSymbol.Compare("7852") || !strSymbol.Compare("7853") || !strSymbol.Compare("7854") || !strSymbol.Compare("7855"))
// 					continue;

				gHdr.width = arWidth.GetAt(ii);
				m_gridHdrX.SetAt(ii + colNAME, gHdr);
			}
		}
		else if((ncnt - colNAME) < wcnt)
		{
			for (int ii = 0 ; ii <= (ncnt - colNAME) ; ii++ )
			{
				if(ii + colNAME >= m_gridHdrX.GetSize())
					continue;
				gHdr = m_gridHdrX.GetAt(ii + colNAME);

// 				//2013.07.03 KSJ 7852 ~ 7855 일때 칼럼표시 안해주기 위해서..
// 				strSymbol = CString(gHdr.symbol, strlen(gHdr.symbol));
//
// 				if(!strSymbol.Compare("7852") || !strSymbol.Compare("7853") || !strSymbol.Compare("7854") || !strSymbol.Compare("7855"))
// 					continue;
//
				gHdr.width = arWidth.GetAt(ii);
				m_gridHdrX.SetAt(ii + colNAME, gHdr);
			}
		}
	}
}

void CGridWnd::ResizeField()
{
	char	buf[1024];

	memset(buf, 0x00, sizeof(buf));

	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
	{
		GetPrivateProfileString(m_section, KEY_WIDTH, "", buf, sizeof(buf), m_fileCFG);
	}
	else
	{
		GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH, "", buf, sizeof(buf), m_fileCFG);
	}

	memset(buf, 0x00, sizeof(buf));

	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
	{
		GetPrivateProfileString(m_section, KEY_WIDTH2, "", buf, sizeof(buf), m_fileCFG);
	}
	else
	{
		GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + m_section, KEY_WIDTH2, "", buf, sizeof(buf), m_fileCFG);
	}

	struct _gridHdr	gHdr;

	m_szWIDTH.Format("%s",buf);
	m_MultiViewCols.Format("%s",buf);

	CString sztmp;
	CWordArray arWidth;
	CString string;

	if(((CMainWnd*)m_pMainWnd)->m_param.options == "WIDE")
	{
		if(m_fileCFG.Find("intercfg3.ini") > -1)
		{
			sztmp.Format("%s", m_MultiViewCols);
			while (sztmp.GetLength())
			{
				string = IH::Parser(sztmp, ",");

				if (string.IsEmpty())
					continue;

				arWidth.Add(atoi(string));
			}
		}
		else
		{
			sztmp.Format("%s", m_szWIDTH);
			while (sztmp.GetLength())
			{
				string = IH::Parser(sztmp, ",");

				if (string.IsEmpty())
					continue;

				arWidth.Add(atoi(string));
			}
		}
	}
	else
	{
		sztmp.Format("%s", m_szWIDTH);
		while (sztmp.GetLength())
		{
			string = IH::Parser(sztmp, ",");

			if (string.IsEmpty())
				continue;

			arWidth.Add(atoi(string));
		}
	}

	const int ncnt = m_gridHdrX.GetSize();
	const int wcnt = arWidth.GetSize();

	CString strSymbol;
	if ((ncnt - colNAME) == wcnt)
	{
		for ( int ii = 0 ; ii < wcnt ; ii++ )
		{
			gHdr = m_gridHdrX.GetAt(ii + colNAME);

			//2013.07.03 KSJ 7852 ~ 7855 일때 칼럼표시 안해주기 위해서..
			strSymbol = CString(gHdr.symbol, strlen(gHdr.symbol));

			if(!strSymbol.Compare("7852") || !strSymbol.Compare("7853") || !strSymbol.Compare("7854") || !strSymbol.Compare("7855"))
				continue;

			gHdr.width = arWidth.GetAt(ii);
			m_gridHdrX.SetAt(ii + colNAME, gHdr);
		}
	}

	initialGrid();
}


bool CGridWnd::loadField(int fno /* = -1*/, bool bDlgSetup/* = false*/, int iEqualizer)
{
	CString tempFile;

	if(((CMainWnd*)m_pMainWnd)->GetFileType() == SMALLTYPE_FILE)
	{
		tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_SMALLFIELD);
	}
	else
	{
		tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_LARGEFIELD);
	}

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
	char	readB[1024]{};
	CString	string, fieldno;

	fieldno.Format("%02d", fno);

	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
		readL  = GetPrivateProfileString("FIELDS", fieldno, "", readB, sizeof(readB), tempFile);
	else
		readL  = GetPrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + "FIELDS", fieldno, "", readB, sizeof(readB), tempFile);

	string = CString(readB, readL);

/*	2014.13.28 KSJ 체결이퀄라이저 만들다가 중지한 기능이므로 주석처리함.
	CString eq = "7778;";
*/
	if (((CMainWnd*)m_pMainWnd)->m_mapName.CompareNoCase("5000") == 0)
	{
		string.Replace("3146;", "");
		string.Replace("3181;", "");
	}

	if(iEqualizer == 0)	//일반
	{
		string += "2022;1915;";
	}

	string += "1918;";

// 	int index = string.Find(eq);

// 	if(index > 0)
// 	{
// 		m_pToolWnd->SendMessage(WM_MANAGE, MK_EQUALIZER, (LPARAM)1);
// 		m_grid->m_isEqualizerField = true;
// 	}
// 	else
// 	{
// 		m_pToolWnd->SendMessage(WM_MANAGE, MK_EQUALIZER, (LPARAM)0);
// 		m_grid->m_isEqualizerField = false;
// 	}

	parsingField(string, bDlgSetup);
	m_colC = m_gridHdrX.GetSize();

	if(((CMainWnd*)m_pMainWnd)->m_bOthers == FALSE)
		WritePrivateProfileString("FIELDS", "ACTIVE", fieldno, tempFile);
	else
		WritePrivateProfileString(((CMainWnd*)m_pMainWnd)->m_mapName + "FIELDS", "ACTIVE", fieldno, tempFile);

	return true;
}

void CGridWnd::parsingOubsUpjong(char* datB, int datL)
{
	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL) return;

	CString	string, stringx, entry;

	string = CString(&datB[gridHoL], datL - gridHoL);


	CString strCode;
	CString tempData, tempStr;

	if (!string.IsEmpty())
	{
		stringx = IH::Parser(string, PNEW);
		tempData = IH::Parser(stringx, PTAB);

		m_dKospi = atof(tempData);
		tempStr.Format("%.2f", m_dKospi);
		m_dKospi = atof(tempStr);

		stringx = IH::Parser(string, PNEW);
		tempData = IH::Parser(stringx, PTAB);
		m_dKosdaq = atof(tempData);
		tempStr.Format("%.2f", m_dKosdaq);
		m_dKosdaq = atof(tempStr);
	}
}

void CGridWnd::SetVScroll(int scroll)
{
	m_grid->SetGridScroll(scroll);
}

void CGridWnd::ClearGrid()
{
	m_grid->SetRedraw(TRUE);

	m_grid->Clear();
	m_grid->MessageToGrid(WM_VSCROLL, SB_TOP);

	m_recomArray.RemoveAll();

	m_grid->Invalidate();
}

void CGridWnd::parsingOubs(char* datB, int datL, int mode)
{
	//KSJ 장운영시간 저장
	CString strTime = ((CMainWnd*)m_pMainWnd)->GetMarketTime();
	CString strTemp;

	if(!strTime.IsEmpty())
	{
		strTime.Trim();

		strTime.Replace("+", "");
		strTime.Replace("-", "");
		strTime.Replace(" ", "");

		const char ch = 0x7e;

		m_strBeginTime = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
		m_strEndTime = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);
		// 장운영시간 저장
	}

	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL)
	{
		return;
	}

	m_grid->SetRedraw(FALSE);

	m_bContinue = FALSE;
	m_bSecond   = FALSE;

	if(!m_bContinue || (m_bContinue && !m_bSecond))
	{
		if (m_seldrop < 0 )
		{
			m_grid->Clear();
			m_grid->MessageToGrid(WM_VSCROLL, SB_TOP);
		}

		m_recomArray.RemoveAll();
		m_recomCodeToData.RemoveAll();
	}

	CString	string, stringx, entry, strField;
	CString	bongdata = _T("");
	const BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	string = CString(&datB[gridHoL], datL - gridHoL);
	//TRACE("%s",string);
	CString str950, str951; //2012.06.19 KSJ 배분, 임의

	if(!m_bSecond)
		m_arExist.RemoveAll();

	const int	kind = CAST_TREEID(m_kind)->kind;
	LPARAM	lParam = 0;

	int nStart{}, nEnd{};

	nStart = 0;
	nEnd = m_gridHdrX.GetSize();
	if(m_bContinue)
	{
		if(m_bSecond)
		{
			// 			m_bSecond = FALSE;
			// 			m_bContinue = FALSE;

			nStart = GRIDMAXNUM;
			nEnd = m_gridHdrX.GetSize();
		}
		else
		{
			nStart = 0;
			nEnd = GRIDMAXNUM;
		}
	}

	// TRACE("START : %d END : %d\n",nStart,nEnd);

	if (kind == xISSUE)
	{
		lParam = MAKELPARAM(0, 1);
	}

	CString strCode, strMargin, strTodayVolumn, strData;
	CString tempData, recommand1, recommand2, recommand3, recommand4, recommand5;


	//string.Trim();  //test 20230209  빈줄이나 책갈피로 oop조회를 하면 빈값인데 이거 처리 해줘야 한다
	if (!string.IsEmpty())
	{
		int rowLength = -1;
		const int startPoint=0;
		//-1 : MO_SELECT 모드
		// 0, 1 : MO_VISIBLE모드
		if(mode == -1)
		{
		//	rowLength = gsl::narrow_cast<int>(m_inters.size());  
			rowLength = GetRowcount();
		}
		else if(mode == 0 || mode > 100)
		{
			rowLength = GetRowcount();
			//두번째 영역
			if(mode > 100)
			{
			//	rowLength = 100 - mode / 100;  //test 20230209
				for(int j=0 ; j < mode / 100 ; j++)
				{
					stringx = IH::Parser(string, PNEW);
				}
			}
		}

		for (int ii = 0; ii < rowLength ; ii++)
		{
			tempData = "";
			recommand1 = "";
			recommand2 = "";
			recommand3 = "";
			recommand4 = "";
			recommand5 = "";


			strTodayVolumn = "";
			stringx = IH::Parser(string, PNEW);
			strData = stringx;


			//2013.07.04 KSJ 필드가 GRIDMAXNUM(27)개를 넘으면 2번 나눠서 조회한다.
			//그렇기때문에 첫번재 데이터에서 27번째까지만 뽑아서 뒷데이터랑 붙이면
			//완전한데이터가 된다.
			if(m_bContinue && m_bSecond)
			{
				CString strValue;
				strCode = m_grid->GetItemText(ii + 1, colCODE);

				if(m_mapCurValue.Lookup(strCode, strValue))
				{
					int nIndex = 0, nCount = 0;

					while(nIndex != -1)
					{
						nIndex = strValue.Find("\t", nIndex+1);

						if(++nCount == GRIDMAXNUM)
						{
							strValue = strValue.Left(nIndex+1);
							break;
						}
					}

					strValue += strData;
					m_mapCurValue.SetAt(strCode, strValue);
				}
			}
			//2013.07.04 END
// CString sss = stringx;
// sss.Replace("\t", "/");
// AxStd::_Msg("receive column [%s]", sss);


			if (!stringx.IsEmpty())
			{
				CString tempOnebyte = stringx.Left(1);
				tempOnebyte.TrimLeft();

				int iHeadersymbol = 0;		//그리드 헤더심볼 값
				//TRACE("%s\n",stringx);
				//

				for (int jj = nStart; jj < nEnd; jj++)
				{
					entry = IH::Parser(stringx, PTAB);

// 					entry.TrimRight();
// 					entry.TrimLeft();
					//TRACE("jj : %d\t entry : %s\n", jj, entry);

					strField = entry;
					strField.TrimLeft();
					strField.TrimRight();

					if(strField == "0" || strField == "-0"|| strField == "0.00" || strField == "+0" || strField == "+0.00" || strField == "-0.00" || strField == ".00")	//2012.07.17 옵션일때의 경우 추가					{
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

						//2012.04.03 KSJ 처음 현재가와 데이터들을 저장.
						m_mapCurValue.SetAt(strCode, strData);
						// KSJ
					}

					const _gridHdr xgridHdr = m_gridHdrX.GetAt(jj);
					auto& pinters = m_inters.at(ii);
					const int nArrange = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);

					if (jj == colNAME)
					{
						UINT attr{};

						if (nArrange == 0 && pinters.get()->gubn == ROW_COMMENT)
						{
							entry.Format("%s", pinters.get()->name);

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
					else if (!entry.IsEmpty() && jj == colRATE && (xgridHdr.attr & GVAT_HIDDEN))
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

					if (!(xgridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
					{
						if ((strcmp("2029", xgridHdr.symbol) == 0) ||
							(strcmp("2030", xgridHdr.symbol) == 0) ||
							(strcmp("2031", xgridHdr.symbol) == 0) )
						{
							if ((1 == xgridHdr.needs) || (3 == xgridHdr.needs))
							{
								CString strVal  = entry;
								CString strDiff = strVal;

								if (strVal[0] == '+' || strVal[0] == '-')
								{
									strDiff = strVal.Mid(1);
								}

								double dDiffOpen = 0.0;
								double dPClose{}, dVal{};
								dVal	= atof(strDiff.GetBuffer(0));
								dPClose = atof( (m_grid->GetItemText(ii + 1, colPCURR)).GetBuffer(0) );

								if (dVal != 0 && dPClose != 0)
								{
									dDiffOpen = (dVal - dPClose) / dPClose * 100;

									if (1 == xgridHdr.needs)
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
									else if (3 == xgridHdr.needs)
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
					if(jj == m_upjongCode)
					{
						entry.TrimLeft();

						if(entry == "")
						{
							entry = "999";
						}

						m_grid->SetItemText(ii+1, jj, entry);
					}

					//2013.08.23 KSJ 스프레드종목이면 보합+, 하락+, 상승- 등 색과 기호를 표시해줘야한다.
					if(jj == colCURR)
					{
						if(strCode.GetLength() == 8 && (strCode.GetAt(0) == '4' || strCode.GetAt(0) == 'D'))
						{
							if (entry.GetAt(0) == ' ')	//처음이 빈칸이면 보합에 -, + 표시가 있는 것이다.
								entry.Replace(" ", "0");
						}
					}
					//2013.08.23 KSJ END

	 				m_grid->SetItemText(ii+1, jj, entry);

					//전일종가를 저장(2320심볼)colPCURR
					CString tempYvol;

					if(jj == colPCURR)
					{
						tempYvol = entry;
						tempYvol.Remove('+');
						tempYvol.Remove('-');
//						m_iYDayVol[ii][YDAYLASTVALUE] = tempYvol;
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

					if(m_recomJisuGubnField >= 0)
					{
						if(m_recomJisuGubnField == jj)
						{
							entry.TrimRight();
							entry.TrimLeft();
//							m_iYDayVol[ii][JISUGUBN] = entry;
						}
					}

					if(jj == colCURR)
					{
						iHeadersymbol = atoi(xgridHdr.symbol);
					}
				}


				tempData = IH::Parser(stringx, PTAB);
				tempData.TrimRight();
				tempData.TrimLeft();

				//전일거래량을 저장(2321심볼)
//				m_iYDayVol[ii][JMCODE] = strCode;
//				m_iYDayVol[ii][YDAYVOLUMN] = tempData;

				//현재가를 저장(2022심볼)
				tempData = IH::Parser(stringx, PTAB);
				tempData.TrimRight();
				tempData.TrimLeft();
				tempData.Remove('+');
				tempData.Remove('-');
//				m_iYDayVol[ii][TDAYVALUE] = tempData;

				//추천일자(1910심볼)
				recommand1 = IH::Parser(stringx, PTAB);
				recommand1.TrimRight();
				recommand1.TrimLeft();

				//추천종목 기준가(1911심볼)
				recommand2 = IH::Parser(stringx, PTAB);
				recommand2.TrimRight();
				recommand2.TrimLeft();

				//수익률(1912심볼)
// 				recommand3 = IH::Parser(stringx, P_TAB);
// 				recommand3.TrimRight();
// 				recommand3.TrimLeft();
//
// 				//추천종목 기준 BM지수(1913심볼)
// 				recommand4 = IH::Parser(stringx, P_TAB);
// 				recommand4.TrimRight();
// 				recommand4.TrimLeft();
//
// 				//BM수익률(1914심볼)
// 				recommand5 = IH::Parser(stringx, P_TAB);
// 				recommand5.TrimRight();
// 				recommand5.TrimLeft();

				//지수구분(1915심볼)
				tempData = IH::Parser(stringx, PTAB);
				tempData.TrimRight();
				tempData.TrimLeft();
///				m_iYDayVol[ii][JISUGUBN] = tempData;

				//2012.06.20 KSJ 배분, 임의 추가
				str950 = IH::Parser(stringx, PTAB);
				str950.TrimRight();
				str950.TrimLeft();

				str951 = IH::Parser(stringx, PTAB);
				str951.TrimRight();
				str951.TrimLeft();

				m_recomArray.Add(tempData);

				if(tempData != "")
					m_recomCodeToData.SetAt(strCode,tempData);

				double	dval1{}, dval2{}, dval3{}, creditPrc{};
				int iDval = 0;

				CString	str;
				dval1 = dval2 = dval3 = creditPrc = 0.0;
				CString futurnGubn, mCode;
				int sizeCode{};

				for (int jj = nStart; jj < nEnd; jj++)
				{
					const _gridHdr xgridHdr = m_gridHdrX.GetAt(jj);
					auto& pinters = m_inters.at(ii);

					mCode = pinters.get()->code;
					mCode.TrimRight(); mCode.TrimLeft();
					sizeCode = strlen(mCode);

					if (!m_posField)
						continue;   //매입단가 등 특정필드가 있을경우 아래로 넘어감

					if (xgridHdr.needs != 9)
						continue;

					switch (xgridHdr.symbol[3])
					{
					case '1':		// 보유수량
						entry = pinters.get()->xnum;
						if (atoi(entry) == 0)
							entry = "";
						break;
					case '2':		// 매입단가
						dval1 = atof(pinters.get()->xprc);
						if(sizeCode == 6 || sizeCode == 9)
						{
							iDval = gsl::narrow_cast<int>(Round(dval1));
							entry.Format("%d", iDval);
							if (atoi(entry) == 0)
								entry = "";
						}
						else if(sizeCode != 0)	//2012.03.22 KSJ 코드길이가 0이면 세팅안해줌.
						{
							entry.Format("%.2f", dval1);
							if (atof(entry) == 0)
								entry = "";
						}
						break;
					case '3':		// 평가손익
						dval1 = atof(pinters.get()->xnum);
						dval2 = atof(pinters.get()->xprc);
						dval3 = IH::TOfabs(m_grid->GetItemText(ii+1, colCURR));

						if (dval1 <= 0 || dval2 <= 0) continue;

						if(sizeCode == 6 || sizeCode == 9)
						{
								entry = CalcuPyungaSonik(pinters.get(), m_grid->GetItemText(ii+1, colCURR));
						}
						else
						{
							futurnGubn = CString(pinters.get()->futureGubn, sizeof(pinters.get()->futureGubn));
							entry = CalFutureEvalPrice(pinters.get(), strCode, futurnGubn, dval3, dval2, dval1);
//							entry = setPyungaSonik(pinters);
						}
						break;
					case '4':		// 수익율
						dval1 = atof(pinters.get()->xnum);
						dval2 = atof(pinters.get()->xprc);
						dval3 = IH::TOfabs(m_grid->GetItemText(ii+1, colCURR));

						if (dval1 <= 0 || dval2 <= 0) continue;

						if(sizeCode == 6 || sizeCode == 9)
						{

//							entry = CalJusikEvalRate(dval3, dval2, dval1);
							if(iHeadersymbol == 2023)
							{
								entry = CalcuSuik(pinters.get(), m_grid->GetItemText(ii+1, colCURR));
							}
							else
							{
								entry = CalcuSuik(pinters.get(), m_iYDayVol[ii][TDAYVALUE]);
							}
						}
						else
						{
							futurnGubn = CString(pinters.get()->futureGubn, sizeof(pinters.get()->futureGubn));
							entry = CalFutureEvalRate(pinters.get(), strCode, futurnGubn, dval3, dval2, dval1);
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
			  //2012.06.20 KSJ 배분, 임의 추가
			  //배분= 11 매수상한,12 매수하한,13 매도상한,14 매도하한
			  //임의= 12:시초동시 임의종료 지정, 14:마감동시 임의종료 지정 , 16:시간외단일가 임의종료 지정
			  bool bCheck = true;	//2012.11.02 KSJ 임의배분 > 단기과열종목 > 시장경보 순으로

			  if (!str950.IsEmpty() /*&& str950.GetAt(0) != '0'*/)
			  {
				  if(str950.GetAt(0) == '1')
				  {
					  m_grid->SetItemData(ii + 1, colINFO, 12);
					  bCheck = false;
				  }
				  else
				  {
					  m_grid->SetItemData(ii + 1, colINFO, 0);
					  bCheck = true;
				  }
			  }
			  else if(!str951.IsEmpty() /*&& str951.GetAt(0) != '0'*/)
			  {
				  if(str951 == "12" || str951 == "14" || str951 == "16")
				  {
					  m_grid->SetItemData(ii + 1, colINFO, 13);
					  bCheck = false;
				  }
				  else
				  {
					  m_grid->SetItemData(ii + 1, colINFO, 0);
					  bCheck = true;
				  }
			  }
			  //KSJ
			  if(bCheck)
			  {
				  SetColInfo(strName, ii + 1, entry);
			  }
		  }
		}
	}

	m_grid->SetRedraw(TRUE);

	if (m_ccField) calcInClient();
	const LONG	ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
	hawkeyeAction((ret) ? true : false);

	m_grid->getmemoCheck();
	if (m_seldrop >= 0)
		m_grid->SetSelectRow(m_seldrop), m_seldrop = -1;

	if (nullptr != m_pMsgWnd)
	{
		delete m_pMsgWnd;
		m_pMsgWnd = nullptr;
	}

	const int	nOver = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	{
		if(!m_bSort)
		{
			if(mode == -1)
			{
				m_bSingle = FALSE;
			}
			else if(mode > 100)
			{
				m_bSingle = FALSE;
				m_bSort = TRUE;
			}
			else if(mode > 0)
			{
				m_bSingle = FALSE;
			}
		}
		else
		{
			m_bSort = FALSE;
		}

		if(m_nSelectedRow > -1)
		{
			m_grid->SetSelectRow(m_nSelectedRow);
			m_grid->SetScrollPos32(SB_VERT,m_nScrollPos);
			m_nSelectedRow = -1;
			m_nScrollPos = -1;
		}

		m_grid->Invalidate();
	}
}

void CGridWnd::getBase(CString m_code)
{
	m_code.TrimLeft(); m_code.TrimRight();

	if (m_code.IsEmpty())
		return;
	const int	nkind = getJKind(m_code);

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
	const TCHAR	ch1 = m_code.GetAt(0);
	const TCHAR	ch2 = m_code.GetAt(1);
	const TCHAR	ch3 = m_code.GetAt(2);

	switch (ch1)
	{
	case '1':	// future
	case 'A':	//파생상품 코드개편
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
	case 'D':  //파생상품 코드개편
		if (ch2 == '0')
			return JK_SPREAD;
		else
			return JK_SFSPREAD;
	case '2':	// call option
	case 'B':	//  파생상품 코드개편
		if (m_code.GetAt(1) == '0')
			return JK_CALLOPT;
		else
			return JK_JCALLOPT;
		break;
	case '3':	// put option
	case 'C':	// 파생상품 코드개편
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

CString	CGridWnd::CalJusikEvalPrice(struct _intersx* pinters, double m_curr, double m_maip, double m_remian)
{
	double result{};
	CString entry, m_rprice;

	const double rPrice = m_curr * m_remian;
	const double mPrice = pinters->maeipPrc;

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

CString	CGridWnd::CalJusikEvalRate(struct _intersx* pinters, double m_curr, double m_maip, double m_remian)
{
	CString entry;
	double result{};
	const double  creditPrc = pinters->creditPrc;
//	double resultVal = 0;

//  	m_maip *= m_remian;
//  	m_curr *= m_remian;

	const double rPrice = m_curr * m_remian;
	const double mPrice = pinters->maeipPrc;

	result = rPrice - mPrice;

//  	result = (int)result;
//  	m_maip = (int)m_maip;

	entry.Format("%+.2f", (result / (mPrice - creditPrc)) * 100);

	return entry;
}



CString CGridWnd::setPyungaSonik(struct _inters* pinters)
{
	const double  pyunggaPrc = pinters->creditPrc;

	CString result;

	if (pyunggaPrc > 0)
		result.Format("+%.0f", pyunggaPrc);
	else if (pyunggaPrc < 0)
		result.Format("-%.0f", pyunggaPrc);

	return result;
}


CString CGridWnd::setsuikRate(struct _inters* pinters)
{
	const double  sonikRate = pinters->maeipPrc;

	CString result;

	result.Format("%.2f", sonikRate);

	if (sonikRate < 0)
		result = "" + result;
	else if (sonikRate > 0)
		result = "+" + result;

	return result;
}


void CGridWnd::parsingOubsOne(char* datB, int datL, int mode, int update)
{
	if (m_staticUpdate < 0)
		return;
	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL) return;

	CString	string, stringx, entry, strField;
	CString	bongdata = _T("");
	string = CString(&datB[gridHoL], datL - gridHoL);
	const BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();

	CString strCode, strMargin, strData;
	CString tempData, recommand1, recommand2, recommand3, recommand4, recommand5;
	CString str950, str951;

	int nStart{}, nEnd{};

	nStart = 0;
	nEnd = m_gridHdrX.GetSize();

	if(m_bContinue)
	{
		if(m_bSecond)
		{
			m_bSecond = FALSE;
			m_bContinue = FALSE;

			nStart = GRIDMAXNUM;
			nEnd = m_gridHdrX.GetSize();
		}
		else
		{
			nStart = 0;
			nEnd = GRIDMAXNUM;
		}
	}

	int rowLength = -1;
//	int startPoint=0;

	//-1 : MO_SELECT 모드
	// 0, 1 : MO_VISIBLE모드
	if(mode == -1)
	{
		rowLength = gsl::narrow_cast<int>(m_inters.size());
	}
	else if(mode == 0 || mode == 1)
	{

		rowLength = GetRowcount();


		//두번째 영역
		if(mode == 1)
		{
			for(int j=0 ; j<10 ; j++)
			{
				stringx = IH::Parser(string, PNEW);
			}
		}
	}

	for (int ii = 0; ii < rowLength; ii++)
	{
		if (string.IsEmpty())
			break;



			tempData = "";
			recommand1 = "";
			recommand2 = "";
			recommand3 = "";
			recommand4 = "";
			recommand5 = "";

			stringx = IH::Parser(string, PNEW);
			strData = stringx;

			if (ii < update) continue;

			//2013.07.04 KSJ 필드가 GRIDMAXNUM(27)개를 넘으면 2번 나눠서 조회한다.
			//그렇기때문에 첫번재 데이터에서 27번째까지만 뽑아서 뒷데이터랑 붙이면
			//완전한데이터가 된다.
			if(m_bContinue && m_bSecond)
			{
				CString strValue;
				strCode = m_grid->GetItemText(ii + 1, colCODE);

				if(m_mapCurValue.Lookup(strCode, strValue))
				{
					int nIndex = 0, nCount = 0;

					while(nIndex != -1)
					{
						nIndex = strValue.Find("\t", nIndex+1);

						if(++nCount == GRIDMAXNUM)
						{
							strValue = strValue.Left(nIndex+1);
							break;
						}
					}

					strValue += strData;
					m_mapCurValue.SetAt(strCode, strValue);
				}
			}
			//2013.07.04 END

			for (int jj = nStart; jj < nEnd; jj++)
			{
				if (stringx.IsEmpty())
					break;

				entry = IH::Parser(stringx, PTAB);

// 			entry.TrimRight();
// 			entry.TrimLeft();
				strField = entry;

				strField.TrimLeft();
				strField.TrimRight();

			if(strField == "0" || strField == "-0" || strField == "0.00" || strField == "+0" || strField == " 0" || strField == ".00" || strField == "+0.00" || strField == "-0.00")	//2012.07.17 KSJ 옵션일때의 경우 추가
			{
				entry = " ";
			}

			// ADD PSH 20070912
			const _gridHdr xgridHdr = m_gridHdrX.GetAt(jj);
			auto& pinters = m_inters.at(ii);

			if (jj == colCODE)
			{
				strCode = entry;

				//2012.08.31 추가한 데이터도 맵에 저장함
				m_mapCurValue.SetAt(strCode, strData);
			}
			else if (jj == colNAME)
			{
				if (pinters.get()->gubn == ROW_COMMENT)
				{
					entry.Format("%s", pinters.get()->name);
				}

				strMargin = m_grid->GetItemText(ii + 1, colMAR);
				m_mapMargin.SetAt(strCode, strMargin);
				m_grid->SetItemData(ii + 1, colNAME, atoi(strMargin.GetBuffer(0)));
			}
			else if (!entry.IsEmpty() && jj == colRATE && (xgridHdr.attr & GVAT_HIDDEN))
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

			if (!(xgridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
			{
				if ((strcmp("2029", xgridHdr.symbol) == 0) ||
					(strcmp("2030", xgridHdr.symbol) == 0) ||
					(strcmp("2031", xgridHdr.symbol) == 0) )
				{
					if ((1 == xgridHdr.needs) || (3 == xgridHdr.needs))
					{
						CString strVal  = entry;
						CString strDiff = strVal.Mid(1);
						double dDiffOpen{}, dPClose{}, dVal;
						dVal	= atof(strDiff.GetBuffer(0));
						dPClose = atof( (m_grid->GetItemText(ii + 1, colPCURR)).GetBuffer(0) );

						dDiffOpen = (dVal - dPClose) / dPClose * 100;

						if (1 == xgridHdr.needs)
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
						else if (3 == xgridHdr.needs)
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

			if (xgridHdr.needs != 9)
				continue;

			// DEL PSH 20070912
			//struct _inters* pinters = m_inters.GetAt(ii);
			// END DEL

			CString mCode;
			int sizeCode;
			mCode = pinters.get()->code;
			mCode.TrimRight(); mCode.TrimLeft();
			sizeCode = strlen(mCode);

			switch (xgridHdr.symbol[3])
			{
			case '1':		// 보유수량
				entry = pinters.get()->xnum;
				break;
			case '2':		// 매입단가
				if(sizeCode !=0 )	//2212.03.22 KSJ 코드가없을때는 아무것도 넣지않는다.
					entry = pinters.get()->xprc;
				else
					entry = "";
				break;
			case '3':		// 평가손익
				entry = CalcuPyungaSonik(pinters.get(), m_grid->GetItemText(ii+1, colCURR));
				break;
			case '4':		// 수익율
				entry = CalcuSuik(pinters.get(), m_grid->GetItemText(ii+1, colCURR));
				break;
			default:
				continue;
			}
			m_grid->SetItemText(ii+1, jj, entry);
		}

		//전일거래량을 저장(2321심볼)
		tempData = IH::Parser(stringx, PTAB);
		tempData.TrimRight();
		tempData.TrimLeft();
//		m_iYDayVol[ii][JMCODE] = strCode;
//		m_iYDayVol[ii][YDAYVOLUMN] = tempData;
		//save the Current Value
//		m_iYDayVol[ii][YDAYLASTVALUE] = m_grid->GetItemText(ii+1, colCURR);

		//현재가를 저장(2022심볼)
		tempData = IH::Parser(stringx, PTAB);

		//추천일자(1910심볼)
		recommand1 = IH::Parser(stringx, PTAB);
		recommand1.TrimRight();
		recommand1.TrimLeft();

		//추천종목 기준가(1911심볼)
		recommand2 = IH::Parser(stringx, PTAB);
		recommand2.TrimRight();
		recommand2.TrimLeft();

		//수익률(1912심볼)
// 		recommand3 = IH::Parser(stringx, P_TAB);
// 		recommand3.TrimRight();
// 		recommand3.TrimLeft();
//
// 		//추천종목 기준 BM지수(1913심볼)
// 		recommand4 = IH::Parser(stringx, P_TAB);
// 		recommand4.TrimRight();
// 		recommand4.TrimLeft();
//
// 		//BM수익률(1914심볼)
// 		recommand5 = IH::Parser(stringx, P_TAB);
// 		recommand5.TrimRight();
// 		recommand5.TrimLeft();

		//지수구분(1915심볼)
		tempData = IH::Parser(stringx, PTAB);
		tempData.TrimRight();
		tempData.TrimLeft();
//		m_iYDayVol[ii][JISUGUBN] = tempData;

		//2012.06.20 KSJ 배분, 임의 추가
		str950 = IH::Parser(stringx, PTAB);
		str950.TrimRight();
		str950.TrimLeft();

		str951 = IH::Parser(stringx, PTAB);
		str951.TrimRight();
		str951.TrimLeft();
		//KSJ

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
				//2012.06.20 KSJ 배분, 임의 추가
				//배분= 11 매수상한,12 매수하한,13 매도상한,14 매도하한
				//임의= 12:시초동시 임의종료 지정, 14:마감동시 임의종료 지정 , 16:시간외단일가 임의종료 지정
				bool bCheck = true;	//2012.11.02 KSJ 임의배분 > 단기과열종목 > 시장경보 순으로
				if (!str950.IsEmpty() /*&& str950.GetAt(0) != '0'*/)
				{
					if(str950.GetAt(0) == '1')
					{
						m_grid->SetItemData(ii + 1, colINFO, 12);
						bCheck = false;
					}
					else
					{
						m_grid->SetItemData(ii + 1, colINFO, 0);
						bCheck = true;
					}
				}
				else if(!str951.IsEmpty() /*&& str951.GetAt(0) != '0'*/)
				{
					if(str951 == "12" || str951 == "14" || str951 == "16")
					{
						m_grid->SetItemData(ii + 1, colINFO, 13);
						bCheck = false;
					}
					else
					{
						m_grid->SetItemData(ii + 1, colINFO, 0);
						bCheck = true;
					}
				}
				//KSJ
				if(bCheck)
				{
					if ("단기" == entry) //2012.10.31 KSJ 단기과열 추가
					{
						m_grid->SetItemData(ii + 1, colINFO, 14);
					}
					else if ("투자주의" == entry)
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
		// END ADD

		if (m_ccField) calcInClient(ii+1);
		const LONG	ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		if (ret) hawkEyeColor(ii+1);
		if (m_grid->setmemoCheck(ii+1)) m_grid->memoRefresh();
		break;
	}

	// ADD PSH 20070927

	stringx = IH::Parser(string, PTAB);

	if (!stringx.IsEmpty())
	{
		//strCode = stringx.Mid(1, 11);
		//strCode.TrimRight();

		//strMargin = stringx.Mid(153, 6);
		//strMargin.TrimRight();

		m_mapMargin.SetAt(strCode, stringx);
	}

	// END ADD

	m_grid->setmemoCheck(m_updateROW + 1);
	if (m_seldrop >= 0)
		m_grid->SetSelectRow(m_seldrop), m_seldrop = -1;

	if(m_bContinue)
	{
		m_bSecond = TRUE;
		sendTransactionTR(-1,GRIDMAXNUM,m_pGridArray.GetSize());

	}
	else
	{
		m_updateROW = -1;
		ReIndexing();
		Invalidate();
	}
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
		const int ncnt = gsl::narrow_cast<int>(m_inters.size());
		int	nIndex = -1;
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			auto& pinters = m_inters.at(ii);
			if (strlen(pinters.get()->code) == 0)
			{
				break;
			}

			if (strcmp(pinters.get()->code, (char*)code.operator LPCTSTR()) == 0)
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
		auto& pinters = m_inters.at(0);
		m_grid->SetItemData(1, colSIG, MAKEWPARAM(0, 1));
		pinters.get()->code = code;
		sendTransaction();
	}
}

//2012.01.19 KSJ Alertx 추가
void CGridWnd::InsertNewsx(DWORD* data)
{
	CString	code = _T(""), szTitle = _T("");

	szTitle = (char*)data[15];
	code = (char*)data[301];

	if (!code.IsEmpty())
	{
		m_szTitle.Format(" %s", (char*)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, m_kind));
		m_szTitle += ("- " + szTitle);
		InvalidateRect(m_rcTitle, FALSE);
		const int ncnt = gsl::narrow_cast<int>(m_inters.size());
		int	nIndex = -1;
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			auto& pinters = m_inters.at(ii);
			if (pinters.get()->code.IsEmpty())
			{
				break;
			}

			if (strcmp(pinters.get()->code, (char*)code.operator LPCTSTR()) == 0)
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
		auto& pinters = m_inters.at(0);
		m_grid->SetItemData(1, colSIG, MAKEWPARAM(0, 1));
		pinters.get()->code = code;
		sendTransaction();
	}
}
//2012.01.19 KSJ Alertx 추가 끝

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
	const int iFind = strData.Find(strToken);
	if(iFind > -1 )
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );
	}


	return sResult;
}

void CGridWnd::SettingGridHeaderName(int index)
{
	const struct	_symbol {
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
		_gridHdr xgridHdr;
		for (int ii = 0; ii < chksymC; ii++)
		{
			for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
			{
				xgridHdr = m_gridHdrX.GetAt(jj);
				if (atoi(xgridHdr.symbol) != atoi(chksym[ii].symb1))
					continue;

				xgridHdr.stid = chksym[ii].stid2;
				m_gridHdrX.SetAt(jj, xgridHdr);
			}
		}
		for ( int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
		{
			xgridHdr = m_gridHdrX.GetAt(jj);
		}
	}
	else
	{
		_gridHdr xgridHdr;
		for (int ii = 0; ii < chksymC; ii++)
		{
			for (int jj = colCURR ; jj < m_gridHdrX.GetSize() ; jj++)
			{
				xgridHdr = m_gridHdrX.GetAt(jj);
				if (atoi(xgridHdr.symbol) != atoi(chksym[ii].symb2))
					continue;

				xgridHdr.stid = chksym[ii].stid1;
				m_gridHdrX.SetAt(jj, xgridHdr);
				break;
			}
		}
	}
}

void  CGridWnd::initgridalert(bool bSend)
{
	if (m_grid->GetSafeHwnd())
	{
		m_grid->setMemoType(_pApp->getMemoType());
		m_grid->setBlinkColor(_pApp->getBlinkColor());
		m_grid->setBlinkType(_pApp->getBlinkType());
	}	

	if (bSend)
	{
		_bManual    = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);	
		_bAutoCheck = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);	
	}
}

//2012.01.19 KSJ Alertx 추가
void CGridWnd::parsingAlertx(LPARAM lParam)
{
	m_grid->setReal(true);
	CString slog;
	int xrow{};
	CString	code, name, symbol, entry, datB, strValue, dataExceptCode, strCode, strData;
	BOOL bTicker{};
	CString strGubn;		// 2012.02.09 KSJ 호가 체결 저장
	CString strTemp;
	CString str950, str951;
	bTicker = TRUE;

	bool bKrx = true;
	struct _alertR* alertR;
	alertR = (struct _alertR*)lParam;

	code = alertR->code;

	if (code.GetLength() == 7)
		strCode = code.Mid(1);
	else if (code.GetAt(0) == 'X')
		strCode = code.Mid(1);
	else
		strCode = code;

	// 시장구분자 NXT, 통합은 장전에 예상가 없음.
	if (code[0] == 'M' || code[0] == 'N')
		bKrx = false;

	DWORD* data{};
	data = (DWORD*)alertR->ptr[0];
	const bool bNeedMap = (data[0] && strcmp(reinterpret_cast<LPCSTR>(data[0]), "w") == 0);

	// ── 인덱스 치환 래퍼 ─────────────────────────────────────
	auto getData = [&](int idx) -> LPCSTR {
		int mapped = bNeedMap?ToMapped(idx) : idx;
		return data[mapped] ? reinterpret_cast<LPCSTR>(data[mapped]) : nullptr;
	};
	auto hasData = [&](int idx) -> bool {
		return data[bNeedMap?ToMapped(idx):idx] != 0;
	};
	// ─────────────────────────────────────────────────────────

	// ── isZeroLike (두 번째 소스 방식으로 통일) ───────────────
	const auto isZeroLike = [](const CString& v, bool includeThirty = false) -> bool {
		CString t(v); t.Trim();
		if (t.IsEmpty()) return true;
		static const char* tokens[] = { "0", "-0", "+0", "0.00", "-0.00", "+0.00", ".00" };
		for (const char* tok : tokens)
			if (t == tok) return true;
		if (includeThirty && t == "30") return true;
		return false;
	};
	// ─────────────────────────────────────────────────────────

	if (hasData(0))
		strGubn = getData(0);

	if (code.CompareNoCase("S0000") == 0)
	{
		parsingNewsx(data);
		return;
	}

	int count = 0;

	{
		// 2013.08.26 KSJ 지수일때 예상가 표시
		BOOL	bZisu = FALSE;
		if (!strGubn.Compare("X") && code.GetLength() == 5)
		{
			code.Delete(0);
			code.Insert(0, 'K');
			strCode = code;
			count = CheckRealTimeCode(code);
			bZisu = TRUE;
		}
		else
			count = CheckRealTimeCode(code);

		if (count == 0)
			return;

		for (int rowPosition = 0; rowPosition < count; rowPosition++)
		{
			xrow = m_irowCode[rowPosition];

			entry = _T("");
			CString	oldEXP = m_grid->GetItemText(xrow, colEXPECT);
			CString	newEXP = _T("");
			BOOL	bTransSymbol = FALSE;
			BOOL	bDaebi = FALSE;
			

			CString strTime, expect, real, excep;
			CString codeExceptA;
			CString en2, saveData;

			// ── 예상가/현재가 판단 (두 번째 소스 방식으로 통일) ──
			CString rawExpectValue;
			CString rawCurrValue;

			if (bNeedMap)
			{
				rawExpectValue = getData(111) ? CString(getData(111)) : CString(); // ToMapped(111) = 611
				rawCurrValue = getData(23) ? CString(getData(23)) : CString(); // ToMapped(23)  = 623
			}
			else if (bZisu)
			{
				rawExpectValue = data[23] ? CString(reinterpret_cast<LPCSTR>(data[23])) : CString();
				rawCurrValue = data[23] ? CString(reinterpret_cast<LPCSTR>(data[23])) : CString();
			}
			else
			{
				rawExpectValue = data[111] ? CString(reinterpret_cast<LPCSTR>(data[111])) : CString();
				rawCurrValue = data[23] ? CString(reinterpret_cast<LPCSTR>(data[23])) : CString();
			}
			rawExpectValue.Trim();
			rawCurrValue.Trim();

			const BOOL bHasExpect = !isZeroLike(rawExpectValue);
			const BOOL bHasCurr = !isZeroLike(rawCurrValue);

			// colEXPECT 세팅은 여기서 한 번만
			m_grid->SetItemText(xrow, colEXPECT, bHasExpect ? "1" : "0");

			LPCSTR expectPtr = bHasExpect ? rawExpectValue.GetString() : nullptr;
			LPCSTR currPtr = bHasCurr ? rawCurrValue.GetString() : nullptr;

			saveData = bHasExpect ? rawExpectValue : rawCurrValue;
			en2 = getData(34) ? CString(getData(34)) : CString();

			{

				slog.Format("[IB202200][symbol] [%s][%s][%d] bZisu=[%d] rawExpectValue=[%s] 예상가=[%s] 현재가=[%s] saveData=[%s]  대비=[%s] 등락률=[%s] 거래량=[%s]",
					strCode, strGubn, count,
					bZisu,
					rawExpectValue,
					expectPtr ? expectPtr : "",
					currPtr ? currPtr : "",
					saveData,
					getData(24),
					getData(33),
					getData(27)
				);
				Output_DebugString(slog);
			}
			// ─────────────────────────────────────────────────────

			entry.Empty();

			LPCSTR p90 = getData(90);
			CString str90 = p90 ? p90 : "";
			BOOL bLast = FALSE;


			//L 선물호가잔량 , 4 선물예상가 , P 옵션호가잔량, g 상품선물호가잔량
			str90.TrimLeft(); str90.TrimRight();
			if ((!strGubn.Compare("L") || !strGubn.Compare("4") || !strGubn.Compare("P") || !strGubn.Compare("g"))
				&& ((!str90.Compare("99")) || (!str90.Compare("40"))))
			{
				m_grid->SetItemText(xrow, colEXPECT, "0");
				m_grid->SetItemData(xrow, colEXPECT, 0);
				bTransSymbol = FALSE;
				bLast = TRUE;
			}

			// ── 예상가/현재가 분기 ────────────────────────────────
			if (expectPtr)
			{
				entry = expectPtr;

				// _typeAuto 대응:
				// _typeAuto==0 (자동): !_bManual && _bAutoCheck
				// _typeAuto==1 (예상): _bManual==TRUE
				// _typeAuto==2 (수동): !_bManual && !_bAutoCheck
				if (_bManual)					// _typeAuto == 1 (예상)
				{
					bTransSymbol = TRUE;
				}
				else if (_bAutoCheck)			// _typeAuto == 0 (자동)
				{
					bTransSymbol = TRUE;
				}
				else							// _typeAuto == 2 (수동) - 예상가 표시 안함
				{
					if (!entry.IsEmpty())
						m_grid->SetItemText(xrow, colEXPECT, "0");
				}
			}
			else if (currPtr && !bLast)
			{
				entry = currPtr;

				if (!_bManual)				// _typeAuto == 0 (자동) 또는 2 (수동)
				{
					if (_bAutoCheck)		// _typeAuto == 0 (자동)
					{
						LPCSTR p34 = getData(34);
						strTime = p34 ? p34 : "";

						if (m_strBeginTime <= strTime && m_strEndTime >= strTime && bKrx)
						{
							// 예상가 시간 범위 안 → colEXPECT 이미 "0"이므로 그대로
							// 필요 시 "1" 세팅은 하지 않음 (expectPtr 없으므로)
						}
						else
						{
							// 시간 범위 밖 (15:30 이후 포함) → 현재가 모드 확정
							m_grid->SetItemText(xrow, colEXPECT, "0");
						}
					}
					// _typeAuto == 2 (수동): colEXPECT "0" 이미 세팅됨, 그대로
				}
				else						// _typeAuto == 1 (예상, _bManual==TRUE)
				{
					if (!entry.IsEmpty())
					{
						// ── 핵심 수정: 시간 범위 체크 추가 (15:30 이후 예상가 오동작 방지) ──
						LPCSTR p34 = getData(34);
						strTime = p34 ? p34 : "";

						if (m_strBeginTime <= strTime && m_strEndTime >= strTime && bKrx)
						{
							bTransSymbol = TRUE;
							if (!isZeroLike(entry))
								m_grid->SetItemText(xrow, colEXPECT, "1");
							else
							{
								m_grid->SetItemText(xrow, colEXPECT, "0");
								entry = m_grid->GetItemText(xrow, colCURR);
								return;
							}
						}
						else
						{
							// 15:30 이후 → 예상가 모드 해제
							m_grid->SetItemText(xrow, colEXPECT, "0");
						}
					}
				}
			}
			// ─────────────────────────────────────────────────────

			newEXP = m_grid->GetItemText(xrow, colEXPECT);
			const BOOL bForceDraw = (newEXP == oldEXP) ? FALSE : TRUE;
			BOOL bExpect = (BOOL)atoi(m_grid->GetItemText(xrow, colEXPECT));
			bool updatePoss = false;
			const int countX = m_gridHdrX.GetSize();
			_gridHdr xgridHdr;
			const LONG ret = 0;

			const COLORREF rtmColor = ((CGroupWnd*)m_pGroupWnd)->GetRTMColor();
			const int	rtmAction = ((CGroupWnd*)m_pGroupWnd)->GetRtmAction();
			const BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();

			// 2012.11.08 KSJ 주식과 선물옵션의 심볼값이 똑같은 것이 있다. 외인소진률 같은것.
			const bool bKospi = (strCode.GetLength() == 6);

			for (int ii = 2; ii < countX; ii++)
			{
				xgridHdr = m_gridHdrX.GetAt(ii);
				symbol = CString(xgridHdr.symbol, strlen(xgridHdr.symbol));

				if (symbol.GetLength() >= 3)
					symbol = symbol.Right(3);

				if (!bTransSymbol)
				{
					// 현재가 모드: 예상 심볼이면 현재가 인덱스로 치환
					if (!symbol.Compare("111"))			// 예상가 심볼 → 현재가(23)
						entry = getData(23);
					else if (!symbol.Compare("112"))	// 예상거래량 → 거래량(27)
						entry = getData(27);
					else if (!symbol.Compare("115"))	// 예상전일대비 → 전일대비(24)
						entry = getData(24);
					else if (!symbol.Compare("116"))	// 예상등락률 → 등락률(33)
						entry = getData(33);
					else if (!bKospi && !symbol.Compare("204"))
						entry = " ";
					else if (atof(symbol) == 0)
						entry = " ";
					else
						entry = getData(atoi(symbol));
				}
				else if (hasData(atoi(symbol)) && bZisu)	// 지수: 심볼과 인덱스 일치
				{
					entry = getData(atoi(symbol));
				}
				else if (bTransSymbol)
				{
					// 예상가 모드: 현재가 심볼이면 예상가 인덱스로 치환
					if (!symbol.Compare("023"))			// 현재가 심볼 → 예상가(111)
						entry = getData(111);
					else if (!symbol.Compare("027"))	// 거래량 → 예상거래량(112)
						entry = getData(112);
					else if (!symbol.Compare("024"))	// 전일대비 → 예상전일대비(115)
						entry = getData(115);
					else if (!symbol.Compare("033"))	// 등락률 → 예상등락률(116)
						entry = getData(116);
					else if (!bKospi && !symbol.Compare("204"))
						entry = " ";
					else if (atof(symbol) == 0)
						entry = " ";
					else
						entry = getData(atoi(symbol));
				}
				else
					continue;

				if (!bForceDraw && IH::TOf(entry) == IH::TOf(m_grid->GetItemText(xrow, ii)))
					continue;

				if (!(xgridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
				{
					if ((strcmp("2029", xgridHdr.symbol) == 0) ||
						(strcmp("2030", xgridHdr.symbol) == 0) ||
						(strcmp("2031", xgridHdr.symbol) == 0))
					{
						if ((1 == xgridHdr.needs) || (3 == xgridHdr.needs))
						{
							CString strVal = entry;
							CString strDiff = strVal;

							if (strVal[0] == '+' || strVal[0] == '-')
								strDiff = strVal.Mid(1);

							double dDiffOpen = 0.0;
							double dPClose{}, dVal{};
							dVal = atof(strDiff.GetBuffer(0));
							dPClose = atof((m_grid->GetItemText(xrow, colPCURR)).GetBuffer(0));

							if (dVal != 0 && dPClose != 0)
							{
								dDiffOpen = (dVal - dPClose) / dPClose * 100;

								if (1 == xgridHdr.needs)
								{
									entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
								}
								else if (3 == xgridHdr.needs)
								{
									if (0 > dDiffOpen)
										entry.Format("%0.2f%c", dDiffOpen, P_PER);
									else if (0 == dDiffOpen)
										entry.Format(" %0.2f%c", dDiffOpen, P_PER);
									else
										entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
								}
							}
						}
					}
				}

				if ((ii == colCURR) && bExpect && !m_bExpect)
				{
					entry.TrimLeft();
					entry.TrimRight();

					if (isZeroLike(entry) || entry == "")
					{
						entry = m_grid->GetItemText(xrow, colCURR);
						return;
					}
				}

				CString tempStr;

				if (ii == colCURR)
				{
					if (code.Find("K0001") > -1)
					{
						m_dKospi = atof(entry);
						tempStr.Format("%.2f", m_dKospi);
						m_dKospi = atof(tempStr);
					}
					else if (code.Find("KQ001") > -1)
					{
						m_dKosdaq = atof(entry);
						tempStr.Format("%.2f", m_dKosdaq);
						m_dKosdaq = atof(tempStr);
					}
				}

				// 2012.03.20 KSJ 현재가 보합될때 대비, 등락률 지워지지 않는 현상 수정
				if ((strcmp("2115", xgridHdr.symbol) == 0) || (strcmp("2024", xgridHdr.symbol) == 0)
					|| (strcmp("2116", xgridHdr.symbol) == 0) || (strcmp("2033", xgridHdr.symbol) == 0))
				{
					entry.TrimLeft(); entry.TrimRight();
					if (isZeroLike(entry, true))
						bDaebi = TRUE;
				}
				else
				{
					entry.TrimLeft(); entry.TrimRight();
					if (isZeroLike(entry) || atof(entry) == 0 || atof(symbol) == 0)
					{
						if (entry.GetLength() > 0 && atof(entry.Mid(1)) == 0)
							entry = " ";
					}
				}

				if (ii == colCURR)
				{
					if (entry == "")
						entry = m_grid->GetItemText(xrow, colCURR);
				}

				CString strPreValue;
				strPreValue = m_grid->GetItemText(xrow, ii);
				strPreValue.TrimLeft();
				strPreValue.TrimRight();

				if (ii != colCURR && (strPreValue != entry || strPreValue.IsEmpty()))
				{
					entry.TrimLeft();
					entry.TrimRight();

					if (symbol.CompareNoCase("146") == 0 || symbol.CompareNoCase("181") == 0)
					{
						if (hasData(146))
							m_grid->SetItemText(xrow, ii, entry);
					}
					else if (symbol.CompareNoCase("051") == 0)
					{
						LPCSTR p25 = getData(25);
						entry = p25 ? p25 : "";
						if (!entry.IsEmpty())
							m_grid->SetItemText(xrow, ii, entry);
					}
					else if (symbol.CompareNoCase("071") == 0)
					{
						LPCSTR p26 = getData(26);
						entry = p26 ? p26 : "";
						if (!entry.IsEmpty())
							m_grid->SetItemText(xrow, ii, entry);
					}
					else if (!entry.IsEmpty())
					{
						if (bDaebi)
							entry = "";
						bDaebi = FALSE;
						m_grid->SetItemText(xrow, ii, entry);
					}
				}
				else if (ii == colCURR)
				{
					// 2013.08.23 KSJ 스프레드종목
					if (strCode.GetLength() == 8 && (strCode.GetAt(0) == '4' || strCode.GetAt(0) == 'D'))
					{
						if (entry.GetAt(0) == ' ')
							entry.Replace(" ", "0");
					}
					m_grid->SetItemText(xrow, ii, entry);
				}

				if (ii == colCURR)
				{
					if (!bExpect && m_bongField >= 0)
					{
						CString bongdata = _T(""), open = _T(""), high = _T(""), low = _T("");

						if (hasData(29))
						{
							open = getData(29);
							m_grid->SetItemText(xrow, colOPEN, open);
						}
						if (hasData(30))
						{
							high = getData(30);
							high.Remove('+'), high.Remove('-');

							if (hasData(31))
							{
								low = getData(31);
								low.Remove('+'), low.Remove('-');
								bongdata.Format("%s%c%s", high, P_TAB, low);
								m_grid->SetItemText(xrow, m_bongField, bongdata);
							}
						}
					}
					else if (bExpect && m_bongField >= 0)
					{
						m_grid->SetItemText(xrow, m_bongField, _T(""));
					}

					if (m_posField && !bExpect)
						updatePoss = true;
				}

				entry.Empty();
			} // for ii 끝

			if (m_ccField)
				calcInClient(xrow);

			if (m_posField)
			{
				for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
				{
					if (m_posField)
					{
						double dval1{}, dval2{}, dval3{};
						CString str;
						dval1 = dval2 = dval3 = 0.0;
						CString futurnGubn, mCode, strCurr;
						int sizeCode{};

						const _gridHdr xxgridHdr = m_gridHdrX.GetAt(jj);
						if (xxgridHdr.needs != 9)
							continue;

						auto& pinters = m_inters.at(xrow - 1);

						mCode = pinters.get()->code;
						mCode.Trim();
						sizeCode = mCode.GetLength();

						if (bExpect == FALSE)		// 장중
						{
							switch (xxgridHdr.symbol[3])
							{
							case '3':		// 평가손익
								dval1 = atof(pinters.get()->xnum);
								dval2 = atof(pinters.get()->xprc);
								dval3 = IH::TOfabs(m_grid->GetItemText(xrow, colCURR));
								if (dval1 <= 0 || dval2 <= 0) continue;
								if (sizeCode == 6 || sizeCode == 9)
									entry = CalcuPyungaSonik(pinters.get(), m_grid->GetItemText(xrow, colCURR));
								else
								{
									futurnGubn = CString(pinters.get()->futureGubn, sizeof(pinters.get()->futureGubn));
									entry = CalFutureEvalPrice(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
								}
								break;

							case '4':		// 수익율
								dval1 = atof(pinters.get()->xnum);
								dval2 = atof(pinters.get()->xprc);
								dval3 = IH::TOfabs(m_grid->GetItemText(xrow, colCURR));
								if (dval1 <= 0 || dval2 <= 0) continue;
								if (sizeCode == 6 || sizeCode == 9)
									entry = CalcuSuik(pinters.get(), m_grid->GetItemText(xrow, colCURR));
								else
								{
									futurnGubn = CString(pinters.get()->futureGubn, sizeof(pinters.get()->futureGubn));
									entry = CalFutureEvalRate(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
								}
								break;

							default:
								continue;
							}
							m_grid->SetItemText(xrow, jj, entry);
						}
						else		// 동시호가 시간
						{
							if (_bAutoCheck)	// _typeAuto == 0 (자동)
							{
								switch (xgridHdr.symbol[3])
								{
								case '3':		// 평가손익
									dval1 = atof(pinters.get()->xnum);
									dval2 = atof(pinters.get()->xprc);
									if (dval1 <= 0 || dval2 <= 0) continue;
									if (sizeCode == 6 || sizeCode == 9)
										entry = CalcuPyungaSonik(pinters.get(), strCurr);
									else
									{
										futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
										entry = CalFutureEvalPrice(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
									}
									break;

								case '4':		// 수익율
									dval1 = atof(pinters.get()->xnum);
									dval2 = atof(pinters.get()->xprc);
									if (dval1 <= 0 || dval2 <= 0) continue;
									if (sizeCode == 6 || sizeCode == 9)
										entry = CalcuSuik(pinters.get(), strCurr);
									else
									{
										futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
										entry = CalFutureEvalRate(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
									}
									break;

								default:
									continue;
								}
								m_grid->SetItemText(xrow, jj, entry);
								entry.Empty();
							}
						}
					}
				}
			}

			// 2012.06.20 KSJ 배분, 임의 추가
			if (hasData(950))
			{
				str950 = getData(950);
				if (!str950.IsEmpty())
				{
					if (str950.GetAt(0) == '1')
						m_grid->SetItemData(xrow, colINFO, 12);
					else
					{
						entry = m_grid->GetItemText(xrow, colINFO);
						CString strName = m_grid->GetItemText(xrow, colNAME);
						SetColInfo(strName, xrow, entry);
					}
				}
			}

			if (hasData(951))
			{
				str951 = getData(951);
				if (!str951.IsEmpty())
				{
					if (str951 == "12" || str951 == "14" || str951 == "16")
						m_grid->SetItemData(xrow, colINFO, 13);
					else
					{
						entry = m_grid->GetItemText(xrow, colINFO);
						CString strName = m_grid->GetItemText(xrow, colNAME);
						SetColInfo(strName, xrow, entry);
					}
				}
			}
		} // for rowPosition 끝

#ifdef DF_RTS_CHECK
		m_grid->ReDrawTimer();
#else
		m_grid->ReDrawAll();
#endif
	}
}
//2012.01.19 KSJ Alertx 추가 끝

void CGridWnd::UpdateDraw()
{
	if (m_grid)
		m_grid->UpdateWindow();
}


void CGridWnd::SetColInfo(CString strName, int nRow, CString strEntry)	//2012.11.02 KSJ 종목특이사항 세팅
{
	if(strName.GetAt(0) == 'X')
	{
		m_grid->SetItemData(nRow, colINFO, 10);
	}
	else if ("정리" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 11);
	}
	else if ("단기" == strEntry) //2012.10.31 KSJ 단기과열 추가
	{
		m_grid->SetItemData(nRow, colINFO, 14);
	}
	else if ("투자주의" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 3);
	}
	else if ("투자경고" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 4);
	}
	else if ("위험예정" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 15);
	}
	else if ("투자위험" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 5);
	}
	else if ("투자주의환기" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 7);
	}
	else if(strName.GetAt(0) == '&')
	{
		m_grid->SetItemData(nRow, colINFO, 1);
	}
	else if ("감리" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 22);
	}
	else if(strName.GetAt(0) == '%')
	{
		m_grid->SetItemData(nRow, colINFO, 8);
	}
	else if(strName.GetAt(0) == '@')
	{
		m_grid->SetItemData(nRow, colINFO, 9);
	}
	else if ("권리" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 16);
	}
	else if ("배당" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 17);
	}
	else if ("액면" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 18);
	}
	else if ("합병" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 20);
	}
	else if ("감자" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 21);
	}
	else if ("초저유동성" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 23);
	}
	else	//2011.12.06 KSJ 일치하는게 없으면 0으로 세팅해준다.
	{
		m_grid->SetItemData(nRow, colINFO, 0);
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
	double	curr{}, open{}, high{}, low{};

	curr  = IH::TOfabs(m_grid->GetItemText(row, colCURR));
	open  = IH::TOfabs(m_grid->GetItemText(row, colOPEN));
	high  = IH::TOfabs(m_grid->GetItemText(row, colHIGH));
	low   = IH::TOfabs(m_grid->GetItemText(row, colLOW));

	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		const _gridHdr xgridHdr = m_gridHdrX.GetAt(ii);

		if (xgridHdr.needs != 8)
			continue;

		string = _T("");

		switch (xgridHdr.symbol[2])
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
			/* 2011.12.22 KSJ
			if (((CGroupWnd*)m_pGroupWnd)->GetRtmAction() == rtmCELL)
				m_grid->Blink(row, ii, ((CGroupWnd*)m_pGroupWnd)->GetRTMColor());
			*/
		}
	}
}

#include "mmsystem.h"
#pragma comment(lib, "winmm")

int CGridWnd::CheckRealTimeCode(CString code)
{
	CString slog;
	class CIndexMap* idx = nullptr;

	const int realtimeCol = 0;
	CString string;

// DWORD	tick;

	const int count = 0;
	int ii = 0;
	for (ii = 0; ii < 100 ; ii++)	//2012.09.10 KSJ 200 -> 100으로 수정
		m_irowCode[ii] = 0;

	// 속도개선 Search 방법 변경

// 	TRACE("map  serarch... gap = [%d]\n", m_grid->GetRowCount()/*, int(timeGetTime() - tick)*/);

	if (m_pSearchMap.GetCount() > 0)
	{
		if (!m_pSearchMap.Lookup(code, (CObject*&)idx))
			return count;

		for (ii = 0; ii < idx->idxCnt; ii++)
			m_irowCode[ii] = idx->index[ii];

		slog.Format("[cx_interest][symbol] [%s] code=[%s] ii=[%d]", __FUNCTION__, code, ii);
		Output_DebugString(slog);

		return idx->idxCnt;
	}

// tick = timeGetTime();
// 	for (count = 0, ii = 1; ii < m_grid->GetRowCount(); ii++)
// 	{
// 		string = m_grid->GetItemText(ii, realtimeCol);
// 		if (string == code) m_irowCode[count++] = ii;
//
// 		string.TrimLeft(); string.TrimRight();
// 		if (string.IsEmpty()) continue;
//
// 		// map에 중복 데이터 체크
// 		if (!m_pSearchMap.Lookup(string, (CObject*&)idx))
// 			idx = new CIndexMap();
//
// 		if (idx->idxCnt >= maxIDX)
// 			continue;
//
// 		idx->index[idx->idxCnt++] = ii;
// 		m_pSearchMap.SetAt(string, (CObject*)idx);
// 	}
// TRACE("grid serarch... gap = [%d][%d]\n", m_grid->GetRowCount(), int(timeGetTime() - tick));

	return count;
}

//2011.12.29 KSJ
void CGridWnd::ReSetSearchMap()
{
	CString slog;
	class CIndexMap* idx = nullptr;

	const int realtimeCol = 0;
	CString string;
	_mapSymbol.clear();
	m_grid->initRowFilter();

	int count = 0, ii = 0;

	for (count = 0, ii = 1; ii < m_grid->GetRowCount(); ii++)
	{
		string = m_grid->GetItemText(ii, realtimeCol);
		m_irowCode[count++] = ii;

		string.TrimLeft(); string.TrimRight();
		if (string.IsEmpty()) continue;

		_mapSymbol.emplace(std::make_pair(string, 1));

		// map에 중복 데이터 체크
		if (!m_pSearchMap.Lookup(string, (CObject*&)idx))
		{
			auto& item = _vIndex.emplace_back(std::make_unique<CIndexMap>());
			idx = item.get();
		}

		if (idx->idxCnt >= maxIDX)
			continue;

		idx->index[idx->idxCnt++] = ii;
		m_pSearchMap.SetAt(string, (CObject*)idx);

		slog.Format("[cx_interest][symbol] [%s] string=[%s] ii=[%d]", __FUNCTION__, string, ii);
		Output_DebugString(slog);

	}
}

void CGridWnd::ClearSearchMap()
{
	_mapSymbol.clear();
	m_pSearchMap.RemoveAll();
	_vIndex.clear();
}


void CGridWnd::ClearInterest()
{
	_mapSymbol.clear();
	m_inters.clear();
}


bool CGridWnd::ExistFile(int gno)
{
	CString	filePath;

	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, gno);

//	DWORD	fLen = 0;
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
	if (nullptr != pProp)
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
	const int nCnt = gsl::narrow_cast<int>(m_inters.size());
	for (int nRow = 1; nRow < nCnt; nRow++)
	{
		auto& pInter = m_inters.at(nRow - 1);

		if (pInter)
		{
			UINT attr{};

			if (ROW_COMMENT == pInter.get()->gubn)
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
	m_arrBaseInters.clear();
}
// END ADD

//int CGridWnd::AddInters(std::shared_ptr<struct _intersx> pinters)
//{
//	if (!pinters->code.IsEmpty())
//		m_mapCode.SetAt(pinters->code, pinters->name);
//
//	return m_inters.push_back(pinters);
//}


void CGridWnd::RemoveInters(int nIndex)
{
	if(nIndex == -1) return;	//2013.08.07 KSJ -1이면 종료

	auto& pinters = m_inters.at(nIndex);
	if (!pinters.get()->code.IsEmpty())
	{
		// ADD PSH 20070913
		if (!m_bSorting)
		{
			const int nRowCnt = gsl::narrow_cast<int>(m_arrBaseInters.size());

			for (int nRow = 0; nRow < nRowCnt; nRow++)
			{
				auto& pInter = m_arrBaseInters.at(nRow);

				if ((pInter) && pInter.get()->code.CompareNoCase(pinters.get()->code) == 0)
				{
					m_arrBaseInters.erase(m_arrBaseInters.begin() + nRow);	//2013.06.03 nIndex를 지우는게 아니라 nRow를 지우도록 변경
					break;
				}
			}
		}

		m_mapMargin.RemoveKey(pinters.get()->code);
		// END ADD

		m_mapCode.RemoveKey(pinters.get()->code);
	}

	m_inters.erase(m_inters.begin() + nIndex);

	insertInterest(MAX_LINE - 1);
}

void CGridWnd::SetKind(UINT kind)
{
	m_kind = kind;
	m_szTitle.Format(" %s", (char*)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, kind));

// 	if (kind != 0)
// 		((CMainWnd*)m_pMainWnd)->SetRTSTreeData(this);

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

void CGridWnd::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1000)
	{
		const int	ncnt = m_arExist.GetCount();
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
		case 0:		//2012.10.11 KSJ 주식, 금리, 통화선물 추가
			if(code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물 파생상품 코드개편
			{
				int nGubn = atoi(code.Mid(1,2));
				CString strGubn = code.Mid(1,2);	//2014.10.21 KSJ 주식선물 신규종목 연동

				if(nGubn > 10 && nGubn < 60)	//주식선물
				{
					string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
					Variant(triggerCC, string);

					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if(nGubn > 60 && nGubn <= 70)	//금리선물
				{
					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if(nGubn > 70 && nGubn < 80)	//통화선물
				{
					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
					|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
					|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z"))
				{
					string.Format("%s\t%s", "ed_fcod", code);	//2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
					Variant(triggerCC, string);

					// m_slog.Format("[cx_interest][SF][%s]<%d> code=[%s] string=[%s]", __FUNCTION__, __LINE__, code, string);
					// OutputDebugString(m_slog);

					string.Format("%s\t%s", SFOD, code);
					break;
				}
			}
		}
		LPCTSTR
		activeTrigger(code);
		//Variant(triggerCC, string);
		m_grid->SetFocus();
		m_grid->SetSelectRow(nIndex + 1);
		m_nTurn++;
		m_nTurn = m_nTurn % ncnt;
	}
	else if(nIDEvent == 2000)
	{
		KillTimer(2000);
//		m_grid->SetSelectRow(m_selectRow);
//		m_grid->SetFocusCellEdit(m_selectRow, colNAME, true);

	}
	else if(nIDEvent == 3000)
	{
		KillTimer(3000);
//		m_grid->SetSelectRow(m_selectRow);
	}
	else if(nIDEvent == 5500)
	{
		KillTimer(5500);

		m_grid->SetFocus();
	}

	CBaseWnd::OnTimer(nIDEvent);
}


void CGridWnd::rebuildInterest()
{
	CString	code, name;

	// ADD PSH 20070913
	if (1 > gsl::narrow_cast<int>(m_arrBaseInters.size()))
	{
		const int nRowCnt = gsl::narrow_cast<int>(m_inters.size());

		for (int nRow = 0; nRow < nRowCnt; nRow++)
		{
			auto& pInter = m_inters.at(nRow);
			if ( (pInter != nullptr) && pInter.get()->gubn != '0' && pInter.get()->gubn != '\0' )
			{
				//_inters* pBaseInter = (_inters *) new char[sz_inters];
				//ZeroMemory(pBaseInter, sz_inters);
				//CopyInter(pBaseInter, pInter);

				m_arrBaseInters.emplace_back(pInter);
			}
		}
	}
	// END ADD
}

void CGridWnd::Reset(bool bAll)
{
	KillTimer(1000);
	RemoveAll();
	const int	nActive = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ACTIVE));
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
	int nScnt = 0;
	CString code(_T("")), string(_T("")), data(_T("")), string_code(_T(""));
	const char chTab = P_TAB;
	const char chLf = P_NEW;


	int i = 0;
	for (i = m_inters.size() - 1; i >= 0; i--)
	{
		auto& pInters = m_inters.at(i);
		if (pInters.get()->code.IsEmpty())
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


	for (i = 0; i <= nScnt; i++)
	{
		auto& pInters = m_inters.at(i);
		if(pInters.get()->code[0] != 'm')
		{

			if(pInters->code[0] != 0)
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

void CGridWnd::saveInterestVisible(BOOL bVisible, int gno)
{

}

void CGridWnd::saveInterestInverse(BOOL bVisible, bool btmp, int gno)
{
	if (gno < 0)
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			gno = CAST_TREEID(m_kind)->value;
		else
			return;
	}
	return;
}

void CGridWnd::saveBookmark(BOOL bVisible, int gno)
{

}




void CGridWnd::saveInterest(BOOL bVisible, bool btmp, int gno, bool bBookMark)
{
	if (gno < 0)
		gno = ((CMainWnd*)m_pMainWnd)->_groupKey;

	if (gno <= 0)
		return;

	((CGroupWnd*)m_pGroupWnd)->WriteFileSumtoEachGroup(gno);
}

CString MakePacket(CString& code, CString amount, CString price, CString name)
// END MODIFY
{
	code.TrimLeft();
	code.TrimRight();
	if (code.IsEmpty())
		return _T("");

	// MODIFY PSH 20070914
	/*
	if (!amount.IsEmpty())
		code += ("\t" + amount);
	*/
	code += "\t";
	if (!amount.IsEmpty())
		code += amount;
	// END MODIFY

	// MODIFY PSH 20070914
	/*
	if (!price.IsEmpty())
		code += ("\t" + price);
	*/
	code += "\t";
	if (!price.IsEmpty())
		code += price;
	// END MODIFY

	// ADD PSH 20070914
	code += "\t";
	if (!name.IsEmpty())
		code += name;
	// END ADD

	return code;
}

int	CGridWnd::GetRowcountVisibleMode()
{
	CWnd*	ptoolWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	int gno = (int)ptoolWnd->SendMessage(WM_MANAGE, MK_GETCBINDEX);

	gno = gno+1;

	CGridData sdata;

	CString	filePath, filePath2;
	UINT	readL{};
	CString	code = _T(""), amount = _T(""), price = _T(""), name = _T("");

	if (ExistFile(gno))
	{
		filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, gno);
	}
	else
		return 0;

	// END MODIFY
	struct	_inters interest;

	CFile	fileH(filePath, CFile::modeRead);

	if (fileH.m_hFile == CFile::hFileNull)
		return 0;

	fileH.SeekToBegin();

	for (int ii = 0; ii < MAX_LINE ; ii++)
	{
		readL = fileH.Read(&interest, sz_inters);

		if (readL < sz_inters)
			break;

		code.Format("%.*s", sizeof(interest.code), interest.code);
		code.TrimRight();
		if(code.IsEmpty())
		{
			code = "          ";
		}

		name.Format("%.*s", sizeof(interest.name), interest.name);
		name.TrimLeft(), name.TrimRight();

		if(interest.code[0] == 'm' && name.IsEmpty())
		{
			CString tempInterCode = CString((interest.code));
			tempInterCode = tempInterCode.Left(12);
			tempInterCode.TrimRight();

			name.Format("%.*s", sizeof(interest.name), interest.name);
			name.TrimLeft(), name.TrimRight();
		}


		amount.Format("%.*s", sizeof(interest.xnum), interest.xnum);
		amount.TrimLeft(), amount.TrimRight();
		price.Format("%.*s", sizeof(interest.xprc), interest.xprc);
		price.TrimLeft(), price.TrimRight();

		sdata.m_arDatas.Add(MakePacket(code, amount, price, name));

		// END MODIFY
		if (sdata.GetCount() == 100)
			break;
	}

	fileH.Close();

	CString data;

	int i = 0;
	for (i = sdata.m_arDatas.GetUpperBound() ; i >= 0 ; i--)
	{
		data = sdata.m_arDatas.GetAt(i);
		code = IH::Parser(data, PTAB);

		if (strlen(code) > 0)
			break;
	}

	sdata.Reset();

	return i+1;
}

int CGridWnd::GetRowcount()
{
	const int maxRow = m_inters.size() - 1;
	int i = 0;
	for (i = maxRow ; i >= 0; i--)
	{
		auto& pInters = m_inters.at(i);
		pInters.get()->code.Trim();
		if (!pInters->code.IsEmpty())
			break;
	}

	return i+1;
}

CString CGridWnd::GetcolName(int index)
{
	return m_grid->GetItemText(index + 1, colNAME);
}


CString	CGridWnd::GetGroupName(int gno)
{
	CString str = ((CMainWnd*)m_pMainWnd)->_groupName;
	return str;;
}

//081215 :: 오른쪽 팜업메뉴로 관심그룹 저장시 저장 안되던 문제 해결
void CGridWnd::OnAllsave()
{
//	DWORD	fLen = 0;
	CFileFind finder;
	CString	filePath, fileTemp;
//	int itemcount = 0;

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
	struct	_inters* pinters{};

	CWnd*	pTreeWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	CWnd*	pGroupWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	const int	nOver = (int)pTreeWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);


	//2012.02.09 KSJ 여기서 왜 m_inters가 변하는지 모르겠다..
	//예상으로는 BaseSort에서 뭔가를 할것 같은데..
	//rebuildInterest에 안쓸것 같은거 주석처리하니까 제대로됨..
	const int scnt = GetRowcount();


	if (scnt < 0)
	{
//		Variant(guideCC, "선택된 그룹에 종목이 없습니다.");
		return;
	}

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{

		saveInterest();
		m_bEditWork = FALSE;
	}
// 	else
// 	{
//
// 		//그룹명 검색해서 내관심종목에 vol 추가하기
// 		stringx = makeGroupName();
// //		stringx.Format("%s%c", _T("내관심종목"), P_DELI);
//
// 		for (int ii = 0; ii <= scnt; ii++)
// 		{
// 			pinters = m_inters.GetAt(ii);
//
// 			code = CString(pinters->code, sizeof(pinters->code));
// 			code.TrimLeft();
// 			code.TrimRight();
//
// 			if(pinters->bookmark[0] == '\0')		//nullptr
// 			{
// 				pinters->bookmark[0] = '0';
// 			}
//
// 			string.Format("%s%c%c%c",code, P_DELI ,pinters->bookmark[0], P_DELI);
// 			stringx += string;
// 		}
//
// 		CString tem; tem.Format("[%s]", stringx);
//
// 		string.Format("%s /p5 /S/d%s%c%s%c", MAP_CONFIG, "appendParentGROUP", P_TAB, stringx, P_NEW);
// 		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_OPENSCR, typeVIEW), (LPARAM)string.operator LPCTSTR());
// 	}
}

CString	CGridWnd::makeGroupName()
{
	CString stringx, string, gno, gname, gnameLTrim, gnameRTrim, strPath;
	int readL = 0, idx = 0, makeGNo = 0;
	char readB[1024]{};
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

		m_pGridMarker.RemoveAll();
		break;
	case IDC_BT_ERASE:
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_CLOSEGRID, m_nIndex));
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_INITGRID);
		break;
	}
	return CBaseWnd::OnCommand(wParam, lParam);
}

// void CGridWnd::DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
// {
// 	CRect	rect;
// 	int	nWidth{}, nHeight{};

// 	drawRC.right += 1;
// 	drawRC.bottom += 1;
// 	nWidth = drawRC.Width();
// 	nHeight = drawRC.Height();

// 	int rs{}, gs{}, bs{};
// 	rs = GetRValue(sColor);
// 	gs = GetGValue(sColor);
// 	bs = GetBValue(sColor);

// 	int re{},ge{},be{};
// 	re = GetRValue(eColor);
// 	ge = GetGValue(eColor);
// 	be = GetBValue(eColor);

// 	float	rStep{}, gStep{}, bStep{};
// 	CPen	*sPen{};
// 	if (bRight)
// 	{
// 		if (!nWidth)
// 		{
// 			CPen	cPen(PS_SOLID, 1, sColor);
// 			sPen = pDC->SelectObject(&cPen);

// 			pDC->MoveTo(drawRC.left, drawRC.top);
// 			pDC->LineTo(drawRC.right, drawRC.bottom);
// 			pDC->SelectObject(sPen);
// 			return;
// 		}

// 		rStep = float(rs - re)/float(nWidth);
// 		gStep = float(gs - ge)/float(nWidth);
// 		bStep = float(bs - be)/float(nWidth);

// 		if (!nHeight)
// 		{
// 			for (int ii = 0; ii < nWidth; ++ii)
// 			{
// 				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
// 				sPen = pDC->SelectObject(&cPen);

// 				pDC->MoveTo(drawRC.left + ii, drawRC.top);
// 				pDC->LineTo(drawRC.left + ii + 1, drawRC.top);
// 				pDC->SelectObject(sPen);
// 			}
// 		}
// 		else
// 		{
// 			for (int ii = 0; ii < nWidth; ++ii)
// 			{
// 				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);

// 				CBrush brush;
// 				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
// 				pDC->FillRect(&rect, &brush);
// 				brush.DeleteObject();
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if (!nHeight)
// 		{
// 			CPen	cPen(PS_SOLID, 1, sColor);
// 			sPen = pDC->SelectObject(&cPen);

// 			pDC->MoveTo(drawRC.left, drawRC.top);
// 			pDC->LineTo(drawRC.right, drawRC.bottom);
// 			pDC->SelectObject(sPen);
// 			return;
// 		}

// 		rStep = float(rs - re)/float(nHeight);
// 		gStep = float(gs - ge)/float(nHeight);
// 		bStep = float(bs - be)/float(nHeight);

// 		if (!nWidth)
// 		{
// 			for (int ii = 0; ii < nHeight; ++ii)
// 			{
// 				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
// 				sPen = pDC->SelectObject(&cPen);

// 				pDC->MoveTo(drawRC.left, drawRC.top + ii);
// 				pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
// 				pDC->SelectObject(sPen);
// 			}
// 		}
// 		else
// 		{
// 			for (int ii = 0; ii < nHeight; ++ii)
// 			{
// 				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);

// 				CBrush brush;
// 				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
// 				pDC->FillRect(&rect, &brush);
// 				brush.DeleteObject();
// 			}
// 		}
// 	}
// }

void CGridWnd::Assign(CGridWnd* pGrid)
{
//	Reset();
	////int* nRowCount = nullptr;
	//int temp = 0;
 //       nRowCount = &temp;

	const int nActive = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ACTIVE));
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
	//pGrid->GetData(m_inters, nRowCount);	//m_inters의 row개수를 반환

	const int nRowCount = GetRowcount();
	for(int ii=1 ; ii < nRowCount ; ii++)
	{
		insertRow(ii, FALSE);
	}

	MarkerSetup();


}

void CGridWnd::GetHearder(CArray< _gridHdr, _gridHdr >& arHeader)
{
	arHeader.Copy(m_gridHdrX);
}

//void CGridWnd::GetData(CArray <_intersx*, _intersx*>& arData)
//{
//	const int ncnt = arData.GetSize();
//	const int nTot = gsl::narrow_cast<int>(m_inters.size());
//	int nMax = 0;
//
//	struct _inters*	pInter1 = nullptr;
//	struct _inters*	pInter2 = nullptr;
//
//	if ( ncnt >= nTot ) nMax = nTot;
//	else nMax = ncnt;
//
//	for ( int ii = 0 ; ii < nMax ; ii++ )
//	{
//		pInter1 = arData.GetAt(ii);
//		auto& pInter2 = m_inters.at(ii);
//		memcpy(pInter1, pInter2, sizeof(struct _intersx));
//	}
//}

//void CGridWnd::GetData(CArray <_inters*, _inters*>& arData, int* nRowCount)
//{
//	const int	ncnt = arData.GetSize();
//	const int nTot = m_inters.GetSize();
//	int nMax = 0;
//	char tCode[12]{};
//	int rCnt = 0;
//
//	struct _inters*	pInter1 = nullptr;
//	struct _inters*	pInter2 = nullptr;
//
//	if ( ncnt >= nTot ) nMax = nTot;
//	else nMax = ncnt;
//
//	for (int ii = 0 ; ii < nMax ; ii++ )
//	{
//		pInter1 = arData.GetAt(ii);
//		pInter2 = m_inters.GetAt(ii);
//
//		ZeroMemory(tCode, sizeof(tCode));
//		CopyMemory(tCode, m_inters.GetAt(ii)->code, sizeof(tCode));
//
//		if(CString(tCode).IsEmpty() == false)
//		{
//			rCnt++;
//		}
//
//		memcpy(pInter1, pInter2, sizeof(struct _inters));
//	}
//
//	*nRowCount = rCnt;
//}

void CGridWnd::toggleAction(bool toggle)
{
// 	if (!GetKind())
// 		return;

	m_bExpect = toggle;

	const struct	_symbol {
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

	//2011.12.27 KSJ 수정본 업종일때는 심볼바꾸지 안도록 수정하기 위해
	CString strCode;

	if (toggle)		// 각 Row마다 코스피, 코스닥이면 헤더를 변경하고 심볼을 변경한다.
	{
		_gridHdr xgridHdr;

		for(int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);

			if(strCode.GetLength() == 6 || strCode.GetLength() == 8)	//코스피, 코스닥
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						xgridHdr = m_gridHdrX.GetAt(jj);

						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb1))
							continue;

						//2013.07.08 SetItemSymbol로 하면 심볼이 바뀌지 않아서 메모리카피로 변경함.
						//m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb2);

						CopyMemory(xgridHdr.symbol, chksym[ii].symb2, 4);
						xgridHdr.stid = chksym[ii].stid2;

						m_gridHdrX.SetAt(jj, xgridHdr);
						break;
					}
				}
			}
		}
	}
	else		// false일때는 무조건 현재가로 바꿔준다.
	{
		_gridHdr xgridHdr;

		for(int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);

			if(strCode.GetLength() == 6 || strCode.GetLength() == 8)	//코스피, 코스닥
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						xgridHdr = m_gridHdrX.GetAt(jj);

						//2013.07.08 chksym[ii].symb2와 비교해야함.
						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb2))
							continue;

						//2013.07.08 SetItemSymbol로 하면 심볼이 바뀌지 않아서 메모리카피로 변경함.
						//m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb1);

						CopyMemory(xgridHdr.symbol, chksym[ii].symb1, 4);
						xgridHdr.stid = chksym[ii].stid1;

						m_gridHdrX.SetAt(jj, xgridHdr);
						break;
					}
				}
			}
		}
	}

	initialGrid(false);
	//sendTransaction();
	// KSJ

/*
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
//	sendTransaction();
*/
}

void CGridWnd::toggleAction5000(bool toggle)
{
	// 	if (!GetKind())
	// 		return;

	m_bExpect = toggle;

	const struct	_symbol {
		UINT	stid1;
		char*	symb1;
		UINT	stid2;
		char*	symb2;
	} chksym [] = {
		{ IDS_GH_CURR,	"2023",	IDS_GH_ANTIPRC,	"811" },
		{ IDS_GH_DIFF,	"2024",	IDS_GH_DIFF,	"813" },
		{ IDS_GH_RATE,	"2033",	IDS_GH_RATE,	"814" },
		{ IDS_GH_VOL,	"2027",	IDS_GH_ANTIVOL,	"812" },
	};
	const	int chksymC = sizeof(chksym) / sizeof(_symbol);

	//2011.12.27 KSJ 수정본 업종일때는 심볼바꾸지 안도록 수정하기 위해

	CString strCode;

	if (toggle)		// 각 Row마다 코스피, 코스닥이면 헤더를 변경하고 심볼을 변경한다.
	{
		_gridHdr xgridHdr;

		for(int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);

			if(strCode.GetLength() == 6)	//코스피, 코스닥
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						xgridHdr = m_gridHdrX.GetAt(jj);

						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb1))
							continue;

						//2013.07.08 SetItemSymbol로 하면 심볼이 바뀌지 않아서 메모리카피로 변경함.
						//m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb2);

						CopyMemory(xgridHdr.symbol, chksym[ii].symb2, 4);
						xgridHdr.stid = chksym[ii].stid2;

						m_gridHdrX.SetAt(jj, xgridHdr);
						break;
					}
				}
			}
		}
	}
	else		// false일때는 무조건 현재가로 바꿔준다.
	{
		_gridHdr xgridHdr;

		for(int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);

			if(strCode.GetLength() == 6)	//코스피, 코스닥
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						xgridHdr = m_gridHdrX.GetAt(jj);

						//2013.07.08 chksym[ii].symb2와 비교해야함.
						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb2))
							continue;

						//2013.07.08 SetItemSymbol로 하면 심볼이 바뀌지 않아서 메모리카피로 변경함.
						//m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb1);

						CopyMemory(xgridHdr.symbol, chksym[ii].symb1, 4);
						xgridHdr.stid = chksym[ii].stid1;

						m_gridHdrX.SetAt(jj, xgridHdr);
						break;
					}
				}
			}
		}
	}

	initialGrid(false);
	//sendTransaction();
	// KSJ

	/* 2011.12.27 원본 업종일때는 심볼바꾸지 안도록 수정하기 위해


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
	//	sendTransaction();
	*/
}

void CGridWnd::toggleAction2(bool toggle)
{
	if (!GetKind())
		return;

	m_bDiff = toggle;

	const struct	_symbol {
		UINT	stid1;
		char*	symb1;
		UINT	stid2;
		char*	symb2;
	} chksym [] = {
		{ IDS_GH_CURR,	"2023",	IDS_GH_CURR,	"2023" },
		{ IDS_GH_DIFF,	"2024",	IDS_GH_RATE,	"2033" },
		{ IDS_GH_VOL,	"2027",	IDS_GH_VOL,	"2027" },
	};
	const	int chksymC = sizeof(chksym) / sizeof(_symbol);

	const struct _symbol2{
		UINT attrx1;
		UINT attrx2;
	} chksym2[] = {
		{ GVAT_COMMA|GVAT_CONTRAST, GVAT_CONDITION|GVAT_SIGNx }
	};

	//2011.12.27 KSJ 수정본 업종일때는 심볼바꾸지 안도록 수정하기 위해

	CString strCode;

	if (toggle)		// 각 Row마다 코스피, 코스닥이면 헤더를 변경하고 심볼을 변경한다.
	{
		_gridHdr xgridHdr;

		for(int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);

			if(strCode.GetLength() == 6)	//코스피, 코스닥
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						xgridHdr = m_gridHdrX.GetAt(jj);

						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb1))
							continue;

						//2013.07.08 SetItemSymbol로 하면 심볼이 바뀌지 않아서 메모리카피로 변경함.
						//m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb2);

						CopyMemory(xgridHdr.symbol, chksym[ii].symb2, 4);
						xgridHdr.stid = chksym[ii].stid2;

						if(ii==1)
							xgridHdr.attrx = chksym2[0].attrx2;

						m_gridHdrX.SetAt(jj, xgridHdr);
						break;
					}
				}
			}
		}
	}
	else		// false일때는 무조건 현재가로 바꿔준다.
	{
		_gridHdr xgridHdr;

		for(int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);

			if(strCode.GetLength() == 6)	//코스피, 코스닥
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						xgridHdr = m_gridHdrX.GetAt(jj);

						//2013.07.08 chksym[ii].symb2와 비교해야함.
						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb2))
							continue;

						//2013.07.08 SetItemSymbol로 하면 심볼이 바뀌지 않아서 메모리카피로 변경함.
						//m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb1);

						CopyMemory(xgridHdr.symbol, chksym[ii].symb1, 4);
						xgridHdr.stid = chksym[ii].stid1;

						if(ii==1)
							xgridHdr.attrx = chksym2[0].attrx1;

						m_gridHdrX.SetAt(jj, xgridHdr);
						break;
					}
				}
			}
		}
	}

	initialGrid(false);
	//sendTransaction();
	// KSJ

	/* 2011.12.27 원본 업종일때는 심볼바꾸지 안도록 수정하기 위해

	if (toggle)
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

				if(ii==1)
					gridHdr.attrx = chksym2[0].attrx2;

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

				if(ii==1)
					gridHdr.attrx = chksym2[0].attrx1;

				m_gridHdrX.SetAt(jj, gridHdr);
				break;
			}
		}
	}

	initialGrid(false);
//	sendTransaction();
	*/
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
	char	key{};
	_trkey* trkey = (struct _trkey*)&key;
//	_trkey* trkey = (struct _trkey*)&((CGroupWnd*)m_pGroupWnd)->m_pTrkey;

	trkey->kind = TRKEY_NEWS;
	trkey->group = m_nIndex;
	sdata.SetData("PIBO2022", key, (char*)senddata.operator LPCTSTR(), senddata.GetLength(), "");	//2012.07.17 KSJ pibo --> pibf 2013.08.05 원복
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
		char    nrec[4]{};
		struct grid	grid[1]{};
	}*mod{};

	char*	buf = (char*)data->m_lParam;
	mod = (struct mod*)buf;
	CString	nrec, code;
	int	ncnt = 0;
	const int	nsize = sizeof(struct grid);
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
			m_inters.at(ii).get()->code = code;
		}
	}

	sendTransaction();
}

void CGridWnd::parsingNews(CString datB)
{
	int	idx = 0;
	CString	code = _T(""), szTitle = _T(""), symbol = _T(""), entry = _T(""), szKey = _T("");

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

		if (symbol.Compare("016") == 0)
		{
			szKey = entry;
			break;
		}

		if (symbol.Compare("301") == 0)
		{
			code = entry;
			code.TrimRight();
		}
	}
	//TRACE("GRID : %d CODEX : %s \n",this,code);
	if (!code.IsEmpty())
	{
		const int	ncnt = m_arExist.GetCount();
		int	nIndex = 0;
		//struct _inters*	pInter = nullptr;

		//TRACE("GRID : %d COUNT : %d \n",this,ncnt);

// 		if(m_arExist.IsExist("A" + code))
// 			TRACE("KEY EXIST!!!");

		nIndex = atoi(m_arExist.GetData("A" + code));
		/*pInter = m_inters.GetAt(nIndex);*/

		//TRACE("EXIST : %d INTERS : %s\n", nIndex,pInter->code);

		/*if (strcmp(pInter->code, (char*)code.operator LPCTSTR()) == 0)*/
		if(GetInter(code) > -1)
		{
			CString str;
			str.Format("015\t%s\n016\t%s\n1301\t%s", szTitle, szKey, code);

			newsFms.SetAt(code,str);
			//TRACE("THIS : %d CODEX : %s \n",this,code);

			const LPARAM	lParam = m_grid->GetItemData(nIndex + 1, colSIG);
			const WORD	low = LOWORD(lParam);
			const WORD	high = 1;
			m_grid->SetItemData(nIndex + 1, colSIG, MAKELPARAM(low, high));
			if (m_bWaveApply) PlayWave();

			//TRACE("NAME : %s CODE : %s INDEX : %d\n",pInter->name,pInter->code,nIndex + 1);
			//				break;		-> 종목 중복 등록 허용으로 인해 수정
		}
	}
}

//2012.01.19 KSJ Alertx 추가

void CGridWnd::parsingNewsx(DWORD* data)
{
	CString	code = _T(""), szTitle = _T(""), szKey = _T("");

	if(data[15])
		szTitle = (char*)data[15];

	if(data[301])
		code = (char*)data[301];

	if(data[16])
		szKey = (char*)data[16];

	code.TrimLeft(); code.TrimRight();

	if (!code.IsEmpty())
	{
		const int	ncnt = m_arExist.GetCount();
		int	nIndex = 0;

		nIndex = atoi(m_arExist.GetData("A" + code));
		if(GetInter(code) > -1)
		{
			//2012.03.20 KSJ 뉴스나오지 않는 현상 수정함..
			CString str;
			str.Format("015\t%s\n016\t%s\n1301\t%s", szTitle, szKey, code);

			newsFms.SetAt(code,str);
			// KSJ

			const LPARAM	lParam = m_grid->GetItemData(nIndex + 1, colSIG);
			const WORD	low = LOWORD(lParam);
			const WORD	high = 1;
			m_grid->SetItemData(nIndex + 1, colSIG, MAKELPARAM(low, high));
			if (m_bWaveApply) PlayWave();
		}
	}
}

//2012.01.19 KSJ Alertx 추가 끝

int CGridWnd::GetInter(CString code)
{
	for(int i=0;i< gsl::narrow_cast<int>(m_inters.size()); i++)
	{
		auto& pInter = m_inters.at(i);
		if (pInter.get()->code.CompareNoCase(code) == 0)
		{
			return i;
		}
		//2013.08.12 KSJ 신주인수권 종목 같은 경우는 조회나 실시간으로 종목코드를 받아올때 J, K, 같은 코드가 안붙어 오지만
		//실제종목파일에는 붙어있기 때문에 같지 않은경우는 문자열검색을 한번 더 검색해준다.
		else if(strstr(pInter.get()->code, (char*)code.operator LPCTSTR()) != nullptr)
		{
			return i;
		}
	}
	return -1;
}

CString CGridWnd::GetNewsData(CString code)
{
	CString str;
	//TRACE("%d\n",this);
	if(newsFms.Lookup(code,str))
	{
		newsFms.RemoveKey(code);
	}

	return str;
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
	const int	ncnt = m_arExist.GetCount();
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
			if ( nindex >= 0 )
			{
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
	const int	ncnt = m_gridHdrX.GetSize();
	struct _gridHdr	xgridHdr;
	int	ret = -1;

	for ( int ii = colCURR + 1 ; ii < ncnt ; ii++ )
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		if (symbol.CompareNoCase(xgridHdr.symbol) == 0)
		{
			ret = ii;
			break;
		}
	}

	return ret;
}


CString CGridWnd::CalcuPyungaSonik(struct _intersx* pinters, CString curr)
{
	CString	ret = _T("");
	const double	dval1 = atof(pinters->xnum);
	const double	dval2 = atof(pinters->xprc);
	const double	dval3 = IH::TOfabs(curr);
	const double	maeipPrc = pinters->maeipPrc;
	const double  creditPrc = pinters->creditPrc;
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

CString CGridWnd::CalcuSuik(struct _intersx* pinters, CString curr)
{
	CString	ret = _T("");
	const double	dval1 = atof(pinters->xnum);
	const double	dval2 = atof(pinters->xprc);
	const double	dval3 = IH::TOfabs(curr);
	const double  creditPrc = pinters->creditPrc;
	const double	maeipPrc = pinters->maeipPrc;
	double	result = 0;
	const double resultVal = 0;
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

CString CGridWnd::CalFutureEvalPrice(struct _intersx* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian)
{
	//	if (!m_dBase)
	getBase(m_code);

	const double maeipPrc = pinters->maeipPrc;	//원장에서 받은 매입금액

	double dSonik = 0, dGubn = 1;
	//dTemp = 0.;
	bool	bMd = false;
	CString m_pprice, m_pgsonik;
	double m_pmaeip = 0;

	if(m_curr == 0)
	{
		return "";
	}

	const int kind = getJKind(m_code);
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

	//TRACE("현재가 : %f * 잔고 : %f * 기본단위 : %f = %s, 매입금액 = %f,  \n", m_curr, m_remian, m_dBase, m_pprice, m_pmaeip);
	//	TRACE("현재가 : %f 기본단위 : %f 평가금액 : %s - 매입금액 : %f = 평가손익 : %f\n", m_curr, m_dBase, m_pprice, m_pmaeip, dSonik);
	if (dSonik > 0)
		m_pgsonik.Format("+%.0f", dSonik);
	else if (dSonik < 0)
		m_pgsonik.Format("-%.0f", dSonik);

	return m_pgsonik;
}

CString CGridWnd::CalFutureEvalRate(struct _intersx* pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian)
{
	getBase(m_code);

	const double maeipPrc = pinters->maeipPrc;	//원장에서 받은 매입금액

	double dSonik = 0, dGubn = 1;
	bool	bMd = false;
	CString m_pprice, m_pgsonik, m_suik;
	double m_pmaeip = 0;

	if(m_curr == 0)
	{
		return "";
	}

	const int kind = getJKind(m_code);
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

// 		m_pTicker->SetFixBkColor(color);
// 		m_pTicker->SetGridColor(GetAxColor(65));

		m_grid->Invalidate();
		//m_pTicker->Invalidate();

// 		m_btDomino.Invalidate(TRUE);
// 		m_btSAVE.Invalidate(TRUE);
// 		m_btCLOSE.Invalidate(TRUE);
		// END ADD

		initialGrid(false);//BUFFET
	}
}

void CGridWnd::ReIndexSpecial()
{
	const int	ncnt = m_gridHdrX.GetSize();
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

// 	m_pTicker->m_bMKClrApply = m_bMKClrApply;
// 	m_pTicker->m_clrKospi  = m_clrKospi;
// 	m_pTicker->m_clrKosdaq = m_clrKosdaq;
}

void CGridWnd::SetDispCode(BOOL bDispCode)
{
	int nWidth = 0;
	m_bDispCode = bDispCode;

	if (m_bDispCode) nWidth = m_nCodeWidth;

	m_grid->SetColumnWidth(colCODE, nWidth);
	m_grid->ResetScrollBars();
	m_grid->Invalidate();

// 	m_pTicker->SetColumnWidth(colCODE, nWidth);
// 	m_pTicker->ResetScrollBars();
// 	m_pTicker->Invalidate();

	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);
}

void CGridWnd::MultiChart()
{
	CString strData(_T(""));
	strData.Format("%s /S/t0/d%s\t", mchartMAP, MCSYMBOL);

	const int nCount = m_grid->GetRowCount();

	for (int i = m_grid->GetFixedRowCount(); i < nCount; i++)
	{
		if (!m_grid->IsSelectedRow(i))
			continue;

		strData += m_grid->GetItemText(i, colCODE);
	}

	openView(typeVIEW, strData);
}

// void CGridWnd::ShowRTSWnd()
// {
// 	m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RTSVISIBLE, ((CMainWnd*)m_pMainWnd)->GetRTSVisible() ? MO_HIDE : MO_SHOW));
// 	((CMainWnd*)m_pMainWnd)->SetRTSTreeData(this);
// }

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

int CGridWnd::GetRowHeight()
{
	return m_grid->GetRowHeight(0);
}

int CGridWnd::GetGroupCount(int gno)
{
	//int result = 0;
	//struct _inters* pInters{};
	//const int nInterCnt = gsl::narrow_cast<int>(m_inters.size());
	//CString tempStr;
	//const int pos = -1;
	//CString	filePath, fileBook;
	//bool	endfile = false;

	//filePath.Format("%s/%s/%s/%s%02d", m_root, USRDIR, m_user, FILE_SUBDATA, gno);

	//if (!ExistFile(gno))
	//{
	//	return 0;
	//}

	//CFile	fileH;
	//UINT	readL{};

	//fileH.Open(filePath, CFile::modeRead);

	//for (int ii = 0; ii < MAX_LINE; ii++)
	//{
	//	pInters = (_inters *) new char[sz_inters];
	//	ZeroMemory(pInters, sz_inters);

	//	if (!endfile)
	//	{
	//		readL = fileH.Read(pInters, sz_inters);
	//
	//		if (readL < sz_inters)
	//		{
	//				endfile = true;
	//		}
	//		else
	//		{
	//			result++;
	//		}

	//	}
	//}
	//fileH.Close();

	return 0;
}

int	CGridWnd::GetScrollCtrl()
{
//	return m_grid->GetGridScroll();
	return m_grid->GetHscroll();
}

int	CGridWnd::GetGridWidth()
{
	const int childWidth = m_grid->GetGridWidth();

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
	if(row> m_recomArray.GetSize() || row > 120)
		return "";
	return m_recomArray.GetAt(row-1);
}

void CGridWnd::SetRecommandInfo(int row, CString strData)
{
	if(row> m_recomArray.GetSize() || row > 120)
	{

	}
	else
		m_recomArray.SetAt(row-1, strData);
}

void CGridWnd::InsertAtRecommandInfo(int row, CString strData)
{
	if(row> m_recomArray.GetSize() || row > 120)
	{

	}
	else
		m_recomArray.InsertAt(row-1, strData);
}

void CGridWnd::ArrangeField(int type, CString save[100][2])
{
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	int maxRow = 0;

	if(nOver == MO_VISIBLE)
	{
		maxRow = GetRowcount();
	}
	else
	{
		maxRow = 100;
	}


	CString strTemp;

	for(int i=0 ; i<100 ; i++)
	{
		save[i][0] = "";
		save[i][1] = "";
	}

	const int nArrange = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);

	//일단 그리드의 종목과 정렬할 값들을 담아온다
	for(int i=0 ; i<maxRow ; i++)
	{
		CString strCode = m_grid->GetItemText(i+1, colCODE);

		if(nArrange != 0 && strCode != "" && strCode.GetAt(0) == 'm')
			continue;

		save[i][0] = strCode;

		if(type == 0)										//등록순
		{
		}
		else if(type == 1)									//업종순
		{
			save[i][1] = m_grid->GetItemText(i+1, m_upjongCode);
		}
		else if(type == 2)									//종목명순
		{
			save[i][1] = m_grid->GetItemText(i+1, colNAME);
		}
		else												//종목코드순
		{
			save[i][1] = m_grid->GetItemText(i+1, colCODE);
		}
	}
}

void CGridWnd::SendWhenSortVisibleMode(CString data[MAX_LINE][2])
{
	m_pGridArray.RemoveAll();

	char tempB[64]{};

	//////////////////////////////////////
	struct	_gridHdr xgridHdr;
	for (int ii = 0 ; ii < m_gridHdrX.GetSize() ; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		sprintf(tempB, "%s%c", xgridHdr.symbol, P_NEW);
		m_pGridArray.Add(tempB);
	}

	//////////////////////////////////////
	// 	CWnd* m_pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	// 	m_pWnd->SendMessage(WM_MANAGE, MK_GETTREEITEM);

	if(m_pGridArray.GetSize() > GRIDMAXNUM)
	{
		const int nRem = m_pGridArray.GetSize() - GRIDMAXNUM;

		if(!m_bContinue && !m_bSecond)
		{
			m_bContinue = TRUE;
			m_bSort = TRUE;

			SendWhenSortVisibleModeTR(data,0,GRIDMAXNUM);
		}
	}
	else
	{
		m_bContinue = FALSE;
		m_bSingle = TRUE;
		m_bSort = TRUE;

		SendWhenSortVisibleModeTR(data,0,m_pGridArray.GetSize());
	}
}

void	CGridWnd::SendWhenSortVisibleModeTR(CString data[MAX_LINE][2],int nStart,int nEnd)
{
	//int	sendL = 0;
	//char	tempB[64]{};
	//const int	bufSize = sizeof(struct _gridHi) + 50 + m_gridHdrX.GetSize()*5 + m_inters.size()*12 + 12;
	//char	*sendB = new char[bufSize];
	//
	//FillMemory(sendB, bufSize, ' ');
	//
	//const BOOL	bExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
	//m_bAutoExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);
	//
	//if(bExpect)
	//{
	//	sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
	//}
	//else
	//{
	//	if(m_bAutoExpect)
	//	{
	//		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
	//	}
	//	else
	//	{
	//		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 0, P_TAB);
	//	}
	//}
	//
	//CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	//sendL += strlen(tempB);
	//
	//sprintf(tempB, "%s%c", gSYMBOL, P_DELI);
	//CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	//sendL += strlen(tempB);
	//
	//struct	_gridHi* gridHi;
	//
	//gridHi = (struct _gridHi *) &sendB[sendL];
	//sendL += sizeof(struct _gridHi);
	//
	//CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	//sprintf(tempB, "%04d", m_inters.size());
	//CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));
	//
	//gridHi->type = '0';
	//gridHi->dir  = '1';
	//gridHi->sort = '0';

// 	struct	_gridHdr gridHdr;
// 	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
// 	{
// 		gridHdr = m_gridHdrX.GetAt(ii);
//
// 		sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
// 		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
// 		sendL += strlen(tempB);
// 	}
//	for (int ii = nStart ; ii < nEnd ; ii++)
//	{
//		//gridHdr = m_gridHdrX.GetAt(ii);
//
//		//sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
//		strcpy(tempB,(LPTSTR)(LPCTSTR)m_pGridArray.GetAt(ii));
//		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//		sendL += strlen(tempB);
//	}
//
//	sprintf(tempB, "2321%c", P_NEW);					//전일거래량
//	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//	sendL += strlen(tempB);
//
//	sprintf(tempB, "2022%c", P_NEW);					//새로 추가된 심볼의 현재가
//	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//	sendL += strlen(tempB);
//
//	sprintf(tempB, "1910%c", P_NEW);					//추천일자
//	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//	sendL += strlen(tempB);
//
//	sprintf(tempB, "1911%c", P_NEW);					//추천종목 기준가
//	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//	sendL += strlen(tempB);
//
//// 	sprintf(tempB, "1912%c", P_NEW);					//수익률
//// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//// 	sendL += strlen(tempB);
////
//// 	sprintf(tempB, "1913%c", P_NEW);					//추천종목 기준 BM지수
//// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//// 	sendL += strlen(tempB);
////
//// 	sprintf(tempB, "1914%c", P_NEW);					//BM수익률
//// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//// 	sendL += strlen(tempB);
//
//	sprintf(tempB, "1915%c", P_NEW);					//지수 구분
//	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//	sendL += strlen(tempB);
//
//	sprintf(tempB, "%s%c", CCOD, P_DELI);				// 종목코드
//	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//	sendL += strlen(tempB);
//
//
//
//	int	nInterCnt = 0;
//
//	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
//	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
//
//	if(nOver == MO_VISIBLE)
//	{
//		nInterCnt = 100;
//	}
//	else
//	{
//		nInterCnt = 100;
//	}
//
//	CString code;
//	int	ncnt = 0;
//	CString tempStr;
//	int pos = -1;
//
//	for (int ii = 0; ii < nInterCnt; ii++)
//	{
// 		code = data[ii][0];
//
//		if (strlen(code))
//		{
//			tempStr = code;
//
//			if(ii == 0 && ((CGroupWnd*)m_pGroupWnd)->m_commIndex < 2)
//			{
//				((CGroupWnd*)m_pGroupWnd)->m_commInfo[((CGroupWnd*)m_pGroupWnd)->m_commIndex][0] = tempStr;
//			}
//
//			pos = tempStr.Find(" ");
//			if(pos != -1)
//			{
////				strcpy(temp, tempStr.Mid(0, pos));
//				code = tempStr.Mid(0, pos);
//			}
//
//			ncnt++;
//		}
//
//		if(strlen(code) > 0)
//		{
//			sprintf(tempB, "%s%c", code.GetString(), P_DELI);
//			CopyMemory(&sendB[sendL], tempB, strlen(tempB));
//			sendL += strlen(tempB);
//		}
//
//	}
//
//	sendB[sendL] = P_NEW;
//	sendL += 1;
//	sendB[sendL] = P_TAB;
//	sendL += 1;
//	sendB[sendL] = 0x00;
//	CSendData	ssdata;
//	char key;
//	key = 0;
//	const _trkey* trkey = (struct _trkey*)&key;
//	//	struct _trkey* trkey = (struct _trkey*)&((CGroupWnd*)m_pGroupWnd)->m_pTrkey;
//
//	m_UpdateGroup = -1;
//
//	m_SendKey = TRKEY_GRIDNEW;
//
//
//
//	if(((CGroupWnd*)m_pGroupWnd)->m_commIndex < 2)
//	{
//		CString strTemp;
//		strTemp.Format("%d", m_nIndex);
//
//		((CGroupWnd*)m_pGroupWnd)->m_commInfo[((CGroupWnd*)m_pGroupWnd)->m_commIndex][1] = strTemp;
//
//		strTemp.Format("%d", trkey->kind);
//		((CGroupWnd*)m_pGroupWnd)->m_commInfo[((CGroupWnd*)m_pGroupWnd)->m_commIndex][2] = strTemp;
//
//		((CGroupWnd*)m_pGroupWnd)->m_commIndex++;
//	}
//
//	m_updateROW = -1;
//
//	ssdata.SetData("pooppoop", key, sendB, sendL, "");
//
//	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&ssdata);
//	m_endsort = false;
//
//	delete[] sendB;
}

void CGridWnd::killFocus()
{
	m_grid->ResetSelectedRange();
	m_grid->SetFocusCell(-1, -1);
}

void CGridWnd::SetRatio(double x, double y)
{
	m_grid->m_xRate = x;
	m_grid->m_yRate = y;
}

void CGridWnd::SetFontSize(int size)
{
	m_grid->SetFontSize(size);
}

void CGridWnd::uploadOK()
{
	// 2017.02.21 마지막 'G'에만 'V' 보내기
	//if (!m_bLastSend)
	//{
	//	m_bLastSend = TRUE;
	//	return;
	//}

	int sendL = 0;
	std::string sendB;
	sendB.resize(1024 * 16);
	char tempB[32]{};


	//int	sendL = 0;
	//char	sendB[16 * 1024]{}, tempB[32]{};
	////	char	strUinfo[500];

	struct _uinfo* uinfo = (struct _uinfo *)&sendB[sendL];
	sendL += sz_uinfo;

	FillMemory((char *) uinfo, sz_uinfo, ' ');

	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'V';

	sprintf(tempB, "00000");
	CopyMemory(uinfo->nblc, tempB, sizeof(uinfo->nblc));

	uinfo->retc[0] = 'O';

	sendB[sendL] = 0x00;

	//	SendTR
	CSendData	sdata;
	char	key;
	key = 0;
	_trkey* trkey = (struct _trkey*)&key;

	trkey->group = m_nIndex;
	trkey->kind = TRKEY_LASTSET;

	sdata.SetData(trUPDOWN, key, sendB.data(), sendB.size(), "");

	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
}

BOOL CGridWnd::DataCheck(char* ptr)
{
	if (CString(ptr).IsEmpty())
		return FALSE;
	return TRUE;
}


void CGridWnd::initRSymbol()
{
	_mRsymbol.clear();
	_mRsymbol.emplace(std::make_pair(0, 0));
	_mRsymbol.emplace(std::make_pair(950, 950));
	_mRsymbol.emplace(std::make_pair(951, 951));
	_mRsymbol.emplace(std::make_pair(32, 32));
	_mRsymbol.emplace(std::make_pair(31, 31));
	_mRsymbol.emplace(std::make_pair(30, 30));
	_mRsymbol.emplace(std::make_pair(29, 29));
	_mRsymbol.emplace(std::make_pair(27, 27));
	_mRsymbol.emplace(std::make_pair(23, 23));
	_mRsymbol.emplace(std::make_pair(24, 24));
	_mRsymbol.emplace(std::make_pair(111, 111));
	_mRsymbol.emplace(std::make_pair(112, 112));
	_mRsymbol.emplace(std::make_pair(115, 115));
	_mRsymbol.emplace(std::make_pair(116, 116));
	_mRsymbol.emplace(std::make_pair(33, 33));
	_mRsymbol.emplace(std::make_pair(34, 34));
	_mRsymbol.emplace(std::make_pair(40, 40));
	_mRsymbol.emplace(std::make_pair(734, 734));
	_mRsymbol.emplace(std::make_pair(740, 740));
	_mRsymbol.emplace(std::make_pair(15, 15));
	_mRsymbol.emplace(std::make_pair(301, 301));
	_mRsymbol.emplace(std::make_pair(16, 16)); 

	const int countX = m_gridHdrX.GetSize();
	//for (int jj = col7852; jj < countX; jj++)
	for (int jj = 2; jj < countX; jj++)
	{
		const _gridHdr xgridHdr = m_gridHdrX.GetAt(jj);
		CString sSymbol = CString(xgridHdr.symbol, strlen(xgridHdr.symbol));

		//외인소진율은 실시간이 아니다.
		if (sSymbol.CompareNoCase("2204") == 0 || sSymbol.IsEmpty())
			continue;

		sSymbol = sSymbol.Right(3);
		const int symbol = atoi(sSymbol);
		if (symbol > 0 && jj > 22)
		{
			_mRsymbol.emplace(std::make_pair(symbol, symbol));
		}
	}


}

void CGridWnd::getSeverMemoCheck(CArray<bool, bool>& parr)
{
	int ii = 0;
	if (CAST_TREEID(m_kind)->kind == xISSUE)
		return;
	for_each(m_inters.begin(), m_inters.end(), [&](auto& pinters) {
		if (pinters.get()->bMemo == "Y")
			parr.SetAt(ii, true);
		ii++;
	});
}

void CGridWnd::setSeverMemoCheck(int xrow, bool bflag)
{
	if (CAST_TREEID(m_kind)->kind == xISSUE)
		return;

	auto& pinters = m_inters.at(xrow);
	pinters.get()->bMemo = bflag ? "Y" : "N";
}