// CancelWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100700.h"
#include "CancelWnd.h"
#include "MapWnd.h"
#include "../../control/fx_misc/misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCancelWnd

CCancelWnd::CCancelWnd(CWnd* pWizard)
	: m_pWizard(pWizard)
{
}

CCancelWnd::~CCancelWnd()
{
/*
	_SAFEDESTROY(m_pGroupStatic);
	_SAFEDESTROY(m_pGroup);
	_SAFEDESTROY(m_pGroupID);
	_SAFEDESTROY(m_pAddAccount);
	_SAFEDESTROY(m_pPassSet);
	_SAFEDESTROY(m_pClear);
	_SAFEDESTROY(m_pOrder);
	_SAFEDESTROY(m_pContract);
	_SAFEDESTROY(m_pCodeCtrl);
	_SAFEDESTROY(m_pCodeSearch);
	_SAFEDESTROY(m_pSymbol);
	_SAFEDESTROY(m_pTypeStatic);
	_SAFEDESTROY(m_pType);
	_SAFEDESTROY(m_pQuantityStatic);
	_SAFEDESTROY(m_pRemainQty);
	_SAFEDESTROY(m_pPartQty);
	_SAFEDESTROY(m_pContractGrid);
	_SAFEDESTROY(m_pCancelGrid);
	_SAFEDESTROY(m_pNonContract);
*/
}


BEGIN_MESSAGE_MAP(CCancelWnd, CWnd)
	//{{AFX_MSG_MAP(CCancelWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADDACCOUNT, OnAddAccount)
	ON_BN_CLICKED(IDC_CODESEARCH, OnCodeSearch)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_CBN_SELENDOK(IDC_GROUP, OnSelGroup)
	ON_BN_CLICKED(IDC_NONCONTRACT, OnNonContract)
	ON_BN_CLICKED(IDC_ORDER, OnOrder)
	ON_BN_CLICKED(IDC_CONTRACT, OnContract)
	ON_CBN_SELENDOK(IDC_CODECTRL, OnCodeSelEndOk)
	ON_CBN_SELENDOK(IDC_TYPE, OnTypeSelEndOk)
	ON_MESSAGE(UG_CONT_CELLCHANGE, OnContractGridRowChange)
	ON_BN_CLICKED(IDC_PASSSET, OnPassSet)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCancelWnd message handlers

int CCancelWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
						CRect(0, 0, 0, 0), this, IDC_GROUPID);
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
	rcCtrl.left = rcClient.right - CTRL_SPACE_W - BTN3_WIDTH - BTN2_WIDTH - BTN4_WIDTH - BTN2_WIDTH;
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
	m_pOrder->Create("\n일괄취소", rcCtrl, this, IDC_ORDER, FALSE, TRUE);

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

	rcCtrl.left = rcCtrl.right + 2;
	rcCtrl.right = rcCtrl.left + BTN1_WIDTH;

	m_pCodeSearch = std::make_unique<CfxImgButton>();
	m_pCodeSearch->Create("\n호가", rcCtrl, this, IDC_CODESEARCH, FALSE, TRUE);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_SYMBOL + 30;

	InitSymbol(rcCtrl);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;
	
	m_pTypeStatic = std::make_unique<CfxStatic>();
	m_pTypeStatic->Create(_T("구분"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_TYPESTATIC);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;					// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_COMBO_2WIDTH;

	m_pType = std::make_unique<CComboBox>();
	m_pType->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_TYPE);
	SetWindowTheme(m_pType->GetSafeHwnd(), L"", L"");
	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_2STATIC;

	m_pQuantityStatic = std::make_unique<CfxStatic>();
	m_pQuantityStatic->Create(_T("수량"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_QUANTITYSTATIC);

	rcCtrl.top += 1;
	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_RADIO_2WIDTH;

	m_pRemainQty = std::make_unique<CButton>();
	m_pRemainQty->Create(_T("잔량"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						WS_GROUP | BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_REMAINQTY);

	rcCtrl.left = rcCtrl.right + CTRL_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_RADIO_2WIDTH;

	m_pPartQty = std::make_unique<CButton>();
	m_pPartQty->Create(_T("일부"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | BS_AUTORADIOBUTTON | 
						BS_TOP | BS_BOTTOM, rcCtrl, this, IDC_PARTQTY);


	rcCtrl.left = SZ_TAB_HMARGIN;
	rcCtrl.right = rcCtrl.left + SZ_CONGRID_WIDTH;
	rcCtrl.top = rcCtrl.bottom + CTRL_SPACE;
	rcCtrl.bottom = rcClient.bottom - SZ_FORM_MARGIN;

	m_pContractGrid = std::make_unique<CContractGrid>();
	//m_pContractGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_CONTRACTGRID);
	m_pContractGrid->CreateGrid(WS_CHILD , rcCtrl, this, IDC_CONTRACTGRID);
	
	rcCtrl.left = 0;
	rcCtrl.right = rcClient.right - INNER_FRAME_SPACE;
	rcCtrl.bottom = rcClient.bottom;

	m_pCancelGrid = std::make_unique<CCancelGrid>();
	m_pCancelGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_CANCELGRID);

	return 0;
}

void CCancelWnd::Init()
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
	m_pOrder->SetWindowText(_T("일괄취소"));
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
	
	m_pRemainQty->SetCheck(1);
	m_pRemainQty->SetFont(pFont);

	m_pPartQty->SetFont(pFont);

	m_pContractGrid->SetFont(pFont);
	m_pCancelGrid->SetFont(pFont);
}

void CCancelWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	CRect rcClient, rcCtrl, rcTmp;
	GetClientRect(&rcClient);

	m_pCodeSearch->GetWindowRect(&rcCtrl);
	ScreenToClient(rcCtrl);

	rcTmp = rcClient;
	rcTmp.bottom = rcCtrl.bottom +INNER_FRAME_SPACE;
	dc.FillSolidRect(&rcTmp, pMapWnd->GetIndexColor(CMD_BACK_INDEX));

	rcTmp = rcClient;
	rcTmp.top = rcCtrl.bottom +INNER_FRAME_SPACE;
	dc.FillSolidRect(&rcTmp, pMapWnd->GetIndexColor(CONTENTS_BACK_INDEX));
}

void CCancelWnd::InitSymbol(CRect rcCtrl)
{
	CString	text;
	CWnd* (APIENTRY * axCreate)(CWnd*, void*) = nullptr;

	text.Format("%s/dev/cx_symbol.dll", m_pParent->GetAxRoot());
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
			rcCtrl.bottom = rc.bottom + 1;

			param.key   = 0;
			param.name  = _T("17413");
			param.rect  = rcCtrl;
			param.fonts = "굴림체";
			param.point = 9;
			param.style = 1;
			param.tRGB  = 69;
			param.pRGB  = 90;
			param.options = _T("/a89/b91/c92/d69/i99/s1007C");

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

void CCancelWnd::OnAddAccount()
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 2), (LPARAM)_T("IB100701 /p5 /S"));
	ClearCancelGrid();
	m_pParent->QueryGroupList();
}

void CCancelWnd::OnCodeSearch()
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

void CCancelWnd::QuerySymbol(CString strCode)
{
	CString strText;
	strText.Format("1301%c%s\t17413\t", 0x7f, strCode);
	m_pParent->SendTR(_T("POOPPOOP"), strText, KEY_SYMBOL, 0);

	m_pParent->SetCode(strCode);

	CString strProc;
	strProc.Format("change_code\t%s", strCode);
	m_pParent->Procedure(strProc);
}


void CCancelWnd::OnCodeSelEndOk()
{
	CString strCode = m_pCodeCtrl->GetEditData();
	QuerySymbol(strCode);
}

void CCancelWnd::ParsingSymbol(WPARAM wParam, LPARAM lParam)
{
	if (m_pSymbol)
		m_pSymbol->SendMessage(WM_USER, wParam, lParam);

	OnNonContract();
}

void CCancelWnd::OnClear()
{
	ClearContractGrid();
	ClearCancelGrid();
}

void CCancelWnd::ClearCancelGrid()
{
	CUGCell cell;
	m_pCancelGrid->SetNumberRows(DEF_CANCELGRIDROW);

	for (int i = 0; i < MAX_CANCELGRIDCOL; i++)
	{
		for (int j = 0; j < DEF_CANCELGRIDROW; j++)
		{
			m_pCancelGrid->GetCell(i, j, &cell);
			cell.ClearProperty(UGCELL_TEXT_SET);
			m_pCancelGrid->SetCell(i, j, &cell);
		}
	}

	m_pCancelGrid->SetCheckState(-1, FALSE);
	m_pCancelGrid->GotoRow(0);
	m_pCancelGrid->RedrawAll();
}

