// ToolWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "ToolWnd.h"
#include "../../control/fx_misc/misctype.h"
#include "IHCheck.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SIZE_TABITEMWIDTH	80
#define SIZE_TABITEMHEIGHT	19
/////////////////////////////////////////////////////////////////////////////
// CToolWnd

CToolWnd::CToolWnd(CWnd* pMainWnd) : CBaseWnd(pMainWnd)
{
	m_activefield = _T("");
	m_bBitmap = true;
	m_bBig = false;

	// ADD PSH 20070918
	m_bMoveCfg  = FALSE;
	m_nMoveSave = 0;
	m_selectedTab = -1;
	// END ADD
}

CToolWnd::~CToolWnd()
{
}


BEGIN_MESSAGE_MAP(CToolWnd, CBaseWnd)
	//{{AFX_MSG_MAP(CToolWnd)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_BTNTAB, OnSelchangeBtntab)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CToolWnd message handlers


void CToolWnd::OperInit()
{
	m_fileIMG.Format("%s\\%s\\", m_root, IMAGEDIR);
	HBITMAP	hBITMAP, hBITMAP_DN, hBITMAP_HV;

	// 트리 펼치기 버튼
	hBITMAP = getBitmap(m_fileIMG + "LEFT_EX.bmp");
	hBITMAP_DN = getBitmap(m_fileIMG + "LEFT_EX_dn.bmp");
	hBITMAP_HV = getBitmap(m_fileIMG + "LEFT_EX_en.bmp");

	m_btPREV.Create("\n종목트리열기", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_PREV);
	m_btPREV.SetFont(m_pFont, false);  
	m_btPREV.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	// 트리 숨기기 버튼
	hBITMAP = getBitmap(m_fileIMG + "RIGHT_EX.bmp");
	hBITMAP_DN = getBitmap(m_fileIMG + "RIGHT_EX_dn.bmp");
	hBITMAP_HV = getBitmap(m_fileIMG + "RIGHT_EX_en.bmp");

	m_btNEXT.Create("\n종목트리닫기", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_NEXT);
	m_btNEXT.SetFont(m_pFont, false);  
	m_btNEXT.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	// 툴바줄이기
// 	hBITMAP = getBitmap(m_fileIMG + "UPMOVE.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "UPMOVE_dn.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "UPMOVE_en.bmp");
// 
// 	m_btUP.Create("\n툴바줄이기", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_UP);
// 	m_btUP.SetFont(m_pFont, false);  
// 	m_btUP.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	// 툴바늘이기
// 	hBITMAP = getBitmap(m_fileIMG + "DOWNMOVE.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "DOWNMOVE_dn.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "DOWNMOVE_en.bmp");
// 
// 	m_btDN.Create("\n툴바늘이기", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_DN);
// 	m_btDN.SetFont(m_pFont, false);  
// 	m_btDN.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	// 텍스트 설정 버튼 (설정/예상/엑셀)
/*	hBITMAP = getBitmap(m_fileIMG + "2BTN.bmp");
	hBITMAP_DN = getBitmap(m_fileIMG + "2BTN_dn.bmp");
	hBITMAP_HV = getBitmap(m_fileIMG + "2BTN_en.bmp");
	UINT	ids[] = {IDC_BT_CONFIG, IDC_BT_EXPECT};
	char*	texts[] = {"설정\n관심종목설정", "예상\n예상체결가"};	
	CString textExpect = "자동\n자동으로 예상체결가보기";

	bool	bchecks[] = {false, true};

	for ( int ii = 0 ; ii < 2 ; ii++ )
	{
		m_btTEXT[ii].Create(texts[ii], CRect(0, 0, 36, SIZE_BUTTONS), this, ids[ii], bchecks[ii]);
		m_btTEXT[ii].SetFont(m_pFont, false);  
		m_btTEXT[ii].SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	}
*/
////////////////////////////////////////////////
	hBITMAP = getBitmap(m_fileIMG + "조회BTN.BMP");
	hBITMAP_DN = getBitmap(m_fileIMG + "조회BTN_dn.BMP");
	hBITMAP_HV = getBitmap(m_fileIMG + "조회BTN_dn.BMP");
	UINT	ids[] = {IDC_BT_CONFIG, IDC_BT_EXPECT};
	char*	texts[] = {"설정\n관심종목설정", "예상\n예상체결가"};	
	CString textExpect = "자동\n자동으로 예상체결가보기";

	bool	bchecks[] = {false, true};

	CFont *font;
	font = GetAxFont("굴림",9,true);

	m_btTEXT[0].Create(texts[0], CRect(0, 0, 38, SIZE_BUTTONS), this, ids[0], bchecks[0]);
	m_btTEXT[0].SetFont(font, false);  
	m_btTEXT[0].SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btTEXT[0].SetTextColor(GetAxColor(186));//BUFFET

	hBITMAP = getBitmap(m_fileIMG + "2BTN.bmp");
	hBITMAP_DN = getBitmap(m_fileIMG + "2BTN_dn.bmp");
	hBITMAP_HV = getBitmap(m_fileIMG + "2BTN_en.bmp");

	m_btTEXT[1].Create(texts[1], CRect(0, 0, 32, SIZE_BUTTONS), this, ids[1], bchecks[1]);
	m_btTEXT[1].SetFont(m_pFont, false);  
	m_btTEXT[1].SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
///////////////////////////////////////////////////
	m_chkAUTO = new CIHCheck("자동 예상체결 보기");
	m_chkAUTO->Create(textExpect, WS_CHILD|BS_AUTOCHECKBOX|BS_LEFT|BS_VCENTER|BS_FLAT, CRect(0, 0, 0, 0), this, IDC_BT_AUTO);
	m_chkAUTO->SetFont(m_pFont);

// 	hBITMAP = getBitmap(m_fileIMG + "MINUSBTN.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "MINUSBTN_dn.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "MINUSBTN_en.bmp");
// 
// 	m_btMINUS.Create("\n화면줄이기", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_MINUS);
// 	m_btMINUS.SetFont(m_pFont, false);  
// 	m_btMINUS.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// 
// 	hBITMAP = getBitmap(m_fileIMG + "H버튼.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "H버튼_dn.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "H버튼_en.bmp");
// 
// // 	m_btHAWKEYE.Create("\n호크아이", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_HAWKEYE, true);
// // 	m_btHAWKEYE.SetFont(m_pFont, false);  
// // 	m_btHAWKEYE.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// // 	m_btHAWKEYE.ShowWindow(SW_HIDE);
// 
// 	hBITMAP = getBitmap(m_fileIMG + "PLUSBTN.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "PLUSBTN_dn.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "PLUSBTN_en.bmp");
// 	
// 	m_btPLUS.Create("\n화면늘이기", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_PLUS);
// 	m_btPLUS.SetFont(m_pFont, false);  
// 	m_btPLUS.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

// 	m_chkEXPAND = new CIHCheck("그룹추가시 화면확장");
// 	m_chkEXPAND->Create("확장", WS_CHILD|BS_CHECKBOX|BS_AUTOCHECKBOX|BS_LEFT|BS_VCENTER|BS_FLAT, CRect(0, 0, 0, 0), this, IDC_CHECK_PLUSMINUS);
// 	m_chkEXPAND->SetFont(m_pFont);

	 

// 	hBITMAP = getBitmap(m_fileIMG + "EQUALIZER.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "EQUALIZER_dn.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "EQUALIZER_en.bmp");
// 	
// 	m_btEqualizer.Create("\n체결 이퀄라이저", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_EQUALIZER, true);
// 	m_btEqualizer.SetFont(m_pFont, false);  
// 	m_btEqualizer.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

// 	int	ncount = GetPrivateProfileInt(SEC_GROUP, KEY_COUNT, 1, m_fileCFG);
// 
// 	if (ncount == 1)
// 		m_btMINUS.EnableWindow(FALSE);
// 	else if (ncount == MAX_GROUP)
// 		m_btPLUS.EnableWindow(FALSE);
// 
// 	m_cbFIELD.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_FIELD);
// 	m_cbFIELD.SetFont(m_pFont);
	//김덕기 추가
	m_cbSort.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_SORT);
	m_cbSort.SetFont(m_pFont);

	m_cbEach.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_EACH);
	m_cbEach.SetFont(m_pFont);
	
	hBITMAP = getBitmap(m_fileIMG + "2BTN.bmp");
	hBITMAP_DN = getBitmap(m_fileIMG + "2BTN_dn.bmp");
	hBITMAP_HV = getBitmap(m_fileIMG + "2BTN_en.bmp");

	m_btHoga.Create("호가\n호가",CRect(0,0,36,SIZE_BUTTONS),this,IDC_BT_HOGA);
	m_btHoga.SetFont(m_pFont);
	m_btHoga.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	/////////////

	m_cbGROUP.Create(CBS_DROPDOWNLIST | WS_VSCROLL, CRect(0, 0, 0, 0), this, IDC_CB_GROUP);
	m_cbGROUP.SetFont(m_pFont);

	hBITMAP = getBitmap(m_fileIMG + "TURN.bmp");
	hBITMAP_DN = getBitmap(m_fileIMG + "TURN_dn.bmp");
	hBITMAP_HV = getBitmap(m_fileIMG + "TURN_en.bmp");

	m_btTURN.Create("\n돌려보기", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_TURN, true);
	m_btTURN.SetFont(m_pFont, false);  
	m_btTURN.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	m_spinTIME = new CfxSpin(m_pFont); 
	m_spinTIME->Create(WS_CHILD, spNUMBER, CRect(0, 0, 0, 0), this, IDC_SPIN_TIME);
	m_spinTIME->Init(attrNUMERIC, spNUMBER, this); m_spinTIME->SetRangeEx(1, 60);	

	m_btntab.Create(WS_CHILD|TCS_BUTTONS|TCS_OWNERDRAWFIXED|TCS_RAGGEDRIGHT, CRect(0, 0, 0, 0), this, IDC_BTNTAB);
	m_btntab.SetFont(m_pFont, false);
	m_btntab.SetColor(RGB(255, 255, 255), GetAxColor(14), RGB(255, 255, 32), GetAxColor(14), m_clrPallete);
	m_btntab.m_bitmapT_nm = getBitmap(m_fileIMG + "관심TAB.bmp");
	m_btntab.m_bitmapT_dn = getBitmap(m_fileIMG + "관심TAB_dn.bmp");
	m_btntab.m_bitmapT_hv = getBitmap(m_fileIMG + "관심TAB_en.bmp");

	// 차트 버튼
