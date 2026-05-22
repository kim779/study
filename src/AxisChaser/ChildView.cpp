// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "AxisChaser.h"
#include "ChildView.h"
#include "chaser.h"
#include "TextColorDlg.h"
#include "FontSetDlg.h"
#include "Options.h"

#define STAB		"\t"
#define SNEWLINE	"\n"
#define CHARLIMIT	15

#define TM_STAYONTOP 9899
 
struct _exeCDSS
{
	DWORD	flag;
	DWORD	len;
};
#define L_cdss	sizeof(struct _exeCDSS)

struct  axishdr {                       /* message header for axis      */
        unsigned char   msgk;           /* I,O message id.              */
        unsigned char   actf;           /* action flags                 */
        unsigned char   chkf;           /* checking flags               */
        unsigned char   xwin;           /* major window id              */
        unsigned char   ywin;           /* minor window id              */
        unsigned char   keyc[1];        /* action key code              */
        unsigned char   keyf[1];        /* next key action flags        */
        unsigned char   svcc[4];        /* service region code          */
        unsigned char   trxc[8];        /* I,O transaction code         */
        unsigned char   mlen[5];        /* length of message data       */
};
#define L_axishdr	sizeof(struct axishdr)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT CChildView::m_findMSG = RegisterWindowMessage(FINDMSGSTRING);
/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	m_bBINARY   = TRUE;
	m_bSNDRCV   = TRUE;
	m_bRTM	    = TRUE;
	m_bSYSTRACE = TRUE;
	m_bREPORT   = FALSE;
	m_bNOSCROLL = FALSE;
	m_bSTAYONTOP= FALSE;

	for (int ii = 0; ii < IDX_MAX; ii++)
	{
		m_bBold[ii]     = FALSE;
		m_bItalic[ii]   = FALSE;
		m_bUline[ii]    = FALSE;
		m_bStrike[ii]   = FALSE;
	}

	m_font.CreatePointFont(100, "Arial");
	m_rtsL = 2;
	m_matchN = -1;

	m_findDlg = NULL;
}

CChildView::~CChildView()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_MODE_BINARY, OnModeBinary)
	ON_UPDATE_COMMAND_UI(ID_MODE_BINARY, OnUpdateModeBinary)
	ON_COMMAND(ID_MODE_TEXT, OnModeText)
	ON_UPDATE_COMMAND_UI(ID_MODE_TEXT, OnUpdateModeText)
	ON_COMMAND(ID_SNDRCV, OnTrBase)
	ON_UPDATE_COMMAND_UI(ID_SNDRCV, OnUpdateTrBase)
	ON_COMMAND(ID_RTM, OnRtm)
	ON_UPDATE_COMMAND_UI(ID_RTM, OnUpdateRtm)
	ON_COMMAND(ID_SYSTEMTRACE, OnSystemtrace)
	ON_UPDATE_COMMAND_UI(ID_SYSTEMTRACE, OnUpdateSystemtrace)
	ON_COMMAND(ID_SETBKCOLOR, OnSetbkcolor)
	ON_COMMAND(ID_SETTEXTCOLOR, OnSettextcolor)
	ON_COMMAND(ID_SETFONT, OnSetfont)
	ON_COMMAND(ID_CLEAR, OnClear)
	ON_WM_DESTROY()
	ON_COMMAND(ID_NOSCROLL, OnNoscroll)
	ON_UPDATE_COMMAND_UI(ID_NOSCROLL, OnUpdateNoscroll)
	ON_COMMAND(ID_STAYONTOP, OnStayontop)
	ON_UPDATE_COMMAND_UI(ID_STAYONTOP, OnUpdateStayontop)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_NEXT_FIND, OnNextFind)
	ON_COMMAND(ID_PREV_FIND, OnPrevFind)
	ON_COMMAND(ID_STRING_FIND, OnStringFind)
	ON_COMMAND(ID_REPORTFORMAT, OnReportformat)
	ON_UPDATE_COMMAND_UI(ID_REPORTFORMAT, OnUpdateReportformat)
	ON_COMMAND(ID_OPTIONS, OnOptions)
	ON_REGISTERED_MESSAGE(m_findMSG, OnFindDialogMessage)
	ON_COMMAND(ID_SAVE, OnSave)
	ON_COMMAND(ID_SAVEAS, OnSaveas)
	ON_EN_CHANGE(IDC_EDIT_KEYWORD, OnChangeEditKeyword)
	ON_BN_CLICKED(IDC_CHK_FILTER, OnClickedChkFilter)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RECEIVE, OnReceive)
	ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers
void CChildView::OnChangeEditKeyword()
{
	m_editKeyword.GetWindowText(m_strKeyword);
}

