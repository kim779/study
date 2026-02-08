// DlgSetGuideLine.cpp : implementation file
//

#include "stdafx.h"
//#include "ChartGuideLineaddin.h"
#include "DlgSetGuideLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CConfigSet g_clConfigSet;
#define BACK_COLOR RGB(234,235,238)
/////////////////////////////////////////////////////////////////////////////
// CDlgSetGuideLine dialog
CDlgSetGuideLine::CDlgSetGuideLine(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetGuideLine::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetGuideLine)	
	m_bNotDrawLine = FALSE;
	//}}AFX_DATA_INIT
	m_strCfgFile	= _T("");
	m_strFullAcct	= _T("");
	m_clConfigSet.InitMember();

	m_brGDrawColor.CreateSolidBrush(BACK_COLOR);	
}


void CDlgSetGuideLine::DoDataExchange(CDataExchange* pDX)
{
	//DDX_Check(pDX, IDC_CHECK_GLINEDRAW, m_bNotDrawLine);
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetGuideLine)	
	DDX_Check(pDX,	 IDC_CHECK_GLINEDRAW			,		m_bNotDrawLine);
	DDX_Control(pDX, IDC_CMB_ACCT, m_cmbAcct);
	DDX_Control(pDX, IDC_EDIT_1YP					 ,		m_edt1YP);
	DDX_Control(pDX, IDC_EDIT_2YP					 ,		m_edt2YP);
	DDX_Control(pDX, IDC_EDIT_3YP					 ,		m_edt3YP);
	DDX_Control(pDX, IDC_EDIT_4YP					 ,		m_edt4YP);	
	DDX_Control(pDX, IDC_EDIT_PLUS_RATIO			 ,		m_edtPmmPD);
	DDX_Control(pDX, IDC_EDIT_MINUS_RATIO			 ,		m_edtPmmMD);
	DDX_Check(pDX, IDC_CHECK_Y_SIGA				   ,      m_bChkYSprice);
	DDX_Check(pDX, IDC_CHECK_Y_GOGA				   ,      m_bChkYHprice);
	DDX_Check(pDX, IDC_CHECK_Y_JEOGA               ,      m_bChkYLprice);
	DDX_Check(pDX, IDC_CHECK_Y_ZONGA               ,	  m_bChkYEprice);
	DDX_Check(pDX, IDC_CHECK_Y_SGJ_D2              ,      m_bChkYSHL);
	DDX_Check(pDX, IDC_CHECK_T_SIGA                ,      m_bChkTSprice);
	DDX_Check(pDX, IDC_CHECK_T_GOGA                ,      m_bChkTHprice);
	DDX_Check(pDX, IDC_CHECK_T_JEOGA               ,      m_bChkTLprice);
	DDX_Check(pDX, IDC_CHECK_T_ZONGA               ,      m_bChkTEprice);
	DDX_Check(pDX, IDC_CHECK_T_GJ_D2               ,      m_bChkTHL);
	DDX_Check(pDX, IDC_CHECK_T_SGJ_D2              ,      m_bChkTSHL);
	DDX_Check(pDX, IDC_CHECK_T_GJZ_D4              ,      m_bChkTHLE);
	DDX_Check(pDX, IDC_CHECK_HIGH_LIMIT            ,      m_bChkHighLimit);
	DDX_Check(pDX, IDC_CHECK_LOW_LIMIT             ,      m_bChkLowLimit);
	DDX_Check(pDX, IDC_CHECK_2_RSST                ,      m_bChkPivot2Rst);
	DDX_Check(pDX, IDC_CHECK_1_RSST                ,      m_bChkPivot1Rst);
	DDX_Check(pDX, IDC_CHECK_PIVOT_GLINE           ,      m_bChkPivotGLine);
	DDX_Check(pDX, IDC_CHECK_1_GG                  ,      m_bChkPivot1GG);
	DDX_Check(pDX, IDC_CHECK_2_GG                  ,      m_bChkPivot2GG);
	DDX_Check(pDX, IDC_CHECK_PREGOGA               ,      m_bChkDMKPreHPrice);
	DDX_Check(pDX, IDC_CHECK_DMK_GLINE             ,      m_bChkDMKGLine);
	DDX_Check(pDX, IDC_CHECK_PREJEOGA              ,      m_bChkDMKPreLPrice);
	DDX_Check(pDX, IDC_CHECK_1YP                   ,      m_bChk2Pyung1);
	DDX_Check(pDX, IDC_CHECK_2YP                   ,      m_bChk2Pyung2);
	DDX_Check(pDX, IDC_CHECK_3YP                   ,      m_bChk2Pyung3);
	DDX_Check(pDX, IDC_CHECK_4YP                   ,      m_bChk2Pyung4);
	DDX_Check(pDX, IDC_CHECK_PMD                   ,      m_bChkPmmDanga);
	DDX_Check(pDX, IDC_CHECK_PMPD                  ,      m_bChkPmmPDanga);
	DDX_Check(pDX, IDC_CHECK_PMMD                  ,      m_bChkPmmMDanga);	
	DDX_Control(pDX, IDC_BTNCLR_Y_SIGA               ,		m_btnYSprice);
	DDX_Control(pDX, IDC_BTNCLR_Y_GOGA               ,      m_btnYHprice);
	DDX_Control(pDX, IDC_BTNCLR_Y_JEOGA              ,      m_btnYLprice);
	DDX_Control(pDX, IDC_BTNCLR_Y_ZONGA              ,		m_btnYEprice);
	DDX_Control(pDX, IDC_BTNCLR_Y_SGJ_D2             ,      m_btnYSHL);
	DDX_Control(pDX, IDC_BTNCLR_T_SIGA               ,      m_btnTSprice);
	DDX_Control(pDX, IDC_BTNCLR_T_GOGA               ,      m_btnTHprice);
	DDX_Control(pDX, IDC_BTNCLR_T_JEOGA              ,      m_btnTLprice);
	DDX_Control(pDX, IDC_BTNCLR_T_ZONGA              ,      m_btnTEprice);
	DDX_Control(pDX, IDC_BTNCLR_T_GJ_D2              ,      m_btnTHL);
	DDX_Control(pDX, IDC_BTNCLR_T_SGJ_D2             ,      m_btnTSHL);
	DDX_Control(pDX, IDC_BTNCLR_T_GJZ_D4             ,      m_btnTHLE);
	DDX_Control(pDX, IDC_BTNCLR_HIGH_LIMIT           ,      m_btnHighLimit);
	DDX_Control(pDX, IDC_BTNCLR_LOW_LIMIT            ,      m_btnLowLimit);
	DDX_Control(pDX, IDC_BTNCLR_2_RSST               ,      m_btnPivot2Rst);
	DDX_Control(pDX, IDC_BTNCLR_1_RSST               ,      m_btnPivot1Rst);
	DDX_Control(pDX, IDC_BTNCLR_PIVOT_GLINE          ,      m_btnPivotGLine);
	DDX_Control(pDX, IDC_BTNCLR_1_GG                 ,      m_btnPivot1GG);
	DDX_Control(pDX, IDC_BTNCLR_2_GG                 ,      m_btnPivot2GG);
	DDX_Control(pDX, IDC_BTNCLR_PREGOGA              ,      m_btnDMKPreHPrice);
	DDX_Control(pDX, IDC_BTNCLR_DMK_GLINE            ,      m_btnDMKGLine);
	DDX_Control(pDX, IDC_BTNCLR_PREJEOGA             ,      m_btnDMKPreLPrice);
	DDX_Control(pDX, IDC_BTNCLR_1YP                  ,      m_btn2Pyung1);
	DDX_Control(pDX, IDC_BTNCLR_2YP                  ,      m_btn2Pyung2);
	DDX_Control(pDX, IDC_BTNCLR_3YP                  ,      m_btn2Pyung3);
	DDX_Control(pDX, IDC_BTNCLR_4YP                  ,      m_btn2Pyung4);
	DDX_Control(pDX, IDC_BTNCLR_PMMD                 ,      m_btnPmmDanga);
	DDX_Control(pDX, IDC_BTNCLR_PMMD2                ,      m_btnPmmPDanga);
	DDX_Control(pDX, IDC_BTNCLR_PMMD3                ,      m_btnPmmMDanga);
	DDX_Control(pDX, IDC_COMBO_Y_SIGA                ,      m_cmbYSprice);
	DDX_Control(pDX, IDC_COMBO_Y_GOGA                ,      m_cmbYHprice);
	DDX_Control(pDX, IDC_COMBO_Y_JEOGA               ,      m_cmbYLprice);
	DDX_Control(pDX, IDC_COMBO_Y_ZONGA               ,      m_cmbYEprice);
	DDX_Control(pDX, IDC_COMBO_Y_SGJ_D2              ,      m_cmbYSHL);
	DDX_Control(pDX, IDC_COMBO_T_SIGA                ,      m_cmbTSprice);
	DDX_Control(pDX, IDC_COMBO_T_GOGA                ,      m_cmbTHprice);
	DDX_Control(pDX, IDC_COMBO_T_JEOGA               ,      m_cmbTLprice);
	DDX_Control(pDX, IDC_COMBO_T_ZONGA               ,      m_cmbTEprice);
	DDX_Control(pDX, IDC_COMBO_T_GJ_D2               ,      m_cmbTHL);
	DDX_Control(pDX, IDC_COMBO_T_SGJ_D2              ,      m_cmbTSHL);
	DDX_Control(pDX, IDC_COMBO_T_GJZ_D4              ,      m_cmbTHLE);
	DDX_Control(pDX, IDC_COMBO_HIGH_LIMIT            ,      m_cmbHighLimit);
	DDX_Control(pDX, IDC_COMBO_LOW_LIMIT             ,      m_cmbLowLimit);
	DDX_Control(pDX, IDC_COMBO_2_RSST                ,      m_cmbPivot2Rst);
	DDX_Control(pDX, IDC_COMBO_1_RSST                ,      m_cmbPivot1Rst);
	DDX_Control(pDX, IDC_COMBO_PIVOT_GLINE           ,      m_cmbPivotGLine);
	DDX_Control(pDX, IDC_COMBO_1_GG                  ,      m_cmbPivot1GG);
	DDX_Control(pDX, IDC_COMBO_2_GG                  ,      m_cmbPivot2GG);
	DDX_Control(pDX, IDC_COMBO_PREGOGA               ,      m_cmbDMKPreHPrice);
	DDX_Control(pDX, IDC_COMBO_DMKGLINE              ,      m_cmbDMKGLine);
	DDX_Control(pDX, IDC_COMBO_PREJEOGA              ,      m_cmbDMKPreLPrice);
	DDX_Control(pDX, IDC_COMBO_1YP                   ,      m_cmb2Pyung1);
	DDX_Control(pDX, IDC_COMBO_2YP                   ,      m_cmb2Pyung2);
	DDX_Control(pDX, IDC_COMBO_3YP                   ,      m_cmb2Pyung3);
	DDX_Control(pDX, IDC_COMBO_4YP                   ,      m_cmb2Pyung4);
	DDX_Control(pDX, IDC_COMBO_PMMD                  ,      m_cmbPmmDanga);
	DDX_Control(pDX, IDC_COMBO_PMM_PD                ,      m_cmbPmmPDanga);
	DDX_Control(pDX, IDC_COMBO_PMMD_MD               ,      m_cmbPmmMDanga);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetGuideLine, CDialog)
	//{{AFX_MSG_MAP(CDlgSetGuideLine)		
	ON_COMMAND_RANGE(IDC_BTNCLR_Y_GOGA, IDC_BTNCLR_Y_SIGA, OnBtnColor)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	//ON_COMMAND_RANGE(IDC_BTNCLR_Y_GOGA, IDC_BTNCLR_Y_SIGA, OnBtnColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetGuideLine message handlers

