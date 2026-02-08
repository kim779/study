#include "stdafx.h"
#include "IB300200.h"
#include "ControlWnd.h"
#include "3DEdit.h"
#include "SpinCtrl.h"
#include "JangoWnd.h"
#include "MultiJumunDlg.h"
#include "ContainerWnd.h"
#include "MarketConfirm.h"
#include "../../control/fx_grid/memdc.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include <math.h>
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_CTRL_ACCOUNT	0x01
#define ID_CTRL_CHART	0x02
#define ID_CTRL_CAN	0x03
#define ID_EDIT_JMCN	0x05
#define ID_SPIN_JMCN	0x06
#define ID_CTRL_JMCN	0x07
#define ID_EDIT_MONEY	0x08
#define ID_COMBO_MULTI	0x09
#define ID_EDIT_TICK	0x0a
#define ID_SPIN_TICK	0x0b
#define ID_WND_CODEGRID	0x0c
#define ID_EDIT_JMDG	0x0d
#define ID_CTRL_JANGO	0x0e
#define ID_CTRL_REVAL	0x0f

#define ID_CTRL_JMCL	0x10
#define ID_CTRL_RVCL	0x20

CControlWnd::CControlWnd(CWnd* pParent, CWnd* pView)
{
	m_pParent = (CMapWnd*)pParent;
	m_pView = pView;

	m_jmsrRc.SetRectEmpty();

	m_text3 = "주문가";
	m_text4 = "주문수량";
	m_text5 = "예약틱";

	m_tgubn = "구      분";
	m_tjgsu = "잔고수량";
	m_tjmga = "주문가능";
	m_tpgga = "평균단가";
	m_tpgsk = "평가손익";

	m_ogubn = "";
	m_ojgsu = "";
	m_ojmga = "";
	m_opgga = "";
	m_opgsk = "";

	m_pJangoWnd = NULL;
	m_pEditJmdg = NULL;
	m_pEditJmcn = NULL;
	m_pEditTick = NULL;
	m_pSpinJmcn = NULL;
	m_pSpinTick = NULL;
	m_pcontwndAmount = NULL;
	m_pJangoWnd = NULL;
	m_pAmount = NULL;
	m_pBtnAccount =  NULL;
	m_pBtnChart =  NULL;
	m_pBtnRevaluate = NULL;
	m_pBtnJmcn = NULL;
	m_pBtnCan = NULL;

	m_rdSel = RD_NOT;
	m_bInit = false;

	m_bMarketConfirm = TRUE;
}	


CControlWnd::~CControlWnd()
{
	if (m_pBtnAccount)
		m_pBtnAccount = nullptr;
	if (m_pBtnChart)
		m_pBtnChart = nullptr;
	if (m_pBtnRevaluate)
		m_pBtnRevaluate = nullptr;
	if (m_pBtnJmcn)
		m_pBtnJmcn = nullptr;
	if (m_pEditJmdg)
		m_pEditJmdg = nullptr;
	if (m_pBtnCan)
		m_pBtnCan = nullptr;
	if (m_pEditJmcn)
		m_pEditJmcn = nullptr;
	if (m_pEditTick)
		m_pEditTick = nullptr;
	if (m_pSpinJmcn)
		m_pSpinJmcn = nullptr;
	if (m_pSpinTick)
		m_pSpinTick = nullptr;
	if (m_pJangoWnd)
		m_pJangoWnd = nullptr;
	if (m_pAmount)
	{
		m_pAmount->DestroyWindow();
		m_pAmount = nullptr;
	}
}

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(ID_CTRL_ACCOUNT, OnButtonAccount)
	ON_BN_CLICKED(ID_CTRL_CHART, OnButtonChart)
	ON_BN_CLICKED(ID_CTRL_REVAL, OnButtonReval)
	ON_BN_CLICKED(ID_CTRL_CAN, OnButtonCan)
	ON_BN_CLICKED(ID_CTRL_JMCN, OnButtonJmcn)
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 200, OnContainerAmount)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers
void CControlWnd::OnDestroy() 
{
	if (m_pTipCtrl)
	{
		m_pTipCtrl->Activate(TRUE);
		m_pTipCtrl = nullptr;
	}

	CWnd::OnDestroy();
}

