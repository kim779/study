// PacketUserDefine.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "PacketUserDefine.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketUserDefine dialog


CPacketUserDefine::CPacketUserDefine( HWND p_hOcxWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CPacketUserDefine::IDD, pParent)
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = p_hOcxWnd;

	//{{AFX_DATA_INIT(CPacketUserDefine)
	m_strPacketName = _T("");
	m_strFieldName = _T("");
	m_bCheckReal = FALSE;
	m_strLength = _T("");
	m_nPacketType = -1;
	m_strDetailPacketType = _T("");
	//}}AFX_DATA_INIT
}


void CPacketUserDefine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPacketUserDefine)
	DDX_Control(pDX, IDC_DETAIL_PACKETTYPE, m_ComboDetailPacketType);
	DDX_Text(pDX, IDC_EDIT1, m_strPacketName);
	DDX_Text(pDX, IDC_EDIT7, m_strFieldName);
	DDX_Check(pDX, IDC_CHECK1, m_bCheckReal);
	DDX_Text(pDX, IDC_EDIT10, m_strLength);
	DDX_CBIndex(pDX, IDC_PACKETTYPE, m_nPacketType);
	DDX_CBString(pDX, IDC_DETAIL_PACKETTYPE, m_strDetailPacketType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPacketUserDefine, CDialog)
	//{{AFX_MSG_MAP(CPacketUserDefine)
	ON_CBN_SELCHANGE(IDC_PACKETTYPE, OnSelchangePackettype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketUserDefine message handlers

void CPacketUserDefine::OnSelchangePackettype() 
{
	for(int i=0 ;i < m_ComboDetailPacketType.GetCount();)
	   m_ComboDetailPacketType.DeleteString(i);

	int nIndex = ((CComboBox* )GetDlgItem(IDC_PACKETTYPE))->GetCurSel();
	if(nIndex == 0) 
		WhenPacketTypeisDataDate();
	else if(nIndex == 1) 
		WhenPacketTypeisNumeral();
	else if(nIndex == 2) 
		WhenPacketTypeisEtc();
}

void CPacketUserDefine::WhenPacketTypeisDataDate()
{
	m_ComboDetailPacketType.AddString("YYYYMMDD");
	m_ComboDetailPacketType.AddString("YYMMDD");
	m_ComboDetailPacketType.AddString("YYYYMM");
	m_ComboDetailPacketType.AddString("YYMM");
	m_ComboDetailPacketType.AddString("MMDD");
	m_ComboDetailPacketType.AddString("MMDDHH");
	m_ComboDetailPacketType.AddString("MMDDHHMM");
	m_ComboDetailPacketType.AddString("DDHH");
	m_ComboDetailPacketType.AddString("DDHHMM");
	m_ComboDetailPacketType.AddString("HHMMSS");
//	m_ComboDetailPacketType.AddString("HHMMSSNN");
	m_ComboDetailPacketType.SetCurSel(0);
}

void CPacketUserDefine::WhenPacketTypeisNumeral()
{
	m_ComboDetailPacketType.AddString("x 0.0001");
	m_ComboDetailPacketType.AddString("x 0.001");		
	m_ComboDetailPacketType.AddString("x 0.01");
	m_ComboDetailPacketType.AddString("x 0.1");
	m_ComboDetailPacketType.AddString("x 1");
	m_ComboDetailPacketType.AddString("x 100");
	m_ComboDetailPacketType.AddString("x 1000");
	m_ComboDetailPacketType.AddString("x 100000");
	m_ComboDetailPacketType.AddString("x 100000000");
	m_ComboDetailPacketType.SetCurSel(4);
}

void CPacketUserDefine::WhenPacketTypeisEtc()
{
	m_ComboDetailPacketType.AddString( _LTEXT6( C6_CHARACTER));
	m_ComboDetailPacketType.AddString("x 0.01%");
	m_ComboDetailPacketType.AddString("x 0.1%");
	m_ComboDetailPacketType.AddString("x 1%");
	m_ComboDetailPacketType.AddString( _LTEXT6( C6_POINT));
	m_ComboDetailPacketType.AddString("$");
	m_ComboDetailPacketType.AddString("กอ");
	m_ComboDetailPacketType.AddString("กฬ");
	m_ComboDetailPacketType.SetCurSel(1);
}
