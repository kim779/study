#ifndef _TABLE_HXX
#define _TABLE_HXX

static struct	_typeTbl {
	char	strName[16];
	BYTE	type;
} typeTbl[] = {
		{"NUM",		IO_NUM},	{"ALPHA",	IO_ALPHA},
		{"HAN",		IO_HAN},	{"MIX",		IO_MIXED}
};
#define	TYPETBL		sizeof(typeTbl) / sizeof(struct _typeTbl)


static struct	_alignTbl {
	char	strName[16];
	BYTE	align;
} alignTbl[] = {
		{"CENTER",	AL_CENTER},
		{"LEFT",  	AL_LEFT},
		{"RIGHT",	AL_RIGHT}
};
#define	ALIGNTBL	sizeof(alignTbl) / sizeof(struct _alignTbl)


#define	EMASK_DATE	-99
#define	EMASK_TIME	-98
static struct	_attrTbl {
	char	strName[16];
	DWORD	attr;
} attrTbl[] = {
		{"COMMA",	FA_COMMA},	{"RIGHT",	FA_RIGHT},
		{"PASSWD",	FA_PASSWD},	{"ZEROSUP",	FA_ZEROSUP},
		{"SEND",	FA_SEND},	{"PROTECT",	FA_PROTECT},
		{"WRAP",	FA_WRAP},	{"MAND",	FA_MAND},
		{"HAN",		FA_HAN},	{"SKIP",	FA_SKIP},
		{"CORGB",	FA_CORGB},	{"DOMINO",	FA_DOMINO},
		{"REF",		FA_REF},	{"DATE",	FA_DATE},
		{"ZERO",	FA_ZERO},	{"FLASH",	FA_FLASH},
		{"HORS",	FA_HORS},	{"FLOAT",	FA_FLOAT},
		{"UPPER",	FA_UPPER},	{"DRAG",	FA_DRAG},
		{"NOR",		FA_NOR},	{"ENUM",	FA_ENUM},
		{"XDELETE",	FA_XDELETE},	{"CONTRAST",	FA_CONTRAST},
		{"PASS",	FA_PASS},	{"XFLOAT",	FA_XFLOAT},
		{"DATE",	EMASK_DATE},	{"TIME",	EMASK_TIME}
};
#define	ATTRTBL		sizeof(attrTbl) / sizeof(struct _attrTbl)

static struct	_dateTbl {
	int	size;
	char	strName[16];
} dateTbl[] = {
		{8,	"9999/99/99"},
		{6,	"9999/99"},
		{4,	"99/99"}
};
#define	DATETBL		sizeof(dateTbl) / sizeof(struct _dateTbl)


static struct	_timeTbl {
	int	size;
	char	strName[16];
} timeTbl[] = {
		{8,	"99:99:99:99"},
		{6,	"99:99:99"},
		{4,	"99:99"}
};
#define	TIMETBL		sizeof(timeTbl) / sizeof(struct _timeTbl)


static struct	_kindTbl {
	char	strName[8];
	BYTE	kind;
	BYTE	type;
} kindTbl[] = {
		{"EDIT",  	FM_EDIT,	0},
		{"COMBO",	FM_COMBO,	0},
		{"OUT",		FM_OUT,		0},
		{"CHECK",	FM_BUTTON,	BTN_CHECK}
};
#define	KINDTBL		sizeof(kindTbl) / sizeof(struct _kindTbl)


static struct	_ioTbl	{
	char	strName[8];
	BYTE	iok;
} ioTbl[] = {
		{"INPUT",	EIO_INPUT},
		{"OUTPUT",	EIO_OUTPUT},
		{"INOUT",	EIO_INOUT}
};
#define	IOTBL		sizeof(ioTbl) / sizeof(struct _ioTbl)

#endif