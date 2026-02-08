// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_codectrl.h"
#include "ControlWnd.h"
#include "symbolwnd.h"

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
	m_pSymbol	= std::make_unique<CSymbolWnd>(pWnd);
	m_pCode		= nullptr;

	m_bgColor	= GetSysColor(COLOR_WINDOW);
	m_pBtnCategory  = std::make_unique<CfxImgButton>();
	m_pBtnInter     = std::make_unique<CfxImgButton>();
	m_hCategory	= nullptr;
	m_pCategory     = nullptr;
	m_pFont		= nullptr;
	m_nFirst	= 0;
	m_bInit		= true;	

	m_szBtnSearch	= _T("");
	m_szBtnHistory  = _T("");
	m_szBtnInter	= _T("");
	m_szBtnCategory = _T("");

	// 컨트롤 모양...
	m_bLastText	= false;
	m_bCate		= false;
	m_bSymbol	= false;
	m_bInter	= false;
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
	m_pSymbol.reset();
	m_pCode.reset();
	m_pBtnCategory.reset();
	m_pBtnInter.reset();

	if (m_pCategory)
	{
		if (IsWindow(m_pCategory->GetSafeHwnd()))
			m_pCategory->SendMessage(WM_CLOSE);
		m_pCategory->Detach();
		delete m_pCategory;
	}

	if (m_hCategory)
	{
		AfxFreeLibrary(m_hCategory);
		m_hCategory = nullptr;
	}
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
	ON_BN_CLICKED(IDC_BUTTON_CATEGORY, OnBtnCode)
	ON_BN_CLICKED(IDC_BUTTON_INTER, OnBtnInter)
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
	if (m_pSymbol)
		m_pSymbol->SendMessage(WM_USER, wParam, lParam);

	CString szData = CString((TCHAR*)lParam);
	CString str;
	int	length = HIWORD(wParam);
	CString tmp;

	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		// 컨트롤에서 읽어 들일때...	===================================================================
		ASSERT(m_pSymbol);
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

			m_pCode->SetValidCheck(false);
			m_pCode->SetEditData(str, false, false);
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
			
			m_pCode->SetValidCheck(false);
			m_pCode->SetEditData((char*)lParam,false,false);
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
			m_pSymbol->Invalidate();
			m_pFont = GetAxFont(tmp, m_Param.point);
			if (m_pCode)
			{
				m_pCode->SetFont(m_pFont);
				m_pCode->ResizeCtrl();
			}
		}
		break;
		
	case formDLL:
		//=======	이화면에서 돌려보기 가로챔	========================================================================
		if (Parser(szData, "\n").CompareNoCase("\tE") == 0)
		{
			m_pCode->SetListCode(szData);
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
	dc.FillSolidRect(rc, GetIndexColor(m_bgColor));

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
		sz = dc.GetTextExtent(m_Text1.IsEmpty() ? m_pSymbol->m_szMarket : m_Text1);
		rc.right = rc.left + sz.cx + 10;
		dc.DrawText(m_Text1.IsEmpty() ? m_pSymbol->m_szMarket : m_Text1, rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		dc.SelectObject(m_pFont);

		// 두번째 라인...
		sz = dc.GetTextExtent(m_Text2.IsEmpty() ? CString(m_pSymbol->m_szUpjong) : m_Text2);
		rc.left  = rc.right;
		rc.right = rc.left + sz.cx + 10;
		dc.DrawText(m_Text2.IsEmpty() ? CString(m_pSymbol->m_szUpjong) : m_Text2, rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		
		// 세번째 라인...
		CString str = CString(m_pSymbol->m_szDanger);
		str = str.CompareNoCase("정상") == 0 ? _T("") : str;

		dc.SetTextColor(RGB(0, 0, 255));
		sz = dc.GetTextExtent(m_Text3.IsEmpty() ? str: m_Text3);
		rc.left  = rc.right;
		rc.right = rc.left + sz.cx + 10;
		dc.DrawText(m_Text3.IsEmpty() ? str: m_Text3, rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
#if 0
		dc.SetTextColor(RGB(255, 0, 0));
		str = "정리매매";
		sz = dc.GetTextExtent(str);
		rc.left  = rc.right;
		rc.right = rc.left + sz.cx;
		dc.DrawText(str, rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
#endif
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
		case 'g':
			m_bCate = true;
			break;
		case 'h':
			m_bInter = true;
			break;
		case 'i':
			m_bSymbol = false;
			//m_bSymbol = true;
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
		}
	}

	m_pSymbol->SetColor(m_boxColor, m_boxColorHover, m_bgCol, m_bgColHover);
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

	m_pCode = std::make_unique<CfxCodeCtrl>(m_pWizard, Variant(homeCC, ""));
	m_pCode->Create(this, rc, IDC_CODE);
	m_pCode->SetUnit(m_nGubn);

	CString file;
	int nType = 0;
	CString usr;

	usr = Variant(nameCC, "");
	m_sRoot = Variant(homeCC, "");

	file.Format("%s\\%s\\%s\\%s", m_sRoot, USRDIR, usr, "axisensetup.ini");
	nType = GetPrivateProfileInt("SCREEN", "CODECTRL", 0, file);

	//종목 검색 방식 선택 : 0:기존검색방식 1:중간검색방식
	m_pCode->SetSearchType(nType);

	// 버튼 이미지 변경 가능성이 높음...
	CString szSearch, szHis;
	szHis = m_szBtnHistory;
	szSearch = m_szBtnSearch;

	m_pCode->SetFont(GetAxFont(m_Param.fonts, m_Param.point));
	
	// 여기 수정 선물 옵션... 주식 기타등등...
//	CString str = Variant(historyCC, m_pCode->GetSymbol());
//	m_pCode->SetEditData(Parser(str, " "));
	
	m_pSymbol->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, IDC_SYMBOL);

	if (m_bCate)
		InitControl(CRect(0, 0, 0, 0));
	if (m_bInter) 
	{
		CString	path;
		const	int	nPos = m_szBtnInter.Find(".");

		path.Format("%s/%s/", m_sRoot, IMAGEDIR);
		m_pBtnInter->Create("관", CRect(0, 0, 0, 0), this, IDC_BUTTON_INTER, FALSE, TRUE);
		m_pBtnInter->SetImgBitmap(GetBitmap(path + m_szBtnInter), 
					  GetBitmap(path + m_szBtnInter.Left(nPos) + "_dn.bmp"), 
					  GetBitmap(path + m_szBtnInter.Left(nPos) + "_en.bmp"));	
	
		m_pBtnInter->SetCursor(GetCursor(11));
	}
	
	GetClientRect(m_ClientRect);
	Resize();
	m_pCode->SetComboBtnBmp(m_szBtnSearch, Parser(szSearch, ".") + "_dn.bmp", 
		                m_szBtnHistory, Parser(szHis, ".") + "_dn.bmp");

	
	m_pCode->HistoryMode();
	CString hcode = m_pCode->GetHistCode(0);
	if (hcode != "") SetCode(hcode);
	
	SetTimer(stockFUTURE, 350, NULL);
	return 0;
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

		width = (m_bCate ? BTN + gap: 0) + (m_bInter ? BTN + gap: 0); 
		nSymbolWidth = rc.Width() - width - CODEWIDTH;

		m_pCode->SetWindowPos(NULL, 0, 0, rc.Width()/*CODEWIDTH*/ , rc.Height(), SWP_SHOWWINDOW);
		nCodeWidth = CODEWIDTH + gap;

		if (m_bCate)
		{
			m_pCategory->SetWindowPos(NULL, nCodeWidth, 0, BTNWIDTH, (int)(BTN * rHeight), SWP_SHOWWINDOW);	
			nCodeWidth += BTNWIDTH + gap - 1;
		}
		
		if (m_bInter)
		{
			m_pBtnInter->SetWindowPos(NULL, nCodeWidth, 0, BTNWIDTH, (int)(BTN * rHeight), SWP_SHOWWINDOW);
			nCodeWidth += BTNWIDTH + gap;	
		}

		if (m_bText)
			nSymbolWidth = nSymbolWidth / 2;
		
		if (m_bSymbol)
		{
			m_pSymbol->SetWindowPos(NULL, nCodeWidth, 0, nSymbolWidth - (m_bText? 10: gap), rc.Height(), SWP_SHOWWINDOW);
			nCodeWidth += nSymbolWidth - 20 + gap;

			if (m_bText)
			{
				const	int	nTextWidth = nSymbolWidth;	//(nSymbolWidth + 10)/4 : (nSymbolWidth + 10)/3;
				m_rcText.SetRect(nCodeWidth, 0, nCodeWidth + nTextWidth, rc.bottom);
			}
			else
				m_rcText.SetRect(0, 0, 0, 0);
		}
		else	
		{
			m_pSymbol->ShowWindow(SW_HIDE);
			const	int	nTextWidth = m_bLastText ? (rc.Width() - nCodeWidth)/4 : (rc.Width() - nCodeWidth)/3;
			m_rcText.SetRect(nCodeWidth, 0, nCodeWidth + nTextWidth, rc.bottom);
		}
	}
	else 
	{
		m_pCode->SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
		if (m_pSymbol)
			m_pSymbol->ShowWindow(SW_HIDE);
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
	m_pSymbol->m_sCode = szCode;
	trData.Format("1301%c%s%c1301%c17413%c", gnCHAR, szCode, tabCHAR, tabCHAR, tabCHAR);

	SendTRC("POOPPOOP", trData, US_OOP);
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CAxWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	Resize();
}

void CControlWnd::OnBtnCode()
{
}
	
void CControlWnd::OnBtnInter()
{
	CString str;
	CRect rc;
	m_pBtnInter->GetWindowRect(rc);
	str.Format("HD702001/S/p5/d appendCODE\t%s\n", m_rtnStr);
	OpenView(str, typePOPUP);
}

void CControlWnd::InitControl(CRect rect)
{
	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*) = nullptr;

	text.Format("%s/%s/cx_category.dll", Variant(homeCC, ""), DEVDIR);
	m_hCategory = AfxLoadLibrary(text);
	if (m_hCategory)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hCategory, _T("axCreate"));
		if (axCreate)
		{
			struct	_param	param;

			param.key   = 0;
			param.name  = _T("category");
			param.rect  = rect;
			param.fonts = "굴림체";
			param.point = 9;
			param.style = 1;
			param.tRGB  = 63;
			param.pRGB  = 62;
			param.options = _T("/i" + m_szBtnCategory);

			m_pCategory = (*axCreate)(this, &param);
//			m_pCategory->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			if (m_pCategory == nullptr)
			{
				AfxFreeLibrary(m_hCategory);
				m_hCategory = nullptr;
			}
		}
	}
	m_pSymbol->ShowWindow(SW_HIDE);
}

