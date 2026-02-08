// OrderLineConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chartperiodsummaryaddin.h"
#include "ChartOrderLineAddInImp.h"
#include "OrderLineConfigDlg.h"
#include "LineObj.h"

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
	m_strCodeName = _T("");
	m_bUpBuy = FALSE;
	m_bUpSell = FALSE;
	m_strCode = _T("");
	m_bDownBuy = FALSE;
	m_bDownSell = FALSE;
	//}}AFX_DATA_INIT
	m_nLineStyle = 0;
	m_nLineThickness = 0;
	m_pParent = pParent;

}


void COrderLineConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderLineConfigDlg)
	DDX_Control(pDX, IDC_SEMIAUTO_SELL2, m_chkSell2);
	DDX_Control(pDX, IDC_SEMIAUTO_SELL, m_chkSell);
	DDX_Control(pDX, IDC_SEMI_AUTO_BUY2, m_chkBuy2);
	DDX_Control(pDX, IDC_SEMI_AUTO_BUY, m_chkBuy);
	DDX_Control(pDX, IDC_BTN_COLOR, m_ctlColorPick);
	DDX_Control(pDX, IDC_LINE_THICKNESS, m_ctlLineThickness);
	DDX_Control(pDX, IDC_LINE_STYLE, m_ctlLineStyle);
	DDX_Check(pDX, IDC_SEMI_AUTO_BUY, m_bUpBuy);
	DDX_Check(pDX, IDC_SEMIAUTO_SELL, m_bUpSell);
	DDX_Check(pDX, IDC_SEMI_AUTO_BUY2, m_bDownBuy);
	DDX_Check(pDX, IDC_SEMIAUTO_SELL2, m_bDownSell);
	DDX_Text(pDX, IDC_CODE, m_strCode);
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

void COrderLineConfigDlg::OnBtnSave() 
{
	UpdateData(TRUE);

	m_nLineStyle = m_ctlLineStyle.GetCurSel();
	m_nLineThickness = m_ctlLineThickness.GetCurSel();
	m_clrLineColor = m_ctlColorPick.GetColor();

	// 일련번호, 종목코드, 종목명, 기준봉,시작날짜,시작값,끝날짜,끝값,매수여부,매도여부,색상,유형,굵기
	CString strFileName = m_strUserPath+OL_FILE;
	CLineObj* pLine = (CLineObj*)m_OLObj;
	pLine->m_bUpSell = m_bUpSell;
	pLine->m_bUpBuy = m_bUpBuy;
	pLine->m_bDownSell = m_bDownSell;
	pLine->m_bDownBuy = m_bDownBuy;
	pLine->m_nLineThickness = m_nLineThickness;
	pLine->m_nLineStyle = m_nLineStyle;
	pLine->m_clrLineColor = m_clrLineColor;

	CString s;
	s.Format("%s,%s,%s,%s,%d,%f,%d,%f,%d,%d,%d,%d,%d,%d,%ld;"
									, pLine->m_strSerial	// 일련번호
									, pLine->m_strCode		// 종목 코드
									, pLine->m_strName		// 종목명
									, pLine->m_strCandle	// 기준봉
									, pLine->m_lSDate		// 시작일
									, pLine->m_dSData		// 시작점 Y 값
									, pLine->m_lEDate		// 종료일
									, pLine->m_dEData		// 종료점 Y 값
									, pLine->m_bUpSell
									, pLine->m_bUpBuy
									, pLine->m_bDownSell
									, pLine->m_bDownBuy
									, pLine->m_nLineThickness
									, pLine->m_nLineStyle
									, pLine->m_clrLineColor
									);
	
	BOOL bExistLine = FALSE;
	POSITION prevpos, pos = m_pAllOrderLines->GetHeadPosition();
	while (pos)
	{
		prevpos = pos;
		if(m_pAllOrderLines->GetNext(pos).Find(m_strPrevOrderLineInfo) >= 0)
		{
			bExistLine = TRUE;
			break;
		}
	}

	if(bExistLine)		// Update
	{
		m_pAllOrderLines->SetAt(prevpos, s);
	}
	else		// Add
	{
		m_pAllOrderLines->AddTail(s);
	}
	

	CFile f;
	if( !f.Open( strFileName, CFile::modeCreate | CFile::modeWrite ) ) {
		  return;
	}
	CArchive ar( &f, CArchive::store);

	m_pAllOrderLines->Serialize(ar);
//	m_strPrevOrderLineInfo = s;
}

