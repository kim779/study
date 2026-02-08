// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB300800.h"
#include "ConfigDlg.h"
#include "MainWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT	nExeChkID[]    =	{IDC_CHECK1, IDC_CHECK2, IDC_CHECK3, IDC_CHECK4, IDC_CHECK5};
UINT	nUnitCboID[]   =	{IDC_COMBO1,IDC_COMBO2,IDC_COMBO3,IDC_COMBO4};
UINT	nEditID[]      =	{IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4, IDC_EDIT5, IDC_EDIT6};
UINT	nTypeRadioID[] =	{IDC_RADIO1, IDC_RADIO2, IDC_RADIO3, IDC_RADIO4};

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog
CString CConfigDlg::GetDlgWindowText(UINT uiID)
{
	CString sResult = _T("");
	if( !GetDlgItem(uiID)) return sResult;

	GetDlgItem(uiID)->GetWindowText(sResult);
	return sResult;
}

bool CConfigDlg::SetDlgButtonCheck(UINT uiID, int icheck)
{
	if( !GetDlgItem(uiID)) return false;

	((CButton*)GetDlgItem(uiID))->SetCheck(icheck);
	return true;
}

int CConfigDlg::GetDlgButtonCheck(UINT uiID)
{
	if( !GetDlgItem(uiID)) return false;

	return ((CButton*)GetDlgItem(uiID))->GetCheck();
	return true;
}

int CConfigDlg::GetDlgComboGetCurSel(UINT uiID)
{
	if( !GetDlgItem(uiID)) return -1;
	return ( (CComboBox*)GetDlgItem(uiID) )->GetCurSel();

}

CConfigDlg::CConfigDlg(int iType, CString sCrtType, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	//}}AFX_DATA_INIT
	m_pMain = pParent;
	m_pHogaWnd = NULL;
	m_iType = iType;


	m_lblbackcolor = 0xFEE4D9;
	m_databackcolor = 0xD4D0CA;

	m_strCode = _T("");
	
	m_lblbackcolor = 0xFEE4D9;
	m_databackcolor = 0xD4D0CA;

	m_strSise[6] = sCrtType;

	if(m_iType)
		m_strSise[7] = "매  도";
	else
		m_strSise[7] = "청 산";

}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SS2, OnDeltaposSs2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SS3, OnDeltaposSs3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SS4, OnDeltaposSs4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SS5, OnDeltaposSs5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SS6, OnDeltaposSs6)
	ON_CBN_SELCHANGE(IDC_PRICECOMBO, OnSelchangePricecombo)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_COMMAND_RANGE(IDC_RADIO1, IDC_RADIO4, OnHogaGubnCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers
LONG CConfigDlg::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	switch(LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUB:
			SetOubData(wParam, lParam );
			break;
		case DLL_ALERT:
			SetRealData((char *)lParam) ;
			break;
	}

	return 0;
}

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	LOGFONT lf = GetLogFont(12);
	m_pFont.CreateFontIndirect(&lf);
	SetWindowTheme(GetDlgItem(IDC_CHECK4)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_CHECK5)->GetSafeHwnd(), L"", L"");
	SetWindowTheme(GetDlgItem(IDC_CHECK6)->GetSafeHwnd(), L"", L"");

	if(!m_iType)
	{
		m_strSise[0] = "전  체";
		SetWindowText("전체설정");
		SetWindowPos(NULL, 0,0,536,302,SWP_NOMOVE);
		GetDlgItem(IDC_STATICCTYPE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATICVOLUNIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_VOLTYPECOMBO)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_STATICVOLUNIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VOLTYPECOMBO)->ShowWindow(SW_SHOW);

		( (CComboBox*)GetDlgItem(IDC_PRICECOMBO) )->AddString("입력");
		
		
		CRect rc1;
		int ibtnw = 74, igapw=12;
		int ibtnh= 19;
		GetClientRect(&rc1);
		int ileft = rc1.right /2;
		ileft -= ( ((ibtnw*3)+(igapw*2) ) / 2 );

		GetDlgItem(IDC_CLEAR)->GetWindowRect(&rc1);
		ScreenToClient(&rc1);

		rc1.left = ileft;
		rc1.right = rc1.left + ibtnw;
		
		GetDlgItem(IDC_CLEAR)->MoveWindow(rc1);
		rc1.left = rc1.right + igapw;
		rc1.right = rc1.left + ibtnw;

		GetDlgItem(IDOK)->MoveWindow(rc1);
		rc1.left = rc1.right + igapw;
		rc1.right = rc1.left + ibtnw;

		GetDlgItem(IDCANCEL)->MoveWindow(rc1);
	}

	CreateHogaWindow();
	SetInitControl();
	SetConfigValue();
	if(!m_strCode.IsEmpty() )
		SendHogaQuery(m_strCode);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::CreateHogaWindow()
{
	CRect cRect, rect;
	GetClientRect(&rect);
	cRect = rect;
	cRect.left = cRect.left + (250*2) + 32;
	cRect.top = 38;
	cRect.right = cRect.left + 207;
	cRect.bottom = cRect.top + 300;

	m_pHogaWnd = new CHogaWindow(this);
	m_pHogaWnd->Create(cRect, this, IDC_HOGA);
	m_pHogaWnd->SetTextColor( ((CMainWnd*)m_pMain)->m_txtcolor );
	m_pHogaWnd->SetLineColor( ((CMainWnd*)m_pMain)->m_gridLinecolor );
	m_pHogaWnd->SetLabelColor( ((CMainWnd*)m_pMain)->m_gridhdrbgcolor );
	m_pHogaWnd->SetLabelTxtColor( ((CMainWnd*)m_pMain)->m_gridhdrtxtcolor);
	
	m_pHogaWnd->SetConditionColor( ((CMainWnd*)m_pMain)->m_clrConditionColor[0], ((CMainWnd*)m_pMain)->m_clrConditionColor[1], ((CMainWnd*)m_pMain)->m_txtcolor );
	m_pHogaWnd->ShowWindow(SW_SHOW);
	m_diffcolor = ((CMainWnd*)m_pMain)->m_txtcolor;
	
}

