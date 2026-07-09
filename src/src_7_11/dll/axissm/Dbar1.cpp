// Dbar1.cpp : implementation file
//

#include "stdafx.h"
#include "Dbar1.h"
#include "xcom.h"
#include "xcomex.h"
#include "nbutton.h"
#include "hBand.h"
#include "SDIBand.h"
#include "trsearch.h"
#include "tsetup.h"
#include "tmenu.h"
#include "smdefine.h"
#include "SVband.h"
#include "fxSpin.h"
#include "misctype.h"
#include "NPalette.h"

#include "color.h"

#include "../../h/axisvar.h"
#include "../../axis/axmsg.hxx"
#include "resource.h"

#include "../../axis/resource.h"
#include "axcommon.hxx"
#include "axSB.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RBANDW		108
#define	GAP		3
#define	LOGOWIDTH	134
#define	COLOR_CTRLBARBK	RGB(236, 236, 236)//::GetSysColor(30) /*COLOR_MENUBAR*/
#define LT_COLOR	RGB(171, 168, 153)
#define RB_COLOR	RGB(115, 113, 100)

#define	TOOL_MENUBASE	6000
#define	TOOL_MENUEND	6100
#define ORDERSCREEN1	6101
#define ORDERSCREEN2	6102
#define COLOR_SET	6200

#define TTYPE1		1
#define TTYPE2		2

#define GAP 10


const	int	maxTICK     = 2;
const	int	tickWIDTH   = 270;	// 1280
const	int	tickWIDTHx  = 205;	// 1280~

struct	_ixinfo {
	UINT	id;				// window id
	UINT	oatype;				// pane type
	char	title[32];
} ixinfo[4] = {
	{ID_VIEW_INDEX1,	OA_RTMK2,	"KP200"},
	{ID_VIEW_INDEX2,	OA_RTMFJ,	"KP선물"},

	{ID_VIEW_INDEXX1,	OA_RTMK0,	"종합"},
	{ID_VIEW_INDEXX2,	OA_RTMNF,	"나스닥"}
};

//const char* strImg[] = {"설정frame", "화면캡춰", "인쇄", "화면캡춰", "화면캡춰"};
const char* strImg[] = {"설정frame", "화면캡춰", "이미지저장", "아이콘", "모두닫기"};


#include "Winuser.h"
/////////////////////////////////////////////////////////////////////////////
// CDbar1

CDbar1::CDbar1(bool (*callback)(int, WPARAM, LPARAM), CString home)
{
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);

	m_axiscall	= callback;
	m_home		= home;
	m_bk		= COLOR_CTRLBARBK;
	m_height	= HEIGHT_CTRLBAR1;
	m_defaultColor	= false;

	m_hide		= NULL;
	m_xcom		= NULL;
	m_searchS	= NULL;
	m_Vband		= NULL;

	m_xcomex	= NULL;
	m_register	= NULL;
	m_band		= NULL;
	m_CSPIN		= NULL;
	m_zero		= NULL;
	m_type      = 2;
	m_itemW = 0;

	m_img[IMG_BACKGROUND].LoadBitmap(IDB_BAR1BACK);
	m_img[IMG_TOOLIMAGE].LoadBitmap(IDB_TOOLBOX);

	CWinApp* app = AfxGetApp();
	m_user = app->GetProfileString(WORKSTATION, SIGNONID);
	int pos = m_user.Find('|');
	if (pos < 0)
		m_user = _T("Anonymous");
	else	
		m_user = m_user.Mid(pos+1);

	if (m_user.IsEmpty())
		m_user = _T("Anonymous");

	CString file;
	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	m_type = GetPrivateProfileInt(USERSN, TOOLTYPE, TTYPE1, file);

	m_tickers.RemoveAll();

	m_maxfunc = 5;

	for(int ii = 0 ; ii < 5 ;ii++)
		m_func[ii] = NULL;

	m_btnBanner = NULL;
}

CDbar1::~CDbar1()
{
	int ii = 0;
	if (m_hide)		delete m_hide;
	if (m_xcom)		delete m_xcom;
	if (m_searchS)		delete m_searchS;
	if (m_Vband)		delete m_Vband;

	if (m_xcomex)		delete m_xcomex;
	if (m_register)		delete m_register;
	if (m_band)		delete m_band;
	if (m_CSPIN)		delete m_CSPIN;
	if (m_zero)		delete m_zero;

	m_font.DeleteObject();
	for (ii = 0; ii < IMG_MAX; ii++)
	{
		if (m_img[ii].GetSafeHandle())
			m_img[ii].DeleteObject();
	}

	for (ii = 0; ii < m_tickers.GetSize(); ii++)
		delete m_tickers.GetAt(ii);
	m_tickers.RemoveAll();
}


