// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100700.h"
#include "MapWnd.h"
#include "../../control/fx_misc/misctype.h"
#include "../H/interMSG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* pWizard)
	: m_pWizard(pWizard)
	//, m_pBuyWnd(NULL)
	//, m_pCancelWnd(NULL)
	//, m_pExpandWnd(NULL)
	//, m_pModifyWnd(NULL)
	//, m_pSellWnd(NULL)
	//, m_pExpand(NULL)
	//, m_pWarning(NULL)
	//, m_pGuide(NULL)
	//, m_pTab(NULL)
	//, m_pNormalFont(NULL)
	//, m_pBoldFont(NULL)
	//, m_strRoot(_T(""))
	//, m_strGroupList(_T(""))
	//, m_strCode(_T(""))
{

	m_lSizeRate[0] = 0.16;
	m_lSizeRate[1] = 0.07;
	m_bInit = FALSE;
	m_iOldSize[0] = SZ_MAP_WIDTH;
	m_iOldSize[1] = SZ_MAP_HEIGHT;
	m_iCurFont = 9;
	m_iWndWidth = SZ_MAP_WIDTH;
	m_iWndHeight = SZ_MAP_HEIGHT;
	m_iExpendWidth = SZ_EXPAND_WIDTH;
	m_bExpendClick = FALSE;
	m_pCmdWnd = NULL;


}

