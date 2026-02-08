// SpinCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SpinCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpinCtrl

CSpinCtrl::CSpinCtrl()
{
	m_pfont		= NULL;
	m_bCreateFont	= FALSE;
	m_peditBuddy	= NULL;

	m_dMin		= 0;
	m_dMax		= 0;
	m_dDelta	= 0;
	m_iMin		= 0;
	m_iMax		= 0;
	m_iDelta	= 0;

	m_enumType	= T_NORMAL;
}

CSpinCtrl::~CSpinCtrl()
{
	if (	m_pfont
		&& m_bCreateFont)
	{
		m_pfont->DeleteObject();
		delete m_pfont;
		m_pfont = NULL;
	}
}


BEGIN_MESSAGE_MAP(CSpinCtrl, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CSpinCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpinCtrl message handlers

BOOL CSpinCtrl::Create(	DWORD dwStyle,
			const RECT& rect,
			CWnd* pParentWnd,
			UINT nID,
			CFont* pfont) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (dwStyle == NULL)
	{
		dwStyle =	WS_CHILDWINDOW
				| WS_VISIBLE
				| UDS_ARROWKEYS
				| UDS_ALIGNRIGHT
				| UDS_SETBUDDYINT;
	}

	m_pfont	= pfont;

	return CWnd::Create(UPDOWN_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
}

int CSpinCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSpinButtonCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//return 0;
	return Initialize();
}

int CSpinCtrl::Initialize()
{
	if (m_pfont == NULL)
	{
		m_pfont = (CFont*) new CFont;
		m_pfont->CreatePointFont(90, "±¼¸²Ã¼");
		m_bCreateFont	= TRUE;
	}

	SetFont(m_pfont);

	return 0;
}

void CSpinCtrl::SetAllAccel(int iAccel)
{
	//UDACCEL* pAccel = NULL;
	UDACCEL udaccel[10];
	const int iTotalAccelIndex = GetAccel(10, &udaccel[0]);
	for (int ii = 0; ii < iTotalAccelIndex; ii++)
	{
		udaccel[ii].nInc = iAccel;
	}
	SetAccel(iTotalAccelIndex, &udaccel[0]);
}

void CSpinCtrl::SetFontFromParent(CFont* pfont)
{
	if ( m_pfont && m_bCreateFont)
	{
		m_pfont->DeleteObject();
		delete m_pfont;
	}

	m_pfont = pfont;
	m_bCreateFont = FALSE;
}

void CSpinCtrl::OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (m_peditBuddy)
	{
		CString strText;
		m_peditBuddy->GetWindowText(strText);
		strText.TrimLeft();
		strText.TrimRight();
		strText.Remove(',');
		double dVal = atof(strText);
		int iVal = atoi(strText);

		switch (m_enumType)
		{
		case T_NATURAL:
			if (pNMUpDown->iDelta >= 0)
				iVal += m_iDelta;
			else
				iVal -= m_iDelta;

			if (iVal < m_iMin)
				strText.Format("%d", m_iMin);
			else if (iVal > m_iMax)
				strText.Format("%d", m_iMax);
			else
				strText.Format("%d", iVal);

			break;
		case T_FO:
			if (pNMUpDown->iDelta >= 0)
			{
				if (dVal >= 3)
					dVal += 0.05;
				else
					dVal += 0.01;

			}
			else
			{
				if (dVal > 3)
					dVal -= 0.05;
				else
					dVal -= 0.01;
			}

			if (dVal < m_dMin)
				strText.Format("%0.2f", m_dMin);
			else if (dVal > m_dMax)
				strText.Format("%0.2f", m_dMax);
			else
				strText.Format("%0.2f", dVal);

			break;
		case T_FLOAT:
			if (pNMUpDown->iDelta >= 0)
				dVal += m_dDelta;
			else
				dVal -= m_dDelta;

			if (dVal < m_dMin)
				strText.Format("%f", m_dMin);
			else if (dVal > m_dMax)
				strText.Format("%f", m_dMax);
			else
				strText.Format("%f", dVal);

			break;
		}
		


		m_peditBuddy->SetWindowText(strText);
		*pResult = 1;
		return;
	}
	*pResult = 0;
}

void CSpinCtrl::SetFloatType(CEdit* peditBuddy, double dMin, double dMax, double dDelta)
{
	CSpinButtonCtrl::SetRange(0, 10);
	CSpinButtonCtrl::SetPos(5);

	m_enumType	= T_FLOAT;
	m_peditBuddy	= peditBuddy;
	m_dMin		= dMin;
	m_dMax		= dMax;
	m_dDelta	= dDelta;
}

void CSpinCtrl::SetFutureOptionType(CEdit* peditBuddy)
{
	CSpinButtonCtrl::SetRange(0, 10);
	CSpinButtonCtrl::SetPos(5);

	m_enumType	= T_FO;
	m_peditBuddy	= peditBuddy;
	m_dMin		= 0.01;
	m_dMax		= 999;

}

void CSpinCtrl::SetNaturalType(CEdit* peditBuddy, int iMin, int iMax, int iDelta)
{
	CSpinButtonCtrl::SetRange(0, 10);
	CSpinButtonCtrl::SetPos(5);

	m_enumType	= T_NATURAL;
	m_peditBuddy	= peditBuddy;
	m_iMin		= iMin;
	m_iMax		= iMax;
	m_iDelta	= iDelta;
}

void CSpinCtrl::SetDelta(int iDelta)
{
	m_iDelta = iDelta;
}

void CSpinCtrl::SetDelta(double dDelta)
{
	m_dDelta = dDelta;
}