// 	hBITMAP = getBitmap(m_fileIMG + "schart.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "schart_dn.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "schart_en.bmp");
// 
// 	m_btCHART.Create("\n툴팁차트끄기", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_CHART, true);
// 	m_btCHART.SetFont(m_pFont, false);  
// 	m_btCHART.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// 	m_btCHART.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

	// 선택한 화면에만 추가
// 	hBITMAP = getBitmap(m_fileIMG + "ADDSELECT.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "ADDSELECT_DN.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "ADDSELECT_EN.bmp");
// 
// 	m_btAddSelect.Create("\n선택한 화면에만 조회", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_ADDSELECT, true);
// 	m_btAddSelect.SetFont(m_pFont, false);  
// 	m_btAddSelect.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// 	m_btAddSelect.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
// 
// 
// 	// 보이는 곳까지 추가
// 	hBITMAP = getBitmap(m_fileIMG + "ADDVISIBLE.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "ADDVISIBLE_DN.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "ADDVISIBLE_EN.bmp");
// 
// 	m_btAddVisible.Create("\n화면의 보이는 곳까지 종목추가후 나머지 다음화면으로", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_ADDVISIBLE, true);
// 	m_btAddVisible.SetFont(m_pFont, false);  
// 	m_btAddVisible.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// 	m_btAddVisible.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);


	// 최대한 채우고 다음화면으로 추가
// 	hBITMAP = getBitmap(m_fileIMG + "ADDFULL.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "ADDFULL_DN.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "ADDFULL_EN.bmp");
// 
// 	m_btAddFull.Create("\n화면의 최대개수까지 추가후 나머지 다음화면으로", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_ADDFULL, true);
// 	m_btAddFull.SetFont(m_pFont, false);  
// 	m_btAddFull.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// 	m_btAddFull.SetWindowPos(&wndTopMost, -5000, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	
// 	m_btAddFull.ShowWindow(SW_HIDE);
// 
// 	// 장바구니 버튼
// 	hBITMAP = getBitmap(m_fileIMG + "3btn.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "3btn_dn.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "3btn_en.bmp");
// 
// 	m_btBASKET.Create("바구니\n종목바구니", CRect(0, 0, 36, 37), this, IDC_BT_BASKET, true);
// 	m_btBASKET.SetFont(m_pFont, false);  
// 	m_btBASKET.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// 	m_btBASKET.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	
	// ADD PSH 20070911
// 	hBITMAP = getBitmap(m_fileIMG + "빈2.bmp");
// 	hBITMAP_DN = getBitmap(m_fileIMG + "빈2_dn.bmp");
// 	hBITMAP_HV = getBitmap(m_fileIMG + "빈2_en.bmp");
// 	m_btTool.Create("도구", CRect(0, 0, 88, SIZE_BUTTONS), this, IDC_MENU_TOOL);
// 	m_btTool.SetFont(m_pFont, false);  
// 	m_btTool.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// 	m_btTool.SetTextColor(GetAxColor(186));//BUFFET

	// 필드
// 	hBITMAP		= getBitmap(m_fileIMG + "빈3.bmp");
// 	hBITMAP_DN	= getBitmap(m_fileIMG + "빈3_DN.bmp");
// 	hBITMAP_HV	= getBitmap(m_fileIMG + "빈3_EN.bmp");
// 
// 	m_btField.Create("", CRect(0, 0, 85, SIZE_BUTTONS), this, IDC_BT_FIELD);
// 	m_btField.SetFont(m_pFont, false);  
// 	m_btField.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// 
// 	// 필드
// 	hBITMAP		= getBitmap(m_fileIMG + "ARROW.bmp");
// 	hBITMAP_DN	= getBitmap(m_fileIMG + "ARROW_DN.bmp");
// 	hBITMAP_HV	= getBitmap(m_fileIMG + "ARROW_EN.bmp");
// 
// 	m_btFieldDrop.Create("", CRect(0, 0, 16, SIZE_BUTTONS), this, IDC_BT_FIELDDROP);
// 	m_btFieldDrop.SetFont(m_pFont, false);  
// 	m_btFieldDrop.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// 
// 	m_menuField.CreatePopupMenu();
	// END ADD
	
	loadcfg();
}

