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
//  Components   : fmProperties.h	AXIS FORM object class
//  Rev. History :
//  		  Ver	 Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2000-07	Initial version
//		 02.00	2001-04	Update  version
//		 03.00	2003-01 Update  version (P/F Upgrade)
//		 03.01	2007-12 Update  version (Header Upgrade)
//		 03.05	2014-08 Integrated version for HTS & MTS
// *****************************************************************************
#pragma once
#include "StdAfx.h"

class CfmProperties : public CObject
{
public:
	CfmProperties()
	{
		kind			= 0;
		name			= _T("");
		size			= 0;
		charType		= 0;
		ioType			= 0;
		keyValues		= 0;
		rect.SetRectEmpty();
		alignment		= 0;
		alignmentImage		= 0;
		borderStyle		= 0;
		index			= 0;
		iorder			= 0;
		fontName		= _T("");
		fontPoint		= 0;
		fontStyle		= 0;
		textColor		= 0;
		paintColor		= 0;
		borderColor		= 0;
		paintColor1		= 0;
		paintColor2		= 0;
		attributes		= 0;
		auxAttributes		= 0;
		styles			= 0;
		checked			= 0;
		typeCursor		= 0;
		typeHint		= 0;
		hintText		= _T("");
		keyNames		= _T("");
		whatEdit		= 0;
		auxValues		= 0;
		editFormat		= _T("");
		offsets			= 0;
		strings			= _T("");
		displays		= _T("");
		images			= _T("");
		auxImages		= _T("");
		posImage		= 0;
		cellProperties		= NULL;
		fixRow			= 0;
		fixColumn		= 0;
		headHeight		= 0;
		rowLines		= 0;
		flashKeys		= _T("");
		referenceName		= _T("");
		keyParam		= 0;
	}
public:
	char	kind;			// kind of FORM
#define		fmLABEL		10	// label
#define		fmBOX		11	// box
#define		fmGROUP		12	// group box
#define		fmIMVIEW	13	// image view/panel 3D
#define		fmEDIT		14	// edit
#define		fmOUTPUT	15	// output
#define		fmBUTTON	16	// push button
#define		fmCHECK		17	// check button
#define		fmRADIO		18	// radio button
#define		fmCOMBO		19	// combobox
#define		fmTABLE		20	// table
#define		fmTREE		21	// treeview
#define		fmOBJECT	22	// embedded custom object
#define		fmFORM		23	// unit FORM object
#define		fmTAB		24	// tab
#define		fmBROWSER	25	// browser (file name, url, ... + '\0')
#define		fmMEMO		26	// memo
#define		fmGRID		27	// grid
#define		fmGRIDEX	28	// gridEx

	CString	name;			// FORM's name
	int	size;			// size of text
					// I/O data size
					// GRID : total row count
					// BOX,OUTPOUT : outline thickness

	char	charType;		// type of display character
#define		ctNUMERIC	1	// numeric
#define		ctALPHA		2	// alpha
#define		ctMULTI		3	// multi byte
#define		ctMIXED		4	// mixed
#define		ctDIGIT		5	// digit (0-9)

#define		ctDATE		1	// YYYY/MM/DD
#define		ctTIME		2	// HH:MM:SS
#define		ctTERM		3	// terminal name

#define		bxLINE		0	// line style box
#define		bxBORDER	1	// 3D style box

#define		tvFLAT		0	// no indentation treeview
#define		tvLINE		1	// line indentation

	char	ioType;			// I/O attributes type
#define		ioFREE		0	//
#define		ioINPUT		1	// in stream
#define		ioOUTPUT	2	// out stream
#define		ioBOTH		3	// in/out stream

	DWORD	keyValues;		// domino index
					// LOWORD(keyValues) = COMBO entry count, HIWORD(keyVaules) = COMBO visible count
					// LOWORD(keyValues) = TAB pages count,   HIWORD(keyVaules) = TAB visible count
					// LOWORD(keyValues) = GRID cell height,  HIWORD(keyVaules) = GRID visible count
					// LOWORD(keyValues) = TABLE cell height

	DWORD	keyParam;		// COMBO spread short key
					// EDIT spin kind
	CRect	rect;
	char	alignment;		// text alignment
	char	alignmentImage;		// image alignment
#define		atCENTER	0	//
#define		atLEFT		1	//
#define		atRIGHT		2	//
#define		atTOP		3	//
#define		atBOTTOM	4	//
#define		atSTRETCH	5	// stretch
#define		atTILE		6	// tile

	char	borderStyle;		// OUTPUT border style
#define		bsNONE		0	//
#define		bsRAISED	1	// raised 3D
#define		bsSUNKEN	2	// sunken 3D
#define		bsBUMP		3	// bump 3D
#define		bsETCHED	4	// etched 3D
#define		bsLINE		5	// line

