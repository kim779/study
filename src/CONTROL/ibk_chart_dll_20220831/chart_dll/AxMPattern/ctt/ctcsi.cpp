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


short    l_CSIfindfile(char *path, char *name, struct ctCSImrec *mrec1);
double   l_CSIcvt(double number, short  cfac);
unsigned short l_CSIdatetojulian(float date);


short l_CSIfindfile(char *path, char *name, struct ctCSImrec *mrec1)
{
	short       err = 0, i1;
	char     buffer[81]{ 0, }, symbol[7];
	FILE    *fp1 = (FILE *) 0;
	struct ctCSImrec mrec;
	
	strcpy_s(buffer, sizeof(buffer), path);
	if (path[0] != '\0' && path[strlen(path) - 1] != ':' && path[strlen(path) - 1] != '/')
		strcat_s(buffer, sizeof(buffer), "/");
	strcat_s(buffer, sizeof(buffer), "qmaster");
	errno_t eno = fopen_s(&fp1, buffer, "rb");
	if (eno) return (ctErrFileOpen);
	
	for (i1 = 0; i1 < 120; i1++)
	{
		if (fread((char *) &mrec, sizeof(struct ctCSImrec), 1, fp1) == 0)
		{
			err = ctErrFileRead;
			break;
		}
		if (mrec.dflag == '0')
		{
			if (mrec.csflag == 'C')
			{
				if (mrec.dm[1] == '0')
				{
					buffer[0] = mrec.dm[1];
					buffer[1] = mrec.dm[0];
				}
				else
				{
					buffer[0] = mrec.dm[0];
					buffer[1] = mrec.dm[1];
				}
				sprintf_s(symbol, sizeof(symbol), "%2.2s%2.2s%2.2s", mrec.csym, buffer, mrec.dy);
			}
			else
			{
				sprintf_s(symbol, sizeof(symbol), "%6.6s", mrec.ssym);
				l_Rtrim(symbol);
			}
			strncpy_s(buffer, sizeof(buffer), mrec.name, 20);
			buffer[20] = '\0';
			l_Rtrim(buffer);
			if (!_stricmp(name, symbol) || !_stricmp(name, buffer))
			{
				memcpy(mrec1, &mrec, sizeof(struct ctCSImrec));
				break;
			}
		}
		else
		{
			if (mrec.id[0] == '9')   /* end of block */
			{
				err = ctErrFileNotFound;
				break;
			}
		}
	}
	if (i1 == 120)
		err = ctErrFileNotFound;
	fclose(fp1);
	if (err)
		return (err);
	else
		return (i1 + 1);
}


double   l_CSIcvt(double number, short  cfac)
{
	double   div;
	double   num, rem;
	
	if (cfac == 0)
		return (number);
	if (cfac > 0 && cfac < 100)
		return (number / pow(10, (double) cfac));
	cfac = abs(cfac);
	if (cfac == 1)
		div = 10;
	else if (cfac <= 4)
		div = 100;
	else
		div = 1000;
	number = number / div;
	rem = modf(number, &num);
	if (rem == 0)
		return (num);
	number = num + (div * rem) / (8 * pow(2, (double) cfac - 1));
	return (number);
}


unsigned short l_CSIdatetojulian(float date)
{
	long     year, month, day;
	
	year = (long) (date / 10000);
	month = (long) ((date - year * 10000) / 100);
	day = (long) ((date - year * 10000 - month * 100));
	return (l_YMDToJulian((short) year, (short) month, (short) day));
}


short WINAPI l_CSIlist(char *path, char *outfile, short  append)
{
	short    i1, err = 0, line = 0, page = 0;
	char     buffer[81], symbol[7];
	errno_t  eno;
	FILE    *fp1 = (FILE *) 0, *out = (FILE *) 0;
	struct ctCSImrec mrec;
	
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
		if (append)
			eno = fopen_s(&out, outfile, _T("at"));
		else
			eno = fopen_s(&out, outfile, _T("wt"));

		if (eno) return (ctErrFileOpen);
	}
	strcpy_s(buffer, sizeof(buffer), path);
	if (path[0] != '\0' && path[strlen(path) - 1] != ':' && path[strlen(path) - 1] != '/')
		strcat_s(buffer, sizeof(buffer), "/");
	strcat_s(buffer, sizeof(buffer), "qmaster");
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	
	fprintf(out, "NUM SYMBOL T NAME\n");
	fprintf(out, "--- ------ - --------------------\n");
	line = 2;
	for (i1 = 0; i1 < 120; i1++)
	{
		if (fread((char *) &mrec, sizeof(struct ctCSImrec), 1, fp1) == 0)
		{
			err = ctErrFileRead;
			break;
		}
		if (mrec.dflag == '0')
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
			if (mrec.csflag == 'C')
			{
				if (mrec.dm[1] == '0')
				{
					buffer[0] = mrec.dm[1];
					buffer[1] = mrec.dm[0];
				}
				else
				{
					buffer[0] = mrec.dm[0];
					buffer[1] = mrec.dm[1];
				}
				sprintf_s(symbol, sizeof(symbol), "%2.2s%2.2s%2.2s", mrec.csym, buffer, mrec.dy);
			}
			else
			{
				sprintf_s(symbol, sizeof(symbol), "%6.6s", mrec.ssym);
			}
			fprintf(out, "%3i %6s %c %20.20s\n", i1 + 1, symbol, mrec.type, mrec.name);
		}
		else
		{
			if (mrec.id[0] == '9')
				break;
		}
	}
	fclose(fp1);
	