void CChildView::OnClickedChkFilter()
{
	m_bFilterOn = (m_chkFilter.GetCheck() == BST_CHECKED);
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	AfxOleInit();
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_READONLY;
	if (!m_trace.Create(dwStyle, CRect(0, 0, 0, 0), this, (UINT) &m_trace))
		return -1;

	Init();

	CRect rcEdit(10, 10, 250, 32);
	m_editKeyword.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		rcEdit, this, IDC_EDIT_KEYWORD);

	CRect rcChk(260, 10, 380, 32);
	m_chkFilter.Create(_T("키워드 필터"),
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		rcChk, this, IDC_CHK_FILTER);

	//m_editKeyword.SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,
	//	SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	//m_chkFilter.SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0,
	//	SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	m_editKeyword.SetFont(GetFont());
	m_chkFilter.SetFont(GetFont());

	m_bFilterOn = FALSE;
	m_strKeyword.Empty();


	SetTimer(TM_STAYONTOP, 1000, nullptr);

	return 0;
}

void CChildView::OnDestroy() 
{
	CAxisChaserApp* app = (CAxisChaserApp*) AfxGetApp();
	app->WriteProfileInt(TRACESETTING, BKCOLOR, m_bColor);

	app->WriteProfileInt(TRACESETTING, "BINARY", m_bBINARY);
	app->WriteProfileInt(TRACESETTING, "SNDRCV", m_bSNDRCV);
	app->WriteProfileInt(TRACESETTING, "RTM", m_bRTM);
	app->WriteProfileInt(TRACESETTING, "SYSTRACE", m_bSYSTRACE);
	app->WriteProfileInt(TRACESETTING, "REPORT", m_bREPORT);
	app->WriteProfileInt(TRACESETTING, "NOSCROLL", m_bNOSCROLL);
	app->WriteProfileInt(TRACESETTING, "STAYONTOP", m_bSTAYONTOP);

#ifdef NDEBUG
	setTrace(false);
#endif
	CWnd ::OnDestroy();
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);

	const int filterHeight = 28;  // 필터 영역 높이 (여백 포함)
	const int margin = 4;
	const int editWidth = 240;
	const int chkWidth = 120;
	const int ctrlHeight = 22;

	if (m_editKeyword.GetSafeHwnd())
	{
		m_editKeyword.MoveWindow(margin, margin, editWidth, ctrlHeight);
	}
	if (m_chkFilter.GetSafeHwnd())
	{
		m_chkFilter.MoveWindow(margin + editWidth + margin, margin,
			chkWidth, ctrlHeight);
	}

	if (m_trace.GetSafeHwnd())
	{
		CRect	cRc;
		GetClientRect(cRc);
		cRc.top += 30;
		m_trace.MoveWindow(cRc);
	}
}

