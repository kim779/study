/* Technical Analysis Programmer's Toolkit - Copyright (C) 1996 FM Labs */

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

long WINAPI l_TXTfindrec(char *path, char *name, struct ctTXTinfo *info,
						 unsigned short date, unsigned short time,
						 unsigned short *actualdate, unsigned short *actualtime)
{
	short      i1 = 0, maxcol = 0;
	unsigned short dt1 = 0, ti1 = 0;
	long     recno = 0, l1 = 0, l2 = 0;
	char     buffer[256], infile[81], *string;
	FILE    *fp1;
	char*	next_token1 = NULL;
	
	maxcol = max(info->dt, info->ti);
	
	strcpy_s(infile, sizeof(infile), path);
	if (infile[0] != '\0' && infile[strlen(infile) - 1] != ':' && infile[strlen(infile) - 1] != '/')
		strcat_s(infile, sizeof(infile), "/");
	strcat_s(infile, sizeof(infile), name);
	
	if (fopen_s(&fp1, infile, _T("rt")))
		return (ctErrFileOpen);
	
	recno = ctErrRecordNotFound;
	for (l1 = 0; fgets(buffer, 255, fp1) != NULL; l1++)
	{
		if (buffer[0] == '\n' || l1 < info->skip)
			continue;
		for (i1 = 1; i1 <= maxcol; i1++)
		{
			if (i1 == 1)
				string = strtok_s(buffer, info->delimit, &next_token1);
			else
				string = strtok_s(NULL, info->delimit, &next_token1);
			if (info->dt == i1)
				dt1 = l_DateTxtToJulian(string, info->dtformat);
			if (info->ti == i1)
				ti1 = l_TimeTxtToJulian(string, info->tiformat);
		}
		if (dt1 > date || (dt1 == date && ti1 >= time))
		{
			recno = l2;
			if (actualdate)
				*actualdate = dt1;
			if (actualtime)
				*actualtime = ti1;
			break;
		}
		l2++;
	}
	fclose(fp1);
	return (recno);
}


