// MapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "IB200200.h"
#include "MapWnd.h"
#include "ioformat.h"

#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/grid.h"
#include "../../axis/axMsg.hxx"
#include "../../h/memDC.h"
#include "CAxGrid.h"
#include "griditem.h"
#include "ContainerWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CURR_CALOR	// 현재가 대비색상

#define	portFILE	"portfolio.ini"
#define	interFILE	"interField.ini"
#define	deliMITER	";"

#define	HCOD		"1301"
#define	FCOD		"30301"
#define	OCOD		"40301"
#define	PCOD		"40301"
#define	UCOD		"20301"
#define SFCOD		"50301"
#define	ECOD		"ECOD"

#define	MAP_WIDTH	1022
#define	MAP_HEIGHT	470

#define	kcurrMAP	"IB200100"		// kospi  현재가
#define	fcurrMAP	"IB400100"		// future 현재가
#define	ocurrMAP	"IB400200"		// option 현재가
#define	focurrMAP	"IB440100"		// 주식옵션현재가
#define	icurrMAP	"IB290300"		// 업종 현재가
#define	configMAP	"IB202201"		// config map
#define sfcurrMAP	"IB400300"		// 주식선물 현재가 @@Zeta 화면 아직 없음

const int idGRID1	= 2001;
const int idGRID2	= 2002;
const int idGRID3	= 2003;
const int idGRID4	= 2004;

const int rowHeight	= 20;

const int gap1		= 1;
const int gap2		= 2;
const int gap3		= 3;
const int gap5		= 5;
const int comboH	= 20;
const int offsetGH	= gap5 + comboH + gap3;

const int comboW	= 102;
const int size18	= 20;
const int size44	= 36;


const int maxGROUP = 100, maxJONGMOK = 100;

const char*	gSYMBOL	= _T("$90303");
const char* 	gEXPECT	= _T("90991");

const char	gnCHAR	= 0x7f;
const char	tabCHAR	= 0x09;
const char	lfCHAR	= 0x0A;

const int	dispRow = 20 +1;
const int	scrollW = ::GetSystemMetrics(SM_CYHSCROLL);

const int	colNAME = 0, colCURR = 1, colDIFF = 2;

#define	TM_ATTACH	1233

/////////////////////////////////////////////////////////////////////////////
// CMapWnd dialog


CMapWnd::CMapWnd(CWnd* parent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CMapWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_parent = parent;
	m_fontsize = 9;

	m_gridW = 0;
//	m_rowC = maxJONGMOK + 1;
//	m_colC = 5;

	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		m_category[vidx] = NULL;
		m_inters[vidx].RemoveAll();
		m_waiting[vidx]   = false;
	}

	m_viewType = 4;
	m_blink = true;

	m_bRemove	=	FALSE;	// 2011.12.29 KDK

	m_nOldRow = -1;
}

CMapWnd::~CMapWnd()
{

}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_INTERNM0, OnSelchangeInterNM0)
	ON_CBN_SELCHANGE(IDC_INTERNM1, OnSelchangeInterNM1)
	ON_CBN_SELCHANGE(IDC_INTERNM2, OnSelchangeInterNM2)
	ON_CBN_SELCHANGE(IDC_INTERNM3, OnSelchangeInterNM3)
	ON_CBN_SELCHANGE(IDC_INTERNM4, OnSelchangeInterNM4)
	ON_CBN_SELCHANGE(IDC_INTERNM5, OnSelchangeInterNM5)
	ON_CBN_SELCHANGE(IDC_INTERNM6, OnSelchangeInterNM6)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_TOPARENT, OnParent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers
BOOL CMapWnd::create(CWnd* parent) 
{
	//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	if (!CWnd::Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0, 0, MAP_WIDTH, MAP_HEIGHT), parent, 100))
		return FALSE;

	OnInit();
	return TRUE;
}

BOOL CMapWnd::Create()
{
	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");

	m_font = getAxFont();
	m_fontB= getAxFont(idTOstring(IDS_DEFAULTFONT), m_fontsize, 3);

	m_strPal = GetPalette();

	return create(m_parent);
}

BOOL CMapWnd::OnInit() 
{	
	SetOwner(m_parent);
	Variant(titleCC, idTOstring(IDS_TITLE));
	
	_bkBrush = getAxBrush(GetColor(64));
	m_blinkColor = GetColor(getFlashIndex());
    	m_bookmarkColor = GetColor(199);

	m_font = getAxFont(idTOstring(IDS_DEFAULTFONT), m_fontsize);	

	loadGridinfo();
	restoreMF(); 
	loadFieldInfo(); 
	initControl(); 
	loadInterName();
	
	CRect	clientRC; 
	GetClientRect(&clientRC); 
	clientRC.top += offsetGH;

	LOGFONT lf; 
	m_font->GetLogFont(&lf); 
	lf.lfHeight = m_fontsize * 10;
	
	int  ii = 0;
	for_each(_grid.begin(), _grid.end(), [&](auto& grid){
		clientRC.left = m_gridW * ii; 
		clientRC.right = clientRC.left + m_gridW;

		grid = std::make_unique<AxGrid>();
		grid->setWizard(m_parent);
		grid->SetUserFont(m_font, m_fontB);
		grid->creategrid(clientRC, this, 20, 300, idGRID1 + ii++);
	});

	initialGrid(false); 
	repositionView();

	m_blink = IsBlink();
	SendMinMaxInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapWnd::loadGridinfo()
{
	CString filePath;
	filePath.Format(_T("%s/%s/%s/%s"), m_root, USRDIR, m_name, interFILE);
	int	readL = 0;
	char	readB[1024];
	CString	string, stringx;
	
	readL = GetPrivateProfileString(_T("INTERFIELD"), "BOOKMARKCOLOR", "", readB, sizeof(readB), filePath);
	if (readL > 0) m_bookmarkColor = _httoi(readB);
}

int CMapWnd::_httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	const CHexMap HexMap[HexMapL] =
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
	std::unique_ptr < TCHAR[] > mstr = std::make_unique<TCHAR[]>(strlen(value) * 2 + 1);
	memcpy(mstr.get(), _tcsupr(_tcsdup(value)), strlen(value));

	TCHAR *ss = mstr.get();
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

	return result;
}

void CMapWnd::OnClose() 
{
	DestroyWindow(); 
	CWnd::OnClose();
}

void CMapWnd::OnDestroy() 
{
	m_hWnd = NULL;
	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		clearInterest(vidx);

		if (m_category[vidx] != nullptr)
		{
			m_category[vidx]->DestroyWindow();
			m_category[vidx] = nullptr;
		}	
		_grid[vidx]->clear();
	}
	_vgridHdr.clear();
	
	saveMF();
	saveGroupInfo();

	CWnd::OnDestroy();
}