BEGIN_MESSAGE_MAP(CDbar1, CDialogBar)
	//{{AFX_MSG_MAP(CDbar1)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbar1 message handlers

void CDbar1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	draw_BackGround(&dc);
//	draw_Gripper(&dc);
}

LRESULT CDbar1::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	CString imgC, imgO, imgD;
	switch (LOWORD(wParam))
	{
	case CTRL_SEARCHS:
		{
			CTrSearch	dlg(m_menu, m_home, m_axiscall);
			dlg.setPos(GetPos(1));
			if (dlg.DoModal() == IDOK)
				(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *)dlg.getTR().operator LPCTSTR());
		}
		break;
	case CTRL_REGISTER:
		(*m_axiscall)(AXI_FUNCKEY, wParam, lParam);
		break;
	case CTRL_HIDE:
		(*m_axiscall)(AXI_HIDETICKER, 0, 4);	break;
		break;
	case CTRL_COLORINDEX:
		ResetPalette();
		break;
	case CTRL_BANNER:
		(*m_axiscall)(AXI_FUNCKEY, wParam, (int) 1);
		break;
	case CTRL_FUNC:
	case CTRL_FUNC+1:	case CTRL_FUNC+2:	case CTRL_FUNC+3:	case CTRL_FUNC+4:
	case CTRL_FUNC+5:
		(*m_axiscall)(AXI_FUNCKEY, wParam, lParam);
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

	if (m_type == TTYPE1)		m_height = HEIGHT_CTRLBAR1;
	else if (m_type == TTYPE2)	m_height = HEIGHT_CTRLBAR2;

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