CMapWnd::~CMapWnd()
{
	//_SAFEDESTROY(m_pBuyWnd);
	//_SAFEDESTROY(m_pCancelWnd);
	//_SAFEDESTROY(m_pExpandWnd);
	//_SAFEDESTROY(m_pModifyWnd);
	//_SAFEDESTROY(m_pSellWnd);
	//_SAFEDESTROY(m_pExpand);
	//_SAFEDESTROY(m_pWarning);
	//_SAFEDESTROY(m_pGuide);
	//_SAFEDESTROY(m_pTab);
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_EXPANDBTN, OnExpand)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabChange)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	Variant(titleCC, "[1007] 주식복수계좌주문");
	m_strRoot = Variant(homeCC);
	m_pNormalFont = GetAxFont("굴림체", 9, false, 0);
	m_pBoldFont = GetAxFont("굴림체", 9, false, 1);	
	
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcCtrl;
//	rcCtrl.SetRect(SZ_FORM_MARGIN, SZ_FORM_MARGIN, 
//					rcClient.Width() - SZ_EXPAND_WIDTH - 2 - SZ_FORM_MARGIN - SZ_CTRL_1BUTTON - SZ_CTRL_HGAP, 
//					SZ_FORM_MARGIN + SZ_TAB_HEIGHT);
	rcCtrl.SetRect(OUTER_FRAME_SPACE, OUTER_FRAME_SPACE, 
					rcClient.Width() - SZ_CTRL_1BUTTON - OUTER_FRAME_SPACE, 
					OUTER_FRAME_SPACE + SZ_TAB_HEIGHT);


	const int ileft = OUTER_FRAME_SPACE;
	m_pTab = std::make_unique<CfxTab>();
	m_pTab->Create(rcCtrl, this, IDC_TAB);

	rcCtrl.left = rcClient.Width() - SZ_FORM_MARGIN - SZ_CTRL_1BUTTON;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_1BUTTON;

	CRect rc = rcCtrl;
	rc.top = rc.bottom - SZ_CTRL_HEIGHT - 1;

	m_pExpand = std::make_unique<CfxImgButton>();
	m_pExpand->Create("\n호가", rc, this, IDC_EXPANDBTN, FALSE, TRUE);

	const int iguidetop = rcClient.bottom - INNER_FRAME_SPACE - (GUIDE_HEIGHT*2) - 1;
	rcCtrl.left = ileft;
	rcCtrl.right = rcClient.Width() - SZ_EXPAND_WIDTH - OUTER_FRAME_SPACE;
	rcCtrl.top += SZ_TAB_HEIGHT;
	rcCtrl.bottom = iguidetop-(INNER_FRAME_SPACE*2);//rcClient.Height() - SZ_FORM_MARGIN - SZ_CTRL_VGAP * 2 - SZ_CTRL_HEIGHT * 2;
	
	m_pSellWnd = std::make_unique<CSellWnd>(m_pWizard);
	m_pSellWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_SELL);

	m_pBuyWnd = std::make_unique<CBuyWnd>(m_pWizard);
	m_pBuyWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_BUY);

	m_pModifyWnd = std::make_unique<CModifyWnd>(m_pWizard);
	m_pModifyWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_MODIFY);

	m_pCancelWnd = std::make_unique<CCancelWnd>(m_pWizard);
	m_pCancelWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_CANCEL);

	rcCtrl.left = rcCtrl.right;
	rcCtrl.right = rcCtrl.left + SZ_EXPAND_WIDTH;
	m_pExpandWnd = std::make_unique<CExpandWnd>(m_pWizard);
	m_pExpandWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_EXPAND);

	
	rcCtrl.left = FRAME_SPACE;
	rcCtrl.top = iguidetop;
	rcCtrl.bottom = rcCtrl.top + GUIDE_HEIGHT;
	rcCtrl.right = rcClient.right - FRAME_SPACE;
	m_pWarning = std::make_unique<CfxStatic>();
	m_pWarning->Create(_T(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_WARNING);

	rcCtrl.top = iguidetop +GUIDE_HEIGHT + 1;//rcCtrl.bottom + SZ_CTRL_VGAP;
	rcCtrl.bottom = rcCtrl.top + GUIDE_HEIGHT;
	m_pGuide = std::make_unique<CfxStatic>();
	m_pGuide->Create(_T(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_GUIDE);

	return 0;
}

void CMapWnd::LoadRoundBitmap()
{
	CString sFile;
	sFile.Format("%s\\tab\\PALETTE.INI", m_strRoot);

	CString sPalette = _T("");
	int readL = 0; 
	char readB[1024]; 
	readL = GetPrivateProfileString(_T("General"), _T("Palette"), _T("Blue"), readB, sizeof(readB), sFile);
	if (readL > 0)	sPalette.Format("%s", readB);

	CString sImage;
	sImage.Format("%s\\Image\\axpanel%s1_",  m_strRoot, sPalette);
	for(int i=0; i<4; i++)
	{
		if(m_hRoundBitmap[i])
			DeleteObject(m_hRoundBitmap[i]);
	}

	CIB100700App* pApp = (CIB100700App*)AfxGetApp(); 
	m_hRoundBitmap[0] = (HBITMAP)::LoadImage( pApp->m_hInstance , sImage+"lt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[1] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"rt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[2] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"lb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[3] = (HBITMAP)::LoadImage(pApp->m_hInstance, sImage+"rb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );

}
void CMapWnd::Init()
{
	m_pTab->InsertItem(0, _T("일괄매도"));
	m_pTab->InsertItem(1, _T("일괄매수"));
	m_pTab->InsertItem(2, _T("일괄정정"));
	m_pTab->InsertItem(3, _T("일괄취소"));

	m_pTab->LoadTabImage(m_strRoot, false);
	m_pTab->SetStyle(tsIMAGE, false);
	m_pTab->SetBorderColor(GetIndexColor(TAB_BORDER_INDEX));
	m_pTab->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CONTENTS_BACK_INDEX), 
					GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CONTENTS_BACK_INDEX), GetIndexColor(CONTENTS_BACK_INDEX));
	m_pTab->SetFont(m_pNormalFont);

	CString strPath = "";
	strPath.Format("%s\\Image\\", m_strRoot);
	/*
	m_hRoundBitmap[0] = GetAxBitmap(strPath+_T("axpanel0_lt.bmp"))->operator HBITMAP();
	m_hRoundBitmap[1] = GetAxBitmap(strPath+_T("axpanel0_rt.bmp"))->operator HBITMAP();
	m_hRoundBitmap[2] = GetAxBitmap(strPath+_T("axpanel0_lb.bmp"))->operator HBITMAP();
	m_hRoundBitmap[3] = GetAxBitmap(strPath+_T("axpanel0_rb.bmp"))->operator HBITMAP();
	*/
	LoadRoundBitmap();
	m_hGuideBitmap = GetAxBitmap(strPath+_T("알림_ic.bmp"))->operator HBITMAP();

	HBITMAP hbmp	= GetAxBitmap(strPath + "투자매매종합100.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= GetAxBitmap(strPath + "투자매매종합100_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = GetAxBitmap(strPath + "투자매매종합100_en.bmp")->operator HBITMAP();

	m_pExpand->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pExpand->SetFont(m_pNormalFont);

	m_pSellWnd->SetFont(m_pNormalFont);
	m_pSellWnd->Init();
	m_pSellWnd->SetPallete();

	m_pBuyWnd->SetFont(m_pNormalFont);
	m_pBuyWnd->Init();
	m_pBuyWnd->SetPallete(this);

	m_pModifyWnd->SetFont(m_pNormalFont);
	m_pModifyWnd->Init();
	m_pModifyWnd->SetPallete(this);

	m_pCancelWnd->SetFont(m_pNormalFont);
	m_pCancelWnd->Init();
	m_pCancelWnd->SetPallete(this);

	m_pExpandWnd->SetFont(m_pNormalFont);
	m_pExpandWnd->Init();

	ShowHideBodyWnd(m_pTab->GetCurSel());

	m_pWarning->SetStyle(m_pGuide->GetStyle() | ssBORDER);
	m_pWarning->SetBkColor(GetIndexColor(GUIDE_BACK_INDEX));
	m_pWarning->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pWarning->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pWarning->SetFont(m_pNormalFont);
	m_pWarning->SetAlignment(alLEFT);
	m_pWarning->SetText(_T("주식복수계좌주문 화면 이용시에는 접속시마다 계좌비밀번호를 새로 저장하여 주시기 바랍니다."));
	m_pWarning->SetCornerRound(m_hRoundBitmap);
	m_pWarning->SetBitmap(m_hGuideBitmap);


	m_pGuide->SetStyle(m_pGuide->GetStyle() | ssBORDER);
	m_pGuide->SetBkColor(GetIndexColor(GUIDE_BACK_INDEX));
	m_pGuide->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pGuide->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pGuide->SetFont(m_pNormalFont);
	m_pGuide->SetAlignment(alLEFT);
	m_pGuide->SetCornerRound(m_hRoundBitmap);
	m_pGuide->SetBitmap(m_hGuideBitmap);

	m_bInit = TRUE;
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient, wrc, rcbox;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, GetIndexColor(BACK_INDEX));
	m_pSellWnd->GetWindowRect(&wrc);
	ScreenToClient(wrc);

	rcbox = rcClient;
	rcbox.DeflateRect(FRAME_SPACE,FRAME_SPACE,FRAME_SPACE,INNER_FRAME_SPACE);
	rcbox.bottom = wrc.bottom + INNER_FRAME_SPACE;
	

	dc.FillSolidRect(&rcbox, GetIndexColor(CONTENTS_BACK_INDEX));
	DrawCornerRoundBitmap(&dc, rcbox, 0);

	rcClient.DeflateRect(SZ_FORM_MARGIN, 0);
	rcClient.bottom = rcClient.Height() - SZ_FORM_MARGIN - SZ_CTRL_VGAP * 2 - SZ_CTRL_HEIGHT * 2;
	rcClient.top = SZ_FORM_MARGIN + SZ_CTRL_HEIGHT;
	
//	dc.FillSolidRect(&rcClient, GetIndexColor(CLR_PANNEL_BACK));
}

COLORREF CMapWnd::GetIndexColor(UINT nIndex)
{
	if (!m_pWizard)
		return 0;

	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
}

CString CMapWnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strRet(_T(""));

	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pData > 1)
		strRet = pData;

	return strRet;
}

CFont* CMapWnd::GetAxFont(CString strName, int nPt, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name = (LPSTR)(LPCTSTR)strName;
	fontR.point = nPt;
	fontR.italic = bItalic;
	fontR.bold = nBold;

	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}


