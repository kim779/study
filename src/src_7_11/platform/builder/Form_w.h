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

#ifndef	_FORM_H
#define	_FORM_H

#define	NOVALUE		0xffffffff
#define	L_NAMEs		16		// etc name length
#define	L_FILED		4		// FA_FILE data length

#define	basPRGB		208
#define	basINPUT	192

#pragma	pack(1)
struct	_formR {
	BYTE	kind;			// kind
	char	*name;			// name
	WORD	size;			// text size
					// I/O data size
					// repeat record count field size
					// TAB height

	BYTE	type;			// detail type
					// GRID keeping	column #

	BYTE	iok;			// I/O kind

	DWORD	keys;			// group index of RADIOs
					// BTN_USER key
					// domino index
					// GRAPH relation index
					// COMBO entry count
					// TAB / UTAB pages #
					// GRID cell height
					// EDIT spin delta
					// TABLE line width
	int	left;
	int	top;
	int	right;
	int	bottom;

	BYTE	alignment;		// text alignment
	BYTE	alignImage;		// image alignment
	BYTE	borders;		// border style
	WORD	index;			// tab order index (1base)
	WORD	iorder;			// IO order index (1base)

	WORD	fStyle;			// font Style
	WORD	fPoint;			// font Point
	char	*fName;			// font Name

	DWORD	tRGB;			// text RGB   (0 ~ 255 or RGB value)
	DWORD	pRGB;			// paint RGB  (0 ~ 255 or RGB value)
					// GRID : GO_ROW, first paint RGB
	DWORD	bRGB;			// border RGB (0 ~ 255 or RGB value)
					// FM_PANEL, FM_GRID, FM_TABLE border RGB
	DWORD	stRGB;			//	selected text RGB
	DWORD	spRGB;			//	selected paint RGB

	DWORD	attr;			// attribute
	DWORD	attr2;			// attribute2 : sub attribute

	WORD	properties;		//
					// enable
					// visible
					// checked
					// show hint
					// show help
					// transparent
					// image including
					// hover image
					// sort
					// has lines(indentation)
					// gradient brush

	BYTE	checked;		// default checked index
					// FM_COMBO, FM_TAB, FM_TAB2

	BYTE	isHint;			// hint type
	char	*help;			// hint/help text offset
	char	*auxS;			// keys auxiliary

	char	*scpN;			// temporary script file name
	int	onClick;		// Click information file offset (if -1, not defined)
	int	onClickL;		// Click information data length
	int	onDblClk;		// DblClk information file offset
	int	onDblClkL;		// DblClk information data length
	int	onChange;		// Edit Change information file offset (if -1, not defined)
	int	onChangeL;		// Edit Change information data length
	int	onChar;			// Edit char information file offset (if -1, not defined)
					// alpha/numeric FORM only
	int	onCharL;		// Edit char information data length
	int	onKey;			// Grid Key Change information file offset (if -1, not defined)
	int	onKeyL;			// Grid Key Change information data length

	BYTE	isEdit;			// edit type
	DWORD	onEdit;			// onEdit format string
					// CORGB
					// HIWORD() -> Up   CORGB index
					// LOWORD() -> Down CORGB index
					// GRID : Focus Color
	char	*editS;			// onEdit format string

	BYTE	offs;			// GRID cellR offset
					// OUT float digit #
	char	*refS;			// reference form index

	char	*image;			// default image
	bool	initC;			// if true, form has initial coursor

	char	*str;			// string record
	char	*str2;			// string record
	char	*dat;			// data record

	char*	auxR;			// GRID/TABLE cell record
					// GRAPH record
	bool	block;			// form in block
	bool	drawn;			//
	bool	bCommon;		// properties common
	bool	bFixedSize;		// Object size fix
	long	vals0;			// value tmp

	struct	_formR*	group;		// Group
	struct	_formR* tabN;		// tab form index
	int	pageN;			// tab page
	int	scrollPos;		// scroll pos

	struct	_formR	*next;		// linked list next form point
};

//
//	formR.kind
//
#define	FM_SYS			1	// system
#define	FM_LABEL		2	// label
#define	FM_BOX			3	// box
#define	FM_GROUP		4	// group box
#define	FM_PANEL		5	// panel 3D
#define	FM_BUTTON		6	// buttons
#define	FM_EDIT			7	// edit
#define FM_COMBO		8	// ComboBox
#define FM_OUT			9	// output
#define	FM_MEMO			10	// memo
#define FM_GRID			11	// grid
#define	FM_TABLE		12	// table
#define	FM_TREE			13	// treeview
#define	FM_AVI			14	// AVI
#define FM_GRAPH		15	// graph region
#define FM_OBJECT		16	// unit MAP object
//#define	FM_USEROB		17	// user object
#define FM_TAB			17	// tab
#define FM_UTAB			18	// user tab
#define	FM_SHEET		19	// sheet
#define	FM_BROWSER		20	// browser (file name, url, ... + '\0')
					// initial value available
