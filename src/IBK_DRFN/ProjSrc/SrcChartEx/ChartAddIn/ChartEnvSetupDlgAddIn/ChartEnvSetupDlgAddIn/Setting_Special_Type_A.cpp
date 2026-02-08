// Setting_Special_Type_A.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "Setting_Special_Type_A.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable
#include "GraphData.h"
#include "Conversion.h"
#include "DlgEnvSetUpAll.h"
#include "GraphSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_A dialog


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Return void : 
// Param  IGraphInfo* p_pIGraphInfo : 
// Param  CWnd* pParent /*=NULL*/ : 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Special_Type_A::CSetting_Special_Type_A( IChartOCX *p_pIChartOCX, CWnd* pParent /*=NULL*/, CWnd *pTopWnd)
	: CBaseDialog(CSetting_Special_Type_A::IDD, pParent, p_pIChartOCX, pTopWnd)
{
	//{{AFX_DATA_INIT(CSetting_Special_Type_A)
	m_strBlockChange = _T("");
	m_strBlockSize = _T("");
	//}}AFX_DATA_INIT
	
	m_nSubGraphIndex = -1;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CSetting_Special_Type_A::~CSetting_Special_Type_A()
{
}


void CSetting_Special_Type_A::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting_Special_Type_A)
	DDX_Control(pDX, IDC_CHECK_FILLUP, m_btnFillUp);
	DDX_Control(pDX, IDC_CHECK_FILLDOWN, m_btnFillDown);
	DDX_Control(pDX, IDC_THICKNESS, m_cmbThickness);
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	DDX_Text(pDX, IDC_EDIT_BLOCKCHANGE, m_strBlockChange);
	DDX_Text(pDX, IDC_EDIT_BLOCKSIZE, m_strBlockSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetting_Special_Type_A, CDialog)
	//{{AFX_MSG_MAP(CSetting_Special_Type_A)
	ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_A message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 23
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CSetting_Special_Type_A::OnInitDialog() 
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
void CSetting_Special_Type_A::Initial(IGraphInfo *p_pIGraphInfo)
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

	ILPCSTR szGraphName = m_pIGraphInfo->GetIndicatorNameString();
