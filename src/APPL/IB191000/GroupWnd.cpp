// GroupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "GroupWnd.h"
#include "MainWnd.h"
#include "multimon.h"

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
#define	deliMITER	";"

CGroupWnd::CGroupWnd(CWnd* pMainWnd) : CBaseWnd(pMainWnd)
{	
	m_nGroup = 1;

	for ( int ii = 0 ; ii < MAX_GROUP ; ii++ )
	{
		m_GridWnd[ii] = NULL;
	}
	
	m_GridWndUpjn = NULL;
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

	m_linkMap   = _T("");
	m_fontsize  = 9;
	m_fontname = _T("굴림체");
	m_bCurr = TRUE;
	m_bMAP = FALSE;

	// ADD PSH 20070917
	m_bName = TRUE;
	m_bWaveApply = FALSE;

	m_clrKospi	  = RGB(0, 0, 0);
	m_clrKosdaq	  = RGB(128, 64, 64);
	m_clrbookMark = RGB(255,128,255);
	m_clrbookMarkPen = RGB(0, 0, 0);
	m_bMKClrApply = FALSE;
	m_bPopupDlg   = FALSE;
	m_bDispCode   = FALSE;

	m_cntGroup = 0;

	m_iDragIn = 0;
	m_isRecommand = false;

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
	struct userdata{
		char usid[8];		//사용자Id
		char innm[100];		//INI파일명
		char senm[100];		//시간데이터
		char skey[100];		//일자데이터	
		char valu[500];		//카운트
		char date[8];		//업데이트일자
	};

	struct mid{
		char gubn[1];
		struct userdata userdata;
	};

	struct mid mid;
	//ZeroMemory(&mid, sizeof(mid));
	memset(&mid, ' ', sizeof(mid));


	mid.gubn[0] = 'I';
	CString m_user = Variant(userCC);
	CString m_filename = "IB202200";

	CTime time;
	CString m_time, m_day, m_update, strCount;
	int count = GetDragInCount();
	
	if(count > 0)
	{
		strCount.Format("%04d", count);
		
		time = CTime::GetCurrentTime();
		
		m_day.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
		m_time.Format("%02d%02d%02d", time.GetHour(), time.GetMinute(), time.GetSecond());		
		m_update.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
		
		CopyMemory(mid.userdata.usid, m_user.operator LPCTSTR(), min(sizeof(mid.userdata.usid), strlen(m_user)));
		
		CopyMemory(mid.userdata.innm, m_filename.operator LPCTSTR(), min(sizeof(mid.userdata.innm), strlen(m_filename)));
		CopyMemory(mid.userdata.senm, m_time.operator LPCTSTR(), min(sizeof(mid.userdata.senm), strlen(m_time)));
		CopyMemory(mid.userdata.skey, m_day.operator LPCTSTR(), min(sizeof(mid.userdata.skey), strlen(m_day)));
		CopyMemory(mid.userdata.valu, strCount.operator LPCTSTR(), min(sizeof(mid.userdata.valu), strlen(strCount)));
		CopyMemory(mid.userdata.date, m_update.operator LPCTSTR(), min(sizeof(mid.userdata.date), strlen(m_update)));
		
		int sMid = sizeof(mid);
		char *sendB = new char[sMid];
		ZeroMemory(sendB, sMid);
		CopyMemory(sendB, &mid, sizeof(mid));
		
		//	SendTR
		CSendData	sdata;
		char	key;
		_trkey* trkey = (struct _trkey*)&key;
		
		trkey->group = m_nIndex;
		trkey->kind = TRKEY_TRCOUNT;
		
		sdata.SetData("pidouini", key, sendB, strlen(sendB), "");
		
		m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
	}

}

void CGroupWnd::OperDraw(CDC* pDC)
{
	if (m_bMAP)
		DrawTitle(pDC);
	else
		DrawDivide(pDC);
}

void CGroupWnd::DrawTitle(CDC* pDC)
{
	pDC->FillSolidRect(m_rcTitle, RGB(255, 255, 0));
	int	nmode = pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_szTITLE, m_rcTitle, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	pDC->SetBkMode(nmode);
}

LONG CGroupWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	int	ii = 0;
	switch(LOWORD(wParam)) 
	{
	case MK_INTSAVE://BUFFET
		OnDestroySave();
		break;
	case MK_GROUPSAVE:
		saveWhenGroupClosed((int)lParam);
		break;
	case MK_SISECATCH:
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->SendMessage(WM_MANAGE, wParam, lParam);
			}
			m_GridWndUpjn->SendMessage(WM_MANAGE, wParam, lParam);
		}
		break;
	case MK_MEMO:
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
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

		((CMainWnd*)m_pMainWnd)->SendMinMaxInfo();
		break;
	case MK_TURNCODE:
		{
			int	nsec = HIWORD(wParam);
			if (nsec == 0)
			{
				for ( int ii = 0 ; ii < m_nGroup ; ii++ )
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
			loadfield();

			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->FieldSetup(false);
				m_GridWnd[ii]->Invalidate();
			}

			m_nCurSel = 0;
		

			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
			m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MK_TURNCODE);
			SelectOper();

			ReSetupGrid();
		
			
		}
		break;
	case MK_CHANGEFIELD:
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
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
				CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
				pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
				m_GridWnd[m_nCurSel]->SendMessage(WM_MANAGE, MK_TURNCODE);
				if (m_nCurSel != (int)lParam)
				{
					m_nCurSel = (int)lParam;
					SelectOper();
				}	
				
				UINT kind = m_GridWnd[m_nCurSel]->GetKind();
				int indx = CAST_TREEID(kind)->value;
 				saveGroupIndex(indx);
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
			for (int ii = 0 ; ii < m_nGroup ; ii++ )
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

		((CMainWnd*)m_pMainWnd)->SendMinMaxInfo();

