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

// [기능] EUC-KR 초성 자모 바이트 테이블 (MatchChosung에서 사용)
static const TCHAR g_chosung[] = {
	0xB1, 0xB2, 0xB4, 0xB7, 0xB8, 0xB9, 0xBB, // ㄱㄲㄴㄷㄸㄹㅁ
	0xBC, 0xBD, 0xBF, 0xC0, 0xC1, 0xC2, 0xC4, // ㅂㅃㅅㅆㅇㅈㅉ
	0xC5, 0xC6, 0xC7                             // ㅊㅋㅌㅍㅎ
};

// [기능] 유니코드 한글 완성형 문자에서 초성 유니코드를 추출
// [파라미터] ch: 유니코드 한글 완성형 문자 (0xAC00~0xD7A3 범위), 범위 밖이면 ch 그대로 반환
// [호출함수] (현재 직접 호출 없음 - 향후 확장용)
TCHAR GetChosung(TCHAR ch)
{
	if (ch < 0xAC00 || ch > 0xD7A3) return ch;
	int idx = (ch - 0xAC00) / (21 * 28);
	const TCHAR chosungTable[] = {
		0xAC00, 0xB098, 0xB2E4, 0xB77C, 0xB9C8, 0xBC14, 0xC0AC,
		0xC544, 0xC790, 0xCC28, 0xCE74, 0xD0C0, 0xD30C, 0xD558
	};
	// 초성 유니코드
	const TCHAR unicodeChosung[] = {
		0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141,
		0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149,
		0x314A, 0x314B, 0x314C, 0x314D, 0x314E
	};
	return unicodeChosung[idx];
}
/////////////////////////////////////////////////////////////////////////////
// CControlWnd

#include <imm.h>
#pragma comment(lib, "imm32.lib")


// [기능] 문자열 첫 글자가 EUC-KR 한글(완성형/자모)인지 판별
// [파라미터] sText: 검사할 문자열
// [호출함수] BuildSearchItems()
BOOL CControlWnd::IsHangul(const CString& sText)
{
	if (sText.IsEmpty()) return FALSE;

	BYTE b1 = (BYTE)sText[0];

	// EUC-KR 한글 범위
	// 완성형: 0xB0A1 ~ 0xC8FE → 첫바이트 0xB0 ~ 0xC8
	// 자모:   0xA4A1 ~ 0xA4FE → 첫바이트 0xA4

	if (b1 == 0xA4 && sText.GetLength() >= 2)
	{
		BYTE b2 = (BYTE)sText[1];
		// 자모 범위 0xA4A1 ~ 0xA4BE
		return (b2 >= 0xA1 && b2 <= 0xBE);
	}

	if (b1 >= 0xB0 && b1 <= 0xC8)
		return TRUE; // 완성형 한글

	return FALSE;
}

BOOL CControlWnd::IsNumber(TCHAR ch)
{
	return (ch >= '0' && ch <= '9');
}

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
	ON_MESSAGE(WM_EDIT_MSG, OnEditMsg)
	ON_MESSAGE(WM_POPLISTWINDOW, OnInitPos)
	ON_BN_CLICKED(IDC_BUTTON_CATEGORY, OnBtnCode)
	ON_BN_CLICKED(IDC_BUTTON_INTER, OnBtnInter)
	ON_BN_CLICKED(ID_BTN_DROP, OnBtnDropClicked)
	ON_BN_CLICKED(ID_BTN_LANG, OnBtnLangClicked)
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