BOOL CMapWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	bool	toggle{};
	CString	string;

	switch (LOWORD(wParam))
	{
	case IDC_HAWKEYE:
		//toggle = m_hawk.IsChecked();
		//hawkeyeAction(toggle);
		break;

	case IDC_EXPECT:
		toggle = m_expect.IsChecked();
		toggleAction(toggle);
		break;

	case IDC_CONFIG:
//		string.Format("%s /S /p5 /d %s\t1", configMAP, "registerCODE"); 
		//2012.01.09 KSJ 종목코드/종목명 앞에 스페이스바가 들어감.
		//string.Format("%s /t /p5 /S /d %s%c%s%c", "IB202201", "appendCODE", P_TAB , " " , P_NEW);
		string.Format(_T("%s /t /p5 /S /d %s%c%s%c"), _T("IB202201"), _T("appendCODE"), P_TAB , _T(" ") , P_NEW);
		//KSJ
		openView(typePOPUP, string);
		break;

	case IDC_PLUS:
	case IDC_MINUS:
		plusminusClick(LOWORD(wParam));
		SendMinMaxInfo();
		break;
	}
	
	return CWnd::OnCommand(wParam, lParam);
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (_grid[0] && IsWindow(_grid[0]->GetSafeHwnd()))
	{
 		CRect rect, grect, clientRC; GetClientRect(&clientRC);	

		rect = grect = clientRC; 
		rect.top += gap5; grect.top += offsetGH ;

		for (int vidx = 0; vidx < maxVIEW; vidx++)
		{
			rect.left = clientRC.left + (m_gridW*vidx); 
			rect.right = rect.left + comboW; 
			rect.bottom = rect.top + comboH;

			if (vidx == 0) rect.left += gap3+gap2;
			if (vidx == (m_viewType - 1))
				rect.right-= 13;
			
			m_internm[vidx].MoveWindow(&rect);

			rect.left = rect.right + gap2; 
			rect.right = rect.left + size18; 
			rect.bottom = rect.top + size18;

			m_category[vidx]->MoveWindow(&rect);
			_grid[vidx]->SetWindowPos(NULL, m_gridW * vidx + gap2 * 2, offsetGH + gap3 * 3, m_gridW - gap2, grect.Height() - gap2 * 7, SWP_NOZORDER /*SWP_HIDEWINDOW*/);
		}		

		rect = clientRC; rect.top += gap5; rect.bottom = rect.top + size18;

		const int xpos = (m_gridW*m_viewType) - ((size18 + gap1 + size44 + gap1) * 2);

		rect.left = xpos;              rect.right = rect.left + size44;
		rect.OffsetRect(-size44, 0);						
		CRect rc = rect;
		rc.right -= 16;
		rc.OffsetRect(15, 0);
		_cxMarket->MoveWindow(&rc);
		rect.OffsetRect(size44, 0);
		rect.left = xpos;              rect.right = rect.left + size44;		m_expect.MoveWindow(&rect);
		rect.left = rect.right + gap1; rect.right = rect.left + size44;		m_config.MoveWindow(&rect);
		rect.left = rect.right + gap1; rect.right = rect.left + size18;		m_minus.MoveWindow(&rect);
		rect.left = rect.right + gap1; rect.right = rect.left + size18;		m_plus.MoveWindow(&rect);
	}

}

std::pair<bool, CString> CMapWnd::getSymbol(int igrid, UINT row)
{
	CString code;
	bool bflag{};
	code.Empty();


	if (igrid >= m_viewType)
		return std::make_pair(std::move(bflag), std::move(code));

	const auto& data = _grid[igrid]->getDataArray();
	if (data->size() <= row)
		return std::make_pair(std::move(bflag), std::move(code));

	const auto& hidden = data->at(row).second;
	if (auto& ft = hidden.find(_T("1301")); ft != hidden.end())
		code = hidden.at("1301");

	code.Trim(); bflag = true;
	return std::make_pair(std::move(bflag), std::move(code));
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	CString code, string;
	NM_USER* puser = reinterpret_cast<NM_USER*>(lParam);
	const int vidx = int(wParam) - idGRID1;
	if (vidx > m_viewType)
		return CWnd::OnNotify(wParam, lParam, pResult);

	// 윈도우 메세지  필터...
	// 쓰지 않는건 원래대로 돌려버림.
	switch (puser->hdr.code)
	{
	case GVNM_LMOUSEDOWN:
	case NM_CLICK:
	case NM_DBLCLK:
	case NM_RCLICK:
		break;
	case LVN_COLUMNCLICK:
		_alertkey.clear();	//  리얼타임 정렬시 초기화 중요...  
	default:
		return CWnd::OnNotify(wParam, lParam, pResult);	

	}
	//////////////////////////////

	auto& item = getSymbol(vidx,  puser->item.row);
	if (!item.first)
		return CWnd::OnNotify(wParam, lParam, pResult);
	code = item.second;

	switch (puser->hdr.code)
	{
	case LVN_COLUMNCLICK:		
		break;	
	case GVNM_LMOUSEDOWN:
		m_nOldRow = puser->item.row;
		break;
	case NM_CLICK:
		if (puser->item.row > -1)
		{
			CString strTrigger(_T(""));
			if(code.GetLength() != 6)
				break;
			
			strTrigger.Format("1301\t%s", code);
			Variant(triggerCC, strTrigger);
		}

		m_nOldRow = -1;
		break;

	case GVNM_COLUMNMOVE:
		{
			//if (nmgv->row >= m_gridHdrX.GetSize()
			//		|| nmgv->col >= m_gridHdrX.GetSize())
			//	break;

			//_gridHdr gridHdr;

			//gridHdr = m_gridHdrX.GetAt(nmgv->col);
			//m_gridHdrX.InsertAt(nmgv->row, gridHdr);

			//if (nmgv->col > nmgv->row)
			//	nmgv->col += 1;

			//m_gridHdrX.RemoveAt(nmgv->col);
		}
		break;

	case NM_DBLCLK:
		{
			if ((puser->item.row >=  0) && (puser->item.col == colNAME || puser->item.col == colCURR))
			{
				code.TrimLeft(); code.TrimRight();
				if (code.IsEmpty()) break;

				switch (getCodeType(code))
				{
				case kospiCODE:
					string.Format(_T("%s /S/t0/d%s\t%s"), kcurrMAP, HCOD, code);
					openView(typeVIEW, string); break;
				case futureCODE:
					string.Format(_T("%s /S/t0/d%s\t%s"), fcurrMAP, FCOD, code);
					openView(typeVIEW, string); break;
				case optionCODE:
					string.Format(_T("%s /S/t0/d%s\t%s"), ocurrMAP, OCOD, code);
					openView(typeVIEW, string); break;
				case foptionCODE:
					string.Format(_T("%s /S/t0/d%s\t%s"), focurrMAP,PCOD, code);
					openView(typeVIEW, string); break;
				case indexCODE:
					string.Format(_T("%s /S/t0/d%s\t%s"), icurrMAP, UCOD, code);
					openView(typeVIEW, string); break;
				case sfCODE:
					string.Format(_T("%s /S/t0/d%s\t%s"), sfcurrMAP, SFCOD, code);
					openView(typeVIEW, string); break;
				}
			}
		}
		break;
	case NM_RCLICK:
		RbuttonAction(_grid[vidx].get(), code);
		break;
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		return _bkBrush->operator HBRUSH();
	default:
		break;
	}
	return hbr;
}


void CMapWnd::ChangeInterNM(int vidx)
{
	const int index = m_internm[vidx].GetCurSel();
	if (index != CB_ERR)
	{
		m_isTimer[vidx] = false;
		const int gno = m_internm[vidx].GetItemData(index);
		loadInterest(vidx, gno);
//		sendTransaction(vidx);
		_grid[vidx]->SetFocus();
	}
}

void CMapWnd::OnSelchangeInterNM0()
{
	ChangeInterNM(0);
}

void CMapWnd::OnSelchangeInterNM1()
{
	ChangeInterNM(1);
}

void CMapWnd::OnSelchangeInterNM2()
{
	ChangeInterNM(2);
}

void CMapWnd::OnSelchangeInterNM3()
{
	ChangeInterNM(3);
}

void CMapWnd::OnSelchangeInterNM4()
{
	ChangeInterNM(4);
}

void CMapWnd::OnSelchangeInterNM5()
{
	ChangeInterNM(5);
}

void CMapWnd::OnSelchangeInterNM6()
{
	ChangeInterNM(6);
}


void CMapWnd::plusminusClick(UINT id)
{
	if (id == IDC_PLUS)
	{		
		switch (m_viewType)
		{
		case 7:
			return;
		default:
			m_viewType += 1;
			break;
		}
	}
	else if (id == IDC_MINUS)
	{
		switch (m_viewType)
		{
		case 2:	return;
		default:
			m_viewType -= 1;
			break;		
		}
	}

	loadFieldInfo(); 
	initialGrid(); 
	repositionView();
	const bool toggle = m_expect.IsChecked();

	if (toggle) 
		toggleAction(toggle);

	for (int vidx = 0; vidx < (int) m_viewType; vidx++)
	{
		const int cursel = m_internm[vidx].GetCurSel();
		if (cursel != CB_ERR)
		{
			const int gno = m_internm[vidx].GetItemData(cursel);
			loadInterest(vidx, gno); 
		}
	}
}

