// RealTimeTransducerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chartcommonaddin.h"
#include "RealTimeTransducerDlg.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTOL0
#include "../../Include_Chart/Conversion.h"					// for	CDataConversion

#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRealTimeTransducerDlg dialog

CRealTimeTransducerDlg::CRealTimeTransducerDlg( __int64 &p_nRealTime, __int64 &p_nAddTime, __int64 &p_nFakeTime, CTimeTypeConvert &p_tcTR, CTimeTypeConvert &p_tcReal, CWnd* pParent /*=NULL*/)
	: CDialog(CRealTimeTransducerDlg::IDD, pParent),
	m_nRealTime( p_nRealTime),
	m_nAddTime( p_nAddTime),
	m_nFakeTime( p_nFakeTime),
	m_tcTR( p_tcTR),
	m_tcReal( p_tcReal)
{
	m_pIPacketManager = NULL;
	m_pIRealReceiver = NULL;

	//{{AFX_DATA_INIT(CRealTimeTransducerDlg)
	//}}AFX_DATA_INIT
}

CRealTimeTransducerDlg::~CRealTimeTransducerDlg()
{
	if( m_pIPacketManager) m_pIPacketManager->Release();
	m_pIPacketManager = NULL;
	if( m_pIRealReceiver) m_pIRealReceiver->Release();
	m_pIRealReceiver = NULL;
}


void CRealTimeTransducerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealTimeTransducerDlg)
	DDX_Control(pDX, IDC_CHK_REAL_ONLY, m_btnRealOnly);
	DDX_Control(pDX, IDC_STC_NMTR_TIME, m_stcNMTRTime);
	DDX_Control(pDX, IDC_CMB_CLOCK_ACCELERATOR, m_cmbClockAccelerator);
	DDX_Control(pDX, IDC_STC_NMTR_CLOCK, m_stcNMTRClock);
	DDX_Control(pDX, IDC_CHK_CLOCK_ONLY, m_btnClockOnly);
	DDX_Control(pDX, IDC_CMB_PRICETYPE, m_cbPriceType);
	DDX_Control(pDX, IDC_CMB_SPEED, m_cbRealSpeed);
	DDX_Control(pDX, IDC_CHK_FAKE_REAL_ON, m_btnOnFakeReal);
	DDX_Control(pDX, IDC_EDT_FIXED_REALTIME, m_edtFixedRealTime);
	DDX_Control(pDX, IDC_LST_REAL_PACKET, m_lcRealState);
	DDX_Control(pDX, IDC_STC_FAKE_TIME, m_stcFakeTime);
	DDX_Control(pDX, IDC_STC_ADD_TIME, m_stcAddTime);
	DDX_Control(pDX, IDC_STC_REAL_TIME, m_stcRealTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealTimeTransducerDlg, CDialog)
	//{{AFX_MSG_MAP(CRealTimeTransducerDlg)
	ON_BN_CLICKED(IDC_BTN_SECOND, OnBtnSecond)
	ON_BN_CLICKED(IDC_BTN_MINUTE, OnBtnMinute)
	ON_BN_CLICKED(IDC_BTN_HOUR, OnBtnHour)
	ON_BN_CLICKED(IDC_BTN_DAY, OnBtnDay)
	ON_BN_CLICKED(IDC_BTN_MONTH, OnBtnMonth)
	ON_BN_CLICKED(IDC_BTN_YEAR, OnBtnYear)
	ON_BN_CLICKED(IDC_CHK_FAKE_REAL_ON, OnChkFakeRealOn)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_FIRE_FIXED_REALTIME, OnBtnFireFixedRealtime)
	ON_CBN_SELENDOK(IDC_CMB_SPEED, OnSelendokCmbSpeed)
	ON_CBN_SELENDOK(IDC_CMB_PRICETYPE, OnSelendokCmbPricetype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealTimeTransducerDlg message handlers

void CRealTimeTransducerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
	DestroyWindow();
}
void CRealTimeTransducerDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	DestroyWindow();
}