void CControlWnd::SetEditFocus(BOOL bflag)
{
	ASSERT(m_pCode);
	if (m_bFocus || bflag)
		m_pCode->SetEditFocus();
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
	if (szSymbol.CompareNoCase(m_pCode->GetSymbol()) == 0)
		m_pCode->SetEditData(str);
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
	if (!m_pCode || m_rtnStr.CompareNoCase(code) == 0)
		return;

	Msg("SetData(%s)", code);
	ASSERT(m_pCode);
	m_pCode->SetEditData(code, true);
	m_pCode->m_bFirst = true;
}

void CControlWnd::SetCode(LPCTSTR code) 
{
	if (!m_pCode || m_rtnStr.CompareNoCase(code) == 0)
		return;
	Msg("SetCode(%s)", code);
	m_bModify = true;
	m_pCode->SetEditData(code);
}


void CControlWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CAxWnd::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here
	
	if (pOldWnd != m_pCode->m_pEdit.get() && pOldWnd != this)
	{
		m_pCode->SetEditFocus();		
	}
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
		m_pCode->EnableWindow(bflags);
		m_pCode->m_pEdit->EnableWindow(bflags);
		//m_pCode->m_pEdit->ShowWindow(SW_SHOW);	
	}
	else
	{
		//m_pCode->m_pEdit->SetWindowText("asdf");
		EnableWindow(bflags);
		m_pCode->EnableWindow(bflags);
		m_pCode->m_pEdit->EnableWindow(bflags);
		//m_pCode->m_pEdit->ShowWindow(SW_HIDE);
	}
	Invalidate();
}

void CControlWnd::Clear() 
{
	// TODO: Add your dispatch handler code here
	if (m_pSymbol)
		m_pSymbol->Clear();
	//SetCode("");
	m_pCode->SetEditData("", true);
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
			CString str = Variant(historyCC, m_pCode->GetSymbol());
			m_pCode->SetEditData(Parser(str, " "));
			m_pCode->SetEditData(Parser(str, " "));
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
	m_pCode->RemoveCode();

	m_pCode->SetUnit(unit);

	m_pCode->HistoryMode();
	CString hcode = m_pCode->GetHistCode(0);
	if (hcode != "") SetCode(hcode);
}

short CControlWnd::GetUnitData() 
{
	return m_pCode->m_Unit;
}

void CControlWnd::SetUnitData(short nNewValue) 
{
	m_pCode->RemoveCode();
	
	m_nGubn = nNewValue;

	m_pCode->SetUnit(nNewValue);
	
	m_pCode->HistoryMode();
	CString hcode = m_pCode->GetHistCode(0);
	if (hcode != "") SetCode(hcode);
}

BOOL CControlWnd::IsKonex() 
{
	return m_pCode->m_bKONEX;
}