void CControlWnd::Init()
{
	CRect rc, cRc;
	GetClientRect(rc);

	m_home = m_pParent->Variant(homeCC, "");
	m_user = m_pParent->Variant(nameCC, "");
	m_pFont = m_pParent->GetAxFont("굴림", 9, false, 0);
	m_pBFont = m_pParent->GetAxFont("굴림", 9, false, FW_BOLD);

// line 1 링크 버튼
	int cx = 4;
	const int cy = 1;
	const int btn3W 	= 48;

	cRc.SetRect(cx, cy, cx + 44, cy + CTRL_HEIGHT);
	m_pBtnAccount = std::make_unique<class CfxImgButton>();
	m_pBtnAccount->Create("\n선물옵션 주문체결 스크린", cRc, this, ID_CTRL_ACCOUNT, FALSE, TRUE);
	m_pBtnAccount->SetWindowText("체결");
	m_pBtnAccount->SetFont(m_pFont);

	cRc.SetRect(cRc.right + BUTTON_SPACE, cRc.top, cRc.right + BUTTON_SPACE + btn3W, cRc.bottom);
	m_pBtnChart = std::make_unique<class CfxImgButton>();
	m_pBtnChart->Create("\n종합차트", cRc, this, ID_CTRL_CHART, FALSE, TRUE);
	m_pBtnChart->SetWindowText("차트");
	m_pBtnChart->SetFont(m_pFont);

	cRc.SetRect(cRc.right + BUTTON_SPACE, cRc.top, cRc.right + BUTTON_SPACE + btn3W, cRc.bottom);
	m_pBtnRevaluate = std::make_unique<class CfxImgButton>();
	m_pBtnRevaluate->Create("\n선물옵션 잔고 조회", cRc, this, ID_CTRL_REVAL, FALSE, TRUE);
	m_pBtnRevaluate->SetWindowText("잔고");
	m_pBtnRevaluate->SetFont(m_pFont);

// line 2 주문수량 조회
	// 주문가
	const int ileft = INNER_FRAME_SPACE;
	m_tRc3.SetRect(ileft, cRc.bottom + GAP * 2, ileft + 44, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_oRc3.SetRect(m_tRc3.right, m_tRc3.top, m_tRc3.right + 40, m_tRc3.bottom);

	m_pEditJmdg = std::make_unique <CfxEdit>(m_pFont);
	m_pEditJmdg->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_RIGHT, m_oRc3, this, ID_EDIT_JMDG);
	m_pEditJmdg->SetPasswordChar(0x0);
	((CEdit*)m_pEditJmdg.get())->LimitText(8);

	cRc = m_oRc3;
	cRc.left = cRc.right + 2;
	cRc.right = cRc.left + 67;
	m_chkJmsr.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_chkJmsr.rcText.SetRect(m_chkJmsr.rcChk.right, cRc.top, cRc.right, cRc.bottom);
	m_chkJmsr.data = _T("조회자동");
	m_chkJmsr.chk = false;
	m_chkJmsr.bRadio = false;
	
	// 주문가능수량
	// row0
	cx = INNER_FRAME_SPACE+INNER_FRAME_SPACE;

	const int icellwidth = 36;
	cx = 8;
	m_jmsr[R_TITLE][C_TITLE].rect.SetRect(cx, m_tRc3.bottom + GAP, cx+icellwidth, m_tRc3.bottom + CTRL_HEIGHT+2);
	m_jmsr[R_TITLE][C_TITLE].text = "";
	m_jmsr[R_TITLE][C_TITLE].color = m_pParent->GetIndexColor(COLOR_GRIDHEAD);

	m_jmsr[R_TITLE][C_NEW].rect.SetRect(cx+icellwidth-1, m_tRc3.bottom + GAP, cx+88, m_tRc3.bottom + CTRL_HEIGHT+2);
	m_jmsr[R_TITLE][C_NEW].text = "신규";
	m_jmsr[R_TITLE][C_NEW].color = m_pParent->GetIndexColor(COLOR_GRIDHEAD);

	m_jmsr[R_TITLE][C_CLEAR].rect.SetRect(cx+85, m_tRc3.bottom + GAP, cx+144, m_tRc3.bottom + CTRL_HEIGHT+2);
	m_jmsr[R_TITLE][C_CLEAR].text = "청산";
	m_jmsr[R_TITLE][C_CLEAR].color = m_pParent->GetIndexColor(COLOR_GRIDHEAD);

	// row1
	m_jmsr[R_MD][C_TITLE].rect.SetRect(cx, m_tRc3.bottom + CTRL_HEIGHT + 1, cx+icellwidth, m_tRc3.bottom + CTRL_HEIGHT*2 + 1);
	m_jmsr[R_MD][C_TITLE].text = "매도";
	m_jmsr[R_MD][C_TITLE].color = m_pParent->GetIndexColor(COLOR_MDB);

	m_jmsr[R_MD][C_NEW].rect.SetRect(cx+icellwidth-1, m_tRc3.bottom + CTRL_HEIGHT + 1, cx+88, m_tRc3.bottom + CTRL_HEIGHT*2 + 1);
	m_jmsr[R_MD][C_NEW].text = "";
	m_jmsr[R_MD][C_NEW].color = m_pParent->GetIndexColor(COLOR_MDB);

	m_jmsr[R_MD][C_CLEAR].rect.SetRect(cx+85, m_tRc3.bottom + CTRL_HEIGHT + 1, cx+144, m_tRc3.bottom + CTRL_HEIGHT*2 + 1);
	m_jmsr[R_MD][C_CLEAR].text = "";
	m_jmsr[R_MD][C_CLEAR].color = m_pParent->GetIndexColor(COLOR_MDB);

	// row2
	m_jmsr[R_MS][C_TITLE].rect.SetRect(cx, m_tRc3.bottom + CTRL_HEIGHT*2, cx+icellwidth, m_tRc3.bottom + CTRL_HEIGHT*3);
	m_jmsr[R_MS][C_TITLE].text = "매수";
	m_jmsr[R_MS][C_TITLE].color = m_pParent->GetIndexColor(COLOR_MSB);

	m_jmsr[R_MS][C_NEW].rect.SetRect(cx+icellwidth-1, m_tRc3.bottom + CTRL_HEIGHT*2, cx+88, m_tRc3.bottom + CTRL_HEIGHT*3);
	m_jmsr[R_MS][C_NEW].text = "";
	m_jmsr[R_MS][C_NEW].color = m_pParent->GetIndexColor(COLOR_MSB);

	m_jmsr[R_MS][C_CLEAR].rect.SetRect(cx+85, m_tRc3.bottom + CTRL_HEIGHT*2, cx+144, m_tRc3.bottom + CTRL_HEIGHT*3);
	m_jmsr[R_MS][C_CLEAR].text = "";
	m_jmsr[R_MS][C_CLEAR].color = m_pParent->GetIndexColor(COLOR_MSB);
	
	m_jmsrRc.SetRect(m_jmsr[R_MD][C_NEW].rect.left, m_jmsr[R_MD][C_NEW].rect.top, 
		m_jmsr[R_MS][C_CLEAR].rect.right, m_jmsr[R_MS][C_CLEAR].rect.bottom);

	cRc = m_jmsr[R_TITLE][C_TITLE].rect;
	m_pBtnCan = std::make_unique<class CfxImgButton>();
	m_pBtnCan->Create("\n수량조회", cRc, this, ID_CTRL_CAN, FALSE, TRUE);
	m_pBtnCan->SetWindowText("조회");
	m_pBtnCan->SetFont(m_pFont);

// line 3 주문수량
	m_tRc4.SetRect(cx, cRc.bottom + GAP * 2 + CTRL_HEIGHT*2, cx + 60, cRc.bottom + GAP * 2 + CTRL_HEIGHT*3);
	
	cRc.SetRect(m_tRc4.right + GAP, m_tRc4.top, m_tRc4.right + GAP + 60, m_tRc4.bottom);
	m_pEditJmcn = std::make_unique<class C3DEdit>();
	m_pEditJmcn->Create(NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_RIGHT, cRc, this, ID_EDIT_JMCN, m_pFont);
	m_pEditJmcn->SetWindowText("0");

	m_pSpinJmcn = std::make_unique<class CSpinCtrl>();
	m_pSpinJmcn->Create(NULL, cRc, this, ID_SPIN_JMCN, m_pFont);
	m_pSpinJmcn->SetBuddy(m_pEditJmcn.get());
	m_pSpinJmcn->SetRange32(0, 99999999);

	cRc.SetRect(cRc.right + 2, cRc.top, cRc.right + 2 + 20, cRc.top + 20);
	m_pBtnJmcn = std::make_unique<class CfxImgButton>();
	m_pBtnJmcn->Create("\n수량선택", cRc, this, ID_CTRL_JMCN, FALSE, TRUE);
	m_pBtnJmcn->SetFont(m_pFont);

	m_pAmount = std::make_unique <CContainerWnd>();
	m_pAmount->SetParent(m_pView);
	m_pAmount->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 1, 1), this, -1);
	m_pAmount->CreateControl(m_home, "cx_Amount", "", CRect(0, 0, 1, 1), "/k2");

	// 매매구분
	cRc.SetRect(cx, cRc.bottom + GAP, cx + 70, cRc.bottom + GAP + CTRL_HEIGHT);
	cRc.right = cRc.left + 46;
	m_rdNOT.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_rdNOT.rcText.SetRect(m_rdNOT.rcChk.right, cRc.top, cRc.right, cRc.bottom);
	m_rdNOT.data = _T("없음");
	m_rdNOT.chk = true;
	m_rdNOT.bRadio = true;

	cRc.left = cRc.right + 2;
	cRc.right = cRc.left + 46;
	m_rdIOC.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_rdIOC.rcText.SetRect(m_rdIOC.rcChk.right, cRc.top, cRc.right, cRc.bottom);
	m_rdIOC.data = _T("IOC");
	m_rdIOC.chk = false;
	m_rdIOC.bRadio = true;	
	
 	cRc.left = cRc.right + 2;
	cRc.right = cRc.left + 46;
	m_rdFOK.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_rdFOK.rcText.SetRect(m_rdFOK.rcChk.right, cRc.top, cRc.right, cRc.bottom);
	m_rdFOK.data = _T("FOK");
	m_rdFOK.chk = false;
	m_rdFOK.bRadio = true;	

	

// line 4 다중매매 
	cRc.SetRect(cx, cRc.bottom + GAP, cx + 70, cRc.bottom + GAP + CTRL_HEIGHT);
	m_multi.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_multi.rcText.SetRect(m_multi.rcChk.right, cRc.top, cRc.right, cRc.bottom);
	m_multi.data = _T("다중매매");
	m_multi.chk = false;
	m_multi.bRadio = false;

	cRc.SetRect(cRc.right + 2, cRc.top, CTRL_WIDTH - GAP - GAP, cRc.bottom + 200);
	m_cbMulti.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST, cRc, this, ID_COMBO_MULTI);
	m_cbMulti.SetFont(m_pFont);
	m_cbMulti.EnableWindow(FALSE);	//2016.08.01 KSJ 초기에 비활성화

	m_cbMulti.AddString("2");
	m_cbMulti.AddString("3");
	m_cbMulti.AddString("4");
	m_cbMulti.AddString("5");
	m_cbMulti.AddString("6");
	m_cbMulti.AddString("7");
	m_cbMulti.AddString("8");
	m_cbMulti.AddString("9");
	m_cbMulti.AddString("10");
	m_cbMulti.SetCurSel(0);

	cRc.SetRect(ileft, m_tRc3.top - GAP*1, CTRL_WIDTH-GAP, m_multi.rcText.bottom + GAP*1);
	m_sRc1 = cRc;