void CConfigDlg::SetInitControl()
{
	CString sTokens[] = {"틱", "%", "가격","입력"};

	int itokensize = 2;
	if(m_iType ) itokensize = 4;

	for(int i=0; i<itokensize; i++)
	{
		for(int icbo=0; icbo<4; icbo++)
			( (CComboBox*)GetDlgItem(nUnitCboID[icbo]) )->InsertString(i, sTokens[i]);
	}

	for(int icbo=0; icbo<4; icbo++)
		( (CComboBox*)GetDlgItem(nUnitCboID[icbo]) )->SetCurSel(0);

	for(int i=0; i<5; i++)
			( (CButton*)GetDlgItem(nExeChkID[i]) )->SetCheck(0);

	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(1);

	for(int i=0; i<6; i++)
		GetDlgItem(nEditID[i])->SetWindowText("0");

	//가능수량 100% default
	GetDlgItem(IDC_EDIT5)->SetWindowText("100");
	//--------------------------------
	//구분:지정가 default
	for(int irdo=0; irdo<4; irdo++)
		SetDlgButtonCheck(nTypeRadioID[irdo], 0);
	
	//가격:현재가 default
	( (CComboBox*)GetDlgItem(IDC_PRICECOMBO) )->SetCurSel(6);
	//( (CComboBox*)GetDlgItem(IDC_PRICECOMBO) )->SetCurSel(16);
	GetDlgItem(IDC_STATICPUNIT)->SetWindowText("틱");

	GetDlgItem(IDC_EDIT6)->SetWindowText("0");

	if(m_iType)
	{
		GetDlgItem(IDC_CHECK6)->ShowWindow(SW_HIDE);
	}
}

void CConfigDlg::SetConfigValue()
{
	for(int i=0; i<3; i++)
	{
		if( m_stConfig.istopexe[i])
		{
			CString strVal;

			if(m_stConfig.iunit[i] > 1)
				strVal.Format("%.2f",m_stConfig.ival[i]);
			else
				strVal.Format("%d",(int)(m_stConfig.ival[i]));

			SetDlgButtonCheck(nExeChkID[i], 1);
			( (CComboBox*)GetDlgItem(nUnitCboID[i]) )->SetCurSel( m_stConfig.iunit[i]);
			GetDlgItem(nEditID[i])->SetWindowText( strVal );

			if(i==2)
			{
				strVal = "";
				if(m_stConfig.iunit[3] > 1)
					strVal.Format("%.2f",m_stConfig.ival[3]);
				else
					strVal.Format("%d",(int)(m_stConfig.ival[3]));

				//고점대비
				( (CComboBox*)GetDlgItem(nUnitCboID[3]) )->SetCurSel( m_stConfig.iunit[3]);
				GetDlgItem(nEditID[3])->SetWindowText( strVal );
			}
		}
	}

	SetDlgButtonCheck(nTypeRadioID[m_stConfig.igubn], 1);//구분:0-지정가, 1-시장가, 2-조건부, 3-최유리
	OnHogaGubnCheck(nTypeRadioID[m_stConfig.igubn]);

	//수량
	( (CComboBox*)GetDlgItem(IDC_VOLTYPECOMBO) )->SetCurSel(m_stConfig.ivoltype);
	GetDlgItem(IDC_EDIT5)->SetWindowText(CStr(m_stConfig.ivolval));//수량

	( (CComboBox*)GetDlgItem(IDC_PRICECOMBO) )->SetCurSel(m_stConfig.iprctype);//가격타입

	CString s;
	s.Format("ORG: [%f]		CONV : [%s]\n",m_stConfig.iprcval,CFStr(m_stConfig.iprcval));
	OutputDebugString(s);

	SetDlgButtonCheck(IDC_CHECK4, m_stConfig.iauto[0]);//자동1 
	SetDlgButtonCheck(IDC_CHECK5, m_stConfig.iauto[1]);
	SetDlgButtonCheck(IDC_CHECK6, m_stConfig.inow);

	if(m_iType)
		((CButton*)GetDlgItem(nExeChkID[4]))->ShowWindow(SW_HIDE);
}

void CConfigDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC	backdc;
	backdc.CreateCompatibleDC(&dc); 
	CBitmap bitmap,  *pOldBitmap;
	CRect rectClient;
	GetClientRect(rectClient);

	bitmap.CreateCompatibleBitmap(&dc, rectClient.right, rectClient.bottom);
	pOldBitmap = (CBitmap*)backdc.SelectObject(&bitmap);
	backdc.SetBkMode(TRANSPARENT);
	
	backdc.FillSolidRect( rectClient, ::GetSysColor(COLOR_3DFACE)); 
	CFont* pOldFont = backdc.SelectObject(&m_pFont);	
	DrawPanel(&backdc);
	dc.BitBlt( rectClient.left , rectClient.top , rectClient.right , rectClient.bottom , &backdc, 0,0, SRCCOPY );
	bitmap.DeleteObject(); 
	backdc.SelectObject(pOldBitmap);
	backdc.SelectObject(pOldFont); 
	DeleteObject(pOldFont);
	DeleteObject(pOldBitmap);
	ReleaseDC(&backdc);	
	// Do not call CDialog::OnPaint() for painting messages
}

void CConfigDlg::DrawPanel(CDC* pDC)
{
	CRect rect, rectlbl, trect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, ((CMainWnd*)m_pMain)->m_toolcolor);

	rect.bottom = 242;
	int ih= 30;
	int iw = 250;
	int itop;
	int ileft;
	CPen pen(PS_SOLID, 1, ((CMainWnd*)m_pMain)->m_gridLinecolor );
	CPen*	pOldPen = pDC->SelectObject(&pen); 
	rect.DeflateRect(4, 4, 4, 4); 
	pDC->FillSolidRect(rect, 0xFFFFFF);
	pDC->Rectangle(rect);

	itop = rect.top+ih;
	pDC->MoveTo(rect.left, itop);
	pDC->LineTo(rect.right, itop);

	rectlbl = rect;
	rectlbl.right = rect.left+iw;
	rectlbl.bottom = itop;
	rectlbl.DeflateRect(1,1,0,1);
	pDC->SetTextColor(((CMainWnd*)m_pMain)->m_gridhdrtxtcolor);

	pDC->FillSolidRect(rectlbl, ((CMainWnd*)m_pMain)->m_gridhdrbgcolor);
	pDC->DrawText("감 시 조 건", rectlbl, DT_SINGLELINE |DT_VCENTER |DT_CENTER); 

	rectlbl.left = rect.left+iw;
	rectlbl.right = rectlbl.left+iw+21;
	pDC->FillSolidRect(rectlbl, ((CMainWnd*)m_pMain)->m_gridhdrbgcolor);
	pDC->DrawText("주 문 조 건", rectlbl, DT_SINGLELINE |DT_VCENTER |DT_CENTER); 

	ileft = rect.left+iw;
	pDC->MoveTo(ileft, rect.top);
	pDC->LineTo(ileft, rect.bottom);
	ileft = ileft+iw+22;

	pDC->SetTextColor(((CMainWnd*)m_pMain)->m_txtcolor);

	if(m_iType == 1)
	{
		pDC->MoveTo(ileft, rect.top);
		pDC->LineTo(ileft, rect.bottom);

		rectlbl = rect;
		rectlbl.top = rect.top + 5;
		rectlbl.left = ileft+5;
		rectlbl.bottom  = 30;
		DrawSisePanel(pDC, rectlbl);

		GetDlgItem(IDC_STATICCTYPE)->GetWindowRect(&trect);
		ScreenToClient(trect);
		trect.left = trect.right+10;
		trect.right = trect.left + 200; 
		trect.InflateRect(-5,3,0,4); 
		DrawLabelBox(pDC, trect);

		pDC->DrawText(m_strSise[6], trect, DT_SINGLELINE |DT_VCENTER |DT_LEFT); 
	}

	DeleteObject(pOldPen);
	GetDlgItem(IDC_STATICCNAME)->GetWindowRect(&trect);
	ScreenToClient(trect);
	trect.left = trect.right+10;
	trect.right = trect.left + 200; 
	
	CRect rc = trect;
	if(m_iType == 1)
	{
		rc.InflateRect(-5,3,0,4); 
		rc.right = rc.left + 52;
		DrawLabelBox(pDC, rc);
		pDC->DrawText(m_strCode, rc, DT_SINGLELINE |DT_VCENTER |DT_LEFT); 

		trect.InflateRect(-64,3,0,4); 
	}
	else trect.InflateRect(-5,3,0,4); 

	DrawLabelBox(pDC, trect);
	pDC->DrawText(m_strSise[0], trect, DT_SINGLELINE |DT_VCENTER |DT_LEFT); 

	GetDlgItem(IDC_STATICJTYPE)->GetWindowRect(&trect);
	ScreenToClient(trect);
	trect.left = trect.right+10;
	trect.right = trect.left + 200; 
	trect.InflateRect(-5,3,0,4); 
	DrawLabelBox(pDC, trect);
	pDC->DrawText(m_strSise[7], trect, DT_SINGLELINE |DT_VCENTER |DT_LEFT); 

	if(m_iType == 1)
	{
		rc = trect;
		GetDlgItem(IDC_STATICNUM)->GetWindowRect(&trect);
		ScreenToClient(trect);
		trect.left = trect.right + 3;
		trect.right = rc.right ;
		trect.InflateRect(0,3,0,3); 
		DrawLabelBox(pDC, trect);
		pDC->DrawText(m_strSise[8], trect, DT_SINGLELINE |DT_VCENTER |DT_RIGHT); 
	}

