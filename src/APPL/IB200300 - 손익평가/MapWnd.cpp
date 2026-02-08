// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB200300.h"
#include "MapWnd.h"
#include "gendef.h"
#include <Winbase.h>
#include <Windows.h>
#include "JangoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GAP			1
#define RATE			"0.05%"
#define IDC_STATIC_CALC		10002
#define IDC_STATIC_CALC_END	10009
#define IDC_COMBO_PORT		10010
#define IDC_BTN_SAVE		10011
#define IDC_EDIT_PORT		10012
#define IDC_BTN_ADD		10014
#define IDC_BTN_DEL		10015
#define IDC_BTN_CHANGE		10016
#define IDC_BTN_JANGO		10017

#define MSG_STRING	"두번째 컬럼 클릭시 종목찾기 활성화됨"
#define TAX_RATE 0.003//세금

// listctrl
enum { COL_CODE = 0,	// code
	COL_CDLG = 1,	// code dialog
	COL_NAME = 2,	// 종목명
	COL_CURR = 3,	// 현재가
	COL_DIFF = 4,	// 대비
	COL_RATE = 5,	// 등락률
	COL_MYGA = 6,	// 매입가
	COL_MYAM = 7,	// 수량
	COL_PGGA = 8,	// 평가금액
	COL_BENEFIT = 9,// 수익률
	COL_SONIK = 10,	// 평가손익
	COL_FEE	= 11	// 수수료
};

// static
enum { STC_MST = 0,	// 총매수금
	STC_MSD = 1, 
	STC_PGT = 2,	// 총평가금
	STC_PGD = 3, 
	STC_SKT = 4,	// 총손익금
	STC_SKD = 5, 
	STC_RTT = 6,	// 총수익율
	STC_RTD = 7
};

// top data
enum { TOP_MSD = 0,	// 총매수금
	TOP_PGD = 1,	// 총평가금
	TOP_SKD = 2, 	// 총손익금
	TOP_RTD = 3,	// 총수익율
};

#pragma warning( disable: 4244 )

struct _portpolio_row
{
	char code[5][20];	// 0:code, 1:name, 2:매입가, 3:수량, 4:수수료
	_portpolio_row()
	{
		memset(this, 0x00, sizeof(_portpolio_row));
	}

};

struct _portpolio
{
	_portpolio_row rowd[MAX_ROW_CNT];
	_portpolio()
	{
		memset(this, 0x00, sizeof(_portpolio));
	}
};

struct _portpolio_list
{
	char ptname[MAX_PORT_LIST][40];
	_portpolio_list()
	{
		memset(this, 0x00, sizeof(_portpolio_list));
	}
};

/////////////////////////////////////////////////////////////////////////////
CMapWnd::CMapWnd()
{
	m_pList		= NULL;
	m_pCombo	= NULL;
	m_pBtnAdd	= NULL;
	m_pBtnDel	= NULL;
	m_pBtnChange	= NULL;
	m_pBtnSave	= NULL;
	m_pEdit		= NULL;
	
	m_nCurSel	= 0;
	m_nPortCurSel	= 0;
	m_nClkItem	= -1;
	m_nClkSubItem	= -1;
	
	for(int ii = 0 ; ii < CNT_STATIC; ii++)
		m_pStatic[ii] = NULL;
	for(int ii = 0 ; ii < CNT_TOPDATA; ii++)
		m_TopData[ii].Empty();
	
	m_pBtnJango	= NULL;
	m_pJangoDlg	= NULL;

	m_ilist.Create(18, 18, ILC_COLOR4, 0, 1);
}

CMapWnd::~CMapWnd()
{
	if (m_pList)
	{
		m_pList->DestroyWindow();
		m_pList = nullptr;
	}
	
	if (m_pCombo) m_pCombo = nullptr;
	for (int ii = 0; ii < CNT_STATIC ; ii++)
	{
		if (m_pStatic[ii]) {
			m_pStatic[ii] = nullptr;
		}
	}
	
	if (m_pBtnSave)		m_pBtnSave = nullptr;
	if (m_pEdit)		m_pEdit = nullptr;
	if (m_pBtnAdd)		m_pBtnAdd = nullptr;;
	if (m_pBtnDel)		m_pBtnDel = nullptr;;
	if (m_pBtnChange)	m_pBtnChange = nullptr;;
	
	if (m_pBtnJango)	m_pBtnJango = nullptr;;

	if (m_pJangoDlg)
	{
		m_pJangoDlg = nullptr;
	}

	m_ilist.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
//{{AFX_MSG_MAP(CMapWnd)
ON_WM_CREATE()
ON_WM_PAINT()
ON_WM_SIZE()
ON_MESSAGE(WM_USER, OnMessage)
ON_MESSAGE(WM_LIST_DBCLK,OnListDbclk)
ON_MESSAGE(WM_LIST_CLK,OnListClk)
ON_MESSAGE(WM_DO_CALC,OnDoCalc)
ON_NOTIFY(NM_CLICK, IDC_GRID, OnClickList)
ON_BN_CLICKED(IDC_BTN_SAVE,OnBtnSave)
ON_BN_CLICKED(IDC_BTN_ADD,OnBtnAdd)
ON_BN_CLICKED(IDC_BTN_DEL,OnBtnDel)
ON_BN_CLICKED(IDC_BTN_CHANGE,OnBtnChange)
ON_BN_CLICKED(IDC_BTN_JANGO,OnBtnJango)
ON_CBN_SELENDOK(IDC_COMBO_PORT, OnSelendokPort)
ON_WM_RBUTTONDOWN()
ON_NOTIFY(NM_RCLICK, IDC_GRID , OnRclickList)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define COLINFO_FIXED	 1
#define COLINFO_EDITABLE 2

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	char* pcBuffer{};
	CString szAlert;
	int len{};
	
	pcBuffer = (char*)lParam;
	len = HIWORD(wParam);
	
	const int key = HIBYTE(LOWORD(wParam));
	const int msg = LOBYTE(LOWORD(wParam));
	// 메세지 받을시... 키값을 세팅하면..
	// 키값은 user_TH.key 
	
	switch(msg)
	{
	case DLL_OUB:
		if (key == TR_DLGACCOUNT || key == TR_DLGACCOUNT-1)	// 계좌명조회,그룹조회
		{
			if (m_pJangoDlg)
				m_pJangoDlg->SendToAccount(WM_USER, wParam, lParam);
		}
		else if (key == TR_DLGREMAINS || key == TR_DLGREMAINF)
		{
			if (m_pJangoDlg)
				m_pJangoDlg->parsingRemainData(key == TR_DLGREMAINF, pcBuffer, len);
		}
		else
		{
			Parsing(pcBuffer, len ,key);
		}
		break;
//	case DLL_ALERT:
//		Alert(pcBuffer ,len ,key);
//		break;
	case DLL_ALERTx:
		parsingAlertx(lParam);
		break;
	case DLL_NOTICE:
		break;
	case DLL_SETPAL:
		ChangeColors();
		break;
	case DLL_SELECT:
		break;
	case DLL_DOMINO:
		//TRACE("DOMINO\n");
		break;
	case DLL_SETFONT:
		break;
	case DLL_TRIGGER:
		//TRACE("TRIGGER\n");
		break;
	case DLL_GUIDE:
		return true;
	}
	return 0;
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	GetAxVariant(titleCC,"[2003] 손익평가");
	m_szRoot = GetAxVariant(homeCC);
	m_szUser = GetAxVariant(nameCC);
	m_pFont = GetAxFont(9, false, "굴림");
	
	InitControl();
	if (!m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0))
	{	// 직원은 잔고버튼 숨김.
		if (m_pBtnJango)	
			m_pBtnJango->ShowWindow(SW_HIDE);
	}
	
	LoadPortList();
	m_strPal = GetPalette();
	
	return 0;
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc,rect;
	GetClientRect(&rc);
	rect = rc;

	dc.FillSolidRect(rc,GetAxColor(64));
	rc.left = rc.left + 1;
	rc.right = rc.right - 1;
	rc.bottom = rc.top + 30;
	rc.top = rc.top + 1;
	dc.FillSolidRect(rc,GetAxColor(66));
	DrawRoundBitmap(&dc, rc, ROUND_PANEL);

	rect.top = rc.bottom + GAP4;
	rect.left = rc.left;
	rect.right = rc.right;
	rect.bottom = rect.bottom - 2;
	dc.FillSolidRect(rect,GetAxColor(192));
	DrawRoundBitmap(&dc, rect, ROUND_PANEL);

