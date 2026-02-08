// Setting_Special_Type_C.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_Special_Type_C.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
#include "../Include_Chart/Conversion.h"

#include "GraphData.h"
#include "DlgEnvSetUpAll.h"
#include "GraphSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_C dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  CWnd* pParent /*=NULL*/ : 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Special_Type_C::CSetting_Special_Type_C( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, CWnd *pTopWnd)
	: CBaseDialog(CSetting_Special_Type_C::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_Special_Type_C)
	m_strBlockChange = _T("");
	//}}AFX_DATA_INIT

	m_nSubGraphIndex = -1;
}


void CSetting_Special_Type_C::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_Special_Type_C)
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_SPIN1, m_spnCond1);
	DDX_Control(pDX, IDC_EDIT1, m_edtCond1);
	DDX_Text(pDX, IDC_EDIT_BLOCKCHANGE, m_strBlockChange);
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_THICKNESS, m_cmbThickness);
}


BEGIN_MESSAGE_MAP(CSetting_Special_Type_C, CBaseDialog)
	//{{AFX_MSG_MAP(CSetting_Special_Type_C)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_C message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_Special_Type_C::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( !m_pIGraphInfo) return TRUE;
	Initial( NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void : 
// Param  IGraphInfo *p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_C::Initial(IGraphInfo *p_pIGraphInfo)
{
	if( m_pIGraphInfo)
	{
		OnApply( m_pIGraphInfo);
		m_pIGraphInfo->Release();
	}
	m_pIGraphInfo = p_pIGraphInfo;
	if( m_pIGraphInfo) m_pIGraphInfo->AddRef();
	else return;

	m_nSubGraphIndex = 0;

	m_btnUp.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
	m_btnUp.Invalidate();
	
	m_btnDown.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3));
	m_btnDown.Invalidate();
	
//	Initial_ThicknessComboBox(m_nSubGraphIndex, 0, m_cmbThickness);

	// 20081028 박동관 패킷타입을 얻어온다. >>
	CString strPacketType = GetPacketType();
	// 20081028 박동관 <<

	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	ILPCSTR szGraphName = m_pIGraphInfo->GetIndicatorNameString();
	if( !strcmp( szGraphName, _MTEXT( C2_RENKO_CHART )))
	{
		GetDlgItem( IDC_STATIC_BLOCKCHANGE)->ShowWindow(SW_SHOW);
		GetDlgItem( IDC_EDIT_BLOCKCHANGE)->ShowWindow(SW_SHOW);

		IDoubleList *pICalCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
		if(pICalCnd)
		{
			POSITION psnCnd = pICalCnd->GetHeadPosition();
			if( psnCnd)
			{
				double dData = 0;
				pICalCnd->GetNext( psnCnd, dData);

				// 20081028 박동관 패킷타입에 따라 값을 지정한다. >>
				//m_strBlockSize.Format("%.1f",dData);
				if(CDataConversion::IsBaseFormat(strPacketType))
				{
					// 진볍변환일 경우... 정수를 진법으로 변환한다.
					CString strData = CDataConversion::GetStringThousandFormat(dData, strPacketType);
					m_strBlockChange = strData;
				}
				else
				{
					// 진법변환이 아닐 경우...

					// 패킷타입에서 소수점 자리수를 얻어온다.
					CString strValue = strPacketType;
					strValue.Remove('%');

					int nPoint=0;
					int nPos = strValue.Find('.');
					if(nPos >= 0)
					{
						CString strPoint = strValue.Mid(nPos+1);
						nPoint = strPoint.GetLength();
					}

					m_strBlockChange.Format("%.*f", nPoint, dData);
				}
				//m_strBlockChange.Format("%.0f",dData);
				// 20081028 박동관 <<
			}
		}
		pICalCnd->Release();
	}
	else
	{
		GetDlgItem( IDC_STATIC_BLOCKCHANGE)->ShowWindow(	SW_HIDE);
		GetDlgItem( IDC_EDIT_BLOCKCHANGE)->ShowWindow(		SW_HIDE);
	}

	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_C::OnBtnUp() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0)
		return;
	
	COLORREF crColor = m_btnUp.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK) 
		return;

	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());

	m_btnUp.SetColor( color.GetColor() );
	m_btnUp.Invalidate();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_C::OnBtnDown() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnDown.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() != IDOK) 
		return;

	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 3, color.GetColor());
	m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 4, color.GetColor());

	m_btnDown.SetColor( color.GetColor() );
	m_btnDown.Invalidate();
}




