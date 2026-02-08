// ModifyWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100700.h"
#include "ModifyWnd.h"
#include "MapWnd.h"
#include "../../control/fx_misc/misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyWnd

CModifyWnd::CModifyWnd(CWnd* pWizard)
	: m_pWizard(pWizard)
{
}

CModifyWnd::~CModifyWnd()
{
/*	_SAFEDESTROY(m_pGroupStatic);
	_SAFEDESTROY(m_pGroup);
	_SAFEDESTROY(m_pGroupID);
	_SAFEDESTROY(m_pAddAccount);
	_SAFEDESTROY(m_pPassSet);
	_SAFEDESTROY(m_pClear);
	_SAFEDESTROY(m_pNonContract);
	_SAFEDESTROY(m_pOrder);
	_SAFEDESTROY(m_pContract);
	_SAFEDESTROY(m_pCodeCtrl);
	_SAFEDESTROY(m_pCodeSearch);
	_SAFEDESTROY(m_pSymbol);
	_SAFEDESTROY(m_pTypeStatic);
	_SAFEDESTROY(m_pType);
	_SAFEDESTROY(m_pQuantityStatic);
	_SAFEDESTROY(m_pQuantity);
	_SAFEDESTROY(m_pCostStatic);
	_SAFEDESTROY(m_pCostType);
	_SAFEDESTROY(m_pCost);
	_SAFEDESTROY(m_pModifyGrid);
	_SAFEDESTROY(m_pContractGrid);
	_SAFEDESTROY(m_pOrderTypeStatic);
	_SAFEDESTROY(m_pOrderType);
	_SAFEDESTROY(m_pCondition);
*/
}


BEGIN_MESSAGE_MAP(CModifyWnd, CWnd)
	//{{AFX_MSG_MAP(CModifyWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADDACCOUNT, OnAddAccount)
	ON_BN_CLICKED(IDC_CODESEARCH, OnCodeSearch)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_NONCONTRACT, OnNonContract)
	ON_BN_CLICKED(IDC_ORDER, OnOrder)
	ON_BN_CLICKED(IDC_CONTRACT, OnContract)
	ON_CBN_SELENDOK(IDC_GROUP, OnSelGroup)
	ON_CBN_SELENDOK(IDC_CODECTRL, OnCodeSelEndOk)
	ON_CBN_SELENDOK(IDC_COSTTYPE, OnCostTypeSelEndOk)
	ON_CBN_SELENDOK(IDC_TYPE, OnTypeSelEndOk)
	ON_MESSAGE(UG_CONT_CELLCHANGE, OnContractGridRowChange)
	ON_EN_CHANGE(IDC_COST, OnCost)
	ON_BN_CLICKED(IDC_PASSSET, OnPassSet)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CModifyWnd message handlers

int CModifyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pParent = (CMapWnd*)GetParent();

	CRect rcCtrl, rcClient;
	GetClientRect(&rcClient);
	rcCtrl.SetRect(SZ_TAB_HMARGIN, SZ_FORM_MARGIN, SZ_TAB_HMARGIN + SZ_CTRL_2STATIC, SZ_FORM_MARGIN + SZ_CTRL_HEIGHT);
	
	m_pGroupStatic = std::make_unique<CfxStatic>();
	m_pGroupStatic->Create(_T("그룹"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_GROUPSTATIC);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_7WIDTH;

	m_pGroup = std::make_unique<CComboBox>();
	m_pGroup->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_GROUP);
	SetWindowTheme(m_pGroup->GetSafeHwnd(), L"", L"");

	m_pGroupID = std::make_unique<CComboBox>();
	m_pGroupID->Create(WS_CHILD | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_GROUPID);
	SetWindowTheme(m_pGroupID->GetSafeHwnd(), L"", L"");

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + BTN4_WIDTH;

	m_pAddAccount = std::make_unique<CfxImgButton>();
	m_pAddAccount->Create("\n계좌등록", rcCtrl, this, IDC_ADDACCOUNT, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + BUTTON_SPACE;
	rcCtrl.right = rcCtrl.left + BTN8_WIDTH;

	m_pPassSet = std::make_unique<CfxImgButton>();
	m_pPassSet->Create("\n계좌비밀번호저장", rcCtrl, this, IDC_PASSSET, FALSE, TRUE);

	rcCtrl.left = rcClient.right - SZ_TAB_HMARGIN - SZ_CTRL_3BUTTON - SZ_CTRL_4BUTTON - SZ_CTRL_2BUTTON * 2 - 6;
	rcCtrl.left = rcClient.right -BTN3_WIDTH -BTN2_WIDTH-BTN4_WIDTH-BTN2_WIDTH-CTRL_SPACE_W;
	rcCtrl.right = rcCtrl.left + BTN3_WIDTH;

	m_pClear = std::make_unique<CfxImgButton>();
	m_pClear->Create("\nClear", rcCtrl, this, IDC_CLEAR, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + BUTTON_SPACE;
	rcCtrl.right = rcCtrl.left + BTN2_WIDTH;

	m_pNonContract = std::make_unique<CfxImgButton>();
	m_pNonContract->Create("\n조회", rcCtrl, this, IDC_NONCONTRACT, FALSE, TRUE);
	
	rcCtrl.left = rcCtrl.right + BUTTON_SPACE;
	rcCtrl.right = rcCtrl.left + BTN4_WIDTH;

	m_pOrder = std::make_unique<CfxImgButton>();
	m_pOrder->Create("\n일괄정정", rcCtrl, this, IDC_ORDER, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + BUTTON_SPACE;
	rcCtrl.right = rcCtrl.left + BTN2_WIDTH;

	m_pContract = std::make_unique<CfxImgButton>();
	m_pContract->Create("\n체결", rcCtrl, this, IDC_CONTRACT, FALSE, TRUE);
	
	rcCtrl.left = SZ_TAB_HMARGIN;
	rcCtrl.right = SZ_COMBO_3WIDTH;
	rcCtrl.top = rcCtrl.bottom + CTRL_SPACE;
	rcCtrl.bottom = rcCtrl.top + SZ_CTRL_HEIGHT;

	m_pCodeCtrl = std::make_unique<CfxCodeCtrl>(this, CString(m_pParent->GetAxRoot() + "\\tab"));
	m_pCodeCtrl->Create(this, rcCtrl, IDC_CODECTRL);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + BTN1_WIDTH;

	m_pCodeSearch = std::make_unique<CfxImgButton>();
	m_pCodeSearch->Create("\n호가", rcCtrl, this, IDC_CODESEARCH, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_SYMBOL;

	InitSymbol(rcCtrl);

	rcCtrl.left = rcCtrl.right + /*SZ_CTRL_HGAP*/  1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;
	
	m_pTypeStatic = std::make_unique<CfxStatic>();
	m_pTypeStatic->Create(_T("구분"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_TYPESTATIC);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;					// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_COMBO_2WIDTH;

	m_pType = std::make_unique<CComboBox>();
	m_pType->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_TYPE);
//asdfasdf
	SetWindowTheme(m_pType->GetSafeHwnd(), L"", L"");

	rcCtrl.left = rcCtrl.right + /*SZ_CTRL_HGAP +*/ CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pQuantityStatic = std::make_unique<CfxStatic>();
	m_pQuantityStatic->Create(_T("수량"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_QUANTITYSTATIC);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_2WIDTH;

	m_pQuantity = std::make_unique<CComboBox>();
	m_pQuantity->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_QUANTITY);

	SetWindowTheme(m_pQuantity->GetSafeHwnd(), L"", L"");

//asdfasdf
	rcCtrl.left = rcCtrl.right + /*SZ_CTRL_HGAP*/  CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;
	
	m_pOrderTypeStatic = std::make_unique<CfxStatic>();
	m_pOrderTypeStatic->Create(_T("유형"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_ORDERTYPESTATIC);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_6WIDTH-30;

	m_pOrderType = std::make_unique<CComboBox>();
	m_pOrderType->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_ORDERTYPE);
	SetWindowTheme(m_pOrderType->GetSafeHwnd(), L"", L"");

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_2WIDTH;
	
	m_pCondition = std::make_unique<CComboBox>();
	m_pCondition->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_CONDITION);

	SetWindowTheme(m_pCondition->GetSafeHwnd(), L"", L"");
	rcCtrl.left = rcCtrl.right + 3;//rcClient.right - SZ_CTRL_2STATIC - SZ_COMBO_4WIDTH - SZ_CTRL_6EDIT - 7+20;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pCostStatic = std::make_unique<CfxStatic>();
	m_pCostStatic->Create(_T("단가"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_COSTSTATIC);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_COMBO_4WIDTH;
	
	m_pCostType = std::make_unique<CComboBox>();
	m_pCostType->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_COSTTYPE);
	SetWindowTheme(m_pCostType->GetSafeHwnd(), L"", L"");

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcClient.right - CTRL_SPACE;//rcCtrl.left + SZ_CTRL_6EDIT-20;

	m_pCost = std::make_unique<CfxEdit>();
	m_pCost->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | ES_RIGHT, rcCtrl, this, IDC_COST);
	m_pCost->SetPasswordChar(0);

	rcCtrl.left = 0;
	rcCtrl.right = rcCtrl.left + INNER_FRAME_SPACE;
	rcCtrl.top = rcCtrl.bottom + CTRL_SPACE;
	rcCtrl.bottom = rcClient.bottom;

	m_pContractGrid = std::make_unique<CContractGrid>();
	m_pContractGrid->CreateGrid(WS_CHILD, rcCtrl, this, IDC_CONTRACTGRID);
	
	rcCtrl.left = 0;
	rcCtrl.right = rcClient.right - INNER_FRAME_SPACE;
	m_pModifyGrid = std::make_unique<CModifyGrid>();
	m_pModifyGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_MODIFYGRID);

	return 0;
}