/*	
	rc = CRect(105,9,180,22);
	rc.InflateRect(1,1,1,1);
	dc.Draw3dRect(rc, RGB(244,244,244),RGB(233,233,233) );
	rc.InflateRect(0,1,0,1);
	dc.Draw3dRect(rc, RGB(150,150,150),RGB(100,100,100) );
*/	
	CFont* pOld = dc.SelectObject(m_pFont);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(GetAxColor(69));
	dc.DrawText(MSG_STRING, CRect(380, 6, 660, 26), DT_VCENTER|DT_LEFT|DT_SINGLELINE);
	dc.SelectObject(pOld);
	
	for(int i = 0 ; i < CNT_TOPDATA ; i++)
	{
		rc = m_rcs[i*2+1];

		dc.Draw3dRect(rc,RGB(240,240,240),RGB(222,222,222));
		rc.DeflateRect(1,1,1,1);
		dc.FillSolidRect(rc,RGB(255,255,255) );
		pOld = dc.SelectObject(m_pFont);
		dc.SetBkMode(TRANSPARENT);
		if (i >= TOP_SKD)
		{
			if (m_TopData[TOP_SKD].GetLength() > 0)
			{
				if (m_TopData[TOP_SKD].Left(1) == "+")
				{
					dc.SetTextColor(RGB(255,0,0));
				}
				else if (m_TopData[TOP_SKD].Left(1) == "-" )
				{
					dc.SetTextColor(RGB(0,0,255));
				}
				else
				{
					dc.SetTextColor(RGB(20,20,20));
				}
			}
			else
			{
				dc.SetTextColor(RGB(20,20,20));
			}
		}
		else
		{
			dc.SetTextColor(RGB(20,20,20));
		}

		rc.DeflateRect(0,0,3,0);
		dc.DrawText(m_TopData[i],rc,DT_VCENTER|DT_RIGHT|DT_SINGLELINE);
		dc.SelectObject(pOld);
	}
}


void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if ( m_pList )
	{
		CRect rc;
		GetClientRect(&rc);
		m_pList->SetWindowPos(NULL, 3, 60, cx-10,cy-50-14,SWP_NOMOVE);
	}
}

void CMapWnd::OnClickList(NMHDR* pNMHDR, LRESULT* pResult){
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
}

LONG CMapWnd::OnListDbclk(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}
void CMapWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CString str;
	CRect rc;
	GetWindowRect(&rc);
	point.x  +=rc.left;
	point.y  +=rc.top;
	str.Format("IB200300\n%d\t%d\n",point.x,point.y);
	
	m_pParent->SendMessage( WM_USER ,MAKEWPARAM(mouseDLL,false) ,(LPARAM)(LPCTSTR)str );
	
	CWnd::OnRButtonDown(nFlags, point);
}

void CMapWnd::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString str;
	CPoint pt,pt1;
	GetCursorPos( &pt);
	
	int index{}, colNum{};
	CRect rc;
	m_pList->GetWindowRect(&rc);
	
	pt1 = pt;
	pt1.x -= rc.left;
	pt1.y -= rc.top ;
	index = m_pList->hitTestEx(pt1, &colNum);
	
	CString szcode = m_pList->GetItemText(index,0);
	if (szcode.GetLength() != 6)
		str.Format("IB200300\n%d\t%d\n", pt.x, pt.y);
	else
		str.Format("IB200300\n%d\t%d\n1301\t%s\n", pt.x, pt.y, szcode);
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (long)str.operator LPCTSTR());
	*pResult = 0;
}

long CMapWnd::OnListClk(WPARAM wParam, LPARAM lParam)
{
	const int nItem = (int)wParam;
	const int nSubItem = (int)lParam;
	
	if( nSubItem == COL_CDLG)	// 종목추가(dialog)
	{
		CString temp,code;
		CPoint pt(152, 50); ClientToScreen(&pt);
		temp = GetAxCode(kospiCODE,MAKELPARAM(pt.x, pt.y));
		if (temp == "")
			return 0;
		code = Parser(temp, "\t");
		if (code== "")
			return 0 ;
		
		m_szCode = code;
		m_szName = temp;
		
		if( m_szCode !="")
		{
			for (int i = 0; i < MAX_ROW_CNT;i++)
			{
				if (m_pList->GetItemText(i,0) == "")
				{
					m_nCurSel =i;
					break;
				}
			}
			if (m_nCurSel >= nItem)
			{
				m_nCurSel = nItem;
			}
			m_pList->SetItemText(m_nCurSel,COL_CODE,m_szCode );
			m_pList->SetItemText(m_nCurSel,COL_NAME,m_szName );
			m_pList->SetItemText(m_nCurSel,COL_MYGA,"0" );
			m_pList->SetItemText(m_nCurSel,COL_MYAM,"0" );
			m_pList->SetItemText(m_nCurSel,COL_PGGA,"0" );
			m_pList->SetItemText(m_nCurSel,COL_BENEFIT,"0%" );
			m_pList->SetItemText(m_nCurSel,COL_SONIK,"0" );
			m_pList->SetItemText(m_nCurSel,COL_FEE,RATE);
			m_pList->Invalidate(false);//BUFFET

			CString data;
			data = "1301";
			data += (char)0x7f;
			data += m_szCode;
			RegistRTS(data, 1, m_nCurSel);
		}
	}
	else 
	{
		CString sDat, sTmp = _T("");
		sTmp = m_pList->GetItemText( nItem ,0);
		if( sTmp.GetLength() != 6)
			return 0L;
		
		sTmp.Insert(0, "1301\t");
		GetAxVariant(triggerCC, sTmp);
	}
	m_nClkItem = nItem;
	m_nClkSubItem = nSubItem;
	return 0L;
}

