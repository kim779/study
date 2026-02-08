// SymbolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Symbol.h"
#include "SymbolDlg.h"

#include "PropertiesData_text.h"
#include "MainSymbolImplementation.h"

#include "SymbolData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CSymbolDlg dialog
CSymbolDlg::CSymbolDlg(CMainSymbolImplementation* pMain, CSymbolData* pSymbolData, CWnd* pParent /*=NULL*/)
	: CDialog(CSymbolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSymbolDlg)
	//}}AFX_DATA_INIT
	m_pMainSymbol = pMain;
	m_bCloseDlg =  false;
	m_pSymbolData  = pSymbolData;
//	m_pSymbolData->SetBuletType(pSymbolData->GetBuletType()); 
}

void CSymbolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSymbolDlg)
	DDX_Control(pDX, IDC_TEXT_SPECIAL_1, m_Char_1_Bt);
	DDX_Control(pDX, IDC_TEXT_SPECIAL_2, m_Char_2_Bt);
	DDX_Control(pDX, IDC_TEXT_SPECIAL_3, m_Char_3_Bt);
	DDX_Control(pDX, IDC_TEXT_SPECIAL_4, m_Char_4_Bt);
	DDX_Control(pDX, IDC_TEXT_SPECIAL_5, m_Char_5_Bt);
	DDX_Control(pDX, IDC_TEXT_SPECIAL_6, m_Char_6_Bt);
	DDX_Control(pDX, IDC_TEXT_H, m_Text_H_Bt);
	DDX_Control(pDX, IDC_TEXT_G, m_Text_G_Bt);
	DDX_Control(pDX, IDC_TEXT_F, m_Text_F_Bt);
	DDX_Control(pDX, IDC_TEXT_E, m_Text_E_Bt);
	DDX_Control(pDX, IDC_TEXT_D, m_Text_D_Bt);
	DDX_Control(pDX, IDC_TEXT_C, m_Text_C_Bt);
	DDX_Control(pDX, IDC_TEXT_B, m_Text_B_Bt);
	DDX_Control(pDX, IDC_TEXT_A, m_Text_A_Bt);
	DDX_Control(pDX, IDC_NUM_TWO, m_NumTwo_Bt);
	DDX_Control(pDX, IDC_NUM_THREE, m_NumThree_Bt);
	DDX_Control(pDX, IDC_NUM_ONE, m_NumOne_Bt);
	DDX_Control(pDX, IDC_NUM_FOUR, m_NumFour_Bt);
	DDX_Control(pDX, IDC_NUM_FIVE, m_NumFive_Bt);
	DDX_Control(pDX, IDC_NUM_SIX, m_NumSix_Bt);
	DDX_Control(pDX, IDC_NUM_SEVEN, m_NumSeven_Bt);
	DDX_Control(pDX, IDC_NUM_EIGHT, m_NumEight_Bt);
	DDX_Control(pDX, IDC_NUM_NINE, m_NumNine_Bt);
	DDX_Control(pDX, IDC_NUM_TEN, m_NumTen_Bt);
	DDX_Control(pDX, IDC_ARROW_UP, m_ArrowUp_Bt);
	DDX_Control(pDX, IDC_ARROW_RIGHTTOP, m_ArrowRightTop_Bt);
	DDX_Control(pDX, IDC_ARROW_RIGHTBOTTOM, m_ArrowRightBottom_Bt);
	DDX_Control(pDX, IDC_ARROW_LEFTBOTTOM, m_ArrowLeftBottom_Bt);
	DDX_Control(pDX, IDC_ARROW_LEFTTOP, m_ArrowLeftTop_Bt);
	DDX_Control(pDX, IDC_ARROW_DOWN, m_ArrowDown_Bt);
	DDX_Control(pDX, IDC_PREVIEW, m_PreView_Static);
	DDX_Control(pDX, IDC_COLOR, m_Color_Bt);
	DDX_Control(pDX, IDC_ARROW_RIGHT, m_ArrowRight_Bt);
	DDX_Control(pDX, IDC_ARROW_LEFT, m_ArrowLeft_Bt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSymbolDlg, CDialog)
	//{{AFX_MSG_MAP(CSymbolDlg)
	ON_BN_CLICKED(IDC_ARROW_LEFT, OnArrowLeft)
	ON_BN_CLICKED(IDC_ARROW_RIGHT, OnArrowRight)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ARROW_LEFTTOP, OnArrowLefttop)
	ON_BN_CLICKED(IDC_ARROW_DOWN, OnArrowDown)
	ON_BN_CLICKED(IDC_ARROW_LEFTBOTTOM, OnArrowLeftbottom)
	ON_BN_CLICKED(IDC_ARROW_RIGHTBOTTOM, OnArrowRightbottom)
	ON_BN_CLICKED(IDC_ARROW_RIGHTTOP, OnArrowRighttop)
	ON_BN_CLICKED(IDC_ARROW_UP, OnArrowUp)
	ON_BN_CLICKED(IDC_NUM_FIVE, OnNumFive)
	ON_BN_CLICKED(IDC_NUM_FOUR, OnNumFour)
	ON_BN_CLICKED(IDC_NUM_ONE, OnNumOne)
	ON_BN_CLICKED(IDC_NUM_THREE, OnNumThree)
	ON_BN_CLICKED(IDC_NUM_TWO, OnNumTwo)
	ON_BN_CLICKED(IDC_TEXT_A, OnTextA)
	ON_BN_CLICKED(IDC_TEXT_B, OnTextB)
	ON_BN_CLICKED(IDC_TEXT_C, OnTextC)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_SYMBOL_SIZE, OnSelchangeSymbolSize)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	ON_BN_CLICKED(IDC_NUM_SIX, OnNumSix)
	ON_BN_CLICKED(IDC_NUM_TEN, OnNumTen)
	ON_BN_CLICKED(IDC_NUM_SEVEN, OnNumSeven)
	ON_BN_CLICKED(IDC_NUM_EIGHT, OnNumEight)
	ON_BN_CLICKED(IDC_NUM_NINE, OnNumNine)
	ON_BN_CLICKED(IDC_TEXT_D, OnTextD)
	ON_BN_CLICKED(IDC_TEXT_E, OnTextE)
	ON_BN_CLICKED(IDC_TEXT_F, OnTextF)
	ON_BN_CLICKED(IDC_TEXT_G, OnTextG)
	ON_BN_CLICKED(IDC_TEXT_H, OnTextH)
	ON_BN_CLICKED(IDC_TEXT_SPECIAL_1, OnTextSpecial1)
	ON_BN_CLICKED(IDC_TEXT_SPECIAL_2, OnTextSpecial2)
	ON_BN_CLICKED(IDC_TEXT_SPECIAL_3, OnTextSpecial3)
	ON_BN_CLICKED(IDC_TEXT_SPECIAL_4, OnTextSpecial4)
	ON_BN_CLICKED(IDC_TEXT_SPECIAL_5, OnTextSpecial5)
	ON_BN_CLICKED(IDC_TEXT_SPECIAL_6, OnTextSpecial6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg message handlers

void CSymbolDlg::OnClose() 
{
	m_pSymbolData->SetBuletType(CSymbolBaseData::SYMBOL_SELECTION);
	m_bCloseDlg = true;

	if(m_pMainSymbol != NULL) 
	{ 		
		m_pMainSymbol->SetCurrentSymbolTool(CSymbol::SYMBOL_SELECTION);
		m_pMainSymbol->DeleteSymbolDlg();
		//m_pSymbolData->GetParent()->OnToo
		//m_pMainSymbol->Initial();		
	}

	//CDialog::OnClose();
}

void CSymbolDlg::OnSelchangeSymbolSize() 
{
	CComboBox* pCombo = (CComboBox* )GetDlgItem(IDC_SYMBOL_SIZE);
	m_pSymbolData->GetPropertiesData()->SetSizeType(CSymbolBaseData::SIZETYPE(pCombo->GetCurSel()));
	m_PreView_Static.ModifySymbolType(m_pSymbolData->GetBuletType());
}

bool CSymbolDlg::IsSymbolDlgClose()
{
	return m_bCloseDlg;
}

void CSymbolDlg::OnArrowLeft() 
{
	//커서 모양 바꾸기
	SetBuletType(CSymbolBaseData::SYMBOL_ARROW_LEFT, &m_ArrowLeft_Bt);
	SetToggle(GetFocus(), &m_ArrowLeft_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_ARROW_LEFT);
}

void CSymbolDlg::OnArrowRight() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_ARROW_RIGHT, &m_ArrowRight_Bt);
	SetToggle(GetFocus(), &m_ArrowRight_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_ARROW_RIGHT);
}
void CSymbolDlg::OnArrowLefttop() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_ARROW_LEFTTOP, &m_ArrowLeftTop_Bt);
	SetToggle(GetFocus(), &m_ArrowLeftTop_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_ARROW_LEFTTOP);
}

