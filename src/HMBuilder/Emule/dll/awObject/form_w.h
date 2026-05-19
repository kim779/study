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
#define	L_FILED		4		// FA_FILE data length

#define	basPRGB		208
#define	basINPUT	192


#pragma	pack(1)

#define _last

#pragma	pack(1)
struct	_formR {
	BYTE	kind;			// kind
	char*	name;			// name
	char*   rts;

	WORD	size;			// text size
					// I/O data size
					// FM_GRID : repeat record count field size
					// FM_BOX / FM_OUT : line thickness
	BYTE	type;	// detail type
					// FM_GRID : keeping column #
					
	BYTE	iok;	// I/O kind
					
	DWORD	keys;	
					// group index of RADIOs
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
							// onResize GRID view type (center/top/bottom valid)
	BYTE	borders;		// FM_OUT : border style
	WORD	index;			// tab order index (1 base)
	WORD	iorder;			// I/O order index (1 base)

	BYTE	fStyle;			// font style
	BYTE	fPoint;			// font size : point

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

	
	
	
	DWORD	attr;			// attribute
	DWORD	attr2;			// attribute2 : sub attribute
					// GRID options

	WORD	properties;
	BYTE	checked;		/// default checked index
					// COMBO, TAB, UTAB


	char*	desc;	// description : voice reading description
					// FM_IMAGEVW / FM_OUT / FM_EDIT / FM_LABEL / FM_COMBO
					// FM_CHECK / FM_RADIO

	char*	auxS;			// FM_OUT   : domino symbol
					// FM_EDIT  : Encrypt key symbol
					// FM_RADIO : group symbol
///////  script ////////////////////////////////////////////////////////////
//	BYTE	isHint;			// hint type
//	DWORD	onHint;			// hint offset
////////////////////////////////////////////////////////////////////////////
	BYTE	isEdit;			// edit type
							// FM_GRID : focus option
	DWORD	onEdit;			// IE_CORGB
							// HIWORD() -> Up   color index
							// LOWORD() -> Down color index
							// FM_GRID : focus color


	
	char*	editS;			// onEdit format string

	BYTE	offs;			// FM_EDIT / FM_OUT : float digit #

	char*	str;			// string record
					// FM_LABEL
					// FM_EDIT : initial string
					// FM_OBJECT : mapname
					// FM_BROWSER : start page
					// FM_BUTTON / FM_CHECK / FM_RADIO : display string
					// FM_GRID : variant
					// FM_COMBO / FM_TAB : data strings

	char*	str2;			// string record
					// FM_IMAGEVW / FM_CHECK / FM_EDIT : image file name
					// FM_LABEL / FM_TAB : image file name

	char*	dat;			// data record
					// FM_EDIT : hint text
					// FM_COMBO / FM_TAB : display strings

	char*	auxR;			// FM_GRID / FM_TABLE : cell record (struct _repR*)

	char*	scpN;			// temporary script file name
	int	onTap;			// TAP information file offset (if -1, not defined)
					// FM_PANEL / FM_EDIT / FM_LABEL / FM_BUTTON / FM_TAB
					// FM_CHECK / FM_RADIO / FM_GRID / FM_TABLE
	int	onTapL;			// Tap information data length

	int	onLongTap;		// Long Tap information file offset (if -1, not defined)
					// FM_PANEL / FM_LABEL / FM_BUTTON / FM_TAB
					// FM_CHECK / FM_RADIO / FM_TABLE / FM_GRID
	int	onLongTapL;		// Long Tap information data length

	int	onChange;		// Edit Change information file offset (if -1, not defined)
					// FM_EDIT / FM_GRID / FM_COMBO
	int	onChangeL;		// Edit Change information data length

	int	onChar;			// Edit char information file offset (if -1, not defined)
					// FM_EDIT / FM_GRID
	int	onCharL;		// Edit char information data length

	bool	block;			// form in block
	bool	drawn;			//
	bool	bCommon;		// properties common

	int	scrollPos;		// scroll pos
	struct	_formR*	group;		// Group
	struct	_formR*	next;		// linked list next form point