//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
	CString strCalCnd1,strCalCnd2;

	// 20081014 박동관 패킷타입을 얻어온다. >>
	CString strPacketType = GetPacketType();
	// 20081014 박동관 <<

	IDoubleList *pICalCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
	if( !pICalCnd) return;
	if( !strcmp( szGraphName, _MTEXT( C2_THREE_LINE_BREAK )))
	{
		GetDlgItem( IDC_STC_THICKNESS)->ShowWindow( SW_SHOW);
		m_cmbThickness.ShowWindow( SW_SHOW);
		m_btnFillUp.ShowWindow( SW_SHOW );
		m_btnFillDown.ShowWindow( SW_SHOW );

//	2006.09.07 삼선전환도 칸 전환수를 수정할수 있도록 함 
		POSITION psnCnd = pICalCnd->GetHeadPosition();
		double dData = 0;
		if( psnCnd)
		{
			pICalCnd->GetNext( psnCnd, dData);
			m_strBlockChange.Format("%.0f",dData);
		}
		Initial_FillDown(m_nSubGraphIndex, m_btnFillDown);
//	2006.09.07 삼선전환도 칸 전환수를 수정할수 있도록 함 
		Initial_FillUp(m_nSubGraphIndex, m_btnFillUp);
//	2006.09.07 삼선전환도 칸 전환수를 수정할수 있도록 함 
//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
//		((CWnd*)GetDlgItem(IDC_EDIT_BLOCKCHANGE))->ShowWindow(SW_HIDE);
//		((CWnd*)GetDlgItem(IDC_STATIC_BLOCKCHANGE))->ShowWindow(SW_HIDE);
		((CWnd*)GetDlgItem(IDC_EDIT_BLOCKSIZE))->ShowWindow(SW_HIDE);
		((CWnd*)GetDlgItem(IDC_STATIC_BLOCKSIZE))->ShowWindow(SW_HIDE);
	} 
	// for P&F
	else
	{
//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
		POSITION psnCnd = pICalCnd->GetHeadPosition();
		double dData = 0;
		if( psnCnd)
		{
			pICalCnd->GetNext( psnCnd, dData);
			m_strBlockChange.Format("%.0f",dData);
		}
		if( psnCnd)
		{
			pICalCnd->GetNext( psnCnd, dData);

			// 20081014 박동관 패킷타입에 따라 값을 지정한다. >>
			//m_strBlockSize.Format("%.1f",dData);
			if(CDataConversion::IsBaseFormat(strPacketType))
			{
				// 진볍변환일 경우... 정수를 진법으로 변환한다.
				CString strData = CDataConversion::GetStringThousandFormat(dData, strPacketType);
				m_strBlockSize = strData;
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

				m_strBlockSize.Format("%.*f", nPoint, dData);
			}
			// 20081014 박동관 <<
		}
		((CWnd*)GetDlgItem(IDC_EDIT_BLOCKSIZE))->ShowWindow(SW_SHOW);
		((CWnd*)GetDlgItem(IDC_STATIC_BLOCKSIZE))->ShowWindow(SW_SHOW);
		GetDlgItem( IDC_STC_THICKNESS)->ShowWindow( SW_HIDE);
		m_cmbThickness.ShowWindow( SW_HIDE);
//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
		m_btnFillUp.ShowWindow( SW_HIDE );
		m_btnFillDown.ShowWindow( SW_HIDE );
	}
	pICalCnd->Release();		
//	2006.09.07 삼선전환도 칸 전환수를 수정할수 있도록 함 

	// Line Thickness 초기화.
	Initial_ThicknessComboBox(m_nSubGraphIndex, 0, m_cmbThickness);

	SetLineColor();
	UpdateData(FALSE);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Return void	: 