//		InitGrid();
		RefreshGrid();
		break;
	case MK_ENDDRAG:
		{
			for ( int ii = 0 ; ii < m_cntGroup ; ii++ )
			{
				m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_ENDDRAG);
			}			
		}
		break;
	case MK_GETMAX:
		{
			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
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
	case MK_TREEDATA:
		{
			if(m_isRecommand = true)
			{
				m_isRecommand = false;
				SetEqualizerField(2);
			}
			CGridData*	pGridData = (CGridData*)lParam;
			SendGrid(m_nCurSel, pGridData);						//5	
		}
		break;
	case MK_UPJONGDATA:
		{
			CGridData*	pGridData = (CGridData*)lParam;
			SendUpjnGrid(pGridData);
		}	
		break;
	case MK_RECVDATA:
		RecvOper(HIWORD(wParam), (CRecvData*)lParam);
		break;
	case MK_RTSDATA:
		RecvRTS((CRecvData*)lParam);
		break;
	case MK_SELINT:
		SendGrid(m_nCurSel, (int)lParam);
		break;
	// ADD PSH 20070917
	case MK_MKPROP:
		{
			_marker* pMkProp = (_marker*)lParam;

			if (NULL != pMkProp)
			{
				for ( int ii = 0 ; ii < m_cntGroup ; ii++ )
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
			CUsefulSetupDlg* pDlg = (CUsefulSetupDlg*)lParam;
			
			if (NULL != pDlg)
			{
				for ( int ii = 0 ; ii < m_cntGroup ; ii++ )
				{
					m_GridWnd[ii]->SetAutoSaveCnd(pDlg->m_bAddCnd, pDlg->m_bChgCnd);
				}
			}
		}
		break;
	case MK_ALERTWAVE:
		{
			CUsefulSetupDlg* pDlg = (CUsefulSetupDlg*)lParam;
			m_bWaveApply  = pDlg->m_bWaveApply;
			m_strWaveFile = pDlg->m_strFile;

			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->m_bWaveApply  = m_bWaveApply;
				m_GridWnd[ii]->m_strWaveFile = m_strWaveFile;
			}

			m_pMainWnd->SendMessage(WM_MANAGE, MK_ALERTWAVE, lParam);
		}
		break;
	case MK_MARKETCOLOR:
		{
			CUsefulSetupDlg* pDlg = (CUsefulSetupDlg*)lParam;
			m_bMKClrApply = pDlg->m_bClrApply;
			m_clrKospi	  = pDlg->m_clrKospi;
			m_clrKosdaq   = pDlg->m_clrKosdaq;

			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->SetMarketColor(m_bMKClrApply, m_clrKospi, m_clrKosdaq);
				m_GridWnd[ii]->Invalidate();
			}

			m_pMainWnd->SendMessage(WM_MANAGE, MK_MARKETCOLOR, lParam);
		}
		break;
	case MK_MARKETCODE:
		{
			m_bDispCode = (BOOL)lParam;

			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->SetDispCode(m_bDispCode);
				m_GridWnd[ii]->Invalidate();
			}

			m_pMainWnd->SendMessage(WM_MANAGE, MK_MARKETCODE, lParam);
		}
		break;
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
	// END ADD
	default:
		ret = CBaseWnd::OnManage(wParam, lParam);
		break;
	}

	return ret;
}

//테스트용
void CGroupWnd::testSaveFile(CString dt)
{
	CString strPath(_T("")), m_root, m_user;
	strPath.Format("c:\\test.ini");
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	CString titletime;
	
	titletime.Format("[%s]GroupWnd::RecvOper", strCurTime);
	
	WritePrivateProfileString("IB202200", titletime, (LPCTSTR)dt, (LPCTSTR)strPath);
}

void CGroupWnd::RecvOper(int kind, CRecvData* rdata)
{
	_trkey* trkey = (struct _trkey*)&kind;

// 	if(m_isRecommand == true)
// 	{
// 		SetEqualizerField(2);
// 	}

	switch (trkey->kind)
	{
	case TRKEY_GRIDNEWS:
	case TRKEY_GRIDROW:
	case TRKEY_GRIDNEW:
//		testSaveFile("NULL");
		m_GridWnd[trkey->group]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)rdata);
		break;
	case TRKEY_RECOMMAND:
		m_GridWnd[trkey->group]->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)rdata);
		m_isRecommand = true;
//		SetEqualizerField(3);
		break;
	case TRKEY_GRIDUPJONG:
		m_GridWndUpjn->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)rdata);
		break;
	}
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
	m_pBrush = GetAxBrush(RGB(100, 100, 100));
	m_clrBack = GetAxColor(68);
	m_bkColor1 = GetAxColor(68);
	m_bkColor2 = GetAxColor(77);//LINECOLOR BUFFET
	m_rtmColor = GetAxColor(8);
	m_clrbookMark = GetAxColor(199);
	m_clrbookMarkPen = GetAxColor(69);
	m_chUse = 0x00;

	for (int ii = 0; ii < 4; ii++)
		m_fgColor[ii] = GetAxColor(39);

	// load info
	m_nGroup = GetPrivateProfileInt(SEC_GROUP, KEY_COUNT, 1, m_fileCFG);
	
	if (m_nGroup <= 0)
		m_nGroup = 1;
	
	if (m_nGroup > MAX_GROUP)
		m_nGroup = MAX_GROUP;
	
	m_cntGroup = m_nGroup;


	int	readL = 0;
	char	readB[1024];
	CString	string, stringx;
	
	memset(readB, 0x00, sizeof(readB));
	GetPrivateProfileString(SEC_GROUP, KEY_RATE, "", readB, sizeof(readB), m_fileCFG);
	string.Format("%s", readB);
	
	if (string.IsEmpty())
	{
		for ( int ii = 0 ; ii < m_nGroup ; ii++ )
		{
			string += "100,";
		}
	}

	SetArray(string);
	m_nCurSel = GetPrivateProfileInt(SEC_GROUP, KEY_SELECT, 0, m_fileCFG);

	loadfield();
}


int CGroupWnd::loadGroupCount()
{
	CString strPath, string;
	char readB[1024];
	strPath.Format("%s/%s/%s/portfolio.ini", m_root, USRDIR, m_user);
	int readL = 0, count = 0;
	
	readL = GetPrivateProfileString(SEC_GROUPORDER, "00", "", readB, sizeof(readB), strPath);
	string = CString(readB, readL);
	
	for(int ii=0 ; !string.IsEmpty() ; ii++)
	{
		IH::Parser(string, ";");
		count++;
	}
	
	return count;
}

void CGroupWnd::addGridWnd()
{
		for ( int ii = m_nGroup-1 ; ii < m_nGroup ; ii++ )
		{
			m_GridWnd[ii] = new CGridWnd(m_pMainWnd, ii);
			
			m_GridWnd[ii]->m_bWaveApply  = m_bWaveApply;
			m_GridWnd[ii]->m_strWaveFile = m_strWaveFile;
			m_GridWnd[ii]->m_bPopupDlg   = m_bPopupDlg;
			m_GridWnd[ii]->m_bDispCode   = m_bDispCode;
			
			m_GridWnd[ii]->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_GRIDWND + ii);
			
			m_GridWnd[ii]->SetMarketColor(m_bMKClrApply, m_clrKospi, m_clrKosdaq);
		}

		m_cntGroup = m_nGroup;
}