	/*
	DWORD	vals[4];// vals[0]
					// COMBO data
					// OBJECT mapname
					// GRID, TABLE cell record
					// TAB  pageR record
					// UTAB id  : "Visible(1/0):ID"
					// default image

					// vals[1]
					// COMBO text
					// EDIT/OUT float digits #
					// TAB/UTAB caption text
					// GRID LOWORD(row), HIWORD(col)

					// vals[2]
					// GRID init string
					// GRID, TAB variant offset
					// reference form index

					// vals[3]
					// GRID : GO_ROW, 1st/2nd paint RGB offset
					// TAB, UTAB : focus text/paint RGB offset
					*/
	// 테이블과 그리드 사용 
	int row;	
	int col;
	int hHeight;
	int rowof;
	int frow;
};

#define	L_FORMR	sizeof(struct _formR)



#define	FM_LABEL		1	// label
#define	FM_BOX			2	// box
#define	FM_GROUP		3	// group box
#define	FM_IMAGEVW		4	// image view
#define	FM_BUTTON		5	// button
#define	FM_CHECK		6	// checkbox
#define	FM_RADIO		7	// radio button
#define	FM_EDIT			8	// edit
#define FM_COMBO		9	// ComboBox
#define FM_OUT			10	// output
#define	FM_MEMO			11	// memo
#define FM_GRID			12	// grid
#define	FM_TABLE		13	// table
#define FM_OBJECT		14	// unit MAP object
#define FM_TAB			15	// tab
#define	FM_BROWSER		16	// browser (file name, url, ... + '\0')
#define	FM_CONTROL		17	// control
#define FM_TRINFO		18  // TRINFO


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
//	formR.kind == FM_LABEL / FM_OUT
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
//	formR.kind == FM_RADIO
//	formR.borders can specify one of the following values
//
#define	RD_NORMAL		0	// normal RADIO
#define	RD_PUSH			1	// push-like RADIO

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
#ifndef _last
#define	FA_COMMA	0x00000001	// comma edit
#define	FA_RIGHT	0x00000002	// right justify
					// IO_NUM only
#define	FA_PASSWD	0x00000004	// hidden field  (password)
#define	FA_ZEROSUP	0x00000008	// leading '0' char compress
//#define	FA_SEND		0x00000010	// send if full
#define	FA_PROTECT	0x00000020	// protected field
#define	FA_WRAP		0x00000040	// auto wrap field
//#define	FA_MAND		0x00000080	// mandatory
//#define	FA_HAN		0x00000100	// auto change hangul state
#define	FA_SKIP		0x00000200	// internal field (No I/O)
#define FA_CORGB	0x00000400	// conditional form
#define	FA_CONTRAST	0x01000000	// contrast data field
#define FA_DOMINO	0x00000800	// domino symbol
#define	FA_FLOAT	0x00020000	// FM_EDIT/FM_OUT float point
#define	FA_UPPER	0x00040000	// capital case
#define	FA_NOR		0x00200000	// ignore OP_SAVE/OP_DOMINO target FORM
					// no relationship with other MAP's FORM
#define	FA_PASS		0x02000000	// tab skip

//   FA _FLOATx  기능 넣어 주어야 함....   

//#define	FA_REF		0x00001000	// reference
//#define	FA_DATE		0x00002000	// date + calendar form(YYYYMMDD)
//#define	FA_ZERO		0x00004000	// internal used
					// blank data fill with '0'
//#define	FA_FLASH	0x00008000	// flash key field
//#define	FA_HORS		0x00010000	// FM_EDIT hScroll
//#define	FA_DRAG		0x00080000	// enable drag
//#define	FA_ENUM		0x00400000	// enum properties (GRID/SHEET/TREE/UTAB)
//#define	FA_XDELETE	0x00800000	// not delete(apply to user workshop only)
//#define	FA_VALIDX	0x08000000	// check validation onTRAN only
//#define	FA_XFLOAT	0x10000000	// prohibit key in '.'

//
#else
//
//	formR.attr can be any combination of the following values.
//
#define	FA_COMMA	0x00000001	// comma edit
#define	FA_PASSWD	0x00000002	// hidden field  (password)
#define	FA_ZEROSUP	0x00000004	// leading '0' char compress
#define	FA_PROTECT	0x00000008	// protected field
#define	FA_WRAP		0x00000010	// auto wrap field
#define	FA_SKIP		0x00000020	// internal field (No I/O)