void CSymbolDlg::OnArrowDown() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_ARROW_DOWN, &m_ArrowDown_Bt);
	SetToggle(GetFocus(), &m_ArrowDown_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_ARROW_DOWN);
}

void CSymbolDlg::OnArrowLeftbottom() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_ARROW_LEFTBOTTOM, &m_ArrowLeftBottom_Bt);
	SetToggle(GetFocus(), &m_ArrowLeftBottom_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_ARROW_LEFTBOTTOM);
}

void CSymbolDlg::OnArrowRightbottom() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_ARROW_RIGHTBOTTOM, &m_ArrowRightBottom_Bt);
	SetToggle(GetFocus(), &m_ArrowRightBottom_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_ARROW_RIGHTBOTTOM);
}

void CSymbolDlg::OnArrowRighttop() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_ARROW_RIGHTTOP, &m_ArrowRightTop_Bt);
	SetToggle(GetFocus(), &m_ArrowRightTop_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_ARROW_RIGHTTOP);
}

void CSymbolDlg::OnArrowUp() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_ARROW_UP, &m_ArrowUp_Bt);
	SetToggle(GetFocus(), &m_ArrowUp_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_ARROW_UP);
}

void CSymbolDlg::OnNumFive() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_NUM_FIVE, &m_NumFive_Bt);
	SetToggle(GetFocus(), &m_NumFive_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_NUM_FIVE);
}