void CGroupWnd::loadfield()
{
	CString filePath;
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);
	int	readL = 0;
	char	readB[1024];
	CString	string, stringx;
	
	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_BKCOLOR, "1", readB, sizeof(readB), filePath);

	m_chUse = readB[0];//BK 사용/미사용

	if (readB[0] == '2')
	{
		readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_COLOR2, string, readB, sizeof(readB), filePath);
		if (readL > 0)
		{
			string = CString(readB, readL);

			m_line = atoi(IH::Parser(string, deliMITER));
			if (m_line <= 0) m_line = 1;
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
	if (readL > 0) m_clrbookMark = _httoi(readB);

	readL = GetPrivateProfileString(SEC_INTERFIELD, "BOOKMARKPENCOLOR", "", readB, sizeof(readB), filePath);
	if (readL > 0) m_clrbookMarkPen = _httoi(readB);

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_REALCOLOR, "", readB, sizeof(readB), filePath);
	if (readL > 0) m_rtmColor = _httoi(readB);

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
		case '1': m_rtmAction = rtmCELL;	break;
		case '2': m_rtmAction = rtmROW;		break;
		default:  m_rtmAction = rtmNONE;	break;
		}
	}

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_LINKMAP, "", readB, sizeof(readB), filePath);
	if (readL > 0)
	{
		string = CString(readB, readL);

		stringx = IH::Parser(string, deliMITER);
		if (stringx.GetLength() > 0)
		{
			switch (stringx.GetAt(0))
			{
			case '1': m_dblAction = dblCURR;	break;
			case '2': m_dblAction = dblCHART;	break;
			case '4':
			default:  m_dblAction = dblUSER;
				  m_linkMap = IH::Parser(string, deliMITER); break;
			}
		}
	}

	m_selAction = GetPrivateProfileInt(SEC_INTERFIELD, "SELACTION", 1, filePath);
	m_overlap = GetPrivateProfileInt(SEC_INTERFIELD, "OVERLAP", 0, filePath);
	m_overlapAlert = GetPrivateProfileInt(SEC_INTERFIELD, "OVERLAPALERT", 0, filePath);

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FONTPOINT, "9", readB, sizeof(readB), filePath);
	
	if (readL > 0) 
		m_fontsize = atoi(CString(readB, readL));

	if (m_fontsize < 8 || m_fontsize > 12)
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
	}	

	m_fontname = _T("굴림체");

	readL = GetPrivateProfileString(SEC_INTERFIELD, KEY_FONTNAME, "굴림체", readB, sizeof(readB), filePath);
	
	if (readL > 0) 
		m_fontname.Format("%s", readB);


	m_bCurr = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_CURRBOLD, 1, filePath);
	m_bName = (BOOL)GetPrivateProfileInt(SEC_INTERFIELD, KEY_NAMEBOLD, 1, filePath);

	
}

void CGroupWnd::saveOverLap(int overlap)
{
	CString filePath;
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);
	
	m_overlap = overlap;

	CString	sztmp = _T("");
	sztmp.Format("%d", overlap);
	WritePrivateProfileString(SEC_INTERFIELD, "OVERLAP", sztmp.operator LPCTSTR(), filePath);
}

void CGroupWnd::savecfg()
{
	// delete resource
	CString	sztmp = _T(""), sztmp2 = _T(""), title = _T("");
	// group count
	sztmp.Format("%d", m_nGroup);	
	WritePrivateProfileString(SEC_GROUP, KEY_COUNT, sztmp.operator LPCTSTR(), m_fileCFG);
	
	// groupwnd size rate
	sztmp = _T("");
	int	ncnt = m_arRATE.GetSize();

	WritePrivateProfileString(SEC_GROUP, KEY_RATE, sztmp.operator LPCTSTR(), m_fileCFG);

	// save Select group 
	sztmp.Format("%d", m_nCurSel);
	WritePrivateProfileString(SEC_GROUP, KEY_SELECT, sztmp.operator LPCTSTR(), m_fileCFG);
}

void CGroupWnd::OperInit()
{
//	m_cntGroup = loadGroupCount();

	loadcfg();
	CreateGrid();

	RefreshGrid();

	SelectOper();

	//트리에 선택시켜 TR발생하게끔한다.
	//관심그룹 선택 방법 변경
	SetTimer(MK_SELFIELD, 400, NULL);
	
}

void CGroupWnd::InitSetGroup()
{
	int gIndex = GetPrivateProfileInt(SEC_MAIN, KEY_SELECTGROUP, 0, m_fileCFG);
	UINT   val = MAKE_TREEID(xINTEREST, 0, 2, gIndex, TD_ITEM);

	//트리, 툴 그룹 선택하기
	m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)val);

	CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));	
	pWnd->SendMessage(WM_MANAGE, MK_CALLINTEREST, val);
}


//관심그룹 index 저장
void CGroupWnd::saveGroupIndex(int index)
{
	CString	str;
	str.Format("%02d", index);
	WritePrivateProfileString(SEC_MAIN, KEY_SELECTGROUP, str.operator LPCTSTR(), m_fileCFG);
}

void CGroupWnd::InitGrid()
{
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	class CGridData	sdata;
	UINT	kind = 0;
	int	max = 0;
	CMapWordToPtr	mapNews;
	int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	
	int saveGroupInfo[MAX_GROUP];
	bool isFind = false;
	
	int arryIndex = 0;
	for(int i=0 ; i< MAX_GROUP ; i++)
	{
		saveGroupInfo[i] = 0;
	}
	
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		sdata.Reset();
		kind = m_GridWnd[ii]->GetKind();
		
		if (CAST_TREEID(kind)->kind == xISSUE)
		{
			mapNews.SetAt(ii, NULL);
			continue;
		}
		
		if (kind)
		{
			sdata.SetKind(kind);
			
			if (nOver == MO_VISIBLE)
//				max = (m_nGroup - ii) * GetRowCount();
				max = (m_nGroup - ii) * MAX_LINE;
			else if (nOver == MO_FULL)
				max = (m_nGroup - ii) * MAX_LINE;
			else
				max = MAX_LINE;
			pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata);  //1
			
			int i = 0;
			isFind = false;
			
			while(i < MAX_GROUP)
			{
				//배열안에 종목이 있는지 확인
				if(saveGroupInfo[i] == kind)
				{
					isFind = true;
					break;
				}
				
				i++;
			}
			
			if(isFind == false)						
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
	int group_data;
	for (int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		group_data = 0;
		void*	ptr = NULL;
		if (mapNews.Lookup(ii, ptr))
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_NEWS);
			continue;
		}
		group_data = m_GridWnd[ii]->loadcfg_data();
		
//		if(group_data != 0)
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
		}
	}
}

void CGroupWnd::RefreshGrid()
{
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	class CGridData	sdata;
	UINT	kind = 0;
	int	max = 0;
	CMapWordToPtr	mapNews;
	int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	int saveGroupInfo[MAX_GROUP];
	bool isFind = false;
	
	int arryIndex = 0;
	for(int i=0 ; i< MAX_GROUP ; i++)
	{
		saveGroupInfo[i] = 0;
	}

	if (nOver == MO_VISIBLE)
	{
		kind = m_GridWnd[m_nCurSel]->GetKind();
		
		if(kind == 0)
		{
			MessageBox("선택한 화면은 빈 공간입니다. 원하시는 그룹을 선택하신 후 버튼을 클릭하여 주세요.", "IBK투자증권");
			CWnd* pToolWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			pToolWnd->SendMessage(WM_COMMAND, IDC_BT_ADDSELECT);
			return;
		}
	}

	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
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
			mapNews.SetAt(ii, NULL);
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
			
			if (nOver == MO_VISIBLE)
			{
				//max = (m_nGroup - ii) * GetRowCount();
				max = (m_nGroup - ii) * MAX_LINE;
			}
			else if (nOver == MO_FULL)
				max = (m_nGroup - ii) * MAX_LINE;
			else
				max = MAX_LINE;
			pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata);  //1
			
			int i = 0;
			isFind = false;

			while(i < MAX_GROUP)
			{
				//배열안에 종목이 있는지 확인
				if(saveGroupInfo[i] == kind)
				{
					isFind = true;
					break;
				}
				
				i++;
			}
			
			if(isFind == false && sdata.GetCount() > 0)						
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
	for (int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		void*	ptr = NULL;
		if (mapNews.Lookup(ii, ptr))
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_NEWS);
			continue;
		}

		if(m_GridWnd[ii]->GetKind())
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
		}
	}
}