void CDlgSetGuideLine::Initial_ThicknessComboBox()
{

	UINT nID[] = {			
			IDC_COMBO_Y_SIGA                ,
			IDC_COMBO_Y_GOGA                ,
			IDC_COMBO_Y_JEOGA               ,
			IDC_COMBO_Y_ZONGA               ,
			IDC_COMBO_Y_SGJ_D2              ,
			IDC_COMBO_T_SIGA                ,
			IDC_COMBO_T_GOGA                ,
			IDC_COMBO_T_JEOGA               ,
			IDC_COMBO_T_ZONGA               ,
			IDC_COMBO_T_GJ_D2               ,
			IDC_COMBO_T_SGJ_D2              ,
			IDC_COMBO_T_GJZ_D4              ,
			IDC_COMBO_HIGH_LIMIT            ,
			IDC_COMBO_LOW_LIMIT             ,
			IDC_COMBO_2_RSST                ,
			IDC_COMBO_1_RSST                ,
			IDC_COMBO_PIVOT_GLINE           ,
			IDC_COMBO_1_GG                  ,
			IDC_COMBO_2_GG                  ,
			IDC_COMBO_PREGOGA               ,
			IDC_COMBO_DMKGLINE              ,
			IDC_COMBO_PREJEOGA              ,
			IDC_COMBO_1YP                   ,
			IDC_COMBO_2YP                   ,
			IDC_COMBO_3YP                   ,
			IDC_COMBO_4YP                   ,
			IDC_COMBO_PMMD                  ,
			IDC_COMBO_PMM_PD                ,
			IDC_COMBO_PMMD_MD               
	};      
	
	int nCnt = sizeof(nID)/sizeof(UINT);     
	CWnd* pWnd=NULL;                         
	
	for(int i=0; i<nCnt; i++)                
	{                                        
		pWnd = GetDlgItem(nID[i]);     
		if(pWnd)    
		{
			CBitmapItemComboBox* pBmpCombo = ((CBitmapItemComboBox*)pWnd);
			pBmpCombo->m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);
			AddLineType(pBmpCombo, 5);
			
			for (int i= -1 ;i < pBmpCombo->GetCount();i++) 
				pBmpCombo->SetItemHeight(i, 14);

			pBmpCombo->SetCurSel(0);			
		}
	}

	//m_btnColor.SetColor(RGB(255,0,0));
	//	m_btnColor.SetBtnText("색 설정");
	//	m_btnColor.Invalidate();		
	



	
//	//선형이고, static 아이디가 들어오지 않았을때는 그냥 나간다.
//	if(idStatic == 0 || pSubGraphData->GetType() == CGraphBaseData::Line_Type)
//		return;
//	
//	CStatic* pStatic = (CStatic* )GetDlgItem(idStatic);
//	pStatic->ShowWindow(SW_HIDE);
//	Thickness_Combo.ShowWindow(SW_HIDE);

}

void CDlgSetGuideLine::AddLineType(CComboBox* pCombo, int Type)
{
	ASSERT(pCombo != NULL);
	pCombo->ResetContent();
	
	for (int i = 0; i < Type; i++)
		pCombo->AddString(szType[i]);
}

BOOL CDlgSetGuideLine::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//{{{계좌 콤보 초기화
	Initial_AcctCombo();
	//{{{색 지정 버튼 초기화
	Initial_ColorBtn();
	//{{{콤보 박스들 초기화
	Initial_ThicknessComboBox();
	//{{{체크 박스 초기화
	Initial_ChkBox();
	
	//{{{화면 설정 기본 파일이 존재하지 않는 경우 기본 환경파일을 만든다.
	CreateEnvSetupFile();
	//{{{화면 설정 내용을 읽어 화면에 표시 한다.
	LoadCfgFile();	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSetGuideLine::Initial_ColorBtn()
{
	UINT nID[] = {			
			IDC_BTNCLR_Y_GOGA               ,
			IDC_BTNCLR_Y_JEOGA              ,
			IDC_BTNCLR_Y_ZONGA              ,
			IDC_BTNCLR_Y_SGJ_D2             ,
			IDC_BTNCLR_T_SIGA               ,
			IDC_BTNCLR_T_GOGA               ,
			IDC_BTNCLR_T_JEOGA              ,
			IDC_BTNCLR_T_ZONGA              ,
			IDC_BTNCLR_T_GJ_D2              ,
			IDC_BTNCLR_T_SGJ_D2             ,
			IDC_BTNCLR_T_GJZ_D4             ,
			IDC_BTNCLR_HIGH_LIMIT           ,
			IDC_BTNCLR_LOW_LIMIT            ,
			IDC_BTNCLR_PMMD                 ,
			IDC_BTNCLR_2_RSST               ,
			IDC_BTNCLR_1_RSST               ,
			IDC_BTNCLR_PIVOT_GLINE          ,
			IDC_BTNCLR_1_GG                 ,
			IDC_BTNCLR_2_GG                 ,
			IDC_BTNCLR_PREGOGA              ,
			IDC_BTNCLR_DMK_GLINE            ,
			IDC_BTNCLR_PREJEOGA             ,
			IDC_BTNCLR_PMMD2                ,
			IDC_BTNCLR_PMMD3                ,
			IDC_BTNCLR_1YP                  ,
			IDC_BTNCLR_2YP                  ,
			IDC_BTNCLR_3YP                  ,
			IDC_BTNCLR_4YP                  ,
			IDC_BTNCLR_Y_SIGA               
			
	};      
	
	int nCnt = sizeof(nID)/sizeof(UINT);     
	CWnd* pWnd=NULL;                         
	
	for(int i=0; i<nCnt; i++)                
	{                                        
		pWnd = GetDlgItem(nID[i]);     
		if(pWnd)    
		{
			((CColorButton*)pWnd)->SetColor(DEFAULT_LINE_CLR);
		}
	}
	
	COLORREF clr = DEFAULT_LINE_CLR;
}