#ifndef _WINDOWS
	if (l_stricmp(outfile, "STDOUT"))
#endif
		fclose(out);
	return (err);
}


long WINAPI l_CSIfindrec(char *path, char *name, unsigned short date,
						 unsigned short *actualdate)
{
	short       err = 0, fnum;
	short    year, month, day;
	float    dt = 0;
	long     recno = -1, guess = 0, lastguess = 0;
	long     maxent = 0, low = 0, high = 0;
	char     buffer[81];
	FILE    *fp1;
	struct ctCSImrec mrec;
	struct ctCSIrec rec;
	struct ctCSIhdr hdr;
	
	if ((fnum = l_CSIfindfile(path, name, &mrec)) < 0)
		return (fnum);
	if (path[0] != '\0' && path[strlen(path) - 1] != ':' && path[strlen(path) - 1] != '/')
		sprintf_s(buffer, sizeof(buffer), _T("%s/F%03.3i.DTA"), path, fnum);
	else
		sprintf_s(buffer, sizeof(buffer), "%sF%03.3i.DTA", path, fnum);
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	if (fread((char *) &hdr, sizeof(struct ctCSIhdr), 1, fp1) <= 0)
	{
		fclose(fp1);
		return (ctErrFileRead);
	}
	l_MStoIEEE(&hdr.ferp, &hdr.ferp);
	l_MStoIEEE(&hdr.mdp, &hdr.mdp);
	l_MStoIEEE(&hdr.hh, &hdr.hh);
	l_MStoIEEE(&hdr.ll, &hdr.ll);
	l_MStoIEEE(&hdr.fpdof, &hdr.fpdof);
	l_MStoIEEE(&hdr.lpdof, &hdr.lpdof);
	
	if (date == 0)
		dt = 0;
	else
	{
		l_JulianToYMD(date, &year, &month, &day);
		dt = (float)year * 10000 + month * 100 + day;
	}
	if (dt > hdr.lpdof)
	{
		fclose(fp1);
		if (actualdate)
			*actualdate = l_CSIdatetojulian(hdr.lpdof);
		return (ctErrRecordNotFound);
	}
	maxent = (long) hdr.mdp - 1;
	high = maxent;
	while (recno < 0 && !err)
	{
		guess = (low + high + 1) / 2;
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
		if (fseek(fp1, guess * sizeof(struct ctCSIrec) + sizeof(struct ctCSIhdr), SEEK_SET))
		{
			err = ctErrFileRead;
			break;
		}
		if (!fread(&rec, sizeof(struct ctCSIrec), 1, fp1))
		{
			err = ctErrFileRead;
			break;
		}
		l_MStoIEEE(&rec.date, &rec.date);
		if (rec.date == dt || guess == 0)
		{
			recno = guess;
			break;
		}
		if (rec.date > dt)
			high = guess;
		if (rec.date < dt)
			low = guess;
		lastguess = guess;
	}
	
	fclose(fp1);
	if (actualdate)
		*actualdate = l_CSIdatetojulian(rec.date);
	if (err)
		return (err);
	return (recno);
}