void CGroupWnd::ReSetupGrid()
{
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	class CGridData	sdata;
	UINT	kind = 0;
	int	max = 0;
	CMapWordToPtr	mapNews;
	int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));

	int saveGroupInfo[MAX_GROUP];
	bool isFind = false;
	
	int arryIndex = 0;
	for(int i=0 ; i< MAX_GROUP ; i++)
	{
		saveGroupInfo[i] = 0;
	}

	if (nOver == MO_VISIBLE)
	{
		kind = m_GridWnd[m_nCurSel]->GetKind();
		
		if(kind == 0)
		{
			MessageBox("선택한 화면은 빈 공간입니다. 원하시는 그룹을 선택하신 후 버튼을 클릭하여 주세요.", "IBK투자증권");
			CWnd* pToolWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
			pToolWnd->SendMessage(WM_COMMAND, IDC_BT_ADDSELECT);
			return;
		}
	}

	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
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
			mapNews.SetAt(ii, NULL);
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
			
			if (nOver == MO_VISIBLE)
			{
				//max = (m_nGroup - ii) * GetRowCount();
				max = (m_nGroup - ii) * MAX_LINE;
			}
			else if (nOver == MO_FULL)
				max = (m_nGroup - ii) * MAX_LINE;
			else
				max = MAX_LINE;
			pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata);  //1
			
			int i = 0;
			isFind = false;

			while(i < MAX_GROUP)
			{
				//배열안에 종목이 있는지 확인
				if(saveGroupInfo[i] == kind)
				{
					isFind = true;
					break;
				}
				
				i++;
			}
			
			if(isFind == false && sdata.GetCount() > 0)						
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
	for (int  ii = 0 ; ii < m_nGroup ; ii++ )
	{
		void*	ptr = NULL;
		if (mapNews.Lookup(ii, ptr))
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_NEWS);
			continue;
		}

		if(m_GridWnd[ii]->GetKind())
		{
			m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
		}
	}
}

void CGroupWnd::SetArray(CString rates)
{	
	CString	sztmp;
	
	CRect rcRect;
	GetClientRect(&rcRect);
	CRect rcPrev = rcRect;

	m_arRATE.RemoveAll();
	
	for ( int ii = 0 ; ii < MAX_GROUP ; ii++ )
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
	int	total = 0;

	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
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

	int nTotSize = getTotalSize();
	int nGapSize = 0;
	
	nGapSize = SIZE_DIVIDE * m_nGroup +2 ;

	CRect rcGrid = rcFull;
	rcGrid.right = 0;
	CRect rcDivide = rcFull;

	CRect rcUp = rcFull;
	CRect rcDn = rcFull;
	
	rcUp.bottom = rcGrid.top + SIZE_DIVIDE;
	rcDn.top = rcGrid.bottom - SIZE_DIVIDE;

	rcGrid.top = rcGrid.top + SIZE_DIVIDE;//BUFFET BLOCK
	rcGrid.bottom  = rcGrid.bottom - SIZE_DIVIDE;//BUFFET BLOCK

	int nWidth = (rcFull.Width() - nGapSize) / m_nGroup;

	for (int i = 0; i < m_nGroup+1; i++)//BUFFET +1
	{
		rcGrid.left = rcGrid.right;
		
		rcUp.left = rcGrid.right;
		rcDn.left = rcGrid.right;

		rcGrid.left += SIZE_DIVIDE;		
		//if (i == m_nGroup - 1)
		if (i == m_nGroup - 1 )//BUFFET +1
			nWidth = (rcFull.Width() - nGapSize) - (nWidth * (m_nGroup - 1));

		rcGrid.right = rcGrid.left + nWidth ;

		rcUp.right = rcGrid.right  ;
		rcDn.right = rcGrid.right  ;
		
		rcDivide.left = rcGrid.left - SIZE_DIVIDE;

		rcDivide.right = rcGrid.left;
		
		//if (i < m_nGroup - 1)
		if (i < m_nGroup+1 )//BUFFET +1
			m_arDIVIDE.Add(rcDivide);// 

		if ( i < m_nGroup)
		{
			m_arRECT.Add(rcGrid);
			m_arUpDIV.Add(rcUp);
			m_arDnDIV.Add(rcDn);

			m_arRATE.SetAt(i, rcGrid.Width());
		}
	}

/*
	int nCount = m_arRECT.GetSize();
	m_arDIVIDE.RemoveAll();

	if (nCount == 0)
		return;

	int nHeight = m_rect.Height();
	int nPrevWidth = 0;
	int nCurWidth = rcFull.Width() - (m_nGroup - 1) * SIZE_DIVIDE;
	int nCalcWidth = 0;
	int nAvgWidth = 0;
	int nFindCount = 0;
	int nGridWidth = 0;
	CRect rcPrev(0, 0, 0, 0);
	CRect rcCur;
	CRect rcDivide = rcFull;

	BOOL bIncrease = FALSE;
	BOOL bFind = TRUE;
	BOOL bOK = FALSE;
	
	for (int i = 0; i < nCount; i++)
		nPrevWidth += m_arRECT.GetAt(i).Width();

	if (nPrevWidth < nCurWidth)
		bIncrease = TRUE;
	else if (nPrevWidth == nCurWidth)
		return;

	nCalcWidth = nCurWidth;

	while (bFind)
	{
		bFind = FALSE;

		if (nCount == nFindCount)
			break;

		nAvgWidth = nCalcWidth / (nCount - nFindCount);

		nCalcWidth = nCurWidth;
		nFindCount = 0;

		for (int i = 0; i < nCount; i++)
		{
			rcCur = m_arRECT.GetAt(i);

			if ((bIncrease && nAvgWidth > rcCur.Width()) || (!bIncrease && nAvgWidth < rcCur.Width()))
				bOK = TRUE;
			else
				bOK = FALSE;

			if (bOK)
			{
				bFind = TRUE;
				nFindCount++;
				nCalcWidth -= rcCur.Width();
			}

			bOK = FALSE;
		}
	}

	for (i = 0; i < nCount; i++)
	{
		rcCur = m_arRECT.GetAt(i);
		nGridWidth = rcCur.Width();

		rcCur.left = rcPrev.right;
		
		if (i > 0)
			rcCur.left += SIZE_DIVIDE;

		if ((bIncrease && nAvgWidth > nGridWidth) || (!bIncrease && nAvgWidth < nGridWidth))
			rcCur.right = rcCur.left + nAvgWidth;
		else
			rcCur.right = rcCur.left + nGridWidth;
		
		rcDivide.left = rcCur.right;
		rcDivide.right = rcDivide.left + SIZE_DIVIDE;

		if (i < m_nGroup - 1)
			m_arDIVIDE.Add(rcDivide);

		rcPrev = rcCur;

		if (rcCur.Height() == 0)
			rcCur.bottom += nHeight;

		m_arRECT.SetAt(i, rcCur);
		m_arRATE.SetAt(i, rcCur.Width());
	}

*/
/*

	int	total = getTotalSize();
	int	nGapsize = SIZE_DIVIDE * (m_nGroup - 1);
	int	nWidth = rcFull.Width();
	int	nReal = nWidth - nGapsize;
	CRect	rect(rcFull.left, rcFull.top, 0, rcFull.bottom);
	CRect	rcDivide = rect;
	CRect	rcGroup;
	int	val = 0;
	
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		if (ii == (m_nGroup - 1))
		{
			rect.right = rcFull.right;
			rcGroup = rect;
		}
		else
		{
			val = m_arRATE.GetAt(ii);
			rect.right = (int)((double)rect.left + ((double)(nReal * val) / (double)total));
//			if (rect.Width() <= SIZE_GROUPMINX)
//				rect.right = rect.left + SIZE_GROUPMINX;
			rcGroup = rect;
			rcDivide.left = rect.right; 
			rcDivide.right = rcDivide.left + SIZE_DIVIDE + 1;
			rect.left = rcDivide.right;
			m_arDIVIDE.Add(rcDivide);
		}

		m_arRECT.Add(rcGroup);					
	}
*/
}