void CDbar1::make_Ctrl()
{
	CString		imgC, imgO, imgD, text;
	DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	m_hide = new CNButton(CTRL_HIDE);
	if (!m_hide->Create(NULL, "숨기기", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_hide;
		m_hide = NULL;
	}
	else
	{
		m_hide->set_ToolTipText("숨기기");
		imgC.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_barhide");
		imgO.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_barhide_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, "image", "mb_barhide_dn");
		m_hide->set_Image(imgC, imgO, imgD);
	}

	m_xcom = new CXcom(CTRL_SCREEN);
	if (!m_xcom->Create(NULL, "screenNo", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_xcom;
		m_xcom = NULL;
	}
	else	m_xcom->init(m_axiscall, m_home);
	m_xcom->SetFont(&m_font);

	m_xcomex = new CXcomex(m_home, this);
	if (!m_xcomex->Create(NULL, "toolgroup", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_xcomex;
		m_xcomex = NULL;
	}
	else	m_xcomex->init();

	
	m_Vband = new CSVband(m_axiscall, m_home/*, IPOS_RIGHT*/);
	if (!m_Vband->Create(NULL, "SVBAND", WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|WS_CHILD, CRect(0), this, -1))
	{
		delete m_Vband;
		m_Vband = NULL;
	}
	else
		m_Vband->make_Ctrl();
	m_Vband->ShowWindow(SW_HIDE);   //가상화면 관련 버튼이라서 생략

		
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
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "mb_screensearch");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "mb_screensearch_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "mb_screensearch_dn");
		m_searchS->set_Image(imgC, imgO, imgD);
	}

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
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "mb_menuadd");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "mb_menuadd_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "mb_menuadd_dn");
		m_register->set_Image(imgC, imgO, imgD);
	}
	m_register->ShowWindow(SW_HIDE);

	m_band = new CHBand(m_axiscall, m_home);
	if (!m_band->Create(NULL, "tool menu", WS_CHILD, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_band;
		m_band = NULL;
	}
	
	//배너 버튼 추가
	m_btnBanner = new CNButton(CTRL_BANNER);
	m_btnBanner->set_Bk(RGB(255,0,255)); 
	if(!m_btnBanner->Create(NULL, "banner", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_btnBanner;
		m_btnBanner = NULL;
	}
	else
	{
		m_btnBanner->set_ToolTipText(_T("메신저"));
		imgC.Format(_T("%s\\%s\\%s%s"), m_home, IMAGEDIR, _T("messanger"), _T(".BMP"));
		imgO.Format(_T("%s\\%s\\%s%s"), m_home, IMAGEDIR, _T("messanger"), _T(".BMP"));
		imgD.Format(_T("%s\\%s\\%s%s"), m_home, IMAGEDIR, _T("messanger"), _T(".BMP"));
		m_btnBanner->set_Image(imgC, imgO, imgD);
	}

	//test 버튼 추가
	CString tooltip[] = { 	"종합환경설정",
				"화면캡쳐",	
				"화면인쇄",
				"모두아이콘",
				"모두닫기",
	};

	for(int ii = 0 ; ii < m_maxfunc ; ii++)
	{
		m_func[ii] = new CNButton(CTRL_FUNC + ii, RGB(255,0,255), 1, false);
		//m_func[ii]->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));  //COLOR_FRAME
		m_func[ii]->set_Bk(RGB(255,0,255)); 

		if (!m_func[ii]->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
		{
			delete m_func[ii];
			m_func[ii] = NULL;
		}
		else
		{
			m_func[ii]->SetMaskDraw(true, COLOR_FRAME);
			m_func[ii]->set_ToolTipText(tooltip[ii]);
			imgC.Format(_T("%s\\%s\\%s%s"), m_home, IMAGEDIR, strImg[ii], _T(".BMP"));
			imgO.Format(_T("%s\\%s\\%s_DN%s"), m_home, IMAGEDIR, strImg[ii], _T(".BMP"));
			imgD.Format(_T("%s\\%s\\%s_EN%s"), m_home, IMAGEDIR, strImg[ii], _T(".BMP"));
			m_func[ii]->set_Image(imgC, imgO, imgD);
		}

	}
	// 버튼 추가

	CAxSB* ticker;
	for (int ii = 0; ii < maxTICK; ii++)
	{
		ticker = new CAxSB(sbTEXT, ii, true); 
		m_tickers.Add(ticker);
	}

	m_CSPIN = new CfxSpin();
	if (!m_CSPIN->Create(attrCORGBx, spNUMBER, CRect(0, 0, 0, 0), this, COLOR_SET))
	{
		delete m_CSPIN;
		m_CSPIN = NULL;
	}
	else
	{
		m_CSPIN->SetRangeEx(-100, 100);
		m_CSPIN->SetText("0");
	}
	m_CSPIN->ShowWindow(SW_HIDE);

	m_zero = new CNButton(CTRL_COLORINDEX);
	m_zero->set_Color(RGB(247, 251, 196));
	if (!m_zero->Create(NULL, "Color Zero", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_zero;
		m_zero = NULL;
	}
	else	
	{
		text.LoadString(ST_TEXT_COLORSET);
		m_zero->set_ToolTipText(text);
	}
	m_zero->ShowWindow(SW_HIDE);

	SetBandType(2);
	load_BandItem();
}

void CDbar1::Change_Skin(CString skinName)
{
	m_band->change_Skin(skinName);
}

void CDbar1::draw_BackGround(CDC* pDC)
{
	CRect	cRc;
	GetClientRect(cRc);
	pDC->FillSolidRect(cRc, COLOR_FRAME);

	//CPen	pen1(PS_SOLID, 1, RGB(128, 128, 128)); //RGB(76, 81, 90))
	CPen	pen1(PS_SOLID, 1, RGB(76, 81, 90));
	CPen	pen2(PS_SOLID, 1, RGB(255, 255, 255));

	CPen	*pOldPen = pDC->SelectObject(&pen1);
	pDC->MoveTo(0, 0);
	pDC->LineTo(cRc.right, 0);
	
	pDC->SelectObject(&pen2);
	pDC->MoveTo(0, 1);
	pDC->LineTo(cRc.right, 1);

	pDC->SelectObject(pOldPen);
#if 0
	BITMAP	bm;
	m_img[IMG_BACKGROUND].GetBitmap(&bm);

	CDC	memDC;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(&m_img[IMG_BACKGROUND]);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), bm.bmHeight, 
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		else	memDC.DeleteDC();
	}
#endif
}

void CDbar1::draw_Gripper(CDC* pDC)
{
	CRect	gRc(get_GripperRect());
	pDC->Draw3dRect(gRc, GRIP_LT, GRIP_RB);
}

void CDbar1::draw_Seperate(CDC* pDC, CRect rc)
{
	CRect	sRc, cRc;

	GetClientRect(cRc);
	sRc.SetRect(rc.right+1, cRc.top + 4, rc.right+3, cRc.bottom - 3);
	pDC->Draw3dRect(sRc, GRIP_RB, GRIP_LT);
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
		pRect.DeflateRect(3, 5, 3, 3);
		pRect.right = pRect.left + 9;
	}
	else if (m_type == TTYPE2)
	{
		pRect.DeflateRect(3, 9);
		pRect.right = pRect.left + 9;
	}

	return pRect;
}

void CDbar1::set_Background(COLORREF color)
{
	m_bk = color;
	m_defaultColor = false;
	Invalidate();
}

void CDbar1::set_Barheight(int height)
{
	m_height = height;
	GetDockingFrame()->RecalcLayout();
	Invalidate();
}

void CDbar1::SetHome()
{
	m_xcom->set_Home();
}

void CDbar1::set_Maps(CString mapname)
{
//	CString	mapN = m_menu->GetMapping(mapname);
	m_xcom->set_Maps(mapname);
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
	return _T("");
}

