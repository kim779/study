// ModCanWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
#include "ModCanWnd.h"
#include "MapWnd.h"
#include "Define.h"
#include "../../control/fx_misc/misctype.h"
#include "ConfirmDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UM_PASSWORD	WM_USER+1111
/////////////////////////////////////////////////////////////////////////////
// CModCanWnd

CModCanWnd::CModCanWnd(CWnd* pWizard)
	: m_pWizard(pWizard)
	, m_pParent(NULL)
	, m_pAccount(NULL)
	, m_pAccName(NULL)
	, m_pPassStatic(NULL)
	, m_pPassword(NULL)
	, m_pModCanPriceStatic(NULL)
	, m_pModCanPriceType(NULL)
	, m_pNonCont(NULL)
	, m_pModify(NULL)
	, m_pCancel(NULL)
	, m_pContract(NULL)
	, m_pModCanGrid(NULL)
	, m_bBlock(FALSE)
{
}

CModCanWnd::~CModCanWnd()
{
	_SAFEDESTROY(m_pAccount);
	_SAFEDESTROY(m_pAccName);
	_SAFEDESTROY(m_pPassStatic);
	_SAFEDESTROY(m_pPassword);
	_SAFEDESTROY(m_pModCanPriceStatic);
	_SAFEDESTROY(m_pModCanPriceType);
	_SAFEDESTROY(m_pNonCont);
	_SAFEDESTROY(m_pModify);
	_SAFEDESTROY(m_pCancel);
	_SAFEDESTROY(m_pContract);
	_SAFEDESTROY(m_pModCanGrid);
}


BEGIN_MESSAGE_MAP(CModCanWnd, CWnd)
	//{{AFX_MSG_MAP(CModCanWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_NONCONT, OnNonCont)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_CANCELMODE, OnCancelMode)
	ON_CBN_SELCHANGE(IDC_MODCANPRICETYPE, OnSelchangeModCanPriceType)
	ON_MESSAGE(WM_USER + 1, OnUser)
	ON_BN_CLICKED(IDC_CONTRACT, OnContract)
	ON_MESSAGE(UM_PASSWORD, OnMyPassword)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CModCanWnd message handlers

void CModCanWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMapWnd* pWnd = (CMapWnd*)GetParent();
	
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, pWnd->GetIndexColor(CLR_PANNEL_BACK));
}

int CModCanWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pParent = (CMapWnd*)GetParent();

	CRect rcCtrl, rcClient;
	GetClientRect(&rcClient);

	rcCtrl.SetRect(SZ_TAB_HMARGIN, SZ_FORM_MARGIN, SZ_TAB_HMARGIN + SZ_CTRL_ACCN, SZ_FORM_MARGIN + SZ_CTRL_HEIGHT);

	m_pAccount = new CAccountCtrl(m_pWizard, this, m_pParent->GetAxRoot());
	m_pAccount->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE|WS_CHILD|WS_TABSTOP, rcCtrl, this, IDC_ACCOUNT);
	m_pAccount->createAccountCtrl("AN20", KEY_ACCOUNT, m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	
	rcCtrl.left = rcCtrl.right + 2;					// 계좌 컨트롤과 계좌명 컨트롤간의 간격은 1로 한다.
	rcCtrl.right = rcCtrl.left + SZ_CTRL_ACCNM;

	m_pAccName = new CfxStatic;
	m_pAccName->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_ACCOUNTNAME);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;	// 컨트롤 그룹간 간격은 5로 한다. (SZ_CTRL_HGAP : 5px,  fxStatic내부 마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4STATIC;
	
	m_pPassStatic = new CfxStatic;
	m_pPassStatic->Create(_T("비밀번호"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
							WS_CLIPSIBLINGS, rcCtrl, this, IDC_PASSSTATIC);

	rcCtrl.left = rcCtrl.right + 3;					// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3EDIT - 10;
	
	m_pPassword = new CfxEdit;
	m_pPassword->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_PASSWORD, rcCtrl, this, IDC_PASSWORD);

	rcCtrl.left = rcCtrl.right + SZ_CTRL_HGAP + 3;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_4STATIC;
	
	m_pModCanPriceStatic = new CfxStatic;
	m_pModCanPriceStatic->Create(_T("가격설정"), WS_CHILD | WS_VISIBLE | SS_LEFT | WS_CLIPCHILDREN | 
								WS_CLIPSIBLINGS, rcCtrl, this, IDC_MODCANPRICESTATIC);

	rcCtrl.left = rcCtrl.right + 3;					// 캡션과 컨트롤간 간격은 5로 한다. (fxStatic 내부마진 2px)
	rcCtrl.right = rcCtrl.left + SZ_COMBO_5WIDTH;

	m_pModCanPriceType = new CComboBox;
	m_pModCanPriceType->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
						CRect(rcCtrl.left, rcCtrl.top, rcCtrl.right, 200), this, IDC_MODCANPRICETYPE);

	rcCtrl.left = rcClient.right - (SZ_CTRL_3BUTTON * 4) - 4 - SZ_TAB_HMARGIN;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3BUTTON;

	m_pNonCont = new CfxImgButton;
	m_pNonCont->Create("\n미체결", rcCtrl, this, IDC_NONCONT);
	
	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3BUTTON;

	m_pModify = new CfxImgButton;
	m_pModify->Create("\n정정", rcCtrl, this, IDC_MODIFY);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3BUTTON;

	m_pCancel = new CfxImgButton;
	m_pCancel->Create("\n취소", rcCtrl, this, IDC_CANCELMODE);

	rcCtrl.left = rcCtrl.right + 1;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_3BUTTON;

	m_pContract = new CfxImgButton;
	m_pContract->Create("\n체결", rcCtrl, this, IDC_CONTRACT);
	
	rcCtrl.left = SZ_TAB_HMARGIN;
	rcCtrl.top = rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.right = rcClient.Width() - SZ_CTRL_VGAP;
	rcCtrl.bottom = rcClient.Height() - SZ_FORM_MARGIN;
	
	m_pModCanGrid = new CModCanGrid;
	m_pModCanGrid->CreateGrid(WS_CHILD | WS_VISIBLE, rcCtrl, this, IDC_MODCANGRID);

	return 0;
}