/* fdgdgfdgfdg */
void CGroupWnd::OperResize(int cx, int cy)
{
	CRect	rect(0, 0, cx, cy);
	CRect	rcGrid;	
	m_rcTitle = rect;
	m_rcTitle.bottom = m_rcTitle.top + 20;
	MakeGridRect(rect);

	int ii = 0;

	for ( ii = 0 ; ii < m_nGroup ; ii++ )
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

	for ( ; ii < m_cntGroup ; ii++ )
	{
		m_GridWnd[ii]->ShowWindow(SW_HIDE);
	}

	InvalidateDivide();
}

void CGroupWnd::CreateGrid()
{
	int ii = 0;
	for ( ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii] = new CGridWnd(m_pMainWnd, ii);
	
		m_GridWnd[ii]->m_bWaveApply  = m_bWaveApply;
		m_GridWnd[ii]->m_strWaveFile = m_strWaveFile;
		m_GridWnd[ii]->m_bPopupDlg   = m_bPopupDlg;
		m_GridWnd[ii]->m_bDispCode   = m_bDispCode;

		m_GridWnd[ii]->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_GRIDWND + ii);

		m_GridWnd[ii]->SetMarketColor(m_bMKClrApply, m_clrKospi, m_clrKosdaq);
	}

	// 업종만 볼 목적으로 추가 생성
	ii = ii+1;
	m_GridWndUpjn = new CGridWnd(m_pMainWnd, ii);
	
	m_GridWndUpjn->m_bWaveApply  = m_bWaveApply;
	m_GridWndUpjn->m_strWaveFile = m_strWaveFile;
	m_GridWndUpjn->m_bPopupDlg   = m_bPopupDlg;
	m_GridWndUpjn->m_bDispCode   = m_bDispCode;
	
	m_GridWndUpjn->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_GRIDWND + ii);

}

void CGroupWnd::DestroyGrid()
{
	for ( int ii = 0 ; ii < m_cntGroup ; ii++ )
	{
		if (LIB_IsWndAlive(m_GridWnd[ii]))
			LIB_DeleteWnd(m_GridWnd[ii]);
	}

	if (LIB_IsWndAlive(m_GridWndUpjn))
			LIB_DeleteWnd(m_GridWndUpjn);
}

void CGroupWnd::OperDestory()
{
//	SendUini();		//테스트용 로그

	savecfg();
	DestroyGrid();
}

void CGroupWnd::DrawDivide(CDC* pDC)
{
	int	ncnt = 0;
	CRect	rect;
	
	COLORREF clrLight = GetAxColor(64);

	ncnt = m_arUpDIV.GetSize();

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		rect = m_arUpDIV.GetAt(ii);
			
		if ( ncnt > 1 && m_nCurSel == ii)
			clrLight = RGB(255, 70, 70);
		else
			clrLight = GetAxColor(gClrBack);

		DrawHigV(pDC,rect,clrLight,clrLight);

		rect = m_arDnDIV.GetAt(ii);
		
		DrawHigV(pDC,rect,clrLight,clrLight);
	}
	// END MODIFY
	
	ncnt = m_arDIVIDE.GetSize();

	for (int ii = 0 ; ii < ncnt ; ii++ )
	{
		rect = m_arDIVIDE.GetAt(ii);
			
		if ( ncnt > 2 && 
			( m_nCurSel == ii || (m_nCurSel+1) == ii) )
			clrLight = RGB(255, 70, 70);
		else
			clrLight = GetAxColor(gClrBack);
		
		DrawHighlight(pDC,rect,clrLight,clrLight);
	}
}

void CGroupWnd::DrawHigV(CDC* pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow)
{
	pDC->FillSolidRect(rect, clrShadow);
	CPen	penW(PS_SOLID, 1, clrLight),penB(PS_SOLID, 2, clrShadow);
	CPen*	oldpen = NULL;

	oldpen = pDC->SelectObject(&penW);
	pDC->MoveTo(rect.TopLeft());
	pDC->LineTo(rect.right, rect.top);
	pDC->SelectObject(oldpen);
	penW.DeleteObject();

	oldpen = pDC->SelectObject(&penB);
	pDC->MoveTo(rect.left, rect.bottom);
	pDC->LineTo(rect.BottomRight());
	pDC->SelectObject(oldpen);
	penB.DeleteObject();
}

void CGroupWnd::DrawHighlight(CDC* pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow)
{
	CPen penLight, penShadow, *pOldPen;

	penLight.CreatePen(PS_SOLID, 1, clrLight);
	penShadow.CreatePen(PS_SOLID, 2, clrShadow);

	pOldPen = pDC->SelectObject(&penLight);

	pDC->SelectObject(&penLight);
		

	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left, rect.bottom);

	pDC->SelectObject(&penShadow);

	pDC->MoveTo(rect.right-1, rect.top);
	pDC->LineTo(rect.right-1, rect.bottom);

	pDC->SelectObject(pOldPen);
}

int CGroupWnd::GroupPlus(BOOL bExpand)
{
	int	nWidth = m_arRATE.GetAt(m_nGroup - 1);
	int nScrMax = GetSystemMetrics(SM_CXVIRTUALSCREEN); 
	BOOL isReset = TRUE;												//화면 리셋할것인지 여부
	BOOL bTree = (BOOL)m_pMainWnd->SendMessage(WM_MANAGE, MK_ISTREE);
	
	m_nGroup++;
	

	for ( int ii = m_nGroup - 2 ; ii < m_nGroup ; ii++ )
	{
		m_arRATE.SetAt(ii, nWidth);
	}
	
	if (bExpand)
	{
		if (m_arRECT.GetSize())
		{
			CRect	rect = m_arRECT.GetAt(m_arRECT.GetUpperBound());

			int nTree = bTree ? SIZE_TREE : 0;


			if (  (m_arRECT.GetSize()+1) * rect.Width() + nTree > nScrMax ) //over screen size
			{
				m_nGroup--;
				isReset = FALSE;
				MessageBox("표시할수 있는 범위를 벗어 났습니다.","IBK투자증권", MB_OK);
			}else
			{
				if(m_cntGroup < m_nGroup)
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
		if(m_cntGroup < m_nGroup)
		{
			addGridWnd();
		}

		OperResize(m_rect.Width(), m_rect.Height());


	}

	if(isReset == TRUE)
	{
		m_GridWnd[m_nGroup - 1]->FieldSetup(false);
		m_GridWnd[m_nGroup - 1]->Reset();
	}

	return m_nGroup;
}

int CGroupWnd::GroupMinus(BOOL bExpand)
{
// 	if(m_nGroup>1)
// 	{
// 		m_nGroup--;
// 	}

	m_nGroup--;	

	if (bExpand)
	{
		if (m_arRECT.GetSize())
		{
			CRect	rect = m_arRECT.GetAt(m_arRECT.GetUpperBound());
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




	return m_nGroup;
}

void CGroupWnd::ExpectOper(int param)
{
	bool	bExpect = false;

	if (param == MO_ON)
		bExpect = true;
	
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii]->toggleAction(bExpect);	
	}
}


void CGroupWnd::AutoExpectOper(int param)
{
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii]->toggleAction(false);	
	}
}