void CMapWnd::InitControl()
{
	CString strPath = "";
	strPath.Format("%s\\Image\\CODESCREENSEARCH.BMP", m_szRoot);

	CBitmap* pBmp = (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)(LPCTSTR)strPath);
	m_ilist.Add(pBmp, RGB(0, 0, 0));

	m_pList = std::make_unique<CSListCtrl1>(this);
	CRect rcc, lrc, btnRc;
	GetClientRect(&rcc);
	lrc.SetRect(GAP5, rcc.top+50+GAP9, rcc.right-18, rcc.bottom-15);
	m_pList->Create(LVS_REPORT|WS_VISIBLE|WS_CHILD|WS_VSCROLL|WS_HSCROLL|WS_CLIPCHILDREN,
		lrc ,this, IDC_GRID);
	m_pList->SetTextColor( GetAxColor(69));
	m_pList->SetBkColor( GetAxColor(68),GetAxColor(77));
	m_pList->SetEdgeColor( GetAxColor(65));
	m_pList->SetSelectColor( GetAxColor(78));
	m_pList->m_HdCtrl.SetTextColor(GetAxColor(76));
	m_pList->m_HdCtrl.SetBkColor(GetAxColor(74));
	m_pList->m_HdCtrl.SetEdgeColor(GetAxColor(65));
	
	m_pList->InsertColumn(COL_CODE, _T("코드"), LVCFMT_CENTER, 45, 0 );m_pList->m_EditableList.Add(0);
	m_pList->InsertColumn (COL_CDLG, _T(""), LVCFMT_CENTER, 21, 1 );
	m_pList->InsertColumn (COL_NAME, _T("종목명"), LVCFMT_LEFT, 80, 2 );
	
	m_pList->HeaderFix(true,COL_NAME);
	
	m_pList->InsertColumn (COL_CURR, _T("현재가"), LVCFMT_RIGHT, 60, 3 );
	m_pList->InsertColumn (COL_DIFF, _T("대비"), LVCFMT_RIGHT, 59, 4 );
	m_pList->InsertColumn (COL_RATE, _T("등락율"), LVCFMT_RIGHT, 39, 5 );
	m_pList->InsertColumn (COL_MYGA, _T("매입가"), LVCFMT_RIGHT, 59, 6 );m_pList->m_EditableList.Add(6);
	m_pList->InsertColumn (COL_MYAM, _T("수량"), LVCFMT_RIGHT, 49, 7 );m_pList->m_EditableList.Add(7);
	m_pList->InsertColumn (COL_PGGA, _T("평가금액"), LVCFMT_RIGHT, 76, 8 );
	m_pList->InsertColumn (COL_BENEFIT, _T("수익율"), LVCFMT_RIGHT, 55, 9 );
	m_pList->InsertColumn (COL_SONIK, _T("손익"), LVCFMT_RIGHT, 75, 10 );
	m_pList->InsertColumn (COL_FEE, _T("수수료"), LVCFMT_RIGHT, 51, 11 );m_pList->m_EditableList.Add(11);
	
	for(int i = 0 ; i < MAX_ROW_CNT ; i ++)
		m_pList->InsertItem(i,"");
	
	m_pList->ModifyStyle(0,LVS_SINGLESEL|LVS_SHOWSELALWAYS );
	m_pList->SetExtendedStyle(LVS_EX_FULLROWSELECT  );
	CFont font;
	font.CreatePointFont(120,"굴림");
	m_pList->SetFont( &font);
	font.DeleteObject();

	m_pList->SetImageList(&m_ilist, LVSIL_STATE);
	
	m_pCombo = std::make_unique<CComboBox>();
	m_pCombo->Create(WS_CHILD|WS_VISIBLE|CBS_AUTOHSCROLL|CBS_HASSTRINGS|CBS_DROPDOWNLIST,
		CRect(5,5,100,25),this,IDC_COMBO_PORT);
	SetWindowTheme(m_pCombo->GetSafeHwnd(), L"", L"");

	m_pCombo->SetCurSel(0);
	CRect wRc;
	m_pCombo->GetWindowRect(&wRc);
	m_pCombo->SetWindowPos(NULL, 0, 0, wRc.Width(), 20*10, SWP_NOMOVE|SWP_SHOWWINDOW);
	m_pCombo->SetFont(m_pFont);
	
	m_pEdit = std::make_unique<CEdit>();
	m_pEdit->CreateEx(WS_EX_CLIENTEDGE, _T("Edit"), "", 
		ES_LEFT|WS_VISIBLE|WS_CHILD,CRect(105,5,180,25),this,IDC_EDIT_PORT);
	m_pEdit->SetFont(m_pFont);
	
	CString szn,szd,szu;
	szn = m_szRoot + "\\image\\" + "2BTN.BMP";
	szd = m_szRoot + "\\image\\" + "2BTN_DN.BMP";
	szu = m_szRoot + "\\image\\" + "2BTN_EN.BMP";
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), szn,
		IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	int bx = 0,by = 0;
	
	if(hBitmap)
	{
		CBitmap Bitmap;
		Bitmap.Attach(hBitmap);
		
		BITMAP	bm;
		
		Bitmap.GetBitmap(&bm);
		bx = bm.bmWidth;
		by = bm.bmHeight;
		Bitmap.Detach();
	}

	//종목버튼 이미지 추가
	CString	fileIMG;
	fileIMG.Format("%s\\%s\\", m_szRoot, "image");
	
	HBITMAP	hbitmap1{}, hbitmap1_dn{};
	hbitmap1 = getBitmap(fileIMG + "손익검색.bmp");
	hbitmap1_dn = getBitmap(fileIMG + "손익검색_DN.bmp");
	m_pList->SetImageType(hbitmap1, hbitmap1_dn);	


	btnRc.SetRect(185, 5, 183 + bx, 5+by);
	
	m_pBtnAdd = std::make_unique<CxSkinButton>();
	m_pBtnAdd->Create("추가", WS_CHILD|WS_VISIBLE, btnRc, this, IDC_BTN_ADD);
	m_pBtnAdd->SetSkin(szn, szd, szu);
	m_pBtnAdd->SetFont(m_pFont);
	
	btnRc.OffsetRect(btnRc.Width() + GAP, 0);
	m_pBtnDel = std::make_unique<CxSkinButton>();
	m_pBtnDel->Create("삭제", WS_CHILD|WS_VISIBLE, btnRc, this, IDC_BTN_DEL);
	m_pBtnDel->SetSkin(szn, szd, szu);
	m_pBtnDel->SetFont(m_pFont);
	
	btnRc.OffsetRect(btnRc.Width() + GAP, 0);
	m_pBtnChange = std::make_unique<CxSkinButton>();
	m_pBtnChange->Create("변경", WS_CHILD|WS_VISIBLE, btnRc, this, IDC_BTN_CHANGE);
	m_pBtnChange->SetSkin(szn, szd, szu);
	m_pBtnChange->SetFont(m_pFont);
	
	btnRc.OffsetRect(btnRc.Width() + GAP, 0);
	m_pBtnJango = std::make_unique<CxSkinButton>();
	m_pBtnJango->Create("잔고", WS_CHILD|WS_VISIBLE, btnRc, this, IDC_BTN_JANGO);
	m_pBtnJango->SetSkin(szn, szd, szu);
	m_pBtnJango->SetFont(m_pFont);
	
	
	CString str[CNT_STATIC]={"총매수금","","총평가금","","총손익금","","총수익율",""};
	//CRect rcS;
	for(int i = 0 ; i < CNT_STATIC ; i++)
	{
		m_pStatic[i] = std::make_unique<CSStatic>();
		if ( i%2 == 0)
		{
			m_pStatic[i]->SetBkColor( GetAxColor(74) );
			m_pStatic[i]->SetTextColor( GetAxColor(76) );
			m_pStatic[i]->SetEdgeColor( GetAxColor(65) );
			if( i==0)
				m_rcs[i] = CRect(GAP5,29+GAP9,70, 56);
			else
				m_rcs[i] = CRect(m_rcs[i-1].right,29+GAP9, m_rcs[i-1].right+69,56);
			
			m_pStatic[i]->Create(str[i],WS_VISIBLE|WS_CHILD,m_rcs[i],this, IDC_STATIC_CALC+i);
			m_pStatic[i]->SetFont(m_pFont);
		}
		else
		{		
			m_pStatic[i]->SetBkColor( RGB(255,255,255) );
			m_pStatic[i]->SetTextColor( RGB(30,30,30) );
			m_pStatic[i]->SetEdgeColor( RGB(211,211,211) );
			m_pStatic[i]->m_bRight = true;
			if( i == 7)
				m_rcs[i] = CRect(m_rcs[i-1].right, 29+GAP9, m_rcs[i-1].right+70, 56);
			else
				m_rcs[i] = CRect(m_rcs[i-1].right,29+GAP9,  m_rcs[i-1].right+115, 56);

		}
	}
	m_pBtnSave = std::make_unique<CxSkinButton>();
	m_pBtnSave->Create("저장",WS_VISIBLE|WS_CHILD,
		CRect( rcc.right- bx - 45, 5, rcc.right-45, 5+by),this, IDC_BTN_SAVE );
	m_pBtnSave->SetSkin( szn,szd,szu);
	m_pBtnSave->SetFont(m_pFont);
}