LONG CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	if (!GetSafeHwnd())
		return 0;

	int	vidx = 0;
	const int key = HIBYTE(LOWORD(wParam));


	if (wParam == 99999999) {
		loadInterest(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}

	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:

		if (HIBYTE(LOWORD(wParam)) == TRKEY_GROUP ) {
			CString data = (char*)lParam;
			receiveOub(HIBYTE(LOWORD(wParam)), data);
		}
		else if ((key >= TRKEY_INTER) && (key < TRKEY_INTER + maxVIEW)) {
			CString data = (char*)lParam;
			vidx = key - TRKEY_INTER;
			receiveOub(TRKEY_INTER, data, vidx);
		}
		else {

			vidx = HIBYTE(LOWORD(wParam));
			if (vidx >= categoryKEY && vidx < categoryKEY + maxVIEW)
			{
				vidx -= categoryKEY;
				if (m_category[vidx] != NULL)
					m_category[vidx]->SendMessage(WM_USER + 99, wParam, lParam);
				return 0;
			}
			
			if (vidx < maxVIEW) {
				const UINT gridHoL = sizeof(struct _gridHo);
				const UINT datL = HIWORD(wParam);// -gridHoL;
				if (datL <= 0) return  0;
				CString sData = AxStd::FORMAT(_T("%s"), CString((char*)lParam, datL)); 
				sData.Trim();
				sData = sData.Mid(gridHoL, sData.GetLength() - gridHoL);

				std::future future = AxStd::async([&](){
					return parsingOubs(sData, vidx);
				});
				
				UINT size = future.get();
				_grid[vidx]->SetItemCountEx(size);
				_grid[vidx]->Invalidate();
				m_waiting[vidx] = false;
			}
			break;
		}
		break;
	case DLL_ALERTx:
		{
			AxStd::async([this, lParam]() {
				parsingAlertx(lParam);
			});
		}	
		break;
	case DLL_SETPAL:
		reloadscreen();
		break;
	case DLL_SETFONT:
		{
			const int	fontsize = HIWORD(wParam);
			CString fname = CString((char*)lParam);
			reloadscreen(fname, fontsize);
		}
		break;
	case DLL_TRIGGER:
		parsingTrigger((char *)lParam);
		break;
	case DLL_DOMINO:
		break;
	}

	return 0;
}

LONG CMapWnd::OnParent(WPARAM wParam, LPARAM lParam)
{
	int	vidx=0;
	switch (LOWORD(wParam))
	{
	case MK_MARKETEVENT:
		if (lParam != 0)
		{
			_market = (int)lParam;
			for (int vidx = 0; vidx < (int)m_viewType; vidx++)
				sendTransaction(vidx);
		}
		break;
	case cpCATEGROY:
		vidx = HIWORD(wParam) - categoryKEY;

		if (vidx < maxVIEW) 
		{
			if (loadInterest(vidx, m_category[vidx]->category_GetJCodes()) <= 0)
				break;
			sendTransaction(vidx);
		}
		break;
	default:
		break;
	}

	return 0;
}

#define gNXT    _T("90777")

void CMapWnd::sendTransaction(int vidx)
{
	int	sendL = 0;
	char	sendB[2048]{};
	char	tempB[64]{};

	if (m_waiting[vidx])
	{
		Variant(guideCC, idTOstring(IDS_GUIDE1));
		return;
	}

	m_waiting[vidx] = true;

	FillMemory(sendB, sizeof(sendB), ' ');

	if(m_expect.IsChecked())
	{
		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
	}
	else
	{
		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 0, P_TAB);
	}

	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);


	//중요..  여기서 NXT시장 구분값을 날려줌...
	sprintf(tempB, "%s%c%d%c", gNXT, P_DELI, _market, P_TAB);	// 시장구분자   KRX :1  NXT :2  TOTAL : 3      NXT / KRX : 4 
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);


	sprintf(tempB, "%s%c", gSYMBOL, gnCHAR);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct	_gridHi* gridHi;

	gridHi = (struct _gridHi *) &sendB[sendL];
	sendL += sizeof(struct _gridHi);

	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", m_inters[vidx].GetSize());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir  = '1';
	gridHi->sort = '0';

	//struct	_gridHdr gridHdr {};
	//for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	//{
	//	gridHdr = m_gridHdrX.GetAt(ii);

	//	sprintf(tempB, "%s%c", gridHdr.symbol, lfCHAR);
	//	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	//	sendL += strlen(tempB);
	//}

	for_each(_vgridHdr.begin(), _vgridHdr.end(), [&](const auto& item) {
		sprintf(tempB, _T("%s%c"), item.symbol.GetString(), lfCHAR);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	});


	sprintf(tempB, "%s%c", "1301", gnCHAR);		// 종목코드
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct _inters* pinters=nullptr;
	CString temp_code;

	for (int ii = 0; ii < m_inters[vidx].GetSize(); ii++)
	{
		pinters = m_inters[vidx].GetAt(ii).get();
	
		//code에서 종목명 떼어내기
		temp_code = pinters->code;
		strcpy(pinters->code,temp_code.Left(6));

		sprintf(tempB, "%s%c", pinters->code, gnCHAR);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}
	sendB[sendL] = lfCHAR;  sendL += 1;
	sendB[sendL] = tabCHAR; sendL += 1;
	sendB[sendL] = 0x00; 
	sendTR("pooppoop", sendB, sendL, vidx);
}

