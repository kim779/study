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
//  Components   : form.h	AXIS FORM object
//  Rev. History :
//  		  Ver	 Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
// *****************************************************************************

#pragma once

#define	NOVALUE		0xffffffff
#define	L_NAMEs		16		// etc name length

#define	basPRGB		208
#define	basINPUT	192

#pragma	pack(1)
struct	_formR {
	BYTE	kind;			// kind
	char*	name;			// name
	WORD	size;			// text size
					// I/O data size
					// FM_GRID : repeat record count field size
					// FM_BOX / FM_OUT : line thickness

	BYTE	type;			// detail type
					// FM_GRID : keeping column #

	BYTE	iok;			// I/O kind

	DWORD	keys;			// group index of RADIOs
					// domino index
					// FM_COMBO : LOWORD - entry count, HIWORD - visible count
					// FM_TAB   : LOWORD - pages count, HIWORD - visible count
					// FM_GRID  : LOWORD - cell height, HIWORD - visible count
					// FM_TABLE : cell height
	int	left;
	int	top;
	int	right;
	int	bottom;

	BYTE	alignment;		// text alignment
	BYTE	alignImage;		// image alignment
	BYTE	borders;		// FM_OUT : border style
	WORD	index;			// tab-order index (1base)
	WORD	iorder;			// IO-order index (1base)

	WORD	fStyle;			// font Style
	WORD	fPoint;			// font Point
	char*	fName;			// font Name

	// color value (0 ~ 255 or ARGB value)
	DWORD	tRGB;			// text color
	DWORD	pRGB;			// paint color
	DWORD	bRGB;			// border color (0 ~ 255 or ARGB value)
					// FM_GRID / FM_TABLE / FM_OUT
	DWORD	stRGB;			// FM_TAB  : selected text color
					// FM_GRID : 1st row paint color
	DWORD	spRGB;			// FM_TAB  : selected paint color
					// FM_GRID : 2nd row paint color
	DWORD	opRGB;			// FM_GRID / FM_COMBO : option color

	DWORD	sfpRGB;	// Selected font color

	int	alpha;			// Alpha

	DWORD	attr;			// attribute
	DWORD	attr2;			// attribute2 : sub attribute

	DWORD	properties;		// properties

	BYTE	checked;		// default checked index
					// FM_COMBO / FM_TAB

	BYTE	cursor;			// cursor type on MouseOver	// for HTS
	BYTE	isHint;			// hint type			// for HTS
	char	*help;			// hint/help text offset	// for HTS


	char*	desc;			// description : voice reading description
					// FM_IMAGEVW / FM_OUT / FM_EDIT / FM_LABEL / FM_COMBO
					// FM_CHECK / FM_RADIO

	char*	auxS;			// FM_OUT   : domino symbol
					// FM_EDIT  : Encrypt key symbol
					// FM_RADIO : group symbol

	BYTE	isEdit;			// edit type
					// FM_GRID : focus option
	DWORD	onEdit;			// IE_CORGB
					// HIWORD() -> Up   color index
					// LOWORD() -> Down color index
					// FM_GRID : focus color
	char*	editS;			// onEdit format string

	BYTE	offs;			// FM_EDIT / FM_OUT : float digit # / tab fixed alignment

	char*	str;			// string record
					// FM_EDIT : initial string
					// FM_OBJECT : mapname
					// FM_BROWSER : start page
					// FM_BUTTON / FM_CHECK / FM_RADIO : display string
					// FM_GRID : variant
					// FM_COMBO / FM_TAB : data strings

	char*	str2;			// string record
					// FM_IMAGEVW / FM_CHECK / FM_RADIO / FM_EDIT : image file name
					// FM_LABEL / FM_TAB / FM_COMBO : image file name
	char*	optionimg;		// FM_TAB / FM_COMBO : Option Image Path
					// FM_BUTTON / FM_CHECK / FM_RADIO :  icon image
	BYTE	optionPos;		// FM_GRID : Option Image Position
	BYTE	selectShape;		// FM_GRID : Select Shape

	BYTE	blinkShape;		// FM_GRID : Blink Shape

	char*	optionimg2;		// FM_COMBO : Option Image Path
	char*	optionimg3;		// FM_COMBO : Option Image Path

	char*	dat;			// data record
					// FM_EDIT : hint text
					// FM_COMBO / FM_TAB : display strings
					// FM_CONTROL : symbol key