HBITMAP CMapWnd::getBitmap(CString path)
{
	CBitmap*	pBitmap = (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (LPARAM)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

void CMapWnd::ChangeColors()
{
	m_pList->SetTextColor( GetAxColor(69));
	m_pList->SetBkColor( GetAxColor(68),GetAxColor(77));
	m_pList->SetEdgeColor( GetAxColor(65));
	m_pList->SetSelectColor( GetAxColor(78));
	m_pList->m_HdCtrl.SetTextColor(GetAxColor(76));
	m_pList->m_HdCtrl.SetBkColor(GetAxColor(74));
	m_pList->m_HdCtrl.SetEdgeColor(GetAxColor(65));
	m_pList->Invalidate();
	m_strPal = GetPalette();

	Invalidate();
	for(int i = 0 ; i < CNT_STATIC ; i++)
	{
		if( i%2 == 0)
		{
			m_pStatic[i]->SetBkColor( GetAxColor(74) );
			m_pStatic[i]->SetTextColor( GetAxColor(76) );
			m_pStatic[i]->SetEdgeColor( GetAxColor(65) );

			m_pStatic[i]->Invalidate();

		}
		
	}
}

void CMapWnd::Clear(int nItem,bool bRepaint)
{
	if( !bRepaint )
	{
		for(int i = 0 ; i < MAX_COL_CNT ; i++)
			m_pList->SetItemText(nItem, i, "");
		
		OnDoCalc(0,5);
	}
	else
	{
		m_pList->DeleteItem(nItem);
		m_pList->InsertItem(19,"" );
		OnDoCalc(0,5);
		m_pList->Invalidate(false);
	}
}

LONG CMapWnd::OnDoCalc(WPARAM wParam, LPARAM lParam)
{
	const int nItem = (int)wParam;
	double hjk=0,
		msd =0,
		msr = 0,
		msk = 0,tmsk = 0,
		pgk = 0,tpgk = 0;
	double	son = 0,tson = 0;
	double ssr= 0.0,
		sir= 0.0,
		tsir = 0.0;
	
	char temp[108]={0};	 
	CString tmp = "",tmp1 = "",tmp2;
	
	if( lParam == COL_CODE )
	{
		tmp =  m_pList->GetItemText(nItem , COL_CODE) ;
		int nCurSel = 0;
		for(int i = 0; i < MAX_ROW_CNT;i++)
		{
			if( m_pList->GetItemText(i,COL_CODE) == "" )
			{
				nCurSel =i;
				break;
			}
		}
		const int spcnt = 6-tmp.GetLength();
		for(int i = 0 ; i < spcnt ; i++)
			tmp.Insert(0,"0");
		
		if( tmp.GetLength() == 6 )
		{
			if( !m_pParent->SendMessage(WM_USER , MAKEWPARAM(variantDLL, whichCC),(LONG)(LPCTSTR)tmp) )
			{
				Clear(nItem,nCurSel-1>nItem);
				return 0L;
			}
			
			if(nCurSel >= (int)nItem )
			{
				nCurSel = (int)nItem;
			}		
			
			CString data;
			data = "1301";
			data+= (char)0x7f;
			data+= tmp;
			RegistRTS(data, 1 , nCurSel );
			
			m_pList->SetItemText(nItem, COL_CODE, "");
			m_pList->SetItemText(nCurSel, COL_CODE , tmp  );
			tmp1 = (char*)m_pParent->SendMessage( WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)(LPCTSTR)tmp );
			m_pList->SetItemText(nCurSel, COL_NAME,tmp1 );
			if( m_pList->GetItemText(nCurSel,COL_MYGA) == "" )
				m_pList->SetItemText(nCurSel,COL_MYGA,"0" );
			if( m_pList->GetItemText(nCurSel,COL_MYAM) == "" )
				m_pList->SetItemText(nCurSel,COL_MYAM,"0" );
			if( m_pList->GetItemText(nCurSel,COL_PGGA) == "" )
				m_pList->SetItemText(nCurSel,COL_PGGA,"0" );
			if( m_pList->GetItemText(nCurSel,COL_BENEFIT) == "" )
				m_pList->SetItemText(nCurSel,COL_BENEFIT,"0%" );
			if( m_pList->GetItemText(nCurSel,COL_SONIK) == "" )
				m_pList->SetItemText(nCurSel,COL_SONIK,"0" );
			if( m_pList->GetItemText(nCurSel,COL_FEE) == "" )
				m_pList->SetItemText(nCurSel ,COL_FEE, RATE) ;
		}
		else
		{
			Clear(nItem,m_pList->GetItemText( min(19,nItem+1), 0)!="") ;
		}
		return 0;
	}
	
	if( m_pList->GetItemText( nItem, COL_CODE )  != "" &&
		m_pList->GetItemText( nItem, COL_CURR )  != "" &&
		m_pList->GetItemText( nItem, COL_MYGA )  != "0" &&
		m_pList->GetItemText( nItem, COL_MYAM )  != "0" )
	{
		hjk = ConvertToInt(m_pList->GetItemText(nItem, COL_CURR));
		msd = ConvertToInt(m_pList->GetItemText(nItem, COL_MYGA));
		msr = ConvertToInt(m_pList->GetItemText(nItem, COL_MYAM));
		tmp = m_pList->GetItemText( nItem,11);
		tmp1 = tmp.Left( tmp.GetLength() -1 );
		ssr = atof((char*)(LPCTSTR)tmp1);

		if ( ssr > 0 )//0.05
		{

			msk = msd*msr+ (int)(msd*msr*(double)ssr/100);//매입금
			pgk = hjk*msr - (int)(hjk*msr*((double)ssr/100 + TAX_RATE)); //평가금
			
			son = (pgk - msk);
			if( msk != 0)
				sir = 100*(son )/(double)msk;
			
			_gcvt(pgk, 14,temp );
			tmp2 = temp;
			int nIdx = tmp2.Find(".");
			if( nIdx >=0)
				tmp2 = tmp2.Left(nIdx);
			tmp2 = GetCommaNum( tmp2);
			m_pList->SetItemText( nItem , COL_PGGA , tmp2);//평가금액
			
			if( sir > 0)
				tmp2.Format("%.2f%%", sir);
			else
				tmp2.Format("%.2f", sir);//BUFFET
			
			m_pList->SetItemText( nItem , COL_BENEFIT , tmp2);//수익률
			
			_gcvt(son, 14,temp );
			tmp2 = temp;
			nIdx = tmp2.Find(".");
			if( nIdx >=0)
				tmp2 = tmp2.Left(nIdx);
			tmp2 = GetCommaNum( tmp2);

			m_pList->SetItemText( nItem , COL_SONIK , tmp2);//평가손익
		}else{
			//const int krst = 0;//( hjk*msr*0.3/(double)100 );
			const int krs = 0;
			
			/*if( krst % 10 >= 5 )
			{
				krs = krst + (10-krst%10);
			}
			else
			{
				krs = krst - krst%10;
			}*/
			
			msk = msd*msr+  (((int)(msd*msr*(double)ssr/100))/10)*10;
			pgk = hjk*msr - (((int)(hjk*msr*(double)ssr/100))/10)*10 - krs; 
			
			son = (pgk - msk);
			if( msk != 0)
				sir = 100*(son )/(double)msk;
			
			_gcvt(pgk, 14,temp );
			tmp2 = temp;
			int nIdx = tmp2.Find(".");
			if( nIdx >=0)
				tmp2 = tmp2.Left(nIdx);
			tmp2 = GetCommaNum( tmp2);
			m_pList->SetItemText( nItem , COL_PGGA , tmp2);
			
			if( sir > 0)
				tmp2.Format("%.2f%%", sir);
			else
				tmp2.Format("%.2f", sir);//BUFFET
			
			m_pList->SetItemText( nItem , COL_BENEFIT , tmp2);
			
			_gcvt(son, 14,temp );
			tmp2 = temp;
			nIdx = tmp2.Find(".");
			if( nIdx >=0)
				tmp2 = tmp2.Left(nIdx);
			tmp2 = GetCommaNum( tmp2);
			
	//		if( son > 0 && tmp2.GetLength() > 0)
	//			tmp2.Insert( 0, "+");
			m_pList->SetItemText( nItem , COL_SONIK , tmp2);
		}
		
	}
	
	for (int i = 0 ; i < m_pList->GetItemCount() ; i++ )
	{
		if( m_pList->GetItemText(i,COL_CODE) == "" ||
			m_pList->GetItemText(i,COL_CURR) == "" ||
			m_pList->GetItemText(i,COL_MYGA) == "0" ||
			m_pList->GetItemText(i,COL_MYAM) == "0" ||
			m_pList->GetItemText(i,COL_FEE) == "0")
			continue;
		
		hjk = ConvertToInt(m_pList->GetItemText(i, COL_CURR));
		msd = ConvertToInt(m_pList->GetItemText(i, COL_MYGA));
		msr = ConvertToInt(m_pList->GetItemText(i, COL_MYAM));
		tmp = m_pList->GetItemText( i,COL_FEE);
		tmp1 = tmp.Left( tmp.GetLength() -1 );
		ssr = atof((char*)(LPCTSTR)tmp1);
		
		//const int krst = 0;//( hjk*msr*0.3/(double)100 );
		const int krs = 0;

	if ( ssr > 0 )
		{
			msk = msd*msr+  (int)(msd*msr*(double)ssr/100);//매입금
			pgk = hjk*msr - (int)(hjk*msr*((double)ssr/100 + TAX_RATE)); //평가금

		}else{	
			/*if( krst % 10 >= 5 )
			{
				krs = krst + (10-krst%10);
			}
			else
			{
				krs = krst - krst%10;
			}*/
			
			msk = msd*msr+  (((int)(msd*msr*(double)ssr/100))/10)*10;
			pgk = hjk*msr - (((int)(hjk*msr*(double)ssr/100))/10)*10 - krs;
		}
		
		son = pgk - msk;
		if( msk != 0 )
			sir = (double)100*(son)/msk;
		
		tmsk += msk;
		tpgk += pgk;
	}
	
	if( tmsk > 0)
	{
		_gcvt(tmsk, 14,temp );
		tmp = temp;
		int nIdx = tmp.Find(".");
		if( nIdx >=0)
			tmp = tmp.Left(nIdx);
		
		tmp2 = GetCommaNum(tmp );
		m_TopData[TOP_MSD] = tmp2;
		
		_gcvt(tpgk, 14,temp );
		tmp = temp;
		nIdx = tmp.Find(".");
		if( nIdx >=0)
			tmp = tmp.Left(nIdx);
		tmp2 = GetCommaNum(tmp );
		m_TopData[TOP_PGD] = tmp2;
		
		tson = tpgk -tmsk;
		_gcvt(tson, 14,temp );
		tmp = temp;
		
		if( tson > 0 && tmp.GetLength() > 0)
			tmp.Insert(0, "+");
		
		nIdx = tmp.Find(".");
		if( nIdx >=0)
			tmp = tmp.Left(nIdx);
		tmp2 = GetCommaNum(tmp );	
		m_TopData[TOP_SKD] = tmp2;
		
		if( tmsk != 0)
			tsir = (double)100*(tson)/tmsk;
		
		if( tsir != 0.0)
		{
			if( tsir > 0)
				tmp2.Format("+%.2f%%", tsir);
			else
				tmp2.Format("%.2f%%", tsir);
			m_TopData[TOP_RTD] = tmp2;
		}
	}
	else
	{
		m_TopData[TOP_MSD] = "";
		m_TopData[TOP_PGD] = "";
		m_TopData[TOP_SKD] = "";
		m_TopData[TOP_RTD] = "";
	}
	
	for(int i = 0 ; i < CNT_TOPDATA ; i++)
		InvalidateRect( m_rcs[i*2+1] );
	return 0L;
}

void CMapWnd::Parsing(char* buff,int len,int key)
{
	if( key < MAX_ROW_CNT)
	{
		_gridHo *grio = (_gridHo*)buff;
		
		CString data = CString(buff + sizeof(_gridHo),len - sizeof(_gridHo));
		CString tmp,tmp1,tmp2;
		int nSign = 0;
		
		tmp = Parser(data,"\t");
		
		if( tmp.GetLength() <= 0 )
		{
			Clear(key);
			return;
		}
		if( tmp == "")
			tmp = "0";
		
		tmp1 = GetCommaNum(tmp);
		
		if( tmp1.Left(1) == "+" || tmp1.Left(1) == "-" )
			m_pList->SetItemText(key,COL_CURR, tmp1.Mid(1));
		else
			m_pList->SetItemText(key,COL_CURR, tmp1);
		
		tmp = Parser(data,"\t");
		if( tmp == "")
			tmp = "0";
		
		tmp1 = tmp.Mid(1);
		if( tmp1.GetLength() > 3 )
			tmp2 = tmp.Left(1) + GetCommaNum(tmp1);
		else
			tmp2 = tmp.Left(1) + tmp1;
		
		nSign = GetSignText(tmp2);
		
		m_pList->SetItemText(key,COL_DIFF, tmp2);
		m_pList->SetItemData(key,nSign);
		
		tmp = Parser(data,"\n");

		if( tmp.Left(1) == "+" )
			m_pList->SetItemText(key,COL_RATE, tmp.Mid(1));
		else
			m_pList->SetItemText(key,COL_RATE, tmp);

//		m_pList->SetItemText(key,COL_RATE, tmp);
		
		OnDoCalc( key , 5 );
		
	}
	else if( key >= MAX_ROW_CNT )
	{
		int	nItem =0;
		CString	text,tmp,code;
		
		text  = CString(buff, len);
		code = Parser(text , "\t");
		nItem = key - MAX_ROW_CNT;
		
		if( code.GetLength() < 6  )
		{
			Clear(nItem);
			return;
		}
		
		code = code.Mid(1);
		tmp = CString( text.Mid(12) , 20 );
		tmp.TrimLeft();tmp.TrimRight();
		m_pList->SetItemText(nItem, COL_NAME , tmp );
		CString data;
		data = "1301";
		data+= (char)0x7f;
		data+= code;
		RegistRTS( data ,1,nItem);
	}
}

void CMapWnd::Alert(char* buff,int len, int key)
{
	CString sRtsData = buff;
	CString sCode = Parser(sRtsData, "\t");
	
	if( sCode.GetLength() < 7)
		return;

	const int	index = sRtsData.Find("\n");
	if (index > 0)
		sRtsData = sRtsData.Left(index);
	WriteAlert(sCode, sRtsData);
}




void CMapWnd::parsingAlertx(LPARAM lParam)
{
	CString	code, symbol, entry, strGubn, strCode;

	DWORD* data{};
	struct _alertR* alertR;
	alertR = (struct _alertR*)lParam;

	code = alertR->code;

	if (code.GetLength() < 7) return;

	data = (DWORD*)alertR->ptr[0];

	CString sSym, sData, temp;

	int nIdx = -1;

	for (int i = 0; i < m_pList->GetItemCount(); i++)
	{
		if (code.Mid(1) == m_pList->GetItemText(i, 0))
		{
			nIdx = i;
			if (nIdx < 0)
				break;

			bool bUpdate = false;

			CString sData;


			if (data[23]) {
				sData = (char*)data[23];
				sData = GetCommaNum(sData);
				if (sData.Left(1) == "+" || sData.Left(1) == "-")
					m_pList->SetItemText(nIdx, COL_CURR, sData.Mid(1));
				else
					m_pList->SetItemText(nIdx, COL_CURR, sData);

				bUpdate = true;
			}


			if (data[24]) {
				sData = (char*)data[24];
				temp = sData.Mid(1);
				if (temp.GetLength() > 3)
					sData = sData.Left(1) + GetCommaNum(temp);
				else
					sData = sData.Left(1) + temp;
				GetSignText(sData);
				m_pList->SetItemText(nIdx, COL_DIFF, sData);
				bUpdate = true;
			}

			if (data[33]) {
				sData = (char*)data[33];
				if (sData.Left(1) == "+")
					m_pList->SetItemText(nIdx, COL_RATE, sData.Mid(1));
				else
					m_pList->SetItemText(nIdx, COL_RATE, sData);
				//					m_pList->SetItemText(nIdx,COL_RATE, sData);
				bUpdate = true;
			}

			if (bUpdate)
			{
				//	TRACE("%d Update\n",nIdx);
				OnDoCalc(nIdx, 5);
			}
		}
	}
}



void CMapWnd::WriteAlert(CString sCode, CString sRtsData)
{
	CString sSym, sData,temp;
	
	int nIdx = -1;
	CString backup = sRtsData;
	for(int i = 0 ; i < m_pList->GetItemCount(); i++)
	{
		if( sCode.Mid(1) == m_pList->GetItemText(i,0) )
		{
			nIdx = i;
			if( nIdx < 0 )
				break;
			
			bool bUpdate = false;
			sRtsData = backup;
			while(!sRtsData.IsEmpty())
			{
				sSym = Parser(sRtsData, "\t");
				sData = Parser(sRtsData, "\t");
				if( sData == "")
					continue;
				if (sSym == "023")	// 현재가
				{
					sData = GetCommaNum(sData);
					if( sData.Left(1) == "+" || sData.Left(1) == "-" )
						m_pList->SetItemText(nIdx,COL_CURR, sData.Mid(1));
					else
						m_pList->SetItemText(nIdx,COL_CURR, sData);
					
					bUpdate = true;
				}
				else if( sSym == "024")	// 대비
				{
					temp = sData.Mid(1);
					if( temp.GetLength() > 3 )
						sData = sData.Left(1) + GetCommaNum(temp);
					else
						sData = sData.Left(1) + temp;
					GetSignText( sData);
					m_pList->SetItemText(nIdx,COL_DIFF, sData);
					bUpdate = true;					
				}
				else if( sSym == "033")	// 등락율
				{
					if( sData.Left(1) == "+" )
						m_pList->SetItemText(nIdx,COL_RATE, sData.Mid(1));
					else
						m_pList->SetItemText(nIdx,COL_RATE, sData);

//					m_pList->SetItemText(nIdx,COL_RATE, sData);
					bUpdate = true;
				}
			}
			
			if( bUpdate)
			{
				//	TRACE("%d Update\n",nIdx);
				OnDoCalc(nIdx,5 );
			}
		}				
	}
}


void CMapWnd::OnBtnAdd()
{
	CString str;
	m_pEdit->GetWindowText(str);
	str.TrimRight();

	if (str.IsEmpty())
	{
		str = "포트폴리오 명을 입력하세요.";
		::MessageBox(m_hWnd, str, "확인", MB_ICONINFORMATION);
		return;
	}
	
	if (m_PtArray.GetSize() >= 10)
	{
		str.Format("포트폴리오는 %d개로 제한되어 있습니다.", MAX_PORT_LIST);
		::MessageBox(m_hWnd, str, "확인", MB_ICONINFORMATION);
		return;
	}
	
	if (m_pCombo->FindString(-1, str) > -1)
	{
		str.Format("동일한 포트폴리오 명이 있습니다.");
		::MessageBox(m_hWnd, str, "확인", MB_ICONINFORMATION);
		return;
	}
	
	const int	idx = m_PtArray.GetSize();
	m_PtArray.InsertAt(idx, str);
	m_pCombo->InsertString(idx, str);
	m_nCurSel = 0;
	m_nPortCurSel = idx;
	SavePortList();
	m_pEdit->SetWindowText("");
	m_pCombo->SetCurSel(idx);
	Clear();
}

void CMapWnd::OnBtnDel()
{
	int	idx = m_pCombo->GetCurSel();
	if (idx == CB_ERR)	return;
	
	CString	str = _T("포트폴리오를 삭제 하시겠습니까?");
	if (::MessageBox(m_hWnd, str, "확인", MB_ICONSTOP|MB_OKCANCEL) != IDOK)
		return;
	const int	aryN = m_PtArray.GetSize();
	CString	file, file2;
	
	HANDLE		hFind{};
	WIN32_FIND_DATA FindFileData{};
	for (int ii = idx; ii < aryN; ii++)
	{
		file.Format("%s\\user\\%s\\ptplo%.2d.dat", m_szRoot , m_szUser, ii);
		file2.Format("%s\\user\\%s\\ptplo%.2d.dat", m_szRoot , m_szUser, ii+1);
		
		DeleteFile(file);
		hFind = FindFirstFile(file2, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE)
			MoveFile(file2, file);
	}
	
	m_pCombo->DeleteString(idx);
	m_PtArray.RemoveAt(idx);
	
	if (idx >= aryN - 1)	idx = m_pCombo->GetCount() - 1;
	
	m_nPortCurSel = idx;
	m_pCombo->SetCurSel(idx);
	SavePortList();
	LoadPortPolio();
}

void CMapWnd::OnBtnChange()
{
	const int	idx = m_pCombo->GetCurSel();
	if (idx == CB_ERR)	return;
	
	CString str, name;
	m_pEdit->GetWindowText(str);	str.TrimRight();
	
	if (str.IsEmpty())
	{
		::MessageBox(m_hWnd, "포트폴리오명을 입력하세요.", "확인", MB_ICONINFORMATION);
		return;
	}
	if (m_pCombo->FindString(-1, str) != CB_ERR)
	{
		::MessageBox(m_hWnd, "같은 포트폴리오명이 있습니다.", "확인", MB_ICONINFORMATION);
		return;
	}
	
	m_nPortCurSel = idx;
	m_nPortCurSel = m_pCombo->GetCurSel();
	m_pCombo->DeleteString(m_nPortCurSel);
	m_pCombo->InsertString(m_nPortCurSel, str);
	m_pCombo->SetCurSel(m_nPortCurSel);
	
	m_PtArray.RemoveAt(m_nPortCurSel);
	m_PtArray.InsertAt(m_nPortCurSel, str);
	SavePortList();
}

void CMapWnd::OnBtnJango()
{
	if (!m_pJangoDlg)
	{
		m_pJangoDlg = std::make_unique<CJangoDlg>(m_pParent, this) ;
	}
	
	if (m_pJangoDlg->DoModal() == IDOK)
	{
		if (m_pJangoDlg->m_arJango.GetSize() <= 0)
			return;
		int idx = 0;
		for (idx = 0; idx < MAX_ROW_CNT; idx++)
		{
			if (m_pList->GetItemText(idx,0) == "")
			{
				break;
			}
		}
		
		if (idx >= MAX_ROW_CNT)
			return;
		
		for (int ii = 0; ii < m_pJangoDlg->m_arJango.GetSize() && idx < MAX_ROW_CNT; ii++, idx++)
		{
			_Jango	Jango = m_pJangoDlg->m_arJango.GetAt(ii);
			
			m_pList->SetItemText(idx,COL_CODE,Jango.code.Mid(1));
			m_pList->SetItemText(idx,COL_NAME,Jango.name);
			m_pList->SetItemText(idx,COL_MYGA,GetCommaNum(Jango.price));
			m_pList->SetItemText(idx,COL_MYAM,GetCommaNum(Jango.num));
			m_pList->SetItemText(idx,COL_PGGA,"0");
			m_pList->SetItemText(idx,COL_BENEFIT,"0%");
			m_pList->SetItemText(idx,COL_SONIK,"0");
			m_pList->SetItemText(idx,COL_FEE,RATE);
			
			CString data;
			data = "1301";
			data+= (char)0x7f;
			data+= Jango.code.Mid(1);
			RegistRTS(data, 1, idx);
		}
		m_pList->Invalidate(false);
	}
	
	m_pJangoDlg = nullptr;
}

void CMapWnd::OnBtnSave()
{
	if (m_PtArray.GetSize() <= 0)
	{
		::MessageBox(m_hWnd, "저장할 항목이 없습니다.", "확인", MB_ICONINFORMATION);
		return;
	}
	
	m_nPortCurSel = m_pCombo->GetCurSel();
	if (m_nPortCurSel >= 0 && m_nPortCurSel <= MAX_PORT_LIST)
		SavePortPolio();
}

void CMapWnd::OnSelendokPort()
{
	m_nPortCurSel = m_pCombo->GetCurSel();
	LoadPortPolio();
	
}

void CMapWnd::Clear()
{
	int i;
	for(i = 0 ; i < CNT_TOPDATA ; i++)
	{
		m_TopData[i] = "";
		InvalidateRect( m_rcs[i*2+1], true);
	}

	for(i = 0 ; i < MAX_ROW_CNT ; i++)
	{
		for( int j = 0 ; j < MAX_COL_CNT ; j++)
		{
			if( m_pList->GetItemText(i,j) != "")
			{
				m_pList->SetItemText(i,j,"");
			}
		}
	}

	//090105 :: 콤보박스 change event시, 지워지지 않는 현상 수정
	m_pList->Invalidate();
//	::InvalidateRect(m_pList->GetSafeHwnd(), NULL, TRUE);
	m_nCurSel = 0;
}

void CMapWnd::LoadPortList()
{
	CString fileName;
	fileName.Format("%s\\user\\%s\\myptlist.dat",m_szRoot , m_szUser);
	
	CFile	file;
	if( !file.Open(fileName, CFile::modeRead) )
		return;
	UINT nReadBytes = 0;
	
	_portpolio_list rd;
	nReadBytes = file.Read( &rd, sizeof(_portpolio_list) );
	if (nReadBytes == sizeof(_portpolio_list))
	{
		for(int ii = 0 ; ii < MAX_PORT_LIST ; ii++)
		{
			if( lstrlen(rd.ptname[ii]) > 0 )
			{
				m_PtArray.InsertAt(ii, rd.ptname[ii] );
				m_pCombo->InsertString(ii, rd.ptname[ii]);
			}
		}
	}
	file.Close();	
	
	if (m_PtArray.GetSize() > 0)
	{
		m_pCombo->SetCurSel(0);
		LoadPortPolio();	
	}
}

void CMapWnd::SavePortList()
{
	CString fileName, Name;
	fileName.Format("%s\\user\\%s\\myptlist.dat",m_szRoot , m_szUser);
	
	CFile	file;
	if (!file.Open(fileName, CFile::modeCreate|CFile::modeWrite))
		return;
	
	_portpolio_list data;
	for (int ii = 0 ; ii < m_PtArray.GetSize(); ii++)
	{
		Name = m_PtArray.GetAt(ii);
		CopyMemory(data.ptname[ii], Name , min(40, Name.GetLength()));
	}
	file.Write( &data, sizeof(_portpolio_list));
	file.Close();
}

void CMapWnd::LoadPortPolio()
{
	CString str = "";
	str.Format("%s\\user\\%s\\ptplo%.2d.dat",m_szRoot , m_szUser ,m_nPortCurSel );
	
	Clear();
	CFile	file;
	if( !file.Open(str, CFile::modeRead) )
		return;
	UINT nReadBytes = 0;
	
	_portpolio rd;
	nReadBytes = file.Read( &rd, sizeof(_portpolio) );
	if( nReadBytes == sizeof(_portpolio))
	{
		for( int i = 0 ; i < MAX_ROW_CNT ; i++)
		{			
			for(int j = 0 ; j < 5 ; j++)
			{
				if( j ==0 )
					m_pList->SetItemText(i,COL_CODE,rd.rowd[i].code[j]  );
				else if( j==1)
					m_pList->SetItemText(i,COL_NAME,rd.rowd[i].code[j]  );
				else if( j==2)
					m_pList->SetItemText(i,COL_MYGA,rd.rowd[i].code[j]  );
				else if( j==3)
					m_pList->SetItemText(i,COL_MYAM,rd.rowd[i].code[j]  );
				else if( j==4)
					m_pList->SetItemText(i,COL_FEE,rd.rowd[i].code[j]  );
			}
			
			if( m_pList->GetItemText(i,COL_CODE) != "")
			{
				if(m_pList->GetItemText(i,COL_MYGA) =="" )
					m_pList->SetItemText(i,COL_MYGA,"0" );
				if(m_pList->GetItemText(i,COL_MYAM) =="" )
					m_pList->SetItemText(i,COL_MYAM,"0" );
				
				m_pList->SetItemText(i,COL_PGGA,"0" );
				m_pList->SetItemText(i,COL_BENEFIT,"0%" );
				m_pList->SetItemText(i,COL_SONIK,"0" );
				
				CString data;
				data = "1301";
				data+= (char)0x7f;
				data+= m_pList->GetItemText(i,0);
				RegistRTS( data, 1, i);
			}
		}
	}
	file.Close();
}


void CMapWnd::SavePortPolio()
{
	CString str = "";
	str.Format("%s\\user\\%s\\ptplo%.2d.dat",m_szRoot , m_szUser ,m_nPortCurSel );
	
	CFile	file;
	if( !file.Open(str, CFile::modeCreate|CFile::modeWrite) )
		return;
	const int nLastCnt = 0;
	_portpolio data;
	
	int col=0;
	for( int  i = 0 ; i < MAX_ROW_CNT ; i ++ )
	{
		
		for(int j = 0 ; j < 5 ; j++)
		{
			if( j ==0 )
				col = COL_CODE;
			else if( j==1)
				col = COL_NAME;
			else if( j==2)
				col = COL_MYGA;
			else if( j==3)
				col = COL_MYAM;
			else if( j==4)
				col = COL_FEE;
			str = m_pList->GetItemText(i,col);
			memcpy(data.rowd[i].code[j], str , min( 20 , str.GetLength()) );
		}	
	}
	
	file.Write( &data, sizeof(_portpolio));
	
	file.Close();
}

void CMapWnd::RegistRTS(CString str,int nCount,int nKey)
{
	CString sCodes ,// = "1301", 
		sTrData,
		sCnt;
	sCodes += str;
	sCnt.Format("%04d", nCount);
	
	sCodes += '\n';
	sTrData = "90304";
	sTrData += (char)0x7f;
	sTrData += "1\t";
	sTrData += "$90303";
	sTrData += (char)0x7f;
	
	struct _gridHi gridH;
	FillMemory(&gridH, sizeof(gridH), ' ');
	strncpy(gridH.visible, "99", 2);
	strncpy(gridH.rows, sCnt, 4);
	gridH.type = '0';
	gridH.dir  = '2';
	gridH.sort = '0';
	FillMemory(&gridH.symbol, 16, ' ');
	gridH.key  = '0';
	
	sTrData += CString((char*)&gridH, sizeof(_gridHi));
	sTrData += sCodes;
	// 현재가/대비/등락율
	sTrData += "2023\n2024\n2033\n\t";
	
	SendTR("POOP0090", sTrData, US_OOP, nKey);
}

void CMapWnd::SendTR(CString name, CString data, BYTE type, int key)
{
	const int len = data.GetLength() + L_userTH + 1;
	struct _userTH udat;

	std::unique_ptr<char[]> buf = std::make_unique<char[]>(len);
	
	strcpy(udat.trc, name);
	udat.key = key;
	udat.stat = type;
	
	CopyMemory(&buf.get()[0], &udat, L_userTH);
	CopyMemory(&buf.get()[L_userTH], data.GetString(), data.GetLength());
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, data.GetLength()), (LPARAM)buf.get());
}



