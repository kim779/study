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


double   l_TT3bytetodouble(unsigned char *threebyter);
double   l_TTcvt(double price, short conversion, short meat);


long WINAPI l_TTfindrec(char *path, char *name, unsigned short date, unsigned short *actualdate)
{
	short    err = 0;
	long     recno = -1, guess = 0, lastguess = 0;
	long     maxent = 0, low = 0, high = 0;
	char     buffer[81];
	FILE    *fp1;
	struct ctTTrec rec;
	
	strcpy_s(buffer, sizeof(buffer), path);
	if (buffer[0] != '\0' && buffer[strlen(buffer) - 1] != ':' && buffer[strlen(buffer) - 1] != '/')
		strcat_s(buffer, sizeof(buffer), "/");
	strcat_s(buffer, sizeof(buffer), name);
	
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);

	fseek(fp1, 0, SEEK_END);
	maxent = ftell(fp1) / sizeof(struct ctTTrec) - 1;
	fseek(fp1, 0, SEEK_SET);
	high = maxent;
	date += 21915;
	while (recno < 0 && !err)
	{
		guess = (low + high + 1) / 2;
		if (guess >= maxent)
			return(ctErrRecordNotFound);
		if (guess == lastguess)
		{
			if (low == 0)
				guess = 0;
			else
			{
				recno = guess;
				break;
			}
		}
		if (fseek(fp1, (guess + 1) * sizeof(struct ctTTrec), SEEK_SET))
		{
			err = ctErrFileRead;
			break;
		}
		if (!fread(&rec, sizeof(struct ctTTrec), 1, fp1))
		{
			err = ctErrFileRead;
			break;
		}
		if (rec.dt == date || guess == 0)
		{
			recno = guess;
			break;
		}
		if (rec.dt > date)
			high = guess;
		if (rec.dt < date)
			low = guess;
		lastguess = guess;
	}
	fclose(fp1);
	if (actualdate)
		*actualdate = rec.dt - 21915;
	if (err)
		return (err);
	return (recno);
}


short WINAPI l_TTread(ctBars *b1, char *path, char *name, long start, short cnt, short allocate)
{
	short    i1 = 0, err = 0, conversion, meat = 0;
	long     reccnt;
	char     buffer[81];
	FILE    *fp1;
	struct ctTTrec rec;
	
	strcpy_s(buffer, sizeof(buffer), path);
	if (buffer[0] != '\0' && buffer[strlen(buffer) - 1] != ':' && buffer[strlen(buffer) - 1] != '/')
		strcat_s(buffer, sizeof(buffer), "/");
	strcat_s(buffer, sizeof(buffer), name);
	
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	
	if (fread((char *) &conversion, 2, 1, fp1) <= 0)
	{
		fclose(fp1);
		return (ctErrFileRead);
	}
	fseek(fp1, 0, SEEK_END);
	reccnt = ftell(fp1) / sizeof(struct ctTTrec) - 1;
	fseek(fp1, 0, SEEK_SET);
	
	if (cnt < 0) {
		if (start > 0)
			cnt = (short)(min((int) reccnt, 16000) - start);
		else
			cnt = (short)(min((int) reccnt, 16000));
	}
	
	if (allocate)
	{
		if ((err = l_AllocBars(b1, cnt)))
		{
			fclose(fp1);
			return (err);
		}
	}
	
	b1->type = ctTT;
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
		start = max(0, b1->reccnt - cnt);
	
	if (fseek(fp1, (start + 1) * sizeof(struct ctTTrec), SEEK_SET))
	{
		fclose(fp1);
		return (ctErrFileSeek);
	}
	if (!_strnicmp(name, "FC_", 3) || !_strnicmp(name, "LC_", 3) ||
		!_strnicmp(name, "LH_", 3) || !_strnicmp(name, "PB_", 3))
		meat = 1;
	for (i1 = 0; i1 < cnt && i1 + start < reccnt && fread((char *) &rec, 16, 1, fp1) > 0; i1++)
	{
		ctArrayItem(b1->dt, i1) = rec.dt - 21915;
		ctArrayItem(b1->ti, i1) = 0;
		ctArrayItem(b1->op, i1) = (float)l_TTcvt(rec.op, conversion, meat);
		ctArrayItem(b1->hi, i1) = (float)l_TTcvt(rec.hi, conversion, meat);
		ctArrayItem(b1->lo, i1) = (float)l_TTcvt(rec.lo, conversion, meat);
		ctArrayItem(b1->cl, i1) = (float)l_TTcvt(rec.cl, conversion, meat);
		ctArrayItem(b1->vol, i1) = (float)l_TT3bytetodouble(rec.vol);
		ctArrayItem(b1->oi, i1) = (float)l_TT3bytetodouble(rec.oi);
	}
	fclose(fp1);
	l_ReSizeBars(b1, i1);
	return (err);
}


double   l_TT3bytetodouble(unsigned char *threebyter)
{
	return ((long) threebyter[0] + ((long) threebyter[1] << 8) + ((long) threebyter[2] << 16));
}


double   l_TTcvt(double price, short conversion, short meat)
{
	double   newprice, whole, fraction;
	
	switch (conversion)
	{
	case 10:
		newprice = price * 10;
		break;
		
	case 3:
		newprice = price / 1000;
		break;
		
	case 2:
		newprice = price / 100;
		if (meat)
			if ((int) (price) % 5 != 0) /* add to .02 and .07 */
				newprice += .005F;
			break;
			
	case 1:
		newprice = price / 10;
		break;
		
	case 0:
		newprice = price;
		break;
		
	case -1:
		whole = floor(price / 10);
		fraction = price - 10 * whole;
		newprice = whole + fraction / 2;
		break;
		
	case -2:
		whole = floor(price / 10);
		fraction = price - 10 * whole;
		newprice = whole + fraction / 4;
		break;
		
	case -3:
		whole = floor(price / 10);
		fraction = price - 10 * whole;
		newprice = whole + fraction / 8;
		break;
		
	case -4:
		whole = floor(price / 100);
		fraction = price - 100 * whole;
		newprice = whole + fraction / 16;
		break;
		
	case -5:
		whole = floor(price / 100);
		fraction = price - 100 * whole;
		newprice = whole + fraction / 32;
		break;
		
	case -6:
		whole = floor(price / 100);
		fraction = price - 100 * whole;
		newprice = whole + fraction / 64;
		break;
		
	case -15:
		newprice = price / 32.0;
		break;
		
	case -16:
		newprice = price / 64.0;
		break;
		
	case -17:
		newprice = price / 128.0;
		break;
		
	case -22:
		newprice = price * .05;
		break;
		
	default:
		return (ctErrInvalidParm);
	}
	return (newprice);
}

