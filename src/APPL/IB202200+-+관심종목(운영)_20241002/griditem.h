#pragma once

//
//	grid view item
//

typedef struct _GVITEM
{
	int	 row{};
	int	 col{};
	UINT	 mask{};		// field mask
	UINT	 state{};		// field state
	UINT	 attr{};		// field attribute
	UINT	 format{};		// field format
	CString	 text;			// field text
	int	 image{};		// field image
	CFont*	 font{};		// filed font
	COLORREF fgcol{};		// filed foreground color
	COLORREF bkcol{};		// field background color

	CString	 symbol;		// field symbol
	CString	 misc;			// field misc (check, combo, date, time, ticktrend, GVMK_MERGE)
	LPARAM	 param{};		// field ...
} GVITEM;

//
//	GVITEM.mask
//
#define	GVMK_STATE		0x00000001
#define GVMK_ATTR		0x00000002
#define	GVMK_FORMAT		0x00000004
#define	GVMK_TEXT		0x00000008
#define	GVMK_IMAGE		0x00000010
#define	GVMK_FONT		0x00000020
#define	GVMK_FGCOLOR		0x00000040
#define	GVMK_BKCOLOR		0x00000080
#define	GVMK_SYMBOL		0x00000100
#define	GVMK_MISC		0x00000200
#define	GVMK_PARAM		0x00000800


//
//	GVITEM.state
//
#define	GVST_FOCUSED		0x00000001
#define	GVST_SELECTED		0x00000002
#define GVST_DROPHILITED	0x00000004
#define	GVST_MODIFIED		0x00000008
#define	GVST_READONLY		0x00000010

//
//	GVITEM.attr
//
#define	GVAT_SORTVAL		0x00000001	// sort value fixed column attr	(header)
#define GVAT_SORTABS		0x00000002	// 
#define	GVAT_SORTSTR		0x00000004	//
#define	GVAT_HIDDEN		0x00000008
#define	GVAT_DRAGMOVE		0x00000010
#define	GVAT_TICKTREND		0x00000020
#define	GVAT_HDRMERGE		0x00000040
#define	GVAT_TEXT		0x00000080	// for excel (header)
#define	GVAT_NOTOLECOLUMN	0x00000100	// not ole column(키움)
#define	GVMK_BOOKMARK		0x00011000

#define	GVAT_COMMA		0x00000200	// nonfixed(cell) column attr	(cell)
#define	GVAT_FLOAT1		0x00000400
#define	GVAT_FLOAT2		0x00000800
#define	GVAT_FLOAT4		0x00001000
#define	GVAT_CONDITION		0x00002000
#define	GVAT_CONDITIONx		0x00004000
#define	GVAT_SIGNx		0x00000300	// 
#define	GVAT_CONTRAST		0x00008000
#define	GVAT_ZEROSUP		0x00010000	// atof() == 0 display '0'
#define	GVAT_ZEROSUPx		0x00020000	// atof() == 0 display ' '
#define	GVAT_ONLYROW		0x00040000

#define	GVAT_EDIT		0x01000000
#define	GVAT_CHECK		0x02000000
#define	GVAT_COMBO		0x04000000
#define	GVAT_DATE		0x08000000
#define	GVAT_TIME		0x10000000
#define	GVAT_TOOLTIP		0x20000000
#define	GVAT_DRAGDROP		0x80000000	// 코드 drag&drag 가능
#define	GVAT_MARKER		0x40000000  // ADD PSH 20070912

//
//	GVITEM.format
//
#define GVFM_HEADER		(DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX|DT_WORDBREAK)
#define	GVFM_RIGHT		(DT_RIGHT |DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX)
#define	GVFM_LEFT		(DT_LEFT  |DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX)
#define	GVFM_CENTER		(DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX)
#define	GVFM_MULTI		(DT_CENTER|DT_NOPREFIX|DT_EXTERNALLEADING|DT_WORDBREAK)
//
//	grid view notify message
//

typedef	struct _NM_GVITEM {
	NMHDR	hdr{};
	int	row{};
	int	col{};
} NM_GVITEM;


//
//	notify message
//
#define	GVNM_BEGINDRAG		1
#define	GVNM_ENDDRAG		2
#define	GVNM_SELCHANGING	3
#define	GVNM_SELCHANGED		4
#define	GVNM_LMOUSEDOWN		5
#define	GVNM_LMOUSEUP		6
#define	GVNM_RMOUSEDOWN		7
#define	GVNM_DBLCLICKED		8
#define	GVNM_COLUMNMOVE		9	// GVAT_DRAGMOVE		
#define	GVNM_BEGINSORT		10	// not use
#define	GVNM_ENDSORT		11	// sort end

#define	GVNM_UPVSCROLL		12	// scroll up
#define	GVNM_DNVSCROLL		13	// scroll down
#define	GVNM_RHSCROLL		14	// scroll right
#define	GVNM_LHSCROLL		15	// scroll left

#define	GVNM_CHGCOLSIZE		16	// col size change
#define	GVNM_CHGROWSIZE		17	// row size change

#define	GVNM_ONCHAR		18	// 미국(not use)

#define	GVNM_BEGINEDIT		19
#define	GVNM_ENDEDIT		20
#define	GVNM_BEGINCOMBO		21	// combo
#define	GVNM_ENDCOMBO		22	// combo
#define	GVNM_BEGINDATE		23	// date
#define	GVNM_ENDDATE		24	// date
#define	GVNM_BEGINTIME		25	// time
#define	GVNM_ENDTIME		26	// time
#define	GVNM_SELCHANGECOMBO	28	// combo

#define	GVNM_KILLFOCUS		27

#define	GVNM_BEGINCOPY		29	// begin copy
#define	GVNM_COPYROW		30	// copy row
#define	GVNM_ENDCOPY		31	// end copy

#define	GVNM_BEGINCUT		32	// begin cut
#define	GVNM_CUTROW		33	// cut row
#define	GVNM_ENDCUT		34	// end cut

#define	GVNM_BEGINPASTE		35	// begin paste
#define	GVNM_PASTEROW		36	// paste row
#define	GVNM_ENDPASTE		37	// end paste
#define	GVNM_COUNTPASTE		38	// paste count 	

#define	GVNM_ENDTABKEY		39	// 
#define	GVNM_ENDRETURN		40	//
#define	GVNM_CHANGECODE		41	//

#define	GVNM_CHANGEMARKER	42	// ADD PSH 20070914
#define	GVNM_ENDEND			43
#define	GVNM_OUTDRAG		44
#define	GVNM_TRIGGER		45
#define GVNM_REFRESH		46
//
//	grid line
//
#define GVLN_NONE                0
#define GVLN_HORZ                1
#define GVLN_VERT                2
#define GVLN_BOTH                3

#define	GVLT_SOLID		PS_SOLID
#define	GVLT_DASH		PS_DASH
#define	GVLT_DOT		PS_DOT
#define	GVLT_DASHDOT		PS_DASHDOT
#define	GVLT_DASHDOTDOT		PS_DASHDOTDOT
//
//

//
//	grid scroll
//
#define	GVSC_NONE		0
#define	GVSC_HORZ		1
#define	GVSC_VERT		2
#define	GVSC_BOTH		3

//
//	dragdrop type
//
#define	GVDD_NONE		0
#define	GVDD_HALF		1
#define	GVDD_FULL		2		

//
//	InPlaceEdit notify message
//

typedef	struct	_NM_USER {
	NMHDR	hdr{};
	GVITEM	item{};
} NM_USER;