CString CMapWnd::Parser(CString &srcstr, CString substr)
{
	CString temp;
	
	if (srcstr.Find(substr) == -1)
	{
		temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	
	return "";
}

COLORREF CMapWnd::GetAxColor(int iColor)
{
	if (iColor & 0x02000000)
		return iColor;
	
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)iColor);
}

CFont* CMapWnd::GetAxFont(int size ,bool bBold,CString str)
{
	
	_fontR fontR;
	memset(&fontR,0x00,sizeof(_fontR));
	fontR.name = (char*)(LPCTSTR)str;
	fontR.point =  size;
	fontR.italic = false;
	fontR.bold = bBold?FW_BOLD:FW_NORMAL;
	return (CFont*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(getFONT,0), (long)&fontR);
}


CString CMapWnd::GetAxVariant(int cmd, CString data)
{
	if( !m_pParent )
		return "";
	
	char* dta = (char*)m_pParent->SendMessage(WM_USER, 
		MAKEWPARAM(variantDLL, cmd), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)	return dta;
	else			return "";
}



CString CMapWnd::GetAxCode(int cmd,LPARAM lParam)
{
	if( !m_pParent )
		return "";
	
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(cmd, NULL)), lParam);
	if ((long)dta > 1)	return dta;
	else			return "";
}



