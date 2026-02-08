// OrderLineConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartPeriodSummaryAddin.h"
#include "OrderLineConfigDlg.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "ChartOrderLineAddInImp.h"						// for OL_FILE
#include "LineObj.h"									// for CLineObj

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define DEFAULT_LINE_CLR	RGB(242,159,36) //주황색

/////////////////////////////////////////////////////////////////////////////
// COrderLineConfigDlg dialog

COrderLineConfigDlg::COrderLineConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COrderLineConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderLineConfigDlg)
	//}}AFX_DATA_INIT

	m_pAllOrderLines	= NULL;
	m_OLObj				= NULL;
	m_hOcxWnd = ( pParent ? pParent->GetSafeHwnd() : NULL);
}


void COrderLineConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderLineConfigDlg)
	DDX_Control(pDX, IDC_CODE, m_eCode);
	DDX_Control(pDX, IDC_SEMIAUTO_SELL2, m_chkSell2);
	DDX_Control(pDX, IDC_SEMIAUTO_SELL, m_chkSell);
	DDX_Control(pDX, IDC_SEMI_AUTO_BUY2, m_chkBuy2);
	DDX_Control(pDX, IDC_SEMI_AUTO_BUY, m_chkBuy);
	DDX_Control(pDX, IDC_BTN_COLOR, m_ctlColorPick);
	DDX_Control(pDX, IDC_LINE_THICKNESS, m_ctlLineThickness);
	DDX_Control(pDX, IDC_LINE_STYLE, m_ctlLineStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderLineConfigDlg, CDialog)
	//{{AFX_MSG_MAP(COrderLineConfigDlg)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_COLOR, OnBtnColorPick)
	ON_BN_CLICKED(IDC_SEMI_AUTO_BUY, OnSemiAutoBuy)
	ON_BN_CLICKED(IDC_SEMI_AUTO_BUY2, OnSemiAutoBuy2)
	ON_BN_CLICKED(IDC_SEMIAUTO_SELL, OnSemiautoSell)
	ON_BN_CLICKED(IDC_SEMIAUTO_SELL2, OnSemiautoSell2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderLineConfigDlg message handlers

BOOL COrderLineConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ML_SET_DEFAULT_RES();
	int i = 0;
	m_ctlLineStyle.m_bitmap.LoadBitmap(IDB_LINE_STYLE);
	AddLineType( &m_ctlLineStyle, 5);
	for(  i = 0; i < m_ctlLineStyle.GetCount(); i++)
		m_ctlLineStyle.SetItemHeight( i, 14);

	m_ctlLineThickness.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);
	AddLineType( &m_ctlLineThickness, 5);
	for( i = 0; i < m_ctlLineThickness.GetCount(); i++) 
		m_ctlLineThickness.SetItemHeight( i, 14);

	m_eCode.SetWindowText(			m_OLObj->m_strCode);
	m_chkSell.SetCheck(				m_OLObj->m_bUpSell);
	m_chkBuy.SetCheck(				m_OLObj->m_bUpBuy);
	m_chkSell2.SetCheck(			m_OLObj->m_bDownSell);
	m_chkBuy2.SetCheck(				m_OLObj->m_bDownBuy);
	m_ctlLineStyle.SetCurSel(		m_OLObj->m_nLineStyle);
	m_ctlColorPick.SetColor(		m_OLObj->m_clrLineColor);

	// for Old version Bug.
	int nThickness = m_OLObj->m_nLineThickness - 1;
	if( nThickness < 0) nThickness = 0;
	m_ctlLineThickness.SetCurSel(	nThickness);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COrderLineConfigDlg::AddLineType(CComboBox* pCombo, int Type)
{
	ASSERT( pCombo != NULL);
	pCombo->ResetContent();
	for (int i = 0; i < Type; i++) pCombo->AddString( szType[ i]);
}

void COrderLineConfigDlg::OnOK() 
{
	m_OLObj->m_bUpSell			= m_chkSell.GetCheck();
	m_OLObj->m_bUpBuy			= m_chkBuy.GetCheck();
	m_OLObj->m_bDownSell		= m_chkSell2.GetCheck();
	m_OLObj->m_bDownBuy			= m_chkBuy2.GetCheck();
	m_OLObj->m_nLineStyle		= m_ctlLineStyle.GetCurSel();
	m_OLObj->m_nLineThickness	= m_ctlLineThickness.GetCurSel() + 1;
	m_OLObj->m_clrLineColor		= m_ctlColorPick.GetColor();

	CDialog::OnOK();
}

void COrderLineConfigDlg::OnSemiAutoBuy() 
{
	if( m_chkSell.GetCheck()) m_chkSell.SetCheck( FALSE);
}

void COrderLineConfigDlg::OnSemiAutoBuy2() 
{
	if( m_chkSell2.GetCheck()) m_chkSell2.SetCheck( FALSE);
}

