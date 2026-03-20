// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_comboList.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int indicatorW	= 11;
const int bitmapW	= 20;
const int gap		= 0;

//const int symbolwidth	= 170;
//const int editwidth	= 80;

#define BTN	20
#define CODE	80

#define IDC_SYMBOL		  0x01
#define	IDC_CODE	 	  0x02
#define IDC_BUTTON_CATEGORY	  0x03
#define IDC_BUTTON_INTER	  0x04

#define CODEWIDTH		(int)(CODE*rWidth)
#define BTNWIDTH		(int)(BTN*rWidth)

#define	eventTABTIME		100
#define stockFUTURE		200

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd(CWnd* pWnd) : CAxWnd(pWnd)
{
	EnableAutomation();
	m_rtnStr	= _T("");

	m_bgColor	= GetSysColor(COLOR_WINDOW);
	m_pFont		= nullptr;
	m_nFirst	= 0;
	m_bInit		= true;	

	m_szBtnSearch	= _T("");
	m_szBtnHistory  = _T("");
	m_szBtnInter	= _T("");
	m_szBtnCategory = _T("");

	// 컨트롤 모양...
	m_bLastText	= false;
	m_bText		= false;
	m_bTrigger	= false;
	m_bDomino	= false;
	m_bFocus	= true;
	m_bNext		= false;
	m_bModify	= false;
	m_bSearch	= false;

	m_Text1 = m_Text2 = m_Text3 = m_Text4 = _T("");
	m_bTabStop = TRUE;

	m_bTimer = true;

}

CControlWnd::~CControlWnd()
{

}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CAxWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CControlWnd, CAxWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_POPLISTWINDOW, OnInitPos)
	ON_BN_CLICKED(IDC_BUTTON_CATEGORY, OnBtnCode)
	ON_BN_CLICKED(IDC_BUTTON_INTER, OnBtnInter)
	ON_BN_CLICKED(ID_BTN_DROP, OnBtnDropClicked)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CControlWnd, CAxWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Visible", m_visible, OnVisibleChanged, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "Data", GetData, SetData, VT_BSTR)
	DISP_PROPERTY_EX(CControlWnd, "TabStop", GetTabStop, SetTabStop, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "UnitData", GetUnitData, SetUnitData, VT_I2)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetCode", SetCode, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetName", GetName, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "Text1", Text1, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Text2", Text2, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Text3", Text3, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Text4", Text4, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetFocus", SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "Trigger", Trigger, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CControlWnd, "Enable", Enable, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CControlWnd, "Clear", Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetUnit", SetUnit, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CControlWnd, "IsKonex", IsKonex, VT_BOOL, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {AA69DFD8-D09A-4295-8B0B-0AC78A214F10}
static const IID IID_IControlWnd =
{ 0xaa69dfd8, 0xd09a, 0x4295, { 0x8b, 0xb, 0xa, 0xc7, 0x8a, 0x21, 0x4f, 0x10 } };