void CCancelWnd::ClearContractGrid()
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

void CCancelWnd::OnNonContract()
{
	ClearCancelGrid();
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
	
	m_pParent->SendTR("PIBOPBXQ", strSendData, KEY_CANCELNONCONTRACT, 0);
}

void CCancelWnd::QueryNCDetail(CString strFundNo)
{
	ClearCancelGrid();

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
	
	m_pParent->SendTR("PIBOPBXQ", strSendData, KEY_CANCELNCDETAIL, 0);
}

void CCancelWnd::OnOrder()
{
	m_arrSendData.RemoveAll();
	m_pCancelGrid->GetSendData(&m_arrSendData);
	m_nCurOrder = 0;

	SendOrder();
}

void CCancelWnd::OnContract()
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

CComboBox* CCancelWnd::GetGroupCombo()
{
	return m_pGroup.get();
}

CComboBox* CCancelWnd::GetGroupIDCombo()
{
	return m_pGroupID.get();
}

void CCancelWnd::SetCode(CString strCode)
{
	if (m_pCodeCtrl == NULL)
		return;

	m_pCodeCtrl->SetEditData(strCode);
	QuerySymbol(strCode);
}


CUGCtrl* CCancelWnd::GetGridWnd()
{
	return (CUGCtrl*)m_pCancelGrid.get();
}

HBRUSH CCancelWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
    const UINT nID = pWnd->GetDlgCtrlID();


	if (nID == IDC_REMAINQTY || nID == IDC_PARTQTY)
	{
		pDC->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
		return (HBRUSH) ::CreateSolidBrush(m_pParent->GetIndexColor(CLR_PANNEL_BACK));// : GetStockObject(HOLLOW_BRUSH);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CCancelWnd::ParsingNonContract(CString strData, int nLen, CString strMsg)
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

void CCancelWnd::ParsingNCDetail(CString strData, int nLen, CString strMsg)
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
	CString strMrcQty(_T(""));

	CString strQCode = m_pCodeCtrl->GetEditData();
	_TRIMDATA(strQCode);

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
				
					m_pCancelGrid->QuickSetText(1, nCurRow, (LPCTSTR)strOrderNo);
					m_pCancelGrid->QuickSetText(2, nCurRow, (LPCTSTR)strAccount);

					if (rec.zBnsTp[0] == _T('1'))
						m_pCancelGrid->QuickSetText(3, nCurRow, _T("매도"));
					else if (rec.zBnsTp[0] == _T('2'))
						m_pCancelGrid->QuickSetText(3, nCurRow, _T("매수"));

					m_pCancelGrid->QuickSetText(4, nCurRow, (LPCTSTR)strOrderQty);
					m_pCancelGrid->QuickSetText(5, nCurRow, (LPCTSTR)strOrderPrice);

					m_pCancelGrid->QuickSetText(6, nCurRow, (LPCTSTR)strNonContQty);
					m_pCancelGrid->QuickSetText(7, nCurRow, (LPCTSTR)strNonContQty);

					m_pCancelGrid->RedrawRow(nCurRow++);
				}
			}
		}

		strData.Delete(0, L_goqry_rec);
	}
}

void CCancelWnd::ChangeSiseData()
{
	;
}

void CCancelWnd::GroupListComplete()
{

}

long CCancelWnd::OnContractGridRowChange(WPARAM wParam, LPARAM lParam)
{
	const long nRow = m_pContractGrid->GetCurrentRow();

	CString strFundNo = m_pContractGrid->QuickGetText(0, nRow);

	_TRIMDATA(strFundNo);
	
	if (strFundNo.IsEmpty())
		return 0;

	QueryNCDetail(strFundNo);

	return 0;
}