void COrderLineConfigDlg::OnSemiautoSell() 
{
	if( m_chkBuy.GetCheck()) m_chkBuy.SetCheck( FALSE);
}

void COrderLineConfigDlg::OnSemiautoSell2() 
{
	if( m_chkBuy2.GetCheck()) m_chkBuy2.SetCheck( FALSE);
}

void COrderLineConfigDlg::OnBtnColorPick() 
{
	COLORREF crColor = m_ctlColorPick.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_ctlColorPick.SetColor(color.GetColor());				
		m_ctlColorPick.Invalidate();
	}
}

void COrderLineConfigDlg::OnBtnSave() 
{
	CString strSave, strTemp, strTemp2, strColor;

	strSave.Format(_T("%s,%s,%s,%s,%ld,"),
				m_OLObj->m_strSerial,	// 일련번호
				m_OLObj->m_strCode,		// 종목 코드
				m_OLObj->m_strName,		// 종목명
				m_OLObj->m_strCandle,	// 기준봉
				m_OLObj->m_lSDate);		// 시작일
	strTemp.Format(_T("%f,"), m_OLObj->m_dSData);		// 시작점 Y 값
	strSave += strTemp;
	strTemp.Format(_T("%ld,"), m_OLObj->m_lEDate);		// 종료일
	strSave += strTemp;
	strTemp.Format(_T("%f,"), m_OLObj->m_dEData);		// 종료점 Y 값
	strSave += strTemp;
	strTemp.Format(_T("%d,%d,%d,%d,%d,%d,%ld,%d;"),
				m_chkSell.GetCheck(),
				m_chkBuy.GetCheck(),
				m_chkSell2.GetCheck(),
				m_chkBuy2.GetCheck(),
				m_ctlLineThickness.GetCurSel() + 1,
				m_ctlLineStyle.GetCurSel(),
				m_ctlColorPick.GetColor(),
				m_OLObj->m_nTimeDiff);		// 20081007 JS.Kim	사용자시간대 선택
	strSave += strTemp;
/*	s.Format(	"%s,"	// m_OLObj->m_strSerial,	// 일련번호
				"%s,"	// m_OLObj->m_strCode,		// 종목 코드
				"%s,"	// m_OLObj->m_strName,		// 종목명
				"%s,"	// m_OLObj->m_strCandle,	// 기준봉
				"%ld,"	// m_OLObj->m_lSDate,		// 시작일
				"%f,"	// m_OLObj->m_dSData,		// 시작점 Y 값
				"%ld,"	// m_OLObj->m_lEDate,		// 종료일
				"%f,"	// m_OLObj->m_dEData,		// 종료점 Y 값
				"%d,"	// m_chkSell.GetCheck(),
				"%d,"	// m_chkBuy.GetCheck(),
				"%d,"	// m_chkSell2.GetCheck(),
				"%d,"	// m_chkBuy2.GetCheck(),
				"%d,"	// m_ctlLineThickness.GetCurSel() + 1,
				"%d,"	// m_ctlLineStyle.GetCurSel(),
				"%ld,"	// m_ctlColorPick.GetColor());
				"%d;",
				m_OLObj->m_strSerial,	// 일련번호
				m_OLObj->m_strCode,		// 종목 코드
				m_OLObj->m_strName,		// 종목명
				m_OLObj->m_strCandle,	// 기준봉
				m_OLObj->m_lSDate,		// 시작일
				m_OLObj->m_dSData,		// 시작점 Y 값
				m_OLObj->m_lEDate,		// 종료일
				m_OLObj->m_dEData,		// 종료점 Y 값
				m_chkSell.GetCheck(),
				m_chkBuy.GetCheck(),
				m_chkSell2.GetCheck(),
				m_chkBuy2.GetCheck(),
				m_ctlLineThickness.GetCurSel() + 1,
				m_ctlLineStyle.GetCurSel(),
				m_ctlColorPick.GetColor(),
				m_OLObj->m_nTimeDiff);		// 20081007 JS.Kim	사용자시간대 선택
	*/
	BOOL bExistLine = FALSE;
	POSITION prevpos = NULL, pos = m_pAllOrderLines->GetHeadPosition();
	while (pos)
	{
		prevpos = pos;
		if( m_pAllOrderLines->GetNext(pos).Find( m_OLObj->m_strSerial) >= 0)
		{
			bExistLine = TRUE;
			break;
		}
	}

	if(bExistLine)		// Update
		m_pAllOrderLines->SetAt( prevpos, strSave);
	else		// Add
		m_pAllOrderLines->AddTail( strSave);
	
	CString strFileName = m_strUserPath + OL_FILE;
	CFile f;
	if( !f.Open( strFileName, CFile::modeCreate | CFile::modeWrite))
		  return;
	CArchive ar( &f, CArchive::store);
	m_pAllOrderLines->Serialize(ar);
	ar.Close();
	f.Close();
}