#define	FM_CONTROL		21	// for DLL

#define	FM_BLOCK		98	// group
#define	FM_SIZE			99	// split resize bar

//
//	formR.kind == FM_SYS
//	formR.type can specify one of the following values
//
#define	SYS_DATE		1	// YYYY/MM/DD
#define	SYS_TIME		2	// HH:MM:SS
#define	SYS_TERM		3	// terminal Name

//
//	formR.kind == FM_EDIT or FM_OUT
//	formR.type can specify one of the following values
//
#define	IO_NUM			1	// numeric I/O
#define	IO_ALPHA		2	// alpha I/O
#define	IO_HAN			3	// hangul I/O
#define	IO_MIXED		4	// mixed I/O
#define	IO_DIGIT		5	// digit I/O (0-9)
#define	IO_ANY			9	// any char. for internal used.

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
#define	PN_LINE3		3	// line : left-bottom-right

//
//	formR.kind == FM_TAB,  FM_UTAB
//	formR.type can specify one of the following values
//
#define	TAB_BORDER		0	// 3D border
#define	TAB_NONE		1	// no 3D border
#define	TAB_OBLIQUE		2	// oblique line tab
#define	TAB_RECT		3	// rect tab
#define	TAB_IMAGE		4	// image tab

//
//	formR.kind == FM_BUTTON
//	formR.type can specify one of the following values
//
#define	BTN_PUSH		1	// push
#define	BTN_RADIO		2	// radio
#define	BTN_CHECK		3	// check
#define BTN_USER		4	// user-Defined

//
//	formR.kind == FM_TREE
//	formR.type can specify onr of the following values
//
#define	TV_FLAT			0	// no indentation treeview
#define	TV_LINE			1	// line indentation

//
//	formR.kind == FM_SHEET
//	formR.type can specify one of the following values
//
#define	SHT_BORDER		0	// 3D border
#define	SHT_SHEET		1	// sheet style

//
//	formR.kind == FM_EDIT, FM_COMBO, FM_BROWSER
//	formR.kind == FM_BUTTON && formR.type == BTN_RADIO, BTN_CHECK
//	formR.iok can specify one of the following values
//
#define	EIO_NOP			0	// NOP edit
#define EIO_INPUT		1	// input mode
#define EIO_INOUT		2	// input / output mode
#define	EIO_OUTPUT		3	// output mode

//
//	formR.kind == FM_LABEL / FM_OUT / FM_SYS / FM_IMAGE / FM_UTAB
//	formR.kind == FM_PANEL / FM_BUTTON / FM_SHEET
//	formR.alignment can specify one of the following values
//
#define	AL_CENTER		0	// center
#define	AL_LEFT			1	// left
#define	AL_RIGHT		2	// right
#define	AL_TOP			3	// top
#define	AL_BOTTOM		4	// bottom
#define	AL_TILE			5	// tile
#define	AL_STRETCH		6	// stretch

//
//	formR.kind == FM_EDIT / FM_PANEL / FM_OUT
//	formR.borders can specify one of the following values
//
#define	BD_NONE			0
#define	BD_RAISED		1	// raised 3D
#define	BD_SUNKEN		2	// sunken 3D
#define	BD_BUMP			3	// bump 3D
#define	BD_ETCHED		4	// etched 3D

//
//	formR.kind == FM_BUTTON, formR.type = BTN_RADIO
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
#define	FA_COMMA	0x00000001	// comma edit
#define	FA_RIGHT	0x00000002	// right justify
					// IO_NUM only

#define	FA_PASSWD	0x00000004	// hidden field  (password)
#define	FA_ZEROSUP	0x00000008	// leading '0' char compress
#define	FA_SEND		0x00000010	// send if full
#define	FA_PROTECT	0x00000020	// protected field
#define	FA_WRAP		0x00000040	// auto wrap field
#define	FA_MAND		0x00000080	// mandatory
#define	FA_HAN		0x00000100	// auto change hangul state

#define	FA_SKIP		0x00000200	// internal field (No I/O)