// Param  IGraphInfo* p_pIGraphInfo : 
// Comments		: 
//-----------------------------------------------------------------------------
// 20081014 박동관 >>
BOOL CSetting_Special_Type_A::OnApply( IGraphInfo* p_pIGraphInfo )
//void CSetting_Special_Type_A::OnApply( IGraphInfo* p_pIGraphInfo )
// 20081014 박동관 <<
{
	if(p_pIGraphInfo == NULL)
		return FALSE;
	
	UpdateData();
	OnApply_Thickness( m_nSubGraphIndex, m_cmbThickness);

//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
	IDoubleList *pICalCnd = m_pIGraphInfo->GetCommonCalculateConditionList();
//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 

	// 20081014 박동관 패킷타입을 얻어온다. >>
	CString strPacketType = GetPacketType();
	// 20081014 박동관 <<

	ILPCSTR szGraphName = m_pIGraphInfo->GetIndicatorNameString();
	if( !strcmp( szGraphName, _MTEXT( C2_THREE_LINE_BREAK )))	// not P&F
	{
		OnApply_FillUp( m_nSubGraphIndex, m_btnFillUp );
//	2006.09.07 삼선전환도 칸 전환수를 수정할수 있도록 함 
		OnApply_FillDown( m_nSubGraphIndex, m_btnFillDown );
		if( pICalCnd)
		{
			double dData = 0;
			POSITION psnCnd = pICalCnd->GetHeadPosition();
			if( psnCnd)
			{
				pICalCnd->SetAt( psnCnd, atof( m_strBlockChange));
				pICalCnd->GetNext( psnCnd, dData);
			}
		}
//	2006.09.07 삼선전환도 칸 전환수를 수정할수 있도록 함 
	}
//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
	else	//P&F
	{
		if( pICalCnd)
		{
			double dData = 0;
			POSITION psnCnd = pICalCnd->GetHeadPosition();
			if( psnCnd)
			{
				pICalCnd->SetAt( psnCnd, atof( m_strBlockChange));
				pICalCnd->GetNext( psnCnd, dData);
			}
			if( psnCnd)
			{
				// 20081014 박동관 패킷타입에 따라 값을 지정한다. >>
				//pICalCnd->SetAt( psnCnd, atof( m_strBlockSize));
//				pICalCnd->GetNext( psnCnd, dData);

				if(CDataConversion::IsBaseFormat(strPacketType))
				{
					// 20081014 박동관 입력된 값이 진법변환에 유효한지 검사한다. >>
					BOOL bResult = CDataConversion::IsValid8_32(m_strBlockSize);
					if(m_strBlockSize == "0") bResult = FALSE;

					if(!bResult)
					{
						AfxMessageBox("입력된 문자열이 알맞은 진법이 아닙니다.");
						
						CWnd *pWnd = GetDlgItem(IDC_EDIT_BLOCKSIZE);
						if(pWnd && IsWindow(pWnd->GetSafeHwnd()))
						{
							pWnd->SetWindowText("");
							pWnd->SetFocus();
						}
						return FALSE;
					}

					// 진법변환일 경우... 진법을 정수로 변환한다.
					CString strOutput;
					bResult = CDataConversion::GetDataToDec(m_strBlockSize, strPacketType, strOutput);

					if(bResult)
						pICalCnd->SetAt(psnCnd, atof(strOutput));
					else
						pICalCnd->SetAt(psnCnd, atof(m_strBlockSize));
				}
				else
				{
					// 진법변환이 아닐경우...
					pICalCnd->SetAt(psnCnd, atof(m_strBlockSize));
				}
				// 20081014 박동관 <<
			}
		}
		IDoubleList *pIDrawCalCnd = m_pIGraphInfo->GetDrawConditionList();
		if( pIDrawCalCnd)
		{
			double dData = 0;
			POSITION psnCnd = pIDrawCalCnd->GetHeadPosition();
			if( psnCnd)
			{
				pIDrawCalCnd->SetAt( psnCnd, atof( "1"));
//				pIDrawCalCnd->GetNext( psnCnd, dData);
			}
		}
		pIDrawCalCnd->Release();
	}
	//칸전화수나 칸크기를 변경했을경우에 다시 계산하여 그리는 코드
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
//	2006.08.14 한칸의 값및 칸 전환수를 수정할수 있도록 함 
	p_pIGraphInfo->GetCopyDataFrom(m_pIGraphInfo);

	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	if( !strcmp( szGraphName, _MTEXT( C2_P_AND_F )))	// not P&F
		m_pIChartOCX->ReCalculateIndicator();

	return TRUE;
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 9
// Return void : 
// Param  void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_A::SetLineColor( void )
{
	if(m_nSubGraphIndex < 0) return;

	m_btnUp.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
	m_btnUp.Invalidate();

	m_btnDown.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3));
	m_btnDown.Invalidate();
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_A::OnBtnUp() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnUp.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 1, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 2, color.GetColor());
		m_btnUp.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 1));
		m_btnUp.Invalidate();
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 10
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
void CSetting_Special_Type_A::OnBtnDown() 
{
	// TODO: Add your control notification handler code here
	if(m_nSubGraphIndex < 0) return;
	
	COLORREF crColor = m_btnDown.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 3, color.GetColor());
		m_pIGraphInfo->SetSubGraphColor( m_nSubGraphIndex, 4, color.GetColor());
		m_btnDown.SetColor( m_pIGraphInfo->GetSubGraphColor( m_nSubGraphIndex, 3));
		m_btnDown.Invalidate();
	}
}

BOOL CSetting_Special_Type_A::PreTranslateMessage(MSG* pMsg) 
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

	return CDialog::PreTranslateMessage(pMsg);
}
