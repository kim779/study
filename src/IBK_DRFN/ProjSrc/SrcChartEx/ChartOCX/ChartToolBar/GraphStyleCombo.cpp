// GraphStyleCombo.cpp : implementation file
//

#include "stdafx.h"
#include "GraphStyleCombo.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for ML_DEFAULT_RES
#include "ToolBarDlg.h"
#include "Definition.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraphStyleCombo

CGraphStyleCombo::CGraphStyleCombo()
{
	m_pParent = NULL;
}

CGraphStyleCombo::~CGraphStyleCombo()
{
}


BEGIN_MESSAGE_MAP(CGraphStyleCombo, CComboBox)
	//{{AFX_MSG_MAP(CGraphStyleCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphStyleCombo message handlers


/*-----------------------------------------------------------------------------------------
 - Function    :  AddLineType
 - Created at  :  2004-11-01   15:09
 - Author      :  최종찬
 - Description :  콤보박스를 reset시키고 새로운 그래프타입에 따른 String을 넣는다.
 -----------------------------------------------------------------------------------------*/
void CGraphStyleCombo::AddLineType(int nType)
{
	ResetContent();
	for(int i = 0; i < nType; i++)
		AddString(chType[i]);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetGraphType
 - Created at  :  2004-11-01   15:12
 - Author      :  최종찬
 - Description :  콤보박스에 그래프타입을 설정하고 해당타입에 맞는 GraphStyle문자열
					(헤더파일에 있음)을 넣는다.
 -----------------------------------------------------------------------------------------*/
void CGraphStyleCombo::SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex)
{
	GetBitmap().DeleteObject();
	
	ML_SET_DEFAULT_RES();
	if(graphType == CGraphBaseData::Bong_Type)
	{
		if(!GetBitmap().LoadBitmap(IDB_BONG_STYLE))
			return;
		AddLineType(3);
	}
	else if(graphType == CGraphBaseData::Line_Type)
	{
		if(!GetBitmap().LoadBitmap(IDB_LINE_STYLE))	
			return;
		AddLineType(5);
	}
	else if(graphType == CGraphBaseData::Bar_Type)
	{
		if(!GetBitmap().LoadBitmap(IDB_BAR_STYLE))	
			return;
		AddLineType(2);
	}
	else if(graphType == CGraphBaseData::Pie_Type)
	{
		if(!GetBitmap().LoadBitmap(IDB_PIE_STYLE))	
			return;
		AddLineType(2);
	}

	if(nIndex >= GetCount())
		nIndex = 0;
	SetCurSel(nIndex);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnSelchange
 - Created at  :  2004-11-25   15:23
 - Author      :  최종찬
 - Description :  콤보에서 특정 아이템 셀렉트시.
 -----------------------------------------------------------------------------------------*/
void CGraphStyleCombo::OnSelchange() 
{
	int nNum = GetCurSel();
	m_pParent->SetToolFunction(CToolOptionInfo::T_GRAPH_STYLE, NULL, (long)nNum);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2004-11-25   15:24
 - Author      :  최종찬
 - Description :  툴팁을 보여주기 위해 부모클래스 호출
 -----------------------------------------------------------------------------------------*/
void CGraphStyleCombo::OnMouseMove(UINT nFlags, CPoint point) 
{
	CImageComboBox::OnMouseMove(nFlags, point);
}
