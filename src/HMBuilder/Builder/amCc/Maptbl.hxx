#ifndef _MAPTBL_HXX
#define _MAPTBL_HXX

#include "../h/mapform.h"

const char* MapHeader[] = {	_T("MAPNAME"), _T("HEIGHT"), _T("WIDTH"), _T("MAPKIND"), _T("MAPDESC"), 
				_T("FONTNAME"), _T("FONTPOINT"), _T("FONTSTYLE"), _T("OPTIONS"), _T("CAPTION"),
				_T("TOPATH"), _T("METHOD"), _T("TRXCODE"), _T("REPEATTIME"), _T("FLASHCOLOR"),
				_T("FLASHSYM"), _T("UPCOLOR"), _T("DNCOLOR"), _T("CONTRAST"), _T("BGKIND"),
				_T("BGFILEN"), _T("BGCOLOR"), _T("FORMTYPE"), _T("PATH_ONLOAD"), _T("PATH_ONSEND"), 
				_T("PATH_ONRECIVE"), _T("PATH_ONALERT"), _T("PATH_ONTIMMER"), _T("PATH_ONCLOSE"), _T("PATH_ONDEVICE"), 
				_T("PATH_ONERROR"), _T("PATH_ONFILCKING"), _T("PATH_ONCHANGELAYOUT"), _T("PATH_ONDECLARATION"), _T("PATH_ONINDECLARATION"), _T("MAPALPHA"), _T("FORMCTRL"),
				NULL};
// Á¤¸®
const char* Control[] = {	_T("SYMBOLNAME"), _T("LEFT"), _T("TOP"), _T("RIGHT"), _T("BOTTOM"), 
				_T("RADIOKIND"), _T("BORDERS"), _T("STR"), _T("STR2"), _T("DAT"), 
				_T("FLOAT"), _T("DESC"), _T("EDITFORMAT"), _T("IFRGB"), _T("IFRGB2"), 
				_T("AUXS"), _T("IMAGETYPE"), _T("EIOIOK"), _T("ALIGNMENT"), _T("ALIGNMENTIMAGE"),
				_T("FONTNAME"), _T("FONTPOINT"), _T("FONTSTYLE"), _T("LEN"), _T("COUNT"),
				_T("VCOUNT"), _T("INDEX"), _T("MATCHKEY"), _T("CHARS"), _T("DATATYPE"),
				_T("ATTRIBUTE"), _T("SUBATTRIBUTE"), _T("PRGB"), _T("TRGB"), _T("SPRGB"), 
				_T("TABORDER"), _T("STRGB"), _T("IOORDER"), _T("GROUPN"), _T("RESIZABLE"), 
				_T("ENABLE"), _T("VISIBLE"), _T("CHECKED"), _T("IMAGE"), _T("IMGTEXT"), 
				_T("HOVER"), _T("SORT"),_T("SHOWFORM"), _T("onclick"), _T("onclickl"),		// 50 
				_T("ONLONGTAB"), _T("ONLONGTABL"), _T("ONCHANGE"), _T("ONCHANGEL"), _T("ONCHAR"),
				_T("ONCHARL"), _T("BLOCK"), _T("DRAWN"), _T("BCOMMON"), _T("SCROLLPOS"), 
				_T("VISIBLEROW"), _T("CELLH"), _T("MATRIX"), _T("FIX"), _T("MAINCOLOR"), 
				_T("LINECOLOR"), _T("FOCUSCOLOR"), _T("FOCUSKIND"), _T("AUTOSIZE"), _T("GRIDOPTION"), 
				_T("COLS"), _T("BRGB"), _T("COLINFO"), _T("CELLLEN"), _T("WIDTH"),
				_T("HEAD"), _T("COLALIGNH"), _T("COLALIGNC"),_T("HEADCOLOR"), _T("HPRGB"),
				_T("HTRGB"), _T("DOM"), _T("COLUMN_VISIBLE"), _T("DISPLAY"), _T("DATA"),
				_T("CHECKSTR"), _T("BUTTONSTR"), _T("BUTTONIMG"), _T("ZEROSUPATTR2"), _T("CORGBATTR2"), 
				_T("CONTRASTATTR2"), _T("FIXEDROW"), _T("FIXEDCOL"), _T("HEADHEIGHT"), _T("ROWOF"), 
				_T("FORMRTSSYMBOL"), _T("DRAGROW"), /*_T("DRAGCOL"),*/ _T("ALPHA"), _T("CELLKIND"), _T("CELLEXKIND"),	// 100
				_T("OVERLAYIMG"), _T("OPTIONIMG"), _T("OPTIONPOS"), _T("SHAPE"), _T("REFS"), 
				_T("MERGECOLUMN"), _T("ONSCROLL"), _T("ONSCROLLL"), _T("COLUMNHEIGHT"), _T("COLUMNHEADHEIGHT"),
				_T("COLUMNHEADIMG"), _T("TABIMG"), _T("TEXTRESIZE"), _T("OPTIONIMG2"), _T("OPTIONIMG3"),
				_T("MARGIN"), _T("USEMARGIN"), _T("OPRGB"), _T("CELLCUSTOM"), _T("BLINKKIND"), 
				_T("CHARTLIMIT"), _T("SFPRGB"), _T("SCREFECT"), _T("REFRESH"),
				NULL
			};   