BEGIN_INTERFACE_MAP(CControlWnd, CAxWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers
bool CControlWnd::m_bTimer;

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString szData = CString((TCHAR*)lParam);
	CString str;
	int	length = HIWORD(wParam);
	CString tmp;

	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		// 컨트롤에서 읽어 들일때...	===================================================================
		
		m_Text1 = m_Text2 = m_Text3 = m_Text4 = _T("");
		Msg("=================================	DLL_INB	=========================================");
		Msg("%s", m_rtnStr);
		Msg("====================================================================================");
		return (long)m_rtnStr.operator LPCTSTR();

	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		length = exth->size;
		if (m_bDomino && length < 13 && length > 2)
		{
			// 선물, 옵션, 주식 구분해서 처리...
			CString str = CString(exth->data);
			str.TrimRight();
			length = str.GetLength();

			if (m_nGubn == GU_FCODE && length != 8)
				return 0;
			else if ((m_nGubn == GU_CODE || m_nGubn == GU_ELWCODE) && length > 6)
				return 0;
			else if (m_nGubn != GU_CODE && m_nGubn != GU_ELWCODE && m_nGubn != GU_INDEX && length <= 6)
				return 0;
			else if (str.IsEmpty())
				return 0;

		
			m_bFocus = false;
		}
		else if (length > 100 && !m_bInit)
		{
			if (m_bFocus)
				SetTimer(eventTABTIME, 100, NULL);
		}
		m_bInit = false;
		break;

	case DLL_OUB:	// NOT USED. Use DLL_OUBx!!
		// Domino, Trigger 처리 control에서는 domino trigger 가 안됨 ======================================
		tmp.Format(", length: [%d]", length);
		if (m_bDomino && length < 13 && length > 2)
		{

			// 선물, 옵션, 주식 구분해서 처리...
			CString str = CString((char*)lParam);
			str.TrimRight();
			Msg("=================================	DLL_OUB	=========================================");
			Msg("%s", str);
			Msg("====================================================================================");

			length = str.GetLength();

			if (m_nGubn == GU_FCODE && length != 8)
				return 0;
			else if ((m_nGubn == GU_CODE || m_nGubn == GU_ELWCODE) && length > 6)
				return 0;
			else if (m_nGubn != GU_CODE && m_nGubn != GU_ELWCODE && m_nGubn != GU_INDEX && length <= 6)
				return 0;
			else if (str.IsEmpty())
				return 0;
			
			m_bFocus = false;
		}
		else if (length > 100 && !m_bInit)
		{
			if (m_bFocus)
				SetTimer(eventTABTIME, 100, NULL);
		}
		m_bInit = false;
		break;
	case DLL_ALERT:
		break;
	case DLL_TRIGGER:
		//AfxMessageBox(szData);
		break;
	case DLL_DOMINO:
		break;
	case DLL_NOTICE:
		break;
	case DLL_SETPAL:
		Invalidate();
		break;
	case DLL_SETFONT:
		{
			m_Param.point = HIWORD(wParam);
			CString tmp = CString((char*)lParam);
			if (!tmp.IsEmpty())
				m_Param.fonts = tmp;
	
			m_pFont = GetAxFont(tmp, m_Param.point);
		}
		break;
		
	case formDLL:
		//=======	이화면에서 돌려보기 가로챔	========================================================================
		if (Parser(szData, "\n").CompareNoCase("\tE") == 0)
		{
			
			return 1;
		}
	default:
		{
			return 0;
			//=======	account 관련 가로챔	================================================================================
			const	UINT value = LOWORD(wParam);
			if (value == 13)
				return 1;
		}
		return m_pWizard->SendMessage(WM_USER, wParam, lParam);
	}
	return 0;
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	rc;
	CSize	sz;

	GetClientRect(rc);
	//dc.FillSolidRect(rc, GetIndexColor(m_bgColor));

	
	dc.FillSolidRect(rc, RGB(255,255,255));
	//dc.Draw3dRect(&rc, RGB(180, 180, 180), RGB(180, 180, 180));
	return;

	if (m_nGubn != GU_CODE)
		return;

	if (!m_pFont)
		m_pFont = this->GetAxFont(m_Param.fonts, m_Param.point, m_Param.style);
	CFont*	poldfont = dc.SelectObject(m_pFont);

	if (m_bText)
	{
		dc.SelectObject(GetAxFont("굴림체", m_Param.point, m_Param.style));
		rc.CopyRect(m_rcText);
		rc.left += 10;

		// 첫번째 라인...
		sz = dc.GetTextExtent(m_Text1);
		rc.right = rc.left + sz.cx + 10;
		dc.DrawText(m_Text1, rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		dc.SelectObject(m_pFont);

		// 두번째 라인...
		sz = dc.GetTextExtent(m_Text2);
		rc.left  = rc.right;
		rc.right = rc.left + sz.cx + 10;
		dc.DrawText(m_Text2, rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		
		
		dc.SetTextColor(RGB(0, 0, 0));
		if (m_bLastText)
		{
			rc.OffsetRect(m_rcText.Width(), 0);
			sz = dc.GetTextExtent(m_Text4);
			rc.left  = rc.right;
			rc.right = rc.left + sz.cx;

			dc.DrawText(m_Text4, rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		}
	
	}
	dc.SelectObject(poldfont);
	return;
}

void CControlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CAxWnd::OnLButtonDown(nFlags, point);
}

void CControlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CAxWnd::OnLButtonUp(nFlags, point);
}