#define FA_CORGB	0x00000040	// conditional form
//#define	FA_CONTRAST	0x00000080	// contrast data field
#define FA_DOMINO	0x00000100	// domino symbol
#define	FA_FLOAT	0x00000200	// FM_EDIT/FM_OUT/FM_GRID float point
#define	FA_FLOATx	0x00000400	// FM_OUT/FM_GRID ignore floating 0
#define	FA_UPPER	0x00000800	// capital case
#define	FA_NOR		0x00001000	// ignore OP_SAVE/OP_DOMINO target FORM
					// no relationship with other MAP's FORM
#define	FA_PASS		0x00002000	// tab order skip

#define FA_RIGHT	0x00004000
#endif
//
//	formR.attr & FA_ZEROSUP
//	formR.attr2 can specify one of the following values
//
#define	SA_ZEROSUP	0x0000000f	// ZERO supress mask
#define	SA_SPACE	0x00000000	//
#define	SA_ASTER	0x00000001	// '*'
#define	SA_ZERO		0x00000002	// all zero convert into '0'
		     					// single '0'

//
//	formR.attr & FA_CORGB
//	formR.attr2 can specify one of the following values
//
#define	SA_CORGB	0x00000ff0	// CORGB Mask
#define	SA_CORAW	0x00000010	//
#define	SA_COSUP	0x00000020	// supress conditional value
#define	SA_COSIGN	0x00000030	// sign
#define SA_TEXT		0x00000040  // 
#define SA_PAINT	0x00000050	// 

#ifndef _last
//
//	formR.attr & FA_SEND
//	formR.attr2 can specify one of the following values
//
//#define	SA_SEND		0x00000f00	// SEND Mask
//#define	SA_ALL		0x00000000	// send ALL
//#define	SA_THIS		0x00000100	// send THIS

//
//	formR.attr & FA_DATE
//	formR.attr2 can specify one of the following values
//
//#define	SA_DATE		0x0000f000	// DATE Mask & None
//#define	SA_AUTO		0x00000000	// auto date by size (YYYYMMDD, YYMMDD)
//#define	SA_YYYYMM	0x00001000	// YYYYMM
//#define	SA_YYMM		0x00002000	// YYMM
//#define	SA_MMDD		0x00003000	// MMDD (default year calendar)

#endif

//
//	formR.attr & FA_CONTRAST
//	formR.attr2 can specify one of the following values
//
//#define	SA_CONTRAST	0x000f0000	// CONTRAST Mask
//#define	SA_TEXT		0x00000000	// change text RGB
//#define	SA_PAINT	0x00010000	// change paint RGB


//
//	formR.properties
//	cellR.properties
//

#ifndef _last
#define	PR_ENABLE	0x0001		// enable
#define	PR_VISIBLE	0x0002		// visible
#define	PR_CHECKED	0x0004		// checked
#define	PR_IMAGE	0x0010		// image including
#define	PR_IMAGETEXT	0x0080		// FM_BUTTON : BTN_PUSH style image+text
#define	PR_HOVER	0x0020		// hover image
#define	PR_SORT		0x0020		// cellR : sort
#define	PR_EDIT		0x0100		// show edit format



//#define	PR_TRANSPARENT	0x0008		// transparent
//#define	PR_SUM		0x0010		// sum (on RTM)
//#define	PR_ACCN		0x0010		// account COMBO
//#define	PR_LINES	0x0020		// FM_TREE : has lines(indentation)
//#define	PR_GRADIENT	0x0020		// gradient brush
//#define	PR_SPIN		0x0020		// FM_EDIT : has spin control
//#define	PR_CODE		0x0040		// FM_EDIT : has code list
//#define	PR_SUPRESS	0x0040		// FM_GRID : supress same data
//#define	PR_VERT		0x0040		// FM_LABEL, FM_BUTTON, FM_PANEL : vertical text
//#define	PR_LIST		0x0080		// FM_EDIT : input history list
#else

//
//	formR.properties
//	cellR.properties
//
#define	PR_ENABLE		0x00000001	// enable
#define	PR_VISIBLE		0x00000002	// visible
#define	PR_CHECKED		0x00000004	// FM_CHECK / FM_RADIO : checked
					// FM_GRID : column option, exclusive check