void CSymbolDlg::OnNumSix() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_NUM_SIX, &m_NumSix_Bt);
	SetToggle(GetFocus(), &m_NumSix_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_NUM_SIX);
}

void CSymbolDlg::OnNumSeven() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_NUM_SEVEN, &m_NumSeven_Bt);
	SetToggle(GetFocus(), &m_NumSeven_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_NUM_SEVEN);
}

void CSymbolDlg::OnNumEight() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_NUM_EIGHT, &m_NumEight_Bt);
	SetToggle(GetFocus(), &m_NumEight_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_NUM_EIGHT);
}

void CSymbolDlg::OnNumNine() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_NUM_NINE, &m_NumNine_Bt);
	SetToggle(GetFocus(), &m_NumNine_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_NUM_NINE);
}

void CSymbolDlg::OnNumTen() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_NUM_TEN, &m_NumTen_Bt);
	SetToggle(GetFocus(), &m_NumTen_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_NUM_TEN);
}

void CSymbolDlg::OnNumFour() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_NUM_FOUR, &m_NumFour_Bt);
	SetToggle(GetFocus(), &m_NumFour_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_NUM_FOUR);
}

void CSymbolDlg::OnNumOne() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_NUM_ONE, &m_NumOne_Bt);
	SetToggle(GetFocus(), &m_NumOne_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_NUM_ONE);
}