void CToolWnd::OperDestory()
{
	savecfg();
	
	m_btPREV.DestroyWindow();
	m_btNEXT.DestroyWindow();
//	m_btBASKET.DestroyWindow();

	for ( int ii = 0 ; ii < 2 ; ii++ )
	{
		m_btTEXT[ii].DestroyWindow();
	}

//	m_btHAWKEYE.DestroyWindow();
// 	m_btMINUS.DestroyWindow();
// 	m_btPLUS.DestroyWindow();
	m_btTURN.DestroyWindow();
// 	m_btCHART.DestroyWindow();
// 	m_btAddSelect.DestroyWindow();
// 	m_btAddVisible.DestroyWindow();
// 	m_btAddFull.DestroyWindow();
// 	m_btEqualizer.DestroyWindow();

	if (m_chkAUTO)
	{
		m_chkAUTO->DestroyWindow();
		delete m_chkAUTO;
	}

// 	if (m_chkEXPAND)
// 	{
// 		m_chkEXPAND->DestroyWindow();
// 		delete m_chkEXPAND;
// 	}

	if (m_spinTIME)
	{
		m_spinTIME->DestroyWindow();
		delete m_spinTIME;
	}

//	m_cbFIELD.DestroyWindow();
	m_cbGROUP.DestroyWindow();
	m_btntab.DestroyWindow();

}

