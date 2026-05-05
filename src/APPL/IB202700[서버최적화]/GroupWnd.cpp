// GroupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202700.h"
#include "GroupWnd.h"
#include "MainWnd.h"
#include "multimon.h"
#include "DlgAutoSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "GridWnd.h"
#include "UsefulSetupDlg.h"
#include <math.h>
/////////////////////////////////////////////////////////////////////////////
// CGroupWnd
#define deliMITER ";"

CGroupWnd::CGroupWnd(CWnd *pMainWnd) : CBaseWnd(pMainWnd)
{
	_classname = "CGroupWnd";
	m_nGroup = 1;

	for (int ii = 0; ii < MAX_GROUP; ii++)
	{
		m_GridWnd[ii] = nullptr;
	}

	m_GridWndUpjn = nullptr;
	m_arRECT.RemoveAll();
	m_arDIVIDE.RemoveAll();
	m_arUpDIV.RemoveAll();
	m_arDnDIV.RemoveAll();
	m_bDivide = FALSE;
	m_nIndex = -1;
	m_nCurSel = 0;

	m_line = 1;
	m_rtmAction = rtmCELL;
	m_dblAction = dblNONE;
	m_selAction = 1;
	m_overlap = 0;
	m_overlapAlert = 0;

	m_linkMap = _T("");
	m_fontsize = 9;
	m_fontname = _T("굴림체");
	m_bCurr = TRUE;
	m_bMAP = FALSE;

	// ADD PSH 20070917
	m_bName = TRUE;
	m_bAllBold = FALSE;
	m_bWaveApply = FALSE;
	m_bAutoSave = FALSE;

	m_clrKospi = RGB(0, 0, 0);
	m_clrKosdaq = RGB(128, 64, 64);
	m_clrbookMark = RGB(255, 128, 255);
	m_clrbookMarkPen = RGB(0, 0, 0);
	m_bMKClrApply = FALSE;
	m_bPopupDlg = FALSE;
	m_bDispCode = FALSE;
	m_bNewsNChart = TRUE; // 2011.12.02	KSJ

	m_cntGroup = 0;

	m_iDragIn = 0;
	m_isRecommand = false;
	for (int ii = 0; ii < 30; ii++)
	{
		m_RecomGroup[ii] = -1;
	}

	m_bActiveArrange = FALSE;
	m_bAutoCloseSave = false;

	m_iTime = 0;
	// END ADD
}

CGroupWnd::~CGroupWnd()
{
}

BEGIN_MESSAGE_MAP(CGroupWnd, CBaseWnd)
//{{AFX_MSG_MAP(CGroupWnd)
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_LBUTTONDBLCLK()
ON_WM_SETCURSOR()
ON_WM_TIMER()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()

void CGroupWnd::SetDragInCount(int data)
{
	m_iDragIn = data;
}

void CGroupWnd::AddDragInCount()
{
	m_iDragIn++;
}

void CGroupWnd::SendUini()
{
	
}

void CGroupWnd::OperDraw(CDC *pDC)
{
	if (m_bMAP)
		DrawTitle(pDC);
	else
		DrawDivide(pDC);
}

void CGroupWnd::DrawTitle(CDC *pDC)
{
	pDC->FillSolidRect(m_rcTitle, RGB(255, 255, 0));
	const int nmode = pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_szTITLE, m_rcTitle, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	pDC->SetBkMode(nmode);
}

LONG CGroupWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG ret = 0;

	switch (LOWORD(wParam))
	{
	case MK_RTS_DATA:
	{
	
	}
	break;
	case MK_INTSAVE: // BUFFET
	{
		if ((int)lParam)
			m_bAutoSave = TRUE;

		OnDestroySave();
	}
	break;
	case MK_GROUPSAVE:
		saveWhenGroupClosed((int)lParam);
		break;
	case MK_SISECATCH:
	{
		for (int ii = 0; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, wParam, lParam);
		}
		// 2012.04.10 KSJ 업종 주석처리 사용하지 않음.
		// 			m_GridWndUpjn->SendMessage(WM_MANAGE, wParam, lParam);
	}
	break;
	case MK_MEMO:
	{
		for (int ii = 0; ii < m_nGroup; ii++)
		{
			if (ii == m_nCurSel)
				continue;
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_MEMO);
		}
	}
	break;
	case MK_DELETEMEMO:
		break;
	case MK_CLOSEGRID:
		CloseGrid(HIWORD(wParam));

		break;
	case MK_TURNCODE:
	{
		const int nsec = HIWORD(wParam);
		if (nsec == 0)
		{
			for (int ii = 0; ii < m_nGroup; ii++)
			{
				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_TURNCODE);
			}
		}
		else
		{
			m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TURNCODE, nsec));
		}
	}
	break;

	case MK_SETUPOK:
	{
		const int nRet = HIWORD(wParam);
		loadfield();


		for (int ii = 0; ii < m_nGroup; ii++)
		{
			if (m_GridWnd[ii])
				m_GridWnd[ii]->setMemo();
		}
		// 2016.12.21 KSJ 99면 그냥 설정한번 다시 읽는다.
		if (nRet == 99)
		{
			break;
		}
		else
		{
			for (int ii = 0; ii < m_nGroup; ii++)
			{
				if (m_RecomGroup[ii] == 1)
				{
				}
				else
				{
					m_GridWnd[ii]->FieldSetup(false);
					m_GridWnd[ii]->Invalidate(false);
					//					int kind = m_GridWnd[ii]->GetKind();

					//					if (CAST_TREEID(kind)->kind == xISSUE)
					//					{
					//						pWnd->SendMessage(WM_MANAGE, MK_SENDTREE);
					//					}
				}
			}

			m_nCurSel = 0;

			CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
			m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MK_TURNCODE);
			SelectOper();
			ReSetupGrid();
		}
	}
	break;
	case MK_CHANGEFIELD:
	{
		for (int ii = 0; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->FieldSetup(false);
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
		}
	}
	break;
	case MK_SELGROUP:
		switch (HIWORD(wParam))
		{
		case MO_SET:
		{
			CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
			m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MK_TURNCODE);
			if (m_nCurSel != (int)lParam)
			{
				m_nCurSel = (int)lParam;
				SelectOper();
			}

			const UINT kind = m_GridWnd[m_nCurSel]->GetKind();
			const int indx = CAST_TREEID(kind)->value;
			saveGroupIndex(indx);
			Sleep(20);
			savecfg();
		}
		break;
		case MO_GET:
			ret = (LONG)m_nCurSel;
			InvalidateDivide();
			break;
		}
		break;
	case MK_HAWKEYE:
	{
		for (int ii = 0; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_HAWKEYE);
		}
	}
	break;
	case MK_GETGROUP:
		switch (HIWORD(wParam))
		{
		case MO_ALL:
			ret = m_nGroup;
			break;
		case MO_ACTIVE:
			ret = m_nCurSel;
			break;
		case MO_LEFT:
			ret = m_nGroup - m_nCurSel;
			break;
		}

		break;
	case MK_GROUP:
		if (HIWORD(wParam))
			ret = GroupPlus((BOOL)lParam);
		else
			ret = GroupMinus((BOOL)lParam);


		//		InitGrid();
		RefreshGrid();
		break;
	case MK_ENDDRAG:
	{
		for (int ii = 0; ii < m_cntGroup; ii++)
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_ENDDRAG);
		}
	}
	break;
	case MK_GETMAX:
	{
		CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
		if (nOver == MO_VISIBLE)
		{
			//				ret = (m_nGroup - m_nCurSel) * GetRowCount();
			ret = (m_nGroup - m_nCurSel) * MAX_LINE;
		}
		else if (nOver == MO_FULL)
			ret = (m_nGroup - m_nCurSel) * MAX_LINE;
		else
			ret = MAX_LINE;
	}
	break;
	case MK_EXCEL:
	{
		m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MK_EXCEL);
	}
	break;
	case MK_EXPECT:
		ExpectOper(HIWORD(wParam));
		break;
	case MK_AUTOEXPECT:
		AutoExpectOper(HIWORD(wParam));
		break;
	case MK_EQUALIZER:
	{
		SetEqualizerField(HIWORD(wParam));
	}
	break;
	case MK_CHANGERECOMMANDFIELD:
	{
		m_isRecommand = true;
		SetEqualizerField(3);
	}
	break;
	case MK_SAVERECOMMANDGROUP:
		saveRecommandGroup();
		break;
	case MK_TREEDATA:
	{
		CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

		CGridData *pGridData = (CGridData *)lParam;
		int index = pGridData->_gridindex < 0 ? m_nCurSel : pGridData->_gridindex;
		
		m_nCurSel = min(m_nCurSel, m_cntGroup - 1);

		if (nOver == MO_VISIBLE)
			index = 0;

		SendGrid(index, pGridData); // 5
	}
	break;
	case MK_UPJONGDATA:
	{
	}
	break;
	case MK_RECVDATA:
		RecvOper(HIWORD(wParam), (CRecvData *)lParam);
		break;
	case MK_RTSDATA:
		//RecvRTS((CRecvData *)lParam);
		break;
	case MK_RTSDATAx:
	
		break;
	case MK_SELINT:
		SendGrid(m_nCurSel, (int)lParam);
		break;
	// ADD PSH 20070917
	case MK_MKPROP:
	{
		_marker *pMkProp = (_marker *)lParam;

		if (nullptr != pMkProp)
		{
			for (int ii = 0; ii < m_cntGroup; ii++)
			{
				m_GridWnd[ii]->SetMarkerProp(pMkProp);
			}
		}
	}
	break;
	case MK_TOOLBUTTON:
	{
		CRect *pRect = (CRect *)lParam;
		m_GridWnd[m_nCurSel]->ShowPopupMenu(pRect->left, pRect->top + 18);
	}
	break;
	case MK_AUTOSAVE:
	{
		CUsefulSetupDlg *pDlg = (CUsefulSetupDlg *)lParam;

		if (nullptr != pDlg)
		{
			for (int ii = 0; ii < m_cntGroup; ii++)
			{
				m_GridWnd[ii]->SetAutoSaveCnd(pDlg->m_bAddCnd, pDlg->m_bChgCnd);
			}
		}
	}
	break;
	case MK_ALERTWAVE:
	{
		CUsefulSetupDlg *pDlg = (CUsefulSetupDlg *)lParam;
		m_bWaveApply = pDlg->m_bWaveApply;
		m_strWaveFile = pDlg->m_strFile;

		for (int ii = 0; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->m_bWaveApply = m_bWaveApply;
			m_GridWnd[ii]->m_strWaveFile = m_strWaveFile;
		}

		m_pMainWnd->SendMessage(WM_MANAGE, MK_ALERTWAVE, lParam);
	}
	break;
	case MK_MARKETCOLOR:
	{
		CUsefulSetupDlg *pDlg = (CUsefulSetupDlg *)lParam;
		m_bMKClrApply = pDlg->m_bClrApply;
		m_clrKospi = pDlg->m_clrKospi;
		m_clrKosdaq = pDlg->m_clrKosdaq;

		for (int ii = 0; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->SetMarketColor(m_bMKClrApply, m_clrKospi, m_clrKosdaq);
			m_GridWnd[ii]->Invalidate(false);
		}

		m_pMainWnd->SendMessage(WM_MANAGE, MK_MARKETCOLOR, lParam);
	}
	break;
	case MK_MARKETCODE:
	{
		m_bDispCode = (BOOL)lParam;

		for (int ii = 0; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->SetDispCode(m_bDispCode);
			m_GridWnd[ii]->Invalidate(false);
		}

		m_pMainWnd->SendMessage(WM_MANAGE, MK_MARKETCODE, lParam);
	}
	break;
		// 2011.12.02	KSJ
	case MK_NEWSNCHART:
	{
		m_bNewsNChart = (BOOL)lParam;

		for (int ii = 0; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->SetNewsNChart(m_bNewsNChart);
			m_GridWnd[ii]->Invalidate(false);
		}

		m_pMainWnd->SendMessage(WM_MANAGE, MK_NEWSNCHART, lParam);
	}
	break;
		// KSJ

		// 2011.12.05	KSJ
	case MK_MARGIN:
	{
		m_bMargin = (BOOL)lParam;

		for (int ii = 0; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->SetMargin(m_bMargin);
			m_GridWnd[ii]->Invalidate(false);
		}

		m_pMainWnd->SendMessage(WM_MANAGE, MK_MARGIN, lParam);
	}
	break;
		// KSJ

		// 2011.12.05	KSJ
	case MK_INFO:
	{
		m_bInfo = (BOOL)lParam;

		for (int ii = 0; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->SetInfo(m_bInfo);
			m_GridWnd[ii]->Invalidate(false);
		}

		m_pMainWnd->SendMessage(WM_MANAGE, MK_INFO, lParam);
	}
	break;
		// KSJ

		// 2011.12.12	KSJ
	case MK_TICKER:
	{
		// m_bTicker = (BOOL)lParam;
		//
		// for ( int ii = 0 ; ii < m_nGroup ; ii++ )
		//{
		//	m_GridWnd[ii]->SetTicker(m_bTicker);
		//	m_GridWnd[ii]->Invalidate(false);
		// }
		//
		// m_pMainWnd->SendMessage(WM_MANAGE, MK_TICKER, lParam);
	}
	break;
		// KSJ

	case MK_INITGRID:
	{
		//선택된 그룹에 반응이 오도록
		//			InitGrid();
		RefreshGrid();
		//			SelectOper();
		//트리에 선택시켜 TR발생하게끔한다.
		//관심그룹 선택 방법 변경
		//			InitSetGroup();
		break;
	}
	case MK_SELECTFIRST:
		selectFirst();
		break;
	case MK_ARRANGE:
	{
		arrangeGroup((int)lParam);
	}
	break;
	case MK_SETARRANGEACTIVE:
	{
		m_bActiveArrange = FALSE;
	}
	break;
	case MK_CLEARGROUP:
	{
		for (int i = m_nCurSel + 1; i < m_nGroup; i++)
		{
			m_GridWnd[i]->SetKind(0);
			m_GridWnd[i]->RemoveAll();
		}
	}
	break;
	case MK_CLEARSECONDSEND:
	{
		if ((int)lParam == 1)
			m_RecomGroup[0] = 1;
		for (int i = 0; i < m_nGroup; i++)
		{
			m_GridWnd[i]->setBoolSecond(false);
		}
	}
	break;
	case MK_SAVECFG:
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			m_GridWnd[i]->SetGridinfo();
		}
	}
	break;
	// END ADD
	case MK_CLOSEAUTOSAVE:
	{
		int nLastEditGrid = 0;

		for (int ii = 0; ii < m_nGroup; ii++)
		{
			if (m_GridWnd[ii]->m_bEditWork)
			{
				m_GridWnd[ii]->m_bLastSend = FALSE;
				nLastEditGrid = ii;
			}
		}

		for (int ii = 0; ii < m_nGroup; ii++)
		{
			if (m_GridWnd[ii]->m_bEditWork)
			{
				if (nLastEditGrid == ii)
					m_GridWnd[ii]->m_bLastSend = TRUE;

				((CMainWnd *)m_pMainWnd)->IsChange(TRUE); // 2016.12.16 KSJ 변경되어 저장하면 세팅해준다.
				m_GridWnd[ii]->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BT_SAVE, 0), 0);
			}
		}
	}
	break;
	case MK_GROUPCHANGECHECK:
	{
		saveWhenGroupClosed((int)lParam);
		return ret;
	}
	break;

	case MK_MARKETRELOAD:
		for (int ii = 0; ii < m_nGroup; ii++)
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
		break;
	default:
		ret = CBaseWnd::OnManage(wParam, lParam);
		break;
	}
	return ret;
}

