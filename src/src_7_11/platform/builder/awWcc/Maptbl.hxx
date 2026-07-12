#ifndef _MAPTBL_
#define _MAPTBL_
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
		{"MAPTYPE",	19},		{"MAPPUBLIC",	20},
		{"PYTHON",	21}
};
#define	GLOBALSYM	sizeof(globalSym) / sizeof(struct _globalSym)

//
//	FSTYLE TABLE
//
struct _fStyle {
	char	strName[12];
	BYTE	defValue;
} fStyle[] = {
		{"NORMAL",	FS_NORM},	{"ITALIC",	FS_ITALIC},
		{"BOLD",	FS_BOLD},	{"BOLD_ITALIC",	FS_IBOLD}
};
#define	FSTYLE		sizeof(fStyle) / sizeof(struct _fStyle)

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
//	MAPTYPE TABLE
//
struct	_mapType {
	char	strName[16];
	BYTE	defValue;
} mapType[] = {
		{"NORM",	VW_NORM},	{"SCROLL",	VW_SCROLL},
		{"RESIZE",	VW_RESIZE},	{"WND",		VW_WND},
		{"FIXED",	VW_FIXED},	{"FIXEDEX",	VW_FEX}

};
#define	MAPTYPE		sizeof(mapType) / sizeof(struct _mapType)

//
//	HEADER TABLE
//
struct	_headers {
	char	strName[8];
	BYTE	defValue;
} headers[] = {
		{"NOP",		TH_NOP},	{"KOSCOM",	TH_KOSCOM},
	 	{"SCUP",	TH_SCUP},	{"4702",	TH_4702},
		{"TUXEDO",	TH_TUXEDO},	{"TOPEND",	TH_TOPEND},
		{"LEDGER",	TH_LEDGER}
};
#define	HEADERS		sizeof(headers) / sizeof(struct _headers)

//
//	OPTION TABLE
//
struct	_options {
	char	strName[16];
	DWORD	defValue;
} options[] = {
		{"FMS",		OP_OOP},	{"CODECNV",	OP_CNV},
		{"FORMAT",	OP_CLS},	{"FLASH",	OP_FLASH},
		{"ENCODE",	OP_ENC},	{"TABSEP",	OP_TABS},	
		{"CR2TAB",	OP_CR2TB},	{"DROP",	OP_DROP},	
		{"USER",	OP_USER},	{"INTER",	OP_INTER},
		{"CERTIFY",	OP_CERTIFY},	{"FMS2",	OP_OOP2},
		{"DOMINO",	OP_DOMINO},	{"XFONT",	OP_XFONT},
		{"SAVE",	OP_SAVE}
};
#define	OPTIONS		sizeof(options) / sizeof(struct _options)

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


//----------------------------------------------------------------------
//
//	FORM TABLE
//
struct	_formTbl {
	char	strName[16];		// 
	BYTE	kind;			// Form Kind
} formTbl[] = {
		{"SYS",		FM_SYS},
		{"LABEL",	FM_LABEL},
		{"BOX",		FM_BOX},
		{"GROUP",	FM_GROUP},
		{"PANEL",	FM_PANEL},
		{"BUTTON",	FM_BUTTON},
		{"EDIT",	FM_EDIT},
		{"COMBO",	FM_COMBO},
		{"OUT",		FM_OUT},
		{"MEMO",	FM_MEMO},
		{"GRID",	FM_GRID},
		{"TABLE",	FM_TABLE},
		{"SEMI",	FM_TABLE},	// delete later
		{"TREEVIEW",	FM_TREE},
		{"AVI",		FM_AVI},
//		{"GRAPH",	FM_GRAPH},
		{"OBJECT",	FM_OBJECT},
		{"TAB",		FM_TAB},
		{"USRTAB",	FM_UTAB},
		{"SHEET",	FM_SHEET},
		{"BROWSER",	FM_BROWSER},
		{"CONTROL",	FM_CONTROL},
};
#define	FORMTBL		sizeof(formTbl) / sizeof(struct _formTbl)