short WINAPI l_TXTread(ctBars *b1, char *path, char *name, struct ctTXTinfo *info,
					   long start, short cnt, short allocate)
{
	short      i1 = 0, i2 = 0, err = 0, maxcol = 0;
	long     reccnt = 0, fpos = 0, l1 = 0, l2 = 0;
	char     buffer[256], infile[81], *string;
	FILE    *fp1;
	char*	next_token1 = NULL;
	
	maxcol = max(max(info->dt, info->ti), max(info->op, info->hi));
	maxcol = max(maxcol, max(info->lo, info->cl));
	maxcol = max(maxcol, max(info->vol, info->oi));
	
	strcpy_s(infile, sizeof(infile), path);
	if (infile[0] != '\0' && infile[strlen(infile) - 1] != ':' && infile[strlen(infile) - 1] != '/')
		strcat_s(infile, sizeof(infile), "/");
	strcat_s(infile, sizeof(infile), name);
	
	buffer[0] = '\0';
	if (fopen_s(&fp1, infile, _T("rt")))
		return (ctErrFileOpen);
	for (i1 = 0; i1 < info->skip; i1++)
		fgets(buffer, 255, fp1);
	
	do
	{
		if (l1 == start)
			fpos = ftell(fp1);
		l1++;
	}
	while (fgets(buffer, 255, fp1));
	
	reccnt = l1 - 1;
	if (cnt < 0) {
		if (start > 0)
			cnt = (int)(min(reccnt, 16000) - start);
		else
			cnt = (int) min(reccnt, 16000);
	}
	if (allocate)
	{
		if ((err = l_AllocBars(b1, cnt)))
		{
			fclose(fp1);
			return (err);
		}
	}
	b1->type = ctTXT;
	b1->reccnt = reccnt;
	strcpy_s(b1->path, sizeof(b1->path), path);
	strcpy_s(b1->name, sizeof(b1->name), name);
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
	{
		start = max(0, b1->reccnt - cnt);
		fpos = 0;
		fseek(fp1, 0, SEEK_SET);
		l1 = 0;
		l2 = 0;
		do
		{
			l1++;
			if (buffer[0] == '\n' || l1 < info->skip)
				continue;
			if (l2 == start)
			{
				fpos = ftell(fp1) - strlen(buffer) - 1;
				break;
			}
			l2++;
		}
		while (fgets(buffer, 255, fp1));
	}
	fseek(fp1, fpos, SEEK_SET);
	
	i1 = 0;
	while (i1 < cnt && fgets(buffer, 255, fp1) != NULL)
	{
		if (buffer[0] == '\n')
			continue;
		ctArrayItem(b1->dt, i1) = 0;
		ctArrayItem(b1->ti, i1) = 0;
		ctArrayItem(b1->op, i1) = 0;
		ctArrayItem(b1->hi, i1) = 0;
		ctArrayItem(b1->lo, i1) = 0;
		ctArrayItem(b1->cl, i1) = 0;
		ctArrayItem(b1->vol, i1) = 0;
		ctArrayItem(b1->oi, i1) = 0;
		for (i2 = 1; i2 <= maxcol; i2++)
		{
			if (i2 == 1)
				string = strtok_s(buffer, info->delimit, &next_token1);
			else
				string = strtok_s(NULL, info->delimit, &next_token1);
			
			if (info->dt == i2)
			{
				ctArrayItem(b1->dt, i1) = l_DateTxtToJulian(string, info->dtformat);
			}
			if (info->ti == i2)
			{
				ctArrayItem(b1->ti, i1) = l_TimeTxtToJulian(string, info->tiformat);
			}
			if (info->op == i2)
				ctArrayItem(b1->op, i1) = (float)atof(string);
			if (info->hi == i2)
				ctArrayItem(b1->hi, i1) = (float)atof(string);
			if (info->lo == i2)
				ctArrayItem(b1->lo, i1) = (float)atof(string);
			if (info->cl == i2)
				ctArrayItem(b1->cl, i1) = (float)atof(string);
			if (info->vol == i2)
				ctArrayItem(b1->vol, i1) = (float)atof(string);
			if (info->oi == i2)
				ctArrayItem(b1->oi, i1) = (float)atof(string);
		}
		i1++;
	}
	fclose(fp1);
	l_ReSizeBars(b1, i1);
	return (0);
}