void CDbar1::load_BandItem()
{
	if (!m_band)	
		return;

	m_band->removeALL();

	char	ssb[1024*4];
	CString	file, cpfile, tmpS, path, str, name, map, temp, sname, strresult, gubn;


	CMapStringToString toolmap;

	path.Format("%s\\%s\\%s", m_home, TABDIR, AXISMENU);

	FILE *fp = NULL;
	errno_t err = fopen_s(&fp, path, "r");
	if (!fp)
	{
		Invalidate();
		return;
	}

	while (fgets(ssb, sizeof(ssb), fp) != NULL)
	{
		str.Format(_T("%s"), ssb);
		tmpS = Parser(str, "\t");
		if(tmpS  == "정보")
		{
			gubn = tmpS;
			name = Parser(str, "\t");
			map = Parser(str, "\t");
			sname = Parser(str, "\t");
			temp.Format(_T("%s=%s;%s\t"),map,name, sname);
			strresult += temp;
			toolmap.SetAt(gubn, strresult);
			
		}
		else if(tmpS  == "수요예측")
		{
			if(gubn != tmpS)
			{
				strresult.Empty();
				gubn = tmpS;
			}
			name = Parser(str, "\t");
			map = Parser(str, "\t");
			sname = Parser(str, "\t");
			temp.Format(_T("%s=%s;%s\t"),map,name, sname);
			strresult += temp;
			toolmap.SetAt(gubn, strresult);
		}
		else if(tmpS  == "공지/게시")
		{
			if(gubn != tmpS)
			{
				strresult.Empty();
				gubn = tmpS;
			}
			name = Parser(str, "\t");
			map = Parser(str, "\t");
			sname = Parser(str, "\t");
			temp.Format(_T("%s=%s;%s\t"),map,name, sname);
			strresult += temp;
			toolmap.SetAt(gubn, strresult);
		}
		else if(tmpS  == "관리")
		{
			if(gubn != tmpS)
			{
				strresult.Empty();
				gubn = tmpS;
			}
			name = Parser(str, "\t");
			map = Parser(str, "\t");
			sname = Parser(str, "\t");
			temp.Format(_T("%s=%s;%4s\t"),map,name, sname);
			strresult += temp;
			toolmap.SetAt(gubn, strresult);
		}
	}
	fclose(fp);

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	cpfile.Format("%s\\%s\\usertool.ini", m_home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	m_type = GetPrivateProfileInt(USERSN, TOOLTYPE, TTYPE1, file);
	//test
	m_type = 2;

	DWORD ssL = GetPrivateProfileString(USERSN, USEREN, "0, 0", ssb, sizeof(ssb), file);
	tmpS = CString(ssb, ssL);
	int	idx = tmpS.Find(',');
	if (idx == -1)	idx = 0;
	else
	{
		tmpS = tmpS.Left(idx);
		idx = atoi(tmpS);
	}

	tmpS.Format("%02d", idx);
	GetPrivateProfileString(USERSN, tmpS, "0", ssb, sizeof(ssb), cpfile);
	CString string, item;
	string.Format(_T("%s"), ssb);
	//CString item, string = CString(ssb, ssL);

	if(toolmap.Lookup(ssb, string))
	{
		for (; !string.IsEmpty(); )
		{
			idx = string.Find('\t');
			if (idx == -1)	break;

			item  = string.Left(idx++);
			string = string.Mid(idx);

			idx = item.Find('=');
			if (idx == -1)	continue;

			m_band->ItemADD(item, IsList(item.Left(idx)));
		}
	}

	m_band->ReCalc();
	m_band->SetToolTipText("USER SPACE");
	m_band->Invalidate();

	//--test
	ssL = GetPrivateProfileString(USERSN, tmpS, "", ssb, sizeof(ssb), file);
	if (ssL <= 0)	
		m_xcomex->set_gName(_T(""));
	else		
		m_xcomex->set_gName(ssb);
	//test
	if (m_CSPIN)
	{
		ssL = GetPrivateProfileString("UserColor", "index", "0", ssb, sizeof(ssb), file);
		if (ssL <= 0)	m_CSPIN->SetText("0");
		else		m_CSPIN->SetText(ssb);
	}
	
	/*
	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	cpfile.Format("%s\\%s\\usertool.ini", m_home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	m_type = GetPrivateProfileInt(USERSN, TOOLTYPE, TTYPE1, file);

	DWORD ssL = GetPrivateProfileString(USERSN, USEREN, "0, 0", ssb, sizeof(ssb), file);
	tmpS = CString(ssb, ssL);
	int	idx = tmpS.Find(',');
	if (idx == -1)	idx = 0;
	else
	{
		tmpS = tmpS.Left(idx);
		idx = atoi(tmpS);
	}

	tmpS.Format("%02d", idx);
	ssL = GetPrivateProfileSection(tmpS, ssb, sizeof(ssb), file);
	if (ssL <= 0)
	{
		m_band->ReCalc();
		m_band->SetToolTipText("USER SPACE");
		m_band->Invalidate();

		ssL = GetPrivateProfileString(USERSN, tmpS, "", ssb, sizeof(ssb), file);
		if (ssL <= 0)	m_xcomex->set_gName(_T(""));
		else		m_xcomex->set_gName(ssb);

		return;
	}

	CString item, string = CString(ssb, ssL);

	for (; !string.IsEmpty(); )
	{
		idx = string.Find('\0');
		if (idx == -1)	break;

		item  = string.Left(idx++);
		string = string.Mid(idx);

		idx = item.Find('=');
		if (idx == -1)	continue;

		m_band->ItemADD(item, IsList(item.Left(idx)));
	}

	m_band->ReCalc();
	m_band->SetToolTipText("USER SPACE");
	m_band->Invalidate();

	//--test
	ssL = GetPrivateProfileString(USERSN, tmpS, "", ssb, sizeof(ssb), file);
	if (ssL <= 0)	
		m_xcomex->set_gName(_T(""));
	else		
		m_xcomex->set_gName(ssb);
	//test
	if (m_CSPIN)
	{
		ssL = GetPrivateProfileString("UserColor", "index", "0", ssb, sizeof(ssb), file);
		if (ssL <= 0)	m_CSPIN->SetText("0");
		else		m_CSPIN->SetText(ssb);
	}
	*/
}

void CDbar1::OnSize(UINT nType, int cx, int cy) 
{
	SetPosition();
	CDialogBar::OnSize(nType, cx, cy);
	if (GetStyle() & WS_BORDER)
		ModifyStyle(WS_BORDER, 0);
}

/*
void CDbar1::set_Code(CString code)
{
	if (m_xcom)	m_xcom->set_Code(code, m_home);
}
*/


void CDbar1::set_List(CStringArray& array)
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
	char		buf[128];
	CString		tmpS = _T("");
	CString		file, msg, title;

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	DWORD dw = GetPrivateProfileString(USERSN, USEREN, "0, 0", buf, sizeof(buf), file);
	tmpS = CString(buf, dw);
	int pos  = tmpS.Find(',');
	if (pos == -1)
	{
		title.LoadString(STSM_TEXT_OK);
		msg.LoadString(ST_MSG_ERRORINFOFILE);
		::MessageBox(m_hWnd, msg, title, MB_ICONSTOP);
		return;
	}

	int tot = atoi(tmpS.Mid(pos+1));
	tmpS.Format("%d, %d", index, tot);
	WritePrivateProfileString(USERSN, USEREN, tmpS, file);
	load_BandItem();
}