const char* Script[] = {	_T("_am_onload_am_"),		_T("_am_onsend_am_"),
				_T("_am_onrecv_am_"),		_T("_am_onalert_am_"),
				_T("_am_ontimer_am_"),		_T("_am_onclose_am_"),
				_T("_am_ondevice_am_"),		_T("_am_onerror_am_"),
				_T("_am_onflicking_am_"),	_T("_am_onchangelayout_am_"),
				_T("_am_ondeclaration_am_"),	_T("_am_onindeclaration_am_"),
				_T("_am_onclick_am_"),		_T("_am_onlongtap_am_"),
				_T("_am_onchange_am_"),		_T("_am_onchar_am_"),
				_T("_am_ondrag_am_"),		_T("_am_onscroll_am_"),
				NULL};																																								

//----------------------------------------------------------------------
//
//	GLOBAL TABLE
//
struct _globalSym {
	char	strName[16];
	BYTE	flow;	
} globalSym[] = {
		{"MAPNAME",	1},		{"MAPSIZE",	2},
		{"TITLE",	3},		{"FONT",	4},
		{"TRXCODE",	5},		{"TOPATH",	6},
		{"METHOD",	7},		{"HEADER",	8},
		{"OPTIONS",	9},		{"REPEATTIME",	10},
		{"WAITTIMEOUT",	11},		{"HELPFILE",	12},
		{"VERSION",	13},		{"MAPDESC",	14},
		{"TRXH",	15},		{"MAPKIND",	16},
		{"BGROUND",	17},		{"MAPGROUP",	18},
		{"MAPTYPE",	19},		{"MAPPUBLIC",	20}
};
#define	GLOBALSYM	sizeof(globalSym) / sizeof(struct _globalSym)

//
//	GridOption
//	attr2
//
struct _gridoption {
	char	strName[16];
	DWORD	attr2;	
} gridoptiontbl[] = {
	{_T("MULTI"),	GO_MULTI},	{_T("FLEX"),		GO_FLEX},
	{_T("VSCR"),	GO_VSCR},	{_T("HSCR"),		GO_HSCR},
	{_T("TOP"),	GO_TOP},	{_T("XHEAD"),		GO_XHEAD},
	{_T("ROW"),	GO_ROW},	{_T("XVLINE"),		GO_XVLINE},	
	{_T("XHLINE"),	GO_XHLINE},	{_T("HEADER"),		GO_HEADER},	
	{_T("FIX"),	GO_FIX},	{_T("MERGE"),		GO_MERGE},
	{_T("VALID"),	GO_VALID},	{_T("AUTOSIZE"),	GO_AUTOSIZE},
	{_T("DRAGCOL"), GO_DRAGCOL},	{_T("INLINE"),		GO_XINLINE},
	{_T("BLINK"),	GO_BLINK}
};
#define	GRIDOPTION	sizeof(gridoptiontbl) / sizeof(struct _gridoption)