CString CRealTimeTransducerDlg::GetTimeString( __int64 p_nTime)
{
	int nSecond = int( p_nTime % 100);
	p_nTime /= 100;
	int nMinute = int( p_nTime % 100);
	p_nTime /= 100;
	int nHour = int( p_nTime % 100);
	p_nTime /= 100;
	int nDay = int( p_nTime % 100);
	p_nTime /= 100;
	int nMonth = int( p_nTime % 100);
	int nYear = int( p_nTime / 100);

	CString strTime;
	strTime.Format( "%04d/%02d/%02d, %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMinute, nSecond);
	return strTime;
}


void CRealTimeTransducerDlg::UpdateTime( IPacketManager *p_pIPacketManager, IRealReceiver *p_pIRealReceiver)
{
	int i = 0;
	if( p_pIRealReceiver)
	{
		if( m_pIRealReceiver) m_pIRealReceiver->Release();
		m_pIRealReceiver = p_pIRealReceiver;
		m_pIRealReceiver->AddRef();
	}

	if( g_iMetaTable.IsSpecialTime( int( m_nRealTime % 1000000)))	m_nFakeTime = m_nRealTime;
	else															m_nFakeTime = CDataConversion::CalcTimeSum( m_nRealTime, m_nAddTime);

	if( GetSafeHwnd())
	{
		m_stcRealTime.SetWindowText(	GetTimeString( m_nRealTime));
		m_stcAddTime.SetWindowText(		GetTimeString( m_nAddTime));
		m_stcFakeTime.SetWindowText(	GetTimeString( m_nFakeTime));

		if( p_pIPacketManager)
		{
			if( m_pIPacketManager) m_pIPacketManager->Release();
			m_pIPacketManager = p_pIPacketManager;
			m_pIPacketManager->AddRef();

			// (2009/11/12 - Seung-Won, Bae) Re-Init NMTR Clock Accelerator.
			m_cmbClockAccelerator.ResetContent();
			int nAccelerators[ 28] = 
			{
				1, 5, 10, 15, 20, 30, 40, 60,					// 8
				90, 120,										// 2
				180, 240, 300, 360, 420, 480, 540, 600,			// 8
				1200, 1800, 3600,								// 3
				7200, 10800, 14400, 21600, 43200, 86400, 172800	// 7
			};
			__int64 nSecInterval = m_pIPacketManager->GetTimeIntervalWithSecond();
			CString strAccelerator;
			for(  i = 0; i < 28; i++)
			{
//				if( nSecInterval < nAccelerators[ i]) continue;
				strAccelerator.Format( "%d", nAccelerators[ i]);
				m_cmbClockAccelerator.AddString( strAccelerator);
			}
			m_cmbClockAccelerator.SetCurSel( 0);

			m_lcRealState.DeleteAllItems();

			CString strPacketName, strValue, strCheck, strRDC;
			int nDataCount = 0;
			double dValue = 0;

			_MHWND( GetParent()->GetSafeHwnd());
			ILPCSTR szPacketNames = m_pIPacketManager->GetTRPacketNames();
			CString strPacketNames( szPacketNames);
			i = 0;
			while( !strPacketNames.IsEmpty())
			{
				int nLen = strPacketNames.Find( ';');
				if( nLen < 0) nLen = strPacketNames.GetLength();
				if( 0 < nLen)
				{
					strPacketName = strPacketNames.Left( nLen);
					m_lcRealState.InsertItem( i, _MTOL0( strPacketName));
					if( nLen == strPacketNames.GetLength()) strPacketNames.Empty();
					else									strPacketNames = strPacketNames.Mid( nLen + 1);

					IPacket *pIPacket = m_pIPacketManager->GetPacket( strPacketName);
					if( pIPacket)
					{
						nDataCount = pIPacket->GetDataCount();
						if( 0 < nDataCount)
						{
							if( pIPacket->GetData( nDataCount - 1, dValue))
							{
								strValue.Format( "%0.5f", dValue);
								m_lcRealState.SetItemText( i, 1, strValue);
							}
						}
						
						pIPacket->Release();
					}

					strCheck = "v";
					strRDC = "1";
					strValue.Empty();
					if( _MTEXT( C0_DATE_TIME) == strPacketName)
						strValue = "30";
					else if( _MTEXT( C0_CLOSE) == strPacketName)
					{
						strValue = "5";
						if( 1 == m_cbPriceType.GetCurSel())
						{
							strValue = "0.00005";
							strRDC = "100000";
						}
					}
					else if( _MTEXT( C0_VOLUME) == strPacketName)
						strValue = "10";
					else
					{
						strCheck.Empty();
						strRDC.Empty();
					}
					m_lcRealState.SetItemText( i, 2, strCheck);
					m_lcRealState.SetItemText( i, 3, strValue);
					m_lcRealState.SetItemText( i, 4, strRDC);

					i++;
				}
			}
		}
	}
}

void CRealTimeTransducerDlg::OnBtnSecond() 
{
	// TODO: Add your control notification handler code here
	m_nAddTime = CDataConversion::CalcTimeSum( m_nAddTime, 1);
	UpdateTime();
}

void CRealTimeTransducerDlg::OnBtnMinute() 
{
	// TODO: Add your control notification handler code here
	m_nAddTime -= m_nAddTime % 100;
	m_nAddTime = CDataConversion::CalcTimeSum( m_nAddTime, 100);
	UpdateTime();
}

void CRealTimeTransducerDlg::OnBtnHour() 
{
	// TODO: Add your control notification handler code here
	m_nAddTime -= m_nAddTime % 10000;
	m_nAddTime = CDataConversion::CalcTimeSum( m_nAddTime, 10000);
	UpdateTime();
}

void CRealTimeTransducerDlg::OnBtnDay() 
{
	// TODO: Add your control notification handler code here
	m_nAddTime -= m_nAddTime % 1000000;
	m_nAddTime = CDataConversion::CalcTimeSum( m_nAddTime, 1000000);
	UpdateTime();
}

void CRealTimeTransducerDlg::OnBtnMonth() 
{
	// TODO: Add your control notification handler code here
	m_nAddTime -= m_nAddTime % 100000000;
	m_nAddTime = CDataConversion::CalcTimeSum( m_nAddTime, 100000000);
	UpdateTime();
}

void CRealTimeTransducerDlg::OnBtnYear() 
{
	// TODO: Add your control notification handler code here
	m_nAddTime -= m_nAddTime % 10000000000;
	m_nAddTime = CDataConversion::CalcTimeSum( m_nAddTime, 10000000000);
	UpdateTime();
}

BOOL CRealTimeTransducerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	// Init Real State
	m_lcRealState.InsertColumn( 0, "패킷명", LVCFMT_LEFT, 100);
	m_lcRealState.m_bEditableColumn[ 0] = 0;
	m_lcRealState.InsertColumn( 1, "값", LVCFMT_RIGHT, 100);
	m_lcRealState.InsertColumn( 2, "실시간", LVCFMT_CENTER, 50);
	m_lcRealState.InsertColumn( 3, "Delta", LVCFMT_RIGHT, 60);
	m_lcRealState.InsertColumn( 4, "RDC", LVCFMT_RIGHT, 50);

	CString strSpeed;
	for( int i = 1; i < 11; i++)
	{
		strSpeed.Format( "%d", i);
		m_cbRealSpeed.AddString( strSpeed);
	}
	m_cbRealSpeed.SetCurSel( 0);

	m_cbPriceType.AddString( "주식");
	m_cbPriceType.AddString( "FX");
	m_cbPriceType.SetCurSel( 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRealTimeTransducerDlg::OnChkFakeRealOn() 
{
	// TODO: Add your control notification handler code here

	CString strSpeed;
	m_cbRealSpeed.GetWindowText( strSpeed);
	int nSpeed = atol( strSpeed);
	if( nSpeed < 1) nSpeed = 1;

	srand( (unsigned)time( NULL));
	if( m_btnOnFakeReal.GetCheck())
			SetTimer( 1023, 1000 / nSpeed, NULL);
	else	KillTimer( 1023);
}
void CRealTimeTransducerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer( 1023);
}


void CRealTimeTransducerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);

	if( 1023 != nIDEvent) return;
	FireFakeReal();
}