CString CDbar1::screenInfo()
{
	CString		info;
	CPoint		point;

	CTrSearch	dlg(m_menu, m_home);
	GetCursorPos(&point);
	dlg.setPos(point);
	if (dlg.DoModal() == IDOK)
		info = dlg.getTR();

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
	CString file;

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	m_type = GetPrivateProfileInt(USERSN, TOOLTYPE, TTYPE1, file);

	SetBandType(2);

	load_BandItem();
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
	if (!m_band)	return;
	m_band->ItemADD(dat);
}

void CDbar1::ShowMenu()
{
	CRect	wRc;

	m_register->GetWindowRect(wRc);
	CPoint	point(wRc.left, wRc.bottom);

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

void CDbar1::SetPosition()
{
	if (!m_band || !m_band->GetSafeHwnd())
		return;

	CDC*	pDC = GetDC();

	CRect	cRc, xRc, wRc, tRc, mRc, sRc;
	CSize	size = pDC->GetTextExtent(_T("$$$$$$"));
	CFont*	font = pDC->SelectObject(&m_font);
	int	mode = pDC->SetBkMode(TRANSPARENT);
	
	GetClientRect(cRc);

	tRc = get_GripperRect();
	if (m_hide)
		m_hide->MoveWindow(tRc);


	tRc.SetRect(tRc.right + 3, cRc.top, tRc.right + 3, cRc.bottom);
	tRc.DeflateRect(0, 9);

	/*
	if (m_CSPIN)
	{
		sRc.SetRect(tRc.right, tRc.top - 6, tRc.right+16, tRc.bottom - 6);
		m_CSPIN->MoveWindow(sRc);
	}
	if (m_zero)
	{
		sRc.SetRect(tRc.right, sRc.bottom + 2, tRc.right+16, tRc.bottom +7);
		m_zero->MoveWindow(sRc);
	}
	*/
	
	
	if (m_xcom)
	{
		
		xRc.SetRect(tRc.right + 10, tRc.top , tRc.right+GAP+size.cx+COMBTN_WIDTH+40, tRc.bottom );
		int	gap = 0;
		if (xRc.Height() != XCOM_HEIGHT)
		{
			gap = (xRc.Height() - XCOM_HEIGHT)/2;
			xRc.DeflateRect(0, gap);
		}

		m_xcom->MoveWindow(xRc);
		m_xcom->changepos();
		

		xRc.SetRect(tRc.right , xRc.top + 19, tRc.right+GAP+size.cx+COMBTN_WIDTH+40, xRc.bottom + 19);
		//xRc.OffsetRect(-1 * xRc.Width(), 0);
		m_Vband->MoveWindow(xRc);

// 		if (m_CSPIN)
// 		{
// 			xRc.SetRect(xRc.right+GAP+ 2, xRc.top - 12, xRc.right+GAP+55+ 2, xRc.top+xRc.Height()-12);
// 			m_CSPIN->MoveWindow(xRc);			
// 		}

		//xRc.SetRect(tRc.right, tRc.top, tRc.right+GAP+size.cx+COMBTN_WIDTH, tRc.bottom);
		
		//wRc.SetRect(xRc.right+GAP, xRc.top, xRc.right+GAP+xRc.Height(), xRc.top+xRc.Height());
		
		//m_searchS->MoveWindow(wRc);		
	}
	
	//배너버튼 , 기능버튼들 
	GetClientRect(cRc);
	if(m_btnBanner)
	{
		tRc = cRc;
		tRc.left = tRc.right - 150;
		m_btnBanner->MoveWindow(tRc);
	}

	
	if (m_func[0])
	{
		for (int ii = 0; ii < m_maxfunc; ii++)
			m_func[ii]->ShowWindow(SW_HIDE);
		//cRc = tRc;
		tRc.top = cRc.top + 10;
		tRc.bottom = tRc.top + 20;
		tRc.OffsetRect(0, -2);
		for (int ii = 5; ii > 0; ii--)
		{
			CSize size;
			size = m_func[ii-1]->get_ImgSize();
			m_func[ii-1]->ShowWindow(SW_SHOW);
			tRc.right = tRc.left - 5;

			//tRc.left = tRc.right - 20;
			tRc.left = tRc.right - size.cx;

			tRc.bottom = tRc.top + size.cy;
			m_func[ii-1]->MoveWindow(tRc);
			//m_func[ii-1]->set_BkMaskSize(cRc);
			m_func[ii-1]->Invalidate();
		}
	}
	
	if (m_Vband)
	{
		//m_Vband->GetWindowRect(wRc);
		m_xcom->GetWindowRect(wRc);
		ScreenToClient(&wRc);
	
		//--test
 	//	wRc.SetRect(wRc.right+GAP, wRc.top, 
	//		wRc.right+GAP+WIDTH_TOOLGROUP, wRc.top+wRc.Height());
	//	wRc.OffsetRect(GAP*2, 0);
 		int	gap = 0;
 		if (wRc.Height() != XCOM_HEIGHT)
 		{
			gap = (wRc.Height() - XCOM_HEIGHT)/2;
			wRc.DeflateRect(0, gap);
 		}
		wRc.OffsetRect(wRc.Width() + 10 , 0);
 		m_xcomex->MoveWindow(wRc);
 		m_xcomex->changepos();
		//test
		
		wRc.SetRect(wRc.right+GAP+ 2, wRc.top, wRc.right+GAP+20+ 2, wRc.top+xRc.Height());
		//wRc.SetRect(wRc.right+GAP+ 2, wRc.top, wRc.right+GAP+20+ 2, wRc.top+wRc.Height());
		m_register->MoveWindow(wRc);	
		
		//mRc.SetRect(wRc.right+GAP*3, cRc.top, cRc.right - tickWIDTH, cRc.bottom - 1);
		mRc.SetRect(wRc.right+GAP, cRc.top, cRc.right, cRc.bottom - 1);
		
		CBitmap	bitmap;
		BITMAP	bm;
		
		bitmap.LoadBitmap(IDB_EXPAND);
		bitmap.GetBitmap(&bm);
		if (mRc.left > mRc.right - bm.bmWidth/3)
			mRc.right = mRc.left + bm.bmWidth/3;
		//cRc.left
		mRc.right = tRc.left - GAP;
		mRc.OffsetRect( -30, 0);
		mRc.bottom += 3;
		m_band->MoveWindow(mRc);
	}

	pDC->SelectObject(font);
	pDC->SetBkMode(mode);

//	draw_BackGround(pDC);
	ReleaseDC(pDC);
}

bool CDbar1::IsInputFocus()
{
	return m_xcom->IsInputFocus();
}

void CDbar1::SetBandType()
{
	m_band->ShowWindow(SW_SHOW);

	GetDockingFrame()->RecalcLayout();
	Invalidate();
}

void CDbar1::SetBandType(int type)
{
	m_type = type;

	CString file, value;
	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	value.Format("%d", m_type);
	WritePrivateProfileString(USERSN, TOOLTYPE, value, file);

	SetBandType();
}

void CDbar1::Update(int type, char* data)
{
	for (int ii = 0; ii < m_tickers.GetSize(); ii++)
		((CAxSB *)m_tickers.GetAt(ii))->SetText(type, data);
}

void CDbar1::Update(int type, CStringArray& items)
{
	for (int ii = 0; ii < m_tickers.GetSize(); ii++)
	{
		CAxSB* ticker = (CAxSB *)m_tickers.GetAt(ii);
		if (ticker->m_oatype == type)
		{
			ticker->SetItems(items);
			break;
		}
	}
}

void CDbar1::SetsbConfig(int type, DWORD item, bool beep, CString file)
{
	for (int ii = 0; ii < m_tickers.GetSize(); ii++)
	{
		CAxSB* sb = (CAxSB *)m_tickers.GetAt(ii);
		if (sb->m_oatype == type)
		{
			sb->SetsbConfig(item, beep, file);
		}
	}
}

void CDbar1::drawingColor(COLORREF bkGround)
{
	for (int ii = 0; ii < m_tickers.GetSize(); ii++)
		((CAxSB *)m_tickers.GetAt(ii))->drawingColor(bkGround);
}

void CDbar1::Initialize_Ticker()
{
	/*
	CRect	rect, cRc;
	int	width = GetSystemMetrics(SM_CXSCREEN), height;
	if (width >= 1280)
		width = tickWIDTH;
	else
		width = tickWIDTHx;

	GetClientRect(rect);
	m_band->GetWindowRect(cRc);

	rect.left = cRc.right + GAP;
	rect.right -= GAP;
	rect.top += GAP;
	height  = rect.Height(); height /= 2;
	for (int ii = 0; ii < maxTICK; ii++)
	{
		rect.bottom  = rect.top + height;
		CAxSB* ticker = (CAxSB *)m_tickers.GetAt(ii);

		ticker->Initialize(ii, this, ixinfo[0+ii].id, rect, ixinfo[0+ii].oatype, NULL, ixinfo[0+ii].title);
		ticker->SetWindowPos(NULL, rect.left, rect.top, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOCOPYBITS|SWP_SHOWWINDOW);
		rect.top = rect.bottom;
	}
	*/
}

void CDbar1::SetSDI()
{
	m_hide->ShowWindow(SW_HIDE);
}

void CDbar1::SetMDI()
{
	m_hide->ShowWindow(SW_SHOW);
}

BOOL CDbar1::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR *p = (NMHDR*)lParam;

	if (wParam == COLOR_SET && p->code == SPNM_ONCHANGE)
		SetPalette();
	
	return CDialogBar::OnNotify(wParam, lParam, pResult);
}

