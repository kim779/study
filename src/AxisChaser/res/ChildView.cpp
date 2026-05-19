// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "AxisChaser.h"
#include "ChildView.h"
#include "chaser.h"
#include "TextColorDlg.h"
#include "FontSetDlg.h"

#define STAB	"\t"

struct _exeCDSS
{
	DWORD	flag;
	DWORD	len;
};

#define L_cdss	sizeof(struct _exeCDSS)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	m_bBINARY   = TRUE;
	m_bSNDRCV   = TRUE;
	m_bRTM	    = TRUE;
	m_bSYSTRACE = TRUE;
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

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
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_MULTILINE|ES_READONLY;
	if (!m_trace.Create(dwStyle, CRect(0, 0, 0, 0), this, (UINT) &m_trace))
		return -1;

	Init();
	return 0;
}

void CChildView::OnDestroy() 
{
	CAxisChaserApp* app = (CAxisChaserApp*) AfxGetApp();
	app->WriteProfileInt(TRACESETTING, BKCOLOR, m_bColor);

//	setTrace(false);
	CWnd ::OnDestroy();
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	
	if (m_trace.GetSafeHwnd())
	{
		CRect	cRc;
		GetClientRect(cRc);
		m_trace.MoveWindow(cRc);
	}
}

void CChildView::OnRCVData(WPARAM wParam, LPARAM lParam)
{
	int	row, maxCnt = 20, len = LOWORD(wParam);
	bool	bHFlag = false;
	CString	string, tmpS;

	char*	dat = (char *) lParam;
	switch (HIWORD(wParam))
	{
	case x_RCVs:
	case x_SNDs:
		if (!m_bSNDRCV)	break;
		if (HIWORD(wParam) == x_SNDs)
			string.Format("#################### [Send data %d Bytes] #################### \n", len);
		else	string.Format("#################### [Receive data %d Bytes] #################### \n", len);

		addTrace(string, K_SNDRCV);
		if (!len)
		{
			addTrace("\n", K_SNDRCV);
			return;
		}
		
		string.Empty();
		if (!m_bBINARY)	maxCnt = 80;

		for (row = 0; row < len / maxCnt + 1; row++)
		{
			CString sRow, sDat;
			sRow.Format("%06d : ", row * maxCnt);
			
			for (int ii = row * maxCnt; ii < (row + 1) * maxCnt; ii++)
			{
				if (ii < len)
				{
					tmpS.Format("%02x ", (unsigned char) dat[ii]);
					sRow += tmpS;
				}
				else	sRow += "   ";
				
				if (ii == row * maxCnt + 9)	sRow += " ";
				if (ii < len)
				{
					char ch = dat[ii];
					if (ch)
					{
						switch(ch)
						{
						case 0x0a:case 0x09:case 0x08:
							ch = ' ';	break;
						}
						
						if (bHFlag && ii == row * maxCnt && ch < 0)
							sDat = " ";
						else	sDat += ch;
						if (ch < 0)	bHFlag = !bHFlag;
						if (ii == (row + 1) * maxCnt - 1 &&
							bHFlag && ii + 1 < len)
						{
							sDat += dat[ii + 1];
							bHFlag = true;
						}
					}
					else	sDat += (char) 0x7f;
				}
				else	sDat += " ";
			}

			sDat  += '\n';
			if (!m_bBINARY)	sRow.Format("%06d : ", row * maxCnt);
			sRow  += sDat;
			string += sRow;
		}
		addTrace(string, K_SNDRCV);
		break;
	case x_RTMs:
		if (m_bRTM)
		{
			string.Format("----- [RTM DATAs %d Bytes] -----\n", len);
			addTrace(string, K_RTMS);	string = dat;
#if 0
			tmpS.Format("[%-12s] ", parse(string, STAB));
			while(!string.IsEmpty())
			{
				tmpS += parse(string, STAB) + "|";
				tmpS += parse(string, STAB) + " ";
			}
			addTrace(tmpS + "\n", K_RTMS);
#else
			tmpS.Format("[KEY = %s]\n", parse(string, STAB));
			addTrace(tmpS, K_RTMS);
			int	cnt = 1;
			CString	datt;
			while(!string.IsEmpty())
			{
#if 0
				tmpS.Format("\t%s : ", parse(string, STAB));
				tmpS += parse(string, STAB) + "\n";
				addTrace(tmpS, K_RTMS);
#else
				tmpS.Format("\t%s : ", parse(string, STAB));
				tmpS += parse(string, STAB);
				if (cnt%m_rtsL == 0)
				{
					if (!datt.IsEmpty())
					{
						datt += tmpS;
						tmpS = datt;
					}
					datt.Format("%s\n", tmpS);
					addTrace(datt, K_RTMS);
					datt.Empty();
				}
				else
					datt += tmpS + "\t";

				cnt++;
#endif
			}
#endif			
		}
		break;
	case x_STRs:
		if (len <= 0)	break;
		if (m_bSYSTRACE)
		{
			tmpS.Format("* SYSTEM TRACE * [ %s ]\n", dat);
			addTrace(tmpS, K_SYSTRACE);
		}
		break;
	}
}