void CDlgSetGuideLine::OnBtnColor(UINT nID)
{		
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(nID);     
	if(pWnd)    
	{
		CColorButton* pClrButton = ((CColorButton*)pWnd);
		COLORREF crColor = pClrButton->GetColor();
		CColorDialog color(crColor, CC_FULLOPEN);
		if(color.DoModal() == IDOK) 
		{
			pClrButton->SetColor(color.GetColor());				
			pClrButton->Invalidate();
		}
	}
}

/*
void CDlgSetGuideLine::fnSaveConfig()
{	
	UpdateData();	
	
	CString szINI, strData = "", strKey = "";
	szINI.Format("%s%s", m_szRootPath, INI_FILENAME);
	
	strData.Format("%d",m_bOneClickOrder);
	WritePrivateProfileString("CONFIG", "OneClick Order", strData, szINI);
	strData.Format("%d",m_bOrderConfirm);
	WritePrivateProfileString("CONFIG", "Order Confirm", strData, szINI);
	strData.Format("%d",m_bJangoAuto);
	WritePrivateProfileString("CONFIG", "JangoAuto", strData, szINI); // 잔고반영여부를 옵션화 20040922 LYH 추가
	strData.Format("%d",m_bSpinDelta);
	WritePrivateProfileString("CONFIG", "SpinDelta", strData, szINI); // 주문수량 증감단위 변경 여부 20040930 LYH 추가
	m_ctlSpinDeltaNum.GetWindowText(strData);
	WritePrivateProfileString("CONFIG", "SpinDeltaNum", strData, szINI); // 주문수량 증감단위 20040930 LYH 추가
	strData.Format("%d",m_bMultipleSet);
	WritePrivateProfileString("CONFIG", "MultipleSet", strData, szINI); // 주문수량 배수로 셋팅 여부 20041011 LYH 추가
	
	m_edtPlusMinusTick.GetWindowText(strData);
	WritePrivateProfileString("CONFIG", "Order Tick",  strData, szINI); // +, - 버튼 클릭시 주문수량 증감 단위 20041123 ITC 추가
	
	strData.Format("%d",m_bCheckOffer1Sort);
	WritePrivateProfileString("CONFIG", "Offer1 Sort", strData, szINI); // 매도1호가 정렬 여부
	
	
	//########################################################################################
	int row, col, cnt = 0;
	for(row = 0; row < 2; row ++)
	{
		for(col = 0; col < 5; col++)
		{
			strKey.Format("QtyInfo%d",cnt);
			strData.Format("%s", m_pGridQty->GetItemText(row, col));
			strData.Remove(',');
			WritePrivateProfileString("CONFIG", strKey, strData, szINI);
			cnt++;
		}
	}
	//########################################################################################
	
}

*/
void CDlgSetGuideLine::OnOK() 
{	
	SaveCfgFile();

	CDialog::OnOK();
}

//{{{차트 사용자 지정 기준서 그리기 환경설정파일 생성}}}
void CDlgSetGuideLine::CreateEnvSetupFile()
{
	CFile pfile ;
	CString strTemp;
	//extern CConfigSet g_clConfigSet;
	extern CString	  g_strConfig;
	
	if( !pfile.Open(m_strCfgFile, CFile::modeRead) )
	{
		
		g_strConfig.TrimRight();
		int nLen = g_strConfig.GetLength();
		
		pfile.Open(  m_strCfgFile, CFile::modeCreate | CFile::modeWrite );
		pfile.Write( (LPTSTR)(LPCTSTR)g_strConfig, nLen);
		pfile.Close();
	}
	else
	{
		CString		strData;
		char		szData[16];
		int			nPos, nPos2;
		
		pfile.Read(szData, 16);
		pfile.Close();
		
		strData = LPCTSTR(szData);
		strData.TrimRight();
		
		nPos  = strData.Find("FILEVER");
		if(nPos >= 0)
		{
			nPos  = strData.Find("=", nPos);
			nPos2 = strData.Find(";", nPos);
			if (nPos2 > nPos+1)
				strTemp = strData.Mid(nPos+1, nPos2-nPos-1);
			else
				strTemp = "";
		}
		else
		{
			strTemp = "";
		}
		
		// 파일 버전이 같지 않은 경우 INI파일 디폴트로 생성		
		/*if (strTemp != g_strConfig)
		{	
			int nLen = strTemp.GetLength();
			pfile.Open( m_strCfgFile, CFile::modeCreate | CFile::modeWrite );
			pfile.Write( g_strConfig, nLen);
			pfile.Close();
		}*/
	}	
}