// 	pDC->SetTextColor(0xFF0000);
// 	GetDlgItem(IDC_CHECK4)->GetWindowRect(&trect);
// 	ScreenToClient(trect);
// 	trect.left = trect.right+1;
// 	trect.right = rect.right; 
// 	pDC->DrawText("잔고변동시 설정된 수량/가격 자동계산", trect, DT_SINGLELINE |DT_VCENTER |DT_LEFT); 
// 
// 	GetDlgItem(IDC_CHECK5)->GetWindowRect(&trect);
// 	ScreenToClient(trect);
// 	trect.left = trect.right+1;
// 	trect.right = rect.right;
// 
// 	if(!m_iType)
// 		pDC->DrawText("신규종목편입시 주문조건 자동적용및감시", trect, DT_SINGLELINE |DT_VCENTER |DT_LEFT); 
}

void CConfigDlg::SetCode(CString sCode, CString sName)
{
	int iFind = 0;
	if( (iFind=sName.Find("&") ) > -1) sName.Insert(iFind, '&'); 
	m_strCode = sCode;
	m_strSise[0] = sName;
}

void CConfigDlg::DrawLabelBox(CDC* pDC, CRect rect)
{
	CBrush brush(((CMainWnd*)m_pMain)->m_lblbackcolor);
	CBrush* pOldBrush = pDC->SelectObject(&brush); 
	CPen pen(PS_SOLID, 1, ((CMainWnd*)m_pMain)->m_lbllinecolor );
	CPen* pOldPen = pDC->SelectObject(&pen); 
	rect.InflateRect(2,0);
	pDC->Rectangle(rect); 

	pDC->SelectObject(pOldPen); 
	pDC->SelectObject(pOldBrush); 

	DeleteObject(pOldPen);
	DeleteObject(pOldBrush);
}

void CConfigDlg::DrawSisePanel(CDC* pDC, CRect rect)
{
	CRect rc = rect, trect;
	CBrush brush(0xFFFFFF);
	CBrush*	pOldBrush = pDC->SelectObject(&brush); 
	rc.DeflateRect(4,4,4,4);
	trect = rc;
	trect.right = trect.left + 100;

	int igap = 2;
	m_siseRect[0] = m_siseRect[1] = m_siseRect[2] = m_siseRect[3] = rc;

	m_siseRect[0].right = m_siseRect[0].left + 1;//종목명
	CRect trc = m_siseRect[0];

	if(m_strSise[3].GetLength() > 0 && m_strSise[3].GetAt(0) == '-')
	{
		m_diffcolor = RGB(0,0,255);
		m_strSise[3].Remove('-');
	}
	else
	{
		m_diffcolor = RGB(255,0,0);
	}

	pDC->SetTextColor(m_diffcolor); 
	m_siseRect[1].left = m_siseRect[0].right + igap;//현재가
	m_siseRect[1].right = m_siseRect[1].left + 64;
	pDC->DrawText(m_strSise[1],  m_siseRect[1], DT_SINGLELINE |DT_VCENTER |DT_LEFT);

	m_siseRect[2].left = m_siseRect[1].right + igap;//대비
	m_siseRect[2].right = m_siseRect[2].left + 62;
	trc = m_siseRect[2];
	trc.DeflateRect (15, 0,0,0); 
	pDC->DrawText(m_strSise[2],  trc, DT_SINGLELINE |DT_VCENTER |DT_RIGHT);

	m_siseRect[3].left = m_siseRect[2].right + 20/*(*/;//등락률
	m_siseRect[3].right = m_siseRect[3].left + 40/*%)*/;
	pDC->DrawText(m_strSise[3],  m_siseRect[3], DT_SINGLELINE |DT_VCENTER |DT_RIGHT);

	//rate
	trc.left = m_siseRect[2].left;
	trc.right = m_siseRect[2].left+15;
	pDC->DrawText(m_strDifSign,  trc, DT_SINGLELINE |DT_VCENTER |DT_LEFT);

	if( ((CMainWnd*)m_pMain) )
		pDC->SetTextColor(((CMainWnd*)m_pMain)->m_txtcolor ); 

	trc = m_siseRect[3];
	trc.left = m_siseRect[3].right;
	trc.right = m_siseRect[3].right+10;
	pDC->DrawText("%",  trc, DT_SINGLELINE |DT_VCENTER |DT_RIGHT);
		
	
	pDC->SelectObject(pOldBrush); 
	DeleteObject(pOldBrush);
}

void CConfigDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	ClearData();
	// TODO: Add your message handler code here
}

void CConfigDlg::ClearData()
{
	if(m_pHogaWnd)
	{
		m_pHogaWnd->DestroyWindow();
		delete m_pHogaWnd;
		m_pHogaWnd = NULL;
	}

	((CMainWnd*)m_pMain)->m_pQueryModalWnd = NULL;
}

//int dlen = sprintf(data, "40301%c%s\t40332\t40323\t40314\t40370\t40361\t40369\t40368\t40367\t", 0x7f, sendCode);
//int dlen = sprintf(data, "30301%c%s\t30021\t30022\t31023\t31024\t31204\t31027\t31201\t31206\t31033\t31203\t31205\t31202\t", 0x7f, m_fcodes.GetAt(n));
void CConfigDlg::SendHogaQuery(CString sCode)
{
	CString strSend = _T(""), strTmp;
// 	char*	symbols[] = {	"1021",  
// 							"2302",  "2303", //상한/하한
// 							"", "2027", "2321", //거래량
// 							"", "3040", "", //11~12 //시간
// 							"3045", "3055", "","3044","3054","", "3043","3053","","3042","3052","", "3041","3051","",//13~27  매도호가1~5
// 							"","3071","3061", "","3072","3062", "","3073","3063", "", "3074","3064",  "", "3075", "3065", //28~42 매수호가1~5
// 							"3101","","3106", //43~45 잔량합
// 							"2023", "2024", "2033"
// 						}; //
	if(sCode.GetAt(0) == '1' || sCode.GetAt(0) == '4' || sCode.GetAt(0) == 'A' || sCode.GetAt(0) == 'D')  //파생상품 코드개편
	{
		char* symbols[] = {	"30021",  
						"30354",  "30355", //상한/하한
						"", "31027", "", //거래량
						"", "31034", "", //11~12 //시간
						"32045", "32055", "","32044","32054","", "32043","32053","","32042","32052","", "32041","32051","",//13~27  매도호가1~5
						"","32071","32061", "","32072","32062", "","32073","32063", "", "32074","32064",  "", "32075", "32065", //28~42 매수호가1~5
						"32101","","32106", //43~45 잔량합
						"31023", "31024", "31033"
					}; //
		int isymbolscnt = 45;
		int r=0, c=0;
		for(int i=0; i<42; i++)
		{
			m_pHogaWnd->m_astrSymbolList[r][c++] = symbols[i];
			if(c >= 3) 
			{
				c= 0;
				r++;
				if(r >= 15) break;
			}
		}
		
		char	key;
		_trkey* trkey = (struct _trkey*)&key;
		trkey->gubn = HOGALIST;
		trkey->key =0;
		
		
		strTmp.Format("30301%c%s%c", 0x7f, sCode,tabCHAR);
		strSend += strTmp;
		
		for (int ii = 0; ii < isymbolscnt; ii++)
		{
			strTmp.Format("%s%c", CString(symbols[ii]), tabCHAR);
			strSend += strTmp;
		}
		
		((CMainWnd*)m_pMain)->m_pQueryModalWnd = this;
		((CMainWnd*)m_pMain)->SendTransaction(POOPQUERY, strSend, US_OOP, key);
	}
	else if(sCode.GetAt(0) == '2' || sCode.GetAt(0) == '3' || sCode.GetAt(0) == 'B' || sCode.GetAt(0) == 'C') //파생상품 코드개편
	{
		char* symbols[] = {	"40021",  
					"40354",  "40355", //상한/하한
					"", "41027", "", //거래량
					"", "41034", "", //11~12 //시간
					"42045", "42055", "","42044","42054","", "42043","42053","","42042","42052","", "42041","42051","",//13~27  매도호가1~5
					"","42071","42061", "","42072","42062", "","42073","42063", "", "42074","42064",  "", "42075", "42065", //28~42 매수호가1~5
					"42101","","42106", //43~45 잔량합
					"41023", "41024", "41033"
					}; //

		int isymbolscnt = 45;
		int r=0, c=0;
		for(int i=0; i<42; i++)
		{
			m_pHogaWnd->m_astrSymbolList[r][c++] = symbols[i];
			if(c >= 3) 
			{
				c= 0;
				r++;
				if(r >= 15) break;
			}
		}
		
		char	key;
		_trkey* trkey = (struct _trkey*)&key;
		trkey->gubn = HOGALIST;
		trkey->key =0;
		
		
		strTmp.Format("40301%c%s%c", 0x7f, sCode,tabCHAR);
		strSend += strTmp;
		
		for (int ii = 0; ii < isymbolscnt; ii++)
		{
			strTmp.Format("%s%c", CString(symbols[ii]), tabCHAR);
			strSend += strTmp;
		}
		
		((CMainWnd*)m_pMain)->m_pQueryModalWnd = this;
		((CMainWnd*)m_pMain)->SendTransaction(POOPQUERY, strSend, US_OOP, key);
	}
	else
	{
		return;
	}

}

void CConfigDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	PopupHoga();
}



void CConfigDlg::SetOubData(WPARAM wParam, LPARAM lParam)
{
	int iKey = HIBYTE(LOWORD(wParam));
	if( iKey == HOGALIST)
	{
		SetHogaData((char *)lParam) ;
		return ;
	}
}

void CConfigDlg::SetHogaData(char* szReceive) 
{
// 	CString s;
// 	s.Format("OUB DATA : %s\n",szReceive);
// 	OutputDebugString(s);
	CStringArray astrList;
	SplitString(szReceive, tabCHAR, astrList);
	if(astrList.GetSize() < 45) return;

// 	s.Format("현재가 : [%s]\n",astrList[42]);
// 	OutputDebugString(s);
	m_strSise[1] = RemoveSign(astrList[42], "-", "+"); //현재가
// 	s.Format("현재가2 : [%s]\n",m_strSise[1]);
// 	OutputDebugString(s);
	m_strSise[2] = SetDiffValue(astrList[43]); //대비
	m_strSise[3] = RemoveSign(astrList[44], "+"); //등락
	m_strRealCode = Trim(astrList[0]);

	for(int i=1; i<4; i++)
		InvalidateRect(m_siseRect[i]);

	if(!m_pHogaWnd) return;
	int r=0, c=0;
	for(int i=0; i<astrList.GetSize(); i++)
	{
		if(m_pHogaWnd->m_iDataFormat[r][c] == 6) astrList[i] = SetDataFormat(6, astrList[i], ":");
		else
		{
			if(m_pHogaWnd->m_iDataFormat[r][c] == 1)
			{
				astrList[i] = SetComma(astrList[i]);
			}
			else if(m_pHogaWnd->m_iDataFormat[r][c] > 0) 
				astrList[i] = astrList[i];
			if (astrList[i] == "0")
				astrList[i] = "";
		}

// 		CString s;
// 		s.Format("HOGAWND : %s\n",astrList[i]);
// 		OutputDebugString(s);
		if(astrList[i] == "0.00")
		{
			astrList[i] = "";
		}

		m_pHogaWnd->m_astrHogaList[r][c++] = astrList[i];

		
		if(c >= 3) 
		{
			c= 0;
			r++;
			if(r >= 15) break;
		}
	}

	m_pHogaWnd->ReDraw();
}

void CConfigDlg::SetRealData(char* szReceive) 
{
	//OutputDebugString("----------------------------------------------\n");
	if(!m_pHogaWnd) return;
// 	OutputDebugString(szReceive);
// 	OutputDebugString("\n");

	CStringArray arRowList;
	SplitString(szReceive, "\n", arRowList);
	if(arRowList.GetSize() <= 0) return;

	
	CString strData = arRowList.GetAt(0); 
	CString sRealCode = Trim( SplitString(strData, tabCHAR) );
	if(sRealCode.IsEmpty()) return; 
	if(sRealCode.GetAt(0) == 'X')
	{
		sRealCode = sRealCode.Mid(1);
	}

	CString s;
// 	s.Format("REALCODE : %s		RCVCODE : %s\n",m_strRealCode,sRealCode);
// 	OutputDebugString(s);

	if( m_strRealCode != sRealCode) return;

	CMapStringToString arTabList;
	if( SplitString_MAP(strData, tabCHAR, arTabList) <= 1) return;
	CString sRealSymbol, sRealval;

	if( arTabList.Lookup("023", sRealval) )
	{
// 		s.Format("현재가 : [%s]\n",sRealval);
// 		OutputDebugString(s);

		m_strSise[1] = RemoveSign(sRealval, "-", "+"); //현재가
		InvalidateRect(m_siseRect[1]);
// 		s.Format("현재가2 : [%s]\n",m_strSise[1]);
// 		OutputDebugString(s);
	}

	if( arTabList.Lookup("024", sRealval) )
	{
		m_strSise[2] = SetDiffValue(sRealval); //대비
		InvalidateRect(m_siseRect[2]);
	}

	if( arTabList.Lookup("033", sRealval) )
	{
		m_strSise[3] = sRealval; //등락
		InvalidateRect(m_siseRect[3]);
	}

	
	for(int r=0; r<15; r++)
	{
		for(int c=0; c<3; c++)
		{
			if(m_pHogaWnd->m_astrSymbolList[r][c].GetLength() < 3 ) continue;

			sRealSymbol = m_pHogaWnd->m_astrSymbolList[r][c].Right(3);
			if( arTabList.Lookup(sRealSymbol, sRealval) )
			{
				if(m_pHogaWnd->m_iDataFormat[r][c] == 6) sRealval = SetDataFormat(6, sRealval, ":");
				else
				{
					if(m_pHogaWnd->m_iDataFormat[r][c] == 1)
					{
						sRealval = SetComma(sRealval);
					}
					else if(m_pHogaWnd->m_iDataFormat[r][c] > 0) 
						sRealval = sRealval;

					if (sRealval == "0")
						sRealval.Empty();
				}

// 				CString s;
// 				s.Format("HOGAWND : [%d]	[%s]\n",m_pHogaWnd->m_iDataFormat[r][c],sRealval);
// 				OutputDebugString(s);
				m_pHogaWnd->m_astrHogaList[r][c] = sRealval;
				m_pHogaWnd->ReDraw(r, c);
			}
		}
	}
}