//테스트용
void CGroupWnd::testSaveFile(CString dt)
{
	return;
	CString strPath(_T("")), m_root, m_user;
	strPath.Format("c:\\test.ini");
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	CString titletime;

	titletime.Format("[%s]GroupWnd::RecvOper", strCurTime);

	WritePrivateProfileString("IB202700", titletime, (LPCTSTR)dt, (LPCTSTR)strPath);
}

// KR7005930003
void CGroupWnd::RecvOper(int kind, CRecvData *rdata)
{
	_trkey *trkey = (struct _trkey *)&kind;
	switch (trkey->kind)
	{
	case TRKEY_GRIDNEWS:
	case TRKEY_GRIDROW:
	case TRKEY_GRIDNEW:
	case TRKEY_LASTSET:
	case TRKEY_GRIDSAVE:
	case TRKEY_OVER:
		m_GridWnd[min(trkey->kind == TRKEY_OVER ? trkey->group + 8 : trkey->group, m_nGroup - 1)]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)rdata);
		if (m_bAutoCloseSave)
		{
			m_bAutoCloseSave = FALSE;
			m_pdlgsave->SendMessage(WM_MANAGE, 0x99, NULL);
		}
		break;
	case TRKEY_RECOMMAND:
		m_GridWnd[trkey->group]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)rdata);
		m_isRecommand = true;
		//		SetEqualizerField(3);
		break;
	case TRKEY_GRIDUPJONG:
		/* 2011.12.16 KSJ
		m_GridWndUpjn->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)rdata);
		// KSJ */
		break;
	}

	//등록순이 아닐경우 정렬시킴
	// 	CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	// 	int ret = pWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);
	//
	// 	switch (trkey->kind)
	// 	{
	// 	case TRKEY_GRIDROW:
	// 	case TRKEY_GRIDNEW:
	// 	case TRKEY_RECOMMAND:
	// 	case TRKEY_GRIDUPJONG:
	// 		{
	// 			if(ret != 0 && trkey->group == m_nGroup-1)
	// 			{
	// 				arrangeGroup(ret);
	// 			}
	// 		}
	// 		break;
	// 	}
}

void CGroupWnd::setIsRecommand(bool bFlag)
{
	m_isRecommand = bFlag;
}

bool CGroupWnd::GetIsRecommand()
{
	return m_isRecommand;
}

void CGroupWnd::loadcfg()
{
	// load resource
	m_pBrush   = getAxBrush(RGB(100, 100, 100));
	m_clrBack  = getAxColor(68);
	m_bkColor1 = getAxColor(68);
	m_bkColor2 = getAxColor(77); // LINECOLOR BUFFET
	m_rtmColor = getAxColor(8);
	m_clrbookMark    = getAxColor(199);
	m_clrbookMarkPen = getAxColor(69);
	m_chUse = 0x00;

	for (int ii = 0; ii < 4; ii++)
		m_fgColor[ii] = getAxColor(39);

	// load info
	m_nGroup = GetPrivateProfileInt(SEC_GROUP, KEY_COUNT, 1, m_fileCFG);

	if (m_nGroup <= 0)
		m_nGroup = 1;

	if (m_nGroup > MAX_GROUP)
		m_nGroup = MAX_GROUP;

	m_cntGroup = m_nGroup;

	// int	readL = 0;
	char readB[1024];
	CString string, stringx;

	memset(readB, 0x00, sizeof(readB));
	GetPrivateProfileString(SEC_GROUP, KEY_RATE, "", readB, sizeof(readB), m_fileCFG);
	string.Format("%s", readB);

	if (string.IsEmpty())
	{
		for (int ii = 0; ii < m_nGroup; ii++)
		{
			string += "100,";
		}
	}

	SetArray(string);
	m_nCurSel = GetPrivateProfileInt(SEC_GROUP, KEY_SELECT, 0, m_fileCFG);

	loadfield();
}

// 2011.12.12	KSJ
void CGroupWnd::loadGroupCheck()
{
	char buf[1024];
	CString sztmp, szUnit;

	if (0 == GetPrivateProfileString(SEC_MAIN, KEY_DISPCODE, "", buf, sizeof(buf), m_fileCFG))
	{
		m_bDispCode = FALSE;
	}
	else
	{
		m_bDispCode = atoi(buf);
	}

	if (0 == GetPrivateProfileString(SEC_MAIN, KEY_NEWSNCHART, "", buf, sizeof(buf), m_fileCFG))
	{
		m_bNewsNChart = TRUE;
	}
	else
	{
		m_bNewsNChart = atoi(buf);
	}

	if (0 == GetPrivateProfileString(SEC_MAIN, KEY_MARGIN, "", buf, sizeof(buf), m_fileCFG))
	{
		m_bMargin = FALSE;
	}
	else
	{
		m_bMargin = atoi(buf);
	}

	if (0 == GetPrivateProfileString(SEC_MAIN, KEY_SYMINFO, "", buf, sizeof(buf), m_fileCFG))
	{
		m_bInfo = FALSE;
	}
	else
	{
		m_bInfo = atoi(buf);
	}

	if (0 == GetPrivateProfileString(SEC_MAIN, KEY_TICKER, "", buf, sizeof(buf), m_fileCFG))
	{
		m_bTicker = FALSE;
	}
	else
	{
		m_bTicker = atoi(buf);
	}
}
// KSJ