void CMapWnd::ShowHideBodyWnd(int nIndex)
{
	switch (nIndex)
	{
	case 0:
		m_pBuyWnd->ShowWindow(SW_HIDE);
		m_pSellWnd->ShowWindow(SW_SHOW);
		m_pSellWnd->SetCode(m_strCode);
		m_pSellWnd->ChangeSiseData();
		m_pModifyWnd->ShowWindow(SW_HIDE);
		m_pCancelWnd->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pBuyWnd->ShowWindow(SW_SHOW);
		m_pBuyWnd->SetCode(m_strCode);
		m_pBuyWnd->ChangeSiseData();
		m_pSellWnd->ShowWindow(SW_HIDE);
		m_pModifyWnd->ShowWindow(SW_HIDE);
		m_pCancelWnd->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pSellWnd->ShowWindow(SW_HIDE);
		m_pBuyWnd->ShowWindow(SW_HIDE);
		m_pModifyWnd->ShowWindow(SW_SHOW);
		m_pModifyWnd->SetCode(m_strCode);
		m_pModifyWnd->ChangeSiseData();
		m_pCancelWnd->ShowWindow(SW_HIDE);
		break;
	case 3:
		m_pSellWnd->ShowWindow(SW_HIDE);
		m_pBuyWnd->ShowWindow(SW_HIDE);
		m_pModifyWnd->ShowWindow(SW_HIDE);
		m_pCancelWnd->ShowWindow(SW_SHOW);
		m_pCancelWnd->SetCode(m_strCode);
		break;
	}

	if (m_strGroupList.IsEmpty())
		QueryGroupList();
	else
		SetGroupList(m_strGroupList);
}

void CMapWnd::OnExpand()
{
	m_bExpendClick = TRUE;
	m_bExpand = !m_bExpand;

	int nWidth = m_iWndWidth;
	const int nHeight = m_iWndHeight;

	if (m_bExpand)
		m_pExpandWnd->ShowWindow(SW_SHOW);
	else
	{
		nWidth -= m_iExpendWidth;
		m_pExpandWnd->ShowWindow(SW_HIDE);
	}

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(nWidth, nHeight));
	MoveWindow(0, 0, nWidth, nHeight);

//	CRect rcCtrl;
//	rcCtrl.SetRect(SZ_FORM_MARGIN, SZ_FORM_MARGIN, nWidth - SZ_FORM_MARGIN, SZ_FORM_MARGIN + SZ_CTRL_HEIGHT);
//	m_pTab->MoveWindow(&rcCtrl);

//	rcCtrl.SetRect(SZ_FORM_MARGIN, nHeight - SZ_FORM_MARGIN - SZ_CTRL_HEIGHT * 2 - SZ_CTRL_VGAP, nWidth - SZ_FORM_MARGIN, 
//					nHeight - SZ_FORM_MARGIN - SZ_CTRL_HEIGHT - SZ_CTRL_VGAP);
//	m_pWarning->MoveWindow(&rcCtrl);

//	rcCtrl.SetRect(SZ_FORM_MARGIN, nHeight - SZ_FORM_MARGIN - SZ_CTRL_HEIGHT, nWidth - SZ_FORM_MARGIN, nHeight - SZ_FORM_MARGIN);
//	m_pGuide->MoveWindow(&rcCtrl);

	CRect rcClient, wrc;
	GetClientRect(&rcClient);

	m_pExpand->GetWindowRect(&wrc);
	ScreenToClient(wrc);

	m_pExpand->MoveWindow(rcClient.Width() - SZ_FORM_MARGIN - wrc.Width(), wrc.top,
								wrc.Width(), wrc.Height());
}


BOOL CMapWnd::SendTR(CString strTR, CString strData, int nKey, int nStat)
{
	CString strTRData = "";
	
	struct _userTH user;
	strncpy(&user.trc[0], strTR, 8);
	user.key  = nKey;
	user.stat = nStat;
	
	strTRData = CString((char*)&user, sizeof(_userTH));
	strTRData += strData;
	
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), 
		(LPARAM)(const char*)strTRData); 
}