BOOL CDlgSetGuideLine::LoadCfgFile()
{
	// 파일 읽어오기 ***************************************************************************
	CFile		pfile ;
	CString		strData, strTemp, strAcct;
	char		szData[2096];
	int			nCount;
	int			nPos, nPos2;
/*	for(int nNum = 0; nNum < NUMBER_COUNT; nNum++)
	{
		m_ArrayNumberCount[nNum] = 0;
		m_ArrayPriceCount[nNum]	 = 0;
	}
*/	
	if( pfile.Open(m_strCfgFile, CFile::modeRead) )
	{
		pfile.Read(szData, 2096);
		pfile.Close();

		strData = LPCTSTR(szData);
		strData.TrimLeft();		strData.TrimRight();

		// 환경파일 변수값 로드 ----------------------------------------------------------------->
		char*	szDataName[] = {	"CHK00","CHK01","CHK02","CHK03","CHK04",   
									"CHK05","CHK06","CHK07","CHK08","CHK09",   //10
									"CHK10","CHK11","CHK12","CHK13","CHK14",   
									"CHK15","CHK16","CHK17","CHK18","CHK19",   //20
									"CHK20","CHK21","CHK22","CHK23","CHK24",   
									"CHK25","CHK26","CHK27","CHK28","CLR00",   //30
									"CLR01","CLR02","CLR03","CLR04", "CLR05",  
									"CLR06","CLR07","CLR08","CLR09", "CLR10",  //40
									"CLR11","CLR12","CLR13","CLR14", "CLR15",  
									"CLR16","CLR17","CLR18","CLR19", "CLR20",  //50
									"CLR21","CLR22","CLR23","CLR24", "CLR25",  
									"CLR26","CLR27","CLR28","LN00" , "LN01" ,  //60
									"LN02" ,"LN03" ,"LN04" ,"LN05" , "LN06" ,
									"LN07" ,"LN08" ,"LN09" ,"LN10" , "LN11" ,  //70
									"LN12" ,"LN13" ,"LN14" ,"LN15" , "LN16" ,
									"LN17" ,"LN18" ,"LN19" ,"LN20" , "LN21" ,  //80	
									"LN22" ,"LN23" ,"LN24" ,"LN25" , "LN26" ,
									"LN27" ,"LN28" ,"YP00" ,"YP01" , "YP02" ,  //90
									"YP03" ,"DAN00","DAN01","ACCT00","NOT_DRAW"	   //95
							};
		
		nCount = sizeof(szDataName)/sizeof(char*);		
		CString sTTT;
		for(int i = 0; i < nCount; i++)
		{
			nPos  = strData.Find(szDataName[i]);
			if(nPos >= 0)
			{
				nPos  = strData.Find("=", nPos);
				nPos2 = strData.Find(";", nPos);
				if (nPos2 > nPos+1)
					strTemp = strData.Mid(nPos+1, nPos2-nPos-1);
				else
					strTemp = "";
			}
			else
			{
				strTemp = "";
			}

//			sTTT.Format("Cnt :%d - %s", i, strTemp);
//			OutputDebugString(sTTT);

			int nValue = atoi(strTemp);		
			switch(i)
			{				
				case  0: { m_clConfigSet.m_bChkYSprice		= m_bChkYSprice		= nValue; } break;//전일가격 시가				체크 여부							
				case  1: { m_clConfigSet.m_bChkYHprice		= m_bChkYHprice		= nValue; } break;//전일가격 고가				체크 여부
				case  2: { m_clConfigSet.m_bChkYLprice		= m_bChkYLprice		= nValue; } break;//전일가격 저가				체크 여부
				case  3: { m_clConfigSet.m_bChkYEprice		= m_bChkYEprice		= nValue; } break;//전일가격 종가				체크 여부
				case  4: { m_clConfigSet.m_bChkYSHL			= m_bChkYSHL		= nValue; } break;//전일가격 (시+고+저)/3		체크 여부
				case  5: { m_clConfigSet.m_bChkTSprice		= m_bChkTSprice		= nValue; } break;//당일가격 시가				체크 여부
				case  6: { m_clConfigSet.m_bChkTHprice		= m_bChkTHprice		= nValue; } break;//당일가격 고가				체크 여부
				case  7: { m_clConfigSet.m_bChkTLprice		= m_bChkTLprice		= nValue; } break;//당일가격 저가				체크 여부
				case  8: { m_clConfigSet.m_bChkTEprice		= m_bChkTEprice		= nValue; } break;//당일가격 종가				체크 여부
				case  9: { m_clConfigSet.m_bChkTHL			= m_bChkTHL			= nValue; } break;//당일가격 (고+저)/2			체크 여부
				case 10: { m_clConfigSet.m_bChkTSHL			= m_bChkTSHL		= nValue; } break;//당일가격 (시+고+저)/3		체크 여부
				case 11: { m_clConfigSet.m_bChkTHLE			= m_bChkTHLE		= nValue; } break;//당일가격 (고+저+(종*2))/4	체크 여부					
				case 12: { m_clConfigSet.m_bChkHighLimit	= m_bChkHighLimit	= nValue; } break;//상한가						체크 여부
				case 13: { m_clConfigSet.m_bChkLowLimit		= m_bChkLowLimit	= nValue; } break;//하한가						체크 여부				
				case 14: { m_clConfigSet.m_bChkPivot2Rst	= m_bChkPivot2Rst	= nValue; } break;// 피봇 2차 저항				체크 여부 
				case 15: { m_clConfigSet.m_bChkPivot1Rst	= m_bChkPivot1Rst	= nValue; } break;// 피봇 1차 저항				체크 여부
				case 16: { m_clConfigSet.m_bChkPivotGLine	= m_bChkPivotGLine	= nValue; } break;// 피봇 기준선				체크 여부
				case 17: { m_clConfigSet.m_bChkPivot1GG		= m_bChkPivot1GG	= nValue; } break;// 피봇 1차 지지				체크 여부
				case 18: { m_clConfigSet.m_bChkPivot2GG		= m_bChkPivot2GG	= nValue; } break;// 피봇 2차 지지				체크 여부				
				case 19: { m_clConfigSet.m_bChkDMKPreHPrice = m_bChkDMKPreHPrice = nValue;} break;// Demark 예상고가			체크 여부
				case 20: { m_clConfigSet.m_bChkDMKGLine		= m_bChkDMKGLine	 = nValue;} break;// Demark 기준선				체크 여부
				case 21: { m_clConfigSet.m_bChkDMKPreLPrice = m_bChkDMKPreLPrice = nValue;} break;// Demark 예상저가			체크 여부				
				case 22: { m_clConfigSet.m_bChk2Pyung1		= m_bChk2Pyung1		= nValue; } break;// 이동평균 첫번째 입력값		체크 여부
				case 23: { m_clConfigSet.m_bChk2Pyung2		= m_bChk2Pyung2		= nValue; } break;// 이동평균 두번째 입력값		체크 여부
				case 24: { m_clConfigSet.m_bChk2Pyung3		= m_bChk2Pyung3		= nValue; } break;// 이동평균 세번째 입력값		체크 여부
				case 25: { m_clConfigSet.m_bChk2Pyung4		= m_bChk2Pyung4		= nValue; } break;// 이동평균 네번째 입력값		체크 여부									
				case 26: { m_clConfigSet.m_bChkPmmDanga		= m_bChkPmmDanga	= nValue; } break;// 평균매입 단가				체크 여부
				case 27: { m_clConfigSet.m_bChkPmmPDanga	= m_bChkPmmPDanga	= nValue; } break;// 평균매입 단가 +n%			체크 여부
				case 28: { m_clConfigSet.m_bChkPmmMDanga	= m_bChkPmmMDanga	= nValue; } break;// 평균매입 단가 -n%			체크 여부				

				case 29: { m_clConfigSet.m_clrYSprice		= nValue;	m_btnYSprice		.SetColor((const COLORREF)nValue); } break;	//전일가격 시가				색 지정값
				case 30: { m_clConfigSet.m_clrYHprice		= nValue;	m_btnYHprice		.SetColor((const COLORREF)nValue); } break;	//전일가격 고가				색 지정값
				case 31: { m_clConfigSet.m_clrYLprice		= nValue;	m_btnYLprice		.SetColor((const COLORREF)nValue); } break;	//전일가격 저가				색 지정값
				case 32: { m_clConfigSet.m_clrYEprice		= nValue;	m_btnYEprice		.SetColor((const COLORREF)nValue); } break;	//전일가격 종가				색 지정값
				case 33: { m_clConfigSet.m_clrYSHL			= nValue;	m_btnYSHL			.SetColor((const COLORREF)nValue); } break;	//전일가격 (시+고+저)/3		색 지정값
				case 34: { m_clConfigSet.m_clrTSprice		= nValue;	m_btnTSprice		.SetColor((const COLORREF)nValue); } break;	//당일가격 시가				색 지정값
				case 35: { m_clConfigSet.m_clrTHprice		= nValue;	m_btnTHprice		.SetColor((const COLORREF)nValue); } break;	//당일가격 고가				색 지정값
				case 36: { m_clConfigSet.m_clrTLprice		= nValue;	m_btnTLprice		.SetColor((const COLORREF)nValue); } break;	//당일가격 저가				색 지정값
				case 37: { m_clConfigSet.m_clrTEprice		= nValue;	m_btnTEprice		.SetColor((const COLORREF)nValue); } break;	//당일가격 종가				색 지정값
				case 38: { m_clConfigSet.m_clrTHL			= nValue;	m_btnTHL			.SetColor((const COLORREF)nValue); } break;	//당일가격 (고+저)/2		색 지정값
				case 39: { m_clConfigSet.m_clrTSHL			= nValue;	m_btnTSHL			.SetColor((const COLORREF)nValue); } break;	//당일가격 (시+고+저)/3		색 지정값
				case 40: { m_clConfigSet.m_clrTHLE			= nValue;	m_btnTHLE			.SetColor((const COLORREF)nValue); } break;	//당일가격 (고+저+(종*2))/4 색 지정값
				case 41: { m_clConfigSet.m_clrHighLimit		= nValue;	m_btnHighLimit		.SetColor((const COLORREF)nValue); } break;	//상한가					색 지정값
				case 42: { m_clConfigSet.m_clrLowLimit		= nValue;	m_btnLowLimit		.SetColor((const COLORREF)nValue); } break;	//하한가					색 지정값
				case 43: { m_clConfigSet.m_clrPivot2Rst		= nValue;	m_btnPivot2Rst		.SetColor((const COLORREF)nValue); } break;	// 피봇 2차 저항 색 지정값
				case 44: { m_clConfigSet.m_clrPivot1Rst		= nValue;	m_btnPivot1Rst		.SetColor((const COLORREF)nValue); } break;	// 피봇 1차 저항 색 지정값
				case 45: { m_clConfigSet.m_clrPivotGLine	= nValue;	m_btnPivotGLine		.SetColor((const COLORREF)nValue); } break;	// 피봇 기준선	 색 지정값
				case 46: { m_clConfigSet.m_clrPivot1GG		= nValue;	m_btnPivot1GG		.SetColor((const COLORREF)nValue); } break;	// 피봇 1차 지지 색 지정값
				case 47: { m_clConfigSet.m_clrPivot2GG		= nValue;	m_btnPivot2GG		.SetColor((const COLORREF)nValue); } break;	// 피봇 2차 지지 색 지정값		
				case 48: { m_clConfigSet.m_clrDMKPreHPrice	= nValue;	m_btnDMKPreHPrice	.SetColor((const COLORREF)nValue); } break;	// Demark 예상고가  색 지정값
				case 49: { m_clConfigSet.m_clrDMKGLine		= nValue;	m_btnDMKGLine		.SetColor((const COLORREF)nValue); } break;	// Demark 기준선	색 지정값
				case 50: { m_clConfigSet.m_clrDMKPreLPrice	= nValue;	m_btnDMKPreLPrice	.SetColor((const COLORREF)nValue); } break;	// Demark 예상저가  색 지정값	
				case 51: { m_clConfigSet.m_clr2Pyung1		= nValue;	m_btn2Pyung1		.SetColor((const COLORREF)nValue); } break;	// 이동평균 첫번째 입력값 색 지정값
				case 52: { m_clConfigSet.m_clr2Pyung2		= nValue;	m_btn2Pyung2		.SetColor((const COLORREF)nValue); } break;	// 이동평균 두번째 입력값 색 지정값
				case 53: { m_clConfigSet.m_clr2Pyung3		= nValue;	m_btn2Pyung3		.SetColor((const COLORREF)nValue); } break;	// 이동평균 세번째 입력값 색 지정값
				case 54: { m_clConfigSet.m_clr2Pyung4		= nValue;	m_btn2Pyung4		.SetColor((const COLORREF)nValue); } break;	// 이동평균 네번째 입력값 색 지정값							
				case 55: { m_clConfigSet.m_clrPmmDanga		= nValue;	m_btnPmmDanga		.SetColor((const COLORREF)nValue); } break;	// 평균매입 단가 색 지정값
				case 56: { m_clConfigSet.m_clrPmmPDanga		= nValue;	m_btnPmmPDanga		.SetColor((const COLORREF)nValue); } break;	// 평균매입 단가 +n% 색 지정값
				case 57: { m_clConfigSet.m_clrPmmMDanga		= nValue;	m_btnPmmMDanga		.SetColor((const COLORREF)nValue); } break;	// 평균매입 단가 -n% 색 지정값

				case 58: { m_clConfigSet.m_nLineYSprice		= nValue;	m_cmbYSprice		.SetCurSel((const)nValue - 1);  } break;	//전일가격 시가				라인 크기
				case 59: { m_clConfigSet.m_nLineYHprice		= nValue;	m_cmbYHprice		.SetCurSel((const)nValue - 1);  } break;	//전일가격 고가				라인 크기
				case 60: { m_clConfigSet.m_nLineYLprice		= nValue;	m_cmbYLprice		.SetCurSel((const)nValue - 1);  } break;	//전일가격 저가				라인 크기
				case 61: { m_clConfigSet.m_nLineYEprice		= nValue;	m_cmbYEprice		.SetCurSel((const)nValue - 1);  } break;	//전일가격 종가				라인 크기
				case 62: { m_clConfigSet.m_nLineYSHL		= nValue;	m_cmbYSHL			.SetCurSel((const)nValue - 1);  } break;	//전일가격 (시+고+저)/3		라인 크기
				case 63: { m_clConfigSet.m_nLineTSprice		= nValue;	m_cmbTSprice		.SetCurSel((const)nValue - 1);  } break;	//당일가격 시가				라인 크기
				case 64: { m_clConfigSet.m_nLineYSprice		= nValue;	m_cmbTHprice		.SetCurSel((const)nValue - 1);  } break;	//당일가격 고가				라인 크기
				case 65: { m_clConfigSet.m_nLineYHprice		= nValue;	m_cmbTLprice		.SetCurSel((const)nValue - 1);  } break;	//당일가격 저가				라인 크기
				case 66: { m_clConfigSet.m_nLineYLprice		= nValue;	m_cmbTEprice		.SetCurSel((const)nValue - 1);  } break;	//당일가격 종가				라인 크기
				case 67: { m_clConfigSet.m_nLineTHL			= nValue;	m_cmbTHL			.SetCurSel((const)nValue - 1);  } break;	//당일가격 (고+저)/2		라인 크기
				case 68: { m_clConfigSet.m_nLineTSHL		= nValue;	m_cmbTSHL			.SetCurSel((const)nValue - 1);  } break;	//당일가격 (시+고+저)/3		라인 크기
				case 69: { m_clConfigSet.m_nLineTHLE		= nValue;	m_cmbTHLE			.SetCurSel((const)nValue - 1);  } break;	//당일가격 (고+저+(종*2))/4 라인 크기		
				case 70: { m_clConfigSet.m_nLineHighLimit	= nValue;	m_cmbHighLimit		.SetCurSel((const)nValue - 1);  } break;	//상한가					라인 크기
				case 71: { m_clConfigSet.m_nLineLowLimit	= nValue;	m_cmbLowLimit		.SetCurSel((const)nValue - 1);  } break;	//하한가					라인 크기		
				case 72: { m_clConfigSet.m_nLinePivot2Rst	= nValue;	m_cmbPivot2Rst		.SetCurSel((const)nValue - 1);  } break;	// 피봇 2차 저항 라인 크기
				case 73: { m_clConfigSet.m_nLinePivot1Rst	= nValue;	m_cmbPivot1Rst		.SetCurSel((const)nValue - 1);  } break;	// 피봇 1차 저항 라인 크기
				case 74: { m_clConfigSet.m_nLinePivotGLine	= nValue;	m_cmbPivotGLine		.SetCurSel((const)nValue - 1);  } break;	// 피봇 기준선	 라인 크기
				case 75: { m_clConfigSet.m_nLinePivot1GG	= nValue;	m_cmbPivot1GG		.SetCurSel((const)nValue - 1);  } break;	// 피봇 1차 지지 라인 크기
				case 76: { m_clConfigSet.m_nLinePivot2GG	= nValue;	m_cmbPivot2GG		.SetCurSel((const)nValue - 1);  } break;	// 피봇 2차 지지 라인 크기		
				case 77: { m_clConfigSet.m_nLineDMKPreHPrice= nValue;	m_cmbDMKPreHPrice	.SetCurSel((const)nValue - 1);  } break;	// Demark 예상고가  라인 크기
				case 78: { m_clConfigSet.m_nLineDMKGLine	= nValue;	m_cmbDMKGLine		.SetCurSel((const)nValue - 1);  } break;	// Demark 기준선	라인 크기
				case 79: { m_clConfigSet.m_nLineDMKPreLPrice= nValue;	m_cmbDMKPreLPrice	.SetCurSel((const)nValue - 1);  } break;	// Demark 예상저가  라인 크기	
				case 80: { m_clConfigSet.m_nLine2Pyung1		= nValue;	m_cmb2Pyung1		.SetCurSel((const)nValue - 1);  } break;	// 이동평균 첫번째 입력값 라인 크기
				case 81: { m_clConfigSet.m_nLine2Pyung2		= nValue;	m_cmb2Pyung2		.SetCurSel((const)nValue - 1);  } break;	// 이동평균 두번째 입력값 라인 크기
				case 82: { m_clConfigSet.m_nLine2Pyung3		= nValue;	m_cmb2Pyung3		.SetCurSel((const)nValue - 1);  } break;	// 이동평균 세번째 입력값 라인 크기
				case 83: { m_clConfigSet.m_nLine2Pyung4		= nValue;	m_cmb2Pyung4		.SetCurSel((const)nValue - 1);  } break;	// 이동평균 네번째 입력값 라인 크기				
				case 84: { m_clConfigSet.m_nLinePmmDanga	= nValue;	m_cmbPmmDanga		.SetCurSel((const)nValue - 1);  } break;	// 평균매입 단가 라인 크기
				case 85: { m_clConfigSet.m_nLinePmmPDanga	= nValue;	m_cmbPmmPDanga		.SetCurSel((const)nValue - 1);  } break;	// 평균매입 단가 +n% 라인 크기
				case 86: { m_clConfigSet.m_nLinePmmMDanga	= nValue;	m_cmbPmmMDanga		.SetCurSel((const)nValue - 1);  } break;	// 평균매입 단가 -n% 라인 크기			

				case 87: { m_clConfigSet.m_nDay2Pyung1		= nValue;	strTemp.Format("%d", nValue); m_edt1YP		.SetWindowText(strTemp); } break;	//이동편균 1번째 		수치(기간) 값
				case 88: { m_clConfigSet.m_nDay2Pyung2		= nValue;	strTemp.Format("%d", nValue); m_edt2YP		.SetWindowText(strTemp); } break;	//이동편균 2번째 		수치(기간) 값
				case 89: { m_clConfigSet.m_nDay2Pyung3		= nValue;	strTemp.Format("%d", nValue); m_edt3YP		.SetWindowText(strTemp); } break;	//이동편균 3번째 		수치(기간) 값
				case 90: { m_clConfigSet.m_nDay2Pyung4		= nValue;	strTemp.Format("%d", nValue); m_edt4YP		.SetWindowText(strTemp); } break;	//이동편균 4번째 		수치(기간) 값
				case 91: { m_clConfigSet.m_nPmmPDanga		= nValue;	strTemp.Format("%d", nValue); m_edtPmmPD	.SetWindowText(strTemp); } break;	// 평균매입 단가 +n%	값
				case 92: { m_clConfigSet.m_nPmmMDanga		= nValue;	strTemp.Format("%d", nValue); m_edtPmmMD	.SetWindowText(strTemp); } break;	// 평균매입 단가 -n%	값	

				case 93: { 
							m_clConfigSet.m_strAcct			= strTemp;
							for(int i = 0; i < m_cmbAcct.GetCount(); i++)
							{
								m_cmbAcct	.GetLBText(i, strAcct); strAcct.Remove('-'); strAcct.Remove(' ');
								if(strAcct.CompareNoCase(strTemp) == 0)
								{
									m_cmbAcct.SetCurSel(i);
									break;
								}								
							}
						} break;	// 계좌 할당	
				case 94: { m_clConfigSet.m_bNotDrawLine		= m_bNotDrawLine = nValue;	}	break;
			}
		
		}

		UpdateData(FALSE);
		g_clConfigSet.SetConfigValue(&m_clConfigSet);

		// <---------------------------------------------------------------------------------
		//pfile.Close();
	}

	
	
	return TRUE;

	//*****************************************************************************************
}