void CGroupWnd::addGridWnd()
{
	CString strTemp;

	loadGroupCheck(); // 2011.12.15 KSJ

	for (int ii = m_nGroup - 1; ii < m_nGroup; ii++)
	{
		m_GridWnd[ii] = std::make_unique<CGridWnd>(m_pMainWnd, ii);
		m_GridWnd[ii]->m_iTime = m_iTime;
		m_GridWnd[ii]->m_sRoot = Variant(homeCC);

		m_GridWnd[ii]->m_bWaveApply = m_bWaveApply;
		m_GridWnd[ii]->m_strWaveFile = m_strWaveFile;
		m_GridWnd[ii]->m_bPopupDlg = m_bPopupDlg;
		m_GridWnd[ii]->m_bDispCode = m_bDispCode;
		m_GridWnd[ii]->m_bNewsNChart = m_bNewsNChart; // 2011.12.02	KSJ
		m_GridWnd[ii]->m_bMargin = m_bMargin;	      // 2011.12.05	KSJ
		m_GridWnd[ii]->m_bInfo = m_bInfo;	      // 2011.12.05	KSJ
		m_GridWnd[ii]->m_bTicker = m_bTicker;	      // 2011.12.02	KSJ

		m_GridWnd[ii]->Create(nullptr, nullptr, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_GRIDWND + ii);

		// 2011.12.12
		m_GridWnd[ii]->SetMarketColor(m_bMKClrApply, m_clrKospi, m_clrKosdaq);
		m_GridWnd[ii]->SetDispCode(m_bDispCode);
		m_GridWnd[ii]->SetNewsNChart(m_bNewsNChart);
		m_GridWnd[ii]->SetMargin(m_bMargin);
		m_GridWnd[ii]->SetInfo(m_bInfo);
		// m_GridWnd[ii]->SetTicker(m_bTicker);
		// 2011.12.12
	}

	m_cntGroup = m_nGroup;
}

void CGroupWnd::loadfield()
{
	CString filePath;
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);
	int readL = 0;
	char readB[1024];
	CString string, stringx;

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_BKCOLOR, "1", readB, sizeof(readB), filePath);

	m_chUse = readB[0]; // BK 사용/미사용

	if (readB[0] == '2')
	{
		readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_COLOR2, string, readB, sizeof(readB), filePath);
		if (readL > 0)
		{
			string = CString(readB, readL);

			m_line = atoi(IH::Parser(string, deliMITER));
			if (m_line <= 0)
				m_line = 1;
			m_bkColor1 = _httoi(IH::Parser(string, deliMITER));
			m_bkColor2 = _httoi(IH::Parser(string, deliMITER));
		}
	}
	else
	{
		readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_COLOR1, "", readB, sizeof(readB), filePath);
		if (readL > 0)
		{
			string = CString(readB, readL);

			m_line = 1;
			m_bkColor1 = _httoi(IH::Parser(string, deliMITER));
			m_bkColor2 = m_bkColor1;
		}
	}

	readL = GetPrivateProfileString(SEC_INTERFIELD, "BOOKMARKCOLOR", "", readB, sizeof(readB), filePath);
	if (readL > 0)
		m_clrbookMark = _httoi(readB);

	readL = GetPrivateProfileString(SEC_INTERFIELD, "BOOKMARKPENCOLOR", "", readB, sizeof(readB), filePath);
	if (readL > 0)
		m_clrbookMarkPen = _httoi(readB);

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_REALCOLOR, "", readB, sizeof(readB), filePath);
	if (readL > 0)
		m_rtmColor = _httoi(readB);

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FRCOLOR, "", readB, sizeof(readB), filePath);

	if (readL > 0)
	{
		string = CString(readB, readL);

		for (int ii = 0; ii < 4; ii++)
			m_fgColor[ii] = _httoi(IH::Parser(string, deliMITER));
	}

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FLASHTYPE, "", readB, sizeof(readB), filePath);
	if (readL > 0)
	{
		switch (readB[0])
		{
		case '1':
			m_rtmAction = rtmCELL;
			break;
		case '2':
			m_rtmAction = rtmROW;
			break;
		default:
			m_rtmAction = rtmNONE;
			break;
		}
	}

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_LINKMAP, "1", readB, sizeof(readB), filePath);
	if (readL > 0)
	{
		string = CString(readB, readL);

		stringx = IH::Parser(string, deliMITER);
		if (stringx.GetLength() > 0)
		{
			switch (stringx.GetAt(0))
			{
			case '1':
				m_dblAction = dblCURR;
				break;
			case '2':
				m_dblAction = dblCHART;
				break;
			case '4':
				m_dblAction = dblUSER;
				m_linkMap = IH::Parser(string, deliMITER);
				break;
			default:
				m_dblAction = dblCURR;
				break;
			}
		}
	}
	else
	{
		m_dblAction = dblCURR;
	}

	m_selAction = GetPrivateProfileInt(SEC_INTERFIELD, "SELACTION", 1, filePath);
	m_overlap = GetPrivateProfileInt(SEC_INTERFIELD, "OVERLAP", 0, filePath);
	m_overlapAlert = GetPrivateProfileInt(SEC_INTERFIELD, "OVERLAPALERT", 0, filePath);
	m_bAutoSave = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, "AUTOSAVE", 0, filePath); // 2016.12.09 KSJ 종료시 자동저장 체크

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FONTPOINT, "9", readB, sizeof(readB), filePath);

	if (readL > 0)
		m_fontsize = atoi(CString(readB, readL));

	if (m_fontsize < 8 || m_fontsize > 15)
		m_fontsize = 9;

	m_rowH = DEF_ROWH;
	switch (m_fontsize)
	{
	case 8:
		m_rowH = DEF_ROWH - 4;
		break;
	case 11:
		m_rowH = DEF_ROWH + 2;
		break;
	case 12:
		m_rowH = DEF_ROWH + 4;
		break;
	case 13:
		m_rowH = DEF_ROWH + 6;
		break;
	case 14:
		m_rowH = DEF_ROWH + 8;
		break;
	case 15:
		m_rowH = DEF_ROWH + 10;
		break;
	}

	m_fontname = _T("굴림체");

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FONTNAME, "굴림체", readB, sizeof(readB), filePath);

	if (readL > 0)
		m_fontname.Format("%s", readB);

	m_bCurr = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_CURRBOLD, 1, filePath);
	m_bName = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_NAMEBOLD, 1, filePath);
	m_bAllBold = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_ALLBOLD, 0, filePath);



}

void CGroupWnd::saveOverLap(int overlap)
{
	CString filePath;
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);

	m_overlap = overlap;

	CString sztmp = _T("");
	sztmp.Format("%d", overlap);
	WritePrivateProfileString(SEC_INTERFIELD, "OVERLAP", sztmp.operator LPCTSTR(), filePath);
}

void CGroupWnd::savecfg()
{
	// delete resource
	CString sztmp = _T(""), sztmp2 = _T(""), title = _T("");
	// group count
	sztmp.Format("%d", m_nGroup);
	WritePrivateProfileString(SEC_GROUP, KEY_COUNT, sztmp.operator LPCTSTR(), m_fileCFG);

	// groupwnd size rate
	sztmp = _T("");
	const int ncnt = m_arRATE.GetSize();
	WritePrivateProfileString(SEC_GROUP, KEY_RATE, sztmp.operator LPCTSTR(), m_fileCFG);
}

void CGroupWnd::OperInit()
{
	CString filePath;
	filePath.Format("%s/%s/InterOption.ini", m_root, "tab");
	m_iTime = GetPrivateProfileInt("DRAWTIME", "TIME", 0, filePath); // win10

	loadcfg();
	CreateGrid();
	RefreshGrid();
//	SelectOper();

	for (int ii = 0; ii < m_nGroup; ii++)
	{
		if (m_GridWnd[ii])
			m_GridWnd[ii]->setMemo();
	}
	//트리에 선택시켜 TR발생하게끔한다.
	//관심그룹 선택 방법 변경
	SetTimer(MK_SELFIELD, 800, nullptr);
}

void CGroupWnd::InitSetGroup()
{
	const UINT kind = m_GridWnd[m_nCurSel]->GetKind();
	const int gIndex = GetPrivateProfileInt(SEC_MAIN, KEY_SELECTGROUP, 1, m_fileCFG);  //test mod
	const UINT val = MAKE_TREEID(xINTEREST, 0, 2, gIndex, TD_ITEM);


	m_nCurSel = gIndex - 1;
	m_nCurSel = max(m_nCurSel, 0);
	m_nCurSel = min(m_nCurSel, m_nGroup);
	m_nCurSel = 0;  //check 0이 안전
	
	//트리, 툴 그룹 선택하기
	m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)val);
}

//관심그룹 index 저장
void CGroupWnd::saveGroupIndex(int index)
{
	CString str;
	str.Format("%02d", index);
	WritePrivateProfileString(SEC_MAIN, KEY_SELECTGROUP, str.operator LPCTSTR(), m_fileCFG);
}

void CGroupWnd::InitGrid()
{
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	class CGridData sdata;
	UINT kind = 0;
	int max = 0;
	CMapWordToPtr mapNews;
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	int saveGroupInfo[MAX_GROUP]{};
	bool isFind = false;

	int arryIndex = 0;
	for (int i = 0; i < MAX_GROUP; i++)
	{
		saveGroupInfo[i] = 0;
	}

	for (int ii = 0; ii < m_nGroup; ii++)
	{
		sdata.Reset();
		kind = m_GridWnd[ii]->GetKind();

		if (CAST_TREEID(kind)->kind == xISSUE)
		{
			mapNews.SetAt(ii, nullptr);
			continue;
		}

		if (kind)
		{
			sdata.SetKind(kind);

			if (nOver == MO_VISIBLE)
				max = (m_nGroup - ii) * MAX_LINE;
			else if (nOver == MO_FULL)
				max = (m_nGroup - ii) * MAX_LINE;
			else
				max = MAX_LINE;
			pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata); // 1
			int i = 0;
			isFind = false;

			while (i < MAX_GROUP)
			{
				//배열안에 종목이 있는지 확인
				if (saveGroupInfo[i] == kind)
				{
					isFind = true;
					break;
				}

				i++;
			}

			if (isFind == false)
			{
				saveGroupInfo[arryIndex] = kind;
				ii = OverOper(ii, &sdata);
				arryIndex++;
			}
			else
			{
				m_GridWnd[ii]->SetKind(0);
				m_GridWnd[ii]->RemoveAll();
			}
		}
	}

	//데이터가 있는 그룹만 내용 보여준다
	int group_data{};
	for (int ii = 0; ii < m_nGroup; ii++)
	{
		group_data = 0;
		void *ptr = nullptr;
		if (mapNews.Lookup(ii, ptr))
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_NEWS);
			continue;
		}
		group_data = m_GridWnd[ii]->loadcfg_data();
		if(group_data != 0)
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
		}
	}
}