void CModifyWnd::Init()
{
	CFont* pFont = m_pParent->GetNormalFont();

	m_pGroupStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pGroupStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pGroupStatic->SetFont(pFont);

	m_pGroup->SetFont(pFont);

	CString strPath = "";
	strPath.Format("%s\\Image\\", m_pParent->GetAxRoot());

	HBITMAP hbmp	= m_pParent->GetAxBitmap(strPath + "4btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= m_pParent->GetAxBitmap(strPath + "4btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = m_pParent->GetAxBitmap(strPath + "4btn_en.bmp")->operator HBITMAP();

	m_pAddAccount->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pAddAccount->SetWindowText(_T("계좌등록"));
	m_pAddAccount->SetFont(pFont);
	
	hbmp	= m_pParent->GetAxBitmap(strPath + "3btn.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "3btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "3btn_en.bmp")->operator HBITMAP();
	m_pClear->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pClear->SetWindowText(_T("Clear"));
	m_pClear->SetFont(pFont);

	hbmp	= m_pParent->GetAxBitmap(strPath + "4btn.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "4btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "4btn_en.bmp")->operator HBITMAP();

	m_pOrder->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pOrder->SetWindowText(_T("일괄정정"));
	m_pOrder->SetFont(pFont);

	hbmp	= m_pParent->GetAxBitmap(strPath + "2btn.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "2btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "2btn_en.bmp")->operator HBITMAP();

	m_pNonContract->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pNonContract->SetWindowText(_T("조회"));
	m_pNonContract->SetFont(pFont);

	m_pContract->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pContract->SetWindowText(_T("체결"));
	m_pContract->SetFont(pFont);

	m_pCodeCtrl->SetUnit(GU_CODE);
	m_pCodeCtrl->SetFont(pFont);
	m_pCodeCtrl->SetHistory(m_pParent->Variant(historyCC, _T("1301")));

	hbmp	= m_pParent->GetAxBitmap(strPath + "s검색.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "s검색_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "s검색_en.bmp")->operator HBITMAP();

	m_pCodeSearch->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pCodeSearch->SetFont(pFont);

	hbmp	= m_pParent->GetAxBitmap(strPath + "8btn.bmp")->operator HBITMAP();
	hbmp_dn	= m_pParent->GetAxBitmap(strPath + "8btn_dn.bmp")->operator HBITMAP();
	hbmp_hv = m_pParent->GetAxBitmap(strPath + "8btn_en.bmp")->operator HBITMAP();
	m_pPassSet->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pPassSet->SetWindowText(_T("계좌비밀번호저장"));
	m_pPassSet->SetFont(pFont);

	m_pTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pTypeStatic->SetFont(pFont);

	m_pType->AddString(_T("전체"));
	m_pType->AddString(_T("매도"));
	m_pType->AddString(_T("매수"));
	m_pType->SetCurSel(0);
	m_pType->SetFont(pFont);

	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pQuantityStatic->SetFont(pFont);
	
	m_pQuantity->AddString(_T("잔량"));
	m_pQuantity->AddString(_T("일부"));
	m_pQuantity->SetCurSel(0);
	m_pQuantity->SetFont(pFont);

	m_pOrderTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pOrderTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pOrderTypeStatic->SetFont(pFont);

	m_pOrderType->AddString(_T("지정가"));
	m_pOrderType->AddString(_T("시장가"));
	m_pOrderType->AddString(_T("조건부지정가"));
	m_pOrderType->AddString(_T("최유리지정가"));
	m_pOrderType->AddString(_T("최우선지정가"));
	m_pOrderType->AddString(_T("장개시전 시간외종가"));
	m_pOrderType->AddString(_T("시간외종가"));
	m_pOrderType->SetDroppedWidth(80);
	m_pOrderType->SetCurSel(0);
	m_pOrderType->SetFont(pFont);

	m_pCondition->AddString(_T("없음"));
	m_pCondition->AddString(_T("FOK"));
	m_pCondition->AddString(_T("IOC"));
	m_pCondition->SetCurSel(0);
	m_pCondition->SetFont(pFont);

	m_pCostStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pCostStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pCostStatic->SetFont(pFont);

	m_pCostType->AddString(_T("지정가"));
	m_pCostType->AddString(_T("현재가"));
	m_pCostType->AddString(_T("매도호가"));
	m_pCostType->AddString(_T("매수호가"));
	m_pCostType->AddString(_T("상한가"));
	m_pCostType->AddString(_T("하한가"));

	m_pCostType->SetCurSel(1);
	m_pCostType->SetFont(pFont);

	m_pCost->ModifyAttribute(0, attrNUMERIC);
	m_pCost->SetFont(pFont);

	m_pContractGrid->SetFont(pFont);
	m_pModifyGrid->SetFont(pFont);
}

void CModifyWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	CRect rcClient, rcCtrl, rcTmp;
	GetClientRect(&rcClient);

	m_pCostType->GetWindowRect(&rcCtrl);
	ScreenToClient(rcCtrl);

	rcTmp = rcClient;
	rcTmp.bottom = rcCtrl.bottom +INNER_FRAME_SPACE;
	dc.FillSolidRect(&rcTmp, pMapWnd->GetIndexColor(CMD_BACK_INDEX));

	rcTmp = rcClient;
	rcTmp.top = rcCtrl.bottom +INNER_FRAME_SPACE;
	dc.FillSolidRect(&rcTmp, pMapWnd->GetIndexColor(CONTENTS_BACK_INDEX));
}

