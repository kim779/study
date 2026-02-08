/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "cttkit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


short WINAPI l_FILEtype(char *type)
{
	short     i1;
	static char *typenames[6] = {"CSI","CT","DJ","FS","TC","TT"};
	static short typenums[7] = {ctCSI,ctCT,ctDJ,ctFS,ctTC,ctTT,ctTXT};
	
	for (i1 = 0; i1 < 6; i1++)
		if (!_stricmp(type, typenames[i1]))
			break;
		return (typenums[i1]);
}


long WINAPI l_FILEfindrec(char *type, char *path, char *name,
						  unsigned short date, unsigned short time,
						  unsigned short *actualdate, unsigned short *actualtime)
{
	short      err;
	struct ctTXTinfo txtinfo;
	
	*actualdate = 0;
	*actualtime = 0;
	switch (l_FILEtype(type))
	{
	case ctTXT:
		if ((err = l_TXTreadinfo(path, type, &txtinfo)))
			return (err);
		return (l_TXTfindrec(path, name, &txtinfo, date, time, actualdate, actualtime));
		
	case ctCSI:
		return (l_CSIfindrec(path, name, date, actualdate));
		
	case ctCT:
		return (l_CTfindrec(path, name, date, time, actualdate, actualtime));
		
	case ctTT:
		return (l_TTfindrec(path, name, date, actualdate));
	}
	return (-1);
}


short WINAPI l_FILElist(char *type, char *path, char *outfile, short append)
{
	switch (l_FILEtype(type))
	{
	case ctCSI:
		return (l_CSIlist(path, outfile, append));
		
	case ctCT:
		return (l_CTlist(path, outfile, append));
		
	case ctDJ:
		return (l_DJlist(path, outfile, append));
		
	case ctTC:
		return (l_TClist(path, outfile, append));
	}
	return (-1);
}


short WINAPI l_FILEprintinfo(ctBars *b1, char *outfile, short append)
{
	FILE    *out;
	char     buffer[12];
	static char *type[8] = {"Unknown","CSI","CompuTrac","Dow Jones","FutureSource","TeleChart","TechTools","Text"};
	static char *frequency[7] = {"Unknown","Intraday","Daily","Weekly","Monthly","Quarterly","Yearly"};
	static char *datatype[7] = {"Unknown","Bond","Commodity","Mutual Fund","Option","Stock","Index"};
	static char *optiontype[3] = {" ","Call","Put"};
	
#ifndef _WINDOWS
	if (!l_stricmp(outfile, "STDOUT"))
		out = stdout;
	else
#endif
	{
		errno_t eno;
		if (append)
			eno = fopen_s(&out, outfile, _T("at"));
		else
			eno = fopen_s(&out, outfile, _T("wt"));
		if (eno)
			return (ctErrFileOpen);
	}
	fprintf(out, "barsize      %i\n", b1->size);
	fprintf(out, "datasize     %i\n", b1->datasize);
	fprintf(out, "reccnt       %li\n", b1->reccnt);
	fprintf(out, "path         %s\n", b1->path);
	fprintf(out, "name         %s\n", b1->name);
	fprintf(out, "description  %s\n", b1->description);
	fprintf(out, "symbol       %s\n", b1->symbol);
	fprintf(out, "cusip        %s\n", b1->cusip);
	l_JulianToDateTxt(b1->begindate, DTFORMAT_MM_DD_YY, "/", buffer);
	fprintf(out, "begindate    %s\n", buffer);
	l_JulianToDateTxt(b1->enddate, DTFORMAT_MM_DD_YY, "/", buffer);
	fprintf(out, "enddate      %s\n", buffer);
	fprintf(out, "frequency    %s\n", frequency[b1->frequency]);
	fprintf(out, "datatype     %s\n", datatype[b1->datatype]);
	fprintf(out, "optiontype   %s\n", optiontype[b1->optiontype]);
	l_JulianToDateTxt(b1->deliverydate, DTFORMAT_MM_DD_YY, "/", buffer);
	fprintf(out, "deliverydate %s\n", buffer);
	fprintf(out, "strikeprice  %8li.%02li\n", (long)b1->strikeprice, l_Decimals(b1->strikeprice, 2));
	fprintf(out, "type         %s\n", type[b1->type]);
	
#ifndef _WINDOWS
	if (l_stricmp(outfile, "STDOUT"))
#endif
		fclose(out);
	return (0);
}


short WINAPI l_FILEread(char *type, ctBars *b1, char *path, char *name,
						long start, short cnt, short allocate)
{
	short      err;
	struct ctTXTinfo txtinfo;
	
	switch (l_FILEtype(type))
	{
	case ctTXT:
		if ((err = l_TXTreadinfo(path, type, &txtinfo)))
			return (err);
		return (l_TXTread(b1, path, name, &txtinfo, start, cnt, allocate));
		
	case ctCSI:
		return (l_CSIread(b1, path, name, start, cnt, allocate));
		
	case ctCT:
		return (l_CTread(b1, path, name, start, cnt, allocate));
		
	case ctDJ:
		return (l_DJread(b1, path, name, start, cnt, allocate));
		
	case ctFS:
		return (l_FSread(b1, path, name, start, cnt, allocate));
		
	case ctTC:
		return (l_TCread(b1, path, name, start, cnt, allocate));
		
	case ctTT:
		return (l_TTread(b1, path, name, start, cnt, allocate));
	}
	return (-1);
}