UINT CMapWnd::parsingOubs(CString& sData, int vidx)
{
	std::vector<CString> vRow;
	std::vector<CString> vCol;
	
	AxStd::SplitSub(vRow, sData, lfCHAR);

	UINT col = 0;
			_alertkey.clear();
		        _grid[vidx]->clear();
	auto* data    = _grid[vidx]->getDataArray();
	auto* codemap = _grid[vidx]->getCodemap();

	for_each(vRow.begin(), vRow.end(), [&](CString& sRow) {	
		vCol.clear();
		AxStd::SplitSub(vCol, sRow, tabCHAR);
		col = 0;		
		auto& dataitem = data->emplace_back(std::vector<CString>(), std::map<CString, CString>());
		for_each(vCol.begin(), vCol.end(), [&](CString& sCol) {
			if (col < _vgridHdr.size())
			{
				sCol.Trim();
				auto& griditem = _vgridHdr.at(col++);
				auto& vData    = dataitem.first;
				auto& mHidden  = dataitem.second;

				if (griditem.attr == GVAT_HIDDEN)
				{
					CString sSymbol(griditem.symbol, 8);
					sSymbol.Trim();
					mHidden.emplace(sSymbol, sCol);

					// _grid->codeSymbol 셋팅...  resetsymbol 할필요 없음.
					if (sSymbol.CompareNoCase(_T("1021")) == 0)
					{
						mHidden.emplace(_T("order"), AxStd::FORMAT(_T("%05d"), data->size() - 1));
						if (!sCol.IsEmpty())
							codemap->emplace(sCol, data->size() - 1);
					}
				}
				else
					vData.emplace_back(sCol);
			}
		});
	});

	return data->size();
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

void CMapWnd::parsingAlertx(LPARAM lParam)
{
	struct _alertR* alertR = (struct _alertR*)lParam;
	CString code = alertR->code;
	DWORD* data = (DWORD*)alertR->ptr[0];
	char cGubn = (char)data[0]; 	

	if (!(alertR->stat & alert_SCR))
		return;
	if(code.CompareNoCase(_T("S0000")) == 0)		
		return;		//2012.09.11 KSJ 뉴스일때는 아래쪽을 탈필요 없다.

	//2013.08.26 KSJ	지수일때 예상가 표시
	/*
		08:30~09:00 'X' 예상가표시
		09:01~14:50 'J' 현재가표시
		14:50~15:00 'X' 예상가표시
		15:00~15:01 'J' 현재가표시
	*/
	
	bool bgisu = false;
	bool bExcept = m_expect.IsChecked();
	if(bExcept && !(cGubn == 'X') && code.GetLength() == 5)		// 지수일때
	{
		if (code[0] == 'X')
		{
			code.Replace('X', 'K');
			cGubn = 'X';
			bgisu = true;
		}
	}
	else if (bExcept != true && cGubn == 'X')
		return;
	
	std::multimap<int, int> mapRow;
	if (!searchCode(mapRow, code))
		return; //false;

	data_Alertx(mapRow, data, bgisu);	

	std::for_each(mapRow.begin(), mapRow.end(), [&](const auto& item) {
		if (item.first < m_viewType)
			_grid[item.first]->PostMessage(gridmessage, grid_onedraw, 0);
	});

}

bool CMapWnd::data_Alertx(const std::multimap<int, int>& mapRow, DWORD* data, bool bgisu)
{
	// mapRow가 비어있으면 바로 반환
	if (mapRow.empty())
		return false;

	const auto& info = mapRow.begin();
	const int gridindex = info->first;
	const int gridrow = info->second;
	if (gridindex >= m_viewType)
		return false;

	const auto& header   = _grid[gridindex]->getHeaderArr();
	const auto& gridData = _grid[gridindex]->getDataArray();
	const int size = header->size();
	auto& rowItem = gridData->at(gridrow).first;


	std::mutex xx;
	std::vector<int> vv;

	int col = 1;
	for (auto it = ++header->begin(); it != header->end(); ++it, ++col) {
		CString sSym = AxStd::FORMAT(_T("%s"), std::get<headerinfo_symbol>(*it));
		if (sSym.GetLength() >= 3)
			sSym = sSym.Mid(1, 4);

		if (bgisu) {
			switch (atoi(sSym)) {
			case 111: sSym = "023"; break;
			case 115: sSym = "024"; break;
			case 116: sSym = "033"; break;
			case 112: sSym = "027"; break;
			default: break;
			}
		}

		int idx = atoi(sSym);
                if (idx < 0) continue; // 잘못된 인덱스 방지

		const CString sData = (char*)data[idx];
		if (!sData.IsEmpty() && size > col) {
			CString& sItem = rowItem.at(col);
			if (sItem.CompareNoCase(sData)) 
			{
				sItem = sData;
				vv.push_back(col);
			}
		}
	}

	if (!vv.empty())
		_grid[gridindex]->drawitem(gridrow, vv); // timer Draw

	// 여러 grid에 대해 동기화 및 draw 처리
	if (mapRow.size() > 1) {
		for (auto it = ++mapRow.begin(); it != mapRow.end(); ++it) {
			const int igrid = it->first;
			const int row   = it->second;
			if (igrid >= m_viewType)
				continue;

			const auto& gridxx = _grid[igrid]->getDataArray();
			auto& vItem = gridxx->at(row).first;

			vItem = rowItem; // 전체 row 동기화
			if (!vv.empty())
				_grid[igrid]->drawitem(row, vv);	        // timer Draw
		}
	}
	return true;
}

bool CMapWnd::searchCode(std::multimap<int, int>& mRow, CString& code)
{
	bool bflag = false;
	mRow.clear();

	if  (auto& ft = _alertkey.find(code); ft != _alertkey.end())
	{
		mRow = ft->second;
		return true;
	}
	else
	{
		for(int ii = 0; ii < m_viewType; ii++) 
		{
			const auto& codemap = _grid[ii]->getCodemap();	
			for (auto it = codemap->find(code);it != codemap->end(); it++) //중복처리
			{
				if (code.CompareNoCase(it->first) != 0)
					break;

				bflag = true;
				mRow.emplace(ii, it->second);
			}
		}
	}
	
	if (mRow.size() >  0)
		_alertkey.emplace(code, mRow);		
	
	return bflag;
}

// KSJ
void CMapWnd::parsingTrigger(CString datB)
{
	CString	symbol;

	symbol = parseX(datB, "\t");

	if (symbol == _T("OnPortfolio") && datB == _T("ok"))
	{
		_alertkey.clear();
		int		idxCBO[maxVIEW]{};
		int		gnoCBO[maxVIEW]{};
		char	namCBO[maxVIEW][128]{};

		CString	filePath; 
		filePath.Format(_T("%s/%s/%s/%s"), m_root, USRDIR, m_name, portFILE);

		// save combo index
		for (int vidx = 0; vidx < maxVIEW; vidx++)
		{
			idxCBO[vidx]  = m_internm[vidx].GetCurSel();
			gnoCBO[vidx]  = m_internm[vidx].GetItemData(idxCBO[vidx]);
			m_internm[vidx].GetLBText(idxCBO[vidx], namCBO[vidx]);
		}
	
		const int count = loadInterName(false);

		if (count > 0)
		{
			int gno{}; 
			CString text;

			for (int vidx = 0; vidx < maxVIEW; vidx++)
			{
				if (m_internm[vidx].GetCurSel() == -1)
				{
					if (m_internm[vidx].GetCount() > 0)
						m_internm[vidx].SetCurSel(0);
					else
						_grid[vidx]->clear();
				}

				if (idxCBO[vidx] < count)
				{
					gno = m_internm[vidx].GetItemData(idxCBO[vidx]);
//					m_internm[vidx].GetLBText(idxCBO[vidx], text);
					m_internm[vidx].GetWindowText(text);

					if (text.IsEmpty())
						continue;

					if (gno == gnoCBO[vidx] && !text.Compare(namCBO[vidx]))
					{
						m_internm[vidx].SetCurSel(idxCBO[vidx]);
						continue;
					}
				}

				if (vidx < count)
					m_internm[vidx].SetCurSel(vidx);
				else if (count >= 1)
					m_internm[vidx].SetCurSel(0);
			}

			for (int vidx = 0; vidx < maxVIEW; vidx++)
			{
				const int cursel = m_internm[vidx].GetCurSel();
				if (cursel != CB_ERR)
				{
					gno = m_internm[vidx].GetItemData(cursel);
					loadInterest(vidx, gno); 
					//sendTransaction(vidx);
				}
			}
		}
		else
		{
			for (int vidx = 0; vidx < maxVIEW; vidx++)
				_grid[vidx]->clear();
		}
	}
	else if (symbol.CompareNoCase(_T("edMarketTrigger")) == 0)
	{
		//m_pToolWnd->marketTrigger(datB);
		_variant_t var(datB);

		if (_cxMarket->GetSafeHwnd())
			_cxMarket->SetCtrlProperty("sMarket", var);

		if (datB == "KRX")
		{
			_market =  1;
		}
		else if (datB == "NXT")
		{
			_market =  2;
		}
		else
		{
			_market =  3;
		}
		for (int vidx = 0; vidx < (int)m_viewType; vidx++)
			sendTransaction(vidx);
	}
}

void CMapWnd::initControl()
{
	int	xpos=0;
	CRect	rect, clientRC, rc; 
	GetClientRect(clientRC);

	CString	imgPath; 
	imgPath.Format("%s/%s/", m_root, IMAGEDIR);

	m_bitmapH_nm = getAxBitmap(imgPath + "H버튼.bmp")->operator HBITMAP();
	m_bitmapH_dn = getAxBitmap(imgPath + "H버튼_dn.bmp")->operator HBITMAP();
	m_bitmapH_hv = getAxBitmap(imgPath + "H버튼_en.bmp")->operator HBITMAP();

	rect.top = clientRC.top + gap2;

	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		rect.left = clientRC.left + (m_gridW*vidx); 
		rect.right = rect.left + comboW; 
		rect.bottom = rect.top + 200;

		if (vidx == 0) rect.left += gap2;

		m_internm[vidx].Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST, rect, this, IDC_INTERNM0 + vidx);
		m_internm[vidx].SetFont(m_font, false); 
		rect.bottom = rect.top + comboH; 
		m_internm[vidx].MoveWindow(&rect);

		rect.left = rect.right + gap2; 
		rect.right = rect.left + size18; 
		rect.bottom = rect.top + size18;

		m_category[vidx] = std::make_unique<CCategory>(m_parent, categoryKEY+vidx);
		m_category[vidx]->CreateCategory(this, rect, m_root);

		if (vidx == 0)
			rc = rect;
	}

	rc.OffsetRect(rc.Width() + 2, 3);
	_cxMarket = std::make_unique<CContainerWnd>();
	_cxMarket->SetParent(m_parent);
	_cxMarket->Create(NULL, NULL, WS_CHILDWINDOW | WS_VISIBLE, rc, this, -1);
	_cxMarket->CreateControl(m_root, "CX_MarketPicker", "xxInter", CRect(0,0,0,0), "/tIB200200/k2");

	CString market = _cxMarket->GetCtrlProperty("sMarket");
	if (market == "KRX")
		_market = 1;
	else if (market == "NXT")
		_market = 2;
	else
		_market = 3;



	m_bitmap2_nm = getAxBitmap(imgPath + "2btn.bmp")->operator HBITMAP();
	m_bitmap2_dn = getAxBitmap(imgPath + "2btn_dn.bmp")->operator HBITMAP();
	m_bitmap2_hv = getAxBitmap(imgPath + "2btn_en.bmp")->operator HBITMAP();

	m_bitmapM_nm = getAxBitmap(imgPath + "minusBTN.bmp")->operator HBITMAP();
	m_bitmapM_dn = getAxBitmap(imgPath + "minusBTN_dn.bmp")->operator HBITMAP();
	m_bitmapM_hv = getAxBitmap(imgPath + "minusBTN_en.bmp")->operator HBITMAP();

	m_bitmapP_nm = getAxBitmap(imgPath + "plusBTN.bmp")->operator HBITMAP();
	m_bitmapP_dn = getAxBitmap(imgPath + "plusBTN_dn.bmp")->operator HBITMAP();
	m_bitmapP_hv = getAxBitmap(imgPath + "plusBTN_en.bmp")->operator HBITMAP();


	rect.SetRect(0, 0, 0, 0);
	m_expect.Create(idTOstring(IDS_EXPECT), rect, this, IDC_EXPECT, true);
	m_expect.SetFont(m_font, false);  
	m_expect.SetImgBitmap(m_bitmap2_nm, m_bitmap2_dn, m_bitmap2_hv);

	m_config.Create(idTOstring(IDS_CONFIG), rect, this, IDC_CONFIG);

	m_config.SetFont(m_font, false); 
	m_config.SetImgBitmap(m_bitmap2_nm,m_bitmap2_dn, m_bitmap2_hv);

	m_minus.Create("", rect, this, IDC_MINUS);
	m_minus.SetFont(m_font, false);  
	m_minus.SetImgBitmap(m_bitmapM_nm, m_bitmapM_dn, m_bitmapM_hv);
	
	m_plus.Create("", rect, this, IDC_PLUS);
	m_plus.SetFont(m_font, false);  
	m_plus.SetImgBitmap(m_bitmapP_nm, m_bitmapP_dn, m_bitmapP_hv);

	rect.top = clientRC.top + gap2;
	rect.bottom = rect.top + size18;

	xpos = (m_gridW*m_viewType) - ((size18 + gap1 + size44 + gap1) * 2);

	rect.left = xpos;              
	rect.right = rect.left + size44;		
	m_expect.MoveWindow(&rect);

	rect.left = rect.right + gap1; 
	rect.right = rect.left + size44;		
	m_config.MoveWindow(&rect);

	rect.left = rect.right + gap1; 
	rect.right = rect.left + size18;		
	m_minus.MoveWindow(&rect);

	rect.left = rect.right + gap1; 
	rect.right = rect.left + size18;		
	m_plus.MoveWindow(&rect);
}