//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void	: 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
// 20081014 박동관 >>
BOOL CSetting_Special_Type_C::OnApply( IGraphInfo* p_pIGraphInfo )
//void CSetting_Special_Type_C::OnApply( IGraphInfo* p_pIGraphInfo )
// 20081014 박동관 <<
{
	if(p_pIGraphInfo == NULL)
		return FALSE;
	
//	OnApply_Thickness( m_nSubGraphIndex, m_cmbThickness);

	// 20081028 박동관 패킷타입을 얻어온다. >>
	CString strPacketType = GetPacketType();
	// 20081028 박동관 <<

	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	ILPCSTR szGraphName = m_pIGraphInfo->GetIndicatorNameString();
	if( !strcmp( szGraphName, _MTEXT( C2_RENKO_CHART )))
	{
		IDoubleList *pICalCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
		if( pICalCnd)
		{
			POSITION psnCnd = pICalCnd->GetHeadPosition();
			if( psnCnd)
			{
				UpdateData();

				// 20081028 박동관 >>
				//pICalCnd->SetAt( psnCnd, atof( m_strBlockChange));
				if(CDataConversion::IsBaseFormat(strPacketType))
				{
					// 입력된 값이 진법변환에 유효한지 검사한다. >>
					BOOL bResult = CDataConversion::IsValid8_32(m_strBlockChange);
					if(m_strBlockChange == "0") bResult = FALSE;

					if(!bResult)
					{
						AfxMessageBox("입력된 문자열이 알맞은 진법이 아닙니다.");
						
						CWnd *pWnd = GetDlgItem(IDC_EDIT_BLOCKCHANGE);
						if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
						{
							pWnd->SetWindowText("");
							pWnd->SetFocus();
						}
						return FALSE;
					}

					// 진법변환일 경우... 진법을 정수로 변환한다.
					CString strOutput;
					bResult = CDataConversion::GetDataToDec(m_strBlockChange, strPacketType, strOutput);

					if(bResult)
						pICalCnd->SetAt(psnCnd, atof(strOutput));
					else
						pICalCnd->SetAt( psnCnd, atof( m_strBlockChange));
				}
				else
				{
					// 진법변환이 아닐경우...
					pICalCnd->SetAt( psnCnd, atof( m_strBlockChange));
				}
				// 20081028 박동관 <<

				double d;
				pICalCnd->GetHead(d);
				
			}
		}

		// (2008/10/27 - Seung-Won, Bae) for Auto Condition.
		IDoubleList *pIDrawCalCnd = m_pIGraphInfo->GetDrawConditionList();
		if( pIDrawCalCnd)
		{
			double dData = 0;
			POSITION psnCnd = pIDrawCalCnd->GetHeadPosition();
			if( psnCnd) pIDrawCalCnd->SetAt( psnCnd, atof( "1"));
		}
		pIDrawCalCnd->Release();
		
		if( !CBaseDialog::m_bDoCalculate)
		{
			IDoubleList *pICalCndOrg = p_pIGraphInfo->GetCommonCalculateConditionList();
			if( pICalCndOrg)
			{
				CBaseDialog::m_bDoCalculate = !DoesEqualDoubleList( pICalCndOrg, pICalCnd);
				pICalCndOrg->Release();
			}
		}
		
		pICalCnd->Release();

		p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

		m_pIChartOCX->ReCalculateIndicator();
	}
	else
		p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	return TRUE;
}

BOOL CSetting_Special_Type_C::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if( m_pParentWnd != NULL )
			{
				if( m_pTopWnd != NULL )
					( (CDlgEnvSetUpAll *)m_pTopWnd)->OK();

				else
					( (CGraphSetting *)m_pParentWnd)->OK();
			}
		}

		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if( m_pParentWnd != NULL )
			{
				if( m_pTopWnd != NULL )
					( (CDlgEnvSetUpAll *)m_pTopWnd)->Cancel();

				else
					( (CGraphSetting *)m_pParentWnd)->Cancel();
			}
		}
	}
	
	return CBaseDialog::PreTranslateMessage(pMsg);
}
