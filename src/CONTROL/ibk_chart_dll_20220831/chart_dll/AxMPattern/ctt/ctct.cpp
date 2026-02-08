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


short      l_CTfindfile(char *path, char *name, struct ctCTdata *data1, char *path1);
short      l_CTreadDOP(char *path, short *cols, short *inputfactor, short *dispfactor);
short      l_CTdefaultDOP(short numfields, short *cols, short *inputfactor, short *dispfactor);
unsigned short l_CTtimetojulian(float time);
unsigned short l_CTdatetojulian(float date);


short      l_CTfindfile(char *path, char *name, struct ctCTdata *data1, char *path1)
{
	short      i1, i2, err, ptr = 0, numentries;
	short      stack[33];
	char     buffer[81], buffer2[21], cwd[81], *p1;
	FILE    *fp1;
	struct ctCTmasthdr masthdr;
	struct ctCTdata data;
	struct ctCTdir dir;
	
	strcpy_s(cwd, sizeof(cwd), path);
	if (cwd[0] != '\0' && cwd[strlen(cwd) - 1] != ':' && cwd[strlen(cwd) - 1] != '/')
		strcat_s(cwd, sizeof(cwd), "/");
	stack[0] = 0;
	while (1)
	{
		strcpy_s(buffer, sizeof(buffer), cwd);
		strcat_s(buffer, sizeof(buffer), _T("master"));
		err = 0;

		if (fopen_s(&fp1, buffer, _T("rb")))
			err = ctErrFileOpen;
		else
			if (fread((char *) &masthdr, sizeof(struct ctCTmasthdr), 1, fp1) == 0)
			{
				fclose(fp1);
				err = ctErrFileRead;
			}
			if (err)
			{
				stack[ptr] = 1;
				numentries = 0;
			}
			else
			{
				numentries = masthdr.numentries;
				if (stack[ptr] == 0)          /* i.e. first time in this master */
				{
					for (i1 = 0; i1 < numentries; i1++)
					{
						if (fread((char *) &dir, sizeof(struct ctCTdir), 1, fp1) == 0)
						{
							fclose(fp1);
							stack[ptr] = 1;
							numentries = 0;
							break;
						}
						if (!strncmp(dir.directory, "<DIR>", 5) && dir.name[0] != ' ')
						{
							if (stack[ptr] == 0)
								stack[ptr] = i1 + 1;
						}
						else
						{
							memcpy(&data, &dir, sizeof(struct ctCTdir));
							strncpy_s(buffer, sizeof(buffer), dir.name, 16);
							buffer[16] = '\0';
							l_Rtrim(buffer);
							i2 = l_StrNthChr(dir.datasvcuse, ' ', l_ChrCnt(name, ' ') + 1);
							strncpy_s(buffer2, sizeof(buffer2), dir.datasvcuse, i2);
							buffer2[i2] = '\0';
							if (!_stricmp(name, buffer) || !_stricmp(name, buffer2))
							{
								fclose(fp1);
								l_MStoIEEE(&data.begindate, &data.begindate);
								l_MStoIEEE(&data.enddate, &data.enddate);
								memcpy(data1, &data, sizeof(struct ctCTdata));
								strcpy_s(path1, sizeof(path1), cwd);
								return (0);
							}
						}
					}
					if (stack[ptr] == 0)
						stack[ptr] = numentries + 1;
				}
				
				/* find next dir */
				if (stack[ptr] <= numentries)
				{
					fseek(fp1, stack[ptr] * sizeof(struct ctCTdir), SEEK_SET);
					for (; stack[ptr] <= numentries; stack[ptr]++)
					{
						if (fread((char *) &dir, sizeof(struct ctCTdir), 1, fp1) == 0)
						{
							fclose(fp1);
							return (ctErrFileRead);
						}
						if (!strncmp(dir.directory, "<DIR>", 5) && dir.name[0] != ' ')
						{
							strncat_s(cwd, sizeof(cwd), dir.name, 8);
							l_Rtrim(cwd);
							strcat_s(cwd, sizeof(cwd), "/");
							stack[ptr]++;
							ptr++;
							stack[ptr] = 0;
							break;
						}
					}
				}
				fclose(fp1);
			}
			/* up one dir level */
			if (stack[ptr] == numentries + 1)
			{
				if (ptr == 0)
					return (ctErrFileNotFound);
				ptr--;
				cwd[strlen(cwd) - 1] = '\0';
				p1 = strrchr(cwd, '/');
				p1[1] = '\0';
				continue;
			}
	}
	return (ctErrFileNotFound);
}