#define FA_CORGB	0x00000400	// conditional form
#define FA_DOMINO	0x00000800	// domino symbol
#define	FA_REF		0x00001000	// reference
#define	FA_DATE		0x00002000	// date + calendar form(YYYYMMDD)
#define	FA_ZERO		0x00004000	// internal used
					// blank data fill with '0'

#define	FA_FLASH	0x00008000	// flash key field
#define	FA_HORS		0x00010000	// FM_EDIT hScroll
#define	FA_FLOAT	0x00020000	// FM_EDIT/FM_OUT float point
#define	FA_UPPER	0x00040000	// capital case
#define	FA_DRAG		0x00080000	// enable drag

#define	FA_NOR		0x00200000	// ignore OP_SAVE/OP_DOMINO target FORM
					// no relationship with other MAP's FORM
#define	FA_ENUM		0x00400000	// enum properties (GRID/SHEET/TREE/UTAB)
#define	FA_XDELETE	0x00800000	// not delete(apply to user workshop only)

#define	FA_CONTRAST	0x01000000	// contrast data field
#define	FA_PASS		0x02000000	// tab skip
#define	FA_VALIDX	0x08000000	// check validation onTRAN only

#define	FA_XFLOAT	0x10000000	// prohibit key in '.'

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
#define	SA_CORGB	0x000000f0	// CORGB Mask
#define	SA_CORAW	0x00000000	//
#define	SA_COSUP	0x00000010	// supress conditional value
#define	SA_COSIGN	0x00000020	// sign

//
//	formR.attr & FA_SEND
//	formR.attr2 can specify one of the following values
//
#define	SA_SEND		0x00000f00	// SEND Mask
#define	SA_ALL		0x00000000	// send ALL
#define	SA_THIS		0x00000100	// send THIS

//
//	formR.attr & FA_DATE
//	formR.attr2 can specify one of the following values
//
#define	SA_DATE		0x0000f000	// DATE Mask & None
#define	SA_AUTO		0x00000000	// auto date by size (YYYYMMDD, YYMMDD)
#define	SA_YYYYMM	0x00001000	// YYYYMM
#define	SA_YYMM		0x00002000	// YYMM
#define	SA_MMDD		0x00003000	// MMDD (default year calendar)

//
//	formR.attr & FA_CONTRAST
//	formR.attr2 can specify one of the following values
//
#define	SA_CONTRAST	0x000f0000	// CONTRAST Mask
#define	SA_TEXT		0x00000000	// change text RGB
#define	SA_PAINT	0x00010000	// change paint RGB

//
//	formR.properties
//	cellR.properties
//
#define	PR_ENABLE	0x0001		// enable
#define	PR_VISIBLE	0x0002		// visible
#define	PR_CHECKED	0x0004		// checked
#define	PR_TRANSPARENT	0x0008		// transparent
#define	PR_IMAGE	0x0010		// image including
#define	PR_SUM		0x0010		// sum (on RTM)
#define	PR_ACCN		0x0010		// account COMBO
#define	PR_HOVER	0x0020		// hover image
#define	PR_SORT		0x0020		// cellR : sort
#define	PR_LINES	0x0020		// FM_TREE : has lines(indentation)
#define	PR_SPIN		0x0020		// FM_EDIT : has spin control
#define	PR_CODE		0x0040		// FM_EDIT : has code list
#define	PR_SUPRESS	0x0040		// FM_GRID : supress same data
#define	PR_VERT		0x0040		// FM_LABEL, FM_BUTTON, FM_PANEL : vertical text
#define	PR_IMAGETEXT	0x0080		// FM_BUTTON : BTN_PUSH style image+text
#define	PR_LIST		0x0080		// FM_EDIT : input history list
#define	PR_EDIT		0x0100		// show edit format
#define	PR_BOLD		0x0200		// bold font column

//
//	formR.isHint
//	cellR.isHint
//
#define	IH_NOP		0		// NOP
#define	IH_STRING	1		// hint string
#define	IH_REF		2		// hint reference form
#define	IH_MAP		3		// hint map name