// line 5 예약매매 주문틱 설정
	//m_tRc5.SetRect(cx, cRc.bottom + GAP * 2, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_tRc5.SetRect(cx, cRc.bottom + GAP * 2, cx + 43, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	
	//cRc.SetRect(m_tRc5.right + GAP, m_tRc5.top, CTRL_WIDTH - GAP- GAP, m_tRc5.bottom);
	cRc.SetRect(m_tRc5.right + GAP, m_tRc5.top, m_tRc5.right + 45, m_tRc5.bottom);
	m_pEditTick = std::make_unique<class C3DEdit>();
	m_pEditTick->Create(NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_TABSTOP | ES_RIGHT, cRc, this, ID_EDIT_TICK, m_pFont);
	m_pEditTick->SetWindowText("2");

	m_pSpinTick = std::make_unique<class CSpinCtrl>();
	m_pSpinTick->Create(NULL, cRc, this, ID_SPIN_TICK, m_pFont);
	m_pSpinTick->SetBuddy(m_pEditTick.get());
	m_pSpinTick->SetRange32(-999, 999);

	cRc.SetRect(cRc.right + GAP,m_tRc5.top,CTRL_WIDTH - GAP- GAP,m_tRc5.bottom);

	m_market.rcChk.SetRect(cRc.left, cRc.top, cRc.left + 13, cRc.bottom);
	m_market.rcText.SetRect(m_market.rcChk.right, cRc.top, cRc.right, cRc.bottom);
	m_market.data = _T("시장가");
	m_market.chk = false;
	m_market.bRadio = false;

	cRc.SetRect(ileft, m_tRc5.top - GAP * 1, CTRL_WIDTH-GAP, cRc.bottom + GAP * 1);
	m_sRc2 = cRc;

	// 원클릭 & 클릭정정 
	cRc.SetRect(cx, cRc.bottom + GAP * 2 -3 , cx + 82, cRc.bottom + GAP * 2 + CTRL_HEIGHT );
	m_oneclick.rcChk.SetRect(cRc.left, cRc.top-4, cRc.left + 13, cRc.bottom-4);
	m_oneclick.rcText.SetRect(m_oneclick.rcChk.right, cRc.top, cRc.right, cRc.bottom-6);
	m_oneclick.data = _T("원클릭주문");
	m_oneclick.chk = false;
	m_oneclick.bRadio = false;

	// 현재가고정
	m_chkHold.rcChk.SetRect(cRc.right, cRc.top-4, cRc.right + 13, cRc.bottom-4);
	m_chkHold.rcText.SetRect(m_chkHold.rcChk.right, cRc.top, m_chkHold.rcChk.right+77, cRc.bottom-6);
	m_chkHold.data = _T("호가고정");
	m_chkHold.chk = false;
	m_chkHold.bRadio = false;

	cRc.SetRect(cx, cRc.bottom + GAP*0, cx + 70, cRc.bottom + GAP*0 + CTRL_HEIGHT);
	//cRc.SetRect(cx, cRc.bottom + GAP * 2, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_clickmod.rcChk.SetRect(cRc.left, cRc.top-7, cRc.left + 13, cRc.bottom-13);
	m_clickmod.rcText.SetRect(m_clickmod.rcChk.right, cRc.top-8, CTRL_WIDTH, cRc.bottom-14);
	m_clickmod.data = _T("원클릭주문+클릭정정");
	m_clickmod.chk = false;
	m_clickmod.bRadio = false;
	
	cRc.SetRect(cx, cRc.bottom + GAP*0, cx + 70, cRc.bottom + GAP*0 + CTRL_HEIGHT -5);
	//cRc.SetRect(cx, cRc.bottom + GAP * 2, cx + 70, cRc.bottom + GAP * 2 + CTRL_HEIGHT);
	m_trackmod.rcChk.SetRect(cRc.left, cRc.top-14, cRc.left + 13, cRc.bottom-14);
	m_trackmod.rcText.SetRect(m_trackmod.rcChk.right, cRc.top-13, CTRL_WIDTH, cRc.bottom-15);
	m_trackmod.data = _T("마우스 트랙");
	m_trackmod.chk = false;
	m_trackmod.bRadio = false;

	cRc.SetRect(ileft, m_oneclick.rcText.top - GAP * 1, CTRL_WIDTH-GAP, m_trackmod.rcText.bottom + GAP * 0 +5);
	m_sRc3 = cRc;

// line 6 선옵 잔고 
	//cRc.SetRect(GAP, cRc.bottom + GAP + 2, CTRL_WIDTH - GAP, rc.bottom);
	cRc.SetRect(ileft, cRc.bottom + GAP + 90, CTRL_WIDTH - ileft, rc.bottom);
	const CRect jgRC = cRc;
	m_pJangoWnd = std::make_unique <CJangoWnd>(this, m_pParent, m_pView);
	m_pJangoWnd->Create(NULL, "", WS_CHILD | WS_VISIBLE, cRc, this, ID_CTRL_JANGO);
	m_pJangoWnd->Init(m_home, m_user);


	// 잔고추가
	cRc.SetRect(ileft, m_trackmod.rcText.bottom + GAP*2, CTRL_WIDTH-GAP, jgRC.top-2);
	m_sRc4 = cRc;

	m_rgubn.SetRect(ileft, m_sRc4.top + GAP * 1, ileft + 60, m_sRc4.top + GAP * 1 + CTRL_HEIGHT - 2);
	m_rjgsu = m_rgubn;	m_rjgsu.OffsetRect(0, CTRL_HEIGHT - 3);
	m_rjmga = m_rjgsu;	m_rjmga.OffsetRect(0, CTRL_HEIGHT - 3);
	m_rpgga = m_rjmga;	m_rpgga.OffsetRect(0, CTRL_HEIGHT - 3);
	m_rpgsk = m_rpgga;	m_rpgsk.OffsetRect(0, CTRL_HEIGHT - 3);
	m_sRc4.bottom = m_rpgsk.bottom+GAP*1;

	const int iright = CTRL_WIDTH - ileft - INNER_FRAME_SPACE;
	m_rogubn.SetRect(m_rgubn.right + GAP, m_rgubn.top, iright, m_rgubn.bottom);
	m_rojgsu.SetRect(m_rjgsu.right + GAP, m_rjgsu.top, iright, m_rjgsu.bottom);
	m_rojmga.SetRect(m_rjmga.right + GAP, m_rjmga.top, iright, m_rjmga.bottom);
	m_ropgga.SetRect(m_rpgga.right + GAP, m_rpgga.top, iright, m_rpgga.bottom);
	m_ropgsk.SetRect(m_rpgsk.right + GAP, m_rpgsk.top, iright, m_rpgsk.bottom);

	m_pTipCtrl = std::make_unique <CToolTipCtrl>();
	m_pTipCtrl->Create(this);

// etc
	setButtonBitmaps();
	m_bInit = true;
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);

	const int bkMode = dc.SetBkMode(TRANSPARENT);
	dc.FillSolidRect(rc, m_pParent->m_ContentsBgColor);			