void CGroupWnd::RefreshGrid()
{
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	class CGridData sdata;
	

	UINT kind = 0;
	int max = 0;
	CMapWordToPtr mapNews;
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	int saveGroupInfo[MAX_GROUP]{};
	bool isFind = false;

	int arryIndex = 0;
	for (int i = 0; i < MAX_GROUP; i++)
	{
		saveGroupInfo[i] = 0;
	}

	if (nOver == MO_VISIBLE)
	{
		kind = m_GridWnd[m_nCurSel]->GetKind();

		if (kind == 0)
		{
			MessageBox("선택한 화면은 빈 공간입니다. 원하시는 그룹을 선택하신 후 버튼을 클릭하여 주세요.", "IBK투자증권");
			CWnd *pToolWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			pToolWnd->SendMessage(WM_COMMAND, IDC_BT_ADDSELECT);
			return;
		}
	}


	if (nOver == MO_SELECT)
	{
		for (int ii = 0; ii < m_nGroup - 1; ii++)
		{
			kind = m_GridWnd[0]->GetKind();		
			if (kind == m_GridWnd[ii + 1]->GetKind())
				m_GridWnd[ii + 1]->SetKind(0);
		}
	}

	for (int ii = 0; ii < m_nGroup; ii++)
	{
		sdata.Reset();
		kind = m_GridWnd[ii]->GetKind();


		if (nOver == MO_VISIBLE)
		{
			kind = m_GridWnd[m_nCurSel]->GetKind();
		}
		else
		{
			kind = m_GridWnd[ii]->GetKind();
		}

		if (CAST_TREEID(kind)->kind == xISSUE)
		{
			mapNews.SetAt(ii, nullptr);
			continue;
		}
		else if (CAST_TREEID(kind)->kind == xREMAIN)
		{
			pWnd->SendMessage(WM_MANAGE, MK_SENDREMAIN);
			continue;
		}

		if (kind)
		{
			sdata.SetKind(kind);
			sdata._gridindex = ii;

			if (nOver == MO_VISIBLE)
				max = (m_nGroup - ii) * MAX_LINE;
			else if (nOver == MO_FULL)
				max = (m_nGroup - ii) * MAX_LINE;
			else
				max = MAX_LINE;
			
			pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata); // 1
			int i = 0;
			isFind = false;

			while (i < MAX_GROUP)
			{
				//배열안에 종목이 있는지 확인
				if (saveGroupInfo[i] == kind)
				{
					isFind = true;
					break;
				}

				i++;
			}

			if (isFind == false && sdata.GetCount() > 0)
			{
				saveGroupInfo[arryIndex] = kind;

				// 2016.04.11 KSJ epb일때
				if (CAST_TREEID(kind)->kind == xEPBCODE)
					m_GridWnd[ii]->FieldSetup(false, 4, true);
				else
					m_GridWnd[ii]->FieldSetup(false);

				ii = OverOper(ii, &sdata);
				arryIndex++;
			}
			else
			{
				m_GridWnd[ii]->SetKind(0);
				m_GridWnd[ii]->RemoveAll();
			}
		}
		else
		{
			m_GridWnd[ii]->SetKind(0);
			m_GridWnd[ii]->RemoveAll();
		}
	}

	//데이터가 있는 그룹만 내용 보여준다
	for (int ii = 0; ii < m_nGroup; ii++)
	{
		void *ptr = nullptr;
		if (mapNews.Lookup(ii, ptr))
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_NEWS);
			continue;
		}


		if (m_GridWnd[ii]->GetKind())
		{
			int group_data = m_GridWnd[ii]->loadcfg_data();
			if(group_data != 0)
				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
		}
	}
}

void CGroupWnd::ReSetupGrid()
{
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	
	class CGridData sdata;
	UINT kind = 0;
	int max = 0;
	CMapWordToPtr mapNews;
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	int saveGroupInfo[MAX_GROUP]{};
	bool isFind = false;

	int arryIndex = 0;
	for (int i = 0; i < MAX_GROUP; i++)
	{
		saveGroupInfo[i] = 0;
	}

	if (nOver == MO_VISIBLE)
	{
		kind = m_GridWnd[m_nCurSel]->GetKind();

		if (kind == 0)
		{
			MessageBox("선택한 화면은 빈 공간입니다. 원하시는 그룹을 선택하신 후 버튼을 클릭하여 주세요.", "IBK투자증권");
			CWnd *pToolWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			pToolWnd->SendMessage(WM_COMMAND, IDC_BT_ADDSELECT);
			return;
		}
	}


	for (int ii = 0; ii < m_nGroup; ii++)
	{
		sdata.Reset();

		if (nOver == MO_VISIBLE)
		{
			kind = m_GridWnd[m_nCurSel]->GetKind();
		}
		else
		{
			kind = m_GridWnd[ii]->GetKind();
		}

		if (CAST_TREEID(kind)->kind == xISSUE)
		{
			pWnd->SendMessage(WM_MANAGE, MK_ISSUE, ii);
			//			mapNews.SetAt(ii, nullptr);
			continue;
		}
		else if (CAST_TREEID(kind)->kind == xREMAIN)
		{
			pWnd->SendMessage(WM_MANAGE, MK_SENDREMAIN);
			continue;
		}

		if (kind)
		{
			sdata.SetKind(kind);
			sdata._gridindex = ii;
			if (nOver == MO_VISIBLE)
			{
				max = (m_nGroup - ii) * MAX_LINE;
			}
			else if (nOver == MO_FULL)
				max = (m_nGroup - ii) * MAX_LINE;
			else
				max = MAX_LINE;
	
			pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata); // 1

			int i = 0;
			isFind = false;

			while (i < MAX_GROUP)
			{
				//배열안에 종목이 있는지 확인
				if (saveGroupInfo[i] == kind)
				{
					isFind = true;
					break;
				}
				i++;
			}

			if (isFind == false && sdata.GetCount() > 0)
			{
				saveGroupInfo[arryIndex] = kind;
				ii = OverOper(ii, &sdata);
				arryIndex++;
			}
			else
			{
				m_GridWnd[ii]->SetKind(0);
				m_GridWnd[ii]->RemoveAll();
			}
		}
		else
		{
			m_GridWnd[ii]->SetKind(0);
			m_GridWnd[ii]->RemoveAll();
		}
	}


	//데이터가 있는 그룹만 내용 보여준다
	for (int ii = 0; ii < m_nGroup; ii++)
	{
		void *ptr = nullptr;
		if (mapNews.Lookup(ii, ptr))
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_NEWS);
			continue;
		}

		if (m_GridWnd[ii]->GetKind())
		{
			int group_data = m_GridWnd[ii]->loadcfg_data();
			if(group_data != 0)
				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
		}
	}
}

void CGroupWnd::SetArray(CString rates)
{
	CString sztmp;

	CRect rcRect;
	GetClientRect(&rcRect);
	CRect rcPrev = rcRect;

	m_arRATE.RemoveAll();

	for (int ii = 0; ii < MAX_GROUP; ii++)
	{
		if (ii < m_nGroup)
			sztmp = IH::Parser(rates, ",");
		m_arRATE.Add(atoi(sztmp));

		if (ii < m_nGroup)
		{
			rcRect.left = rcPrev.left;

			if (ii > 0)
				rcRect.left += SIZE_DIVIDE;

			rcRect.right = rcRect.left + atoi(sztmp);
			rcPrev = rcRect;
			m_arRECT.Add(rcRect);
		}
	}
}

int CGroupWnd::getTotalSize()
{
	if (m_arRATE.GetSize() != MAX_GROUP)
		return -1;
	int total = 0;

	for (int ii = 0; ii < m_nGroup; ii++)
	{
		total += m_arRATE.GetAt(ii);
	}

	return total;
}

void CGroupWnd::MakeGridRect(CRect rcFull)
{
	m_arRECT.RemoveAll();
	m_arDIVIDE.RemoveAll();
	m_arUpDIV.RemoveAll();
	m_arDnDIV.RemoveAll();

	const int nTotSize = getTotalSize();
	int nGapSize = 0;

	nGapSize = SIZE_DIVIDE * m_nGroup + 2;

	CRect rcGrid = rcFull;
	rcGrid.right = 0;
	CRect rcDivide = rcFull;

	CRect rcUp = rcFull;
	CRect rcDn = rcFull;

	rcUp.bottom = rcGrid.top + SIZE_DIVIDE;
	rcDn.top = rcGrid.bottom - SIZE_DIVIDE;

	rcGrid.top = rcGrid.top + SIZE_DIVIDE;	     // BUFFET BLOCK
	rcGrid.bottom = rcGrid.bottom - SIZE_DIVIDE; // BUFFET BLOCK

	int nWidth = (rcFull.Width() - nGapSize) / m_nGroup;

	for (int i = 0; i < m_nGroup + 1; i++) // BUFFET +1
	{
		rcGrid.left = rcGrid.right;

		rcUp.left = rcGrid.right;
		rcDn.left = rcGrid.right;

		rcGrid.left += SIZE_DIVIDE;
		// if (i == m_nGroup - 1)
		if (i == m_nGroup - 1) // BUFFET +1
			nWidth = (rcFull.Width() - nGapSize) - (nWidth * (m_nGroup - 1));

		rcGrid.right = rcGrid.left + nWidth;

		rcUp.right = rcGrid.right;
		rcDn.right = rcGrid.right;

		rcDivide.left = rcGrid.left - SIZE_DIVIDE;

		rcDivide.right = rcGrid.left;

		// if (i < m_nGroup - 1)
		if (i < m_nGroup + 1)		  // BUFFET +1
			m_arDIVIDE.Add(rcDivide); //

		if (i < m_nGroup)
		{
			m_arRECT.Add(rcGrid);
			m_arUpDIV.Add(rcUp);
			m_arDnDIV.Add(rcDn);

			m_arRATE.SetAt(i, rcGrid.Width());
		}
	}
}

