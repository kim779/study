// SetupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SetupDialog.h"
#include "afxdialogex.h"


// CSetupDialog dialog

IMPLEMENT_DYNAMIC(CSetupDialog, CDialog)

CSetupDialog::CSetupDialog(UINT idResource, CWnd* pParent /*=NULL*/)
	: CDialog(idResource, pParent)
{
	m_resourceID = idResource;
}

CSetupDialog::~CSetupDialog()
{
}

void CSetupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetupDialog, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CSetupDialog message handlers


BOOL CSetupDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	/* --test
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;
	}
	else
		*/
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}


HBRUSH CSetupDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
	/*
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	*/
}


BOOL CSetupDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSetupDialog::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CSetupDialog::DefaultSetup()
{
}

void CSetupDialog::ApplySetup()
{
}