	char*	auxR;			// FM_GRID / FM_TABLE : cell record (struct _repR*)

	int	fixrow;			// fixed row
	int	fixcol;			// fixed col
	int	headH;			// head height
	int	rowof;			// row of line
	int	row;
	int	col;

	char*	rtssy;			// rts symbol

	char*	scpN;			// temporary script file name
	int	onTap;			// TAP information file offset (if -1, not defined)
					// FM_IMAGEVIEW / FM_EDIT / FM_LABEL / FM_BUTTON / FM_TAB
					// FM_CHECK / FM_RADIO / FM_GRID / FM_TABLE
	int	onTapL;			// Tap information data length
	int	onLongTap;		// Long Tap information file offset (if -1, not defined)
					// FM_IMAGEVIEW / FM_LABEL / FM_BUTTON / FM_TAB
					// FM_CHECK / FM_RADIO / FM_TABLE / FM_GRID
	int	onLongTapL;		// Long Tap information data length
	int	onChange;		// Edit Change information file offset (if -1, not defined)
					// FM_EDIT / FM_GRID / FM_COMBO
	int	onChangeL;		// Edit Change information data length
	int	onChar;			// Edit char information file offset (if -1, not defined)
					// FM_EDIT / FM_GRID
	int	onCharL;		// Edit char information data length
	int	onDrag;			// GRID
	int	onDragL;		// GRID
	int	onScroll;		// GRID
	int	onScrollL;		// GRID

	// for HTS
	int	onKey;			// Grid Key Change information file offset (if -1, not defined)
	int	onKeyL;			// Grid Key Change information data length
	int	onSetFocus;		// OnSetFocus information file offset (if -1, not defined)
	int	onSetFocusL;		// OnSetFocus information data length
	int	onEnter;		// OnMouseEnter information file offset (if -1, not defined)
	int	onEnterL;		// OnMouseEnter information data length
	int	onLeave;		// OnMouseLeave information file offset (if -1, not defined)
	int	onLeaveL;		// OnMouseLeave inforamtion data length

	// Layout
	CMapStringToPtr*  layout; 
	CStringArray*	margin;		// margin

	char	*refS;			// reference form index

	bool	block;			// form in block
	bool	drawn;			//
	bool	bCommon;		// properties common

	int	scrollPos;		// scroll pos
	struct	_formR*	group;		// Group
	struct	_formR*	next;		// linked list next form point
};


//
//	formR.kind
//
#define	FM_LABEL		1	// label
#define	FM_BOX			2	// box
#define	FM_GROUP		3	// group box
#define	FM_IMAGEVIEW		4	// image view
#define	FM_BUTTON		5	// button
#define	FM_CHECK		6	// checkbox
#define	FM_RADIO		7	// radio button
#define	FM_EDIT			8	// edit
#define FM_COMBO		9	// ComboBox
#define FM_OUT			10	// output
#define	FM_GRID			11	// grid
#define FM_GRIDEX		12	// gridex
#define	FM_TABLE		13	// table
#define FM_OBJECT		14	// unit MAP object
#define FM_TAB			15	// tab
#define	FM_BROWSER		16	// browser (file name, url, ... + '\0')
#define	FM_CONTROL		17	// control
		
//
//	formR.kind == FM_EDIT / FM_OUT / FM_GRID.cell
//	formR.type can specify one of the following values
//
#define	IO_NUM			1	// numeric I/O
#define	IO_ALPHA		2	// alpha I/O
#define	IO_HAN			3	// hangul I/O
#define	IO_MIXED		4	// mixed I/O
#define	IO_DIGIT		5	// digit I/O (0-9)
#define	IO_ANY			9	// any char. for internal used.


//
//	formR.kind == FM_EDIT / FM_COMBO / FM_BROWSER
//	formR.kind == FM_RADIO / FM_CHECK / FM_GRID.cell
//	formR.iok can specify one of the following values
//
#define	EIO_NOP			0	// NOP edit
#define EIO_INPUT		1	// input mode
#define EIO_INOUT		2	// input / output mode
#define	EIO_OUTPUT		3	// output mode


//
//	formR.kind == FM_LABEL / FM_EDIT / FM_OUT
//	formR.alignment can specify one of the following values
//
#define	AL_CENTER		0	// center
#define	AL_LEFT			1	// left
#define	AL_RIGHT		2	// right
#define	AL_TOP			3	// top
#define	AL_BOTTOM		4	// bottom