void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CAxWnd::OnMouseMove(nFlags, point);
}

void CControlWnd::OnDestroy() 
{
	CAxWnd::OnDestroy();	
}

void CControlWnd::SetParam(_param *pParam)
{
	CAxWnd::SetParam(pParam);
	SetProperties(pParam->options);
}

void CControlWnd::SetProperties(LPCTSTR sParam) 
{
	m_Param.options = sParam;

	int	idx = 0, pos = 0;
	CString	string, text, tmps, keys;
	
	tmps	= _T("/ ");
	keys	= _T("abcdefghijklmnopuqs");
	keys	= _T("uklpoqs");
	string	= sParam;
	
	for (int ii = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = string.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = string.Find('/', idx);

		text = (pos < 0) ? string.Mid(idx) : string.Mid(idx, pos-idx);
		text.TrimLeft();

		switch (keys[ii])
		{
		case 'a':	// box color
			if (text.IsEmpty())
				m_boxColor = GetIndexColor(89);
			else
				m_boxColor = GetIndexColor(atoi(text));
			break;
		case 'b':	// hover box color
			if (text.IsEmpty())
				m_boxColorHover = GetIndexColor(100);
			else
				m_boxColorHover = GetIndexColor(atoi(text));
			break;
		case 'c':	// hover background color
			if (text.IsEmpty())
				m_bgCol = GetIndexColor(90);
			else
				m_bgCol = GetIndexColor(atoi(text));
			break;
		case 'd':	// 
			if (text.IsEmpty())
				m_bgColHover = GetIndexColor(68);
			else
				m_bgColHover = GetIndexColor(atoi(text));
			break;
		case 'e':
			if (text.IsEmpty())
				m_bgColor = 64;
			else 
				m_bgColor = atoi(text);
			break;
		case 'f':
			m_bLastText = true;
			break;
		case 'j':
			m_bText = true;
			break;		
		
		case 'k':
			m_szBtnSearch = text.IsEmpty() ? "history_code.bmp": text; 
			break;
		case 'l':
			m_szBtnHistory = text.IsEmpty() ? "history_arrow.bmp": text; 
			break;
		case 'm':
			m_szBtnInter = text.IsEmpty() ? "관심종목.bmp" : text;
			break;
		case 'n':
			m_szBtnCategory = text.IsEmpty() ? "category.bmp" : text;
			break;
		case 'o':	
			m_bTrigger = true;
			break;
		case 'p':	//Domiono
			m_bDomino = true;
			break;
		case 'u':
			m_nGubn = atoi(text);
			break;
		case 'q':
			m_bNext = true;
			break;
		case 's':
			//if (atoi(text)==1)
			m_bSearch = (atoi(text)==1);
			break;
		case 'h':
			m_sHistoryFile = text;
			break;
		case 'i':
			m_sSection = text;
			break;
		}
	}
}

