// Dbar1.cpp : implementation file
// MDI 상태에서의 상단 기본 툴바
//메인툴바 로고때문에 72픽셀 띄움
#include "stdafx.h"
#include "Dbar1.h"
#include "xcom.h"
#include "xcomex.h"
#include "nbutton.h"
#include "hBand.h"
#include "svBand.h"
#include "SDIBand.h"
#include "trsearch.h"
#include "tmenu.h"
#include "smdefine.h"
#include "NPalette.h"

#include "../../h/axisvar.h"
#include "../../axis/axmsg.hxx"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RBANDW		108
#define	GAP		3
#define	LOGOWIDTH	134

#define	TOOL_MENUBASE	6000
#define	TOOL_MENUEND	6100
#define ORDERSCREEN1	6101
#define ORDERSCREEN2	6102

#define CTRL_CHANGE	0x58

#define TOOL_HIDE_WIDTH 0
#define TOPMARGIN		7
#define ID_SETACCOUNT                   32865

#define DF_BTNHOBWIDTH 98
#define DF_BTNAIWIDTH 98

const int buttonSize = 22;

#include "Winuser.h"
/////////////////////////////////////////////////////////////////////////////
// CDbar1

CDbar1::CDbar1(bool (*callback)(int, WPARAM, LPARAM))
{
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);

	m_axiscall	= callback;
	m_height	= HEIGHT_CTRLBAR1;

	m_hide		= NULL;
	m_xcom		= NULL;
	m_searchS	= NULL;

	for (int ii = 0; ii < MAX_FUNC; ii++)
	{
		m_func[ii] = NULL;
	}

	m_xcomex	= NULL;
	m_register	= NULL;
	m_band		= NULL;
	m_sband		= NULL;
	m_vband		= NULL;
	m_dept		= "";

	m_itemW = 0;

	CProfile profile(pkUserTool);
	m_type = profile.GetInt(szUserSN, TOOLTYPE, TTYPE2);
	m_type = TTYPE2;

	m_vsN = V_SCREEN1;
	m_bFunc = profile.GetInt(szUserSN, "tool", 1);
	m_vMode = profile.GetInt(szUserSN, "toolex", 0);

	m_arFunc.RemoveAll();

	CString value(profile.GetString(szToolFunc, "Func", "0, 1, 2, 3, 5, 6"));

	for (int ii = 0; !value.IsEmpty(); ii++)
	{
		CString func = Parser(value, ",");	
		func.TrimLeft();	func.TrimRight();
		if (func.IsEmpty())	continue;

		m_arFunc.Add(atoi(func));
	}
	m_nFunc	= m_arFunc.GetSize();

	m_FuncType = profile.GetInt(szToolFunc, "Type", 1);

	m_bmpBand = Axis::GetSkinBitmap("BAND");

	m_bmpHome = Axis::GetBitmap("TEXT_SCREEN");

	

	m_registerX = m_funcX = m_xcomX = 0;

	m_bRotate = FALSE;
	m_nRCnt = 0;
	m_nRTime = 0;

	m_btnBanner = NULL;
	m_btnTop10 = NULL;
}

CDbar1::~CDbar1()
{
	XMSG();
#ifndef DF_USESTL_DBAR1
	if (m_hide)		delete m_hide;
	if (m_xcom)		delete m_xcom;
	if (m_searchS)	delete m_searchS;
	if (m_tacnt)	delete m_tacnt;

	for (int ii = 0; ii < MAX_FUNC; ii++)
		delete m_func[ii];

	if (m_xcomex)	delete m_xcomex;
	if (m_register)	delete m_register;
	if (m_band)		delete m_band;
	if (m_sband)	delete m_sband;
	if (m_banner)   delete m_banner;
	//if (m_hHtmlInst) FreeLibrary(m_hHtmlInst);
	if (m_vband)	
	{
		m_vMode = m_vband->GetBandMode();

		CProfile profile(pkUserTool);
		profile.Write(szUserSN, "toolex", Format("%d", m_vMode));
		profile.Write(szUserSN, "tool", Format("%d", m_bFunc));

		delete m_vband;
	}

	m_font.DeleteObject();
#else

#endif
}


BEGIN_MESSAGE_MAP(CDbar1, CDialogBar)
	//{{AFX_MSG_MAP(CDbar1)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbar1 message handlers
#ifdef DF_MK_CAPTION
CString ExtractBracketContent(const CString& input)
{
	int start = input.Find(_T('['));
	int end = input.Find(_T(']'));

	if (start != -1 && end != -1 && end > start)
	{
		return input.Mid(start + 1, end - start - 1);
	}

	return _T("");
}
#endif
void CDbar1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	draw_BackGround(&dc);
}

LONG CDbar1::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_SEARCHS:
		{
			CTrSearch	dlg(m_menu, m_axiscall);
			dlg.setPos(GetPos(1));
			if (dlg.DoModal() == IDOK)
				(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)dlg.getTR());
		}
		break;
	case CTRL_REGISTER:
		(*m_axiscall)(AXI_FUNCKEY, wParam, lParam);
		break;
	case CTRL_HIDE:
		(*m_axiscall)(AXI_HIDETICKER, 0, 4);	break;
		break;
	case CTRL_FUNC:
		{
			m_bFunc = !m_bFunc;
			
			SetPosition();
		}
		break;
	case CTRL_FUNC+1:	case CTRL_FUNC+2:	case CTRL_FUNC+3:	case CTRL_FUNC+4:
	case CTRL_FUNC+5:	case CTRL_FUNC+6:	case CTRL_FUNC+7:	case CTRL_FUNC+8:
	case CTRL_FUNC+9:	case CTRL_FUNC+10:	case CTRL_FUNC+11:	case CTRL_FUNC+12:
		(*m_axiscall)(AXI_FUNCKEY, wParam, lParam);
		break;
	case CTRL_FUNC+14:
	//	(*m_axiscall)(AXI_FUNCKEY, wParam, lParam); 
		(*m_axiscall)(AXI_FUNCKEY, wParam, m_currEvent);
		break;
	case CTRL_FUNC+16:
		{
			CRect rc;
			
			m_btnTop10->GetWindowRect(&rc);
			
			RECT r;
			r.top = rc.top;
			r.left = rc.left;
			r.bottom = rc.bottom;
			r.right = rc.right;
// 			CString s;
// 			s.Format("TOP10 SM [%d] [%d] [%d] [%d]\n",rc.top,rc.left,rc.bottom,rc.right);
// 			OutputDebugString(s);
			(*m_axiscall)(AXI_FUNCKEY, wParam, (LPARAM)&r);
		}
		break;