// [기능] WM_USER 메시지 처리 - DLL_INB(데이터 읽기), DLL_OUBx(도미노/트리거), DLL_SETFONT 등 처리
// [파라미터] wParam: LOBYTE=메시지 종류(DLL_INB/DLL_OUBx 등), HIWORD=데이터 길이
//                  lParam:   데이터 포인터 또는 _extTHx 구조체 포인터
// [호출함수] MESSAGE_MAP(ON_MESSAGE(WM_USER, OnMessage))
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
		if (length == 3 && m_nGubn == GU_FCODE && m_sOption.Find("기초자산") >=0 )
		{
			tmp.Format("%s", exth->data);
			tmp.TrimRight();
			CString hnam;
			if (m_mapCodeToGichoName.Lookup(tmp, hnam))
			{
				hnam.TrimRight();
				m_bSettingCode = true;
				m_pCodeCtrl->SetEditData(hnam);
				m_bSettingCode = false;
				AddItem(tmp, hnam);
			}
			else
			{
				tmp.Format("[[cx_combolist][Error]  m_mapCodeToGichoName 에 코드[%s] 가 없음", tmp);
				OutputDebugString(tmp);
			}	
		}
		else if (m_bDomino && length < 13 && length > 2)
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

			tmp.Format("[cx_comboList][DLL_SETFONT] size=[%d]", m_Param.point);
			OutputDebugString(tmp);

			// 에디트에 폰트 적용
			if (m_pCodeCtrl && m_pCodeCtrl->GetSafeHwnd())
			{
				m_pCodeCtrl->SetFont(m_pFont);
				OutputDebugString("[cx_comboList][DLL_SETFONT] CodeCtrl SetFont\n");
			}
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
	//keys	= _T("abcdefghijklmnopuqs");
	keys	= _T("uklpoqst");
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
		case 't':
			m_sOption = text;
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
			//m_sHistoryFile = text;
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

	// m_szOriginal 이 0 이면 기본값 설정
	if (cx > 0 && cy > 0)
	{
		m_szOriginal.cx = cx;
		m_szOriginal.cy = cy;
	}
	else
	{
		// lpCreateStruct 에서 크기 가져오기
		m_szOriginal.cx = lpCreateStruct->cx;
		m_szOriginal.cy = lpCreateStruct->cy;
	}
	m_nBtnWidthOrig = m_nBtnWidth; // 최초 버튼 너비 저장

	CString slog;
	slog.Format("[cx_combolist][CControlWnd][OnCreate] cx=%d cy=%d orig(%d,%d)\n",
		cx, cy, m_szOriginal.cx, m_szOriginal.cy);
	OutputDebugString(slog);

	CString file;
	int nType = 0;
	CString usr;

	m_sUser = Variant(nameCC, "");
	m_sRoot = Variant(homeCC, "");

	//nType = GetPrivateProfileInt("SCREEN", "CODECTRL", 0, file);

	m_pCodeCtrl = std::make_unique<CfxCodeCtrl>(m_pWizard, Variant(homeCC, ""));
	m_pCodeCtrl->Create(this, CRect(0, 0, cx - m_nBtnWidthOrig, m_szOriginal.cy), ID_EDIT_CODE);
	//m_pCodeCtrl->ShowWindow(SW_HIDE);

	// CfxImgButton
	m_pBtnDrop = std::make_unique<CfxImgButton>();
	m_pBtnDrop->Create("",
		CRect(cx - m_nBtnWidth, 0, m_szOriginal.cx, m_szOriginal.cy),
		this, ID_BTN_DROP);

	m_pBtnDrop->LoadPng(
		m_sRoot + "\\image\\" + "AXCOMBO.PNG",     // 일반
		m_sRoot + "\\image\\" + "AXCOMBO_DN.png",  // 클릭 (없으면 "" 전달)
		m_sRoot + "\\image\\" + "AXCOMBO_EN.png"); // 호버 (없으면 "" 전달)

	m_pBtnLang = std::make_unique<CfxImgButton>();
	m_pBtnLang->Create("",
		CRect(m_szOriginal.cx - m_nBtnWidthOrig - m_nBtnLangWidth, 0,
			m_szOriginal.cx - m_nBtnWidthOrig, m_szOriginal.cy),
		this, ID_BTN_LANG);

	m_pBtnLang->LoadPng(
		m_sRoot + "\\image\\axEng.png", m_sRoot + "\\image\\axEng.png", m_sRoot + "\\image\\axEng.png");


	// 클라이언트 좌표 → 스크린 좌표 변환
	PostMessage(WM_POPLISTWINDOW, POPLIST_CREATE, 0);  //postmessage 로 비동기 최초 생성

	// 버튼 이미지 변경 가능성이 높음...
	CString szSearch, szHis;
	szHis = m_szBtnHistory;
	szSearch = m_szBtnSearch;

	GetClientRect(m_ClientRect);
	Resize();
	
	SetTimer(stockFUTURE, 350, NULL);
	return 0;
}