#define	PR_IMAGE		0x00000010	// image including
#define	PR_IMAGETEXT	0x00000020	// image + text including
#define	PR_HOVER		0x00000080	// FM_IMAGEVW / FM_BUTTON : hover image

#define	PR_SORT			0x00000040	// cellR : sort
#define	PR_EDIT			0x00000080	// show edit format
#define PR_FIXED		0x00000200 // tab Fixed
#endif



//
//	formR.isEdit
//	cellR.isEdit
//
#define IE_NOP		0		// NOP
#define IE_EDIT		1		// I/O edit format
#define IE_CORGB	2		// conditional color
#ifdef _last
#define IE_CORGB2	9		// conditional color (internal used)
#endif


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

struct	_precellR {
	BYTE	kind;			// kind
	BYTE	type;			// data type
	BYTE	iok;			// I/O kind

	DWORD	name;			// name
	DWORD	keys;			// domino index in same cellR
							// COMBO entry count
	BYTE	alignH;			// head alignment
	BYTE	alignC;			// text alignment

	WORD	size;
	WORD	width;			// column width
	DWORD	tRGB;
	DWORD	pRGB;

	DWORD	head;			// column head offset in strR
	DWORD	htRGB;			// column Head text RGB
	DWORD	hpRGB;			// column Head back RGB

	DWORD	attr;
	DWORD	attr2;

	BYTE	isHint;			// hint type
	DWORD	onHint;			// hint offset

	BYTE	isEdit;			// edit type
	DWORD	onEdit;			// onEdit format string
							// IE_CORGB  : value offset, Up/Down RGB value

	
	WORD	properties;		// visible
					// sort
					
	DWORD	vals[3];		// vals[0]
				// reference index in same cellR
				// COMBO style cell data offset
				// CHECK, BUTTON string
					
				// vals[1]
				// float digits #
				// COMBO style cell display offset
				// BUTTON image files

				// vals[2]
				// COMBO dropdown key
};

struct	_cellR {

	BYTE	kind;			// kind
	BYTE	type;			// data type
	BYTE	iok;			// I/O kind

	char	name[32];		// name
	char	rts[15];
	DWORD	keys;			// domino index in same cellR
							// COMBO entry count
	BYTE	alignH;			// head alignment
	BYTE	alignC;			// text alignment

	WORD	size;
	WORD	width;			// column width
	DWORD	tRGB;
	DWORD	pRGB;

	char	head[64];		// column head offset in strR
	DWORD	htRGB;			// column Head text RGB
	DWORD	hpRGB;			// column Head back RGB

	DWORD	attr;
	DWORD	attr2;


	BYTE	isEdit;			// edit type
	DWORD	onEdit;			// onEdit format string
							// IE_CORGB  : value offset, Up/Down RGB value

	char	editS[64];		// edit string
	char	symbol[20];

	
	WORD	properties;		// visible
					// sort



	/*
	DWORD	vals[3];		// vals[0]
				// reference index in same cellR
				// COMBO style cell data offset
				// CHECK, BUTTON string
					
				// vals[1]
				// float digits #
				// COMBO style cell display offset
				// BUTTON image files

				// vals[2]
				// COMBO dropdown key
				*/


	DWORD	digit;			// float digit #
	DWORD	vals2;			// match key
	char	auxS[L_NAMEs+1];	// domino column symbol

	char	dat[1024];		// COMBO display string
	char	txt[256];		// BUTTON string
	char	str[1024];		// COMBO data string
	char	str2[256];		// CHECK  display string
	char	str3[512];		// image file name
	char	desc[1024];		// description

	//BYTE	headN;			// head line #
	BYTE	lineN;			// line number
	bool	block;			// multi selection
	struct	_cellR	*next;		// linked list next cell point


};



#define	L_CELLR	sizeof(struct _precellR)

struct	_repR {
	WORD	rows;			// # of rows (GRID only)
	BYTE	cols;			// # of cols
	int	selected;		// selected cell #

	struct	_precellR	*cellR;
};


//
//	formR.attr2
//

