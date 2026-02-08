// GraphLineThicknessCombo.cpp : implementation file
//

#include "stdafx.h"
#include "GraphLineThicknessCombo.h"
#include "ToolBarDlg.h"

#include "Definition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphLineThicknessCombo

CGraphLineThicknessCombo::CGraphLineThicknessCombo()
{
}

CGraphLineThicknessCombo::~CGraphLineThicknessCombo()
{
}


BEGIN_MESSAGE_MAP(CGraphLineThicknessCombo, CComboBox)
	//{{AFX_MSG_MAP(CGraphLineThicknessCombo)
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphLineThicknessCombo message handlers


/*-----------------------------------------------------------------------------------------
 - Function    :  SetLineThickness
 - Created at  :  2004-11-03   10:55
 - Author      :  최종찬
 - Description :  라인두께를 설정한다.
 -----------------------------------------------------------------------------------------*/
void CGraphLineThicknessCombo::SetLineThickness(int nThickness)
{
	if(nThickness <= 1)
		SetCurSel(0);
	else
		SetCurSel(nThickness - 1);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  AddLineType
 - Created at  :  2004-11-03   11:04
 - Author      :  최종찬
 - Description :  콤보박스를 reset시키고 새로운 라인두께설정을 Combo에 넣는다.
 -----------------------------------------------------------------------------------------*/
void CGraphLineThicknessCombo::AddLineType(int nType)
{
	ResetContent();
	for (int i = 0; i< nType; i++)
		AddString(szTypeLine[i]);
}



/*-----------------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2004-11-25   15:24
 - Author      :  최종찬
 - Description :  툴팁을 보여주기 위한 부모클래스의 MouseMove호출
 -----------------------------------------------------------------------------------------*/
void CGraphLineThicknessCombo::OnMouseMove(UINT nFlags, CPoint point) 
{
	CImageComboBox::OnMouseMove(nFlags, point);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnSelchange
 - Created at  :  2004-11-25   15:25
 - Author      :  최종찬
 - Description :  콤보의 아이템 선택시.
 -----------------------------------------------------------------------------------------*/
void CGraphLineThicknessCombo::OnSelchange() 
{
	int nNum = GetCurSel();
	m_pParent->SetToolFunction(CToolOptionInfo::T_GRAPH_THICKNESS, NULL, (long)nNum);
}