HBITMAP CToolWnd::getBitmap(CString path)
{
	CBitmap*	pBitmap = (CBitmap*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETBITMAP, (LPARAM)(char*)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

void CToolWnd::OperResize(int cx, int cy)
{
	m_rect = CRect(0, 0, cx, cy);
	
	if (cx <= 0 || cy <= 0)
		return;

	CRect	rect = m_rect;

	m_btPREV.ShowWindow(SW_HIDE);
	m_btNEXT.ShowWindow(SW_HIDE);
// 	m_btUP.ShowWindow(SW_HIDE);
// 	m_btDN.ShowWindow(SW_HIDE);

	// ADD PSH 20070911
	CRect	rcTemp, rctmp;
	// END ADD

	if (!IsInit())
	{
		m_arDIVIDE.RemoveAll();
		
		// DEL PSH 20070911
		//CRect	rcTemp, rctmp;
		// END DEL

		// 첫번째 컨트롤
		// tree버튼	
		//rect.OffsetRect(3, 2);
		rect.OffsetRect(4, 3);
		rect.right = rect.left + SIZE_BUTTONS;
		rect.bottom = rect.top + SIZE_BUTTONS;
		m_btPREV.MoveWindow(rect, FALSE);	
		m_btNEXT.MoveWindow(rect, FALSE);

// 		rect = GetNextRect(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS, 0));
// 		m_btUP.MoveWindow(rect, FALSE);
// 		m_btDN.MoveWindow(rect, FALSE);
		
		
		// 콤보
		rect = GetNextRect(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS, 0));
		
		// 업버튼
		
		// fields콤보		
		rect.right = rect.left + SIZE_COMBO;
		rect.bottom = rect.top + SIZE_DROPDOWN;

		// group 콤보
		m_cbGROUP.MoveWindow(rect, FALSE);
		m_cbGROUP.ShowWindow(SW_SHOW);		

		rect = MoveButton(rect, CSize(38, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btTEXT[0]);
		rect = MoveButton(rect, CSize(36, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btTEXT[1]);
		
		
		rect = GetNextRect(rect, CSize(42, SIZE_BUTTONS), CSize(SIZE_GAPS, 0));
		rctmp = rect;
		rctmp.OffsetRect(0, 1);
		
		m_chkAUTO->MoveWindow(rctmp, FALSE);
		m_chkAUTO->ShowWindow(SW_SHOW);		
		m_arCONTROL.Add((CWnd*)m_chkAUTO);

		rect = GetNextRect(rect,CSize(38, SIZE_BUTTONS), CSize(SIZE_GAPS , 0));
		rect.right = rect.left + SIZE_COMBO;
		rect.bottom = rect.top + SIZE_DROPDOWN;

		m_cbSort.MoveWindow(rect,FALSE);
		m_cbSort.ShowWindow(SW_SHOW);

		rect = GetNextRect(rect,CSize(38,SIZE_BUTTONS),CSize(SIZE_GAPS, 0));

		rect.right = rect.left + SIZE_COMBO;
		rect.bottom = rect.top + SIZE_DROPDOWN;

		m_cbEach.MoveWindow(rect,FALSE);
		m_cbEach.ShowWindow(SW_SHOW);

		rect = MoveButton(rect, CSize(38,SIZE_BUTTONS), CSize(SIZE_GAPS, 0), &m_btHoga);

		//ScreenToClient(rect);
		CRect tRect;

		GetWindowRect(tRect);

		rect.left = tRect.Width() - 75;
		rect.right = rect.left + 20;

		m_btTURN.MoveWindow(rect,FALSE);
		m_btTURN.ShowWindow(SW_SHOW);

		//rect = MoveButton(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btTURN);
		// 
 		// 수치입력
	 	rect = GetNextRect(rect, CSize(44, SIZE_BUTTONS), CSize(SIZE_GAPS, 0));
		m_spinTIME->MoveWindow(rect, FALSE);
 		m_spinTIME->ShowWindow(SW_SHOW);
		m_spinTIME->Invalidate();
 		m_arCONTROL.Add((CWnd*)m_spinTIME);

		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 둘째 라인 컨트롤
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		m_btNEXT.GetWindowRect(rect);
		ScreenToClient(rect);

		{
			CRect	rcTAB;
			// MODIFY PSH 20070910 - Basket을 현재 사용하지 않으므로 Tab을 화면 끝까지 확대
			//rcTAB.right = rect.left - SIZE_GAPS;
			rcTAB.right = m_rect.right;
// 			// END MODIFY
// 			//m_cbGROUP.GetWindowRect(rect);
// 
 			rcTAB.top = rect.bottom + 4;
 			rcTAB.bottom = rcTAB.top + SIZE_TABITEMHEIGHT + 2;
 			rcTAB.left = rect.left;		
			m_btntab.MoveWindow(rcTAB);
			m_btntab.SetItemSize(CSize(SIZE_TABITEMWIDTH, SIZE_TABITEMHEIGHT));
			
			if (!IsInit() && !m_btntab.IsWindowVisible())
			{
				m_btntab.ShowWindow(SW_SHOW);			
			}
		}

// 		rect = MoveButton(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(0 , SIZE_GAPS + 1), &m_btCHART);
// 
// 	
// 		m_cbGROUP.GetWindowRect(rcTemp);
// 		ScreenToClient(rcTemp);		

//		rect = GetNextRect(rcTemp, CSize(SIZE_COMBO, SIZE_BUTTONS), CSize(0, SIZE_GAPS));
		// MODIFY PSH 20070911
		/*
		m_rcFIELD = rect = GetNextRect(rect, CSize(SIZE_COMBO, SIZE_BUTTONS), CSize(SIZE_GAPS, 0));
		rect.left = rcTemp.left;
		m_rcFIELD.right = rect.left - 1;
		rect.right = rect.left + SIZE_COMBO;
		rect.bottom = rect.top + SIZE_DROPDOWN;
		m_cbFIELD.MoveWindow(rect, FALSE);
		m_cbFIELD.ShowWindow(SW_SHOW);		
		*/
// 		rect = MoveButton(rect, CSize(85, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btField);
// 		rect.left  = rect.right;
// 		rect.right = rect.left + 16;
// 		m_btFieldDrop.MoveWindow(rect, FALSE);
// 		m_btFieldDrop.ShowWindow(SW_SHOW);
//		m_arCONTROL.Add((CWnd*)&m_btFieldDrop);
		// END MODIFY		
		
//		rect = MoveButton(rect, CSize(38, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btTEXT[0]);

		// plus minus
// 		rect = MoveButton(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btMINUS);
// 		rect = MoveButton(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btPLUS);

		// 시세포착
//		rect = MoveButton(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btHAWKEYE);

// 		rect = GetNextRect(rect, CSize(42, SIZE_BUTTONS), CSize(SIZE_GAPS, 0));
// 		rctmp = rect;
// 		rctmp.OffsetRect(0, 1);
// 		m_chkEXPAND->MoveWindow(rctmp, FALSE);
// 		m_chkEXPAND->ShowWindow(SW_SHOW);
// 		m_arCONTROL.Add((CWnd*)m_chkEXPAND);

// 		rect = MoveButton(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btEqualizer);
// 		
// 		m_btEqualizer.ShowWindow(SW_HIDE);
	}

	{
// 		m_btFieldDrop.GetWindowRect(rect);
// 		ScreenToClient(rect);
// 
// 		int nMove	= m_rect.Width() - 218;
// 		nMove = max(nMove, 320)+25;
// 		rect.left	= nMove;
// 		rect.right	= nMove;
// 
// //		rect.top = rcTemp.top;
// // 		rcTemp = rect = MoveButton(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btAddSelect);
// // 		rect = MoveButton(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btAddVisible);
// // //		rect = MoveButton(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btAddFull);
// // 		m_btAddFull.ShowWindow(SW_HIDE);
// 
// 		// 도구
// 		//rcTemp = rect = MoveButton(rect, CSize(36, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btTool);
// 		rect = MoveButton(rect, CSize(36, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btTEXT[1]);
// 		
// 		
// 		rect = GetNextRect(rect, CSize(42, SIZE_BUTTONS), CSize(SIZE_GAPS, 0));
// 		rctmp = rect;
// 		rctmp.OffsetRect(0, 1);
// 		
// 		m_chkAUTO->MoveWindow(rctmp, FALSE);
// 		m_chkAUTO->ShowWindow(SW_SHOW);		
// 		m_arCONTROL.Add((CWnd*)m_chkAUTO);
// 
// 		// END ADD
// 
// 		// divide 2
// 		// DEL PSH 20070911
// 		//rect = AddDivide(rect);
// 		// END DEL
// 
// 		// 돌려보기
// 		rect = MoveButton(rect, CSize(SIZE_BUTTONS, SIZE_BUTTONS), CSize(SIZE_GAPS , 0), &m_btTURN);
// 
// 		// 수치입력
// 		rect = GetNextRect(rect, CSize(44, SIZE_BUTTONS), CSize(SIZE_GAPS, 0));
// 		m_spinTIME->MoveWindow(rect, FALSE);
// 		m_spinTIME->ShowWindow(SW_SHOW);
// 		m_spinTIME->Invalidate();
// 		m_arCONTROL.Add((CWnd*)m_spinTIME);
// 
// 		// 초
// 		rect = GetNextRect(rect, CSize(10, SIZE_BUTTONS), CSize(SIZE_GAPS, 0));
// 		m_rcSECOND = rect;


	}


	if (m_pMainWnd->SendMessage(WM_MANAGE, MK_TREEVISIBLE))
		m_btNEXT.ShowWindow(SW_SHOW);		
	else
		m_btPREV.ShowWindow(SW_SHOW);

// 	if (m_bBig)
// 		m_btUP.ShowWindow(SW_SHOW);
// 	else
// 		m_btDN.ShowWindow(SW_SHOW);

// 	rect = m_rect;
// 	rect.OffsetRect(-4, 2);
// 	rect.bottom = rect.top + SIZE_BUTTONS;
// 	rect.left = rect.right - 55;
// 	m_btBASKET.MoveWindow(rect, FALSE);

// 	if (m_pMainWnd->SendMessage(WM_MANAGE, MK_BASKETVISIBLE))
// 		m_btBASKET.SetCheck(TRUE);	
// 	else
// 		m_btBASKET.SetCheck(FALSE);
		

// 	if (!IsInit() && !m_btBASKET.IsWindowVisible())
// 	{
// 		m_btBASKET.ShowWindow(SW_SHOW);
// 	}
// 	m_btBASKET.ShowWindow(SW_HIDE);

// 	{
// 		CRect	rcTAB;
// 		// MODIFY PSH 20070910 - Basket을 현재 사용하지 않으므로 Tab을 화면 끝까지 확대
// 		//rcTAB.right = rect.left - SIZE_GAPS;
// 		rcTAB.right = m_rect.right;
// 		// END MODIFY
// 		//m_cbGROUP.GetWindowRect(rect);
// 		ScreenToClient(rect);
// 		rcTAB.top = rect.top + 1;
// 		rcTAB.bottom = rcTAB.top + SIZE_TABITEMHEIGHT + 2;
// 		rcTAB.left = rect.right + 1;		
// 		m_btntab.MoveWindow(rcTAB);
// 		m_btntab.SetItemSize(CSize(SIZE_TABITEMWIDTH, SIZE_TABITEMHEIGHT));
// 
// 		if (!IsInit() && !m_btntab.IsWindowVisible())
// 		{
// 			m_btntab.ShowWindow(SW_SHOW);			
// 		}
// 	}
}



BOOL CToolWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	BOOL	bCheck = FALSE;
	CString	sztmp = _T("");
	int	GroupNumber = 0;
	int	nOver = 0;

	switch (LOWORD(wParam))
	{
	case IDC_BT_UP:
		m_bBig = false;
		m_pMainWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE);
		break;
	case IDC_BT_DN:
		m_bBig = true;
		m_pMainWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE);
		break;
	case IDC_BT_NEXT:
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEVISIBLE, IDC_BT_NEXT));
		break;
	case IDC_BT_PREV:
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEVISIBLE, IDC_BT_PREV));
		break;
	case IDC_BT_BASKET:
		//m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_BASKETVISIBLE, (m_btBASKET.IsChecked()) ? MO_SHOW : MO_HIDE));
		break;
	case IDC_BT_HAWKEYE:
		{
			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			pWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		}
		break;
	case IDC_BT_PLUS:
		{
// 			int	ncnt = m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GROUP, 1), (LPARAM)m_chkEXPAND->GetCheck());
// 			if (ncnt >= MAX_GROUP)
// 				m_btPLUS.EnableWindow(FALSE);
// 			else if (ncnt == 2)
// 				m_btMINUS.EnableWindow(TRUE);
		}
		break;
	case IDC_BT_MINUS:
		{
// 			int	ncnt = m_pMainWnd->SendMessage(WM_MANAGE, MK_GROUP, (LPARAM)m_chkEXPAND->GetCheck());
// 			if (ncnt == 1)
// 				m_btMINUS.EnableWindow(FALSE);
// 			else if (ncnt == (MAX_GROUP - 1))
// 				m_btPLUS.EnableWindow(TRUE);
// 
// 			// ADD PSH 20070911
// 			Invalidate();
// 			// END ADD
		}
	case IDC_BT_TURN:
		{
			bCheck = m_btTURN.IsChecked();
			int	nsec = 0;
			CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			if (bCheck)
			{
				nsec = atoi(m_spinTIME->GetText());
				if (nsec < 3)
				{
					m_spinTIME->SetText("3");
					nsec = 3;
				}
			}
			m_spinTIME->EnableWindow(!bCheck);
			pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TURNCODE, nsec));
			break;
		}
	case IDC_BT_CONFIG:
		{
			//선택한 탭, 넘기기 -> 서버작업 끝나고 반영
			GroupNumber = m_cbGROUP.GetCurSel();
// 			int	nOver = -1;
// 			if (m_btAddVisible.IsChecked())
// 				nOver = MO_VISIBLE;
// 			else if (m_btAddFull.IsChecked())
// 				nOver = MO_FULL;
// 			else
// 				nOver = MO_SELECT;

			//분할표시된 그룹의 처음에 선택되도록 수정
			if(nOver == MO_VISIBLE)
			{
				CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
				pWnd->SendMessage(WM_MANAGE, MK_SELECTFIRST);
			}

			sztmp.Format("%s /p5 /S /d %s%c%d%c%s%c", MAP_CONFIG, "selectGROUP", P_TAB ,GroupNumber , P_NEW , "NO", P_TAB);
			m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_OPENSCR, typePOPUP), (LPARAM)sztmp.operator LPCTSTR());
		}
		break;

	case IDC_BT_EXPECT:
		bCheck = m_btTEXT[1].IsChecked();
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_EXPECT, (bCheck ? MO_ON : MO_OFF)));
		break;

	case IDC_BT_AUTO:
		bCheck = m_chkAUTO->GetCheck();
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_AUTOEXPECT, (bCheck ? MO_ON : MO_OFF)));
		
		if(bCheck == TRUE)
		{
			 m_btTEXT[1].EnableWindow(FALSE);
			 m_btTEXT[1].SetCheck(FALSE);
		}
		else
		{
			m_btTEXT[1].EnableWindow(TRUE);
		}
		break;

	case IDC_CB_FIELD:
// 		if (HIWORD(wParam) == CBN_SELCHANGE)
// 		{
// 			int	nIndex = m_cbFIELD.GetCurSel();
// 
// 			if (nIndex >= 0)
// 			{
// 				int	ndata = m_cbFIELD.GetItemData(nIndex);
// 				char	readB[1024];
// 				CString key;
// 				key.Format("%02d", ndata);
// 				memset(readB, 0x00, sizeof(readB));
// 				GetPrivateProfileString(SEC_FIELDS, key, "", readB, sizeof(readB), m_fileFIELD);
// 				m_activefield.Format("%s", readB);		
// 				IH::Parser(m_activefield, ";");
// 				m_pMainWnd->SendMessage(WM_MANAGE, MK_CHANGEFIELD);
// 
// 				// ADD PSH 20070911
// 				m_cbFIELD.GetLBText(nIndex, key);
// 				m_btField.SetWindowText(key);
// 				m_btField.Invalidate();
// 				// END ADD
// 			}
// 		}
		break;
	case IDC_CB_GROUP:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			int	nIndex = m_cbGROUP.GetCurSel();
			if (nIndex >= 0 && nIndex < m_btntab.GetItemCount())
			{
				if(m_selectedTab != -1)
				{
					saveGroupwhenClose(m_selectedTab);
				}
				
				m_selectedTab = nIndex;

				m_btntab.SetCurSel(nIndex);
				SendTree(nIndex);
			}			
		}
		break;
	case IDC_BT_ADDSELECT:
		{
// 			int	nIndex = m_cbGROUP.GetCurSel();
// 			
// 			if (nIndex >= 0 && nIndex < m_btntab.GetItemCount())
// 			{
// 				if(m_selectedTab != -1)
// 				{
// 					saveGroupwhenClose(m_selectedTab);
// 				}
// 				
// 				m_selectedTab = nIndex;
// 			}
// 			
// 			
// 			m_btAddSelect.SetCheck(true);
// 			m_btAddVisible.SetCheck(false);
// 			m_btAddFull.SetCheck(false);
			
// 			m_btAddSelect.EnableWindow(FALSE);
// 			m_btAddVisible.EnableWindow(TRUE);
// 			m_btAddFull.EnableWindow(TRUE);
			
//			SetGroupDataView();
		}
		
		break;
	case IDC_BT_ADDVISIBLE:
		{
// 			int	nIndex = m_cbGROUP.GetCurSel();
// 			
// 			if (nIndex >= 0 && nIndex < m_btntab.GetItemCount())
// 			{
// 				if(m_selectedTab != -1)
// 				{
// 					saveGroupwhenClose(m_selectedTab);
// 				}
// 				
// 				m_selectedTab = nIndex;
// 			}
// 			
// 			
// 			m_btAddVisible.SetCheck(true);
// 			m_btAddSelect.SetCheck(false);
// 			m_btAddFull.SetCheck(false);
// 			
// // 			m_btAddVisible.EnableWindow(FALSE);
// // 			m_btAddSelect.EnableWindow(TRUE);
// // 			m_btAddFull.EnableWindow(TRUE);
// 			
// 			SetGroupDataView();		
		}
		break;
	case IDC_BT_ADDFULL:
// 			m_btAddVisible.SetCheck(false);
// 			m_btAddSelect.SetCheck(false);
// 			m_btAddFull.SetCheck(true);	
//			m_btAddVisible.SetCheck(true);
//			m_btAddSelect.SetCheck(true);
//			m_btAddFull.SetCheck(false);
// 			SetGroupDataView();
		break;
	// ADD PSH 20070911
	case IDC_BT_FIELD:
	case IDC_BT_FIELDDROP:
// 		{
// 			CRect rcBtn;
// 			m_btField.GetWindowRect(rcBtn);
// 			//rcBtn.bottom += SIZE_BUTTONS;
// 			rcBtn.right += 16;
// 			//ScreenToClient(rcBtn);
// 
// 			UINT nField = -1;
// 			nField = m_menuField.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, rcBtn.left, rcBtn.bottom, this);
// 
// 			if ((int)nField != m_cbFIELD.GetCurSel())	
// 			{
// 				int nBefore = m_cbFIELD.GetCurSel();
// 
// 				m_menuField.CheckMenuItem(IDC_MENU_FIELD + nBefore, MF_UNCHECKED);
// 				m_cbFIELD.SetCurSel(nField - IDC_MENU_FIELD);
// 				m_menuField.CheckMenuItem(nField, MF_UNCHECKED);
// 				OnCommand(MAKEWPARAM(IDC_CB_FIELD, CBN_SELCHANGE), 0);
// 			}
// 		}
		break;
	case IDC_MENU_TOOL:
// 		{
// 			CRect rcBtn;
// 			m_btTool.GetWindowRect(rcBtn);
// 			
// 			CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
// 			pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TOOLBUTTON, 0), (long)&rcBtn);
// 		}
		break;
	case IDC_BT_EQUALIZER:
// 		{
// 			bCheck = m_btEqualizer.IsChecked();
// 			
// 			m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_EQUALIZER, bCheck));
// 		}
		break;
	// END ADD
	}
	return CBaseWnd::OnCommand(wParam, lParam);
}

void CToolWnd::SetGroupDataView()
{

 	CWnd* pgWnd;
	pgWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
 	pgWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_INITGRID, 0), 0);

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	int		nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	//MO_VISIBLE모드일때는 항상 맨 앞을 선택하도록 한다
	if(nOver == MO_VISIBLE)
	{
		pgWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)0);
	}

}


void CToolWnd::OperDraw(CDC* pDC)
{
	CBaseWnd::OperDraw(pDC);

	int	ncnt = m_arDIVIDE.GetSize();

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		DrawSEP(pDC, m_arDIVIDE.GetAt(ii));	
	}

	CFont*	oldfont = pDC->SelectObject(m_pFont);
	pDC->DrawText("초", m_rcSECOND, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	// DEL PSH 20070912
	//pDC->DrawText("F", m_rcFIELD, DT_SINGLELINE|DT_VCENTER|DT_CENTER); 
	// END DEL
	pDC->SelectObject(oldfont);

	// ADD PSH 20070907
	CPen NewPen, *pOldPen = NULL;
	NewPen.CreatePen(PS_SOLID, 1, m_clrOutLine);

	pOldPen = pDC->SelectObject(&NewPen);

//	pDC->MoveTo(0, m_rect.bottom - 1);
//	pDC->LineTo(m_rect.Width(), m_rect.bottom - 1);
	pDC->FillSolidRect(m_rect, GetAxColor(66));
	DrawRoundBitmap(pDC, m_rect, ROUND_PANEL);

	pDC->SelectObject(pOldPen);
	// END ADD
}