void COrderLineConfigDlg::Initial_ThicknessComboBox()
{
	CWnd* pWnd=NULL;                         
	
	pWnd = GetDlgItem(IDC_LINE_THICKNESS);
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

void COrderLineConfigDlg::Initial_StyleComboBox()
{

	CWnd* pWnd=NULL;                         
	
	pWnd = GetDlgItem(IDC_LINE_STYLE);
	if(pWnd)    
	{
		CBitmapItemComboBox* pBmpCombo = ((CBitmapItemComboBox*)pWnd);
		pBmpCombo->m_bitmap.LoadBitmap(IDB_LINE_STYLE);
		AddLineType(pBmpCombo, 5);
		
		for (int i= -1 ;i < pBmpCombo->GetCount();i++)
			pBmpCombo->SetItemHeight(i, 14);

		pBmpCombo->SetCurSel(0);
	}
}

void COrderLineConfigDlg::Initial_ColorBtn()
{
	m_ctlColorPick.SetColor(m_clrLineColor);
	
	COLORREF clr = DEFAULT_LINE_CLR;
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
	m_clrLineColor = m_ctlColorPick.GetColor();
}

void COrderLineConfigDlg::AddLineType(CComboBox* pCombo, int Type)
{
	ASSERT(pCombo != NULL);
	pCombo->ResetContent();
	
	for (int i = 0; i < Type; i++)
		pCombo->AddString(szType[i]);
}

BOOL COrderLineConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CLineObj* pLine = (CLineObj*)m_OLObj;

	m_ctlLineStyle.SetCurSel(pLine->m_nLineStyle);
	m_ctlLineThickness.SetCurSel(pLine->m_nLineThickness-1);
	m_clrLineColor = pLine->m_clrLineColor;

	m_bUpSell = pLine->m_bUpSell;
	m_bUpBuy = pLine->m_bUpBuy;
	m_bDownSell = pLine->m_bDownSell;
	m_bDownBuy= pLine->m_bDownBuy;

	m_strCode = pLine->m_strCode;

	//{{{콤보 박스들 초기화
	Initial_StyleComboBox();
	Initial_ThicknessComboBox();
	//{{{색 지정 버튼 초기화
	Initial_ColorBtn();

	m_strPrevOrderLineInfo = pLine->m_strSerial;
// 	m_strPrevOrderLineInfo.Format("%s,%s,%s,%s,%d,%f,%d,%f,%d,%d,%d,%d,%d,%d,%ld;"
// 									, pLine->m_strSerial	// 일련번호
// 									, pLine->m_strCode		// 종목 코드
// 									, pLine->m_strName		// 종목명
// 									, pLine->m_strCandle	// 기준봉
// 									, pLine->m_lSDate		// 시작일
// 									, pLine->m_dSData		// 시작점 Y 값
// 									, pLine->m_lEDate		// 종료일
// 									, pLine->m_dEData		// 종료점 Y 값
// 									, pLine->m_bUpSell
// 									, pLine->m_bUpBuy
// 									, pLine->m_bDownSell
// 									, pLine->m_bDownBuy
// 									, pLine->m_nLineThickness
// 									, pLine->m_nLineStyle
// 									, pLine->m_clrLineColor
// 									); 
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COrderLineConfigDlg::OnOK() 
{
	UpdateData(TRUE);
	
	((CLineObj*)m_OLObj)->m_nLineStyle = m_ctlLineStyle.GetCurSel();
	((CLineObj*)m_OLObj)->m_nLineThickness = m_ctlLineThickness.GetCurSel()+1;
	((CLineObj*)m_OLObj)->m_clrLineColor = m_ctlColorPick.GetColor();

	((CLineObj*)m_OLObj)->m_bUpSell = m_bUpSell;
	((CLineObj*)m_OLObj)->m_bUpBuy = m_bUpBuy;
	((CLineObj*)m_OLObj)->m_bDownSell = m_bDownSell;
	((CLineObj*)m_OLObj)->m_bDownBuy = m_bDownBuy;

	CDialog::OnOK();
}

void COrderLineConfigDlg::OnCancel() 
{
	UpdateData(FALSE);
	
	CDialog::OnCancel();
}

void COrderLineConfigDlg::OnSemiAutoBuy() 
{
	m_bUpBuy = !m_bUpBuy;
	if(m_bUpSell)
		m_bUpSell = FALSE;
	m_chkBuy.SetCheck(m_bUpBuy);
	m_chkSell.SetCheck(m_bUpSell);
}

void COrderLineConfigDlg::OnSemiAutoBuy2() 
{
	m_bDownBuy = !m_bDownBuy;
	if(m_bDownSell)
		m_bDownSell = FALSE;
	m_chkBuy2.SetCheck(m_bDownBuy);
	m_chkSell2.SetCheck(m_bDownSell);
}

void COrderLineConfigDlg::OnSemiautoSell() 
{
	m_bUpSell = !m_bUpSell;
	if(m_bUpBuy)
		m_bUpBuy = FALSE;
	m_chkBuy.SetCheck(m_bUpBuy);
	m_chkSell.SetCheck(m_bUpSell);
}

void COrderLineConfigDlg::OnSemiautoSell2() 
{
	m_bDownSell = !m_bDownSell;
	if(m_bDownBuy)
		m_bDownBuy = FALSE;
	m_chkBuy2.SetCheck(m_bDownBuy);
	m_chkSell2.SetCheck(m_bDownSell);
}

void COrderLineConfigDlg::SetOrderLines(CStringList &Orderlines)
{

}