void CModCanWnd::Init()
{
	CFont* pFont = m_pParent->GetNormalFont();

	m_pAccName->SetStyle(m_pAccName->GetStyle() | ssBORDER);
	m_pAccName->SetBkColor(m_pParent->GetIndexColor(CLR_SYMBOL_BACK));
	m_pAccName->SetFgColor(m_pParent->GetIndexColor(CLR_SYMBOL_FORE));
	m_pAccName->SetBorderColor(m_pParent->GetIndexColor(CLR_SYMBOL_LINE));
	m_pAccName->SetFont(pFont);
	m_pAccName->SetAlignment(alLEFT);

	m_pPassStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPassStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pPassStatic->SetFont(pFont);

	m_pPassword->LimitText(8);
	m_pPassword->SetFont(pFont);

	m_pModCanPriceStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pModCanPriceStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pModCanPriceStatic->SetFont(pFont);

	m_pModCanPriceType->SetFont(pFont);
	m_pModCanPriceType->AddString(_T("현재가"));
	m_pModCanPriceType->AddString(_T("상대1호가"));
	m_pModCanPriceType->AddString(_T("상대2호가"));
	m_pModCanPriceType->AddString(_T("상대3호가"));
	m_pModCanPriceType->AddString(_T("우선1호가"));
	m_pModCanPriceType->AddString(_T("우선2호가"));
	m_pModCanPriceType->AddString(_T("우선3호가"));
	m_pModCanPriceType->AddString(_T("지정가"));
	m_pModCanPriceType->SetCurSel(7);

	CString strPath = "";
	strPath.Format("%s\\Image\\", m_pParent->GetAxRoot());

	HBITMAP hbmp	= m_pParent->GetAxBitmap(strPath + "3btn.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= m_pParent->GetAxBitmap(strPath + "3btn_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = m_pParent->GetAxBitmap(strPath + "3btn_en.bmp")->operator HBITMAP();

	m_pNonCont->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pNonCont->SetWindowText(_T("미체결"));
	m_pNonCont->SetFont(pFont);

	m_pModify->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pModify->SetWindowText(_T("정정"));
	m_pModify->SetFont(pFont);

	m_pCancel->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pCancel->SetWindowText(_T("취소"));
	m_pCancel->SetFont(pFont);

	m_pContract->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pContract->SetWindowText(_T("체결"));
	m_pContract->SetFont(pFont);

	SetInputFieldFocus();
}

void CModCanWnd::QueryFOCHoga(BOOL bHoga, CString strCode, CString strState)
{
	CStringArray arrHoga;
	int nCount = 0;

	if (bHoga)
	{
		m_pModCanGrid->GetCheckCode(&arrHoga);
		nCount = arrHoga.GetSize();
	}
	else
		nCount = 1;

	int nCurSel = m_pModCanPriceType->GetCurSel();

	if (nCount < 0)
		return;
	
	struct fhmid mid;
	CString strSendData(_T(""));
	CString strCount(_T(""));
	CString strVal;
	
	strCount.Format("%04d", nCount);
	
	FillMemory(&mid, L_fhmid, ' ');
	
	switch (nCurSel)
	{
	case 0:
		CopyMemory(&mid.gubn, _T("00"), sizeof(mid.gubn));
		break;
	case 1:
		CopyMemory(&mid.gubn, _T("20"), sizeof(mid.gubn));
		break;
	case 2:
		CopyMemory(&mid.gubn, _T("21"), sizeof(mid.gubn));
		break;
	case 3:
		CopyMemory(&mid.gubn, _T("22"), sizeof(mid.gubn));
		break;
	case 4:
		CopyMemory(&mid.gubn, _T("10"), sizeof(mid.gubn));
		break;
	case 5:
		CopyMemory(&mid.gubn, _T("11"), sizeof(mid.gubn));
		break;
	case 6:
		CopyMemory(&mid.gubn, _T("12"), sizeof(mid.gubn));
		break;
	}
	
	CopyMemory(&mid.nrec, (LPCTSTR)strCount, sizeof(mid.nrec));
	
	if (bHoga)
	{
		for (int i = 0; i < nCount; i++)
		{
			strVal = arrHoga.GetAt(i);
			struct i_hoga hoga;
			FillMemory(&hoga, L_i_hoga, ' ');
			
			hoga.dsgb[0] = strVal.GetAt(0);
			
			CopyMemory(&hoga.cod2, strVal.Right(8), sizeof(hoga.cod2));
			CopyMemory(&mid.rec[i], &hoga, L_i_hoga);
		}
	}
	else
	{
		struct i_hoga hoga;
		FillMemory(&hoga, L_i_hoga, ' ');

		if (strState == _T("매도"))
			hoga.dsgb[0] = _T('1');
		else
			hoga.dsgb[0] = _T('2');

//		hoga.dsgb[0] = m_pSell->GetCheck() == 1 ? _T('1') : _T('2');

		CopyMemory(&hoga.cod2, strCode, sizeof(hoga.cod2));
		CopyMemory(&mid.rec[0], &hoga, L_i_hoga);
	}
	
	strSendData = CString((char*)&mid, nCount * L_i_hoga + 6);
	
	m_pParent->SendTR("PIBO3006", strSendData, bHoga ? KEY_MODCANFOHOGA : KEY_MODCANFOCURR, 0);
}
  
CString CModCanWnd::Parser(CString &srcstr, CString substr)
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

void CModCanWnd::OnNonCont()
{
	CString strAccount = m_pAccount->GetAccNo();
	CString strPassword(_T(""));
	CString strUser(_T(""));
	CString strData(_T(""));

	if (strAccount.IsEmpty())
	{
		AfxMessageBox("계좌번호를 입력하세요");
		return;
	}

	m_pPassword->GetWindowText(strPassword);
	
	if (strPassword.IsEmpty())
	{
		AfxMessageBox("비밀번호를 입력하세요");
		return;
	}
	if (strPassword.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}

	ClearModCanGrid();

	strUser = m_pParent->Variant(userCC, "");

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, strAccount.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	strData = CString((char*)&ledger, L_ledgerH);

	struct f_cmid cmid;
	
	FillMemory(&cmid, L_fcmid, ' ');
	CopyMemory(cmid.accn, strAccount, sizeof(cmid.accn));

	

	CString passX;
	passX.Format("%s\t%s", strPassword, strAccount.Left(8));
	passX = (char *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	CopyMemory(&cmid.pswd, passX, passX.GetLength());

//	cmid.gubn[0] = _T('2');
//	cmid.sort[0] = _T('1');
//	cmid.mkgb[0] = _T('0');
//	cmid.dsgb[0] = _T('0');

CopyMemory(cmid.gubn, "2", sizeof(cmid.gubn));
CopyMemory(cmid.sort, "1", sizeof(cmid.sort));
CopyMemory(cmid.mkgb, "0", sizeof(cmid.mkgb));
CopyMemory(cmid.dsgb, "0", sizeof(cmid.dsgb));

	strData += CString((char*)&cmid, L_fcmid);
	m_pParent->SendTR("PIBOFCHG", strData, KEY_NONCONT, 0);
}

void CModCanWnd::OnModify()
{
	SendOrder(TRUE);
}

void CModCanWnd::OnCancelMode()
{
	SendOrder(FALSE);
}

void CModCanWnd::ParsingNonCont(CString strData, int nLen, CString strMsg)
{
	if (strData.GetLength() < 15 + L_frec)
	{
		strMsg.Delete(0, 4);
		m_pParent->SetGuide(strMsg);
		return;
	}

	CString strCode(_T(""));
	CUGCell cell;
	CString strVal(_T(""));
	CString strAccnount = strData.Left(11);
	int nRec = atoi((LPCTSTR)strData.Mid(11, 4));

	strData.Delete(0, 15);
	struct f_crec rec;

	ClearModCanGrid();
	
	if (nRec >= m_pModCanGrid->GetNumberRows())
		m_pModCanGrid->SetNumberRows(nRec + 1);

	for (int i = 0; i < nRec; i++)
	{
		CopyMemory(&rec, (LPCTSTR)strData.Left(L_fcrec), L_fcrec);

		strVal = CString(rec.cod2, sizeof(rec.cod2));
		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(1, i, strVal);

		strVal = CString(rec.juno, sizeof(rec.juno));
		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(2, i, strVal);

		strVal = CString(rec.hnam, sizeof(rec.hnam));
		_TRIMDATA(strVal);

		strVal.Insert(2, _T("20"));

		if (strVal.GetAt(0) == _T('F'))
			strVal = _T("K200 ") + strVal;

		m_pModCanGrid->QuickSetText(3, i, strVal);

		strVal = CString(rec.odgb, sizeof(rec.odgb));
		_TRIMDATA(strVal);

		if (strVal.GetAt(0) == _T('+') || strVal.GetAt(0) == _T('-'))
			strVal.Delete(0);

		m_pModCanGrid->QuickSetText(4, i, strVal);

		QueryFOCHoga(FALSE, m_pModCanGrid->QuickGetText(1, i), strVal);

		strVal = CString(rec.hogb, sizeof(rec.hogb));
		_TRIMDATA(strVal);
		
		if (strVal.Find(_T("조건부")) != -1)
			strVal = _T("조건부");
		else if (strVal.Find(_T("시장가")) != -1)
			strVal = _T("시장가");
		else if (strVal.Find(_T("최유리")) != -1)
			strVal = _T("최유리");
		else if (strVal.Find(_T("IOC")) != -1)
			strVal = _T("IOC");
		else if (strVal.Find(_T("FOK")) != -1)
			strVal = _T("FOK");

		m_pModCanGrid->QuickSetText(5, i, strVal);

		
		/* 2009/02/11 정정수량에 미체결수량을 보여주도록 수정함 by lkm */
		//strVal = CString(rec.jqty, sizeof(rec.jqty));
		strVal = CString(rec.wqty, sizeof(rec.wqty));


		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(6, i, strVal);

		strVal = CString(rec.jprc, sizeof(rec.jprc));
		_TRIMDATA(strVal);
		m_pModCanGrid->QuickSetText(7, i, strVal);

		strData.Delete(0, L_fcrec);
		m_pModCanGrid->GotoRow(i);
	}

	m_pModCanGrid->RedrawAll();
}

void CModCanWnd::ClearModCanGrid()
{
	CUGCell cell;
	m_pModCanGrid->SetNumberRows(DEF_MODCANGRIDROW);

	for (int i = 0; i < MAX_MODCANGRIDCOL; i++)
	{
		for (int j = 0; j < DEF_MODCANGRIDROW; j++)
		{
			m_pModCanGrid->GetCell(i, j, &cell);
			cell.ClearProperty(UGCELL_TEXT_SET);
			m_pModCanGrid->SetCell(i, j, &cell);
		}
	}

	m_pModCanGrid->GotoRow(0);
	m_pModCanGrid->RedrawAll();
}

void CModCanWnd::ParsingFOCHoga(CString strData, BOOL bHoga)
{
	if (strData.IsEmpty())
		return;

	int nCount = atoi((LPCTSTR)strData.Left(4));

	CString strCurPrice(_T(""));
	CString strCode(_T(""));
	CString strPrice(_T(""));
	CStringArray arrHoga;
	struct o_hoga hoga;

	strData.Delete(0, 4);

	for (int i = 0; i < nCount; i++)
	{
		CString strVal;
		CopyMemory(&hoga, (LPCTSTR)strData.Left(L_o_hoga), L_o_hoga);
		strData.Delete(0, L_o_hoga);

		strCode = CString(hoga.cod2, sizeof(hoga.cod2));
		strCurPrice = CString(hoga.curr, sizeof(hoga.curr));
		strPrice = CString(hoga.hoga, sizeof(hoga.hoga));

		_TRIMDATA(strCode);
		_TRIMDATA(strCurPrice);
		_TRIMDATA(strPrice);
		
		strVal.Format("%s\t%s\t%s", strCode, strCurPrice, strPrice);
		arrHoga.Add(strVal);
	}

	m_pModCanGrid->SetFOCData(&arrHoga, bHoga);
}

void CModCanWnd::SendOrder(BOOL bModify)
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
	int nCount = 0;
	CStringArray arrData;
	CStringArray arrConfirm;
	CString strData(_T(""));
	CString strCode(_T(""));		// 종목코드
	CString strOrderNo(_T(""));		// 원주문번호
	CString strOrderState(_T(""));	// 매매유형
	CString strOrderType(_T(""));	// 호가구분
	CString strQuantity(_T(""));	// 수량
	CString strOrderPrice(_T(""));	// 주문단가
	CString strCount(_T(""));;
	CString strUser(_T(""));
	CString strPassword(_T(""));
	CString strEncPass(_T(""));
	CString strSendData(_T(""));
	CString strAccount = m_pAccount->GetAccNo();

	m_pModCanGrid->GetSendData(&arrData);
	nCount = arrData.GetSize();

	if (nCount <= 0)
		return;

	if (strAccount.IsEmpty())
	{
		AfxMessageBox("계좌번호를 입력하세요");
		return;
	}

	m_pPassword->GetWindowText(strPassword);
	
	if (strPassword.IsEmpty())
	{
		AfxMessageBox("비밀번호를 입력하세요");
		return;
	}
	if (strPassword.GetLength()<4)
	{
		MessageBox("비밀번호를 확인하십시오.","IBK투자증권");
		return;
	}

	strUser = m_pParent->Variant(userCC, "");
	strEncPass.Format("%s\t%s", strPassword, strAccount.Left(8));
	strPassword = (char *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)strEncPass);
	
	CConfirmDlg dlg(this, m_pParent, bModify ? 3 : 4);
	CString strConfirm(_T(""));

	for (int i = 0; i < nCount; i++)
	{
		strConfirm = strAccount + _T("\t");
		strConfirm += m_pAccount->GetAccName();
		strConfirm += _T("\t");
		strConfirm += arrData.GetAt(i);
		arrConfirm.Add(strConfirm);
	}

	dlg.SetOrderData(&arrConfirm);
	
	if (dlg.DoModal() == IDCANCEL)
		return;

	/* 취소주문시 그리드를 삭제하지 않도록 주석처리함 2009/02/12 by lkm */
	/*
	if (!bModify)
		ClearModCanGrid();
	*/

	struct i_com mid;
	FillMemory(&mid, L_i_com, ' ');

	strCount.Format("%04d", nCount);

	CopyMemory(mid.nrec, (LPCTSTR)strCount, strCount.GetLength());

	for (i = 0; i < nCount; i++)
	{
		struct irec rec;
		strData = arrData.GetAt(i);

		if (strData.IsEmpty())
			continue;
		
		FillMemory(&rec, L_irec, ' ');

		strCode = Parser(strData, _T("\t"));
		strOrderNo = Parser(strData, _T("\t"));
		Parser(strData, _T("\t"));				// 종목명
		strOrderState = Parser(strData, _T("\t"));
		strOrderType = Parser(strData, _T("\t"));
		strQuantity = Parser(strData, _T("\t"));
		strOrderPrice = Parser(strData, _T("\t"));

		int nPrice = (int)(atof((LPCTSTR)strOrderPrice) * 100);
		strOrderPrice.Format("%d", nPrice);

		rec.odgb[0] = _T('2');
		rec.mmgb[0] = bModify ? _T('3') : _T('4');
		CopyMemory(rec.accn, strAccount, strAccount.GetLength());
		CopyMemory(rec.pswd, strPassword, strPassword.GetLength());
		CopyMemory(rec.cod2, strCode, strCode.GetLength());
		CopyMemory(rec.ojno, strOrderNo, strOrderNo.GetLength());
		CopyMemory(rec.jqty, strQuantity, strQuantity.GetLength());
		CopyMemory(rec.jprc, strOrderPrice, strOrderPrice.GetLength());
		
		/* 주문유형 적용 */
		//CopyMemory(rec.hogb, _T("00"), sizeof(rec.hogb));
		if      (strOrderType == _T("지정가")) CopyMemory(rec.hogb, _T("00"), sizeof(rec.hogb));
		else if (strOrderType == _T("시장가")) CopyMemory(rec.hogb, _T("03"), sizeof(rec.hogb));
		else if (strOrderType == _T("조건부")) CopyMemory(rec.hogb, _T("05"), sizeof(rec.hogb));
		else if (strOrderType == _T("최유리")) CopyMemory(rec.hogb, _T("06"), sizeof(rec.hogb));
		else if (strOrderType == _T("IOC"))    CopyMemory(rec.hogb, _T("10"), sizeof(rec.hogb));
		else if (strOrderType == _T("FOK"))    CopyMemory(rec.hogb, _T("20"), sizeof(rec.hogb));
		else                                   CopyMemory(rec.hogb, _T("00"), sizeof(rec.hogb));

		if (strOrderType == _T("IOC"))
			rec.cdgb[0] = _T('1');
		else if (strOrderType == _T("FOK"))
			rec.cdgb[0] = _T('2');
		else
			rec.cdgb[0] = _T('0');

		rec.mdgb[0] = _T('2');
		rec.prgb[0] = _T('X');

		CopyMemory(&mid.rec[i], &rec, L_irec);
	}

	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		strUser = pData;

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, strAccount.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '2';

	strSendData = CString((char*)&ledger, L_ledgerH);


	strSendData += CString((char*)&mid, nCount * L_irec + 4);
	m_pParent->SendTR("PIBOFODR", strSendData, KEY_MODCANFORDER, 0);
}