CRect CToolWnd::AddDivide(CRect rect)
{
	rect.left = rect.right + SIZE_GAPS;
	rect.right = rect.left + 1;
	m_arDIVIDE.Add(rect);
	return rect;
}

CRect CToolWnd::MoveButton(CRect rcPrev, CSize size, CSize gap, CfxImgButton* pButton)
{
	CRect	rect;
	rect = GetNextRect(rcPrev, size, gap);
	pButton->MoveWindow(rect, FALSE);
	pButton->ShowWindow(SW_SHOW);
	pButton->Invalidate();
	m_arCONTROL.Add((CWnd*)pButton);
	return rect;
}

CRect CToolWnd::GetNextRect(CRect rcPrev, CSize size, CSize gap)
{
	CRect	rect;
	
	rect.left = (gap.cx) ? rcPrev.right + gap.cx : rcPrev.left;
	rect.right = rect.left + size.cx;
	rect.top = (gap.cy) ? rcPrev.bottom + gap.cy : rcPrev.top;
	rect.bottom = rect.top + size.cy;

	return rect;
}

void CToolWnd::DrawSEP(CDC* pDC, CRect rect)
{
	CPen	pen1(PS_SOLID, 1, RGB(128, 128, 128)), pen2(PS_SOLID, 1, RGB(255, 255, 255));
	CPen*	oldpen = pDC->SelectObject(&pen1);

	pDC->MoveTo(rect.TopLeft());
	pDC->LineTo(rect.left, rect.bottom);

	pDC->SelectObject(&pen2);
	pDC->MoveTo(rect.TopLeft() + CPoint(1, 0));
	pDC->LineTo(rect.left + 1, rect.bottom);

	pDC->SelectObject(oldpen);
	pen1.DeleteObject();
	pen2.DeleteObject();
}

void CToolWnd::HideControl(CRect rcBasket)
{
	CRect	rect;
	int	ncnt = m_arCONTROL.GetSize();
	CWnd*	pWnd = NULL;
	
	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		pWnd = m_arCONTROL.GetAt(ii);
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);
		if (rect.IntersectRect(rcBasket, rect))
			pWnd->ShowWindow(SW_HIDE);
		else
			pWnd->ShowWindow(SW_SHOW);
	}
}

HBRUSH CToolWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBaseWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_BT_AUTO:
	case IDC_CHECK_PLUSMINUS:
		pDC->SetBkMode(TRANSPARENT);
		hbr = *m_pBrush;
	}
	return hbr;
}

void CToolWnd::OperPallette()
{
//	m_pBrush = GetAxBrush(m_clrPallete);

	COLORREF	color = GetAxColor(14);
	COLORREF	crback = GetAxColor(66);
	m_pBrush = GetAxBrush(crback);

//	m_btntab.SetColor(RGB(255, 255, 255), color, RGB(255, 255, 32), color, m_clrPallete);
//	m_btntab.SetColor(RGB(255, 255, 255), color, RGB(255, 255, 32), color, crback);
	m_btntab.SetColor(GetAxColor(48), GetAxColor(48), GetAxColor(130), color, crback);

	// ADD PSH 20070907
	m_clrOutLine = GetAxColor(65);
	// END ADD
}

LONG CToolWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	bool bchk = FALSE;

	switch (LOWORD(wParam))
	{
	case MK_GETGROUP:
		{
			CMapStringToString*	pMap = (CMapStringToString*)lParam;
			int	ncnt = m_cbGROUP.GetCount();
			CString	sztmp = _T(""), szval = _T("");
			int	nval = 0;
			
			for ( int ii = 0 ; ii < ncnt; ii++ )
			{
				m_cbGROUP.GetLBText(ii, sztmp);
				nval = m_cbGROUP.GetItemData(ii);
				szval.Format("%02d", nval);
				pMap->SetAt(szval, sztmp);
			}
			return pMap->GetCount();
		}
		break;
	case MK_EXPECT:
		if (m_btTEXT[1].IsChecked())
			ret = 1;
		break;
	case MK_AUTOEXPECT:
		bchk = m_chkAUTO->GetCheck();
		if (bchk)
			ret = 1;
		break;
	case MK_TOOLSIZE:
		ret = (LONG)m_bBig;
		break;
	case MK_CHART:
		
		
		//ret = (LONG)m_btCHART.IsChecked();
		break;
	case MK_TURNCODE:
		m_btTURN.SetCheck(false);
		m_spinTIME->EnableWindow(TRUE);
		break;
	case MK_SELGROUP:
		if (HIWORD(wParam) == MO_SET)
			SelectGroup((int)lParam);
		break;
	case MK_GETAFIELD:	// get active field
// 		{
// 			int	nPos = m_cbFIELD.GetCurSel();
// 			ret = (LONG)m_cbFIELD.GetItemData(nPos);
// 		}
		break;
	case MK_HAWKEYE:
// 		ret = (LONG)m_btHAWKEYE.IsChecked();
		break;
	case MK_SETUPOK:
		ret = Reload();
		break;
	case MK_SENDTREE:
		{
			int index = m_btntab.GetCurSel();
			SendTree(index);
		}
		break;
	case MK_ISEXPAND:
// 		ret = (LONG)m_chkEXPAND->GetCheck();
		break;
	case MK_INPUTOVER:
// 		if (m_btAddVisible.IsChecked())
// 			ret = MO_VISIBLE;
// 		else if (m_btAddFull.IsChecked())
// 			ret = MO_FULL;
// 		else
// 			ret = MO_SELECT;
		break;
	// ADD PSH 20070918
	case MK_GETMOVECFG:
		ret = m_bMoveCfg;
		break;
	case MK_SETMOVECFG:
		m_bMoveCfg = lParam;
		break;
	case MK_GETMOVESAVE:
		ret = m_nMoveSave;
		break;
	case MK_SETMOVESAVE:
		m_nMoveSave = lParam;
		break;
	case MK_SELECTGROUP:						//종합화면에서 2022열면서 그룹정보 보낼 시
			{
			int index = HIWORD(wParam);
			dominoSelectGroup(index-1);
		}
		break;
	case MK_GETCBINDEX:
		ret = m_cbGROUP.GetCurSel();
	// END ADD
	case MK_REFRESH:
		clickBtn();
		break;
	case MK_EQUALIZER:
// 		if(lParam)
// 			m_btEqualizer.SetCheck(true);
// 		else
// 			m_btEqualizer.SetCheck(false);

		break;
	default:
		ret = CBaseWnd::OnManage(wParam, lParam);
		break;
	}

	return ret;
}