void CModifyWnd::InitSymbol(CRect rcCtrl)
{
	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*) = nullptr;

	text.Format("%s/dev_symbol.dll", m_pParent->GetAxRoot());
	m_hiSymbol = AfxLoadLibrary(text);

	rcCtrl.OffsetRect(0, SZ_CTRL_HEIGHT + SZ_FORM_MARGIN);
	if (m_hiSymbol != NULL)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiSymbol, _T("axCreate"));
		if (axCreate != NULL)
		{
			struct	_param	param;

			m_pWizard->ClientToScreen(&rcCtrl);
			ScreenToClient(&rcCtrl);

			CRect rc;
			m_pCodeSearch->GetWindowRect(&rc);
			ScreenToClient(&rc);
			rcCtrl.top = rc.top ;
			rcCtrl.bottom = rc.bottom ;

			param.key   = 0;
			param.name  = _T("17413");
			param.rect  = rcCtrl;
			param.fonts = "굴림체";
			param.point = 9;
			param.style = 1;
			param.tRGB  = 69;
			param.pRGB  = 90;
			param.options = _T("/a89/b91/c92/d69/i99");

			m_pSymbol = (*axCreate)(m_pWizard, &param);
			m_pSymbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

			m_pSymbol->SetParent(this);

			if (m_pSymbol == NULL)
			{
				AfxFreeLibrary(m_hiSymbol);
				m_hiSymbol = NULL;
			}
		}
	}
}

void CModifyWnd::OnAddAccount()
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 2), (LPARAM)_T("IB100701 /p5 /S"));
	ClearModifyGrid();
	m_pParent->QueryGroupList();
}

void CModifyWnd::OnCodeSearch()
{
	CRect rcCtrl;
	m_pCodeSearch->GetClientRect(rcCtrl);
	m_pCodeSearch->ClientToScreen(&rcCtrl);
	CString strCodeData = (char*) m_pWizard->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(allCODE, NULL)), MAKELPARAM(rcCtrl.left, rcCtrl.bottom));
	
	if (!strCodeData.IsEmpty() && strCodeData.GetLength() > 6)
	{
		CString strCode = strCodeData.Left(6);
		if (m_pCodeCtrl)
			m_pCodeCtrl->SetEditData(strCode);

		QuerySymbol(strCode);
	}
}

void CModifyWnd::QuerySymbol(CString strCode)
{
	CString strText;
	strText.Format("1301%c%s\t17413\t", 0x7f, strCode);
	m_pParent->SendTR(_T("POOPPOOP"), strText, KEY_SYMBOL, 0);

	m_pParent->SetCode(strCode);

	CString strProc;
	strProc.Format("change_code\t%s", strCode);
	m_pParent->Procedure(strProc);
}


void CModifyWnd::OnCodeSelEndOk()
{
	CString strCode = m_pCodeCtrl->GetEditData();
	QuerySymbol(strCode);
}

void CModifyWnd::OnSelGroup()
{
	OnNonContract();
}

void CModifyWnd::ParsingSymbol(WPARAM wParam, LPARAM lParam)
{
	if (m_pSymbol)
		m_pSymbol->SendMessage(WM_USER, wParam, lParam);

	OnNonContract();
}

void CModifyWnd::OnClear()
{
	ClearContractGrid();
	ClearModifyGrid();
}

void CModifyWnd::ClearModifyGrid()
{
	CUGCell cell;
	m_pModifyGrid->SetNumberRows(DEF_SELLGRIDROW);

	for (int i = 0; i < MAX_MODIFYGRIDCOL; i++)
	{
		for (int j = 0; j < DEF_MODIFYGRIDROW; j++)
		{
			m_pModifyGrid->GetCell(i, j, &cell);
			cell.ClearProperty(UGCELL_TEXT_SET);
			m_pModifyGrid->SetCell(i, j, &cell);
		}
	}

	m_pModifyGrid->GotoRow(0);
	m_pModifyGrid->RedrawAll();
}

void CModifyWnd::ClearContractGrid()
{
	CUGCell cell;
	m_pContractGrid->SetNumberRows(DEF_SELLGRIDROW);

	for (int i = 0; i < MAX_CONTRACTGRIDCOL; i++)
	{
		for (int j = 0; j < DEF_CONTRACTGRIDROW; j++)
		{
			m_pContractGrid->GetCell(i, j, &cell);
			cell.ClearProperty(UGCELL_TEXT_SET);
			m_pContractGrid->SetCell(i, j, &cell);
		}
	}

	m_pContractGrid->GotoRow(0);
	m_pContractGrid->RedrawAll();
}

