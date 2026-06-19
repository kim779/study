// PhonePad.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "PhonePad.h"
#include "trsmain.h"
#include "PhoneThread.h"
#include "axMsg.hxx"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhonePad dialog


CPhonePad::CPhonePad(CWnd* pParent /*=NULL*/)
	: CDialog(CPhonePad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhonePad)

	//}}AFX_DATA_INIT
}


void CPhonePad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhonePad)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhonePad, CDialog)
	//{{AFX_MSG_MAP(CPhonePad)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDCANCEL, OnButton2)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CPhonePad::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL2, &CPhonePad::OnBnClickedCancel2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhonePad message handlers

void CPhonePad::OnButton1() 
{

}

void CPhonePad::OnButton2() 
{

	CDialog::OnCancel();
}

void CPhonePad::OnOK() 
{

	CDialog::OnOK();
}


void CPhonePad::OnBnClickedOk()
{
	CDialog::OnOK();
}


void CPhonePad::OnBnClickedCancel2()
{
	EndDialog(IDCANCEL2);
}