//	dc.FillSolidRect(rc, 0xff0000);
	CPen *pPen = (CPen *)m_pParent->GetAxPen(m_pParent->GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	CPen *pOldPen = (CPen *)dc.SelectObject(pPen);
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(m_sRc1);

	CRect rect;
	// 주문가능수량--그리드
	for (int row = R_TITLE; row <= R_MS; row++)
	{
		for (int col = C_TITLE; col <= C_CLEAR; col++)
		{
			dc.FillSolidRect(m_jmsr[row][col].rect, m_jmsr[row][col].color);
			dc.Rectangle(m_jmsr[row][col].rect);
			if (col == C_TITLE || row == R_TITLE)
			{
				DrawSiseText(&dc, m_jmsr[row][col].text, m_jmsr[row][col].rect, DT_CENTER, 0, m_pParent->GetIndexColor(COLOR_GRIDHEADTXT) );
			}
			else
			{
				rect = m_jmsr[row][col].rect;
				rect.DeflateRect(0, 3, 0, 0);
				DrawSiseText(&dc, m_jmsr[row][col].text, rect, DT_RIGHT, ST_COMMA | ST_NORMAL);
			}
		}
	}

	DrawSiseText(&dc, m_text3, m_tRc3, DT_LEFT);

	// 주문가능 수량 자동 적용
	dc.DrawFrameControl(m_chkJmsr.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_chkJmsr.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&dc, m_chkJmsr.data, m_chkJmsr.rcText, DT_LEFT);

	DrawSiseText(&dc, m_text4, m_tRc4, DT_RIGHT);
	dc.DrawFrameControl(m_multi.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_multi.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&dc, m_multi.data, m_multi.rcText, DT_RIGHT);

	dc.Rectangle(m_sRc2);
	DrawSiseText(&dc, m_text5, m_tRc5, DT_LEFT);

	dc.DrawFrameControl(m_market.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_market.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&dc, m_market.data, m_market.rcText, DT_LEFT);

	dc.DrawFrameControl(m_oneclick.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_oneclick.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&dc, m_oneclick.data, m_oneclick.rcText, DT_LEFT);
	dc.DrawFrameControl(m_clickmod.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_clickmod.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&dc, m_clickmod.data, m_clickmod.rcText, DT_LEFT);
	dc.DrawFrameControl(m_chkHold.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_chkHold.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&dc, m_chkHold.data, m_chkHold.rcText, DT_LEFT);
	dc.DrawFrameControl(m_trackmod.rcChk, DFC_BUTTON, DFCS_BUTTONCHECK | (m_trackmod.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&dc, m_trackmod.data, m_trackmod.rcText, DT_LEFT);

	dc.Rectangle(m_sRc3);

	// 잔고
	DrawSiseText(&dc, m_tgubn, m_rgubn, DT_RIGHT);
	DrawSiseText(&dc, m_tjgsu, m_rjgsu, DT_RIGHT);
	DrawSiseText(&dc, m_tjmga, m_rjmga, DT_RIGHT);
	DrawSiseText(&dc, m_tpgga, m_rpgga, DT_RIGHT);
	DrawSiseText(&dc, m_tpgsk, m_rpgsk, DT_RIGHT);

	dc.FillSolidRect(m_rogubn, m_pParent->GetIndexColor(COLOR_DATA));		dc.Rectangle(m_rogubn);
	dc.FillSolidRect(m_rojgsu, m_pParent->GetIndexColor(COLOR_DATA));		dc.Rectangle(m_rojgsu);
	dc.FillSolidRect(m_rojmga, m_pParent->GetIndexColor(COLOR_DATA));		dc.Rectangle(m_rojmga);
	dc.FillSolidRect(m_ropgga, m_pParent->GetIndexColor(COLOR_DATA));		dc.Rectangle(m_ropgga);
	dc.FillSolidRect(m_ropgsk, m_pParent->GetIndexColor(COLOR_DATA));		dc.Rectangle(m_ropgsk);
	
	DrawSiseText(&dc, m_ogubn, m_rogubn, DT_RIGHT, ST_MMCOLOR);
	DrawSiseText(&dc, m_ojgsu, m_rojgsu, DT_RIGHT, ST_COMMA | ST_NORMAL);
	DrawSiseText(&dc, m_ojmga, m_rojmga, DT_RIGHT, ST_COMMA | ST_NORMAL);
	DrawSiseText(&dc, m_opgga, m_ropgga, DT_RIGHT, ST_TEXT);
	DrawSiseText(&dc, m_opgsk, m_ropgsk, DT_RIGHT, ST_COMMA | ST_NORMAL | ST_COLOR | ST_PLUS_REMOVE);

	dc.Rectangle(m_sRc4);

	// 매매구분
	dc.DrawFrameControl(m_rdNOT.rcChk, DFC_BUTTON, DFCS_BUTTONRADIO | (m_rdNOT.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&dc, m_rdNOT.data, m_rdNOT.rcText, DT_RIGHT);

	dc.DrawFrameControl(m_rdIOC.rcChk, DFC_BUTTON, DFCS_BUTTONRADIO | (m_rdIOC.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&dc, m_rdIOC.data, m_rdIOC.rcText, DT_RIGHT);

	dc.DrawFrameControl(m_rdFOK.rcChk, DFC_BUTTON, DFCS_BUTTONRADIO | (m_rdFOK.chk ? DFCS_CHECKED : 0));
	DrawSiseText(&dc, m_rdFOK.data, m_rdFOK.rcText, DT_RIGHT);

	dc.SetBkMode(bkMode);
	dc.SelectObject(pOldPen);
}

void CControlWnd::setButtonBitmaps()
{
	CString path = "";
	path.Format("%s\\%s\\", m_home, IMAGEDIR);

	m_hBitmap2    = m_pParent->GetAxBitmap(path + "3btn.bmp")->operator HBITMAP();
	m_hBitmap2_dn = m_pParent->GetAxBitmap(path + "3btn_dn.bmp")->operator HBITMAP();
	m_hBitmap2_hv = m_pParent->GetAxBitmap(path + "3btn_en.bmp")->operator HBITMAP();
	m_pBtnAccount->SetImgBitmap(m_hBitmap2, m_hBitmap2_dn, m_hBitmap2_hv);
	m_pBtnChart->SetImgBitmap(m_hBitmap2, m_hBitmap2_dn, m_hBitmap2_hv);
	m_pBtnRevaluate->SetImgBitmap(m_hBitmap2, m_hBitmap2_dn, m_hBitmap2_hv);

	m_hBitmap1    = m_pParent->GetAxBitmap(path + "2btn.bmp")->operator HBITMAP();
	m_hBitmap1_dn = m_pParent->GetAxBitmap(path + "2btn_dn.bmp")->operator HBITMAP();
	m_hBitmap1_hv = m_pParent->GetAxBitmap(path + "2btn_en.bmp")->operator HBITMAP();
	m_pBtnCan->SetImgBitmap(m_hBitmap1, m_hBitmap1_dn, m_hBitmap1_hv);


	m_hBitmapJMCN	 = m_pParent->GetAxBitmap(path + "수량.bmp")->operator HBITMAP();
	m_hBitmapJMCN_dn = m_pParent->GetAxBitmap(path + "수량_dn.bmp")->operator HBITMAP();
	m_hBitmapJMCN_hv = m_pParent->GetAxBitmap(path + "수량_en.bmp")->operator HBITMAP();
	m_pBtnJmcn->SetImgBitmap(m_hBitmapJMCN, m_hBitmapJMCN_dn, m_hBitmapJMCN_hv);
}	

void CControlWnd::DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat, UINT nType, COLORREF tcolor)
{
	rc.DeflateRect(4, 0);
	nFormat = DT_SINGLELINE | DT_VCENTER | nFormat;
	COLORREF color{}, clrText = m_pParent->GetIndexColor(COLOR_TEXT);
	if(tcolor) clrText = tcolor;

	int	pos = -1;
	CString value, giho = "", percent = "";

	if (data.IsEmpty())	return;

	CFont* pOldFont{};
	if (nType & ST_BOLD)
		pOldFont = pDC->SelectObject(m_pBFont);
	else
		pOldFont = pDC->SelectObject(m_pFont);

	if (nType & ST_TEXT)
	{
		pDC->DrawText(data, rc, nFormat);
		return;
	}

	if (nType & ST_SHGA)
	{
		if (data[0] == '1')
		{
			giho = "↑ ";
			clrText = m_pParent->GetIndexColor(94);
		}
		else if (data[0] == '2')
		{
			giho = "▲ ";
			clrText = m_pParent->GetIndexColor(94);
		}
		else if (data[0] == '4')
		{
			giho = "↓ ";
			clrText = m_pParent->GetIndexColor(95);
		}
		else if (data[0] == '5')
		{
			giho = "▼ ";	
			clrText = m_pParent->GetIndexColor(95);
		}

		data = data.Mid(1);
	}

	if (nType & ST_COLOR)
	{
		pos = data.Find('+');
		if (pos >= 0)
			clrText = m_pParent->GetIndexColor(94);

		pos = data.Find('-');
		if (pos >= 0)
			clrText = m_pParent->GetIndexColor(95);
	}

	if (nType & ST_MMCOLOR)
	{
		pos = data.Find('+');
		if (pos >= 0)
		{
			clrText = m_pParent->GetIndexColor(94);
			data = data.Mid(pos+1);
		}

		pos = data.Find('-');
		if (pos >= 0)
		{
			clrText = m_pParent->GetIndexColor(95);
			data = data.Mid(pos+1);
		}

	}

	if (nType & ST_PERCENT)
		percent = " %";

	if (nType & ST_REMOVE)
	{
		data.Remove('+');
		data.Remove('-');
	}

	if (nType & ST_PLUS_REMOVE)
	{
		data.Remove('+');
	}

	if (nType & ST_NORMAL)
	{
		CString pre = _T("");
		pos = data.Find('+');
		if (pos >= 0)
		{
			pre = "+";
			data.Delete(0);
//			data.Remove('+');
		}

		pos = data.Find('-');
		if (pos >= 0)
		{
			pre = "-";
			data.Delete(0);
//			data.Remove('-');
		}

		data.Remove(' ');
		pos = data.Find('.');
		if (pos >= 0)
			value.Format("%.2f", atof(data));
		else
			value.Format("%d", atoi(data));

		if (atof(data) == 0.0)
			data = "";
		else
			data = pre + value;
	}

	if (nType & ST_COMMA)
	{
		data = m_pParent->SetComma(data);	
	}

	color = pDC->SetTextColor(clrText);
	pDC->DrawText(giho + data + percent, rc, nFormat);

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(color);
}

void CControlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_multi.rcChk.PtInRect(point) || m_multi.rcText.PtInRect(point))
	{
		m_multi.chk = !m_multi.chk;

		if (m_multi.chk)
		{
			CMultiJumunDlg dlg;

			if (dlg.DoModal() == IDCANCEL)
				m_multi.chk = false;
		}
		m_cbMulti.EnableWindow(m_multi.chk);

		InvalidateRect(&m_multi.rcChk);
	}
	else if (m_chkJmsr.rcChk.PtInRect(point) || m_chkJmsr.rcText.PtInRect(point))
	{
		m_chkJmsr.chk = !m_chkJmsr.chk;
		/*
		if (m_chkJmsr.chk)
		{
			CString msg  = "호가를 클릭할때마다 신규주문수량과 청산가능수량이 조회되기 때문에";
			msg += "\n\n네트워크 부하가 있습니다.";
			::MessageBox(m_hWnd, msg, "선물옵션 클릭주문", MB_ICONWARNING);
		}
		*/

		InvalidateRect(&m_chkJmsr.rcChk);
	}
	else if (m_market.rcChk.PtInRect(point) || m_market.rcText.PtInRect(point))
	{
		m_market.chk = !m_market.chk;

		if(m_market.chk == true)
		{
			m_pEditTick->EnableWindow(FALSE);
			m_pSpinTick->EnableWindow(FALSE);

			if(m_bMarketConfirm)
			{
				CMarketConfirm marketConfirm(this);
				
				marketConfirm.DoModal();
			}
		}
		else
		{
			m_pEditTick->EnableWindow(TRUE);
			m_pSpinTick->EnableWindow(TRUE);
		}
		
		InvalidateRect(&m_market.rcChk);
	}
	else if (m_oneclick.rcChk.PtInRect(point) || m_oneclick.rcText.PtInRect(point))
	{
		m_oneclick.chk = !m_oneclick.chk;

		InvalidateRect(&m_oneclick.rcChk);
	}
	else if (m_chkHold.rcChk.PtInRect(point) || m_chkHold.rcText.PtInRect(point))
	{
		m_chkHold.chk = !m_chkHold.chk;
		InvalidateRect(&m_chkHold.rcChk);
		((CMapWnd *)m_pParent)->SetHold(m_chkHold.chk);
	}
	else if (m_clickmod.rcChk.PtInRect(point) || m_clickmod.rcText.PtInRect(point))
	{
		CString msg  = "마우스 원클릭만으로 기존 주문들을 일괄정정 합니다.";
			msg += "\n정정가격은 마우스가 클릭된 가격으로 지정되며,";
			msg += "\n매도매수별로 미체결 주문 모두가 일괄 정정됩니다.";
			msg += "\n";
			msg += "\n미체결 주문이 없을 경우에는 신규주문이 입력됩니다.";
			msg += "\n기존 미체결 주문과 동일한 가격을 마우스로 클릭하면 신규주문이 실행되며,";
			msg += "\n이는 여러건의 미체결 주문이 있는 상황에서도 동일하게 적용됩니다.";

		m_clickmod.chk = !m_clickmod.chk;
		if (m_clickmod.chk)
			::MessageBox(m_hWnd, msg, "선물옵션 클릭주문", MB_ICONWARNING);

		InvalidateRect(&m_clickmod.rcChk);
	}
	else if (m_trackmod.rcChk.PtInRect(point) || m_trackmod.rcText.PtInRect(point))
	{
		CString msg  = "마우스 트랙 기능을 사용하시면, 저사양일 경우 다소 느려질수 있습니다.";
		if (!m_trackmod.chk)
			::MessageBox(m_hWnd, msg, "선물옵션 클릭주문", MB_ICONWARNING);
		m_trackmod.chk = !m_trackmod.chk;
		InvalidateRect(&m_trackmod.rcChk);
	}
	else if (m_rdNOT.rcChk.PtInRect(point) || m_rdNOT.rcText.PtInRect(point))
	{
		m_rdNOT.chk = true;
		m_rdIOC.chk = false;
		m_rdFOK.chk = false;
		m_rdSel = RD_NOT;
		InvalidateRect(&m_rdNOT.rcChk);
		InvalidateRect(&m_rdIOC.rcChk);
		InvalidateRect(&m_rdFOK.rcChk);
	}
	else if (m_rdIOC.rcChk.PtInRect(point) || m_rdIOC.rcText.PtInRect(point))
	{
		m_rdNOT.chk = false;
		m_rdIOC.chk = true;
		m_rdFOK.chk = false;
		m_rdSel = RD_IOC;
		InvalidateRect(&m_rdNOT.rcChk);
		InvalidateRect(&m_rdIOC.rcChk);
		InvalidateRect(&m_rdFOK.rcChk);
	}
	else if (m_rdFOK.rcChk.PtInRect(point) || m_rdFOK.rcText.PtInRect(point))
	{
		m_rdNOT.chk = false;
		m_rdIOC.chk = false;
		m_rdFOK.chk = true;
		m_rdSel = RD_FOK;
		InvalidateRect(&m_rdNOT.rcChk);
		InvalidateRect(&m_rdIOC.rcChk);
		InvalidateRect(&m_rdFOK.rcChk);
	}
	
/*
	if (m_oRc1.PtInRect(point))
	{
		if (atoi(m_output1) > 0)
		{
			CString jmcn;
			jmcn.Format("%d", atoi(m_output1));
			m_pEditJmcn->SetWindowText(jmcn);
		}
	}
	else if (m_oRc2.PtInRect(point))
	{
		if (atoi(m_output2) > 0)
		{
			CString jmcn;
			jmcn.Format("%d", atoi(m_output2));
			m_pEditJmcn->SetWindowText(jmcn);
		}
	}
	else 
*/
	if (m_rojmga.PtInRect(point))
	{
		if (atoi(m_ojmga) > 0)
		{
			m_pEditJmcn->SetWindowText(m_ojmga);
		}
	}
// @@ jjhur 죽어 버리잖아.... 
//   왜 LButtonDown일때 SetWindowText 를 하는 거지????????
/*
	else if (m_jmsr[R_MD][C_NEW].rect.PtInRect(point))		// 신규매도
	{
		if (atoi(m_jmsr[R_MD][C_NEW].text) > 0)
		{
			m_pEditJmcn->SetWindowText(m_jmsr[R_MD][C_NEW].text);
		}
	}
	else if (m_jmsr[R_MS][C_NEW].rect.PtInRect(point))		// 신규매수
	{
		if (atoi(m_jmsr[R_MS][C_NEW].text) > 0)
		{
			m_pEditJmcn->SetWindowText(m_jmsr[R_MS][C_NEW].text);
		}
	}
	else if (m_jmsr[R_MD][C_CLEAR].rect.PtInRect(point))	// 청산매도
	{
		if (atoi(m_jmsr[R_MD][C_CLEAR].text) > 0)
		{
			m_pEditJmcn->SetWindowText(m_jmsr[R_MD][C_CLEAR].text);
		}
	}
	else if (m_jmsr[R_MS][C_CLEAR].rect.PtInRect(point))	// 청산매수
	{
		if (atoi(m_jmsr[R_MS][C_CLEAR].text) > 0)
		{
			m_pEditJmcn->SetWindowText(m_jmsr[R_MS][C_CLEAR].text);
		}
	}
*/

	CWnd::OnLButtonUp(nFlags, point);
}