void CConfigDlg::PopupHoga()
{
	if( m_pHogaWnd->IsWindowVisible() )
	{
//		m_pHogaWnd->Show(false);
//		GetDlgItem(IDC_BUTTON1)->SetWindowText(">>");
		return;
	}

	CRect rect;
	GetDlgItem(IDC_BUTTON1)->SetWindowText("<<");

	GetDlgItem(IDC_PRICECOMBO)->GetWindowRect(rect);
	ScreenToClient(&rect);
	rect.right = rect.left +207;
	rect.bottom = rect.top ;
	rect.top = rect.top - (LINEH * 15);

	ClientToScreen(&rect);
	m_pHogaWnd->Show(true, rect);
}

void CConfigDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{	CDialog::OnLButtonUp(nFlags, point);}


void CConfigDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

HBRUSH CConfigDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT); 
	//pDC->SetTextColor( ((CMainWnd*)m_pMain)->m_txtcolor); 
	
	if(nCtlColor == CTLCOLOR_STATIC || nCtlColor == CTLCOLOR_BTN  )
	{
		if(pWnd->GetDlgCtrlID() == IDC_STATICNUM)
			pDC->SetTextColor( RGB(0, 0, 255)); 
		
		if (pWnd->GetDlgCtrlID() == IDC_CHECK4 || pWnd->GetDlgCtrlID() == IDC_CHECK5 || pWnd->GetDlgCtrlID() == IDC_CHECK6)
		{
			pDC->SetTextColor(RGB(0, 0, 255));
		}
		
		hbr = CreateSolidBrush(0xFFFFFF);
	}

	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


void CConfigDlg::SetSpinDeltaValue(NM_UPDOWN* pNMUD, CEdit* pEdit, int iunit)
{
	if(!pEdit) return;

	CString text;
	double	value, delta;

	pEdit->GetWindowText(text);
	value = atof(text);
	switch (iunit)
	{
		case 1:
		case 0:	delta = 1;	break;		// 호가단위 계산 필요
		default:	delta = 0.01;	break;
	}

	delta = (pNMUD->iDelta > 0) ? -delta : delta;
	value += delta;
	value = max(0, value);

// 	CString s;
// 	s.Format("SPIN1 : [%f]\n",value);
// 	OutputDebugString(s);

	if(iunit == 2 || iunit == 3)
	{
		text.Format("%.2f", value);
	}
	else
	{
		text.Format("%d", (int)value);
	}

// 	s.Format("SPIN2 : [%s]\n",text);
// 	OutputDebugString(s);

	pEdit->SetWindowText(text);	
}


//이익목표
void CConfigDlg::OnDeltaposSs2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	SetSpinDeltaValue(pNMUpDown, (CEdit*)GetDlgItem(IDC_EDIT2), ((CComboBox *)GetDlgItem(IDC_COMBO2))->GetCurSel() );
	*pResult = 0;
}

//이익보전
void CConfigDlg::OnDeltaposSs3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	SetSpinDeltaValue(pNMUpDown, (CEdit*)GetDlgItem(IDC_EDIT3), ((CComboBox *)GetDlgItem(IDC_COMBO3))->GetCurSel() );
	*pResult = 0;
}
//최고가대비
void CConfigDlg::OnDeltaposSs4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	SetSpinDeltaValue(pNMUpDown, (CEdit*)GetDlgItem(IDC_EDIT4), ((CComboBox *)GetDlgItem(IDC_COMBO4))->GetCurSel() );
	*pResult = 0;
}

//손실목표
void CConfigDlg::OnDeltaposSs5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	SetSpinDeltaValue(pNMUpDown, (CEdit*)GetDlgItem(IDC_EDIT1), ((CComboBox *)GetDlgItem(IDC_COMBO1))->GetCurSel() );
	*pResult = 0;
}
//주문가격
void CConfigDlg::OnDeltaposSs6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	SetSpinDeltaValue(pNMUpDown, (CEdit*)GetDlgItem(IDC_EDIT6), 0);
	*pResult = 0;
}