void CMapWnd::initialGrid(int size)
{
	LOGFONT lf{}, lfB{};
	m_font->GetLogFont(&lf); lf.lfHeight = m_fontsize * 10;
	m_fontB->GetLogFont(&lfB); lfB.lfHeight = m_fontsize * 10;

	for (int vidx = 0; vidx < maxVIEW; vidx++)
	{
		while (_grid[vidx]->DeleteColumn(0)) {};
		int col =  0;
		auto* harr = _grid[vidx]->getHeaderArr();
		harr->clear();
		for_each(_vgridHdr.begin(), _vgridHdr.end(), [&](auto& item){		
			if (item.attr == GVAT_HIDDEN)
				return;		

			harr->emplace_back(idTOstring(item.stid), item.width, item.attr, item.format, item.attrx, true, CString(item.symbol, 8).Trim());
			_grid[vidx]->InsertColumn(col++, idTOstring(item.stid), DT_CENTER|DT_VCENTER, item.width);		
		});
		_grid[vidx]->SetBKSelColor(GetColor(78));
		_grid[vidx]->SetConditionColor(GetColor(94), GetColor(95));
		_grid[vidx]->SetStepColor(1, GetColor(68), GetColor(77));
		_grid[vidx]->SetHeaderColor(GetColor(76), GetColor(74));
	}

	if (size)
	{
		int colW{}, rowH{};
	 	CRect rc;
		_grid[0]->GetClientRect(rc);
		colW = m_gridW;
		rowH = rc.Height();

		colW = colW * m_viewType;
		m_parent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(colW-m_viewType, rowH+offsetGH+1));
		writeVtCode(colW, rowH+offsetGH+1);
	}
}

int CMapWnd::loadInterName(bool bcursel)
{
	Request_GroupList();
	return 0;
}

void CMapWnd::loadFieldInfo()
{
	const struct _gridHdr gridHdr[] = {
		{IDS_GH_RCOD,	0,	GVAT_HIDDEN,		"1021",	GVFM_LEFT,	0,				9},	// 0
		{IDS_GH_CODE,	0,	GVAT_HIDDEN,		"1301",	GVFM_LEFT,	0,				9},	// 1
		{IDS_GH_OPEN,	0,	GVAT_HIDDEN,		"2029",	GVFM_RIGHT,	0,				9},	// 2
		{IDS_GH_HIGH,	0,	GVAT_HIDDEN,		"2030",	GVFM_RIGHT,	0,				9},	// 3
		{IDS_GH_LOW,	0,	GVAT_HIDDEN,		"2031",	GVFM_RIGHT,	0,				9},	// 4
		{IDS_GH_PRECURR,0,	GVAT_HIDDEN,		"2934",	GVFM_RIGHT,	0,				9},	// 5
		{IDS_GH_PREOPEN,0,	GVAT_HIDDEN,		"2931",	GVFM_RIGHT,	0,				9},	// 6
		{IDS_GH_PREHIGH,0,	GVAT_HIDDEN,		"2932",	GVFM_RIGHT,	0,				9},	// 7
		{IDS_GH_PRELOW,	0,	GVAT_HIDDEN,		"2933",	GVFM_RIGHT,	0,				9},	// 8
		{IDS_GH_NAME,	70,	GVAT_SORTSTR,		"1022",	GVFM_LEFT,	0,				9},	// 9
		{IDS_GH_CURR,	60,	GVAT_SORTABS,		"2023",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	7},	// 10
		{IDS_GH_DIFF,	60,	GVAT_SORTVAL,		"2024",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONTRAST,	7},	// 11
		{IDS_GH_RATE,	46,	GVAT_SORTVAL,		"2033",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITION,	7},	// 12
		{IDS_GH_VOL,	75,	GVAT_SORTABS,		"2027",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	3},	// 13
		{IDS_GH_ASK,	60,	GVAT_SORTABS,		"2025",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	2},	// 14
		{IDS_GH_BID,	60,	GVAT_SORTABS,		"2026",	GVFM_RIGHT,	GVAT_COMMA|GVAT_CONDITIONx,	2},	// 15
	};

	_vgridHdr.clear();
	const int gridHdrC = sizeof(gridHdr) / sizeof(_gridHdr);

	m_gridW = 0;
	for (int ii = 0; ii < gridHdrC; ii++)
	{
		if (gridHdr[ii].fidx >= m_viewType)
		{
			_vgridHdr.push_back(gridHdr[ii]);
			if (!(gridHdr[ii].attr & GVAT_HIDDEN))
				m_gridW += gridHdr[ii].width;
		}
	}

	if (m_gridW > 0) m_gridW += scrollW;
}

