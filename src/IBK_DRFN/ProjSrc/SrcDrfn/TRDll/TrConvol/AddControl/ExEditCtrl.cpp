// ExEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ExEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExEditCtrl

CExEditCtrl::CExEditCtrl()
{
	m_nMaxVal = 0;

}

CExEditCtrl::~CExEditCtrl()
{
}


BEGIN_MESSAGE_MAP(CExEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CExEditCtrl)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExEditCtrl message handlers

void CExEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if ((nChar >= '0') && (nChar <= '9'))
	{
		//if (m_nMaxIndex

		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if (nChar == VK_BACK)
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CExEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	SetEditVal(nChar);
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExEditCtrl::OnKillfocus() 
{
	SetEditVal(VK_RETURN);
}

void CExEditCtrl::SetEditVal(UINT nChar)
{
	CString strTemp;
	int nData;

	GetWindowText(strTemp);
	nData = atoi(strTemp);
	if (nChar == VK_UP) 
	{
		nData++;
		if (nData < m_nMaxVal)
		{
			strTemp.Format("%d", nData);
			SetWindowText(strTemp);

			m_pbtnSpin->SetPos(nData);
		}
	}
	else if (nChar == VK_DOWN) 
	{
		nData--;
		if (nData > -1)
		{
			strTemp.Format("%d", nData);
			SetWindowText(strTemp);

			m_pbtnSpin->SetPos(nData);
		}
	}
	else
	{
		if ((nData < m_nMaxVal) && (nData > -1))
			m_pbtnSpin->SetPos(nData);
		else
		{
			if (nData >= m_nMaxVal)
			{
				strTemp.Format("%d", m_nMaxVal);
				SetWindowText(strTemp);

				nData = m_nMaxVal;
			}
			else if (nData < -1)
			{
				SetWindowText("0");

				nData = 0;
			}

			m_pbtnSpin->SetPos(nData);
		}
	}
}


int CExEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}