	int	index;			// tab-order index (1base)
	int	iorder;			// io-order index (1base)

	CString	fontName;
	int	fontPoint;
	char	fontStyle;
#define		fsNONE		0	// not defined...use FORM's font
#define		fsNORMAL	1	// normal font
#define		fsITALIC	2	// italic font
#define		fsBOLD		3	// bold font
#define		fsIBOLD		4	// italic bold font

	// color's value (0 ~ 255 or PALETTERGB value)
	DWORD	textColor;
	DWORD	paintColor;
	DWORD	borderColor;		// GRID, TABLE, OUTPUT
	DWORD	paintColor1;		// TAB = focus text color, GRID = 1st row's paint color
	DWORD	paintColor2;		// TAB = focus paint color, GRID = 2nd row's paint color

	DWORD	attributes;
#define		faCOMMA		0x00000001	// comma edit
#define		faPASSWD	0x00000002	// password edit *
#define		faZEROSUP	0x00000004	// leading '0' char compress
#define		faPROTECT	0x00000008	// protected field
#define		faWRAP		0x00000010	// auto wrap field
#define		faSKIP		0x00000020	// internal field (No I/O)
#define		faMULTI		0x00000040	// auto change multi-byte state
#define		faCORGB		0x00000080	// conditional form
#define		faDOMINO	0x00000100	// domino symbol
#define		faFLOAT		0x00000200	// EDIT/OUT float point
#define		faFLOATx	0x00000400	// EDIT/OUT ignore floating '0'
#define		faUPPER		0x00000800	// capital case
#define		faNOR		0x00001000	// ignore OP_SAVE/OP_DOMINO target FORM
						// no relationship with other MAP's FORM
#define		faPASS		0x00002000	// tab skip
#define		faXFLOAT	0x00004000	// prohibit key in '.'
//V3.0
#define		faENUM		0x00010000	// enum properties (GRID/TREE/TAB)
#define		faXDELETE	0x00020000	// not delete(apply to user workshop only)
#define		faCONTRAST	0x00040000	// contrast data field
#define		faMARKET	0x00080000	// code market field
#define		faVALIDX	0x00100000	// check validation onTRAN only
#define		faSAVE		0x00200000	// save data field
#define		faRIGHT		0x00400000	// right justify
#define		faSEND		0x00800000	// send if full
#define		faMUST		0x01000000	// mandatory
#define		faREF		0x02000000	// reference
#define		faFLASH		0x04000000	// flash key field
#define		faDATE		0x08000000	// date + calendar form(YYYYMMDD)
#define		faZERO		0x10000000	// internal used, blank data fill with '0'
#define		faHORS		0x20000000	// EDIT hScroll
#define		faDRAG		0x40000000	// enable drag
#define		faDROP		0x80000000	// enable DRAG&DROP (GRID only)

	DWORD	auxAttributes;
#define		getTypeOfZeroSupress(x)	(x&0x0000000f)		// type of faZEROSUP
#define		spSPACE		0		// ' '
#define		spASTER		1		// '*'
#define		spZERO		2		// all '0' convert into single '0'

#define		getTypeOfConditionColor(x) (x&0x000000f0)	// type of faCORGB or faCONTRAST
#define		ccRAW		0x10		// raw
#define		ccSUP		0x20		// supress conditional value
#define		ccSIGN		0x30		// sign
#define		ccTEXT		0x40		// change text RGB
#define		ccPAINT		0x50		// change paint RGB
#define		ccTEXTSUP	0x60		// change color & supress sign
#define		ccPAINTSUP	0x70		// change color & supress sign

//V3.0
#define		getTypeOfSendMethod(x)	(x&0x00000f00)		// type of SEND
#define		setTypeOfSendMethod(x, s) ((x&0xfffff0ff)|s)
#define		smALL		0x00000000	// send ALL
#define		smTHIS		0x00000100	// send THIS

#define		getTypeOfDateFormat(x)	(x&0x0000f000)		// type of faDATE
#define		dfAUTO		0x00000000	// auto date by size (YYYYMMDD, YYMMDD)
#define		dfYYYYMM	0x00001000	// YYYYMM
#define		dfYYMM		0x00002000	// YYMM
#define		dfMMDD		0x00003000	// MMDD (default year calendar)

//GRID attributes only
#define		gaMULTI		0x00000001	// multi line
#define		gaVSCR		0x00000002	// vertical scroll
#define		gaHSCR		0x00000004	// horizontal scroll
#define		gaTOP		0x00000008	// insert top
#define		gaXHEAD		0x00000010	// none head
#define		gaROW		0x00000020	// row color
#define		gaXVLINE	0x00000040	// no vertial line
#define		gaXHLINE	0x00000080	// no horizontal line
#define		gaHEADER	0x00000100	// use grid header format
#define		gaFIX		0x00000200	// fix grid to view
#define		gaMERGE		0x00000400	// merge head(column or row)
#define		gaAUTO		0x00000800	// auto column width
#define		gaVALID		0x00001000	// show valid rows only
#define		gaXINLINE	0x00002000	// GRIDEX is multi-line row merge
#define		gaDRAG		0x00004000	// drag enable column
//V3.0
#define		gaFLEX		0x00010000	// flexible row
#define		gaX3D		0x00020000	// no 3D effect
#define		gaPAGE		0x00040000	// has page scroll
#define		gaTABLE		0x00080000	// table (fixed format)
#define		gaTHUMB		0x00100000	// thumb position(not smooth)
#define		gaKEY		0x00200000	// arrow key focus
#define		gaCUD		0x00400000	// cud GRID
#define		gaALWAYS	0x00800000	// selection always
#define		gaEXPAND	0x01000000	// flexible rows expand(all valid rows)
#define		gaXSIZE		0x02000000	// prohibit column resizing
#define		ga3DHEAD	0x04000000	// 3D effect head(only head)
#define		gaFTEXT		0x08000000	// keep forecolor on focus
#define		gaXXLS		0x10000000	// prohibit right-mouse excel menu
#define		gaXBORDER	0x20000000	// no border