long WINAPI l_CTfindrec(char *path, char *name, unsigned short date, unsigned short time,
						unsigned short *actualdate, unsigned short *actualtime)
{
	short      err, cols[8], dfact[8], ifact[8];
	float    fields[20], dt = 0, ti = 0;
	short    year, month, day, hr, min, sec;
	long     recno = -1, guess = 0, lastguess = 0;
	long     maxent = 0, low = 0, high = 0;
	char     path1[81], buffer[81];
	FILE    *fp1;
	struct ctCTdata data;
	struct ctCTdathdr dathdr;
	
	if ((err = l_CTfindfile(path, name, &data, path1)))
		return (err);
	
	sprintf_s(buffer, sizeof(buffer), "%sF%i.DOP", path1, data.filenbr);
	if ((err = l_CTreadDOP(buffer, cols, ifact, dfact)))
		l_CTdefaultDOP(data.numfields, cols, ifact, dfact);
	
	l_JulianToYMD(date, &year, &month, &day);
	dt = (float) year *10000 + month * 100 + day;
	if (cols[1])
	{
		l_JulianToHMS(time, &hr, &min, &sec);
		ti = (float) hr *10000 + min * 100 + sec;
	}
	else
		ti = 0;
	sprintf_s(buffer, sizeof(buffer), "%sF%i.DAT", path1, data.filenbr);
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	fread((char *) &dathdr, sizeof(struct ctCTdathdr), 1, fp1);
	fields[0] = 0;
	
	maxent = (long) dathdr.lastrec - 2;
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
		if (fseek(fp1, (guess + 1) * data.reclen, SEEK_SET))
		{
			err = ctErrFileRead;
			break;
		}
		if (!fread((char *) &fields[1], data.reclen, 1, fp1))
		{
			err = ctErrFileRead;
			break;
		}
		l_MStoIEEE(&fields[cols[0]], &fields[cols[0]]);
		l_MStoIEEE(&fields[cols[1]], &fields[cols[1]]);
		if ((fields[cols[0]] == dt && fields[cols[1]] == ti) || guess == 0)
		{
			recno = guess;
			break;
		}
		if (fields[cols[0]] > dt || (fields[cols[0]] == dt && fields[cols[1]] > ti))
			high = guess;
		if (fields[cols[0]] < dt || (fields[cols[0]] == dt && fields[cols[1]] < ti))
			low = guess;
		lastguess = guess;
	}
	fclose(fp1);
	if (fields[cols[0]] < dt || (fields[cols[0]] == dt && fields[cols[1]] < ti))
		err = ctErrRecordNotFound;
	if (actualdate)
		*actualdate = l_CTdatetojulian(fields[cols[0]]);
	if (actualtime)
		*actualtime = l_CTtimetojulian(fields[cols[1]]);
	if (err)
		return (err);
	return (recno);
}

