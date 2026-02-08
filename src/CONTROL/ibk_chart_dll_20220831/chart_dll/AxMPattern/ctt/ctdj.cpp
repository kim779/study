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

short l_DJfindfile(char *path, char *name, struct ctDJinfo *info1);

short WINAPI l_DJlist(char *path, char *outfile, short append)
{
	short      err = 0, i1, reccnt = 0, line = 0, page = 0;
	char     buffer[81];
	FILE    *fp1, *out;
	struct ctDJinfo info;
	
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
	if (path[0] != '\0' && path[strlen(path) - 1] != ':' && path[strlen(path) - 1] != '/')
		strcat_s(buffer, sizeof(buffer), "/");
	strcat_s(buffer, sizeof(buffer), _T("mapinfo.dat"));
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	if (fread((char *) &info, sizeof(struct ctDJinfo), 1, fp1) == 0)
		err = ctErrFileRead;
	else
		reccnt = info.reccnt;
	fprintf(out, "NUM SYMBOL   D E NAME            TAG\n");
	fprintf(out, "--- -------- - - --------------- ---\n");
	line = 2;
	for (i1 = 0; i1 < reccnt && !err; i1++)
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
		
		if (fread((char *) &info, sizeof(struct ctDJinfo), 1, fp1) == 0)
			err = ctErrFileRead;
		else
			fprintf(out, "%3i %8.8s %c %c %15.15s %3.3s\n", i1 + 1, info.symbol,
			info.datatype, info.equitytype, info.company, info.tag);
	}
	fclose(fp1);
	
#ifndef _WINDOWS
	if (l_stricmp(outfile, "STDOUT"))
#endif
		fclose(out);
	return (err);
}


short      l_DJfindfile(char *path, char *name, struct ctDJinfo *info1)
{
	int      err = 0, i1 = 0, reccnt = 0;
	char     buffer[81], shortname[9], company[16], symbol[11], equitytype, datatype;
	FILE    *fp1;
	struct ctDJinfo info;
	struct ctDJinfohdr infohdr;
	
	i1 = strlen(name);
	equitytype = name[i1 - 3];
	datatype = name[i1 - 1];
	strncpy_s(shortname, sizeof(shortname), name, i1 - 4);
	shortname[i1 - 4] = '\0';
	
	strcpy_s(buffer, sizeof(buffer), path);
	if (path[0] != '\0' && path[strlen(path) - 1] != ':' && path[strlen(path) - 1] != '/')
		strcat_s(buffer, sizeof(buffer), "/");
	strcat_s(buffer, sizeof(buffer), "mapinfo.dat");
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	if (fread((char *) &infohdr, sizeof(struct ctDJinfohdr), 1, fp1) == 0)
	{
		fclose(fp1);
		return (ctErrFileRead);
	}
	reccnt = infohdr.reccnt;
	for (i1 = 0; i1 < reccnt && !err; i1++)
	{
		if (fread((char *) &info, sizeof(struct ctDJinfo), 1, fp1) == 0)
			err = ctErrFileRead;
		else
		{
			sprintf_s(symbol, sizeof(symbol), "%8.8s", info.symbol);
			sprintf_s(company, sizeof(company), "%15.15s", info.company);
			l_Rtrim(symbol);
			l_Rtrim(company);
			if ((!_stricmp(shortname, symbol) || !_stricmp(shortname, company) &&
				equitytype == info.equitytype && datatype == info.datatype))
			{
				memcpy(info1, &info, sizeof(struct ctDJinfo));
				break;
			}
		}
	}
	if (i1 == reccnt)
		err = ctErrFileNotFound;
	fclose(fp1);
	return (err);
}