// [기능] 에디트 컨트롤에서 발생한 WM_EDIT_MSG 처리
//        EDIT_MSG_KILLFOCUS(포커스 잃음/SearchPop 닫기), EDIT_MSG_HANGUL(한영전환),
//        EDIT_MSG_CHAR(문자입력→검색), EDIT_MSG_ARROW_DOWN(리스트 포커스 이동) 등
// [파라미터] wp: 메시지 종류(EDIT_MSG_*), lp: 부가 데이터(문자열 또는 HWND)
// [호출함수] CCodeEdit::OnKeyDown()        → PostMessage(WM_EDIT_MSG, EDIT_MSG_SHOW_HISTORYPOP/...)
//            CCodeEdit::OnInputLangChange() → PostMessage(WM_EDIT_MSG, EDIT_MSG_HANGUL)
//            CCodeEdit::OnKillFocus()       → PostMessage(WM_EDIT_MSG, EDIT_MSG_KILLFOCUS)
//            CfxCodeCtrl::OnChangeEdit()    → SendMessage(WM_EDIT_MSG, EDIT_MSG_CHAR)
LRESULT CControlWnd::OnEditMsg(WPARAM wp, LPARAM lp)
{
	switch (wp)
	{
		case EDIT_MSG_FOCUS:      // 포커스 받음
			OutputDebugString("[OnEditMsg] EDIT_MSG_FOCUS\n");
			UpdateLangBtn();
			break;

		case EDIT_MSG_KILLFOCUS:  // 포커스 잃음
		{
			OutputDebugString("[OnEditMsg] EDIT_MSG_KILLFOCUS\n");
			HWND hNewWnd = (HWND)lp;
			// 포커스가 SearchPop 또는 그 자식으로 가는 경우 닫지 않음
			if (m_pSearchPop && m_pSearchPop->GetSafeHwnd())
			{
				if (hNewWnd &&
					(hNewWnd == m_pSearchPop->GetSafeHwnd() ||
						m_pSearchPop->IsChild(CWnd::FromHandle(hNewWnd))))
				{
					OutputDebugString("[OnEditMsg] SearchPop 자식으로 포커스 이동 무시\n");
					break;
				}

				// 그 외 → SearchPop 닫기
				m_pSearchPop->ShowWindow(SW_HIDE);
				OutputDebugString("[OnEditMsg] SearchPop 닫기\n");
			}
		}
			break;

		case EDIT_MSG_HANGUL:     // 한영 상태 변경
			OutputDebugString("[OnEditMsg] EDIT_MSG_HANGUL\n");
			UpdateLangBtn();
			break;
		case EDIT_MSG_CHAR:
		{
			CString sText = (LPCTSTR)lp;
			OutputDebugString("[OnEditMsg] EDIT_MSG_CHAR text=" + sText + "\n");
			if (m_bSettingCode) // ← 코드 설정 중이면 무시
			{
				OutputDebugString("[OnEditMsg] EDIT_MSG_CHAR 코드설정중 무시\n");
				break;
			}

			SearchCode(sText);
		}
		break;
		case EDIT_MSG_ARROW_DOWN:
		{
			if (m_pSearchPop && m_pSearchPop->GetSafeHwnd()
				&& m_pSearchPop->IsWindowVisible()
				&& m_pSearchPop->m_pCodelist
				&& m_pSearchPop->m_pCodelist->GetSafeHwnd())
			{
				OutputDebugString("[OnEditMsg] EDIT_MSG_ARROW_DOWN SearchPop 포커스\n");

				// 첫번째 행 선택
				m_pSearchPop->m_pCodelist->SetItemStates(0,
					RC_ITEM_SELECTED | RC_ITEM_FOCUSED);
				m_pSearchPop->m_pCodelist->EnsureVisible(0, FALSE);
				m_pSearchPop->m_pCodelist->SetFocus();

				return (LRESULT)1; // 처리했음
			}
			return (LRESULT)0;
		}
		break;
		case EDIT_MSG_SHOW_HISTORYPOP:
		{
			//if(m_pSearchPop != nullptr && !m_pSearchPop->IsWindowVisible())
				PostMessage(WM_POPLISTWINDOW, POPLIST_SHOW, 0);
		}
		break;
		case EDIT_MSG_SHOW_ALLCODEPOP:
		{
			
		}
		break;
		case EDIT_MSG_KEY_DOWN:
		{
			PostMessage(WM_POPLISTWINDOW, POPLIST_SHOW, 0);
		}
		break;
		
	}
	return 0;
}