void CCancelWnd::ParsingOrder(CString strData, int nLen, CString strMsg)
{
	CString strOrdNo(_T(""));
	strData.Delete(0, L_gcan_mid);

	const int nRows = m_pCancelGrid->GetNumberRows();
	int nCount = 0;
	BOOL bFind = FALSE;

	for (int i = 0; i < nRows; i++)
	{
		if (m_pCancelGrid->IsChecked(i))
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
			m_pCancelGrid->QuickSetText(9, nCount, (LPCTSTR)strMsg);
		}
		else
		{
			m_pCancelGrid->QuickSetText(9, nCount, (LPCTSTR)strMsg);
			
			strOrdNo = strData.Mid(15, 10);
			_TRIMDATA(strOrdNo);
			strOrdNo.TrimLeft(_T("+0"));

			m_pCancelGrid->QuickSetText(8, nCount, (LPCTSTR)strOrdNo);
		}

		m_pCancelGrid->RedrawRow(nCount);
	}

	if (m_nCurOrder < m_arrSendData.GetSize())
		SendOrder(FALSE);
	else
	{
		m_nCurOrder = 0;
		m_arrSendData.RemoveAll();
	}
}


void CCancelWnd::SendOrder(BOOL bStart /* = TRUE */)
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
	struct gcan_mid mid {};

	strDept = m_pParent->Variant(deptCC);
	_TRIMDATA(strDept);

	strUser = m_pParent->Variant(userCC, "");
	_TRIMDATA(strUser);

	if (m_pParent->IsCustomer())
		bCustomer = TRUE;

	FillMemory(&ledger, L_ledgerH, ' ');

	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(ledger.svcd, _T("SONAT062"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(ledger.brno, (LPCTSTR)strDept, strDept.GetLength());
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '2';

	FillMemory(&mid, L_gcan_mid, ' ');

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
	//if (m_pRemainQty->GetCheck() == 1)
	//	nPrice = 0;
	//else
		nPrice = atoi((LPCTSTR)strVal);

	strVal.Format("+%015d", nPrice);
	CopyMemory(mid.lOrdQty, (LPCTSTR)strVal, strVal.GetLength());

	m_pParent->Parser(strOrderData);

	if (bStart)
		mid.zGrpPtnTp[0] = _T('S');

	strSendData = CString((char*)&ledger, L_ledgerH) + CString((char*)&mid, L_gcan_mid);
	m_pParent->SendTR("PIBOPBXO", strSendData, KEY_CANCELORDER, 0);
}

void CCancelWnd::OnPassSet()
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 2), (LPARAM)_T("IB0000A1 /p5 /S"));
}

bool CCancelWnd::IsNumber(CString str)
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

void CCancelWnd::OnTypeSelEndOk()
{
	OnNonContract();

}

void CCancelWnd::OnSelGroup()
{
	OnNonContract();
}

void CCancelWnd::ChangeSize(double* lRate, int ipoint, LPCTSTR lszname)
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
	ileft +=  iw + 1;

	m_pGroupID->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pGroupID->SetFont(pFont);
	m_pGroupID->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + CTRL_SPACE;

	m_pAddAccount->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pAddAccount->SetFont(pFont);
	m_pAddAccount->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + BUTTON_SPACE;
	
	m_pPassSet->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pPassSet->SetFont(pFont);
	m_pPassSet->MoveWindow(ileft, itop, iw, ih );
	ileft +=  iw + CTRL_SPACE;
	

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

	ileft = rc.right - (ibtnw[0] + ibtnw[1] + ibtnw[2] + ibtnw[3]) - (BUTTON_SPACE*4) - CTRL_SPACE;
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
	ileft += (iw+CTRL_SPACE);

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
	ileft += (iw+CTRL_SPACE);


	m_pRemainQty->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pRemainQty->SetFont(pFont);
	m_pRemainQty->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+CTRL_SPACE);


	m_pPartQty->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);
	m_pPartQty->SetFont(pFont);
	m_pPartQty->MoveWindow(ileft, itop, iw, ih );
	ileft += (iw+CTRL_SPACE);

	itop += (ih + CTRL_SPACE);
	ih = rc.Height()  - itop - 2;
	m_pContractGrid->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	ileft = wrc.left ;
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );

	
	m_pContractGrid->MoveWindow(ileft, itop, iw, ih);
	m_pContractGrid->ChangeFont(pFont, lRate);

	ileft = iw + 1;
	m_pCancelGrid->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_pCancelGrid->MoveWindow(0, itop, iw, ih);
	m_pCancelGrid->ChangeFont(pFont, lRate);

}

void CCancelWnd::SetPallete(CMapWnd* pMapWnd)
{
	m_pCancelGrid->SetPallete(pMapWnd);
	m_pGroupStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pGroupStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pTypeStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pTypeStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pQuantityStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pQuantityStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));


	Invalidate();
	
}

