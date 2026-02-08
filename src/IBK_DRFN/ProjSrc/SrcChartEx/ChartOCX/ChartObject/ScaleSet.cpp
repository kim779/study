// ScaleSet.cpp: implementation of the CScaleSet class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartObject.h"
#include "ScaleSet.h"

#include "ColorButton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CScaleSet::CScaleSet(UINT nIDTemplate, CWnd* pParent)
	: CDialog(nIDTemplate, pParent),
	m_pParent(pParent)
{
	m_bAllScaleGroup = FALSE;
	m_nScaleGroup = 0;
	m_nScaleGrid = 0; 

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = NULL;
	if( pParent) m_hOcxWnd = pParent->GetSafeHwnd();
}


// protected ==================================================================
void CScaleSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVertScaleSet)
	DDX_Check(pDX, IDC_ALLSCALEGROUP_CHECK, m_bAllScaleGroup);
	DDX_CBIndex(pDX, IDC_SCALEGROUP, m_nScaleGroup);
	DDX_Radio(pDX, IDC_SCALEPOSITION, m_nScalePosition);
	DDX_Radio(pDX, IDC_SCALEGRID, m_nScaleGrid);
	DDX_Control(pDX, IDC_TEXT_COLOR, m_ctlTextColor);
	DDX_Control(pDX, IDC_GRID_COLOR, m_ctlGridColor);
	//}}AFX_DATA_MAP
}

BOOL CScaleSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Dialog에 포커스를 주기 위해 사용
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	InitializeData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// ----------------------------------------------------------------------------
void CScaleSet::OnOK() 
{
	if( !ApplyInAllData()) return;

	// (2007/7/16 - Seung-Won, Bae) Do not call UpdateData(). It was called in ApplyInAllData().
	EndDialog(IDOK);
}

void CScaleSet::OnCancel() 
{
	if(m_pParent != NULL)
		m_pParent->Invalidate();

	CDialog::OnCancel();
}

void CScaleSet::OnApplyBt() 
{
	ApplyInAllData();
	if(m_pParent != NULL)
		m_pParent->Invalidate();
}

void CScaleSet::OnAllscalegroupCheck() 
{
	UpdateData(TRUE);
	EnableWindowOnAllScaleGroup();
}

void CScaleSet::OnSelchangeScalegroup() 
{
	int nLastScaleGroup = m_nScaleGroup;
	UpdateData(TRUE);
	SetScale( nLastScaleGroup);
	ChangeScaleWindow(m_nScaleGroup);
}

void CScaleSet::OnGridColor() 
{
	CColorDialog colorDlg;

	if(colorDlg.DoModal() != IDOK)
		return;

	SetScaleColorInButton(&m_ctlGridColor, colorDlg.GetColor());
}

void CScaleSet::OnTextColor() 
{
	CColorDialog colorDlg;

	if(colorDlg.DoModal() != IDOK)
		return;

	SetScaleColorInButton(&m_ctlTextColor, colorDlg.GetColor());
}

// ----------------------------------------------------------------------------
// 모든 scale group에 적용 여부에 따른 Enable.
void CScaleSet::EnableWindowOnAllScaleGroup()
{
	// scale group
	EnableWindow(GetDlgItem(IDC_SCALEGROUP));
}

// ----------------------------------------------------------------------------
void CScaleSet::SetAllScaleGroup(const BOOL bAllScaleGroup)
{
	m_bAllScaleGroup = bAllScaleGroup;
}

void CScaleSet::SetScalePosition(const int nScalePosition)
{
	m_nScalePosition = nScalePosition;
}

void CScaleSet::SetScaleColorInButton(const COLORREF& gridColor, const COLORREF& textColor)
{
	SetScaleColorInButton(&m_ctlGridColor, gridColor);
	SetScaleColorInButton(&m_ctlTextColor, textColor);
}

// ----------------------------------------------------------------------------
CWnd* CScaleSet::GetParent()
{
	return m_pParent;
}

BOOL CScaleSet::IsAllScaleGroup() const
{
	return m_bAllScaleGroup;
}

int CScaleSet::GetScalePosition() const
{
	return m_nScalePosition;
}

int CScaleSet::GetScaleGrid() const
{
	return m_nScaleGrid;
}

// ----------------------------------------------------------------------------
void CScaleSet::ChangeScaleWindow(const int nGroup, const int nPosition, const int nGrid)
{
	m_nScaleGroup = nGroup;
	m_nScalePosition = nPosition;
	m_nScaleGrid = nGrid;

	UpdateData(FALSE);
}

void CScaleSet::EnableWindow(CWnd* pItem)
{
	EnableWindow(pItem, !m_bAllScaleGroup);
}

void CScaleSet::EnableWindow(CWnd* pItem, const bool bEnable)
{
	if(pItem == NULL)
		return;

	pItem->EnableWindow(bEnable);
}


// private ====================================================================
BOOL CScaleSet::ApplyInAllData()
{
	if( !SetScale()) return FALSE;
	SetScaleColor(m_ctlGridColor.GetColor(), m_ctlTextColor.GetColor());
	ApplyInEtcData();
	return TRUE;
}

BOOL CScaleSet::SetScale()
{
	if( !UpdateData(TRUE)) return FALSE;

	if(m_bAllScaleGroup)
		SetAllScale();
	else
		SetScale(m_nScaleGroup);
	return TRUE;
}

void CScaleSet::SetScaleColorInButton(CColorButton* pColorButton, const COLORREF& color)
{
	if(pColorButton == NULL)
		return;

	pColorButton->SetColor(color);
	pColorButton->Invalidate();
}