short WINAPI l_CSIread(ctBars *b1, char *path, char *name, long start, short  cnt, short  allocate)
{
	short    i1 = 0, err = 0, cf, fnum;
	long     recnum;
	char     buffer[80];
	FILE    *fp1;
	struct ctCSImrec mrec;
	struct ctCSIrec rec;
	struct ctCSIhdr hdr;
	
	if ((fnum = l_CSIfindfile(path, name, &mrec)) < 0)
		return (fnum);
	
	if (path[0] != '\0' && path[strlen(path) - 1] != ':' && path[strlen(path) - 1] != '/')
		sprintf_s(buffer, sizeof(buffer), "%s/F%03.3i.DTA", path, fnum);
	else
		sprintf_s(buffer, sizeof(buffer), "%sF%03.3i.DTA", path, fnum);

	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	if (fread((char *) &hdr, sizeof(struct ctCSIhdr), 1, fp1) <= 0)
	{
		fclose(fp1);
		return (ctErrFileRead);
	}
	l_MStoIEEE(&hdr.ferp, &hdr.ferp);
	l_MStoIEEE(&hdr.mdp, &hdr.mdp);
	l_MStoIEEE(&hdr.hh, &hdr.hh);
	l_MStoIEEE(&hdr.ll, &hdr.ll);
	l_MStoIEEE(&hdr.fpdof, &hdr.fpdof);
	l_MStoIEEE(&hdr.lpdof, &hdr.lpdof);
	
	if (cnt < 0)
		if (start > 0)
			cnt = (short)(min((short ) hdr.mdp, 16000) - start);
		else
			cnt = (short)(min((short ) hdr.mdp, 16000));
		
	if (allocate)
	{
		if ((err = l_AllocBars(b1, cnt)))
		{
			fclose(fp1);
			return (err);
		}
	}
	buffer[0] = mrec.cf[0];
	buffer[1] = mrec.cf[1];
	buffer[2] = '\0';
	cf = atoi(buffer);
	
	b1->reccnt = (long) hdr.mdp - 1;
	strcpy_s(b1->path, sizeof(b1->path), path);
	strcpy_s(b1->name, sizeof(b1->name), name);
	strncpy_s(b1->description, sizeof(b1->description), mrec.name, 20);
	b1->description[20]  = '\0';
	l_Rtrim(b1->description);
	if (mrec.dm[1] == '0')
	{
		mrec.dm[1] = mrec.dm[0];
		mrec.dm[0] = '0';
	}
	if (mrec.csflag == 'C')
		sprintf_s(b1->symbol, sizeof(b1->symbol), "%2.2s%2.2s%2.2s", mrec.csym, mrec.dm, mrec.dy);
	else
		sprintf_s(b1->symbol, sizeof(b1->symbol), "%6.6s", mrec.ssym);
	l_Rtrim(b1->symbol);
	b1->cusip[0] = '\0';
	b1->begindate = l_CSIdatetojulian(hdr.fpdof);
	b1->enddate = l_CSIdatetojulian(hdr.lpdof);
	if (mrec.type == 'D')
		b1->frequency = ctIDAILY;
	else if (mrec.type == 'W')
		b1->frequency = ctWEEKLY;
	else if (mrec.type == 'M')
		b1->frequency = ctMONTHLY;
	else 
		b1->frequency = ctUNKNOWN;
	if (mrec.csflag == 'C')
		b1->datatype = ctCOMMODITY;
	else if (mrec.csflag == 'S')
		b1->datatype = ctSTOCK;
	else
		b1->datatype = ctUNKNOWN;
	if (mrec.oflag == 'P')
		b1->optiontype = ctPUT;
	else if (mrec.oflag == 'C')
		b1->optiontype = ctCALL;
	else
		b1->optiontype = ctUNKNOWN;
	sprintf_s(buffer, sizeof(buffer), "%2.2s%2.2s01", mrec.dy, mrec.dm);
	b1->deliverydate = l_DateTxtToJulian(buffer, DTFORMAT_YYMMDD);
	b1->strikeprice = (float)atof(mrec.sprice);
	b1->type = ctCSI;
	
	if (cnt == 0)
	{
		fclose(fp1);
		return (0);
	}
	cnt = min(cnt, b1->datasize);
	if (start < 0)
		start = max(0, b1->reccnt - cnt);
	
	if (fseek(fp1, start * sizeof(struct ctCSIrec) + sizeof(struct ctCSIhdr), SEEK_SET))
	{
		fclose(fp1);
		return (ctErrFileSeek);
	}
	i1 = 0;
	recnum = start;
	while (i1 < cnt && recnum < (short ) hdr.mdp && fread((char *) &rec, sizeof(struct ctCSIrec), 1, fp1) > 0)
	{
		recnum++;
		/*
		if (!rec.open && !rec.high && !rec.low && !rec.close && cnt > 1)
		continue;
		*/
		l_MStoIEEE(&rec.date, &rec.date);
		ctArrayItem(b1->dt, i1) = l_CSIdatetojulian(rec.date);
		ctArrayItem(b1->ti, i1) = 0;
		ctArrayItem(b1->op, i1) = (float)l_CSIcvt(rec.open + (long)rec.exop * 65536, cf);
		ctArrayItem(b1->hi, i1) = (float)l_CSIcvt(rec.high + (long)rec.exhi * 65536, cf);
		ctArrayItem(b1->lo, i1) = (float)l_CSIcvt(rec.low + (long)rec.exlo * 65536, cf);
		ctArrayItem(b1->cl, i1) = (float)l_CSIcvt(rec.close + (long)rec.excl * 65536, cf);
		ctArrayItem(b1->vol, i1) = rec.tvol + (float) rec.tvol1 * 65536;
		ctArrayItem(b1->oi, i1) = rec.toi + (float) rec.toi1 * 65536;
		i1++;
	}
	fclose(fp1);
	l_ReSizeBars(b1, i1);
	return (err);
}