void CGroupWnd::SetEqualizerField(int param)
{
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		if(param == 1)
		{
			m_GridWnd[ii]->FieldSetup(false, 1);
		}
		else if(param == 2)
		{
			m_GridWnd[ii]->FieldSetup(false, 2);
		}
		else
		{
			m_GridWnd[ii]->FieldSetup(false, 3);
		}
		
	
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
	}

	if(param != 3)
	{
		m_GridWnd[0]->SaveAsInterField();
	}
}


void CGroupWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
//	TRACE("CGroupWnd::OnMouseMove\n");

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
	}
	
	CBaseWnd::OnMouseMove(nFlags, point);
}

void CGroupWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_bDivide = TRUE;
	m_xPosMove = m_xPos = point.x;
	m_nIndex = GetIndexDivide(point);

	if ( m_nIndex < 0 ) return;

	CRect	rect = GetArea(m_nIndex);
	m_rcDIVIDE = m_arDIVIDE.GetAt(m_nIndex+1);
	DrawMove(m_rcDIVIDE);

	ClientToScreen(rect);
	ClipCursor(rect);
	CBaseWnd::OnLButtonDown(nFlags, point);
}

void CGroupWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	ClipCursor(NULL);
	if (m_bDivide && point.x != m_xPos)
	{
		if ( m_nIndex < 0 ) return;

		CRect	rcDivide = GetMovedRect(m_nIndex, point.x - m_xPos);//BUFFET +1
		m_arDIVIDE.SetAt(m_nIndex+1, rcDivide);//BUFFET +1
		
		//BUFFET FOR REPOSITION
		SetMovedRect(m_nIndex, point.x - m_xPos);

		CRect	rcL, rcR, rect;
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
		for ( ii = 0 ; ii < m_nGroup ; ii++ )
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
		
		for ( ; ii < m_nGroup ; ii++ )
		{
			m_GridWnd[ii]->ShowWindow(SW_HIDE);
		}
		
		InvalidateDivide();
//		OperResize(m_rect.Width(), m_rect.Height());
	}

	DrawMove(m_rcDIVIDE);
	DrawMove(m_rcUpDIV);//BUFFET
	DrawMove(m_rcDnDIV);//BUFFET

	m_bDivide = FALSE;
	m_rcDIVIDE.SetRectEmpty();
	m_rcUpDIV.SetRectEmpty();
	m_rcDnDIV.SetRectEmpty();

	m_xPos = -1;
	m_nIndex = -1;
	CBaseWnd::OnLButtonUp(nFlags, point);
}

int CGroupWnd::GetIndexDivide(CPoint pt)
{
	int	ncnt = m_arDIVIDE.GetSize();

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		if (m_arDIVIDE.GetAt(ii).PtInRect(pt))
		{
			if ( ii == ncnt-1 ) return -1;
			else return (ii-1);
		}
	}

	return -1;
}

CRect CGroupWnd::GetMovedRect(int nIndex, int xPos)
{
	CRect	rect = m_arDIVIDE.GetAt(nIndex+1);
	rect.OffsetRect(xPos, 0);
	return rect;
}

int CGroupWnd::SetMovedRect(int nIndex, int xPos)
{
	if ( nIndex < 0 ) return 0;

	CRect	rect = m_arUpDIV.GetAt(nIndex);
	rect.right = rect.right + xPos;
	m_arUpDIV.SetAt(nIndex, rect);

	rect = m_arDnDIV.GetAt(nIndex);
	rect.right = rect.right + xPos;
	m_arDnDIV.SetAt(nIndex, rect);

	for ( int i = nIndex + 1 ; i < nIndex + 2 ; i++ )
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
	CString	strBUF = _T("");
	for ( int ii = 0 ; ii < m_nGroup+1 ; ii++ )//BUFFET +1
	{
		strBUF += "100,";
	}
	SetArray(strBUF);
	OperResize(m_rect.Width(), m_rect.Height());
	CBaseWnd::OnLButtonDblClk(nFlags, point);
}

CRect CGroupWnd::GetArea(int nIndex)
{
	CRect	rcL, rcR;
	CRect	rcRet;
	rcL = m_arRECT.GetAt(nIndex);
	rcR = m_arRECT.GetAt(nIndex + 1);
	rcRet = rcL;
	rcRet.right = rcR.right - SIZE_GROUPMINX + 1;
	rcRet.left = rcL.left + SIZE_GROUPMINX;
	if (rcRet.right <= rcRet.left)
		rcRet = m_arDIVIDE.GetAt(nIndex+1);//BUFFET +1
	return rcRet;
}

void CGroupWnd::DrawMove(CRect rect)
{
	CClientDC	dc(this);
	int	nMode = dc.SetROP2(R2_XORPEN);
	CBrush	*oldbrush = dc.SelectObject(m_pBrush);
	dc.Rectangle(rect);
	dc.SelectObject(oldbrush);
	dc.SetROP2(nMode);
}

void CGroupWnd::SelectOper()
{
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SELGROUP);
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_HIDECODE);
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_TURNCODE);
	}

	UINT	val = m_GridWnd[m_nCurSel]->GetKind();
	m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)val);

	
	((CMainWnd*)m_pMainWnd)->SetRTSTreeData(m_GridWnd[m_nCurSel]);
}

//설정화면 누를시 분할표시된 화면의 가장 앞부분에 선택되도록 수정
void CGroupWnd::selectFirst()
{
	UINT	CurrentKind = m_GridWnd[m_nCurSel]->GetKind();
	UINT	beforeKind = 0;

	while(m_nCurSel > 0)
	{
		beforeKind = m_GridWnd[m_nCurSel-1]->GetKind();

		if(CurrentKind != beforeKind)
		{
			break;
		}
		else
		{
			m_nCurSel--;
		}
	}

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	pWnd->SendMessage(WM_MANAGE, MK_TURNCODE);
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
	CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *ss = mstr;
	int result = 0;
	if (*ss == '0' && *(ss + 1) == 'X') ss += 2;
	bool firsttime = true;
	while (*ss != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*ss == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		ss++;
		firsttime = false;
	}
	free(mstr);
	return result;
}

