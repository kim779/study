/* Technical Analysis Programmer's Toolkit - Copyright (C) 1997 FM Labs */

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cttkit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


unsigned short WINAPI l_DateTxtDif(char *date1, char *date2, short format1, short format2)
{
	return (abs(l_DateTxtToJulian(date1, format1) -
		l_DateTxtToJulian(date2, format2)));
}

unsigned short WINAPI l_DateTxtToJulian(char *date, short format)
{
	unsigned short cent = 0, year, month, day;
	char     buffer[3] = "00";
	short      cols[12][4] = {-1, 0, 3, 6,  0, 2, 5, 8,
		-1, 6, 0, 3,  6, 8, 0, 3,
		-1, 6, 3, 0,  6, 8, 3, 0,
		-1, 0, 2, 4,  0, 2, 4, 6,
		-1, 4, 0, 2,  4, 6, 0, 2,
		-1, 4, 2, 0,  4, 6, 2, 0};
	
	while (date[0] == ' ' )
		date++;
	if(date[0] == '\"')
		date++;
	if(date[0] == '\0')
		return(0);
	if (format < 1 || format > 12)
		return(0);
	format--;
	if (cols[format][0] >= 0)
	{
		buffer[0] = date[cols[format][0]];
		buffer[1] = date[cols[format][0] + 1];
		cent = 100 * (atoi(buffer) - 19);
	}
	buffer[0] = date[cols[format][1]];
	buffer[1] = date[cols[format][1] + 1];
	year = atoi(buffer);
	buffer[0] = date[cols[format][2]];
	buffer[1] = date[cols[format][2] + 1];
	month = atoi(buffer);
	buffer[0] = date[cols[format][3]];
	buffer[1] = date[cols[format][3] + 1];
	day = atoi(buffer);
	if (!year && !month && !day)
		return (0);
	return (l_YMDToJulian(year + cent, month, day));
}

short WINAPI l_DateTxtVal(char *date, short format)
{
	unsigned short cent = 0, year, month, day;
	char     buffer[3] = "00";
	unsigned days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	short      cols[12][4] = {-1, 0, 3, 6,  0, 2, 5, 8,
		-1, 6, 0, 3,  6, 8, 0, 3,
		-1, 6, 3, 0,  6, 8, 3, 0,
		-1, 0, 2, 4,  0, 2, 4, 6,
		-1, 4, 0, 2,  4, 6, 0, 2,
		-1, 4, 2, 0,  4, 6, 2, 0};
	
	while (date[0] == ' ')
		date++;
	if(date[0] == '\"')
		date++;
	if(date[0] == '\0')
		return(0);
	if (format < 1 || format > 12)
		return(ctErrInvalidParm);
	format--;
	if (cols[format][0] >= 0)
	{
		buffer[0] = date[cols[format][0]];
		buffer[1] = date[cols[format][0] + 1];
		cent = 100 * (atoi(buffer) - 19);
	}
	buffer[0] = date[cols[format][1]];
	buffer[1] = date[cols[format][1] + 1];
	year = atoi(buffer);
	buffer[0] = date[cols[format][2]];
	buffer[1] = date[cols[format][2] + 1];
	month = atoi(buffer);
	buffer[0] = date[cols[format][3]];
	buffer[1] = date[cols[format][3] + 1];
	day = atoi(buffer);
	if (!year && !month && !day)
		return (ctErrInvalidParm);
	if (cent + year < 61 || cent + year > 245)
		return (ctErrInvalidParm);
	if (year % 4 == 0)
		days[1] = 29;
	if (month > 12)
		return (ctErrInvalidParm);
	if (day > days[month - 1])
		return (ctErrInvalidParm);
	return (0);
}

unsigned short WINAPI l_HMSDif(short hr1, short min1, short sec1, short hr2, short min2, short sec2)
{
	return (abs(l_HMSToJulian(hr1, min1, sec1) - l_HMSToJulian(hr2, min2, sec2)));
	
}

unsigned short WINAPI l_HMSToJulian(short hr, short min, short sec)
{
	return (hr * 1800 + min * 30 + sec / 2);
}

unsigned short WINAPI l_JulianDif(unsigned short dti1, unsigned short dti2)
{
	return (abs(dti1 - dti2));
}

short WINAPI l_JulianDOW(unsigned short date)
{
	return ((date - 1) % 7);
}