void CSymbolDlg::OnNumThree() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_NUM_THREE, &m_NumThree_Bt);
	SetToggle(GetFocus(), &m_NumThree_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_NUM_THREE);
}

void CSymbolDlg::OnNumTwo() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_NUM_TWO, &m_NumTwo_Bt);
	SetToggle(GetFocus(), &m_NumTwo_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_NUM_TWO);
}

void CSymbolDlg::OnTextA() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_TEXT_A, &m_Text_A_Bt);
	SetToggle(GetFocus(), &m_Text_A_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_TEXT_A);
}

void CSymbolDlg::OnTextB() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_TEXT_B, &m_Text_B_Bt);
	SetToggle(GetFocus(), &m_Text_B_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_TEXT_B);
}

void CSymbolDlg::OnTextC() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_TEXT_C, &m_Text_C_Bt);
	SetToggle(GetFocus(), &m_Text_C_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_TEXT_C);
}

void CSymbolDlg::OnTextD() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_TEXT_D, &m_Text_D_Bt);
	SetToggle(GetFocus(), &m_Text_D_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_TEXT_D);
}

void CSymbolDlg::OnTextE() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_TEXT_E, &m_Text_E_Bt);
	SetToggle(GetFocus(), &m_Text_E_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_TEXT_E);
}

void CSymbolDlg::OnTextF() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_TEXT_F, &m_Text_F_Bt);
	SetToggle(GetFocus(), &m_Text_F_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_TEXT_F);
}

void CSymbolDlg::OnTextG() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_TEXT_G, &m_Text_G_Bt);
	SetToggle(GetFocus(), &m_Text_G_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_TEXT_G);
}

void CSymbolDlg::OnTextH() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_TEXT_H, &m_Text_H_Bt);
	SetToggle(GetFocus(), &m_Text_H_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_TEXT_H);
}

void CSymbolDlg::OnTextSpecial1() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_CHAR_1, &m_Char_1_Bt);
	SetToggle(GetFocus(), &m_Char_1_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_CHAR_1);
}

void CSymbolDlg::OnTextSpecial2() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_CHAR_2, &m_Char_2_Bt);
	SetToggle(GetFocus(), &m_Char_2_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_CHAR_2);
}

void CSymbolDlg::OnTextSpecial3() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_CHAR_3, &m_Char_3_Bt);
	SetToggle(GetFocus(), &m_Char_3_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_CHAR_3);
}

void CSymbolDlg::OnTextSpecial4() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_CHAR_4, &m_Char_4_Bt);
	SetToggle(GetFocus(), &m_Char_4_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_CHAR_4);
}

void CSymbolDlg::OnTextSpecial5() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_CHAR_5, &m_Char_5_Bt);
	SetToggle(GetFocus(), &m_Char_5_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_CHAR_5);
}

void CSymbolDlg::OnTextSpecial6() 
{
	SetBuletType(CSymbolBaseData::SYMBOL_CHAR_6, &m_Char_6_Bt);
	SetToggle(GetFocus(), &m_Char_6_Bt);
	m_pMainSymbol->SetSymbolType(CSymbolBaseData::SYMBOL_CHAR_6);
}

void CSymbolDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	m_bCloseDlg = false;
	Initial();

	if(m_pSymbolData->GetBuletType() == CSymbolBaseData::SYMBOL_SELECTION)
		m_pSymbolData->SetBuletType(m_PreView_Static.GetSymbolType());
	m_PreView_Static.ModifySymbolType(m_pSymbolData->GetBuletType());

	SetToggle(GetFocus(), GetButtonType(m_pSymbolData->GetBuletType()));
}

BOOL CSymbolDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	Initial();
	//sy 2005.03.30.
	//미리보기
	//Initial_View();