	DWORD	styles;		// properties styles
#define		stENABLE	0x00000001	// enable
#define		stVISIBLE	0x00000002	// visible
#define		stCHECKED	0x00000004	// checked(CHECK, RADIO), GRID=column option exclusive check
#define		stIMAGE		0x00000008	// image including
#define		stIMAGETEXT	0x00000010	// image/text including
#define		stHOVER		0x00000020	// hover image
#define		stSORT		0x00000020	// cellR : sort
#define		stSPIN		0x00000020	// EDIT has spin control
#define		stEDIT		0x00000040	// show edit format
#define		stCODE		0x00000080	// EDIT has code list
#define		stSUPRESS	0x00000080	// GRID supress same data
#define		stDATE		0x00000100	// EDIT has date control
#define		stFIXTAB	0x00000200	// TAB fixed
#define		stDRAGROW	0x00000400	// GRID drag row
#define		stIMOVERLAY	0x00000800	// CHECK overlay image
#define		stTABS		0x00001000	// TAB setup button
#define		stDRAG		0x00002000	// enable drag
#define		stSUM		0x00004000	// sum (on RTM)
#define		stSUBTOTAL	0x00008000	// GRID subtotal(use CCellProperties.formula)
#define		stTOTAL		0x00800000	// GRID total (can be combined with stSUBTOTAL)
//V3.0
#define		stLINES		0x00000020	// TREE : has lines(indentation)
#define		stGRADIENT	0x00000020	// gradient brush
#define		stVERT		0x00000080	// LABEL, BUTTON, PANEL = vertical text
#define		stLIST		0x00010000	// EDIT input history list
#define		stTHOUSAND	0x00020000	// convert '.' into "000"
#define		stBOLD		0x00040000	// bold font column
#define		stCODEx		0x00040000	// EDIT has code list, but recent code sort
#define		stMARK		0x00080000	// GRID head action mark
#define		stWHEEL		0x00080000	// EDIT wheel on spin
#define		stXZERO		0x00100000	// EDIT not fill data with leading '0' in code field
#define		stHIMAGE	0x00100000	// GRID image head
#define		stHIMAGETEXT	0x00200000	// GRID image+text head
#define		stNOR		0x00200000	// EDIT NOR on force
#define		stXPAL		0x00400000	// not apply palette index automatically
#define		stACCN		0x00000008	// account COMBO
#define		stFLEXCB	0x00100000	// flexible combo item
#define		stTRANSPARENT	0x00100000	// transparent

	int	checked;		// COMBO/TAB, default checked index

	char	typeCursor;		// cursor's type on mouse-over
#define		csNONE		0
#define		csSORTd		1	// sort cursor : descending
#define		csSORTa		2	// sort cursor : ascening (internal used)
#define		csPOP		3	// pop onMouseOver
#define		csIBEAM		4	// I-beam cursor
#define		csCURSOR	10	// user-defined cursor base

	char	typeHint;		// hint type
#define		htNOP		0	// NOP
#define		htSTRING	1	// hint string
#define		htREF		2	// hint reference form
#define		htFORM		3	// hint FORM name
#define		htDESC		4	// hint description

	CString	hintText;		// hint/help text

	CString	keyNames;		// OUTPUT = domino FORM's name
					// EDIT   = encrypt key FORM's name
					// RADIO  = group FORM's name

	char	whatEdit;		// dispaly edit format mask options
					// conditional color options
#define		weNOP		0	// NOP
#define		weIT		1	// I/O edit format
#define		weCORGB		2	// conditional color