//
//	FORM ETC. TABLE
//
struct	_formEtcTbl {
	char	strName[16];
	BYTE	flow;
} formEtcTbl[] = {
		{"FONT",	1},		{"ENABLE",	2},
		{"VISIBLE",	3},		{"CHECKED",	4},
		{"HINTTYPE",	5},		{"FIXEDSIZE",	6},
		{"TRANSPARENT",	7},		{"IMAGE",	8},
		{"HOVER",	9},		{"SORT",	10},
		{"HASLINE",	11},		{"HELPTEXT",	12},
		{"VERTICAL",	13},		{"IMAGETEXT",	14},
		{"SHOWFORM",	15},		{"ACCCBO",	16},
		{"RESIZABLE",	17},		{"GROUPN",	18},
		{"TABIMAGE",	19}
};

#define	FORMETCTBL	sizeof(formEtcTbl) / sizeof(struct _formEtcTbl)
 
//
//	formR.kind == EDIT, COMBO, OUT, SCROLL, GRID, TABLE
//	formR.type
//
struct	_ioTbl {
	char	strName[16];
	BYTE	type;
} ioTbl[] = {
		{"IO_NUM",     IO_NUM},		{"IO_ALPHA",  IO_ALPHA},
		{"IO_HAN",     IO_HAN},		{"IO_MIXED",  IO_MIXED},
		{"IO_DIGIT",	IO_DIGIT}
};
#define	IOTBL		sizeof(ioTbl) / sizeof(struct _ioTbl)

//
//	formR.isHint
//
struct	_hintTbl {
	char	strName[16];
	BYTE	type;
} hintTbl[] = {
		{"IH_NONE",	IH_NOP},	{"IH_STRING",	IH_STRING},
		{"IH_REF",	IH_REF},	{"IH_MAP",	IH_MAP}
};
#define	HINTTBL		sizeof(hintTbl) / sizeof(struct _hintTbl)

//
//	formR.kind == BOX
//	formR.type
//
struct	_boxTbl {
	char	strName[16];
	BYTE	type;
} boxTbl[] = {
		{"BX_LINE",	BX_LINE},	
		{"BX_BORDER",	BX_BORDER},
		{"BX_2CLR",	BX_2CLR}
};
#define	BOXTBL		sizeof(boxTbl) / sizeof(struct _boxTbl)

//
//	formR.alignment 
//
struct	_alignTbl {
	char	strName[16];
	BYTE	alignment;
} alignTbl[] = {
		{"AL_CENTER",	AL_CENTER},	{"AL_LEFT",	AL_LEFT},
		{"AL_RIGHT",	AL_RIGHT},	{"AL_TOP",	AL_TOP},
		{"AL_BOTTOM",	AL_BOTTOM},	{"AL_TILE",	AL_TILE},
		{"AL_STRETCH",	AL_STRETCH}
};
#define	ALIGNTBL	sizeof(alignTbl) / sizeof(struct _alignTbl)

//
//	formR.kind == TAB, UTAB
//	formR.type
//
struct	_tabStyleTbl {
	char	strName[16];
	BYTE	type;
} tabStyleTbl[] = {
		{"TAB_3D",	TAB_BORDER},
		{"TAB_NO3D",	TAB_NONE},
		{"TAB_OBLIQUE",	TAB_OBLIQUE},
		{"TAB_RECT",	TAB_RECT},
		{"TAB_IMAGE",	TAB_IMAGE}
};
#define	TABSTYLETBL	sizeof(tabStyleTbl) / sizeof(struct _tabStyleTbl)

//
//	formR.kind == SHEET
//	formR.type
//
struct	_sheetStyleTbl {
	char	strName[16];
	BYTE	type;
} sheetStyleTbl[] = {
		{"SHT_BORDER",	SHT_BORDER},
		{"SHT_SHEET",	SHT_SHEET}
};
#define	SHEETSTYLETBL	sizeof(sheetStyleTbl) / sizeof(struct _sheetStyleTbl)

//
//	formR.kind == SYS
//	formR.type
//
struct	_sysTbl {
	char	strName[16];
	BYTE	type;
} sysTbl[] = {
		{"TP_DATE",	SYS_DATE},
		{"TP_TIME",	SYS_TIME},
		{"TP_TERM",	SYS_TERM}
};
#define	SYSTBL		sizeof(sysTbl) / sizeof(struct _sysTbl)

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
		{"TP_ETCHED",	BD_ETCHED}	
};
#define	PANELTBL	sizeof(panelTbl) / sizeof(struct _panelTbl)