void CChildView::OnRCVData(WPARAM wParam, LPARAM lParam)
{
	int	row, maxCnt = 20, len = LOWORD(wParam);
	CString	string, tmpS, timeS;
	SYSTEMTIME	tm;

	GetLocalTime(&tm);
	timeS.Format("[ %02d:%02d:%02d:%03d ]", tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
	char*	dat = (char *) lParam;
	switch (HIWORD(wParam))
	{
	case x_RCVs:
	case x_SNDs:
		if (!m_bSNDRCV)	break;
		if (HIWORD(wParam) == x_SNDs && !m_options.send)	break;
		if (HIWORD(wParam) == x_RCVs && !m_options.receive)	break;
		if (HIWORD(wParam) == x_SNDs)
			string.Format("\n>>>>>>>>>>>>>>>>>>>> [Send Data %d Bytes][%s] <<<<<<<<<<<<<<<<<<<< \n", len, timeS);
		else	string.Format("\n<<<<<<<<<<<<<<<<<<<< [Receive Data %d Bytes][%s] >>>>>>>>>>>>>>>>>>>> \n", len, timeS);

		addTrace(string, K_SNDRCV);
		if (!len)
		{
			addTrace("\n", K_SNDRCV);
			return;
		}
		
		if (!m_options.header && !m_options.data)
		{
			break;
		}
		else if (m_options.header && !m_options.data)
		{
			if (len > L_axishdr)
				len = L_axishdr;
		}
		else if (!m_options.header && m_options.data)
		{
			if (len > L_axishdr)
			{
				len -= L_axishdr;
				dat += L_axishdr;
			}
		}

		string.Empty();
		if (!m_bBINARY)	maxCnt = 80;

		for (row = 0; row < len / maxCnt + 1; row++)
		{
			CString sRow, sDat = "  ";
			sRow.Format("%05d  ", row * maxCnt);
			
			for (int ii = row * maxCnt; ii < (row + 1) * maxCnt; ii++)
			{
#ifdef DF_MBCS
				if (ii < len)
				{
					tmpS.Format("%02X", (unsigned char)dat[ii]);
					sRow += tmpS;
					if (ii == row * maxCnt + 9)
						sRow += "-";
					else	sRow += " ";
				}
				else
					sRow += "   ";

				if (ii < len)
				{
					unsigned char ch = (unsigned char)dat[ii];

					// CP949 lead byte이고, 다음 바이트가 같은 줄 안에 있고, trail byte이면 한글로 처리
					if (_ismbblead(ch)
						&& (ii + 1) < len
						&& (ii + 1) < (row + 1) * maxCnt
						&& _ismbbtrail((unsigned char)dat[ii + 1]))
					{
						unsigned char ch2 = (unsigned char)dat[ii + 1];

						// ASCII 칸에 한글 2바이트 그대로
						sDat += (char)ch;
						sDat += (char)ch2;

						// 다음 바이트(trail)의 hex도 미리 출력하고 ii 증가
						ii++;
						tmpS.Format("%02X", ch2);
						sRow += tmpS;
						if (ii == row * maxCnt + 9)
							sRow += "-";
						else	sRow += " ";
			}
					else if (0x20 <= ch && ch <= 0x7f)
					{
						sDat += (char)ch;
					}
					else
					{
						sDat += '.';
					}
		}
				else	sDat += " ";
#else
				if (ii < len)
				{
					tmpS.Format("%02X", (unsigned char) dat[ii]);
					sRow += tmpS;
					if (ii == row * maxCnt + 9)
						sRow += "-";
					else	sRow += " ";
				}
				else
					sRow += "   ";
				
				if (ii < len)
				{
					char ch = dat[ii];
					if (0x20 <= ch && ch <= 0x7f)
						sDat += ch;
					else	sDat += '.';
				}
				else	sDat += " ";
#endif
			}

			sDat  += '\n';
			if (!m_bBINARY)	sRow.Format("%05d  ", row * maxCnt);
			sRow  += sDat;
			string += sRow;
		}
		addTrace(string, K_SNDRCV);
		break;
	case x_RTMs:
		if (m_bRTM)
		{
			string = dat;		
			CString code = parse(string, STAB);
			if (m_options.rts && !m_mapCode.Lookup(code, string))	break;

			string.Format("\n----- [RTM DATAs %d Bytes] %s -----\n", len, timeS);
			addTrace(string, K_RTMS);	string = dat;

			tmpS.Format("< RTS KEY = %s >\n", parse(string, STAB));
			addTrace(tmpS, K_RTMS);

			CString rtm = string;
			for (int xx = 0; !rtm.IsEmpty(); xx++)
			{
				string = parse(rtm, SNEWLINE);

				CMapStringToString fms;
				CStringArray	   ary;
				CString		   symS, value;
				
				row = 1;
				parseData(string, fms, ary);
				string.Empty();
				for (int ii = 0; ii < ary.GetSize(); ii++)
				{
					symS = ary.GetAt(ii);
					fms.Lookup(symS, value);
					
					if (value.GetLength() > CHARLIMIT)
					{
						if (string.IsEmpty())
							tmpS.Format("\t[ %s ] : %s\n", symS, value);
						else	tmpS.Format("\n\t[ %s ] : %s\n", symS, value);
						string += tmpS;	row = 1;
					}
					else
					{
						tmpS.Format("\t[ %s ] : %-15s", symS, value);
						string += tmpS;
						if (row++%m_rtsL == 0 && ii != ary.GetSize()-1)
							string +=  "\n";
					}
				}
				string +=  "\n";
				if (xx)	string = "\n" + string;
				addTrace(string, K_RTMS);
				fms.RemoveAll();
				ary.RemoveAll();
			}

		}
		break;
	case x_STRs:
		if (len <= 0)	break;
		if (m_bSYSTRACE)
		{
			tmpS.Format("\n* SYSTEM TRACE * [ %s ] %s\n", dat, timeS);
			addTrace(tmpS, K_SYSTRACE);
		}
		break;
	case x_CONs:
		if (m_bREPORT)
		{
			string = "*******************************************************\n";	addTrace(string, K_REPORT);
			string = "*                   REPORT DATAs                      *\n";	addTrace(string, K_REPORT);
			string = "*******************************************************\n";	addTrace(string, K_REPORT);
			string.Format("◈ %s ◈", timeS);	addTrace(string, K_REPORT);
			string = dat;

			CMapStringToString fms;
			CStringArray	   ary;
			CString		   symS, value;

			row = 1;
			ReportParse(string, fms, ary);
			string.Empty();
			for (int ii = 0; ii < ary.GetSize(); ii++)
			{
				symS = ary.GetAt(ii);
				fms.Lookup(symS, value);

				if (value.GetLength() > CHARLIMIT)
				{
					if (string.IsEmpty())
						tmpS.Format("\t[ %s ] : %s\n", symS, value);
					else	tmpS.Format("\n\t[ %s ] : %s\n", symS, value);
					string += tmpS;	row = 1;
				}
				else
				{
					tmpS.Format("\t[ %s ] : %-15s", symS, value);
					string += tmpS;
					if (row++%m_rtsL == 0 && ii != ary.GetSize()-1)
						string +=  "\n";
				}
			}
			string +=  "\n";	addTrace(string, K_REPORT);
			fms.RemoveAll();
			ary.RemoveAll();
		}
		break;
	}
}

void CChildView::addTrace(CString dat, int kind)
{
	CString slog;
	CString stmp;
	stmp = dat;
	stmp.TrimRight();
	stmp.Replace("\n", "");
	slog.Format("[addTrace] [%d][%s]", stmp.GetLength(), stmp);
	OutputDebugString(slog);

	if (dat.IsEmpty())	return;
	if (m_bFilterOn && !m_strKeyword.IsEmpty() && dat.Find(">>>>") < 0 && dat.Find("<<<<") < 0)
	{
		if (dat.Find(m_strKeyword) < 0)
			return;  // 키워드 없으면 스킵
	}

	if (m_findDlg)
		m_findDlg->SetFocus();
//	else	SetFocus();
	m_trace.SetSel(-1, -1);
#if 0
	m_trace.ReplaceSel(dat);
#else
	CRTFBuilder	c;

	if (m_bSameFont)
	{
		c << font(m_fName[IDX_ALL]) << size(m_fSize[IDX_ALL]);

		if (m_bBold[IDX_ALL])	c << bold;
		if (m_bItalic[IDX_ALL])	c << italic;
		if (m_bStrike[IDX_ALL])	c << strike;
		if (m_bUline[IDX_ALL])	c << underline;
	}
	else
	{
		switch (kind)
		{
		default:	c << font(m_fName[IDX_ALL]) << size(m_fSize[IDX_ALL]);	break;
		case K_SNDRCV:	c << font(m_fName[IDX_SNR]) << size(m_fSize[IDX_SNR]);	break;
		case K_RTMS:	c << font(m_fName[IDX_RTM]) << size(m_fSize[IDX_RTM]);	break;
		case K_SYSTRACE:c << font(m_fName[IDX_SYS]) << size(m_fSize[IDX_SYS]);	break;
		case K_REPORT:	c << font(m_fName[IDX_RPT]) << size(m_fSize[IDX_RPT]);	break;
		}

		if (m_bBold[kind])	c << bold;
		if (m_bItalic[kind])	c << italic;
		if (m_bStrike[kind])	c << strike;
		if (m_bUline[kind])	c << underline;
	}

	if (m_bSameColor)
		c << color(m_tColor[IDX_ALL]);
	else
	{
		switch (kind)
		{
		default:	c << color(m_tColor[IDX_ALL]);	break;
		case K_SNDRCV:	c << color(m_tColor[IDX_SNR]);	break;
		case K_RTMS:	c << color(m_tColor[IDX_RTM]);	break;
		case K_SYSTRACE:c << color(m_tColor[IDX_SYS]);	break;
		case K_REPORT:	c << color(m_tColor[IDX_RPT]);	break;
		}
	}

	c << dat >> m_trace;
#endif
	if (m_bNOSCROLL)	return;
	m_trace.SendMessage(WM_VSCROLL, SB_BOTTOM);
}

CString CChildView::parse(CString &dat, CString separate)
{
	CString	tmpS;
	if (dat.Find(separate) == -1)
	{
		tmpS = dat;
		dat.Empty();
	}
	else
	{
		tmpS = dat.Left(dat.Find(separate));
		dat = dat.Mid(dat.Find(separate) + separate.GetLength());
	}
	return tmpS;
}

void CChildView::parseData(CString dat, CMapStringToString& ary, CStringArray& aryS)
{
	CString sym, value;
	while (!dat.IsEmpty())
	{
		int pos = dat.Find('\t');
		if (pos == -1)	break;
		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat = dat.Mid(pos);
		}
		ary.SetAt(sym, value);	aryS.Add(sym);
	}
}