//
//	FSTYLE TABLE
//
struct _fStyle {
	char	strName[12];
	BYTE	defValue;
} fStyle[] = {
		{"NORMAL",	FS_NORM},	{"ITALIC",	FS_ITALIC},
		{"BOLD",	FS_BOLD},	{"BOLD_ITALIC",	FS_IBOLD},
};
#define FSTYLE		sizeof(fStyle) / sizeof(struct _fStyle)

//
//	TOPATH TABLE
//
struct	_toPath {
	char	strName[8];
	BYTE	defValue;
} toPath[] = {
		{"AXIS",	TP_AXIS},	{"BP1",		TP_AXIS+1},
		{"BP2",		TP_AXIS+2},	{"BP3",		TP_AXIS+3},
		{"BP4",		TP_AXIS+4},	{"BP5",		TP_AXIS+5},
		{"BP6",		TP_AXIS+6},	{"BP7",		TP_AXIS+7},
		{"FEP",		TP_FEP}
};
#define	TOPATH		sizeof(toPath) / sizeof(struct _toPath)

//
//	MAPJUMP TABLE
//
struct	_mapMethod {
	char	strName[8];
	BYTE	defValue;
} mapMethod[] = {
		{"FORM",	MM_FORM},	{"MENU",	MM_MENU},
		{"SEND",	MM_SEND}
};
#define	MAPMETHOD	sizeof(mapMethod) / sizeof(struct _mapMethod)

//
//	OPTION  TABLE
//
struct	_options {
	char	strName[16];
	DWORD	defValue;
} options[] = {
		{"LEDGER",	OP_LEDGER},	{"FMS",		OP_OOP},	
		{"FLASH",	OP_FLASH},	{"CERTIFY",	OP_CERTIFY},
		{"TABS",	OP_TABS},	{"CR2TB",	OP_CR2TB},
		{"ENC",		OP_ENC},	{"DOMINO",	OP_DOMINO},
		{"SAVE",	OP_SAVE}
};
#define	OPTIONS		sizeof(options) / sizeof(struct _options)

//
//	MAPKIND TABLE
//
struct	_mapKind {
	char	strName[16];
	BYTE	defValue;
} mapKind[] = {
		{"NORMAL",	MK_NORM},	
		{"PROCEDURES",	MK_PROCEDURES}
};
#define	MAPKIND		sizeof(mapKind) / sizeof(struct _mapKind)

//
//	BGROUND	TABLE
//
struct	_bground {
	char	strName[8];
	BYTE	defValue;
} bground[] = {
		{"BRUSH",	IK_BRUSH},
		{"IMAGE",	IK_BMP}
};
#define	BGROUND		sizeof(bground) / sizeof(struct _bground)

//
//	FORM TABLE
//
struct	_formTbl {
	char	strName[16];		// 
	BYTE	kind;			// Form Kind
} formTbl[] = {
		{"FM_LABEL",	FM_LABEL},
		{"FM_BOX",	FM_BOX},
		{"FM_GROUP",	FM_GROUP},
		{"FM_IMAGEVIEW",FM_IMAGEVIEW},
		{"FM_BUTTON",	FM_BUTTON},
		{"FM_CHECK",	FM_CHECK},
		{"FM_RADIO",	FM_RADIO},
		{"FM_EDIT",	FM_EDIT},
		{"FM_COMBO",	FM_COMBO},
		{"FM_OUT",	FM_OUT},
		{"FM_GRID",	FM_GRID},
		{"FM_GRIDEX",	FM_GRIDEX},
		{"FM_TABLE",	FM_TABLE},
		{"FM_OBJECT",	FM_OBJECT},
		{"FM_TAB",	FM_TAB},
		{"FM_BROWSER",	FM_BROWSER},
		{"FM_CONTROL",	FM_CONTROL}
};
#define	FORMTBL		sizeof(formTbl) / sizeof(struct _formTbl)