void CModCanWnd::ParsingFOrder(CString strData, int nLen, CString strMsg)
{
	if (strData.GetLength() < 4)
		return;

	int nCount = atoi((LPCTSTR)strData.Left(4));
	
	if (nCount == 0)
		return;

	strData.Delete(0, 4);

	CStringArray arrData;
	CString strVal(_T(""));

	for (int i = 0; i < nCount; i++)
	{
		if (strData.IsEmpty())
			break;

		strVal = strData.Left(L_orec);
		arrData.Add(strVal);

		strData.Delete(0, L_orec);
	}

	m_pModCanGrid->SetOrderResult(&arrData);
}


long CModCanWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case 100:	// CAccountCtrl 계좌변경시 수신 Message
		{
			if (!m_pAccName)
				return 0;

			CString data = (char*)lParam;
			/*
			Parser(data, "\t");
			m_pAccName->SetText(Parser(data, "\t"));
			Parser(data, "\t");
			*/
			CString accno = Parser(data, "\t");
			CString accnm = Parser(data, "\t");
			CString accpw = Parser(data, "\t");
			m_pAccName->SetText(accnm);
			m_pPassword->SetText(accpw);
			m_pPassword->SetFocus();
			if (accpw.GetLength()>0) this->OnNonCont();
		}
		break;
	}
	return 0;
}