void CConfigDlg::OnOK() 
{
	m_stConfig.istopexe[0] = GetDlgButtonCheck(IDC_CHECK1);//손실
	m_stConfig.istopexe[1] = GetDlgButtonCheck(IDC_CHECK2);//이익목표
	m_stConfig.istopexe[2] = m_stConfig.istopexe[3] = GetDlgButtonCheck(IDC_CHECK3);//보존

	m_stConfig.igubn = 0;
	for(int i=0; i<4; i++)
	{
		m_stConfig.iunit[i] = GetDlgComboGetCurSel(nUnitCboID[i]); //단위
		m_stConfig.ival[i] = atof( GetDlgWindowText(nEditID[i]) ); //값

// 		CString s;
// 		s.Format("IVAL : %s		%f\n",GetDlgWindowText(nEditID[i]), m_stConfig.ival[i]);
// 		OutputDebugString(s);
		if( GetDlgButtonCheck(nTypeRadioID[i]) ) m_stConfig.igubn = i; //구분:0-지정가, 1-시장가, 2-조건부, 3-최유리
	}

	m_stConfig.ivoltype = ( (CComboBox*)GetDlgItem(IDC_VOLTYPECOMBO) )->GetCurSel();
	int ivolinput = m_stConfig.ivolval = CInt( GetDlgWindowText(IDC_EDIT5) );//수량
	int ipossvol = CInt(m_strSise[8]) ; //가능수량

	if(m_stConfig.ivoltype)
	{
		if( ipossvol < ivolinput)
		{
			MessageBox("수량입력이 잘못되었습니다(입력>가능수량)", MAPTITLE, MB_ICONINFORMATION|MB_OK);
			GetDlgItem(IDC_EDIT5)->SetFocus();
			return;
		}
	}

	m_stConfig.iprctype = GetDlgComboGetCurSel(IDC_PRICECOMBO);//가격타입
	m_stConfig.sprctype = GetDlgWindowText(IDC_PRICECOMBO);
	m_stConfig.iprcval = atof( GetDlgWindowText(IDC_EDIT6) );//가격 

	m_stConfig.iauto[0] = GetDlgButtonCheck(IDC_CHECK4) ;//자동1 
	m_stConfig.iauto[1] = GetDlgButtonCheck(IDC_CHECK5) ;//자동2

	m_stConfig.inow = GetDlgButtonCheck(IDC_CHECK6);

	m_stConfig.type = m_strSise[7];

	CDialog::OnOK();
}

BOOL CConfigDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (LOWORD(wParam))
	{
		case IDC_COMBO1:
		case IDC_COMBO2:
		case IDC_COMBO3:
		case IDC_COMBO4:
			if(CBN_SELCHANGE == HIWORD(wParam))
			{
				
				for(int i=0; i<4; i++)
				{
					if( nUnitCboID[i] == LOWORD(wParam) )
					{
						GetDlgItem(nEditID[i])->SetWindowText("0");
						break;
					}
				}
			}
			break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

CString CConfigDlg::SetDiffValue(CString sDiff)
{
	
	if(sDiff.IsEmpty() ) return sDiff;

	CString s;
	s.Format("DIFF : %s\n",sDiff);
	OutputDebugString(s);
	
	char stype = sDiff.GetAt(0);
	sDiff = sDiff.Right(sDiff.GetLength() - 1);
	
// 	switch(stype)
// 	{
// 		case '1': //상한
// 			m_diffcolor = RGB(255, 0, 0);
// 			m_strDifSign = "↑";
// 			break;
// 		case '2'://상승
// 			m_strDifSign = "▲";
// 			m_diffcolor = RGB(255, 0, 0);
// 			break;
// 			
// 		case '4'://하한
// 			m_diffcolor = RGB(0, 0, 255);
// 			m_strDifSign = "↓";
// 			break;
// 		case '5'://하락
// 			m_diffcolor = RGB(0, 0, 255);
// 			m_strDifSign = "▼";
// 			break;
// 		case '3'://보합
// 		default :
// 			break;
// 	}
	if(stype == '-')
	{
		m_diffcolor = RGB(0, 0, 255);
		m_strDifSign = "▼";
	}
	else
	{
		m_strDifSign = "▲";
		m_diffcolor = RGB(255, 0, 0);
	}

	return sDiff;
}

void CConfigDlg::OnHogaGubnCheck(UINT nID)
{
	BOOL benable = TRUE;
	if( nTypeRadioID[1] == nID || nTypeRadioID[3] == nID ) benable = FALSE;

	GetDlgItem(IDC_PRICECOMBO)->EnableWindow(benable);
	GetDlgItem(IDC_EDIT6)->EnableWindow(benable);
	//GetDlgItem(IDC_SS6)->EnableWindow(benable);
}

void CConfigDlg::OnSelchangePricecombo() 
{
	// TODO: Add your control notification handler code here
	int icbocnt = ( (CComboBox*)GetDlgItem(IDC_PRICECOMBO) )->GetCount();
	int isel = ( (CComboBox*)GetDlgItem(IDC_PRICECOMBO) )->GetCurSel();
	
	if( isel == icbocnt-1)
	{
		GetDlgItem(IDC_EDIT6)->SetWindowText("0.00");
		GetDlgItem(IDC_STATICPUNIT)->SetWindowText("원");
	}
	else 
	{
		GetDlgItem(IDC_EDIT6)->SetWindowText("0");
		GetDlgItem(IDC_STATICPUNIT)->SetWindowText("틱");
	}
}

void CConfigDlg::OnClear() 
{
	SetInitControl();
	
}