BSTR CControlWnd::GetProperties() 
{
	CString strResult = m_Param.options;
	return strResult.AllocSysString();
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CAxWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO: Add your specialized creation code here
	CRect rc;
	GetClientRect(rc);

	const int cx = rc.Width();
	const int cy = rc.Height();

	// 최초 크기 저장
	m_szOriginal.cx = cx;
	m_szOriginal.cy = cy;
	m_nBtnWidthOrig = m_nBtnWidth; // 최초 버튼 너비 저장

	CString file;
	int nType = 0;
	CString usr;

	usr = Variant(nameCC, "");
	m_sRoot = Variant(homeCC, "");

	m_sHistoryFile.Format("%s\\%s\\%s\\%s", m_sRoot, USRDIR, usr, "userconf.ini");
	m_sSection = "IB425301";
	LoadHistory();
	//nType = GetPrivateProfileInt("SCREEN", "CODECTRL", 0, file);

	//m_pCodeCtrl = std::make_unique<CfxCodeCtrl>(m_pWizard, Variant(homeCC, ""));
	//m_pCodeCtrl->Create(this, CRect(0, 0, cx - m_nBtnWidthOrig, cy), 1001);

	// CfxImgButton
	m_pBtnDrop = std::make_unique<CfxImgButton>();
	m_pBtnDrop->Create("",
		CRect(cx - m_nBtnWidth, 0, cx, cy),
		this, ID_BTN_DROP);

	m_pBtnDrop->LoadPng(
		m_sRoot + "\\image\\" + "axspin1.png",     // 일반
		m_sRoot + "\\image\\" + "axspin1_dn.png",  // 클릭 (없으면 "" 전달)
		m_sRoot + "\\image\\" + "axspin1_en.png"); // 호버 (없으면 "" 전달)

// m_pBmpNormal = GetBitmap(m_sRoot + "\\image\\" + "axspin1.bmp");
// m_pBmpHover = GetBitmap(m_sRoot + "\\image\\" + "axspin1_en.bmp");
// m_pBmpDown = GetBitmap(m_sRoot + "\\image\\" + "axspin1_dn.bmp");
//	if (m_pBmpNormal && m_pBtnDrop)
//		m_pBtnDrop->SetImgBitmap(m_pBmpNormal, m_pBmpDown, m_pBmpHover);

	 // 클라이언트 좌표 → 스크린 좌표 변환
	PostMessage(WM_POPLISTWINDOW, POPLIST_CREATE, 0);  //postmessage 로 비동기 최초 생성

	//종목 검색 방식 선택 : 0:기존검색방식 1:중간검색방식


	// 버튼 이미지 변경 가능성이 높음...
	CString szSearch, szHis;
	szHis = m_szBtnHistory;
	szSearch = m_szBtnSearch;

	// 여기 수정 선물 옵션... 주식 기타등등...
//	CString str = Variant(historyCC, m_pCode->GetSymbol());
//	m_pCode->SetEditData(Parser(str, " "));

	GetClientRect(m_ClientRect);
	Resize();
	
	SetTimer(stockFUTURE, 350, NULL);
	return 0;
}
LRESULT CControlWnd::OnInitPos(WPARAM wp, LPARAM lp) //OnInitPos WM_POPLISTWINDOW
{
	CRect rc;
	GetWindowRect(&rc);

	CString slog;
	slog.Format("[OnInitPos] LT(%d,%d) RB(%d,%d)\n",
		rc.left, rc.top, rc.right, rc.bottom);
	OutputDebugString(slog);

	switch (wp)
	{
		case POPLIST_SHOW:
		{
			m_bDropVisible = TRUE;
			ShowDropList(TRUE);
		}
		break;
		case POPLIST_CREATE:
		{
			CRect rcClient;
			GetClientRect(&rcClient);

			CPoint ptLT(rcClient.left, rcClient.bottom);
			CPoint ptRB(rcClient.right + 100, rcClient.bottom + 200);

			ClientToScreen(&ptLT);
			ClientToScreen(&ptRB);

			CString slog;
			slog.Format("[OnShowDropList] LT(%d,%d) RB(%d,%d)\n",
				ptLT.x, ptLT.y, ptRB.x, ptRB.y);
			OutputDebugString(slog);

			CRect rcPop(ptLT.x, ptLT.y, ptRB.x, ptRB.y);

			m_pListPop = std::make_unique<CPopListWnd>();
			m_pListPop->SetType(VS1_TYPE);
			m_pListPop->CreatePopUpWindow(this, rcPop);
			m_sItems = GetItemsString();
			m_pListPop->CreateListBox(m_sItems);
			m_pListPop->ShowWindow(SW_HIDE);
		}
		break;
		case POPLIST_HIDE: // 다른 곳 클릭으로 닫힘 → 버튼 누르면 바로 열려야
		{
			if (m_pListPop)
				m_pListPop->ShowWindow(SW_HIDE);
		}
		break;
		case POPLIST_CLEARALL:
		{
			ClearItems();
			SaveHistory(); // 파일에도 저장
		}
		break;
	}

	return 0;
}
void CControlWnd::OnBtnDropClicked()
{
	PostMessage(WM_POPLISTWINDOW, POPLIST_SHOW, 0); //OnBtnDropClicked 보여준다
}

