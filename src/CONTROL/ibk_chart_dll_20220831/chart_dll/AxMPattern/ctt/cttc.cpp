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


unsigned short l_TCDateToJulian(float date)
{
	long     year = 0, month = 0, day = 0;
	short    syr = 0, smm = 0, sday = 0;
	
	year = (long) (date / 10000);
	month = (long) ((date - year * 10000) / 100);
	day = (long) ((date - year * 10000) - (month * 100));
	syr = (short) year;
	smm = (short) month;
	sday = (short) day;
	return (l_YMDToJulian(syr, smm, sday));
}


short WINAPI l_TClist(char *path, char *outfile, short append)
{
	short    err = 0, i1, line = 0, page = 0;
	long     cnt;
	char     buffer[81], symbol[7];
	FILE    *fp1, *out;
	
#ifndef _WINDOWS
	if (!l_stricmp(outfile, "STDOUT"))
	{
		out = stdout;
		if (append)
			page = 1;
	}
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
	strcpy_s(buffer, sizeof(buffer), path);
	if (path[0] != '\0' || path[strlen(path) - 1] != ':' || path[strlen(path) - 1] != '/')
		strcat_s(buffer, sizeof(buffer), "/");
	strcat_s(buffer, sizeof(buffer), "basename");
	
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	
	fgets(symbol, 80, fp1);
	cnt = atol(symbol);             /* First entry holds count */
	fprintf(out, "SYMBOL\n");
	fprintf(out, "------\n");
	line = 2;
	for (i1 = 0; (fgets(symbol, 80, fp1) != NULL); i1++)
	{
		line++;
		
#ifndef _WINDOWS
		if (page && line == 25)
		{
			printf("Press any key to continue . . .");
			getchar();
			printf("\r                               \r");
			line = 1;
		}
#endif
		fprintf(out, "%3i %s", i1 + 1, symbol);
	}
	fclose(fp1);
#ifndef _WINDOWS
	if (l_stricmp(outfile, "STDOUT"))
#endif
		fclose(out);
	return (err);
}


short WINAPI l_TCread(ctBars *b1, char *path, char *name, long start, short cnt, short allocate)
{
	short    i1 = 0, err = 0;
	long     reccnt = 0;
	char     buffer[81];
	FILE    *fp1;
	struct ctTCrec rec;
	
	if (path[0] != '\0' || path[strlen(path) - 1] != ':' || path[strlen(path) - 1] != '/')
		sprintf_s(buffer, sizeof(buffer), "%s/%s.NDX", path, name);
	else
		sprintf_s(buffer, sizeof(buffer), "%s%s.NDX", path, name);
	
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	
	fseek(fp1, 0, SEEK_END);
	reccnt = ftell(fp1) / 24 - 1;
	fseek(fp1, 0, SEEK_SET);
	
	if (cnt < 0) {
		if (start > 0)
			cnt = (short)(min(reccnt - 1, 16000) - start);
		else
			cnt = (short)(min(reccnt - 1, 16000));
	}
	if (allocate)
	{
		if ((err = l_AllocBars(b1, cnt)))
			return (err);
	}
	b1->type = ctTC;
	strcpy_s(b1->path, sizeof(b1->path), path);
	strcpy_s(b1->name, sizeof(b1->name), name);
	b1->reccnt = reccnt;
	strcpy_s(b1->description, sizeof(b1->description), name);
	memset(b1->symbol, 0, 16);
	memset(b1->cusip, 0, 12);
	b1->begindate = 0;
	b1->enddate = 0;
	b1->frequency = 0;
	b1->datatype = 0;
	b1->optiontype = 0;
	b1->deliverydate = 0;
	b1->strikeprice = 0;
	
	if (cnt == 0)
	{
		fclose(fp1);
		return (0);
	}
	cnt = min(cnt, b1->datasize);
	if (start < 0)
		start = max(0, b1->reccnt - cnt);
	
	if (fread((char *) &rec, sizeof(struct ctTCrec), 1, fp1) <= 0)
	{
		fclose(fp1);
		return (ctErrFileRead);
	}
	if (fseek(fp1, start * sizeof(struct ctTCrec) + sizeof(struct ctTCrec), SEEK_SET))
	{
		fclose(fp1);
		return (ctErrFileRead);
	}
	
	i1 = 0;
	while (i1 < cnt && fread((char *) &rec, sizeof(struct ctTCrec), 1, fp1) > 0)
	{
		l_MStoIEEE(&rec.dt, &rec.dt);
		l_MStoIEEE(&rec.op, &rec.op);
		l_MStoIEEE(&rec.hi, &rec.hi);
		l_MStoIEEE(&rec.lo, &rec.lo);
		l_MStoIEEE(&rec.cl, &rec.cl);
		l_MStoIEEE(&rec.vol, &rec.vol);
		ctArrayItem(b1->dt, i1) = l_TCDateToJulian(rec.dt);
		ctArrayItem(b1->ti, i1) = 0;
		ctArrayItem(b1->op, i1) = rec.op;
		ctArrayItem(b1->hi, i1) = rec.hi;
		ctArrayItem(b1->lo, i1) = rec.lo;
		ctArrayItem(b1->cl, i1) = rec.cl;
		ctArrayItem(b1->vol, i1) = rec.vol;
		i1++;
	}
	fclose(fp1);
	l_ReSizeBars(b1, i1);
	return (err);
}
