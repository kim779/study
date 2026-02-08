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
//  Components   : grid.h	AXIS GRID FORM information header
//  Rev. History :
//  		  Ver	 Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2001-08	Initial version
//*****************************************************************************
#pragma once

#pragma	pack(1)

//------------------------------------------------------------------------------
//	INBOUND format
//
struct	_gridHi	{
	char	visible[2]{};		// # of visible rows
	char	rows[4]{};		// # of maximum rows
	char	type{};			// type
					// '0' : fixed GRID
					// '1' : flexible GRID
	char	dir{};			// update direction(real-tile)
					// '1' : top update
					// '2' : bottom update
	char	sort{};			// sort direction
					// '0' : disable
					// '1' : ascending
					// '2' : descending
	char	symbol[16]{};		// sort column symbol name string
	char	key{};			// event key
					// '0' : enter(default)
					// '1' : page up
					// '2' : page dn
					// '3' : scroll up
					// '4' : scroll dn
					// '5' : first/end
					// '6' : sort column
	char	page[4]{};
	char	save[80]{};
};



//------------------------------------------------------------------------------
//	OUTBOUND format
//
struct	_gridHo	{
	char	mode{};			// mode
					// '0' : update
					// '1' : insert top
					// '2' : append tail
	char	sort{};			// sort direction
					// '0' : disable
					// '1' : ascending
					// '2' : descending
	char	symbol[16]{};		// sort column symbol name string
	char	stat{};			// default value bit OR operation
					// 0x40 : default, normal transaction
	char	page[4]{};		// script reference value
	char	save[80]{};
};

//
//	gridHo.stat can be any combination of the following values.
//
#define	statPREV		0x01	// PREV(page up, scroll up) enable
#define	statNEXT		0x02	// NEXT(page dn, scroll dn) enable
#define	statSORT		0x04	// by server sort enable


#pragma	pack()