void CGroupWnd::OperResize(int cx, int cy)
{
	const CRect rect(0, 0, cx, cy);
	CRect rcGrid;
	m_rcTitle = rect;
	m_rcTitle.bottom = m_rcTitle.top + 20;
	MakeGridRect(rect);

	int ii = 0;
	for (ii = 0; ii < m_nGroup; ii++)
	{
		rcGrid = m_arRECT.GetAt(ii);
		m_GridWnd[ii]->MoveWindow(rcGrid, FALSE);
		m_arRATE.SetAt(ii, rcGrid.Width());
		if (!m_bMAP)
		{
			if (!m_GridWnd[ii]->IsWindowVisible())
				m_GridWnd[ii]->ShowWindow(SW_SHOW);
		}
	}

	for (; ii < m_cntGroup; ii++)
	{
		m_GridWnd[ii]->ShowWindow(SW_HIDE);
	}

	InvalidateDivide();
}

void CGroupWnd::CreateGrid()
{
	for (int ii = 0; ii < m_nGroup; ii++)
	{
		m_GridWnd[ii] = std::make_unique<CGridWnd>(m_pMainWnd, ii);
		m_GridWnd[ii]->m_iTime = m_iTime;
		m_GridWnd[ii]->m_sRoot = Variant(homeCC);

		m_GridWnd[ii]->m_bWaveApply = m_bWaveApply;
		m_GridWnd[ii]->m_strWaveFile = m_strWaveFile;
		m_GridWnd[ii]->m_bPopupDlg = m_bPopupDlg;
		m_GridWnd[ii]->m_bDispCode = m_bDispCode;
		m_GridWnd[ii]->m_bNewsNChart = m_bNewsNChart; // 2011.12.02	KSJ
		m_GridWnd[ii]->m_bMargin = m_bMargin;	      // 2011.12.05	KSJ
		m_GridWnd[ii]->m_bInfo = m_bInfo;	      // 2011.12.05	KSJ
		m_GridWnd[ii]->m_bTicker = m_bTicker;	      // 2011.12.02	KSJ

		m_GridWnd[ii]->Create(nullptr, nullptr, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_GRIDWND + ii);

		m_GridWnd[ii]->SetMarketColor(m_bMKClrApply, m_clrKospi, m_clrKosdaq);
	}
}

void CGroupWnd::DestroyGrid()
{
	if (LIB_IsWndAlive(m_GridWndUpjn))
		LIB_DeleteWnd(m_GridWndUpjn);
}

void CGroupWnd::OperDestory()
{
	savecfg();
	DestroyGrid();
}

void CGroupWnd::DrawDivide(CDC *pDC)
{
	int ncnt = 0;
	CRect rect;

	COLORREF clrLight = getAxColor(64);

	ncnt = m_arUpDIV.GetSize();

	for (int ii = 0; ii < ncnt; ii++)
	{
		rect = m_arUpDIV.GetAt(ii);

		if (ncnt > 1 && m_nCurSel == ii)
			clrLight = RGB(255, 70, 70);
		else
			clrLight = getAxColor(gClrBack);

		DrawHigV(pDC, rect, clrLight, clrLight);

		rect = m_arDnDIV.GetAt(ii);

		DrawHigV(pDC, rect, clrLight, clrLight);
	}
	// END MODIFY

	ncnt = m_arDIVIDE.GetSize();

	for (int ii = 0; ii < ncnt; ii++)
	{
		rect = m_arDIVIDE.GetAt(ii);

		if (ncnt > 2 &&
		    (m_nCurSel == ii || (m_nCurSel + 1) == ii))
			clrLight = RGB(255, 70, 70);
		else
			clrLight = getAxColor(gClrBack);

		DrawHighlight(pDC, rect, clrLight, clrLight);
	}
}

void CGroupWnd::DrawHigV(CDC *pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow)
{
	pDC->FillSolidRect(rect, clrShadow);
	
	CPen *penW{}, *penB{}, *oldpen{};
	penW = getAxPen(clrLight, 1);
	penB = getAxPen(clrShadow, 2);

	oldpen = pDC->SelectObject(penW);
	pDC->MoveTo(rect.TopLeft());
	pDC->LineTo(rect.right, rect.top);
	pDC->SelectObject(oldpen);

	pDC->SelectObject(penB);
	pDC->MoveTo(rect.left, rect.bottom);
	pDC->LineTo(rect.BottomRight());
	pDC->SelectObject(oldpen);
}

void CGroupWnd::DrawHighlight(CDC *pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow)
{
	CPen *penLight{}, *penShadow{}, *pOldPen{};

	penLight = getAxPen(clrLight, 1);
	penShadow = getAxPen(clrShadow, 2);

	pOldPen = pDC->SelectObject(penLight);

	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left, rect.bottom);

	pDC->SelectObject(penShadow);

	pDC->MoveTo(rect.right - 1, rect.top);
	pDC->LineTo(rect.right - 1, rect.bottom);

	pDC->SelectObject(pOldPen);

}

int CGroupWnd::GroupPlus(BOOL bExpand)
{
	const int nWidth = m_arRATE.GetAt(m_nGroup - 1);
	const int nScrMax = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	BOOL isReset = TRUE; //화면 리셋할것인지 여부
	const BOOL bTree = (BOOL)m_pMainWnd->SendMessage(WM_MANAGE, MK_ISTREE);

	m_nGroup++;

	for (int ii = m_nGroup - 2; ii < m_nGroup; ii++)
	{
		m_arRATE.SetAt(ii, nWidth);
	}

	if (bExpand)
	{
		if (m_arRECT.GetSize())
		{
			CRect rect = m_arRECT.GetAt(m_arRECT.GetUpperBound());

			const int nTree = bTree ? SIZE_TREE : 0;

			if ((m_arRECT.GetSize() + 1) * rect.Width() + nTree > nScrMax) // over screen size
			{
				m_nGroup--;
				isReset = FALSE;
				MessageBox("표시할 수 있는 범위를 벗어 났습니다.\n     확장 체크를 해제해 주십시오.", "IBK투자증권", MB_OK); // 2013.07.08 문구 변경
			}
			else
			{
				if (m_cntGroup < m_nGroup)
				{
					addGridWnd();
				}

				rect.OffsetRect(rect.Width() + SIZE_DIVIDE, 0);
				m_arRECT.Add(rect);
				m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SIZE, MO_PLUS), MAKELPARAM(rect.Width(), 0));
			}
		}
	}
	else
	{
		if (m_cntGroup < m_nGroup)
		{
			addGridWnd();
		}

		OperResize(m_rect.Width(), m_rect.Height());
	}

	if (isReset == TRUE)
	{
		m_GridWnd[m_nGroup - 1]->FieldSetup(false);
		m_GridWnd[m_nGroup - 1]->Reset();
	}

	// 2011.12.05 KSJ
	AllGroupSave();
	// KSJ

	return m_nGroup;
}

// 2011.12.12 KSJ
void CGroupWnd::IniGroupDel()
{
	CString strFile, strLine, strSection, strTemp;
	CStdioFile file;

	BOOL bFind = FALSE;

	strSection.Format("GRID%02d", m_nGroup);
	strFile.Empty();

	file.Open(m_fileCFG, CFile::modeRead | CFile::modeNoTruncate | CFile::shareDenyNone); //없을경우 체크하지 않음

	while (file.ReadString(strLine))
	{
		if (strLine.Find(strSection) != -1)
		{
			bFind = TRUE;

			while (file.ReadString(strLine))
			{
				//다음 그룹까지 넘어가다가 while문을 종료한다.
				if (strLine.Find("[GRID") != -1)
					break;
			}
		}

		strFile += strLine;
		strFile += "\n";
	}
	file.Close();

	if (bFind)
	{
		file.Open(m_fileCFG, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::shareDenyNone); //없을경우 체크하지 않음

		file.SeekToBegin();
		file.SetLength(0);
		file.Write(strFile, strFile.GetLength() - 1); //마지막 '\n' 지우기
		file.Close();
	}
}
// KSJ

int CGroupWnd::GroupMinus(BOOL bExpand)
{
	m_nGroup--;
	// 2011.15.09 KSJ 삭제하는 부분 추가하여야함.
	IniGroupDel();
	// KSJ

	if (bExpand)
	{
		if (m_arRECT.GetSize())
		{
			const CRect rect = m_arRECT.GetAt(m_arRECT.GetUpperBound());
			m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SIZE, MO_MINUS), MAKELPARAM(rect.Width(), 0));
		}
	}
	else
	{
		OperResize(m_rect.Width(), m_rect.Height());
	}

	if (m_nCurSel == m_nGroup)
	{
		m_nCurSel = m_nGroup - 1;
		SelectOper();
	}
	m_GridWnd[m_nGroup]->Reset();
	m_GridWnd[m_nGroup]->SendMessage(WM_MANAGE, MK_TURNCODE);
	m_cntGroup = m_nGroup; // 2011.12.13 KSJ

	return m_nGroup;
}

void CGroupWnd::ExpectOper(int param)
{
	bool bExpect = false;

	if (param == MO_ON)
		bExpect = true;

	for (int ii = 0; ii < m_nGroup; ii++)
	{
		m_GridWnd[ii]->toggleAction(bExpect);
	}
}

void CGroupWnd::AutoExpectOper(int param)
{
	for (int ii = 0; ii < m_nGroup; ii++)
	{
		m_GridWnd[ii]->toggleAction(false);
	}
}

void CGroupWnd::SetEqualizerField(int param)
{
	for (int ii = 0; ii < m_nGroup; ii++)
	{
		m_GridWnd[ii]->FieldSetup(false, param);
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
	}

	if (param != 3)
	{
		m_GridWnd[0]->SaveAsInterField();
	}
}

void CGroupWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	CString strX, strY, strW, strH, strFormat;

	if (m_bDivide)
	{
		DrawMove(m_rcDIVIDE);
		DrawMove(m_rcUpDIV);
		DrawMove(m_rcDnDIV);

		m_rcDIVIDE.OffsetRect(point.x - m_xPosMove, 0);
		m_rcUpDIV.OffsetRect(point.x - m_xPosMove, 0);
		m_rcDnDIV.OffsetRect(point.x - m_xPosMove, 0);
		m_xPosMove = point.x;

		DrawMove(m_rcDIVIDE);
		DrawMove(m_rcUpDIV);
		DrawMove(m_rcDnDIV);

		// Invalidate(false);
	}
	CBaseWnd::OnMouseMove(nFlags, point);
}

void CGroupWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	m_bDivide = TRUE;
	m_xPosMove = m_xPos = point.x;
	m_nIndex = GetIndexDivide(point);

	if (m_nIndex < 0)
		return;

	CRect rect = GetArea(m_nIndex);
	m_rcDIVIDE = m_arDIVIDE.GetAt(m_nIndex + 1);
	DrawMove(m_rcDIVIDE);

	ClientToScreen(rect);
	ClipCursor(rect);

	m_rcUpDIV.SetRectEmpty();
	m_rcDnDIV.SetRectEmpty();

	CBaseWnd::OnLButtonDown(nFlags, point);
}

void CGroupWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	ClipCursor(nullptr);
	if (m_bDivide && point.x != m_xPos)
	{
		if (m_nIndex < 0)
			return;

		const CRect rcDivide = GetMovedRect(m_nIndex, point.x - m_xPos); // BUFFET +1
		m_arDIVIDE.SetAt(m_nIndex + 1, rcDivide);			 // BUFFET +1

		// BUFFET FOR REPOSITION
		SetMovedRect(m_nIndex, point.x - m_xPos);

		CRect rcL{}, rcR{};
		rcL = m_arRECT.GetAt(m_nIndex);
		rcR = m_arRECT.GetAt(m_nIndex + 1);
		rcL.right = rcDivide.left;
		rcR.left = rcDivide.right;
		m_arRECT.SetAt(m_nIndex, rcL);
		m_arRECT.SetAt(m_nIndex + 1, rcR);

		m_arRATE.SetAt(m_nIndex, rcL.Width());
		m_arRATE.SetAt(m_nIndex + 1, rcR.Width());

		CRect rcGrid;
		int ii = 0;
		for (ii = 0; ii < m_nGroup; ii++)
		{
			rcGrid = m_arRECT.GetAt(ii);
			m_GridWnd[ii]->MoveWindow(rcGrid, FALSE);
			m_arRATE.SetAt(ii, rcGrid.Width());
			if (!m_bMAP)
			{
				if (!m_GridWnd[ii]->IsWindowVisible())
					m_GridWnd[ii]->ShowWindow(SW_SHOW);
			}
		}

		for (; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->ShowWindow(SW_HIDE);
		}

		InvalidateDivide();
	}

	DrawMove(m_rcDIVIDE);
	DrawMove(m_rcUpDIV); // BUFFET
	DrawMove(m_rcDnDIV); // BUFFET

	m_bDivide = FALSE;
	m_rcDIVIDE.SetRectEmpty();
	m_rcUpDIV.SetRectEmpty();
	m_rcDnDIV.SetRectEmpty();

	m_xPos = -1;
	m_nIndex = -1;

	Invalidate(false);

	CBaseWnd::OnLButtonUp(nFlags, point);
}

int CGroupWnd::GetIndexDivide(CPoint pt)
{
	const int ncnt = m_arDIVIDE.GetSize();

	for (int ii = 0; ii < ncnt; ii++)
	{
		if (m_arDIVIDE.GetAt(ii).PtInRect(pt))
		{
			if (ii == ncnt - 1)
				return -1;
			else
				return (ii - 1);
		}
	}

	return -1;
}

CRect CGroupWnd::GetMovedRect(int nIndex, int xPos)
{
	CRect rect = m_arDIVIDE.GetAt(nIndex + 1);
	rect.OffsetRect(xPos, 0);
	return rect;
}

int CGroupWnd::SetMovedRect(int nIndex, int xPos)
{
	if (nIndex < 0)
		return 0;

	CRect rect = m_arUpDIV.GetAt(nIndex);
	rect.right = rect.right + xPos;
	m_arUpDIV.SetAt(nIndex, rect);

	rect = m_arDnDIV.GetAt(nIndex);
	rect.right = rect.right + xPos;
	m_arDnDIV.SetAt(nIndex, rect);

	for (int i = nIndex + 1; i < nIndex + 2; i++)
	{
		rect = m_arUpDIV.GetAt(i);
		rect.left = rect.left + xPos;
		m_arUpDIV.SetAt(i, rect);

		rect = m_arDnDIV.GetAt(i);
		rect.left = rect.left + xPos;
		m_arDnDIV.SetAt(i, rect);
	}

	return TRUE;
}

void CGroupWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CString strBUF = _T("");
	for (int ii = 0; ii < m_nGroup + 1; ii++) // BUFFET +1
	{
		strBUF += "100,";
	}
	SetArray(strBUF);
	OperResize(m_rect.Width(), m_rect.Height());
	CBaseWnd::OnLButtonDblClk(nFlags, point);
}

CRect CGroupWnd::GetArea(int nIndex)
{
	CRect rcL, rcR;
	CRect rcRet;
	rcL = m_arRECT.GetAt(nIndex);
	rcR = m_arRECT.GetAt(nIndex + 1);
	rcRet = rcL;
	rcRet.right = rcR.right - SIZE_GROUPMINX + 1;
	rcRet.left = rcL.left + SIZE_GROUPMINX;
	if (rcRet.right <= rcRet.left)
		rcRet = m_arDIVIDE.GetAt(nIndex + 1); // BUFFET +1
	return rcRet;
}

void CGroupWnd::DrawMove(CRect rect)
{
	CClientDC dc(this);
	const int nMode = dc.SetROP2(R2_XORPEN);
	CBrush *oldbrush = dc.SelectObject(m_pBrush);
	dc.Rectangle(rect);
	dc.SelectObject(oldbrush);
	dc.SetROP2(nMode);
}

void CGroupWnd::SelectOper()
{
	for (int ii = 0; ii < m_nGroup; ii++)
	{
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SELGROUP);
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_HIDECODE);
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_TURNCODE);
	}

	const UINT val = m_GridWnd[m_nCurSel]->GetKind();
	m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)val);
}

//설정화면 누를시 분할표시된 화면의 가장 앞부분에 선택되도록 수정
void CGroupWnd::selectFirst()
{
	const UINT CurrentKind = m_GridWnd[m_nCurSel]->GetKind();
	UINT beforeKind = 0;

	while (m_nCurSel > 0)
	{
		beforeKind = m_GridWnd[m_nCurSel - 1]->GetKind();

		if (CurrentKind != beforeKind)
		{
			break;
		}
		else
		{
			m_nCurSel--;
		}
	}

	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
	if (m_nCurSel> 0)
		m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MK_TURNCODE);
	SelectOper();
}

int CGroupWnd::_httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	const CHexMap HexMap[HexMapL] =
	    {{'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *ss = mstr;
	int result = 0;
	if (*ss == '0' && *(ss + 1) == 'X')
		ss += 2;
	bool firsttime = true;
	while (*ss != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*ss == HexMap[i].chr)
			{
				if (!firsttime)
					result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found)
			break;
		ss++;
		firsttime = false;
	}
	[[gsl::suppress(26408)]]
	free(mstr);
	return result;
}

void CGroupWnd::SendGrid(int nIndex, UINT kind)
{

	const int max = MAX_LINE;
	CGridData sdata;
	if (CAST_TREEID(kind)->kind == xINTEREST)
	{
		for (int ii = 0; ii < m_nGroup; ii++)
		{
			if (ii == m_nCurSel)
				continue;
			const UINT datakind = m_GridWnd[ii]->GetKind();
			if (datakind == kind)
				m_GridWnd[ii]->Reset();
		}
	}

	if (CAST_TREEID(kind)->kind == xEPBCODE) // 2016.04.11 KSJ EPB
	{
		SetEqualizerField(4);
	}
	else
	{
		SetEqualizerField(0);
	}

	sdata.SetKind(kind);
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata);
	m_GridWnd[nIndex]->SetKind(kind);
	m_GridWnd[nIndex]->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)&sdata);
	m_GridWnd[nIndex]->SendMessage(WM_MANAGE, MK_SENDTR);

}

void CGroupWnd::saveRecommandGroup()
{
	m_RecomGroup[m_nCurSel] = 1;
}

void CGroupWnd::SendGrid(int nIndex, CGridData *sdata)
{
	if (nIndex >= 30) // XMSG(확인요망...)
		return;

	if (nIndex >= m_nGroup)
		return;

	if (m_GridWnd[nIndex]->getArray().size() == 0)
		return;

	bool bResize = true;
	if (nIndex == m_nCurSel)
	{
		// 2013.04.30 KSJ  현재선택된 그리드이면 사이즈 조절을 하지 않는다.
		bResize = false;
		// 2013.04.30 KSJ  END
	}


	const UINT kind = sdata->GetKind();
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	bool bEPB = false;

	if (kind == 0)
		return;

	if (CAST_TREEID(kind)->kind == xINTEREST)
	{
		for (int ii = 0; ii < m_nGroup; ii++)
		{
			if (ii == m_nCurSel)
			{
				continue;
			}

			const UINT datakind = m_GridWnd[ii]->GetKind();
			if (datakind == kind)
				m_GridWnd[ii]->Reset();
		}
	}
	else if (CAST_TREEID(kind)->kind == xEPBCODE) // 2016.04.08 KSJ EPB 추가
	{
		m_GridWnd[nIndex]->FieldSetup(false, 4, true);
		bEPB = true;
	}

	const int nEnd = OverOper(nIndex, sdata);
	if (!bEPB)
	{
		if (nOver == MO_VISIBLE)
		{
			for (int ii = nIndex; ii <= nEnd && ii < m_nGroup; ii++)
			{
				if (m_RecomGroup[nIndex] == 1)
				{
					m_GridWnd[ii]->FieldSetup(false, 3, true);
				}
				else
				{
					m_GridWnd[ii]->FieldSetup(false, 0, bResize);
				}
			}
		}
		else
		{

			if (nIndex < MAX_FIELD) 
			{
				if (m_RecomGroup[nIndex] == 1)
				{
					m_GridWnd[nIndex]->FieldSetup(false, 3, true);
				}
				else
				{
					m_GridWnd[nIndex]->FieldSetup(false, 0, bResize);
				}
			}
		}
	}

	for (int ii = nIndex; ii <= nEnd && ii < m_nGroup; ii++)
	{
		if (ii < MAX_GROUP)
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
	}

	if (nOver == MO_VISIBLE)
	{
		for (int ii = nEnd + 1; ii < m_nGroup; ii++)
		{
			if (ii < MAX_GROUP)
				m_GridWnd[ii]->Reset();
		}
	}
}