void CModCanWnd::SetInputFieldFocus()
{
	if (m_pAccount && m_pPassword && m_pNonCont) 
	{
		if (m_pAccount->GetAccNo().IsEmpty())
			m_pAccount->SetFocus();
		else if (m_pPassword->GetText().IsEmpty())
			m_pPassword->SetFocus();
		else
			m_pNonCont->SetFocus();
	}
}

void CModCanWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	if (bShow)
		SetInputFieldFocus();	
}

void CModCanWnd::SetPallete()
{
	m_pAccName->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pAccName->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pPassStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pPassStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));
	m_pModCanPriceStatic->SetBkColor(m_pParent->GetIndexColor(CLR_PANNEL_BACK));
	m_pModCanPriceStatic->SetFgColor(m_pParent->GetIndexColor(CLR_STATIC_FORE));

	m_pModCanGrid->SetPallete();
	Invalidate();

}

BOOL CModCanWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == IDC_PASSWORD)
		OnNonCont();	

	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CModCanWnd::OnSelchangeModCanPriceType()
{
	QueryFOCHoga(TRUE);
}

void CModCanWnd::OnContract()
{
	CMenu* pMenu;
	CRect  rcMenu;
	int nCount = 0;

	pMenu = new CMenu;
	pMenu->CreatePopupMenu();
	pMenu->AppendMenu(MF_STRING, 0, "체결조회");
	pMenu->AppendMenu(MF_STRING, 1, "미체결조회");

	m_pContract->GetWindowRect(&rcMenu);

	int nSelectGroup = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_RETURNCMD,
								rcMenu.left, rcMenu.bottom, this);
	delete pMenu;

	CString strData(_T(""));
	if (nSelectGroup == 1)
		strData = _T("IB310100 /S /t");
	else if (nSelectGroup == 0)
		strData = _T("IB310200 /S /t");

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
}