//
//	formR.alignment 
//
struct	_alignTbl {
	char	strName[16];
	BYTE	alignment;
} alignTbl[] = {
		{"AL_CENTER",	AL_CENTER},
		{"AL_LEFT",	AL_LEFT},
		{"AL_RIGHT",	AL_RIGHT},
		{"AL_TOP",	AL_TOP},
		{"AL_BOTTOM",	AL_BOTTOM},
};
#define	ALIGNTBL	sizeof(alignTbl) / sizeof(struct _alignTbl)

//
//	formR.kind == BUTTON, IMAGEVW
//	formR.alignment
//
struct	_alignTblImage {
	char	strName[16];
	BYTE	alignment;
} alignTblImage[] = {
			{"IA_STRETCH",	IA_STRETCH},
			{"IA_CENTER",	IA_CENTER},	
			{"IA_TILE",	IA_TILE}
};
#define	ALIGNTBLIMAGE	sizeof(_alignTblImage) / sizeof(struct _alignTblImage)

//
//	formR.kind == EDIT, COMBO, OUT, SCROLL, GRID, TABLE
//	formR.type
//
struct	_ioTbl {
	char	strName[16];
	BYTE	type;
} ioTbl[] = {
		{"IO_NUM",	IO_NUM},	{"IO_ALPHA",	IO_ALPHA},
		{"IO_MIXED",	IO_MIXED},	{"IO_DIGIT",	IO_DIGIT},
		{"IO_ANY",	IO_ANY}
};
#define	IOTBL	sizeof(ioTbl) / sizeof(struct _ioTbl)

//
//	formR.kind == PANEL
//	formR.type
//
struct	_panelTbl {
	char	strName[16];
	BYTE	borders;
} panelTbl[] = {
		{"TP_NONE",	BD_NONE},	{"TP_RAISED",	BD_RAISED},
		{"TP_SUNKEN",	BD_SUNKEN},	{"TP_BUMP",	BD_BUMP},
		{"TP_ETCHED",	BD_ETCHED},	{"TP_LINE",	BD_LINE}		
};
#define	PANELTBL	sizeof(panelTbl) / sizeof(struct _panelTbl)

//
//	formR.kind == BUTTON, CHECK, RADIO, IMAGEVW
//	formR.properties
//
struct	_propertiesTbl {
	char	strName[16];
	DWORD	properties;
} propertiesTbl[] = {
	{_T("ENABLE"),		PR_ENABLE},	
	{_T("VISIBLE"),		PR_VISIBLE},	
	{_T("CHECKED"),		PR_CHECKED},	
	{_T("IMAGE"),		PR_IMAGE},	
	{_T("IMAGETEXT"),	PR_IMAGETEXT},
	{_T("IMAGEHOVER"),	PR_HOVER},
	{_T("SORT"),		PR_SORT},
	{_T("EDIT"),		PR_EDIT},
	{_T("FIXED"),		PR_FIXED},
	{_T("DRAGROW"),		PR_DRAGROW},
	{_T("SCREFECT"),	PR_SCREFECT},
	{_T("REFRESH"),         PR_REFRESH},
	{_T("LANGUAGE"),	PR_LANGUAGE},
};
#define	PROPERTTBL	sizeof(propertiesTbl) / sizeof(struct _propertiesTbl)

//
//	formR.kind == EDIT, COMBO, RADIO, CHECK, BROWSER
//	formR.iok
//
struct	_emodeTbl {
	char	strName[16];
	BYTE	iok;
} emodeTbl[] = {
		{"EIO_NOP",	EIO_NOP},	{"EIO_INPUT",	EIO_INPUT},
		{"EIO_INOUT",	EIO_INOUT},	{"EIO_OUTPUT",	EIO_OUTPUT}
};
#define	EMODETBL	sizeof(emodeTbl) / sizeof(struct _emodeTbl)