int CGroupWnd::OverOper(int nIndex, CGridData *sdata)
{
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	const UINT kind = sdata->GetKind();
	const int nOld = nIndex;
	const int nGroup = m_nGroup - nIndex;

	if (sdata->GetCount() == 0)
		nOver = MO_SELECT;

	if (CAST_TREEID(kind)->kind == xISSUE)	// || CAST_TREEID(kind)->kind == xINTEREST)
		nOver = MO_SELECT;

	switch (nOver)
	{
	case MO_VISIBLE:
	{
		int max = MAX_LINE;
		if (nOver == MO_VISIBLE)
		{
			// 2013.09.13 KSJ 트리데이터를 보내지 않으면 Gridwnd의 m_arrBase가 초기화되지 않아
			//북마크 지정할때 에러발생함.
			m_GridWnd[nIndex]->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)sdata);
			max = GetRowCount();
			const int gridWidth = m_GridWnd[nIndex]->GetWidth();
			const int intGridWidth = m_GridWnd[nIndex]->GetGridWidth();

			if ((gridWidth - intGridWidth) < 0)
			{
				max = max - 1;
			}
		}
		if (max == 0)
			max = MAX_LINE;

		int ncnt = sdata->GetCount();

		if (ncnt > MAX_LINE)
		{
			ncnt = MAX_LINE;
		}

		m_GridWnd[nIndex]->SetKind(kind);

		for (int ii = nIndex; ii < m_nGroup; ii++)
		{
			m_GridWnd[ii]->RemoveAll();
		}


		int nPos = 0;
		int kk = 0;
		for (kk = 0; kk < ncnt; kk++)
		{
			if (kk != 0 && (kk % max) == 0)
			{
				nIndex++;

				if (nIndex == m_nGroup)
					break;

				//m_GridWnd[nIndex]->RemoveAll();

				nPos = 0;
				// KET_DATA를 봐서 nIndex는 증가했는데 KEY_DATA가 0일경우 데이터를 저장
				const int keydata = m_GridWnd[nIndex]->loadcfg_data();
				int beforekeydata = 0;

				if (nIndex - 1 >= 0)
				{
					beforekeydata = m_GridWnd[nIndex - 1]->loadcfg_data();
				}

				CString strKey;
				if (keydata != beforekeydata)
				{
					strKey.Format("%d", beforekeydata);
					m_GridWnd[nIndex]->savecfg_data(strKey);
				}
				m_GridWnd[nIndex]->SetKind(kind);
			}
			m_GridWnd[nIndex]->AddData(nPos++, sdata->m_arDatas.GetAt(kk));
		}

		//그룹에 못보여진 종목이 있을경우 마지막 창에 append한다
		while (kk < ncnt)
		{
			m_GridWnd[nIndex - 1]->AddData(nPos++, sdata->m_arDatas.GetAt(kk));
			kk++;
		}
	}
	break;
	case MO_FULL:
		break;
	case MO_SELECT:
	{
		m_GridWnd[nIndex]->SetKind(kind);
		m_GridWnd[nIndex]->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)sdata);
	}
	break;
	}
	return nIndex;
}


// 2012.02.10 KSJ
void CGroupWnd::RecvRTSx(LPARAM lParam)
{
	if (m_nGroup == 0)
		return;

	_cacheGrid.clear();
	
	const auto* alertR  = reinterpret_cast<const _alertR*>(lParam);
	const CString code  = alertR->code;
		
	for_each_n(m_GridWnd.begin(), m_nGroup, [this, code](const auto& gridWnd) {	
		if (gridWnd->IsCode(code))
			_cacheGrid.push_back(gridWnd.get());
	});

	for_each(_cacheGrid.begin(), _cacheGrid.end(), [this, lParam](CGridWnd* pGridWnd) {
		pGridWnd->HoldDraw();
		pGridWnd->parsingAlertx(lParam);
	});

	for (auto& grid : _cacheGrid)
		grid->ReleaseDraw();
}

void CGroupWnd::UpdateDraw()
{
	for (auto& grid : _cacheGrid)
		grid->UpdateAll();

	_cacheGrid.clear();
}

void CGroupWnd::initAlert()
{
	for (int ii = 0; ii < m_nGroup; ii++)
	{
		if (m_GridWnd[ii]->GetSafeHwnd())
			m_GridWnd[ii]->initgridalert();
	}
}

void CGroupWnd::CloseGrid(int nIndex)
{
	if (m_nGroup == 1)
		return;

	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const BOOL bExpand = (BOOL)pWnd->SendMessage(WM_MANAGE, MK_ISEXPAND);

	if (nIndex == (m_nGroup - 1))
	{
		if (nIndex <= m_nCurSel)
			m_nCurSel--;
		if (m_nCurSel < 0)
			m_nCurSel = 0;

		GroupMinus(bExpand);
		SelectOper();
		return;
	}

	const int width = m_arRATE.GetAt(m_arRATE.GetUpperBound());
	m_arRATE.Add(width);
	m_arRATE.RemoveAt(nIndex);

	for (int ii = nIndex; ii < (m_nGroup - 1); ii++)
	{
		m_GridWnd[ii]->Assign(m_GridWnd[ii + 1].get());
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
	}

	if (nIndex <= m_nCurSel)
		m_nCurSel--;
	if (m_nCurSel < 0)
		m_nCurSel = 0;

	SelectOper();
	GroupMinus(bExpand);
}

void CGroupWnd::InvalidateDivide()
{
	int ncnt = m_arDIVIDE.GetSize();
	for (int ii = 0; ii < ncnt; ii++)
	{
		InvalidateRect(m_arDIVIDE.GetAt(ii), FALSE);
	}

	ncnt = m_arUpDIV.GetSize();
	for (int ii = 0; ii < ncnt; ii++)
	{
		InvalidateRect(m_arUpDIV.GetAt(ii), FALSE);
		InvalidateRect(m_arDnDIV.GetAt(ii), FALSE);
	}
}

BOOL CGroupWnd::OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message)
{
	const int ncnt = m_arDIVIDE.GetSize();
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	for (int ii = 0; ii < ncnt; ii++)
	{
		if (m_arDIVIDE.GetAt(ii).PtInRect(pt))
		{
			if (ii == 0 || ii == ncnt - 1)
				return TRUE;
			else
				::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_LR));
			return TRUE;
		}
	}
	return CBaseWnd::OnSetCursor(pWnd, nHitTest, message);
}

int CGroupWnd::GetRowCount()
{
	const int nHeight = (int)m_pMainWnd->SendMessage(WM_MANAGE, MK_GROUPHEIGHT);

	return (int)((double)nHeight / (double)m_rowH - 3);
}

void CGroupWnd::SetGridBkColor(COLORREF color)
{
	for (int ii = 0; ii < m_nGroup; ii++)
		m_GridWnd[ii]->SetGridBkColor(color);

	InvalidateDivide();
}

BOOL CGroupWnd::IsValidCode(CString strCode)
{
	if (m_GridWnd[m_nCurSel])
		return m_GridWnd[m_nCurSel]->IsValidCode(strCode);

	return FALSE;
}

void CGroupWnd::GetMinMaxWidth(int &nMinWidth, int &nMaxWidth)
{
	nMinWidth = nMaxWidth = -1;
	CRect rcGrid;
	const int nCount = m_arRECT.GetSize();

	if (nCount <= 0)
		return;

	for (int i = 0; i < nCount; i++)
	{
		rcGrid = m_arRECT.GetAt(i);

		nMinWidth = min(nMinWidth, rcGrid.Width());
		nMaxWidth = max(nMaxWidth, rcGrid.Width());
	}
}

int CGroupWnd::GetGroupCount()
{
	return m_nGroup;
}

void CGroupWnd::OnDestroySave()
{
	if (!IsNewDrop())
		return;
	else
	{
		// 2016.12.09 KSJ HTS를 종료할때는 자동저장이 체크되어 있으면 물어보지 않고 저장한다.

		m_pdlgsave = std::make_unique<DlgAutoSave>(this);
		m_bAutoCloseSave = true;
		m_pdlgsave->DoModal();
	}
}

// 2011.12.13 KSJ
void CGroupWnd::AllGroupSave()
{
	if (!IsNewDrop())
		return;
	else
	{
		// 2016.12.09 KSJ HTS를 종료할때는 자동저장이 체크되어 있으면 물어보지 않고 저장한다.
		if (m_bAutoSave || (MessageBox("[관심종목]변경 사항을 저장하시겠습니까?\n", "관심종목 저장 확인창", MB_YESNO) == IDYES))
		{
			const int originCursel = m_nCurSel;

			// 2017.02.21 여기서 수정된 마지막 그리드를 찾고 그 그리드만 'V'보냄.
			int nLastEditGrid = 0;

			for (int ii = 0; ii < m_nGroup; ii++)
			{
				if (m_GridWnd[ii]->m_bEditWork)
				{
					m_GridWnd[ii]->m_bLastSend = FALSE;
					nLastEditGrid = ii;
				}
			}

			for (int ii = 0; ii < m_nGroup; ii++)
			{
				if (m_GridWnd[ii]->m_bEditWork)
				{
					if (nLastEditGrid == ii)
						m_GridWnd[ii]->m_bLastSend = TRUE;

					m_GridWnd[ii]->m_bPlusMinus = true;
					m_GridWnd[ii]->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BT_SAVE, 0), 0);
					m_GridWnd[ii]->m_bPlusMinus = false;
				}
			}

			SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)originCursel);
		}
		else
		{

			for (int ii = 0; ii < m_nGroup; ii++)
			{
				m_GridWnd[ii]->m_bEditWork = FALSE;
			}
		}
	}
}
// KSJ
void CGroupWnd::saveWhenGroupClosed(int index)
{
	if (!IsNewDrop())
		return;
	else
	{
		// 2016.12.09 KSJ HTS를 종료할때는 자동저장이 체크되어 있으면 물어보지 않고 저장한다.
		if (m_bAutoSave || (MessageBox("[관심종목]변경 사항을 저장하시겠습니까?\n", "관심종목 저장 확인창", MB_YESNO) == IDYES))
		{
			const int originCursel = m_nCurSel;

			// 2017.02.21 여기서 수정된 마지막 그리드를 찾고 그 그리드만 'V'보냄.
			int nLastEditGrid = 0;

			for (int ii = 0; ii < m_nGroup; ii++)
			{
				if (m_GridWnd[ii]->m_bEditWork)
				{
					m_GridWnd[ii]->m_bLastSend = FALSE;
					nLastEditGrid = ii;
				}
			}

			for (int ii = 0; ii < m_nGroup; ii++)
			{
				if (m_GridWnd[ii]->m_bEditWork)
				{
					if (nLastEditGrid == ii)
						m_GridWnd[ii]->m_bLastSend = TRUE;

					m_GridWnd[ii]->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BT_SAVE, 0), 0);
				}
			}
			SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)originCursel);
		}
		else
		{
			for (int ii = 0; ii < m_nGroup; ii++)
			{
				m_GridWnd[ii]->m_bEditWork = FALSE;
			}
		}
	}
}