BOOL CDlgSetGuideLine::SaveCfgFile()
{
	// 파일 저장하기 ***************************************************************************
	CFile		pfile ;
	CString		strData, strTemp;
	CString		strSep = ";\n" ;
	UpdateData(TRUE);
	if( pfile.Open(m_strCfgFile, CFile::modeCreate | CFile::modeWrite) )
	{
		 
		 CString strCfg, strSep, sTemp;
		 strSep = ";\n" ;
		 strCfg = "FILEVER=1;\n[CHECK]\n";
		 m_clConfigSet.m_bChkYSprice	   = m_bChkYSprice			;sTemp.Format("CHK00=%d", m_bChkYSprice		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkYHprice	   = m_bChkYHprice	        ;sTemp.Format("CHK01=%d", m_bChkYHprice		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkYLprice	   = m_bChkYLprice	        ;sTemp.Format("CHK02=%d", m_bChkYLprice		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkYEprice	   = m_bChkYEprice	        ;sTemp.Format("CHK03=%d", m_bChkYEprice		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkYSHL		   = m_bChkYSHL		        ;sTemp.Format("CHK04=%d", m_bChkYSHL		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkTSprice	   = m_bChkTSprice	        ;sTemp.Format("CHK05=%d", m_bChkTSprice		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkTHprice	   = m_bChkTHprice	        ;sTemp.Format("CHK06=%d", m_bChkTHprice		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkTLprice	   = m_bChkTLprice	        ;sTemp.Format("CHK07=%d", m_bChkTLprice		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkTEprice	   = m_bChkTEprice	        ;sTemp.Format("CHK08=%d", m_bChkTEprice		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkTHL		   = m_bChkTHL		        ;sTemp.Format("CHK09=%d", m_bChkTHL			); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkTSHL		   = m_bChkTSHL		        ;sTemp.Format("CHK10=%d", m_bChkTSHL		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkTHLE		   = m_bChkTHLE		        ;sTemp.Format("CHK11=%d", m_bChkTHLE		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkHighLimit	   = m_bChkHighLimit	    ;sTemp.Format("CHK12=%d", m_bChkHighLimit	); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkLowLimit	   = m_bChkLowLimit	        ;sTemp.Format("CHK13=%d", m_bChkLowLimit	); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkPivot2Rst	   = m_bChkPivot2Rst	    ;sTemp.Format("CHK14=%d", m_bChkPivot2Rst	); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkPivot1Rst	   = m_bChkPivot1Rst	    ;sTemp.Format("CHK15=%d", m_bChkPivot1Rst	); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkPivotGLine	   = m_bChkPivotGLine       ;sTemp.Format("CHK16=%d", m_bChkPivotGLine	); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkPivot1GG	   = m_bChkPivot1GG	        ;sTemp.Format("CHK17=%d", m_bChkPivot1GG	); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkPivot2GG	   = m_bChkPivot2GG	        ;sTemp.Format("CHK18=%d", m_bChkPivot2GG	); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkDMKPreHPrice  = m_bChkDMKPreHPrice     ;sTemp.Format("CHK19=%d", m_bChkDMKPreHPrice); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkDMKGLine	   = m_bChkDMKGLine	        ;sTemp.Format("CHK20=%d", m_bChkDMKGLine	); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkDMKPreLPrice  = m_bChkDMKPreLPrice     ;sTemp.Format("CHK21=%d", m_bChkDMKPreLPrice); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChk2Pyung1	   = m_bChk2Pyung1	        ;sTemp.Format("CHK22=%d", m_bChk2Pyung1		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChk2Pyung2	   = m_bChk2Pyung2	        ;sTemp.Format("CHK23=%d", m_bChk2Pyung2		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChk2Pyung3	   = m_bChk2Pyung3	        ;sTemp.Format("CHK24=%d", m_bChk2Pyung3		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChk2Pyung4	   = m_bChk2Pyung4	        ;sTemp.Format("CHK25=%d", m_bChk2Pyung4		); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkPmmDanga	   = m_bChkPmmDanga	        ;sTemp.Format("CHK26=%d", m_bChkPmmDanga	); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkPmmPDanga     = m_bChkPmmPDanga	    ;sTemp.Format("CHK27=%d", m_bChkPmmPDanga	); strCfg += (sTemp + strSep);		 
		 m_clConfigSet.m_bChkPmmMDanga	   = m_bChkPmmMDanga	    ;sTemp.Format("CHK28=%d", m_bChkPmmMDanga	); strCfg += (sTemp + strSep);	 

		 strCfg += "[COLOR]\n";
		 m_clConfigSet.m_clrYSprice			=m_btnYSprice		.GetColor();sTemp.Format("CLR00=%d", m_clConfigSet.m_clrYSprice		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrYHprice			=m_btnYHprice		.GetColor();sTemp.Format("CLR01=%d", m_clConfigSet.m_clrYHprice		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrYLprice			=m_btnYLprice		.GetColor();sTemp.Format("CLR02=%d", m_clConfigSet.m_clrYLprice		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrYEprice			=m_btnYEprice		.GetColor();sTemp.Format("CLR03=%d", m_clConfigSet.m_clrYEprice		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrYSHL			=m_btnYSHL			.GetColor();sTemp.Format("CLR04=%d", m_clConfigSet.m_clrYSHL		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrTSprice			=m_btnTSprice		.GetColor();sTemp.Format("CLR05=%d", m_clConfigSet.m_clrTSprice		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrTHprice			=m_btnTHprice		.GetColor();sTemp.Format("CLR06=%d", m_clConfigSet.m_clrTHprice		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrTLprice			=m_btnTLprice		.GetColor();sTemp.Format("CLR07=%d", m_clConfigSet.m_clrTLprice		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrTEprice			=m_btnTEprice		.GetColor();sTemp.Format("CLR08=%d", m_clConfigSet.m_clrTEprice		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrTHL				=m_btnTHL			.GetColor();sTemp.Format("CLR09=%d", m_clConfigSet.m_clrTHL			); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrTSHL			=m_btnTSHL			.GetColor();sTemp.Format("CLR10=%d", m_clConfigSet.m_clrTSHL		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrTHLE			=m_btnTHLE			.GetColor();sTemp.Format("CLR11=%d", m_clConfigSet.m_clrTHLE		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrHighLimit		=m_btnHighLimit		.GetColor();sTemp.Format("CLR12=%d", m_clConfigSet.m_clrHighLimit	); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrLowLimit		=m_btnLowLimit		.GetColor();sTemp.Format("CLR13=%d", m_clConfigSet.m_clrLowLimit	); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrPivot2Rst		=m_btnPivot2Rst		.GetColor();sTemp.Format("CLR14=%d", m_clConfigSet.m_clrPivot2Rst	); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrPivot1Rst		=m_btnPivot1Rst		.GetColor();sTemp.Format("CLR15=%d", m_clConfigSet.m_clrPivot1Rst	); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrPivotGLine		=m_btnPivotGLine	.GetColor();sTemp.Format("CLR16=%d", m_clConfigSet.m_clrPivotGLine	); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrPivot1GG		=m_btnPivot1GG		.GetColor();sTemp.Format("CLR17=%d", m_clConfigSet.m_clrPivot1GG	); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrPivot2GG		=m_btnPivot2GG		.GetColor();sTemp.Format("CLR18=%d", m_clConfigSet.m_clrPivot2GG	); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrDMKPreHPrice	=m_btnDMKPreHPrice	.GetColor();sTemp.Format("CLR19=%d", m_clConfigSet.m_clrDMKPreHPrice); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrDMKGLine		=m_btnDMKGLine		.GetColor();sTemp.Format("CLR20=%d", m_clConfigSet.m_clrDMKGLine	); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrDMKPreLPrice	=m_btnDMKPreLPrice	.GetColor();sTemp.Format("CLR21=%d", m_clConfigSet.m_clrDMKPreLPrice); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clr2Pyung1			=m_btn2Pyung1		.GetColor();sTemp.Format("CLR22=%d", m_clConfigSet.m_clr2Pyung1		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clr2Pyung2			=m_btn2Pyung2		.GetColor();sTemp.Format("CLR23=%d", m_clConfigSet.m_clr2Pyung2		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clr2Pyung3			=m_btn2Pyung3		.GetColor();sTemp.Format("CLR24=%d", m_clConfigSet.m_clr2Pyung3		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clr2Pyung4			=m_btn2Pyung4		.GetColor();sTemp.Format("CLR25=%d", m_clConfigSet.m_clr2Pyung4		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrPmmDanga		=m_btnPmmDanga		.GetColor();sTemp.Format("CLR26=%d", m_clConfigSet.m_clrPmmDanga	); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrPmmPDanga		=m_btnPmmPDanga		.GetColor();sTemp.Format("CLR27=%d", m_clConfigSet.m_clrPmmPDanga	); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_clrPmmMDanga		=m_btnPmmMDanga		.GetColor();sTemp.Format("CLR28=%d", m_clConfigSet.m_clrPmmMDanga	); strCfg += (sTemp + strSep);

		 strCfg += "[LINE]\n";
		 m_clConfigSet.m_nLineYSprice		=  m_cmbYSprice		.GetCurSel()	 + 1;sTemp.Format("LN00=%d", m_clConfigSet.m_nLineYSprice		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLineYHprice		=  m_cmbYHprice		.GetCurSel()	 + 1;sTemp.Format("LN01=%d", m_clConfigSet.m_nLineYHprice		); strCfg += (sTemp + strSep);				
		 m_clConfigSet.m_nLineYLprice		=  m_cmbYLprice		.GetCurSel()	 + 1;sTemp.Format("LN02=%d", m_clConfigSet.m_nLineYLprice		); strCfg += (sTemp + strSep);				
		 m_clConfigSet.m_nLineYEprice		=  m_cmbYEprice		.GetCurSel()	 + 1;sTemp.Format("LN03=%d", m_clConfigSet.m_nLineYEprice		); strCfg += (sTemp + strSep);				
		 m_clConfigSet.m_nLineYSHL			=  m_cmbYSHL		.GetCurSel()	 + 1;sTemp.Format("LN04=%d", m_clConfigSet.m_nLineYSHL			); strCfg += (sTemp + strSep);		
		 m_clConfigSet.m_nLineTSprice		=  m_cmbTSprice		.GetCurSel()	 + 1;sTemp.Format("LN05=%d", m_clConfigSet.m_nLineTSprice		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLineTHprice		=  m_cmbTHprice		.GetCurSel()	 + 1;sTemp.Format("LN06=%d", m_clConfigSet.m_nLineTHprice		); strCfg += (sTemp + strSep);
		 m_clConfigSet.m_nLineTLprice		=  m_cmbTLprice		.GetCurSel()	 + 1;sTemp.Format("LN07=%d", m_clConfigSet.m_nLineTLprice		); strCfg += (sTemp + strSep);		
		 m_clConfigSet.m_nLineTEprice		=  m_cmbTEprice		.GetCurSel()	 + 1;sTemp.Format("LN08=%d", m_clConfigSet.m_nLineTEprice		); strCfg += (sTemp + strSep);		
		 m_clConfigSet.m_nLineTHL			=  m_cmbTHL			.GetCurSel()	 + 1;sTemp.Format("LN09=%d", m_clConfigSet.m_nLineTHL			); strCfg += (sTemp + strSep);		
		 m_clConfigSet.m_nLineTSHL			=  m_cmbTSHL		.GetCurSel()	 + 1;sTemp.Format("LN10=%d", m_clConfigSet.m_nLineTSHL			); strCfg += (sTemp + strSep);		
		 m_clConfigSet.m_nLineTHLE			=  m_cmbTHLE		.GetCurSel()	 + 1;sTemp.Format("LN11=%d", m_clConfigSet.m_nLineTHLE			); strCfg += (sTemp + strSep);		
		 m_clConfigSet.m_nLineHighLimit		=  m_cmbHighLimit	.GetCurSel()	 + 1;sTemp.Format("LN12=%d", m_clConfigSet.m_nLineHighLimit		); strCfg += (sTemp + strSep);	 				
		 m_clConfigSet.m_nLineLowLimit		=  m_cmbLowLimit	.GetCurSel()	 + 1;sTemp.Format("LN13=%d", m_clConfigSet.m_nLineLowLimit		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLinePivot2Rst		=  m_cmbPivot2Rst	.GetCurSel()	 + 1;sTemp.Format("LN14=%d", m_clConfigSet.m_nLinePivot2Rst		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLinePivot1Rst		=  m_cmbPivot1Rst	.GetCurSel()	 + 1;sTemp.Format("LN15=%d", m_clConfigSet.m_nLinePivot1Rst		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLinePivotGLine	=  m_cmbPivotGLine	.GetCurSel()	 + 1;sTemp.Format("LN16=%d", m_clConfigSet.m_nLinePivotGLine	); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLinePivot1GG		=  m_cmbPivot1GG	.GetCurSel()	 + 1;sTemp.Format("LN17=%d", m_clConfigSet.m_nLinePivot1GG		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLinePivot2GG		=  m_cmbPivot2GG	.GetCurSel()	 + 1;sTemp.Format("LN18=%d", m_clConfigSet.m_nLinePivot2GG		); strCfg += (sTemp + strSep);		
		 m_clConfigSet.m_nLineDMKPreHPrice	=  m_cmbDMKPreHPrice.GetCurSel()	 + 1;sTemp.Format("LN19=%d", m_clConfigSet.m_nLineDMKPreHPrice	); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLineDMKGLine		=  m_cmbDMKGLine	.GetCurSel()	 + 1;sTemp.Format("LN20=%d", m_clConfigSet.m_nLineDMKGLine		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLineDMKPreLPrice	=  m_cmbDMKPreLPrice.GetCurSel()	 + 1;sTemp.Format("LN21=%d", m_clConfigSet.m_nLineDMKPreLPrice	); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLine2Pyung1		=  m_cmb2Pyung1		.GetCurSel()	 + 1;sTemp.Format("LN22=%d", m_clConfigSet.m_nLine2Pyung1		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLine2Pyung2		=  m_cmb2Pyung2		.GetCurSel()	 + 1;sTemp.Format("LN23=%d", m_clConfigSet.m_nLine2Pyung2		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLine2Pyung3		=  m_cmb2Pyung3		.GetCurSel()	 + 1;sTemp.Format("LN24=%d", m_clConfigSet.m_nLine2Pyung3		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLine2Pyung4		=  m_cmb2Pyung4		.GetCurSel()	 + 1;sTemp.Format("LN25=%d", m_clConfigSet.m_nLine2Pyung4		); strCfg += (sTemp + strSep);	
		 m_clConfigSet.m_nLinePmmDanga		=  m_cmbPmmDanga	.GetCurSel()	 + 1;sTemp.Format("LN26=%d", m_clConfigSet.m_nLinePmmDanga		); strCfg += (sTemp + strSep);		
		 m_clConfigSet.m_nLinePmmPDanga		=  m_cmbPmmPDanga	.GetCurSel()	 + 1;sTemp.Format("LN27=%d", m_clConfigSet.m_nLinePmmPDanga		); strCfg += (sTemp + strSep);		
		 m_clConfigSet.m_nLinePmmMDanga		=  m_cmbPmmMDanga	.GetCurSel()	 + 1;sTemp.Format("LN28=%d", m_clConfigSet.m_nLinePmmMDanga		); strCfg += (sTemp + strSep);	

		 strCfg += "[2PYUNG]\n";
		 CString strValue;
 		 m_edt1YP	.GetWindowText(strValue); strValue.TrimLeft(); strValue.TrimRight(); strValue.Remove(' ');	 
		 if(!strValue.GetLength()){strValue = "0";}
		 m_clConfigSet.m_nDay2Pyung1 = atoi(strValue); sTemp.Format("YP00=%d", m_clConfigSet.m_nDay2Pyung1);	strCfg += (sTemp + strSep);
		 
		 m_edt2YP	.GetWindowText(strValue); strValue.TrimLeft(); strValue.TrimRight(); strValue.Remove(' ');
		 if(!strValue.GetLength()){strValue = "0";}
		 m_clConfigSet.m_nDay2Pyung2 = atoi(strValue); sTemp.Format("YP01=%d", m_clConfigSet.m_nDay2Pyung2);	strCfg += (sTemp + strSep);
		 
		 m_edt3YP	.GetWindowText(strValue); strValue.TrimLeft(); strValue.TrimRight(); strValue.Remove(' ');
		 if(!strValue.GetLength()){strValue = "0";}
		 m_clConfigSet.m_nDay2Pyung3 = atoi(strValue); sTemp.Format("YP02=%d", m_clConfigSet.m_nDay2Pyung3);	strCfg += (sTemp + strSep);
		 
		 m_edt4YP	.GetWindowText(strValue); strValue.TrimLeft(); strValue.TrimRight(); strValue.Remove(' ');
		 if(!strValue.GetLength()){strValue = "0";}
		 m_clConfigSet.m_nDay2Pyung4 = atoi(strValue); sTemp.Format("YP03=%d", m_clConfigSet.m_nDay2Pyung4);	strCfg += (sTemp + strSep);

		 strCfg += "[PMMD]\n";
		 m_edtPmmPD	.GetWindowText(strValue);m_clConfigSet.m_nPmmPDanga = atoi(strValue); sTemp.Format("DAN00=%d", m_clConfigSet.m_nPmmPDanga);		strCfg += (sTemp + strSep);
		 m_edtPmmMD	.GetWindowText(strValue);m_clConfigSet.m_nPmmMDanga = atoi(strValue); sTemp.Format("DAN01=%d", m_clConfigSet.m_nPmmMDanga);		strCfg += (sTemp + strSep);

		 strCfg += "[ACCT]\n";
		 strValue = _T("");
		 int nSel = m_cmbAcct.GetCurSel();
		 if(nSel == CB_ERR)
		 {
			strValue = _T("");
		 }
		 else
			m_cmbAcct.GetLBText(nSel, strValue);
		 
		 strValue.Remove('-'); strValue.TrimLeft(); strValue.TrimRight();
		 m_clConfigSet.m_strAcct = strValue; sTemp.Format("ACCT00=%s", m_clConfigSet.m_strAcct); strCfg += (sTemp + strSep);
		 
		 strCfg += "[DRAW]\n";
		 m_clConfigSet.m_bNotDrawLine	   = m_bNotDrawLine	    ;sTemp.Format("NOT_DRAW=%d", m_bNotDrawLine ); strCfg += (sTemp + strSep);	 

		 g_clConfigSet.SetConfigValue(&m_clConfigSet);
		 
		 int nLen = strCfg.GetLength();
		 pfile.Write( strCfg, nLen);
		 pfile.Close();
	}

//	pfile.Close();
	return TRUE;
}		 

void CDlgSetGuideLine::Initial_ChkBox()
{
	m_bChkYSprice = FALSE ;
	m_bChkYHprice = FALSE ;
	m_bChkYLprice = FALSE ;
	m_bChkYEprice = FALSE ;
	m_bChkYSHL = FALSE ;
	m_bChkTSprice = FALSE ;
	m_bChkTHprice = FALSE ;
	m_bChkTLprice = FALSE ;
	m_bChkTEprice = FALSE ;
	m_bChkTHL = FALSE ;
	m_bChkTSHL = FALSE ;
	m_bChkTHLE = FALSE ;
	m_bChkHighLimit = FALSE ;
	m_bChkLowLimit = FALSE ;
	m_bChkPivot2Rst = FALSE ;
	m_bChkPivot1Rst = FALSE ;
	m_bChkPivotGLine = FALSE ;
	m_bChkPivot1GG = FALSE ;
	m_bChkPivot2GG = FALSE ;
	m_bChkDMKPreHPrice = FALSE ;
	m_bChkDMKGLine = FALSE ;
	m_bChkDMKPreLPrice = FALSE ;
	m_bChk2Pyung1 = FALSE ;
	m_bChk2Pyung2 = FALSE ;
	m_bChk2Pyung3 = FALSE ;
	m_bChk2Pyung4 = FALSE ;
	m_bChkPmmDanga = FALSE ;
	m_bChkPmmPDanga = FALSE ;
	m_bChkPmmMDanga = FALSE ;
	m_bNotDrawLine	= FALSE ;
	
	UpdateData(FALSE);
}

void CDlgSetGuideLine::Initial_AcctCombo()
{
	if(m_strFullAcct.GetLength())
	{
		CString strAcct, strAcctName, GuejaText, Key, strPassGubun;
		int nLength = m_strFullAcct.GetLength();
		int nAccCount = nLength / (SIZEFULLACCT+1);	
		
		strAcct = m_strFullAcct;
		// 계좌정보 조회
		for (int i = 0; i < nAccCount; i++)
		{
			GuejaText = strAcct.Left(ACCT_NUM);	
			Key = GuejaText.Mid(3, 2);
			//if( Key == KEY01 )		// 위탁계좌 가져온다.
			//{
			AddDashToAccount(GuejaText, GuejaText);

			m_cmbAcct.InsertString(i, GuejaText);
			
			strAcctName = strAcct.Mid(ACCT_NUM, ACCT_NAME);	
			strAcctName.TrimRight();


			strAcct.Delete(0, SIZEFULLACCT+1);
		}

		m_cmbAcct.SetCurSel(0);
		
		///////////////////////////////////////////////////////////
	}
}



void CDlgSetGuideLine::AddDashToAccount(LPCSTR _lpIn, CString& szOut)
{
	CString szIn = _lpIn;
	ASSERT(szIn.GetLength()==11);
	
	szIn.Insert(3, "-");
	szIn.Insert(6, "-");
	szOut = szIn;
}


HBRUSH CDlgSetGuideLine::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if( pWnd->GetDlgCtrlID() == IDC_CHECK_GLINEDRAW)
	{
		pDC->SetBkColor(BACK_COLOR);
		hbr = (HBRUSH)m_brGDrawColor;
		return hbr;
	}	
	return hbr;
}

void CDlgSetGuideLine::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	DeleteObject(m_brGDrawColor);
}