short WINAPI l_CTlist(char *path, char *outfile, short append)
{
	short      i1 = 0, err = 0, done = 0, line = 0, page = 0;
	short      stack[33], ptr = 0, numentries;
	char     buffer[81], cwd[81], *p1;
	FILE    *fp1, *out;
	struct ctCTmasthdr masthdr;
	struct ctCTdata data;
	struct ctCTdir dir;
	
	strcpy_s(cwd, sizeof(cwd), path);
	if (cwd[0] != '\0' && cwd[strlen(cwd) - 1] != ':' && cwd[strlen(cwd) - 1] != '/')
		strcat_s(cwd, sizeof(cwd), "/");
	
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
		if (eno) return (ctErrFileOpen);
	}
	
	fprintf(out, "DIR\n");
	fprintf(out, "--- NAME             DATA SERVICE USE T\n");
	fprintf(out, "    ---------------- ---------------- -\n");
	line = 3;
	stack[0] = 0;
	while (!done)
	{
		strcpy_s(buffer, sizeof(buffer), cwd);
		strcat_s(buffer, sizeof(buffer), _T("master"));
		err = 0;
		if (fopen_s(&fp1, buffer, _T("rb")))
			err = ctErrFileOpen;
		else
			if (fread((char *) &masthdr, sizeof(struct ctCTmasthdr), 1, fp1) == 0)
			{
				fclose(fp1);
				err = ctErrFileRead;
			}
			if (err)
			{
				stack[ptr] = 1;
				numentries = 0;
			}
			else
			{
				numentries = masthdr.numentries;
				if (stack[ptr] == 0)          /* i.e. first time in this master */
				{
#ifndef _WINDOWS
					line++;
					if (page && line == 25)
					{
						printf("Press any key to continue . . .");
						getchar();
						printf("\r                               \r");
						line = 1;
					}
#endif
					fprintf(out, "%s\n", cwd);
					for (i1 = 0; i1 < numentries; i1++)
					{
						if (fread((char *) &dir, sizeof(struct ctCTdir), 1, fp1) == 0)
						{
							fclose(fp1);
							stack[ptr] = 1;
							numentries = 0;
							break;
						}
						if (!strncmp(dir.directory, "<DIR>", 5) && dir.name[0] != ' ')
						{
							if (stack[ptr] == 0)
								stack[ptr] = i1 + 1;
						}
						else
						{
#ifndef _WINDOWS
							line++;
							if (page && line == 25)
							{
								printf("Press any key to continue . . .");
								getchar();
								printf("\r                               \r");
								line = 1;
							}
#endif
							memcpy(&data, &dir, sizeof(struct ctCTdir));
							fprintf(out, "    %-16.16s %-16.16s %c\n", data.name, data.datasvcuse, data.dataformat);
						}
					}
					if (stack[ptr] == 0)
						stack[ptr] = numentries + 1;
				}
				
				/* find next dir */
				if (stack[ptr] <= numentries)
				{
					fseek(fp1, stack[ptr] * sizeof(struct ctCTdir), SEEK_SET);
					for (; stack[ptr] <= numentries; stack[ptr]++)
					{
						if (fread((char *) &dir, sizeof(struct ctCTdir), 1, fp1) == 0)
						{
							fclose(fp1);
#ifndef _WINDOWS
							if (l_stricmp(outfile, "STDOUT"))
#endif
								fclose(out);
							return (ctErrFileRead);
						}
						if (!strncmp(dir.directory, "<DIR>", 5) && dir.name[0] != ' ')
						{
							strncat_s(cwd, sizeof(cwd), dir.name, 8);
							l_Rtrim(cwd);
							strcat_s(cwd, sizeof(cwd), "/");
							stack[ptr]++;
							ptr++;
							stack[ptr] = 0;
							break;
						}
					}
				}
				fclose(fp1);
			}
			/* up one dir level */
			if (stack[ptr] == numentries + 1)
			{
				if (ptr == 0)
				{
					done = 1;
					continue;
				}
				ptr--;
				cwd[strlen(cwd) - 1] = '\0';
				p1 = strrchr(cwd, '/');
				p1[1] = '\0';
				continue;
			}
 }
#ifndef _WINDOWS
 if (l_stricmp(outfile, "STDOUT"))
#endif
	 fclose(out);
 return (0);
}


short WINAPI l_CTread(ctBars *b1, char *path, char *name, long start, short cnt, short allocate)
{
	int      i1, i2, err, recno;
	short      cols[8], ifact[8], dfact[8];
	float    fields[20];
	char     path1[81], buffer[81];
	FILE    *fp1;
	struct ctCTdata data;
	struct ctCTdathdr dathdr;
	
	if ((err = l_CTfindfile(path, name, &data, path1)))
		return (err);
	sprintf_s(buffer, sizeof(buffer), "%sF%i.DOP", path1, data.filenbr);
	if ((err = l_CTreadDOP(buffer, cols, ifact, dfact)))
		l_CTdefaultDOP(data.numfields, cols, ifact, dfact);
	
	sprintf_s(buffer, sizeof(buffer), "%sF%i.DAT", path1, data.filenbr);
	if (fopen_s(&fp1, buffer, _T("rb")))
		return (ctErrFileOpen);
	fread((char *) &dathdr, sizeof(struct ctCTdathdr), 1, fp1);
	
	if (cnt < 0)
		if (start > 0)
			cnt = (short)(min(dathdr.lastrec - 1, 16000) - start);
		else
			cnt = (short)(min(dathdr.lastrec - 1, 16000));
		
		if (allocate)
		{
			if ((err = l_AllocBars(b1, cnt)))
				return (err);
		}
		
		b1->type = ctCT;
		b1->reccnt = (long) dathdr.lastrec - 1;
		strcpy_s(b1->path, sizeof(b1->path), path1);
		strcpy_s(b1->name, sizeof(b1->name), name);
		strcpy_s(b1->description, sizeof(b1->description), data.name);
		strcpy_s(b1->symbol, sizeof(b1->symbol), data.datasvcuse);
		memset(b1->cusip, 0, 12);
		l_MStoIEEE(&data.begindate, &data.begindate);
		b1->begindate = l_CTdatetojulian(data.begindate);
		l_MStoIEEE(&data.enddate, &data.enddate);
		b1->enddate = l_CTdatetojulian(data.enddate);
		switch (data.dataformat)
		{
		case 'I':
			b1->frequency = ctINTRADAY;
			break;
		case 'D':
			b1->frequency = ctIDAILY;
			break;
		case 'W':
			b1->frequency = ctWEEKLY;
			break;
		case 'M':
			b1->frequency = ctMONTHLY;
			break;
		case 'Q':
			b1->frequency = ctQUARTERLY;
			break;
		case 'Y':
			b1->frequency = ctYEARLY;
			break;
		default:
			b1->frequency = ctUNKNOWN;
			break;
		}
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
		
		if (start > dathdr.lastrec - 2)
		{
			fclose(fp1);
			return (ctErrRecordNotFound);
		}
		recno = start + 2;
		fseek(fp1, data.reclen * (start + 1), SEEK_SET);
		
		fields[0] = 0;
		i1 = 0;
		cnt = min(cnt, b1->datasize);
		while (i1 < cnt && recno <= dathdr.lastrec && fread((char *) &fields[1], data.reclen, 1, fp1) > 0)
		{
			for (i2 = 1; i2 <= data.numfields; i2++)
				l_MStoIEEE(&fields[i2], &fields[i2]);
			ctArrayItem(b1->dt, i1) = l_CTdatetojulian(fields[cols[0]]);
			ctArrayItem(b1->ti, i1) = l_CTtimetojulian(fields[cols[1]]);
			ctArrayItem(b1->op, i1) = fields[cols[2]];
			ctArrayItem(b1->hi, i1) = fields[cols[3]];
			ctArrayItem(b1->lo, i1) = fields[cols[4]];
			ctArrayItem(b1->cl, i1) = fields[cols[5]];
			ctArrayItem(b1->vol, i1) = fields[cols[6]];
			ctArrayItem(b1->oi, i1) = fields[cols[7]];
			i1++;
			recno++;
		}
		fclose(fp1);
		l_ReSizeBars(b1, i1);
		return (err);
}