//
//	formR.kind == FM_BUTTON / FM_IMAGEVW
//	formR.alignment can specify one of the following values
//
#define	IA_STRETCH		0	// stretch
#define	IA_CENTER		1	// center
#define	IA_TILE			2	// tile

//
//	formR.kind == FM_OUT
//	formR.borders can specify one of the following values
//
#define	BD_NONE			0
#define	BD_RAISED		1	// raised 3D
#define	BD_SUNKEN		2	// sunken 3D
#define	BD_BUMP			3	// bump 3D
#define	BD_ETCHED		4	// etched 3D
#define	BD_LINE			5	// line


//
//	formR.fStyle
//
#define	FS_NONE			0	// use mapH.font
#define	FS_NORM			1	// normal font
#define	FS_ITALIC		2	// italic font
#define	FS_BOLD			3	// bold font
#define	FS_IBOLD		4	// italic bold font


//
//	formR.attr can be any combination of the following values.
//
#define	FA_COMMA	0x00000001	// comma edit
#define	FA_PASSWD	0x00000002	// hidden field  (password)
#define	FA_ZEROSUP	0x00000004	// leading '0' char compress
#define	FA_PROTECT	0x00000008	// protected field
#define	FA_WRAP		0x00000010	// auto wrap field
#define	FA_SKIP		0x00000020	// internal field (No I/O)
#define	FA_CORGB	0x00000040	// conditional form

#define FA_DOMINO	0x00000100	// domino symbol
#define	FA_FLOAT	0x00000200	// FM_EDIT/FM_OUT/FM_GRID float point
#define	FA_FLOATx	0x00000400	// FM_OUT/FM_GRID ignore floating 0
#define	FA_UPPER	0x00000800	// capital case
#define	FA_NOR		0x00001000	// ignore OP_SAVE/OP_DOMINO target FORM
					// no relationship with other MAP's FORM
#define	FA_PASS		0x00002000	// tab order skip
#define FA_LANGUAGE	0x00004000	// Multi language




//
//	formR.attr & FA_ZEROSUP
//	formR.attr2 can specify one of the following values
//
#define	SA_ZEROSUP	0x0000000f	// ZERO supress mask
#define	SA_SPACE	0x00000000	//
#define	SA_ASTER	0x00000001	// '*'
#define	SA_ZERO		0x00000002	// all zero convert into '0'
					// single '0'

//	formR.attr & FA_CORGB
//	formR.attr2 can specify one of the following values

#define	SA_CORGB	0x00000ff0	// CORGB Mask
#define	SA_CORAW	0x00000010	//
#define	SA_COSUP	0x00000020	// suppress conditional value
#define	SA_COSIGN	0x00000030	// sign
#define	SA_TEXT		0x00000040	// change text RGB
#define	SA_PAINT	0x00000050	// change paint RGB
#define SA_TEXTSUP	0x00000060	// 
#define SA_PAINTSUP	0x00000070	// 

//
//	formR.properties
//	cellR.properties
//
#define	PR_ENABLE	0x00000001	// enable
#define	PR_VISIBLE	0x00000002	// visible
#define	PR_CHECKED	0x00000004	// FM_CHECK / FM_RADIO : checked
					// FM_GRID : column option, exclusive check
#define	PR_MARGIN	0x00000008	// define margin
#define	PR_IMAGE	0x00000010	// image including
#define	PR_IMAGETEXT	0x00000020	// image + text including
#define	PR_HOVER	0x00000080	// FM_IMAGEVW / FM_BUTTON : hover image

#define	PR_SORT		0x00000040	// cellR : sort	// tab fixed width // out coRGB
#define	PR_EDIT		0x00000080	// show edit format
#define PR_FIXED	0x00000200	// tab Fixed
#define	PR_DRAGROW	0x00000400	// grid column drag

#define PR_OVERLAYIMG	0x00000800	// FM_CHECK : overlay Image Setting
#define PR_TABSET	0x00000800	// FM_TAB   : Tab Image Setting
#define	PR_RESIZE	0x00000800	// FM_OUT   : text resize in rect

#define PR_SCREFECT	0x00001000	// FM_GRID/FM_GRIDEX : scroll effect
#define PR_REFRESH      0x00002000	// FM_GRID/FM_GRIDEX : pull to refresh
#define PR_LANGUAGE	0x00003000	// FM_BUTTON/FM_OUTPUT/FM_GRID  MULTI_LANGUAGE