short WINAPI l_TXTreadinfo(char *path, char *infile, struct ctTXTinfo *info)
{
	short      i1;
	char     fullname[81];
	char     buffer[81], buffer2[81], buffer3[81];
	char    *dtformat[13] = {"N/A", "YY_MM_DD", "CCYY_MM_DD", "MM_DD_YY", "MM_DD_CCYY", "DD_MM_YY", "DD_MM_CCYY",
		"YYMMDD", "CCYYMMDD", "MMDDYY", "MMDDCCYY", "DDMMYY", "DDMMCCYY"};
	char    *tiformat[5] = {"N/A", "HH_MM_SS", "HH_MM", "HHMMSS", "HHMM"};
	FILE    *fp1;
	
	strcpy_s(fullname, sizeof(fullname), path);
	if (path[0] != '\0' && path[strlen(path) - 1] != ':' && path[strlen(path) - 1] != '/')
		strcat_s(fullname, sizeof(fullname), "/");
	strcat_s(fullname, sizeof(fullname), infile);
	if (fopen_s(&fp1, fullname, _T("rt")))
		return (ctErrFileOpen);
	memset(info, 0, sizeof(struct ctTXTinfo));
	info->delimit[0] = ' ';
	while (fgets(buffer, 80, fp1) != NULL)
	{
		if (buffer[0] == '\n')
			continue;
		i1 = static_cast<short>(strcspn(buffer, "="));
		strncpy_s(buffer2, sizeof(buffer2), buffer, i1);
		buffer2[i1] = '\0';
		l_Rtrim(buffer2);
		l_Ltrim(buffer2);
		strcpy_s(buffer3, sizeof(buffer3), buffer + i1 + 1);
		buffer3[strlen(buffer3) - 1] = '\0';
		l_Ltrim(buffer3);
		l_Rtrim(buffer3);
		if (!_stricmp(buffer2, "delimit"))
		{
			strncpy_s(info->delimit, sizeof(info->delimit), buffer3, 16);
			for (i1 = 0; info->delimit[i1]; i1++)
				if (info->delimit[i1] == '_')
					info->delimit[i1] = ' ';
		}
		if (!_stricmp(buffer2, "skip"))
			info->skip = atoi(buffer3);
		if (!_stricmp(buffer2, "dt"))
			info->dt = atoi(buffer3);
		if (!_stricmp(buffer2, "ti"))
			info->ti = atoi(buffer3);
		if (!_stricmp(buffer2, "op"))
			info->op = atoi(buffer3);
		if (!_stricmp(buffer2, "hi"))
			info->hi = atoi(buffer3);
		if (!_stricmp(buffer2, "lo"))
			info->lo = atoi(buffer3);
		if (!_stricmp(buffer2, "cl"))
			info->cl = atoi(buffer3);
		if (!_stricmp(buffer2, "vol"))
			info->vol = atoi(buffer3);
		if (!_stricmp(buffer2, "oi"))
			info->oi = atoi(buffer3);
		if (!_stricmp(buffer2, "dtformat"))
		{
			for (i1 = 0; i1 < 13; i1++)
				if (!_stricmp(buffer3, dtformat[i1]))
					info->dtformat = i1;
		}
		if (!_stricmp(buffer2, "tiformat"))
		{
			for (i1 = 0; i1 < 5; i1++)
				if (!_stricmp(buffer3, tiformat[i1]))
					info->tiformat = i1;
		}
	}
	fclose(fp1);
	return (0);
}


short WINAPI l_TXTwriteinfo(char *path, char *outfile, struct ctTXTinfo *info)
{
	short      i1;
	char     fullname[81];
	char    *dtformat[13] = {"N/A", "YY_MM_DD", "CCYY_MM_DD", "MM_DD_YY", "MM_DD_CCYY", "DD_MM_YY", "DD_MM_CCYY",
		"YYMMDD", "CCYYMMDD", "MMDDYY", "MMDDCCYY", "DDMMYY", "DDMMCCYY"};
	char    *tiformat[5] = {"N/A", "HH_MM_SS", "HH_MM", "HHMMSS", "HHMM"};
	FILE    *fp1;
	
	strcpy_s(fullname, sizeof(fullname), path);
	if (path[0] != '\0' && path[strlen(path) - 1] != ':' && path[strlen(path) - 1] != '/')
		strcat_s(fullname, sizeof(fullname), "/");
	strcat_s(fullname, sizeof(fullname), outfile);
	if (fopen_s(&fp1, fullname, _T("wt")))
		return (ctErrFileOpen);
	for (i1 = 0; info->delimit[i1]; i1++)
		if (info->delimit[i1] == ' ')
			info->delimit[i1] = '_';
		fprintf(fp1, "delimit  = %s\n", info->delimit);
		fprintf(fp1, "skip     = %i\n", info->skip);
		fprintf(fp1, "dt       = %i\n", info->dt);
		fprintf(fp1, "ti       = %i\n", info->ti);
		fprintf(fp1, "op       = %i\n", info->op);
		fprintf(fp1, "hi       = %i\n", info->hi);
		fprintf(fp1, "lo       = %i\n", info->lo);
		fprintf(fp1, "cl       = %i\n", info->cl);
		fprintf(fp1, "vol      = %i\n", info->vol);
		fprintf(fp1, "oi       = %i\n", info->oi);
		fprintf(fp1, "dtformat = %s\n", dtformat[info->dtformat]);
		fprintf(fp1, "tiformat = %s\n", tiformat[info->tiformat]);
		fclose(fp1);
		return (0);
}