void CChildView::ReportParse(CString dat, CMapStringToString& ary, CStringArray& aryS)
{
	int	pos, pos2;
	CString	str, symbol, value;

	pos = dat.Find('\n');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	while (!str.IsEmpty())
	{
		pos = str.Find('\t');
		if (pos == -1)	break;

		symbol = str.Left(pos++);
		str    = str.Mid(pos);

		pos = str.Find('\t');
		if (pos == -1)
		{
			value = str;
			str.Empty();
		}
		else
		{
			pos2 = pos;
			value = str.Left(pos2++);
			pos = value.Find('\r');
			if (pos != -1)
			{
				value = value.Left(pos++);
				str   = str.Mid(pos);
			}
			else
				str = str.Mid(pos2);
		}

		value.TrimRight();
		ary.SetAt(symbol, value);
		aryS.Add(symbol);
	}

	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;

		symbol = dat.Left(pos++);
		dat    = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat   = dat.Mid(pos);
		}

		value.TrimRight();
		ary.SetAt(symbol, value);
		aryS.Add(symbol);
	}
}

void CChildView::setTrace(bool setting)
{
	CAxisChaserApp* app = (CAxisChaserApp *) AfxGetApp();
	if (app->m_class.IsEmpty())	return;
	CWnd* wnd = CWnd::FindWindow(app->m_class, NULL);
	if (!wnd)	return;

	if (setting)
		wnd->SendMessage(WM_AXIS, MAKEWPARAM(axChaserON, 0), 0);
	else	wnd->PostMessage(WM_AXIS, MAKEWPARAM(axChaserOFF, 0), 0);
}

void CChildView::WriteFile(char* pBytes, int nBytes)
{
	CFile		Dfile;
	DWORD		pos;
	CString		filename, time;

	filename.Format("d:\\trace");
	BOOL bExist = Dfile.Open(filename, CFile::modeReadWrite);//CFile::modeCreate|CFile::modeReadWrite);
	if (!bExist)
	{
		Dfile.Open(filename, CFile::modeCreate|CFile::modeReadWrite);
		Dfile.Write(pBytes, nBytes);
	}
	else
	{
		pos = Dfile.SeekToEnd();
		Dfile.Seek((long) pos, CFile::begin);
		Dfile.Write(pBytes, nBytes);
	}
	Dfile.Close();
}