void CModifyWnd::OnNonContract()
{
	OnClear();
	ClearContractGrid();

	CString strGroupID(_T(""));
	CString strCode(_T(""));
	CString strDept(_T(""));
	CString strUser(_T(""));
	CString strSendData(_T(""));

	if (m_pGroup->GetCurSel() == -1)
		return;
	else
		strGroupID.Format("%03d", m_pGroup->GetItemData(m_pGroup->GetCurSel()));

	m_pGroupID->SetCurSel(m_pGroup->GetCurSel());
	//m_pGroupID->GetWindowText(strGroupID);
	//AfxMessageBox(strGroupID);

	_TRIMDATA(strGroupID);

	BOOL bCustomer = FALSE;
	struct _ledgerH ledger;
	struct gntn_mid mid {};

	strDept = m_pParent->Variant(deptCC);
	_TRIMDATA(strDept);

	strUser = m_pParent->Variant(userCC, "");
	_TRIMDATA(strUser);

	if (m_pParent->IsCustomer())
		bCustomer = TRUE;

	FillMemory(&ledger, L_ledgerH, ' ');

	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(ledger.svcd, _T("SONAQ015"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(ledger.brno, (LPCTSTR)strDept, strDept.GetLength());
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	strSendData = CString((char*)&ledger, L_ledgerH);

	FillMemory(&mid, L_gntn_mid, ' ');

	CopyMemory(mid.In, _T("00001"), sizeof(mid.In));

	if (bCustomer)
		mid.zUserTp[0] = _T('2');
	else
		mid.zUserTp[0] = _T('1');
	
	CopyMemory(mid.zCreUserId, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(mid.zFundNm, (LPCTSTR)strGroupID, strGroupID.GetLength());
	CopyMemory(mid.zFundKindTp, _T("2 "), sizeof(mid.zFundKindTp));
	CopyMemory(mid.lFundOrdNo, _T("+000000000"), sizeof(mid.lFundOrdNo));

	SYSTEMTIME st;
	GetLocalTime(&st);
	
	CString strTime(_T(""));
	strTime.Format("%04d%02d%02d", st.wYear, st.wMonth, st.wDay);

	CopyMemory(mid.zOrdDt, (LPCTSTR)strTime, strTime.GetLength());

	strSendData += CString((char*)&mid, L_gntn_mid);
	
	m_pParent->SendTR("PIBOPBXQ", strSendData, KEY_MODIFYNONCONTRACT, 0);
}

void CModifyWnd::QueryNCDetail(CString strFundNo)
{
	ClearModifyGrid();

	CString strGroupID(_T(""));
	CString strCode(_T(""));
	CString strDept(_T(""));
	CString strUser(_T(""));
	CString strSendData(_T(""));

	if (m_pGroup->GetCurSel() == -1)
		return;
	else
		strGroupID.Format("%03d", m_pGroup->GetItemData(m_pGroup->GetCurSel()));
	m_pGroupID->SetCurSel(m_pGroup->GetCurSel());
	m_pGroupID->GetWindowText(strGroupID);

	_TRIMDATA(strGroupID);

	BOOL bCustomer = FALSE;
	struct _ledgerH ledger;
	struct goqry_mid mid {};

	strDept = m_pParent->Variant(deptCC);
	_TRIMDATA(strDept);

	strUser = m_pParent->Variant(userCC, "");
	_TRIMDATA(strUser);

	if (m_pParent->IsCustomer())
		bCustomer = TRUE;

	FillMemory(&ledger, L_ledgerH, ' ');

	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(ledger.svcd, _T("SONAQ041"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(ledger.brno, (LPCTSTR)strDept, strDept.GetLength());
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	strSendData = CString((char*)&ledger, L_ledgerH);

	FillMemory(&mid, L_goqry_mid, ' ');

	CopyMemory(mid.In, _T("00001"), sizeof(mid.In));

	if (bCustomer)
		mid.zUserTp[0] = _T('2');
	else
		mid.zUserTp[0] = _T('1');

	CopyMemory(mid.zCreUserId, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(mid.zFundNm, (LPCTSTR)strGroupID, strGroupID.GetLength());
	CopyMemory(mid.zFundKindTp, _T("2 "), sizeof(mid.zFundKindTp));

	CString strGroupNo(_T(""));
	strGroupNo.Format("+%09d", atoi((LPCTSTR)strFundNo));
	CopyMemory(mid.lFundOrdNo, (LPCTSTR)strGroupNo, strGroupNo.GetLength());

	SYSTEMTIME st;
	GetLocalTime(&st);
	
	CString strTime(_T(""));
	strTime.Format("%04d%02d%02d", st.wYear, st.wMonth, st.wDay);

	CopyMemory(mid.zOrdDt, (LPCTSTR)strTime, strTime.GetLength());

	strSendData += CString((char*)&mid, L_goqry_mid);
	
	m_pParent->SendTR("PIBOPBXQ", strSendData, KEY_MODIFYNCDETAIL, 0);
}

void CModifyWnd::OnOrder()
{
	m_arrSendData.RemoveAll();
	m_pModifyGrid->GetSendData(&m_arrSendData);
	m_nCurOrder = 0;

	SendOrder();
}

void CModifyWnd::OnContract()
{
	CRect  rcMenu;
	std::unique_ptr<CMenu> pMenu = std::make_unique<CMenu>();
	pMenu->CreatePopupMenu();
	pMenu->AppendMenu(MF_STRING, 0, "체결조회");
	pMenu->AppendMenu(MF_STRING, 1, "미체결조회");

	m_pContract->GetWindowRect(&rcMenu);

	const int nSelectGroup = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
								rcMenu.left, rcMenu.bottom, this);
	pMenu = nullptr;

	CString strData(_T(""));

	if (nSelectGroup == 1)
		strData = _T("IB110100 /S/t");
	else if (nSelectGroup == 0)
		strData = _T("IB110200 /S/t");

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

CComboBox* CModifyWnd::GetGroupCombo()
{
	return m_pGroup.get();
}

CComboBox* CModifyWnd::GetGroupIDCombo()
{
	return m_pGroupID.get();
}

void CModifyWnd::SetCode(CString strCode)
{
	if (m_pCodeCtrl == NULL)
		return;

	m_pCodeCtrl->SetEditData(strCode);
	QuerySymbol(strCode);
}


CUGCtrl* CModifyWnd::GetGridWnd()
{
	return (CUGCtrl*)m_pModifyGrid.get();
}

HBRUSH CModifyWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
    const UINT nID = pWnd->GetDlgCtrlID();

    if (nID == IDC_REMAINQTY || nID == IDC_PARTQTY)
    {    
		pDC->SetBkMode(TRANSPARENT);                              

		return (HBRUSH)::GetStockObject(NULL_BRUSH);                      
    }

	// TODO: Return a different brush if the default is not desired
	return hbr;

}


void CModifyWnd::ParsingNonContract(CString strData, int nLen, CString strMsg)
{
	strData.Delete(0, L_gntn_mid + 105);

	int nRec = atoi((LPCTSTR)strData.Left(5));
	strData.Delete(0, 5);

	CString strFundOdrNo(_T(""));
	CString strFundOrgOdrNo(_T(""));
	CString strModCanType(_T(""));
	CString strExecQty(_T(""));
	CString strOrdQty(_T(""));
	CString strTrxTime(_T(""));
	CString strAbleQty(_T(""));
	CString strTemp(_T(""));

	int nCurRow = 0;

	for (int i = 0; i < nRec; i++)
	{
		struct gntn_rec rec;
		CopyMemory(&rec, (LPCTSTR)strData.Left(L_gntn_rec), L_gntn_rec);

		strTemp = CString(rec.lFundOrdNo, sizeof(rec.lFundOrdNo));
		strTemp.TrimLeft(_T("+-0"));
		if (strTemp.IsEmpty())
			strTemp = _T("0");
		strFundOdrNo.Format("%03d", atoi((LPCTSTR)strTemp));
		
		strFundOrgOdrNo = CString(rec.lFundOrgOrdNo, sizeof(rec.lFundOrgOrdNo));
		strFundOrgOdrNo.TrimLeft(_T("+-0"));
		
		strModCanType = CString(rec.zMrcTp, sizeof(rec.zMrcTp));
		
		strExecQty = CString(rec.lExecQty, sizeof(rec.lExecQty));
		strExecQty.TrimLeft(_T("+-0"));
		if (strExecQty.IsEmpty())
			strExecQty = _T("0");
		
		strOrdQty = CString(rec.lOrdQty, sizeof(rec.lOrdQty));
		strOrdQty.TrimLeft(_T("+-0"));
		if (strOrdQty.IsEmpty())
			strOrdQty = _T("0");
		
		strTrxTime = CString(rec.zTrxTime, sizeof(rec.zTrxTime));
		
		strAbleQty = CString(rec.lMrcAbleQty, sizeof(rec.lMrcAbleQty));
		strAbleQty.TrimLeft(_T("+-0"));
		if (strAbleQty.IsEmpty())
			strAbleQty = _T("0");
		m_pParent->FormatThousand(strAbleQty);

		const int nNonContQty = atoi(strAbleQty);	//abs(atoi((LPCTSTR)strExecQty) - atoi((LPCTSTR)strOrdQty));

		if (nNonContQty > 0)
		{		
			strTemp.Format("%d", nNonContQty);
			m_pParent->FormatThousand(strTemp);
			
			m_pContractGrid->QuickSetText(0, nCurRow, (LPCTSTR)strFundOdrNo);
			m_pContractGrid->QuickSetText(1, nCurRow, (LPCTSTR)strTemp);

			if (strModCanType == _T("0"))
				m_pContractGrid->QuickSetText(2, nCurRow, _T("정상"));
			else if (strModCanType == _T("1"))
				m_pContractGrid->QuickSetText(2, nCurRow, _T("정정"));
			else if (strModCanType == _T("2"))
				m_pContractGrid->QuickSetText(2, nCurRow, _T("취소"));

			m_pContractGrid->QuickSetText(3, nCurRow, strFundOrgOdrNo);
			m_pContractGrid->RedrawRow(nCurRow++);
		}

		strData.Delete(0, L_gntn_rec);
	}

	if (nRec > 0)
		OnContractGridRowChange(0, 0);
}

void CModifyWnd::ParsingNCDetail(CString strData, int nLen, CString strMsg)
{
	int nRec = atoi((LPCTSTR)strData.Mid(L_goqry_mid, 5));
	strData.Delete(0, L_goqry_mid + 5);

	CString strAccount(_T(""));
	CString strAccName(_T(""));
	CString strCode(_T(""));
	CString strCodeName(_T(""));
	CString strOrderQty(_T(""));
	CString strExecQty(_T(""));
	CString strNonContQty(_T(""));
	CString strOrderPrice(_T(""));
	CString strOrderNo(_T(""));
	CString strTemp(_T(""));
	CString strModifyPrice(_T(""));
	CString strMrcQty(_T(""));

	CString strQCode = m_pCodeCtrl->GetEditData();
	_TRIMDATA(strQCode);

	m_pCost->GetWindowText(strModifyPrice);
	_TRIMDATA(strModifyPrice);
	
	int nCurRow = 0;

	for (int i = 0; i < nRec; i++)
	{
		struct goqry_rec rec;
		CopyMemory(&rec, (LPCTSTR)strData.Left(L_goqry_rec), L_goqry_rec);

		strTemp  = CString(rec.zAcntNo, sizeof(rec.zAcntNo));
		_TRIMDATA(strTemp);
		
		if (!strTemp.IsEmpty())
		{
			strAccount = strTemp;
			strAccount.Insert(3, _T("-"));
			strAccount.Insert(6, _T("-"));
		}

		strTemp  = CString(rec.zAcntNm, sizeof(rec.zAcntNm));
		_TRIMDATA(strTemp);
		
		if (!strTemp.IsEmpty())
			strAccName = strTemp;
		
		strCode = CString(rec.zIsuNo, sizeof(rec.zIsuNo));
		_TRIMDATA(strCode);
		
		if (strCode.GetLength() == 7)
		{
			strCode.Delete(0);
			
			if (strCode == strQCode)
			{
				strOrderNo = CString(rec.lOrdNo, sizeof(rec.lOrdNo));
				_TRIMDATA(strOrderNo);
				strOrderNo.TrimLeft(_T("+-0"));
				strOrderNo.Format("%03d", atoi((LPCTSTR)strOrderNo));

				strCodeName = CString(rec.zIsuNm, sizeof(rec.zIsuNm));
				_TRIMDATA(strCodeName);

				strOrderQty = CString(rec.lOrdQty, sizeof(rec.lOrdQty));
				_TRIMDATA(strOrderQty);
				strOrderQty.TrimLeft(_T("+-0"));

				strExecQty = CString(rec.lExecQty, sizeof(rec.lExecQty));
				_TRIMDATA(strExecQty);
				strExecQty.TrimLeft(_T("+-0"));

				strMrcQty = CString(rec.lMrcQty, sizeof(rec.lMrcQty));
				_TRIMDATA(strMrcQty);
				strMrcQty.TrimLeft(_T("+=0"));

				strNonContQty.Format("%d", atoi((LPCTSTR)strOrderQty) - atoi((LPCTSTR)strExecQty) - atoi((LPCTSTR)strMrcQty));
				
				if (atoi((LPCTSTR)strNonContQty) > 0)
				{
					bool opt = false;
					CString ordtp; ordtp.Format("%d", m_pType->GetCurSel());
					if (m_pType->GetCurSel() == 0) opt = true;
					else if (ordtp == rec.zBnsTp[0]) opt = true;
					if (!opt) continue;
					m_pParent->FormatThousand(strOrderQty);
					m_pParent->FormatThousand(strNonContQty);

					strOrderPrice = CString(rec.dOrdPrc, sizeof(rec.dOrdPrc));
					_TRIMDATA(strOrderPrice);
					strOrderPrice = strOrderPrice.Left(strOrderPrice.GetLength() - 3);
					strOrderPrice.TrimLeft(_T("+-0"));
					m_pParent->FormatThousand(strOrderPrice);
				
					m_pModifyGrid->QuickSetText(1, nCurRow, (LPCTSTR)strOrderNo);
					m_pModifyGrid->QuickSetText(2, nCurRow, (LPCTSTR)strAccount);

					if (rec.zBnsTp[0] == _T('1'))
						m_pModifyGrid->QuickSetText(3, nCurRow, _T("매도"));
					else if (rec.zBnsTp[0] == _T('2'))
						m_pModifyGrid->QuickSetText(3, nCurRow, _T("매수"));

					m_pModifyGrid->QuickSetText(4, nCurRow, (LPCTSTR)strOrderQty);
					m_pModifyGrid->QuickSetText(5, nCurRow, (LPCTSTR)strOrderPrice);

					m_pModifyGrid->QuickSetText(6, nCurRow, (LPCTSTR)strNonContQty);

					m_pModifyGrid->QuickSetText(8, nCurRow, (LPCTSTR)strNonContQty);
					m_pModifyGrid->RedrawRow(nCurRow++);
				}
			}
		}

		strData.Delete(0, L_goqry_rec);
	}
}

void CModifyWnd::ChangeSiseData()
{
	OnCostTypeSelEndOk();
}

void CModifyWnd::OnCost()
{
	CString strVal;
	m_pCost->GetWindowText(strVal);

	m_pModifyGrid->UpdateCheckedRowData(7, strVal);

	if (m_pCostType->GetCurSel() == 0)
		m_pParent->SetCustomPrice(strVal);
}

void CModifyWnd::OnCostTypeSelEndOk()
{
	CString strVal(_T(""));

	const int nIndex = m_pCostType->GetCurSel();

	switch (nIndex)
	{
	case 0:
		strVal = m_pParent->GetSiseData(_T("000"));
		break;
	case 1:
		strVal = m_pParent->GetSiseData(_T("023"));
		break;
	case 2:
		strVal = m_pParent->GetSiseData(_T("025"));
		break;
	case 3:
		strVal = m_pParent->GetSiseData(_T("026"));
		break;
	case 4:
		strVal = m_pParent->GetSiseData(_T("302"));
		break;
	case 5:
		strVal = m_pParent->GetSiseData(_T("303"));
		break;
	}

	if (nIndex == 0)
		m_pCost->SetReadOnly(FALSE);
	else
		m_pCost->SetReadOnly(TRUE);

	m_pParent->FormatThousand(strVal);
	m_pCost->SetWindowText(strVal);
}

void CModifyWnd::GroupListComplete()
{

}

long CModifyWnd::OnContractGridRowChange(WPARAM wParam, LPARAM lParam)
{
	const long nRow = m_pContractGrid->GetCurrentRow();

	CString strFundNo = m_pContractGrid->QuickGetText(0, nRow);

	_TRIMDATA(strFundNo);
	
	if (strFundNo.IsEmpty())
		return 0;

	QueryNCDetail(strFundNo);

	return 0;
}

void CModifyWnd::ParsingOrder(CString strData, int nLen, CString strMsg)
{
	CString strOrdNo(_T(""));
	strData.Delete(0, L_gmod_mid);

	const int nRows = m_pModifyGrid->GetNumberRows();
	int nCount = 0;
	BOOL bFind = FALSE;

	for (int i = 0; i < nRows; i++)
	{
		if (m_pModifyGrid->IsChecked(i))
		{
			if (nCount == m_nCurOrder - 1)
			{
				nCount = i;
				bFind = TRUE;
				break;
			}
			else
				nCount++;
		}
	}

	if (bFind)
	{
		if (strMsg.Left(3) == _T("ERR"))
		{
			strMsg.Delete(0, 4);
			m_pModifyGrid->QuickSetText(10, nCount, (LPCTSTR)strMsg);
		}
		else
		{
			m_pModifyGrid->QuickSetText(10, nCount, (LPCTSTR)strMsg);
			strOrdNo = strData.Mid(15, 10);
			_TRIMDATA(strOrdNo);
			strOrdNo.TrimLeft(_T("+0"));

			m_pModifyGrid->QuickSetText(9, nCount, (LPCTSTR)strOrdNo);
		}

		m_pModifyGrid->RedrawRow(nCount);
	}

	if (m_nCurOrder < m_arrSendData.GetSize())
		SendOrder(FALSE);
	else
	{
		m_nCurOrder = 0;
		m_arrSendData.RemoveAll();
	}
}


void CModifyWnd::SendOrder(BOOL bStart /* = TRUE */)
{
	//본사직원 주문 안나가도록 예외 처리 [ Start ]
	if (!(long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L))
	{
		if (!(long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCCx), 0L))
		{
			MessageBox("본사직원은 주문이 불가합니다.","IBK투자증권");
			return;
		}	
	}
	//본사직원 주문 안나가도록 예외 처리 [ End ]
	if (m_nCurOrder >= m_arrSendData.GetSize())
		return;

	CString strGroupID(_T(""));
	CString strCode(_T(""));
	CString strDept(_T(""));
	CString strUser(_T(""));
	CString strSendData(_T(""));
	CString strGroupOdrNo(_T(""));

	strCode = m_pCodeCtrl->GetEditData();
	_TRIMDATA(strCode);

	strGroupOdrNo = m_pContractGrid->QuickGetText(0, m_pContractGrid->GetCurrentRow());
	_TRIMDATA(strGroupOdrNo);
	strGroupOdrNo.Format("+%09d", atoi((LPCTSTR)strGroupOdrNo));

	if (strCode.IsEmpty())
		return;

	strCode = _T("A") + strCode;

	if (m_pGroup->GetCurSel() == -1)
		return;
	else
		strGroupID.Format("%03d", m_pGroup->GetItemData(m_pGroup->GetCurSel()));

	m_pGroupID->SetCurSel(m_pGroup->GetCurSel());
	m_pGroupID->GetWindowText(strGroupID);
	
	_TRIMDATA(strGroupID);

	BOOL bCustomer = FALSE;
	struct _ledgerH ledger;
	struct gmod_mid mid {};

	strDept = m_pParent->Variant(deptCC);
	_TRIMDATA(strDept);

	strUser = m_pParent->Variant(userCC, "");
	_TRIMDATA(strUser);

	if (m_pParent->IsCustomer())
		bCustomer = TRUE;

	FillMemory(&ledger, L_ledgerH, ' ');

	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(ledger.svcd, _T("SONAT061"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(ledger.brno, (LPCTSTR)strDept, strDept.GetLength());
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '2';

	FillMemory(&mid, L_gmod_mid, ' ');

	CopyMemory(mid.In, _T("00001"), sizeof(mid.In));
	
	if (bCustomer)
	{
		mid.zUserTp[0] = _T('2');
		CopyMemory(mid.zCommdaCode, _T("01"), sizeof(mid.zCommdaCode));
	}
	else
	{
		mid.zUserTp[0] = _T('1');
		CopyMemory(mid.zCommdaCode, _T("00"), sizeof(mid.zCommdaCode));
	}

	CopyMemory(mid.zUserId, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(mid.zFundNm, (LPCTSTR)strGroupID, strGroupID.GetLength());
	mid.zIvstfndPtnTp[0] = _T('2');
	CopyMemory(mid.lFundOrgOrdNo, (LPCTSTR)strGroupOdrNo, strGroupOdrNo.GetLength());
	CopyMemory(mid.lSpotOrdCnt, _T("+00001"), sizeof(mid.lSpotOrdCnt));
	CopyMemory(mid.zIsuNo, (LPCTSTR)strCode, strCode.GetLength());

	CString strOrderData(_T(""));
	CString strVal(_T(""));
	//float fRate = 0;
	//int nQty = 0;
	int nPrice = 0;

	strOrderData = m_arrSendData.GetAt(m_nCurOrder++);

	strVal = m_pParent->Parser(strOrderData);
	_TRIMDATA(strVal);
	strVal.Remove(_T(','));
	strVal.Format("+%09d", atoi((LPCTSTR)strVal));
	CopyMemory(mid.lOrgOrdNo, (LPCTSTR)strVal, strVal.GetLength());

	strVal = m_pParent->Parser(strOrderData);
	_TRIMDATA(strVal);
	strVal.Remove(_T('-'));
	CopyMemory(mid.zAcntNo, (LPCTSTR)strVal, strVal.GetLength());

	CString strPass = m_pParent->Variant(passCC, strVal);

	if (strPass.IsEmpty() || strPass.GetLength() != 4)
	{
		MessageBox(_T("계좌비밀번호 저장후 사용하십시오"));
		return;
	}

	m_pParent->Parser(strOrderData);
	m_pParent->Parser(strOrderData);
	m_pParent->Parser(strOrderData);
	strVal = m_pParent->Parser(strOrderData);
	_TRIMDATA(strVal);
	strVal.Remove(_T(','));

	nPrice = atoi((LPCTSTR)strVal);
	//if (m_pQuantity->GetCurSel() == 0)
	//	nPrice = 0;
	//else
		nPrice = atoi((LPCTSTR)strVal);
	strVal.Format("+%015d", nPrice);
	//asdfasdf
	CopyMemory(mid.lOrdQty, (LPCTSTR)strVal, strVal.GetLength());

	strVal = m_pParent->Parser(strOrderData);
	_TRIMDATA(strVal);
	strVal.Remove(_T(','));
	CString ord_name;
	bool zero = true;
	switch (m_pOrderType->GetCurSel())
	{
	case 1: //시장가
		ord_name = "시장가";
		break;
	case 3: //최유리지정가
		ord_name = "최유리지정가";
		break;
	case 4: //최우선지정가
		ord_name = "최우선지정가";
		break;
	case 5: //장개시전시간외종가
		ord_name = "장개시전시간외종가";
		break;
	case 6: //시간외종가
		ord_name = "시간외종가";
		break;
	default :
		zero = false;
		break;

	}
	if (zero)
	{

		if (atoi((LPCTSTR)strVal) != 0)
		{
			CString msg; msg.Format("%s 주문은 정정단가를 0으로 자동 셋팅 됩니다.", ord_name);
			MessageBox(msg,"IBK투자증권",MB_OK);
			m_pModifyGrid->QuickSetText(7, m_pContractGrid->GetCurrentRow(), "0");
		}
		strVal = "0";
	}
	
	nPrice = atoi((LPCTSTR)strVal);

	strVal.Format("+%09d.00", nPrice);
	CopyMemory(mid.dOrdPrc, (LPCTSTR)strVal, strVal.GetLength());

	BOOL bCondition = FALSE;

	switch (m_pOrderType->GetCurSel())
	{
	case 0:
		CopyMemory(mid.zOrdprcPtnCode, _T("00"), sizeof(mid.zOrdprcPtnCode));
		bCondition = TRUE;
		break;
	case 1:
		CopyMemory(mid.zOrdprcPtnCode, _T("03"), sizeof(mid.zOrdprcPtnCode));
		bCondition = TRUE;
		break;
	case 2:
		CopyMemory(mid.zOrdprcPtnCode, _T("05"), sizeof(mid.zOrdprcPtnCode));
		bCondition = FALSE;
		break;
	case 3:
		CopyMemory(mid.zOrdprcPtnCode, _T("06"), sizeof(mid.zOrdprcPtnCode));
		bCondition = TRUE;
		break;
	case 4:
		CopyMemory(mid.zOrdprcPtnCode, _T("07"), sizeof(mid.zOrdprcPtnCode));
		bCondition = FALSE;
		break;
	case 5:
		CopyMemory(mid.zOrdprcPtnCode, _T("61"), sizeof(mid.zOrdprcPtnCode));
		bCondition = FALSE;
		break;
	case 6:
		CopyMemory(mid.zOrdprcPtnCode, _T("81"), sizeof(mid.zOrdprcPtnCode));
		bCondition = FALSE;
		break;
	}

	if (bCondition)
	{
		switch (m_pCondition->GetCurSel())
		{
		case 0:
			mid.zOrdCndiTp[0] = _T('0');
			break;
		case 1:
			mid.zOrdCndiTp[0] = _T('1');
			break;
		case 2:
			mid.zOrdCndiTp[0] = _T('2');
			break;
		}
	}
	else
		mid.zOrdCndiTp[0] = _T('0');

	if (bStart)
		mid.zGrpPtnTp[0] = _T('S');

	strSendData = CString((char*)&ledger, L_ledgerH) + CString((char*)&mid, L_gmod_mid);
	m_pParent->SendTR("PIBOPBXO", strSendData, KEY_MODIFYORDER, 0);
}

void CModifyWnd::OnPassSet()
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 2), (LPARAM)_T("IB0000A1 /p5 /S"));
}

bool CModifyWnd::IsNumber(CString str)
{
    BOOL bRet = TRUE;
    const int nLen = str.GetLength();
    for( int i=0 ; i < nLen ; ++i)
    {
        if( isdigit(str.GetAt(i)) == FALSE)
        {
            bRet = FALSE;
            break;
        }
    }
    return bRet;
}

void CModifyWnd::OnTypeSelEndOk()
{
	OnNonContract();

}


void CModifyWnd::ChangeSize(double* lRate, int ipoint, LPCTSTR lszname)
{
	CFont* pFont = m_pParent->GetNormalFont();
	CRect wrc, rc;
	GetClientRect(&rc);

	int iw = 0, ih = 0, itop = 0, ileft = 0;
	itop = CTRL_SPACE;//max(3 + (ipoint - 10), 3);

	m_pGroupStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pGroupStatic->SetFont(pFont);
	m_pGroupStatic->MoveWindow(wrc.left, itop, wrc.left + iw, ih );
	ileft = wrc.left + iw + CTRL_SPACE;
	
	m_pGroup->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pGroup->SetFont(pFont);
	m_pGroup->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + CTRL_SPACE;


	m_pAddAccount->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pAddAccount->SetFont(pFont);
	m_pAddAccount->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + CTRL_SPACE;
	
	m_pPassSet->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pPassSet->SetFont(pFont);
	m_pPassSet->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + BUTTON_SPACE;

	int ibtnw[4]{};
	m_pClear->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	ibtnw[0] = iw;

	m_pNonContract->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	ibtnw[1] = iw;

	m_pOrder->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	ibtnw[2] = iw;

	m_pContract->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	ibtnw[3] = iw;

	ileft = rc.right - (ibtnw[0] + ibtnw[1] + ibtnw[2] + ibtnw[3]) - (BUTTON_SPACE*4);
	m_pClear->SetFont(pFont);
	m_pClear->MoveWindow(ileft, itop, ibtnw[0], ih );
	ileft +=  ibtnw[0] + BUTTON_SPACE;

	m_pNonContract->SetFont(pFont);
	m_pNonContract->MoveWindow(ileft, itop, ibtnw[1], ih );
	ileft +=  ibtnw[1] + BUTTON_SPACE;

	m_pOrder->SetFont(pFont);
	m_pOrder->MoveWindow(ileft, itop, ibtnw[2], ih );
	ileft +=  ibtnw[2] + BUTTON_SPACE;

	m_pContract->SetFont(pFont);
	m_pContract->MoveWindow(ileft, itop, ibtnw[3], ih );
//	ileft +=  ibtnw[3] + CTRL_SPACE;

	itop += (ih + CTRL_SPACE);
	m_pCodeCtrl->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);

	ileft = wrc.left ;
	m_pCodeCtrl->SetFont(pFont);
	m_pCodeCtrl->MoveWindow(ileft, itop, iw, ih );
	ileft += iw;

	m_pCodeSearch->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pCodeSearch->SetFont(pFont);
	m_pCodeSearch->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+1);

	m_pSymbol->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pSymbol->MoveWindow(ileft, itop, iw, ih );
	m_pSymbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, ipoint), (LPARAM)lszname);
	ileft += (iw+13);

	m_pTypeStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pTypeStatic->SetFont(pFont);
	m_pTypeStatic->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+1);

	m_pType->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pType->SetFont(pFont);
	m_pType->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+CTRL_SPACE);

	m_pQuantityStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pQuantityStatic->SetFont(pFont);
	m_pQuantityStatic->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+1);

	m_pQuantity->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pQuantity->SetFont(pFont);
	m_pQuantity->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+13);

	m_pOrderTypeStatic->GetWindowRect(&wrc);//유형
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pOrderTypeStatic->SetFont(pFont);
	m_pOrderTypeStatic->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+1);

	m_pOrderType->GetWindowRect(&wrc);//콤보
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pOrderType->SetFont(pFont);
	m_pOrderType->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+1);


	m_pCondition->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pCondition->SetFont(pFont);
	m_pCondition->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+CTRL_SPACE);


	m_pCostStatic->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pCostStatic->SetFont(pFont);
	m_pCostStatic->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+1);

	m_pCostType->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pCostType->SetFont(pFont);
	m_pCostType->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+CTRL_SPACE);

	m_pCost->GetWindowRect(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);

	const int iright = rc.Width() - ileft - CTRL_SPACE; 
	m_pCost->SetFont(pFont);
	m_pCost->MoveWindow(ileft, itop, iright, ih );

	itop += (ih + CTRL_SPACE);
	ih = rc.Height()  - itop - 2;
	m_pContractGrid->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	ileft = wrc.left ;
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pContractGrid->MoveWindow(ileft, itop, iw, ih);
	m_pContractGrid->ChangeFont(pFont, lRate);

	ileft = iw+1;
	m_pModifyGrid->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pModifyGrid->MoveWindow(0, itop, iw, ih);
	m_pModifyGrid->ChangeFont(pFont, lRate);

}
void CModifyWnd::SetPallete(CMapWnd* pMapWnd)
{
	m_pModifyGrid->SetPallete(pMapWnd);
	m_pGroupStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pGroupStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pCostStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pCostStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pOrderTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pOrderTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	Invalidate();
}

void CModifyWnd::SetCost( CString strCost )
{
	m_pCostType->SetCurSel(0);
	m_pParent->FormatThousand(strCost);
	m_pCost->SetWindowText(strCost);
}



//BOOL CModifyWnd::PreTranslateMessage(MSG* pMsg)
//{
//	if (pMsg->message == WM_CTLCOLORBTN)
//	{
//		const UINT nID = ::GetDlgCtrlID((HWND) pMsg->lParam);
//
//		if (nID == IDC_REMAINQTY || nID == IDC_PARTQTY)
//		{
//			::SetBkMode((HDC)pMsg->wParam, TRANSPARENT);
//
////			return (HBRUSH)::GetStockObject(NULL_BRUSH);
//		}
//
//	}
//
//	return CWnd::PreTranslateMessage(pMsg);
//}