LRESULT CModCanWnd::OnMyPassword(WPARAM wParam, LPARAM lParam)
{
	//CString rcb = (char*)lParam;
	//MessageBox((char*)lParam);
	m_pPassword->SetWindowText((char*)lParam);
	//MessageBox("Received: "+rcb);
	return 1;
}

bool CModCanWnd::IsNumber(CString str)
{
    BOOL bRet = TRUE;
    int nLen = str.GetLength();
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

void CModCanWnd::OnAlert(CString altmsg)
{
	int st, ed;
	CString key = Parser(altmsg, "\t");
	long rows = m_pModCanGrid->GetNumberRows();
	for(long i=0; i<rows; ++i)
	{
		CString code = m_pModCanGrid->QuickGetText(1, i);

		if (code==key) {
			CString currSymb = "\t023\t";	/* 현재가 심볼을 찾는다 */
			if ((st=altmsg.Find(currSymb))>-1) {	
				/* 현재가값을 추출한다 */
				st += currSymb.GetLength();
				ed = altmsg.Find('\t', st);

				char giho = altmsg[st];
				int col;
				switch(giho)
				{
				case '+': col = 94; break;
				case '-': col = 95; break;
				default : col = 69; break;
				}
				
				m_pModCanGrid->QuickSetText(8, i, (LPCTSTR)altmsg.Mid(st, ed-st));
				/*
				CUGCell cell;
				m_pOrderGrid->GetCell(8, i, &cell);
				cell.SetTextColor(m_pParent->GetAxColor(col));
				cell.SetText((LPCTSTR)altmsg.Mid(st+1, ed-st-1));
				m_pOrderGrid->SetCell(8, i, &cell);
				*/
				m_pModCanGrid->RedrawCell(8,i);

				break;
			}
			break;
		}
	}
}