void WINAPI l_JulianToDateTxt(unsigned short date, short format, char *sep, char *buffer)
{
	short cent = 0, year = 0, month = 0, day = 0;
	
	if (date)
	{
		l_JulianToYMD(date, &year, &month, &day);
		cent = year / 100;
		year -= cent * 100;
		cent += 19;
	}
	switch (format)
	{
	case DTFORMAT_YY_MM_DD:
		sprintf_s(buffer, sizeof(buffer), "%.2i%c%.2i%c%.2i", year, sep[0], month, sep[0], day);
		break;
	case DTFORMAT_CCYY_MM_DD:
		sprintf_s(buffer, sizeof(buffer), "%.2i%.2i%c%.2i%c%.2i", cent, year, sep[0], month, sep[0], day);
		break;
	case DTFORMAT_MM_DD_YY:
		sprintf_s(buffer, sizeof(buffer), "%.2i%c%.2i%c%.2i", month, sep[0], day, sep[0], year);
		break;
	case DTFORMAT_MM_DD_CCYY:
		sprintf_s(buffer, sizeof(buffer), "%.2i%c%.2i%c%.2i%.2i", month, sep[0], day, sep[0], cent, year);
		break;
	case DTFORMAT_DD_MM_YY:
		sprintf_s(buffer, sizeof(buffer), "%.2i%c%.2i%c%.2i", day, sep[0], month, sep[0], year);
		break;
	case DTFORMAT_DD_MM_CCYY:
		sprintf_s(buffer, sizeof(buffer), "%.2i%c%.2i%c%.2i%.2i", day, sep[0], month, sep[0], cent, year);
		break;
	case DTFORMAT_YYMMDD:
		sprintf_s(buffer, sizeof(buffer), "%.2i%.2i%.2i", year, month, day);
		break;
	case DTFORMAT_CCYYMMDD:
		sprintf_s(buffer, sizeof(buffer), "%.2i%.2i%.2i%.2i", cent, year, month, day);
		break;
	case DTFORMAT_MMDDYY:
		sprintf_s(buffer, sizeof(buffer), "%.2i%.2i%.2i", month, day, year);
		break;
	case DTFORMAT_MMDDCCYY:
		sprintf_s(buffer, sizeof(buffer), "%.2i%.2i%.2i%.2i", month, day, cent, year);
		break;
	case DTFORMAT_DDMMYY:
		sprintf_s(buffer, sizeof(buffer), "%.2i%.2i%.2i", day, month, year);
		break;
	case DTFORMAT_DDMMCCYY:
		sprintf_s(buffer, sizeof(buffer), "%.2i%.2i%.2i%.2i", day, month, cent, year);
		break;
	default:
		buffer[0] = '\0';
		break;
	}
	return;
}

void WINAPI l_JulianToHMS(unsigned short time, short *hr, short *min, short *sec)
{
	*hr = time / 1800;
	time %= 1800;
	*min = time / 30;
	*sec = (time % 30) * 2;
}

void WINAPI l_JulianToTimeTxt(unsigned short time, short format, char *sep, char *buffer)
{
	short hour = 0, min = 0, sec = 0;
	
	if (time)
		l_JulianToHMS(time, &hour, &min, &sec);
	
	switch (format)
	{
	case TIFORMAT_HH_MM_SS:
		sprintf_s(buffer, sizeof(buffer), "%.2i%c%.2i%c%.2i", hour, sep[0], min, sep[0], sec);
		break;
	case TIFORMAT_HH_MM:
		sprintf_s(buffer, sizeof(buffer), "%.2i%c%.2i", hour, sep[0], min);
		break;
	case TIFORMAT_HHMMSS:
		sprintf_s(buffer, sizeof(buffer), "%.2i%.2i%.2i", hour, min, sec);
		break;
	case TIFORMAT_HHMM:
		sprintf_s(buffer, sizeof(buffer), "%.2i%.2i", hour, min);
		break;
	default:
		buffer[0] = '\0';
		break;
	}
	return;
}

void WINAPI l_JulianToYMD(unsigned short date, short *year, short *month, short *day)
{
	short daystable[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	short mm = 0, dd = 0, yy = 0;
	
	yy = (unsigned short)((4 * (long) date - 1) / 1461);
	dd = (unsigned short)(date - ((1461 * (long) yy) / 4));
	yy += 61;
	if (!(yy % 4))
		for (mm = 2; mm < 12; mm++)
			daystable[mm]++;
		for (mm = 11; mm >= 0 && dd <= daystable[mm]; mm--);
		dd -= daystable[mm];
		*year = yy;
		*month = mm + 1;
		*day = dd;
}

unsigned short WINAPI l_TimeTxtDif(char *time1, char *time2, short format1, short format2)
{
	return (abs(l_TimeTxtToJulian(time1, format1) -
		l_TimeTxtToJulian(time2, format2)));
}

unsigned short WINAPI l_TimeTxtToJulian(char *time, short format)
{
	unsigned short hour, min, sec = 0;
	char     buffer[3] = "00";
	short      cols[4][3] = {0, 3, 6,   0, 3,-1,   0, 2, 4,   0, 2,-1};
	
	if (format < 1 || format > 4)
		return(0);
	format--;
	buffer[0] = time[cols[format][0]];
	buffer[1] = time[cols[format][0] + 1];
	hour = atoi(buffer);
	buffer[0] = time[cols[format][1]];
	buffer[1] = time[cols[format][1] + 1];
	min = atoi(buffer);
	if (cols[format][2] >= 0)
	{
		buffer[0] = time[cols[format][2]];
		buffer[1] = time[cols[format][2] + 1];
		sec = atoi(buffer);
	}
	if (!hour && !min && !sec)
		return (0);
	return (l_HMSToJulian(hour, min, sec));
}

unsigned short WINAPI l_YMDDif(short year1, short month1, short day1,
							   short year2, short month2, short day2)
{
	return(abs(l_YMDToJulian(year1, month1, day1) - l_YMDToJulian(year2, month2, day2)));
}

unsigned short WINAPI l_YMDToJulian(short year, short month, short day)
{
	unsigned date, daystable[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	
	if (year > 1900)
		year -= 1900;
	date = day + (1461L * (year - 61) / 4) + daystable[month - 1];
	if (month > 2 && year % 4 == 0)
		date++;
	return (date);
}