void CControlWnd::DispatchData(char* pData, int len)
{
//	if (len != sz_SMOD)
//		return;
	CString tmpS;

	struct _smod* pMod = (struct _smod *)pData;

//	if (pMod->jmgb[0] == '+')
//		m_market = KOSPI;
//	else
//		m_market = KOSDAQ;

	m_curr = CString(pMod->curr, sizeof(pMod->curr));

	if (atof(m_cmsg) > 0)
		m_output5.Format("%.f", atof(m_cmsg) / atoi(m_output4));
	else
		m_output5 = "";
	
	m_output6.Format("%.f", atof(m_curr) * atoi(m_output4) - atoi(m_cmsg));

//	m_output1 = m_output2 = _T("");
	m_jmsr[R_MD][C_NEW].text = m_jmsr[R_MD][C_CLEAR].text = m_jmsr[R_MS][C_NEW].text = m_jmsr[R_MS][C_CLEAR].text = "";
	
	m_pEditJmdg->SetWindowText("");

	
	Invalidate();

	SearchCan();
}

void CControlWnd::SetMicheg(CString code, int cnt)
{
	m_pJangoWnd->SetMicheg(code, cnt);
	m_pJangoWnd->invalidateRC();
}

void CControlWnd::DispatchMicheg(char *pData, int len, bool bErase)
{
	static std::map<CString, int> tMap;

	struct _cmod *pMod = (struct _cmod *)pData;
	int nrec = atoi(CString(pMod->nrec, sizeof(pMod->nrec)));
	
	if (bErase)
		tMap.clear();

	for(int ii=0; ii<nrec; ++ii)
	{
		CString code( pMod->rec[ii].cod2 , sizeof(pMod->rec[ii].cod2) );
		std::map<CString,int>::iterator pos = tMap.find(code);
		if (pos!=tMap.end()) pos->second += 1;
		else tMap[code] = 1;
	}

	std::map<CString,int>::iterator st = tMap.begin(), ed=tMap.end();
	for(;st!=ed;++st)
		m_pJangoWnd->SetMicheg(st->first, st->second);
	m_pJangoWnd->invalidateRC();

}

void CControlWnd::NoticeParse(CString dat, CMapStringToString& ary)
{
	int	pos{}, pos2{};
	CString	str, symbol, value;

	pos = dat.Find('\n');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	while (!str.IsEmpty())
	{
		pos = str.Find('\t');
		if (pos == -1)	break;

		symbol = str.Left(pos++);
		str    = str.Mid(pos);

		pos = str.Find('\t');
		if (pos == -1)
		{
			value = str;
			str.Empty();
		}
		else
		{
			pos2 = pos;
			value = str.Left(pos2++);
			pos = value.Find('\r');
			if (pos != -1)
			{
				value = value.Left(pos++);
				str   = str.Mid(pos);
			}
			else
				str = str.Mid(pos2);
		}

		value.TrimRight();
		ary.SetAt(symbol, value);
	}

	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;

		symbol = dat.Left(pos++);
		dat    = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat   = dat.Mid(pos);
		}

		value.TrimRight();
		ary.SetAt(symbol, value);

	}
}

int CControlWnd::GetJumunCount()
{
	CString data;
	m_pEditJmcn->GetWindowText(data);
	data.Remove(',');

	return atoi(data);
}	

int CControlWnd::GetTickCount()
{
	CString tick;
	m_pEditTick->GetWindowText(tick);
	return atoi(tick);
}

int CControlWnd::GetMultiCount()
{
	CString	count;
	m_cbMulti.GetWindowText(count);

	if (m_multi.chk)
		return atoi(count);
	else
		return 1;
}

int CControlWnd::GetPGDG()
{
	return atoi(m_output5);
}

void CControlWnd::SetPrice(int mmgb, int price)
{
	return;

}

