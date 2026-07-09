
// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information Inc.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic, 
// mechanical, or otherwise without the written permission of Winix Information Inc.
//
//  Components   : listbox.h	AXIS list box information
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

#include "../h/axisvar.h"
#include "form.h"

#pragma	pack(1)
struct	_listH	{
	char	name[L_MAPN+1];		// listbox name
	char	fullN[128];		// PC file full path
	int	itemN;			// # of list items
};

#define	L_LISTH	sizeof(struct _listH)

struct	_itemR	{
	BYTE	kind;				// action kind
	char	entry[80];			// listbox entry text
	BYTE	type;				// resource type
	union	{
		char	sSym[L_NAMEs+1];	// source FORM symbol
		char	listN[L_MAPN+1];	// listbox name
		char	mapN[L_MAPN+1];		// map name
		char	str[80];		// string
	} item;
	char	dSym[L_NAMEs+1];		// dest FORM symbol
};
#define	L_ITEMR	sizeof(struct _itemR)

//
//	_itemR.kind
//
#define	IK_INS			0	// insert string
#define	IK_LIST			1	// listbox
#define	IK_MAP			2	// MAP

//
//	_itemtR.type
//
#define	IT_FORM			0	// FORM symbol
#define	IT_STR			1	// string

#pragma	pack()

//
//	.Name(LISTBOX)
//
//	%Entry(AAAAA)	insert(78000)	to	CODE
//	%Entry(BBBBB)	copy(CODE1)	to	CODE
//	%Entry(CCCCC)	listbox(LISTBOX)
//	%Entry(DDDDD)	select(FP000000)
//	%Entry(EEEEE)	insert(00010)	to	FORM
//	%Entry(FFFFF)	copy(CODE2)	to	FORM
//