double CRealTimeTransducerDlg::GetLastPacketValue( const char *p_szPacketName, BOOL p_bPrev)
{
	if( !m_pIPacketManager) return 0.0;

	IPacket *pIPacket = m_pIPacketManager->GetPacket( p_szPacketName);
	if( !pIPacket) return 0.0;

	int nDataCount = pIPacket->GetDataCount();
	if( p_bPrev) nDataCount--;
	double dValue = 0;
	if( 0 < nDataCount) pIPacket->GetData( nDataCount - 1, dValue);
	pIPacket->Release();
	return dValue;
}


void CRealTimeTransducerDlg::UpdateReal( void)
{
	_MHWND( GetParent()->GetSafeHwnd());
	CString strPacketName, strValue;
	double dValue = 0;
	for( int i = 0; i < m_lcRealState.GetItemCount(); i++)
	{
		strPacketName = _LTOM0( m_lcRealState.GetItemText( i, 0));
		dValue = GetLastPacketValue( strPacketName);
		strValue.Format( "%0.5f", dValue);
		m_lcRealState.SetItemText( i, 1, strValue);
	}
}

// (2009/10/27 - Seung-Won, Bae) Fire Fixed Real Time
void CRealTimeTransducerDlg::OnBtnFireFixedRealtime() 
{
	// TODO: Add your control notification handler code here
	FireFakeReal( TRUE);
}
void CRealTimeTransducerDlg::FireFakeReal( BOOL p_bWithFixedTime)
{
	if( !m_pIRealReceiver) return;

	_MHWND( GetParent()->GetSafeHwnd());
	CString strUseReal, strPacketName, strPacketType;
	double dValue = 0, dDelta = 0, dRDC = 1;
	for( int i = 0; i < m_lcRealState.GetItemCount(); i++)
	{
		strUseReal = m_lcRealState.GetItemText( i, 2);
		strUseReal.TrimLeft();
		if( strUseReal.IsEmpty()) continue;

		strPacketName = _LTOM0( m_lcRealState.GetItemText( i, 0));
		dDelta = atof( m_lcRealState.GetItemText( i, 3));
		dDelta = atof( m_lcRealState.GetItemText( i, 3));
		dRDC = atof( m_lcRealState.GetItemText( i, 4));
		if( 0 == dRDC) dRDC = 1;
		
		if( _MTEXT( C0_DATE_TIME) == strPacketName)
		{
			if( p_bWithFixedTime)
			{
				CString strValue;
				m_edtFixedRealTime.GetWindowText( strValue);
				dValue = atof( strValue);
			}
			else
			{
				if( g_iMetaTable.IsSpecialTime( int( m_nRealTime % 1000000)))
				{
					dValue = GetLastPacketValue( strPacketName, TRUE);
					dValue = ( double)m_tcTR.GetFullTypeTime( ( __int64)dValue);
				}
				else dValue = double( m_nRealTime);
				dValue = ( double)CDataConversion::CalcTimeSum( ( __int64)dValue, __int64( ( dDelta + 1) * rand() / RAND_MAX));
				dValue = ( double)m_tcReal.GetPacketTypeTime( ( __int64)dValue);
			}
		}
		else if( _MTEXT( C0_VOLUME) == strPacketName)
		{
			IPacket *pIPacket = m_pIPacketManager->GetPacket( strPacketName);
			if( pIPacket)
			{
				if( pIPacket->IsUseStandardValue( 0))
					dValue = pIPacket->GetStandardValue( 0);
				pIPacket->Release();
			}
			dValue += 1 + dDelta * rand() / RAND_MAX;
		}
		else
		{
			dValue = GetLastPacketValue( strPacketName);
			dValue += ( dDelta * 2) * rand() / RAND_MAX - dDelta;
			dValue *= dRDC;
		}

		m_pIRealReceiver->SetRealDataRQ( "DEFAULT", strPacketName, dValue, TRUE);
	}
	m_pIRealReceiver->SetRealPacketEndRQ( "DEFAULT", TRUE);
}