bool CControlWnd::Resize()
{
	CRect	rc;

	GetClientRect(&rc);
	const	double	rWidth  = (double)rc.Width()  / m_ClientRect.Width();
	const	double	rHeight = (double)rc.Height() / m_ClientRect.Height();

	if (m_nGubn == GU_CODE)
	{
		int	nSymbolWidth = 0; 
		int	nCodeWidth = 0, width = 0;

		nSymbolWidth = rc.Width() - width - CODEWIDTH;

		nCodeWidth = CODEWIDTH + gap;

		if (m_bText)
			nSymbolWidth = nSymbolWidth / 2;
	}
	else 
	{
		m_rcText = CRect(0, 0, 0, 0);
	}

	return true;
}

//const char*	gSYMBOL	= _T("$90303");
const char	gnCHAR	= 0x7f;
const char	tabCHAR	= 0x09;
const char	lfCHAR	= 0x0A;

void CControlWnd::Send(CString szCode)
{
	CString trData;

	szCode.TrimLeft();
	trData.Format("1301%c%s%c1301%c17413%c", gnCHAR, szCode, tabCHAR, tabCHAR, tabCHAR);

	SendTRC("POOPPOOP", trData, US_OOP);
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CAxWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (cx <= 0 || cy <= 0) return;
	if (m_szOriginal.cx <= 0 || m_szOriginal.cy <= 0) return;

	// 비율 계산
	const float ratioX = (float)cx / m_szOriginal.cx;
	const float ratioY = (float)cy / m_szOriginal.cy;

	// 버튼 너비도 비율 적용
	const int nBtnW = (int)(m_nBtnWidthOrig * ratioX);

	// CfxCodeCtrl - 에디트 영역
	if (m_pCodeCtrl && m_pCodeCtrl->GetSafeHwnd())
		m_pCodeCtrl->MoveWindow(0, 0, cx - nBtnW, cy);

	// CfxImgButton - 버튼 비율 적용
	if (m_pBtnDrop && m_pBtnDrop->GetSafeHwnd())
	{
		m_pBtnDrop->MoveWindow(cx - nBtnW, 0, nBtnW, cy);

		CString slog;
		slog.Format("[CControlWnd] OnSize BtnDrop x=%d w=%d h=%d\n",
			cx - nBtnW, nBtnW, cy);
		OutputDebugString(slog);
	}

	//Resize();
}

void CControlWnd::OnBtnCode()
{
}
	
void CControlWnd::OnBtnInter()
{
	CString str;
	CRect rc;

	str.Format("HD702001/S/p5/d appendCODE\t%s\n", m_rtnStr);
	OpenView(str, typePOPUP);
}

void CControlWnd::InitControl(CRect rect)
{

}

void CControlWnd::SetEditFocus(BOOL bflag)
{

}

BSTR CControlWnd::GetName() 
{
	CString strResult = GetHNam(m_rtnStr);
	return strResult.AllocSysString();
}

void CControlWnd::Text1(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here
	CString sz = CString(str);
	m_Text1 = sz.IsEmpty() ? _T("") : sz;
}

void CControlWnd::Text2(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here
	CString sz = CString(str);
	m_Text2 = sz.IsEmpty() ? _T("") : sz;

}

void CControlWnd::Text3(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here
	CString sz = CString(str);
	m_Text3 = sz.IsEmpty() ? _T("") : sz;
}

void CControlWnd::Text4(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here
	CString sz = CString(str);
	m_Text4 = sz.IsEmpty() ? _T("") : sz;
}

void CControlWnd::SetFocus() 
{
	// TODO: Add your dispatch handler code here
	if (SetAxFocus(false))
		SetEditFocus(TRUE);
}

void CControlWnd::OnTrigger(CString str)
{
	CString szSymbol;
	szSymbol = Parser(str, "\t");

}

void CControlWnd::Trigger(LPCTSTR name, BOOL all) 
{
	// TODO: Add your dispatch handler code here
	//symbol\tdata/r/key

	CString str, sName;
	sName = CString(name);

	if (sName.IsEmpty())
		name = m_Param.name;
	
	if (all == TRUE)	str.Format("%s\t%s", CString(name), m_rtnStr);	
	else			str.Format("%s\t%s\r%d", CString(name), m_rtnStr, m_Param.key);

	Variant(triggerCC, str);
}

BSTR CControlWnd::GetData() 
{
	return m_rtnStr.AllocSysString();
}

void CControlWnd::SetData(LPCTSTR code) 
{
	CString sval;
	sval.Format("%s", code);
}