// 초성 매칭
// [기능] 입력 초성 문자열이 종목명의 초성과 일치하는지 EUC-KR 기준으로 검사
//        초성 자모(0xA4A1~0xA4BE)와 완성형 한글의 초성 범위를 비교
// [파라미터] sFindChosung: 검색할 초성 문자열 (예: "ㅅㅎ")
//           sName: 비교할 종목명 문자열
// [호출함수] BuildSearchItems()
BOOL CControlWnd::MatchChosung(CString sFindChosung, CString sName)
{
	if (sFindChosung.IsEmpty() || sName.IsEmpty()) return FALSE;

	int fi = 0; // sFindChosung 인덱스
	int ni = 0; // sName 인덱스

	while (fi < sFindChosung.GetLength() && ni < sName.GetLength())
	{
		BYTE cf1 = (BYTE)sFindChosung[fi];
		BYTE cn1 = (BYTE)sName[ni];

		// 한글 2바이트 처리
		if (cf1 >= 0x80 && fi + 1 < sFindChosung.GetLength())
		{
			BYTE cf2 = (BYTE)sFindChosung[fi + 1];
			WORD wFind = MAKEWORD(cf2, cf1); // 입력 초성

			if (cn1 >= 0x80 && ni + 1 < sName.GetLength())
			{
				BYTE cn2 = (BYTE)sName[ni + 1];
				WORD wName = MAKEWORD(cn2, cn1); // 종목명 글자

				// 완성형 한글에서 초성 추출
				if (wName >= 0xB0A1 && wName <= 0xC8FE)
				{
					// EUC-KR 완성형에서 초성 추출
					// 초성 테이블 (EUC-KR 기준)
					static const WORD chosungStart[] = {
						0xB0A1, 0xB1EE, 0xB3AA, 0xB4D9, 0xB5FB, 0xB6F3,
						0xB8B6, 0xB9D9, 0xBAFC, 0xBBE7, 0xBDCE, 0xBEC6,
						0xC0DA, 0xC2A5, 0xC2F7, 0xC4AB, 0xC5B8, 0xC6C4, 0xC7CF
					};
					static const WORD chosungEnd[] = {
						0xB1ED, 0xB3A9, 0xB4D8, 0xB5FA, 0xB6F2, 0xB8B5,
						0xB9D8, 0xBAFB, 0xBBE6, 0xBDCD, 0xBEC5, 0xC0D9,
						0xC2A4, 0xC2F6, 0xC4AA, 0xC5B7, 0xC6C3, 0xC7CE, 0xC8FE
					};
					// 입력 초성 EUC-KR 자모 테이블
					static const WORD chosungJamo[] = {
						0xA4A1, 0xA4A2, 0xA4A4, 0xA4A7, 0xA4A8, 0xA4A9,
						0xA4B1, 0xA4B2, 0xA4B3, 0xA4B5, 0xA4B6, 0xA4B7,
						0xA4B8, 0xA4B9, 0xA4BA, 0xA4BB, 0xA4BC, 0xA4BD, 0xA4BE
					};

					// wFind 가 초성 자모인지 확인
					int chosIdx = -1;
					for (int k = 0; k < 19; k++)
					{
						if (wFind == chosungJamo[k])
						{
							chosIdx = k;
							break;
						}
					}

					if (chosIdx < 0) return FALSE; // 초성 아님

					// wName 이 해당 초성 범위인지 확인
					if (wName < chosungStart[chosIdx] ||
						wName > chosungEnd[chosIdx])
						return FALSE;
				}
				else
					return FALSE;

				fi += 2;
				ni += 2;
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}

	return (fi == sFindChosung.GetLength());
}

// [기능] 입력 문자열로 전체 종목 맵 검색 후 결과를 "코드 이름\t" 형식 문자열로 반환
//        숫자→코드 앞자리 매칭, 한글→종목명/초성 매칭, 영문→대소문자 무관 앞자리 매칭
// [파라미터] sText: 검색어 문자열
// [호출함수] SearchCode()
CString CControlWnd::BuildSearchItems(CString sText)
{
	if (sText.IsEmpty()) return "";
	CString slog;
	CString sResult;

	POSITION pos = m_mapCodeToGichoName.GetStartPosition();
	CString sCode, sName;

	while (pos)
	{
		m_mapCodeToGichoName.GetNextAssoc(pos, sCode, sName);
		sName.TrimRight();
		sName.TrimLeft();

		BOOL bMatch = FALSE;

		if (IsNumber(sText[0]))
		{
			// 숫자 → 코드 앞자리 매칭
			if (sCode.Left(sText.GetLength()) == sText)
				bMatch = TRUE;
		}
		else if (IsHangul(sText))
		{
			// 한글 → 종목명 앞자리 매칭
			if (sName.Left(sText.GetLength()) == sText)
				bMatch = TRUE;

			// 초성 매칭
			if (!bMatch && MatchChosung(sText, sName))
				bMatch = TRUE;

			slog.Format("[CControlWnd] BuildSearchItems 한글 \n");
			OutputDebugString(slog);
		}
		else
		{
			// 영문 → 종목명 영문 앞자리 매칭
			CString sNameUpper = sName;
			CString sTextUpper = sText;
			sNameUpper.MakeUpper();
			sTextUpper.MakeUpper();
			if (sNameUpper.Left(sTextUpper.GetLength()) == sTextUpper)
				bMatch = TRUE;
		}
		sCode.TrimRight();
		sName.TrimRight();
		if (bMatch)
		{
			CString s;
			s.Format("%s %s\t", sCode, sName);
			sResult += s;
		}
	}


	slog.Format("[cx_combolist][CControlWnd][BuildSearchItems] text=%s result=%d\n",
		sText, sResult.IsEmpty() ? 0 : 1);
	OutputDebugString(slog);

	return sResult;
}

// [기능] 입력 문자열로 종목 검색 후 SearchPop(검색 팝업)에 결과 표시
//        팝업이 없으면 생성, 있으면 RefreshList로 갱신. 에디트 포커스 유지
// [파라미터] sText: 에디트에 입력된 검색어
// [호출함수] OnEditMsg(EDIT_MSG_CHAR)
void CControlWnd::SearchCode(CString sText)
{
	CString sItems = BuildSearchItems(sText);
	if (sText.IsEmpty())
	{
		if (0)
		{
			OutputDebugString("[SearchCode] 전종목 표시\n");
			CString sCode, sName;
			POSITION pos = m_mapCodeToGichoName.GetStartPosition();
			while (pos)
			{
				m_mapCodeToGichoName.GetNextAssoc(pos, sCode, sName);
				sCode.TrimRight();
				sName.TrimRight();
				CString s;
				s.Format("%s %s\t", sCode, sName);
				sItems += s;
			}
		}
		else
		{   //빈리스트
			if (m_pSearchPop && m_pSearchPop->GetSafeHwnd()
					&& m_pSearchPop->m_pCodelist
					&& m_pSearchPop->m_pCodelist->GetSafeHwnd())
					{
						m_pSearchPop->m_pCodelist->DeleteAllItems();
						OutputDebugString("[CControlWnd] SearchCode 결과없음 리스트 비움\n");
					}
		}
	}
	else
	{
		sItems = BuildSearchItems(sText);
	}

	// 검색 팝업 없으면 생성
	if (!m_pSearchPop || !m_pSearchPop->GetSafeHwnd())
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		CPoint ptLT(rcClient.left, rcClient.bottom);
		CPoint ptRB(rcClient.right + 100, rcClient.bottom + 200);

		ClientToScreen(&ptLT);
		ClientToScreen(&ptRB);

		CRect rcPop(ptLT.x, ptLT.y, ptRB.x, ptRB.y);

		m_pSearchPop = std::make_unique<CPopListWnd>();
		m_pSearchPop->m_nPopupType = POPUP_TYPE_SEARCH;
		m_pSearchPop->m_pParent = this;
		m_pSearchPop->SetType(VS1_TYPE);
		m_pSearchPop->CreatePopUpWindow(this, rcPop);
		m_pSearchPop->ShowWindow(SW_HIDE);
		m_pSearchPop->CreateListBox("");
		m_sItems = sItems;
		SetTimer(1818, 10, nullptr);

		OutputDebugString("[CControlWnd] -------- SearchCode 팝업 생성\n");
	}
	else
		m_pSearchPop->RefreshList(sItems);

	// 위치 재계산 후 표시
	CRect rcScreen;
	GetWindowRect(&rcScreen);
	CRect rcClient;
	GetClientRect(&rcClient);

	m_pSearchPop->SetWindowPos(
		&CWnd::wndTopMost,
		rcScreen.left,
		rcScreen.bottom,
		rcClient.Width() * 2,
		200,
		SWP_SHOWWINDOW| SWP_NOACTIVATE);

	if (m_pCodeCtrl && m_pCodeCtrl->GetSafeHwnd())
	{
		HWND hEdit = m_pCodeCtrl->GetEditSafeHwnd();
		if (hEdit)
		{
			::SetFocus(hEdit);
			// 커서 맨 끝으로
			int nLen = ::GetWindowTextLength(hEdit);
			::SendMessage(hEdit, EM_SETSEL, nLen, nLen);
		}
	}
}

LRESULT CControlWnd::OnInitPos(WPARAM wp, LPARAM lp) //OnInitPos WM_POPLISTWINDOW
{
	CRect rc;
	GetWindowRect(&rc);

	CString slog;
	slog.Format("[OnInitPos] wp=%d)\n",
		wp);
	OutputDebugString(slog);

	switch (wp)
	{
		case POPLIST_SHOW:
		{
			ShowDropList(TRUE);
			// 리스트 갱신
			if (m_pListPop && m_pListPop->m_pCodelist
				&& m_pListPop->m_pCodelist->GetSafeHwnd())
			{
				m_sItems = GetItemsString();
				m_pListPop->RefreshList(m_sItems);
				OutputDebugString("[CControlWnd] POPLIST_SHOW RefreshList\n");
			}
		}
		break;
		case POPLIST_ENTER:
		case POPLIST_DBCLICKCODE:
		{

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
			m_pListPop->m_nPopupType = POPUP_TYPE_HISTORY;
			m_pListPop->m_pParent = this;
			m_sItems = GetItemsString();
			m_pListPop->CreateListBox(m_sItems);
			m_pListPop->ShowWindow(SW_HIDE);
		}
		break;
		case POPLIST_HIDE: // 다른 곳 클릭으로 닫힘 → 버튼 누르면 바로 열려야
		{
			if (m_pSearchPop)
			{
				if (m_pSearchPop->m_nPopupType == POPUP_TYPE_SEARCH)
				{
					slog.Format("[OnInitPos] m_nPopupType=%d)\n",
						m_pSearchPop->m_nPopupType);
					OutputDebugString(slog);
					return 0;
				}
			}
			
			if (m_pListPop)
			{
				if (m_pListPop->IsWindowVisible())
					m_pListPop->ShowWindow(SW_HIDE);
			}
		}
		break;
		case POPLIST_CLEARALL:
		{
			ClearItems();
			
			if (m_pCodeCtrl && m_pCodeCtrl->GetSafeHwnd())
			{
				m_bSettingCode = true;
				m_pCodeCtrl->SetEditData("", false, false);
				m_bSettingCode = false;

				HWND hEdit = m_pCodeCtrl->GetEditSafeHwnd();
				if (hEdit)
				{
					::SetFocus(hEdit);
					int nLen = ::GetWindowTextLength(hEdit);
					::SendMessage(hEdit, EM_SETSEL, nLen, nLen);
				}
			}
		}
		break;
		case POPLIST_REMOVE:
		{
			CString scode = (LPCTSTR)lp;
			OutputDebugString("[CControlWnd] POPLIST_REMOVE code=" + scode + "\n");
			RemoveItem(scode); // 히스토리에서 삭제 + SaveHistory
		}
		break;
		case POPLIST_EDIT_FOCUS:
		{
			// 에디트 포커스 + 커서 맨 끝
			if (m_pCodeCtrl && m_pCodeCtrl->GetSafeHwnd())
			{
				HWND hEdit = m_pCodeCtrl->GetEditSafeHwnd();
				if (hEdit)
				{
					::SetFocus(hEdit);
					int nLen = ::GetWindowTextLength(hEdit);
					::SendMessage(hEdit, EM_SETSEL, nLen, nLen);
				}
			}
		}
	}

	return 0;
}
void CControlWnd::OnBtnDropClicked()
{
	PostMessage(WM_POPLISTWINDOW, POPLIST_SHOW, 0); //OnBtnDropClicked 보여준다
}

void CControlWnd::OnBtnLangClicked()
{
	OutputDebugString("[CControlWnd] OnBtnLangClicked\n");

	if (!m_pCodeCtrl) return;

	HWND hEdit = m_pCodeCtrl->GetEditSafeHwnd();
	HIMC hImc = ImmGetContext(hEdit);
	if (!hImc) return;

	DWORD dwConversion = 0, dwSentence = 0;
	ImmGetConversionStatus(hImc, &dwConversion, &dwSentence);

	// 한글 모드 체크
	BOOL bHangul = (dwConversion & IME_CMODE_HANGUL);

	CString slog;
	slog.Format("[OnBtnLangClicked] dwConversion=0x%x bHangul=%d\n",
		dwConversion, bHangul);
	OutputDebugString(slog);

	if (bHangul)
		dwConversion &= ~IME_CMODE_HANGUL; // 한글 → 영문
	else
		dwConversion |= IME_CMODE_HANGUL;  // 영문 → 한글

	ImmSetConversionStatus(hImc, dwConversion, dwSentence);
	ImmReleaseContext(hEdit, hImc);

	UpdateLangBtn();

	if (m_pCodeCtrl && m_pCodeCtrl->GetSafeHwnd())
	{
		HWND hEdit = m_pCodeCtrl->GetEditSafeHwnd();
		if (hEdit)
		{
			::SetFocus(hEdit);
			// 커서 맨 끝으로
			int nLen = ::GetWindowTextLength(hEdit);
			::SendMessage(hEdit, EM_SETSEL, nLen, nLen);
		}
	}
}

void CControlWnd::UpdateLangBtn()
{
	if (!m_pCodeCtrl || !m_pBtnLang) return;

	HWND hEdit = m_pCodeCtrl->GetEditSafeHwnd();
	HIMC hImc = ImmGetContext(hEdit);
	if (!hImc) return;

	DWORD dwConversion = 0, dwSentence = 0;
	ImmGetConversionStatus(hImc, &dwConversion, &dwSentence);
	ImmReleaseContext(hEdit, hImc);

	BOOL bHangul = (dwConversion & IME_CMODE_HANGUL);
	m_pCodeCtrl->m_bHangulMode = bHangul;
	m_pCodeCtrl->Invalidate();  // 다시 그리기


	CString sPng = bHangul
		? m_sRoot + "\\image\\axHan.png"
		: m_sRoot + "\\image\\axEng.png";

	m_pBtnLang->LoadPng(sPng, sPng, sPng);

	CString slog;
	slog.Format("[UpdateLangBtn] bHangul=%d png=%s\n", bHangul, sPng);
	OutputDebugString(slog);
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

	if (cx <= 0 || cy <= 0) return;
	if (m_szOriginal.cx <= 0 || m_szOriginal.cy <= 0) return;

	const float ratioX = (float)cx / m_szOriginal.cx;
	const int   nBtnW = (int)(m_nBtnWidthOrig * ratioX);
	 int   nLangW = (int)(m_nBtnLangWidth * ratioX);
	nLangW = 0;

	CString slog;
	slog.Format("[OnSize] cx=%d cy=%d nBtnW=%d nLangW=%d editW=%d\n",
		cx, cy, nBtnW, nLangW, cx - nBtnW - nLangW);
	OutputDebugString(slog);

	// 에디트 영역 - 버튼 두개 제외
	if (m_pCodeCtrl && m_pCodeCtrl->GetSafeHwnd())
		m_pCodeCtrl->MoveWindow(0, 0, cx - nBtnW - nLangW, cy);

	// 한영 버튼 - 드롭버튼 왼쪽
	if (m_pBtnLang && m_pBtnLang->GetSafeHwnd())
		m_pBtnLang->MoveWindow(cx - nBtnW - nLangW, 0, nLangW, cy);

	// 드롭 버튼 - 맨 오른쪽
	if (m_pBtnDrop && m_pBtnDrop->GetSafeHwnd())
		m_pBtnDrop->MoveWindow(cx - nBtnW, 0, nBtnW, cy);
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
	if (m_nGubn == GU_FCODE && m_sOption.Find("기초자산") >= 0)
	{
		CString sName;
		m_pCodeCtrl->m_pEdit->GetWindowText(sName);
		CString sCode{}, sTmpName{};
		sName.TrimRight();
		if(m_mapGichoNameToCode.Lookup(sName, sCode))
			return sCode.AllocSysString();

		return sCode.AllocSysString();
	}
	else
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
	//EnableWindow(bflags);
	if (m_pCodeCtrl->m_pEdit)
		m_pCodeCtrl->m_pEdit->EnableWindow(bflags);
	
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

		if (m_nGubn == GU_FCODE && m_sOption.Find("기초자산") >= 0)
		{
			LoadMaster();
			LoadHistory();
		}
	}
	else if (nIDEvent == 1818)
	{
		KillTimer(nIDEvent);
		m_pSearchPop->RefreshList(m_sItems);
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

		int nPopWidth = rcClient.Width() * 2;

		m_pListPop->SetWindowPos(
			&CWnd::wndTopMost,
			rcScreen.left,
			rcScreen.bottom,    // 에디트 바로 아래
			nPopWidth,   // 너비 동일
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
	m_sHistoryFile.Format("%s\\%s\\%s\\%s", m_sRoot, USRDIR, m_sUser, "userconf.ini");
	m_sSection = "IB425301";

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
	slog.Format("[cx_combolist][CControlWnd] LoadHistory data=[%s][%s][%s]\n", sData, m_sHistoryFile, m_sSection);
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

		sCode.TrimLeft();
		sCode.TrimRight();
		sName.TrimLeft();
		sName.TrimRight();

		if (m_mapItems.find(sCode) != m_mapItems.end()) continue;

		CodeItem item;
		item.sCode = sCode;
		item.sName = sName;

		m_mapItems[sCode] = (int)m_vecItems.size();
		m_vecItems.push_back(item); // ← 뒤에 추가 (순서 유지)

		slog.Format("[cx_combolist][CControlWnd] LoadHistory code=%s name=%s\n",
			sCode, sName);
		OutputDebugString(slog);
	}
	m_sItems = GetItemsString();

	// 가장 최근 종목 (첫번째) 에디트에 설정
	if (!m_vecItems.empty())
	{
		CString sCode = m_vecItems.front().sCode;
		CString sName = m_vecItems.front().sName;

		CString slog;
		slog.Format("[CControlWnd] LoadHistory 최근종목 code=%s name=%s\n",
			sCode, sName);
		OutputDebugString(slog);

		if (m_pCodeCtrl && m_pCodeCtrl->GetSafeHwnd())
		{
			m_bSettingCode = true;
			m_pCodeCtrl->SetEditData(sName, false, false);
			m_bSettingCode = false;
		}
	}
	OutputDebugString("[cx_combolist][CControlWnd] LoadHistory 완료\n");
}

void CControlWnd::SaveHistory()
{
	OutputDebugString("[cx_combolist][CControlWnd] SaveHistory\n");

	if (m_sHistoryFile.IsEmpty() || m_sSection.IsEmpty()) return;

	CString slog;
	CString sData;
	for (const auto& item : m_vecItems)
	{
		CString s;
		s.Format("%s %s\t", item.sCode, item.sName);
		slog.Format("[cx_combolist][CControlWnd] ----- [%s]\n", s);
		OutputDebugString(slog);
		sData += s;
	}

	WritePrivateProfileString(
		m_sSection,       // 섹션 ex) "4253"
		"combohistory",   // 키
		sData,            // 값
		m_sHistoryFile);  // 파일 경로
}

void CControlWnd::AddItem(CString sCode, CString sName, bool bSave /*= true*/)
{
	// 중복 체크 - 있으면 맨 위로 이동
	sCode.TrimRight();
	sCode.TrimLeft();
	sName.TrimRight();
	sName.TrimLeft();

	auto it = m_mapItems.find(sCode);
	if (it != m_mapItems.end())
	{
		int nIdx = it->second;

		// 이미 맨 위면 그냥 리턴
		if (nIdx == 0)
		{
			OutputDebugString("[cx_combolist][CControlWnd] AddItem 이미 맨위 " + sCode + "\n");
			return;
		}

		// 맨 위로 이동
		CodeItem item = m_vecItems[nIdx];
		m_vecItems.erase(m_vecItems.begin() + nIdx);
		m_vecItems.insert(m_vecItems.begin(), item);

		// map 인덱스 재정렬
		for (auto& pair : m_mapItems)
			pair.second = -1; // 초기화

		for (int i = 0; i < (int)m_vecItems.size(); i++)
			m_mapItems[m_vecItems[i].sCode] = i;

		OutputDebugString("[cx_combolist][CControlWnd] AddItem 맨위로 이동 " + sCode + "\n");

		if (bSave)
			SaveHistory();
		return;
	}

	// 신규 - 맨 위에 추가 (insert at front)
	CodeItem item;
	item.sCode = sCode;
	item.sName = sName;

	m_vecItems.insert(m_vecItems.begin(), item);

	// map 인덱스 재정렬
	for (int i = 0; i < (int)m_vecItems.size(); i++)
		m_mapItems[m_vecItems[i].sCode] = i;

	CString slog;
	slog.Format("[cx_combolist][CControlWnd] AddItem code=%s name=%s\n", sCode, sName);
	OutputDebugString(slog);

	if (bSave)
		SaveHistory();
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
	SaveHistory();
}

void CControlWnd::ClearItems()
{
	m_vecItems.clear();
	m_mapItems.clear();
	OutputDebugString("[CControlWnd] ClearItems\n");
	SaveHistory(); // 파일에도 저장
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


/*
#define GU_NONE		-1
#define	GU_CODE		0	// 주식
#define	GU_FUTURE	1	// 선물
#define	GU_OPTION	2	// 옵션
#define	GU_INDEX	3	// 업종
#define	GU_FOCODE	4	// 선물  + 옵션
#define	GU_POPTION	5	// 주식옵션
#define	GU_ELWCODE	6	// ELW
#define GU_SCODE	7	// 신주인수권
#define GU_BOND		8	// 채권(상품)
#define GU_SBOND	9	// 소매채권
#define GU_FCODE	10	// 주식선물
#define GU_FOSTOCK	11	// 주식선물 + 옵션
#define GU_FOREIGN	12	// 해외
#define GU_JCODE	13	// 수익증권
*/
void CControlWnd::LoadMaster()
{
	CFile file;
	CString path;

	switch (m_nGubn)
	{
	case GU_CODE:  // 주식
	{

	}
	break;
	case GU_FUTURE:  // 선물
	{

	}
	break;
	case GU_OPTION:  // 옵션
	{

	}
	break;
	case GU_INDEX:  // 업종
	{

	}
	break;
	case GU_FOCODE:  // 선물  + 옵션
	{

	}
	break;
	case GU_POPTION:  // 주식옵션
	{

	}
	break;
	case GU_ELWCODE: // ELW
	{

	}
	break;
	case GU_SCODE: // 신주인수권
	{

	}
	break;
	case GU_BOND: // 채권(상품)
	{

	}
	break;
	case GU_FCODE: // 10	주식선물
	{
		if (m_fjcode.size() > 0)
			return;

		CString tmp;
		path = m_sRoot + "\\tab\\sfcode.dat";
		if (file.Open(path, CFile::modeRead))
		{
			m_fjcode.resize(gsl::narrow_cast<const int>(file.GetLength() / sizeof(struct sfcode)));
			file.Read(&m_fjcode[0], m_fjcode.size() * sizeof(struct sfcode));
			file.Close();
		}

		m_mapCodeToGichoName.RemoveAll();
		// CSortStringArray sortArray;

		for (size_t i = 0;i < m_fjcode.size();i++)
		{
			CString str(m_fjcode[i].gnam, sizeof(m_fjcode[i].gnam));
			

			str.TrimLeft();
			str.TrimRight();

			if (!m_mapCodeToGichoName.Lookup(str, tmp))
			{
				CString code(m_fjcode[i].gcod, sizeof(m_fjcode[i].gcod));
				CString codx(m_fjcode[i].codx, sizeof(m_fjcode[i].codx));
				CString hname(m_fjcode[i].hnam, sizeof(m_fjcode[i].hnam));
				CString gnam(m_fjcode[i].gnam, sizeof(m_fjcode[i].gnam));

				codx.TrimRight();
				codx.TrimLeft();

				gnam.TrimRight();
				gnam.TrimLeft();

				hname.TrimRight();
				hname.TrimLeft();
				str = hname;

				if (codx.GetAt(0) == '4' || codx.GetAt(0) == 'D') //파생상품 코드개편
					str = str.Right(16).Mid(0, 9);
				else
					str = str.Right(13).Mid(0, 6);


				// sortArray.Add(str);
				 //m_NameList.AddString(str);

				 ////////////////////////////////////
				int pid{}, pid2{};
				int ival{};
				char tmp2[3] = {};
				memcpy(tmp2, codx.Mid(1, 2), 2);

				if (tmp2[0] >= 'B')
				{
					pid = 61 + (tmp2[0] - 66) * 36;

					if (tmp2[1] >= 'A' && tmp2[1] <= 'Z')
						pid2 = tmp2[1] - 55;
					else
						pid2 = tmp2[1] - 48;

					ival = pid + pid2 + 100;
				}
				else if (tmp2[0] >= '0' && tmp2[0] <= '9')
				{
					pid = (tmp2[0] - 48) * 36;

					if (tmp2[1] >= 'A' && tmp2[1] <= 'Z')
						pid2 = tmp2[1] - 55;
					else
						pid2 = tmp2[1] - 48;

					ival = pid + pid2 + 400;
				}
				else
					ival = atoi(tmp2) + 100;

				tmp.Format("[sfcode] code=[%s]  codx =[%s] str=[%s] ival=[%d] hname=[%s]  gnam=[%s]", code, codx, str, ival, hname, gnam);
				OutputDebugString(tmp);
				tmp.Format("%d", ival);
				tmp.TrimRight();
				tmp.TrimLeft();
				m_mapCodeToGichoName.SetAt(tmp, gnam.TrimRight());
				m_mapGichoNameToCode.SetAt(gnam.TrimRight(), tmp);
				/////////////////////////////////////
			}
		}
		tmp.Format("[sfcode] m_mapCodeToGichoName size=[%d]", m_mapCodeToGichoName.GetSize());
		OutputDebugString(tmp);
	}
	break;
	case GU_FOSTOCK: //11 주식선물 + 옵션
	{

	}
	break;
	case GU_FOREIGN: //12  해외
	{

	}
	break;
	case GU_JCODE: //13  수익증권
	{

	}
	break;
	}






	//sortArray.Sort();

	//if (m_NameList.GetSafeHwnd())
	//{
	//    for (int i = 0; i <= sortArray.GetUpperBound(); i++)
	//    {
	//        CString name = sortArray.GetAt(i);
	//        m_NameList.AddString(name);
	//    }
	//}
}