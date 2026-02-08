// XXGrid.cpp: implementation of the CXXGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB0000A1.h"
#include "XXGrid.h"
#include "libDefine.h"
#include "../../control/fx_grid/InPlaceEdit.h"

#define	IDC_INPLACE_EDIT	8

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXXGrid::CXXGrid()
{
	m_nPassCol = -1;
	m_temp = _T("");
	m_pEdit = NULL;
	m_bEdit = false;
}

CXXGrid::~CXXGrid()
{

}

BEGIN_MESSAGE_MAP(CXXGrid, CfxGrid)
	//{{AFX_MSG_MAP(CfxGrid)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+100, OnEditEnd)
END_MESSAGE_MAP()


LONG CXXGrid::OnEditEnd(WPARAM wParam, LPARAM lParam)
{
	m_bEdit = false;
	return TRUE;
}

void CXXGrid::CreateInPlaceEdit(CRect& rect, DWORD dwStyle, UINT nID,
					int nRow, int nCol, LPCTSTR szText, int limit, int nChar)
{
	if (dwStyle & ES_UPPERCASE)		// 20070510 계좌번호 대문자로 입력되는 오류 수정
		dwStyle &= ~ES_UPPERCASE;
	LOGFONT* logfont = GetItemFont(nRow, nCol);
	if (m_nPassCol == nCol)
	{
		dwStyle |= ES_PASSWORD;
		limit = 10;
	}
	else
	{
		switch (nCol)
		{
		case IECOL_RATE:
			limit = 5;
			break;
		case IECOL_ALIAS:
			limit = 18;
			break;
		}
	}
	CString	sztmp = _T("");
	sztmp.Format("%s", szText);
	
	if (!sztmp.IsEmpty() && m_nPassCol == nCol)
	{
		sztmp.Format("%s", m_temp);
	}

	m_pEdit = new CInPlaceEdit(this, rect, dwStyle, nID, nRow, nCol, sztmp.operator LPCTSTR(), limit, nChar, logfont);
	m_bEdit = true;
}


CString CXXGrid::GetPass()
{ 
	CString pass = _T("");

	if (m_pEdit && IsWindow(m_pEdit->m_hWnd) && m_bEdit)	
	{
		m_pEdit->GetWindowText(pass);
	}
	m_temp = _T("");
	return pass;
}