void CControlWnd::SetCode(LPCTSTR code) 
{
	
}


void CControlWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CAxWnd::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here
	

}

BOOL CControlWnd::GetTabStop() 
{
	// TODO: Add your property handler here
	return m_bTabStop;
}

void CControlWnd::SetTabStop(BOOL bNewValue) 
{
	// TODO: Add your property handler here
	m_bTabStop = bNewValue;
	m_pWizard->SendMessage(WM_USER, 
			MAKEWPARAM(tabsDLL, MAKEWORD(m_Param.key, m_bTabStop ? 1 : 0)), 
			(LPARAM)m_Param.name.GetString());
}

void CControlWnd::Enable(BOOL bflags) 
{
	// TODO: Add your dispatch handler code here
	if (bflags)
	{
		EnableWindow(bflags);
	}
	else
	{
		EnableWindow(bflags);
	}
	Invalidate();
}

void CControlWnd::Clear() 
{
	// TODO: Add your dispatch handler code here
	//SetCode("");
	m_rtnStr = _T("");
}

void CControlWnd::clean()
{
	Clear();
}

CString CControlWnd::GetHNam(CString code)
{
	if (code.GetLength() <= 0)
		return _T("");

	CString hnam;
	char	cBuffer[16]{};

	CopyMemory(cBuffer, code.GetString(), code.GetLength());
	const	char* pcData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)cBuffer);
	if ((long)pcData > 1)
		hnam = pcData;

	return hnam;
}

void CControlWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CAxWnd::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == eventTABTIME)
	{	
		if (!m_bModify &&!m_bNext && SetAxFocus(false))
		{
			::SetFocus(GetSafeHwnd());
			PostMessage(WM_KEYDOWN, VK_TAB, 0);
		}

		m_bModify = false;
		KillTimer(eventTABTIME);
	}
	else if (nIDEvent == stockFUTURE)
	{
		KillTimer(stockFUTURE);

		if (m_rtnStr.IsEmpty() && (m_nGubn == GU_FCODE || m_nGubn == GU_FOSTOCK) )
		{
		
		}
	}

	CAxWnd::OnTimer(nIDEvent);
}

void CControlWnd::OnVisibleChanged() 
{
	// TODO: Add notification handler code
	if (m_visible)
		ShowWindow(SW_SHOWNORMAL);
	else
		ShowWindow(SW_HIDE);
}

void CControlWnd::SetUnit(short unit) 
{
	
}

short CControlWnd::GetUnitData() 
{
	return 0;
}

void CControlWnd::SetUnitData(short nNewValue) 
{
	m_nGubn = nNewValue;
}

BOOL CControlWnd::IsKonex() 
{
	return 0;
}

void CControlWnd::ShowDropList(bool bShow)
{
	if (!m_pListPop || !m_pListPop->GetSafeHwnd()) return;

	if (bShow)
	{
		// 매번 위치 재계산 (윈도우 이동 대비)
		CRect rcScreen;
		GetWindowRect(&rcScreen);

		CRect rcClient;
		GetClientRect(&rcClient);

		m_pListPop->SetWindowPos(
			&CWnd::wndTopMost,
			rcScreen.left,
			rcScreen.bottom,    // 에디트 바로 아래
			rcClient.Width() + 100,   // 너비 동일
			200,                // 드롭 높이
			SWP_SHOWWINDOW);
		m_pListPop->ShowWindow(SW_SHOW);



		// 리스트 항목 로드 (히스토리 or 검색결과)
		//if (m_pListPop->m_pCodelist == nullptr)
		//	m_pListPop->CreateListBox(m_sItems); // m_sItems = 표시할 데이터
	}
	else
	{
		m_pListPop->ShowWindow(SW_HIDE);
	}

	m_bDropVisible = bShow;
}



void CControlWnd::SetImgBitmap(CBitmap* pNormal, CBitmap* pDown, CBitmap* pHover)
{
	m_pBmpNormal = pNormal;
	m_pBmpDown = pDown;
	m_pBmpHover = pHover;

	// 버튼이 이미 생성된 경우 즉시 적용
	if (m_pBtnDrop && m_pBtnDrop->GetSafeHwnd())
		m_pBtnDrop->SetImgBitmap(pNormal, pDown, pHover);
}