void CMapWnd::toggleAction(bool toggle)
{
	struct	_symbol {
		UINT	stid1;
		char*	symb1;
		UINT	stid2;
		char*	symb2;
	};
	
	const struct _symbol chksym [] = {
		{ IDS_GH_CURR,	"2023",	IDS_GH_ANTIPRC,	"2111" },
		{ IDS_GH_DIFF,	"2024",	IDS_GH_DIFF,	"2115" },
		{ IDS_GH_RATE,	"2033",	IDS_GH_RATE,	"2116" },
		{ IDS_GH_VOL,	"2027",	IDS_GH_ANTIVOL,	"2112" },
	};

// 		{ IDS_GH_CURR,	"2023",	IDS_GH_ANTIPRC,	"2111" },
// 		{ IDS_GH_DIFF,	"2024",	IDS_GH_DIFF,	"2113" },
// 		{ IDS_GH_RATE,	"2033",	IDS_GH_RATE,	"2114" },
// 		{ IDS_GH_VOL,	"2027",	IDS_GH_ANTIVOL,	"2112" },

	const	int chksymC = sizeof(chksym) / sizeof(_symbol);
	CString strCode;

		
	if (toggle)		// 예상체결가 형태로 변경
	{
		for (int ii = 0; ii < chksymC; ii++)
		{
			auto& item = std::find_if(_vgridHdr.begin(), _vgridHdr.end(), [&](const auto& header){
				return  !header.symbol.CompareNoCase(chksym[ii].symb1);
			});

			if (item != _vgridHdr.end())
			{
				item->stid    =  chksym[ii].stid2;
				item->symbol  =  chksym[ii].symb2;
			}
		}
	}
	else
	{
		for (int ii = 0; ii < chksymC; ii++)
		{
			auto& item = std::find_if(_vgridHdr.begin(), _vgridHdr.end(), [&](const auto& header) {
				return !header.symbol.CompareNoCase(chksym[ii].symb2);
			});

			if (item != _vgridHdr.end())
			{
				item->stid = chksym[ii].stid1;
				item->symbol = chksym[ii].symb1;
			}
		}
	}

	initialGrid(false);

	for (int vidx = 0; vidx < (int)m_viewType; vidx++)
		sendTransaction(vidx);
}

int CMapWnd::loadInterest(int vidx, int gno)
{
	clearInterest(vidx);
	Request_GroupCode(gno, vidx);
	return 0;
}

int CMapWnd::loadInterest2(int vidx, int gno)
{
	return m_inters[vidx].GetSize();
}

int CMapWnd::loadInterest(int vidx, CString codes)
{
	if (codes.IsEmpty()) return 0;

	clearInterest(vidx);

	CString	code, name;
	//struct	_inters* pinters = nullptr;
	std::shared_ptr<struct	_inters> pinters = nullptr;

	for (int ii = 0; ii < maxJONGMOK; ii++)
	{
		code = parseX(codes, tabCHAR);
		if (code.IsEmpty()) continue;

		if (code.GetLength() == 6 || code.GetLength() == 8)
		{
			char wB[64]; strcpy(wB, code);
			char* xxP = (char *)m_parent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)wB);
			name = xxP; 
			name.TrimLeft(); 
			name.TrimRight();
		}

		if (name.IsEmpty()) continue;

//		pinters = (_inters*) new char[sz_inters];
		pinters = std::make_shared<_inters>();

		FillMemory(pinters.get(), sz_inters, ' ');

		pinters->gubn[0] = '0';
		strcpy(pinters.get()->code, code);
		strcpy(pinters.get()->name, name);

		m_inters[vidx].Add(pinters);
	}

	return m_inters[vidx].GetSize();
}

void CMapWnd::saveInterest(int vidx, int gno)
{
	CString	filePath;
	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	struct	_inters* pinters=nullptr;

	CFile	fileH(filePath, CFile::modeWrite|CFile::modeCreate);

	for (int ii = 0; ii < m_inters[vidx].GetSize(); ii++)
	{
		pinters = m_inters[vidx].GetAt(ii).get();
		fileH.Write(pinters, sz_inters);
	}
	fileH.Close();
}

void CMapWnd::clearInterest(int vidx)
{
	//if (m_hawk.IsChecked()) hawkeyeClear(vidx);

	for (int ii = 0; ii < m_inters[vidx].GetSize(); ii++)
		m_inters[vidx].GetAt(ii).reset();
	m_inters[vidx].RemoveAll();
}

bool CMapWnd::existFile(int gno)
{
	CString	filePath;
	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	DWORD	fLen = 0;
	CFileFind finder;
	if (!finder.FindFile(filePath))
		return false;

	CFile file(filePath, CFile::modeRead);
	fLen = (DWORD)file.GetLength(); 
	file.Close();

	if (fLen <= 0)
		return false;

	return true;
}

int CMapWnd::getCodeType(CString code)
{
	const int codeL = code.GetLength();

	if (codeL == 6)					// kospi code
	{
		return kospiCODE;
	}
	else if (codeL <= 3)				// upjong code
	{
		return indexCODE;
	}
	else if (code[0] == '1' || code[0] == '4')	// future code
	{
		if (code[1] == '0')
			return futureCODE;
		else
			return sfCODE;
	}
	else if (code[0] == '2' || code[0] == '3')
	{
		if (code[1] == '0')			// future option code
		{
			return optionCODE;
		}
		else					// 
		{
			return foptionCODE;
		}
	}

	return 0;
}

void CMapWnd::writeVtCode(int cx, int cy)
{
	CString	filePath; filePath.Format("%s/%s/%s", m_root, TABDIR, VTCODE);
	CString	keyName;  keyName = _T("IB200200");
	CString string;   string.Format(" %d %d Normal\t;관심종목전광판", cx, cy);

	WritePrivateProfileString("DLL Controls", keyName, string, filePath);
}


int CMapWnd::getFlashIndex()
{
	int	readL;
	char	readB[128];
	CString	filePath; filePath.Format("%s/%s/%s", m_root, TABDIR, PALETTE);

	readL = GetPrivateProfileString(GENERALSN, FLASHSN, "8", readB, sizeof(readB), filePath);
	return atoi(CString(readB, readL));
}

void CMapWnd::repositionView()
{
	for (int vidx = 2; vidx < maxVIEW; vidx++)
	{
		m_internm[vidx].ShowWindow(SW_SHOW);
		m_category[vidx]->ShowWindow(SW_SHOW);
		_grid[vidx]->ShowWindow(SW_SHOW);
	}
	for (int vidx = m_viewType; vidx < maxVIEW; vidx++)
	{
		m_internm[vidx].ShowWindow(SW_HIDE);
		m_category[vidx]->ShowWindow(SW_HIDE);
		_grid[vidx]->ShowWindow(SW_HIDE);
	}
}

void CMapWnd::reloadscreen(CString fname, int fsize)
{
	_bkBrush = getAxBrush(GetColor(64));
	m_blinkColor = GetColor(getFlashIndex());

	m_strPal = GetPalette();

	if (!fname.IsEmpty() || fsize != 0)
	{
		if (fname.IsEmpty()) fname = idTOstring(IDS_DEFAULTFONT);
		m_font = getAxFont(fname, fsize);
	}

	initialGrid(false);
	Invalidate();
	
//	if (m_hawk.IsChecked()) hawkeyeAction(true);

}

void CMapWnd::saveMF()
{
	CString	filePath; 
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, portFILE);

	CRect	rect;	GetClientRect(&rect);
	CString	string; 
	string.Format("%04d:%04d", rect.Width()+2, rect.Height()+2);
	WritePrivateProfileString("IB200200", "size", string, filePath);

	string.Format("%01d", (int)m_viewType);
	WritePrivateProfileString("IB200200", "viewtype", string, filePath);
}


void Wait(DWORD dwMillisecond) {
	MSG msg;
	ULONGLONG t0, t1, diff;

	t0 = GetTickCount64();
	while (TRUE) {
		t1 = GetTickCount64();

		if (t0 <= t1) diff = t1 - t0;
		else diff = 0xffffffff - t0 + t1;


		if (diff > dwMillisecond) break;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}
}

void CMapWnd::loadGroupInfo()
{
	CString	filePath, strIndex; 
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, portFILE);
	
	int gno{}, gIndex{};
	
	for(int ii=0 ; ii<maxVIEW ; ii++)
	{
		strIndex.Format("group%d", ii);	
		gIndex = (int)GetPrivateProfileInt("IB200200", strIndex, ii, filePath);
 
//		cursel = m_internm[ii].GetCurSel();
		m_internm[ii].SetCurSel(gIndex);

		if (gIndex != CB_ERR)
		{
			gno = m_internm[ii].GetItemData(gIndex);
//			loadInterest(ii, gno); 
//			Wait(20);

			PostMessage(WM_USER, 99999999, MAKELPARAM(ii, gno));

			//sendTransaction(ii);
		}
	}

}