//
//	formR.isEdit
//	cellR.isEdit
//
#define IE_NOP		0		// NOP
#define IE_EDIT		1		// I/O edit format
#define IE_CORGB	2		// conditional color
#define IE_CORGB2	9		// conditional color (internal used)

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

	char	name[30];		// name
	WORD	keys;			// COMBO entry count

	BYTE	alignH;			// head alignment
	BYTE	alignC;			// text alignment
	BYTE	alignI;			// image alignment

	WORD	size;
	WORD	width;			// column width
	DWORD	tRGB;
	DWORD	pRGB;

	char	head[64];		// column head offset in strR
					// head | tooltip1 | tooltip2 |....
	DWORD	htRGB;			// column Head text RGB
	DWORD	hpRGB;			// column Head back RGB

	DWORD	attr;
	DWORD	attr2;

	BYTE	isEdit;			// edit type
	DWORD	onEdit;			// edit string
					// CORGB
					// HIWORD() -> Up   CORGB index
					// LOWORD() -> Down CORGB index
	char	editS[32];		// edit string

	WORD	properties;		// visible
					// sort
	DWORD	vals;			// float digit #
	DWORD	vals2;			// match key
	char	auxS[L_NAMEs+1];	// keys auxiliary
	char	refS[L_NAMEs+1];	// reference index in same cellR

	char	dat[512];		// combo style cell display
	char	txt[256];		// button style cell string
	char	str[256];		// combo style cell data
	char	str2[256];		// check style cell string
	char	str3[512];		// button style image cell string
	BYTE	isHint;			// hint type
	char	onHint[1024];		// help text

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

//
//	formR.attr2
//
#define	GO_MULTI	0x00000001	// multi line
#define	GO_FLEX		0x00000002	// flexible row
#define	GO_VSCR		0x00000004	// vertical scroll
#define	GO_HSCR		0x00000008	// horizontal scroll
#define	GO_TOP		0x00000010	// insert top
#define	GO_XHEAD	0x00000020	// none head
#define	GO_X3D		0x00000040	// no 3D effect
#define	GO_ROW		0x00000080	// row color
#define	GO_XVLINE	0x00000100	// no vertial line
#define	GO_XHLINE	0x00000200	// no horizontal line
#define	GO_PAGE		0x00000400	// has page scroll
#define	GO_HEADER	0x00000800	// use grid header format
#define	GO_TABLE	0x00001000	// table (fixed format)
#define	GO_THUMB	0x00002000	// thumb position(not smooth)
#define	GO_KEY		0x00004000	// arrow key focus
#define	GO_FIX		0x00008000	// fix grid to view
#define	GO_CUD		0x00010000	// cud grid
#define	GO_MERGE	0x00020000	// merge head(column or row)
#define	GO_ALWAYS	0x00040000	// selection always
#define	GO_EXPAND	0x00080000	// flexible rows expand(all valid rows)
#define	GO_XSIZE	0x00100000	// prohibit column resizing
#define	GO_AUTO		0x00200000	// auto column width
#define	GO_VALID	0x00400000	// show valid rows only
#define	GO_FTEXT	0x00800000	// keep forecolor on focus
#define	GO_XXLS		0x01000000	// prohibit right-mouse excel menu
#define	GO_3DHEAD	0x02000000	// 3D effect head(only head)

#define	GO_AUTOSIZE	0x80000000	// only workshop auto size

//
//	cellR.kind
//
#define	CK_NORMAL	0		// output
#define	CK_CHECK	1		// edit
#define	CK_COMBO	2		// combobox
#define	CK_BUTTON	3		// push button
#define	CK_GRAPH	9		// simple graph

//
//	cellR.kind == CK_GRAPH
//	cellR.type can specify one of the following values
#define	CK_BAR		0		// bar chart
#define	CK_LINE		1		// line chart


//
//	AXIS system internal reserved symbol
//
#define	InterestGrid	"!CODES"	// 관심종목 GRID
#define	InterestCodes	"!INTEREST"	// 관심종목 TREE,		tab/!Interest
#define	InterestGroup	"!GROUP"	// 관심그룹 TAB,		tab/!Group
#define	InterestFolder	"!FOLDER"	// 관심그룹폴더 TREE,		user/!Folder??
#define	AccountTree	"!ACCOUNTS"	// 관리계좌 TREE,		tab/!Accounts
#define	AccountGrid	"!ACCOUNT"	// 계좌정보 GRID
#define	AccountNo	"!ACCNO"	// 계좌번호 CELL
#define	AccountPass	"!ACCPWD"	// 계좌비밀번호
#define	PushCodes	"!PUSH"		// 변동종목 GRID
#define	DropCaption	"!CAPTION"	// Drag&Drop 화면 Caption
#define	GarbageCan	"!CAN"		// 휴지통
#define	GarbageKey	"!CANK"		// 휴지통 폴더 


#pragma	pack()

#endif