//
//	formR.kind == BUTTON
//	formR.type
//
struct	_buttonTbl {
	char	strName[16];
	BYTE	type;
} buttonTbl[] = {
		{"TP_PUSH",   BTN_PUSH},	{"TP_RADIO",  BTN_RADIO},
		{"TP_CHECK",  BTN_CHECK},	{"TP_USER",   BTN_USER}
};
#define	BUTTONTBL	sizeof(buttonTbl) / sizeof(struct _buttonTbl)

//
//	formR.kind == TREE
//	formR.type
//
struct	_treeTbl {
	char	strName[16];
	BYTE	type;
} treeTbl[] = {
		{"TV_FLAT",	TV_FLAT},	{"TV_LINE",	TV_LINE}
};
#define	TREETBL		sizeof(treeTbl) / sizeof(struct _treeTbl)

//
//	formR.kind == EDIT, MEMO, COMBO, RADIO, CHECK, BROWSER
//	formR.iok
//
struct	_emodeTbl {
	char	strName[16];
	BYTE	iok;
} emodeTbl[] = {
		{"EDIT_NOP",	EIO_NOP},	{"EDIT_INPUT",	EIO_INPUT},
		{"EDIT_INOUT",	EIO_INOUT},	{"EDIT_OUTPUT",	EIO_OUTPUT}
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
		{"FA_COMMA",	FA_COMMA},	{"FA_RIGHT",	FA_RIGHT},
		{"FA_PASSWD",	FA_PASSWD},	{"FA_ZEROSUP",	FA_ZEROSUP},
		{"FA_SEND",	FA_SEND},	{"FA_PROTECT",	FA_PROTECT},
		{"FA_WRAP",	FA_WRAP},	{"FA_MAND",	FA_MAND},
		{"FA_HAN",	FA_HAN},	{"FA_SKIP",	FA_SKIP},
		{"FA_CORGB",	FA_CORGB},	{"FA_DOMINO",	FA_DOMINO},
		{"FA_REF",	FA_REF},	{"FA_DATE",	FA_DATE},
		{"FA_ZERO",	FA_ZERO},	{"FA_FLASH",	FA_FLASH},
		{"FA_HORS",	FA_HORS},	{"FA_FLOAT",	FA_FLOAT},
		{"FA_UPPER",	FA_UPPER},	{"FA_DRAG",	FA_DRAG},
		{"FA_NOR",	FA_NOR},	{"FA_ENUM",	FA_ENUM},
		{"FA_XDELETE",	FA_XDELETE},	{"FA_CONTRAST",	FA_CONTRAST},
		{"FA_PASS",	FA_PASS},	{"FA_XFLOAT",	FA_XFLOAT}
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
		{"SA_CORAW",	SA_CORAW},	{"SA_COSUP",	SA_COSUP},
		{"SA_COSIGN",	SA_COSIGN}
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
//	formR.attr & FA_SEND
//	formR.attr2
//
struct	_asendTbl {
	char	strName[16];
	DWORD	attr2;
} asendTbl[] = {
		{"SA_ALL",	SA_ALL},	{"SA_THIS",	SA_THIS}
};
#define	ASENDTBL	sizeof(asendTbl) / sizeof(struct _asendTbl)

//
//	formR.attr & FA_CONTRAST
//	formR.attr2
//
struct	_contrastTbl {
	char	strName[16];
	DWORD	attr2;
} contrastTbl[] = {
		{"SA_TEXT",	SA_TEXT},	{"SA_PAINT",	SA_PAINT}
};
#define	CONTRASTTBL	sizeof(contrastTbl) / sizeof(struct _contrastTbl)

//
//	formR.attr & FA_DATE
//	formR.attr2
//
struct	_dateTbl {
	char	strName[16];
	DWORD	attr2;
} dateTbl[] = {
		{"SA_AUTO",	SA_AUTO},	{"SA_YYYYMM",	SA_YYYYMM},
		{"SA_YYMM",	SA_YYMM},	{"SA_MMDD",	SA_MMDD}
};
#define	DATETBL	sizeof(dateTbl) / sizeof(struct _dateTbl)

//
//	formR.kind == BUTTON && BTN_USER
//	formR.keys
//
struct	_vkTbl {
	char	strName[16];
	WORD	vkKey;
} vkTbl[] = {
		{"NOTMAPPING",	0},
		{"PAGEUP",	VK_PRIOR},	{"PAGEDN",	VK_NEXT},
		{"HOME",	VK_HOME},	{"END",		VK_END},
		{"F2",		VK_F2},		{"F3",		VK_F3},
		{"F4",		VK_F4},		{"F5",		VK_F5},
		{"F6",		VK_F6},		{"F7",		VK_F7},
		{"F8",		VK_F8},		{"F9",		VK_F9},
		{"F11",		VK_F11},	{"F12",		VK_F12},
		{"NUMPAD+",	'+'},		{"NUMPAD-",	'-'},
		{"NUMPAD/",	'/'},		{"NUMPAD*",	'*'},
		{"KEYUP",	VK_UP},		{"KEYDOWN",	VK_DOWN}
};
#define	VKTBL		sizeof(vkTbl) / sizeof(struct _vkTbl)

//
//	I/O FORM EXT. TABLE
//
struct	_ioExt {
	char	strName[10];
	BYTE	flow;
} ioExt[] = {
		{"LEN",		1}, 		{"COLOR",	2},
		{"CURSOR",	3},		{"DOM",		4},
		{"COUNT",	5},		{"DISPLAY",	6},
		{"DATA",	7},		{"INIT",	8},
		{"FLOAT",	9},		{"CAPTION",	10},
		{"UNIT",	11},		{"INDEX",	12},
		{"OBJECT",	13},		{"TABINFO",	14},
		{"FOCUS",	15},		{"CHARS",	16},
		{"REFERENCE",	17},		{"MULTISEL",	18},
		{"DEFAULT",	19},		{"TABH",	20},
		{"URL",		21},		{"SPIN",	22},
		{"CODE",	23},		{"HASTOOL",	24},
		{"SUM",		25},		{"FCOLOR",	26},
		{"VCOUNT",	27},		{"CVAR",	28},
		{"TABORDER",	29},		{"PAGE",	30},
		{"UNRESERVE",	31},		{"IOORDER",	32},
		{"MATCHKEY",	33},		{"SPINTYPE",	34},
		{"ENCRYPT",	35},		{"LINECOLOR",	36},
		{"LIST",	38}
};
#define	IOEXT		sizeof(ioExt) / sizeof(struct _ioExt)

//
//	FORM EXT. TABLE
struct	_formExt {
	char	strName[16];
	BYTE	flow;
} formExt[] = {
		{"COLOR",	1},		{"GROUP",	2},
		{"FOCUS",	3},		{"DATE",	4},
		{"KEYMAP",	5},		{"REF",		6},
		{"LINEW",	7},		{"LINECOLOR",	8},
		{"TABORDER",	9},		{"UNRESERVED_",	10},
		{"IOORDER",	11}
};
#define	FORMEXT		sizeof(formExt) / sizeof(struct _formExt)
 
//
//	formR.kind == SCROLL, GRID, TABLE
//	cell info.
//
struct	_columnTbl {
	char	strName[16];
	BYTE	flow;
} columnTbl[] = {
		{"COLUMN",	1},	{"MATRIX",	2},
		{"COLINFO",	3},	{"LEN",		4},
		{"COLOR",	5},	{"DOM",		6},
		{"VISIBLEROW",	7},	{"HEAD",	8},
		{"WIDTH",	9},	{"HEADCOLOR",	10},
		{"COLUMN_VISIBLE",	11},
		{"MULTI",	12},	{"FLOAT",	13},
		{"FLEXIBLE",	14},	{"VSCROLL",	15},
		{"HSCROLL",	16},	{"FOCUSKIND",	17},
		{"COLALIGN",	18},	{"INS_TOP",	19},
		{"NOHEAD",	20},	{"NO3D",	21},
		{"RROW",	22},	{"XVLINE",	23},
		{"XHLINE",	24},	{"LINECOLOR",	25},
		{"LINEW",	26},	{"CELLH",	27},
		{"COLUMN_SORT",	28},	{"REFERENCE",	29},
		{"ICOMBO",	30},	{"ICHECK",	31},
		{"COUNT",	32},	{"DISPLAY",	33},
		{"DATA",	34},	{"CHECKSTR",	35},
		{"LEAVE",	36},	{"CELLDROP",	37},
		{"FIX",		38},	{"CELLEDIT",	39},
		{"VARIANT",	40},	{"ROWC",	41},
		{"PAGE",	42},	{"HEADER",	43},
		{"TABLE",	44},	{"THUMB",	45},
		{"CXDELETE",	46},	{"TABORDER",	47},
		{"EDITFORMAT",	48},	{"IFRGB",	49},
		{"IOORDER",	50},	{"INORMAL",	51},
		{"IGRAPH",	52},	{"GROUPN",	53},
		{"AUTOSIZE",	54},	{"SUPRESS",	55},
		{"CUD",		56},	{"ISFOCUS",	57},
		{"MATCHKEY",	58},	{"MERGE",	59},
		{"IBUTTON",	60},	{"BUTTONSTR",	61},
		{"BUTTONIMG",	62},	{"COLUMN_IMAGE",63},
		{"COLUMN_IMGTEXT",64},	{"FOCUSCOLOR",	65},
		{"KEYFOCUS",	66},	{"SELALWAYS",	67},
		{"COLUMN_SHOWFORM", 68},{"COLHINTTYPE",	69},
		{"COLHELPTEXT",	70},	{"GRFIX",	71},
		{"GRFLEXEXPAND", 72},	{"COLUMN_EXCHECKE", 73},
		{"GRXSIZE",	74},	{"GRAUTOWIDTH",	75},
		{"GRVALID",	79},	{"GRFTEXT",	80},
		{"GRXXSL",	81},	{"GRSHARE",	82},
		{"COLUMN_BOLD",	83},	{"ONLYHEAD3D",	84}
};
#define	COLUMNTBL	sizeof(columnTbl) / sizeof(struct _columnTbl)

//
//	formR.isEdit
//
struct	_ieTbl {
	char	strName[16];
	BYTE	isEdit;
} ieTbl[] = {
		{"EDITFORMAT",	IE_EDIT},
		{"IFRGB",	IE_CORGB}
};
#define	IETBL		sizeof(ieTbl) / sizeof(struct _ieTbl)




//----------------------------------------------------------------------
//
//	GRAPH TABLE
//
struct _graphTbl {
	char	strName[8];
	BYTE	flow;
} graphTbl[] = {
		{"GRAPH",	1},	{"XTICK",	2},
		{"LTICK",	3},	{"RTICK",	4},
		{"REGION",	5},	{"IGRAPH",	6},
		{"VALUE",	7},	{"ICOLOR",	8},
		{"COLOR",	9},	{"INAME",	10},
		{"NAME",	11},	{"ILTYPE",	12},
		{"COUNT",	13},	{"OPER",	14},
		{"GAP",		15}
};
#define	GRAPHTBL	sizeof(graphTbl) / sizeof(struct _graphTbl)

//
//	Script Information
//

struct _scriptTbl {
	char	strName[21];
	BYTE	flow;
} scriptTbl[] = {
		{"_AW_SCRIPT_AW_",	1},	{"_AW_ENDSCRIPT_AW_",	2},
		{"_AW_ONSTART_AW_",	3},	{"_AW_ONSEND_AW_",	4},
		{"_AW_ONRECEIVE_AW_",	5},	{"_AW_ONALERT_AW_",	6},
		{"_AW_ONSERVICE_AW_",	7},	{"_AW_ONFILE_AW_",	8},
		{"_AW_ONSELECT_AW_",	9},	{"_AW_ONTIMER_AW_",	10},
		{"_AW_ONFOCUS_AW_",	11},	{"_AW_ONCLOSE_AW_",	12},
		{"_AW_ONDEVICE_AW_",	13},	{"_AW_ONAPPROVE_AW_",	14},
		{"_AW_ONKEY_AW_",	15},	{"_AW_ONTRIGGER_AW_",	16},
		{"_AW_ONENDSUB_AW_",	17},	{"_AW_ONDECLARE_AW_",	18},
		{"_AW_ONINDECLARE_AW_",	19},	{"_AW_ONENDFUNC_AW_",	20},
		{"_AW_ONTIMERX_AW_",	21},
		{"PROCEDURE",	100},	{"ENDPROC",	101}
};
#define	SCRIPTTBL	sizeof(scriptTbl) / sizeof(struct _scriptTbl)

//
//	FORM Event Information
//
struct _fEventTbl {
	char	strName[21];
	BYTE	flow;
} fEventTbl[] = {
		{"_AW_ONEDIT_AW_",		1},	{"_AW_ONCLICK_AW_",		2},
		{"_AW_ONDBLCLK_AW_",		3},	{"_AW_ONCHANGE_AW_",		4},
		{"_AW_ONCHARCHANGE_AW_",	5},	{"_AW_ONKEYCHANGE_AW_",		6}
};
#define	FEVENTTBL	sizeof(fEventTbl) / sizeof(struct _fEventTbl)


#endif