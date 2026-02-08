// BaseTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "BaseTabDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// LineType에 저장할 Type들
static TCHAR *szType[] = 
{
	_T("0"),	_T("1"), 	_T("2"),	_T("3"),	_T("4"),	_T("5"),
	_T("6"),	_T("7"),	_T("8"),	_T("9"),	_T("10"),	_T("11"),
	_T("12"),	_T("13"),	_T("14"),	_T("15"),	_T("16"),	_T("17"),
	_T("18"),	_T("19"),	_T("20"),	_T("21"),	_T("22"),	_T("23"),
	_T("24")
};

/////////////////////////////////////////////////////////////////////////////
// CBaseTabDlg dialog

CBaseTabDlg::CBaseTabDlg( int IDD, CWnd* pParent /*=NULL*/)
	: CDialog( IDD, pParent )
{
	//{{AFX_DATA_INIT(CBaseTabDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


IMPLEMENT_DYNAMIC( CBaseTabDlg, CDialog)

void CBaseTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseTabDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBaseTabDlg, CDialog)
	//{{AFX_MSG_MAP(CBaseTabDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseTabDlg message handlers

//sy 2005.10.12
bool CBaseTabDlg::ChangeTabPage( CBaseTabDlg *pSourceDlgTabPage )
{
	return false;
}

bool CBaseTabDlg::GetCurrentPrice(int& nCurPrice) const
{
	nCurPrice = -1;
	return false;
}
//sy end

void CBaseTabDlg::Initial_LineStyleComboBox( CBitmapItemComboBox &LineStyle_Combo, int nCount, int nCurSel, int nType/*=0*/)
//20131211 이문수 <<
{
	// 1. BitMap을 Load한다.
	//20131211 이문수 >>
	//if( !LineStyle_Combo.m_bitmap.GetSafeHandle())
	//	LineStyle_Combo.m_bitmap.LoadBitmap(IDB_AXIS_LINE_STYLE);
	if( !LineStyle_Combo.m_bitmap.GetSafeHandle())
	{
		if(nType == 0)
			LineStyle_Combo.m_bitmap.LoadBitmap(IDB_LINE_STYLE);
		else if(nType == 1)
			LineStyle_Combo.m_bitmap.LoadBitmap(IDB_LINE_STYLE2);
	}
	//20131211 이문수 <<

	AddLineType(&LineStyle_Combo, nCount);

	// 2. 값을 setting한다.
	for( int i = -1 ; i < LineStyle_Combo.GetCount() ; i++ )
		LineStyle_Combo.SetItemHeight( i, 14 );

	LineStyle_Combo.SetCurSel( nCurSel );
}

void CBaseTabDlg::Initial_ThicknessComboBox( CBitmapItemComboBox &LineStyle_Combo, int nCount, int nCurSel )
{
	// 1. BitMap을 Load한다.
	if( !LineStyle_Combo.m_bitmap.GetSafeHandle())
		LineStyle_Combo.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);

	AddLineType(&LineStyle_Combo, nCount);

	// 2. 값을 setting한다.
	for( int i = -1 ; i < LineStyle_Combo.GetCount() ; i++ )
		LineStyle_Combo.SetItemHeight( i, 14 );

	LineStyle_Combo.SetCurSel( nCurSel );
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 21
// Return void : 
// Param  CComboBox *pCombo : 
// Param  int Type : 
// Comments		: pCombo에 LineType을 등록한다.
//-----------------------------------------------------------------------------
void CBaseTabDlg::AddLineType( CComboBox *pCombo, int nCount )
{
	ASSERT( pCombo != NULL );
	
	// 1. Combo를 초기화 시킨다.
	pCombo->ResetContent();

	// 2. LineType을 지정한다.
	for( int i = nCount-1 ; i >= 0 ; i-- )
		pCombo->InsertString(0, szType[i] );
}