long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		{
			if (m_pGuide)
				m_pGuide->SetText(_T(""));

			const int key = HIBYTE(LOWORD(wParam));
			const int len = HIWORD(wParam);
			CString strTemp;
			struct _ledgerH ledger;
			CString strLedger(_T(""));
			CString strErrCode(_T(""));
			CString strErrText(_T(""));
			CString strMsg(_T(""));
			int nNextKey = 0;

			CString stmp;
			stmp.Format("[memo][IB100700]  key =[%s]   len=[%d]\r\n", GetMemoTrKeyType(key), HIWORD(wParam));
			OutputDebugString(stmp);

			if ((key >= TRKEY_MEMO_POPUPOOP && key <= TRKEY_MEMO_CHECK) || key == 0)
			{
				switch (m_pTab->GetCurSel())
				{
				case 0:
				{
					if (m_pSellWnd)
						m_pSellWnd->m_pSymbol->SendMessage(WM_USER, wParam, lParam);
				}
				break;
				case 1:
				{
					if (m_pBuyWnd)
						m_pBuyWnd->m_pSymbol->SendMessage(WM_USER, wParam, lParam);
				}
				break;
				case 2:
				{
					if (m_pModifyWnd)
						m_pModifyWnd->m_pSymbol->SendMessage(WM_USER, wParam, lParam);
				}
				break;
				case 3:
				{
					if (m_pCancelWnd)
						m_pCancelWnd->m_pSymbol->SendMessage(WM_USER, wParam, lParam);
				}
				break;
				}
				break;
			}

			switch (key)
			{
			case KEY_CHKPASSWORD:
				{
					
					CopyMemory(&ledger, (void*)lParam, L_ledgerH);
					strErrCode = CString((char*)&ledger.ecod, 4);
					strErrText = CString((char*)&ledger.emsg, 98);
					strErrText.TrimRight(" "); 
					if(strErrCode.Left(1) == "0")
					{
						if(m_pCmdWnd)
							m_pCmdWnd->SendMessage(WM_USER+1, KEY_CHKPASSWORD, 1);
					}
					else
						AfxMessageBox(strErrCode + ":" + strErrText);

					m_pCmdWnd = NULL;
				}
				break;

			case KEY_SYMBOL:
				
				switch (m_pTab->GetCurSel())
				{
				case 0:
					if (m_pSellWnd)
						m_pSellWnd->ParsingSymbol(wParam, lParam);
					break;
				case 1:
					if (m_pBuyWnd)
						m_pBuyWnd->ParsingSymbol(wParam, lParam);
					break;
				case 2:
					if (m_pModifyWnd)
						m_pModifyWnd->ParsingSymbol(wParam, lParam);
					break;
				case 3:
					if (m_pCancelWnd)
						m_pCancelWnd->ParsingSymbol(wParam, lParam);
					break;
				}
				break;

			case KEY_SISE:
				ParsingSise((char*)lParam, HIWORD(wParam));
				break;
			case KEY_BUYABLEQTY:
				if (m_pBuyWnd)
					m_pBuyWnd->ParsingAbleQty((LPCSTR)lParam, HIWORD(wParam));

			case KEY_GROUPLIST:
			case KEY_REMAIN:
			case KEY_MODIFYNCDETAIL:
			case KEY_SELLORDER:
			case KEY_GROUPACCOUNT:
			case KEY_BUYORDER:
			case KEY_MODIFYNONCONTRACT:
			case KEY_MODIFYORDER:
			case KEY_CANCELNONCONTRACT:
			case KEY_CANCELNCDETAIL:
			case KEY_CANCELORDER:
				CopyMemory(&ledger, (void*)lParam, L_ledgerH);
				strLedger = CString((char*)&ledger, L_ledgerH);
				strErrCode = CString((char*)&ledger.emsg, 4);
				strErrText = CString((char*)&ledger.emsg, 98);
				nNextKey = ledger.next[0];
				
				strMsg = "ERR\t" + strErrText;
				OutputDebugString(strMsg+"\n");

				if (strErrCode.GetAt(0) != _T('0'))	// 오류
				{
					if (key == KEY_SELLORDER && m_pTab->GetCurSel() == 0)
						m_pSellWnd->ParsingOrder((char*)lParam, HIWORD(wParam), strMsg);
					else if (key == KEY_BUYORDER && m_pTab->GetCurSel() == 1)
						m_pBuyWnd->ParsingOrder((char*)lParam, HIWORD(wParam), strMsg);
					else if (key == KEY_MODIFYORDER && m_pTab->GetCurSel() == 2)
						m_pModifyWnd->ParsingOrder((char*)lParam, HIWORD(wParam), strMsg);
					else if (key == KEY_CANCELORDER && m_pTab->GetCurSel() == 3)
						m_pCancelWnd->ParsingOrder((char*)lParam, HIWORD(wParam), strMsg);
					else					
						strMsg.Format("[%s]%s", strErrCode, strErrText.Right(strErrText.GetLength() - 4));

					return 0;
				}

				strMsg = strErrText;

				lParam += L_ledgerH;

				if (key == KEY_GROUPLIST)
					ParsingGroupList((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_REMAIN && m_pTab->GetCurSel() == 0)
					m_pSellWnd->ParsingRemain((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_MODIFYNCDETAIL && m_pTab->GetCurSel() == 2)
					m_pModifyWnd->ParsingNCDetail((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_MODIFYNONCONTRACT && m_pTab->GetCurSel() == 2)
					m_pModifyWnd->ParsingNonContract((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_CANCELNCDETAIL && m_pTab->GetCurSel() == 3)
					m_pCancelWnd->ParsingNCDetail((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_CANCELNONCONTRACT && m_pTab->GetCurSel() == 3)
					m_pCancelWnd->ParsingNonContract((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_SELLORDER && m_pTab->GetCurSel() == 0)
					m_pSellWnd->ParsingOrder((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_BUYORDER && m_pTab->GetCurSel() == 1)
					m_pBuyWnd->ParsingOrder((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_GROUPACCOUNT)
					ParsingGroupAccount((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_MODIFYORDER && m_pTab->GetCurSel() == 2)
					m_pModifyWnd->ParsingOrder((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_CANCELORDER && m_pTab->GetCurSel() == 3)
					m_pCancelWnd->ParsingOrder((char*)lParam, HIWORD(wParam), strMsg);

				break;
			}
		}
		break;

	case DLL_TRIGGER:
		ParsingTrigger((char*)lParam);
		break;

	case DLL_DOMINO:
		ParsingDomino((char*)lParam);
		break;

	case DLL_SETPAL:
		SetPallete();
		break;

	case DLL_GUIDE:
		SetGuide((char*)lParam);
		return 1;

// 	case DLL_ALERT:
// 		struct _alertR* alertR;
// 		alertR = (struct _alertR*)lParam;
// 		ParseAlert(alertR);
// 		break;
	case DLL_ALERTx:
		struct _alertR* alertR;
		alertR = (struct _alertR*)lParam;
		ParseAlert(alertR);
		break;
	case DLL_SETFONT:break;
	case DLL_SETFONTx:
		ChangeFont( HIWORD(wParam), CString((char*)lParam) );

	}
	return 0;
}

void CMapWnd::SetPallete()
{
	LoadRoundBitmap();
	m_pTab->SetBorderColor(GetIndexColor(TAB_BORDER_INDEX));
	m_pTab->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CONTENTS_BACK_INDEX), 
					GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CONTENTS_BACK_INDEX), GetIndexColor(CONTENTS_BACK_INDEX));

	m_pTab->LoadTabImage(m_strRoot, false);
	
	if(m_pSellWnd)
		m_pSellWnd->SetPallete();

	if(m_pModifyWnd)
		m_pModifyWnd->SetPallete(this);

	if(m_pCancelWnd)
		m_pCancelWnd->SetPallete(this);

	if(m_pBuyWnd)
		m_pBuyWnd->SetPallete(this);

	m_pWarning->SetBkColor(GetIndexColor(GUIDE_BACK_INDEX));
	m_pWarning->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pWarning->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pWarning->SetCornerRound(m_hRoundBitmap);
	m_pWarning->SetBitmap(m_hGuideBitmap);


	m_pGuide->SetBkColor(GetIndexColor(CLR_GUIDE_BACK));
	m_pGuide->SetFgColor(GetIndexColor(CLR_GUIDE_FORE));
	m_pGuide->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pGuide->SetCornerRound(m_hRoundBitmap);
	m_pGuide->SetBitmap(m_hGuideBitmap);

	m_pGuide->Invalidate();

	Invalidate();
}

void CMapWnd::SetGuide(CString strGuide)
{
	if( strGuide.Find("그룹계좌") > -1 && strGuide.Find("존재") > -1 )
		return;

	m_pGuide->SetText(strGuide);
	SetTimer(TT_GUIDE, 5000, NULL);
}

void CMapWnd::Procedure(CString strProc)
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(GetMajor(), GetMinor())), (LPARAM)(LPCTSTR)strProc);
}

int CMapWnd::GetMajor()
{
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0L);
}

int CMapWnd::GetMinor()
{
	return m_pExpandWnd->GetMapKey();
}

BOOL CMapWnd::IsCustomer()
{
	return (long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L);
}

void CMapWnd::ParsingTrigger(CString strData)
{
	if (strData.IsEmpty())
		return;

	CString strData2 = strData;
	if (Parser(strData2, _T("\t")) == _T("ed_jprc"))
	{
		switch (m_pTab->GetCurSel())
		{
		case 0:
			m_pSellWnd->SetCost(strData2);
			break;
		case 1:
			m_pBuyWnd->SetCost(strData2);
			break;
		case 2:
			m_pModifyWnd->SetCost(strData2);
			break;
		}		
	}
	
	if (Parser(strData, _T("\t")) == _T("OnGroupAcc"))
	{
		m_strGroupList = strData;
		SetGroupList(strData);
	}
	else
	{
		while (!strData.IsEmpty())
		{
			CString str = Parser(strData, '\n');
			CString sym = Parser(str, '\t');

			if (sym.CompareNoCase("CODX") && sym.CompareNoCase("1301"))
				continue;
			
			CString strCodeName = Variant(nameDLL, str);
			
			if (strCodeName.IsEmpty())
				return;

			switch (m_pTab->GetCurSel())
			{
			case 0:
				m_pSellWnd->SetCode(str);
				break;
			case 1:
				m_pBuyWnd->SetCode(str);
				break;
			case 2:
				m_pModifyWnd->SetCode(str);
				break;
			case 3:
				m_pCancelWnd->SetCode(str);
				break;
			}
		}
	}
}

CString CMapWnd::Parser(CString &srcstr, CString substr)
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

CString CMapWnd::GetGroupList()
{
	return m_strGroupList;
}

void CMapWnd::QueryGroupList()
{
	CString strUser(_T(""));

	strUser = Variant(userCC);

	_TRIMDATA(strUser);

	struct gqry_mid mid;
	FillMemory(&mid, L_gqry_mid, ' ');

	CopyMemory(mid.In, _T("00001"), sizeof(mid.In));

	if (IsCustomer())
		mid.zUserTp[0] = _T('2');
	else
		mid.zUserTp[0] = _T('1');

	CopyMemory(mid.zCreUserId, (LPCTSTR)strUser, strUser.GetLength());
	
	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(ledger.svcd, _T("SONAQ038"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	CString strSendData = CString((char*)&ledger, L_ledgerH);

	m_strGroupList = "";
	strSendData += CString((char*)&mid, L_gqry_mid);
	SendTR("PIBOPBXQ", strSendData, KEY_GROUPLIST, 0);
}


void CMapWnd::ParsingGroupList(CString strData, int nLen, CString strMsg)
{
	int nRec = atoi((LPCTSTR)strData.Mid(L_gqry_mid, 5));

	strData.Delete(0, L_gqry_mid + 5);

	for (int i = 0; i < nRec; i++)
	{
		struct gqry_rec rec;

		CopyMemory(&rec, (LPCTSTR)strData.Left(L_gqry_rec), L_gqry_rec);
		
		if (rec.zFundKindTp[0] == _T('2'))
		{
			CString strGroupID(rec.zFundNm, sizeof(rec.zFundNm));
			CString strGroupName(rec.zCampDescr, sizeof(rec.zCampDescr));

			_TRIMDATA(strGroupID);
			_TRIMDATA(strGroupName);

			m_strGroupList += strGroupID;
			m_strGroupList += _T("\t");
			m_strGroupList += strGroupName;
			m_strGroupList += _T("\n");
		}

		strData.Delete(0, L_gqry_rec);
	}

	SetGroupList(m_strGroupList);
}

void CMapWnd::SetGroupList(CString strData)
{
	CString strItem(_T(""));
	CComboBox* pCombo = NULL;
	CComboBox* pComboID = NULL;

	switch (m_pTab->GetCurSel())
	{
	case 0:
		pCombo = m_pSellWnd->GetGroupCombo();
		pComboID = m_pSellWnd->GetGroupIDCombo();
		break;
	case 1:
		pCombo = m_pBuyWnd->GetGroupCombo();
		pComboID = m_pBuyWnd->GetGroupIDCombo();
		break;
	case 2:
		pCombo = m_pModifyWnd->GetGroupCombo();
		pComboID = m_pModifyWnd->GetGroupIDCombo();
		break;
	case 3:
		pCombo = m_pCancelWnd->GetGroupCombo();
		pComboID = m_pCancelWnd->GetGroupIDCombo();
		break;
	}

	if (pCombo == NULL)
		return;

	pCombo->ResetContent();
	pComboID->ResetContent();
	m_gpidMap.RemoveAll();

	while (!strData.IsEmpty())
	{
		strItem = Parser(strData, _T("\n"));
		CString sID = Parser(strItem, _T("\t"));
		pComboID->AddString((LPCTSTR)sID);

		const int nID = atoi(sID);
		const int nIndex = pCombo->AddString((LPCTSTR)strItem);

		m_gpidMap.SetAt(nIndex, sID);

		pCombo->SetItemData(nIndex, (DWORD)nID);
	}

	if (pCombo->GetCount() > 0)
	{
		pCombo->SetCurSel(0);

//		CString strGroupID(_T(""));
//		strGroupID.Format("%03d", pCombo->GetItemData(pCombo->GetCurSel()));
//		QueryGroupAccount(strGroupID);

		switch (m_pTab->GetCurSel())
		{
		case 0:
			m_pSellWnd->GroupListComplete();
			break;
		case 1:
			m_pBuyWnd->GroupListComplete();
			break;
		case 2:
			m_pModifyWnd->GroupListComplete();
			break;
		case 3:
			m_pCancelWnd->GroupListComplete();
			break;
		}
	}
}

void CMapWnd::ParsingDomino(CString strData)
{
	while (!strData.IsEmpty())
	{
		CString str = Parser(strData, '\n');
		CString sym = Parser(str, '\t');
		if (!sym.CompareNoCase("1301"))
		{
			switch (m_pTab->GetCurSel())
			{
			case 0:
				m_pSellWnd->SetCode(str);
				break;
			case 1:
				m_pBuyWnd->SetCode(str);
				break;
			case 2:
				m_pModifyWnd->SetCode(str);
				break;
			case 3:
				m_pCancelWnd->SetCode(str);
				break;
			}
		}
	}
}

void CMapWnd::QueryRemain(CString strGroupID, CString strCode)
{

}



void CMapWnd::OnTabChange(NMHDR * pHdr, LRESULT * result)
{
	if (pHdr->idFrom == IDC_TAB)
		ShowHideBodyWnd(m_pTab->GetCurSel());

	return;
}

void CMapWnd::ParseAlert(CString strAlert)
{
	if (m_strCode.IsEmpty())
		return;

	CString strCode(_T(""));
	CString strVal(_T(""));
	CString strSymbol(_T(""));
	int nPos = strAlert.Find(_T("\t"));

	if (nPos == -1)
		return;

	strCode = strAlert.Left(nPos++);
	
	if (!strCode.IsEmpty() && strCode.GetAt(0) == _T('A'))
		strCode.Delete(0);
	else
		return;

	if (m_strCode != strCode)
		return;

	strAlert = strAlert.Mid(nPos);
	
	while (!strAlert.IsEmpty())
	{
		nPos = strAlert.Find(_T("\t"));

		if (nPos == -1)
			break;

		strSymbol = strAlert.Left(nPos++);
		strAlert = strAlert.Mid(nPos);

		nPos = strAlert.Find(_T("\t"));

		if (nPos == -1)
			nPos = strAlert.GetLength();

		strVal = strAlert.Left(nPos++);

		DeleteSign(strVal);
		m_mapSise.SetAt(strSymbol, strVal);

		if (strAlert.GetLength() <= nPos)
			break;

		strAlert = strAlert.Mid(nPos);
	}

//	ChangeSiseData();
}

void CMapWnd::ParseAlert(struct _alertR* alertR)
{
	if (m_strCode.IsEmpty())
		return;
	
	CString strCode(_T(""));
	CString strVal(_T(""));
	CString strSymbol(_T(""));
	
	strCode = alertR->code;

	DWORD* data{};
	
	if (!strCode.IsEmpty() && strCode.GetAt(0) == _T('A'))
		strCode.Delete(0);
	else
		return;
	
	if (m_strCode != strCode)
		return;
	
	for (int i = alertR->size - 1; i > -1; --i)
	{		
		data = (DWORD*)alertR->ptr[i];

// 		if(data[0])
// 		{
// 			strVal = (char*)data[0];
// 			strSymbol.Format("%03d",0);
// 
// 			m_mapSise.SetAt(strSymbol, strVal);
// 		}

		if(data[23])
		{
			strVal = (char*)data[23];
			strSymbol.Format("%03d",0);
			
			m_mapSise.SetAt(strSymbol, strVal);
		}

		if(data[25])
		{
			strVal = (char*)data[25];
			strSymbol.Format("%03d",0);
			
			m_mapSise.SetAt(strSymbol, strVal);
		}

		if(data[26])
		{
			strVal = (char*)data[26];
			strSymbol.Format("%03d",0);
			
			m_mapSise.SetAt(strSymbol, strVal);
		}

		if(data[302])
		{
			strVal = (char*)data[302];
			strSymbol.Format("%03d",0);
			
			m_mapSise.SetAt(strSymbol, strVal);
		}

		if(data[303])
		{
			strVal = (char*)data[303];
			strSymbol.Format("%03d",0);
			
			m_mapSise.SetAt(strSymbol, strVal);
		}	
	}
	
	//	ChangeSiseData();
}

void CMapWnd::SetCode(CString strCode)
{
	if (m_strCode == strCode)
		return;

	m_strCode = strCode;

	if (m_strCode.IsEmpty())
	{
		m_mapSise.RemoveAll();
		ChangeSiseData();
		return;
	}

	CString strText;
	strText.Format("1301%c%s\t2301\t2023\t2025\t2026\t2302\t2303\t", 0x7f, m_strCode);

	SendTR(_T("POOPPOOP"), strText, KEY_SISE, US_OOP);	
}

void CMapWnd::ParsingSise(CString strData, int nLen)
{
	Parser(strData);

	CString strVal(_T(""));

	strVal = Parser(strData);
	DeleteSign(strVal);
	m_mapSise.SetAt(_T("023"), strVal);
	m_mapSise.SetAt(_T("000"), strVal);		// 지정가 초기값은 현재가로 셋팅

	strVal = Parser(strData);
	DeleteSign(strVal);
	m_mapSise.SetAt(_T("025"), strVal);

	strVal = Parser(strData);
	DeleteSign(strVal);
	m_mapSise.SetAt(_T("026"), strVal);

	strVal = Parser(strData);
	DeleteSign(strVal);
	m_mapSise.SetAt(_T("302"), strVal);

	strVal = Parser(strData);
	DeleteSign(strVal);
	m_mapSise.SetAt(_T("303"), strVal);

	ChangeSiseData();
}

CString CMapWnd::GetSiseData(CString strSymbol)
{
	CString strVal(_T(""));
	m_mapSise.Lookup(strSymbol, strVal);

	return strVal;
}

void CMapWnd::DeleteSign(CString &strVal)
{
	if (!strVal.IsEmpty())
	{
		if (strVal.GetAt(0) == _T('+') || strVal.GetAt(0) == _T('-'))
			strVal.Delete(0);
	}
}

void CMapWnd::FormatThousand(CString& strVal)
{
	int nThound = 0;
	
	strVal.Remove(_T(','));

	for (int i = 0; i < strVal.GetLength(); i++)
	{
		if ((i - nThound) % 3 == 0 && i != (3 * nThound) + nThound)
		{
			strVal.Insert(strVal.GetLength() - i, _T(","));
			nThound++;
		}
	}
}

void CMapWnd::ChangeSiseData()
{
	switch (m_pTab->GetCurSel())
	{
	case 0:
		if (m_pSellWnd)
			m_pSellWnd->ChangeSiseData();
		break;

	case 1:
		if (m_pBuyWnd)
			m_pBuyWnd->ChangeSiseData();
		break;

	case 2:
		if (m_pModifyWnd)
			m_pModifyWnd->ChangeSiseData();
		break;
	}
}

void CMapWnd::QueryGroupAccount(CString strGroupID)
{
	m_arrGroupAccount.RemoveAll();

	CString strUser(_T(""));
	CString strPass(_T(""));
	CString strGroupName(_T(""));

	strUser = Variant(userCC);
	_TRIMDATA(strUser);

	strPass = Variant(passCCx);
	_TRIMDATA(strPass);

	if (strGroupID.IsEmpty())
		return;

	struct aqry_mid mid;

	FillMemory(&mid, L_aqry_mid, ' ');
	CopyMemory(&mid.In, _T("00001"), sizeof(mid.In));
	
	if ((long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L) == 0)
		mid.zUserTp[0] = _T('1');
	else
		mid.zUserTp[0] = _T('2');

	CopyMemory(&mid.zCreUserId, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&mid.zPwd, (LPCTSTR)strPass, strPass.GetLength());
	CopyMemory(&mid.zFundNm, (LPCTSTR)strGroupID, strGroupID.GetLength());
	mid.zFundKindTp[0] = _T('2');

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.svcd, _T("SONAQ039"), sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '3';
	ledger.odrf[0] = '1';

	CString strSendData = CString((char*)&ledger, L_ledgerH);

	strSendData += CString((char*)&mid, L_aqry_mid);
	SendTR("PIBOPBXQ", strSendData, KEY_GROUPACCOUNT, 0);
}

void CMapWnd::ParsingGroupAccount(CString strData, int nLen, CString strMsg)
{
	int nRec = atoi((LPCTSTR)strData.Mid(L_aqry_mid, 5));
	OutputDebugString(strData+"\n");
	strData.Delete(0, L_aqry_mid + 5);

	CString strAccData(_T(""));

	for (int i = 0; i < nRec; i++)
	{
		struct aqry_rec rec;

		CopyMemory(&rec, (LPCTSTR)strData.Left(L_aqry_rec), L_aqry_rec);

		CString strAccount(rec.zAcntNo, sizeof(rec.zAcntNo));
		CString strAccName(rec.zAcntNm, sizeof(rec.zAcntNm));
		CString strRate(rec.dRatVal, sizeof(rec.dRatVal));
		
		_TRIMDATA(strAccount);
		_TRIMDATA(strAccName);
		_TRIMDATA(strRate);

		strAccount.Insert(3, _T('-'));
		strAccount.Insert(6, _T('-'));

		const double dRat = atof((LPCTSTR)strRate);

		strRate.Format("%.2f", dRat);

		strAccData.Format("%s\t%s\t%s\t", strAccount, strAccName, strRate);
		m_arrGroupAccount.Add(strAccData); 

		strData.Delete(0, L_aqry_rec);
	}

	if (m_pTab->GetCurSel() == 1)
		m_pBuyWnd->SetGroupAccount(&m_arrGroupAccount);
}

CString CMapWnd::GetAccountRate(CString strAccount)
{
	const int nCount = m_arrGroupAccount.GetSize();
	CString strAccountData(_T(""));

	for (int i = 0; i < nCount; i++)
	{
		strAccountData = m_arrGroupAccount.GetAt(i);

		if (strAccount == Parser(strAccountData))
		{
			Parser(strAccountData);
			return Parser(strAccountData);
		}
	}

	return _T("");
}

void CMapWnd::SetCustomPrice(CString strPrice)
{
	m_mapSise.SetAt(_T("000"), strPrice);
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TT_GUIDE)
	{
		m_pGuide->SetText(_T(""));
		KillTimer(TT_GUIDE);
	}
		
	CWnd::OnTimer(nIDEvent);
}

void CMapWnd::ChangeFont(int ifontsixze, LPCTSTR lszFont)
{
	CString slog;
	slog.Format("Cur:%d-Old:%d",  ifontsixze, m_iCurFont);
	if(m_pGuide)
		m_pGuide->SetWindowText(slog);

	if(!m_bInit) return;
	m_lszFontName = lszFont;
	if(m_iCurFont == ifontsixze) 
		return;


	const double ldif = float(ifontsixze - 9);

	m_pNormalFont = GetAxFont(lszFont, ifontsixze, false, 0);
	m_pBoldFont = GetAxFont(lszFont, ifontsixze, false, 1);
	if(!m_pNormalFont) return;

	const int iWndWidth = SZ_MAP_WIDTH + (int)(m_lSizeRate[0] * (float)SZ_MAP_WIDTH * ldif);
	const int iWndHeight = SZ_MAP_HEIGHT +(int) (m_lSizeRate[1] * (float)SZ_MAP_HEIGHT * ldif);

	m_iWndWidth = iWndWidth;
	m_iWndHeight = iWndHeight;
	
	this->SetFont(m_pNormalFont);
	m_pTab->SetFont(m_pNormalFont);
	m_pExpand->SetFont(m_pNormalFont);
	m_pGuide->SetFont(m_pNormalFont);
	
	m_iCurFont = ifontsixze;
	SetWindowPos(NULL, 0, 0, iWndWidth, iWndHeight, SWP_NOZORDER | SWP_NOMOVE);
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(iWndWidth, iWndHeight) );
	
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	if(cx == 0 || cy == 0) return;
	if(!m_bInit) return;
	if(m_bExpendClick)
	{
		m_bExpendClick = FALSE;
		return;
	}
	
	// TODO: Add your message handler code here
	if(m_iOldSize[0] == 0 && m_iOldSize[1] == 0)
	{
		m_iOldSize[0] = cx ;
		m_iOldSize[1] = cy ;
		return;
	}

	if(m_iOldSize[0] == cx && m_iOldSize[1] == cy) return;
	double lcalcrate[2] = {( (float)cx - (float)m_iOldSize[0] ) / (float)m_iOldSize[0], 
							( (float)cy - (float)m_iOldSize[1] ) /(float)m_iOldSize[1]};
	
	m_iOldSize[0] = cx ;
	m_iOldSize[1] = cy ;

	CRect wrc, rc, trc;
	int iw=0, ih=0, itop=0;
	GetClientRect(&rc);
	m_pTab->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	m_pTab->GetItemRect(0, trc);
	CSize tabsize = CSize(trc.right, trc.bottom);
	tabsize.cx = tabsize.cx + (int)(lcalcrate[0] * (float)tabsize.cx );
	tabsize.cy = tabsize.cy + (int)(lcalcrate[1] * (float)tabsize.cy );
	m_pTab->SetItemSize(tabsize);
	ih = tabsize.cy + 1;
	wrc.SetRect(wrc.left, wrc.top, wrc.left + iw, wrc.top+ih-1);
	m_pTab->MoveWindow(wrc );
	itop = wrc.top + ih;
	itop = wrc.top+ih-1;

	const int igridTop = itop;
	m_pExpand->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	int ileft = cx - iw - 5 ;
	m_pExpand->MoveWindow(ileft, wrc.top, iw, ih );

	
////////////////////////
	const int igap = 3;
	m_pWarning->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	int iright = cx - FRAME_SPACE;
	iw = wrc.Width(), ih = wrc.Height() ;
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	int iguideH = (ih* 2) + (igap*3);
	iguideH = (ih* 2) + INNER_FRAME_SPACE + 1;

	itop = rc.bottom - iguideH;
	const int iguidetop = itop - 1;
	m_pWarning->MoveWindow(wrc.left, itop, iright, ih);

	itop += (ih + 1);
	m_pGuide->MoveWindow(wrc.left, itop, iright, ih);

////////////////////////	
	m_pSellWnd->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	ileft = wrc.left;
	iw = wrc.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	ih = rc.bottom -iguideH-igridTop - INNER_FRAME_SPACE-INNER_FRAME_SPACE ;
	itop = igridTop;

	m_pSellWnd->MoveWindow(ileft, itop, iw, ih);
	m_pBuyWnd->MoveWindow(ileft, itop, iw, ih);
	m_pModifyWnd->MoveWindow(ileft, itop, iw, ih);
	m_pCancelWnd->MoveWindow(ileft, itop, iw, ih);
	m_pSellWnd->ChangeSize(lcalcrate, m_iCurFont, m_lszFontName);
	m_pBuyWnd->ChangeSize(lcalcrate, m_iCurFont, m_lszFontName);
	m_pModifyWnd->ChangeSize(lcalcrate, m_iCurFont, m_lszFontName);
	m_pCancelWnd->ChangeSize(lcalcrate, m_iCurFont, m_lszFontName);

	ileft = ileft + iw+1;
	iright = cx - ileft - igap;

	m_pExpandWnd->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width();
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	m_pExpandWnd->MoveWindow(ileft, itop, iright, ih);
	m_iExpendWidth = iright;

}

void CMapWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	m_bInit = FALSE;
	
}