void CChildView::CopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	char*	rcvB = (char *) pCopyDataStruct->lpData;
	struct _exeCDSS* cdss = (struct _exeCDSS*) rcvB;

	DWORD	len  = cdss->len;
	DWORD	flag = cdss->flag;
#if 1
	CQue*	que = new CQue;
	que->m_flag = flag;
	que->m_nBytes = len;
	que->m_pBytes = new char[que->m_nBytes+1];
	ZeroMemory(que->m_pBytes, len+1);
	CopyMemory(que->m_pBytes, &rcvB[L_cdss], len);
	
	m_sync.Lock();
	m_que.Add(que);
	m_sync.Unlock();
	PostMessage(WM_RECEIVE);
#else
	OnRCVData(MAKEWPARAM(len, flag), (LPARAM) &rcvB[L_cdss]);
#endif
}

LRESULT CChildView::OnReceive(WPARAM wParam, LPARAM lParam)
{
	CQue*	que;
	while (m_que.GetSize())
	{
		m_sync.Lock();
		que = (CQue *) m_que.GetAt(0);
		m_que.RemoveAt(0);
		m_sync.Unlock();
		OnRCVData(MAKEWPARAM(que->m_nBytes, que->m_flag), (LPARAM) que->m_pBytes);
		delete que;
	}
	return 0;
}

void CChildView::OnModeBinary() 
{
	m_bBINARY = TRUE;
}

void CChildView::OnModeText() 
{
	m_bBINARY = FALSE;
}

void CChildView::OnTrBase() 
{
	m_bSNDRCV = !m_bSNDRCV;	
}

void CChildView::OnRtm() 
{
	m_bRTM = !m_bRTM;
}

void CChildView::OnSystemtrace() 
{
	m_bSYSTRACE = !m_bSYSTRACE;
}

void CChildView::OnReportformat() 
{
	m_bREPORT = !m_bREPORT;
}

void CChildView::OnUpdateModeBinary(CCmdUI* pCmdUI) 
{
	if (m_bBINARY)
		pCmdUI->SetRadio(TRUE);
	else	pCmdUI->SetRadio(FALSE);
}

void CChildView::OnUpdateModeText(CCmdUI* pCmdUI) 
{
	if (!m_bBINARY)
		pCmdUI->SetRadio(TRUE);
	else	pCmdUI->SetRadio(FALSE);
}

void CChildView::OnUpdateTrBase(CCmdUI* pCmdUI) 
{
	if (m_bSNDRCV)
		pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);

}

void CChildView::OnUpdateRtm(CCmdUI* pCmdUI) 
{
	if (m_bRTM)
		pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);
}

void CChildView::OnUpdateSystemtrace(CCmdUI* pCmdUI) 
{
	if (m_bSYSTRACE)
		pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);
}

void CChildView::OnUpdateReportformat(CCmdUI* pCmdUI) 
{
	if (m_bREPORT)
		pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);
}


void CChildView::OnUpdateNoscroll(CCmdUI* pCmdUI) 
{
	if (m_bNOSCROLL)
		pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);
}

void CChildView::OnUpdateStayontop(CCmdUI* pCmdUI) 
{
	if (m_bSTAYONTOP)
		pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);
}

void CChildView::OnSetbkcolor() 
{
	CColorDialog	dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_bColor = dlg.GetColor();
		m_trace.SetBackgroundColor( FALSE, m_bColor);
	}	
}

void CChildView::OnSettextcolor() 
{
	CTextColorDlg	dlg;
	if (dlg.DoModal() == IDOK)
	{
		for (int ii = 0; ii < IDX_MAX; ii++)
			m_tColor[ii] = dlg.GetTColor(ii);
		CAxisChaserApp* app = (CAxisChaserApp *) AfxGetApp();
		m_bSameColor = app->GetProfileInt(TRACESETTING, SAMECOLOR, 1);
	}
}

void CChildView::OnSetfont() 
{
	CFontSetDlg	dlg;
	if (dlg.DoModal() == IDOK)
	{
		for (int ii = 0; ii < IDX_MAX; ii++)
		{
			m_fName[ii] = dlg.GetTFontName(ii);
			m_fSize[ii] = dlg.GetTFontSize(ii);

			m_bBold[ii] = dlg.IsBold(ii);
			m_bItalic[ii] = dlg.IsItalic(ii);
			m_bUline[ii] = dlg.IsUline(ii);
			m_bStrike[ii] = dlg.IsStrike(ii);
		}
		CAxisChaserApp* app = (CAxisChaserApp *) AfxGetApp();
		m_bSameFont  = app->GetProfileInt(TRACESETTING, SAMEFONT, 1);
	}
}

void CChildView::OnClear() 
{
	m_trace.SetWindowText("");	
}
	
void CChildView::OnNoscroll() 
{
	m_bNOSCROLL = !m_bNOSCROLL;
	if (m_bNOSCROLL)
		SetFocus();
}