short WINAPI l_DJread(ctBars *b1, char *path, char *name, long start, short cnt, short allocate)
{
	int    err = 0, i1, i2;
	char     buffer[81], name1[9];
	FILE    *fp1;
	struct ctDJinfo info;
	struct ctDJdc dc;          /* 1 */
	struct ctDJdvc dvc;        /* 2 */
	struct ctDJdhlc dhlc;      /* 3 */
	struct ctDJdvhlc dvhlc;    /* 4 */
	struct ctDJdhlco dhlco;    /* 5 */
	struct ctDJdihlco dihlco;  /* 6 */
	struct ctDJdvhlco dvhlco;  /* 7 */
	struct ctDJdvihlco dvihlco;/* 8 */
	struct ctDJdiv div;        /* 9 */
	
	
	strcpy_s(name1, sizeof(name1), name);
	i2 = 1;
	while (name[(i1 = strcspn(name1, "+<>|=;/"))])
		name1[i1] = '0' + (i2++);
	
	if ((err = l_DJfindfile(path, name1, &info)))
		return (err);
	if (path[0] != '\0' && path[strlen(path) - 1] != ':' && path[strlen(path) - 1] != '/')
		sprintf_s(buffer, sizeof(buffer), "%s/%s", path, name1);
	else
		sprintf_s(buffer, sizeof(buffer), "%s%s", path, name1);
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	if (cnt < 0) {
		if (start > 0)
			cnt = (short)(min((int) info.reccnt, 16000) - start);
		else
			cnt = (short)(min((int) info.reccnt, 16000));
	}
		
	if (allocate)
	{
		if ((err = l_AllocBars(b1, cnt)))
		{
			fclose(fp1);
			return (err);
		}
	}
	
	b1->type = ctDJ;
	strcpy_s(b1->path, sizeof(b1->path), path);
	strcpy_s(b1->name, sizeof(b1->name), name);
	b1->reccnt = info.reccnt;
	strcpy_s(b1->description, sizeof(b1->description), info.company);
	strcpy_s(b1->symbol, sizeof(b1->symbol), info.symbol);
	strcpy_s(b1->cusip, sizeof(b1->cusip), info.cusip);
	b1->begindate = info.begindt + ctDAYSFROM1920;
	b1->enddate = info.enddt + ctDAYSFROM1920;
	switch (info.datatype)
	{
	case 'D':
		b1->frequency = ctIDAILY;
		break;
	case 'W':
		b1->frequency = ctWEEKLY;
		break;
	case 'M':
		b1->frequency = ctMONTHLY;
		break;
	default:
		b1->frequency = ctUNKNOWN;
		break;
	}
	switch (info.equitytype)
	{
	case 'B':
	case 'T':
		b1->datatype = ctBOND;
		break;
	case 'V':
		b1->datatype = ctCOMMODITY;
		break;
	case 'M':
		b1->datatype = ctMUTUALFUND;
		break;
	case 'O':
		b1->datatype = ctOPTION;
		break;
	case 'S':
		b1->datatype = ctSTOCK;
		break;
	case 'W':
	case 'X':
	case 'Y':
		b1->datatype = ctINDEX;
		break;
	default:
		b1->datatype  = ctUNKNOWN;
		break;
	}
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
	
	switch (info.content)
	{
	case '1':
		i1 = sizeof(struct ctDJdc);
		break;
	case '2':
		i1 = sizeof(struct ctDJdvc);
		break;
	case '3':
		i1 = sizeof(struct ctDJdhlc);
		break;
	case '0':
	case '4':
		i1 = sizeof(struct ctDJdvhlc);
		break;
	case '5':
		i1 = sizeof(struct ctDJdhlco);
		break;
	case '6':
		i1 = sizeof(struct ctDJdihlco);
		break;
	case '7':
		i1 = sizeof(struct ctDJdvhlco);
		break;
	case '8':
		i1 = sizeof(struct ctDJdvihlco);
		break;
	case '9':
		i1 = sizeof(struct ctDJdiv);
		break;
	}
	
	if (fseek(fp1, start * i1, SEEK_SET))
	{
		fclose(fp1);
		return (ctErrFileSeek);
	}
	
	i1 = 0;
	switch (info.content)
	{
	case '1':
		while (i1 < cnt && fread((char *) &dc, sizeof(struct ctDJdc), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = dc.dt + ctDAYSFROM1920;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = 0;
			ctArrayItem(b1->hi, i1) = 0;
			ctArrayItem(b1->lo, i1) = 0;
			ctArrayItem(b1->cl, i1) = dc.cl;
			ctArrayItem(b1->vol, i1) = 0;
			ctArrayItem(b1->oi, i1) = 0;
			i1++;
		}
		break;
		
	case '2':
		while (i1 < cnt && fread((char *) &dvc, sizeof(struct ctDJdvc), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = dvc.dt + ctDAYSFROM1920;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = 0;
			ctArrayItem(b1->hi, i1) = 0;
			ctArrayItem(b1->lo, i1) = 0;
			ctArrayItem(b1->cl, i1) = dvc.cl;
			ctArrayItem(b1->vol, i1) = (float) dvc.vol;
			ctArrayItem(b1->oi, i1) = 0;
			i1++;
		}
		break;
		
	case '3':
		while (i1 < cnt && fread((char *) &dhlc, sizeof(struct ctDJdhlc), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = dhlc.dt + ctDAYSFROM1920;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = 0;
			ctArrayItem(b1->hi, i1) = dhlc.hi;
			ctArrayItem(b1->lo, i1) = dhlc.lo;
			ctArrayItem(b1->cl, i1) = dhlc.cl;
			ctArrayItem(b1->vol, i1) = 0;
			ctArrayItem(b1->oi, i1) = 0;
			i1++;
		}
		break;
		
	case '0':
	case '4':
		while (i1 < cnt && fread((char *) &dvhlc, sizeof(struct ctDJdvhlc), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = dvhlc.dt + ctDAYSFROM1920;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = 0;
			ctArrayItem(b1->hi, i1) = dvhlc.hi;
			ctArrayItem(b1->lo, i1) = dvhlc.lo;
			ctArrayItem(b1->cl, i1) = dvhlc.cl;
			ctArrayItem(b1->vol, i1) = (float) dvhlc.vol;
			ctArrayItem(b1->oi, i1) = 0;
			i1++;
		}
		break;
		
	case '5':
		while (i1 < cnt && fread((char *) &dhlco, sizeof(struct ctDJdhlco), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = dhlco.dt + ctDAYSFROM1920;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = dhlco.op;
			ctArrayItem(b1->hi, i1) = dhlco.hi;
			ctArrayItem(b1->lo, i1) = dhlco.lo;
			ctArrayItem(b1->cl, i1) = dhlco.cl;
			ctArrayItem(b1->vol, i1) = 0;
			ctArrayItem(b1->oi, i1) = 0;
			i1++;
		}
		break;
		
	case '6':
		while (i1 < cnt && fread((char *) &dihlco, sizeof(struct ctDJdihlco), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = dihlco.dt + ctDAYSFROM1920;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = dihlco.op;
			ctArrayItem(b1->hi, i1) = dihlco.hi;
			ctArrayItem(b1->lo, i1) = dihlco.lo;
			ctArrayItem(b1->cl, i1) = dihlco.cl;
			ctArrayItem(b1->vol, i1) = 0;
			ctArrayItem(b1->oi, i1) = (float) dihlco.oi;
			i1++;
		}
		break;
		
	case '7':
		while (i1 < cnt && fread((char *) &dvhlco, sizeof(struct ctDJdvhlco), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = dvhlco.dt + ctDAYSFROM1920;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = dvhlco.op;
			ctArrayItem(b1->hi, i1) = dvhlco.hi;
			ctArrayItem(b1->lo, i1) = dvhlco.lo;
			ctArrayItem(b1->cl, i1) = dvhlco.cl;
			ctArrayItem(b1->vol, i1) = (float) dvhlco.vol;
			ctArrayItem(b1->oi, i1) = 0;
			i1++;
		}
		break;
		
	case '8':
		while (i1 < cnt && fread((char *) &dvihlco, sizeof(struct ctDJdvihlco), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = dvihlco.dt + ctDAYSFROM1920;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = dvihlco.op;
			ctArrayItem(b1->hi, i1) = dvihlco.hi;
			ctArrayItem(b1->lo, i1) = dvihlco.lo;
			ctArrayItem(b1->cl, i1) = dvihlco.cl;
			ctArrayItem(b1->vol, i1) = (float) dvihlco.vol;
			ctArrayItem(b1->oi, i1) = (float) dvihlco.oi;
			i1++;
		}
		break;
		
	case '9':
		while (i1 < cnt && fread((char *) &div, sizeof(struct ctDJdiv), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = div.dt + ctDAYSFROM1920;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = 0;
			ctArrayItem(b1->hi, i1) = 0;
			ctArrayItem(b1->lo, i1) = 0;
			ctArrayItem(b1->cl, i1) = 0;
			ctArrayItem(b1->vol, i1) = (float) div.vol;
			ctArrayItem(b1->oi, i1) = (float) div.oi;
			i1++;
		}
		break;
	}
	fclose(fp1);
	l_ReSizeBars(b1, i1);
	return (err);
}