void CChildView::addTrace(CString dat, int kind)
{
	if (dat.IsEmpty())	return;
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
	OnRCVData(MAKEWPARAM(len, flag), (LPARAM) &rcvB[L_cdss]);	
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

	m_fSize[IDX_ALL] = app->GetProfileInt(TRACESETTING, ALLFONT, DEFAULTFONTN);
	m_fSize[IDX_SNR] = app->GetProfileInt(TRACESETTING, SNRFONT, DEFAULTFONTN);
	m_fSize[IDX_RTM] = app->GetProfileInt(TRACESETTING, RTMFONT, DEFAULTFONTN);
	m_fSize[IDX_SYS] = app->GetProfileInt(TRACESETTING, SYSFONT, DEFAULTFONTN);

	m_tColor[IDX_ALL] = app->GetProfileInt(TRACESETTING, ALLCOLOR, COLOR_BLACK);
	m_tColor[IDX_SNR] = app->GetProfileInt(TRACESETTING, SNRCOLOR, COLOR_BLACK);
	m_tColor[IDX_RTM] = app->GetProfileInt(TRACESETTING, RTMCOLOR, COLOR_RED);
	m_tColor[IDX_SYS] = app->GetProfileInt(TRACESETTING, SYSCOLOR, COLOR_BLUE);

	m_bBold[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allBold", 0);
	m_bBold[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrBold", 0);
	m_bBold[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmBold", 0);
	m_bBold[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysBold", 0);

	m_bItalic[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allItalic", 0);
	m_bItalic[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrItalic", 0);
	m_bItalic[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmItalic", 0);
	m_bItalic[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysItalic", 0);

	m_bUline[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allUline", 0);
	m_bUline[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrUline", 0);
	m_bUline[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmUline", 0);
	m_bUline[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysUline", 0);

	m_bStrike[IDX_ALL] = app->GetProfileInt(TRACESETTING, "allStrike", 0);
	m_bStrike[IDX_SNR] = app->GetProfileInt(TRACESETTING, "snrStrike", 0);
	m_bStrike[IDX_RTM] = app->GetProfileInt(TRACESETTING, "rtmStrike", 0);
	m_bStrike[IDX_SYS] = app->GetProfileInt(TRACESETTING, "sysStrike", 0);
	
	m_trace.SetBackgroundColor(FALSE, m_bColor);
	m_trace.SetFont(&m_font, FALSE);
	m_trace.LimitText(1024*1024*100);
}

void CChildView::Test() 
{
	LockWindowUpdate();
	CRTFBuilder c ;
	c << font("±Ã¼­Ã¼" );
	c << size(12) ;
	c << color( RGB(255,255,255)) << "¾È³çÇÏ¼¼¿ä" ;
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
	c << "About to PUSH with these new settings¾È³çÇÏ¼¼¿ä\n";
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