void CToolWnd::savecfg()
{
 	CString	sztmp;
// 	sztmp.Format("%d", m_chkEXPAND->GetCheck());
// 	WritePrivateProfileString(SEC_TOOL, KEY_EXPAND, sztmp, m_fileCFG);

	sztmp.Format("%d", m_chkAUTO->GetCheck());
	WritePrivateProfileString(SEC_TOOL, KEY_AUTO, sztmp, m_fileCFG);

	// ADD PSH 20070918
// 	sztmp.Format("%d", m_bMoveCfg);
// 	WritePrivateProfileString(SEC_TOOL, KEY_MOVECFG, sztmp, m_fileCFG);
// 
// 	sztmp.Format("%d", m_nMoveSave);
// 	WritePrivateProfileString(SEC_TOOL, KEY_MOVESAVE, sztmp, m_fileCFG);
	// END ADD

// 	int	nmode = MO_SELECT;
// 	if (m_btAddVisible.IsChecked())
// 		nmode = MO_VISIBLE;
// 	else if (m_btAddFull.IsChecked())
// 		nmode = MO_FULL;
// 	else
// 		nmode = MO_SELECT;
// 	sztmp.Format("%d", nmode);
// 	WritePrivateProfileString(SEC_TOOL, KEY_FILLMODE, sztmp, m_fileCFG);
	
//	nmode = m_btEqualizer.IsChecked();
// 	sztmp.Format("%d", nmode);
// 	WritePrivateProfileString(SEC_TOOL, KEY_EQUALIZER, sztmp, m_fileCFG);

// 	sztmp.Format("%d", (int)m_bBig);
// 	WritePrivateProfileString(SEC_TOOL, KEY_BIG, sztmp, m_fileCFG);

	sztmp = m_spinTIME->GetText();
	WritePrivateProfileString(SEC_TOOL, KEY_TIME, sztmp, m_fileCFG);

// 	CString tempFile; tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);
// 	int	nsel = m_cbFIELD.GetCurSel();
// 	int	ndata = m_cbFIELD.GetItemData(nsel);
// 	sztmp.Format("%02d", ndata);
// 	WritePrivateProfileString(SEC_FIELDS, KEY_ACTIVE, sztmp.operator LPCTSTR(), tempFile);

	sztmp = "";
}

void CToolWnd::loadcfg()
{
	// MODIFY PSH 20070918
	//BOOL	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_EXPAND, 0, m_fileCFG);
	BOOL	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_EXPAND, 1, m_fileCFG);
	// END MODIFY
	//m_chkEXPAND->SetCheck(bCheck);
	
	bCheck = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_AUTO, 1, m_fileCFG);
	m_chkAUTO->SetCheck(bCheck);
	
	if(bCheck)
	{
		m_btTEXT[1].EnableWindow(FALSE);
	}
	else
	{
		m_btTEXT[1].EnableWindow(TRUE);
	}

	// ADD PSH 20070918
	m_bMoveCfg  = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_MOVECFG, 0, m_fileCFG);
	m_nMoveSave = GetPrivateProfileInt(SEC_TOOL, KEY_MOVESAVE, 0, m_fileCFG);
	// END ADD
	
	int	nmode = (int)GetPrivateProfileInt(SEC_TOOL, KEY_FILLMODE, MO_SELECT, m_fileCFG);
	
//	SelectMode(nmode);
	
	m_bBig = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_BIG, 1, m_fileCFG);
	bool m_bIsEqual = (BOOL)GetPrivateProfileInt(SEC_TOOL, KEY_EQUALIZER, 0, m_fileCFG);

// 	if(m_bIsEqual)
// 		m_btEqualizer.SetCheck(true);
// 	else
// 		m_btEqualizer.SetCheck(false);


// 	int	ncount = (int)GetPrivateProfileInt(SEC_GROUP, KEY_COUNT, 1, m_fileCFG);
// 	
// 	if (ncount <= 0)
// 		ncount = 1;
// 	
// 	if (ncount > MAX_GROUP)
// 		ncount = MAX_GROUP;
// 
// 	if (ncount == 1)
// 		m_btMINUS.EnableWindow(FALSE);
// 	else if (ncount == MAX_GROUP)
// 		m_btPLUS.EnableWindow(FALSE);
	
	int	nval = GetPrivateProfileInt(SEC_TOOL, KEY_TIME, 3, m_fileCFG);
	CString	sztmp;
	sztmp.Format("%d", nval);
	m_spinTIME->SetText(sztmp);

 	m_fileFIELD.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);	
// 	m_activefield = loadFieldnm(loadActiveField());
// 
 	m_filePPOLIO.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_GROUP);	
	loadTabItem();	
}

// void CToolWnd::RemoveAllMenu()
// {
// 	int nCnt = m_menuField.GetMenuItemCount();
// 
// 	for(int nIdx = nCnt - 1; nIdx >= 0; nIdx--)
// 	{
// 		m_menuField.DeleteMenu(nIdx, MF_BYPOSITION);
// 	}
// }

// CString CToolWnd::loadFieldnm(int fno)
// {
// 	CString ret = _T("");
// 	m_cbFIELD.ResetContent();
// 	RemoveAllMenu();
// 
// 	CFileFind finder;
// 	if (!finder.FindFile(m_fileFIELD))
// 	{
// 		CString	string = DEF_FIELD_CONTENT1;
// 		WritePrivateProfileString(SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
// 	}
// 
// 	int	readL;
// 	char	readB[1024];
// 	CString	fieldnm, fieldno, string, stringx;
// 	int	index = 0, active = -1;
// 
// 	// ADD PSH 20070911
// 	int nCnt = 0;
// 	// END ADD
// 	
// 	for (int ii = 0; ii < MAX_FIELD; ii++)
// 	{
// 		fieldno.Format("%02d", ii);
// 		
// 		readL = GetPrivateProfileString(SEC_FIELDS, fieldno, "", readB, sizeof(readB), m_fileFIELD);
// 		if (readL > 0)
// 		{
// 			string = CString(readB, readL);
// 			stringx = IH::Parser(string, ";");		// fields name			
// 			if (!stringx.IsEmpty())
// 			{
// 				index = m_cbFIELD.AddString(stringx);
// 				m_cbFIELD.SetItemData(index, ii); 
// 				if (ii == fno) active = index;
// 
// 				// ADD PSH 20070911
// 				m_menuField.AppendMenu(MF_STRING, IDC_MENU_FIELD + nCnt++, stringx);
// 				// END ADD
// 			}
// 		}
// 	}
// 
// 	if (m_cbFIELD.GetCount() > 0)
// 	{
// 		if (active < 0) 
// 			active = 0;
// 
// 		m_cbFIELD.SetCurSel(active);
// 		int	ndata = m_cbFIELD.GetItemData(active);
// 		CString key;
// 		key.Format("%02d", ndata);
// 		readL = GetPrivateProfileString(SEC_FIELDS, key, "", readB, sizeof(readB), m_fileFIELD);
// 		ret.Format("%s", readB);		
// 		IH::Parser(ret, ";");		
// 
// 		// ADD PSH 20070911
// 		m_cbFIELD.GetLBText(active, key);
// 		m_btField.SetWindowText(key);
// 		m_menuField.CheckMenuItem(IDC_MENU_FIELD + active, MF_CHECKED);
// 		// END ADD
// 	}
// 	else
// 	{
// 		CString	string = DEF_FIELD_CONTENT1;
// 		WritePrivateProfileString(SEC_FIELDS, "00", string.operator LPCTSTR(), m_fileFIELD);
// 		loadFieldnm();
// 	}
// 	
// 	return ret;
// }