					// GRID = focus style
#define		wfNONE		0	// none
#define		wfCELL		1	//
#define		wfRBOX		2	// row box
#define		wfRBAR		3	// row bar with auxValue's RGB
#define		wfRECORD	4	// record bar with auxVlue's RGB

	DWORD	auxValues;		// LOWORD(auxValues) = down conditional color index, HIWORD(auxValues) = up...index
					// GRID = focus color
	CString	editFormat;		// edit mask format string

	char	offsets;		// EDIT/OUTPUT = float digit #
					// TAB = fixed alignment

	CString	strings;		// LABEL/IMAGEVW/EDIT/BUTTON/CHECK/RADIO = text
					// FORM = subFORM's name
					// BROWSER = start page
					// GRID = variant
					// COMBO/TAB = I/O data text
					// OBJECT = custom OBJECT's name

	CString	displays;		// EDIT = hints text
					// COMBO/TAB = display text

	CString	images;			// IMAGEVW/CHECK/RADIO/EDIT/LABEL/TAB = image name
	CString	auxImages;		// TAB/BUTTON/CHECK = optional image/icon name
	char	posImage;		// GRID = optional image position

	CObArray* cellProperties;	// GRID/TABLE cell's array

	int	fixRow;			// fixed row #
	int	fixColumn;		// fixed column #
	int	headHeight;		// height of head
	int	rowLines;		// # of GRID repeat row's line color

	CString	flashKeys;		// RTS keys
	CString	referenceName;		// reference FORM's name
};


class CcellProperties : public CObject
{
public:
	CcellProperties()
	{
		kind			= NULL;
		headKind		= NULL;
		name			= _T("");
		size			= 0;
		charType		= NULL;
		ioType			= NULL;
		width			= 0;
		entryCombo		= 0;
		alignmentHead	= NULL;
		alignmentText	= NULL;
		alignmentImage	= NULL;
		textColor		= 0;
		paintColor		= 0;
		headText		= _T("");
		headTextColor	= 0;
		headPaintColor	= 0;
		fontStyle		= NULL;
		fontPoint		= 0;
		fontName		= _T("");
		attributes		= 0;
		auxAttributes	= 0;
		flashKeys		= _T("");
		whatEdit		= NULL;
		auxValues		= 0;
		editFormat		= _T("");
		styles			= 0;
		offsets			= NULL;
		keyParam		= 0;
		dominoName		= _T("");
		displays		= _T("");
		strings			= _T("");
		images			= _T("");
		auxImages		= _T("");
		merges			= _T("");
		typeCursor		= NULL;
		typeCursorH		= NULL;
		typeHint		= NULL;
		hintText		= _T("");
		referenceName	= _T("");
	}
public:
	char	kind;			// kind of column
#define		ckNORMAL	0	// edit / output
#define		ckCHECK		1	// check
#define		ckCOMBO		2	// combobox
#define		ckBUTTON	3	// push button
#define		ckCHART		4	// chart

	char	headKind;		// kind of head
	CString	name;			// column's name
	int	size;			// size of text
	char	charType;		// type of display character
	char	ioType;			// I/O attributes type
	int	width;			// column width

	int	entryCombo;		// COMBO entry count
	char	alignmentHead;		// head alignment
	char	alignmentText;		// text alignment
	char	alignmentImage;		// image alignment

	// color's value (0 ~ 255 or PALETTERGB value)
	DWORD	textColor;
	DWORD	paintColor;

	CString	headText;		// head's display text
	DWORD	headTextColor;
	DWORD	headPaintColor;

	char	fontStyle;
	int	fontPoint;
	CString	fontName;

	DWORD	attributes;
	DWORD	auxAttributes;

	CString	flashKeys;		// RTS keys

	char	whatEdit;		// dispaly edit format mask options
					// conditional color options
	DWORD	auxValues;		// LOWORD(auxVlues) = down conditional color index, HIWORD(auxValues) = up...index
	CString	editFormat;		// edit mask format string

	DWORD	styles;			// properties styles
					// visible, sort...
	char	offsets;		// float digit #
	DWORD	keyParam;		// combo's spread short key, only display cell merge #
	CString	dominoName;		// domino's column name

	CString	displays;		// COMBO/BUTTON/CHECK = display text
	CString	strings;		// EDIT/BUTTON/CHECK = text
					// COMBO = I/O data text
	CString	images;			// image/icon name
	CString	auxImages;		// optional image name

	CString	merges;			// merge column's name

	char	typeCursor;		// cursor's type on mouse-over
	char	typeCursorH;		// head cursor type on mouse-over
	char	typeHint;		// hint type
	CString	hintText;		// hint/help text

	CString	referenceName;		// reference FORM's name
	CString	formula;		// formula
};