void CControlWnd::LoadHistory()
{
	OutputDebugString("[CControlWnd] LoadHistory\n");

	m_vecItems.clear();
	m_mapItems.clear();

	if (m_sHistoryFile.IsEmpty() || m_sSection.IsEmpty()) return;

	// combohistory 읽기
	char buf[4096] = { 0 };

	GetPrivateProfileString(
		m_sSection,       // 섹션 ex) "4253"
		"combohistory",   // 키
		"",               // 기본값
		buf,
		sizeof(buf),
		m_sHistoryFile);  // 파일 경로

	CString sData(buf);
	sData.TrimRight();

	CString slog;
	slog.Format("[CControlWnd] LoadHistory data=%s\n", sData);
	OutputDebugString(slog);

	if (sData.IsEmpty()) return;

	// "000660 SK하이닉스\t000020 동화약품\t..." 파싱
	CString sTemp = sData;
	while (!sTemp.IsEmpty())
	{
		int nTab = sTemp.Find('\t');
		CString sItem;

		if (nTab >= 0)
		{
			sItem = sTemp.Left(nTab);
			sTemp = sTemp.Mid(nTab + 1);
		}
		else
		{
			sItem = sTemp;
			sTemp.Empty();
		}

		sItem.TrimRight();
		if (sItem.IsEmpty()) continue;

		// "000660 SK하이닉스" 파싱
		int nSpace = sItem.Find(' ');
		if (nSpace < 0) continue;

		CString sCode = sItem.Left(nSpace);
		CString sName = sItem.Mid(nSpace + 1);

		AddItem(sCode, sName);

		slog.Format("[CControlWnd] LoadHistory code=%s name=%s\n",
			sCode, sName);
		OutputDebugString(slog);
	}

	OutputDebugString("[CControlWnd] LoadHistory 완료\n");
}

void CControlWnd::SaveHistory()
{
	OutputDebugString("[CControlWnd] SaveHistory\n");

	if (m_sHistoryFile.IsEmpty() || m_sSection.IsEmpty()) return;

	// "000660 SK하이닉스\t000020 동화약품\t..." 형태로 생성
	CString sData;
	for (const auto& item : m_vecItems)
	{
		CString s;
		s.Format("%s %s\t", item.sCode, item.sName);
		sData += s;
	}

	WritePrivateProfileString(
		m_sSection,       // 섹션 ex) "4253"
		"combohistory",   // 키
		sData,            // 값
		m_sHistoryFile);  // 파일 경로

	CString slog;
	slog.Format("[CControlWnd] SaveHistory data=%s\n", sData);
	OutputDebugString(slog);
}

void CControlWnd::AddItem(CString sCode, CString sName)
{
	// 중복 체크
	if (m_mapItems.find(sCode) != m_mapItems.end())
	{
		OutputDebugString("[CControlWnd] AddItem 중복 " + sCode + "\n");
		return;
	}

	CodeItem item;
	item.sCode = sCode;
	item.sName = sName;

	m_mapItems[sCode] = (int)m_vecItems.size();
	m_vecItems.push_back(item);

	CString slog;
	slog.Format("[CControlWnd] AddItem code=%s name=%s\n", sCode, sName);
	OutputDebugString(slog);
}

void CControlWnd::RemoveItem(CString sCode)
{
	auto it = m_mapItems.find(sCode);
	if (it == m_mapItems.end()) return;

	int nIdx = it->second;
	m_vecItems.erase(m_vecItems.begin() + nIdx);
	m_mapItems.erase(it);

	// map 인덱스 재정렬
	for (auto& pair : m_mapItems)
	{
		if (pair.second > nIdx)
			pair.second--;
	}

	OutputDebugString("[CControlWnd] RemoveItem code=" + sCode + "\n");
}

void CControlWnd::ClearItems()
{
	m_vecItems.clear();
	m_mapItems.clear();
	OutputDebugString("[CControlWnd] ClearItems\n");
}

// CPopListWnd 에 넘길 문자열 생성
CString CControlWnd::GetItemsString()
{
	CString sResult;
	for (const auto& item : m_vecItems)
	{
		CString s;
		s.Format("%s %s\t", item.sCode, item.sName);
		sResult += s;
	}
	return sResult;
}