//
//
//

//
//	formR.kind == FM_GRID
//	formR.isEdit
//
#define	FI_NONE		0		// NONE
#define	FI_CELL		1		// cell focus
#define	FI_RBOX		2		// row box focus
#define	FI_RBAR		3		// row bar focus, bar color : onEdit
#define	FI_RREC		4		// record bar focus, bar color : onEdit


#define	MAX_COLUMNs	32

struct	_cellR {
	BYTE	kind;			// kind
	BYTE	type;			// data type
	BYTE	iok;			// I/O kind

	char	name[32];		// name
	WORD	keys;			// COMBO entry count

	BYTE	alignH;			// head alignment
	BYTE	alignC;			// text alignment

	WORD	size;
	WORD	width;			// column width
	DWORD	tRGB;
	DWORD	pRGB;

	char	head[64];		// column head offset in strR
	DWORD	htRGB;			// column Head text RGB
	DWORD	hpRGB;			// column Head back RGB

	WORD	fStyle;			// font Style
	WORD	fPoint;			// font Point
	char*	fName;			// font Name

	WORD	height;			// column height in multiline
	WORD	headH;			// head height in multiline

	DWORD	attr;
	DWORD	attr2;
	char	rtssy[256];		// rts symbol

	BYTE	isEdit;			// edit type
	DWORD	onEdit;			// edit string
					// CORGB
					// HIWORD() -> Up   CORGB index
					// LOWORD() -> Down CORGB index
	char	editS[32];		// edit string

	DWORD	properties;		// visible
					// sort
	DWORD	vals;			// float digit #
	DWORD	vals2;			// match key
	char	auxS[L_NAMEs+1];	// domino column symbol

	char	dat[512];		// COMBO display string, CUSTOM object name
	char	txt[256];		// BUTTON string
	char	str[256];		// COMBO data string, CUSTOM object property
	char	str2[256];		// CHECK  display string
	char	str3[256];		// image file name
	char	str4[256];		// head image file name
	char	merge[512];		// Merge Column Name

	char	desc[1024];		// description

	DWORD	chartLimit;		// chart Limit

	CStringArray*	margin;

	BYTE	drag;			// Drag Row index
	//BYTE	headN;			// head line #
	BYTE	lineN;			// line number
	bool	block;			// multi selection
	struct	_cellR	*next;		// linked list next cell point
};

struct	_repR {
	WORD	rows;			// # of rows (GRID only)
	BYTE	cols;			// # of cols
	int	selected;		// selected cell #

	struct	_cellR	*cellR;
};


struct	_Layout {
	int	left;
	int	top;
	int	right;
	int	bottom;

	bool	bfirst;
	DWORD	properties;
};


//
//	formR.attr2
//	grid option
//
#define	GO_MULTI	0x00000001	// multi line
#define	GO_FLEX		0x00000002	// flexible row
#define	GO_VSCR		0x00000004	// vertical scroll
#define	GO_HSCR		0x00000008	// horizontal scroll
#define	GO_TOP		0x00000010	// insert top
#define	GO_XHEAD	0x00000020	// none head
#define	GO_ROW		0x00000040	// row color
#define	GO_XVLINE	0x00000080	// no vertial line
#define	GO_XHLINE	0x00000100	// no horizontal line
#define	GO_HEADER	0x00000200	// use grid header format
#define	GO_FIX		0x00000400	// fix grid to view
#define	GO_MERGE	0x00000800	// merge head(column or row)
#define	GO_VALID	0x00001000	// show valid rows only
#define	GO_AUTOSIZE	0x00002000	// only workshop auto size
#define GO_XINLINE	0x00004000	// FM_GRIDEX : Is Multi_Line Row Merge?
#define GO_DRAGCOL	0x00008000	// DRAGCOL
#define	GO_BLINK	0x00010000	// rts blink

/*
#define GO_FOCUSTEXT	0x00004000	// focustext
#define GO_KEYFOCUS	0x00008000	// keyfocus
#define GO_SCROLLVALID	0x00010000	// scroll valied row
*/

//
//	cellR.kind
//
#define	CK_NORMAL	0		// edit / output
#define	CK_CHECK	1		// check
#define	CK_COMBO	2		// combobox
#define	CK_BUTTON	3		// push button
#define	CK_CUSTOM	4		// custom object
#define	CK_CHART	9		// chart
#define	CK_C_CHART	10		// Candle chart


#pragma	pack()