/*
	//버튼에 비트맵 입히기
	CDynamicBitmapLoad bitmapbutton;
	bitmapbutton.SetFilePath(m_pMainSymbol->GetFilePath());

	bitmapbutton.LoadBitmap(&m_ArrowLeft_Bt, "left_up.BMP", "left_dn.BMP");
	bitmapbutton.LoadBitmap(&m_ArrowRight_Bt, "right_up.BMP", "right_dn.BMP");
	bitmapbutton.LoadBitmap(&m_ArrowLeftTop_Bt, "lefttop_up.bmp", "lefttop_dn.bmp");
	bitmapbutton.LoadBitmap(&m_ArrowRightTop_Bt, "righttop_up.bmp", "righttop_dn.bmp");

	bitmapbutton.LoadBitmap(&m_ArrowDown_Bt, "bottom_up.bmp", "bottom_dn.bmp");
	bitmapbutton.LoadBitmap(&m_ArrowUp_Bt, "top_up.bmp", "top_dn.bmp");
	bitmapbutton.LoadBitmap(&m_ArrowLeftBottom_Bt, "leftbottom_up.bmp", "leftbottom_dn.bmp");
	bitmapbutton.LoadBitmap(&m_ArrowRightBottom_Bt, "rightbottom_up.bmp", "rightbottom_dn.bmp");

	bitmapbutton.LoadBitmap(&m_NumOne_Bt, "1_up.bmp", "1_dn.bmp");
	bitmapbutton.LoadBitmap(&m_NumTwo_Bt, "2_up.bmp", "2_dn.bmp");
	bitmapbutton.LoadBitmap(&m_NumThree_Bt, "3_up.bmp", "3_dn.bmp");
	bitmapbutton.LoadBitmap(&m_NumFour_Bt, "4_up.bmp", "4_dn.bmp");

	bitmapbutton.LoadBitmap(&m_NumFive_Bt, "5_up.bmp", "5_dn.bmp");
	bitmapbutton.LoadBitmap(&m_Text_A_Bt, "a_up.bmp", "a_dn.bmp");
	bitmapbutton.LoadBitmap(&m_Text_B_Bt, "b_up.bmp", "b_dn.bmp");
	bitmapbutton.LoadBitmap(&m_Text_C_Bt, "c_up.bmp", "c_dn.bmp");
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSymbolDlg::OnColor() 
{
	CColorDialog color;
	if(color.DoModal() == IDOK) {
		m_pSymbolData->GetPropertiesData()->SetColor(color.GetColor());
		m_Color_Bt.SetColor(m_pSymbolData->GetPropertiesData()->GetColor());
		m_Color_Bt.Invalidate();
		m_PreView_Static.ModifySymbolType(m_pSymbolData->GetBuletType());
	}
}

void CSymbolDlg::OnFont() 
{
	//sy 2005.03.30.
	CPropertiesData_text* pPropertiesData_Text = m_pSymbolData->GetPropertiesData();
	if(pPropertiesData_Text == NULL)
		return;

	CFontDialog fontDlg(&pPropertiesData_Text->GetLogFont());
	fontDlg.m_cf.rgbColors = pPropertiesData_Text->GetTextColor();
	if(fontDlg.DoModal() == IDOK)
	{
		fontDlg.GetCurrentFont(&pPropertiesData_Text->GetLogFont());
		pPropertiesData_Text->SetTextColor(fontDlg.GetColor());
		m_PreView_Static.ModifySymbolType(m_pSymbolData->GetBuletType());
	}	
}

void CSymbolDlg::Initial()
{
	//color
	m_Color_Bt.SetColor(m_pSymbolData->GetPropertiesData()->GetColor());
	//size
	CComboBox* pCombo = (CComboBox* )GetDlgItem(IDC_SYMBOL_SIZE);
	pCombo->SetCurSel(m_pSymbolData->GetPropertiesData()->GetSizeType());

	//sy 2005.03.30.
	// 미리보기 초기화
	Initial_View();
}

//sy 2005.03.30.
void CSymbolDlg::Initial_View()
{
	m_PreView_Static.SetPropertiesData(m_pSymbolData->GetPropertiesData());
	IChartManager* pMainBlock = m_pSymbolData->GetMainBlock();
	if( pMainBlock)
	{
		m_PreView_Static.SetBackgroundColor(pMainBlock->GetBackgroundColor());
		pMainBlock->Release();
	}
}

void CSymbolDlg::SetBuletType(const CSymbolBaseData::TOOLTYPE bulettype, CDynamicBitmapLoadButton* pcurrentButton)
{
	CDynamicBitmapLoadButton* pButton = GetButtonType(m_pSymbolData->GetBuletType());
	if(pButton != NULL)
		pButton->SetButtonStyle(pButton->GetButtonStyle() & ~BS_DEFPUSHBUTTON);

	m_pSymbolData->SetBuletType(bulettype);	
	m_PreView_Static.ModifySymbolType(m_pSymbolData->GetBuletType());

	if(pcurrentButton != NULL)
		pcurrentButton->SetButtonStyle(pcurrentButton->GetButtonStyle() | BS_DEFPUSHBUTTON);
}

CDynamicBitmapLoadButton* CSymbolDlg::GetButtonType(const CSymbolBaseData::TOOLTYPE bulettype)
{
	switch(bulettype) {
	case CSymbolBaseData::SYMBOL_ARROW_LEFT://왼쪽 화살표
		return &m_ArrowLeft_Bt;
	case CSymbolBaseData::SYMBOL_ARROW_RIGHT://오른쪽 화살표
		return &m_ArrowRight_Bt;
	case CSymbolBaseData::SYMBOL_ARROW_UP://위쪽 화살표
		return &m_ArrowUp_Bt;
	case CSymbolBaseData::SYMBOL_ARROW_DOWN://아래쪽 화살표
		return &m_ArrowDown_Bt;
	case CSymbolBaseData::SYMBOL_ARROW_LEFTTOP://왼쪽 위 방향 화살표
		return &m_ArrowLeftTop_Bt;
	case CSymbolBaseData::SYMBOL_ARROW_LEFTBOTTOM://왼쪽 아래 방향 화살표
		return &m_ArrowLeftBottom_Bt;
	case CSymbolBaseData::SYMBOL_ARROW_RIGHTTOP://오른쪽 위 방향 화살표
		return &m_ArrowRightTop_Bt;
	case CSymbolBaseData::SYMBOL_ARROW_RIGHTBOTTOM://오른쪽 아래 방향 화살표
		return &m_ArrowRightBottom_Bt;
	case CSymbolBaseData::SYMBOL_NUM_ONE://숫자 1
		return &m_NumOne_Bt;
	case CSymbolBaseData::SYMBOL_NUM_TWO: //2
		return &m_NumTwo_Bt;
	case CSymbolBaseData::SYMBOL_NUM_THREE://3
		return &m_NumThree_Bt;
	case CSymbolBaseData::SYMBOL_NUM_FOUR://4
		return &m_NumFour_Bt;
	case CSymbolBaseData::SYMBOL_NUM_FIVE:  //5
		return &m_NumFive_Bt;
	case CSymbolBaseData::SYMBOL_NUM_SIX:  //6
		return &m_NumSix_Bt;
	case CSymbolBaseData::SYMBOL_NUM_SEVEN:  //7
		return &m_NumSeven_Bt;
	case CSymbolBaseData::SYMBOL_NUM_EIGHT:  //8
		return &m_NumEight_Bt;
	case CSymbolBaseData::SYMBOL_NUM_NINE:  //9
		return &m_NumNine_Bt;
	case CSymbolBaseData::SYMBOL_NUM_TEN:  //10
		return &m_NumTen_Bt;
	case CSymbolBaseData::SYMBOL_TEXT_A: //a
		return &m_Text_A_Bt;
	case CSymbolBaseData::SYMBOL_TEXT_B: //b
		return &m_Text_B_Bt;
	case CSymbolBaseData::SYMBOL_TEXT_C: //c
		return &m_Text_C_Bt;
	case CSymbolBaseData::SYMBOL_TEXT_D: //d
		return &m_Text_D_Bt;
	case CSymbolBaseData::SYMBOL_TEXT_E: //e
		return &m_Text_E_Bt;
	case CSymbolBaseData::SYMBOL_TEXT_F: //f
		return &m_Text_F_Bt;
	case CSymbolBaseData::SYMBOL_TEXT_G: //g
		return &m_Text_G_Bt;
	case CSymbolBaseData::SYMBOL_TEXT_H: //h
		return &m_Text_H_Bt;
	case CSymbolBaseData::SYMBOL_CHAR_1: //☞
		return &m_Char_1_Bt;
	case CSymbolBaseData::SYMBOL_CHAR_2: //☜
		return &m_Char_2_Bt;
	case CSymbolBaseData::SYMBOL_CHAR_3: //◆
		return &m_Char_3_Bt;
	case CSymbolBaseData::SYMBOL_CHAR_4: //★
		return &m_Char_4_Bt;
	case CSymbolBaseData::SYMBOL_CHAR_5: //●
		return &m_Char_5_Bt;
	case CSymbolBaseData::SYMBOL_CHAR_6: //■
		return &m_Char_6_Bt;
	}
	return NULL;
}

void CSymbolDlg::SetToggle(CWnd* pFocuswnd, CDynamicBitmapLoadButton* pCurrentWnd)
{
	bool state = false;
	if(pCurrentWnd == pFocuswnd) 
		state = pCurrentWnd->GetToggleState();

	m_Char_1_Bt.SetToggleState(FALSE);
	m_Char_2_Bt.SetToggleState(FALSE);
	m_Char_3_Bt.SetToggleState(FALSE);
	m_Char_4_Bt.SetToggleState(FALSE);
	m_Char_5_Bt.SetToggleState(FALSE);
	m_Char_6_Bt.SetToggleState(FALSE);
	m_Text_H_Bt.SetToggleState(FALSE);
	m_Text_G_Bt.SetToggleState(FALSE);
	m_Text_F_Bt.SetToggleState(FALSE);
	m_Text_E_Bt.SetToggleState(FALSE);
	m_Text_D_Bt.SetToggleState(FALSE);
	m_Text_C_Bt.SetToggleState(FALSE);
	m_Text_B_Bt.SetToggleState(FALSE);
	m_Text_A_Bt.SetToggleState(FALSE);
	m_NumTwo_Bt.SetToggleState(FALSE);
	m_NumThree_Bt.SetToggleState(FALSE);
	m_NumOne_Bt.SetToggleState(FALSE);
	m_NumFour_Bt.SetToggleState(FALSE);
	m_NumFive_Bt.SetToggleState(FALSE);
	m_NumSix_Bt.SetToggleState(FALSE);
	m_NumSeven_Bt.SetToggleState(FALSE);
	m_NumEight_Bt.SetToggleState(FALSE);
	m_NumNine_Bt.SetToggleState(FALSE);
	m_NumTen_Bt.SetToggleState(FALSE);
	m_ArrowUp_Bt.SetToggleState(FALSE);
	m_ArrowRightTop_Bt.SetToggleState(FALSE);
	m_ArrowRightBottom_Bt.SetToggleState(FALSE);
	m_ArrowLeftBottom_Bt.SetToggleState(FALSE);
	m_ArrowLeftTop_Bt.SetToggleState(FALSE);
	m_ArrowDown_Bt.SetToggleState(FALSE);
	m_ArrowRight_Bt.SetToggleState(FALSE);
	m_ArrowLeft_Bt.SetToggleState(FALSE);

	if(state) {
		pCurrentWnd->SetToggleState(false);
		m_pSymbolData->SetBuletType(CSymbolBaseData::SYMBOL_SELECTION);	
	}
	else 
		pCurrentWnd->SetToggleState(true);
}

void CSymbolDlg::SetData(CSymbolData* pSymbolData)
{
	m_bCloseDlg =  false;
	m_pSymbolData  = pSymbolData;
}