short      l_CTreadDOP(char *path, short *cols, short *inputfactor, short *dispfactor)
{
	short	i1, i2, ifact, dfact;
	char	*p1, buffer[81], fname[81];
	char	*fields[8] = {"DATE", "TIME", "OPEN", "HIGH", "LOW", "CLOSE", "VOL", "OI"};
	FILE	*fp1 = (FILE *) 0;
	char*	next_token1 = NULL;
	
	if (fopen_s(&fp1, path, _T("rb")))
		return (ctErrFileOpen);
	
	for (i1 = 0; i1 < 8; i1++)
	{
		cols[i1] = 0;
		dispfactor[i1] = 0;
		inputfactor[i1] = 0;
	}
	i1 = 1;
	while (fgets(buffer, 80, fp1))
	{
		p1 = strtok_s(buffer, ",", &next_token1);
		strcpy_s(fname, sizeof(fname), p1 + 1);
		p1 = strtok_s(NULL, ",", &next_token1);
		ifact = atoi(p1);
		p1 = strtok_s(NULL, ",", &next_token1);
		dfact = atoi(p1);
		for (i2 = 0; i2 < 8; i2++)
		{
			if (!_strnicmp(fname, fields[i2], strlen(fields[i2])))
			{
				cols[i2] = i1++;
				inputfactor[i2] = ifact;
				dispfactor[i2] = dfact;
				break;
			}
		}
	}
	fclose(fp1);
	return (0);
}


short      l_CTdefaultDOP(short numfields, short *cols, short *inputfactor, short *dispfactor)
{
	short      columns[3][8] = {{1,0,0,2,3,4,5,0},{1,0,2,3,4,5,6,0},{1,0,2,3,4,5,6,7}};
	
	memcpy(cols, columns[numfields - 5], 8 * sizeof(int));
	memset(inputfactor, 0, 8 * sizeof(int));
	memset(dispfactor, 0, 8 * sizeof(int));
	return (0);
}


unsigned short l_CTtimetojulian(float time)
{
	long     hr, min, sec;
	
	hr = (long) (time / 10000);
	min = (long) ((time - hr * 10000) / 100);
	sec = (long) ((time - hr * 10000) - (min * 100));
	return (l_HMSToJulian((short) hr, (short) min, (short) sec));
}


unsigned short l_CTdatetojulian(float date)
{
	long     year, month, day;
	
	year = (long) (date / 10000);
	month = (long) ((date - year * 10000) / 100);
	day = (long) ((date - year * 10000 - month * 100));
	return (l_YMDToJulian((short) year, (short) month, (short) day));
}