void CDbar1::SetPalette()
{
	char		buffer[128];
	//COLORREF	color;
	CString		orgfile, file, data, skinName, SNKey;

	CString		setcolor;
	int		tmp = atoi(m_CSPIN->GetText());

	if (m_home.IsEmpty())	return;

	int nRGB[3];
	int addData;

	CArray	< int, int >	arRGB;

	orgfile.Format("%s\\%s\\PALETTE_ORG.INI", m_home, TABDIR);
	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	GetPrivateProfileString(GENERALSN, "Palette", "", buffer, sizeof(buffer), file);

	skinName = buffer;
	DWORD dw;
	for (int ii = 0; ii < 150; ii++)
	{
		SNKey.Format("RGB%03d", ii);
		dw = GetPrivateProfileString(skinName, SNKey, "", buffer, sizeof(buffer), orgfile);
		if (dw <= 0)
		{
			if (!skinName.Compare("Palette"))
				continue;
			//skinName = _T("Palette");
			dw = GetPrivateProfileString("Palette", SNKey, "", buffer, sizeof(buffer), orgfile);
			if (dw <= 0)	continue;
		}

		data = buffer;
		while (!data.IsEmpty())
		{
			int pos = data.Find(',');
			if (pos == -1)
			{
				arRGB.Add(atoi(data));
				break;
			}
			SNKey = data.Left(pos++);
			data = data.Mid(pos);
			arRGB.Add(atoi(SNKey));
		}
		
		if (tmp > 0)
		{
			if (arRGB.GetAt(0) == 255 || arRGB.GetAt(1) == 255 || arRGB.GetAt(2) == 255)
			{
				arRGB.RemoveAll();
				continue;
			}

			for (int jj = 0; jj < 3; jj++)
			{
				nRGB[jj] = 0;

				if (arRGB.GetAt(jj) == 0)
					addData = 5;
				else
					addData = arRGB.GetAt(jj) / 100 * 5;

				nRGB[jj] = arRGB.GetAt(jj) + (addData * tmp);

				if (nRGB[jj] > 255) nRGB[jj] = 255;
			}
		}
		else
		{
			if (arRGB.GetAt(0) == 0 || arRGB.GetAt(1) == 0 || arRGB.GetAt(2) == 0)
			{
				arRGB.RemoveAll();
				continue;
			}

			for (int jj = 0; jj < 3; jj++)
			{
				nRGB[jj] = 0;

				if (arRGB.GetAt(jj) == 0)
					addData = 5;
				else
					addData = arRGB.GetAt(jj) / 100 * 5;

				nRGB[jj] = arRGB.GetAt(jj) - (addData * (int)fabs((float)tmp));

				if (nRGB[jj] < 0) nRGB[jj] = 0;
			}
		}

		setcolor.Format("\t%03d, %03d, %03d", nRGB[0], nRGB[1], nRGB[2]);
		TRACE("setcolor [%d][%s]\n", ii, setcolor);
		
		SNKey.Format("RGB%03d", ii);
		WritePrivateProfileString(skinName, SNKey, setcolor, file);

		arRGB.RemoveAll();

// 		CColor	V_color;
// 		V_color.SetRGB(arRGB.GetAt(0), arRGB.GetAt(1), arRGB.GetAt(2));
// 
// 		float Luminance = atof(tmps);
// 		float SetLuminance;
// 		float GetLuminance;
// 
// 		if (tmps == "0")
// 		{
// 			setcolor.Format("\t%03d, %03d, %03d", arRGB.GetAt(0), arRGB.GetAt(1), arRGB.GetAt(2));
// 			
// 			SNKey.Format("RGB%03d", ii);
// 			WritePrivateProfileString(skinName, SNKey, setcolor, file);
// 		}
// 		else if (Luminance == 0)
// 		{
// 			arRGB.RemoveAll();
// 			return;
// 		}
// 		else
// 		{
// 			GetLuminance = V_color.GetLuminance();
// 
// 			CString str;
// 			str.Format("%f", GetLuminance * 200);
// 
// // 			int pos = str.Find('.');
// // 			
// // 			if (pos == -1)	Luminance += atof(str);
// // 			else
// 			Luminance += atof(str);
// 
// 			if (Luminance >= 200) Luminance = 200;
// 
// 			if (Luminance < 0) Luminance = 0;
// 
// 			SetLuminance = Luminance / 200;
// 
// 			V_color.SetLuminance(SetLuminance);
// 
// 			color = RGB(V_color.GetRed(), V_color.GetGreen(), V_color.GetBlue());
// 
// 			setcolor.Format("\t%03d, %03d, %03d", V_color.GetRed(), V_color.GetGreen(), V_color.GetBlue());
// 			
// 			SNKey.Format("RGB%03d", ii);
// 			WritePrivateProfileString(skinName, SNKey, setcolor, file);
// 		}
	}
	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	WritePrivateProfileString("UserColor", "index", m_CSPIN->GetText(), file);

	(*m_axiscall)(AXI_CHANGEBAR2, 0, (long)skinName.operator LPCTSTR());
}