int CControlWnd::calculateCommission(int money)
{
/*	int iTax = 0;
	if (money <= 10000000)			// 1천만원 이하 -> 0.1%
		iTax = (int)(money * 0.001);
	else if (money <= 100000000)		// 1억원 이하 -> 0.07%
		iTax = (int)(money * 0.0007);
	else					// 1억원 초과 -> 0.03%
		iTax = (int)(money * 0.0003);
*/
	const int iTax = (int)(money * 0.00024);
	return iTax;
}

void CControlWnd::OnButtonAccount()
{
	CString param;
//	param.Format("DH720300/S/s/p5");
	param.Format("IB310200/S/s/p5/dAN20\t%s", m_pParent->GetAccount());
	m_pParent->SetView(typeVIEW, param);
}

void CControlWnd::OnButtonChart()
{
	CString param;
//	param.Format("DH510300/S/s/p5/ded_focod\t%s", m_pParent->GetCode());
	param.Format("IB711400/t/S/s/p5/d30301\t%s", m_pParent->GetCode());	//2013.01.17 KSJ 구차트를 신차트로 변경

	m_pParent->SetView(typeVIEW, param);

}

void CControlWnd::OnButtonReval()
{
	CString param;
//	param.Format("DH721100/S/s/p5");
	param.Format("IB320100/S/s/p5/dAN20\t%s", m_pParent->GetAccount());
	m_pParent->SetView(typeVIEW, param);

}

void CControlWnd::SearchCan()
{
	if (!GetChkJmsr())	return;

	CString jmdg;
	jmdg.Format("%.2f", fabs(atof(m_curr)));		
	m_pEditJmdg->SetWindowText(jmdg);
	
	m_pParent->SendMaxJmcn(jmdg);
}

void CControlWnd::OnButtonCan()
{
	CString jmdg;
	m_pEditJmdg->GetWindowText(jmdg);

	if (atof(jmdg) == 0.0)
	{
		jmdg.Format("%.2f", fabs(atof(m_curr)));		
		m_pEditJmdg->SetWindowText(jmdg);
	}

	m_jmsr[R_MD][C_NEW].text = m_jmsr[R_MD][C_CLEAR].text = m_jmsr[R_MS][C_NEW].text = m_jmsr[R_MS][C_CLEAR].text = "";
	InvalidateRect(&m_jmsrRc);

	m_pParent->SendMaxJmcn(jmdg);

}
/*
LRESULT CControlWnd::OnButtonMSCan()
{
	CString jmdg;
	m_pEditJmdg->GetWindowText(jmdg);

	if (atof(jmdg) == 0.0)
	{
		jmdg.Format("%.2f", fabs(atof(m_curr)));		
		m_pEditJmdg->SetWindowText(jmdg);
	}

	//m_text1 = "청산매수";
	//m_text2 = "신규매수";
	//InvalidateRect(&m_tRc1);
	//InvalidateRect(&m_tRc2);

	m_jmsr[R_MD][C_NEW].text = m_jmsr[R_MD][C_CLEAR].text = m_jmsr[R_MS][C_NEW].text = m_jmsr[R_MS][C_CLEAR].text = "0";
	m_mmgb = 2;
	m_pParent->SendMaxJmcn(jmdg, m_mmgb);

	return 0;
}
*/

void CControlWnd::OnButtonJmcn()
{
	CRect rect;
	m_pBtnJmcn->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	m_pAmount->SetWindowPos(NULL, rect.right + 2, rect.top, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	m_pAmount->BtnClick("");

}


void CControlWnd::SetJmdg(CString jmdg)
{
	m_pEditJmdg->SetWindowText(jmdg);

	if (GetChkJmsr())
		OnButtonCan();
}

void CControlWnd::SetCanClearS(CString str)
{
	str.Delete(0, L_max_mid);

	struct max_mod mod;

	CopyMemory(&mod, str, L_max_mod);
	m_jmsr[R_MD][C_NEW].text = CString(mod.lNewOrdAbleQty, sizeof(mod.lNewOrdAbleQty));
	m_jmsr[R_MD][C_CLEAR].text = CString(mod.lLqdtOrdAbleQty, sizeof(mod.lLqdtOrdAbleQty));

	Invalidate();
}

void CControlWnd::SetCanClearB(CString str)
{
	str.Delete(0, L_max_mid);

	struct max_mod mod;

	CopyMemory(&mod, str, L_max_mod);
	m_jmsr[R_MS][C_NEW].text = CString(mod.lNewOrdAbleQty, sizeof(mod.lNewOrdAbleQty));
	m_jmsr[R_MS][C_CLEAR].text = CString(mod.lLqdtOrdAbleQty, sizeof(mod.lLqdtOrdAbleQty));

	Invalidate();
}

/*
void CControlWnd::SetMaxJmcn(CString oldj, CString newj, int mmgb)
{
	// 청산가능수량
	if (oldj != "-1" && !oldj.IsEmpty())
	{
		int mmgb = oldj.GetAt(0) == '+' ? 2 : 1;
		if (mmgb == 2)
		{
			m_jmsr[R_MS][C_CLEAR].text = oldj.Mid(1);
		}
		else
		{
			m_jmsr[R_MD][C_CLEAR].text = oldj.Mid(1);
		}
//		if (mmgb == m_mmgb)
//			m_output1 = oldj.Mid(1);
	}
	// 신규주문수량
	if (newj != "-1")
	{
		if (mmgb == MD_JUMUN)
			m_jmsr[R_MD][C_NEW].text = newj;
		else
			m_jmsr[R_MS][C_NEW].text = newj;
	}

	InvalidateRect(&m_jmsrRc);
//	InvalidateRect(&m_oRc1);
//	InvalidateRect(&m_oRc2);
}
*/
LRESULT CControlWnd::OnContainerAmount(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	strText.Format("%s", (char*)lParam);

	if (atoi(strText) == 0)
		return 0;

	m_pEditJmcn->SetWindowText(strText);

	return 0;		
}

void CControlWnd::SetSize(int depth)
{
	if (depth == 0)
		m_pJangoWnd->ShowWindow(SW_SHOW);
	else
		m_pJangoWnd->ShowWindow(SW_HIDE);
}
/*
void CControlWnd::SetJango(WPARAM wParam, LPARAM lParam)
{
	m_pJangoWnd->SetJango(wParam, lParam);
}
*/

void CControlWnd::SetJango(CString code, CString jgvl, CString pgdg, CString pgga, CString jmgr, CString pgsk, bool bAll)
{	
	m_pJangoWnd->SetJango(code, jgvl, pgdg);
	if (m_pParent->GetCode() == code)
	{
		m_ogubn = m_ojgsu = m_ojmga = m_opgga = m_opgsk = "";
		if (!jgvl.IsEmpty())
		{
			if (jgvl.GetAt(0) == '+')
				m_ogubn = "+매수";
			else
				m_ogubn = "-매도";
			m_ojgsu = jgvl.Mid(1);
		}
		m_ojmga = jmgr;
		m_opgga = pgga;
		const double pgskD = atof(pgsk);
		if (pgskD > 0.0)
			m_opgsk.Format("+%.f", pgskD);
		else if (pgskD < 0.0)
			m_opgsk.Format("%.f", pgskD);
		else
			m_opgsk = "0";

		m_jango = jgvl;
		m_maip = pgdg;
		InvalidateRect(m_sRc4);

		//if (bAll)	SetJmcn(code, jmgr);
	}
	
}


void CControlWnd::WriteFile(char* pBytes, int nBytes)
{
	CFile		Dfile;
	ULONGLONG   pos{};
	CString		filename, time;

	filename.Format("%s\\dev\\IB300200.log", m_home);
	const BOOL bExist = Dfile.Open(filename, CFile::modeReadWrite);//CFile::modeCreate|CFile::modeReadWrite);
	if (!bExist)
	{
		Dfile.Open(filename, CFile::modeCreate|CFile::modeReadWrite);
		Dfile.Write(pBytes, nBytes);
	}
	else
	{
		pos = Dfile.SeekToEnd();
		Dfile.Seek((long) pos, CFile::begin);
		Dfile.Write(pBytes, nBytes);
	}
	Dfile.Close();
}

void CControlWnd::ClearJango()
{
//	m_output1 = "";
//	m_output2 = "";
	m_jmsr[R_MD][C_NEW].text = m_jmsr[R_MD][C_CLEAR].text = m_jmsr[R_MS][C_NEW].text = m_jmsr[R_MS][C_CLEAR].text = "";
	m_output3 = "";
	m_maip	  = "";
	m_jango   = "";

	m_ogubn   = "";
	m_ojgsu   = "";
	m_ojmga   = "";
	m_opgga   = "";
	m_opgsk   = "";

	if (m_pJangoWnd)
		m_pJangoWnd->ClearJango();

	Invalidate();
}

void CControlWnd::ClearMaip()
{
	m_maip	  = "";
	m_jango   = "";
}

void CControlWnd::SetJmcn(CString code, CString jmcn)
{
	if (m_pParent->GetCode() != code)
		return;
	CString tmpS;
	tmpS.Format("%d", abs(atoi(jmcn)));
	m_pEditJmcn->SetWindowText(tmpS);
}

void CControlWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	ClientToScreen(&pt);
	if (m_pParent->GetCode().IsEmpty())
		m_mouseDLL.Format("IB300200\n%d\t%d\n", pt.x, pt.y);
	else
		m_mouseDLL.Format("IB300200\n%d\t%d\ned_focod\t%s\n", pt.x, pt.y, m_pParent->GetCode());
	m_pView->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (long)m_mouseDLL.operator LPCTSTR());
	
	CWnd::OnRButtonUp(nFlags, point);
}

