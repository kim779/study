// NumericalInquiryOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "NumericalInquiryOptionDlg.h"

#include "ToolandRButtonTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ChartCtrl클래스부분에도 추가.
#define NUMERICALINQUIRYDLG 0x1
#define CROSSLINEVIEW		0x2
#define TOOLTIPVIEW			0x4

/////////////////////////////////////////////////////////////////////////////
// CNumericalInquiryOptionDlg dialog


CNumericalInquiryOptionDlg::CNumericalInquiryOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNumericalInquiryOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNumericalInquiryOptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nOption = 0;
}


void CNumericalInquiryOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNumericalInquiryOptionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNumericalInquiryOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CNumericalInquiryOptionDlg)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK_TOOLTIP, OnCheckTooltip)
	ON_BN_CLICKED(IDC_CHECK_CROSSLINEVIEW, OnCheckCrosslineview)
	ON_BN_CLICKED(IDC_RDO_NUMERICALINQUIRY, OnRdoNumericalinquiry)
	ON_BN_CLICKED(IDC_RDO_DRAGZOOM, OnRdoDragzoom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNumericalInquiryOptionDlg message handlers


/*-------------------------------------------------------------------------------
 - Function    :  GetNumericalInquiryOption
 - Created at  :  2004-03-17   13:42
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Parameters  :  None
 - Return Value:  int (비트값. 0001:수치조회창사용, 0010:십자선사용, 0100:수치조회툴팁사용)
 - Description :  옵션상태를 가져온다.
 -------------------------------------------------------------------------------*/
int CNumericalInquiryOptionDlg::GetNumericalInquiryOption()
{
	return m_nOption;
}


/*-------------------------------------------------------------------------------
 - Function    :  SetNumericalInquiryOption
 - Created at  :  2004-03-17   13:54
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Parameters  :  nOption - (비트값. 0001:수치조회창사용, 0010:십자선사용, 0100:수치조회툴팁사용)
 - Return Value:  None
 - Description :  nOption에 따라 각각의 옵션의 체크상태를 추가.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryOptionDlg::AddNumericalInquiryOption(int nOption)
{
	m_nOption |= nOption;
}


/*-------------------------------------------------------------------------------
 - Function    :  SubNumericalInquiryOption
 - Created at  :  2004-03-17   11:28
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Parameters  :  nOption - (상동)
 - Return Value:  None
 - Description :  nOption에 따라 각각의 옵션의 체크상태를 뺌.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryOptionDlg::SubNumericalInquiryOption(int nOption)
{
	// (2005/11/17 - Seung-Won, Bae) Subtract Option
	m_nOption &= ~nOption;
	// 수치조회창이 Disabled되면 자동으로 십자선보기도 Disabled가 된다.
	// (2005/11/17 - Seung-Won, Bae) It is processed in CNumericalInquiryOptionDlg::OnRdoDragzoom()
}

/*-------------------------------------------------------------------------------
 - Function    :  OnShowWindow
 - Created at  :  2004-03-17   11:01
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  ShowWindow. 체크상태를 표시해준다.
 -------------------------------------------------------------------------------*/
// (2005/11/17 - Seung-Won, Bae) It is OnInitDialog()
void CNumericalInquiryOptionDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	if( !bShow) return;

	// 수치조회창사용 옵션
	if( m_nOption & NUMERICALINQUIRYDLG)
	{
		( ( CButton *)GetDlgItem( IDC_RDO_NUMERICALINQUIRY))->SetCheck( 1);
		// 수치조회창사용 옵션이 체크되어 있으면 십자선사용체크박스를 활성화시킴.
		OnRdoNumericalinquiry();	
	}
	else
	{
		( ( CButton *)GetDlgItem( IDC_RDO_DRAGZOOM))->SetCheck( 1);
		// 수치조회창사용 옵션이 체크되어 있으면 십자선사용체크박스를 활성화시킴.
		OnRdoDragzoom();	
	}
	// 십자선사용
	( ( CButton *)GetDlgItem( IDC_CHECK_CROSSLINEVIEW))->SetCheck( m_nOption & CROSSLINEVIEW);
	// 수치조회툴팁사용
	( ( CButton *)GetDlgItem( IDC_CHECK_TOOLTIP))->SetCheck( m_nOption & TOOLTIPVIEW);
}


/*-------------------------------------------------------------------------------
 - Function    :  OnCheckTooltip
 - Created at  :  2004-03-17   11:14
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  '툴팁'체크박스 클릭시.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryOptionDlg::OnCheckTooltip() 
{
	CButton* pButton = NULL; //임시변수

	pButton = (CButton*)GetDlgItem(IDC_CHECK_TOOLTIP);
	
	if(1 == pButton->GetCheck())
		AddNumericalInquiryOption(TOOLTIPVIEW);
	else
		SubNumericalInquiryOption(TOOLTIPVIEW);
}


/*-------------------------------------------------------------------------------
 - Function    :  OnCheckCrosslineview
 - Created at  :  2004-03-17   11:38
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  '십자선 보기' 체크박스 클릭시
 -------------------------------------------------------------------------------*/
void CNumericalInquiryOptionDlg::OnCheckCrosslineview() 
{
	CButton* pButton = NULL; //임시변수

	pButton = (CButton*)GetDlgItem(IDC_CHECK_CROSSLINEVIEW);
	
	if(1 == pButton->GetCheck())
		AddNumericalInquiryOption(CROSSLINEVIEW);
	else
		SubNumericalInquiryOption(CROSSLINEVIEW);
}

// ADD: 종찬(04/03/25) Enter와 Esc를 막는다.
void CNumericalInquiryOptionDlg::OnOK()
{
	return;
}
void CNumericalInquiryOptionDlg::OnCancel()
{
	return;
}

/*-------------------------------------------------------------------------------
 - Function    :  OnCheckNumericalinquirydlg
 - Created at  :  2004-03-17   11:24
 - Author      :  Choi Jong-chan(mono@dooriic.co.kr)
 - Description :  수치조회창사용으로 체크되면 '십자선보이기'옵션을 Enabled시킨다.
 - Update	   :  (03/25) '십자선보이기'옵션을 Enabled됨과 동시에 Check한다.
 -------------------------------------------------------------------------------*/
// (2005/11/17 - Seung-Won, Bae) Change UI from CHECK to RADIO
void CNumericalInquiryOptionDlg::OnRdoNumericalinquiry() 
{
	// TODO: Add your control notification handler code here
	AddNumericalInquiryOption( NUMERICALINQUIRYDLG);
	CButton *pCrossLine = ( CButton *)GetDlgItem( IDC_CHECK_CROSSLINEVIEW);
	pCrossLine->EnableWindow();
	pCrossLine->SetCheck( TRUE);
	AddNumericalInquiryOption( CROSSLINEVIEW);
}
// (2005/11/17 - Seung-Won, Bae) Change UI from CHECK to RADIO
void CNumericalInquiryOptionDlg::OnRdoDragzoom() 
{
	// TODO: Add your control notification handler code here
	SubNumericalInquiryOption( NUMERICALINQUIRYDLG);
	CButton *pCrossLine = ( CButton *)GetDlgItem( IDC_CHECK_CROSSLINEVIEW);
	pCrossLine->SetCheck( FALSE);
	pCrossLine->EnableWindow( FALSE);
	SubNumericalInquiryOption( CROSSLINEVIEW);
}