void CToolWnd::loadTabItem()
{
	int bIndex = m_btntab.GetCurSel();
	m_btntab.DeleteAllItems(); 
	m_cbGROUP.ResetContent();
	m_selectedTab = bIndex;

	int readL = 0, idx = 0, nTemp = 0;
	char readB[1024];
	CString	string, gno, gname;

	readL = GetPrivateProfileString(SEC_GROUPORDER, "00", "", readB, sizeof(readB), m_filePPOLIO);
	string = CString(readB, readL);
	
	for (int ii = 0; !string.IsEmpty(); ii++)
	{
		gno = IH::Parser(string, ";");
		readL = GetPrivateProfileString(SEC_GROUPNAME, gno, "", readB, sizeof(readB), m_filePPOLIO);
		gname = CString(readB, readL);

		if (!gname.IsEmpty())
		{
			CString tmpNm;
			tmpNm.Format("%d",ii);

			idx = atoi(gno);
			m_btntab.InsertItem(ii, " " + tmpNm);
			nTemp = m_cbGROUP.AddString(gname);
			m_cbGROUP.SetItemData(nTemp, idx);
		}
	}
	TRACE("Tab iNdex : %d\n", bIndex);
	if (bIndex >= 0 && bIndex < m_cbGROUP.GetCount())
	{
		m_btntab.SetCurSel(bIndex);
		m_cbGROUP.SetCurSel(bIndex);
		
 		SendTree(bIndex);
	}
}


void CToolWnd::dominoSelectGroup(int index)
{
	if (index >= 0 && index < m_cbGROUP.GetCount())
	{
		m_btntab.SetCurSel(index);
		m_cbGROUP.SetCurSel(index);
		
		SendTree(index);
	}
}


void CToolWnd::SetSel(int nIndex)
{
	int	ncnt = m_cbGROUP.GetCount();
	if (nIndex < 0 || nIndex >= ncnt)
		return;

	m_cbGROUP.SetCurSel(nIndex);
	m_btntab.SetCurSel(nIndex);
	m_selectedTab = nIndex;

	
	

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	pWnd->SendMessage(WM_MANAGE, MK_SELECTTREE, (LPARAM)nIndex);
}

//관심그룹 index 저장
void CToolWnd::saveGroupIndex(int index)
{
	CString	str;
	str.Format("%02d", index);
	WritePrivateProfileString(SEC_MAIN, KEY_SELECTGROUP, str.operator LPCTSTR(), m_fileCFG);
}

void CToolWnd::saveGroupwhenClose(int index)
{
	CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	pWnd->SendMessage(WM_MANAGE, MK_GROUPSAVE, (LPARAM)index);
}

//테스트용
void CToolWnd::testSaveFile()
{
	CString strPath(_T("")), m_root, m_user;
	strPath.Format("c:\\test.ini");
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	CString titletime;
	
	titletime.Format("[%s]ToolWnd::SelectTab", strCurTime);

	WritePrivateProfileString("IB202200", titletime, "select Botton", (LPCTSTR)strPath);
}

void CToolWnd::OnSelchangeBtntab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_btntab.SetFocus();
	clickBtn();
	*pResult = 0;

//	testSaveFile();
}

void CToolWnd::clickBtn()
{
	int	nIndex = m_btntab.GetCurSel();
	if (nIndex >= 0 && nIndex < m_cbGROUP.GetCount())
	{
		m_cbGROUP.SetCurSel(nIndex);
		
		if(m_selectedTab != -1)
		{
			saveGroupwhenClose(m_selectedTab);
		}
		m_selectedTab = nIndex;
		
		SendTree(nIndex);
	}
}


void CToolWnd::SendTree(int nIndex)
{
	//관심그룹 index 저장, Tree는 1베이스이므로 +1
	saveGroupIndex(nIndex+1);

	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	UINT	kind = MAKE_TREEID(xINTEREST, 0, 2, (int)m_cbGROUP.GetItemData(nIndex), TD_ITEM);
	pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), kind);
	pWnd->SendMessage(WM_MANAGE, MK_SELECTTREE, (LPARAM)nIndex);
	pWnd->SendMessage(WM_MANAGE, MK_CALLINTEREST, kind);
}


void CToolWnd::SelectGroup(UINT kind)
{
// 	int	ncount = (int)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ALL));
// 	
// 	if (ncount == 1)
// 	{
// 		m_btMINUS.EnableWindow(FALSE);
// 	}
// 	else if (ncount == MAX_GROUP)
// 	{
// 		m_btPLUS.EnableWindow(FALSE);
// 	}
// 	else
// 	{
// 		m_btMINUS.EnableWindow(TRUE);
// 		m_btPLUS.EnableWindow(TRUE);
// 	}

	struct _treeID*	treeID = CAST_TREEID(kind);
	if (treeID->kind != xINTEREST)
	{
		m_cbGROUP.SetCurSel(-1);
		m_btntab.SetCurSel(-1);
		return;
	}

	
	int	ncnt = m_cbGROUP.GetCount();

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		if (treeID->value == (int)m_cbGROUP.GetItemData(ii))
		{
			saveGroupIndex((int)m_cbGROUP.GetItemData(ii));
			SetSel(ii);	
			break;
		}
	}	

}

int CToolWnd::loadActiveField()
{
	CString tempFile; tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);

	CFileFind finder; if (!finder.FindFile(tempFile)) return -1;

	int	readL = 0;
	char	readB[256];

	readL = GetPrivateProfileString(SEC_FIELDS, KEY_ACTIVE, "00", readB, sizeof(readB), tempFile);
	return atoi(CString(readB, readL));
}

int CToolWnd::Reload()
{
	int ncntBeforeload = m_cbGROUP.GetCount();		//변경전 그룹 개수
	//int	ncursel = m_cbFIELD.GetCurSel();
	int	ndata = 0;
	int	ret = 0;
// 	if (ncursel < 0)
// 		ndata = -1;
// 	else
// 		ndata = m_cbFIELD.GetItemData(ncursel);

	m_fileFIELD.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);	
//	CString szfield = loadFieldnm(ndata);

// 	if (m_activefield.CompareNoCase(szfield))
// 	{
// 		ret = 1;
// 		m_activefield.Format("%s", szfield);
// 	}

	m_filePPOLIO.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_GROUP);	
	int ncursel = m_cbGROUP.GetCurSel();
	ndata = -1;

	if (ncursel < 0)
		ndata = -1;
	else
		ndata = m_cbGROUP.GetItemData(ncursel);
	
	loadTabItem();
	int	ncnt = m_cbGROUP.GetCount();

	//그룹 개수가 하나 늘어났을 경우, 마지막에 생성된 탭에 선택 되도록 변경 09.11.19
	if(ncnt == ncntBeforeload+1)
	{
		m_cbGROUP.SetCurSel(ncnt-1);
		m_btntab.SetCurSel(ncnt-1);
		m_selectedTab = ncnt-1;
//		SendTree(ncnt-1);
	}
	else
	{
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			ncursel = m_cbGROUP.GetItemData(ii);
			if (ncursel == ndata)
			{
				m_cbGROUP.SetCurSel(ii);
				m_btntab.SetCurSel(ii);
				m_selectedTab = ii;
//				SendTree(ii);
			}
		}
	}

	return ret;
}


void CToolWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CBaseWnd::OnMouseMove(nFlags, point);
}

LRESULT CToolWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CBaseWnd::WindowProc(message, wParam, lParam);
}


// void CToolWnd::SelectMode(int nmode)
// {
// 	m_btAddSelect.SetCheck(false);
// 	m_btAddVisible.SetCheck(false);
// 	m_btAddFull.SetCheck(false);
// 
// 	switch (nmode)
// 	{
// 	case MO_VISIBLE:
// 		m_btAddVisible.SetCheck(true);
// 		break;
// 	case MO_FULL:
// //		m_btAddFull.SetCheck(true);
// //		break;
// 	case MO_SELECT:
// 	default:
// 		m_btAddSelect.SetCheck(true);
// 		break;
// 	}
// }