//테스트용
void CGroupWnd::testSaveFile()
{
	CString strPath(_T("")), m_root, m_user;
	strPath.Format("c:\\test.ini");
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	CString titletime;
	
	titletime.Format("[%s]GroupWnd::SendGrid", strCurTime);

	WritePrivateProfileString("IB202200", titletime, "before Send the data", (LPCTSTR)strPath);
}

void CGroupWnd::SendGrid(int nIndex, UINT kind)
{
//	testSaveFile();

	int	max = MAX_LINE;
	CGridData sdata;
	if (CAST_TREEID(kind)->kind == xINTEREST)
	{
		for (int ii = 0 ; ii < m_nGroup ; ii++ )
		{
			if (ii == m_nCurSel)
				continue;
			UINT	datakind = m_GridWnd[ii]->GetKind();
			if (datakind == kind)
				m_GridWnd[ii]->Reset();
		}
	}
	sdata.SetKind(kind);
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, max), (LPARAM)&sdata);
	m_GridWnd[nIndex]->SetKind(kind);
	m_GridWnd[nIndex]->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)&sdata);

	m_GridWnd[nIndex]->SendMessage(WM_MANAGE, MK_SENDTR);
}


void CGroupWnd::SendUpjnGrid(CGridData* sdata)
{
	m_GridWndUpjn->SendMessage(WM_MANAGE, MK_UPJONGDATA, (LPARAM)sdata);
}


void CGroupWnd::SendGrid(int nIndex, CGridData* sdata)
{
	UINT	kind = sdata->GetKind();
	if (kind == 0)
		return;

	if (CAST_TREEID(kind)->kind == xINTEREST)
	{
		for (int ii = 0 ; ii < m_nGroup ; ii++ )
		{
			if (ii == m_nCurSel)
				continue;

			UINT	datakind = m_GridWnd[ii]->GetKind();

			if (datakind == kind)
				m_GridWnd[ii]->Reset();
		}
	}

	int	nEnd = OverOper(nIndex, sdata);
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	for (int ii = nIndex ; ii <= nEnd && ii < m_nGroup; ii++ ) 
	{
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
	}
	
	if(nOver == MO_VISIBLE)
	{
		for (int ii = nEnd+1 ; ii < m_nGroup ; ii++ ) 
		{
			m_GridWnd[ii]->Reset();
		}	
	}
}


int CGroupWnd::OverOper(int nIndex, CGridData* sdata)
{
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));

	int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
	UINT	kind = sdata->GetKind();
	int	nOld = nIndex;
	int	nGroup = m_nGroup - nIndex;

	if (sdata->GetCount() == 0)
		nOver = MO_SELECT;

	if (CAST_TREEID(kind)->kind == xISSUE && CAST_TREEID(kind)->kind == xINTEREST)
		nOver = MO_SELECT;
	
	switch (nOver)
	{
	case MO_VISIBLE:
		{		
			int	max = MAX_LINE;

			if (nOver == MO_VISIBLE)
			{
				max = GetRowCount();
				
				int gridWidth = m_GridWnd[nIndex]->GetWidth();
				int intGridWidth = m_GridWnd[nIndex]->GetGridWidth();

				if((gridWidth - intGridWidth) < 0)
				{
					max = max - 1;
				}
			}

			if (max == 0)
				max = MAX_LINE;

			int	ncnt = sdata->GetCount();
			
			if(ncnt > MAX_LINE)
			{
				ncnt = MAX_LINE;	
			}

			m_GridWnd[nIndex]->SetKind(kind);
			m_GridWnd[nIndex]->RemoveAll();
			
			int	nPos = 0;
			int kk = 0;
			for ( kk = 0 ; kk < ncnt ; kk++ )
			{
				if (kk != 0 && (kk % max) == 0)
				{
					nIndex++;

					if (nIndex == m_nGroup)
						break;

					m_GridWnd[nIndex]->RemoveAll();
					
					nPos = 0;
					//KET_DATA를 봐서 nIndex는 증가했는데 KEY_DATA가 0일경우 데이터를 저장
					int keydata = m_GridWnd[nIndex]->loadcfg_data();
					int beforekeydata = 0;

					if(nIndex-1 >= 0)
					{
						beforekeydata = m_GridWnd[nIndex-1]->loadcfg_data();
					}
					
					CString strKey; 
					if(keydata != beforekeydata)
					{
						strKey.Format("%d", beforekeydata);
						m_GridWnd[nIndex]->savecfg_data(strKey);
					}
					

					
					m_GridWnd[nIndex]->SetKind(kind);					
				}				

				m_GridWnd[nIndex]->AddData(nPos++, sdata->m_arDatas.GetAt(kk));							
			}				
			
			//그룹에 못보여진 종목이 있을경우 마지막 창에 append한다
			while(kk < ncnt)
			{
				m_GridWnd[nIndex-1]->AddData(nPos++, sdata->m_arDatas.GetAt(kk));	
				kk++;
			}
		}
		break;
	case MO_FULL:
		break;
	case MO_SELECT:
		m_GridWnd[nIndex]->SetKind(kind);
		m_GridWnd[nIndex]->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)sdata);	
		break;
/*
	case MO_HORZ:
		{
			int	ncnt = sdata->GetCount();
			int	gcnt = m_nGroup - nIndex;
			
			if (ncnt < gcnt)
				gcnt = ncnt;
			
			for ( int jj = nIndex ; jj < (nIndex + gcnt) ; jj++ )
			{
				m_GridWnd[jj]->SetKind(kind);
				m_GridWnd[jj]->RemoveAll();
			}
			
			int	nPos = 0;
			for ( int kk = 0 ; kk < ncnt ; kk++ )
			{
				if (kk != 0 && (kk % gcnt) == 0)
					nPos++;
				m_GridWnd[nIndex + (kk % gcnt)]->AddData(nPos, sdata->m_arData.GetAt(kk));
			}
			
			nIndex += gcnt;
		}
		break;
*/
	}

	return nIndex;
}

void CGroupWnd::RecvRTS(CRecvData* rdata)
{
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_RTSDATA, rdata->m_lParam);
	}
	m_GridWndUpjn->SendMessage(WM_MANAGE, MK_RTSDATA, rdata->m_lParam);
}

void CGroupWnd::CloseGrid(int nIndex)
{
	if (m_nGroup == 1)
		return;

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	BOOL bExpand = (BOOL)pWnd->SendMessage(WM_MANAGE, MK_ISEXPAND);
	
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

	int	width = m_arRATE.GetAt(m_arRATE.GetUpperBound());
	m_arRATE.Add(width);
	m_arRATE.RemoveAt(nIndex);

	for ( int ii = nIndex ; ii < (m_nGroup - 1) ; ii++ )
	{
		m_GridWnd[ii]->Assign(m_GridWnd[ii + 1]);
		m_GridWnd[ii]->SendMessage(WM_MANAGE, MK_SENDTR);
	}
	
	if (nIndex <= m_nCurSel)
		m_nCurSel--;
	if (m_nCurSel < 0)
		m_nCurSel = 0;

	SelectOper();
	GroupMinus(bExpand);

//	pWnd->SendMessage(WM_MANAGE, MK_SENDTREE);
}

