
#pragma once

#include "h/formOb.h"

static struct	_typeTbl {
	char	strName[16];
	BYTE	type;
} typeTbl[] = {
		{ "NUMERIC", ctNUMERIC }, { "ALPHA", ctALPHA },
		{ "MULTI", ctMULTI }, { "MIXED", ctMIXED }
};
#define	TYPETBL		sizeof(typeTbl) / sizeof(struct _typeTbl)


static struct	_alignTbl {
	char	strName[16];
	BYTE	align;
} alignTbl[] = {
		{"CENTER",	atCENTER},
		{"LEFT",  	atLEFT},
		{"RIGHT",	atRIGHT}
};
#define	ALIGNTBL	sizeof(alignTbl) / sizeof(struct _alignTbl)


#define	EMASK_DATE	-99
#define	EMASK_TIME	-98
static struct	_attrTbl {
	char	strName[16];
	DWORD	attr;
} attrTbl[] = {
		{"COMMA",	faCOMMA},		{"RIGHT",		faRIGHT},
		{"PASSWD",	faPASSWD},		{"ZEROSUP",		faZEROSUP},
		{"SEND",	faSEND},		{"PROTECT",		faPROTECT},
		{"WRAP",	faWRAP},		{"MAND",		faDOMINO},
		{"HAN",		faMULTI},		{"SKIP",		faSKIP},
		{"CORGB",	faCORGB},		{"DOMINO",		faDOMINO},
		{"REF",		faREF},			{"DATE",		faDATE},
		{"ZERO",	faZERO},		{"FLASH",		faFLASH},
		{"HORS",	faHORS},		{"FLOAT",		faFLOAT},
		{"UPPER",	faUPPER},		{"DRAG",		faDRAG},
		{"NOR",		faNOR},			{"ENUM",		faENUM},
		{"XDELETE",	faXDELETE},		{"CONTRAST",	faCONTRAST},
		{"PASS",	faPASS},		{"XFLOAT",		faXFLOAT},
		{"DATE",	EMASK_DATE},	{"TIME",		EMASK_TIME}
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
		{"EDIT",  	fmEDIT,	0},
		{"COMBO",	fmCOMBO,	0},
		{"OUT",		fmOUTPUT,		0},
		{"CHECK",	fmCHECK,	0},
};
#define	KINDTBL		sizeof(kindTbl) / sizeof(struct _kindTbl)


static struct	_ioTbl	{
	char	strName[8];
	BYTE	iok;
} ioTbl[] = {
		{ "INPUT",	ioINPUT },
		{ "OUTPUT",	ioOUTPUT },
		{ "INOUT",	ioBOTH }
};
#define	IOTBL		sizeof(ioTbl) / sizeof(struct _ioTbl)