void CDbar1::ResetPalette()
{
	char		buffer[128];
	//COLORREF	color;
	CString		orgfile, file, data, skinName, SNKey;

	CString		setcolor;

	if (m_home.IsEmpty())	return;

	CArray	< int, int >	arRGB;

	orgfile.Format("%s\\%s\\PALETTE_ORG.INI", m_home, TABDIR);
	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	GetPrivateProfileString(GENERALSN, "Palette", "", buffer, sizeof(buffer), file);

	skinName = buffer;
	DWORD dw;
	for (int ii = 0; ii < 150; ii++)
	{
		SNKey.Format("RGB%03d", ii);
		dw = GetPrivateProfileString(skinName, SNKey, "", buffer, sizeof(buffer), orgfile);
		if (dw <= 0)
		{
			if (!skinName.Compare("Palette"))
				continue;
			//skinName = _T("Palette");
			dw = GetPrivateProfileString("Palette", SNKey, "", buffer, sizeof(buffer), orgfile);
			if (dw <= 0)	continue;
		}

		data = buffer;
		while (!data.IsEmpty())
		{
			int pos = data.Find(',');
			if (pos == -1)
			{
				arRGB.Add(atoi(data));
				break;
			}
			SNKey = data.Left(pos++);
			data = data.Mid(pos);
			arRGB.Add(atoi(SNKey));
		}

		setcolor.Format("\t%03d, %03d, %03d", arRGB.GetAt(0), arRGB.GetAt(1), arRGB.GetAt(2));
		
		SNKey.Format("RGB%03d", ii);
		WritePrivateProfileString(skinName, SNKey, setcolor, file);

		arRGB.RemoveAll();
	}

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, m_user);
	WritePrivateProfileString("UserColor", "index", 0, file);

	m_CSPIN->SetText("0");

	(*m_axiscall)(AXI_CHANGEBAR2, 0, (long)skinName.operator LPCTSTR());
}