CString CControlWnd::GetJmgb()
{
	CString tmpS;
	tmpS.Format("%d", m_rdSel);
	return tmpS;
}

CString CControlWnd::Parser(CString &srcstr, CString substr)
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

BOOL CControlWnd::GetWaitMode() 
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	if (pMapWnd)
		return pMapWnd->GetWaitMode();
	else
		return FALSE;
}

void CControlWnd::SetWaitMode(BOOL bWait) 
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	if (pMapWnd)
		pMapWnd->SetWaitMode(bWait);
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

}

void CControlWnd::ResizeControl(int cx, int cy)
{
	if(!m_bInit) return;
}

void CControlWnd::ChangeFont(double* lRate, CFont* pfont, CFont* boldFont)
{
	m_pFont = pfont;
	m_pBFont = boldFont;

	CRect rc, wrc;
	GetClientRect(&rc);
	int iw{}, ih{}, itop{}, ileft{};
	const int iheight = 0;
	const int ircHGap = 1;

	m_pBtnAccount->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() , ih = wrc.Height() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih);

	wrc.right = wrc.left + iw;
	wrc.bottom = wrc.top + ih;
	m_pBtnAccount->SetFont(m_pFont); 
	m_pBtnAccount->MoveWindow(wrc); 
	ileft = wrc.right +GAP;
	itop = wrc.top ; 
	

	m_pBtnChart->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	wrc.SetRect(ileft, itop, ileft + iw, itop + ih); 
	ileft = wrc.right +GAP;
	itop = wrc.top; 
	m_pBtnChart->SetFont(m_pFont); 
	m_pBtnChart->MoveWindow(wrc); 

	m_pBtnRevaluate->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	wrc.SetRect(ileft, itop, ileft + iw, itop + ih); 
	m_pBtnRevaluate->SetFont(m_pFont); 
	m_pBtnRevaluate->MoveWindow(wrc); 

	//LINE 2--주문가
	itop = wrc.bottom + GAP;
	iw = m_sRc1.Width(); 
	ih = m_sRc1.Height();
	ileft = m_sRc1.left; 
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = ih + (int)(lRate[1] * (float)ih );
	if(iw < rc.Width() - GAP) iw =  rc.Width() -GAP;
	m_sRc1.SetRect(ileft, itop, iw, itop + ih); 
	m_sRc1.right = rc.right;

	itop = m_sRc1.top + GAP;
	ileft = m_tRc3.left; 
	iw = m_tRc3.Width(); 
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = m_tRc3.Height();
	ih = ih + (int)(lRate[1] * (float)ih );
	m_tRc3.SetRect(ileft, itop, ileft + iw, itop + ih); //주문가

	ileft = m_tRc3.right + GAP;
	m_pEditJmdg->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = wrc.Height(); 
	ih = ih + (int)(lRate[1] * (float)ih );
	wrc.SetRect(ileft, itop, ileft + iw, itop + ih); 
	m_pEditJmdg->SetFont(m_pFont); 
	m_pEditJmdg->MoveWindow(wrc); 
	
	//조회자동
	ileft = wrc.right+1;
	iw = m_chkJmsr.rcChk.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );

	m_chkJmsr.rcChk = wrc;
	m_chkJmsr.rcChk.left = ileft;
	m_chkJmsr.rcChk.right = ileft + iw;

	ileft = m_chkJmsr.rcChk.right;
	iw = m_chkJmsr.rcText.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	m_chkJmsr.rcText = wrc;
	m_chkJmsr.rcText.left = ileft;
	m_chkJmsr.rcText.right = ileft + iw;

	//조회버튼
	itop = wrc.bottom + GAP;
	m_pBtnCan->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	ileft = wrc.left ;
	iw = wrc.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	ih = wrc.Height(); 
	ih = ih + (int)(lRate[1] * (float)ih );
	wrc.SetRect(ileft, itop, ileft + iw, itop + ih); 
	m_pBtnCan->SetFont(m_pFont); 
	m_pBtnCan->MoveWindow(wrc); 

	ih = m_jmsr[0][0].rect.Height();
	ih = ih + (int)(lRate[1] * (float)ih );
	const int ileft1 = ileft;
	for (int row = R_TITLE; row <= R_MS; row++)
	{
		ileft = ileft1;
		for (int col = C_TITLE; col <= C_CLEAR; col++)
		{
			iw = m_jmsr[row][col].rect.Width();
			iw = iw + (int)(lRate[0] * (float)iw );
			m_jmsr[row][col].rect.SetRect(ileft, itop, ileft+iw, itop+ih);
			ileft += (iw-1);
		}

		itop += (ih-1);
	}

	itop += GAP;
	iw = m_tRc4.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_tRc4.right = m_tRc4.left + iw;
	m_tRc4.top = itop;
	m_tRc4.bottom = itop + ih; //주문수량


	m_pSpinJmcn->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	const int ispinw = iw;
	const int irightgap = GAP*2;


	ileft = m_tRc4.right + GAP;
	m_pBtnJmcn->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width() ;
	const int ibtnw = iw + (int)(lRate[0] * (float)iw );

	const int icontrolh = ih;
	int iright = rc.right - irightgap - ispinw - ibtnw - GAP - 1;
	wrc.SetRect(ileft, itop, iright, itop + ih); 
	m_pEditJmcn->SetFont(m_pFont); 
	m_pEditJmcn->MoveWindow(wrc); 
	ileft = wrc.right;

	m_pSpinJmcn->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	wrc.SetRect(ileft, itop, ileft + ispinw, itop + ih); 
	m_pSpinJmcn->SetFont(m_pFont); 
	m_pSpinJmcn->MoveWindow(wrc); 


	ileft = wrc.right + 1;
//	m_pBtnJmcn->GetWindowRect( &wrc);
//	ScreenToClient(&wrc);
//	iw = wrc.Width() ;
//	iw = iw + (int)(lRate[0] * (float)iw );
	wrc.SetRect(ileft, itop, ileft + ibtnw, itop + ih); 
	m_pBtnJmcn->SetFont(m_pFont); 
	m_pBtnJmcn->MoveWindow(wrc); 

	itop += (ih+GAP);
	//없음, IOC, FOK
	iw = m_rdNOT.rcChk.Width() ;
	int ichkw = iw + (int)(lRate[0] * (float)iw );
	ileft = m_rdNOT.rcChk.left;
	m_rdNOT.rcChk.right = ileft + ichkw;
	m_rdNOT.rcChk.top = itop;
	m_rdNOT.rcChk.bottom = itop + ih;

	iw = m_rdNOT.rcText.Width() ;
	const int ichktxtw = iw + (int)(lRate[0] * (float)iw );
	m_rdNOT.rcText.left = m_rdNOT.rcChk.right;
	m_rdNOT.rcText.right = m_rdNOT.rcText.left + ichktxtw;
	m_rdNOT.rcText.top = itop;
	m_rdNOT.rcText.bottom = itop + ih;
	ileft = m_rdNOT.rcText.right + GAP;

	m_rdIOC.rcChk.left = ileft;
	m_rdIOC.rcChk.right = ileft + ichkw;
	m_rdIOC.rcChk.top = itop;
	m_rdIOC.rcChk.bottom = itop + ih;
	m_rdIOC.rcText.left = m_rdIOC.rcChk.right;
	m_rdIOC.rcText.right = m_rdIOC.rcText.left + ichktxtw;
	m_rdIOC.rcText.top = itop;
	m_rdIOC.rcText.bottom = itop + ih;
	ileft = m_rdIOC.rcText.right + GAP;

	m_rdFOK.rcChk.left = ileft;
	m_rdFOK.rcChk.right = ileft + ichkw;
	m_rdFOK.rcChk.top = itop;
	m_rdFOK.rcChk.bottom = itop + ih;

	m_rdFOK.rcText.left = m_rdFOK.rcChk.right;
	m_rdFOK.rcText.right = m_rdFOK.rcText.left + ichktxtw;
	m_rdFOK.rcText.top = itop;
	m_rdFOK.rcText.bottom = itop + ih;

	//다중매매
	itop += ih;
	ileft = m_multi.rcChk.left;
	iw = m_multi.rcChk.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	m_multi.rcChk.right = ileft + iw;
	m_multi.rcChk.top = itop;
	m_multi.rcChk.bottom = itop + ih;

	ileft = m_multi.rcChk.right;
	iw = m_multi.rcText.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );
	m_multi.rcText.left =ileft;
	m_multi.rcText.right = ileft + iw;
	m_multi.rcText.top = itop;
	m_multi.rcText.bottom = itop + ih;

	ileft = m_multi.rcText.right + GAP;
