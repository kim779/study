#ifndef _MISCTYPE__INCLUDE_
#define _MISCTYPE__INCLUDE_

//
//	attributes
//
#define	attrNUMERIC	0x00000001
#define	attrCOMMA	0x00000002
#define	attrFLOAT	0x00000004
#define	attrZEROSUP	0x00000008
#define	attrCORGB	0x00000010
#define	attrCORGBx	0x00000020
#define	attrCONTRAST	0x00000040
#define	attrCORGBm	0x00000080	// +부호 표시 없음

//
//	alignment
//
#define	alCENTER	0
#define	alLEFT		1
#define	alRIGHT		2


/////////////////////////////////////////////////////////////////////////////////////////
//	fxEdit

//
//	edit style
//
#define	esNORMAL	0x00000001
#define	esPOPUP		0x00000002

//	control message ID	: WM_USER+111
//	LOWORD(WPARAM)
//
//

#define	WM_ENDEDIT	(WM_USER + 930)	// message(WM_COLORPOPUP, WPARAM(ENM_ENDEDIT, id), Text)

#define	ENM_VKRETURN	1		// return key : WM_NOTIFY
#define	ENM_ENDEDIT	0x0001		// kill focus : WM_USER+111



///////////////////////////////////////////////////////////////////////////////////////////
//	fxStatic

//
//	style
//
#define	ssTEXT		0x00000001
#define	ssCOLOR		0x00000002
#define	ssIMAGE		0x00000004
#define	ssCONTRAST	0x00000008
#define	ssBORDER	0x00000010
#define ssGRADIENT	0x00000020

//
//	notify message
//
#define	SNM_CLICK	1		// click event : WM_NOTIFY

typedef	struct	_NM_STATIC {		// notify struct
	NMHDR	hdr;
	CString	text;
} NM_STATIC;



////////////////////////////////////////////////////////////////////////////////////////////
//	fxButton

//
//	button style
//
#define	bsTEXT		0x00000001
#define	bsCOLOR		0x00000002
#define	bsIMAGE		0x00000004	// 1 IMAGE, size DeflateRect(2, 2)
#define	bsGRAD		0x00000008	// gradiation

#define	bsPUSH		0x00000010	// push button
#define	bsFLATE		0x00000020	// flate button
#define	bsTOGGLE	0x00000040	// toggle button
#define	bsRECT		0x00000080	// draw rect only
#define	bsGENERAL	0x00000100	// general button


/////////////////////////////////////////////////////////////////////////////////////////////
//	fxColorPopup

//
//	message
//
#define	WM_COLORPOPUP	(WM_USER + 930)		//	message(WM_COLORPOPUP, id, COLORREF)

#endif	// _MISCTYPE__INCLUDE_


/////////////////////////////////////////////////////////////////////////////////////////////
//	fxSheet

//
//	sheet_view mask
//
#define	SVMK_FONT	0x0001
#define	SVMK_FGCOLOR	0x0002
#define	SVMK_BKCOLOR	0x0004

//
//	sheet notify	(WM_NOTIFY)
//
typedef struct _NM_SVITEM {
	NMHDR	hdr;
	int	key;
	CPoint	pt;
} NM_SVITEM;

//
//	sheet notify message
//
#define	SVNM_LBTNDOWN	1		// LButtonDown
#define	SVNM_RBTNDOWN	2		// RButtonDown
#define	SVNM_BEGINDRAG	3		// BeginDrag
#define	SVNM_ENDDRAG	4		// EndDrag


////////////////////////////////////////////////////////////////////////////////
//	fxSpin

#define	UNUSEDVAL	-1

//
//	spin type
//

#define	spNOT		0x00000000
#define	spKOSPI		0x00000001		// 주식(KOSPI)
#define	spFUTURE	0x00000002		// 선물
#define	spOPTION	0x00000004		// 옵션
#define	spSOPT		0x00000008		// 주식옵션
#define	spKOSDAQ	0x00000010		// 주식(KOSDAQ)
#define	spETFKOSPI	0x00000020		// ETF KOSPI
#define	spETFKOSDAQ	0x00000040		// ETF KOSDAQ
#define	spINDEX		0x00000080		// index(kospi 지수...)
#define	spNUMBER	0x00000100		// step (1, 2, 3)

#define	spCOLOR		0x00010000		// color edit

//
//	spin notify message
//
#define	SPNM_ONCHANGE	1
#define	SPNM_ONECHANGE	2
#define	SPNM_ONSCHANGE	3

///////////////////////////////////////////////////////////////////////////////////////////
//	fxTab

//
//	style
//
#define	tsNORMAL	0x00000000
#define	tsIMAGE		0x00000001