void CMapWnd::saveGroupInfo()
{
	CString	filePath, strGno, strIndex; 
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, portFILE);

	int gNo[7]{};
	
	for(int ii=0 ; ii < maxVIEW ; ii++)
	{
		gNo[ii] = m_internm[ii].GetCurSel();

		strIndex.Format("group%d", ii);	
			
//		if (gNo[ii] != CB_ERR)
		{
			strGno.Format("%d", gNo[ii]);
			WritePrivateProfileString("IB200200", strIndex, strGno, filePath);
		}
// 		else
// 		{
// 			strGno.Format("0");
// 			WritePrivateProfileString("IB200200", strIndex, strGno, filePath);
// 		}
	}	
}


void CMapWnd::restoreMF()
{
	DWORD	wbL = 0; char	wbX[64];

	CString	filePath; filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, portFILE);

	wbL = GetPrivateProfileString("IB200200", "viewtype", "", wbX, sizeof(wbX), filePath);
	if (wbL > 0)
	{
		if (atoi(CString(wbX, wbL)) >= 2 && atoi(CString(wbX, wbL)) <= 7)
		{
			//switch (atoi(CString(wbX, wbL)))
			//{
			//case 2: m_viewType = viewTWO;	break;
			//case 3: m_viewType = viewTHREE;	break;
			//case 4: m_viewType = viewFOUR;	break;
			//}
				
			m_viewType = atoi(CString(wbX, wbL));
		}
	}

	wbL = GetPrivateProfileString("IB200200", "size", "", wbX, sizeof(wbX), filePath);
	if (wbL <= 0) return;

	CString	string, stringx;
	
	string  = CString(wbX, wbL);
	stringx = parseX(string, ":");

	const int width  = atoi(stringx);
	const int height = atoi(string);

	if (width <= 0 || height <= 0)
		return;

	m_parent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(width-2, height-2));
	return;
}
//
//void CMapWnd::hawkeyeAction(bool toggle)
//{
//	CString	code;
//
//	if (toggle)	// hawkeye color
//	{
//		for (int vidx = 0; vidx < maxVIEW; vidx++)
//		{
//			for (int ii = 1; ii < m_rowC; ii++)
//			{
//				code = m_grid[vidx]->GetItemText(ii, colCODE);
//				code.TrimLeft(); code.TrimRight();
//				if (!code.IsEmpty()) hawkEyeColor(vidx, ii);
//			}
//		}
//	}
//	else
//	{
//		for (int vidx = 0; vidx < maxVIEW; vidx++)
//		{
//			for (int ii = 1; ii < m_rowC; ii++)
//			{
//				code = m_grid[vidx]->GetItemText(ii, colCODE);
//				code.TrimLeft(); code.TrimRight();
//				if (!code.IsEmpty())
//				{
//					m_grid[vidx]->SetItemFgColor(ii, colNAME, GetColor(69));
//					m_grid[vidx]->SetItemBkColor(ii, colNAME, GetColor(68));
//					m_grid[vidx]->SetItemBkColor(ii, colCURR, GetColor(68));
//				}
//			}
//		}
//	}
//}

//void CMapWnd::hawkeyeAction(int vidx)
//{
//	CString	code;
//	for (int ii = 1; ii < m_rowC; ii++)
//	{
//		code = m_grid[vidx]->GetItemText(ii, colCODE);
//		code.TrimLeft(); code.TrimRight();
//		if (!code.IsEmpty()) hawkEyeColor(vidx, ii);
//	}
//}
//
//void CMapWnd::hawkeyeClear(int vidx)
//{
//	CString	code;
//	for (int ii = 1; ii < m_rowC; ii++)
//	{
//		code = m_grid[vidx]->GetItemText(ii, colCODE);
//		code.TrimLeft(); code.TrimRight();
//		if (!code.IsEmpty())
//		{
//			m_grid[vidx]->SetItemFgColor(ii, colNAME, GetColor(69));
//			m_grid[vidx]->SetItemBkColor(ii, colNAME, GetColor(68));
//			m_grid[vidx]->SetItemBkColor(ii, colCURR, GetColor(68));
//		}
//	}
//}

const int colorKKRED  = PALETTERGB(255,   0,   0);
const int colorDKRED  = PALETTERGB(255, 137, 155);
const int colorLTRED  = PALETTERGB(255, 201, 206);

const int colorKKBLUE = PALETTERGB(  0,   0, 255);
const int colorDKBLUE = PALETTERGB(126, 176, 255);
const int colorLTBLUE = PALETTERGB(200, 222, 255);
//
//void CMapWnd::hawkEyeColor(int vidx, int row)
//{
//	double	curr{}, open{}, high{}, low{};
//	double	pcurr{}, popen{}, phigh{}, plow{}, dval{};
//
//	curr  = TOfabs(m_grid[vidx]->GetItemText(row, colCURR));
//	open  = TOfabs(m_grid[vidx]->GetItemText(row, colOPEN));
//	high  = TOfabs(m_grid[vidx]->GetItemText(row, colHIGH));
//	low   = TOfabs(m_grid[vidx]->GetItemText(row, colLOW));
//
//	pcurr = TOfabs(m_grid[vidx]->GetItemText(row, colPCURR));
//	popen = TOfabs(m_grid[vidx]->GetItemText(row, colPOPEN));
//	phigh = TOfabs(m_grid[vidx]->GetItemText(row, colPHIGH));
//	plow  = TOfabs(m_grid[vidx]->GetItemText(row, colPLOW));
//	
//	if (curr <= 0) curr = pcurr;
//	if (curr <= 0) return;
//
//	// colNAME foreground
//	if (open >= phigh)	// 상승갭
//	{
//		m_grid[vidx]->SetItemFgColor(row, colNAME, colorKKRED);
//	}
//	else if (open <= plow)	// 하락갭
//	{
//		m_grid[vidx]->SetItemFgColor(row, colNAME, colorKKBLUE);
//	}
//	else
//	{
//		m_grid[vidx]->SetItemFgColor(row, colNAME, GetColor(69));
//	}
//
//	// colNAME background
//	if (curr >= high)
//	{
//		m_grid[vidx]->SetItemBkColor(row, colNAME, colorLTRED);
//	}
//	else if (curr <= low)
//	{
//		m_grid[vidx]->SetItemBkColor(row, colNAME, colorLTBLUE);
//	}
//	else
//	{
//		m_grid[vidx]->SetItemBkColor(row, colNAME, GetColor(68));
//	}
//
//	dval = (phigh - plow) / 3.;
//
//	// colCURR background
//	if (curr >= phigh)
//	{
//		m_grid[vidx]->SetItemBkColor(row, colCURR, colorDKRED);
//	}
//	else if (curr >= (plow + dval*2))
//	{
//		m_grid[vidx]->SetItemBkColor(row, colCURR, colorLTRED);
//	}
//	else if (curr >= (plow + dval))
//	{
//		m_grid[vidx]->SetItemBkColor(row, colCURR, GetColor(68));
//	}
//	else if (curr >= plow)
//	{
//		m_grid[vidx]->SetItemBkColor(row, colCURR, colorLTBLUE);
//	}
//	else if (curr < plow)
//	{
//		m_grid[vidx]->SetItemBkColor(row, colCURR, colorDKBLUE);
//	}
//	else // default
//	{
//		TRACE("check  error....\n");
//	}
//}

bool CMapWnd::IsBlink()
{
	char	readB[128];
	CString filePath;

	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, "axisensetup.ini");
	GetPrivateProfileString("SCREEN", "UNFLESH", "0", readB, sizeof(readB), filePath);

	return atoi(readB) == 0;
}

///////////////////////////////////////////////////////////////////////////
//
CString CMapWnd::parseX(CString &srcstr, CString substr)
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

CString CMapWnd::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

int CMapWnd::openView(int type, CString data)
{
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

void CMapWnd::sendTR(CString trCode, char* datB, int datL, int key)
{
	std::unique_ptr<char[]> sendB = std::make_unique<char[]>(L_userTH + datL + 1);

	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB.get();

	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB.get()[L_userTH], datB, datL);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB.get());

	sendB = nullptr;
}