CString CMapWnd::GetCommaNum(CString str)
{
	CString output ="";
	CString tmp;
	str.TrimLeft();
	str.TrimRight();
	int nDigit{};
	bool bSign = false,
		bComma = false;
	
	CString szSign = "";
	CString szleftc ="";
	CString szrightc = "";
	
	int comidx = 0;
	
	if( (comidx =str.Find(".") ) >=0 ){
		szleftc = str.Left(comidx);
		szrightc = str.Mid(comidx+1);
		bComma = true;
	}
	
	if( bComma ){
		for(int i = 0 ; i < 2 ; i++){
			output = "";
			tmp = "";
			if( i == 0)
				str = szleftc;
			else
				str = szrightc;
			
			if( str.Left(1) == "-" || str.Left(1) == "+")
			{
				tmp = str.Mid(1);
				nDigit = tmp.GetLength();
				bSign = true;
				szSign = str.Left(1);
			}else{
				tmp = str;
				nDigit = str.GetLength();
				bSign = false;
			}
			
			for( int j = 0 ; j < tmp.GetLength() ; j++ ){
				
				output+=tmp.GetAt( j );
				
				if( j != tmp.GetLength()-1 && nDigit > 3 && (nDigit-j)%3 == 1 ){
					output+=",";
				}
				
			}
			if(bSign){
				output = szSign + output;
			}	
			if( i ==0 )
				szleftc = output;
			else
				szrightc = output;
			
		}
		output = szleftc + "." + szrightc;
	}
	else
	{
		
		
		if( str.Left(1) == "-" || str.Left(1) == "+")
		{
			tmp = str.Mid(1);
			nDigit = tmp.GetLength();
			bSign = true;
			szSign = str.Left(1);
		}else{
			tmp = str;
			nDigit = str.GetLength();
		}
		
		for( int i = 0 ; i < tmp.GetLength() ; i++ ){
			
			output+=tmp.GetAt( i );
			
			if( i != tmp.GetLength()-1 && nDigit > 3 && (nDigit-i)%3 == 1 ){
				output+=",";
			}
			
		}
		if(bSign){
			output = szSign + output;
		}
	}
	return output;
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
		
		//right + top
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
		//left + top
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

	CString strPath = GetAxVariant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;

	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
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
	file.Format("%s\\%s\\%s", m_szRoot, "tab", "palette.ini");

	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;

	return strPal;
}

