void CChildView::OnStayontop() 
{
	m_bSTAYONTOP = !m_bSTAYONTOP;
	if (m_bSTAYONTOP)
		GetParent()->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	else	GetParent()->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
}

void CChildView::Init()
{
	CAxisChaserApp* app = (CAxisChaserApp*) AfxGetApp();

	m_bColor = app->GetProfileInt(TRACESETTING, BKCOLOR, RGB(255, 255, 215));
	m_bSameColor = app->GetProfileInt(TRACESETTING, SAMECOLOR, 1);
	m_bSameFont  = app->GetProfileInt(TRACESETTING, SAMEFONT, 1);
	
	m_fName[IDX_ALL] = app->GetProfileString(TRACESETTING, ALLFONT, DEFAULTFONT);
	m_fName[IDX_SNR] = app->GetProfileString(TRACESETTING, SNRFONT, DEFAULTFONT);
	m_fName[IDX_RTM] = app->GetProfileString(TRACESETTING, RTMFONT, DEFAULTFONT);
	m_fName[IDX_SYS] = app->GetProfileString(TRACESETTING, SYSFONT, DEFAULTFONT);
	m_fName[IDX_RPT] = app->GetProfileString(TRACESETTING, RPTFONT, DEFAULTFONT);

	m_fSize[IDX_ALL] = app->GetProfileInt(TRACESETTING, ALLSIZE, DEFAULTFONTN);
	m_fSize[IDX_SNR] = app->GetProfileInt(TRACESETTING, SNRSIZE, DEFAULTFONTN);
	m_fSize[IDX_RTM] = app->GetProfileInt(TRACESETTING, RTMSIZE, DEFAULTFONTN);
	m_fSize[IDX_SYS] = app->GetProfileInt(TRACESETTING, SYSSIZE, DEFAULTFONTN);
	m_fSize[IDX_RPT] = app->GetProfileInt(TRACESETTING, SYSSIZE, DEFAULTFONTN);

	m_tColor[IDX_ALL] = app->GetProfileInt(TRACESETTING, ALLCOLOR, COLOR_BLACK);
	m_tColor[IDX_SNR] = app->GetProfileInt(TRACESETTING, SNRCOLOR, COLOR_BLACK);
	m_tColor[IDX_RTM] = app->GetProfileInt(TRACESETTING, RTMCOLOR, COLOR_RED);
	m_tColor[IDX_SYS] = app->GetProfileInt(TRACESETTING, SYSCOLOR, COLOR_BLUE);
	m_tColor[IDX_RPT] = app->GetProfileInt(TRACESETTING, RPTCOLOR, COLOR_BLUE);

	m_bBold[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allBold", 0);
	m_bBold[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrBold", 0);
	m_bBold[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmBold", 0);
	m_bBold[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysBold", 0);
	m_bBold[IDX_RPT] = app->GetProfileInt(TRACESETTING, "rptBold", 0);

	m_bItalic[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allItalic", 0);
	m_bItalic[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrItalic", 0);
	m_bItalic[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmItalic", 0);
	m_bItalic[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysItalic", 0);
	m_bItalic[IDX_RPT] = app->GetProfileInt(TRACESETTING, "rptItalic", 0);

	m_bUline[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allUline", 0);
	m_bUline[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrUline", 0);
	m_bUline[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmUline", 0);
	m_bUline[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysUline", 0);
	m_bUline[IDX_RPT] = app->GetProfileInt(TRACESETTING, "rptUline", 0);

	m_bStrike[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allStrike", 0);
	m_bStrike[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrStrike", 0);
	m_bStrike[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmStrike", 0);
	m_bStrike[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysStrike", 0);
	m_bStrike[IDX_RPT] = app->GetProfileInt(TRACESETTING, "rptStrike", 0);
	
	m_trace.SetBackgroundColor(FALSE, m_bColor);
	m_trace.SetFont(&m_font, FALSE);
	m_trace.LimitText(1024*1024*100);

	m_bBINARY = app->GetProfileInt(TRACESETTING, "BINARY", 1);
	m_bSNDRCV = app->GetProfileInt(TRACESETTING, "SNDRCV", 1);
	m_bRTM = app->GetProfileInt(TRACESETTING, "RTM", 1);
	m_bSYSTRACE = app->GetProfileInt(TRACESETTING, "SYSTRACE", 1);
	m_bREPORT = app->GetProfileInt(TRACESETTING, "REPORT", 0);
	m_bNOSCROLL = app->GetProfileInt(TRACESETTING, "NOSCROLL", 0);
	m_bSTAYONTOP = app->GetProfileInt(TRACESETTING, "STAYONTOP", 0);

	loadOptions();
}

void CChildView::Test() 
{
	LockWindowUpdate();
	CRTFBuilder c ;
	c << font("궁서체" );
	c << size(12) ;
	c << color( RGB(255,255,255)) << "안녕하세요" ;
	c << red	<< "Red\n"
	  << blue	<< "Blue"
	  << green	<< "Green\n"
	  << black	<< "Black\n"	
	  << bold	<< "Bold\n"
	  << italic	<< "Bold&Italic\n"
	  << underline	<< "Bold&Italic&Underline\n"
	  << strike	<< "Bold&Italic&Underline&Strike\n" 
	  << push	<< "Settings Pushed\n";

	c	<< normal ;
	c << font("Comic Sans MS" );
	c << size(7) ;
	c << red ;
	c << "About to PUSH with these new settings안녕하세요\n";
	c		<< push << blue << bold << size(20) << "Some more new settings\n" 
			<< pull << "Settings PULLED\n";
	c	<< pull << "Settings PULLED again\n" ;
			
	c
			<< strike(false)		<< "Bold&Italic&Underline\n"
			<< underline(false) << "Bold&Italic\n"
			<< italic(false)		<< "Bold\n"
			<< bold(false)			<< "Normal\n"
			<< size(30/2)					<< "Bigger\n"
			<< size(40/2)					<< "Bigger still\n"
			<< size(50/2)					<< "Bigger still\n"
			<< size(60/2)					<< "Bigger still\n"
			<< size(70/2)					<< "Bigger still\n"
			<< size(25/2)					<< "_______________\n\n"
			<< font("Tahoma")		<< "Tahoma\n"
			<< font("Arial")		<< "Arial\n"
			<< font("System")		<< "System\n"
			<< font("Courier New") 
			<< "Courier New\n"
			>> m_trace ;	
	UnlockWindowUpdate();
}

void CChildView::OnEditCopy() 
{
	m_trace.Copy();	
}

/*void CChildView::OnDataFind() 
{
	if(m_findDlg)	return;
	m_findDlg = new CFindReplaceDialog();
	m_findDlg->Create(TRUE, "", NULL, FR_DOWN, this);
}
*/

LRESULT CChildView::OnFindDialogMessage(WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_findDlg != NULL);

	// If the FR_DIALOGTERM flag is set,
	// invalidate the handle identifying the dialog box.
	if (m_findDlg->IsTerminating())
	{
		m_findDlg = NULL;
		return 0;
	}

	// If the FR_FINDNEXT flag is set,
	// call the application-defined search routine
	// to search for the requested string.
	if(m_findDlg->FindNext())
	{
		//read data from dialog
		m_findS = m_findDlg->GetFindString();
		m_bMatchCase = m_findDlg->MatchCase() == TRUE;
		m_bMatchWholeWord = m_findDlg->MatchWholeWord() == TRUE;

		//with given name do search
		SearchData(m_findS, m_bMatchCase, m_bMatchWholeWord, m_findDlg->SearchDown() == TRUE);
	}
	return 0;
}

BOOL CChildView::SearchData(CString FindName, bool bMatchCase, bool bMatchWholeWord, bool bSearchDown)
{
//	if(bMatchCase == false)	FindName.MakeUpper();

	if (!bSearchDown)
		return SearchUP(FindName, bMatchCase, bMatchWholeWord);

	CHARRANGE	cr;
	FINDTEXTEX	ft;
	DWORD		dwFlags = 0;
	
	m_trace.GetSel(cr);
	ft.chrg.cpMin = cr.cpMax;
	ft.chrg.cpMax = -1;
	ft.lpstrText = (LPSTR) (char *)FindName.operator LPCTSTR();

	if (bMatchCase)	dwFlags |= FR_MATCHCASE;
	if (bMatchWholeWord) dwFlags |= FR_WHOLEWORD;

	if (m_trace.FindText(dwFlags, &ft) != -1)
	{
		if (m_findDlg)	m_findDlg->SendMessage(WM_CLOSE);
		MovePos(ft.chrgText);
	}
	else
	{
		ft.chrg.cpMin = 0;
		ft.chrg.cpMax = cr.cpMax;
		if (m_trace.FindText(dwFlags, &ft) != -1)
		{
			if (m_findDlg)	m_findDlg->SendMessage(WM_CLOSE);
			MovePos(ft.chrgText);
		}
		else
		{
			::MessageBox(m_hWnd, "Not found string..", "Axis Chaser", MB_ICONINFORMATION);
			if (m_findDlg)	m_findDlg->SetFocus();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CChildView::SearchUP(CString FindName, bool bMatchCase, bool bMatchWholeWord)
{
	BOOL	bSearch = FALSE;
	DWORD	dwFlags = 0;

	CHARRANGE	cr, sCr;
	FINDTEXTEX	ft;

	m_trace.GetSel(cr);
	ft.chrg.cpMin = 0;
	ft.chrg.cpMax = cr.cpMin;
	ft.lpstrText = (LPSTR) (char *)FindName.operator LPCTSTR();

	if (bMatchCase)	dwFlags |= FR_MATCHCASE;
	if (bMatchWholeWord) dwFlags |= FR_WHOLEWORD;

	while (m_trace.FindText(dwFlags, &ft) != -1)
	{
		ft.chrg.cpMin = ft.chrgText.cpMax;
		sCr = ft.chrgText;
		bSearch = TRUE;
	}

	if (bSearch)
	{
		if (m_findDlg)	m_findDlg->SendMessage(WM_CLOSE);
		MovePos(sCr);
	}
	else
	{
		ft.chrg.cpMin = cr.cpMax;
		ft.chrg.cpMax = -1;
		while (m_trace.FindText(dwFlags, &ft) != -1)
		{
			ft.chrg.cpMin = ft.chrgText.cpMax;
			sCr = ft.chrgText;
			bSearch = TRUE;
		}

		if (bSearch)
		{
			if (m_findDlg)	m_findDlg->SendMessage(WM_CLOSE);
			MovePos(sCr);
		}
		else
		{
			::MessageBox(m_hWnd, "Not found string..", "Axis Chaser", MB_ICONINFORMATION);
			if (m_findDlg)	m_findDlg->SetFocus();
			return FALSE;
		}
	}

	return TRUE;
}

void CChildView::OnNextFind() 
{
	CString selText = m_trace.GetSelText();
	if (!selText.IsEmpty())	m_findS = selText;

	if (m_findS.IsEmpty())
	{
		::MessageBox(m_hWnd, "Not found string..", "Axis Chaser", MB_ICONINFORMATION);
		return;	
	}

	SearchData(m_findS, m_bMatchCase, m_bMatchWholeWord, TRUE);	
}

void CChildView::OnPrevFind() 
{
	CString selText = m_trace.GetSelText();
	if (!selText.IsEmpty())	m_findS = selText;

	if (m_findS.IsEmpty())
	{
		::MessageBox(m_hWnd, "Not found string..", "Axis Chaser", MB_ICONINFORMATION);
		return;	
	}

	SearchData(m_findS, m_bMatchCase, m_bMatchWholeWord, FALSE);
}

void CChildView::MovePos(CHARRANGE cr)
{
	m_trace.SetFocus();
	m_trace.SetSel(cr);


	int lineCnt = m_trace.GetLineCount();
	int moveLine = m_trace.LineFromChar(-1);
	SCROLLINFO	sinfo;

	m_trace.GetScrollInfo(SB_VERT, &sinfo);
	int lineScale = sinfo.nMax/lineCnt;
	int nPage = sinfo.nPage/lineScale;
	int nMax  = sinfo.nMax/lineScale;

	int scrollPos = 0;
	while (nPage/2 < moveLine)
	{
		moveLine--;
		scrollPos++;
	}

	m_trace.LockWindowUpdate();
	m_trace.SendMessage(WM_VSCROLL, SB_TOP);
	if (nMax - nPage < scrollPos)
	{
		m_trace.SendMessage(WM_VSCROLL, SB_BOTTOM);
		m_trace.UnlockWindowUpdate();
		return;
	}
	m_trace.LineScroll(scrollPos);
	m_trace.UnlockWindowUpdate();
}

void CChildView::OnStringFind() 
{
	if(m_findDlg)	return;
	m_findDlg = new CFindReplaceDialog();
	m_findDlg->Create(TRUE, "", NULL, FR_DOWN, this);	
}

void CChildView::OnOptions() 
{
	COptions	dlg;
	dlg.DoModal();

	loadOptions();
}		

void CChildView::loadOptions()
{
	m_mapCode.RemoveAll();

	CString file, value, key, section = "Options";
	char buf[512];		FillMemory(buf, sizeof(buf), ' ');
	GetCurrentDirectory(sizeof(buf), buf);
	file.Format("%s\\chaser.ini", buf);

	m_options.send = GetPrivateProfileInt(section, "send", 1, file);
	m_options.receive = GetPrivateProfileInt(section, "receive", 1, file);
	m_options.header = GetPrivateProfileInt(section, "header", 1, file);
	m_options.data = GetPrivateProfileInt(section, "data", 1, file);
	m_options.rts = GetPrivateProfileInt(section, "rts", 0, file);

	section = "code";
	for (int ii = 0; ;ii++)
	{
		key.Format("%03d", ii);
		DWORD dw = GetPrivateProfileString(section, key, "", buf, sizeof(buf), file);
		if (dw <= 0)	break;
		
		value = buf;	value.TrimLeft();	value.TrimRight();
		if (value.IsEmpty())	break;

		m_mapCode.SetAt(value, "");
	}
}

void CChildView::OnSave() 
{
	if (m_filename.IsEmpty())	
	{
		OnSaveas();
		return;
	}
	
	CString strText;
	CFile file;
	file.Open(m_filename, CFile::modeCreate | CFile::modeWrite);
	
	m_trace.GetWindowText(strText);

	file.Write((LPCSTR)strText, strText.GetLength());
	file.Close();
}

void CChildView::OnSaveas() 
{
	CFileDialog	dlg(FALSE, NULL, m_filename, 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER,
			"All Files(*.*) |*.*||", NULL );
	if (dlg.DoModal() == IDOK)
		m_filename = dlg.GetPathName();
	else 
		return;

	CString strText;
	CFile file;
	file.Open(m_filename, CFile::modeCreate | CFile::modeWrite);
	
	m_trace.GetWindowText(strText);

	file.Write((LPCSTR)strText, strText.GetLength());
	file.Close();
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TM_STAYONTOP:
		{
			KillTimer(TM_STAYONTOP);
			m_bSTAYONTOP = false;
			OnStayontop();
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