void CRealTimeTransducerDlg::OnSelendokCmbSpeed() 
{
	// TODO: Add your control notification handler code here

	OnChkFakeRealOn();
}


void CRealTimeTransducerDlg::OnSelendokCmbPricetype() 
{
	// TODO: Add your control notification handler code here
	UpdateTime( m_pIPacketManager);	
}

BOOL CRealTimeTransducerDlg::IsClockTimeOnly( void)
{
	if( !GetSafeHwnd()) return FALSE;
	return m_btnClockOnly.GetCheck();
}


// (2009/11/12 - Seung-Won, Bae) for Clock Monitor
BOOL CRealTimeTransducerDlg::IsNoClockTime( __int64 &p_nRealClockSecs)
{
	if( !GetSafeHwnd()) return FALSE;

	if( m_btnRealOnly.GetCheck())
	{
		if( 0 == p_nRealClockSecs % 5)
		{
			CString strRealClockSecs;
			strRealClockSecs.Format( "%I64d", p_nRealClockSecs);
			m_stcNMTRClock.SetWindowText( strRealClockSecs);
		}
		return TRUE;
	}

	return FALSE;
}
void CRealTimeTransducerDlg::UpdateClockTime( __int64 &p_nRealClockSecs)
{
	if( !GetSafeHwnd()) return;
	
	CString strClockAccelerator;
	m_cmbClockAccelerator.GetWindowText( strClockAccelerator);
	long lClockAccelerator = atol( strClockAccelerator);
	int nMod5 = int( p_nRealClockSecs % 5);
	if( 1 < lClockAccelerator && 0 != nMod5)
	{
		p_nRealClockSecs += lClockAccelerator - 1;
		if( 5 - nMod5 < lClockAccelerator - 1)
			p_nRealClockSecs -= p_nRealClockSecs % 5;
	}

	if( p_nRealClockSecs % 5) return;

	CString strRealClockSecs;
	strRealClockSecs.Format( "%I64d", p_nRealClockSecs);
	m_stcNMTRClock.SetWindowText( strRealClockSecs);

	__int64	nLastClockTime = 0, nLastRealTime = 0;
	m_pIPacketManager->GetLastClockAndRealTime( nLastClockTime, nLastRealTime);
	nLastRealTime = CDataConversion::CalcTimeSum( nLastRealTime, p_nRealClockSecs - nLastClockTime, TRUE);
	m_stcNMTRTime.SetWindowText( GetTimeString( nLastRealTime));
}