//	m_cbMulti.GetWindowRect( &wrc);
//	ScreenToClient(&wrc);
//	iw = wrc.Width() ;
//	iw = iw + (int)(lRate[0] * (float)iw );

	
	wrc.SetRect(ileft, itop, rc.right - irightgap, itop + ih); 
	m_cbMulti.SetFont(m_pFont); 
	m_cbMulti.MoveWindow(wrc); 

	m_sRc1.bottom = wrc.bottom + GAP;
	ih = m_sRc2.Height();
	ih = ih + (int)(lRate[1] * (float)ih );
	m_sRc2 = m_sRc1;
	m_sRc2.top = m_sRc1.bottom+GAP;
	m_sRc2.bottom = m_sRc2.top+ih;

	//LINe --예약주문틱
	iw = m_tRc5.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );

	//예약주문틱 text
	itop = m_sRc2.top + GAP ;
	m_tRc5.top = itop;
	m_tRc5.bottom = itop + icontrolh;
	m_tRc5.right = m_tRc5.left + iw;

	ileft = m_tRc5.right + GAP;
	
	m_pEditTick->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	iright = rc.right - irightgap - ispinw;
	wrc.SetRect(ileft, itop, iright, itop + icontrolh); 
	m_pEditTick->SetFont(m_pFont); 
	m_pEditTick->MoveWindow(wrc); 

	m_sRc2.bottom =wrc.bottom + ircHGap;

	ileft = wrc.right;
	wrc.SetRect(ileft, itop, ileft + ispinw, itop + icontrolh); 
	m_pSpinTick->SetFont(m_pFont); 
	m_pSpinTick->MoveWindow(wrc); 

	//원클릭주문
	ih  = m_sRc3.Height();
	ih = ih + (int)(lRate[1] * (float)ih );
	m_sRc3 = m_sRc2;
	m_sRc3.top = m_sRc2.bottom-1;
	m_sRc3.bottom = m_sRc3.top + ih+1;

	// 원클릭 & 클릭정정 
	itop = m_sRc3.top + ircHGap;
	ih  = m_oneclick.rcText.Height();
	ih = ih + (int)(lRate[1] * (float)ih );

	iw = m_oneclick.rcChk.Width();
	ichkw = iw + (int)(lRate[0] * (float)iw );
	m_oneclick.rcChk.right = m_oneclick.rcChk.left + ichkw;
	m_oneclick.rcChk.top = itop;
	m_oneclick.rcChk.bottom = itop + ih;

	ileft = m_oneclick.rcChk.right;
	iw = m_oneclick.rcText.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_oneclick.rcText.left = ileft;
	m_oneclick.rcText.right = ileft + iw;
	m_oneclick.rcText.top = itop;
	m_oneclick.rcText.bottom = itop + ih;

	// 현재가고정
	ileft = m_oneclick.rcText.right;
	m_chkHold.rcChk.left = ileft;
	m_chkHold.rcChk.right = m_chkHold.rcChk.left + ichkw;
	m_chkHold.rcChk.top = itop;
	m_chkHold.rcChk.bottom = itop + ih;
	ileft = m_chkHold.rcChk.right;
	iw = m_chkHold.rcText.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_chkHold.rcText.left = ileft;
	m_chkHold.rcText.right = ileft + iw;
	m_chkHold.rcText.top = itop;
	m_chkHold.rcText.bottom = itop + ih;

	//원클릭주문+클릭정정"
	itop  = (m_chkHold.rcText.bottom + GAP);
	ileft = m_clickmod.rcChk.left;
	m_clickmod.rcChk.left = ileft;
	m_clickmod.rcChk.right = m_clickmod.rcChk.left + ichkw;
	m_clickmod.rcChk.top = itop;
	m_clickmod.rcChk.bottom = itop + ih;
	ileft = m_clickmod.rcChk.right;
	iw = m_clickmod.rcText.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_clickmod.rcText.left = ileft;
	m_clickmod.rcText.right = ileft + iw;
	m_clickmod.rcText.top = itop;
	m_clickmod.rcText.bottom = itop + ih;

	//마우스 트랙
	itop  += (ih + GAP);
	ileft = m_trackmod.rcChk.left;
	m_trackmod.rcChk.left = ileft;
	m_trackmod.rcChk.right = m_trackmod.rcChk.left + ichkw;
	m_trackmod.rcChk.top = itop;
	m_trackmod.rcChk.bottom = itop + ih;
	ileft = m_trackmod.rcChk.right;
	iw = m_trackmod.rcText.Width();
	iw = iw + (int)(lRate[0] * (float)iw );
	m_trackmod.rcText.left = ileft;
	m_trackmod.rcText.right = ileft + iw;
	m_trackmod.rcText.top = itop;
	m_trackmod.rcText.bottom = itop + ih;
	m_sRc3.bottom = m_trackmod.rcText.bottom + ircHGap;

	//구분
	itop = m_sRc3.bottom;
	ih  = m_sRc4.Height();
	ih = ih + (int)(lRate[1] * (float)ih );
	m_sRc4 = m_sRc3;
	m_sRc4.top = itop-1;
	m_sRc4.bottom = itop + ih;

	ih = m_rgubn.Height();
	ih = ih + (int)(lRate[1] * (float)ih );
	iw = m_rgubn.Width();
	iw = iw + (int)(lRate[0] * (float)iw );


	ileft = m_rgubn.left;
	itop = m_sRc4.top + GAP;
	m_rgubn.top = itop;
	m_rgubn.bottom = itop + ih;
	m_rgubn.right = m_rgubn.left  +iw;
	m_rogubn = m_rgubn;
	m_rogubn.left = m_rgubn.right + GAP;
	m_rogubn.right = m_sRc4.right - GAP;

	const int iline = 1;
	itop += ih;
	m_rjgsu.top = itop-iline;
	m_rjgsu.bottom = itop + ih;
	m_rjgsu.right = m_rjgsu.left  +iw;
	m_rojgsu = m_rjgsu;
	m_rojgsu.left = m_rjgsu.right + GAP;
	m_rojgsu.right = m_sRc4.right - GAP;


	itop += ih;
	m_rjmga.top = itop-iline;
	m_rjmga.bottom = itop + ih;
	m_rjmga.right = m_rjmga.left  +iw;
	m_rojmga = m_rjmga;
	m_rojmga.left = m_rjmga.right + GAP;
	m_rojmga.right = m_sRc4.right - GAP;


	itop += ih;
	m_rpgga.top = itop-iline;
	m_rpgga.bottom = itop + ih;
	m_rpgga.right = m_rpgga.left  +iw;
	m_ropgga = m_rpgga;
	m_ropgga.left = m_rpgga.right + GAP;
	m_ropgga.right = m_sRc4.right - GAP;

	itop += ih;
	m_rpgsk.top = itop-iline;
	m_rpgsk.bottom = itop + ih;
	m_rpgsk.right = m_rpgsk.left  +iw;
	m_ropgsk = m_rpgsk;
	m_ropgsk.left = m_rpgsk.right + GAP;
	m_ropgsk.right = m_sRc4.right - GAP;

	m_sRc4.bottom = m_rpgsk.bottom + GAP;
	
	itop = m_sRc4.bottom + GAP;
	m_pJangoWnd->GetWindowRect( &wrc);
	ScreenToClient(&wrc);
	ih  = wrc.Height();
	ih = ih + (int)(lRate[1] * (float)ih );

	ileft = wrc.left;
	iw = wrc.Width() ;
	iw = iw + (int)(lRate[0] * (float)iw );

	int ibottom = m_pParent->m_iContentHeight - m_sRc4.bottom;
	iright = rc.Width() - GAP + 1; 

	ibottom = rc.bottom - itop;
	m_pJangoWnd->MoveWindow(ileft, itop, iright, ibottom); 
	m_pJangoWnd->ChangeFont(lRate, pfont, boldFont); 
}


void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_market.rcChk.PtInRect(point))
	{
		if(m_pTipCtrl)
		{
			m_pTipCtrl->AddTool(this, "사각형 표시는 시장가 주문을 뜻합니다.", m_market.rcChk, 100);
			m_pTipCtrl->Activate(TRUE);
		}
	}
	else if(m_market.rcText.PtInRect(point))
	{
		if(m_pTipCtrl)
		{
			m_pTipCtrl->AddTool(this, "사각형 표시는 시장가 주문을 뜻합니다.", m_market.rcText, 100);
			m_pTipCtrl->Activate(TRUE);
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CControlWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;
	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		if (m_pTipCtrl)	
			m_pTipCtrl->RelayEvent(&msg);
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}