void CMapWnd::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap)
{
	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, hBitmap); //icon
//	::BitBlt( pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(),	hDC, 0, 0, SRCCOPY);
	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);

	::DeleteDC (hDC);
}

void CMapWnd::DrawCornerRoundBitmap(CDC* pDC, CRect rc, int iType)
{
	const int iPoint = 5;
	int idx = 0;
	if(iType) idx = 4;

	DrawBitmap(pDC, CRect(rc.left, rc.top, rc.left+iPoint, rc.top +iPoint), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.top, rc.right, rc.top +iPoint), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.left, rc.bottom-iPoint, rc.left+iPoint, rc.bottom), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.bottom-iPoint, rc.right, rc.bottom), m_hRoundBitmap[idx++]);
}


void CMapWnd::CheckPassWord(CString sAcc, CString sPass, CWnd* pCmdWnd) 
{
	m_pCmdWnd = pCmdWnd;
	CString sTRCode = _T("SACMQ901");
	sTRCode = _T("SACMT238");
	
	CString sUser = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), NULL);

	struct	_ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

//	CopyMemory(&ledger.size, "000374", sizeof(ledger.size));
	CopyMemory(&ledger.svcd, sTRCode, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)sUser, sUser.GetLength());
	CopyMemory(&ledger.brno, sAcc.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';
//00
	//sAcc = sAcc.Left(3) + "00" +  sAcc.Mid(5, 6);  //종합잔고로 변경
	for(int i=sAcc.GetLength() ; i<20;i++) sAcc += _T(" ");
	
	for(int i=sPass.GetLength() ; i<8;i++) sPass += _T(" ");
	CString sChkPass = _T("00001");
	sChkPass += sAcc;
	sChkPass += sPass;
	//sChkPass += _T("1");

	CString strSendData = CString((char*)&ledger, L_ledgerH);
	strSendData += sChkPass;

	CString sTmp;
	sTmp.Format("[%d]%s",  strSendData.GetLength(), strSendData);

	SendTR(_T("piboPBxQ"), strSendData, KEY_CHKPASSWORD, 0);
}

CString CMapWnd::GetGroupID( int nIndex )
{
	CString strID;
	if (m_gpidMap.Lookup(nIndex, strID)) return strID;
	else return "";
}