void CGroupWnd::InvalidateDivide()
{
	int	ncnt = m_arDIVIDE.GetSize();
	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		InvalidateRect(m_arDIVIDE.GetAt(ii), FALSE);
	}

	ncnt = m_arUpDIV.GetSize();
	for (int ii = 0 ; ii < ncnt ; ii++ )
	{
		InvalidateRect(m_arUpDIV.GetAt(ii), FALSE);
		InvalidateRect(m_arDnDIV.GetAt(ii), FALSE);
	}
}

BOOL CGroupWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	int	ncnt = m_arDIVIDE.GetSize();
	CPoint	pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		if (m_arDIVIDE.GetAt(ii).PtInRect(pt))
		{
			if ( ii == 0 || ii == ncnt -1 ) return TRUE;
			else
			::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_LR));
			return TRUE;
		}
	}

	return CBaseWnd::OnSetCursor(pWnd, nHitTest, message);
}

int CGroupWnd::GetRowCount()
{
	int	nHeight = (int)m_pMainWnd->SendMessage(WM_MANAGE, MK_GROUPHEIGHT);

	return (int)((double)nHeight / (double)m_rowH - 3);
}

void CGroupWnd::SetGridBkColor(COLORREF color)
{
	for (int ii = 0 ; ii < m_nGroup ; ii++ )
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
	int nCount = m_arRECT.GetSize();

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
	if ( !IsNewDrop() )
		return;
	else
	{
		if (MessageBox("[관심종목]변경 사항을 저장하시겠습니까?\n", "관심종목 저장 확인창", MB_YESNO) == IDYES)
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				if ( m_GridWnd[ii]->m_bEditWork )
				m_GridWnd[ii]->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BT_SAVE,0), 0);
			}
		}
	}
}

void CGroupWnd::saveWhenGroupClosed(int index)
{
	if ( !IsNewDrop() )
		return;
	else
	{
		if (MessageBox("[관심종목]변경 사항을 저장하시겠습니까?\n", "관심종목 저장 확인창", MB_YESNO) == IDYES)
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				if ( m_GridWnd[ii]->m_bEditWork )
					m_GridWnd[ii]->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BT_SAVE,0), 0);
			}
			
//			m_GridWnd[ii]->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_BT_SAVE,0), 0);
		}		
		else
		{
			for ( int ii = 0 ; ii < m_nGroup ; ii++ )
			{
				m_GridWnd[ii]->m_bEditWork = false;
			}
		}	

	}
}


BOOL CGroupWnd::IsNewDrop()
{ 
	BOOL ret = FALSE;
	for ( int ii = 0 ; ii < m_nGroup ; ii++ )
	{
		if ( m_GridWnd[ii]->m_bEditWork )
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

	for(int index=0 ; index<GetGroupCount() ; index++)
	{
		currentGroup = m_GridWnd[index]->GetKind();
		
		if(currentGroup)
		{
			sum = sum + m_GridWnd[index]->GetRowcount();
		}
	}

	return sum;
}


int CGroupWnd::WriteFileSumtoEachGroup(UINT curruntGroup)			// index : 그룹 시작점
{
	UINT nextGroup;
	int curruntGroupCount, nextGroupCount, gno;
	CString	code = _T(""), amount = _T(""), price = _T(""), name = _T(""), bookmark = _T("");

	CString strPath(_T("")), strTemp(_T("")), strBook(_T(""));
	CString strSendData(_T(""));

	int index = 0;
	//현재 펼쳐져있는 그룹들과 현그룹과 비교하여 같은 그룹인지 확인(제일 앞단에 있는 그룹을 찾는다)
	for(index=0 ; index<GetGroupCount() ; index++)
	{
		nextGroup = m_GridWnd[index]->GetKind();

		if(curruntGroup == nextGroup)
		{
			curruntGroup = m_GridWnd[index]->GetKind();
			//일단 첫번째 그룹의 데이터를 파일에 쓴다
			if (CAST_TREEID(curruntGroup)->kind == xINTEREST)
				gno = CAST_TREEID(curruntGroup)->value;
			else
				return 0;	

			break;
		}
	}

	strPath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, gno);
	strBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_user, gno);

	char szTemp[10];
	int nScnt = 0;
	
	strPath += ".tmp";
	
	struct _inters* pInters;

	::DeleteFile(strPath);
	CFile	file(strPath, CFile::modeWrite|CFile::modeCreate);
	
	::DeleteFile(strBook);
	CFile	file2(strBook, CFile::modeWrite|CFile::modeCreate);
	
	sprintf(szTemp, "%02d", gno);
	CString strGrouptName = m_GridWnd[index]->GetGroupName(gno);

	curruntGroupCount = m_GridWnd[index]->GetRowcount();

	for(int ii=0 ; ii<curruntGroupCount ; ii++)
	{
		pInters = m_GridWnd[index]->GetData(ii);

		if (strlen(pInters->code) && strlen(pInters->name) == 0)
		{
			CString strName = m_GridWnd[index]->GetcolName(ii);
			strcpy(pInters->name, strName);
		}
		
		file.Write(pInters, sz_inters);

		struct _bookmarkinfo binfo;
//		FillMemory(&binfo, sizeof(_bookmarkinfo), ' ');
		ZeroMemory(&binfo, sizeof(_bookmarkinfo));

		if(strlen(pInters->code) == 0)
		{
			CopyMemory(binfo.code, "            ", sizeof(binfo.code));
			CopyMemory(binfo.name, "", sizeof(binfo.name));
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

				
	//같은 그룹일 경우 옆그룹들의 데이터를 파일에 쓰기
	for(int ii=index+1 ; ii<GetGroupCount() ; ii++)
	{
		nextGroup = m_GridWnd[ii]->GetKind();

		if(curruntGroup == nextGroup)
		{
			nextGroupCount = m_GridWnd[ii]->GetRowcount();
	
			for(int jj=0 ; jj<nextGroupCount ; jj++)
			{
				m_GridWnd[ii]->m_bEditWork = FALSE;		//한 섹션만 TRUE, FALSE 담당하면 된다

				pInters = m_GridWnd[ii]->GetData(jj); 

				if (strlen(pInters->code) && strlen(pInters->name) == 0)
				{
					CString strName = m_GridWnd[ii]->GetcolName(jj);
					strcpy(pInters->name, strName);
				}

				file.Write(pInters, sz_inters);
				
				struct _bookmarkinfo binfo;
//				FillMemory(&binfo, sizeof(_bookmarkinfo), ' ');
				ZeroMemory(&binfo, sizeof(_bookmarkinfo));

				if(strlen(pInters->code) == 0)
				{
					CopyMemory(binfo.code, "      ", sizeof(binfo.code));
					CopyMemory(binfo.name, "", sizeof(binfo.name));
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
		}
	}

	file2.Close();
	file.Close();

	return gno;
}

CString CGroupWnd::MakePacket(CString& code, CString amount, CString price, CString name, CString bookmark)
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
		
		InitSetGroup();
	}

	CBaseWnd::OnTimer(nIDEvent);
}

CGridWnd* CGroupWnd::GetGridWndUpjn()
{
	return m_GridWndUpjn; 
}