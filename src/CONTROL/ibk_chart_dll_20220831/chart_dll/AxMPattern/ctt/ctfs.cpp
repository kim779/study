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

short    l_FSfracrec(struct ctFSfracs *fracs, short record, char *path);
double   l_FSfixprice(struct ctFSfracs fracs, unsigned long price);


short WINAPI l_FSread(ctBars *b1, char *path, char *name, long start, short cnt, short allocate)
{
	short    i1, err, recno, done = 0;
	long     l1, reccnt;
	char     buffer[81];
	FILE    *fp1;
	struct ctFSinfo info, voihdr;
	struct ctFSdaily16 daily16;
	struct ctFSdaily32 daily32;
	struct ctFSvoi16 voi16;
	struct ctFSvoi32 voi32;
	struct ctFSfracs fracs;
	
	strcpy_s(buffer, sizeof(buffer), path);
	if (buffer[0] != '\0' && buffer[strlen(buffer) - 1] != ':' && buffer[strlen(buffer) - 1] != '/')
		strcat_s(buffer, sizeof(buffer), "/");
	strcat_s(buffer, sizeof(buffer), name);
	
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	fread((char *) &info, sizeof(struct ctFSinfo), 1, fp1);
	if (info.version)
	{
		fclose(fp1);
		return (ctErrInvalidParm);
	}
	fseek(fp1, 0L, SEEK_END);
	l1 = ftell(fp1);
	reccnt = (l1 - info.dataoffset) / info.recordlen;
	
	if (cnt < 0) {
		if (start > 0)
			cnt = (short)(min(reccnt, 16000) - start);
		else
			cnt = (short)(min(reccnt, 16000));
	}
	
	if (allocate)
	{
		if ((err = l_AllocBars(b1, cnt)))
			return (err);
	}
	
	b1->type = ctFS;
	b1->reccnt = reccnt;
	strcpy_s(b1->path, sizeof(b1->path), path);
	strcpy_s(b1->name, sizeof(b1->name), name);
	strcpy_s(b1->description, sizeof(b1->description), info.desc);
	strcpy_s(b1->symbol, sizeof(b1->symbol), info.symbolname);
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
	if (start > reccnt)
	{
		fclose(fp1);
		return (ctErrRecordNotFound);
	}
	recno = (short)start;
	fseek(fp1, recno * info.recordlen + info.dataoffset, SEEK_SET);
	i1 = 0;
	cnt = min(cnt, b1->datasize);
	
	l_FSfracrec(&fracs, info.fraction, path);
	
	if (info.bit32values == 1)
	{
		while (i1 < cnt && recno <= reccnt && fread((char *) &daily32, sizeof(struct ctFSdaily32), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = daily32.date;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = (float) l_FSfixprice(fracs, daily32.open);
			ctArrayItem(b1->hi, i1) = (float) l_FSfixprice(fracs, daily32.high);
			ctArrayItem(b1->lo, i1) = (float) l_FSfixprice(fracs, daily32.low);
			ctArrayItem(b1->cl, i1) = (float) l_FSfixprice(fracs, daily32.close);
			ctArrayItem(b1->vol, i1) = 0;
			ctArrayItem(b1->oi, i1) = 0;
			i1++;
			recno++;
		}
	}
	else
	{
		while (i1 < cnt && recno <= reccnt && fread((char *) &daily16, sizeof(struct ctFSdaily16), 1, fp1) > 0)
		{
			ctArrayItem(b1->dt, i1) = daily16.date;
			ctArrayItem(b1->ti, i1) = 0;
			ctArrayItem(b1->op, i1) = (float) l_FSfixprice(fracs, daily16.open);
			ctArrayItem(b1->hi, i1) = (float) l_FSfixprice(fracs, daily16.high);
			ctArrayItem(b1->lo, i1) = (float) l_FSfixprice(fracs, daily16.low);
			ctArrayItem(b1->cl, i1) = (float) l_FSfixprice(fracs, daily16.close);
			ctArrayItem(b1->vol, i1) = 0;
			ctArrayItem(b1->oi, i1) = 0;
			i1++;
			recno++;
		}
	}
	fclose(fp1);
	l_ReSizeBars(b1, i1);
	
	if (info.voifileexists)
	{
		buffer[strlen(buffer) - 2] = 'V';
		buffer[strlen(buffer) - 1] = 'I';

		if (fopen_s(&fp1, buffer, _T("rb")))
		{
			info.voifileexists = 0;
			return (0);
		}
		i1 = 0;
		voi32.date = 0;
		voi16.date = 0;
		fread((char *) &voihdr, sizeof(struct ctFSinfo), 1, fp1);
		fseek(fp1, (long) (voihdr.dataoffset), SEEK_SET);
		if (voihdr.bit32values == 1)
		{
			while (!done)
			{
				if (ctArrayItem(b1->dt, i1) < voi32.date)
				{
					i1++;
					if (i1 > b1->size)
						done = 1;
				}
				else if (ctArrayItem(b1->dt, i1) > voi32.date)
				{
					if (fread((char *) &voi32, sizeof(struct ctFSvoi32), 1, fp1) <= 0)
						done = 1;
				}
				else
				{
					ctArrayItem(b1->vol, i1) = (float) voi32.vol;
					ctArrayItem(b1->oi, i1) = (float) voi32.oi;
					i1++;
				}
			}
		}
		else /* 16bit */
		{
			while (!done)
			{
				if (ctArrayItem(b1->dt, i1) < voi16.date)
				{
					i1++;
					if (i1 > cnt)
						done = 1;
				}
				else if (ctArrayItem(b1->dt, i1) > voi16.date)
				{
					if (fread((char *) &voi16, sizeof(struct ctFSvoi16), 1, fp1) <= 0)
						done = 1;
				}
				else
				{
					ctArrayItem(b1->vol, i1) = (float) voi16.vol;
					ctArrayItem(b1->oi, i1) = (float) voi16.oi;
					i1++;
				}
			}
		}
	}
	return (0);
}


short    l_FSfracrec(struct ctFSfracs *fracs, short record, char *path)
{
	char     buffer[81];
	FILE    *fp1;
	struct ctFStablehdr tablehdr;
	
	fracs->xmitpower = 1;
	fracs->xmitdenom = 1;
	
	strcpy_s(buffer, sizeof(buffer), path);
	if (buffer[0] != '\0' && buffer[strlen(buffer) - 1] != ':' && buffer[strlen(buffer) - 1] != '/')
		strcat_s(buffer, sizeof(buffer), "/");
	strcat_s(buffer, sizeof(buffer), "fsfracs.fil");
	
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (-2);
	fread((char *) &tablehdr, sizeof(struct ctFStablehdr), 1, fp1);
	if (tablehdr.version > 300)
		fseek(fp1, (long) ((1 + record) * tablehdr.recordlen), SEEK_SET);
	else
		fseek(fp1, (long) (4 + record * tablehdr.recordlen), SEEK_SET);
	fread(fracs, tablehdr.recordlen, 1, fp1);
	fclose(fp1);
	return (0);
}


double   l_FSfixprice(struct ctFSfracs fracs, unsigned long price)
{
	double   f1;
	ldiv_t   num;
	
	num = ldiv(price, (long)fracs.xmitpower);
	f1 = (double)num.quot + (double)num.rem / fracs.xmitdenom;
	return (f1);
}