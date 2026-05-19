
// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information Inc.
//
//  Components   : mapxml.h	AXIS MAP format
//  Rev. History :
//  		  Ver	 Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************
#pragma once
#define GLOBAL_DIVISION		"GLOBAL_DIVISION"
#define FORM_DIVISION		"FORM_DIVISION"
#define RESOURCE_DIVISION	"RESOURCE_DIVISION"
#define TRLAYOUT_DIVISION	"TRLAYOUT_DIVISION"
#define RTS_DIVISION		"RTS_DIVISION"
#define LAYOUTEDIT_DIVISION	"LAYOUTEDIT_DIVISION"
#define COLINFO			"COLINFO"

#define LABEL		"LABEL"
#define BOX		"BOX"
#define GROUP		"GROUP"
#define PANEL		"PANEL"
#define BUTTON		"BUTTON"
#define EDIT		"EDIT"
#define COMBO		"COMBO"
#define OUTPUT		"OUT"
#define MEMO		"MEMO"
#define GRID		"GRID"
#define GRIDEX		"GRIDEX"
#define TABLE		"TABLE"
#define TREEVIEW	"TREEVIEW"
#define OBJECT		"OBJECT"
#define USRTAB		"USRTAB"
#define BROWSER		"BROWSER"
#define CONTROL		"CONTROL"
#define TP_PUSH		"TP_PUSH"
#define TP_RADIO	"TP_RADIO"
#define TP_CHECK	"TP_CHECK"

class ElementNode
{
public:
	CString strElement;
	CString strValue;
	//CArray<ElementNode*, ElementNode*> arySub;
	ElementNode()
	{
	}
};

typedef CArray<ElementNode*, ElementNode*> CNode;