#ifndef _last
#define	GO_MULTI	0x00000001	// multi line
#define	GO_FLEX		0x00000002	// flexible row
#define	GO_VSCR		0x00000004	// vertical scroll
#define	GO_HSCR		0x00000008	// horizontal scroll
#define	GO_TOP		0x00000010	// insert top
#define	GO_XHEAD	0x00000020	// none head
#define	GO_ROW		0x00000080	// row color
#define	GO_XVLINE	0x00000100	// no vertial line
#define	GO_XHLINE	0x00000200	// no horizontal line
#define	GO_HEADER	0x00000800	// use grid header format
#define	GO_FIX		0x00008000	// fix grid to view
#define	GO_MERGE	0x00020000	// merge head(column or row)
#define	GO_VALID	0x00400000	// show valid rows only
#define	GO_AUTO		0x00200000	// auto column width



#define	GO_X3D		0x00000040	// no 3D effect&
#define	GO_PAGE		0x00000400	// has page scroll
#define	GO_TABLE	0x00001000	// table (fixed format)
#define	GO_THUMB	0x00002000	// thumb position(not smooth)
#define	GO_KEY		0x00004000	// arrow key focus
#define	GO_ALWAYS	0x00040000	// selection always
#define	GO_EXPAND	0x00080000	// flexible rows expand(all valid rows)
#define	GO_XSIZE	0x00100000	// prohibit column resizing
#define	GO_FTEXT	0x00800000	// keep forecolor on focus
//#define	GO_XXLS		0x01000000	// prohibit right-mouse excel menu



#else
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




//**********************************************************************
/// 사용하지 않음.... 
#define	GO_X3D		0x00000040	// no 3D effect&
#define	GO_PAGE		0x00000400	// has page scroll
#define	GO_TABLE	0x00001000	// table (fixed format)
#define	GO_THUMB	0x00002000	// thumb position(not smooth)
#define	GO_KEY		0x00004000	// arrow key focus
#define	GO_ALWAYS	0x00040000	// selection always
#define	GO_EXPAND	0x00080000	// flexible rows expand(all valid rows)
#define	GO_XSIZE	0x00100000	// prohibit column resizing
#define	GO_FTEXT	0x00800000	// keep forecolor on focus
//**********************************************************************
#endif

//
//	cellR.kind
//
#define	CK_NORMAL	0		// normal
#define	CK_CHECK	1		// check
#define	CK_COMBO	2		// combobox
#define	CK_BUTTON	3		// push button
#define	CK_CHART	9		// simple graph


//
//	formR.kind == FM_BUTTON
//	formR.type can specify one of the following values
//
#define	BTN_PUSH		1	// push
#define	BTN_RADIO		2	// radio
#define	BTN_CHECK		3	// check
#define BTN_USER		4	// user-defined


//**************************************************************
//#ifndef _last

struct	_pageR {
	int	formN;				// # of FORMs in page
	DWORD	formR;			// VALUE offset
};

#define	L_PAGER	sizeof(struct _pageR)


//
//	script variable definition
//

//
//	target
//
#define	targetTHIS	0		// target THIS
#define	targetMAIN	1		// target MAIN
#define	targetALL	2		// target ALL

//
//	depth
//
#define	dpCURRENT	0		// access current level form / variable
#define dpPARENT	1		//        parent
#define dpGRAND		2		//        grandparent
#define dpGREAT		3		//        great-grandparent
#define dpMAIN		4		//        main
#define dpALL		5		//        all


//
//	formR.kind == FM_BOX
//	formR.type can specify one of the following values
//
#define	BX_LINE			0	// line
#define	BX_BORDER		1	// 3D border
#define	BX_2CLR			2	// 2 colored box

//
//	formR.kind == FM_PANEL
//	formR.type can specify one of the following values
//
#define	PN_BORDER		0	// 3D border
#define	PN_LINE			1	// line
#define	PN_NONE			2	// no border

//
//	formR.kind == FM_TAB,  FM_UTAB
//	formR.type can specify one of the following values
//
#define	TAB_BORDER		0	// 3D border
#define	TAB_NONE		1	// no 3D border
#define	TAB_OBLIQUE		2	// oblique line tab
#define	TAB_RECT		3	// rect tab
#define	TAB_IMAGE		4	// image tab


//#endif
//**********************************************************************
#pragma	pack()


