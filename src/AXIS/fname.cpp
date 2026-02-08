// fname.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "fname.h"

#include <Imm.h>
#include <Windows.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFname dialog


CFname::CFname(CWnd* pParent /*=NULL*/)
	: CDialog(CFname::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFname)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFname::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFname)
	DDX_Control(pDX, IDC_NAME, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFname, CDialog)
	//{{AFX_MSG_MAP(CFname)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_EN_SETFOCUS(IDC_NAME, OnSetfocusName)
	ON_EN_KILLFOCUS(IDC_NAME, OnKillfocusName)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFname message handlers

BOOL CFname::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_edit.LimitText(8);

	char buff[MAX_PATH+1];

	CString file;
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "axisensetup.ini");

	GetPrivateProfileString("DEVRUN","MAPNAME","",buff,MAX_PATH+1,file);

	CString mapN;
	mapN.Format("%s",buff);

	m_edit.SetWindowText(mapN);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFname::OnChangeName() 
{
// 	CString	mapN;
// 	m_edit.GetWindowText(mapN);
// 	if (mapN.GetLength() == 8)
// 	{
// 		m_mapN = mapN;
// 		OnOK();
// 	}
}

void CFname::OnOK() 
{
	CString	mapN;
	
	m_edit.GetWindowText(mapN);
	
	CString s;
	s.Format("DEVRUN : [%s]\n",mapN);
	OutputDebugString(s);
	
	if (mapN.GetLength() == 8)
	{
		m_mapN = mapN;
	}
	OutputDebugString("DEV RUN DIALOG ONOK\n");
	CDialog::OnOK();
}

void CFname::OnSetfocusName() 
{
	DWORD	dwConversion{}, dwSentence{};

	HIMC hImc = ImmGetContext(m_edit.m_hWnd);
	if ( ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		m_Conversion = dwConversion;
		m_Sentence = dwSentence;
		dwConversion = IME_CMODE_ALPHANUMERIC;
		ImmSetConversionStatus(hImc, dwConversion, dwSentence);
	}
}

void CFname::OnKillfocusName() 
{
	DWORD	dwConversion{}, dwSentence{};

	HIMC hImc = ImmGetContext(m_edit.m_hWnd);
	if ( ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		if (m_Conversion != dwConversion)
		{
			dwConversion = m_Conversion;
			dwSentence = m_Sentence;
			ImmSetConversionStatus(hImc, dwConversion, dwSentence);
		}
	}	
}

void CFname::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
// 	CString	mapN;
// 
// 	if(nChar == VK_RETURN)
// 	{
// 		m_edit.GetWindowText(mapN);
// 		
// 		CString s;
// 		s.Format("DEVRUN : [%s]\n",mapN);
// 		OutputDebugString(s);
// 
// 		if (mapN.GetLength() == 8)
// 		{
// 			m_mapN = mapN;
// 			OnOK();
// 		}
// 	}
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
