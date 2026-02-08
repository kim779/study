// DlgBlockInsert.cpp : implementation file
//

#include "stdafx.h"
#include "chartobject.h"
#include "DlgBlockInsert.h"

#include "../Include_Chart/ScaleDataList.h"
#include "MainBlockImp.h"						// for CMainBlockImp

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockInsert dialog


CDlgBlockInsert::CDlgBlockInsert( CWnd* pParent, CMainBlockImp *p_pMainBlockImp, CInsertingBlockHorzScaleData* pInsertingHorzScaleData)
	: CDialog(CDlgBlockInsert::IDD, pParent),
	m_pParent(pParent),
	m_pInsertingBlockHorzScaleData(pInsertingHorzScaleData)
{
	//{{AFX_DATA_INIT(CDlgBlockInsert)
	m_bOnlyBottomShow = FALSE;
	m_nScaleGrid = 0;
	m_nScalePosition = 3;
	//}}AFX_DATA_INIT

	// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
	//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
	m_pMainBlockImp = p_pMainBlockImp;
}


void CDlgBlockInsert::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgBlockInsert)
	DDX_Check(pDX, IDC_ONLYBOTTOMSHOW, m_bOnlyBottomShow);
	DDX_Radio(pDX, IDC_SCALEPOSITION, m_nScalePosition);
	DDX_Radio(pDX, IDC_SCALEGRID, m_nScaleGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgBlockInsert, CDialog)
	//{{AFX_MSG_MAP(CDlgBlockInsert)
	ON_BN_CLICKED(IDC_ONLYBOTTOMSHOW, OnOnlyBottomShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgBlockInsert message handlers

BOOL CDlgBlockInsert::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Dialog에 포커스를 주기 위해 사용
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	InitializeData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
void CDlgBlockInsert::OnOK() 
{
	SetHorzScaleData();
	
	CDialog::OnOK();
}

void CDlgBlockInsert::OnCancel() 
{
	CDialog::OnCancel();
}

void CDlgBlockInsert::OnOnlyBottomShow() 
{
	UpdateData(TRUE);
	m_nScalePosition = (int) CScaleBaseData::HORZ_BOTTOM;
	UpdateData(FALSE);
	EnableWindowOnOnlyEndScale(!m_bOnlyBottomShow);
}


/////////////////////////////////////////////////////////////////////////////
// private ==================================================================
void CDlgBlockInsert::InitializeData()
{
	if(m_pInsertingBlockHorzScaleData == NULL)
		return;

	m_bOnlyBottomShow = m_pInsertingBlockHorzScaleData->IsOnlyBottomBlockShow();
	m_nScalePosition = (int) m_pInsertingBlockHorzScaleData->GetHorizontalScale().GetHorzScalePosition();
	m_nScaleGrid = (int) m_pInsertingBlockHorzScaleData->GetHorizontalScale().GetGridType();

	UpdateData(FALSE);
	OnOnlyBottomShow();
}

void CDlgBlockInsert::SetHorzScaleData()
{
	if(m_pInsertingBlockHorzScaleData == NULL)
		return;

	UpdateData(TRUE);
	m_pInsertingBlockHorzScaleData->SetOnlyBottomBlockShow(m_bOnlyBottomShow);

	// (2009/9/6 - Seung-Won, Bae) Do not support CHorizontalScale::SetHorzScalePosition() as public.
	//		To rearrange the row positions. Use CMainBlockImp::SetHorzScalePosition().
	if( m_pMainBlockImp) m_pMainBlockImp->SetHorzScalePosition( m_pInsertingBlockHorzScaleData->GetHorizontalScale(), ( CScaleBaseData::HORZSCALEPOSITION)m_nScalePosition);

	m_pInsertingBlockHorzScaleData->GetHorizontalScale().SetGridType((CScaleBaseData::SCALEGRIDTYPE) m_nScaleGrid);
}

// 하단의 block만 scale 적용 여부에 따른 Enable.
void CDlgBlockInsert::EnableWindowOnOnlyEndScale(const bool bEnable)
{
	// scale position
	EnableWindow(GetDlgItem(IDC_SCALEPOSITION), bEnable);
	EnableWindow(GetDlgItem(IDC_SCALEPOSITION1), bEnable);
	EnableWindow(GetDlgItem(IDC_SCALEPOSITION2), bEnable);
	EnableWindow(GetDlgItem(IDC_SCALEPOSITION3), bEnable);
	EnableWindow(GetDlgItem(IDC_SCALEPOSITION4), bEnable);
}

void CDlgBlockInsert::EnableWindow(CWnd* pItem, const bool bEnable)
{
	if(pItem == NULL)
		return;

	pItem->EnableWindow(bEnable);
}