CFont* CMapWnd::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point; fontR.italic = false; fontR.bold = 0;

	switch (style)
	{
	case 0: case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_parent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* CMapWnd::getAxBitmap(CString path)
{
	return (CBitmap*)m_parent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

COLORREF CMapWnd::GetColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

double CMapWnd::TOfabs(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;

	dval = fabs(atof(string));
	return dval;
}

double CMapWnd::TOf(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;

	dval = atof(string);
	return dval;
}

CString CMapWnd::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

CString	CMapWnd::charTOstring(char* datB, int datL)
{
	CString	string = CString(datB, datL);
	string.TrimLeft(); string.TrimRight();
	return string;
}


void CMapWnd::SendMinMaxInfo()
{
	const int nKey = m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);

	if (nKey == 0)
		return;

	int nWidth = 0;
//	int colW, rowH; 
	//m_grid[0]->GetCellSize(0, dispRow, colW, rowH); 
	//nWidth = (colW + scrollW) * m_viewType;

	CRect rc;
	_grid[0]->GetClientRect(rc);
	nWidth = m_gridW * m_viewType + 13;

	

	m_MMI.ptReserved.x = 13;
	m_MMI.ptReserved.y = 0;
	m_MMI.ptMinTrackSize.x = nWidth + 10 - m_viewType;
	//m_MMI.ptMinTrackSize.x = nWidth + 10 - m_viewType;
	m_MMI.ptMaxSize.x = nWidth;

	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axMinMaxInfo, nKey), (LPARAM)&m_MMI);
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
//	CMemDC	mdc(&dc);
//	Draw(&mdc);		

	Draw(&dc);		
}

void CMapWnd::Draw(CDC *pDC)
{
	CRect rect,rectb;

	GetClientRect(&rect);

	//background
	pDC->FillSolidRect(&rect,GetColor(64));//64
	rectb = rect;
	
	//upper panel
	rect.bottom = offsetGH + gap2;
	rect.left = rect.left + gap1;
	rect.top = rect.top + gap1;
	rect.right = rect.right - gap1;

	pDC->FillSolidRect(&rect,GetColor(66));
	DrawRoundBitmap(pDC, rect, ROUND_CONTENTS);
	
	//below grid outbox
	rectb.top = offsetGH + gap2*3;
	rectb.left = rectb.left + gap1;
	rectb.right = rectb.right - gap1;
	rectb.bottom = rectb.bottom - gap2;

	pDC->FillSolidRect(&rectb,GetColor(192));
	DrawRoundBitmap(pDC, rectb, ROUND_CONTENTS);
}

void CMapWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + topg
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");

		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");

		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top0
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");

		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");

		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CMapWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;


	auto* BitMap = getAxBitmap(sImageDir);
	//HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;

	memDC.CreateCompatibleDC(pDC);

	if (memDC.m_hDC != NULL) {
		memDC.SelectObject(BitMap);
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	}

	DeleteObject(BitMap);
	memDC.DeleteDC();
}

CString CMapWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_root, "tab", "palette.ini");

	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;

	return strPal;
}

void CMapWnd::RbuttonAction(AxGrid* m_grid, CString& code)
{
	int	index = -1, offs=0;

	const	int userBASE = 100;
	const	int menuBASE = 200;

	char* ptr = nullptr, wb[1024]{};
	CString string, path, mapN = _T("IB200200");
	std::unique_ptr<CMenu> popM = std::make_unique<CMenu>(); 
	CStringArray items; items.RemoveAll();

	CPoint	point; 
	GetCursorPos(&point);

	popM->CreatePopupMenu();

	m_root = Variant(homeCC, "");

	path.Format("%s/%s/%s", m_root, MTBLDIR, "IB20");
	DWORD	rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
	
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

	popM->AppendMenu(MF_SEPARATOR, 0);
	popM->AppendMenu(MF_STRING, userBASE+0, "연결화면 편집...");
	popM->AppendMenu(MF_SEPARATOR, 0);
	popM->AppendMenu(MF_STRING, userBASE+1,  "엑셀로 보내기");

	if (index >= 0)
	{
		index++;
	}
	
	index = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, point.x, point.y, m_parent);
	
	popM = nullptr;

	CString  domino = _T("");
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
	case userBASE+0:
		{
			mapN = _T("IB200200");
			m_parent->SendMessage(WM_USER, MAKEWPARAM(linkDLL, 0), (LPARAM)(LPCTSTR)mapN);
		}
		break;
	case userBASE+1:
		{
			if (m_grid) m_grid->runExcelThread();	
		}
		break;
	}
}

int CMapWnd::GetCodeType(CString code)
{
	const int codeL = code.GetLength();
	if (codeL <= 0) return 0;
	
	const int result = m_parent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);
	
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


void CMapWnd::Request_GroupList()
{
	int	sendL = 0;
	std::string sendB;
	sendB.resize(16 * 1024);
	const int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	sendTR(trUPDOWN, sendB.data(), sendL, TRKEY_GROUP);
}

void CMapWnd::Request_GroupCode(int iseq, int vidx)
{
	const int index = iseq;
	int sendL = 0;
	CString stmp;
	std::string sendB;
	sendB.resize(16 * 1024);

	const int isz_updn = sz_uinfo + 2;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;
	FillMemory((char*)updn, isz_updn, ' ');
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'j';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';
	stmp.Format("%2d", index);
	memcpy((char*)&sendB[sz_uinfo], stmp, 2);
	sendTR(trUPDOWN, sendB.data(), sendL, TRKEY_INTER + vidx);
	_alertkey.clear();
}

void CMapWnd::receiveOub(int key, CString data, int vidx)
{
	struct  _glistex {
		char    ngrs[2];           /* New Group Seqn       */
		char    ogrs[2];           /* Old Group Seqn       */
		char	gnam[30];
	};

	switch (key)
	{
	case TRKEY_GROUP:
	{
		const int cnt = atoi(data.Left(4));
		data = data.Mid(4);
		data.Trim();
		if (!data.IsEmpty() && cnt > 0)
		{
			std::vector<std::pair<CString, CString>> vGroupName;
			const gsl::span<_glistex> groupList((_glistex*)data.GetString(), cnt);

			std::sort(groupList.begin(), groupList.end(), [](auto item, auto item1) {
				return CString(item.ngrs, 2).Trim().Compare((CString(item1.ngrs, 2).Trim())) < 0;
			});

			for_each(groupList.begin(), groupList.end(), [&](auto item) {
				const CString groupKey = CString(item.ngrs, 2).Trim();
				const CString groupName = CString(item.gnam, 30).Trim();
				vGroupName.emplace_back(std::make_pair(groupName, groupKey));
			});

			for (int ii = 0; ii < maxVIEW; ii++)
				m_internm[ii].ResetContent();

			for (int name = 0; name < cnt; name++) 
			{
				for (int ii = 0; ii < maxVIEW; ii++)
				{
					const int index = m_internm[ii].AddString(vGroupName[name].first);
					m_internm[ii].SetItemData(index, atoi(vGroupName[name].second));
				}
			}
			loadGroupInfo();
		}
	}
	break;
	case TRKEY_INTER:
	{
		const CString skey = data.Left(2);
		const CString sGroupName = data.Mid(2, 20);
		const CString sCount = data.Mid(22, 4);
		//
		data = data.Mid(26);

		const gsl::span<_jinfo> codelist((_jinfo*)data.GetString(), atoi(sCount));

		for_each(codelist.begin(), codelist.end(), [&](auto item) {
			std::shared_ptr<_inters> pinters = std::make_shared<_inters>();
			m_inters[vidx].Add(pinters);

			memcpy(&pinters.get()->code[0], &item.code[0], 12);
			memcpy(&pinters.get()->name[0], &pinters->code[0], 12);
		});

		sendTransaction(vidx);
	}
	break;
	default:
		break;
	}
}

CBrush* CMapWnd::getAxBrush(COLORREF color)
{
	if (m_parent->GetSafeHwnd())
		return (CBrush*)m_parent->SendMessage(WM_USER, MAKEWPARAM(getBRUSH, 0), (LPARAM)color);
	return nullptr;
}