//
//	formR.kind == EDIT, COMBO, OUT, SCROLL, GRID, TABLE
//	formR.attr
//
struct	_attrTbl {
	char	strName[16];
	DWORD	attr;
} attrTbl[] = {
		{"FA_COMMA",	FA_COMMA},	{"FA_PASSWD",	FA_PASSWD},	
		{"FA_ZEROSUP",	FA_ZEROSUP},	{"FA_PROTECT",	FA_PROTECT},
		{"FA_WRAP",	FA_WRAP},	{"FA_SKIP",	FA_SKIP},
		{"FA_CORGB",	FA_CORGB},	{"FA_DOMINO",	FA_DOMINO},
		{"FA_FLOAT",	FA_FLOAT},	{"FA_FLOATx",	FA_FLOATx},
		{"FA_UPPER",	FA_UPPER},	{"FA_NOR",	FA_NOR},
		{ "FA_PASS",	 FA_PASS },	
};
#define	ATTRTBL		sizeof(attrTbl) / sizeof(struct _attrTbl)


//
//	Special Case attr2 (OR VALUE)
//

//
//	formR.attr & FA_CORGB
//	formR.attr2
//
struct	_coRgbTbl {
	char	strName[16];
	DWORD	attr2;
} coRgbTbl[] = {
		{"SA_CORAW",	SA_CORAW},	{"SA_COSUP",	SA_COSUP},	{"SA_COSIGN",	SA_COSIGN},
		{"SA_TEXT",	SA_TEXT},	{"SA_PAINT",	SA_PAINT},	{"SA_TEXTSUP",	SA_TEXTSUP},
		{"SA_PAINTSUP",	SA_PAINTSUP}
};
#define	CORGBTBL	sizeof(coRgbTbl) / sizeof(struct _coRgbTbl)

//
//	formR.attr & FA_ZEROSUP
//	formR.attr2
//
struct	_zerosupTbl {
	char	strName[16];
	DWORD	attr2;
} zerosupTbl[] = {
		{"SA_SPACE",	SA_SPACE},	{"SA_ASTER",	SA_ASTER},
		{"SA_ZERO",	SA_ZERO}
};
#define	ZEROSUPTBL	sizeof(zerosupTbl) / sizeof(struct _zerosupTbl)

//
//	formR.kind == BUTTON && BTN_USER
//	formR.keys
//
struct	_vkTbl {
	char	strName[16];
	WORD	vkKey;
} vkTbl[] = {
		{"NOTMAPPING",	0},
		{"PAGEUP",	VK_PRIOR},
		{"PAGEDN",	VK_NEXT},
		{"HOME",	VK_HOME},
		{"END",		VK_END},
		{"F2",		VK_F2},
		{"F3",		VK_F3},
		{"F4",		VK_F4},
		{"F5",		VK_F5},
		{"F6",		VK_F6},
		{"F7",		VK_F7},
		{"F8",		VK_F8},
		{"F9",		VK_F9},
		{"F11",		VK_F11},
		{"F12",		VK_F12},
		{"NUMPAD+",	'+'},	
		{"NUMPAD-",	'-'},
		{"NUMPAD/",	'/'},	
		{"NUMPAD*",	'*'},
		{"KEYUP",	VK_UP},
		{"KEYDOWN",	VK_DOWN}
};
#define	VKTBL		sizeof(vkTbl) / sizeof(struct _vkTbl)

//----------------------------------------------------------------------
//
//	RESOURCE TABLE
//

//
//	FORM Event Information
//
struct _fEventTbl {
	char	strName[21];
	BYTE	flow;
} fEventTbl[] = {
		{"ONEDIT",		1},	{"ONCLICK",		2},
		{"ONDBLCLK",		3},	{"ONCHANGE",		4},
		{"ONCHARCHANGE",	5},	{"ONKEYCHANGE",		6},
		{"ONSETFOCUS",		7}
};
#define	FEVENTTBL	sizeof(fEventTbl) / sizeof(struct _fEventTbl)


#endif