BOOL CGroupWnd::IsNewDrop()
{
	BOOL ret = FALSE;
	for (int ii = 0; ii < m_nGroup; ii++)
	{
		if (m_GridWnd[ii]->m_bEditWork)
		{
			ret = TRUE;
			break;
		}
	}
	return ret;
}

int CGroupWnd::sumEachGroupCount()
{
	UINT currentGroup = 0;
	UINT sum = 0;

	for (int index = 0; index < GetGroupCount(); index++)
	{
		currentGroup = m_GridWnd[index]->GetKind();

		if (currentGroup)
		{
			sum = sum + m_GridWnd[index]->GetRowValidcount();
		}
	}
	return sum;
}

std::pair<int, std::vector<std::shared_ptr<struct _intersx>>> CGroupWnd::WriteFileSumtoEachGroup(UINT curruntGroup) // index : 그룹 시작점
{
	UINT nextGroup = 0;
	int curruntGroupCount{}, nextGroupCount{}, gno{};
	CString code, amount, price, name, bookmark;

	CString strPath, strTemp, strBook;
	CString strSendData;

	std::vector<std::shared_ptr<struct _intersx>> vInters;

	//현재 펼쳐져있는 그룹들과 현그룹과 비교하여 같은 그룹인지 확인(제일 앞단에 있는 그룹을 찾는다)
	int index = 0;
	for (index = 0; index < GetGroupCount(); index++)
	{
		nextGroup = m_GridWnd[index]->GetKind();
		if (curruntGroup == nextGroup)
		{
			curruntGroup = m_GridWnd[index]->GetKind();
			//일단 첫번째 그룹의 데이터를 파일에 쓴다
			if (CAST_TREEID(curruntGroup)->kind == xINTEREST)
				gno = CAST_TREEID(curruntGroup)->value;
			else
				return std::make_pair(0, vInters);
			break;
		}
	}
	
	if (gno < 0)
		gno = 0;
	CString trGrouptName = m_GridWnd[index]->GetGroupName(gno);
	curruntGroupCount     = m_GridWnd[index]->GetRowValidcount();

	for (int ii = 0; ii < curruntGroupCount; ii++)
	{
		const auto& pInters = m_GridWnd[index]->GetData(ii);
		vInters.emplace_back(pInters);
	}

	//같은 그룹일 경우 옆그룹들의 데이터를 파일에 쓰기
	for (int ii = index + 1; ii < GetGroupCount(); ii++)
	{
		nextGroup = m_GridWnd[ii]->GetKind();
		if (curruntGroup == nextGroup)
		{
			nextGroupCount = m_GridWnd[ii]->GetRowValidcount();
			for (int jj = 0; jj < nextGroupCount; jj++)
			{
				m_GridWnd[ii]->m_bEditWork = FALSE; //한 섹션만 TRUE, FALSE 담당하면 된다
				const auto& pInters = m_GridWnd[ii]->GetData(jj);
				vInters.emplace_back(pInters);
			}
		}
	}
	return std::make_pair(gno, vInters);
}

CString CGroupWnd::MakePacket(CString &code, CString amount, CString price, CString name, CString bookmark)
// END MODIFY
{
	code.TrimLeft();
	code.TrimRight();
	if (code.IsEmpty())
		return _T("");

	// MODIFY PSH 20070914
	code += "\t";
	if (!amount.IsEmpty())
		code += amount;
	// END MODIFY

	code += "\t";
	if (!price.IsEmpty())
		code += price;
	// END MODIFY

	// ADD PSH 20070914
	code += "\t";
	if (!name.IsEmpty())
		code += name;

	// 북마크 추가
	code += "\t";
	if (!bookmark.IsEmpty())
		code += bookmark;
	// END ADD

	return code;
}

void CGroupWnd::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if (nIDEvent == MK_SELFIELD)
	{
		KillTimer(MK_SELFIELD);
		CRect r;
		m_pMainWnd->GetWindowRect(&r);
		((CMainWnd *)m_pMainWnd)->m_rectMain = r;
		//InitSetGroup();
	}
	CBaseWnd::OnTimer(nIDEvent);
}

CGridWnd *CGroupWnd::GetGridWndUpjn()
{
	return m_GridWndUpjn;
}

void CGroupWnd::arrangeGroup(int type)
{
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	if (type == 0)
	{
		CWnd *ptreeWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
		if (m_RecomGroup[m_nCurSel] >= 0)
		{
			ptreeWnd->SendMessage(WM_MANAGE, MK_RECOMMAND);
		}
		else
		{
			ptreeWnd->SendMessage(WM_MANAGE, MK_TREERESEND);
		}
		return;
	}

	CString save[100][2];
	CString temp[100][2];

	for (int i = 0; i < MAX_LINE; i++)
	{
		save[i][0] = "";
		save[i][1] = "";
		temp[i][0] = "";
		temp[i][1] = "";
	}

	int count = 0;

	int maxRow = 0;
	CString strTemp = "";

	if (nOver == MO_SELECT)
	{
		maxRow = 100;

		// code를 담고
		m_GridWnd[m_nCurSel]->ArrangeField(type, &save[0]);
		for (int i = 0; i < maxRow - 1; i++)
		{
			for (int j = i + 1; j < maxRow; j++)
			{
				if (save[i][1].Compare(save[j][1]) > 0)
				{
					strTemp = save[i][1];
					save[i][1] = save[j][1];
					save[j][1] = strTemp;

					strTemp = save[i][0];
					save[i][0] = save[j][0];
					save[j][0] = strTemp;
				}
			}
		}
		m_GridWnd[m_nCurSel]->SendWhenVisibleMode(&save[0]);
	}
	else
	{
		maxRow = 100;
		int row{};
		
		std::vector<int> separate;
		separate.resize(m_nGroup);

		//그리드 데이터 가져오고
		for (int i = 0; i < m_nGroup; i++)
		{
			row = m_GridWnd[i]->ArrangeField(type, &save[0]);
			separate[i] = row; //각 섹터별로 rows를 저장

			for (int j = 0; j < row; j++)
			{
				if (save[j][0] != "")
				{
					temp[count][0] = save[j][0];
					temp[count][1] = save[j][1];
					count++;
				}
			}
		}

		//정렬
		for (int i = 0; i < count - 1; i++)
		{
			for (int j = i + 1; j < count; j++)
			{
				if (temp[i][1].Compare(temp[j][1]) > 0)
				{
					strTemp = temp[i][1];
					temp[i][1] = temp[j][1];
					temp[j][1] = strTemp;

					strTemp = temp[i][0];
					temp[i][0] = temp[j][0];
					temp[j][0] = strTemp;
				}
			}
		}

		int startPoint = 0;
		int EndPoint = 0;

		for (int i = 0; i < m_nGroup; i++)
		{
			for (int k = 0; k < MAX_LINE; k++)
			{
				save[k][0] = "";
				save[k][1] = "";
			}

			const int cut = separate[i];

			EndPoint = startPoint + cut;

			count = 0;
			//각 화면별로 개수만큼만 send 시킨다
			for (int j = startPoint; j < EndPoint; j++)
			{
				save[count][0] = temp[j][0];
				save[count][1] = temp[j][1];
				count++;
			}

			startPoint += cut;

			m_GridWnd[i]->SendWhenVisibleMode(&save[0]);
		}
	}

	//데이터가 있는 그룹만 내용 보여준다
}

void CGroupWnd::checktheRecommand()
{
	int rGroup = 0;

	for (int ii = 0; ii < m_nGroup; ii++)
	{
		rGroup = m_RecomGroup[ii];

		if (rGroup == 1)
		{
			m_GridWnd[ii]->alertBMsuik();
		}
	}
}

// MO_VISIBLE모드일 경우에 다른 그리드에서 수정이 일어났는지 여부 판단
bool CGroupWnd::isModified(int kind)
{
	bool result = FALSE;
	bool editWork = FALSE;

	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	if (nOver == MO_VISIBLE)
	{
		for (int i = 0; i < m_nGroup; i++)
		{
			const int compareKind = m_GridWnd[i]->GetKind();

			if (kind == compareKind)
			{
				editWork = m_GridWnd[i]->GetEditWork();

				if (editWork == TRUE)
				{
					result = TRUE;
					m_GridWnd[i]->SetEditWork(false);
				}
			}
		}
	}

	return result;
}

const std::unordered_map<int, int>& CGroupWnd::getRSymbol()
{
	return gsl::not_null<CGridWnd*>(m_GridWnd[0].get())->_mRsymbol;
}

bool CGroupWnd::isCodeSymbol(CString code)
{
	if (code.CompareNoCase("S0000") == 0 || code[0] == 'X')
		return true;	

	for (int ii = 0; ii < m_nGroup; ii++)
	{
		if (m_GridWnd[ii] && m_GridWnd[ii]->GetSafeHwnd())
		{
			const auto& map = m_GridWnd[ii]->_mapSymbol;
			const auto ft = map.find(code);
			if (ft != map.end())
				return true;
		}
	}
	return false;
}

BOOL CGroupWnd::CheckGetRTSHoga()
{
	for (int ii = 0; ii < m_nGroup; ii++)
	{
		if (m_GridWnd[ii] && m_GridWnd[ii]->GetSafeHwnd())
		{
			if(m_GridWnd[ii]->m_bGetHogaRTS)
				return TRUE;
		}
	}
	return FALSE;
}