#ifdef DF_MK_CAPTION
	case CTRL_FUNC + 18:
	{
		CString stmp, skey;
		CString	Path;
		Path.Format("%s\\%s\\AXNXT.INI", Axis::home, "tab");

		char readB[64]{};
		int readL;
		readL = GetPrivateProfileString("KRX_POPMENU", "CNT", "", readB, sizeof(readB), Path);

		if (readL <= 0)
			return 0;

		CMapStringToString mapMenu{};
		stmp.Format("%s", readB);
		int icnt = atoi(stmp);

		CMenu cMenu;
		cMenu.CreatePopupMenu();
		for (int ii = 0; ii < icnt; ii++)
		{
			stmp.Format("%d", ii);
			GetPrivateProfileString("KRX_POPMENU", stmp, "", readB, sizeof(readB), Path);

			stmp.Format("%s", readB);
			skey.Format("%d", ii);
			mapMenu.SetAt(skey, stmp);

			cMenu.AppendMenu(MFT_STRING, ii + 1, stmp);
		}

		CPoint	point;
		GetCursorPos(&point);

		CRect rec;
		m_btKRX->GetWindowRect(rec);
		rec.OffsetRect(0, -35);

		const	int	ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RETURNCMD, rec.left, rec.bottom + rec.Height() + 13, this, NULL);
		if (ret > 0)
		{
			skey.Format("%d", ret - 1);
			if (mapMenu.Lookup(skey, stmp))
			{
				if (stmp.Left(2) == "IB")
					m_slog = stmp;
				else
					m_slog = ExtractBracketContent(stmp);
				CWnd* pMain = AfxGetMainWnd();
				if (pMain)
					pMain->SendMessage(WM_USER, MAKEWPARAM(MMSG_POP_MAP, 3), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
			}
			return 0;
		}
	}
	break;
	case CTRL_FUNC + 19:
	{
		CString stmp, skey;
		CString	Path;
		Path.Format("%s\\%s\\AXNXT.INI", Axis::home, "tab");

		char readB[64]{};
		int readL;
		readL = GetPrivateProfileString("NXT_POPMENU", "CNT", "", readB, sizeof(readB), Path);

		if (readL <= 0)
			return 0;

		CMapStringToString mapMenu{};
		stmp.Format("%s", readB);
		int icnt = atoi(stmp);

		CMenu cMenu;
		cMenu.CreatePopupMenu();
		for (int ii = 0; ii < icnt; ii++)
		{
			stmp.Format("%d", ii);
			GetPrivateProfileString("NXT_POPMENU", stmp, "", readB, sizeof(readB), Path);

			stmp.Format("%s", readB);
			skey.Format("%d", ii);
			mapMenu.SetAt(skey, stmp);

			cMenu.AppendMenu(MFT_STRING, ii + 1, stmp);
		}

		CPoint	point;
		GetCursorPos(&point);

		CRect rec;
		m_btKRX->GetWindowRect(rec);
		rec.OffsetRect(0, -35);

		const	int	ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RETURNCMD, rec.left, rec.bottom + rec.Height() + 13, this, NULL);
		if (ret > 0)
		{
			skey.Format("%d", ret - 1);
			if (mapMenu.Lookup(skey, stmp))
			{
				m_slog = ExtractBracketContent(stmp);
				CWnd* pMain = AfxGetMainWnd();
				if (pMain)
					pMain->SendMessage(WM_USER, MAKEWPARAM(MMSG_POP_MAP, 3), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
			}
			return 0;
		}
	}
	break;
	case CTRL_FUNC + 20:
	{
		char redB[32]{};
		int redL;
		CString file, stmp;
		file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "userconf.ini");
		redL = GetPrivateProfileString("MKgubn", "Market", "KRX", redB, sizeof(redB), file);
		stmp.Format("%s", redB);
		stmp.TrimRight();

		CMenu cMenu;

		cMenu.CreatePopupMenu();
		const	char* const pDisplay[3] = { "KRX", "NXT", "통합" };
		for (int ii = 0; ii < 3; ii++)
		{
			if (stmp == pDisplay[ii])
				cMenu.AppendMenu(MFT_STRING | MF_CHECKED, ii, pDisplay[ii]);
			else
				cMenu.AppendMenu(MFT_STRING, ii + 1, pDisplay[ii]);
		}

		CRect rec;
		m_btMKSELECT->GetWindowRect(rec);
		rec.OffsetRect(0, -35);

		const	int	ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RETURNCMD, rec.left, rec.bottom + rec.Height() + 13, this, NULL);

		if (ret < 1)
			return 0;

		switch (ret)
		{
		case 3:
			m_btMKSELECT->set_Image(CAxButtonName(Format("TOOL_%s", "TOT_MK")));
			WritePrivateProfileString("MKgubn", "Market", "통합", file);
			break;
		case 1:
			m_btMKSELECT->set_Image(CAxButtonName(Format("TOOL_%s", "KRX_MK")));
			WritePrivateProfileString("MKgubn", "Market", "KRX", file);
			break;
		case 2:
			m_btMKSELECT->set_Image(CAxButtonName(Format("TOOL_%s", "NXT_MK")));
			WritePrivateProfileString("MKgubn", "Market", "NXT", file);
			break;
		}

		m_btMKSELECT->Invalidate();

		CWnd* pMain = AfxGetMainWnd();
		if (pMain)
			pMain->SendMessage(WM_USER, MAKEWPARAM(MMSG_MKMSG_FROM_SM, ret), ret); //lparam 은 딱히 필요없어서..

	}
	break;
#endif
	case CTRL_CHANGE:
		SetPosition();
		Invalidate();
		break;
	case CTRL_TACNT:
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_SETACCOUNT, 0);
		break;
	default:
		break;
	}
	return 0;
}

CSize CDbar1::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	/**
	if (m_type == TTYPE1)		m_height = HEIGHT_CTRLBAR1;
	else if (m_type == TTYPE2)	m_height = HEIGHT_CTRLBAR2;
	**/
	m_height = HEIGHT_CTRLBAR1;

	CSize	size;
	CRect	fRc;
	size.cx = 32767;
	size.cy = m_height;
	size.cy += (rect.Height());

	if (IsFloating())
	{
		GetDockingFrame()->GetParent()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}
	else
	{
		GetDockingFrame()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}

	return size;
}

void CDbar1::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

void CDbar1::init()
{
}

const char* strImg[] = { "HIDE", "설정", "인쇄", /**"홈페이지",**/ "잠금", "비번", "저장화면", "계산기", 
	/**"오디오",**/ 	"해외지수", "저장", "엑셀", };

void CDbar1::setDept(CString dept)
{
	m_dept = dept;
	SetPosition();
}

void CDbar1::make_Ctrl()
{
	CString		imgC, imgO, imgD, text, theme = CNPalette::GetTheme();
	const DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	/**
	m_hide = new CNButton(CTRL_HIDE);
	if (!m_hide->Create(NULL, "숨기기", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_hide;
		m_hide = NULL;
	}
	else
	{
		m_hide->set_ToolTipText("숨기기");
		m_hide->set_Image(CAxButtonName(Format("mb_%s_barhide", theme)));
	}
	**/

#ifdef DF_USESTL_DBAR1
	m_xcom = std::make_unique<CXcom>(CTRL_SCREEN);
	if (!m_xcom->Create(NULL, "screenNo", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_xcom = NULL;
	else	m_xcom->init(m_axiscall);

	m_searchS =std::make_unique<CNButton>(CTRL_SEARCHS);
	m_searchS->set_Color(RGB(247, 251, 196));
	if (!m_searchS->Create(NULL, "search screen", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_searchS = NULL;
	else
	{
		text.LoadString(ST_TEXT_SCREENSEARCH);
		m_searchS->set_ToolTipText(text);

		m_searchS->set_Image(CAxButtonName("TOOL_검색", true));
	}

	m_tacnt = std::make_unique<CNButton>(CTRL_TACNT);
	m_tacnt->set_Color(RGB(247, 251, 196));
	if (!m_tacnt->Create(NULL, "Total Account", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_tacnt = NULL;
	else
	{
		text.LoadString(ST_TEXT_SCREENSEARCH);
		m_tacnt->set_ToolTipText("계좌연동");

		m_tacnt->set_Image(CAxButtonName("TOOL_계좌", false));
	}

	CString tooltip[] = { "도구 숨기기", "환경설정", "인쇄", "화면잠금", "계좌 비밀번호 저장",
		"사용자 WorkSpace", "증권계산기", "해외지수 실시간티커", "이미지 저장", "엑셀시세연동", };

	for (int ii = 0; ii < MAX_FUNC; ii++)
	{
		//m_func[ii] = new CNButton(CTRL_FUNC + ii);
		m_func[ii] = std::make_unique<CNButton>(CTRL_FUNC + ii);
		m_func[ii]->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
		if (!m_func[ii]->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
			m_func[ii] = NULL;
		else
		{
			m_func[ii]->set_ToolTipText(tooltip[ii]);
			m_func[ii]->set_Image(CAxButtonName(Format("TOOL_%s", strImg[ii])));
		}
	}

	btnHOB = std::make_shared<CNButton>(CTRL_FUNC + 11);
	btnHOB->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));

	if (!btnHOB->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
		btnHOB = NULL;
	else
	{
		btnHOB->set_ToolTipText("HUB");
		btnHOB->set_Image(CAxButtonName(Format("TOOL_%s", "HOB")));

		m_HOB = (CWnd*)btnHOB.get();
	}

#ifndef DF_MK_CAPTION
	btnAI = std::make_shared<CNButton>(CTRL_FUNC + 12);
	btnAI->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));

	if (!btnAI->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
		btnAI = NULL;
	else
	{
		btnAI->set_ToolTipText("AI");
		btnAI->set_Image(CAxButtonName(Format("TOOL_%s", "AI")));

		m_AI = (CWnd*)btnAI.get();
		btnAI->ShowWindow(SW_HIDE);
	}
#else
	//NXT
	m_btNXT = new CNButton(CTRL_FUNC + 19);
	m_btNXT->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));

	if (!m_btNXT->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_btNXT = NULL;
	else
		m_btNXT->set_Image(CAxButtonName(Format("TOOL_%s", "NXT_ON")));

	//KRX
	m_btKRX = new CNButton(CTRL_FUNC + 18);
	m_btKRX->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));

	if (!m_btKRX->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_btKRX = NULL;
	else
		m_btKRX->set_Image(CAxButtonName(Format("TOOL_%s", "KRX_ON")));

	//MK Select
	m_btMKSELECT = new CNButton(CTRL_FUNC + 20);
	m_btMKSELECT->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
	

	char redB[32]{};
	int redL;
	CString file;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "userconf.ini");
	redL = GetPrivateProfileString("MKgubn", "Market", "KRX", redB, sizeof(redB), file);

	if (!m_btMKSELECT->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_btMKSELECT = NULL;
	else
	{
		text.Format("%s", redB); text.TrimRight();

		if(text == "KRX")
			m_btMKSELECT->set_Image(CAxButtonName(Format("TOOL_%s", "KRX_MK")));
		else if(text == "NXT")
			m_btMKSELECT->set_Image(CAxButtonName(Format("TOOL_%s", "NXT_MK")));
		else if (text == "통합")
			m_btMKSELECT->set_Image(CAxButtonName(Format("TOOL_%s", "TOT_MK")));
	}
	m_btMKSELECT->ShowWindow(SW_HIDE);
#endif
	m_xcomex = std::make_unique<CXcomex>(this);
	if (!m_xcomex->Create(NULL, "toolgroup", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_xcomex = NULL;
	else	m_xcomex->init();

#else
	m_xcom = new CXcom(CTRL_SCREEN);
	if (!m_xcom->Create(NULL, "screenNo", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_xcom;
		m_xcom = NULL;
	}
	else	m_xcom->init(m_axiscall);

	m_searchS = new CNButton(CTRL_SEARCHS);
	m_searchS->set_Color(RGB(247, 251, 196));
	if (!m_searchS->Create(NULL, "search screen", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_searchS;
		m_searchS = NULL;
	}
	else
	{
		text.LoadString(ST_TEXT_SCREENSEARCH);
		m_searchS->set_ToolTipText(text);

		m_searchS->set_Image(CAxButtonName("TOOL_검색", true));
	}

	m_tacnt = new CNButton(CTRL_TACNT);
	m_tacnt->set_Color(RGB(247, 251, 196));
	if (!m_tacnt->Create(NULL, "Total Account", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_tacnt;
		m_tacnt = NULL;
	}
	else
	{
		text.LoadString(ST_TEXT_SCREENSEARCH);
		m_tacnt->set_ToolTipText("계좌연동");

		m_tacnt->set_Image(CAxButtonName("TOOL_계좌", false));
	}

	CString tooltip[] = { "도구 숨기기", "환경설정", "인쇄", "화면잠금", "계좌 비밀번호 저장",
		"사용자 WorkSpace", "증권계산기", "해외지수 실시간티커", "이미지 저장", "엑셀시세연동", };

	for (int ii = 0; ii < MAX_FUNC; ii++)
	{
		m_func[ii] = new CNButton(CTRL_FUNC + ii);
		m_func[ii]->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
		if (!m_func[ii]->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
		{
			delete m_func[ii];
			m_func[ii] = NULL;
		}
		else
		{
			m_func[ii]->set_ToolTipText(tooltip[ii]);
			m_func[ii]->set_Image(CAxButtonName(Format("TOOL_%s", strImg[ii])));
		}
		//**m_func[ii]->set_MaskingColor(Axis::maskColor);
		//**if (ii)	m_func[ii]->set_Embossing(theme);
	}

	CNButton* btnHOB = new CNButton(CTRL_FUNC + 11);
	btnHOB->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));

	if (!btnHOB->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete btnHOB;
		btnHOB = NULL;
	}
	else
	{
		btnHOB->set_ToolTipText("HUB");
		btnHOB->set_Image(CAxButtonName(Format("TOOL_%s", "HOB")));

		m_HOB = (CWnd*)btnHOB;

		m_btnHOB = btnHOB;
	}

	m_xcomex = new CXcomex(this);
	if (!m_xcomex->Create(NULL, "toolgroup", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_xcomex;
		m_xcomex = NULL;
	}
	else	m_xcomex->init();

#endif
	
	if(Axis::isCustomer)
		btnHOB->ShowWindow(SW_HIDE); //HOB
	else
		btnHOB->ShowWindow(SW_SHOW);

	m_register = new CNButton(CTRL_REGISTER);
	m_register->set_Color(RGB(247, 251, 196));
	if (!m_register->Create(NULL, "function key", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_register;
		m_register = NULL;
	}
	else
	{
		text.LoadString(ST_TEXT_REGTOOL_CUR);
		m_register->set_ToolTipText(text);
		m_register->set_Image(CAxButtonName("TOOL_화면추가"));
	}

	m_band = new CHBand(m_axiscall);
	if (!m_band->Create(NULL, "tool menu", WS_CHILD, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_band;
		m_band = NULL;
	}

	m_sband = new CSDIBand(m_axiscall, false);
	if (!m_sband->Create(NULL, "tool menu", WS_CHILD, CRect(0, 0, 0, 0), this, -2))
	{
		delete m_sband;
		m_sband = NULL;
	}

	m_banner = NULL;
	m_staffbanner = NULL;
// 	CString dllpath = Axis::home + "\\dev\\CX_THTMLEX.DLL";
// 	m_hHtmlInst = LoadLibrary((LPCSTR)dllpath);
// 	if (m_hHtmlInst)
// 	{
// 		typedef CWnd* (WINAPI *fnCreate)(CWnd *, void*);
// 		fnCreate CreateHtml = (fnCreate)GetProcAddress(m_hHtmlInst, "axCreate");
// 		if (CreateHtml)
// 		{
// 			_param param;
// 			param.fonts = "굴림체";
// 			param.name = "IBKSBanner";
// 			param.options = "/wurlhttp:\\\\www.ibks.com\\hts\\top_banner.jsp";
// 			param.point = 9;
// 			m_banner = CreateHtml(this, &param);
// 		}
// 	}

// 	CNButton *btnBanner = new CNButton(CTRL_FUNC + 14);
// 	btnBanner->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
// 	
// 	if (!btnBanner->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
// 	{
// 		delete btnBanner;
// 		btnBanner = NULL;
// 	}
// 	else
// 	{
// 		btnBanner->set_Image(CAxButtonName("top_banner"));
// 		
// 		m_banner = (CWnd *)btnBanner;
// 	}
	//배너 광고 숨기기
	CString	Path;
	Path.Format("%s\\%s\\BANNER.INI", Axis::home, "tab");
	
	char readB[1024];
	int readL;
	
	readL = GetPrivateProfileString("BANNER","use","YES",readB,sizeof(readB),Path);
	
	CString useBanner(readB,readL);

	readL = GetPrivateProfileString("BANNER_STAFF","use","NO",readB,sizeof(readB),Path);

	CString useSTAFF(readB,readL);

	//if(Axis::isCustomer)
	{
		m_nRCnt = GetPrivateProfileInt("BANNER","RCNT",0,Path);
		
		m_nRTime = GetPrivateProfileInt("BANNER","RTIME",0,Path);

		readL = GetPrivateProfileString("BANNER","ROTATE","NO",readB,sizeof(readB),Path);

		CString rotateBanner(readB,readL);

		int nUser;

		nUser = GetPrivateProfileInt("BANNER","USER",0,Path);

		if(rotateBanner == "NO")
		{
			m_bRotate = FALSE;

			CNButton *btnBanner = new CNButton(CTRL_FUNC + 14);
			btnBanner->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
			
			if (!btnBanner->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
			{
				delete btnBanner;
				btnBanner = NULL;
			}
			else
			{
				btnBanner->set_Image(CAxButtonName("top_banner"));
				
				m_banner = (CWnd *)btnBanner;
			}
		}
		else
		{
			m_bRotate = TRUE;

			CNButton *btnBanner = new CNButton(CTRL_FUNC + 14);
			btnBanner->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
			
			if (!btnBanner->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
			{
				delete btnBanner;
				btnBanner = NULL;
			}
			else
			{
				btnBanner->set_Image(CAxButtonName("top_banner_01"));
				
				m_banner = (CWnd *)btnBanner;

				m_btnBanner = btnBanner;
			}

			m_currEvent = 1;

			SetTimer(TM_TOPBANNER_ROTATE, m_nRTime, NULL);
		}

		m_bUseBanner = TRUE;

		if(useBanner == "NO")
		{
			m_banner->ShowWindow(SW_HIDE);
			m_bUseBanner = FALSE;
		}
		else
		{
			if(nUser == 1 && !Axis::isCustomer)
			{
				m_banner->ShowWindow(SW_HIDE);
				m_bUseBanner = FALSE;
			}
		}
		//TOP10 롤링 버튼 추가

		m_nTOPCount = GetPrivateProfileInt("TOP10","RCNT",10,Path);
		
		m_nTOPTime = GetPrivateProfileInt("TOP10","RTIME",3000,Path);

		CNButton *btntop10 = new CNButton(CTRL_FUNC + 16);
		btntop10->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
		
		if (!btntop10->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
		{
			delete btntop10;
			btntop10 = NULL;
		}
		else
		{
			btntop10->set_Image(CAxButtonName("TOP10"));
			
			m_top10 = (CWnd *)btntop10;
			
			m_btnTop10 = btntop10;
		}
		
		m_currTOP10 = 0;

		CString strFile;
		strFile.Format("%s\\tab\\TOP10.ini", Axis::home); 
		const int nUse = GetPrivateProfileInt("TOP10","AUTOPOPUP",0,strFile);
		if(nUse == 1)
			SetTimer(TM_TOP10, 5000, NULL);
	}
	
	if(!Axis::isCustomer)
	{
		m_nStaffRCnt = GetPrivateProfileInt("BANNER_STAFF","RCNT",0,Path);
		
		m_nStaffRTime = GetPrivateProfileInt("BANNER_STAFF","RTIME",0,Path);
		
// 		readL = GetPrivateProfileString("BANNER_STAFF","ROTATE","NO",readB,sizeof(readB),Path);
// 		
// 		CString rotateBanner(readB,readL);
// 		
// 		if(rotateBanner == "NO")
		{
			m_bStaffRotate = FALSE;
			
			CNButton *btnBanner = new CNButton(CTRL_FUNC + 15);
			btnBanner->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
			
			if (!btnBanner->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
			{
				delete btnBanner;
				btnBanner = NULL;
			}
			else
			{
				btnBanner->set_Image(CAxButtonName("top_banner_staff"));
				
				m_staffbanner = (CWnd *)btnBanner;
			}
		}
// 		else
// 		{
// 			m_bStaffRotate = TRUE;
// 			
// 			CNButton *btnBanner = new CNButton(CTRL_FUNC + 14);
// 			btnBanner->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
// 			
// 			if (!btnBanner->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
// 			{
// 				delete btnBanner;
// 				btnBanner = NULL;
// 			}
// 			else
// 			{
// 				btnBanner->set_Image(CAxButtonName("top_banner_staff_01"));
// 				
// 				m_staffbanner = (CWnd *)btnBanner;
// 				
// 				m_btnStaffBanner = btnBanner;
// 			}
// 			
// 			m_currStaffEvent = 1;
// 			
// 			SetTimer(TM_STAFF_TOPBANNER_ROTATE, m_nRTime, NULL);
// 		}

		if(useSTAFF == "NO")
		{
			m_staffbanner->ShowWindow(SW_HIDE);
		}
	}

	m_vband = new CSVband(m_axiscall);
	if (!m_vband->Create(NULL, "SVBAND", WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|WS_CHILD, CRect(0, 0, 0, 0), this, -3))
	{
		delete m_vband;
		m_vband = NULL;
	}
	else
		m_vband->make_Ctrl(m_vMode);

	SetBandType();
	load_BandItem();
}

void CDbar1::RotateBanner()
{
	
	if (m_nRCnt < 1)
		return;

	m_currEvent++;

	if (m_currEvent >= m_nRCnt)
		m_currEvent = 0;

	SetEventBitmap(m_currEvent + 1);

	CString s;
// 	s.Format("Rotate Top Banner [%d]\n",m_currEvent);
// 	OutputDebugString(s);
/*
	for (int i = IDC_EVENT1; i < IDC_EVENT1 + 3; i++)
		GetDlgItem(i)->SendMessage(BM_SETCHECK);
	
	CWnd* btn = GetDlgItem(IDC_EVENT1 + m_currEvent);
	if (btn)
		btn->SendMessage(BM_SETCHECK, TRUE);
*/
}

void CDbar1::SetEventBitmap(int index)
{
	const DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	m_currEvent = index - 1;

	if (m_bitmapEvent)
	{
		DeleteObject(m_bitmapEvent);
		m_bitmapEvent = NULL;
	}

	CString fileName;
	fileName.Format("top_banner_%02d", index);

// 	CString s;
// 	s.Format("TOP BANNER FILE [%s]\n",fileName);
// 	OutputDebugString(s);

	if (m_btnBanner)
	{
		m_btnBanner->set_Image(CAxButtonName(fileName));

		m_btnBanner->Invalidate(TRUE);
	}
	else
	{
		m_banner->ShowWindow(SW_HIDE);
	}

	{
		KillTimer(TM_TOPBANNER_ROTATE);
		SetTimer(TM_TOPBANNER_ROTATE, m_nRTime, NULL);
	}
}

void CDbar1::Change_Skin(CString skinName)
{
	m_band->change_Skin(skinName);
	m_sband->change_Skin(skinName);
	m_vband->change_Skin(skinName);

	CString		imgC, imgO, imgD, text, theme = CNPalette::GetTheme();

	if (m_hide)
	{
		m_hide->set_Image(CAxButtonName(Format((m_type == TTYPE1 ? "%s_barhides": "%s_barhide"), //"mb_%s_barhides": "mb_%s_barhide"), 
			theme)));

	}

	if (m_searchS)
	{
		m_searchS->set_Image(CAxButtonName("TOOL_검색",true));
	}

	for (int ii = 0; ii < MAX_FUNC; ii++)
	{
		if (m_func[ii])
		{
			m_func[ii]->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
			m_func[ii]->set_Image(CAxButtonName(Format("TOOL_%s", strImg[ii])));

			/**
			if (ii)	
				m_func[ii]->set_Embossing(theme);
			**/
		}		
	}

	if (m_register)
	{
		m_register->set_Image(CAxButtonName("TOOL_화면추가"));
	}

	m_xcom->Invalidate();
	m_xcomex->Invalidate();
	m_searchS->Invalidate();

	Invalidate();
}

void CDbar1::draw_BackGround(CDC* pDC)
{
	CRect	cRc;
	GetClientRect(cRc);

	BITMAP	bm{};
	CDC	memDC;
	CBitmap* oldBmp = NULL;

	if (m_bmpBand->m_hObject)
	{
		m_bmpBand->GetBitmap(&bm);

		if (memDC.CreateCompatibleDC(pDC))
		{
			oldBmp = memDC.SelectObject(m_bmpBand);
			if (oldBmp != (CBitmap *) 0)
			{
				pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), bm.bmHeight, 
						&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				memDC.SelectObject(oldBmp);
			}
			memDC.DeleteDC();
		}	
	}
	else
	{
		pDC->FillSolidRect(cRc, CNPalette::GetIndexColor(CLR_TOOL_BK));

		CPen	pen(PS_SOLID, 1, RGB(255, 255, 255));

		CPen	*pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(0, 0);
		pDC->LineTo(cRc.right, 0);
		
		pDC->SelectObject(pOldPen);
	}
}

void CDbar1::draw_Button(bool push)
{

}

CRect CDbar1::get_GripperRect()
{
	CRect pRect;
	GetClientRect(pRect);
	pRect.OffsetRect(-pRect.left,-pRect.top);

	if (m_type == TTYPE1)
	{
		pRect.top = pRect.top + 1;
		pRect.right = pRect.left + 11;
	}
	else if (m_type == TTYPE2)
	{
		pRect.top = pRect.top + 1;
		pRect.right = pRect.left + 11;
	}

	return pRect;
}

void CDbar1::set_Background(COLORREF color)
{
	if (m_vband)
		m_vband->set_Background(color);
}

void CDbar1::set_Barheight(int height)
{
	m_height = height;
	(*m_axiscall)(AXI_RECALCLAYOUT, 0, 0);
}

void CDbar1::set_Maps(CString mapname)
{
	CString slog;
	slog.Format("[axissm][CDbar1][set_Maps]  m_menu=[%p] mapname =[%s]\n",  m_menu, mapname);
	//OutputDebugString(slog);

	CString	mapN = m_menu->GetMapping(mapname);
	m_xcom->set_Maps(mapN);
}

void CDbar1::load_BandItem()
{
	if (!m_band || !m_sband)	return;
	//구차트를 제거하기 위한 코드
	const char* const oldchart[] = {"IB700000","IB710000","IB720000","IB701000","IB702000","IB702100","IB740100","IB703000","IB704000","IB730000","IB731000"};

	m_band->removeALL();
	m_sband->removeALL();

	CProfile profile(pkUserTool);
	CString tmpS, barName, cpFile(Format("%s\\%s\\usertool.ini", Axis::home, MTBLDIR));

	CopyFile(cpFile, profile.GetFileName(), TRUE);

	m_type = profile.GetInt(szUserSN, TOOLTYPE, TTYPE2);
	tmpS = profile.GetString(szUserSN, USEREN, "0, 0");

	int	idx = tmpS.Find(',');
	if (idx == -1)	idx = 0;
	else
	{
		tmpS = tmpS.Left(idx);
		idx = atoi(tmpS);
	}

	tmpS.Format("%02d", idx);

	barName = profile.GetString(szUserSN, tmpS, "");
	CString buffer = profile.GetSectionLF(tmpS);
	const int len = buffer.GetLength();

	if (len <= 0)
	{
		m_band->ReCalc();
		m_band->SetToolTipText("USER SPACE");
		m_band->Invalidate();

		m_sband->ReCalc();
		m_sband->SetToolTipText("USER SPACE");
		m_sband->Invalidate();

		buffer = profile.GetString(szUserSN, tmpS);
		
		m_xcomex->set_gName(buffer);

		return;
	}

	CString item, string(buffer);

	if (!Axis::isCustomer && barName=="주식매매")
	{
		m_band->ItemADD("IB871000=관리체결;관리체결", false);
		m_band->m_arItem[m_band->m_arItem.GetSize()-1]->draw = 1;
	}

#if 0
	if ( (string.Find("IB2X2T00")==-1) && (barName=="주식매매") )
	{
		m_band->ItemADD("IB2X2T00=2X2종합;2X2종합", false);
		m_band->m_arItem[m_band->m_arItem.GetSize()-1]->draw = 1;
	}
#endif

	bool flagNewChart = false;
	bool bNeedSave = false;

	char		buf[512];
	
	CString file;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "axisensetup.ini");
	
	const DWORD ret = GetPrivateProfileString("OLDCHARTCHANGE","CHANGE","NO",buf,sizeof(buf),file);

	CString strChange(buf);

	for (; !string.IsEmpty(); )
	{
		idx = string.Find('\n');
		if (idx == -1)	break;

		item  = string.Left(idx++);
		string = string.Mid(idx);

		idx = item.Find('=');
		if (idx == -1)	continue;
		
		/////////////////////////////////////////////////////
		if(strChange == "NO")
		{
			if(item.Left(idx).GetAt(0) == '#')
			{
				if(GetOldChart(item.Left(idx)))
				{
					::CopyFile(cpFile,cpFile+".bak",FALSE);

					flagNewChart = true;
					bNeedSave = true;

					m_band->ItemADD("#999=종합차트;종합차트", true);
					m_sband->ItemADD("#999=종합차트;종합차트", true);
					
					continue;
				}
			}
			else
			{
				bool bChange = false;

				for(int i=0;i<ArraySize(oldchart);i++)
				{
	// 				CString s;
	// 				s.Format("TOOLBAR ORG [%s] NEW [%s]\n",item.Left(idx),CString(oldchart[i]));
	// 				OutputDebugString(s);

					if(item.Left(idx) == CString(oldchart[i]))
					{
						::CopyFile(cpFile,cpFile+".bak",FALSE);

						bChange = true;
						bNeedSave = true;

						item.Replace(item.Left(idx),GetNewChart(CString(oldchart[i])));

						item.Replace("구차트","차트");

						m_band->ItemADD(item, false);
						m_sband->ItemADD(item, false);

						break;
					}
				}
				
				if(bChange)
				{
					continue;
				}
			}
		}
		//////////////////////////////////////////////////////////

		m_band->ItemADD(item, IsList(item.Left(idx)));
		m_sband->ItemADD(item, IsList(item.Left(idx)));
	}
    ///////////////////////////////////////////////////////////
	if(flagNewChart)
	{
		::CopyFile(profile.GetFileName(),Format("%s.bak",profile.GetFileName()),FALSE);
		CString data;
		data.Format("IB711100=종합차트;종합차트\n"\
			"IB711200=주식종합차트;주식차트\n"\
			"IB711300=업종종합차트;업종차트\n"\
			"IB711400=선물옵션차트;선옵차트\n"\
			"IB713800=주선종합차트;주선차트\n"\
			"IB711500=상품선물종합차트;상선차트\n"\
			"IB711600=해외증시차트;해외차트\n");

		data += '\0';
		
		profile.WriteSection("#999",data);
	}

	if(bNeedSave)
	{
		::CopyFile(profile.GetFileName(),Format("%s.bak",profile.GetFileName()),FALSE);

		m_band->SaveItems();

		WritePrivateProfileString("OLDCHARTCHANGE","CHANGE","YES",file);
	}
	///////////////////////////////////////////////////////////////

	m_band->ReCalc();
	m_band->SetToolTipText("USER SPACE");
	m_band->Invalidate();

	m_sband->ReCalc();
	m_sband->SetToolTipText("USER SPACE");
	m_sband->Invalidate();

	m_xcomex->set_gName(profile.GetString(szUserSN, tmpS));
}

BOOL CDbar1::GetOldChart(CString section)
{
// 	CString s;
// 	s.Format("TOOL SECTION [%s]\n",section);
// 	OutputDebugString(s);
	const char* const oldchart[] = {"IB700000","IB710000","IB720000","IB701000","IB702000","IB702100","IB740100","IB703000","IB704000","IB730000","IB731000","IB711100","IB711300","IB712500"};
	
	CProfile profile(pkUserTool);
	CString buffer(profile.GetSectionLF(section));
	
	const int len = buffer.GetLength();
	
	if (len <= 0)
		return FALSE;
	
	BOOL bReturn{};

	CString subitem;
	for (; !buffer.IsEmpty(); )
	{
		int idx = buffer.Find('\n');
		if (idx == -1)	break;
		
		subitem  = buffer.Left(idx++);
		buffer = buffer.Mid(idx);
		
		idx = subitem.Find('=');
		if (idx == -1)	continue;

		bReturn = FALSE;
		
		for(int i=0;i<ArraySize(oldchart);i++)
		{
			if(subitem.Left(idx) == CString(oldchart[i]))
			{
				bReturn = TRUE;
			}
		}
	}
	
	return bReturn;
}

CString CDbar1::GetNewChart(CString old)
{
	CString strReturn;

	if(old == "IB700000")
	{
		strReturn = "IB711100";
	}
	else if(old == "IB710000")
	{
		strReturn = "IB711300";
	}
	else if(old == "IB720000")
	{
		strReturn = "IB711400";
	}
	else if(old == "IB701000")
	{
		strReturn = "IB711100";
	}
	else if(old == "IB702000")
	{
		strReturn = "IB711100";
	}
	else if(old == "IB702100")
	{
		strReturn = "IB711100";
	}
	else if(old == "IB740100")
	{
		strReturn = "IB712300";
	}
	else if(old == "IB703000")
	{
		strReturn = "IB711100";
	}
	else if(old == "IB704000")
	{
		strReturn = "IB712000";
	}
	else if(old == "IB730000")
	{
		strReturn = "IB711600";
	}
	else if(old == "IB731000")
	{
		strReturn = "IB711100";
	}

	return strReturn;
}

void CDbar1::OnSize(UINT nType, int cx, int cy) 
{
	SetPosition();
	CDialogBar::OnSize(nType, cx, cy);
	if (GetStyle() & WS_BORDER)
		ModifyStyle(WS_BORDER, 0);
}

void CDbar1::set_Code(CString code)
{
	if (m_xcom)	m_xcom->set_Code(code);
}

void CDbar1::set_List(const CStringArray& array)
{
	if (m_xcom)	m_xcom->set_List(array);
}

void CDbar1::add_History(CString dat)
{
	if (m_xcom)	m_xcom->add_History(dat);
}

BOOL CDbar1::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	comm = LOWORD(wParam);
	if (comm >= COMM_BASE && comm < COMM_END)
	{
		comm -= COMM_BASE;
		switch (comm)
		{
		case 0:	(*m_axiscall)(AXI_EDITUSERTOOL, 0, 0);	break;
		default:change_Group(comm-1);			break;
		}

		return TRUE;
	}
	/*else if (comm >= 0 && comm < 100 && m_ar.GetSize() > comm)
	{
		CString	str = m_ar.GetAt(comm);
		(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *) str.Left(8).operator LPCTSTR());
		return TRUE;
	}*/
	else if (comm >= TOOL_MENUBASE && comm < TOOL_MENUEND)
	{
		comm -= TOOL_MENUBASE;
		(*m_axiscall)(AXI_FUNCKEY, MAKEWPARAM(CTRL_REGISTERALL, 0), (int) comm);
	}
	
	return CDialogBar::OnCommand(wParam, lParam);
}

void CDbar1::change_Group(int index)
{
	CProfile profile(pkUserTool);
	CString tmpS(profile.GetString(szUserSN, USEREN, "0, 0"));
	const int pos  = tmpS.Find(',');
	CString	msg, title;
	if (pos == -1)
	{
		title.LoadString(STSM_TEXT_OK);
		msg.LoadString(ST_MSG_ERRORINFOFILE);
		::MessageBox(m_hWnd, msg, title, MB_ICONSTOP);
		return;
	}

	int tot = atoi(tmpS.Mid(pos+1));
	tmpS.Format("%d, %d", index, tot);
	profile.Write(szUserSN, USEREN, tmpS);

	load_BandItem();
}

CString CDbar1::screenInfo()
{
	CString		info;
	CPoint		point;
	
	CTrSearch	dlg(m_menu);
	GetCursorPos(&point);
	dlg.setPos(point);

	if (dlg.DoModal() == IDOK)
	{
		info = dlg.getTR();
	}
	return info;
}

CPoint CDbar1::GetPos(int index)
{
	CPoint	pos(0, 0);
	CRect	wRc;
	switch (index)
	{
	case 1:	// tr search pos
		m_searchS->GetWindowRect(wRc);
		pos = CPoint(wRc.left, wRc.bottom + 5);
		break;
	case CTRL_FUNC + 6:
		m_func[6]->GetWindowRect(wRc);
		pos = CPoint(wRc.left, wRc.bottom + 1);
		break;
	default:
		break;
	}
	return pos;
}

void CDbar1::OnLButtonDown(UINT nFlags, CPoint point) 
{

	CDialogBar::OnLButtonDown(nFlags, point);
}

void CDbar1::Refresh()
{
	CProfile profile(pkUserTool);
	m_type = profile.GetInt(szUserSN, TOOLTYPE, TTYPE2);

	SetBandType();
	load_BandItem();
}

void CDbar1::RefreshFunc()
{
	CProfile profile(pkUserTool);

	m_arFunc.RemoveAll();

	CString value(profile.GetString(szToolFunc, "Func", "0, 1, 2, 3, 5, 6"));

	for (int ii = 0; !value.IsEmpty(); ii++)
	{
		CString func = Parser(value, ",");	
		Trim(func);
		if (func.IsEmpty())	continue;

		m_arFunc.Add(atoi(func));
	}
	m_nFunc	= m_arFunc.GetSize();
	m_FuncType = profile.GetInt(szToolFunc, "Type", 1);

	SetPosition();
}

bool CDbar1::IsList(CString key)
{
	if (key.Find('#') != -1)
		return true;
	return false;
}

//mapN=툴팁;단축명
void CDbar1::AddItem(CString dat)
{
	if (!m_band || !m_sband)	return;
	m_band->ItemADD(dat);
	m_sband->ItemADD(dat);
}

void CDbar1::ShowMenu()
{
	CRect	wRc;

	m_register->GetWindowRect(wRc);
	const CPoint	point(wRc.left, wRc.bottom);

	CMenu		menu;
	//ClientToScreen(&xpoint);
	menu.CreatePopupMenu();

	CString	text;
	text.LoadString(ST_TEXT_REGTOOL_ALLMAP);
	menu.AppendMenu(MF_STRING, TOOL_MENUBASE, text);
	text.LoadString(ST_TEXT_SAVE_WROKSPACE);
	menu.AppendMenu(MF_STRING, TOOL_MENUBASE+1, text);

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
	menu.DestroyMenu();
}

void CDbar1::set_Owner()
{
	m_xcom->set_Owner();
}

#define FUNC_GAP	9
#define FUNC_BUTTON_GAP		3
#define BANNER_GAP	1
#define CHOKNTALK_WIDTH	30

void CDbar1::SetPosition()
{
	if (!m_band || !m_band->GetSafeHwnd())
		return;

	if (!m_sband || !m_band->GetSafeHwnd())
		return;

	CDC*	pDC = GetDC();

	CRect	cRc, xRc, wRc, tRc, mRc;
	const CSize	size = pDC->GetTextExtent(_T("$$$$$$"));
	CFont*	font = pDC->SelectObject(&m_font);
	const int	mode = pDC->SetBkMode(TRANSPARENT);
	
	GetClientRect(cRc);
	

	tRc = get_GripperRect();
	tRc.OffsetRect(72,0);
	if (m_hide)
		m_hide->MoveWindow(tRc);

	if (m_type == TTYPE1)
		m_type = TTYPE2;

    if (m_type == TTYPE2)
	{
		
	const int cy = TOPMARGIN;

		tRc.SetRect(tRc.right + 3, cRc.top, tRc.right + 3, cRc.bottom);
		tRc.DeflateRect(0, TOPMARGIN);
		
		if (m_xcom)
		{
			xRc.SetRect(tRc.right, tRc.top, tRc.right+GAP+size.cx+COMBTN_WIDTH, tRc.bottom);
			int	gap = 0;
			if (xRc.Height() != XCOM_HEIGHT)
			{
				gap = (xRc.Height() - XCOM_HEIGHT)/2;
				xRc.DeflateRect(0, gap);
			}
			xRc.top = cy;
			xRc.bottom = cy + XCOM_HEIGHT;
			m_xcom->MoveWindow(xRc);
			m_xcom->changepos();

			m_xcomX = xRc.left;
			
			wRc.SetRect(xRc.right-1/*+GAP*/, xRc.top, xRc.right-1/*+GAP*/+buttonSize, xRc.top+xRc.Height());
			m_searchS->MoveWindow(wRc);		

			CString	Path;
			Path.Format("%s\\%s\\ACCNTDEPT.INI", Axis::home, "tab");
			
			char readB[1024];
			int readL;
			
			readL = GetPrivateProfileString("ACCNTDEPT","DEPT","811",readB,sizeof(readB),Path);
			
			CString tDept(readB,readL);
			tDept.TrimLeft();tDept.TrimRight();

			if(m_dept == tDept)  //test acc
			{
				wRc.SetRect(wRc.right-1+GAP, xRc.top, wRc.right-1+GAP+buttonSize, xRc.top+xRc.Height());
				m_tacnt->MoveWindow(wRc);
			}
		}
		
		if (m_func[0])
			wRc.OffsetRect(3/*FUNC_GAP*/, 0);
		
		CString strFile;
		
		strFile.Format("%s\\tab\\TOP10.ini", Axis::home); 
		
		const int nUse = GetPrivateProfileInt("TOP10","USE",0,strFile);
		
		if (m_xcomex)
		{
			wRc.SetRect(wRc.right/*+GAP*/, wRc.top, 
				wRc.right/*+GAP*/+WIDTH_TOOLGROUP, wRc.top+wRc.Height());
			//wRc.OffsetRect(GAP, 0);
			int	gap = 0;
			
			if (wRc.Height() != XCOM_HEIGHT)
			{
				gap = (wRc.Height() - XCOM_HEIGHT)/2;
				wRc.DeflateRect(0, gap);
			}
			
			m_xcomex->MoveWindow(wRc);
			m_xcomex->changepos();
			
			wRc.SetRect(wRc.right+GAP, wRc.top, wRc.right+GAP+23, wRc.top+wRc.Height());
			m_register->MoveWindow(wRc);	

			m_registerX = wRc.right;
			
			mRc.SetRect(wRc.right+GAP*3, cRc.top, cRc.right /*- m_vband->GetBandWidth().cx*/, cRc.bottom);

			//0 = 사용안함 1 = 직원용 2 = 고객용 3 = 모두
			//if((nUse == 2 && Axis::isCustomer) || (nUse == 1 && !Axis::isCustomer) || nUse == 3)
			{
				mRc.right -= (m_nFunc*(buttonSize+FUNC_BUTTON_GAP)+8) + (BANNER_WIDTH + BANNER_STAFF_WIDTH + (FUNC_BUTTON_GAP*5) ) + 90 ;

#ifdef DF_MK_CAPTION
				CRect badnRec;
				badnRec = mRc;
				badnRec.right -= (m_HOB->IsWindowVisible() * DF_BTNHOBWIDTH) + DF_BTNHOBWIDTH + FUNC_BUTTON_GAP;

				if (m_btMKSELECT->IsWindowVisible())
					badnRec.right -= DF_BTNHOBWIDTH;

				m_band->MoveWindow(badnRec);
				m_sband->MoveWindow(badnRec);
#endif
				CRect tmpRect, tmpAIRec;
				tmpRect.left = mRc.right + FUNC_BUTTON_GAP;
				tmpRect.top = mRc.top + FUNC_BUTTON_GAP + 1;
				tmpRect.right = tmpRect.left + DF_BTNHOBWIDTH;
				tmpRect.bottom = mRc.top + BANNER_HEIGHT - FUNC_BUTTON_GAP + 1;
				m_HOB->MoveWindow(tmpRect);

#ifndef DF_MK_CAPTION
				if(m_showAI)
				{
					CRect bandrec;
					bandrec = mRc;
					bandrec.right -= BANNER_WIDTH;// -FUNC_BUTTON_GAP * 13;
					m_band->MoveWindow(bandrec);
					m_sband->MoveWindow(bandrec);

					if (Axis::isCustomer)  //고객은 HOB 버튼을 안보여줌
						tmpRect.OffsetRect(0 * tmpRect.Width() - FUNC_BUTTON_GAP, 0);
					else
						tmpRect.OffsetRect(-1 * tmpRect.Width() - FUNC_BUTTON_GAP, 0);

					m_AI->ShowWindow(SW_SHOW);
					m_AI->MoveWindow(tmpRect);
				}
				else
				{
					m_band->MoveWindow(mRc);
					m_sband->MoveWindow(mRc);
				}
				mRc.right += DF_BTNHOBWIDTH + FUNC_BUTTON_GAP;
#else
				mRc.right += DF_BTNHOBWIDTH + FUNC_BUTTON_GAP;
				if (m_HOB->IsWindowVisible())
				{
					tmpRect.OffsetRect(-1 * (DF_BTNHOBWIDTH + FUNC_BUTTON_GAP), 0);
					m_btNXT->MoveWindow(tmpRect);

					m_slog.Format("[btnkrx] m_btNXT  L=[%d] T=[%d] R=[%d] B=[%d] Height=[%d] Width=[%d]",
						tmpRect.left, tmpRect.top, tmpRect.right, tmpRect.bottom, tmpRect.Height(), tmpRect.Width());
					OutputDebugString(m_slog);

					tmpRect.OffsetRect((-1 * (DF_BTNHOBWIDTH)) - 2, 0);
					m_btKRX->MoveWindow(tmpRect);

					m_slog.Format("[btnkrx] m_btKRX L=[%d] T=[%d] R=[%d] B=[%d] Height=[%d] Width=[%d]",
						tmpRect.left, tmpRect.top, tmpRect.right, tmpRect.bottom, tmpRect.Height(), tmpRect.Width());
					OutputDebugString(m_slog);

					tmpRect.OffsetRect((-1 * (DF_BTNHOBWIDTH + FUNC_BUTTON_GAP)) - 2, 0);
					m_btMKSELECT->MoveWindow(tmpRect);
				}
				else
				{
					m_btNXT->MoveWindow(tmpRect);
					tmpRect.OffsetRect((-1 * (DF_BTNHOBWIDTH)) - 1, 0);
					m_btKRX->MoveWindow(tmpRect);
					tmpRect.OffsetRect((-1 * (DF_BTNHOBWIDTH + FUNC_BUTTON_GAP)) - 2, 0);
					m_btMKSELECT->MoveWindow(tmpRect);

					m_slog.Format("[btnkrx] m_btNXT  L=[%d] T=[%d] R=[%d] B=[%d]",
						tmpRect.left, tmpRect.top, tmpRect.right, tmpRect.bottom, tmpRect.Height(), tmpRect.Width());
					OutputDebugString(m_slog);
				}
#endif
				CRect sRc;
				sRc = cRc;
				sRc.right -= GAP;
				sRc.left = sRc.right - 110;
				sRc.InflateRect(0, -1 * GAP);
				sRc.bottom -= 1;
				m_top10->MoveWindow(sRc);
				m_top10->BringWindowToTop();	

				sRc.OffsetRect(-1 * BANNER_WIDTH - GAP, 0);
				sRc.right = sRc.left + BANNER_WIDTH;
				m_banner->MoveWindow(sRc);

				CRect vRc;
				vRc = cRc;
				vRc.right = sRc.left - GAP;
				vRc.left = vRc.right - m_nFunc * (buttonSize + FUNC_BUTTON_GAP) - 10;
				m_vband->MoveWindow(vRc);
			}
		}
		
		if (m_func[1])
		{
			for (int ii = 1; ii < MAX_FUNC; ii++)
				m_func[ii]->ShowWindow(SW_HIDE);
	
			if((nUse == 2 && Axis::isCustomer) || (nUse == 1 && !Axis::isCustomer) || nUse == 3)
			{
				wRc.left = mRc.right + FUNC_BUTTON_GAP;
			}
			else
			{
				wRc.left = mRc.right + (BANNER_WIDTH + (FUNC_BUTTON_GAP*2));
			}

			if (m_bFunc)
			{
				if (m_FuncType == 1)
				{
					wRc.OffsetRect(5, 1);
					wRc.right = wRc.left + buttonSize;
					
					for (int ii = 0; ii < m_nFunc; ii++)
					{	
						const int index = m_arFunc.GetAt(ii) + 1;
						
						m_func[index]->MoveWindow(wRc);
						m_func[index]->ShowWindow(SW_SHOW);
						wRc.OffsetRect(buttonSize + FUNC_BUTTON_GAP, 0);
						m_funcX = wRc.right;
					}
					wRc.OffsetRect(-buttonSize, 0);
				}
				else
				{
					mRc.OffsetRect(18, 0);
					mRc.right = mRc.left + buttonSize;

					mRc.SetRect(wRc.left, cRc.top + 2, wRc.right, cRc.top + 2 + buttonSize);
					const int quotient = (int)(m_nFunc / 2.0 + 0.5);
					
					for (int ii = 0; ii < m_nFunc; ii++)
					{	
						const int index = m_arFunc.GetAt(ii) + 1;
						
						m_func[index]->MoveWindow(mRc);
						m_func[index]->ShowWindow(SW_SHOW);

						if (ii + 1 == quotient)
						{
							tRc = mRc;
							mRc.OffsetRect(-buttonSize * ii, buttonSize);
							wRc.SetRect(tRc.left, wRc.top, tRc.right, wRc.bottom);
						}
						else
							mRc.OffsetRect(buttonSize + FUNC_BUTTON_GAP, 0);
					}
				}
			}
		}	
	}

	pDC->SelectObject(font);
	pDC->SetBkMode(mode);

	draw_BackGround(pDC);
	ReleaseDC(pDC);
}

bool CDbar1::IsInputFocus()
{
	return m_xcom->IsInputFocus();
}

void CDbar1::SetBandType()
{
	if (m_type == TTYPE1)
		m_type = TTYPE2;
	if (m_type == TTYPE1)
	{
		m_band->ShowWindow(SW_HIDE);
		m_sband->ShowWindow(SW_SHOW);
	}
	else if (m_type == TTYPE2)
	{
		m_band->ShowWindow(SW_SHOW);
		m_sband->ShowWindow(SW_HIDE);
	}	

	m_vband->SetBandType(m_type);
	(*m_axiscall)(AXI_RECALCLAYOUT, 0, 0);

	Change_Skin(CNPalette::GetTheme());

	Invalidate();
}

void CDbar1::SetBandType(int type)
{
	m_type = type;

	CProfile profile(pkUserTool);
	profile.Write(szUserSN, TOOLTYPE, Format("%d", m_type));

	SetBandType();
}

CString CDbar1::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CDbar1::SetHome()
{
	if (m_xcom)
		m_xcom->set_Home();
}

int CDbar1::GetHeight()
{
	return m_height;
}




void CDbar1::OnClose() 
{
	if (m_hHtmlInst) FreeLibrary(m_hHtmlInst);
	
	CDialogBar::OnClose();
}

void CDbar1::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TM_TOPBANNER_ROTATE:
		{
			if(m_bRotate)
			{
				RotateBanner();
			}
		}
		break;
	case TM_TOP10_RATATE:
		{
			if(m_arrTOP10.GetSize() > 0)
			{
				CString str,data;
				str = m_arrTOP10.GetAt(m_currTOP10);

				(*m_axiscall)(AXI_FUNCKEY, (WPARAM)(CTRL_FUNC + 17), (LPARAM)m_currTOP10);

				CString sTop;
				sTop.Format("%02d %s",m_currTOP10+1,str);

				m_btnTop10->set_Text(sTop);
				m_currTOP10++;
				
				if (m_currTOP10 >= m_arrTOP10.GetSize())
					m_currTOP10 = 0;
			}
		}
		break;
	case TM_TOP10:
		{
			KillTimer(TM_TOP10);

			CRect rc;
			m_btnTop10->GetWindowRect(&rc);
			
			RECT r;
			r.top = rc.top;
			r.left = rc.left;
			r.bottom = rc.bottom;
			r.right = rc.right;
		
			(*m_axiscall)(AXI_FUNCKEY, MAKEWPARAM(CTRL_FUNC+16, 0), (LPARAM)&r);	
		}
		break;
	}
	
	CDialogBar::OnTimer(nIDEvent);
}

void CDbar1::SetTOP10(const CStringArray &arr )
{
	KillTimer(TM_TOP10_RATATE);

	m_arrTOP10.RemoveAll();

	for(int i=0;i<arr.GetSize();i++)
	{
		m_arrTOP10.Add(arr.GetAt(i));
	}

	if(m_currTOP10 > m_arrTOP10.GetSize() - 1)
	{
		m_currTOP10 = 0;
	}

	if(m_arrTOP10.GetSize() > 0)
		SetTimer(TM_TOP10_RATATE,m_nTOPTime,NULL);
}

void CDbar1::PBMsgFromMain(CString strdata)
{
	if(strdata == "0")  //뉴스 수신시 main에서 메시지
	{
	//	KillTimer(TM_CHOKNTOAK_ROTATE);
	//	SetTimer(TM_CHOKNTOAK_ROTATE, 1000, NULL);
	//	m_nChokRotate = 0;

	}
	else
	{
	//	KillTimer(TM_CHOKNTOAK_ROTATE);
	//	m_btnChokNTalk->set_Image(CAxButtonName(Format("TOOL_%s", "촉앤토크")));  
	//	m_btnChokNTalk->Invalidate();
	//	m_nChokRotate = 0;
	
	}
}

#ifdef DF_MK_CAPTION
void CDbar1::SetShowAIBtn(int Kgubn, int Ngubn)
{
	CString stmp;
	//stmp.Format("%d", igubn);
	if (1)
	{
		if (Kgubn >= 0)
		{
			switch (Kgubn)
			{
			case 0: //장마감
			{
				m_btKRX->set_Image(CAxButtonName(Format("TOOL_%s", "KRX_OFF")));
			}
			break;
			case 1: //KRX 시간외
			{
				m_btKRX->set_Image(CAxButtonName(Format("TOOL_%s", "KRX_TIMEOUT")));
			}
			break;
			case 2: //KRX 정규장
			{
				m_btKRX->set_Image(CAxButtonName(Format("TOOL_%s", "KRX_ON")));
			}
			break;
			case 3: //KRX 단일가
			{
				m_btKRX->set_Image(CAxButtonName(Format("TOOL_%s", "KRX_SINGLE")));
			}
			break;
			case 4: //KRX 장전
			{
				m_btKRX->set_Image(CAxButtonName(Format("TOOL_%s", "KRX_MBEFORE")));
			}
			break;
			case 5: //KRX 프리
			{
				m_btKRX->set_Image(CAxButtonName(Format("TOOL_%s", "KRX_FREE")));
			}
			break;
			case 6: //KRX 애프터
			{
				m_btKRX->set_Image(CAxButtonName(Format("TOOL_%s", "KRX_AFTER")));
			}
			break;
			}
		}
		m_btKRX->Invalidate();


		if (Ngubn >= 0)
		{
			switch (Ngubn)
			{
			case 0: //장마감
			{
				m_btNXT->set_Image(CAxButtonName(Format("TOOL_%s", "NXT_OFF")));
			}
			break;
			case 1: //NXT 프리
			{
				m_btNXT->set_Image(CAxButtonName(Format("TOOL_%s", "NXT_FREE")));
			}
			break;
			case 2: //NXT 메인
			{
				m_btNXT->set_Image(CAxButtonName(Format("TOOL_%s", "NXT_ON")));
			}
			break;
			case 3: //NXT 애프터
			{
				m_btNXT->set_Image(CAxButtonName(Format("TOOL_%s", "NXT_AFTER")));
			}
			break;
			case 4: //NXT 15:20~15:30  단일가 개시
			{
				m_btNXT->set_Image(CAxButtonName(Format("TOOL_%s", "NXT_ONEPRC"))); 
			}
			break;
			case 5: //NXT 장전
			{
				m_btNXT->set_Image(CAxButtonName(Format("TOOL_%s", "NXT_MBEFORE")));
			}
			break;
			}
		}
		m_btNXT->Invalidate();
	}
}
#endif

